/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    cube_hal.h
  * @author  MCD Application Team
  * @brief   
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
/* USER CODE END Header */

#ifndef _CUBE_HAL_H_
#define _CUBE_HAL_H_

/* Includes ------------------------------------------------------------------*/

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_conf.h"

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

#define UART_TX_TIMEOUT 5 

#define Uart_Irq_Handler      USART2_IRQHandler


void SystemClock_Config(void);
void Init_Device(void);
void MX_USART2_UART_Init(void);


#endif /* _CUBE_HAL_H_ */
