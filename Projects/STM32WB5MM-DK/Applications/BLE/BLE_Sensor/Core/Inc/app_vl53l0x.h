/**
  ******************************************************************************
  * @file    app_vl53l0x.h 
  * @author  MCD Application Team
  * @brief   Header for app_vl53l0x.c module
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
#ifndef __APP_VL53L0X_H
#define __APP_VL53L0X_H

/* Includes ------------------------------------------------------------------*/
#include "vl53l0x_def.h"
#include "vl53l0x_api.h"
#include "vl53l0x_tof.h"


#include "app_common.h"
/* Defines -------------------------------------------------------------------*/
#define VL53L0X_ID                    ((uint16_t)0xEEAA)

/* Exported functions ------------------------------------------------------- */
void VL53L0X_PROXIMITY_Init(void);
uint16_t VL53L0X_PROXIMITY_GetDistance(void);
void VL53L0X_PROXIMITY_PrintValue(void);
void VL53L0X_Start_Measure(void);
void VL53L0X_Stop_Measure(void);
#endif /* __APP_VL53L0X_H */

