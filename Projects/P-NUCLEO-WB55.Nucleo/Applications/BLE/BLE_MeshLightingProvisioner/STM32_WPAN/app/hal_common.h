/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hal_common.h
  * @author  MCD Application Team
  * @brief   Common functions of HAL file 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#ifndef _HAL_H_
#define _HAL_H_

/* Includes ------------------------------------------------------------------*/

#if defined(NUCLEO_L152RE) || defined(USE_STM32WBXX_NUCLEO) || USE_STM32WBXX_USB_DONGLE /* NUCLEO_L152RE */
#include "user_if.h" 
#else /* not NUCLEO_L152RE */
#if defined(BLUENRG1) || defined(BLUENRG2) /* BLUENRG1 or BLUENRG2 */
#include "hal.h"   
#else /* not BLUENRG1 or BLUENRG2 */
#error "Unsupported board"
#endif /* BLUENRG1 or BLUENRG2 */
#endif /* NUCLEO_L152RE */

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>

#include "types.h"

void SetLed(int state);
BUTTON_STATE GetButtonState(void);
BUTTON_STATE GetButton2State(void);
void InitDevice(void);

void ShouldSleepFunc(void);

#endif /* _HAL_H_ */

