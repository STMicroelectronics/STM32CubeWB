/**
  ******************************************************************************
  * @file    stm32wb5mm_dk_env_sensors.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32wb5mm_dk_env_sensors.c driver.
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
#ifndef STM32WB5MM_DK_ENV_SENSORS_H
#define STM32WB5MM_DK_ENV_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wb5mm_dk_conf.h"
#include "stm32wb5mm_dk_errno.h"
#include "env_sensor.h"

#ifndef USE_ENV_SENSOR_STTS22H_0
#define USE_ENV_SENSOR_STTS22H_0          1
#endif

#if (USE_ENV_SENSOR_STTS22H_0 == 1)
#include "stts22h.h"
#endif

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup STM32WB5MM_DK
 * @{
 */

/** @addtogroup STM32WB5MM_DK_ENV_SENSORS
 * @{
 */

/** @defgroup STM32WB5MM_DK_ENV_SENSORS_Exported_Types STM32WB5MM-DK ENV SENSORS Exported Types
 * @{
 */

/* Environmental Sensor instance Info */
typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
} ENV_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} ENV_SENSOR_Ctx_t;

/**
 * @}
 */

/** @defgroup STM32WB5MM_DK_ENV_SENSORS_Exported_Constants STM32WB5MM-DK ENV SENSORS Exported Constants
 * @{
 */

#if (USE_ENV_SENSOR_STTS22H_0 == 1)
#define ENV_SENSOR_STTS22H_0 0
#endif

#ifndef ENV_TEMPERATURE
#define ENV_TEMPERATURE      1U
#endif
#ifndef ENV_PRESSURE
#define ENV_PRESSURE         2U
#endif
#ifndef ENV_HUMIDITY
#define ENV_HUMIDITY         4U
#endif

#define ENV_SENSOR_FUNCTIONS_NBR    3U
#define ENV_SENSOR_INSTANCES_NBR    (USE_ENV_SENSOR_STTS22H_0)

#if (ENV_SENSOR_INSTANCES_NBR == 0)
#error "No environmental sensor instance has been selected"
#endif

/**
 * @}
 */

/** @addtogroup STM32WB5MM_DK_ENV_SENSORS_Exported_Functions
 * @{
 */

int32_t BSP_ENV_SENSOR_Init(uint32_t Instance, uint32_t Functions);
int32_t BSP_ENV_SENSOR_DeInit(uint32_t Instance);
int32_t BSP_ENV_SENSOR_GetCapabilities(uint32_t Instance, ENV_SENSOR_Capabilities_t *Capabilities);
int32_t BSP_ENV_SENSOR_ReadID(uint32_t Instance, uint8_t *Id);
int32_t BSP_ENV_SENSOR_Enable(uint32_t Instance, uint32_t Function);
int32_t BSP_ENV_SENSOR_Disable(uint32_t Instance, uint32_t Function);
int32_t BSP_ENV_SENSOR_GetOutputDataRate(uint32_t Instance, uint32_t Function, float *Odr);
int32_t BSP_ENV_SENSOR_SetOutputDataRate(uint32_t Instance, uint32_t Function, float Odr);
int32_t BSP_ENV_SENSOR_GetValue(uint32_t Instance, uint32_t Function, float *Value);

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

#endif /* STM32WB5MM_DK_ENV_SENSORS_H */
