/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_entry.c
  * Description        : Entry application source file for STM32WPAN Middleware.
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
#include "main.h"
#include "app_entry.h"
#include "app_thread.h"
#include "app_conf.h"
#include "hw_conf.h"
#include "stm32_seq.h"
#include "stm_logging.h"
#include "dbg_trace.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "app_ble.h"
#include "shci.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
EXTI_HandleTypeDef exti_handle;
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* POOL_SIZE = 2(TL_PacketHeader_t) + 258 (3(TL_EVT_HDR_SIZE) + 255(Payload size)) */
#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH*4*DIVC(( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4))
/* USER CODE BEGIN PD */
/* Section specific to button management using UART */
#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

extern RTC_HandleTypeDef hrtc; /**< RTC handler declaration */

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t	SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t	BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
uint8_t g_ot_notification_allowed = 0U;
/* SELECT THE PROTOCOL THAT WILL START FIRST (BLE or THREAD) */
static SHCI_C2_CONCURRENT_Mode_Param_t ConcurrentMode = BLE_ENABLE;

/* Section specific to button management using UART */
static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;

/* Global variables ----------------------------------------------------------*/

/* Global function prototypes -----------------------------------------------*/
#if(CFG_DEBUG_TRACE != 0)
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);
#endif

/* USER CODE BEGIN GFP */

/* USER CODE END GFP */

/* Private functions prototypes-----------------------------------------------*/
static void SystemPower_Config( void );
static void Init_Debug( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx( void * pPayload );
static void APPE_SysEvtReadyProcessing( void );
static void APPE_SysEvtError( SCHI_SystemErrCode_t ErrorCode);

static void appe_Tl_Init( void );
/* USER CODE BEGIN PFP */
static void Led_Init( void );
static void Button_Init( void );

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

/* USER CODE END PFP */

static void displayConcurrentMode(void);
static void Process_Switch_Protocol(void);


/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  /* Configure the system Power Mode */
  SystemPower_Config();
  
  /* Initialize the TimerServer */
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc);
  
/* USER CODE BEGIN APPE_Init_1 */
  /* initialize debugger module if supported and debug trace if activated */
  Init_Debug();

  /* Display concurrent mode (BLE or Thread) that will start first */
  displayConcurrentMode();

  /* Task common to Thread and BLE */
  UTIL_SEQ_RegTask( 1<<CFG_Task_Switch_Protocol, UTIL_SEQ_RFU,Process_Switch_Protocol);

  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

  Led_Init();
  Button_Init();
  
  RxUART_Init();

/* USER CODE END APPE_Init_1 */
  /* Initialize all transport layers and start CPU2 which will send back a ready event to CPU1 */
  appe_Tl_Init();
  
  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */
   return;
}

static void displayConcurrentMode()
{
  if(ConcurrentMode == BLE_ENABLE)
  {
    APP_DBG("ConcurrentMode starting in BLE mode");
  }
  else if (ConcurrentMode == THREAD_ENABLE)
  {
    APP_DBG("ConcurrentMode starting in Thread mode");
  }
  else
  {
    APP_DBG("ERROR: Starting with unknown ConcurrentMode ID %d", ConcurrentMode);
  }
}

/** Scheduler tasks **/
static void Process_Switch_Protocol(void)
{
  APP_DBG("Process_Switch_Protocol");

  /* Send Switch event to M0 */
  /* 0 = BLE ; 1 = THREAD */
  /* SWITCH BLE <-> THREAD */
  if(ConcurrentMode == THREAD_ENABLE)
  {
    APP_DBG("STOP THREAD");
    APP_THREAD_Stop();

    /* wait Thread has stopped completely -> callback changed state called */
    APP_DBG("WAIT for CFG_Evt_ThreadStop");
    UTIL_SEQ_WaitEvt(1<<CFG_Evt_ThreadStop);
    APP_DBG("AFTER WAIT CFG_Evt_ThreadStop");

    APP_DBG("Clean THREAD registered callbacks");
    APP_THREAD_CleanCallbacks();

    APP_DBG("SWITCH PROTOCOL TO BLE");
  }
  else
  {
    APP_DBG("STOP BLE");

    APP_BLE_Stop();

    APP_DBG("SWITCH PROTOCOL TO THREAD");
  }

  /* Toggle Mode flag */
  if(ConcurrentMode == THREAD_ENABLE){
    ConcurrentMode = BLE_ENABLE;
  }else{
    ConcurrentMode = THREAD_ENABLE;
  }

  /* Once Switch has been acknowledged from M0, starts appropriate Protocol Init */
  if(ConcurrentMode == THREAD_ENABLE)
  {
    APP_DBG("INIT THREAD");
    APP_THREAD_Init();
  }
  else
  {
    APP_DBG("INIT BLE");
    APP_BLE_Init( );
  }

  /**
   *  When the application wants to disable the output of traces, it is not 
   *  enough to just disable the UART. Indeed, CPU2 will still send its traces 
   *  through IPCC to CPU1. By default, CPU2 enables the debug traces. The 
   *  command below will indicate to CPU2 to disable its traces if 
   *  CFG_DEBUG_TRACE is set to 0.
   *  The result is a gain of performance for both CPUs and a reduction of power
   *  consumption, especially in dense networks where there can be a massive
   *  amount of data to print.
   */
  SHCI_C2_DEBUG_TracesConfig_t APPD_TracesConfig;
  /* Set to 1 to enable Thread traces, 0 to disable */
  APPD_TracesConfig.thread_config = CFG_DEBUG_TRACE;
  /* Set to 1 to enable BLE traces, 0 to disable */
  APPD_TracesConfig.ble_config = CFG_DEBUG_TRACE;
  
  SHCI_C2_DEBUG_GeneralConfig_t APPD_GeneralConfig = {0};
  
  SHCI_C2_DEBUG_Init_Cmd_Packet_t DebugCmdPacket =
  {
    {{0,0,0}},
    {(uint8_t *)NULL,
    (uint8_t *)&APPD_TracesConfig,
    (uint8_t *)&APPD_GeneralConfig,
    0,
    0,
    0}
  };
  
  SHCI_C2_DEBUG_Init(&DebugCmdPacket);
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void Init_Debug( void )
{
#if (CFG_DEBUGGER_SUPPORTED == 1)
  /**
   * Keep debugger enabled while in any low power mode
   */
  HAL_DBGMCU_EnableDBGSleepMode();
  
  /* Enable debugger EXTI lines */
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);
  LL_C2_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);

#else
  /* Disable debugger EXTI lines */
  LL_EXTI_DisableIT_32_63(LL_EXTI_LINE_48);
  LL_C2_EXTI_DisableIT_32_63(LL_EXTI_LINE_48);

  GPIO_InitTypeDef gpio_config = {0};

  gpio_config.Pull = GPIO_NOPULL;
  gpio_config.Mode = GPIO_MODE_ANALOG;

  gpio_config.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &gpio_config);
  __HAL_RCC_GPIOA_CLK_DISABLE();

  gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_3;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &gpio_config);
  __HAL_RCC_GPIOB_CLK_DISABLE();

  /**
   * Do not keep debugger enabled while in any low power mode
   */
  HAL_DBGMCU_DisableDBGSleepMode();
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();
#endif /* (CFG_DEBUGGER_SUPPORTED == 1) */
  
#if(CFG_DEBUG_TRACE != 0)
  DbgTraceInit();
#endif
  
  return;
}

/**
 * @brief  Configure the system for power optimization
 *
 * @note  This API configures the system to be ready for low power mode
 *
 * @param  None
 * @retval None
 */
static void SystemPower_Config( void )
{
  // Before going to stop or standby modes, do the settings so that system clock and IP80215.4 clock
  // start on HSI automatically
  LL_RCC_HSI_EnableAutoFromStop();
  
  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init( );
  
  /* Disable low power mode until INIT is complete */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  
#if (CFG_USB_INTERFACE_ENABLE != 0)
  /**
   *  Enable USB power
   */
  HAL_PWREx_EnableVddUSB();
#endif

  /* Enable RAM1 (because OT instance.o is located here for Concurrent Mode */
  LL_C2_AHB1_GRP1_EnableClock(LL_C2_AHB1_GRP1_PERIPH_SRAM1);
  LL_C2_AHB1_GRP1_EnableClockSleep(LL_C2_AHB1_GRP1_PERIPH_SRAM1);

  return;
}

static void appe_Tl_Init( void )
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;

  /**< Reference table initialization */
  TL_Init();

  /**< System channel initialization */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SYSTEM_HCI_ASYNCH_EVT_ID, UTIL_SEQ_RFU, shci_user_evt_proc );
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
  shci_init(APPE_SysUserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  tl_mm_config.p_BleSpareEvtBuffer = BleSpareEvtBuffer;
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init( &tl_mm_config );

  TL_Enable();

  return;
}

static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status )
{
  UNUSED(status);
  return;
}

/**
 * The type of the payload for a system user event is tSHCI_UserEvtRxParam
 * When the system event is both :
 *    - a ready event (subevtcode = SHCI_SUB_EVT_CODE_READY)
 *    - reported by the FUS (sysevt_ready_rsp == FUS_FW_RUNNING)
 * The buffer shall not be released
 * ( eg ((tSHCI_UserEvtRxParam*)pPayload)->status shall be set to SHCI_TL_UserEventFlow_Disable )
 * When the status is not filled, the buffer is released by default
 */
static void APPE_SysUserEvtRx( void * pPayload )
{
  TL_AsynchEvt_t *p_sys_event;
  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);
  
  switch(p_sys_event->subevtcode)
  {
    case SHCI_SUB_EVT_CODE_READY:
      APPE_SysEvtReadyProcessing();
      break;
      
    case SHCI_SUB_EVT_ERROR_NOTIF:
      APPE_SysEvtError((SCHI_SystemErrCode_t) (p_sys_event->payload[0]));
      break;
      
    default:
      break;
  }
  return;
}

/**
 * @brief Notify a system error coming from the M0 firmware
 * @param  ErrorCode  : errorCode detected by the M0 firmware
 *
 * @retval None
 */
static void APPE_SysEvtError( SCHI_SystemErrCode_t ErrorCode)
{
  switch(ErrorCode)
  {
  case ERR_THREAD_LLD_FATAL_ERROR:
    APP_DBG("** ERR_THREAD : LLD_FATAL_ERROR \n");
    break;
    
  case ERR_THREAD_UNKNOWN_CMD:
    APP_DBG("** ERR_THREAD : UNKNOWN_CMD \n");
    break;
    
  default:
    APP_DBG("** ERR_THREAD : ErroCode=%d \n",ErrorCode);
    break;
  }
  return;
}

static void APPE_SysEvtReadyProcessing( void )
{
  /* Traces channel initialization */
  TL_TRACES_Init( );

  if(ConcurrentMode == THREAD_ENABLE){
    APP_THREAD_Init();
  }else{
    APP_BLE_Init();
  }

  /**
   *  When the application wants to disable the output of traces, it is not 
   *  enough to just disable the UART. Indeed, CPU2 will still send its traces 
   *  through IPCC to CPU1. By default, CPU2 enables the debug traces. The 
   *  command below will indicate to CPU2 to disable its traces if 
   *  CFG_DEBUG_TRACE is set to 0.
   *  The result is a gain of performance for both CPUs and a reduction of power
   *  consumption, especially in dense networks where there can be a massive
   *  amount of data to print.
   */
  SHCI_C2_DEBUG_TracesConfig_t APPD_TracesConfig;
  /* Set to 1 to enable Thread traces, 0 to disable */
  APPD_TracesConfig.thread_config = CFG_DEBUG_TRACE;
  /* Set to 1 to enable BLE traces, 0 to disable */
  APPD_TracesConfig.ble_config = CFG_DEBUG_TRACE;
  
  SHCI_C2_DEBUG_GeneralConfig_t APPD_GeneralConfig = {0};
  
  SHCI_C2_DEBUG_Init_Cmd_Packet_t DebugCmdPacket =
  {
    {{0,0,0}},
    {(uint8_t *)NULL,
    (uint8_t *)&APPD_TracesConfig,
    (uint8_t *)&APPD_GeneralConfig,
    0,
    0,
    0}
  };
  
  SHCI_C2_DEBUG_Init(&DebugCmdPacket);

#if ( CFG_LPM_SUPPORTED == 1)
  /* Thread stack is initialized, low power mode can be enabled */
  UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
  UTIL_LPM_SetStopMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
#endif

  return;
}

static void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1U)
  /**
   * Leds Initialization
   */

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  BSP_LED_On(LED_GREEN);
#endif

  return;
}

static void Button_Init( void )
{

#if (CFG_BUTTON_SUPPORTED == 1U)
  /**
   * Button Initialization
   */

  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW2, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW3, BUTTON_MODE_EXTI);
#endif

  return;
}

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void UTIL_SEQ_Idle( void )
{
#if ( CFG_LPM_SUPPORTED == 1)
  UTIL_LPM_EnterLowPower( );
#endif
  return;
}

/**
  * @brief  This function is called by the scheduler each time an event
  *         is pending.
  *
  * @param  evt_waited_bm : Event pending.
  * @retval None
  */
void UTIL_SEQ_EvtIdle( UTIL_SEQ_bm_t task_id_bm, UTIL_SEQ_bm_t evt_waited_bm )
{
  switch(evt_waited_bm)
  {
  case EVENT_ACK_FROM_M0_EVT:
    if(g_ot_notification_allowed == 1U)
    {
      /* Some OT API send M0 to M4 notifications so allow notifications when waiting for OT Cmd response */
      UTIL_SEQ_Run(TASK_MSG_FROM_M0_TO_M4);
    }
    else
    {
      /* Does not allow other tasks when waiting for OT Cmd response */
      UTIL_SEQ_Run(0);
    }
    break;
  case EVENT_SYNCHRO_BYPASS_IDLE:
    UTIL_SEQ_SetEvt(EVENT_SYNCHRO_BYPASS_IDLE);
    /* Run only the task CFG_TASK_MSG_FROM_M0_TO_M4 */
    UTIL_SEQ_Run(TASK_MSG_FROM_M0_TO_M4);
    break;
  default :
    /* default case */
  UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
    break;
  }
}

void shci_notify_asynch_evt(void* pdata)
{
  UNUSED(pdata);
  UTIL_SEQ_SetTask(1U << CFG_TASK_SYSTEM_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UNUSED(flag);
  UTIL_SEQ_SetEvt(1U << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UNUSED(timeout);
  UTIL_SEQ_WaitEvt(1U << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
  return;
}

/* Received trace buffer from M0 */
void TL_TRACES_EvtReceived( TL_EvtPacket_t * hcievt )
{
#if(CFG_DEBUG_TRACE != 0)
  /* Call write/print function using DMA from dbg_trace */
  /* - Cast to TL_AsynchEvt_t* to get "real" payload (without Sub Evt code 2bytes),
     - (-2) to size to remove Sub Evt Code */
  DbgTraceWrite(1U, (const unsigned char *) ((TL_AsynchEvt_t *)(hcievt->evtserial.evt.payload))->payload, hcievt->evtserial.evt.plen - 2U);
#endif /* CFG_DEBUG_TRACE */
  /* Release buffer */
  TL_MM_EvtDone( hcievt );
}
/**
  * @brief  Initialization of the trace mechanism
  * @param  None
  * @retval None
  */
#if(CFG_DEBUG_TRACE != 0)
void DbgOutputInit( void )
{
#if (CFG_HW_USART1_ENABLED == 1)
  HW_UART_Init(CFG_DEBUG_TRACE_UART);
#endif
  return;
}

/**
  * @brief  Management of the traces
  * @param  p_data : data
  * @param  size : size
  * @param  call-back :
  * @retval None
  */
void DbgOutputTraces(  uint8_t *p_data, uint16_t size, void (*cb)(void) )
{
  HW_UART_Transmit_DMA(CFG_DEBUG_TRACE_UART, p_data, size, cb);

  return;
}
#endif

/**
 * @brief This function manage the Push button action
 * @param  GPIO_Pin : GPIO pin which has been activated
 * @retval None
 */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  switch (GPIO_Pin)
  {
  case BUTTON_SW1_PIN:
    if(ConcurrentMode == THREAD_ENABLE){
      APP_DBG("BUTTON 1 PUSHED ! : COAP MESSAGE SENDING");
      UTIL_SEQ_SetTask(TASK_COAP_MSG_BUTTON,CFG_SCH_PRIO_0);
    } else{
      APP_BLE_Key_Button1_Action();
    }
    break;

  case BUTTON_SW2_PIN:
    APP_DBG("BUTTON 2 PUSHED ! : SWITCHING PROTOCOL");
    /* Set "Switch Protocol" Task */
    UTIL_SEQ_SetTask(1<<CFG_Task_Switch_Protocol,CFG_SCH_PRIO_0);
    break;

  case BUTTON_SW3_PIN:
    APP_DBG("BUTTON 3 PUSHED ! : NO ACTION MAPPED ON SW3");
    APP_BLE_Key_Button3_Action();
    break;

  default:
    break;
  }
  return;
}

static void RxUART_Init(void)
{
  HW_UART_Receive_IT((hw_uart_id_t)CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    if (aRxBuffer[0] == '\r')
    {
      APP_DBG_MSG("received %s\n", CommandString);

      UartCmdExecute();

      /* Clear receive buffer and character counter*/
      indexReceiveChar = 0;
      memset(CommandString, 0, C_SIZE_CMD_STRING);
    }
    else
    {
      CommandString[indexReceiveChar++] = aRxBuffer[0];
    }
  }

  /* Once a character has been sent, put back the device in reception mode */
  HW_UART_Receive_IT((hw_uart_id_t)CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void UartCmdExecute(void)
{
  /* Parse received CommandString */
  if(strcmp((char const*)CommandString, "SW1") == 0)
  {
    APP_DBG_MSG("SW1 OK\n");
    exti_handle.Line = EXTI_LINE_4;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW2") == 0)
  {
    APP_DBG_MSG("SW2 OK\n");
    exti_handle.Line = EXTI_LINE_0;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW3") == 0)
  {
    APP_DBG_MSG("SW3 OK\n");
    exti_handle.Line = EXTI_LINE_1;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else
  {
    APP_DBG_MSG("NOT RECOGNIZED COMMAND : %s\n", CommandString);
  }
}
