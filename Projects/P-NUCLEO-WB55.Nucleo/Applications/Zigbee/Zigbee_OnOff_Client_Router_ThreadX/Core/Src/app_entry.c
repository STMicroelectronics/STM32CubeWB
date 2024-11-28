/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_entry.c
  * @author  MCD Application Team
  * @brief   Entry point of the application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "app_zigbee.h"
#include "app_conf.h"
#include "hw_conf.h"
#include "tx_api.h"
#include "stm_logging.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "shci.h"
#include "otp.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* POOL_SIZE = 2(TL_PacketHeader_t) + 258 (3(TL_EVT_HDR_SIZE) + 255(Payload size)) */
#define POOL_SIZE (CFG_TL_EVT_QUEUE_LENGTH * 4U * DIVC((sizeof(TL_PacketHeader_t) + TL_EVENT_FRAME_SIZE), 4U))

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
extern uint8_t g_ot_notification_allowed;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Global variables ----------------------------------------------------------*/
TX_MUTEX          MtxShciId;
TX_SEMAPHORE      SemShciId;
TX_SEMAPHORE      SemShciNotify;
TX_THREAD         ShciUserEvtProcessId;
TX_BYTE_POOL      * pBytePool;

/* Global function prototypes -----------------------------------------------*/
#if (CFG_DEBUG_TRACE != 0)
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);
#endif /* CFG_DEBUG_TRACE != 0 */

/* USER CODE BEGIN GFP */

/* USER CODE END GFP */

/* Private functions prototypes-----------------------------------------------*/
static void ShciUserEvtProcess(ULONG argument);
static void Config_HSE(void);
static void Reset_Device(void);
#if (CFG_HW_RESET_BY_FW == 1)
static void Reset_IPCC(void);
static void Reset_BackupDomain(void);
#endif /* CFG_HW_RESET_BY_FW == 1*/
static void System_Init(void);
static void SystemPower_Config(void);
static void Init_Debug(void);
static void appe_Tl_Init(void);
static void APPE_SysStatusNot(SHCI_TL_CmdStatus_t status);
static void APPE_SysUserEvtRx(void * pPayload);
static void APPE_SysEvtReadyProcessing(void);
static void APPE_SysEvtError(SCHI_SystemErrCode_t ErrorCode);
#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif /* CFG_HW_LPUART1_ENABLED == 1 */
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif /* CFG_HW_USART1_ENABLED == 1 */
static void Init_Rtc(void);

/* USER CODE BEGIN PFP */
static void Led_Init                ( void );
static void Button_Init             ( void );

/* Section specific to button management using UART */
static void RxUART_Init             ( void );
static void RxCpltCallback          ( void );
static void UartCmdExecute          ( void );

#define C_SIZE_CMD_STRING           256U
#define RX_BUFFER_SIZE              8U

static uint8_t          aRxBuffer[RX_BUFFER_SIZE];
static uint8_t          CommandString[C_SIZE_CMD_STRING];
static uint16_t         indexReceiveChar = 0;
EXTI_HandleTypeDef      exti_handle;

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void MX_APPE_Config(void)
{
  /**
   * The OPTVERR flag is wrongly set at power on
   * It shall be cleared before using any HAL_FLASH_xxx() api
   */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  /**
   * Reset some configurations so that the system behave in the same way
   * when either out of nReset or Power On
   */
  Reset_Device();

  /* Configure HSE Tuning */
  Config_HSE();

  return;
}

uint32_t MX_APPE_Init(void *p_param)
{
  /* Save ThreadX byte pool for whole WPAN middleware */
  pBytePool = p_param;

  System_Init();       /**< System initialization */

  SystemPower_Config(); /**< Configure the system Power Mode */

  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

/* USER CODE BEGIN APPE_Init_1 */
  Init_Debug();

  Led_Init();
  Button_Init();
  RxUART_Init();

/* USER CODE END APPE_Init_1 */
  appe_Tl_Init();	/* Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event (VS_HCI_C2_Ready)
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */

  return(TX_SUCCESS);
}

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

void Init_Exti(void)
{
  /* Enable IPCC(36), HSEM(38) wakeup interrupts on CPU1 */
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_36 | LL_EXTI_LINE_38);

  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void Init_Debug(void)
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

#if (CFG_DEBUG_TRACE != 0)
  DbgTraceInit();
#endif /* CFG_DEBUG_TRACE != 0 */

  return;
}
static void Reset_Device(void)
{
#if (CFG_HW_RESET_BY_FW == 1)
  Reset_BackupDomain();

  Reset_IPCC();
#endif /* CFG_HW_RESET_BY_FW == 1 */

  return;
}

#if (CFG_HW_RESET_BY_FW == 1)
static void Reset_BackupDomain(void)
{
  if ((LL_RCC_IsActiveFlag_PINRST() != FALSE) && (LL_RCC_IsActiveFlag_SFTRST() == FALSE))
  {
    HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

    /**
     *  Write twice the value to flush the APB-AHB bridge
     *  This bit shall be written in the register before writing the next one
     */
    HAL_PWR_EnableBkUpAccess();

    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();
  }

  return;
}

static void Reset_IPCC(void)
{
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_IPCC);

  LL_C1_IPCC_ClearFlag_CHx(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_ClearFlag_CHx(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C1_IPCC_DisableTransmitChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_DisableTransmitChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C1_IPCC_DisableReceiveChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_DisableReceiveChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  return;
}
#endif /* CFG_HW_RESET_BY_FW == 1 */

static void Config_HSE(void)
{
    OTP_ID0_t * p_otp;

  /**
   * Read HSE_Tuning from OTP
   */
  p_otp = (OTP_ID0_t *) OTP_Read(0);
  if (p_otp)
  {
    LL_RCC_HSE_SetCapacitorTuning(p_otp->hse_tuning);
  }

  return;
}

static void System_Init(void)
{
  Init_Smps();

  Init_Exti();

  Init_Rtc();

  return;
}

static void Init_Rtc(void)
{
  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);

  LL_RTC_WAKEUP_SetClock(RTC, CFG_RTC_WUCKSEL_DIVIDER);

  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);

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
  /* Before going to stop or standby modes, do the settings so that system clock and IP80215.4 clock start on HSI automatically */
  LL_RCC_HSI_EnableAutoFromStop();

  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init();
  /* Initialize the CPU2 reset value before starting CPU2 with C2BOOT */
  LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

  /* Disable Stop & Off Modes until Initialisation is complete */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

#if (CFG_USB_INTERFACE_ENABLE != 0)
  /**
   *  Enable USB power
   */
  HAL_PWREx_EnableVddUSB();
#endif /* CFG_USB_INTERFACE_ENABLE != 0 */

  return;
}

static void appe_Tl_Init(void)
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;
  UINT ThreadXStatus;
  CHAR * pTempBuf = TX_NULL;

  /**< Reference table initialization */
  TL_Init();

  /* Create Mutex & Semaphore and make it busy at initialization */
  tx_mutex_create(&MtxShciId, "MtxShciId", TX_NO_INHERIT);
  tx_semaphore_create(&SemShciId, "SemShciId", 0);
  tx_semaphore_create(&SemShciNotify, "SemShciNotify", 0);

  /* System task creation */
  tx_byte_allocate(pBytePool, (VOID**) &pTempBuf, THREADX_STACK_SIZE_LARGE, TX_NO_WAIT);
  ThreadXStatus = tx_thread_create(&ShciUserEvtProcessId,
                                    "ShciUserEvtProcessId",
                                    ShciUserEvtProcess,
                                    0,
                                    pTempBuf,
                                    THREADX_STACK_SIZE_LARGE,
                                    SHCI_USER_EVT_PROCESS_PRIORITY,
                                    SHCI_USER_EVT_PROCESS_PRIORITY,
                                    TX_NO_TIME_SLICE,
                                    TX_AUTO_START);

  if (ThreadXStatus != TX_SUCCESS)
  {
    APP_ZIGBEE_Error(ERR_ZIGBEE_THREAD_X_FAILED,1);
  }

  /**< System channel initialization */
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
  shci_init(APPE_SysUserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  memset(&tl_mm_config, 0, sizeof(TL_MM_Config_t));
  tl_mm_config.p_BleSpareEvtBuffer = 0;
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init(&tl_mm_config);

  TL_Enable();

  return;
}

static void APPE_SysStatusNot(SHCI_TL_CmdStatus_t status)
{
  switch (status)
  {
    case SHCI_TL_CmdBusy:
      tx_mutex_get(&MtxShciId, TX_WAIT_FOREVER  );
      break;

    case SHCI_TL_CmdAvailable:
      tx_mutex_put(&MtxShciId );
      break;

    default:
      break;
  }
  return;
}

/**
 * The type of the payload for a system user event is tSHCI_UserEvtRxParam
 * When the system event is both :
 *    - a ready event (subevtcode = SHCI_SUB_EVT_CODE_READY)
 *    - reported by the FUS (sysevt_ready_rsp == FUS_FW_RUNNING)
 * The buffer shall not be released
 * (eg ((tSHCI_UserEvtRxParam*)pPayload)->status shall be set to SHCI_TL_UserEventFlow_Disable)
 * When the status is not filled, the buffer is released by default
 */
static void APPE_SysUserEvtRx(void * pPayload)
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
static void APPE_SysEvtError(SCHI_SystemErrCode_t ErrorCode)
{
  switch(ErrorCode)
  {
  case ERR_ZIGBEE_UNKNOWN_CMD:
       APP_DBG("** ERR_ZIGBEE : UNKNOWN_CMD \n");
       break;
  default:
       APP_DBG("** ERR_ZIGBEE : ErroCode=%d \n",ErrorCode);
       break;
  }
  return;
}

static void APPE_SysEvtReadyProcessing(void)
{
  /* Traces channel initialization */
  TL_TRACES_Init();

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
  
  return;
}

/*************************************************************
 *
 * ThreadX WRAPPER FUNCTIONS
 *
*************************************************************/
static void ShciUserEvtProcess(ULONG argument)
{
  UNUSED(argument);

  for(;;)
  {
    /* USER CODE BEGIN SHCI_USER_EVT_PROCESS_1 */

    /* USER CODE END SHCI_USER_EVT_PROCESS_1 */
     tx_semaphore_get(&SemShciNotify, TX_WAIT_FOREVER);
     shci_user_evt_proc();
    /* USER CODE BEGIN SHCI_USER_EVT_PROCESS_2 */

    /* USER CODE END SHCI_USER_EVT_PROCESS_2 */
  }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
static void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1U)
  /* Leds Initialization */
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
#endif /* (CFG_LED_SUPPORTED == 1U) */

  return;
}

static void Button_Init( void )
{
#if (CFG_BUTTON_SUPPORTED == 1U)
  /* Button Initialization */
  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW2, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW3, BUTTON_MODE_EXTI);
  
  /* only button 1 is used */
  HAL_NVIC_SetPriority((IRQn_Type)BUTTON_SW1_EXTI_IRQn, 0x0E, 0x00);
#endif /* (CFG_BUTTON_SUPPORTED == 1U) */

  return;
}

/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += HAL_GetTickFreq();
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
    /************************************************************************************
     * ENTER SLEEP MODE
     ***********************************************************************************/
    LL_LPM_EnableSleep(); /**< Clear SLEEPDEEP bit of Cortex System Control Register */

    /**
     * This option is used to ensure that store operations are completed
     */
  #if defined (__CC_ARM) || defined (__ARMCC_VERSION)
    __force_stores();
  #endif /*__ARMCC_VERSION */

    __WFI();
  }
}

void shci_notify_asynch_evt(void* pdata)
{
  UNUSED(pdata);
  tx_semaphore_put(&SemShciNotify);
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UNUSED(flag);
  tx_semaphore_put(&SemShciId);
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UNUSED(timeout);
  tx_semaphore_get(&SemShciId, TX_WAIT_FOREVER);
  return;
}

/* Received trace buffer from M0 */
void TL_TRACES_EvtReceived(TL_EvtPacket_t * hcievt)
{
#if (CFG_DEBUG_TRACE != 0)
  /* Call write/print function using DMA from dbg_trace */
  /* - Cast to TL_AsynchEvt_t* to get "real" payload (without Sub Evt code 2bytes),
     - (-2) to size to remove Sub Evt Code */
  DbgTraceWrite(1U, (const unsigned char *) ((TL_AsynchEvt_t *)(hcievt->evtserial.evt.payload))->payload, hcievt->evtserial.evt.plen - 2U);
#endif /* CFG_DEBUG_TRACE != 0 */
  /* Release buffer */
  TL_MM_EvtDone(hcievt);
}
/**
  * @brief  Initialisation of the trace mechanism
  * @param  None
  * @retval None
  */
#if (CFG_DEBUG_TRACE != 0)
void DbgOutputInit(void)
{
#ifdef CFG_DEBUG_TRACE_UART
  MX_USART1_UART_Init();
  return;
#endif /* CFG_DEBUG_TRACE_UART */
}

/**
  * @brief  Management of the traces
  * @param  p_data : data
  * @param  size : size
  * @param  call-back :
  * @retval None
  */
void DbgOutputTraces(uint8_t *p_data, uint16_t size, void (*cb)(void))
{
  HW_UART_Transmit_DMA(CFG_DEBUG_TRACE_UART, p_data, size, cb);

  return;
}
#endif /* CFG_DEBUG_TRACE != 0 */
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
        APP_ZIGBEE_LaunchPushButtonTask();
        break;

    case BUTTON_SW2_PIN:
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
