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
  RUN_MODE_UP_TO_48MHZ   = 3
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

RunMode_Typedef RunMode_Next = RUN_MODE_DOWN_TO_16MHZ;
__IO uint8_t ubSequenceRunModes_OnGoing = 1;
#ifdef USE_LED  
__IO uint16_t uhLedBlinkSpeed = LED_BLINK_FAST;
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

#ifdef USE_LED
void     LED_Blinking(void);
#endif
void     EnterRunMode_DownTo16MHz(void);
void     EnterRunMode_LowPower_DownTo100KHz(void);
void     EnterRunMode_UpTo16MHz(void);
void     EnterRunMode_UpTo48MHz(void);

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

  /* Remain in this loop until end of sequences of different run modes */
  while(ubSequenceRunModes_OnGoing != 0)
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
    /* Led blinking in infinite loop */
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  }

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* MSI configuration and activation */
  LL_RCC_MSI_Enable();
  while(LL_RCC_MSI_IsReady() != 1)
  {
  }

  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_11);
  /* Sysclk activation on the MSI */
  /* Set CPU1 prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set CPU2 prescaler*/
  LL_C2_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
  {
  }

  /* Set AHB SHARED prescaler*/
  LL_RCC_SetAHB4Prescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set APB2 prescaler*/
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(48000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(48000000);
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  LL_RCC_SetSMPSClockSource(LL_RCC_SMPS_CLKSOURCE_HSI);
  LL_RCC_SetSMPSPrescaler(LL_RCC_SMPS_DIV_1);
  LL_RCC_SetRFWKPClockSource(LL_RCC_RFWKP_CLKSOURCE_NONE);
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
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);

  /**/
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
  EXTI_InitStruct.Line_32_63 = LL_EXTI_LINE_NONE;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(B1_GPIO_Port, B1_Pin, LL_GPIO_PULL_UP);

  /**/
  LL_GPIO_SetPinMode(B1_GPIO_Port, B1_Pin, LL_GPIO_MODE_INPUT);

  /**/
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

#ifdef USE_LED
/**
  * @brief  Set LED2 to Blinking mode (Shall be call in a Loop).
  * @param  None
  * @retval None
  */
void LED_Blinking(void)
{
  /* Toggle IO. This function shall be called in a loop to toggle */
  LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
  LL_mDelay(uhLedBlinkSpeed);
}
#endif

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
  };
  
  /* Set MSI to 16MHz */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_8);
  
  /* Disable PLL to decrease power consumption */
  LL_RCC_PLL_Disable();
  while(LL_RCC_PLL_IsReady() != 0) 
  {
  };
  LL_RCC_PLL_DisableDomain_SYS();

  /* Set systick to 1ms in using frequency set to 16MHz */
  LL_Init1msTick(16 * 1000000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(16 * 1000000);

  /* 2 - Adjust Flash Wait state after decrease Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
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

  /* 2 - Adjust Flash Wait state before increase Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  
  /* Wait for flash latency setting effective before increase clock frequency */
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  };
  
  /* 3 - Set Frequency to 16MHz (MSI) */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_8);
  /* Set systick to 1ms in using frequency set to 16MHz */
  LL_Init1msTick(16 * 1000000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(16 * 1000000);
}

/**
  * @brief  Function to increase Frequency at 48MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_UpTo48MHz(void)
{
  /* Set back same system configuration as before sequences of different run modes */
  SystemClock_Config();
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
  if(ubSequenceRunModes_OnGoing != 0)
  {
    switch(RunMode_Next)
    {
    case RUN_MODE_DOWN_TO_16MHZ:
      {
        /* Decrease core frequency and voltage
         * Frequency: 48MHz -> 16MHz
         * Voltage Scaling Range 2
         */
        EnterRunMode_DownTo16MHz();
  #ifdef USE_LED
        uhLedBlinkSpeed = LED_BLINK_MEDIUM;
  #endif
        /* Set Next RunMode to execute */
        RunMode_Next = RUN_MODE_DOWN_TO_100KHZ;
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
        RunMode_Next = RUN_MODE_UP_TO_16MHZ;
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
        RunMode_Next = RUN_MODE_UP_TO_48MHZ;
        break;
      }
    case RUN_MODE_UP_TO_48MHZ:
      {
        /* Increase core frequency and voltage:
         * Frequency: 16MHz -> 48MHz
         * Voltage Scaling Range 1
         */
        EnterRunMode_UpTo48MHz();
  #ifdef USE_LED
        uhLedBlinkSpeed = LED_BLINK_FAST;
  #endif
  
        /* Exit Test */
        ubSequenceRunModes_OnGoing = 0;
        break;
      }
    }
  }
  else
  {
    /* End of sequences of different run modes, no action */
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
