/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_motion_sensors.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32wb5mm_dk_motion_sensors.c driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WB5MM_DK_MOTION_SENSORS_H
#define STM32WB5MM_DK_MOTION_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_conf.h"
#include "stm32wb5mm_dk_errno.h"
#include "motion_sensor.h"

#ifndef USE_MOTION_SENSOR_ISM330DHCX_0
#define USE_MOTION_SENSOR_ISM330DHCX_0        1
#endif

#if (USE_MOTION_SENSOR_ISM330DHCX_0 == 1)
#include "ism330dhcx.h"
#endif

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup STM32WB5MM_DK STM32WB5MM-DK
 * @{
 */

/** @addtogroup STM32WB5MM_DK_MOTION_SENSORS
 * @{
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSORS_Exported_Types STM32WB5MM-DK MOTION SENSORS Exported Types
 * @{
 */

typedef struct
{
  int32_t x;
  int32_t y;
  int32_t z;
} MOTION_SENSOR_Axes_t;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} MOTION_SENSOR_AxesRaw_t;

/* Motion Sensor instance Info */
typedef struct
{
  uint8_t  Acc;
  uint8_t  Gyro;
  uint8_t  Magneto;
  uint8_t  LowPower;
  uint32_t GyroMaxFS;
  uint32_t AccMaxFS;
  uint32_t MagMaxFS;
  float    GyroMaxOdr;
  float    AccMaxOdr;
  float    MagMaxOdr;
} MOTION_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} MOTION_SENSOR_Ctx_t;

/**
 * @}
 */

/** @defgroup STM32WB5MM_DK_MOTION_SENSOR_Exported_Constants STM32WB5MM-DK MOTION SENSOR Exported Constants
 * @{
 */

#if (USE_MOTION_SENSOR_ISM330DHCX_0 == 1)
#define MOTION_SENSOR_ISM330DHCX_0 0
#endif

#ifndef MOTION_GYRO
#define MOTION_GYRO             1U
#endif
#ifndef MOTION_ACCELERO
#define MOTION_ACCELERO         2U
#endif
#ifndef MOTION_MAGNETO
#define MOTION_MAGNETO          4U
#endif

#define MOTION_SENSOR_FUNCTIONS_NBR    3U
#define MOTION_SENSOR_INSTANCES_NBR    (USE_MOTION_SENSOR_ISM330DHCX_0)

#if (MOTION_SENSOR_FUNCTIONS_NBR == 0)
#error "No motion sensor instance has been selected"
#endif

/**
 * @}
 */

/** @addtogroup STM32WB5MM_DK_MOTION_SENSORS_Exported_Functions
 * @{
 */

int32_t BSP_MOTION_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t BSP_MOTION_SENSOR_DeInit(uint32_t Instance);
int32_t BSP_MOTION_SENSOR_GetCapabilities(uint32_t Instance, MOTION_SENSOR_Capabilities_t *Capabilities);
int32_t BSP_MOTION_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t BSP_MOTION_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t BSP_MOTION_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t BSP_MOTION_SENSOR_GetAxes(uint32_t Instance, uint32_t Function, MOTION_SENSOR_Axes_t *Axes);
int32_t BSP_MOTION_SENSOR_GetAxesRaw(uint32_t Instance, uint32_t Function, MOTION_SENSOR_AxesRaw_t *Axes);
int32_t BSP_MOTION_SENSOR_GetSensitivity(uint32_t Instance, uint32_t Function, float *Sensitivity);
int32_t BSP_MOTION_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr);
int32_t BSP_MOTION_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr);
int32_t BSP_MOTION_SENSOR_GetFullScale(uint32_t Instance, uint32_t Function, int32_t *Fullscale);
int32_t BSP_MOTION_SENSOR_SetFullScale(uint32_t Instance, uint32_t Function, int32_t Fullscale);

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
#endif

#endif /* STM32WB5MM_DK_MOTION_SENSORS_H */
