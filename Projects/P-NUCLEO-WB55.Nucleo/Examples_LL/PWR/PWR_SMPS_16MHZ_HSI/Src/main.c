/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_SMPS_16MHZ_HSI/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use power converters of STM32WB
  *          (SMPS, LDO and LP-LDO) through the STM32WBxx PWR LL API.
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
#define BUTTON_MODE_GPIO  0
#define BUTTON_MODE_EXTI  1

/* RF stack configuration */
//#define RF_STACK_AVAILABLE              /* RF stacked loaded in STM32WB: it will start once CPU2 is booted and manage low-power mode of CPU2 and RF parts. */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  
#if defined(RF_STACK_AVAILABLE)
  /* Start CPU2 to let RF stack manage low-power mode of CPU2 and RF parts */
  LL_PWR_EnableBootC2();
#endif
  
  /* Initialize User push-button (SW1) in EXTI mode */
  UserButton_Init(BUTTON_MODE_EXTI);
  
  /* Configure Power IP */
  Configure_PWR();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* Toggle IO in an infinite loop */
    LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);  
    
    /* Get SMPS effective operating mode */
    if(LL_PWR_SMPS_GetEffectiveMode() == LL_PWR_SMPS_STEP_DOWN)
    {
      /* SMPS effective operating mode: step-down */
      LL_mDelay(LED_BLINK_FAST);
    }
    else
    {
      /* SMPS effective operating mode: bypass */
      LL_mDelay(LED_BLINK_SLOW);
    }
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
    NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 0x03);  
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
  /* Check if the system was resumed from standby mode */
  /* Note: On STM32WB, both CPU1 and CPU2 must be in standby mode to set the entire system in standby mode */
  if(   (LL_PWR_IsActiveFlag_C1SB() != 0)
     && (LL_PWR_IsActiveFlag_C2SB() != 0)
    )
  {
    /* Clear standby flag */
    LL_PWR_ClearFlag_C1STOP_C1STB();
    LL_PWR_ClearFlag_C1STOP_C1STB();
    
    /* Wait that user release the User push-button (SW1) */
    while(UserButton_GetState() == 0){}
    
    /* Turn-on LED during 1 second to indicate wake-up from standby mode */
    LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
    LL_mDelay(1000);
    LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
  }
  
  /*## Configuration of NVIC ###############################################*/
  /* Configure NVIC to enable PWR interruptions */
  NVIC_SetPriority(PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQn, 0);
  NVIC_EnableIRQ(PWR_SOTF_BLEACT_802ACT_RFPHASE_IRQn);
  
  /*## Configuration of SMPS ###############################################*/
  /* Configure SMPS step down converter clock source */
  /* Note: Setting already done in function "SystemClock_Config()" before     */
  /*       disabling MSI.                                                     */
  LL_RCC_SetSMPSClockSource(LL_RCC_SMPS_CLKSOURCE_HSI);
  
  /* Set SMPS prescaler */
  /* Note: The system must always be configured so as to get a SMPS Step Down */
  /*       converter clock frequency between 2 MHz and 8 MHz.                 */
  /*       With setting of this example (SMPS clock source HSI 16 MHz):       */
  /*       clock div 0 and additional div 2, SMPS clock source is 8Mhz.       */
  LL_RCC_SetSMPSPrescaler(LL_RCC_SMPS_DIV_0);

  /* Set BOR configuration */
  LL_PWR_SetBORConfig(LL_PWR_BOR_SMPS_FORCE_BYPASS);
  
  /* Set SMPS step down converter supply startup current selection */
  LL_PWR_SMPS_SetStartupCurrent(LL_PWR_SMPS_STARTUP_CURRENT_80MA);
  
  /* Set SMPS step down converter output voltage scaling */
  LL_PWR_SMPS_SetOutputVoltageLevel(LL_PWR_SMPS_OUTPUT_VOLTAGE_1V40);
  
  /* Set SMPS operating mode */
  LL_PWR_SMPS_SetMode(LL_PWR_SMPS_STEP_DOWN);

  /* Check SMPS clock source */
  /* Note: Clock source updated after SMPS operating mode set to step down */
  /* Wait for clock source status update */
  LL_mDelay(1);
  /* Get the SMPS clock source (SMPS clock source effectively applied) */
  if(LL_RCC_GetSMPSClockSource() != LL_RCC_SMPS_CLKSOURCE_STATUS_HSI)
  {
    LED_Blinking(LED_BLINK_ERROR);
  }

  /*## Configuration of PWR interruptions ##################################*/
  
  /* Clear BORH flag to not trig an interruption if Vdd is currently above Vdd */
  LL_PWR_ClearFlag_BORH();
  
  /* Enable SMPS step down converter forced in bypass mode by BORH interrupt for CPU1 */
  LL_PWR_EnableIT_BORH_SMPSFB();
}

/**
  * @brief  Function to configure and enter in STANDBY Mode.
  * @param  None
  * @retval None
  */
void EnterStandbyMode(void)
{
  /* Wait that user release the User push-button (SW1) */
  while(UserButton_GetState() == 0){}
  
  /* Turn-off LED */
  /* Note: LED state at this step depends on blinking state at the instant of user button is pressed. */
  LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
  
  /* Disable all used wakeup sources */
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);
  
  /* Clear all wake up flags */
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
  
  /* As default User push-button (SW1) state is high level, need to clear all wake up flag again */
  LL_PWR_ClearFlag_WU();

  /** Request to enter STANDBY mode
    * Following procedure described in STM32WBxx Reference Manual
    */
  /* Set STANDBY mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
  
#if !defined(RF_STACK_AVAILABLE)
  /* Case of CPU2 not started: low power mode of CPU2 must be managed by CPU1 */
  
  /* Set STANDBY mode of CPU2 */
  /* Note: On STM32WB, both CPU1 and CPU2 must be in standby mode to set the entire system in standby mode */
  LL_C2_PWR_SetPowerMode(LL_PWR_MODE_STANDBY);
#endif
  
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
  /* Configure and enter in STANDBY Mode */
  EnterStandbyMode();
  
  /* Here Device is in STANDBY mode */
}

/**
  * @brief  Function to manage SMPS forced bypass IRQ Handler
  * @note   Function called when Vdd dropped below BORH level.
  *         SMPS forced from step-down to bypass mode automatically, by hardware.
  * @param  None
  * @retval None
  */
void PWR_SMPS_ForcedBypass_Callback(void)
{
  /* Optionally: User code to be implemented here */
  __NOP();
}

/**
  * @brief  Function to manage BORH IRQ Handler
  * @note   Function called when Vdd rose above BORH level.
  *         SMPS remains in bypass mode. It up to application to set the SMPS
  *         is mode step-down.
  * @param  None
  * @retval None
  */
void PWR_BORH_Callback(void)
{
  /* Set SMPS operating mode */
  LL_PWR_SMPS_SetMode(LL_PWR_SMPS_STEP_DOWN);
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
}

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
