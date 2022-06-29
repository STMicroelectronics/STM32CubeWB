/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32wbxx_nucleo.h"
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
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

#define RAM_BASE 0x20000000
#define RAM_BB_BASE 0x22000000
#define VariableBit_Reset_BB(VariableAddress, BitNumber) \
(*(volatile uint32_t *) (RAM_BB_BASE | ((VariableAddress - RAM_BASE) << 5) | ((BitNumber) << 2)) = 0)
#define VariableBit_Set_BB(VariableAddress, BitNumber) \
(*(volatile uint32_t *) (RAM_BB_BASE | ((VariableAddress - RAM_BASE) << 5) | ((BitNumber) << 2)) = 1)
#define VariableBit_Get_BB(VariableAddress, BitNumber) \
(*(volatile uint32_t *) (RAM_BB_BASE | ((VariableAddress - RAM_BASE) << 5) | ((BitNumber) << 2)))

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
