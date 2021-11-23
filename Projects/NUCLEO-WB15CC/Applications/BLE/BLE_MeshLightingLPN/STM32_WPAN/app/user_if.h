/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_if.h
  * @author  MCD Application Team
  * @brief   Header file for User interface file 
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
#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_conf.h"
#include "ble_clock.h" 

#define BUTTON_PRESSED GPIO_PIN_RESET

/* Exported typedefs ---------------------------------------------*/
typedef GPIO_PinState BUTTON_STATE;
typedef void (* IO_RECEIVE_DATA_USER_CALLBACK_TYPE) (uint8_t * rx_data, uint16_t data_size);

/* Exported Functions Prototypes ---------------------------------------------*/

#endif /* _USER_INTERFACE_H_ */


