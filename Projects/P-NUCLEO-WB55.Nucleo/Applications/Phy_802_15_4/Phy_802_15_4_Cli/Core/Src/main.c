/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  *
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
  @verbatim
  ==============================================================================
                    ##### IMPORTANT NOTE #####
  ==============================================================================

  This application requests having a M0 LLD tests binary
  flashed on the Wireless Coprocessor.
  If it is not the case, you need to use STM32CubeProgrammer to load the appropriate
  binary.

  All available binaries are located under following directory:
  /Projects/STM32_Copro_Wireless_Binaries

  Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
  Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the
  detailed procedure to change the Wireless Coprocessor binary.

  @endverbatim
  ******************************************************************************
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "main.h"
#include "app_entry.h"
#include "app_lld_tests.h"
#include "stm_logging.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32_lpm.h"
#include "stm32_seq.h"
#include "dbg_trace.h"
#include "hw_conf.h"
#include "otp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* HSE trimming defines */
volatile uint32_t * pHSETuningInfo;
PLACE_IN_SECTION("HSE_TUNING_INFO_REQUEST") uint32_t HSETuningInfo = 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if (CFG_HW_LPUART1_ENABLED == 1)
UART_HandleTypeDef hlpuart1;
#if (CFG_HW_LPUART1_DMA_TX_SUPPORTED == 1)
DMA_HandleTypeDef hdma_lpuart1_tx;
#endif
#endif
#if (CFG_HW_USART1_ENABLED == 1)
UART_HandleTypeDef huart1;
#if (CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
DMA_HandleTypeDef hdma_usart1_tx;
#endif
#endif

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_DMA_Init(void);
/* USER CODE BEGIN PFP */
static void EraseUserFlash(uint32_t address);
static void PeriphClock_Config(void);
static void Reset_Device( void );
static void Reset_IPCC( void );
static void Reset_BackupDomain( void );
static void Init_Exti( void );
static void Config_HSE(void);
void SystemPower_Config(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /**
   * The OPTVERR flag is wrongly set at power on
   * It shall be cleared before using any HAL_FLASH_xxx() api
   */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  Reset_Device();

#if 0
  // Remove tuning done to HSE, reset to default state (i.e. HSE tuning done from OTP)
  Config_HSE_tuning(0xFFFFFFFF);
#endif
  Config_HSE();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  PeriphClock_Config();
  Init_Exti(); /**< Configure the system Power Mode */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init code for STM32_WPAN */
  APPE_Init();
  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
    * Assuming that MSI is enabled by default after boot, lets go to HSE without using PLL
  */
  SystemClock_Config_HSE(0);
  /* Configure Others clocks */
  RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48 | 
                                           RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI2;
  RCC_OscInitStruct.LSEState             = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState             = RCC_HSI_OFF;
  RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState             = RCC_LSI_OFF;
  RCC_OscInitStruct.LSI2CalibrationValue = 0;
  RCC_OscInitStruct.HSI48State           = RCC_HSI48_OFF;
  RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief System Clock Configuration : API to be called to use HSE (with or without PLL use) as 32Mhz system clock.
           SystemClock_Config_HSE() must be called once just after boot (to go from default MSI to HSE).
           Then application user can call both SystemClock_Config_HSE() and SystemClock_Config_MSI() at any time.
  * @retval None
  */
void SystemClock_Config_HSE(uint32_t usePLL)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  
  /* First, just set MSI ON (with the 32Mhz range) in case it was OFF, without any update on PLL */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_10;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Select MSI as system clock in order to be able to update HSE and PLL configuration */
  RCC_ClkInitStruct.ClockType    = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Configure HSE and PLL if needed*/
  RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState             = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLSource        = RCC_PLLSOURCE_HSE;
  if (usePLL == 1)
    RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
  else
    RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL.PLLM             = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN             = 8;
  RCC_OscInitStruct.PLL.PLLP             = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ             = RCC_PLLQ_DIV4;
  RCC_OscInitStruct.PLL.PLLR             = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure the system clock source and the dividers according to the fact that system clock source is 32Mhz */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK | 
                                     RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  if (usePLL == 1)
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  else
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  // Note that function UTILS_SetFlashLatency() could be used to set the correct Flash latency
  // (with 32Mhz, 2WS are needed if the range is changed to 1V instead of 1.2V)
  
  /* Disable MSI Oscillator as the MSI is no more needed by the application */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState        = RCC_MSI_OFF;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;  /* No update on PLL */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/**
  * @brief System Clock Configuration : API to be called to use MSI (with or without PLL use) as 32Mhz system clock.
           SystemClock_Config_HSE() must be called once just after boot (to go from default MSI to HSE).
           Then application user can call both SystemClock_Config_HSE() and SystemClock_Config_MSI() at any time.
  * @retval None
  */
void SystemClock_Config_MSI(uint32_t usePLL, uint32_t stopHSE)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  
  /* First, just set HSE ON (with the 32Mhz range) in case it was OFF, without any update on PLL */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Select HSE as system clock in order to be able to update MSI and PLL configuration */
  RCC_ClkInitStruct.ClockType    = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Configure MSI and PLL if needed*/
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_10;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  if (usePLL == 1)
    RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_ON;
  else
    RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_OFF;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN            = 8;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV4;
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Configure the system clock source and the dividers according to the fact that system clock source is 32Mhz */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK4 | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK | 
                                     RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  if (usePLL == 1)
  {
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  }
  else
  {
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  }
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  if (stopHSE == 1) {
    /* Disable HSE Oscillator as the HSE is no more needed by the application */
    RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState        = RCC_HSE_OFF;
    RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;  /* No update on PLL */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }
}

#if (CFG_HW_LPUART1_ENABLED == 1)
/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OverSampling = UART_OVERSAMPLING_16;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

void MX_LPUART1_UART_DeInit(void)
{
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OverSampling = UART_OVERSAMPLING_16;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_DeInit(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif

#if (CFG_HW_USART1_ENABLED == 1)
/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  huart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void MX_USART1_UART_DeInit(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  huart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_DeInit(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
#ifdef STM32WB35xx
  __HAL_RCC_DMA2_CLK_ENABLE();
#endif

  /* DMA interrupt init */
#ifdef STM32WB35xx
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA2_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
#else
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
#endif
}

static void EraseUserFlash(uint32_t address ) {
  FLASH_EraseInitTypeDef EraseInit;
  uint32_t page_error;

  EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInit.Page      = (address- FLASH_BASE) / FLASH_PAGE_SIZE;
  EraseInit.NbPages   = 1;

  // Sometime it seems the erase fails retry once
  if (HAL_FLASHEx_Erase(&EraseInit, &page_error) != HAL_OK)
  {
    if (HAL_FLASHEx_Erase(&EraseInit, &page_error) != HAL_OK)
    {
      APP_DBG("Flash erase operation failed, page error 0x%x", page_error);
    }
  }
}

/* USER CODE BEGIN 4 */
static void PeriphClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  
#if USE_SMPS_ENABLED_BY_DEFAULT
  PeriphClkInitStruct.PeriphClockSelection   = RCC_PERIPHCLK_SMPS | RCC_PERIPHCLK_RFWAKEUP | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_LPUART1;
  PeriphClkInitStruct.Usart1ClockSelection   = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Lpuart1ClockSelection  = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
  PeriphClkInitStruct.SmpsClockSelection     = RCC_SMPSCLKSOURCE_HSE;
  PeriphClkInitStruct.SmpsDivSelection       = RCC_SMPSCLKDIV_RANGE1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Initialize SMPS here like in BLE applis */
  LL_PWR_SMPS_SetStartupCurrent(LL_PWR_SMPS_STARTUP_CURRENT_80MA);
  LL_PWR_SMPS_SetOutputVoltageLevel(LL_PWR_SMPS_OUTPUT_VOLTAGE_1V40);
  LL_PWR_SMPS_Enable();
#else
  PeriphClkInitStruct.PeriphClockSelection   = RCC_PERIPHCLK_RFWAKEUP | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_LPUART1;
  PeriphClkInitStruct.Usart1ClockSelection   = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Lpuart1ClockSelection  = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
#endif

  return;
}
/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

static void Config_HSE(void)
{
  OTP_ID0_t * p_otp;

  if ( (HSETuningInfo & HSE_TRIM_VALID_MSK) == HSE_TRIM_VALID)
  {
    /* Read HSE_Tuning from Flash */
    LL_RCC_HSE_SetCapacitorTuning( (uint8_t) (HSETuningInfo & 0xFF));
  } else {
    /**
     * Read HSE_Tuning from OTP
     */
    p_otp = (OTP_ID0_t *) OTP_Read(0);
    if (p_otp)
    {
      LL_RCC_HSE_SetCapacitorTuning(p_otp->hse_tuning);
    }
  }

  return;
}


static void Reset_Device( void )
{
#if ( CFG_HW_RESET_BY_FW == 1 )
  Reset_BackupDomain();

  Reset_IPCC();
#endif

  return;
}

static void Reset_IPCC( void )
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

static void Reset_BackupDomain( void )
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

static void Init_Exti( void )
{
  /* Enable LPUART(25), IPCC(36), HSEM(38) wakeup interrupts on CPU1 */
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_25 | LL_EXTI_LINE_36 | LL_EXTI_LINE_38);

  return;
}

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
/**
 * @brief As the default systick is not used, declare here, at least, an empty function to 
 * over-write the default one as it declared as WEAK in HAL.
 */
void HAL_Delay(__IO uint32_t Delay)
{
  us_delay(Delay*1000);
  return;
}

/**
 * @brief Declare here empty functions to over-write the default one as it declared as WEAK in HAL.
 * This is the way to avoid systick use which is initialized by default in HAL_Init() and suspended or resumed in LPM.
 *
 * @param None
 */
HAL_StatusTypeDef HAL_InitTick( uint32_t TickPriority )
{
  return (HAL_OK);
}

void HAL_SuspendTick(void)
{
}

void HAL_ResumeTick(void)
{
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @brief  Update the default HSE tuning value. Doesn't require OTP memory,
  *         instead uses RTC Backup register
  * @param  hse_tuning: New HSE tuning value (0..63)
  * @retval None
  */
void Config_HSE_tuning(const uint32_t hse_tuning)
{
  pHSETuningInfo = (volatile uint32_t*)&HSETuningInfo;

  /* Unlock the Flash to enable the flash control register access *************/
  if (HAL_FLASH_Unlock() != HAL_OK)
  {
    APP_DBG("Unlock of flash failed");
    return;
  }

  /* Erase flash beforehand */
  EraseUserFlash( (uint32_t) pHSETuningInfo);

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
  if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)&HSETuningInfo,
                        (uint64_t) hse_tuning) != HAL_OK)
  {
    APP_DBG("New HSE tuning operation failed");
  }
}
