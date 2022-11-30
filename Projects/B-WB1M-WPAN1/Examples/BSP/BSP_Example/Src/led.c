/**
  ******************************************************************************
  * @file    led.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led supported by the
  *          B-WB1M-WPAN1 board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "b_wb1m_wpan1_bus.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  demo of led.
  */
void Led_demo(void)
{
  /* Init the LEDs */
  BSP_LED_Init(LED_BLUE);

  /* Turn on the leds */
  BSP_LED_On(LED_BLUE);

  HAL_Delay(2000);

  /* Toggle LEDs 10 times  */
  for (int i = 0 ; i < 10 ; i++)
  {
    HAL_Delay(250);
    BSP_LED_Toggle(LED_BLUE);
  }
  HAL_Delay(1000);

  /* Turn off the leds */
  BSP_LED_Off(LED_BLUE);
  HAL_Delay(1000);
  
  /* Don't DeInit the only LED Blue, because it is used by the main loop */
}

/**
  * @}
  */

/**
  * @}
  */
