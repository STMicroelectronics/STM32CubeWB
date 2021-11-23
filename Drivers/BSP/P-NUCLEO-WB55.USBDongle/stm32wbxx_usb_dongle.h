/** 
  ******************************************************************************
  * @file    stm32wbxx_usb_dongle.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32WBXX_USB_DONGLE_H
#define __STM32WBXX_USB_DONGLE_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WBXX_USB_DONGLE
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"


/** @defgroup STM32WBXX_USB_DONGLE_Exported_Types Exported Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  /* Color led aliases */
  LED_BLUE   = LED1,
  LED_GREEN  = LED2,
  LED_RED    = LED3
}Led_TypeDef;

typedef enum 
{
  BUTTON_SW1 = 0
}Button_TypeDef;

typedef enum 
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;

/**
  * @}
  */

/** @defgroup STM32WBXX_USB_DONGLE_Exported_Constants Exported Constants
  * @{
  */ 

/**
  * @brief Define for STM32WBXX_USB_DONGLE board  
  */
#if !defined (USE_STM32WBXX_USB_DONGLE)
 #define USE_STM32WBXX_USB_DONGLE
#endif

/** @defgroup STM32WBXX_USB_DONGLE_LED LED Constants
  * @{
  */
#define LEDn                                    3

#define LED1_PIN                                GPIO_PIN_4
#define LED1_GPIO_PORT                          GPIOA
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOA_CLK_DISABLE()  

#define LED2_PIN                                GPIO_PIN_0
#define LED2_GPIO_PORT                          GPIOB
#define LED2_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LED3_PIN                                GPIO_PIN_1
#define LED3_GPIO_PORT                          GPIOB
#define LED3_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)         do { if ((__INDEX__) == LED1) LED1_GPIO_CLK_ENABLE(); else \
                                                if ((__INDEX__) == LED2) LED2_GPIO_CLK_ENABLE(); else \
                                                if ((__INDEX__) == LED3) LED3_GPIO_CLK_ENABLE();} while(0)
                                                  
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)         do { if ((__INDEX__) == LED1) LED1_GPIO_CLK_DISABLE(); else \
                                                if ((__INDEX__) == LED2) LED2_GPIO_CLK_DISABLE(); else \
                                                if ((__INDEX__) == LED3) LED3_GPIO_CLK_DISABLE();} while(0)
/**
  * @}
  */

/** @defgroup STM32WBXX_USB_DONGLE_BUTTON BUTTON Constants
  * @{
  */
#define BUTTONn                                 1

/**
 * @brief Key push-button
 */
#if defined (USE_STM32WBXX_USB_DONGLE_REVA)
#define BUTTON_SW1_PIN                          GPIO_PIN_9
#else
#define BUTTON_SW1_PIN                          GPIO_PIN_10
#endif
#define BUTTON_SW1_GPIO_PORT                    GPIOA
#define BUTTON_SW1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_SW1_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()
#if defined (USE_STM32WBXX_USB_DONGLE_REVA)
#define BUTTON_SW1_EXTI_LINE                    GPIO_PIN_9
#else
#define BUTTON_SW1_EXTI_LINE                    GPIO_PIN_10
#endif
#if defined (USE_STM32WBXX_USB_DONGLE_REVA)
#define BUTTON_SW1_EXTI_IRQn                    EXTI9_5_IRQn
#else
#define BUTTON_SW1_EXTI_IRQn                    EXTI15_10_IRQn
#endif

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)      __HAL_RCC_GPIOA_CLK_ENABLE()

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)     __HAL_RCC_GPIOA_CLK_DISABLE()

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup STM32WBXX_USB_DONGLE_Exported_Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);  

/** @addtogroup STM32WBXX_USB_DONGLE_LED_Functions
  * @{
  */
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
/**
  * @}
  */

/** @addtogroup STM32WBXX_USB_DONGLE_BUTTON_Functions
  * @{
  */
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
void             BSP_PB_DeInit(Button_TypeDef Button);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);
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

#ifdef __cplusplus
}
#endif

#endif /* __STM32WBXX_USB_DONGLE_H */
