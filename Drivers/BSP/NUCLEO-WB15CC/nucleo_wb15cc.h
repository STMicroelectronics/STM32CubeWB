/**
  ******************************************************************************
  * @file    nucleo_wb15cc.h
  * @author  MCD Application Team
  * @brief   Header for nucleo_wb15cc.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NUCLEO_WB15CC_H
#define NUCLEO_WB15CC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "nucleo_wb15cc_errno.h"
#include "nucleo_wb15cc_conf.h"
   
#if (USE_BSP_COM_FEATURE > 0)
  #if (USE_COM_LOG > 0)
    #ifndef __GNUC__
      #include "stdio.h"
    #endif
  #endif
#endif
   
/** @addtogroup BSP
  * @{
  */

/** @defgroup NUCLEO_WB15CC NUCLEO-WB15CC
  * @{
  */

/** @defgroup NUCLEO_WB15CC_LOW_LEVEL LOW LEVEL
  * @{
  */

/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_Exported_Types LOW LEVEL Exported Types
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
  BUTTON_SW1 = 0,
  BUTTON_SW2 = 1,
  BUTTON_SW3 = 2,
}Button_TypeDef;

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
#endif /* (USE_BSP_COM_FEATURE > 0) */

typedef enum 
{
  ABSENT  = 0,
  PRESENT = 1,
} Presence_TypeDef;
/**
  * @}
  */

/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_Exported_Constants LOW LEVEL Exported Constants
  * @{
  */

/**
  * @brief NUCLEO-WB15CC BSP Driver version number
  */
#define __NUCLEO_WB15CC_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __NUCLEO_WB15CC_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define __NUCLEO_WB15CC_BSP_VERSION_SUB2   (0x03U) /*!< [15:8]  sub2 version */
#define __NUCLEO_WB15CC_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */ 
#define __NUCLEO_WB15CC_BSP_VERSION        ((__NUCLEO_WB15CC_BSP_VERSION_MAIN << 24)\
                                              |(__NUCLEO_WB15CC_BSP_VERSION_SUB1 << 16)\
                                              |(__NUCLEO_WB15CC_BSP_VERSION_SUB2 << 8 )\
                                              |(__NUCLEO_WB15CC_BSP_VERSION_RC))

/** 
  * @brief Define for NUCLEO-WB15CC board  
  */
#if !defined (USE_NUCLEO_WB15CC)
 #define USE_NUCLEO_WB15CC
#endif /* !USE_NUCLEO_WB15CC */

/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_LED LOW LEVEL LED Constants
  * @{
  */
#define LEDn                                    3

#define LED1_PIN                                GPIO_PIN_5
#define LED1_GPIO_PORT                          GPIOB
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED2_PIN                                GPIO_PIN_0
#define LED2_GPIO_PORT                          GPIOB
#define LED2_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED3_PIN                                GPIO_PIN_1
#define LED3_GPIO_PORT                          GPIOB
#define LED3_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)         __HAL_RCC_GPIOB_CLK_ENABLE() /* All Led on same port */
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)        __HAL_RCC_GPIOB_CLK_DISABLE() /* All Led on same port */
/**
  * @}
  */
  
/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_BUTTON LOW LEVEL BUTTON Constants
  * @{
  */
#define BUTTONn                                 3

/**
 * @brief Key push-buttons
 */
#define BUTTON_SW1_PIN                          GPIO_PIN_0
#define BUTTON_SW1_GPIO_PORT                    GPIOA
#define BUTTON_SW1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_SW1_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUTTON_SW1_EXTI_LINE                    EXTI_LINE_0
#ifdef CORE_CM0PLUS
#define BUTTON_SW1_EXTI_IRQn                    EXTI1_0_IRQn
#else
#define BUTTON_SW1_EXTI_IRQn                    EXTI0_IRQn
#endif

#define BUTTON_SW2_PIN                          GPIO_PIN_4
#define BUTTON_SW2_GPIO_PORT                    GPIOE
#define BUTTON_SW2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUTTON_SW2_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOE_CLK_DISABLE()
#define BUTTON_SW2_EXTI_LINE                    EXTI_LINE_4
#ifdef CORE_CM0PLUS
#define BUTTON_SW2_EXTI_IRQn                    EXTI15_4_IRQn
#else
#define BUTTON_SW2_EXTI_IRQn                    EXTI4_IRQn
#endif

#define BUTTON_SW3_PIN                          GPIO_PIN_6
#define BUTTON_SW3_GPIO_PORT                    GPIOA
#define BUTTON_SW3_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUTTON_SW3_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUTTON_SW3_EXTI_LINE                    EXTI_LINE_6
#ifdef CORE_CM0PLUS
#define BUTTON_SW3_EXTI_IRQn                    EXTI15_4_IRQn
#else
#define BUTTON_SW3_EXTI_IRQn                    EXTI9_5_IRQn
#endif /* CORE_CM0PLUS */

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    do { if ((__INDEX__) == BUTTON_SW1) BUTTON_SW1_GPIO_CLK_ENABLE(); else \
                                              if ((__INDEX__) == BUTTON_SW2) BUTTON_SW2_GPIO_CLK_ENABLE(); else \
                                              if ((__INDEX__) == BUTTON_SW3) BUTTON_SW3_GPIO_CLK_ENABLE();} while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)    do { if ((__INDEX__) == BUTTON_SW1) BUTTON_SW1_GPIO_CLK_DISABLE(); else \
                                              if ((__INDEX__) == BUTTON_SW2) BUTTON_SW2_GPIO_CLK_DISABLE(); else \
                                              if ((__INDEX__) == BUTTON_SW3) BUTTON_SW3_GPIO_CLK_DISABLE();} while(0)

/**
  * @}
  */
  
#if (USE_BSP_COM_FEATURE > 0)
/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_COM LOW LEVEL COM Port Constants
  * @{
  */
#define COM1_UART                             USART1
#define COM1_CLK_ENABLE()                     __HAL_RCC_USART1_CLK_ENABLE()
#define COM1_CLK_DISABLE()                    __HAL_RCC_USART1_CLK_ENABLE()

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
#endif /* (USE_BSP_COM_FEATURE > 0)*/

/**
  * @}
  */

/** @addtogroup NUCLEO_WB15CC_LOW_LEVEL_Exported_Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE > 0)
extern UART_HandleTypeDef hcom_uart[];
#endif /* (USE_BSP_COM_FEATURE > 0) */
/**
  * @}
  */

/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_Exported_Functions LOW LEVEL Exported Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);

/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_LED_Functions LOW LEVEL LED Functions
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

/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_BUTTON_Functions  LOW LEVEL BUTTON Functions
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

#if (USE_BSP_COM_FEATURE > 0)
/** @defgroup NUCLEO_WB15CC_LOW_LEVEL_COM_Functions LOW LEVEL COM Port Functions
  * @{
  */
int32_t BSP_COM_Init(COM_TypeDef COM, COM_InitTypeDef *COM_Init);
int32_t BSP_COM_DeInit(COM_TypeDef COM);
#if (USE_COM_LOG > 0)
int32_t BSP_COM_SelectLogPort (COM_TypeDef COM);
#endif
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM);
int32_t BSP_COM_RegisterMspCallbacks(COM_TypeDef COM, BSP_COM_Cb_t *Callback);
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

#endif /* NUCLEO_WB15CC_H */
