
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/app_zigbee.c
  * @author  MCD Application Team
  * @brief   Zigbee Application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

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

/* Private includes -----------------------------------------------------------*/
#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.meter.id.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define CHANNEL                                     19

#define SW1_ENDPOINT                                17

/* USER CODE BEGIN PD */
#define NB_MAX_STRING                               10
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* External definition -------------------------------------------------------*/
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* USER CODE BEGIN ED */
/* USER CODE END ED */

/* Private function prototypes -----------------------------------------------*/
static void APP_ZIGBEE_StackLayersInit(void);
static void APP_ZIGBEE_ConfigEndpoints(void);
static void APP_ZIGBEE_NwkForm(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);

static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);

/* USER CODE BEGIN PFP */
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_SW2_Process(void);
static void APP_ZIGBEE_App_Init(void);
static void APP_ZIGBEE_MeterId_Client_Init(void);
static void APP_ZIGBEE_MeterId_Client_Read_COMPANY_NAME_ATTR(void);
static void APP_ZIGBEE_MeterId_Client_Read_METER_TYPE_ID_ATTR(void);
static void APP_ZIGBEE_MeterId_Read_Attr_cb(const ZbZclReadRspT *rsp, void *arg);
/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
static TL_CmdPacket_t   *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t   *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t   *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t    CptReceiveNotifyFromM0 = 0;
static __IO uint32_t    CptReceiveRequestFromM0 = 0;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRequestBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
uint8_t g_ot_notification_allowed = 0U;

struct zigbee_app_info
{
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;
  bool init_after_join;

  struct ZbZclClusterT *meter_id_client_1;
};
static struct zigbee_app_info zigbee_app_info;

/* USER CODE BEGIN PV */
/* USER CODE END PV */
/* Functions Definition ------------------------------------------------------*/

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

  /* USER CODE BEGIN APP_ZIGBEE_INIT */
  /* Task associated with push button SW1 and SW2 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_SW2_Process);
  
  /* Task associated with application init */
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_START, UTIL_SEQ_RFU, APP_ZIGBEE_App_Init);
  /* USER CODE END APP_ZIGBEE_INIT */

  /* Start the Zigbee on the CPU2 side */
  ZigbeeInitStatus = SHCI_C2_ZIGBEE_Init();
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ZigbeeInitStatus);

  /* Initialize Zigbee stack layers */
  APP_ZIGBEE_StackLayersInit();

}

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

  /* USER CODE BEGIN APP_ZIGBEE_StackLayersInit */
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);
  /* USER CODE END APP_ZIGBEE_StackLayersInit */

  /* Configure the joining parameters */
  zigbee_app_info.join_status = (enum ZbStatusCodeT) 0x01; /* init to error status */
  zigbee_app_info.join_delay = HAL_GetTick(); /* now */
  zigbee_app_info.startupControl = ZbStartTypeJoin;

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  /* run the task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
}

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

  /* Endpoint: SW1_ENDPOINT */
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_ONOFF_SWITCH;
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);

  /* Meter id client */
  zigbee_app_info.meter_id_client_1 = ZbZclMeterIdClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT);
  assert(zigbee_app_info.meter_id_client_1 != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.meter_id_client_1);

  /* USER CODE BEGIN CONFIG_ENDPOINT */
  /* USER CODE END CONFIG_ENDPOINT */
}

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

    /* Configure Zigbee Logging */
    ZbSetLogging(zigbee_app_info.zb, ZB_LOG_MASK_LEVEL_5, NULL);

    /* Attempt to join a zigbee network */
    ZbStartupConfigGetProDefaults(&config);

    /* Set the centralized network */
    APP_DBG("Network config : APP_STARTUP_CENTRALIZED_ROUTER");
    config.startupControl = zigbee_app_info.startupControl;

    /* Using the default HA preconfigured Link Key */
    memcpy(config.security.preconfiguredLinkKey, sec_key_ha, ZB_SEC_KEYSIZE);

    config.channelList.count = 1;
    config.channelList.list[0].page = 0;
    config.channelList.list[0].channelMask = 1 << CHANNEL; /*Channel in use */

    /* Using ZbStartupWait (blocking) */
    status = ZbStartupWait(zigbee_app_info.zb, &config);

    APP_DBG("ZbStartup Callback (status = 0x%02x)", status);
    zigbee_app_info.join_status = status;

    if (status == ZB_STATUS_SUCCESS)
    {
      zigbee_app_info.join_delay = 0U;
      zigbee_app_info.init_after_join = true;
      APP_DBG("Startup done !\n");
      /* USER CODE BEGIN 0 */
      BSP_LED_On(LED_BLUE);

      /* USER CODE END 0 */
    }
    else
    {
      APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
      zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
      /* USER CODE BEGIN 1 */

      /* USER CODE END 1 */
    }
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_FORM, CFG_SCH_PRIO_0);
  }
  /* USER CODE BEGIN NW_FORM */
  else
  {
    zigbee_app_info.init_after_join = false;

    /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
    uint32_t bcast_timeout = 3;
    ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
    
    /* Starting application init task */
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_START, CFG_SCH_PRIO_0);
  }
  /* USER CODE END NW_FORM */
}

/*************************************************************
 * ZbStartupWait Blocking Call
 *************************************************************/
struct ZbStartupWaitInfo
{
  bool active;
  enum ZbStatusCodeT status;
};

static void ZbStartupWaitCb(enum ZbStatusCodeT status, void *cb_arg)
{
  struct ZbStartupWaitInfo *info = cb_arg;

  info->status = status;
  info->active = false;
  UTIL_SEQ_SetEvt(EVENT_ZIGBEE_STARTUP_ENDED);
}

enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config)
{
  struct ZbStartupWaitInfo *info;
  enum ZbStatusCodeT status;

  info = malloc(sizeof(struct ZbStartupWaitInfo));
  if (info == NULL)
  {
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
}

/**
 * @brief  Trace the error or the warning reported.
 * @param  ErrId :
 * @param  ErrCode
 * @retval None
 */
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode)
{
  switch (ErrId)
  {
    default:
      APP_ZIGBEE_TraceError("ERROR Unknown ", 0);
      break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * @brief  Warn the user that an error has occurred.
 *
 * @param  pMess  : Message associated to the error.
 * @param  ErrCode: Error code associated to the module (Zigbee or other module if any)
 * @retval None
 */
static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode)
{
  APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
  /* USER CODE BEGIN TRACE_ERROR */
  while (1U == 1U) 
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500U);
  }
  /* USER CODE END TRACE_ERROR */

}

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

  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success)
  {
    APP_ZIGBEE_Error((uint32_t)ERR_ZIGBEE_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
  }
  else
  {
    APP_DBG("**********************************************************");
    APP_DBG("WIRELESS COPROCESSOR FW:");
    /* Print version */
    APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

    switch (p_wireless_info->StackType)
    {
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

    /* print the application name */
    char *__PathProject__ = (strstr(__FILE__, "Zigbee") ? strstr(__FILE__, "Zigbee") + 7 : __FILE__);
    char *pdel = NULL;
    if((strchr(__FILE__, '/')) == NULL)
    {
      pdel = strchr(__PathProject__, '\\');
    }
    else
    {
      pdel = strchr(__PathProject__, '/');
    }

    int index = (int)(pdel - __PathProject__);
    APP_DBG("Application flashed: %*.*s", index, index, __PathProject__);

    /* print channel */
    APP_DBG("Channel used: %d", CHANNEL);
    /* print Link Key */
    APP_DBG("Link Key: %.16s", sec_key_ha);
    /* print Link Key value hex */
    char Z09_LL_string[ZB_SEC_KEYSIZE*3+1];
    Z09_LL_string[0] = 0;
    for (int str_index = 0; str_index < ZB_SEC_KEYSIZE; str_index++)
    {
      sprintf(&Z09_LL_string[str_index*3], "%02x ", sec_key_ha[str_index]);
    }

    APP_DBG("Link Key value: %s", Z09_LL_string);
    /* print clusters allocated */
    APP_DBG("Clusters allocated are:");
    APP_DBG("meter_id Client on Endpoint %d", SW1_ENDPOINT);
    APP_DBG("**********************************************************");
  }
}

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer)
{
  p_ZIGBEE_otcmdbuffer = p_buffer;
}

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)p_ZIGBEE_otcmdbuffer->cmdserial.cmd.payload;
}

Zigbee_Cmd_Request_t * ZIGBEE_Get_OTCmdRspPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)((TL_EvtPacket_t *)p_ZIGBEE_otcmdbuffer)->evtserial.evt.payload;
}

Zigbee_Cmd_Request_t * ZIGBEE_Get_NotificationPayloadBuffer(void)
{
  return (Zigbee_Cmd_Request_t *)(p_ZIGBEE_notif_M0_to_M4)->evtserial.evt.payload;
}

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
}

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
 * @brief  This function is called when the M0+ acknowledge the fact that it has received a Cmd
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
}

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
}

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
}

/**
 * @brief  This function waits for getting an acknowledgment from the M0.
 *
 * @param  None
 * @retval None
 */
static void Wait_Getting_Ack_From_M0(void)
{
  UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_M0_EVT);
}

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
}

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
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessRequestM0ToM4(void)
{
  if (CptReceiveRequestFromM0 != 0)
  {
    CptReceiveRequestFromM0 = 0;
    Zigbee_M0RequestProcessing();
  }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_App_Init(void)
{
  /* Initialize Zigbee Meter Identification Client parameters */
  APP_ZIGBEE_MeterId_Client_Init();
}

/**
 * @brief  Meter Identification client initialization
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_MeterId_Client_Init(void)
{
  APP_DBG("[METER ID] Meter Identification client init done!\n");  
}

/**
 * @brief  Reading Company Name attribute (Meter Identification) from server
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_MeterId_Client_Read_COMPANY_NAME_ATTR(void)
{
  enum ZclStatusCodeT status;
  ZbZclReadReqT req;
  
  APP_DBG("[METER ID] Reading Company Name attribute ...");  
  
  /* Read request */
  memset(&req, 0, sizeof(req));
  req.dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  req.dst.endpoint = SW1_ENDPOINT;
  req.dst.nwkAddr = 0x0;
  req.count = 1;
  req.attr[0] = ZCL_METER_ID_ATTR_COMPANY_NAME;
  
  status = ZbZclReadReq(zigbee_app_info.meter_id_client_1, &req, APP_ZIGBEE_MeterId_Read_Attr_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS) 
  {
     APP_DBG("[METER ID] Error, ZbZclReadReq failed.");
  }
}

/**
 * @brief  Reading Meter Type ID attribute (Meter Identification) from server
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_MeterId_Client_Read_METER_TYPE_ID_ATTR(void)
{
  enum ZclStatusCodeT status;
  ZbZclReadReqT req;
  
  APP_DBG("[METER ID] Reading Meter Type ID attribute ...");  
  
  /* Read request */
  memset(&req, 0, sizeof(req));
  req.dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  req.dst.endpoint = SW1_ENDPOINT;
  req.dst.nwkAddr = 0x0;
  req.count = 1;
  req.attr[0] = ZCL_METER_ID_ATTR_METER_TYPE_ID;
  
  status = ZbZclReadReq(zigbee_app_info.meter_id_client_1, &req, APP_ZIGBEE_MeterId_Read_Attr_cb, NULL);
  if (status != ZCL_STATUS_SUCCESS)
  {
     APP_DBG("[METER ID] Error, ZbZclReadReq failed.");
  }
  
}

/**
 * @brief  Read Attribute caalback
 * @param  rsp: ZCL response
 * @param  arg: passed argument
 * @retval None
 */
static void APP_ZIGBEE_MeterId_Read_Attr_cb(const ZbZclReadRspT *rsp, void *arg){
  uint8_t buf[256];
  uint8_t char_buf[NB_MAX_STRING];
  unsigned int *val=(unsigned int*)buf;
  unsigned int length;
  
  UNUSED(arg);
  memset(buf, 0, sizeof(buf)/sizeof(uint8_t));
  memset(char_buf, 0, sizeof(char_buf)/sizeof(uint8_t));
  
  if (rsp->status != ZCL_STATUS_SUCCESS)
  {
    APP_DBG("[METER ID] The server returned an error: 0x%02x.\n", rsp->status);
  }
  
  memcpy(buf, rsp->attr[0].value, rsp->attr[0].length);
  
  switch(rsp->attr[0].attrId)
  {
    case ZCL_METER_ID_ATTR_COMPANY_NAME:
      /* Compute ZCL string in standard string */
      length = buf[0];
      memcpy(char_buf, &buf[1], length);
      char_buf[length+1]='\0';
      APP_DBG("[METER ID] Company Name attribute value: \"%s\".\n", (char*)char_buf);
      break;
      
    case ZCL_METER_ID_ATTR_METER_TYPE_ID:
      APP_DBG("[METER ID] Meter Type ID attribute value: 0x%04x.\n", *val);
      break;
      
    default:
      APP_DBG("[METER ID] Error: unsupported attribute.\n");
      break;
  }
}

/**
 * @brief  Button 1 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW1_Process(void)
{
  uint64_t epid = 0U;

  if (zigbee_app_info.zb == NULL)
  {
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) 
  {
    return;
  }
  if (epid == 0U) 
  {
    return;
  }
  
  APP_ZIGBEE_MeterId_Client_Read_COMPANY_NAME_ATTR();
}

/**
 * @brief  Button 2 callback
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW2_Process(void)
{
  uint64_t epid = 0U;

  if (zigbee_app_info.zb == NULL)
  {
    return;
  }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS)
  {
    return;
  }
  if (epid == 0U)
  {
    return;
  }
  
  APP_ZIGBEE_MeterId_Client_Read_METER_TYPE_ID_ATTR();
}

/* USER CODE END FD_LOCAL_FUNCTIONS */
