/**
  ******************************************************************************
  * @file    cube_hal.h
  * @author  AMS VMA RF Application team
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 

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
