/**
  ******************************************************************************
  * @file    BSP/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017(-2021) STMicroelectronics.
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
#include "stm32wb5mm_dk.h"
#include "stm32wb5mm_dk_env_sensors.h"
#include "stm32wb5mm_dk_motion_sensors.h"
#include "stm32wb5mm_dk_errno.h"
#include "stm32wb5mm_dk_conf.h"
#include "stm32wb5mm_dk_bus.h"
#include "stm32wb5mm_dk_lcd.h"
#include "stm32wb5mm_dk_qspi.h"
#include "stm32_lcd.h"
#include "stm32wb5mm_dk_audio.h"
#include "stm32wbxx.h"
#include "stm32wbxx_it.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio_in.h"
#include "usbd_audio_if.h"    

#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50];
  uint32_t DemoIndex;
}BSP_DemoTypedef;
/* Exported variables --------------------------------------------------------*/
extern __IO uint32_t ButtonState;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/*Comment this define if you want to configure and start acquisition 
depending on USB functionalities implemented by user*/
#define DISABLE_USB_DRIVEN_ACQUISITION
#define USB_IRQHandler OTG_FS_IRQHandler
/* Exported functions ------------------------------------------------------- */
void Temperature_Test(void);
void Gyro_Test(void);
void Accelero_Test(void);
void Proximity_Test(void);
void led_demo(void);
uint8_t CheckForUserInput(void);
void Display_Demo(void);
void LCD_demo(void);
void Led_demo(void);
void QSPI_demo (void);
void Audio_Record_Demo(void);

void Error_Handler(void);
uint32_t Serial_Scanf(uint32_t value);

#endif /* MAIN_H */
