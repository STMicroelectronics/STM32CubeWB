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
#include "zcl/general/zcl.alarm.h"
#include "zcl/general/zcl.doorlock.h"

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define SW1_ENDPOINT                                17
#define CHANNEL                                     19

/* DoorLock specific defines -------------------------------------------------*/
#define USER_ID                                     0x1
#define USER_STATUS                                 ZCL_DRLK_USER_STATUS_OCC_ENABLED
#define USER_TYPE                                   ZCL_DRLK_USER_TYPE_UNRESTRICTED
#define PIN_CODE                                    "PASSWD"

/* external definition */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_SW2_Process(void);
static void APP_ZIGBEE_SW3_Process(void);
static void APP_ZIGBEE_NwkForm(void);
static void APP_ZIGBEE_App_Init(void);

static void APP_ZIGBEE_ALARM_CLIENT_cb(void *arg, uint16_t nwk_addr, uint8_t endpoint, uint8_t alarm_code, uint16_t cluster_id);

static void APP_ZIGBEE_DoorLock_Client_Init(void);
static void APP_ZIGBEE_DoorLock_Client_Send_Lock(void);
static void APP_ZIGBEE_DoorLock_Client_Send_Unlock(void);
static void APP_ZIGBEE_DoorLock_Client_Send_UnlockWithWrongPIN(void);
static void APP_ZIGBEE_Generic_Cmd_Rsp_cb(struct ZbZclCommandRspT *zcl_rsp, void *arg);
static void APP_ZIGBEE_DoorLock_Cmd_Rsp_Mgmt(struct ZbZclCommandRspT *zcl_rsp);

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
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;

  struct ZbZclClusterT *doorlock_client;
  struct ZbZclClusterT *alarm_client;
};
static struct zigbee_app_info zigbee_app_info;

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  Alarm client callback
 * @param  nwk_addr: Alarm server ntw address
 * @param  endpoint: Alarm server endpoint
 * @param  alarm_code: Alarm code (cluster specific)
 * @param  cluster_id: Cluster ID (related to Alarm code)
 * @param  arg: passed arguments
 * @retval None
 */
static void APP_ZIGBEE_ALARM_CLIENT_cb(void *arg, uint16_t nwk_addr, uint8_t endpoint, uint8_t alarm_code, uint16_t cluster_id){
  UNUSED(arg);
  
  switch(cluster_id){
    case ZCL_CLUSTER_DOOR_LOCK:
      APP_DBG("[ALARM] Alarm received from Door Lock cluster (code %d) from addr 0x%04x.", alarm_code, nwk_addr);
      
      /* RED led on during 500ms */
      BSP_LED_On(LED_RED);
      APP_DBG("LED_RED ON\n");
      HAL_Delay(500);
      BSP_LED_Off(LED_RED);
      APP_DBG("LED_RED OFF\n");
      break;
      
    default:
      APP_DBG("[ALARM] Error, alarm from an unexpected cluster received.");
      break;
  }
}

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_App_Init(void){
  /* Initialize Zigbee Meter Identification Client parameters */
  APP_ZIGBEE_DoorLock_Client_Init();
} /* APP_ZIGBEE_App_Init */

/**
 * @brief  Door Lock client initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Client_Init(void){
  struct ZbApsAddrT dst;
  struct ZbZclDoorLockSetPinReqT DoorLockSetPinReq;
  enum ZclStatusCodeT status;
  
  APP_DBG("[DOORLOCK] Requesting PIN code setting for the user.");
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Creating a request for new user and its associated pin */ 
  memset(&DoorLockSetPinReq, 0, sizeof(DoorLockSetPinReq));
  DoorLockSetPinReq.user_id = USER_ID;
  DoorLockSetPinReq.user_status = USER_STATUS;
  DoorLockSetPinReq.user_type = USER_TYPE;
  DoorLockSetPinReq.pin_len = sizeof(PIN_CODE);
  memset(DoorLockSetPinReq.pin, 0, ZCL_DRLK_MAX_PIN_LEN);
  memcpy(DoorLockSetPinReq.pin, PIN_CODE, DoorLockSetPinReq.pin_len);
  
  /* Send the request */ 
  status = ZbZclDoorLockClientSetPinReq(zigbee_app_info.doorlock_client, &dst, &DoorLockSetPinReq, &APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("[DOORLOCK] Error, ZbZclDoorLockClientSetPinReq failed");
    return;
  }
} /* APP_ZIGBEE_DoorLock_Client_Init */

/**
 * @brief  DoorLock client lock command
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Client_Send_Lock(void){
  struct ZbApsAddrT dst;
  struct ZbZclDoorLockLockDoorReqT ZbZclDoorLockLockDoorReq;
  enum ZclStatusCodeT status;
  
  APP_DBG("[DOORLOCK] Requesting a Lock.");
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Creating a request for the unlock cmd */ 
  memset(&ZbZclDoorLockLockDoorReq, 0, sizeof(ZbZclDoorLockLockDoorReq));
  ZbZclDoorLockLockDoorReq.pin_len = sizeof(PIN_CODE);
  memset(ZbZclDoorLockLockDoorReq.pin, 0, ZCL_DRLK_MAX_PIN_LEN);
  memcpy(ZbZclDoorLockLockDoorReq.pin, PIN_CODE, ZbZclDoorLockLockDoorReq.pin_len);
  
  /* Send the request */ 
  status = ZbZclDoorLockClientLockReq(zigbee_app_info.doorlock_client, &dst, &ZbZclDoorLockLockDoorReq, &APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("[DOORLOCK] Error, ZbZclDoorLockClientLockReq failed.");
    return;
  }
} /* APP_ZIGBEE_DoorLock_Client_Send_Lock */

/**
 * @brief  DoorLock client unlock command
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Client_Send_Unlock(void){
  struct ZbApsAddrT dst;
  struct ZbZclDoorLockUnlockDoorReqT DoorLockUnlockDoorReq;
  enum ZclStatusCodeT status;
  
  APP_DBG("[DOORLOCK] Requesting an Unlock.");
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  /* Creating a request for the unlock cmd */ 
  memset(&DoorLockUnlockDoorReq, 0, sizeof(DoorLockUnlockDoorReq));
  DoorLockUnlockDoorReq.pin_len = sizeof(PIN_CODE);
  memset(DoorLockUnlockDoorReq.pin, 0, ZCL_DRLK_MAX_PIN_LEN);
  memcpy(DoorLockUnlockDoorReq.pin, PIN_CODE, DoorLockUnlockDoorReq.pin_len);
  
  /* Send the request */ 
  status = ZbZclDoorLockClientUnlockReq(zigbee_app_info.doorlock_client, &dst, &DoorLockUnlockDoorReq, &APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("[DOORLOCK] Error, ZbZclDoorLockClientUnlockReq failed");
    return;
  }
} /* APP_ZIGBEE_DoorLock_Client_Send_Unlock */

static void APP_ZIGBEE_DoorLock_Client_Send_UnlockWithWrongPIN(void){
  struct ZbApsAddrT dst;
  struct ZbZclDoorLockUnlockDoorReqT DoorLockUnlockDoorReq;
  enum ZclStatusCodeT status;
  
  APP_DBG("[DOORLOCK] Requesting an Unlock with a wrong PIN.");
  
  /* Setting up the addressing mode */ 
  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = 0x0;
  
  
  /* Creating a request for the unlock cmd  with a wrong pin (set to 0) */ 
  DoorLockUnlockDoorReq.pin_len = 1;
  memset(DoorLockUnlockDoorReq.pin, 0, ZCL_DRLK_MAX_PIN_LEN);
  
  /* Send the request */ 
  status = ZbZclDoorLockClientUnlockReq(zigbee_app_info.doorlock_client, &dst, &DoorLockUnlockDoorReq, &APP_ZIGBEE_Generic_Cmd_Rsp_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) {
    APP_DBG("[DOORLOCK] Error, ZbZclDoorLockClientUnlockReq failed");
    return;
  }
} /* APP_ZIGBEE_DoorLock_Client_Send_UnlockWithWrongPIN */

/**
 * @brief  Device generic command response callback
 * @param  zcl_rsp: ZCL response
 * @param  arg: passed arguments
 * @retval None
 */
static void APP_ZIGBEE_Generic_Cmd_Rsp_cb(struct ZbZclCommandRspT *zcl_rsp, void *arg){
  UNUSED(arg);
  
  /* Checking response consistency */
  assert(zcl_rsp != NULL);
  
  switch(zcl_rsp->hdr.frameCtrl.frameType){
    case ZCL_FRAMETYPE_PROFILE:
      /* general frame cmd response */   
      
      switch(zcl_rsp->hdr.cmdId){
        case ZCL_COMMAND_DEFAULT_RESPONSE:
          /* general default response */
          if (zcl_rsp->status != ZCL_STATUS_SUCCESS) {
            APP_DBG("Default response: the server returned an error: 0x%02x.\n", zcl_rsp->status);
          } else {
            APP_DBG("Default response: success!\n");
          }
         break;
         
         default:
          APP_DBG("Error: unsupported general cmd type.\n");
          break;
      }
      break;
      
    case ZCL_FRAMETYPE_CLUSTER:
      /* cluster specific frame cmd response */
      
      if(zcl_rsp->status != ZCL_STATUS_SUCCESS){
        APP_DBG("Error: cmd not successfully received. 0x%02x.\n", zcl_rsp->status);
        return;
      }
      
      switch(zcl_rsp->clusterId){
        case ZCL_CLUSTER_DOOR_LOCK:
          /* DoorLock cluster */
             APP_ZIGBEE_DoorLock_Cmd_Rsp_Mgmt(zcl_rsp);
             break;
         
         default:
          APP_DBG("Error: unsupported cluster.\n");
          break;
      }
      break;
      
    default:
      APP_DBG("Error: unsupported frame type.\n");
      break;
  }
} /* APP_ZIGBEE_Generic_Cmd_Rsp_cb */

/**
 * @brief  DoorLock client command response management
 * @param  zcl_rsp: ZCL response
 * @retval None
 */
static void APP_ZIGBEE_DoorLock_Cmd_Rsp_Mgmt(struct ZbZclCommandRspT *zcl_rsp){
  uint8_t rsp_status = 0;
  
  /* Checking response consistency */
  assert(zcl_rsp != NULL && zcl_rsp->payload != NULL);
  
  memcpy(&rsp_status, zcl_rsp->payload, sizeof(rsp_status));
  switch(zcl_rsp->hdr.cmdId){
    case ZCL_DRLK_SVR_LOCK_RSP:
      /* DoorLock lock cmd response */
      if (rsp_status != ZCL_DRLK_STATUS_SUCCESS) {
        APP_DBG("[DOORLOCK] Lock response: the server returned an error: 0x%02x.\n", rsp_status);
      } else {
        APP_DBG("[DOORLOCK] Lock response: success!\n");
      }
      break;
      
    case ZCL_DRLK_SVR_UNLOCK_RSP:
      /* DoorLock unlock cmd response */
      if (rsp_status != ZCL_DRLK_STATUS_SUCCESS) {
        APP_DBG("[DOORLOCK] Unlock response: the server returned an error: 0x%02x.\n", rsp_status);
      } else {
        APP_DBG("[DOORLOCK] Unlock response: success!\n");
      }
      break;
      
    case ZCL_DRLK_SVR_SETPIN_RSP:
      /* DoorLock set_pin cmd response */
      if (rsp_status != ZCL_DRLK_STATUS_SUCCESS) {
        APP_DBG("[DOORLOCK] set_pin response: the server returned an error: 0x%02x.\n", rsp_status);
      } else {
        APP_DBG("[DOORLOCK] set_pin response: success!\n");
      }
      break;
             
    default:
      APP_DBG("Error: unsupported general cmd type.\n");
      break;
    }
} /* APP_ZIGBEE_DoorLock_Cmd_Rsp_Mgmt */

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
  
  /* Task associated with push button SW1, 2 and 3 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_SW2_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW3, UTIL_SEQ_RFU, APP_ZIGBEE_SW3_Process);
  
  /* Task associated with application init */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_START, UTIL_SEQ_RFU, APP_ZIGBEE_App_Init);
  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();
  

  

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

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
} /* APP_ZIGBEE_StackLayersInit */

/**
 * @brief  Configure Zigbee application endpoints
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigEndpoints(void)
{
  struct ZbApsmeAddEndpointReqT req;
  struct ZbApsmeAddEndpointConfT conf;

  memset(&req, 0, sizeof(req));
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_DATA_COLLECTION_UNIT;

  /* Endpoint: SW1_ENDPOINT */
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);
  
  /* DoorLock and Alarm cluster are mapped in the same endpoint */
  
  /* Alarm Client */
  zigbee_app_info.alarm_client = ZbZclAlarmClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT, APP_ZIGBEE_ALARM_CLIENT_cb, NULL);
  assert(zigbee_app_info.alarm_client != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.alarm_client);

  /* DoorLock Client */
  zigbee_app_info.doorlock_client = ZbZclDoorLockClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT);
  assert(zigbee_app_info.doorlock_client != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.doorlock_client);

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
    struct ZbStartupT config;
    enum ZbStatusCodeT status;

    /* Configure Zigbee Logging (only need to do this once, but this is a good place to put it) */
    ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_5, NULL);

    /* Attempt to join a zigbee network */
    ZbStartupConfigGetProDefaults(&config);

    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_ROUTER");
    config.startupControl = ZbStartTypeJoin;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);   
    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /* Channel in use*/

    /* Using ZbStartupWait (blocking) here instead of ZbStartup, in order to demonstrate how to do
     * a blocking call on the M4. */
    status = ZbStartupWait(zigbee_app_info.zb, &config);

    APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
    zigbee_app_info.join_status = status;

    if (status == ZB_STATUS_SUCCESS) {
      zigbee_app_info.join_delay = 0U;
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
  else
  {

    /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
    uint32_t bcast_timeout = 3;
    ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
    
    /* Starting application init task */
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_START, CFG_SCH_PRIO_0);
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
    APP_DBG("Door lock Client on Endpoint %d",SW1_ENDPOINT );
    APP_DBG("Alarm Client on Endpoint %d",SW1_ENDPOINT );
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
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL){
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    return;
  }
  if (epid == 0U) {
    return;
  }
  
  APP_ZIGBEE_DoorLock_Client_Send_Unlock();
} /* APP_ZIGBEE_SW1_Process */

/**
 * @brief  Button 2 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW2_Process(void)
{
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL){
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    return;
  }
  if (epid == 0U) {
    return;
  }
  
  APP_ZIGBEE_DoorLock_Client_Send_Lock();
} /* APP_ZIGBEE_SW2_Process */

/**
 * @brief  Button 3 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW3_Process(void)
{
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL){
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    return;
  }
  if (epid == 0U) {
    return;
  }
  
  APP_ZIGBEE_DoorLock_Client_Send_UnlockWithWrongPIN();
} /* APP_ZIGBEE_SW3_Process */

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
    if (CptReceiveRequestFromM0 != 0) {
        CptReceiveRequestFromM0 = 0;
        Zigbee_M0RequestProcessing();
    }
}

