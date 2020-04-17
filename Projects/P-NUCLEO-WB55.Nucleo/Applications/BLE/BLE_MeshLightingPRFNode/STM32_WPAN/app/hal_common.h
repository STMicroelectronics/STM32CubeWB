/**
  ******************************************************************************
  * @file    hal_common.h
  * @author  BLE Mesh Team
  * @brief   Common functions of HAL file 
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
bool Accel_Process(uint8_t *evt);
bool Temperature_Read(int16_t *data);
void InitDevice(void);

void ShouldSleepFunc(void);

#endif /* _HAL_H_ */
/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

