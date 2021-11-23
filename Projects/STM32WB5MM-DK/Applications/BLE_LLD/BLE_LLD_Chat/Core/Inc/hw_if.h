/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hw_if.h
  * @author  MCD Application Team
  * @brief   Hardware Interface
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HW_IF_H
#define HW_IF_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
#include "stm32wbxx.h"
#include "stm32wbxx_ll_exti.h"
#include "stm32wbxx_ll_system.h"
#include "stm32wbxx_ll_rcc.h"
#include "stm32wbxx_ll_ipcc.h"
#include "stm32wbxx_ll_bus.h"
#include "stm32wbxx_ll_pwr.h"
#include "stm32wbxx_ll_cortex.h"
#include "stm32wbxx_ll_utils.h"
#include "stm32wbxx_ll_hsem.h"
#include "stm32wbxx_ll_gpio.h"
#include "stm32wbxx_ll_rtc.h"

#ifdef  USE_STM32WBXX_USB_DONGLE
#include "stm32wbxx_usb_dongle.h"
#endif

#ifdef  USE_STM32WBXX_NUCLEO

#ifdef STM32WB35xx
#include "nucleo_wb35ce.h"
#endif

#ifdef STM32WB55xx
#include "stm32wbxx_nucleo.h"
#endif

#endif

#ifdef  USE_X_NUCLEO_EPD
#include "x_nucleo_epd.h"
#endif

#ifdef USE_STM32WB5M_DK
#include "stm32wb5mm_dk.h"
#endif
  
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/******************************************************************************
 * HW UART
 ******************************************************************************/
typedef enum
{
  hw_uart1,
  hw_uart2,
  hw_lpuart1,
} hw_uart_id_t;

typedef enum
{
  hw_uart_ok,
  hw_uart_error,
  hw_uart_busy,
  hw_uart_to,
} hw_status_t;

#if (CFG_HW_USART1_ENABLED == 1)
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
#endif

//void HW_UART_Init(hw_uart_id_t hw_uart_id);
hw_status_t HW_UART_Receive_IT(hw_uart_id_t hw_uart_id, uint8_t *pData, uint16_t Size, void (*Callback)(void));
hw_status_t HW_UART_Transmit_IT(hw_uart_id_t hw_uart_id, uint8_t *pData, uint16_t Size,  void (*Callback)(void));
hw_status_t HW_UART_Transmit(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size,  uint32_t timeout);
hw_status_t HW_UART_Transmit_DMA(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size, void (*Callback)(void));
#if 0
void HW_UART_Interrupt_Handler(hw_uart_id_t hw_uart_id);
void HW_UART_DMA_Interrupt_Handler(hw_uart_id_t hw_uart_id);
#endif

void MX_UART_Init(hw_uart_id_t uart);
void MX_UART_Deinit(hw_uart_id_t uart);

#ifdef __cplusplus
}
#endif

#endif /*HW_IF_H */
