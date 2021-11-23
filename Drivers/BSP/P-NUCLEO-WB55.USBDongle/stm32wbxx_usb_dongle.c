/**
  ******************************************************************************
  * @file    stm32wbxx_usb_dongle.c
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage:
  *          - LEDs and push-button available on STM32WBXX Usb Dongle Kit 
  *            from STMicroelectronics
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
#include "stm32wbxx_usb_dongle.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32WBXX_USB_DONGLE STM32WBxx Usb Dongle
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32WBxx Usb Dongle Kit from STMicroelectronics.
  * @{
  */

/** @defgroup STM32WBXX_USB_DONGLE_Private_Defines Private Defines
  * @{
  */ 

/**
  * @brief STM32WBxx usb dongle BSP Driver
  */
#define __STM32WBxx_USB_DONGLE_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __STM32WBxx_USB_DONGLE_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define __STM32WBxx_USB_DONGLE_BSP_VERSION_SUB2   (0x04U) /*!< [15:8]  sub2 version */
#define __STM32WBxx_USB_DONGLE_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */ 
#define __STM32WBxx_USB_DONGLE_BSP_VERSION        ((__STM32WBxx_USB_DONGLE_BSP_VERSION_MAIN << 24)\
                                             |(__STM32WBxx_USB_DONGLE_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32WBxx_USB_DONGLE_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32WBxx_USB_DONGLE_BSP_VERSION_RC))   

/**
  * @brief LINK SD Card
  */
#define SD_DUMMY_BYTE            0xFF    
#define SD_NO_RESPONSE_EXPECTED  0x80

/**
  * @}
  */

/** @defgroup STM32WBXX_USB_DONGLE_LOW_LEVEL_Private_Variables Private Variables
  * @{
  */
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {BUTTON_SW1_GPIO_PORT}; 
const uint16_t BUTTON_PIN[BUTTONn] = {BUTTON_SW1_PIN}; 
const uint8_t BUTTON_IRQn[BUTTONn] = {BUTTON_SW1_EXTI_IRQn};

/**
  * @}
  */

/** @defgroup STM32WBXX_USB_DONGLE_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  This method returns the STM32WBxx usb dongle BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32WBxx_USB_DONGLE_BSP_VERSION;
}

/** @defgroup STM32WBXX_USB_DONGLE_LED_Functions LED Functions
  * @{
  */

/**
  * @brief  Configures LED GPIO.
  * @param  Led: LED to be configured. 
  *          This parameter can be one of the following values:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};

  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  gpioinitstruct.Pin = GPIO_PIN[Led];
  gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull = GPIO_NOPULL;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIO_PORT[Led], &gpioinitstruct);

  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  DeInit LEDs.
  * @param  Led: LED to be de-init. 
  *   This parameter can be one of the following values:
  *     @arg  LED2
  * @note Led DeInit does not disable the GPIO clock nor disable the Mfx 
  * @retval None
  */
void BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Turn off LED */
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
  /* DeInit the GPIO_LED pin */
  gpio_init_structure.Pin = GPIO_PIN[Led];
  HAL_GPIO_DeInit(GPIO_PORT[Led], gpio_init_structure.Pin);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET); 
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED2  
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
  * @}
  */

/** @defgroup STM32WBXX_USB_DONGLE_BUTTON_Functions BUTTON Functions
  * @{
  */ 

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_KEY
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpioinitstruct = {0};

  /* Enable the BUTTON Clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);

  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    gpioinitstruct.Pin = BUTTON_PIN[Button];
    gpioinitstruct.Mode = GPIO_MODE_INPUT;
    gpioinitstruct.Pull = GPIO_PULLUP;
    gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);
  }

  if(ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    gpioinitstruct.Pin = BUTTON_PIN[Button];
    gpioinitstruct.Pull = GPIO_PULLUP;
    gpioinitstruct.Mode = GPIO_MODE_IT_FALLING; 
    HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);
    
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Push Button DeInit.
  * @param  Button: Button to be configured
  *   This parameter should be: BUTTON_KEY
  * @note PB DeInit does not disable the GPIO clock
  * @retval None
  */
void BSP_PB_DeInit(Button_TypeDef Button)
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = BUTTON_PIN[Button];
    HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
    HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY  
  * @retval The Button GPIO pin value.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
