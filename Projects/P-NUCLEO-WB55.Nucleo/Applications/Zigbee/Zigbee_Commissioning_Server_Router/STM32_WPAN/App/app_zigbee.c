/**
  ******************************************************************************
  * File Name          : App/app_zigbee.c
  * Description        : Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_zigbee.h"
#include "zigbee_interface.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32wbxx_core_interface_def.h"
#include "zigbee_types.h"
#include "stm32_seq.h"

#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.commission.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define SW1_ENDPOINT                                17
#define CHANNEL                                     19
#define COMMISSIONING_DEST_ENDPOINT                 ZB_ENDPOINT_BCAST


#define HW_TS_SERVER_1MS_NB_TICKS                   (1*1000/CFG_TS_TICK_VAL) /**< 1s */

/* Commissioning specific defines --------------------------------------------*/
#define ZR_EXT_ADDR                                 (uint64_t)0x1111111111111111
#define MAX_CONFIG_TAB_SIZE                         1

/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_App_Init(void);
static void APP_ZIGBEE_SW1_Process(void);

static void APP_ZIGBEE_Commissioning_Server_Init(void);
static enum ZclStatusCodeT ZbZclCommissionServerSetStartup(struct ZbZclClusterT *clusterPtr, struct ZbStartupT *config);
static void APP_ZIGBEE_RestartDevice(void);
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_RestartDevice_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientRestartDev *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_SaveStartup_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientSaveStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_RestoreStartup_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientRestoreStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_ResetStartup_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientResetStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);


/* Private variables -----------------------------------------------*/
static TL_CmdPacket_t *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t CptReceiveNotifyFromM0 = 0;
static __IO uint32_t CptReceiveRequestFromM0 = 0;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRequestBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
uint8_t g_ot_notification_allowed = 0U;

struct zigbee_app_info {
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;
  bool init_after_join;

  struct ZbZclClusterT *commissioning_server;
};
static struct zigbee_app_info zigbee_app_info;

/* Commissioning server callbacks */
static struct ZbZclCommissionServerCallbacksT CommissionServerCallbacks = {
  .restart_device = APP_ZIGBEE_Commissioning_RestartDevice_cb,
  .save_startup = APP_ZIGBEE_Commissioning_SaveStartup_cb,
  .restore_startup = APP_ZIGBEE_Commissioning_RestoreStartup_cb,
  .reset_startup = APP_ZIGBEE_Commissioning_ResetStartup_cb,
};

static struct ZbStartupT SavedConfigTab[MAX_CONFIG_TAB_SIZE];
static struct ZbStartupT config;

static uint8_t TS_ID1;

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  Restarting the device NWK (performing a NWK join)
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_RestartDevice(void){
  APP_DBG("GREEN LED OFF.");
  APP_DBG("Restarting the Startup process ...\n.");
  BSP_LED_Off(LED_GREEN);
  ZbReset(zigbee_app_info.zb);
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
}

/**
 * @brief  Restart Device Commissioning client request callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  req: Commissioning Restart Device client command
 * @param  srcInfo: src address information
 * @param  arg: passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_RestartDevice_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientRestartDev *req, struct ZbZclAddrInfoT *srcInfo, void *arg){
  enum ZclStatusCodeT status;
  struct ZbZclCommissionServerRestartDevRsp rsp = {.status=ZCL_STATUS_SUCCESS};
  uint32_t delay = 0;
  
  UNUSED(arg);
  
  APP_DBG("RestartDevice request received."); 
  
  BSP_LED_Off(LED_BLUE);
  zigbee_app_info.join_status = ZB_ZDP_STATUS_NOT_ACTIVE;
  
  if(req->options == ZCL_COMMISS_RESTART_OPTS_MODE_USE_STARTUP){
    /* Installing current set of startup parameters */
    status = ZbZclCommissionServerGetStartup(zigbee_app_info.commissioning_server, &config);
    if(status!=ZCL_STATUS_SUCCESS){
      APP_DBG("Error, ZbZclCommissionServerGetStartup failed\n.");
      rsp.status = status;
      (void)ZbZclCommissionServerSendRestartRsp(zigbee_app_info.commissioning_server, srcInfo, &rsp);
      return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
    }
  }
  
  delay = req->delay*1000; /* restart delay in ms */
  if(delay<ZCL_COMMISSION_RESTART_DEVICE_DELAY_MS){
    APP_DBG("Warning, a delay of %d ms for restarting is too short.", delay);
    APP_DBG("Delay set to %d ms.", ZCL_COMMISSION_RESTART_DEVICE_DELAY_MS);
    delay = ZCL_COMMISSION_RESTART_DEVICE_DELAY_MS;
  }
  HW_TS_Start(TS_ID1, delay*HW_TS_SERVER_1MS_NB_TICKS);
  
  APP_DBG("Restarting the device in %d ms.", delay);
  APP_DBG("GREEN LED ON.");
  BSP_LED_On(LED_GREEN);
  
  (void) ZbZclCommissionServerSendRestartRsp(zigbee_app_info.commissioning_server, srcInfo, &rsp);
  return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
}

/**
 * @brief  Save Startup Parameters Commissioning client request callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  req: Commissioning Save Startup Parameters client command
 * @param  srcInfo: src address information
 * @param  arg: passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_SaveStartup_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientSaveStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg){
  enum ZclStatusCodeT status;
  struct ZbZclCommissionServerSaveStartupRsp rsp = {.status=ZCL_STATUS_SUCCESS};
  struct ZbStartupT currentConfig;
  uint8_t index = req->index;
  
  UNUSED(arg);
    
  APP_DBG("SaveStartup request received.");
  
  /* Check boundary */
  if(index>=MAX_CONFIG_TAB_SIZE){
    APP_DBG("Error, index out of bound.\n");
    rsp.status = ZCL_STATUS_FAILURE; 
  }
  else {
      /* Get the current network configuration from Commissioning server attributes */
      status = ZbZclCommissionServerGetStartup(zigbee_app_info.commissioning_server, &currentConfig);
      if(status!=ZCL_STATUS_SUCCESS){
        APP_DBG("Error, ZbZclCommissionServerGetStartup failed.\n");
        rsp.status = status;
      } else {
        APP_DBG("Configuration saved with index %d.\n", req->index);

        /* Save the current network configuration */
        memset(&SavedConfigTab[index], 0, sizeof(struct ZbStartupT));
        SavedConfigTab[index] = currentConfig;
      }
  }
  
  (void) ZbZclCommissionServerSendSaveStartupRsp(zigbee_app_info.commissioning_server, srcInfo, &rsp);
  return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
}

/**
 * @brief  Restore Startup Parameters Commissioning client request callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  req: Commissioning Restore Startup Parameters client command
 * @param  srcInfo: src address information
 * @param  arg: passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_RestoreStartup_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientRestoreStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg){
  enum ZclStatusCodeT status = ZCL_STATUS_SUCCESS;
  struct ZbZclCommissionServerRestoreStartupRsp rsp = {.status=ZCL_STATUS_SUCCESS};
  uint8_t index = req->index;
  
  UNUSED(arg);
  
  APP_DBG("RestoreStartup request received."); 
  
  if(index>=MAX_CONFIG_TAB_SIZE){
    APP_DBG("Error, index out of bound.\n");
    rsp.status = ZCL_STATUS_FAILURE; 
  }
  else {
      /* Set the Commissioning server attributes to the values present at the requested index */
      status = ZbZclCommissionServerSetStartup(zigbee_app_info.commissioning_server, &SavedConfigTab[index]);
      if(status!=ZCL_STATUS_SUCCESS){
        APP_DBG("Error, ZbZclCommissionServerSetStartup failed.\n");
        rsp.status = status;
      } else {
        APP_DBG("Startup parameters restored with index %d.\n", index);
      }
  }
  
  (void) ZbZclCommissionServerSendRestoreStartupRsp(zigbee_app_info.commissioning_server, srcInfo, &rsp);
  return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
}

/**
 * @brief  Reset Startup Parameters Commissioning client request callback
 * @param  clusterPtr: ZCL cluster pointer
 * @param  req: Commissioning Reset Startup Parameters client command
 * @param  srcInfo: src address information
 * @param  arg: passed argument
 * @retval ZCL status code
 */
static enum ZclStatusCodeT APP_ZIGBEE_Commissioning_ResetStartup_cb(struct ZbZclClusterT *clusterPtr, struct ZbZclCommissionClientResetStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg){
  enum ZclStatusCodeT status = ZCL_STATUS_SUCCESS;
  struct ZbZclCommissionServerResetStartupRsp rsp = {.status=ZCL_STATUS_SUCCESS};
  struct ZbStartupT currentConfig;
  
  UNUSED(arg);
  
  APP_DBG("ResetStartup request received."); 
  
  /* reset the current network configuration */
  memset(&currentConfig, 0, sizeof(currentConfig));
  currentConfig.stackProfile = ZB_NWK_STACK_PROFILE_PRO;
  status = ZbZclCommissionServerSetStartup(zigbee_app_info.commissioning_server, &currentConfig);
  if(status !=ZCL_STATUS_SUCCESS){
    APP_DBG("Error, ZbZclCommissionServerSetStartup failed.");
    rsp.status = status;
  } else {
    APP_DBG("Startup parameters reset with default values.\n");
  }
  
  (void) ZbZclCommissionServerSendResetStartupRsp(zigbee_app_info.commissioning_server, srcInfo, &rsp);
  return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE;
}

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_App_Init(void){
  /* Task associated with push button SW1 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  
  /* Timer associated with the DeviceRestart command handling */
  HW_TS_Create(1U << CFG_TIM_ZIGBEE_RESTART_DEVICE_DELAY, &TS_ID1, hw_ts_SingleShot, APP_ZIGBEE_RestartDevice);
 
  APP_ZIGBEE_Commissioning_Server_Init();
} /* APP_ZIGBEE_App_Init */

/**
 * @brief  Power Profile server initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_Commissioning_Server_Init(void){
  enum ZclStatusCodeT status;
  struct ZbZclCommissionServerEnableInfoT info;
  
  /* Enabling Commissioning client */
  info.page = 0;
  info.channel = 19;
  status = ZbZclCommissionServerEnable(zigbee_app_info.commissioning_server, TRUE,&info);
  if (status != ZCL_STATUS_SUCCESS){
    APP_DBG("ZbZclCommissionServerEnable failed."); 
    assert(0);
  }
  APP_DBG("Commissioning server enabled."); 
  
  /* Init nwk configuration with NULL values */
  memset(&config, 0, sizeof(config));
  config.stackProfile = ZB_NWK_STACK_PROFILE_PRO;
  
  status = ZbZclCommissionServerSetStartup(zigbee_app_info.commissioning_server, &config);
  if (status != ZCL_STATUS_SUCCESS){
    APP_DBG("ZbZclCommissionServerSetStartup failed."); 
    assert(0);
  }
  
  /* Change Extended Address */
  ZbChangeExtAddr(zigbee_app_info.zb, ZR_EXT_ADDR);
  
  APP_DBG("Commissioning server init done!\n");  
} /* APP_ZIGBEE_Commissioning_Server_Init */

static enum ZclStatusCodeT ZbZclCommissionServerSetStartup(struct ZbZclClusterT *clusterPtr, struct ZbStartupT *config){
    enum ZclStatusCodeT status;
    uint8_t scan_count = 0;

    /* ZCL_COMMISSION_SVR_ATTR_SHORT_ADDR */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_SHORT_ADDR, (uint16_t)config->shortAddress);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_EPID */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_EPID, (uint64_t)config->extendedPanId);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_PANID */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_PANID, (uint16_t)config->panId);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_CHANNELMASK */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_CHANNELMASK, (config->channelList.list[0].channelMask & WPAN_PAGE_CHANNELMASK_ALL));
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_STACKPROFILE */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_STACKPROFILE, (uint8_t)config->stackProfile);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_STARTUPCONTROL */
    switch (config->startupControl) {
        case ZbStartTypePreconfigured:
        case ZbStartTypeForm:
        case ZbStartTypeRejoin:
        case ZbStartTypeJoin:
            break;

        default:
            return ZCL_STATUS_FAILURE;
    }
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_STARTUPCONTROL, (enum ZbStartType)config->startupControl);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_TCADDR */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_TCADDR, (uint64_t)config->security.trustCenterAddress);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_NWKKEY */
    status = ZbZclAttrWrite(clusterPtr, NULL, ZCL_COMMISSION_SVR_ATTR_NWKKEY, (const uint8_t*)&(config->security.networkKey), ZB_SEC_KEYSIZE, ZCL_ATTR_WRITE_FLAG_NORMAL);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_USEINSECJOIN */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_USEINSECJOIN, (bool)config->security.useInsecureRejoin);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_PRECONFLINKKEY */
    status = ZbZclAttrWrite(clusterPtr, NULL, ZCL_COMMISSION_SVR_ATTR_PRECONFLINKKEY, (const uint8_t*)&(config->security.preconfiguredLinkKey), ZB_SEC_KEYSIZE, ZCL_ATTR_WRITE_FLAG_NORMAL);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_NWKKEYSEQNUM */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_NWKKEYSEQNUM, (uint8_t)config->security.networkKeySeqNum);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_NWKKEYTYPE */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_NWKKEYTYPE, (enum ZbSecKeyTypeT)config->security.networkKeyType);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_NWKMGRADDR */
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_NWKMGRADDR, (uint16_t)config->networkManagerAddress);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    /* ZCL_COMMISSION_SVR_ATTR_SCANATTEMPTS */
    ZbApsGet(clusterPtr->zb, ZB_APS_IB_ID_SCAN_COUNT, &scan_count, sizeof(scan_count));
    status = ZbZclAttrIntegerWrite(clusterPtr, ZCL_COMMISSION_SVR_ATTR_SCANATTEMPTS, (uint8_t)scan_count);
    if (status != ZCL_STATUS_SUCCESS) {
        return ZCL_STATUS_FAILURE;
    }

    return ZCL_STATUS_SUCCESS;
}

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_Init(void)
{
  SHCI_CmdStatus_t ZigbeeInitStatus;

  APP_DBG("APP_ZIGBEE_Init");

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_ZIGBEE_CheckWirelessFirmwareInfo();

  /* Register cmdbuffer */
  APP_ZIGBEE_RegisterCmdBuffer(&ZigbeeOtCmdBuffer);

  /* Init config buffer and call TL_ZIGBEE_Init */
  APP_ZIGBEE_TL_INIT();

  /* Register task */
  /* Create the different tasks */
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessNotifyM0ToM4);
  UTIL_SEQ_RegTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_ZIGBEE_ProcessRequestM0ToM4);

  /* Task associated with network creation process */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, UTIL_SEQ_RFU, APP_ZIGBEE_NwkForm);

  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();
  
  /* Task associated with application init */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_START, UTIL_SEQ_RFU, APP_ZIGBEE_App_Init);

} /* APP_ZIGBEE_Init */

/**
 * @brief  Initialize Zigbee stack layers
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_StackLayersInit(void)
{
  APP_DBG("APP_ZIGBEE_StackLayersInit");

  zigbee_app_info.zb = ZbInit(0U, NULL, NULL);
  assert(zigbee_app_info.zb != NULL);

  /* Create the endpoint and cluster(s) */
  APP_ZIGBEE_ConfigEndpoints();

  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);

  /* Configure the joining parameters */
  zigbee_app_info.join_status = (enum ZbStatusCodeT) 0x01; /* init to error status */
  zigbee_app_info.join_delay = HAL_GetTick(); /* now */
  zigbee_app_info.startupControl = ZbStartTypeJoin;

  /* Initialization Complete */
  zigbee_app_info.has_init = true;
  
  /* Not starting nwk now but init Commissioning server */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_START, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_StackLayersInit */

/**
 * @brief  Configure Zigbee application endpoints
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigEndpoints(void)
{
  /* Commissioning Server */
  zigbee_app_info.commissioning_server = ZbZclCommissionServerAlloc(zigbee_app_info.zb, COMMISSIONING_DEST_ENDPOINT, ZCL_PROFILE_HOME_AUTOMATION,false, &CommissionServerCallbacks, NULL);
  assert(zigbee_app_info.commissioning_server != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.commissioning_server); 
  
} /* APP_ZIGBEE_ConfigEndpoints */

/**
 * @brief  Handle Zigbee network forming and joining
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkForm(void)
{
  if ((zigbee_app_info.join_status != ZB_STATUS_SUCCESS) && (HAL_GetTick() >= zigbee_app_info.join_delay))
  {
    enum ZbStatusCodeT status;

    /* Configure Zigbee Logging (only need to do this once, but this is a good place to put it) */
    ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_5, NULL);

    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_ROUTER");
    
    /* Setting up additional startup configuration parameters (not Commissioning attributes) */
    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.security.level = 0x05;
    config.bdbCommissioningMode = 0;
    memcpy(config.security.distributedGlobalKey, sec_key_distrib_uncert, ZB_SEC_KEYSIZE);
    config.capability = (MCP_ASSOC_CAP_PWR_SRC 
                         | MCP_ASSOC_CAP_RXONIDLE 
                         | MCP_ASSOC_CAP_ALLOC_ADDR 
                         | MCP_ASSOC_CAP_DEV_TYPE);
    
    /* As Extended PAN ID is provided, we will try to connect only to the NWK with this extended PAN ID */

    /* Using ZbStartupWait (blocking) here instead of ZbStartup, in order to demonstrate how to do
     * a blocking call on the M4. */
    status = ZbStartupWait(zigbee_app_info.zb, &config);

    APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
    zigbee_app_info.join_status = status;

    if (status == ZB_STATUS_SUCCESS) {
      zigbee_app_info.join_delay = 0U;
      zigbee_app_info.init_after_join = true;
      APP_DBG("BLUE LED ON.");
      BSP_LED_On(LED_BLUE);
    }
    else
    {
      APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
      zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
    }
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
} /* APP_ZIGBEE_NwkForm */

/*************************************************************
 * ZbStartupWait Blocking Call
 *************************************************************/
struct ZbStartupWaitInfo {
  bool active;
  enum ZbStatusCodeT status;
};

static void ZbStartupWaitCb(enum ZbStatusCodeT status, void *cb_arg)
{
  struct ZbStartupWaitInfo *info = cb_arg;

  info->status = status;
  info->active = false;
  UTIL_SEQ_SetEvt(EVENT_ZIGBEE_STARTUP_ENDED);
} /* ZbStartupWaitCb */

enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config)
{
  struct ZbStartupWaitInfo *info;
  enum ZbStatusCodeT status;

  info = malloc(sizeof(struct ZbStartupWaitInfo));
  if (info == NULL) {
    return ZB_STATUS_ALLOC_FAIL;
  }
  memset(info, 0, sizeof(struct ZbStartupWaitInfo));

  info->active = true;
  status = ZbStartup(zb, config, ZbStartupWaitCb, info);
  if (status != ZB_STATUS_SUCCESS)
  {
    free(info);
    return status;
  }
  UTIL_SEQ_WaitEvt(EVENT_ZIGBEE_STARTUP_ENDED);
  status = info->status;
  free(info);
  return status;
} /* ZbStartupWait */

/**
 * @brief  Trace the error or the warning reported.
 * @param  ErrId :
 * @param  ErrCode
 * @retval None
 */
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode)
{
  switch (ErrId) {
  default:
    APP_ZIGBEE_TraceError("ERROR Unknown ", 0);
    break;
  }
} /* APP_ZIGBEE_Error */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * @brief  Warn the user that an error has occurred.In this case,
 *         the LEDs on the Board will start blinking.
 *
 * @param  pMess  : Message associated to the error.
 * @param  ErrCode: Error code associated to the module (Zigbee or other module if any)
 * @retval None
 */
static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode)
{
  APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
  while (1U == 1U) {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500U);
  }
} /* APP_ZIGBEE_TraceError */

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Zigbee
 *        and display associated information
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t wireless_info_instance;
  WirelessFwInfo_t *p_wireless_info = &wireless_info_instance;

  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success) {
    APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
  }
  else {
    APP_DBG("**********************************************************");
    APP_DBG("WIRELESS COPROCESSOR FW:");
    /* Print version */
    APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

    switch (p_wireless_info->StackType) {
    case INFO_STACK_TYPE_ZIGBEE_FFD:
      APP_DBG("FW Type : FFD Zigbee stack");
      break;
   case INFO_STACK_TYPE_ZIGBEE_RFD:
      APP_DBG("FW Type : RFD Zigbee stack");
      break;
    default:
      /* No Zigbee device supported ! */
      APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
      break;
    }
    // print the application name
    char* __PathProject__ =(strstr(__FILE__, "Zigbee") ? strstr(__FILE__, "Zigbee") + 7 : __FILE__);
    char *del;
    if ( (strchr(__FILE__, '/')) == NULL)
        {del = strchr(__PathProject__, '\\');}
    else 
        {del = strchr(__PathProject__, '/');}
    
        int index = (int) (del - __PathProject__);
        APP_DBG("Application flashed: %*.*s",index,index,__PathProject__);
    
    //print channel
    APP_DBG("Channel used: %d", CHANNEL);
    //print Link Key
    APP_DBG("Link Key: %.16s", sec_key_ha);
    //print Link Key value hex   
    char Z09_LL_string[ZB_SEC_KEYSIZE*3+1];
    Z09_LL_string[0]=0;
    for(int str_index=0; str_index < ZB_SEC_KEYSIZE; str_index++)
      {           
        sprintf(&Z09_LL_string[str_index*3],"%02x ",sec_key_ha[str_index]);
      }
  
    APP_DBG("Link Key value: %s",Z09_LL_string);
    //print clusters allocated
    APP_DBG("Clusters allocated are:");  
    APP_DBG("Commissioning Server on Endpoint %d",COMMISSIONING_DEST_ENDPOINT);
    APP_DBG("**********************************************************");
  }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

/**
 * @brief  Button 1 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW1_Process(void)
{
  uint16_t pid = 0, nwk_addr = 0;
  uint64_t epid = 0;
  
  APP_DBG("Device current network parameters:");
  
  /* NWK address */
  if(ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkAddress, &nwk_addr, sizeof(nwk_addr)) != ZB_STATUS_SUCCESS){
    APP_DBG("Error, ZbNwkGet failed.\n");
    return; 
  }
  APP_DBG("NWK address: 0x%04x", nwk_addr);
  
  /* PAN ID */
  if(ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_PanId, &pid, sizeof(pid)) != ZB_STATUS_SUCCESS){
    APP_DBG("Error, ZbNwkGet failed.\n");
    return; 
  }
  APP_DBG("PAN ID: 0x%04x", pid);
  
  /* Extended PAN ID */
  if(ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS){
    APP_DBG("Error, ZbNwkGet failed.\n");
    return; 
  }
  APP_DBG("Extended PAN ID: 0x%" PRIx64 "\n.", epid);
} /* APP_ZIGBEE_SW1_Process */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer)
{
  p_ZIGBEE_otcmdbuffer = p_buffer;
} /* APP_ZIGBEE_RegisterCmdBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;
} /* ZIGBEE_Get_OTCmdPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdRspPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)((TL_EvtPacket_t *)p_ZIGBEE_otcmdbuffer)->evtserial.evt.payload;
} /* ZIGBEE_Get_OTCmdRspPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_NotificationPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_notif_M0_to_M4)->evtserial.evt.payload;
} /* ZIGBEE_Get_NotificationPayloadBuffer */

Zigbee_Cmd_Request_t * ZIGBEE_Get_M0RequestPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_request_M0_to_M4)->evtserial.evt.payload;
}

/**
 * @brief  This function is used to transfer the commands from the M4 to the M0.
 *
 * @param   None
 * @return  None
 */
void ZIGBEE_CmdTransfer(void)
{
  Zigbee_Cmd_Request_t *cmd_req = (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;

  /* Zigbee OT command cmdcode range 0x280 .. 0x3DF = 352 */
  p_ZIGBEE_otcmdbuffer->cmdserial.cmd.cmdcode = 0x280U;
  /* Size = otCmdBuffer->Size (Number of OT cmd arguments : 1 arg = 32bits so multiply by 4 to get size in bytes)
   * + ID (4 bytes) + Size (4 bytes) */
  p_ZIGBEE_otcmdbuffer->cmdserial.cmd.plen = 8U + (cmd_req->Size * 4U);

  TL_ZIGBEE_SendM4RequestToM0();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_M0();
} /* ZIGBEE_CmdTransfer */

/**
 * @brief  This function is used to transfer the commands from the M4 to the M0 with notification
 *
 * @param   None
 * @return  None
 */
void ZIGBEE_CmdTransferWithNotif(void)
{
	g_ot_notification_allowed = 1;
	ZIGBEE_CmdTransfer();
}

/**
 * @brief  This function is called when the M0+ acknowledge  the fact that it has received a Cmd
 *
 *
 * @param   Otbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_CmdEvtReceived(TL_EvtPacket_t *Otbuffer)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Otbuffer);

  Receive_Ack_From_M0();
} /* TL_ZIGBEE_CmdEvtReceived */

/**
 * @brief  This function is called when notification from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_NotReceived(TL_EvtPacket_t *Notbuffer)
{
  p_ZIGBEE_notif_M0_to_M4 = Notbuffer;

  Receive_Notification_From_M0();
} /* TL_ZIGBEE_NotReceived */

/**
 * @brief  This function is called before sending any ot command to the M0
 *         core. The purpose of this function is to be able to check if
 *         there are no notifications coming from the M0 core which are
 *         pending before sending a new ot command.
 * @param  None
 * @retval None
 */
void Pre_ZigbeeCmdProcessing(void)
{
  UTIL_SEQ_WaitEvt(EVENT_SYNCHRO_BYPASS_IDLE);
} /* Pre_ZigbeeCmdProcessing */

/**
 * @brief  This function waits for getting an acknowledgment from the M0.
 *
 * @param  None
 * @retval None
 */
static void Wait_Getting_Ack_From_M0(void)
{
  UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_M0_EVT);
} /* Wait_Getting_Ack_From_M0 */

/**
 * @brief  Receive an acknowledgment from the M0+ core.
 *         Each command send by the M4 to the M0 are acknowledged.
 *         This function is called under interrupt.
 * @param  None
 * @retval None
 */
static void Receive_Ack_From_M0(void)
{
  UTIL_SEQ_SetEvt(EVENT_ACK_FROM_M0_EVT);
} /* Receive_Ack_From_M0 */

/**
 * @brief  Receive a notification from the M0+ through the IPCC.
 *         This function is called under interrupt.
 * @param  None
 * @retval None
 */
static void Receive_Notification_From_M0(void)
{
    CptReceiveNotifyFromM0++;
    UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_NOTIFY_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief  This function is called when a request from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_ZIGBEE_M0RequestReceived(TL_EvtPacket_t *Reqbuffer)
{
    p_ZIGBEE_request_M0_to_M4 = Reqbuffer;

    CptReceiveRequestFromM0++;
    UTIL_SEQ_SetTask(1U << (uint32_t)CFG_TASK_REQUEST_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
}

/**
 * @brief Perform initialization of TL for Zigbee.
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_TL_INIT(void)
{
    ZigbeeConfigBuffer.p_ZigbeeOtCmdRspBuffer = (uint8_t *)&ZigbeeOtCmdBuffer;
    ZigbeeConfigBuffer.p_ZigbeeNotAckBuffer = (uint8_t *)ZigbeeNotifRspEvtBuffer;
    ZigbeeConfigBuffer.p_ZigbeeNotifRequestBuffer = (uint8_t *)ZigbeeNotifRequestBuffer;
    TL_ZIGBEE_Init(&ZigbeeConfigBuffer);
}

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void)
{
  if (CptReceiveNotifyFromM0 != 0)
  {
    /* Reset counter */
    CptReceiveNotifyFromM0 = 0;
    Zigbee_CallBackProcessing();
  }
}

/**
 * @brief Process the requests coming from the M0.
 * @param None
 * @return None
 */
static void APP_ZIGBEE_ProcessRequestM0ToM4(void)
{
    if (CptReceiveRequestFromM0 != 0)
    {
        CptReceiveRequestFromM0 = 0;
        Zigbee_M0RequestProcessing();
    }
}

