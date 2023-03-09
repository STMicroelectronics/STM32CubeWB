/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    TIM/TIM_OCToggle/Inc/main.h
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
#ifndef __MAIN_H
#define __MAIN_H

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
/*## Define Timer period and pulse #######################################*/
/* ---------------------------------------------------------------------------
   TIM2 Configuration: Output Compare Toggle Mode:

  To get TIM2 counter clock at 1 MHz, the prescaler is computed as follows:
  Prescaler = (TIM2CLK / TIM2 counter clock) - 1
  Prescaler = (SystemCoreClock /1000000) - 1

  CC1 update rate = TIM2 counter clock / uhCCR1_Val
                  = 1 MHz/625 = 1600 Hz
  ==> So the TIM2 Channel 1 generates a periodic signal with a frequency equal
      to 800 Hz.

  CC2 update rate = TIM2 counter clock / uhCCR2_Val
                  = 1 MHz/1250 = 800 Hz
  ==> So the TIM2 Channel 2 generates a periodic signal with a frequency equal
      to 400 Hz.

  CC3 update rate = TIM2 counter clock / uhCCR3_Val
                  = 1 MHz/2500 = 400 Hz
  ==> So the TIM2 Channel 3 generates a periodic signal with a frequency equal
      to 200 Hz.

  CC4 update rate = TIM2 counter clock / uhCCR4_Val
                  = 1 MHz/5000 = 200 Hz
  ==> So the TIM2 Channel 4 generates a periodic signal with a frequency equal
      to 100 Hz.
  --------------------------------------------------------------------------- */
#define PRESCALER_VALUE (uint32_t)((SystemCoreClock / 1000000) - 1)

#define PULSE1_VALUE 625
#define PULSE2_VALUE 1250
#define PULSE3_VALUE 2500
#define PULSE4_VALUE 5000

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
