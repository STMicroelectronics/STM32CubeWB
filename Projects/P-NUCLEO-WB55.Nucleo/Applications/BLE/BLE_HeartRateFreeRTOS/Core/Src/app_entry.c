/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_entry.c
  * Description        : Entry application source file for STM32WPAN Middleware
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "main.h"
#include "app_entry.h"
#include "app_ble.h"
#include "ble.h"
#include "tl.h"
#include "cmsis_os.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH*4U*DIVC(( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4U))

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];

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
    .stack_size = CFG_SHCI_USER_EVT_PROCESS_STACk_SIZE
};

/* Private functions prototypes-----------------------------------------------*/
static void ShciUserEvtProcess(void *argument);
static void SystemPower_Config( void );
static void Init_Debug( void );
static void appe_Tl_Init( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx( void * pPayload );

#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif

/* USER CODE BEGIN PFP */
static void Led_Init( void );
static void Button_Init( void );
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  SystemPower_Config(); /**< Configure the system Power Mode */

  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

/* USER CODE BEGIN APPE_Init_1 */
  Init_Debug();

  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

  Led_Init();

  Button_Init();
/* USER CODE END APPE_Init_1 */
  appe_Tl_Init();	/* Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */
  return;
}
/* USER CODE BEGIN FD */

/* USER CODE END FD */

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
static void SystemPower_Config( void )
{

  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init( );

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

  MtxShciId = osMutexNew( NULL );
  SemShciId = osSemaphoreNew( 1, 0, NULL ); /*< Create the semaphore and make it busy at initialization */

  /**< System channel initialization */
  ShciUserEvtProcessId = osThreadNew(ShciUserEvtProcess, NULL, &ShciUserEvtProcess_attr);
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
  switch (status)
  {
    case SHCI_TL_CmdBusy:
      osMutexAcquire( MtxShciId, osWaitForever );
      break;

    case SHCI_TL_CmdAvailable:
      osMutexRelease( MtxShciId );
      break;

    default:
      break;
  }
  return;
}

static void APPE_SysUserEvtRx( void * pPayload )
{
  UNUSED(pPayload);
  /* Traces channel initialization */
  TL_TRACES_Init( );

  APP_BLE_Init( );
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
#if (CFG_LED_SUPPORTED == 1)
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
#if (CFG_BUTTON_SUPPORTED == 1)
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

void shci_notify_asynch_evt(void* pdata)
{
  UNUSED(pdata);
  osThreadFlagsSet( ShciUserEvtProcessId, 1 );
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UNUSED(flag);
  osSemaphoreRelease( SemShciId );
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UNUSED(timeout);
  osSemaphoreAcquire( SemShciId, osWaitForever );
  return;
}

/**
  * @brief  Initialisation of the trace mechanism
  * @param  None
  * @retval None
  */
#if(CFG_DEBUG_TRACE != 0)
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
#endif

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  switch (GPIO_Pin)
  {
    case BUTTON_SW1_PIN:
     APP_BLE_Key_Button1_Action();
      break; 

    case BUTTON_SW2_PIN:
      APP_BLE_Key_Button2_Action();
      break; 

    case BUTTON_SW3_PIN:
      APP_BLE_Key_Button3_Action();
      break;

    default:
      break;

}
  return;
}
/* USER CODE END FD_WRAP_FUNCTIONS */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
