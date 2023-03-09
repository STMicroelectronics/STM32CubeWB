/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to enter and exit the standby mode with
  *          a wakeup pin or external reset through the STM32WBxx PWR LL API.
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

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BUTTON_MODE_GPIO  0
#define BUTTON_MODE_EXTI  1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t LedSpeed = LED_BLINK_FAST;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

void     Configure_PWR(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(uint32_t Button_Mode);
uint32_t UserButton_GetState(void);
void     EnterStandbyMode(void);

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
  /* Initialize User push-button (SW1) in GPIO mode */
  UserButton_Init(BUTTON_MODE_GPIO);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* Configure Power IP */
  Configure_PWR();

  /* Initialize User push-button (SW1) in EXTI mode */
  UserButton_Init(BUTTON_MODE_EXTI);
  /* Led blinking in RUN mode */
  LED_Blinking(LedSpeed);  
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Sysclk activation on the HSI */
  /* Set CPU1 prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set CPU2 prescaler*/
  LL_C2_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  /* Set AHB SHARED prescaler*/
  LL_RCC_SetAHB4Prescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set APB2 prescaler*/
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(16000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
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
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);

  /**/
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button (SW1) in GPIO or EXTI Line Mode.
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */
void UserButton_Init(uint32_t Button_Mode)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_UP);

  if(Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Connect External Line to the GPIO */
    USER_BUTTON_SYSCFG_SET_EXTI();
    
    /* Enable a rising trigger EXTI line 13 Interrupt */
    USER_BUTTON_EXTI_LINE_ENABLE();
    USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
    
    /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
    NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
    NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  None
  * @retval The Button GPIO pin value.
  */
uint32_t UserButton_GetState(void)
{
  return LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
}


/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Check if the system was resumed from Standby mode */
  /* Note: On STM32WB, both CPU1 and CPU2 must be in Standby mode to set the entire system in Standby mode */
  if(   (LL_PWR_IsActiveFlag_C1SB() != 0)
     && (LL_PWR_IsActiveFlag_C2SB() != 0)
    )
  {
    /* Clear Standby flag */
    LL_PWR_ClearFlag_C1STOP_C1STB();
    LL_PWR_ClearFlag_C1STOP_C1STB();
    
    /* Change LED speed to SLOW to indicate exit from Standby mode */
    LedSpeed = LED_BLINK_SLOW;
    /* Wait that user release the User push-button (SW1) */
    while(UserButton_GetState() == 0){}
  }
  
  /* Specific procedure on STM32WB, in case of initial power-up and RF stack no started */
  /* Note: This procedure is required when user application wants to request  */
  /*       a low-power mode in the particular case:                           */
  /*       - RF stack not started: On STM32WB, system low-power mode is fixed */
  /*         by the deepest low-power modes of each sub-system (CPU1,         */
  /*         CPU2, RF).                                                       */
  /*         Standard case is RF stack started and managing low-power modes   */
  /*         of CPU2 and RF.                                                  */
  /*         In case of RF stack not started, CPU2 low-power mode must be     */
  /*         forced to the lowest level. This allows to require all system    */
  /*         low-power modes using only PWR for CPU1.                         */
  /*         low-power mode.                                                  */
  /*       - Initial power-up: In case of power-on reset, CPU2 low-power mode */
  /*         has its reset value and must be set.                             */
  /*         In case of system is resumed from low-power mode standby         */
  /*         or shutdown, configuration of PWR parameters related to CPU2 are */
  /*         retained and must not modified (This check is required in case   */
  /*         of RF stack started afterwards and to not overwritte its         */
  /*         low-power configuration).                                        */
  if(   (LL_PWR_IsActiveFlag_C1SB() == 0)
     || (LL_PWR_IsActiveFlag_C2SB() == 0)
    )
  {
    /* Set the lowest low-power mode for CPU2: shutdown mode */
    LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);
  }

  /* Check and Clear the Wakeup flag */
  if (LL_PWR_IsActiveFlag_WU2() != 0)
  {
    LL_PWR_ClearFlag_WU2();
  }
}

/**
  * @brief  Function to configure and enter in Standby Mode.
  * @param  None
  * @retval None
  */
void EnterStandbyMode(void)
{
  /* Wait that user release the User push-button (SW1) */
  while(UserButton_GetState() == 0){}
  
  /* Turn-off LED */
  /* Note: LED state at this step depends on blinking state at the instant of user button is pressed. */
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
  
  /* Disable all used wakeup sources */
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  
  /* Clear all wake up Flag */
  LL_PWR_ClearFlag_WU();
  
  /* Enable pull up on wakeup pin */
  /* Note: Setting not mandatory but recommended since there is no external pulling resistor on pin PC13 on STM32WB Nucleo board */
  LL_PWR_EnableGPIOPullUp(LL_PWR_GPIO_C, LL_PWR_GPIO_BIT_13);
  
  /* Enable pull-up and pull-down configuration for CPU1 */
  LL_PWR_EnablePUPDCfg();
  
  /* Set wakeup pin polarity */
  LL_PWR_SetWakeUpPinPolarityLow(LL_PWR_WAKEUP_PIN2);

  /* Enable wakeup pin */
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  
  /* As default User push-button (SW1) state is high level, need to clear all wake up Flag again */
  LL_PWR_ClearFlag_WU();

  /** Request to enter Standby mode
    * Following procedure describe in STM32WBxx Reference Manual
    * See PWR part, section Low-power modes, Standby mode
    */
  /* Set Standby mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();
  
  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
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
  /* Configure and enter in Standby Mode */
  EnterStandbyMode();
  
  /* Here Device is in Standby mode */
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
