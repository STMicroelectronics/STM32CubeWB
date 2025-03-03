/**
  ******************************************************************************
 * @file    app_entry.c
 * @author  MCD Application Team
 * @brief   Entry point of the Application
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
#include "main.h"
#include "app_entry.h"
#include "app_zigbee.h"
#include "app_conf.h"
#include "hw_conf.h"
#include "stm32_seq.h"
#include "stm_logging.h"
#include "dbg_trace.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "app_ble.h"
#include "shci.h"


/* External definition */
extern RTC_HandleTypeDef hrtc; /**< RTC handler declaration */

EXTI_HandleTypeDef exti_handle;

/* Private defines -----------------------------------------------------------*/
/* POOL_SIZE */
#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH*4*DIVC(( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4))

/* Section specific to button management using UART */
#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U

/* Switch timeout */
#define SWITCH_TMO 3*1000*1000/CFG_TS_TICK_VAL

#define HW_TS_SWITCH_PROTOCOL                    (5*1000*1000/CFG_TS_TICK_VAL)

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t    SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t    BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
extern uint8_t g_ot_notification_allowed;

static ConcurentModeStateType concurentModeState = State_Ble;
static int NbTotalSwitch = 0;

static uint8_t TS_ID1;
#ifdef SWITCH_AUTO
static uint8_t TS_ID2;
#endif /* SWITCH_AUTO */

/* Section specific to button management using UART */
static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;

/* Global functions prototypes -----------------------------------------------*/
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);
ConcurentModeStateType APP_GetCurrentProtocolMode(void);

/* Private function prototypes -----------------------------------------------*/
static void SystemPower_Config( void );
static void Init_Debug( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx( void * pPayload );

static void appe_Tl_Init( void );
static void Led_Init( void );
static void Button_Init( void );

static void Process_InitiateSwitchProtocol(void);
static void Process_ActivateNewProtocol(void);
static void ScheduleProcessSwitchProtocol(void);

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  SystemPower_Config(); /**< Configure the system Power Mode */
  
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

  Init_Debug();

  /* Task common to Zigbee and BLE */
  UTIL_SEQ_RegTask( 1U <<CFG_TASK_INIT_SWITCH_PROTOCOL, UTIL_SEQ_RFU,Process_InitiateSwitchProtocol);
  UTIL_SEQ_RegTask( 1U <<CFG_TASK_ACTIVATE_PROTOCOL, UTIL_SEQ_RFU,Process_ActivateNewProtocol);
  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

  Led_Init();

  Button_Init();
  
  RxUART_Init();

  appe_Tl_Init(); /**< Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the BLE or Zigbee Stack
   * This system event is received with APPE_UserEvtRx()
   */

  return;
}

/**
 * @brief  Return the current protocol state in use
 * @param  None
 * @retval None
 */
ConcurentModeStateType APP_GetCurrentProtocolMode(void)
{
    return concurentModeState;
}

/**
 * @brief  Process used to initiate the protocol switch
 * @param  None
 * @retval None
 */
static void Process_InitiateSwitchProtocol(void)
{
   APP_DBG("Process_InitiateSwitchProtocol (nb = %d)",NbTotalSwitch ++);

   if(concurentModeState == State_Zigbee)
   {
     concurentModeState = State_Zigbee_To_Ble;
     APP_DBG("STOP ZIGBEE");
     APP_ZIGBEE_Stop();

     /* start a timer to postpone the process of switching the protocol */
     HW_TS_Create(CFG_TIM_WAIT_BEFORE_SWITCH, &TS_ID1, hw_ts_SingleShot, ScheduleProcessSwitchProtocol);
     HW_TS_Start(TS_ID1, SWITCH_TMO);
     APP_DBG("SWITCH PROTOCOL TO BLE");
   }

   if (concurentModeState == State_Ble)
   {
     concurentModeState = State_Ble_To_Zigbee;
     APP_DBG("STOP BLE");
     APP_BLE_Stop();
     APP_DBG("SWITCH PROTOCOL TO Zigbee");
     ScheduleProcessSwitchProtocol();
   }
}

/**
 * @brief  Schedule the process used to switch protocol
 * @param  None
 * @retval None
 */
static void ScheduleProcessSwitchProtocol(void)
{
    UTIL_SEQ_SetTask(1 << CFG_TASK_ACTIVATE_PROTOCOL,CFG_SCH_PRIO_0);
}

/**
 * @brief  Process the activation of the new protocol
 * @param  None
 * @retval None
 */
static void Process_ActivateNewProtocol(void)
{

   if(concurentModeState == State_Zigbee_To_Ble)
   {
      APP_DBG("INIT BLE");
      APP_BLE_Init();
      concurentModeState = State_Ble;
   }

   if(concurentModeState == State_Ble_To_Zigbee)
   {
      APP_DBG("INIT ZIGBEE");
      APP_ZIGBEE_Init();
      concurentModeState = State_Zigbee;
    }
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

  /***************** ENABLE DEBUGGER *************************************/
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);
  LL_C2_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);

#else

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
static void SystemPower_Config(void)
{

  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init();
  /* Initialize the CPU2 reset value before starting CPU2 with C2BOOT */
  LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

#if (CFG_USB_INTERFACE_ENABLE != 0)
  /**
   *  Enable USB power
   */
  HAL_PWREx_EnableVddUSB();
#endif

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
  UNUSED(pPayload);
  /* Traces channel initialization */
  TL_TRACES_Init( );

  if(concurentModeState == State_Zigbee)
  {
    APP_ZIGBEE_Init();
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
    /* Set to 1 to enable Zigbee traces, 0 to disable */
    APPD_TracesConfig.zigbee_config = CFG_DEBUG_TRACE;
    /* Set to 1 to enable BLE traces, 0 to disable */
    APPD_TracesConfig.ble_config = CFG_DEBUG_TRACE;
  
    SHCI_C2_DEBUG_Init_Cmd_Packet_t DebugCmdPacket =
    {
      {{0,0,0}},
      {(uint8_t *)NULL,
      (uint8_t *)&APPD_TracesConfig,
      (uint8_t *)NULL,
      0,
      0,
      0}
    };
    SHCI_C2_DEBUG_Init(&DebugCmdPacket);
  }
  if(concurentModeState == State_Ble)
  {
    APP_BLE_Init();
  }

  UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);

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
	/* Check the notification condition */
	if (g_ot_notification_allowed) {
		UTIL_SEQ_Run(1U << CFG_TASK_NOTIFY_FROM_M0_TO_M4);
	}	
  switch (evt_waited_bm) {
    case EVENT_ACK_FROM_M0_EVT:
      /* Run only the task CFG_TASK_REQUEST_FROM_M0_TO_M4 to process
      * direct requests from the M0 (e.g. ZbMalloc), but no stack notifications
      * until we're done the request to the M0. */
      UTIL_SEQ_Run((1U << CFG_TASK_REQUEST_FROM_M0_TO_M4));
      break;

    case EVENT_SYNCHRO_BYPASS_IDLE:
      UTIL_SEQ_SetEvt(EVENT_SYNCHRO_BYPASS_IDLE);
      /* Process notifications and requests from the M0 */
      UTIL_SEQ_Run((1U << CFG_TASK_NOTIFY_FROM_M0_TO_M4) | (1U << CFG_TASK_REQUEST_FROM_M0_TO_M4));
      break;

    case EVENT_ZIGBEE_STARTUP_ENDED:
      /* Do not authorize any switch of protocol until the EVENT_ZIGBEE_STARTUP_ENDED has been received.startup ended has been received in order
       * The purpose is to be not restart a network form once the previous one has been completed
       */
      UTIL_SEQ_Run(~(1U <<CFG_TASK_INIT_SWITCH_PROTOCOL));
      break;
    default:
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
  HW_UART_Init(CFG_DEBUG_TRACE_UART);
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
    if (APP_GetCurrentProtocolMode() == State_Zigbee)
    {
      APP_DBG("BUTTON 1 PUSHED ! : ZIGBEE MESSAGE SENDING");
      UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON_SW1,CFG_SCH_PRIO_1);
    }
    if (APP_GetCurrentProtocolMode() == State_Ble)
    {
      APP_BLE_Key_Button1_Action();
    }
    break;

  case BUTTON_SW2_PIN:
    if ((APP_GetCurrentProtocolMode() == State_Ble) || (APP_GetCurrentProtocolMode() == State_Zigbee))
    {
      APP_DBG("BUTTON 2 PUSHED ! : SWITCHING PROTOCOL");
      UTIL_SEQ_SetTask(1U << CFG_TASK_INIT_SWITCH_PROTOCOL,CFG_SCH_PRIO_0);
    }
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
#ifdef SWITCH_AUTO
/**
 * @brief This function manage the automatic transitions
 * @retval None
 */
void APPE_SwitchProtocolAuto( void )
{
  /* Test robustness...*/
  HW_TS_Create(CFG_SWITCH_PROTOCOL_REPEAT, &TS_ID2, hw_ts_Repeated, Process_InitiateSwitchProtocol);
  HW_TS_Start(TS_ID2, (HW_TS_SWITCH_PROTOCOL));
}
#endif /*SWITCH_AUTO*/


