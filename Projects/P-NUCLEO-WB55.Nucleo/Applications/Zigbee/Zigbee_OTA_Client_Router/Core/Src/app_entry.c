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

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "main.h"
#include "app_entry.h"
#include "app_zigbee.h"
#include "app_conf.h"
#include "hw_conf.h"
#include "stm32_seq.h"
#include "stm_logging.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "shci.h"

/* Private includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/* Private defines -----------------------------------------------------------*/
#define POOL_SIZE (CFG_TL_EVT_QUEUE_LENGTH * 4U * DIVC((sizeof(TL_PacketHeader_t) + TL_EVENT_FRAME_SIZE), 4U))

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
extern uint8_t g_ot_notification_allowed;

/* Global function prototypes -----------------------------------------------*/
size_t __write(int handle, const unsigned char *buf, size_t bufSize);

/* Private functions prototypes-----------------------------------------------*/
static void SystemPower_Config(void);
static void Init_Debug(void);
static void APPE_Tl_Init(void);
static void APPE_SysStatusNot(SHCI_TL_CmdStatus_t status);
static void APPE_SysUserEvtRx(void *pPayload);
static SHCI_TL_UserEventFlowStatus_t APPE_SysevtReadyProcessing( SHCI_C2_Ready_Evt_t *pReadyEvt );
static void APPE_SysEvtError(SCHI_SystemErrCode_t ErrorCode);

#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif

/* USER CODE BEGIN PFP */
static void Led_Init(void);
static void Button_Init(void);

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U

static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;
EXTI_HandleTypeDef exti_handle;
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  /* Configure the system Power Mode */
  SystemPower_Config(); 

  /* Initialize the TimerServer */
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); 

  Init_Debug();
  
  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  Led_Init();
  Button_Init();
  RxUART_Init();

  /* Initialize all transport layers */
  APPE_Tl_Init();	

  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */
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

#else /* (CFG_DEBUGGER_SUPPORTED == 1) */

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

static void APPE_Tl_Init( void )
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;
  
  /**< Reference table initialization */
  TL_Init();

  /**< System channel initialization */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SYSTEM_HCI_ASYNCH_EVT, UTIL_SEQ_RFU, shci_user_evt_proc );
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
  shci_init(APPE_SysUserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  memset(&tl_mm_config, 0, sizeof(TL_MM_Config_t));
  tl_mm_config.p_BleSpareEvtBuffer = 0;
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init( &tl_mm_config );

  TL_Enable();

  return;
}


static void APPE_SysStatusNot(SHCI_TL_CmdStatus_t status)
{
  UNUSED(status);
  return;
}

/**
 * @brief Trap a notification coming from the M0 firmware
 * @param  pPayload  : payload associated to the notification
 *
 * @retval None
 */

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
         ((tSHCI_UserEvtRxParam*)pPayload)->status = APPE_SysevtReadyProcessing( (SHCI_C2_Ready_Evt_t*)p_sys_event->payload );
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

static SHCI_TL_UserEventFlowStatus_t APPE_SysevtReadyProcessing( SHCI_C2_Ready_Evt_t *pReadyEvt )
{
  uint8_t fus_state_value;
  SHCI_TL_UserEventFlowStatus_t return_value;

#if ( CFG_LED_SUPPORTED != 0)
  BSP_LED_Off(LED_BLUE);
#endif

  if(pReadyEvt->sysevt_ready_rsp == WIRELESS_FW_RUNNING)
  {
    return_value = SHCI_TL_UserEventFlow_Enable;

    if((*(uint8_t*)SRAM1_BASE) == CFG_REBOOT_ON_CPU2_UPGRADE)
    {
      /**
       * The wireless stack update has been completed
       * Reboot on the firmware application
       */
        APP_DBG("[OTA] : Wireless stack update complete. Reboot on the firmware application");
        HAL_Delay(100);
        *(uint8_t*)SRAM1_BASE = CFG_REBOOT_ON_DOWNLOADED_FW;
        NVIC_SystemReset();
    }
    else
    {
      /**
       * Run the Application
       */

      /* Traces channel initialization */
      TL_TRACES_Init( );

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
	  
      UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
    }
  }
  else
  {
    /**
     * FUS is running on CPU2
     */
    return_value = SHCI_TL_UserEventFlow_Disable;

    /**
     * The CPU2 firmware update procedure is starting from now
     * There may be several device reset during CPU2 firmware upgrade
     * The key word at the beginning of SRAM1 shall be changed CFG_REBOOT_ON_CPU2_UPGRADE
     *
     * Wireless Firmware upgrade:
     * Once the upgrade is over, the CPU2 will run the wireless stack
     * When the wireless stack is running, the SRAM1 is checked and when equal to CFG_REBOOT_ON_CPU2_UPGRADE,
     * it means we may restart on the firmware application.
     *
     * FUS Firmware Upgrade:
     * Once the upgrade is over, the CPU2 will run FUS and the FUS return the Idle state
     * The SRAM1 is checked and when equal to CFG_REBOOT_ON_CPU2_UPGRADE,
     * it means we may restart on the firmware application.
     */
    fus_state_value = SHCI_C2_FUS_GetState( NULL );

    if( fus_state_value == 0xFF)
    {
      /**
       * This is the first time in the life of the product the FUS is involved. After this command, it will be properly initialized
       * Request the device to reboot to install the wireless firmware
       */
      APP_DBG("[OTA] : First time in the life of the product the FUS is involved. Reboot to install the wireless firmware");
      HAL_Delay(100);
      NVIC_SystemReset();
    }
    else if( fus_state_value != 0)
    {
      /**
       * An upgrade is on going
       * Wait to reboot on the wireless stack
       */
      APP_DBG("[OTA] : An upgrade is on going. Wait to reboot on the wireless stack");
      HAL_Delay(100);
#if ( CFG_LED_SUPPORTED != 0)
      BSP_LED_On(LED_BLUE);
#endif
      while(1)
      {
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      }
    }
    else
    {
      /**
       * FUS is idle
       * Request an upgrade and wait to reboot on the wireless stack
       * The first two parameters are currently not supported by the FUS
       */
      if((*(uint8_t*)SRAM1_BASE) == CFG_REBOOT_ON_CPU2_UPGRADE)
      {
        /**
         * The FUS update has been completed
         * Reboot the CPU2 on the firmware application
         */
        APP_DBG("[OTA] : FUS update has been completed. Reboot the on the firmware application");
        HAL_Delay(100);
        *(uint8_t*)SRAM1_BASE = CFG_REBOOT_ON_DOWNLOADED_FW;
        SHCI_C2_FUS_StartWs( );
  #if ( CFG_LED_SUPPORTED != 0)
        BSP_LED_On(LED_BLUE);
  #endif
        while(1)
        {
          HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
        }
      }
      else
      {
        APP_DBG("[OTA] : SHCI_C2_FUS_FwUpgrade launch. Next Reboot is on the wireless update");
        HAL_Delay(100);
        *(uint8_t*)SRAM1_BASE = CFG_REBOOT_ON_CPU2_UPGRADE;
        /**
         * Note:
         * If a reset occurs now, on the next reboot the FUS will be idle and a CPU2 reboot on the
         * wireless stack will be requested because SRAM1 is set to CFG_REBOOT_ON_CPU2_UPGRADE
         * The device is still operational but no CPU2 update has been done.
         */
        SHCI_C2_FUS_FwUpgrade(0,0);
  #if ( CFG_LED_SUPPORTED != 0)
        BSP_LED_On(LED_BLUE);
  #endif
        while(1)
        {
          HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
        }
      }
    }
  }

  return return_value;
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
#endif /* (CFG_BUTTON_SUPPORTED == 1U) */

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
  switch (evt_waited_bm) 
  {
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
  UTIL_SEQ_SetTask(1U << CFG_TASK_SYSTEM_HCI_ASYNCH_EVT, CFG_SCH_PRIO_0);
  return;
}


void shci_cmd_resp_release(uint32_t flag)
{
  UNUSED(flag);
  UTIL_SEQ_SetEvt(1U << CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP);
  return;
}


void shci_cmd_resp_wait(uint32_t timeout)
{
  UNUSED(timeout);
  UTIL_SEQ_WaitEvt(1U << CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP);
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
  * @brief  Initialisation of the trace mechanism
  * @param  None
  * @retval None
  */
#if (CFG_DEBUG_TRACE != 0)
void DbgOutputInit( void )
{
  MX_USART1_UART_Init(); 

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
#endif /* (CFG_DEBUG_TRACE != 0) */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
/**
  * @brief This function manage the Push button action
  * @param  GPIO_Pin : GPIO pin which has been activated
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin) 
  {
    case BUTTON_SW1_PIN:
        break;

    case BUTTON_SW2_PIN:
        UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON_SW2,CFG_SCH_PRIO_1);
        break;

    case BUTTON_SW3_PIN:
        break;

    default:
        break;
  }
}


static void RxUART_Init(void)
{
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}


static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    if (aRxBuffer[0] == '\r')
    {
      APP_DBG("received %s", CommandString);

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
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}


static void UartCmdExecute(void)
{
  /* Parse received CommandString */
  if(strcmp((char const*)CommandString, "SW1") == 0)
  {
    APP_DBG("SW1 OK");
    exti_handle.Line = EXTI_LINE_4;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW2") == 0)
  {
    APP_DBG("SW2 OK");
    exti_handle.Line = EXTI_LINE_0;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW3") == 0)
  {
    APP_DBG("SW3 OK");
    exti_handle.Line = EXTI_LINE_1;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "RST") == 0)
  {
    APP_DBG("RESET CMD RECEIVED");
    HAL_NVIC_SystemReset();
  }  
  else
  {
    APP_DBG("NOT RECOGNIZED COMMAND : %s", CommandString);
  }
}
/* USER CODE END FD_WRAP_FUNCTIONS */
