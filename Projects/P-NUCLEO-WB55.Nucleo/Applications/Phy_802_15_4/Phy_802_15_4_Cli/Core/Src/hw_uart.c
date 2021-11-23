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
#include "hw_conf.h"

/* Macros --------------------------------------------------------------------*/
#define HW_UART_RX_IT(__HANDLE__, __USART_BASE__)                                                   \
        do{                                                                                         \
            HW_##__HANDLE__##RxCb = cb;                                                             \
            (__HANDLE__).Instance = (__USART_BASE__);                                               \
            hal_status = HAL_UART_Receive_IT(&(__HANDLE__), p_data, size);                          \
        } while(0)

#define HW_UART_TX_IT(__HANDLE__, __USART_BASE__)                                                   \
        do{                                                                                         \
            HW_##__HANDLE__##TxCb = cb;                                                             \
            (__HANDLE__).Instance = (__USART_BASE__);                                               \
            hal_status = HAL_UART_Transmit_IT(&(__HANDLE__), p_data, size);                         \
        } while(0)

#define HW_UART_TX(__HANDLE__, __USART_BASE__)                                                      \
        do{                                                                                         \
            (__HANDLE__).Instance = (__USART_BASE__);                                               \
            hal_status = HAL_UART_Transmit(&(__HANDLE__), p_data, size, timeout);                   \
        } while(0)

/* Variables -----------------------------------------------------------------*/
#if (CFG_HW_USART1_ENABLED == 1)
    void (*HW_huart1RxCb)(void);
    void (*HW_huart1TxCb)(void);
#endif

#if (CFG_HW_LPUART1_ENABLED == 1)
    void (*HW_hlpuart1RxCb)(void);
    void (*HW_hlpuart1TxCb)(void);
#endif

/* Functions Definition ------------------------------------------------------*/
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

#if (CFG_HW_LPUART1_ENABLED == 1)
        case hw_lpuart1:
            HW_UART_RX_IT(hlpuart1, LPUART1);
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

#if (CFG_HW_LPUART1_ENABLED == 1)
        case hw_lpuart1:
            HW_UART_TX_IT(hlpuart1, LPUART1);
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

#if (CFG_HW_LPUART1_ENABLED == 1)
        case hw_lpuart1:
            HW_UART_TX(hlpuart1, LPUART1);
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

// To be use carrefuly as it must only be used when the UART is not used by another task or interrupt context
hw_status_t HW_UART_Transmit_LL(hw_uart_id_t hw_uart_id, uint8_t data)
{
    hw_status_t hw_status = hw_uart_ok;

    switch (hw_uart_id)
    {
#if (CFG_HW_USART1_ENABLED == 1)
        case hw_uart1:
            while (!(LL_USART_IsActiveFlag_TXE_TXFNF(USART1))) {};
            LL_USART_TransmitData8(USART1, data);
            break;
#endif

#if (CFG_HW_LPUART1_ENABLED == 1)
        case hw_lpuart1:
            while (!(LL_LPUART_IsActiveFlag_TXE_TXFNF(LPUART1))) {};
            LL_LPUART_TransmitData8(LPUART1, data);
            break;
#endif

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

#if (CFG_HW_LPUART1_ENABLED == 1)
        case hw_lpuart1:
            HW_hlpuart1TxCb = cb;
            hlpuart1.Instance = LPUART1;
            hal_status = HAL_UART_Transmit_DMA(&hlpuart1, p_data, size);
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

#if (CFG_HW_LPUART1_ENABLED == 1)
        case hw_lpuart1:
            HAL_UART_IRQHandler(&hlpuart1);
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

#if (CFG_HW_LPUART1_DMA_TX_SUPPORTED == 1)
        case hw_lpuart1:
            HAL_DMA_IRQHandler(hlpuart1.hdmatx);
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

#if (CFG_HW_LPUART1_ENABLED == 1)
        case (uint32_t)LPUART1:
            if(HW_hlpuart1RxCb)
            {
                HW_hlpuart1RxCb();
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

#if (CFG_HW_LPUART1_ENABLED == 1)
        case (uint32_t)LPUART1:
            if(HW_hlpuart1TxCb)
            {
                HW_hlpuart1TxCb();
            }
            break;
#endif

        default:
            break;
    }

    return;
}

