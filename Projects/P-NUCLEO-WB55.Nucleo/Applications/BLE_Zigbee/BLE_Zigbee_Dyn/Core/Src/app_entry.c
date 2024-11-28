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
#include "app_debug.h"

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
#define T_1S_NB_TICKS           (1*1000*1000/CFG_TS_TICK_VAL) /**< 1s */
#define T_1MS_NB_TICKS          (1*1000/CFG_TS_TICK_VAL) /**< 1s */
#define T_50MS                  (50*T_1MS_NB_TICKS)
#define T_100MS                 (100*T_1MS_NB_TICKS)
#define T_200MS                 (200*T_1MS_NB_TICKS)
#define T_2S                    (2*T_1S_NB_TICKS)
/* USER CODE BEGIN PD */
/* Section specific to button management using UART */
#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U

/* USER CODE END PD */
/* Private variables ---------------------------------------------------------*/

extern RTC_HandleTypeDef hrtc;  /**< RTC handler declaration */
extern uint8_t ZbStackType;     /* ZB stack type, static or dynamic */

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t	SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t	BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
extern uint8_t g_ot_notification_allowed;

/* SELECT THE PROTOCOL THAT WILL START FIRST (BLE or ZIGBEE) */
static SHCI_C2_CONCURRENT_Mode_Param_t ConcurrentMode = BLE_ENABLE;
//static SHCI_C2_CONCURRENT_Mode_Param_t ConcurrentMode = ZIGBEE_ENABLE;

/* Section specific to button management using UART */
static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;

/* Global variables ----------------------------------------------------------*/

/* Global function prototypes -----------------------------------------------*/
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);

/* Private function prototypes -----------------------------------------------*/
static void SystemPower_Config( void );
static void Init_Debug( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx( void * pPayload );
static void APPE_SysEvtReadyProcessing(void);
static void APPE_SysEvtError(SCHI_SystemErrCode_t ErrorCode);

static void appe_Tl_Init( void );
/* USER CODE BEGIN PFP */
static void Led_Init( void );
static void Button_Init( void );

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

/* USER CODE END PFP */


/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  SystemPower_Config(); /**< Configure the system Power Mode */
  
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

  Init_Debug();

  APP_DBG("ConcurrentMode = %d", ConcurrentMode);
  
  APPD_Init();

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
  /* Initialize the CPU2 reset value before starting CPU2 with C2BOOT */
  LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);
  
  /* Disable low power mode until INIT is complete */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

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
    switch (ErrorCode)
  {
        case ERR_ZIGBEE_UNKNOWN_CMD:
            APP_DBG("** ERR_ZIGBEE : UNKNOWN_CMD \n");
            break;
        default:
            APP_DBG("** ERR_ZIGBEE : ErroCode=%d \n", ErrorCode);
            break;
    }
    return;
}

static void APPE_SysEvtReadyProcessing( void )
{
    /* Traces channel initialization */
    APPD_EnableCPU2();      

    APP_DBG("==> Start_BLE");   /* Start BLE first */
    APP_BLE_Init();

    APP_DBG("==> Start Zigbee stack & Networking");
    APP_ZIGBEE_Init();
       

    switch (ZbStackType) { /* Check ZB stack type */
    case INFO_STACK_TYPE_ZIGBEE_FFD: /* Start ZB only */
    case INFO_STACK_TYPE_ZIGBEE_RFD: /* Start ZB only */
    case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_STATIC: /* Start ZB only */
    case INFO_STACK_TYPE_BLE_ZIGBEE_RFD_STATIC: /* Start ZB only */
      break;
    case INFO_STACK_TYPE_BLE_ZIGBEE_FFD_DYNAMIC:  /* Start ZB/BLE Dynamic mode */
    case INFO_STACK_TYPE_BLE_ZIGBEE_RFD_DYNAMIC:  /* Start ZB/BLE Dynamic mode */
      APP_DBG("==> Start_BLE Advertising");
      UTIL_SEQ_SetTask(1U << CFG_TASK_APP_BLE_START, CFG_SCH_PRIO_1);
      break;
    default:
      /* No Zigbee device supported ! */
      APP_DBG("FW Type : No ZB STACK type detected");
      break;
    }


#if ( CFG_LPM_SUPPORTED == 1)
  /* ZB stack is initialized, low power mode will be enabled later (zb_interface.c) */
    UTIL_LPM_SetStopMode(1U << CFG_LPM_APP, UTIL_LPM_DISABLE);
#endif

    return;
}


/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
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

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
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
    if (1) {
      APP_DBG("BUTTON 1 PUSHED ! : ZIGBEE MESSAGE SENDING");
      UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON_SW1, CFG_SCH_PRIO_1);
    } else {
      APP_BLE_Key_Button1_Action();
    }
    break;

  case BUTTON_SW2_PIN:
    APP_DBG("BUTTON 2 PUSHED ! : Request Connection Update");
    UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON_SW2, CFG_SCH_PRIO_1);
    break;

  case BUTTON_SW3_PIN:
    APP_DBG("BUTTON 3 PUSHED ! : Request Connection Update");
    UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON_SW3, CFG_SCH_PRIO_1);

    /* Start BLE App + Advertising */
//    APP_DBG("==> Start_BLE Advertising");
//    UTIL_SEQ_SetTask(1U << CFG_TASK_APP_BLE_START, CFG_SCH_PRIO_1);

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
/* USER CODE END FD_WRAP_FUNCTIONS */
