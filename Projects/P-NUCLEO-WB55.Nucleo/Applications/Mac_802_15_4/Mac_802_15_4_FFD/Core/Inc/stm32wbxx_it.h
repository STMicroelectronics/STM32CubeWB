/**
  ******************************************************************************
  * @file    stm32wbxx_it.h
  * @author  MCD Application Team
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WB_IT_H
#define STM32WB_IT_H

#include "stm32wbxx_hal.h"
#include "hw_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* External variable ----------------------------------------------------------*/
#ifdef USAGE_OF_VCP
extern PCD_HandleTypeDef hpcd; /* Defined in the Thread Middleware             */
#endif /* USAGE_OF_VCP */

/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void SysTick_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void IPCC_C1_RX_IRQHandler(void);
void IPCC_C1_TX_IRQHandler(void);
#if(CFG_HW_USART1_ENABLED == 1)
void USART1_IRQHandler(void);
#endif
#if(CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
void CFG_HW_USART1_DMA_TX_IRQHandler(void);
#endif
#if(CFG_HW_LPUART1_ENABLED == 1)
void LPUART1_IRQHandler(void);
#endif
#if(CFG_HW_LPUART1_DMA_TX_SUPPORTED == 1)
void CFG_HW_LPUART1_DMA_TX_IRQHandler(void);
#endif
#ifdef USAGE_OF_VCP
void USB_LP_IRQHandler(void);
void USB_HP_IRQHandler(void);
void USB_FS_WKUP_IRQHandler(void);
#endif /* USAGE_OF_VCP */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* STM32WB_IT_H */

