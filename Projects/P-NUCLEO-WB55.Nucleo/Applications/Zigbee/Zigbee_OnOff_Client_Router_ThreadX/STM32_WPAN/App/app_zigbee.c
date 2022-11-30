
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
#include "main.h"
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
#include "tx_api.h"
#include "stm32_lpm_if.h"

/* Private includes -----------------------------------------------------------*/
#include <assert.h>
#include "zcl/zcl.h"
#include "zcl/general/zcl.onoff.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APP_ZIGBEE_STARTUP_FAIL_DELAY               5000U
#define CHANNEL                                     13

#define SW1_ENDPOINT                                17

/* USER CODE BEGIN PD */
#define SW1_GROUP_ADDR          0x0001
#define PB_REBOUND_DELAY        250

#ifdef TX_ENABLE_EVENT_TRACE
#define APP_MAX_THREAD			14u
#define DEBUG_TRACEX_SIZE		(64u * 1024u )
#define APP_TRACE_INFO          (TX_TRACE_USER_EVENT_START + 1)
#endif // TX_ENABLE_EVENT_TRACE
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

static void APP_ZIGBEE_ProcessNotifyM0ToM4(ULONG thread_input);
static void APP_ZIGBEE_ProcessRequestM0ToM4(ULONG thread_input);
static void APP_ZIGBEE_ProcessNwkForm(ULONG thread_input);
static void APP_ZIGBEE_ProcessPushButton(ULONG thread_input);

static void APP_ZIGBEE_TimingElapsed(void);

/* USER CODE BEGIN PFP */
static void APP_ZIGBEE_SW1_Process(void);
static void APP_ZIGBEE_ConfigGroupAddr(void);
/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
static TL_CmdPacket_t   *p_ZIGBEE_otcmdbuffer;
static TL_EvtPacket_t   *p_ZIGBEE_notif_M0_to_M4;
static TL_EvtPacket_t   *p_ZIGBEE_request_M0_to_M4;
static __IO uint32_t    CptReceiveNotifyFromM0 = 0;
static __IO uint32_t    CptReceiveRequestFromM0 = 0;

static uint8_t  TimerID;

TX_THREAD       OsTaskNotifyM0ToM4Id;
TX_THREAD       OsTaskRequestM0ToM4Id;
TX_THREAD       OsTaskNwkFormId;
TX_THREAD       OsTaskPushButtonId;
TX_MUTEX        MtxZigbeeId;

TX_SEMAPHORE    TransferToM0Semaphore;
TX_SEMAPHORE    StartupEndSemaphore;

TX_SEMAPHORE    NWKFormSemaphore;
TX_SEMAPHORE    NWKFormWaitSemaphore;
TX_SEMAPHORE    NotifyM0ToM4Semaphore;
TX_SEMAPHORE    RequestM0ToM4Semaphore;
TX_SEMAPHORE    PushButtonSemaphore;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_ZIGBEE_Config_t ZigbeeConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ZigbeeOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ZigbeeNotifRequestBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];

struct zigbee_app_info {
  bool has_init;
  struct ZigBeeT *zb;
  enum ZbStartType startupControl;
  enum ZbStatusCodeT join_status;
  uint32_t join_delay;
  bool init_after_join;

  struct ZbZclClusterT *onOff_client_1;
};
static struct zigbee_app_info zigbee_app_info;

/* USER CODE BEGIN PV */
#ifdef TX_ENABLE_EVENT_TRACE
static UCHAR	cDebugTraceX[DEBUG_TRACEX_SIZE];
#endif // TX_ENABMLE_EVENT_TRACE

#ifdef TX_LOW_POWER
typedef struct
{
  uint32_t LpTXTimeLeftOnEntry;
  uint8_t LpTXTimerThreadx_Id;
} LpTXTimerContext_t;

static LpTXTimerContext_t LpTXTimerContext;

static uint32_t     lPreviousPriMask;
static uint8_t      cLowPowerExecute = 0x00;
static uint8_t      cLowPowerLevel = 0x00;
#endif // TX_LOW_POWER

/* USER CODE END PV */
/* Functions Definition ------------------------------------------------------*/

/**
 * @brief  Zigbee application initialization
 * @param  None
 * @retval None
 */
void APP_ZIGBEE_Init(TX_BYTE_POOL* p_byte_pool)
{
  SHCI_CmdStatus_t ZigbeeInitStatus;
  CHAR* pointer = TX_NULL;
  UINT ThreadXStatus = TX_THREAD_ERROR;
  
  APP_DBG("APP_ZIGBEE_Init");

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_ZIGBEE_CheckWirelessFirmwareInfo();

#ifdef TX_ENABLE_EVENT_TRACE
  // -- Enable ThreadX Trace --
  tx_trace_enable( cDebugTraceX, sizeof(cDebugTraceX), APP_MAX_THREAD );
#endif // TX_ENABLE_EVENT_TRACE
  
  /* Register cmdbuffer */
  APP_ZIGBEE_RegisterCmdBuffer(&ZigbeeOtCmdBuffer);

  /* Init config buffer and call TL_ZIGBEE_Init */
  APP_ZIGBEE_TL_INIT();

  /* Initialize the mutex */
  tx_mutex_create(&MtxZigbeeId, "MtxZigbeeId", TX_NO_INHERIT);
  
  /* Initialize the semaphores */
  tx_semaphore_create(&StartupEndSemaphore, "StartupEndSemaphore", 0); /*< Create the semaphore and make it busy at initialization */
  tx_semaphore_create(&TransferToM0Semaphore, "TransferToM0Semaphore", 0); 
  tx_semaphore_create(&NWKFormSemaphore, "NWKFormSemaphore", 0);
  tx_semaphore_create(&NWKFormWaitSemaphore, "NWKFormWaitSemaphore", 0);
  tx_semaphore_create(&NotifyM0ToM4Semaphore, "NotifyM0ToM4Semaphore", 0);
  tx_semaphore_create(&RequestM0ToM4Semaphore, "RequestM0ToM4Semaphore", 0);
  tx_semaphore_create(&PushButtonSemaphore, "PushButtonSemaphore", 0);

  /* Create the Timer service */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &TimerID, hw_ts_SingleShot, APP_ZIGBEE_TimingElapsed);
  
  /* Register task */
  /* Create the different tasks */
  tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskNotifyM0ToM4Id,
                                   "NotifyM0ToM4Id",
                                   APP_ZIGBEE_ProcessNotifyM0ToM4,
                                   0,
                                   pointer,
                                   DEMO_STACK_SIZE_LARGE,
                                   NOTIFY_M0_TO_M4_PRIORITY,
                                   NOTIFY_M0_TO_M4_PRIORITY,
                                   TX_NO_TIME_SLICE,
                                   TX_AUTO_START);
  if (ThreadXStatus != TX_SUCCESS)
    { APP_ZIGBEE_Error(ERR_ZIGBEE_THREAD_X_FAILED,1); }
  
  tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskRequestM0ToM4Id,
                                   "RequestM0ToM4Id",
                                   APP_ZIGBEE_ProcessRequestM0ToM4,
                                   0,
                                   pointer,
                                   DEMO_STACK_SIZE_LARGE,
                                   REQUEST_M0_TO_M4_PRIORITY,
                                   REQUEST_M0_TO_M4_PRIORITY,
                                   TX_NO_TIME_SLICE,
                                   TX_AUTO_START);
  if (ThreadXStatus != TX_SUCCESS)
    { APP_ZIGBEE_Error(ERR_ZIGBEE_THREAD_X_FAILED,1); }
    
  /* Task associated with network creation process */
  tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskNwkFormId,
                                   "NwkFormId",
                                   APP_ZIGBEE_ProcessNwkForm,
                                   0,
                                   pointer,
                                   DEMO_STACK_SIZE_LARGE,
                                   NWL_FORM_PRIORITY,
                                   NWL_FORM_PRIORITY,
                                   TX_NO_TIME_SLICE,
                                   TX_AUTO_START);
  if (ThreadXStatus != TX_SUCCESS)
    { APP_ZIGBEE_Error(ERR_ZIGBEE_THREAD_X_FAILED,1); }

  /* USER CODE BEGIN APP_ZIGBEE_INIT */
  /* Task associated with push button SW1 */
  tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskPushButtonId,
                                   "PushButtonId",
                                   APP_ZIGBEE_ProcessPushButton,
                                   0,
                                   pointer,
                                   DEMO_STACK_SIZE_LARGE,
                                   PUSH_BUTTON_PRIORITY,
                                   PUSH_BUTTON_PRIORITY,
                                   TX_NO_TIME_SLICE,
                                   TX_AUTO_START);
  if (ThreadXStatus != TX_SUCCESS)
    { APP_ZIGBEE_Error(ERR_ZIGBEE_THREAD_X_FAILED,1); }
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
  tx_semaphore_put(&NWKFormSemaphore);
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
  req.deviceId = ZCL_DEVICE_ONOFF_SWITCH;
  req.endpoint = SW1_ENDPOINT;
  ZbZclAddEndpoint(zigbee_app_info.zb, &req, &conf);
  assert(conf.status == ZB_STATUS_SUCCESS);

  /* OnOff client */
  zigbee_app_info.onOff_client_1 = ZbZclOnOffClientAlloc(zigbee_app_info.zb, SW1_ENDPOINT);
  assert(zigbee_app_info.onOff_client_1 != NULL);
  ZbZclClusterEndpointRegister(zigbee_app_info.onOff_client_1);

  /* USER CODE BEGIN CONFIG_ENDPOINT */
  /* USER CODE END CONFIG_ENDPOINT */
} /* APP_ZIGBEE_ConfigEndpoints */


/**
  * @brief  Callback triggered when the Timer expire
  * @param  None
  * @retval None
  */
static void APP_ZIGBEE_TimingElapsed(void)
{
  APP_DBG("--- APP_ZIGBEE_InitWaitElapsed ---");
  tx_semaphore_put(&NWKFormWaitSemaphore);
}

/**
 * @brief  Handle Zigbee network forming and joining task for ThreadX
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_ProcessNwkForm(ULONG argument)
{
  UNUSED(argument);
  for(;;)
  {
    tx_semaphore_get(&NWKFormSemaphore, TX_WAIT_FOREVER);
    if ( zigbee_app_info.join_status != ZB_STATUS_SUCCESS )
    { 
      //tx_thread_sleep( ( APP_ZIGBEE_STARTUP_FAIL_DELAY ) ); 
      HW_TS_Start(TimerID, APP_ZIGBEE_STARTUP_FAIL_DELAY);
      tx_semaphore_get(&NWKFormWaitSemaphore, TX_WAIT_FOREVER);
    }
    APP_ZIGBEE_NwkForm();
  }
}

/**
 * @brief  Handle Zigbee network forming and joining
 * @param  None
 * @retval None
 */
static void APP_ZIGBEE_NwkForm(void)
{
  //if ((zigbee_app_info.join_status != ZB_STATUS_SUCCESS) && (HAL_GetTick() >= zigbee_app_info.join_delay))
  if (zigbee_app_info.join_status != ZB_STATUS_SUCCESS)
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
      /* USER CODE BEGIN 0 */
      zigbee_app_info.join_delay = 0U;
      zigbee_app_info.init_after_join = true;
      APP_DBG("OnOff Client init done!\n"); 
      BSP_LED_On(LED_BLUE);
      
#ifdef TX_LOW_POWER
      APP_ZIGBEE_ThreadX_LowPowerEnable( LOWPOWER_STOPMODE );
#endif // TX_LOW_POWER
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
    tx_semaphore_put(&NWKFormSemaphore);
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
  tx_semaphore_put(&StartupEndSemaphore);
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
  if (status != ZB_STATUS_SUCCESS) {
    info->active = false;
    return status;
  }
  tx_semaphore_get(&StartupEndSemaphore, TX_WAIT_FOREVER);
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
  switch (ErrId) 
  {
    case ERR_ZIGBEE_THREAD_X_FAILED:
        APP_ZIGBEE_TraceError("ERROR : ERR_ZIGBEE_THREAD_X_FAILED ", ErrCode);
        break;
    
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

} /* APP_ZIGBEE_TraceError */

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Zigbee
 *        and display associated information
 * @param  None
 * @retval None
 */
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
    for(int str_index=0; str_index < ZB_SEC_KEYSIZE; str_index++)
    {
      sprintf(&Z09_LL_string[str_index*3],"%02x ",sec_key_ha[str_index]);
    }

    APP_DBG("Link Key value: %s", Z09_LL_string);
    /* print clusters allocated */
    APP_DBG("Clusters allocated are:");  
    APP_DBG("OnOff Client on Endpoint %d", SW1_ENDPOINT);
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
  tx_mutex_get(&MtxZigbeeId, TX_WAIT_FOREVER);
} /* Pre_ZigbeeCmdProcessing */

/**
 * @brief  This function waits for getting an acknowledgment from the M0.
 *
 * @param  None
 * @retval None
 */
static void Wait_Getting_Ack_From_M0(void)
{
   tx_semaphore_get(&TransferToM0Semaphore, TX_WAIT_FOREVER);
   tx_mutex_put(&MtxZigbeeId);
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
   tx_semaphore_put(&TransferToM0Semaphore);
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
    tx_semaphore_put(&NotifyM0ToM4Semaphore);
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
    tx_semaphore_put(&RequestM0ToM4Semaphore);
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
void APP_ZIGBEE_ProcessNotifyM0ToM4( ULONG argument )
{
    UNUSED(argument);

    for ( ;;) 
    {
      tx_semaphore_get(&NotifyM0ToM4Semaphore, TX_WAIT_FOREVER);
      if (CptReceiveNotifyFromM0 != 0) 
      {
        /* If CptReceiveNotifyFromM0 is > 1. it means that we did not serve all the events from the radio */
        if (CptReceiveNotifyFromM0 > 1U) 
            { APP_ZIGBEE_Error(ERR_REC_MULTI_MSG_FROM_M0, 0); }
        else 
            { Zigbee_CallBackProcessing(); }
        
        /* Reset counter */
        CptReceiveNotifyFromM0 = 0;
      }
    }
}

/**
 * @brief Process the requests coming from the M0.
 * @param
 * @return
 */
void APP_ZIGBEE_ProcessRequestM0ToM4( ULONG argument )
{
  UNUSED(argument);
  
  for ( ;;) 
  {
    tx_semaphore_get(&RequestM0ToM4Semaphore, TX_WAIT_FOREVER);
    
    if (CptReceiveRequestFromM0 != 0)
    {
      Zigbee_M0RequestProcessing();
      CptReceiveRequestFromM0 = 0;
    }
  }
}

void APP_ZIGBEE_LaunchPushButtonTask(void)
{
#ifdef TX_ENABLE_EVENT_TRACE    
    tx_trace_user_event_insert( APP_TRACE_INFO, 1, 0, 0, 1 );
#endif // TX_ENABLE_EVENT_TRACE
  tx_semaphore_put(&PushButtonSemaphore);
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

static void APP_ZIGBEE_ProcessPushButton(ULONG argument)
{
  UNUSED(argument);
#ifndef TX_LOW_POWER
  uint32_t          lCurrentTime;
  static uint32_t   lDetectRebound = 0;
#endif // TX_LOW_POWER
  
  for(;;)
  {
     tx_semaphore_get(&PushButtonSemaphore, TX_WAIT_FOREVER);
     
#ifdef TX_LOW_POWER
     // -- During LowPower it's not possible to count Tick --
     APP_ZIGBEE_SW1_Process(); 
#else // TX_LOW_POWER     
     lCurrentTime = HAL_GetTick();
     if ( lCurrentTime < lDetectRebound )  /* restart tick should handle the PB process */
     {
       lDetectRebound = lCurrentTime;
       APP_ZIGBEE_SW1_Process(); 
     }
     else 
     {
       if ( lCurrentTime > ( lDetectRebound + PB_REBOUND_DELAY ) ) /* check if we have a rebound in the PushButton */
       {
         lDetectRebound = lCurrentTime;
         APP_ZIGBEE_SW1_Process(); 
       }
       else
        { APP_DBG("Error during Rebound : Current (%d) and Old (%d) ", lCurrentTime, lDetectRebound ) ; }
     }
#endif // TX_LOW_POWER
  }
}

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
  struct ZbApsAddrT dst;
  uint64_t epid = 0U;

  if(zigbee_app_info.zb == NULL)
    { return; }
  
  /* Check if the router joined the network */
  if (ZbNwkGet(zigbee_app_info.zb, ZB_NWK_NIB_ID_ExtendedPanId, &epid, sizeof(epid)) != ZB_STATUS_SUCCESS) 
    { return; }
  
  if (epid == 0U) 
    { return; }

  memset(&dst, 0, sizeof(dst));
  dst.mode = ZB_APSDE_ADDRMODE_GROUP;
  dst.endpoint = SW1_ENDPOINT;
  dst.nwkAddr = SW1_GROUP_ADDR;

  APP_DBG("SW1 PUSHED (SENDING TOGGLE TO GROUP 0x0001)");
  if ( ZbZclOnOffClientToggleReq(zigbee_app_info.onOff_client_1, &dst, NULL, NULL) != ZCL_STATUS_SUCCESS ) 
    { APP_DBG("Error, ZbZclOnOffClientToggleReq failed (SW1_ENDPOINT)"); }
}

/* USER CODE END FD_LOCAL_FUNCTIONS */

#ifdef TX_LOW_POWER

/**
 * @brief  Request to start a low power timer
 *
 * @param  tx_low_power_next_expiration: Number of ThreadX ticks
 * @retval None
 */
void APP_ZIGBEE_ThreadX_Low_Power_Setup(ULONG tx_low_power_next_expiration)
{
  uint64_t time;
  static uint8_t    cDebugOneTime = 0;

  // -- Execute only if LowPower enable or First time --
  if ( ( cLowPowerLevel == 0x00u ) && ( cDebugOneTime == 0x01u ) )
    { return; }
  
  if ( cDebugOneTime == 0x00u )
  {
    APP_DBG("APP_ZIGBEE_ThreadX_Low_Power_Setup : START");
    cDebugOneTime = 0x01;
  }  
  
  /* Timer was already created, here we need to start it */
  /* By default, see  tx_initialize_low_level.S, each tick is 10 ms */
  /* This function should be very similar to LpTimerStart used in freertos_port.c */
  /* Converts the number of FreeRTOS ticks into hw timer tick */
  if (tx_low_power_next_expiration > (__UNSIGNED_LONG_LONG_MAX__ / 1e12)) /* Prevent overflow in else statement */
  {
    time = 0xFFFF0000; /* Maximum value equal to 24 days */
  }
  else
  {
    /* The result always fits in uint32_t and is always less than 0xFFFF0000 */
    time = tx_low_power_next_expiration * 1000000000000ULL;
    time = (uint64_t)( time /  ( CFG_TS_TICK_VAL_PS * TX_TIMER_TICK_PER_SECOND ));
  }

  HW_TS_Start(LpTXTimerContext.LpTXTimerThreadx_Id, (uint32_t)time);

  /**
   * There might be other timers already running in the timer server that may elapse
   * before this one.
   * Store how long before the next event so that on wakeup, it will be possible to calculate
   * how long the tick has been suppressed
   */
  LpTXTimerContext.LpTXTimeLeftOnEntry = HW_TS_RTC_ReadLeftTicksToCount( );

  return;
}

/**
 * @brief  Read how long the tick has been suppressed
 *
 * @param  None
 * @retval The number of tick rate (FreeRTOS tick)
 */
unsigned long APP_ZIGBEE_ThreadX_Low_Power_Adjust_Ticks(void)
{
  uint64_t val_ticks, time_ps;
  uint32_t LpTimeLeftOnExit;
  static uint8_t    cDebugOneTime = 0;

  if ( cDebugOneTime == 0x00u )
  {
    APP_DBG("APP_ZIGBEE_ThreadX_Low_Power_Adjust_Ticks : START");
    cDebugOneTime = 0x01;
  }
  
  LpTimeLeftOnExit = HW_TS_RTC_ReadLeftTicksToCount();
  /* This cannot overflow. Max result is ~ 1.6e13 */
  time_ps = (uint64_t)((CFG_TS_TICK_VAL_PS) * (uint64_t)(LpTXTimerContext.LpTXTimeLeftOnEntry - LpTimeLeftOnExit));

  /* time_ps can be less than 1 RTOS tick in following situations
   * a) MCU didn't go to STOP2 due to wake-up unrelated to Timer Server or woke up from STOP2 very shortly after.
   *    Advancing RTOS clock by 1 ThreadX tick doesn't hurt in this case.
   * b) APP_BLE_ThreadX_Low_Power_Setup(tx_low_power_next_expiration) was called with xExpectedIdleTime = 2 which is minimum value defined by configEXPECTED_IDLE_TIME_BEFORE_SLEEP.
   *    The xExpectedIdleTime is decremented by one RTOS tick to wake-up in advance.
   *    Ex: RTOS tick is 1ms, the timer Server wakes the MCU in ~977 us. RTOS clock should be advanced by 1 ms.
   * */
  if(time_ps <= (1e12 / TX_TIMER_TICK_PER_SECOND)) /* time_ps < RTOS tick */
  {
    val_ticks = 1;
  }
  else
  {
    /* Convert pS time into OS ticks */
    val_ticks = time_ps * TX_TIMER_TICK_PER_SECOND; /* This cannot overflow. Max result is ~ 1.6e16 */
    val_ticks = (uint64_t)(val_ticks / (1e12)); /* The result always fits in uint32_t */
  }

  /**
   * The system may have been out from another reason than the timer
   * Stop the timer after the elapsed time is calculated other wise, HW_TS_RTC_ReadLeftTicksToCount()
   * may return 0xFFFF ( TIMER LIST EMPTY )
   * It does not hurt stopping a timer that exists but is not running.
   */
  HW_TS_Stop(LpTXTimerContext.LpTXTimerThreadx_Id);

  return (unsigned long)val_ticks;
}


void APP_ZIGBEE_ThreadX_LowPowerEnable( uint8_t cEnable )
{
  cLowPowerLevel = cEnable;
}


void APP_ZIGBEE_ThreadX_EnterLowPower( void )
{
  cLowPowerExecute = 0x00; 
  
  // -- Exucute only if LowPower enable --
  if ( cLowPowerLevel == LOWPOWER_NONE )
    { return; }
  
#if ( CFG_HW_USART1_ENABLED == 1 )
  if ( HW_UART_OnGoing( CFG_DEBUG_TRACE_UART ) != 0x00u )
    { return; }
#endif // ( CFG_HW_USART1_ENABLED == 1 )
#if (CFG_HW_LPUART1_ENABLED == 1)
  if ( HW_UART_OnGoing( CFG_CLI_UART ) != 0x00u )
    { return;  }
#endif // ( CFG_HW_LPUART1_ENABLED == 1 )
  
  cLowPowerExecute = 0x01;
//  BSP_LED_On(LED_RED);    // Only for Debug

  // -- Enter Critical Section --
  lPreviousPriMask = __get_PRIMASK( );
  __disable_irq();

  // -- Enter in LowPower --
  switch ( cLowPowerLevel )
  {
    case LOWPOWER_SLEEPMODE :   PWR_EnterSleepMode();
                                break;
                                
    case LOWPOWER_STOPMODE :    PWR_EnterStopMode();
                                break;
                                
    case LOWPOWER_OFFMODE :     PWR_EnterOffMode();
                                break;
  }
}


void APP_ZIGBEE_ThreadX_ExitLowPower( void )
{
  // -- Exucute only if LowPower was executed --
  if ( cLowPowerExecute != 0x00u ) 
  { 
    switch ( cLowPowerLevel )
    {
      case LOWPOWER_SLEEPMODE : PWR_ExitSleepMode();
                                break;
                                
      case LOWPOWER_STOPMODE :  PWR_ExitStopMode();
                                break;
                                
      case LOWPOWER_OFFMODE :   PWR_ExitOffMode();
                                break;
    }
    
    // -- Exit Critical Section --
    __set_PRIMASK( lPreviousPriMask );
    
//    BSP_LED_Off(LED_RED);       // Only for Debug
    
    cLowPowerExecute = 0x00;
  }
}

#endif // TX_LOW_POWER

