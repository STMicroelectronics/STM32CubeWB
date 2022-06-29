/**
  ******************************************************************************
  * @file    b_wb1m_wpan1_motion_sensors_ex.h
  * @author  MEMS Software Solutions Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the b_wb1m_wpan1_motion_sensors_ex.c driver.
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
#ifndef B_WB1M_WPAN1_MOTION_SENSOR_EX_H
#define B_WB1M_WPAN1_MOTION_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "b_wb1m_wpan1_motion_sensors.h"

/** @addtogroup BSP BSP
  * @{
  */

/** @addtogroup B_WB1M_WPAN1 B-WB1M-WPAN1
  * @{
  */

/** @addtogroup B_WB1M_WPAN1_MOTION_SENSOR_EX B-WB1M-WPAN1 MOTION SENSOR EX
  * @{
  */

/** @addtogroup B_WB1M_WPAN1_MOTION_SENSOR_EX_Exported_Functions B-WB1M-WPAN1 MOTION SENSOR EX Exported Functions
  * @{
  */

int32_t BSP_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t BSP_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t BSP_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BSP_MOTION_SENSOR_EX_H */
