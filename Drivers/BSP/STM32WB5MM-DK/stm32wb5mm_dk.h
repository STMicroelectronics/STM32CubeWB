/**
  ******************************************************************************
  * @file    stm32wb5mm_dk.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs, push-buttons and COM ports available on STM32WB5MM-DK
  *            board (MB1292) from STMicroelectronics
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
#ifndef STM32WB5MM_DK_H
#define STM32WB5MM_DK_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_conf.h"
#include "stm32wb5mm_dk_errno.h"
#if (USE_COM_LOG == 1)
#include <stdio.h>
#endif /* USE_COM_LOG */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32WB5MM_DK
  * @{
  */

/** @addtogroup STM32WB5MM_DK_COMMON
  * @{
  */

/** @defgroup STM32WB5MM_DK_COMMON_Exported_Types STM32WB5MM-DK COMMON Exported Types
  * @{
  */
typedef enum
{
  PWM_LED_RED = 0,
  PWM_LED_GREEN,
  PWM_LED_BLUE,
  PWM_LED_NB
} PwmLed_TypeDef;

typedef uint8_t PwmLedGsData_TypeDef;

typedef PwmLedGsData_TypeDef aPwmLedGsData_TypeDef[PWM_LED_NB];

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
typedef struct
{
  pTIM_CallbackTypeDef  pMspInitCb;
  pTIM_CallbackTypeDef  pMspDeInitCb;
} BSP_PWM_LED_Cb_t;
#endif /* (USE_HAL_TIM_REGISTER_CALLBACKS == 1) */


typedef enum
{
  BUTTON_USER1 = 0,
  BUTTON_USER2
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#if (USE_BSP_COM_FEATURE == 1)
typedef enum
{
  COM1 = 0,
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

/** @defgroup STM32WB5MM_DK_COMMON_Exported_Constants STM32WB5MM-DK COMMON Exported Constants
  * @{
  */

/**
  * @brief STM32WB5MM_DK BSP Driver version number
  */
#define STM32WB5MM_DK_BSP_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define STM32WB5MM_DK_BSP_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define STM32WB5MM_DK_BSP_VERSION_SUB2   (0x05U) /*!< [15:8]  sub2 version */
#define STM32WB5MM_DK_BSP_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define STM32WB5MM_DK_BSP_VERSION    ((STM32WB5MM_DK_BSP_VERSION_MAIN << 24U)\
                                            |(STM32WB5MM_DK_BSP_VERSION_SUB1 << 16U)\
                                            |(STM32WB5MM_DK_BSP_VERSION_SUB2 << 8U )\
                                            |(STM32WB5MM_DK_BSP_VERSION_RC))

/** @defgroup STM32WB5MM_DK_PWM_LED PWM LED
  * @{
  */
#define PWM_LEDn                                       1

#define PWM_LED_TIM                                    TIM17
#define PWM_LED_TIM_CLOCK_ENABLE()                     __HAL_RCC_TIM17_CLK_ENABLE()
#define PWM_LED_TIM_CLOCK_DISABLE()                    __HAL_RCC_TIM17_CLK_DISABLE()
#define PWM_LED_TIM_COUNTER_FREQ                       1000000
#define PWM_LED_TIM_FREQ                               200000
#define PWM_LED_TIM_GET_COUNTER_CLK_FREQ()             HAL_RCC_GetPCLK2Freq()
#define PWM_LED_TIM_UP_IRQN                            TIM1_TRG_COM_TIM17_IRQn

#define PWM_LED_SDI_GPIO_PIN                           GPIO_PIN_7
#define PWM_LED_SDI_GPIO_PORT                          GPIOA
#define PWM_LED_SDI_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define PWM_LED_SDI_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOA_CLK_DISABLE()

#define PWM_LED_SELECT_GPIO_PIN                           GPIO_PIN_1
#define PWM_LED_SELECT_GPIO_PORT                          GPIOH
#define PWM_LED_SELECT_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOH_CLK_ENABLE()
#define PWM_LED_SELECT_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOH_CLK_DISABLE()

#define PWM_LED_GSDATA_OFF    (PwmLedGsData_TypeDef) 0u     /* 0% On-Time duty cycle    */
#define PWM_LED_GSDATA_0_02   (PwmLedGsData_TypeDef) 1u     /* 0.02% On-Time duty cycle */
#define PWM_LED_GSDATA_0_05   (PwmLedGsData_TypeDef) 2u     /* 0.05% On-Time duty cycle */
#define PWM_LED_GSDATA_0_3    (PwmLedGsData_TypeDef) 3u     /* 0.3% On-Time duty cycle  */
#define PWM_LED_GSDATA_0_6    (PwmLedGsData_TypeDef) 6u     /* 0.6% On-Time duty cycle  */
#define PWM_LED_GSDATA_0_7    (PwmLedGsData_TypeDef) 7u     /* 0.7% On-Time duty cycle  */
#define PWM_LED_GSDATA_0_9    (PwmLedGsData_TypeDef) 8u     /* 0.9% On-Time duty cycle  */
#define PWM_LED_GSDATA_1_1    (PwmLedGsData_TypeDef) 9u     /* 1.1% On-Time duty cycle  */
#define PWM_LED_GSDATA_1_3    (PwmLedGsData_TypeDef) 10u    /* 1.3% On-Time duty cycle  */
#define PWM_LED_GSDATA_5_5    (PwmLedGsData_TypeDef) 30u    /* 5.5% On-Time duty cycle  */
#define PWM_LED_GSDATA_5_7    (PwmLedGsData_TypeDef) 31u    /* 5.7% On-Time duty cycle  */
#define PWM_LED_GSDATA_6_2    (PwmLedGsData_TypeDef) 32u    /* 6.2% On-Time duty cycle  */
#define PWM_LED_GSDATA_6_6    (PwmLedGsData_TypeDef) 33u    /* 6.6% On-Time duty cycle  */
#define PWM_LED_GSDATA_7_0    (PwmLedGsData_TypeDef) 34u    /* 7.0% On-Time duty cycle  */
#define PWM_LED_GSDATA_18_8   (PwmLedGsData_TypeDef) 62u    /* 18.8% On-Time duty cycle */
#define PWM_LED_GSDATA_19_2   (PwmLedGsData_TypeDef) 63u    /* 19.2% On-Time duty cycle */
#define PWM_LED_GSDATA_19_6   (PwmLedGsData_TypeDef) 64u    /* 19.6% On-Time duty cycle */
#define PWM_LED_GSDATA_20_0   (PwmLedGsData_TypeDef) 65u    /* 20.0% On-Time duty cycle */
#define PWM_LED_GSDATA_20_5   (PwmLedGsData_TypeDef) 66u    /* 20.5% On-Time duty cycle */
#define PWM_LED_GSDATA_46_1   (PwmLedGsData_TypeDef) 127u   /* 46.1% On-Time duty cycle */
#define PWM_LED_GSDATA_46_5   (PwmLedGsData_TypeDef) 128u   /* 46.5% On-Time duty cycle */
#define PWM_LED_GSDATA_47_0   (PwmLedGsData_TypeDef) 129u   /* 47% On-Time duty cycle   */
#define PWM_LED_GSDATA_99_1   (PwmLedGsData_TypeDef) 253u   /* 99.1% On-Time duty cycle */
#define PWM_LED_GSDATA_99_5   (PwmLedGsData_TypeDef) 254u   /* 99.5% On-Time duty cycle */
#define PWM_LED_GSDATA_99_9   (PwmLedGsData_TypeDef) 255u   /* 99.9% On-Time duty cycle */
/**
  * @}
  */

/** @defgroup STM32WB5MM_DK_BUTTON BUTTON
  * @{
  */
#define BUTTONn                                 2

/**
 * @brief Key push-buttons
 */
#define BUTTON_USER1_PIN                          GPIO_PIN_12
#define BUTTON_USER1_GPIO_PORT                    GPIOC
#define BUTTON_USER1_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER1_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER1_EXTI_LINE                    EXTI_LINE_12
#ifdef CORE_CM0PLUS
#define BUTTON_USER1_EXTI_IRQn                    EXTI15_4_IRQn
#else
#define BUTTON_USER1_EXTI_IRQn                    EXTI15_10_IRQn
#endif

#define BUTTON_USER2_PIN                          GPIO_PIN_13
#define BUTTON_USER2_GPIO_PORT                    GPIOC
#define BUTTON_USER2_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER2_GPIO_CLK_DISABLE()           __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER2_EXTI_LINE                    EXTI_LINE_13
#ifdef CORE_CM0PLUS
#define BUTTON_USER2_EXTI_IRQn                    EXTI15_4_IRQn
#else
#define BUTTON_USER2_EXTI_IRQn                    EXTI15_10_IRQn
#endif /* CORE_CM0PLUS */

#define BUTTON_USERx_GPIO_CLK_ENABLE(__INDEX__)    do { if ((__INDEX__) == BUTTON_USER1) BUTTON_USER1_GPIO_CLK_ENABLE(); else \
                                                        if ((__INDEX__) == BUTTON_USER2) BUTTON_USER2_GPIO_CLK_ENABLE();} while(0)

#define BUTTON_USERx_GPIO_CLK_DISABLE(__INDEX__)    do { if ((__INDEX__) == BUTTON_USER1) BUTTON_USER1_GPIO_CLK_DISABLE(); else \
                                                         if ((__INDEX__) == BUTTON_USER2) BUTTON_USER2_GPIO_CLK_DISABLE();} while(0)
/**
  * @}
  */

#if (USE_BSP_COM_FEATURE == 1)
/** @defgroup STM32WB5MM_DK_COM COM
  * @{
  */
#define COMn                              1U

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
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @addtogroup STM32WB5MM_DK_COMMON_Exported_Variables
  * @{
  */
extern EXTI_HandleTypeDef hpb_exti[];
#if (USE_BSP_COM_FEATURE == 1)
extern UART_HandleTypeDef hcom_uart[COMn];
#endif /* (USE_BSP_COM_FEATURE == 1) */

/**
  * @}
  */

/** @addtogroup STM32WB5MM_DK_COMMON_Exported_Functions
  * @{
  */
int32_t BSP_GetVersion(void);

/** @addtogroup STM32WB5MM_DK_COMMON_PWM_LED_Functions
  * @{
  */
int32_t BSP_PWM_LED_Init(void);
int32_t BSP_PWM_LED_DeInit(void);
int32_t BSP_PWM_LED_On(aPwmLedGsData_TypeDef aPwmLedGsData);
int32_t BSP_PWM_LED_Off(void);
int32_t BSP_PWM_LED_Toggle(aPwmLedGsData_TypeDef aPwmLedGsData);
int32_t BSP_PWM_LED_GetState(void);
void    BSP_PWM_LED_IRQHandler(void);
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
int32_t BSP_PWM_LED_RegisterDefaultMspCallbacks(void);
int32_t BSP_PWM_LED_RegisterMspCallbacks(BSP_PWM_LED_Cb_t *CallBacks);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS == 1 */
#if (PA7_AUTO_MANAGEMENT == 1)
void    BSP_PWM_LED_Pin_AutoManagement(void);
#endif /* PA7_AUTO_MANAGEMENT == 1*/
/**
  * @}
  */

/** @addtogroup STM32WB5MM_DK_COMMON_BUTTON_Functions
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
/** @addtogroup STM32WB5MM_DK_COMMON_COM_Functions
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

#if (USE_COM_LOG == 1)
#if !defined(__GNUC__)
int fgetc(FILE *f);
#else
int __io_getchar(void);
#endif /* !__GNUC__ */
#endif /* USE_COM_LOG */
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

#endif /* STM32WB5MM_DK_H */
