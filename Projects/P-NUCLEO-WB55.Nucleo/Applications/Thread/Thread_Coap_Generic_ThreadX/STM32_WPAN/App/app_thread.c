/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : App/app_thread.c
 * Description        : Thread Application.
 ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENQ file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "utilities_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "app_thread.h"
#include "stm32wbxx_core_interface_def.h"
#include "openthread_api_wb.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32_lpm.h"
#include "stm32_lpm_if.h"
#include "tx_api.h"
#if (CFG_USB_INTERFACE_ENABLE != 0)
#include "vcp.h"
#include "vcp_conf.h"
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
#ifdef ENABLE_OPENTHREAD_CLI
#include "uart.h"
#endif /* ENABLE_OPENTHREAD_CLI */

/* Private includes -----------------------------------------------------------*/


/* Private defines -----------------------------------------------------------*/
#define C_SIZE_CMD_STRING       256U
#define C_PANID                 0x1111U
//#define C_CHANNEL_NB            17U     // To work with OpenThread + ThreadX
#define C_CHANNEL_NB            14U       // To work with OpenTread Only

/* USER CODE BEGIN PD */
#define C_RESSOURCE             "light"
#define COAP_PAYLOAD_LENGTH     2
#define WAIT_TIMEOUT            ( 5u * 1000u * 1000u / CFG_TS_TICK_VAL) /**< 5s */
#define PB_REBOUND_DELAY        250


#ifdef TX_ENABLE_EVENT_TRACE
#define APP_MAX_THREAD			14u
#define DEBUG_TRACEX_SIZE		(64u * 512u )
#define APP_TRACE_INFO          (TX_TRACE_USER_EVENT_START + 1)
#endif // TX_ENABLE_EVENT_TRACE
/* USER CODE END PD */



/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
static void APP_THREAD_CheckWirelessFirmwareInfo(void);
static void APP_THREAD_DeviceConfig(void);
static void APP_THREAD_StateNotif(uint32_t NotifFlags, void *pContext);
#if (CFG_FULL_LOW_POWER == 0)
static void Send_CLI_To_M0_Task(ULONG argument);
static void Send_CLI_To_M0(void);
#endif /* (CFG_FULL_LOW_POWER == 0) */
static void Send_CLI_Ack_For_OT(void);
static void HostTxCb( void );
static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
static void APP_THREAD_ProcessCoapNotConfirmable(ULONG argument);
static void APP_THREAD_ProcessCoapConfirmable(ULONG argument);

#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif
#if (CFG_USB_INTERFACE_ENABLE != 0)
static uint32_t ProcessCmdString(uint8_t* buf , uint32_t len);
#else
#if (CFG_FULL_LOW_POWER == 0)
static void RxCpltCallback(void);
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
#ifdef ENABLE_OPENTHREAD_CLI
extern void otAppCliInit(otInstance *aInstance);
#endif /* ENABLE_OPENTHREAD_CLI */

/* USER CODE BEGIN PFP */
static void APP_THREAD_CoapSendRequest( otCoapResource          * aCoapRessource,
                                        otCoapType              aCoapType,
                                        otCoapCode              aCoapCode,
                                        const char              * aStringAddress,
                                        const otIp6Address      * aPeerAddress,
                                        uint8_t                 * aPayload,
                                        uint16_t                Size,
                                        otCoapResponseHandler   aHandler,
                                        void                    * aContext);

static void APP_THREAD_CoapRequestHandler(void                * pContext,
                                          otMessage           * pMessage,
                                          const otMessageInfo * pMessageInfo);

static void APP_THREAD_CoapSendDataResponse(otMessage * pMessage, const otMessageInfo * pMessageInfo);

static void APP_THREAD_CoapDataRespHandler( void                * aContext,
                                            otMessage           * pMessage,
                                            const otMessageInfo * pMessageInfo,
                                            otError             result);

static void APP_THREAD_InitPayloadWrite(void);
static void APP_THREAD_SendCoapNonConf(void);
static void APP_THREAD_SendCoapConf(void);
static bool APP_THREAD_CheckMsgValidity(void);
/* USER CODE END PFP */

/* Private variables -----------------------------------------------*/
#if (CFG_USB_INTERFACE_ENABLE != 0)
static uint8_t TmpString[C_SIZE_CMD_STRING];
static uint8_t VcpRxBuffer[sizeof(TL_CmdSerial_t)];        /* Received Data over USB are stored in this buffer */
static uint8_t VcpTxBuffer[sizeof(TL_EvtPacket_t) + 254U]; /* Transmit buffer over USB */
#else
#if (CFG_FULL_LOW_POWER == 0)
static uint8_t aRxBuffer[C_SIZE_CMD_STRING];
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_FULL_LOW_POWER == 0)
static uint8_t CommandString[C_SIZE_CMD_STRING];
#endif /* (CFG_FULL_LOW_POWER == 0) */
static __IO uint16_t indexReceiveChar = 0;
static __IO uint16_t CptReceiveCmdFromUser = 0;

static TL_CmdPacket_t *p_thread_otcmdbuffer;
static TL_EvtPacket_t *p_thread_notif_M0_to_M4;
static __IO uint32_t  CptReceiveMsgFromM0 = 0;

TX_THREAD OsTaskMessageM0ToM4Id;
TX_THREAD OsTaskSendCliToM0;
TX_THREAD OsTaskCoapNotConfirmable;
TX_THREAD OsTaskCoapConfirmable;

TX_MUTEX MtxThreadId;

TX_SEMAPHORE TransferToM0Semaphore;
TX_SEMAPHORE MessageM0ToM4Semaphore;
TX_SEMAPHORE CoapNonConfSemaphore;
TX_SEMAPHORE CoapConfSemaphore;
TX_SEMAPHORE SendCliCmdSemaphore;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_TH_Config_t ThreadConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ThreadNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadCliCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadCliNotBuffer;
extern uint8_t g_ot_notification_allowed;

/* USER CODE BEGIN PV */
static otCoapResource OT_Ressource = {C_RESSOURCE, APP_THREAD_CoapRequestHandler,"MyOwnContext", NULL};
static otMessageInfo OT_MessageInfo = {0};
static otMessage* pOT_Message = NULL;
static otMessage* pOT_MessageResponse = NULL;

static uint8_t PayloadWrite[COAP_PAYLOAD_LENGTH]= {0};
static uint8_t PayloadRead[COAP_PAYLOAD_LENGTH]= {0};


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
  * @brief  Thread initialization.
  * @param  p_byte_pool : Memory pool
  * @retval None
  */

void APP_THREAD_Init(TX_BYTE_POOL* p_byte_pool)
{
  UINT ThreadXStatus;
  SHCI_CmdStatus_t ThreadInitStatus;
  CHAR* pointer = TX_NULL;

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_THREAD_CheckWirelessFirmwareInfo();

#if (CFG_USB_INTERFACE_ENABLE != 0)
  VCP_Init(&VcpTxBuffer[0], &VcpRxBuffer[0]);
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#ifdef TX_ENABLE_EVENT_TRACE
  // -- Enable ThreadX Trace --
  tx_trace_enable( cDebugTraceX, sizeof(cDebugTraceX), APP_MAX_THREAD );
#endif // TX_ENABLE_EVENT_TRACE
    
  /* Register cmdbuffer */
  APP_THREAD_RegisterCmdBuffer(&ThreadOtCmdBuffer);

  /**
   * Do not allow standby in the application
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_THREAD, UTIL_LPM_DISABLE);

  /* Init config buffer and call TL_THREAD_Init */
  APP_THREAD_TL_THREAD_INIT();

  /* Initialize the mutex */
  tx_mutex_create(&MtxThreadId, "MtxThreadId", TX_NO_INHERIT);

  /* Initialize the semaphores */
  tx_semaphore_create(&TransferToM0Semaphore, "TransferToM0Semaphore", 0);
  tx_semaphore_create(&MessageM0ToM4Semaphore, "MessageM0ToM4Semaphore", 0);
  tx_semaphore_create(&CoapNonConfSemaphore, "CoapNonConfSemaphore", 0);
  tx_semaphore_create(&CoapConfSemaphore, "CoapConfSemaphore", 0);

  /* Create the different tasks */
  /* Task to manage the messages from the M0 to the M4 */
  ThreadXStatus = tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskMessageM0ToM4Id,
                                   "MessageM0ToM4",
                                   APP_THREAD_ProcessMsgM0ToM4,
                                   0,
                                   pointer,
                                   DEMO_STACK_SIZE_LARGE,
                                   MESSAGE_M0_TO_M4_PRIORITY,
                                   MESSAGE_M0_TO_M4_PRIORITY,
                                   TX_NO_TIME_SLICE,
                                   TX_AUTO_START);
  if (ThreadXStatus != TX_SUCCESS)
    { APP_THREAD_Error(ERR_THREAD_THREAD_X_FAILED, 3); }

  
  /* Task to manage the task used to send a COAP confirmable request */
  tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskCoapConfirmable,
                                    "CoapConfirmable",
                                    APP_THREAD_ProcessCoapConfirmable,
                                    0,
                                    pointer,
                                    DEMO_STACK_SIZE_LARGE,
                                    COAP_CONFIRMABLE_PRIORITY,
                                    COAP_CONFIRMABLE_PRIORITY,
                                    TX_NO_TIME_SLICE,
                                    TX_AUTO_START);

  if (ThreadXStatus != TX_SUCCESS)
    { APP_THREAD_Error(ERR_THREAD_THREAD_X_FAILED, 4); }

  
  /* Task to manage the task used to send a COAP non- confirmable request */
  tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskCoapNotConfirmable,
                                    "CoapNotConfirmable",
                                    APP_THREAD_ProcessCoapNotConfirmable,
                                    0,
                                    pointer,
                                    DEMO_STACK_SIZE_LARGE,
                                    COAP_NON_CONFIRMABLE_PRIORITY,
                                    COAP_NON_CONFIRMABLE_PRIORITY,
                                    TX_NO_TIME_SLICE,
                                    TX_AUTO_START);
  
  if ( ThreadXStatus != TX_SUCCESS )
    { APP_THREAD_Error(ERR_THREAD_THREAD_X_FAILED,5); }


  /* Configure UART for sending CLI command from M4 */
  APP_THREAD_Init_UART_CLI(p_byte_pool);

  /* Send Thread start system cmd to M0 */
  ThreadInitStatus = SHCI_C2_THREAD_Init();

  /* Prevent unused argument(s) compilation warning */
  UNUSED(ThreadInitStatus);

  /* Initialize and configure the Thread device*/
  APP_THREAD_DeviceConfig();
  
  APP_THREAD_InitPayloadWrite();
}

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */
void APP_THREAD_Error(uint32_t ErrId, uint32_t ErrCode)
{
  /* USER CODE BEGIN APP_THREAD_Error_1 */

  /* USER CODE END APP_THREAD_Error_1 */
  switch(ErrId)
  {
  	case ERR_REC_MULTI_MSG_FROM_M0 :
  		APP_THREAD_TraceError("ERROR : ERR_REC_MULTI_MSG_FROM_M0 ", ErrCode);
  		break;

  	case ERR_THREAD_SET_STATE_CB :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_STATE_CB ",ErrCode);
  		break;

  	case ERR_THREAD_SET_CHANNEL :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_CHANNEL ",ErrCode);
  		break;

  	case ERR_THREAD_SET_PANID :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_PANID ",ErrCode);
  		break;

  	case ERR_THREAD_IPV6_ENABLE :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_IPV6_ENABLE ",ErrCode);
  		break;

  	case ERR_THREAD_START :
  		APP_THREAD_TraceError("ERROR: ERR_THREAD_START ", ErrCode);
  		break;

  	case ERR_THREAD_ERASE_PERSISTENT_INFO :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_ERASE_PERSISTENT_INFO ",ErrCode);
  		break;

  	case ERR_THREAD_SET_NETWORK_KEY :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_NETWORK_KEY ",ErrCode);
  		break;

  	case ERR_THREAD_CHECK_WIRELESS :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_CHECK_WIRELESS ",ErrCode);
  		break;

  	/* USER CODE BEGIN APP_THREAD_Error_2 */
  	case ERR_THREAD_COAP_START :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_START ",ErrCode);
  		break;

  	case ERR_THREAD_COAP_ADD_RESSOURCE :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_ADD_RESSOURCE ",ErrCode);
  		break;

  	case ERR_THREAD_MESSAGE_READ :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_MESSAGE_READ ",ErrCode);
  		break;

  	case ERR_THREAD_COAP_SEND_RESPONSE :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_SEND_RESPONSE ",ErrCode);
  		break;

  	case ERR_THREAD_COAP_APPEND :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_APPEND ",ErrCode);
  		break;

  	case ERR_THREAD_COAP_SEND_REQUEST :
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_SEND_REQUEST ",ErrCode);
  		break;

  	case ERR_THREAD_MSG_COMPARE_FAILED:
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_MSG_COMPARE_FAILED ",ErrCode);
  		break;

  	case ERR_THREAD_THREAD_X_FAILED:
  		APP_THREAD_TraceError("ERROR : ERR_THREAD_THREAD_X_FAILED ",ErrCode);
  		break;

  	/* USER CODE END APP_THREAD_Error_2 */
  	default :
  		APP_THREAD_TraceError("ERROR Unknown ", 0);
    	break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * @brief Thread initialization.
 * @param  None
 * @retval None
 */
static void APP_THREAD_DeviceConfig(void)
{
  otError error;
  otNetworkKey networkKey = {{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}};

#ifdef ENABLE_OPENTHREAD_CLI
  static otInstance *PtOpenThreadInstance;
  otInstanceFinalize(NULL);
  PtOpenThreadInstance = otInstanceInitSingle();
  otAppCliInit(PtOpenThreadInstance);
#endif /* ENABLE_OPENTHREAD_CLI */
  
  error = otInstanceErasePersistentInfo(NULL);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_ERASE_PERSISTENT_INFO,error);
  }
  
  error = otSetStateChangedCallback(NULL, APP_THREAD_StateNotif, NULL);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_STATE_CB,error);
  }
  error = otLinkSetChannel(NULL, C_CHANNEL_NB);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_CHANNEL,error);
  }
  error = otLinkSetPanId(NULL, C_PANID);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_PANID,error);
  }
  error = otThreadSetNetworkKey(NULL, &networkKey);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_NETWORK_KEY,error);
  }
  error = otIp6SetEnabled(NULL, true);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_IPV6_ENABLE,error);
  }
  error = otThreadSetEnabled(NULL, true);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_START,error);
  }

  /* USER CODE BEGIN DEVICECONFIG */
  /* Start the COAP server */
  error = otCoapStart(NULL, OT_DEFAULT_COAP_PORT);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_COAP_START,error);
  }
  /* Add COAP resources */
  otCoapAddResource(NULL, &OT_Ressource);

  /* USER CODE END DEVICECONFIG */
}

/**
 * @brief Thread notification when the state changes.
 * @param  aFlags  : Define the item that has been modified
 *         aContext: Context
 *
 * @retval None
 */
static void APP_THREAD_StateNotif(uint32_t NotifFlags, void *pContext)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pContext);

  /* USER CODE BEGIN APP_THREAD_STATENOTIF */
  
  /* USER CODE END APP_THREAD_STATENOTIF */

  if ((NotifFlags & (uint32_t)OT_CHANGED_THREAD_ROLE) == (uint32_t)OT_CHANGED_THREAD_ROLE)
  {
    switch (otThreadGetDeviceRole(NULL))
    {
      case OT_DEVICE_ROLE_DISABLED:
        /* USER CODE BEGIN OT_DEVICE_ROLE_DISABLED */
        BSP_LED_Off(LED2);
        BSP_LED_Off(LED3);
        /* USER CODE END OT_DEVICE_ROLE_DISABLED */
        break;
      
      case OT_DEVICE_ROLE_DETACHED:
        /* USER CODE BEGIN OT_DEVICE_ROLE_DETACHED */
        BSP_LED_Off(LED2);
        BSP_LED_Off(LED3);
        /* USER CODE END OT_DEVICE_ROLE_DETACHED */
        break;
      
      case OT_DEVICE_ROLE_CHILD:
        /* USER CODE BEGIN OT_DEVICE_ROLE_CHILD */
        BSP_LED_Off(LED2);
        BSP_LED_On(LED3);
#ifdef TX_LOW_POWER
        APP_THREAD_ThreadX_LowPowerEnable( LOWPOWER_STOPMODE );
#endif // TX_LOW_POWER      
        /* USER CODE END OT_DEVICE_ROLE_CHILD */
        break;
      
      case OT_DEVICE_ROLE_ROUTER :
        /* USER CODE BEGIN OT_DEVICE_ROLE_ROUTER */
        BSP_LED_Off(LED2);
        BSP_LED_On(LED3);
#ifdef TX_LOW_POWER      
        APP_THREAD_ThreadX_LowPowerEnable( LOWPOWER_STOPMODE );
#endif // TX_LOW_POWER      
      /* USER CODE END OT_DEVICE_ROLE_ROUTER */
      break;
      
      case OT_DEVICE_ROLE_LEADER :
        /* USER CODE BEGIN OT_DEVICE_ROLE_LEADER */
        BSP_LED_On(LED2);
        BSP_LED_Off(LED3);
#ifdef TX_LOW_POWER
        APP_THREAD_ThreadX_LowPowerEnable( LOWPOWER_STOPMODE );
#endif // TX_LOW_POWER        
        /* USER CODE END OT_DEVICE_ROLE_LEADER */
        break;
        
      default:
        /* USER CODE BEGIN DEFAULT */
        BSP_LED_Off(LED2);
        BSP_LED_Off(LED3);
        /* USER CODE END DEFAULT */
        break;
    }
  }
}

/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (OpenThread or other module if any)
  * @retval None
  */
void APP_THREAD_TraceError(const char * pMess, uint32_t ErrCode)
{
  /* USER CODE BEGIN TRACE_ERROR */
  APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
  while(1U == 1U)
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
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Thread
 *        and display associated information
 * @param  None
 * @retval None
 */
static void APP_THREAD_CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t 	wireless_info_instance;
  WirelessFwInfo_t	* p_wireless_info = &wireless_info_instance;
  SHCI_CmdStatus_t	eShciComdStatus;

  eShciComdStatus = SHCI_GetWirelessFwInfo(p_wireless_info);

  APP_DBG("**********************************************************");
  APP_DBG("WIRELESS COPROCESSOR FW:");

  /* Print version */
  APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

  switch(p_wireless_info->StackType)
  {
  	case INFO_STACK_TYPE_THREAD_FTD :
  		APP_DBG("FW Type : Thread FTD");
  		break;

    case INFO_STACK_TYPE_THREAD_MTD :
    	APP_DBG("FW Type : Thread MTD");
    	break;

    case INFO_STACK_TYPE_BLE_THREAD_FTD_STATIC :
    	APP_DBG("FW Type : Static Concurrent Mode BLE/Thread");
    	break;

    case INFO_STACK_TYPE_ZIGBEE_FFD :
    	APP_DBG("FW Type : Zigbee FFD");
    	break;

    case INFO_STACK_TYPE_ZIGBEE_RFD :
    	APP_DBG("FW Type : Zigbee RFD");
    	break;

    default :
    	/* No Thread device supported ! */
    	APP_DBG("FW Type : Unsupported");
    	eShciComdStatus = SHCI_ERR_UNSUPPORTED_FEATURE;
    	break;
  }

  if ( eShciComdStatus != SHCI_Success )
  	  { APP_THREAD_Error((uint32_t)ERR_THREAD_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL); }
  else
  	  { APP_DBG("**********************************************************"); }
}


void APP_THREAD_SW1_Task(void)
{
    // -- Launch a ThreadX after a interrupt due to Key Pressed --
#ifdef TX_ENABLE_EVENT_TRACE
    tx_trace_user_event_insert( APP_TRACE_INFO, 3, 0, 0, 1 );
#endif // TX_ENABLE_EVENT_TRACE
    (void)tx_semaphore_ceiling_put(&CoapNonConfSemaphore,1);
}

void APP_THREAD_SW2_Task(void)
{
    // -- Launch a ThreadX after a interrupt due to Key Pressed --
#ifdef TX_ENABLE_EVENT_TRACE
    tx_trace_user_event_insert( APP_TRACE_INFO, 3, 0, 0, 1 );
#endif // TX_ENABLE_EVENT_TRACE
    (void)tx_semaphore_ceiling_put(&CoapConfSemaphore,1);
}


static void APP_THREAD_ProcessCoapNotConfirmable(ULONG argument)
{
    UNUSED(argument);
  
    // -- Start a ThreadX for send 'SendCoapNonConf' when a Key is pressed --
#ifdef TX_ENABLE_EVENT_TRACE    
    tx_trace_user_event_insert( APP_TRACE_INFO, 1, 0, 0, 0 );
#endif // TX_ENABLE_EVENT_TRACE    
  
    for(;;)
    {
        // -- Wait Key pressed --    
        (void)tx_semaphore_get(&CoapNonConfSemaphore, TX_WAIT_FOREVER);
#ifdef TX_ENABLE_EVENT_TRACE        
        tx_trace_user_event_insert( APP_TRACE_INFO, 1, 0, 0, 1 );
#endif // TX_ENABLE_EVENT_TRACE        
     
        // -- Send a Frame, via M0 --
        APP_THREAD_SendCoapNonConf();

    }
}

static void APP_THREAD_ProcessCoapConfirmable(ULONG argument)
{
    UNUSED(argument);
      
    // -- Start a ThreadX for send 'SendCoapConf' if a delay after Key pressed occurs --
#ifdef TX_ENABLE_EVENT_TRACE    
    tx_trace_user_event_insert( APP_TRACE_INFO, 2, 0, 0, 0 );
#endif // TX_ENABLE_EVENT_TRACE    
  
    for(;;)
    {
        // -- Wait End of TimeOut after a Key pressed --  
        (void)tx_semaphore_get(&CoapConfSemaphore, TX_WAIT_FOREVER);
#ifdef TX_ENABLE_EVENT_TRACE        
        tx_trace_user_event_insert( APP_TRACE_INFO, 2, 0, 0, 1 );
#endif // TX_ENABLE_EVENT_TRACE        
     
        // -- Send a Frame, via M0 --
        APP_THREAD_SendCoapConf();
    }
}


#ifdef TX_LOW_POWER

/**
 * @brief  Request to start a low power timer
 *
 * @param  tx_low_power_next_expiration: Number of ThreadX ticks
 * @retval None
 */
void APP_THREAD_ThreadX_Low_Power_Setup(ULONG tx_low_power_next_expiration)
{
  uint64_t time;
  static uint8_t    cDebugOneTime = 0;

  // -- Execute only if LowPower enable or First time --
  if ( ( cLowPowerLevel == LOWPOWER_NONE ) && ( cDebugOneTime == 0x01u ) )
    { return; }
  
  if ( cDebugOneTime == 0x00u )
  {
    APP_DBG("APP_THREAD_ThreadX_Low_Power_Setup : START");
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
unsigned long APP_THREAD_ThreadX_Low_Power_Adjust_Ticks(void)
{
  uint64_t val_ticks, time_ps;
  uint32_t LpTimeLeftOnExit;
  static uint8_t    cDebugOneTime = 0;

  if ( cDebugOneTime == 0x00u )
  {
    APP_DBG("APP_THREAD_ThreadX_Low_Power_Adjust_Ticks : START");
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


void APP_THREAD_ThreadX_LowPowerEnable( uint8_t cEnable )
{
	cLowPowerLevel = cEnable;
}


void APP_THREAD_ThreadX_EnterLowPower( void )
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


void APP_THREAD_ThreadX_ExitLowPower( void )
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
//#endif // (CFG_LPM_SUPPORTED == 1)


/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
/**
 * @brief Send a CoAP request with defined parameters.
 *
 * @param[in]  aCoapRessource   A pointer to a otCoapResource.
 * @param[in]  aCoapType        otCoapType.
 * @param[in]  aCoapCode        otCoapCode.
 * @param[in]  aStringAddress   A pointer to a NULL-terminated string representing the address. Example: "FF03::1" for Multicast.
 * @param[in]  aPeerAddress     A pointer to otIp6Address Peer Address.
 * @param[in]  aPayload         A pointer to payload.
 * @param[in]  aHandler         A pointer to CoAP response handler.
 * @param[in]  aContext         A pointer to application specific context.
 *
 * @retval none.
 */
static void APP_THREAD_CoapSendRequest( otCoapResource      * aCoapRessource,
                                        otCoapType          aCoapType,
                                        otCoapCode          aCoapCode,
                                        const char          * aStringAddress,
                                        const otIp6Address  * aPeerAddress,
                                        uint8_t             * aPayload,
                                        uint16_t            Size,
                                        otCoapResponseHandler aHandler,
                                        void                * aContext)
{
  otError error = OT_ERROR_NONE;

  do
  {
    pOT_Message = otCoapNewMessage(NULL, NULL);
    if (pOT_Message == NULL)
    {
      APP_THREAD_Error(ERR_THREAD_COAP_NEW_MSG,error);
      break;
    }

    otCoapMessageInit(pOT_Message, aCoapType, aCoapCode);
(void)    otCoapMessageAppendUriPathOptions(pOT_Message, aCoapRessource->mUriPath);
(void)    otCoapMessageSetPayloadMarker(pOT_Message);

    if ( ( aPayload != NULL ) && ( Size > 0 ) )
    {
      error = otMessageAppend(pOT_Message, aPayload, Size);
      if (error != OT_ERROR_NONE)
      {
        APP_THREAD_Error(ERR_THREAD_COAP_APPEND,error);
        break;
      }
    }
    else
    {
      APP_DBG("APP_THREAD_CoapSendRequest: No payload passed");
    }

    memset(&OT_MessageInfo, 0, sizeof(OT_MessageInfo));
    OT_MessageInfo.mPeerPort = OT_DEFAULT_COAP_PORT;

    if ( ( aPeerAddress == NULL ) && ( aStringAddress != NULL ) )
    {
      APP_DBG("Use String Address : %s ", aStringAddress);
(void)      otIp6AddressFromString(aStringAddress, &OT_MessageInfo.mPeerAddr);
    }
    else
    {
      if ( aPeerAddress != NULL )
      {
        APP_DBG("Use Peer Address");
        memcpy(&OT_MessageInfo.mPeerAddr, aPeerAddress, sizeof(OT_MessageInfo.mPeerAddr));
      }
      else
      {
        APP_DBG("ERROR: Address string and Peer Address not defined");
        APP_THREAD_Error(ERR_THREAD_COAP_ADDRESS_NOT_DEFINED, 0);
      }
    }

    if ( aCoapType == OT_COAP_TYPE_NON_CONFIRMABLE )
    {
      APP_DBG("aCoapType == OT_COAP_TYPE_NON_CONFIRMABLE");
      error = otCoapSendRequest(NULL,
          pOT_Message,
          &OT_MessageInfo,
          NULL,
          NULL);
    }
    
    if ( aCoapType == OT_COAP_TYPE_CONFIRMABLE )
    {
      APP_DBG("aCoapType == OT_COAP_TYPE_CONFIRMABLE");
      error = otCoapSendRequest(NULL,
          pOT_Message,
          &OT_MessageInfo,
          aHandler,
          aContext);
    }
  }
  while(false);
  
  if (error != OT_ERROR_NONE && pOT_Message != NULL)
  {
    otMessageFree(pOT_Message);
    APP_THREAD_Error(ERR_THREAD_COAP_SEND_REQUEST,error);
  }
}

/**
 * @brief Handler called when the server receives a COAP request.
 *
 * @param pContext : Context
 * @param pHeader : Header
 * @param pMessage : Message
 * @param pMessageInfo : Message information
 * @retval None
 */
static void APP_THREAD_CoapRequestHandler(void                 * pContext,
                                          otMessage            * pMessage,
                                          const otMessageInfo  * pMessageInfo)

{
  APP_DBG(" Received CoAP request (context = %s)",pContext);
  /* USER CODE BEGIN APP_THREAD_CoapRequestHandler */
  if (otMessageRead(pMessage, otMessageGetOffset(pMessage), &PayloadRead, sizeof(PayloadRead)) != sizeof(PayloadRead))
  {
    APP_THREAD_Error(ERR_THREAD_MESSAGE_READ, 0);
  }

  if (APP_THREAD_CheckMsgValidity() == true){
    BSP_LED_Toggle(LED1);
  }
  /* USER CODE END APP_THREAD_CoapRequestHandler */

  /* If Message is Confirmable, send response */
  if (otCoapMessageGetType(pMessage) == OT_COAP_TYPE_CONFIRMABLE)
  {
    APP_THREAD_CoapSendDataResponse(pMessage, pMessageInfo);
  }
}

/**
 * @brief This function acknowledges the data reception by sending an ACK
 *    back to the sender.
 * @param  pMessage coap message
 * @param  pMessageInfo message info pointer
 * @retval None
 */
static void APP_THREAD_CoapSendDataResponse(otMessage  * pMessage, const otMessageInfo * pMessageInfo)
{
  otError  error = OT_ERROR_NONE;

  do{
    APP_DBG("APP_THREAD_CoapSendDataResponse");

    pOT_MessageResponse = otCoapNewMessage(NULL, NULL);
    if (pOT_MessageResponse == NULL)
    {
      APP_DBG("WARNING : pOT_MessageResponse = NULL ! -> exit now");
      break;
    }

(void)    otCoapMessageInitResponse(pOT_MessageResponse,
        pMessage,
        OT_COAP_TYPE_ACKNOWLEDGMENT,
        OT_COAP_CODE_VALID);

    error = otCoapSendResponse(NULL, pOT_MessageResponse, pMessageInfo);
    if (error != OT_ERROR_NONE && pOT_MessageResponse != NULL)
    {
      otMessageFree(pOT_MessageResponse);
      APP_THREAD_Error(ERR_THREAD_COAP_DATA_RESPONSE,error);
    }
  }while(false);
}

/**
 * @brief This function manages the data response handler.
 *
 * @param pHeader  context
 * @param pMessage message pointer
 * @param pMessageInfo message info pointer
 * @param result error code
 * @retval None
 */
static void APP_THREAD_CoapDataRespHandler( void                * pContext,
                                            otMessage           * pMessage,
                                            const otMessageInfo * pMessageInfo,
                                            otError               result)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pMessage);
  UNUSED(pMessageInfo);

  if (result == OT_ERROR_NONE)
  {
    APP_DBG("APP_THREAD_CoapDataRespHandler : NO ERROR");
  }
  else
  {
    APP_DBG("APP_THREAD_CoapDataRespHandler : WARNING Result = %d",result);
  }
}

/**
 * @brief Initialize CoAP write buffer.
 * @param  None
 * @retval None
 */
static void APP_THREAD_InitPayloadWrite(void)
{
  uint8_t i;
  for(i = 0; i < COAP_PAYLOAD_LENGTH; i++)
  {
    PayloadWrite[i] = 0xFF;
  }
}

/**
 * @brief Task associated to the push button.
 * @param  None
 * @retval None
 */
static void APP_THREAD_SendCoapNonConf(void)
{
  APP_DBG("Send a CoAP NON-CONFIRMABLE PUT Request");
  /* Send a NON-CONFIRMABLE PUT Request */
  APP_THREAD_CoapSendRequest(&OT_Ressource,
                              OT_COAP_TYPE_NON_CONFIRMABLE,
                              OT_COAP_CODE_PUT,
                              MULICAST_FTD_MED,
                              NULL,
                              PayloadWrite,
                              sizeof(PayloadWrite),
                              NULL,
                              NULL);
}

/**
 * @brief Task associated to the push button.
 * @param  None
 * @retval None
 */
static void APP_THREAD_SendCoapConf(void)
{

  APP_DBG("Send a CoAP CONFIRMABLE PUT Request");
  /* Send a CONFIRMABLE PUT Request */
  APP_THREAD_CoapSendRequest(&OT_Ressource,
                              OT_COAP_TYPE_CONFIRMABLE,
                              OT_COAP_CODE_PUT,
                              MULICAST_FTD_MED,
                              NULL,
                              PayloadWrite,
                              sizeof(PayloadWrite),
                              APP_THREAD_CoapDataRespHandler,
                              NULL);
}

/**
 * @brief  Compare the message received versus the original message.
 * @param  None
 * @retval None
 */
static bool APP_THREAD_CheckMsgValidity(void)
{
  bool valid = true;
  uint32_t i;

  for(i = 0; i < COAP_PAYLOAD_LENGTH; i++)
  {
    if(PayloadRead[i] != PayloadWrite[i])
    {
      valid = false;
    }
  }

  if(valid == true)
  {
    APP_DBG("PAYLOAD Comparison OK!");
  }else
  {
    APP_THREAD_Error(ERR_THREAD_MSG_COMPARE_FAILED, 0);
  }
  return valid;
}
/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_THREAD_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer)
{
  p_thread_otcmdbuffer = p_buffer;
}

Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)p_thread_otcmdbuffer->cmdserial.cmd.payload;
}

Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdRspPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)((TL_EvtPacket_t *)p_thread_otcmdbuffer)->evtserial.evt.payload;
}

Thread_OT_Cmd_Request_t* THREAD_Get_NotificationPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)(p_thread_notif_M0_to_M4)->evtserial.evt.payload;
}

static void Ot_Cmd_Transfer_Common(void)
{

  /* OpenThread OT command cmdcode range 0x280 .. 0x3DF = 352 */
  p_thread_otcmdbuffer->cmdserial.cmd.cmdcode = 0x280U;
  /* Size = otCmdBuffer->Size (Number of OT cmd arguments : 1 arg = 32bits so multiply by 4 to get size in bytes)
   * + ID (4 bytes) + Size (4 bytes) */
  uint32_t l_size = ((Thread_OT_Cmd_Request_t*)(p_thread_otcmdbuffer->cmdserial.cmd.payload))->Size * 4U + 8U;
  p_thread_otcmdbuffer->cmdserial.cmd.plen = l_size;

  TL_OT_SendCmd();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_M0();

}

/**
 * @brief  This function is used to transfer the Ot commands from the
 *         M4 to the M0.
 *
 * @param   None
 * @return  None
 */
void Ot_Cmd_Transfer(void)
{
  Ot_Cmd_Transfer_Common();
}

/**
 * @brief  This function is used to transfer the Ot commands from the
 *         M4 to the M0 with Notification M0 to M4 allowed.
 *
 * @param   None
 * @return  None
 */
void Ot_Cmd_TransferWithNotif(void)
{
  /* Flag to specify to UTIL_SEQ_EvtIdle that M0 to M4 notifications are allowed */
  g_ot_notification_allowed = 1U;

  Ot_Cmd_Transfer_Common();
}

/**
 * @brief  This function is called when acknowledge from OT command is received from the M0+.
 *
 * @param   Otbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_OT_CmdEvtReceived( TL_EvtPacket_t * Otbuffer )
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Otbuffer);

  Receive_Ack_From_M0();

  /* Does not allow OpenThread M0 to M4 notification */
  g_ot_notification_allowed = 0U;
}

/**
 * @brief  This function is called when notification from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_THREAD_NotReceived( TL_EvtPacket_t * Notbuffer )
{
  p_thread_notif_M0_to_M4 = Notbuffer;

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
void Pre_OtCmdProcessing(void)
{
  /* OT command not be sent in interrupt context */
  if (__get_IPSR() != 0U)
  {
    /* 'assert' endless loop */
    for( ; ; );
  }
  
  tx_mutex_get(&MtxThreadId, TX_WAIT_FOREVER);
}

/**
  * @brief  This function is called at the end of any ot commands sent to the M0
  *         core. It is the counterpart of Pre_OtCmdProcessing() function, 
  *         unlocking sending of new ot commands.
  * @param  None
  * @retval None
  */
void Post_OtCmdProcessing(void)
{
   tx_mutex_put(&MtxThreadId);
}

/**
  * @brief  This function waits for getting an acknowledgment from the M0.
  *
  * @param  None
  * @retval None
  */
static void Wait_Getting_Ack_From_M0(void)
{
  tx_semaphore_get(&TransferToM0Semaphore, TX_WAIT_FOREVER);
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
  tx_semaphore_put(&TransferToM0Semaphore);
}

/**
  * @brief  Receive a notification from the M0+ through the IPCC.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Notification_From_M0(void)
{
  CptReceiveMsgFromM0++;
  tx_semaphore_put(&MessageM0ToM4Semaphore);
}

#if (CFG_USB_INTERFACE_ENABLE == 0)
#if (CFG_FULL_LOW_POWER == 0)

static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    CommandString[indexReceiveChar++] = aRxBuffer[0];
    if ( aRxBuffer[0] == '\r' )
    {
      CptReceiveCmdFromUser = 1U;
      tx_semaphore_put(&SendCliCmdSemaphore);
    }
  }

  /* Once a character has been sent, put back the device in reception mode */
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1U, RxCpltCallback);
}

#endif /* (CFG_FULL_LOW_POWER == 0) */
#else /* (CFG_USB_INTERFACE_ENABLE == 0) */

/**
 * @brief Process the command strings.
 *        As soon as a complete command string has been received, the task
 *        in charge of sending the command to the M0 is scheduled
 * @param  None
 * @retval None
 */
static uint32_t  ProcessCmdString( uint8_t * buf , uint32_t len )
{
  uint32_t i,j,tmp_start;
  tmp_start = 0;
  uint32_t res = 0;

  i= 0;
  while ((buf[i] != '\r') && (i < len))
  {
    i++;
  }

  if (i != len)
  {
    memcpy(CommandString, buf,(i+1));
    indexReceiveChar = i + 1U; /* Length of the buffer containing the command string */
    //UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_CLI_TO_M0, CFG_SCH_PRIO_0);
    tmp_start = i;
    for (j = 0; j < (len - tmp_start - 1U) ; j++)
    {
      buf[j] = buf[tmp_start + j + 1U];
    }
    res = len - tmp_start - 1U;
  }
  else
  {
    res = len;
  }
  return res; /* Remaining characters in the temporary buffer */
}

#endif // (CFG_USB_INTERFACE_ENABLE != 0) 
#if (CFG_FULL_LOW_POWER == 0)

/**
 * @brief  TaskProcess sends receive CLI command to M0.
 * @param  None
 * @retval None
 */
static void Send_CLI_To_M0_Task(ULONG argument)
{
    UNUSED(argument);
 
#ifdef TX_ENABLE_EVENT_TRACE    
    tx_trace_user_event_insert( APP_TRACE_INFO, 4, 0, 0, 0 );
#endif // TX_ENABLE_EVENT_TRACE    
  
    for(;;)
    {
        (void)tx_semaphore_get(&SendCliCmdSemaphore, TX_WAIT_FOREVER);
#ifdef TX_ENABLE_EVENT_TRACE    
        tx_trace_user_event_insert( APP_TRACE_INFO, 4, 0, 0, 1 );
#endif // TX_ENABLE_EVENT_TRACE 
        Send_CLI_To_M0();
    }
}

/**
 * @brief Process sends receive CLI command to M0.
 * @param  None
 * @retval None
 */
static void Send_CLI_To_M0(void)
{
#ifdef ENABLE_OPENTHREAD_CLI
  /* Don't use the ThreadCliCmdBuffer when buffer is too large as data may be overwritten.
     Use locals variables instead */
  uint16_t l_plen = 0;
  uint8_t l_ThreadCliCmdBuffer[255] = {0};
  
  memcpy(l_ThreadCliCmdBuffer, CommandString, indexReceiveChar);
  l_plen = indexReceiveChar;
  
    /* Clear receive buffer, character counter and command complete */
  CptReceiveCmdFromUser = 0;
  indexReceiveChar = 0;
  memset(CommandString, 0, C_SIZE_CMD_STRING);
  
  APP_DBG("[Send_CLI_To_M0] payload : %s", l_ThreadCliCmdBuffer);

  otPlatUartReceived(l_ThreadCliCmdBuffer, l_plen);  
#else
  memset(ThreadCliCmdBuffer.cmdserial.cmd.payload, 0x0U, 255U);
  memcpy(ThreadCliCmdBuffer.cmdserial.cmd.payload, CommandString, indexReceiveChar);
  ThreadCliCmdBuffer.cmdserial.cmd.plen = indexReceiveChar;
  ThreadCliCmdBuffer.cmdserial.cmd.cmdcode = 0x0;

  /* Clear receive buffer, character counter and command complete */
  CptReceiveCmdFromUser = 0;
  indexReceiveChar = 0;
  memset(CommandString, 0, C_SIZE_CMD_STRING);

  TL_CLI_SendCmd();
#endif /* ENABLE_OPENTHREAD_CLI */
}
#endif /* (CFG_FULL_LOW_POWER == 0) */

#ifdef ENABLE_OPENTHREAD_CLI
/**
 * @brief  Wrapper function to flush UART data (called from the OpenThread stack)
           Not used but definition needed.
 * @param  None
 * @retval OT_ERROR_NONE
 */
otError otPlatUartFlush(void)
{
  return OT_ERROR_NONE;
}

/**
 * @brief  Wrapper function to send data through the UART from the OpenThread stack
 * @param  aBuf: Buffer of data to transmit
 * @param  aBufLength: Number of data to transmit (in bytes)
 * @retval OT_ERROR_NONE
 */
otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
   /* WORKAROUND: if string to output is "> " then respond directly to M0 and do not output it */
  if (strcmp((const char *)aBuf, "> ") != 0)
  {
    /* Write to CLI UART */
    HW_UART_Transmit(CFG_CLI_UART, (uint8_t*)aBuf, aBufLength, 100);
  }

  otPlatUartSendDone();
  
  return OT_ERROR_NONE;
}
#endif /* ENABLE_OPENTHREAD_CLI */

/**
 * @brief Send notification for CLI TL Channel.
 * @param  None
 * @retval None
 */
static void Send_CLI_Ack_For_OT(void)
{

  /* Notify M0 that characters have been sent to UART */
  TL_THREAD_CliSendAck();
}

/**
 * @brief Perform initialization of CLI UART interface.
 * @param  None
 * @retval None
 */
void APP_THREAD_Init_UART_CLI(TX_BYTE_POOL* p_byte_pool)
{
#if (CFG_FULL_LOW_POWER == 0)
  UINT ThreadXStatus;
  CHAR* pointer = TX_NULL;
  
  tx_semaphore_create(&SendCliCmdSemaphore, "SendCliCmdSemaphore", 0);
  tx_byte_allocate(p_byte_pool, (VOID**) &pointer, DEMO_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&OsTaskSendCliToM0,
                   	   	   	   	   "OsTaskSendCliToM0",
								   Send_CLI_To_M0_Task,
								   0,
								   pointer,
								   DEMO_STACK_SIZE_LARGE,
								   SEND_CLI_TO_M0_PRIORITY,
								   SEND_CLI_TO_M0_PRIORITY,
								   TX_NO_TIME_SLICE,
								   TX_AUTO_START);

  if (ThreadXStatus != TX_SUCCESS)
    { APP_THREAD_Error( ERR_THREAD_THREAD_X_FAILED, 2 ); }


#endif /* (CFG_FULL_LOW_POWER == 0) */

#if (CFG_USB_INTERFACE_ENABLE != 0)
#else
#if (CFG_FULL_LOW_POWER == 0)
  MX_LPUART1_UART_Init();
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1, RxCpltCallback);
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
}

/**
 * @brief Perform initialization of TL for THREAD.
 * @param  None
 * @retval None
 */
void APP_THREAD_TL_THREAD_INIT(void)
{
  ThreadConfigBuffer.p_ThreadOtCmdRspBuffer = (uint8_t*)&ThreadOtCmdBuffer;
  ThreadConfigBuffer.p_ThreadNotAckBuffer = (uint8_t*)ThreadNotifRspEvtBuffer;
  ThreadConfigBuffer.p_ThreadCliRspBuffer = (uint8_t*)&ThreadCliCmdBuffer;
  ThreadConfigBuffer.p_ThreadCliNotBuffer = (uint8_t*)&ThreadCliNotBuffer;

  TL_THREAD_Init( &ThreadConfigBuffer );
}

/**
 * @brief  This function is called when notification on CLI TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_THREAD_CliNotReceived( TL_EvtPacket_t * Notbuffer )
{
  TL_CmdPacket_t* l_CliBuffer = (TL_CmdPacket_t*)Notbuffer;
  uint8_t l_size = l_CliBuffer->cmdserial.cmd.plen;

  /* WORKAROUND: if string to output is "> " then respond directly to M0 and do not output it */
  if (strcmp((const char *)l_CliBuffer->cmdserial.cmd.payload, "> ") != 0)
  {
    /* Write to CLI UART */
#if (CFG_USB_INTERFACE_ENABLE != 0)
    VCP_SendData( l_CliBuffer->cmdserial.cmd.payload, l_size, HostTxCb);
#else
    HW_UART_Transmit_IT(CFG_CLI_UART, l_CliBuffer->cmdserial.cmd.payload, l_size, HostTxCb);
#endif /*USAGE_OF_VCP */
  }
  else
  {
    Send_CLI_Ack_For_OT();
  }
}

/**
 * @brief  End of transfer callback for CLI UART sending.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void HostTxCb(void)
{
  Send_CLI_Ack_For_OT();
}

void APP_THREAD_ProcessMsgM0ToM4(ULONG argument)
{
    UNUSED(argument);

    for ( ;;) 
    {
      tx_semaphore_get(&MessageM0ToM4Semaphore, TX_WAIT_FOREVER);
      if (CptReceiveMsgFromM0 != 0) 
      {
        /* If CptReceiveNotifyFromM0 is > 1. it means that we did not serve all the events from the radio */
        if (CptReceiveMsgFromM0 > 1U) 
        {
            APP_THREAD_Error(ERR_REC_MULTI_MSG_FROM_M0, 0);
        }
        else 
        {
            OpenThread_CallBack_Processing();
        }
        /* Reset counter */
        CptReceiveMsgFromM0 = 0;
      }
    }
}

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
 * @brief  This function is called when thereare some data coming
 *         from the Hyperterminal via the USB port
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data received
 * @param  Len: Number of data received (in bytes)
 * @retval Number of characters remaining in the buffer and not yet processed
 */
void VCP_DataReceived(uint8_t* Buf , uint32_t *Len)
{
  uint32_t i,flag_continue_checking = TRUE;
  uint32_t char_remaining = 0;
  static uint32_t len_total = 0;

  /* Copy the characters in the temporary buffer */
  for (i = 0; i < *Len; i++)
  {
    TmpString[len_total++] = Buf[i];
  }

  /* Process the buffer commands one by one     */
  /* A command is limited by a \r caracaters    */
  while (flag_continue_checking == TRUE)
  {
    char_remaining = ProcessCmdString(TmpString,len_total);
    /* If char_remaining is equal to len_total, it means that the command string is not yet
     * completed.
     * If char_remaining is equal to 0, it means that the command string has
     * been entirely processed.
     */
    if ((char_remaining == 0) || (char_remaining == len_total))
    {
      flag_continue_checking = FALSE;
    }
    len_total = char_remaining;
  }
}
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
