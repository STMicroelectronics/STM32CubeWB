/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_OptimizedRunMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to enter or exit Low Power Run mode and update
  *          the core frequency on the fly through the STM32WBxx PWR LL API.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define USE_LED

typedef enum {
  RUN_MODE_DOWN_TO_16MHZ = 0,
  RUN_MODE_DOWN_TO_100KHZ  = 1,
  RUN_MODE_UP_TO_16MHZ   = 2,
  RUN_MODE_UP_TO_64MHZ   = 3
}RunMode_Typedef;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

RunMode_Typedef RunMode = RUN_MODE_DOWN_TO_16MHZ;
__IO uint8_t ubExecuteRunMode = 1;
#ifdef USE_LED  
__IO uint16_t uhLedBlinkSpeed = LED_BLINK_FAST;
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

void     Configure_PWR(void);
#ifdef USE_LED
void     LED_Init(void);
void     LED_Blinking(void);
#endif
void     EnterRunMode_DownTo16MHz(void);
void     EnterRunMode_LowPower_DownTo100KHz(void);
void     EnterRunMode_UpTo16MHz(void);
void     EnterRunMode_UpTo64MHz(void);

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


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

#ifdef USE_LED
  /* Initialize LED2 */
  LED_Init();
#endif

  /* Configure Power IP */
  Configure_PWR();

  while(ubExecuteRunMode)
  {
#ifdef USE_LED   
    /* Led blinking until User push-button action */
    LED_Blinking();
#endif
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

#ifdef USE_LED   
    /* Led blinking in infinite loop*/
    LED_Blinking();
#endif
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  /* MSI configuration and activation */
  LL_RCC_MSI_Enable();
  while(LL_RCC_MSI_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 32, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Sysclk activation on the main PLL */
  /* Set CPU1 prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set CPU2 prescaler*/
  LL_C2_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set AHB SHARED prescaler*/
  LL_RCC_SetAHB4Prescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set APB2 prescaler*/
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(64000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE4);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_4;
  EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_PULL_UP);

  /**/
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_MODE_INPUT);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0));
  NVIC_EnableIRQ(EXTI4_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

#ifdef USE_LED
/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Set LED2 to Blinking mode (Shall be call in a Loop).
  * @param  None
  * @retval None
  */
void LED_Blinking(void)
{
  /* Toggle IO. Shall be call in a loop to toggle */
  LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
  LL_mDelay(uhLedBlinkSpeed);
}
#endif

/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL, 
                                         LL_GPIO_PULL_NO, LL_GPIO_AF_0};
  
  /* Set unused GPIO ports with all pins in analog state to reduce            */
  /* power consumption.                                                       */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.                    */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD |
                           LL_AHB2_GRP1_PERIPH_GPIOE |
                           LL_AHB2_GRP1_PERIPH_GPIOH);
  
  LL_GPIO_Init(GPIOD, &gpio_initstruct);
  LL_GPIO_Init(GPIOE, &gpio_initstruct);
  LL_GPIO_Init(GPIOH, &gpio_initstruct);
  
  LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOD |
                            LL_AHB2_GRP1_PERIPH_GPIOE |
                            LL_AHB2_GRP1_PERIPH_GPIOH);


  /* Ensure that MSI is wake-up system clock */ 
  /* MSI is the default value. Not mandatory to call function in this case */
  //LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_MSI);
}

/**
  * @brief  Function to decrease Frequency at 16MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_DownTo16MHz(void)
{
  /* 1 - Switch clock source on MSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);    
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI) 
  {
  }
  
  /* Set MSI to 16MHz */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_8);
  
  /* Disable PLL to decrease power consumption */
  LL_RCC_PLL_Disable();
  while(LL_RCC_PLL_IsReady() != 0) 
  {
  }
  LL_RCC_PLL_DisableDomain_SYS();

  /* Set systick to 1ms in using frequency set to 16MHz */
  LL_Init1msTick(16 * 1000000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(16 * 1000000);

  /* 2 - Adjust Flash Wait state after decrease Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* 3 - Set Voltage scaling to LL_PWR_REGU_VOLTAGE_SCALE2. Decrease VCore  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
}

/**
  * @brief  Function to decrease Frequency at 100KHZ in Low Power Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_LowPower_DownTo100KHz(void)
{
  /* 1 - Set Frequency to 100KHz to activate Low Power Run Mode: 100KHz */
  /* Range Selection already enabled. Need to change Range only */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_0);
  /* Set systick to 1ms in using frequency set to 100KHz */
  LL_Init1msTick(100 * 1000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(100 * 1000);

  /* 2 - Adjust Flash Wait state after decrease Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* Voltage Scaling already set to LL_PWR_REGU_VOLTAGE_SCALE2. VCore already decreased */

  /* 3 - Activate Low Power Run Mode */
  LL_PWR_EnterLowPowerRunMode();
}

/**
  * @brief  Function to increase Frequency at 16MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_UpTo16MHz(void)
{
  /* 1 - Deactivate Low Power Run Mode to increase Frequency up to 16MHz */
  LL_PWR_ExitLowPowerRunMode();

  /* Voltage Scaling already set to LL_PWR_REGU_VOLTAGE_SCALE2. VCore already decreased */

  /* 2 - Adjust Flash Wait state before increase Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  
  /* Wait for flash latency setting effective before increase clock frequency */
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  
  /* 3 - Set Frequency to 16MHz (MSI) */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_8);
  /* Set systick to 1ms in using frequency set to 16MHz */
  LL_Init1msTick(16 * 1000000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(16 * 1000000);
}

/**
  * @brief  Function to increase Frequency at 64MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_UpTo64MHz(void)
{
  /* 1 - Set Voltage scaling to LL_PWR_REGU_VOLTAGE_SCALE1 before increase Clock Frequency */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

  /* 2 - Wait Voltage Scaling 1 before increase frequency */
  while(LL_PWR_IsActiveFlag_VOS() !=  0)
  {
  }

  /* 3 - Adjust Flash Wait state before increase Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

  /* Wait for flash latency setting effective before increase clock frequency */
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3)
  {
  }
  
  /* 4 - Set Frequency to 64MHz (PLL) */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  /* Enable PLL and wait for activation */ 
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  }

  /* Switch on PLL. Previous configuration done by SystemClock_Config is used */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  }

  /* Set systick to 1ms in using frequency set to 64MHz */
  LL_Init1msTick(64 * 1000000);

  /* Update CMSIS variable */
  LL_SetSystemCoreClock(64 * 1000000);
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage BUTTON IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  switch(RunMode)
  {
  case RUN_MODE_DOWN_TO_16MHZ:
    {
      /* Decrease core frequency and voltage
       * Frequency: 64MHz -> 16MHz
       * Voltage Scaling Range 2
       */
      EnterRunMode_DownTo16MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_MEDIUM;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_DOWN_TO_100KHZ;
      break;
    }
  case RUN_MODE_DOWN_TO_100KHZ:
    {
      /* Decrease core frequency and enter Low Power Run mode
       * Frequency: 100KHz
       * Voltage Scaling Range 2
       * LowPowerRunMode activated
       */
      EnterRunMode_LowPower_DownTo100KHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_SLOW;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_UP_TO_16MHZ;
      break;
    }
  case RUN_MODE_UP_TO_16MHZ:
    {
      /* Increase core frequency and exit Low Power Run mode 
       * Frequency: 100KHz -> 16MHz
       * Voltage Scaling Range 2
       * LowPowerRunMode deactivated
       */
      EnterRunMode_UpTo16MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_MEDIUM;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_UP_TO_64MHZ;
      break;
    }
  case RUN_MODE_UP_TO_64MHZ:
    {
      /* Increase core frequency and voltage:
       * Frequency: 16MHz -> 64MHz
       * Voltage Scaling Range 1
       */
      EnterRunMode_UpTo64MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_FAST;
#endif
      /* Exit Test */
      NVIC_DisableIRQ(USER_BUTTON_EXTI_IRQn); 
      USER_BUTTON_EXTI_LINE_DISABLE();
      
      ubExecuteRunMode = 0;
      break;
    }
  }
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
