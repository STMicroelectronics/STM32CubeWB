/**
  ******************************************************************************
  * @file    stm32wbxx_it.c
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
#include "appli_mesh.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
aPwmLedGsData_TypeDef aPwmLedGsData = {PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_0_7};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
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
  aPwmLedGsData_TypeDef aPwmLedGsData;

  aPwmLedGsData[PWM_LED_RED]   = PWM_LED_GSDATA_7_0;
  aPwmLedGsData[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
  aPwmLedGsData[PWM_LED_BLUE]  = PWM_LED_GSDATA_OFF;
  LED_On(aPwmLedGsData);
#ifndef DISABLE_TRACES
  printf("HARDFAULT !\r\n");
#endif
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
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
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/********************************************************************************/
/*                 STM32WBxx Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s), for the          */
/*  available peripheral interrupt handler's name please refer to the startup   */
/*  file (startup_stm32wb55xx_cm4.s).                                           */
/********************************************************************************/

/**
 * @brief  This function handles External line
 *         interrupt request.
 * @param  None
 * @retval None
 */
void PUSH_BUTTON_SW_EXTI_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);        //BUTTON_SW1_PIN
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);        //BUTTON_SW2_PIN
}

#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION       
/**
 * @brief  This function handles External line
 *         interrupt request.
 * @param  None
 * @retval None
 */
void POWEROFF_EXTI_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(POWEROFF_PIN);
}
#endif

/**
  * @brief  This function handles TIM17 IRQ Handler.
  * @param  None
  * @retval None
  */
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
  BSP_PWM_LED_IRQHandler();
}

#if(CFG_HW_USART1_ENABLED == 1)
void USART1_IRQHandler(void)
{
  HW_UART_Interrupt_Handler(hw_uart1);
}
#endif

#if(CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
void CFG_HW_USART1_DMA_TX_IRQHandler( void )
{
  HW_UART_DMA_Interrupt_Handler(hw_uart1);
}
#endif

#if(CFG_HW_LPUART1_ENABLED == 1)
void LPUART1_IRQHandler(void)
{
  HW_UART_Interrupt_Handler(hw_lpuart1);
}
#endif

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

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM1 global interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_IRQn 0 */

  /* USER CODE END TIM1_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_IRQn 1 */

  /* USER CODE END TIM1_IRQn 1 */
}

