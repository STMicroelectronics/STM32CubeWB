
/* USER CODE BEGIN Header */
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
#include "stm32wb5mm_dk_bus.h"
/* Private includes -----------------------------------------------------------*/
#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.temp.meas.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               500U
#define CHANNEL                                     13

#define SW1_ENDPOINT                                17

/* USER CODE BEGIN PD */
#define SW1_GROUP_ADDR          0x0001
#define READ_UPDATE_PERIOD       (uint32_t)(0.5*1000*1000/CFG_TS_TICK_VAL) /*500ms*/
/* Report defines */
#define ZCL_MIN_REPORT 0x000A /* 10s*/
#define ZCL_MAX_REPORT 0x000A
#define ZCL_REPORT_CHANGE 0x0000
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
static void APP_ZIGBEE_ConfigGroupAddr(void);
static void APP_ZIGBEE_Read_Temp_Attribute(void);
static void APP_ZIGBEE_Temp_Meas_Read_cb(const ZbZclReadRspT *readRsp, void *arg);
static void APP_ZIGBEE_Update_Timer_Callback(void);
static void LED_On(aPwmLedGsData_TypeDef aPwmLedGsData);
static void LED_Off(void);
static void LED_Deinit(void);
static void LED_Toggle(aPwmLedGsData_TypeDef aPwmLedGsData);
static void APP_ZIGBEE_ReportConfig(void);
static void APP_ZIGBEE_Report_cb(struct ZbZclCommandRspT *cmd_rsp,void *arg);
static void APP_ZIGBEE_Temp_meas_client_report(struct ZbZclClusterT *clusterPtr, 
                                               struct ZbApsdeDataIndT *dataIndPtr, 
                                               uint16_t attributeId,
                                               enum ZclDataTypeT dataType, 
                                               const uint8_t *in_payload,
                                               uint16_t in_len);
static void APP_Zigbee_Display_ErrorCnt(void);
/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
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
  struct ZbZclClusterT *temp_meas_client;
};
static struct zigbee_app_info zigbee_app_info;
uint8_t APP_ZIGBEE_Update_Timer_Id;
uint32_t APP_ZIgbee_Error_Cnt = 0;

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
  LED_Off();
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
  /* Task associated with push button SW1 */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_SW2_Process);
  
  /* Read attribute task */
   UTIL_SEQ_RegTask(1U << CFG_TASK_READ_ATTR, UTIL_SEQ_RFU, APP_ZIGBEE_Read_Temp_Attribute);
   /* Create timer to get the measure of environment data */
    HW_TS_Create(CFG_TIM_PROC_ID_ISR,
                 &APP_ZIGBEE_Update_Timer_Id,
                 hw_ts_Repeated,
                 APP_ZIGBEE_Update_Timer_Callback);
  /* USER CODE END APP_ZIGBEE_INIT */

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

  /* Configure the joining parameters */
  zigbee_app_info.join_status = (enum ZbStatusCodeT) 0x01; /* init to error status */
  zigbee_app_info.join_delay = HAL_GetTick(); /* now */
  zigbee_app_info.startupControl = ZbStartTypeJoin;

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  /* run the task */
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

  /* Endpoint: SW1_ENDPOINT */
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_ENVIRONMENTAL_SENSOR;
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);

  /* Temp meas client */
  zigbee_app_info.temp_meas_client = ZbZclTempMeasClientAlloc(zigbee_app_info.zb,SW1_ENDPOINT);
  assert(zigbee_app_info.temp_meas_client != NULL);
  zigbee_app_info.temp_meas_client->report = &APP_ZIGBEE_Temp_meas_client_report;
  ZbZclClusterEndpointRegister(zigbee_app_info.temp_meas_client);
  /* USER CODE BEGIN CONFIG_ENDPOINT */
  /* USER CODE END CONFIG_ENDPOINT */
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

    if (status == ZB_STATUS_SUCCESS) {
      /* USER CODE BEGIN 0 */
      zigbee_app_info.join_delay = 0U;
      zigbee_app_info.init_after_join = true;
      UTIL_LCD_ClearStringLine(2);
      UTIL_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)"Join OK", CENTER_MODE);
      BSP_LCD_Refresh(0);
    }
    else
    {
      /* USER CODE END 0 */
      APP_DBG("Startup failed, attempting again after a short delay (%d ms)", APP_ZIGBEE_STARTUP_FAIL_DELAY);
      zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
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

    /* Assign ourselves to the group addresses */
    APP_ZIGBEE_ConfigGroupAddr();

    /* Since we're using group addressing (broadcast), shorten the broadcast timeout */
    uint32_t bcast_timeout = 3;
    ZbNwkSet(zigbee_app_info.zb, ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime, &bcast_timeout, sizeof(bcast_timeout));
  }
  APP_Zigbee_Display_ErrorCnt();
  APP_ZIGBEE_ReportConfig();
  /* USER CODE END NW_FORM */
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

/**
 * @brief  LCD initialisation
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_LCD_DisplayInit(void)
{
    BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);
    /* Set LCD Foreground Layer  */
    UTIL_LCD_SetFuncDriver(&LCD_Driver); /* SetFunc before setting device */
    UTIL_LCD_SetDevice(0);            /* SetDevice after funcDriver is set */
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_DisplayOn(0);
    BSP_LCD_Refresh(0);
    UTIL_LCD_SetFont(&Font12);
    /* Set the LCD Text Color */
    UTIL_LCD_SetTextColor(SSD1315_COLOR_WHITE);
    UTIL_LCD_SetBackColor(SSD1315_COLOR_BLACK);
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)"Temp_Meas CLIENT", CENTER_MODE);
    BSP_LCD_Refresh(0);
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
  while (1U == 1U) {
    UTIL_LCD_ClearStringLine(4);
    UTIL_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)"FATAL_ERROR", CENTER_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(500U);
    UTIL_LCD_ClearStringLine(4);
    BSP_LCD_Refresh(0);

  }
  /* USER CODE END TRACE_ERROR */

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
    APP_DBG("Temperature measurement Client on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("**********************************************************");
  }
} /* APP_ZIGBEE_CheckWirelessFirmwareInfo */

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
 * @param  None
 * @return None
 */
static void APP_ZIGBEE_ProcessRequestM0ToM4(void)
{
    if (CptReceiveRequestFromM0 != 0) {
        CptReceiveRequestFromM0 = 0;
        Zigbee_M0RequestProcessing();
    }
}
/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

/**
 * @brief  Set group addressing mode
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ConfigGroupAddr(void)
{
  struct ZbApsmeAddGroupReqT req;
  struct ZbApsmeAddGroupConfT conf;

  memset(&req, 0, sizeof(req));
  req.endpt = SW1_ENDPOINT;
  req.groupAddr = SW1_GROUP_ADDR;
  ZbApsmeAddGroupReq(zigbee_app_info.zb, &req, &conf);

} /* APP_ZIGBEE_ConfigGroupAddr */

static void APP_ZIGBEE_SW1_Process()
{
   HW_TS_Start(APP_ZIGBEE_Update_Timer_Id, READ_UPDATE_PERIOD);// APP_ZIGBEE_Read_Temp_Attribute();
}
static void APP_ZIGBEE_SW2_Process()
{
   HW_TS_Stop(APP_ZIGBEE_Update_Timer_Id);// APP_ZIGBEE_Read_Temp_Attribute();
}

static void APP_ZIGBEE_Update_Timer_Callback(void)
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_READ_ATTR, CFG_SCH_PRIO_0);
}

  /**
 * @brief  Read OTA temp attribute
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_Read_Temp_Attribute(void)
{
  ZbZclReadReqT readReq;
  uint64_t epid = 0U;
  aPwmLedGsData_TypeDef aPwmLedGsData;

  aPwmLedGsData[PWM_LED_RED] = PWM_LED_GSDATA_7_0;
  aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_BLUE] = PWM_LED_GSDATA_7_0;
  LED_Toggle(aPwmLedGsData);
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
  
  memset(&readReq, 0, sizeof(readReq));
  readReq.dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  readReq.dst.endpoint =SW1_ENDPOINT;
  readReq.dst.nwkAddr = 0x0000;
  
  readReq.count = 1U;
  readReq.attr[0]=ZCL_TEMP_MEAS_ATTR_MEAS_VAL ;
   
  APP_DBG("[TEMP MEAS] Triggered a Read Coordinator Temp Attributes");
  if(ZbZclReadReq(zigbee_app_info.temp_meas_client, &readReq, APP_ZIGBEE_Temp_Meas_Read_cb,zigbee_app_info.temp_meas_client) != ZCL_STATUS_SUCCESS)
  {
    APP_DBG("[TEMP MEAS] Error, ZbZclReadReq failed (SW1_ENDPOINT)");
    APP_ZIgbee_Error_Cnt++;
    UTIL_LCD_ClearStringLine(3);
    UTIL_LCD_DisplayStringAt(0, LINE(3), (uint8_t *) "READ ERR", CENTER_MODE);
    BSP_LCD_Refresh(0);
    APP_Zigbee_Display_ErrorCnt();
  }
  
}/*APP_ZIGBEE_Read_Temp_Attribute*/

            



 /**
 * @brief  Read OTA temp attribute callback
 * @param  read rsp
 * @retval None
 */
static void APP_ZIGBEE_Temp_Meas_Read_cb(const ZbZclReadRspT *readRsp, void *arg)
{
  int i;
  int16_t current_temp_attr;
  float current_temp;
  char text[32];
  aPwmLedGsData_TypeDef aPwmLedGsData;
  aPwmLedGsData[PWM_LED_RED] = PWM_LED_GSDATA_7_0;
  aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_BLUE] = PWM_LED_GSDATA_OFF;
 
  if (readRsp->count == 0)
  {
     APP_ZIgbee_Error_Cnt++;
     APP_DBG("[TEMP MEAS] Error, No attribute read");
     UTIL_LCD_ClearStringLine(3);
     UTIL_LCD_DisplayStringAt(0, LINE(3), (uint8_t *) "READ NO ATTR ERR", CENTER_MODE);
     BSP_LCD_Refresh(0);
     LED_On(aPwmLedGsData);
     APP_Zigbee_Display_ErrorCnt();
  }
  else
  {
    for(i=0;i<readRsp->count;i++)
    {
      if(readRsp->attr[i].status == ZCL_STATUS_SUCCESS)
      {
        switch(readRsp->attr[i].attrId)
        {
        case ZCL_TEMP_MEAS_ATTR_MEAS_VAL:
          current_temp_attr = pletoh16(readRsp->attr[i].value);
          current_temp = (float)current_temp_attr/100;
          APP_DBG("[TEMP MEAS] Current Temp= %.1f C",(double)current_temp);
          sprintf(text,"Remote T:%.1f C",(double)current_temp);
          UTIL_LCD_ClearStringLine(3);
          UTIL_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)text, CENTER_MODE);
          BSP_LCD_Refresh(0);
          break;
     
        default:
          break;
        }
      }
      else
      {
        APP_ZIgbee_Error_Cnt++;
        APP_DBG("[TEMP MEAS] Failed to read attribute %d",readRsp->attr[i].attrId);
        UTIL_LCD_ClearStringLine(3);
        UTIL_LCD_DisplayStringAt(0, LINE(3), (uint8_t *) "READ_CB ERR", CENTER_MODE);
        BSP_LCD_Refresh(0);
        LED_On(aPwmLedGsData);
        APP_Zigbee_Display_ErrorCnt();
      }
    }
  }
}/*APP_ZIGBEE_Temp_Meas_Read_cb*/

/**
 * @brief  Display error counter
 * @param  None
 * @retval None
 */            
static void APP_Zigbee_Display_ErrorCnt(void)
{
   char text[32];
   sprintf(text,"Error Cnt: %ld",APP_ZIgbee_Error_Cnt);
   UTIL_LCD_ClearStringLine(4);
   UTIL_LCD_DisplayStringAt(0, LINE(4), (uint8_t * ) text, CENTER_MODE);
   BSP_LCD_Refresh(0);
}/*APP_Zigbee_Display_ErrorCnt*/
            
            
/* Report management */

/**
 * @brief  report 
 * @param  cluster,dataind,attribute,datatype,datapayload,datalength
 * @retval None
 */
static void APP_ZIGBEE_Temp_meas_client_report(struct ZbZclClusterT *clusterPtr, 
                                               struct ZbApsdeDataIndT *dataIndPtr, 
                                               uint16_t attributeId,
                                               enum ZclDataTypeT dataType, 
                                               const uint8_t *in_payload,
                                               uint16_t in_len)
{
    int attrLen;
    uint16_t attr_val;
    int16_t temp_cur;
    float temp_val;
    char text[32]; 
    aPwmLedGsData_TypeDef aPwmLedGsData;
    
    attrLen = ZbZclAttrParseLength(dataType, in_payload, dataIndPtr->asduLength, 0);
    if (attrLen < 0)
    {
        APP_DBG("[TEMP MEAS] Report error length 0");
        APP_ZIgbee_Error_Cnt++;
        APP_Zigbee_Display_ErrorCnt();
        return;
    }
    if (attrLen > (int)in_len)
    {
          APP_DBG("[TEMP MEAS] Report error length >");
          APP_ZIgbee_Error_Cnt++;
          APP_Zigbee_Display_ErrorCnt();
          return;
    }
    if (dataIndPtr->src.endpoint!= SW1_ENDPOINT) 
    {
          APP_DBG("[TEMP MEAS] Report error wrong endpoint");
          APP_ZIgbee_Error_Cnt++;
          APP_Zigbee_Display_ErrorCnt();
          return;
    }
    
    
    switch(attributeId)
    {
      case ZCL_TEMP_MEAS_ATTR_MEAS_VAL:
        attr_val= pletoh16(in_payload);
        temp_cur = (int16_t)attr_val;
        APP_DBG("[From 0x%016llx] - [TEMP MEAS]     %d C",dataIndPtr->src.extAddr,temp_cur/100);
        temp_val = (float) temp_cur/100;
        sprintf(text,"Rep.Rem T:%.1f C",(double)temp_val);
        UTIL_LCD_ClearStringLine(3);
        UTIL_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)text, CENTER_MODE);
        BSP_LCD_Refresh(0);
        aPwmLedGsData[PWM_LED_RED] = PWM_LED_GSDATA_OFF;
        aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
        aPwmLedGsData[PWM_LED_BLUE] = PWM_LED_GSDATA_7_0;
        LED_Toggle(aPwmLedGsData);
        break;
      
      default:
        APP_DBG("[From 0x%016llx] - Reported another attribute 0x%04X",dataIndPtr->src.extAddr,attributeId);
        APP_DBG("");
        break;
    }
 
} /* APP_ZIGBEE_temp_meas_client_report */

/**
 * @brief  report config 
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ReportConfig(void)
{
  uint16_t rep_change = (uint16_t) ZCL_REPORT_CHANGE;
  struct ZbZclAttrReportConfigT report;
  report.dst.endpoint = SW1_ENDPOINT;
  report.dst.mode = ZB_APSDE_ADDRMODE_SHORT;
  report.dst.nwkAddr = 0x0000;
  report.num_records = 1;
  report.record_list[0].direction = ZCL_REPORT_DIRECTION_NORMAL;
  report.record_list[0].min = ZCL_MIN_REPORT;
  report.record_list[0].max = ZCL_MAX_REPORT;
  report.record_list[0].change = 0;
  report.record_list[0].attr_id = ZCL_TEMP_MEAS_ATTR_MEAS_VAL;
  report.record_list[0].attr_type = ZCL_DATATYPE_SIGNED_16BIT;
  ZbZclAttrReportConfigReq(zigbee_app_info.temp_meas_client,&report,&APP_ZIGBEE_Report_cb,(void*)&rep_change);
}/*APP_ZIGBEE_ReportConfig*/

/**
 * @brief  report config callback
 * @param  cmdRsp,arg
 * @retval None
 */
static void APP_ZIGBEE_Report_cb(struct ZbZclCommandRspT *cmd_rsp,void *arg)
{
  if(cmd_rsp->status != ZCL_STATUS_SUCCESS)
  {
    APP_ZIgbee_Error_Cnt++;
    APP_DBG("[TEMP MEAS] Report Config Failed error:%d",cmd_rsp->status);
    APP_Zigbee_Display_ErrorCnt();
  }
  else
  {
    APP_DBG("[TEMP MEAS] Report Config set with success");
  }
}/*APP_ZIGBEE_Report_cb*/



/* LED Management */

/**
 * @brief  Deinit LED
 * @param  None
 * @retval None
 */
static void LED_Deinit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure = {0};
  
  /* RGB Led de-init */
  BSP_PWM_LED_DeInit();

  /* configure SPIx MOSI for LCD */
  GPIO_InitStructure.Pin       = BUS_SPI1_MOSI_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Alternate = BUS_SPI1_AF;
  HAL_GPIO_Init(BUS_SPI1_GPIO_PORTA, &GPIO_InitStructure);
}/*LED_Deinit*/

/**
 * @brief  LED On
 * @param  PwmData
 * @retval None
 */
static void LED_On(aPwmLedGsData_TypeDef aPwmLedGsData)
{
  BSP_PWM_LED_Init();
  BSP_PWM_LED_On(aPwmLedGsData);
  LED_Deinit();
}/*LED_On*/

/**
 * @brief  LED off
 * @param  None
 * @retval None
 */
static void LED_Off(void)
{
  BSP_PWM_LED_Init();
  BSP_PWM_LED_Off();
  LED_Deinit();
}/*LED_Off*/

/**
 * @brief  LED Toggle
 * @param  PwmData
 * @retval None
 */
static void LED_Toggle(aPwmLedGsData_TypeDef aPwmLedGsData)
{
  if (BSP_PWM_LED_GetState() == 0U)
  {
    LED_On(aPwmLedGsData);
  }
  else
  {
    LED_Off();
  }
}/*LED_Toggle*/

/* USER CODE END FD_LOCAL_FUNCTIONS */


