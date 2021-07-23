/**
  ******************************************************************************
  * @file    app_vl53l0x.h 
  * @author  MCD Application Team
  * @brief   Header for app_vl53l0x.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_VL53L0X_H
#define __APP_VL53L0X_H

/* Includes ------------------------------------------------------------------*/
#include "vl53l0x_def.h"
#include "vl53l0x_api.h"
#include "vl53l0x_tof.h"
//#include "stm32wb5M_DK_bus.h"
#include "stm32wb5mm_dk_bus.h"
#include "stm32wb5mm_dk.h"
//#include "stm32wbxx_discovery_lcd.h"
#include "stm32_seq.h"
#include "app_common.h"
/* Defines -------------------------------------------------------------------*/
#define PROXIMITY_I2C_ADDRESS         ((uint16_t)0x0052)
#define VL53L0X_ID                    ((uint16_t)0xEEAA)

/* Exported functions ------------------------------------------------------- */
void VL53L0X_PROXIMITY_Init(void);
uint16_t VL53L0X_PROXIMITY_GetDistance(void);
void VL53L0X_PROXIMITY_PrintValue(void);
void VL53L0X_Start_Measure(void);
void VL53L0X_Stop_Measure(void);
#endif /* __APP_VL53L0X_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
