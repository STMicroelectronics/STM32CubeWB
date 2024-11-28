/* Copyright [2020 - 2020] Exegin Technologies Limited. All rights reserved. */

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

/* Private includes -----------------------------------------------------------*/
#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.identify.h"
#include "zcl/security/zcl.ias_wd.h"
#include "zcl/se/zcl.message.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               5000U
#define CHANNEL                                     13U

#define SW1_ENDPOINT                                12U

/* USER CODE BEGIN PD */
#define SW1_GROUP_ADDR          0x0001U
#define SW1_SCENE_ID            0x0001U
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
static void APP_ZIGBEE_NwkJoin(void);

static void APP_ZIGBEE_TraceError(const char *pMess, uint32_t ErrCode);
static void APP_ZIGBEE_CheckWirelessFirmwareInfo(void);

static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
static void APP_ZIGBEE_ProcessNotifyM0ToM4(void);
static void APP_ZIGBEE_ProcessRequestM0ToM4(void);
static void check_bindings(void);

/* USER CODE BEGIN PFP */
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_SW2_Process(void);
static void APP_ZIGBEE_SW3_Process(void);
static void APP_ZIGBEE_ConfigGroupAddr(void);
static void check_bindings(void);
static int numOfBindings(void);
static enum ZclStatusCodeT msg_client_display_msg_cb(struct ZbZclClusterT *cluster, void *arg,
    struct ZbZclMsgMessageT *msg, struct ZbZclAddrInfoT *srcInfo);
static enum ZclStatusCodeT allocate_IAS_client(void);
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
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;
  bool init_after_join;

  uint32_t fb_delay;
  uint8_t numOfBindings;
  struct ZbZclClusterT *messaging_client;
  struct ZbZclClusterT *identify_client;
  struct ZbZclClusterT *ias_client;
};
static struct zigbee_app_info zigbee_app_info;

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
  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_NETWORK_JOIN, UTIL_SEQ_RFU, APP_ZIGBEE_NwkJoin);

  /* Register tasks to handle button press */
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW1, UTIL_SEQ_RFU, APP_ZIGBEE_SW1_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW2, UTIL_SEQ_RFU, APP_ZIGBEE_SW2_Process);
  UTIL_SEQ_RegTask(1U << CFG_TASK_BUTTON_SW3, UTIL_SEQ_RFU, APP_ZIGBEE_SW3_Process);

  UTIL_SEQ_RegTask(1U << CFG_TASK_ZIGBEE_APP_CHECK_BINDINGS, UTIL_SEQ_RFU, check_bindings);


  /* USER CODE BEGIN APP_ZIGBEE_INIT */
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

  /* Initialization Complete */
  zigbee_app_info.has_init = true;

  /* run the task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_JOIN, CFG_SCH_PRIO_0);
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
  struct ZbZclMsgClientCallbacksT msg_callbacks;

  memset(&req, 0, sizeof(req));
  memset(&msg_callbacks, 0, sizeof(msg_callbacks));
  msg_callbacks.display_message = msg_client_display_msg_cb;

  /* Endpoint: SW1_ENDPOINT */
  req.profileId = ZCL_PROFILE_HOME_AUTOMATION;
  req.deviceId = ZCL_DEVICE_ONOFF_SWITCH;
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);

  zigbee_app_info.messaging_client = ZbZclMsgClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT, &msg_callbacks, NULL);
  assert(zigbee_app_info.messaging_client != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.messaging_client);
  // ZbZclClusterSetMinSecurity(zigbee_app_info.messaging_client, ZB_APS_STATUS_SECURED_LINK_KEY);
  ZbZclClusterSetProfileId(zigbee_app_info.messaging_client, ZCL_PROFILE_HOME_AUTOMATION);

  /* Idenfity server */
  zigbee_app_info.identify_client = ZbZclIdentifyClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT);
  assert(zigbee_app_info.identify_client != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.identify_client);
}

/**
 * @brief  Handle Zigbee network forming and joining
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkJoin(void)
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
    config.startupControl = ZbStartTypeJoin;

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
      /* USER CODE BEGIN 3 */
      zigbee_app_info.join_delay = 0U;
      zigbee_app_info.init_after_join = true;
      BSP_LED_Off(LED_RED);
      BSP_LED_On(LED_BLUE);
    }
    else
    {
      /* USER CODE END 3 */
      BSP_LED_On(LED_RED);
      APP_DBG("Startup failed, attempting again after a short delay");
      zigbee_app_info.join_delay = HAL_GetTick() + APP_ZIGBEE_STARTUP_FAIL_DELAY;
    }
  }

  /* If Network forming/joining was not successful reschedule the current task to retry the process */
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_NETWORK_JOIN, CFG_SCH_PRIO_0);
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
  /* USER CODE END NW_FORM */
}

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
}

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
}

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
    APP_DBG("Identify Client on Endpoint %d",SW1_ENDPOINT);
    APP_DBG("Messaging Client on Endpoint %d",SW1_ENDPOINT);
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
}

/**
 * @brief  Button 1 handler. Sends a get last message command via binding if device is on a network.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW1_Process()
{
  uint8_t status = ZCL_STATUS_FAILURE;
  uint64_t epid = 0U;

  if (zigbee_app_info.zb == NULL) {
    APP_DBG("Error, zigbee stack not initialized");
    return;
  }

  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    APP_DBG("Error, failed to get network information");
    return;
  }
  if (epid == 0U) {
    APP_DBG("Error, device not on a network");
    return;
  }
  APP_DBG("SW1 PUSHED (Send Get Last Message Command)");
  status = ZbZclMsgClientGetLastReq(zigbee_app_info.messaging_client, ZbApsAddrBinding, NULL, NULL);
  if (status != ZB_STATUS_SUCCESS) {
      APP_DBG(" Error, failed to send get last message request");
  }
}

/**
 * @brief  Button 2 handler. Sends a IAS start warning command via binding if device is on a network.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW2_Process()
{
  // uint8_t status = ZCL_STATUS_FAILURE;
  uint64_t epid = 0U;
  struct ZbZclIasWdClientStartWarningReqT req;

  if (zigbee_app_info.zb == NULL) {
    APP_DBG("Error, zigbee stack not initialized");
    return;
  }

  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    APP_DBG("Error, failed to get network information");
    return;
  }
  if (epid == 0U) {
    APP_DBG("Error, device not on a network");
    return;
  }

  if (zigbee_app_info.ias_client != NULL) {
    APP_DBG("SW2 PUSHED (Send Start Warning Command)");
    memset(&req, 0, sizeof(req));
    req.warning_mode = ZCL_IAS_WD_WARNING_MODE_STOP;
    req.strobe = ZCL_IAS_WD_STROBE_ON;
    req.siren_level = ZCL_IAS_WD_LEVEL_LOW;
    req.warning_duration = 3U;
    req.strobe_dutycycle = 100U;
    req.strobe_level = ZCL_IAS_WD_LEVEL_LOW;
    ZbZclIasWdClientStartWarningReq(zigbee_app_info.ias_client, ZbApsAddrBinding, &req, NULL, NULL);
  }
}

/**
 * @brief  Button 3 handler. Start Finding and Binding process as an initiator.
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_SW3_Process()
{
  uint8_t status = ZCL_STATUS_FAILURE;
  uint64_t epid = 0U;

  if (zigbee_app_info.zb == NULL) {
    APP_DBG("Error, zigbee stack not initialized");
    return;
  }

  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) {
    APP_DBG("Error, failed to get network information");
    return;
  }
  if (epid == 0U) {
    APP_DBG("Error, device not on a network");
    return;
  }
  APP_DBG("SW3 PUSHED (Initiate F&B)");
  status = ZbStartupFindBindStart(zigbee_app_info.zb, NULL, NULL);

  if (status != ZB_STATUS_SUCCESS) {
    APP_DBG(" Error, cannot start Finding & Binding, status = 0x%02x", status);
  }
  else {
    zigbee_app_info.fb_delay = HAL_GetTick() + 12000U;
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_CHECK_BINDINGS, CFG_SCH_PRIO_0);
  }
}

/**
 * @brief  Task called after F&B process to check the number of bindings. Will turn on Green LED
 * if bindings are successfully created, and attempt to allocate the IAS WD client cluster.
 *
 * @param  None
 * @retval None
 */
static void check_bindings()
{
  if (HAL_GetTick() < zigbee_app_info.fb_delay) {
    /* Reschedule task until FB delay timeout */
    UTIL_SEQ_SetTask(1U << CFG_TASK_ZIGBEE_APP_CHECK_BINDINGS, CFG_SCH_PRIO_0);
  }
  else {
    zigbee_app_info.numOfBindings = numOfBindings();
    if (zigbee_app_info.numOfBindings >= 1U) {
      BSP_LED_On(LED_GREEN);
      /* First F&B is successful allocate IAS cluster */
      allocate_IAS_client();
    }
    APP_DBG("Binding entries created: %d", zigbee_app_info.numOfBindings);
  }
}

/**
 * @brief  Checks the number of valid entries in the binding table.
 * @param  None
 * @retval Number of bindings
 */
static int numOfBindings()
{
  uint8_t i, num_entries = 0;
  struct ZbApsmeBindT entry;

  for (i = 0;; i++) {
    if (ZbApsGetIndex(zigbee_app_info.zb, ZB_APS_IB_ID_BINDING_TABLE, &entry, sizeof(entry), i) != ZB_APS_STATUS_SUCCESS) {
      break;
    }
    if (entry.srcExtAddr == 0ULL) {
      continue;
    }
    num_entries++;
  }
  return num_entries;
}

/**
 * @brief  Callback that will print out the contents of the Display Message command from the
 * ZCL Messaging server
 *
 * @param  cluster : ZCL Cluster pointer
 * @param  arg     : Passed argument
 * @param  msg     : ZCL Message command struct
 * @param  srcInfo : Sender address info
 * @retval ZCL status code
 */
static enum ZclStatusCodeT
msg_client_display_msg_cb(struct ZbZclClusterT *cluster, void *arg,
    struct ZbZclMsgMessageT *msg, struct ZbZclAddrInfoT *srcInfo)
{
  APP_DBG("MESSAGE CLIENT: DISPLAY_MSG (%d)", ZCL_MESSAGE_SVR_CMD_DISPLAY_MESSAGE);
  APP_DBG("   message_id = 0x%08x", msg->message_id);
  APP_DBG("   start_time = 0x%08x", msg->start_time);
  APP_DBG("   duration = %d", msg->duration);
  APP_DBG("   message_control = 0x%02x", msg->message_control);
  APP_DBG("   message_str = \"%s\"", msg->message_str);
  APP_DBG("   extended_control = 0x%02x", msg->extended_control);
  APP_DBG("");
  return ZCL_STATUS_SUCCESS;
}

/**
 * @brief  Allocates the IAS Warn Device Client if it doesn't exists. Otherwise, it just returns
 * a status of ZCL_STATUS_SUCCESS if IAS WD client has been previously allocated.
 * @param  None
 * @retval ZCL status code
 */
static enum ZclStatusCodeT
allocate_IAS_client()
{
  if (zigbee_app_info.ias_client != NULL) {
    /* Cluster already allocated, do nothing */
    return ZCL_STATUS_SUCCESS;
  }
  /* Idenfity server */
  zigbee_app_info.ias_client = ZbZclIasWdClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT, NULL);
  assert(zigbee_app_info.ias_client != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.ias_client);
  return ZCL_STATUS_SUCCESS;
}

/* USER CODE END FD_LOCAL_FUNCTIONS */


