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
  * Copyright (c) 2017(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32wbxx_it.h"
#include "stm32wb5mm_dk.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd;
extern SAI_HandleTypeDef hAudioInSai;

/******************************************************************************/
/*            Cortex-M4 and Cortex-M0+ Processors Exceptions Handlers         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
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
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
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

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
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

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/******************************************************************************/
/*                 STM32WBxx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32wbxxxx.s).                                             */
/******************************************************************************/

/**
  * @brief  This function handles EXTI0_IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hpb_exti[BUTTON_USER1]);
}

/**
  * @brief  This function handles EXTI13_IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  BSP_PB_IRQHandler(BUTTON_USER1);
}

/**
  * @brief  This function handles TIM17 IRQ Handler.
  * @param  None
  * @retval None
  */
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
  BSP_PWM_LED_IRQHandler();
}

/**
  * @brief  This function handles DMA Stream interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hAudioInSai.hdmarx);
}

/**
  * @brief  This function handles USB Handler.
  * @param  None
  * @retval None
  */
void USB_LP_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}

/**
  * @}
  */ 

/**
  * @}
  */
