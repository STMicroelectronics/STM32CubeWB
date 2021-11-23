/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define HSE_TRIM_VALID 0xACCE5500U
#define HSE_TRIM_VALID_MSK 0xFFFFFF00U

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
void Config_HSE_tuning(const uint32_t hse_tuning);
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
