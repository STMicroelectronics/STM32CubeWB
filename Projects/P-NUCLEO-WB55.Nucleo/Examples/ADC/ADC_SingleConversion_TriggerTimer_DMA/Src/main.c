/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples/ADC/ADC_SingleConversion_TriggerTimer_DMA/Src/main.c
  * @author  MCD Application Team
  * @brief   Use ADC to convert a single channel at each trig from timer.
  *          Conversion data are transferred by DMA into a table,
  *          indefinitely (circular mode).
  *          Example using the STM32WBxx ADC HAL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;
/* Variables for ADC conversion data */
__IO   uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADC group regular conversion data (array of data) */

/* Variables for ADC conversion data computation to physical values */
uint16_t   aADCxConvertedData_Voltage_mVolt[ADC_CONVERTED_DATA_BUFFER_SIZE];  /* Value of voltage calculated from ADC conversion data (unit: mV) (array of data) */

/* Variable to report status of DMA transfer of ADC group regular conversions */
/*  0: DMA transfer is not completed                                          */
/*  1: DMA transfer is completed                                              */
/*  2: DMA transfer has not yet been started yet (initial state)              */
__IO   uint8_t ubDmaTransferStatus = 2; /* Variable set into DMA interruption callback */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void Configure_ADC(void);
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
  uint32_t tmp_index_adc_converted_data = 0;
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  for (tmp_index_adc_converted_data = 0; tmp_index_adc_converted_data < ADC_CONVERTED_DATA_BUFFER_SIZE; tmp_index_adc_converted_data++)
  {
    aADCxConvertedData[tmp_index_adc_converted_data] = VAR_CONVERTED_DATA_INIT_VALUE;
  }
  
  /* Initialize LED on board */
  BSP_LED_Init(LED2);
  
 /* Configure ADC */
  /* Note: This function configures the ADC but does not enable it.           */
  /*       Only ADC internal voltage regulator is enabled by function         */
  /*       "HAL_ADC_Init()".                                                  */
  /*       To activate ADC (ADC enable and ADC conversion start), use         */
  /*       function "HAL_ADC_Start_xxx()".                                    */
  /*       This is intended to optimize power consumption:                    */
  /*       1. ADC configuration can be done once at the beginning             */
  /*          (ADC disabled, minimal power consumption)                       */
  /*       2. ADC enable (higher power consumption) can be done just before   */
  /*          ADC conversions needed.                                         */
  /*          Then, possible to perform successive ADC activation and         */
  /*          deactivation without having to set again ADC configuration.     */
  Configure_ADC();
  
  /* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /*## Enable Timer ########################################################*/
  if (HAL_TIM_Base_Start(&htim2) != HAL_OK)
  {
    /* Counter enable error */
    Error_Handler();
  }
  
  /*## Start ADC conversions ###############################################*/
  /* Start ADC group regular conversion with DMA */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedData,
                        ADC_CONVERTED_DATA_BUFFER_SIZE
                       ) != HAL_OK)
  {
    /* ADC conversion start error */
    Error_Handler();
  }  
  
  while (1)
  {
    /* Note: At this step, a voltage can be supplied to ADC channel input     */
    /*       (by connecting an external signal voltage generator to the       */
    /*       analog input pin) to perform a ADC conversion on a determined    */
    /*       voltage level.                                                   */
    /*       Otherwise, ADC channel input can be let floating, in this case   */
    /*       ADC conversion data will be undetermined.                        */

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Note: LED state depending on DMA transfer status is set into DMA       */
    /*       IRQ handler, refer to functions "HAL_ADC_ConvCpltCallback()"     */
    /*       and "HAL_ADC_ConvHalfCpltCallback()".                            */

    /* Note: ADC conversions data are stored into array                       */
    /*       "aADCConvertedData"                                              */
    /*       (for debug: see variable content into watch window).             */
    
    /* Note: ADC conversion data are computed to physical values              */
    /*       into array "aADCxConvertedData_Voltage_mVolt"                    */
    /*       using helper macro "__ADC_CALC_DATA_VOLTAGE()".                  */
    /*       (for debug: see variable content into watch window).             */

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
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV5;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 39999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Configure ADC (ADC instance: ADCx) and GPIO used by ADC channels.
  *         Configuration of GPIO:
  *           - Pin:                    PA.01 (on this STM32 device, ADC1 channel 6 is mapped on this GPIO)
  *           - Mode:                   analog
  *         Configuration of ADC:
  *         - Common to several ADC:
  *           - Conversion clock:       Synchronous from PCLK
  *           - Internal path:          None                         (default configuration from reset state)
  *         - Multimode
  *           Feature not used: all parameters let to default configuration from reset state
  *           - Mode                    Independent                  (default configuration from reset state)
  *           - DMA transfer:           Disabled                     (default configuration from reset state)
  *           - Delay sampling phases   1 ADC clock cycle            (default configuration from reset state)
  *         - ADC instance
  *           - Resolution:             12 bits                      (default configuration from reset state)
  *           - Data alignment:         right aligned                (default configuration from reset state)
  *           - Low power mode:         disabled                     (default configuration from reset state)
  *           - Offset:                 none                         (default configuration from reset state)
  *         - Group regular
  *           - Trigger source:         external trigger from TIM2
  *           - Trigger edge:           rising                       (default configuration from reset state)
  *           - Continuous mode:        single conversion            (default configuration from reset state)
  *           - DMA transfer:           enabled, unlimited requests
  *           - Overrun:                data overwritten
  *           - Sequencer length:       disabled: 1 rank             (default configuration from reset state)
  *           - Sequencer discont:      disabled: sequence done in 1 scan (default configuration from reset state)
  *           - Sequencer rank 1:       ADCx ADCx_CHANNELa
  *         - Group injected
  *           Feature not used: all parameters let to default configuration from reset state
  *           - Trigger source:         SW start                     (default configuration from reset state)
  *           - Trigger edge:           not applicable with SW start
  *           - Auto injection:         disabled                     (default configuration from reset state)
  *           - Contexts queue:         disabled                     (default configuration from reset state)
  *           - Sequencer length:       disabled: 1 rank             (default configuration from reset state)
  *           - Sequencer discont:      disabled: sequence done in 1 scan (default configuration from reset state)
  *           - Sequencer rank 1:       first channel available      (default configuration from reset state)
  *         - Channel
  *           - Sampling time:          ADCx ADCx_CHANNELa set to sampling time 247.5 ADC clock cycles (on this STM32 serie, sampling time is channel wise)
  *           - Differential mode:      single ended                 (default configuration from reset state)
  *         - Analog watchdog
  *           Feature not used: all parameters let to default configuration from reset state
  *           - AWD number:             1
  *           - Monitored channels:     none                         (default configuration from reset state)
  *           - Threshold high:         0x000                        (default configuration from reset state)
  *           - Threshold low:          0xFFF                        (default configuration from reset state)
  *         - Oversampling
  *           Feature not used: all parameters let to default configuration from reset state
  *           - Scope:                  none                         (default configuration from reset state)
  *           - Discontinuous mode:     disabled                     (default configuration from reset state)
  *           - Ratio:                  2                            (default configuration from reset state)
  *           - Shift:                  none                         (default configuration from reset state)
  *         - Interruptions
  *           None: with HAL driver, ADC interruptions are set using
  *           function "HAL_ADC_start_xxx()".
  * @note   Using HAL driver, configuration of GPIO used by ADC channels,
  *         NVIC and clock source at top level (RCC)
  *         are not implemented into this function,
  *         must be implemented into function "HAL_ADC_MspInit()".
  * @param  None
  * @retval None
  */
__STATIC_INLINE void Configure_ADC(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  
  /*## Configuration of ADC ##################################################*/
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## common to several ADC, ADC instance, ADC group regular  ###############*/
  
  /* Set ADC instance of HAL ADC handle AdcHandle */
  AdcHandle.Instance = ADCx;
  
  /* Configuration of HAL ADC handle init structure:                          */
  /* parameters of scope ADC instance and ADC group regular.                  */
  /* Note: On this STM32 serie, ADC group regular sequencer is                */
  /*       fully configurable: sequencer length and each rank                 */
  /*       affectation to a channel are configurable.                         */
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */
  AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trig of conversion by software start (no external event) */
  AdcHandle.Init.DMAContinuousRequests = ENABLE;                        /* ADC with DMA transfer: continuous requests to DMA to match with DMA configured in circular mode */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  AdcHandle.Init.OversamplingMode      = DISABLE;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
  
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## ADC group injected and channels mapped on group injected ##############*/
  
  /* Note: ADC group injected not used and not configured in this example.    */
  /*       Refer to other ADC examples using this feature.                    */
  /* Note: Call of the functions below are commented because they are         */
  /*       useless in this example:                                           */
  /*       setting corresponding to default configuration from reset state.   */
  
  
  /*## Configuration of ADC hierarchical scope: ##############################*/
  /*## channels mapped on group regular         ##############################*/
  
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: On this STM32 serie, ADC group regular sequencer is                */
  /*       fully configurable: sequencer length and each rank                 */
  /*       affectation to a channel are configurable.                         */
  /* Note: Considering IT occurring after each ADC conversion                 */
  /*       (IT by ADC group regular end of unitary conversion),               */
  /*       select sampling time and ADC clock with sufficient                 */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADCx_CHANNELa;               /* ADC channel selection */
  sConfig.Rank         = ADC_REGULAR_RANK_1;          /* ADC group regular rank in which is mapped the selected ADC channel */
  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;  /* ADC channel sampling time */
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* ADC channel differential mode */
  sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* ADC channel affected to offset number */
  sConfig.Offset       = 0;                           /* Parameter discarded because offset correction is disabled */
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  
  /*## Configuration of ADC hierarchical scope: multimode ####################*/
  /* Note: ADC multimode is not available on this device:                     */
  /*       only 1 ADC instance is present.                                    */
  
  
  /*## Configuration of ADC transversal scope: analog watchdog ###############*/
  
  /* Note: ADC analog watchdog not used and not configured in this example.   */
  /*       Refer to other ADC examples using this feature.                    */
  
  
  /*## Configuration of ADC transversal scope: oversampling ##################*/
  
  /* Note: ADC oversampling not used and not configured in this example.      */
  /*       Refer to other ADC examples using this feature.                    */
  
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/


/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  uint32_t tmp_index = 0;
  
  /* Computation of ADC conversions raw data to physical values               */
  /* using LL ADC driver helper macro.                                        */
  /* Management of the 2nd half of the buffer */
  for (tmp_index = (ADC_CONVERTED_DATA_BUFFER_SIZE/2); tmp_index < ADC_CONVERTED_DATA_BUFFER_SIZE; tmp_index++)
  {
    aADCxConvertedData_Voltage_mVolt[tmp_index] = __ADC_CALC_DATA_VOLTAGE(VDDA_APPLI, aADCxConvertedData[tmp_index]);
  }
  
  /* Update status variable of DMA transfer */
  ubDmaTransferStatus = 1;
  
  /* Set LED depending on DMA transfer status */
  /* - Turn-on if DMA transfer is completed */
  /* - Turn-off if DMA transfer is not completed */
  BSP_LED_On(LED2);
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  uint32_t tmp_index = 0;
  
  /* Computation of ADC conversions raw data to physical values               */
  /* using LL ADC driver helper macro.                                        */
  /* Management of the 1st half of the buffer */
  for (tmp_index = 0; tmp_index < (ADC_CONVERTED_DATA_BUFFER_SIZE/2); tmp_index++)
  {
    aADCxConvertedData_Voltage_mVolt[tmp_index] = __ADC_CALC_DATA_VOLTAGE(VDDA_APPLI, aADCxConvertedData[tmp_index]);
  }
  
  /* Update status variable of DMA transfer */
  ubDmaTransferStatus = 0;
  
  /* Set LED depending on DMA transfer status */
  /* - Turn-on if DMA transfer is completed */
  /* - Turn-off if DMA transfer is not completed */
  BSP_LED_Off(LED2);
}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
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
  while(1) 
  {
    /* Toggle LED2 */
    BSP_LED_Off(LED2);
    HAL_Delay(800);
    BSP_LED_On(LED2);
    HAL_Delay(10);
    BSP_LED_Off(LED2);
    HAL_Delay(180);
    BSP_LED_On(LED2);
    HAL_Delay(10);
  }
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
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
