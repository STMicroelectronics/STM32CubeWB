/**
  ******************************************************************************
 * @file    stm32xx_it.c
 * @author  MCD Application Team
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
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
#include "stm32wbxx_it.h"
#include "app_common.h"
#include "hw_conf.h"

/******************************************************************************/
/*            Cortex-M4+ Processor Exceptions Handlers                         */
/******************************************************************************/


/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
  while(1 == 1);
}

/**
 * @brief  This function handles SysTick exception.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles IPCC RX occupied global interrupt request.
  * @param  None
  * @retval None
  */
void IPCC_C1_RX_IRQHandler(void)
{
  HW_IPCC_Rx_Handler();
}

/**
  * @brief  This function handles IPCC TX free global interrupt request.
  * @param  None
  * @retval None
  */
void IPCC_C1_TX_IRQHandler(void)
{
   HW_IPCC_Tx_Handler();
}

/**
 * @brief  This function handles the UART1 IT handler.
 * @param  None
 * @retval None
 */
#if(CFG_HW_USART1_ENABLED == 1)
void USART1_IRQHandler(void)
{
  HW_UART_Interrupt_Handler(hw_uart1);
}
#endif

/**
 * @brief  This function handles the UART1 DMA IT handler.
 * @param  None
 * @retval None
 */
#if(CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
void CFG_HW_USART1_DMA_TX_IRQHandler( void )
{
  HW_UART_DMA_Interrupt_Handler(hw_uart1);
}
#endif

/**
 * @brief  This function handles the LPUART1 IT handler.
 * @param  None
 * @retval None
 */
#if(CFG_HW_LPUART1_ENABLED == 1)
void LPUART1_IRQHandler(void)
{
  HW_UART_Interrupt_Handler(hw_lpuart1);
}
#endif

/**
 * @brief  This function handles the LPUART1 DMA IT handler.
 * @param  None
 * @retval None
 */
#if(CFG_HW_LPUART1_DMA_TX_SUPPORTED == 1)
void CFG_HW_LPUART1_DMA_TX_IRQHandler( void )
{
  HW_UART_DMA_Interrupt_Handler(hw_lpuart1);
}
#endif


#ifdef USAGE_OF_VCP
/**
  * @brief  This function handles USB FS global interrupt request.
  * @param  None
  * @retval None
  */
void USB_LP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}
#endif /* USAGE_OF_VCP */

#ifdef USAGE_OF_VCP
/**
  * @brief  This function handles USB FS global interrupt request.
  * @param  None
  * @retval None
  */
void USB_HP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}
#endif /* USAGE_OF_VCP */

#ifdef USAGE_OF_VCP
/**
  * @brief  This function handles USB FS global interrupt request.
  * @param  None
  * @retval None
  */
void USB_FS_WKUP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}
#endif /* USAGE_OF_VCP */

/******************************************************************************/
/* STM32WBxx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32wbxx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
/**
  * @brief  This function handles External External line 4 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_SW1_PIN);
}
/**
  * @brief  This function handles External External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_SW2_PIN);
}
/**
  * @brief  This function handles External External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_SW3_PIN);
}

/* USER CODE END 1 */
