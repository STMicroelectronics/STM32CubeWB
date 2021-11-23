/**
  ******************************************************************************
  * @file    BSP/BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the bsp API.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t delay = 250;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_Init();

  /* Configure the system clock to 32 MHz */
  SystemClock_Config();
  
  /* Initialize Led */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  
  /* Initialize the button in interrupt mode */
  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);
  
  /* From there, pressing the button will change the frequency of the toggling of the LEDs */
  
  /* Toggle LEDs in an infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(delay);
    
    BSP_LED_Toggle(LED2);
    HAL_Delay(delay);
    
    BSP_LED_On(LED3);
    HAL_Delay(delay);
    
    BSP_LED_Off(LED3);
    HAL_Delay(delay);
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin) 
  {
    case BUTTON_SW1_PIN:
      /* Change the period from 500ms to 250ms or 250ms to 500ms */
      delay = (delay == 250) ? 500 : 250; 
      break;
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 4000000
  *            HCLK(Hz)                       = 4000000
  * @retval None
  */
void SystemClock_Config(void)
{
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
