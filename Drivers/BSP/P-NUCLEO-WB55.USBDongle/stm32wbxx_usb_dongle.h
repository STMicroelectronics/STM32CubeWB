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

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_usb_dongle_errno.h"

#if (USE_BSP_COM_FEATURE > 0)
  #if (USE_COM_LOG > 0)
    #ifndef __GNUC__
      #include "stdio.h"
    #endif /* !__GNUC__ */
  #endif /* USE_COM_LOG */
#endif /* USE_BSP_COM_FEATURE */

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
} Led_TypeDef;

typedef enum
{
  BUTTON_SW1 = 0
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE > 0)
typedef enum
{
  COM1 = 0U,
  COMn
} COM_TypeDef;

typedef enum
{
  COM_STOPBITS_1     =   UART_STOPBITS_1,
  COM_STOPBITS_2     =   UART_STOPBITS_2,
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE     =  UART_PARITY_NONE,
  COM_PARITY_EVEN     =  UART_PARITY_EVEN,
  COM_PARITY_ODD      =  UART_PARITY_ODD,
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE    =  UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS     =  UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS     =  UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS =  UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B,
} COM_WordLengthTypeDef;

typedef struct
{
  uint32_t              BaudRate;
  COM_WordLengthTypeDef WordLength;
  COM_StopBitsTypeDef   StopBits;
  COM_ParityTypeDef     Parity;
  COM_HwFlowCtlTypeDef  HwFlowCtl;
} COM_InitTypeDef;

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
typedef struct
{
  pUART_CallbackTypeDef  pMspInitCb;
  pUART_CallbackTypeDef  pMspDeInitCb;
} BSP_COM_Cb_t;
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */
#endif /* (USE_BSP_COM_FEATURE > 0) */
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

#if (USE_BSP_COM_FEATURE > 0)
/** @defgroup STM32WBXX_NUCLEO_LOW_LEVEL_COM LOW LEVEL COM Port Constants
  * @{
  */
#define COM1_UART                             USART1
#define COM1_CLK_ENABLE()                     __HAL_RCC_USART1_CLK_ENABLE()
#define COM1_CLK_DISABLE()                    __HAL_RCC_USART1_CLK_DISABLE()

#define COM1_TX_PIN                           GPIO_PIN_6
#define COM1_TX_GPIO_PORT                     GPIOB
#define COM1_TX_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define COM1_TX_AF                            GPIO_AF7_USART1

#define COM1_RX_PIN                           GPIO_PIN_7
#define COM1_RX_GPIO_PORT                     GPIOB
#define COM1_RX_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define COM1_RX_AF                            GPIO_AF7_USART1
#define COM_POLL_TIMEOUT                      1000

#define MX_UART_InitTypeDef COM_InitTypeDef
/**
  * @}
  */
#endif /* (USE_BSP_COM_FEATURE > 0)*/

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

#if (USE_BSP_COM_FEATURE > 0)
/** @defgroup STM32WLXX_NUCLEO_LOW_LEVEL_COM_Functions LOW LEVEL COM Port Functions
  * @{
  */
int32_t  BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t  BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG > 0)
int32_t  BSP_COM_SelectLogPort (COM_TypeDef COM);
#endif
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t  BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t  BSP_COM_RegisterMspCallbacks(COM_TypeDef COM , BSP_COM_Cb_t *Callback);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
/**
  * @}
  */
#endif /* (USE_BSP_COM_FEATURE > 0) */

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
