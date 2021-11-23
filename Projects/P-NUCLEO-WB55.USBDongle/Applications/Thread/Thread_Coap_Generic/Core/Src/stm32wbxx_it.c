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
#include "app_common.h"
#include "stm32wbxx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */
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
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{ 
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while(1 == 1); 
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
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

void RTC_WKUP_IRQHandler(void)
{
  HW_TS_RTC_Wakeup_Handler();
}

void IPCC_C1_TX_IRQHandler(void)
{
  HW_IPCC_Tx_Handler();

  return;
}

void IPCC_C1_RX_IRQHandler(void)
{
  HW_IPCC_Rx_Handler();
  return;
}

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
  * @brief  This function handles USB FS global interrupt request.
  * @param  None
  * @retval None
  */
void USB_LP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
  * @brief  This function handles USB FS global interrupt request.
  * @param  None
  * @retval None
  */
void USB_HP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
  * @brief  This function handles USB FS global interrupt request.
  * @param  None
  * @retval None
  */
void USB_FS_WKUP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */


/******************************************************************************/
/* STM32WBxx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32wbxx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
/**
  * @brief  This function handles EXTI15_10_IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BUTTON_SW1_PIN);
}
/* USER CODE END 1 */
