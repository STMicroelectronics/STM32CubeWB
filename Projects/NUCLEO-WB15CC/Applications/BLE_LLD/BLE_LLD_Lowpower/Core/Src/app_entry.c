/* USER CODE BEGIN Header */
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "app_entry.h"
#include "app_ble_lld.h"
#include "app_conf.h"
#include "hw_conf.h"
#include "stm32_seq.h"
#include "stm_logging.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "shci.h"
#include "lowpower_app.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern RTC_HandleTypeDef hrtc;
EXTI_HandleTypeDef exti_handle;

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* POOL_SIZE = 2(TL_PacketHeader_t) + 258 (3(TL_EVT_HDR_SIZE) + 255(Payload size)) */
#define POOL_SIZE (CFG_TL_EVT_QUEUE_LENGTH * 4U * DIVC(( sizeof(TL_PacketHeader_t) + TL_EVENT_FRAME_SIZE ), 4U))

/* USER CODE BEGIN PD */
/* Section specific to button management using UART */
#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];

/* USER CODE BEGIN PV */
/* Section specific to button management using UART */
static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;

/* USER CODE END PV */

/* Global function prototypes -----------------------------------------------*/
#if(CFG_DEBUG_TRACE != 0)
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);
#endif

/* USER CODE BEGIN GFP */

/* USER CODE END GFP */

/* Private functions prototypes-----------------------------------------------*/
static void appe_Tl_Init( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx( void * pPayload );
static void APPE_SysEvtReadyProcessing( void );
static void APPE_SysEvtError( SCHI_SystemErrCode_t ErrorCode);

/* USER CODE BEGIN PFP */

/* Section specific to button management using UART */
static void RxCpltCallback(void);
static void UartCmdExecute(void);
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  /**< Configure the system Power Mode */
  SystemPower_Config();
  
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

/* USER CODE BEGIN APPE_Init_1 */
  /* initialize debugger module if supported and debug trace if activated */
  Init_Debug();
  
  Led_Init();
  Button_Init();
  
  RxUART_Init();
/* USER CODE END APPE_Init_1 */
  /* Initialize all transport layers and start CPU2 which will send back a ready event to CPU1 */
  appe_Tl_Init();
  
  /**
   * From now, the application is waiting for the ready event ( sub event : SHCI_SUB_EVT_CODE_READY / payload : WIRELESS_FW_RUNNING)
   * received on the system channel before starting the LLD test appli using system message SHCI_OPCODE_C2_LLD_TESTS_INIT
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */
   return;
}
/* USER CODE BEGIN FD */
void Init_Smps(void)
{
#if (CFG_USE_SMPS != 0)
  /**
   *  Configure and enable SMPS
   *
   *  The SMPS configuration is not yet supported by CubeMx
   *  when SMPS output voltage is set to 1.4V, the RF output power is limited to 3.7dBm
   *  the SMPS output voltage shall be increased for higher RF output power
   */
  LL_PWR_SMPS_SetStartupCurrent(LL_PWR_SMPS_STARTUP_CURRENT_80MA);
  LL_PWR_SMPS_SetOutputVoltageLevel(LL_PWR_SMPS_OUTPUT_VOLTAGE_1V40);
  LL_PWR_SMPS_Enable();
#endif /* CFG_USE_SMPS != 0 */

  return;
}
/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
void Init_Debug( void )
{
#if (CFG_DEBUGGER_SUPPORTED == 1)
  /**
   * Keep debugger enabled while in any low power mode
   */
  HAL_DBGMCU_EnableDBGSleepMode();
  HAL_DBGMCU_EnableDBGStopMode();
   
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
  
  /* Send a first trace to debug trace port to see that M4 is alive */
  APP_DBG("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  APP_DBG("traces init done on M4");
  APP_DBG("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  
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

void SystemPower_Config( void )
{
  // Disable internal wake-up which is active by default and is for RTC wake-up
  LL_PWR_DisableInternWU();
  
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
  
  return;
}

static void appe_Tl_Init( void )
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
  tl_mm_config.p_BleSpareEvtBuffer = 0;
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init( &tl_mm_config );
  
  /* Enable transport layer and start CPU2 */
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
      if (p_sys_event->payload[0] == WIRELESS_FW_RUNNING)
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
    APP_DBG("** ERR_LLD_TESTS : LLD_FATAL_ERROR \n");
    break;
    
  case ERR_THREAD_UNKNOWN_CMD:
    APP_DBG("** ERR_LLD_TESTS : UNKNOWN_CMD \n");
    break;
    
  default:
    APP_DBG("** ERR_LLD_TESTS : ErroCode=%d \n",ErrorCode);
    break;
  }
  return;
}

static void APPE_SysEvtReadyProcessing( void )
{
  /* Traces channel initialization */
  TL_TRACES_Init( );
  
  /* Application specific init */
  LOWPOWER_APP_Init();
  
#if ( CFG_LPM_SUPPORTED == 1)
  /* Thread stack is initialized, low power mode can be enabled */
  UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
  UTIL_LPM_SetStopMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
#endif

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1U)
  /**
   * Leds Initialization
   */
#if (CFG_HW_LPUART1_ENABLED != 1) || ! defined (STM32WB35xx)
  // On WB35, LED_BLUE share the GPIO PB5 with LPUART
  BSP_LED_Init(LED_BLUE);
#endif
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
#endif
  
  return;
}

void Button_Init( void )
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

/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void UTIL_SEQ_Idle( void )
{
  /* Note that WFI (i.e. SLEEP mode) is required for SF timer tests but STOP or OFF mode will be managed by low-power test itself */
#if ( CFG_LPM_SUPPORTED == 1)
  UTIL_LPM_EnterLowPower( );
#endif
  return;
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
#if(CFG_DEBUG_TRACE != 0)
void DbgOutputInit( void )
{
/* USER CODE BEGIN DbgOutputInit */
#ifdef CFG_DEBUG_TRACE_UART
  MX_UART_Init(CFG_DEBUG_TRACE_UART);
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

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

void RxUART_Init(void)
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
      APP_DBG("received %s\n", CommandString);

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
    APP_DBG("SW1 OK\n");
    exti_handle.Line = EXTI_LINE_0;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW2") == 0)
  {
    APP_DBG("SW2 OK\n");
    exti_handle.Line = EXTI_LINE_4;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW3") == 0)
  {
    APP_DBG("SW3 OK\n");
    exti_handle.Line = EXTI_LINE_6;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else
  {
    APP_DBG("NOT RECOGNIZED COMMAND : %s\n", CommandString);
  }
}
/* USER CODE END FD_WRAP_FUNCTIONS */
