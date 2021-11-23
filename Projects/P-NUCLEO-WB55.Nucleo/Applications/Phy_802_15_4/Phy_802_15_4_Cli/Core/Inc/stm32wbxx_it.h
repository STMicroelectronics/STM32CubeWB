/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32wbxx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef __STM32WBxx_IT_H
#define __STM32WBxx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_common.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
#ifdef STM32WB35xx
void DMA1_Channel4_IRQHandler(void);
void DMA2_Channel4_IRQHandler(void);
#else
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
#endif
void USART1_IRQHandler(void);
void LPUART1_IRQHandler(void);
/* USER CODE BEGIN EFP */
#if 0
void RTC_WKUP_IRQHandler(void);
#endif
void IPCC_C1_TX_IRQHandler(void);
void IPCC_C1_RX_IRQHandler(void);
#if 0 // Not needed for LLD tests : remove to use less power
void PUSH_BUTTON_SW1_EXTI_IRQHandler(void);
void PUSH_BUTTON_SW2_EXTI_IRQHandler(void);
void PUSH_BUTTON_SW3_EXTI_IRQHandler(void);
#endif
/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __STM32WBxx_IT_H */
