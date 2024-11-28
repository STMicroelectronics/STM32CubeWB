/* USER CODE BEGIN Header */
/**
  ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @brief   RF LLD tests application
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
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "app_entry.h"
#include "main.h"

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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
RTC_HandleTypeDef hrtc;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_DMA_Init(void);
static void MX_RTC_Init(void);

/* USER CODE BEGIN PFP */
static void SystemClock_Config(void);
static void PeriphClock_Config(void);
static void Reset_Device( void );
static void Reset_IPCC( void );
static void Reset_BackupDomain( void );
static void Config_HSE(void);
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
  Config_HSE();
  /* USER CODE END Init */

  /* Configure the system clock on HSE without using PLL and the periph clock needed by this application */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  PeriphClock_Config();
  Init_Exti();
  //Init_Smps();

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_DMA_Init();
  MX_RTC_Init();

  /* Init code for STM32_WPAN */
  APPE_Init();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
    /* USER CODE END WHILE */

    /* USER CODE END WHILE */
  }
  /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
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
}

/**
  * @brief System Clock Configuration : API to be called to use MSI (with or without PLL use) as 32Mhz system clock.
           SystemClock_Config_HSE() must be called once just after boot (to go from default MSI to HSE).
           Then application user can call both SystemClock_Config_HSE() and SystemClock_Config_MSI() at any time.
  * @retval None
  */
void SystemClock_Config_MSI(uint32_t usePLL)
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
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  else
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

/* HSE cannot be stopped while using RF */
#if 0
  /* Disable HSE Oscillator as the HSE is no more needed by the application */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState        = RCC_HSE_OFF;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_NONE;  /* No update on PLL */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
#endif
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
/**
  * @brief System Clock Configuration : must be called during application start-up
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Configure LSE Drive Capability */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  #ifdef STM32WB15xx
  #else
  /* Configure the main internal regulator output voltage */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  #endif
  
  /* Assuming that MSI is enabled by default after boot, lets go to HSE without using PLL */
  SystemClock_Config_HSE(0);

  /* Configure Others clock */
  #ifdef STM32WB15xx
  RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI |  
                                           RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI2;
  #else
  RCC_OscInitStruct.OscillatorType       = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48 | 
                                           RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI2;
  RCC_OscInitStruct.HSI48State           = RCC_HSI48_OFF;
  #endif
  RCC_OscInitStruct.LSEState             = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState             = RCC_HSI_OFF;
  RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState             = RCC_LSI_OFF;
  RCC_OscInitStruct.LSI2CalibrationValue = 0;
  RCC_OscInitStruct.PLL.PLLState         = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

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

void Init_Exti( void )
{
  /* Enable LPUART(25), IPCC(36), HSEM(38) wakeup interrupts on CPU1 */
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_25 | LL_EXTI_LINE_36 | LL_EXTI_LINE_38);

  return;
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = CFG_RTC_ASYNCH_PRESCALER;
  hrtc.Init.SynchPrediv = CFG_RTC_SYNCH_PRESCALER;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);

  LL_RTC_WAKEUP_SetClock(RTC, CFG_RTC_WUCKSEL_DIVIDER);

  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);
  /* USER CODE END RTC_Init 2 */

}

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
    LL_LPM_EnableSleep( ); /**< Clear SLEEPDEEP bit of Cortex System Control Register */

    /**
     * This option is used to ensure that store operations are completed
     */
  #if defined (__CC_ARM) || defined (__ARMCC_VERSION)
    __force_stores();
  #endif

    __WFI( );
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  BSP_LED_On(LED_BLUE);
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  /* USER CODE END Error_Handler */
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
  /* USER CODE BEGIN assert_failed */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END assert_failed */
}
#endif /* USE_FULL_ASSERT */
