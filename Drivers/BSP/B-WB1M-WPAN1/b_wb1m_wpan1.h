/**
  ******************************************************************************
  * @file    b_wb1m_wpan1.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs, push-buttons and COM ports available on B-WB1M-WPAN1
  *            board (MB1868) from STMicroelectronics
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef B_WB1M_WPAN1_H
#define B_WB1M_WPAN1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "b_wb1m_wpan1_conf.h"
#include "b_wb1m_wpan1_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_WB1M_WPAN1
  * @{
  */

/** @addtogroup B_WB1M_WPAN1_COMMON
  * @{
  */

/** @defgroup B_WB1M_WPAN1_COMMON_Exported_Types B-WB1M-WPAN1 COMMON Exported Types
  * @{
  */
typedef enum
{
  LED1 = 0,
  LEDn,

  /* Color led aliases */
  LED_BLUE   = LED1,
} Led_TypeDef;

typedef enum
{
  BUTTON_USER1 = 0,
  BUTTONn
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE == 1)
typedef enum
{
  COM1_UART = 0,
  COMn
} COM_TypeDef;

typedef enum
{
  COM_WORDLENGTH_7B = UART_WORDLENGTH_7B,
  COM_WORDLENGTH_8B = UART_WORDLENGTH_8B,
  COM_WORDLENGTH_9B = UART_WORDLENGTH_9B
} COM_WordLengthTypeDef;

typedef enum
{
  COM_STOPBITS_1 = UART_STOPBITS_1,
  COM_STOPBITS_2 = UART_STOPBITS_2
} COM_StopBitsTypeDef;

typedef enum
{
  COM_PARITY_NONE = UART_PARITY_NONE,
  COM_PARITY_EVEN = UART_PARITY_EVEN,
  COM_PARITY_ODD  = UART_PARITY_ODD
} COM_ParityTypeDef;

typedef enum
{
  COM_HWCONTROL_NONE    = UART_HWCONTROL_NONE,
  COM_HWCONTROL_RTS     = UART_HWCONTROL_RTS,
  COM_HWCONTROL_CTS     = UART_HWCONTROL_CTS,
  COM_HWCONTROL_RTS_CTS = UART_HWCONTROL_RTS_CTS,
} COM_HwFlowCtlTypeDef;

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
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_COMMON_Exported_Constants B-WB1M-WPAN1 COMMON Exported Constants
  * @{
  */

/**
  * @brief B_WB1M_WPAN1 BSP Driver version number
  */
#define __B_WB1M_WPAN1_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __B_WB1M_WPAN1_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define __B_WB1M_WPAN1_BSP_VERSION_SUB2   (0x02U) /*!< [15:8]  sub2 version */
#define __B_WB1M_WPAN1_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __B_WB1M_WPAN1_BSP_VERSION        ((__B_WB1M_WPAN1_BSP_VERSION_MAIN << 24U)\
                                           |(__B_WB1M_WPAN1_BSP_VERSION_SUB1 << 16U)\
                                           |(__B_WB1M_WPAN1_BSP_VERSION_SUB2 << 8U)\
                                           |(__B_WB1M_WPAN1_BSP_VERSION_RC))

/**
  * @brief Define for B_WB1M_WPAN1 board
  */
#if !defined (USE_B_WB1M_WPAN1)
#define USE_B_WB1M_WPAN1
#endif /* !defined (USE_B_WB1M_WPAN1) */

/** @defgroup B_WB1M_WPAN1_COMMON_LED COMMON LED Constants
  * @{
  */
#define LED1_PIN                                GPIO_PIN_0
#define LED1_GPIO_PORT                          GPIOB
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)         __HAL_RCC_GPIOB_CLK_ENABLE() /* All Led on same port */
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)        __HAL_RCC_GPIOB_CLK_DISABLE() /* All Led on same port */

/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_COMMON_BUTTON COMMON BUTTON Constants
  * @{
  */

/**
  * @brief Key push-buttons
  */
#define BUTTON_USER1_PIN                          GPIO_PIN_1
#define BUTTON_USER1_GPIO_PORT                    GPIOB
#define BUTTON_USER1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUTTON_USER1_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUTTON_USER1_EXTI_LINE                    EXTI_LINE_1
#ifdef CORE_CM0PLUS
#define BUTTON_USER1_EXTI_IRQn                    EXTI1_0_IRQn
#else
#define BUTTON_USER1_EXTI_IRQn                    EXTI1_IRQn
#endif /* CORE_CM0PLUS */
#define H_EXTI_0                                  hpb_exti[BUTTON_USER1]

#define BUTTON_USERx_GPIO_CLK_ENABLE(__INDEX__)   \
  do { if ((__INDEX__) == BUTTON_USER1) BUTTON_USER1_GPIO_CLK_ENABLE(); } \
  while(0)

#define BUTTON_USERx_GPIO_CLK_DISABLE(__INDEX__)  \
  do { if ((__INDEX__) == BUTTON_USER1) BUTTON_USER1_GPIO_CLK_DISABLE(); } \
  while(0)
/**
  * @}
  */

#if (USE_BSP_COM_FEATURE == 1)
/** @defgroup B_WB1M_WPAN1_COM COM
  * @{
  */
#define COM1_UART1                            USART1
#define COM1_CLK_ENABLE()                     __HAL_RCC_USART1_CLK_ENABLE()
#define COM1_CLK_DISABLE()                    __HAL_RCC_USART1_CLK_DISABLE()

#define COM1_TX_PIN                           GPIO_PIN_9
#define COM1_TX_GPIO_PORT                     GPIOA
#define COM1_TX_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_TX_AF                            GPIO_AF7_USART1

#define COM1_RX_PIN                           GPIO_PIN_10
#define COM1_RX_GPIO_PORT                     GPIOA
#define COM1_RX_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOA_CLK_DISABLE()
#define COM1_RX_AF                            GPIO_AF7_USART1

#define COM_POLL_TIMEOUT                      1000

#define MX_UART_InitTypeDef COM_InitTypeDef

/**
  * @}
  */
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @addtogroup B_WB1M_WPAN1_COMMON_Exported_Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE == 1)
extern UART_HandleTypeDef hcom_uart[COMn];
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @addtogroup B_WB1M_WPAN1_COMMON_Exported_Functions B-WB1M-WPAN1 COMMON Exported Functions
  * @{
  */
int32_t BSP_GetVersion(void);

/** @addtogroup B_WB1M_WPAN1_COMMON_LED_Functions
  * @{
  */
int32_t BSP_LED_Init(Led_TypeDef Led);
int32_t BSP_LED_DeInit(Led_TypeDef Led);
int32_t BSP_LED_On(Led_TypeDef Led);
int32_t BSP_LED_Off(Led_TypeDef Led);
int32_t BSP_LED_Toggle(Led_TypeDef Led);
int32_t BSP_LED_GetState(Led_TypeDef Led);

/**
  * @}
  */

/** @addtogroup B_WB1M_WPAN1_COMMON_BUTTON_Functions
  * @{
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
int32_t BSP_PB_DeInit(Button_TypeDef Button);
int32_t BSP_PB_GetState(Button_TypeDef Button);
void    BSP_PB_Callback(Button_TypeDef Button);
void    BSP_PB_IRQHandler(Button_TypeDef Button);
/**
  * @}
  */

#if (USE_BSP_COM_FEATURE == 1)
/** @addtogroup B_WB1M_WPAN1_COMMON_COM_Functions
  * @{
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t BSP_COM_DeInit(COM_TypeDef COM);

#if (USE_COM_LOG == 1)
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM);
#endif /* (USE_COM_LOG == 1) */

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *CallBacks);
#endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 1) */

HAL_StatusTypeDef MX_LPUART1_Init(UART_HandleTypeDef *huart, MX_UART_InitTypeDef *COM_Init);
/**
  * @}
  */
#endif /* (USE_BSP_COM_FEATURE == 1) */

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
#endif /* __cplusplus */

#endif /* B_WB1M_WPAN1_H */
