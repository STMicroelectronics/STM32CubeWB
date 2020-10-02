/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported variables --------------------------------------------------------*/
#if (CFG_HW_LPUART1_ENABLED == 1)
extern UART_HandleTypeDef hlpuart1;
#if (CFG_HW_LPUART1_DMA_TX_SUPPORTED == 1)
extern DMA_HandleTypeDef hdma_lpuart1_tx;
#endif
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern UART_HandleTypeDef huart1;
#if (CFG_HW_USART1_DMA_TX_SUPPORTED == 1)
extern DMA_HandleTypeDef hdma_usart1_tx;
#endif
#endif
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config_HSE(uint32_t usePLL);
void SystemClock_Config_MSI(uint32_t usePLL, uint32_t stopHSE);
#if (CFG_HW_LPUART1_ENABLED == 1)
void MX_LPUART1_UART_Init(void);
void MX_LPUART1_UART_DeInit(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
void MX_USART1_UART_Init(void);
void MX_USART1_UART_DeInit(void);
#endif


/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
