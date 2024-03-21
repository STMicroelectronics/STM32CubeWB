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
#include "cmsis_os.h"
#include "stm_logging.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "shci.h"
#include "otp.h"
#include "stm_list.h"
#include "advanced_memory_manager.h"
#include "stm32_mm.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/**
 * Structure of Trace_Elt_t
 * buffer : trace buffer
 * size : size of the trace buffer
 */
typedef __PACKED_STRUCT
{
  uint32_t *next;
  uint32_t *prev;
} TraceEltHeader_t;

/**
 * Structure of Trace_Elt_t
 * buffer : trace buffer
 * size : size of the trace buffer
 */
typedef __PACKED_STRUCT
{
  uint8_t   buffer[255];
  uint32_t  size;
} TraceElt_t;

/**
 * Structure of Trace_Elt_t
 * buffer : trace buffer
 * size : size of the trace buffer
 */
typedef struct __attribute__((packed, aligned(4)))
{
  TraceEltHeader_t header;
  TraceElt_t trace;
} TraceEltPacket_t;


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
uint8_t g_ot_notification_allowed = 0U;

#if (CFG_AMM_ENABLED != 0)
static tListNode TraceBufferList;
static uint32_t TracePool[CFG_AMM_POOL_SIZE];

/* AMM_BCKGND_TASK related resources */
osThreadId_t AMM_BCKGND_Thread;
const osThreadAttr_t AMM_BCKGND_Thread_Attr;
osSemaphoreId_t AMM_BCKGND_Thread_Sem;
const osSemaphoreAttr_t AMM_BCKGND_Thread_Sem_Attr;

/* TRC_BCKGND_TASK related resources */
osThreadId_t TRC_BCKGND_Thread;
const osThreadAttr_t TRC_BCKGND_Thread_Attr;
osSemaphoreId_t TRC_BCKGND_Thread_Sem;
const osSemaphoreAttr_t TRC_BCKGND_Thread_Sem_Attr;

osMutexId_t LINK_LAYER_Thread_Mutex;
static AMM_VirtualMemoryConfig_t vmConfig[CFG_AMM_VIRTUAL_MEMORY_NUMBER] = {
  /* Virtual Memory #1 */
  {
    .Id = CFG_AMM_VIRTUAL_APP_TRACE,
    .BufferSize = CFG_AMM_VIRTUAL_APP_TRACE_BUFFER_SIZE
  }
};

static AMM_InitParameters_t ammInitConfig =
{
  .p_PoolAddr = TracePool,
  .PoolSize = CFG_AMM_POOL_SIZE,
  .VirtualMemoryNumber = CFG_AMM_VIRTUAL_MEMORY_NUMBER,
  .p_VirtualMemoryConfigList = vmConfig
};
#endif

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Global variables ----------------------------------------------------------*/
osMutexId_t MtxShciId;
osSemaphoreId_t SemShciId;
osThreadId_t ShciUserEvtProcessId;

const osThreadAttr_t ShciUserEvtProcess_attr = {
    .name = CFG_SHCI_USER_EVT_PROCESS_NAME,
    .attr_bits = CFG_SHCI_USER_EVT_PROCESS_ATTR_BITS,
    .cb_mem = CFG_SHCI_USER_EVT_PROCESS_CB_MEM,
    .cb_size = CFG_SHCI_USER_EVT_PROCESS_CB_SIZE,
    .stack_mem = CFG_SHCI_USER_EVT_PROCESS_STACK_MEM,
    .priority = CFG_SHCI_USER_EVT_PROCESS_PRIORITY,
    .stack_size = CFG_SHCI_USER_EVT_PROCESS_STACK_SIZE
};

/* Global function prototypes -----------------------------------------------*/
#if (CFG_DEBUG_TRACE != 0)
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);
#endif /* CFG_DEBUG_TRACE != 0 */

/* USER CODE BEGIN GFP */

/* USER CODE END GFP */

/* Private functions prototypes-----------------------------------------------*/
static void ShciUserEvtProcess(void *argument);
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
#if (CFG_AMM_ENABLED != 0)
static void TRC_BackgroundProcess(void);
#endif
#if(CFG_DEBUG_TRACE != 0)
static void writeTrace(char * buffer, uint32_t size);
#endif /* CFG_DEBUG_TRACE */
#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif /* CFG_HW_LPUART1_ENABLED == 1 */
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif /* CFG_HW_USART1_ENABLED == 1 */
static void Init_Rtc(void);

/* USER CODE BEGIN PFP */
static void Led_Init( void );
static void Button_Init( void );

#if (CFG_AMM_ENABLED != 0)
/**
 * @brief Wrapper for init function of the MM for the AMM
 *
 * @param p_PoolAddr: Address of the pool to use - Not use -
 * @param PoolSize: Size of the pool - Not use -
 *
 * @return None
 */
static void AMM_WrapperInit (uint32_t * const p_PoolAddr, const uint32_t PoolSize);

/**
 * @brief Wrapper for allocate function of the MM for the AMM
 *
 * @param BufferSize
 *
 * @return Allocated buffer
 */
static uint32_t * AMM_WrapperAllocate (const uint32_t BufferSize);

/**
 * @brief Wrapper for free function of the MM for the AMM
 *
 * @param p_BufferAddr
 *
 * @return None
 */
static void AMM_WrapperFree (uint32_t * const p_BufferAddr);
#endif
/* USER CODE END PFP */

#if (CFG_AMM_ENABLED != 0)
static void AMM_BackgroundProcess_Entry(void* thread_input);
static void TRC_BackgroundProcess_Entry(void* thread_input);
#endif

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

void MX_APPE_Init(void)
{
  System_Init();       /**< System initialization */

  SystemPower_Config(); /**< Configure the system Power Mode */

#if (CFG_AMM_ENABLED != 0)
  /* Initialize the Advance Memory Manager */
  AMM_Init (&ammInitConfig);

  /* Register the AMM background task */
  AMM_BCKGND_Thread_Sem = osSemaphoreNew(1, 0, &AMM_BCKGND_Thread_Sem_Attr);
  AMM_BCKGND_Thread = osThreadNew(AMM_BackgroundProcess_Entry, NULL, &AMM_BCKGND_Thread_Attr);

  /* Register the TRC background task */
  TRC_BCKGND_Thread_Sem = osSemaphoreNew(1, 0, &TRC_BCKGND_Thread_Sem_Attr);
  TRC_BCKGND_Thread = osThreadNew(TRC_BackgroundProcess_Entry, NULL, &TRC_BCKGND_Thread_Attr);

  LST_init_head (&TraceBufferList);
#endif

  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

/* USER CODE BEGIN APPE_Init_1 */
  Init_Debug();

  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  Led_Init();
  Button_Init();
/* USER CODE END APPE_Init_1 */
  appe_Tl_Init();	/* Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event (VS_HCI_C2_Ready)
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */
   return;
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
#endif /* CFG_USB_INTERFACE_ENABLE != 0 */

  return;
}

static void appe_Tl_Init(void)
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;
  /**< Reference table initialization */
  TL_Init();

  MtxShciId = osMutexNew(NULL);
  SemShciId = osSemaphoreNew(1, 0, NULL); /*< Create the semaphore and make it busy at initialization */

  /** FreeRTOS system task creation */
  ShciUserEvtProcessId = osThreadNew(ShciUserEvtProcess, NULL, &ShciUserEvtProcess_attr);

  /**< System channel initialization */
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
  shci_init(APPE_SysUserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
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
      osMutexAcquire(MtxShciId, osWaitForever);
      break;

    case SHCI_TL_CmdAvailable:
      osMutexRelease(MtxShciId);
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

static void APPE_SysEvtReadyProcessing(void)
{
  /* Traces channel initialization */
  TL_TRACES_Init();

  APP_THREAD_Init();
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_ENABLE);
  UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
  return;
}

/*************************************************************
 *
 * FREERTOS WRAPPER FUNCTIONS
 *
*************************************************************/
static void ShciUserEvtProcess(void *argument)
{
  UNUSED(argument);
  for(;;)
  {
    /* USER CODE BEGIN SHCI_USER_EVT_PROCESS_1 */

    /* USER CODE END SHCI_USER_EVT_PROCESS_1 */
     osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);
     shci_user_evt_proc();
    /* USER CODE BEGIN SHCI_USER_EVT_PROCESS_2 */

    /* USER CODE END SHCI_USER_EVT_PROCESS_2 */
    }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
static void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1U)
  /**
   * Leds Initialization
   */
#if (CFG_HW_LPUART1_ENABLED != 1) || ! defined (STM32WB35xx)
  // On STM32WB3x, LED_BLUE share the GPIO PB5 with LPUART
  BSP_LED_Init(LED_BLUE);
#endif
  
#if (CFG_HW_EXTPA_ENABLED != 1)
  BSP_LED_Init(LED_GREEN);
#endif
  
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
#endif
  
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

#if (CFG_AMM_ENABLED != 0)
static void AMM_WrapperInit (uint32_t * const p_PoolAddr, const uint32_t PoolSize)
{
  UTIL_MM_Init ((uint8_t *)p_PoolAddr, ((size_t)PoolSize * sizeof(uint32_t)));
}

static uint32_t * AMM_WrapperAllocate (const uint32_t BufferSize)
{
  return (uint32_t *)UTIL_MM_GetBuffer (((size_t)BufferSize * sizeof(uint32_t)));
}

static void AMM_WrapperFree (uint32_t * const p_BufferAddr)
{
  UTIL_MM_ReleaseBuffer ((void *)p_BufferAddr);
}

void AMM_BackgroundProcess_Entry(void* thread_input)
{
  (void)(thread_input);

  while(1)
  {
    osSemaphoreAcquire(AMM_BCKGND_Thread_Sem , osWaitForever);
    AMM_BackgroundProcess();
  }
}

void AMM_RegisterBasicMemoryManager (AMM_BasicMemoryManagerFunctions_t * const p_BasicMemoryManagerFunctions)
{
  /* Fulfill the function handle */
  p_BasicMemoryManagerFunctions->Init = AMM_WrapperInit;
  p_BasicMemoryManagerFunctions->Allocate = AMM_WrapperAllocate;
  p_BasicMemoryManagerFunctions->Free = AMM_WrapperFree;
}

void AMM_ProcessRequest (void)
{
  /* Ask for AMM background task scheduling */
  osSemaphoreRelease(AMM_BCKGND_Thread_Sem);
}

void TRC_BackgroundProcess(void)
{
  TraceEltPacket_t * traceElt = NULL;
  while (LST_is_empty (&TraceBufferList) == FALSE)
  {
    LST_remove_tail (&TraceBufferList, (tListNode**)&traceElt);
    if (traceElt != NULL)
    {
#if(CFG_DEBUG_TRACE != 0)
      DbgTraceWrite(1U, (const unsigned char *) traceElt->trace.buffer, traceElt->trace.size);
#endif /* CFG_DEBUG_TRACE */
      AMM_Free((uint32_t *)traceElt);
    }
  }
}

void TRC_BackgroundProcess_Entry(void* thread_input)
{
  (void)(thread_input);

  while(1)
  {
    osSemaphoreAcquire(TRC_BCKGND_Thread_Sem , osWaitForever);
    TRC_BackgroundProcess();
  }
}
#endif

void shci_notify_asynch_evt(void* pdata)
{
  UNUSED(pdata);
  osThreadFlagsSet(ShciUserEvtProcessId,1);
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UNUSED(flag);
  osSemaphoreRelease(SemShciId);
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UNUSED(timeout);
  osSemaphoreAcquire(SemShciId, osWaitForever);
  return;
}

/* Received trace buffer from M0 */
void TL_TRACES_EvtReceived( TL_EvtPacket_t * hcievt )
{
#if(CFG_DEBUG_TRACE != 0)
  writeTrace(( char *) ((TL_AsynchEvt_t *)(hcievt->evtserial.evt.payload))->payload, hcievt->evtserial.evt.plen - 2U);
#endif /* CFG_DEBUG_TRACE */
  /* Release buffer */
  TL_MM_EvtDone( hcievt );
}

#if(CFG_DEBUG_TRACE != 0)
void writeTrace(char * buffer, uint32_t size)
{
#if (CFG_AMM_ENABLED != 0)
  TraceEltPacket_t * traceElt = NULL;
  if(AMM_ERROR_OK == AMM_Alloc (CFG_AMM_VIRTUAL_APP_TRACE, DIVC(sizeof(TraceEltPacket_t), sizeof(uint32_t)), (uint32_t **)&traceElt, NULL))
  {
    if(traceElt != NULL)
    {
      memcpy(traceElt->trace.buffer, (const unsigned char *) buffer, size);
      traceElt->trace.size = size;
      LST_insert_head (&TraceBufferList, (tListNode *)traceElt);
      osSemaphoreRelease(TRC_BCKGND_Thread_Sem);
    }
  }
#endif
}
#endif /* CFG_DEBUG_TRACE */
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

/* USER CODE END FD_WRAP_FUNCTIONS */
