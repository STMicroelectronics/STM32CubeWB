/**
  ******************************************************************************
  * @file    BSP/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Defines -------------------------------------------------------------------*/
/* #define USE_LPS22HB_TEMP */

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"
#include "b_wb1m_wpan1.h"
#include "b_wb1m_wpan1_env_sensors.h"
#include "b_wb1m_wpan1_motion_sensors.h"
#include "b_wb1m_wpan1_eeprom.h"
#include "b_wb1m_wpan1_errno.h"
#include "b_wb1m_wpan1_conf.h"
#include "b_wb1m_wpan1_bus.h"
#include "stm32wbxx.h"
#include "stm32wbxx_it.h"

#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void (*DemoFunc)(void);
  uint8_t DemoName[50];
  uint32_t DemoIndex;
} BSP_DemoTypeDef;

/* Exported variables --------------------------------------------------------*/
extern __IO uint32_t ButtonState;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypeDef))

/*Comment this define if you want to configure and start acquisition
depending on USB functionalities implemented by user*/
#define DISABLE_USB_DRIVEN_ACQUISITION
#define USB_IRQHandler OTG_FS_IRQHandler
/* Exported functions ------------------------------------------------------- */
void Led_demo(void);
void Temperature_demo(void);
void Gyro_demo(void);
void Accelero_demo(void);
void EEPROM_demo(void);
uint8_t CheckForUserInput(void);


void Error_Handler(void);
uint32_t Serial_Scanf(uint32_t value);

#endif /* MAIN_H */
