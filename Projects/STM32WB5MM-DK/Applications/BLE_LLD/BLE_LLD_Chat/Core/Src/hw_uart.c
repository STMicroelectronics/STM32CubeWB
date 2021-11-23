/**
 ******************************************************************************
  * File Name          : Src/hw_uart.c
  * Description        : HW UART source file for STM32WPAN Middleware.
  *
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
#include "app_common.h"

/* Macros --------------------------------------------------------------------*/
#define HW_UART_RX_IT(__HANDLE__, __USART_BASE__)                                    \
        do{                                                                          \
            HW_##__HANDLE__##RxCb = cb;                                              \
            (__HANDLE__).Instance = (__USART_BASE__);                                \
            hal_status = HAL_UART_Receive_IT(&(__HANDLE__), p_data, size);           \
        } while(0)

#define HW_UART_TX_IT(__HANDLE__, __USART_BASE__)                                    \
        do{                                                                          \
            HW_##__HANDLE__##TxCb = cb;                                              \
            (__HANDLE__).Instance = (__USART_BASE__);                                \
            hal_status = HAL_UART_Transmit_IT(&(__HANDLE__), p_data, size);          \
        } while(0)

#define HW_UART_TX(__HANDLE__, __USART_BASE__)                                       \
        do{                                                                          \
            (__HANDLE__).Instance = (__USART_BASE__);                                \
            hal_status = HAL_UART_Transmit(&(__HANDLE__), p_data, size, timeout);    \
        } while(0)

/* Variables -----------------------------------------------------------------*/
#if (CFG_HW_USART1_ENABLED == 1)
UART_HandleTypeDef huart1;
#if (CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
DMA_HandleTypeDef hdma_usart1_tx;
#endif
void (*HW_huart1RxCb)(void);
void (*HW_huart1TxCb)(void);
#endif

/* Functions Definition ------------------------------------------------------*/

void MX_UART_Init(hw_uart_id_t uart)
{
  UART_HandleTypeDef *handle = NULL;
  USART_TypeDef *instance = NULL;
  switch(uart){
    case hw_uart1:
#if (CFG_HW_USART1_ENABLED != 1)
      return;
#endif
      handle = &huart1;
      instance = USART1;
      break;
    default: Error_Handler();
  }
  handle->Instance = instance;
  handle->Init.BaudRate = 115200;
  handle->Init.WordLength = UART_WORDLENGTH_8B;
  handle->Init.StopBits = UART_STOPBITS_1;
  handle->Init.Parity = UART_PARITY_NONE;
  handle->Init.Mode = UART_MODE_TX_RX;
  handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  handle->Init.OverSampling = UART_OVERSAMPLING_16;
  handle->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  handle->Init.ClockPrescaler = UART_PRESCALER_DIV1;
  handle->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  handle->FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(handle) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(handle) != HAL_OK)
  {
    Error_Handler();
  }
}

void MX_UART_Deinit(hw_uart_id_t uart)
{
  UART_HandleTypeDef *handle = NULL;
  switch(uart){
    case hw_uart1:
#if (CFG_HW_USART1_ENABLED != 1)
      return;
#endif
      handle = &huart1;
      break;
    default: Error_Handler();
  }
  if (HAL_UART_DeInit(handle) != HAL_OK)
  {
    Error_Handler();
  }
}

hw_status_t HW_UART_Receive_IT(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size, void (*cb)(void))
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  hw_status_t hw_status = hw_uart_ok;
  
  switch (hw_uart_id)
  {
#if (CFG_HW_USART1_ENABLED == 1)
    case hw_uart1:
      HW_UART_RX_IT(huart1, USART1);
      break;
#endif
 
    default:
      break;
  }
  
  switch (hal_status)
  {
    case HAL_OK:
      hw_status = hw_uart_ok;
      break;
    
    case HAL_ERROR:
      hw_status = hw_uart_error;
      break;
    
    case HAL_BUSY:
      hw_status = hw_uart_busy;
      break;
    
    case HAL_TIMEOUT:
      hw_status = hw_uart_to;
      break;
    
    default:
      break;
  }
  
  return hw_status;
}

hw_status_t HW_UART_Transmit_IT(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size,  void (*cb)(void))
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  hw_status_t hw_status = hw_uart_ok;
  
  switch (hw_uart_id)
  {
#if (CFG_HW_USART1_ENABLED == 1)
    case hw_uart1:
      HW_UART_TX_IT(huart1, USART1);
      break;
#endif

    default:
      break;
  }
  
  switch (hal_status)
  {
    case HAL_OK:
      hw_status = hw_uart_ok;
      break;
    
    case HAL_ERROR:
      hw_status = hw_uart_error;
      break;
    
    case HAL_BUSY:
      hw_status = hw_uart_busy;
      break;
    
    case HAL_TIMEOUT:
      hw_status = hw_uart_to;
      break;
    
    default:
      break;
  }
  
  return hw_status;
}

hw_status_t HW_UART_Transmit(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size,  uint32_t timeout)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  hw_status_t hw_status = hw_uart_ok;
  
  switch (hw_uart_id)
  {
#if (CFG_HW_USART1_ENABLED == 1)
    case hw_uart1:
      HW_UART_TX(huart1, USART1);
      break;
#endif
    
    default:
      break;
  }
  
  switch (hal_status)
  {
    case HAL_OK:
      hw_status = hw_uart_ok;
      break;
    
    case HAL_ERROR:
      hw_status = hw_uart_error;
      break;
    
    case HAL_BUSY:
      hw_status = hw_uart_busy;
      break;
    
    case HAL_TIMEOUT:
      hw_status = hw_uart_to;
      break;
    
    default:
      break;
  }
  
  return hw_status;
}

hw_status_t HW_UART_Transmit_DMA(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size, void (*cb)(void))
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  hw_status_t hw_status = hw_uart_ok;
  
  switch (hw_uart_id)
  {
#if (CFG_HW_USART1_ENABLED == 1)
    case hw_uart1:
      HW_huart1TxCb = cb;
      huart1.Instance = USART1;
      hal_status = HAL_UART_Transmit_DMA(&huart1, p_data, size);
      break;
#endif
    
    default:
      break;
  }
  
  switch (hal_status)
  {
    case HAL_OK:
      hw_status = hw_uart_ok;
      break;
    
    case HAL_ERROR:
      hw_status = hw_uart_error;
      break;
    
    case HAL_BUSY:
      hw_status = hw_uart_busy;
      break;
    
    case HAL_TIMEOUT:
      hw_status = hw_uart_to;
      break;
    
    default:
      break;
  }
  
  return hw_status;
}

#if 0
void HW_UART_Interrupt_Handler(hw_uart_id_t hw_uart_id)
{
  switch (hw_uart_id)
  {
#if (CFG_HW_USART1_ENABLED == 1)
    case hw_uart1:
      HAL_UART_IRQHandler(&huart1);
      break;
#endif
    
    default:
      break;
  }
  
  return;
}

void HW_UART_DMA_Interrupt_Handler(hw_uart_id_t hw_uart_id)
{
  switch (hw_uart_id)
  {
#if (CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
    case hw_uart1:
      HAL_DMA_IRQHandler(huart1.hdmatx);
      break;
#endif
    
    default:
      break;
  }
  
  return;
}
#endif

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  switch ((uint32_t)huart->Instance)
  {
#if (CFG_HW_USART1_ENABLED == 1)
    case (uint32_t)USART1:
      if(HW_huart1RxCb)
      {
          HW_huart1RxCb();
      }
      break;
#endif
    
    default:
      break;
  }
  
  return;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  switch ((uint32_t)huart->Instance)
  {
#if (CFG_HW_USART1_ENABLED == 1)
    case (uint32_t)USART1:
      if(HW_huart1TxCb)
      {
          HW_huart1TxCb();
      }
      break;
#endif
    
    default:
      break;
  }
  
  return;
}

