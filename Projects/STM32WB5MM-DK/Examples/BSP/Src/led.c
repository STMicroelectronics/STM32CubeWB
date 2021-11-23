/**
  ******************************************************************************
  * @file    led.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led supported by the 
  *          STM32WB55MM-DK board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "stm32wb5mm_dk_bus.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
aPwmLedGsData_TypeDef aPwmLedGsData;
GPIO_InitTypeDef GPIO_InitStructure;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t ButtonState;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  demo of led.
  */
void Led_demo(void)
{
  ButtonState = 0;
  //printf("Press User button 1 to start the test\n");
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(0, 16, (uint8_t *)"Led demo", CENTER_MODE);
  UTIL_LCD_DrawRect(0, 16, 127 , 11, SSD1315_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(0, 40, (uint8_t *)"Press user button1", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 52, (uint8_t *)"to skip LED demo", CENTER_MODE);
  BSP_LCD_Refresh(0);
  HAL_Delay(2000);
  /* Configure the PWM LED */
  BSP_PWM_LED_Init();
  /* PWM LED BSP Test sequence */
  /* Turn on Red led */
  aPwmLedGsData[PWM_LED_RED] = PWM_LED_GSDATA_7_0;
  aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_BLUE] = PWM_LED_GSDATA_OFF;
  BSP_PWM_LED_On(aPwmLedGsData);
  HAL_Delay(2000);
  BSP_PWM_LED_Off();
  HAL_Delay(1000);
  
  /* Turn on Green led */
  aPwmLedGsData[PWM_LED_RED] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_7_0;
  aPwmLedGsData[PWM_LED_BLUE] = PWM_LED_GSDATA_OFF;
  BSP_PWM_LED_On(aPwmLedGsData);
  HAL_Delay(2000);
  BSP_PWM_LED_Off();
  HAL_Delay(1000);
  
  /* Turn on Blue led */
  aPwmLedGsData[PWM_LED_RED] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_BLUE] = PWM_LED_GSDATA_7_0;
  BSP_PWM_LED_On(aPwmLedGsData);
  HAL_Delay(2000);
  BSP_PWM_LED_Off();
  HAL_Delay(1000);
  
  aPwmLedGsData[PWM_LED_RED] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_7_0;
  aPwmLedGsData[PWM_LED_BLUE] = PWM_LED_GSDATA_OFF;
  BSP_PWM_LED_On(aPwmLedGsData);
  for (int i = 0 ; i < 10 ; i++)
  {
    HAL_Delay(500);
    BSP_PWM_LED_Toggle(aPwmLedGsData);
  }
  BSP_PWM_LED_Off();
  
  if (BSP_PWM_LED_GetState() == 0)
  {
    BSP_PWM_LED_On(aPwmLedGsData);
    HAL_Delay(2000);    
  }
  
  if (BSP_PWM_LED_GetState() == 1)
  {
    BSP_PWM_LED_Off();
  }
  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      BSP_PWM_LED_DeInit();

      /* configure SPIx MOSI */
      GPIO_InitStructure.Pin       = BUS_SPI1_MOSI_PIN;
      GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStructure.Pull      = GPIO_PULLDOWN;
      GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStructure.Alternate = BUS_SPI1_AF;
      HAL_GPIO_Init(BUS_SPI1_GPIO_PORTA, &GPIO_InitStructure);
      return;
    }
  }
}

/**
  * @}
  */ 

/**
  * @}
  */
 
