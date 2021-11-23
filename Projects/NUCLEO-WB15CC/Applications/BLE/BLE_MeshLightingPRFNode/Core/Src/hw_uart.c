/* USER CODE BEGIN Header */
/**
  ******************************************************************************
 * @file    hw_uart.c
 * @author  MCD Application Team
 * @brief   hardware access
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


/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;

/* Macros --------------------------------------------------------------------*/
#define HW_UART_RX_IT(__HANDLE__, __USART_BASE__)                                                   \
        do{                                                                                         \
            HW_##__HANDLE__##RxCb = cb;                                                             \
            (__HANDLE__).Instance = (__USART_BASE__);                                               \
            HAL_UART_Receive_IT(&(__HANDLE__), p_data, size);                                       \
        } while(0)

#define HW_UART_TX_IT(__HANDLE__, __USART_BASE__)                                                   \
        do{                                                                                         \
            HW_##__HANDLE__##TxCb = cb;                                                             \
            (__HANDLE__).Instance = (__USART_BASE__);                                               \
            HAL_UART_Transmit_IT(&(__HANDLE__), p_data, size);                                      \
        } while(0)

#define HW_UART_TX(__HANDLE__, __USART_BASE__)                                                      \
        do{                                                                                         \
            (__HANDLE__).Instance = (__USART_BASE__);                                               \
            hal_status = HAL_UART_Transmit(&(__HANDLE__), p_data, size, timeout);                   \
        } while(0)

/* Variables -----------------------------------------------------------------*/
DMA_HandleTypeDef HW_hdma_huart1_tx ={0};
void (*HW_huart1RxCb)(void);
void (*HW_huart1TxCb)(void);

DMA_HandleTypeDef HW_hdma_hlpuart1_tx ={0};
void (*HW_hlpuart1RxCb)(void);
void (*HW_hlpuart1TxCb)(void);

void HW_UART_Receive_IT(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size, void (*cb)(void))
{
  switch (hw_uart_id)
  {
    case hw_uart1:
      HW_UART_RX_IT(huart1, USART1);
      break;
    
    case hw_lpuart1:
      HW_UART_RX_IT(hlpuart1, LPUART1);
      break;
    
    default:
      break;
  }
  
  return;
}

void HW_UART_Transmit_IT(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size,  void (*cb)(void))
{
  switch (hw_uart_id)
  {
    case hw_uart1:
      HW_UART_TX_IT(huart1, USART1);
      break;
    
    case hw_lpuart1:
      HW_UART_TX_IT(hlpuart1, LPUART1);
      break;
    
    default:
      break;
  }
  
  return;
}

hw_status_t HW_UART_Transmit(hw_uart_id_t hw_uart_id, uint8_t *p_data, uint16_t size,  uint32_t timeout)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  hw_status_t hw_status = hw_uart_ok;
  
  switch (hw_uart_id)
  {
    case hw_uart1:
      HW_UART_TX(huart1, USART1);
      break;
    
    case hw_lpuart1:
                HW_UART_TX(hlpuart1, LPUART1);
      break;
    
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
    case hw_uart1:
      HW_huart1TxCb = cb;
      huart1.Instance = USART1;
      hal_status = HAL_UART_Transmit_DMA(&huart1, p_data, size);
      break;
    
    case hw_lpuart1:
      HW_hlpuart1TxCb = cb;
                hlpuart1.Instance = LPUART1;
                hal_status = HAL_UART_Transmit_DMA(&hlpuart1, p_data, size);
      break;
    
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  switch ((uint32_t)huart->Instance)
  {
    case (uint32_t)USART1:
      if(HW_huart1RxCb)
      {
        HW_huart1RxCb();
      }
      break;
    
    case (uint32_t)LPUART1:
      if(HW_hlpuart1RxCb)
      {
        HW_hlpuart1RxCb();
      }
      break;
    
    default:
      break;
  }
  
  return;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  switch ((uint32_t)huart->Instance)
  {
    case (uint32_t)USART1:
      if(HW_huart1TxCb)
      {
        HW_huart1TxCb();
      }
      break;
    
    case (uint32_t)LPUART1:
      if(HW_hlpuart1TxCb)
      {
        HW_hlpuart1TxCb();
      }
      break;
    
    default:
      break;
  }
  
  return;
}
