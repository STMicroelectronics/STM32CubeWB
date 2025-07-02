/**
  ******************************************************************************
  * @file    sensors.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the sensors supported by the
  *          B-WB1M-WPAN1 board
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Demo of STTS22H temperature sensor.
  */
void Temperature_demo(void)
{
#if defined(USE_ENV_SENSOR_STTS22H_0)
  float temperature;
  temperature = 0;
  
  uint32_t ret = BSP_ENV_SENSOR_Init(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  ret = BSP_ENV_SENSOR_Enable(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  for(uint8_t i = 0; i < 10; i++)
  {
    ret = BSP_ENV_SENSOR_GetValue(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE, &temperature);
    if (ret != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    
    printf("\tTemperature = %f\n", (double)temperature);
    HAL_Delay(1000);
  }
  
  ret = BSP_ENV_SENSOR_Disable(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  ret = BSP_ENV_SENSOR_DeInit(ENV_SENSOR_STTS22H_0);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
#endif /* defined(USE_ENV_SENSOR_STTS22H_0) */
}

/**
  * @brief  Demo of ISM330DHCX accelerometer sensor.
  */
void Accelero_demo(void)
{
#if defined(USE_MOTION_SENSOR_ISM330DHCX_0)
  MOTION_SENSOR_Axes_t acceleration;

  uint32_t ret = BSP_MOTION_SENSOR_Init(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  ret = BSP_MOTION_SENSOR_Enable(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  for(uint8_t i = 0; i < 10; i++)
  {
    memset(&acceleration, 0, sizeof(MOTION_SENSOR_Axes_t));
    ret = BSP_MOTION_SENSOR_GetAxes(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO, &acceleration);
    if (ret != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    printf( "\tAccelero (x = %ld, y = %ld, z = %ld)\n", acceleration.x, acceleration.y, acceleration.z);

    HAL_Delay(1000);
  }
  
  ret = BSP_MOTION_SENSOR_Disable(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  ret = BSP_MOTION_SENSOR_DeInit(MOTION_SENSOR_ISM330DHCX_0);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
#endif /* USE_MOTION_SENSOR_ISM330DHCX_0 */
}

/**
  * @brief  Demo of ISM330DHCX gyroscope sensor.
  */
void Gyro_demo(void)
{
#if defined(USE_MOTION_SENSOR_ISM330DHCX_0)
  MOTION_SENSOR_Axes_t angular_velocity;

  uint32_t ret = BSP_MOTION_SENSOR_Init(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  ret = BSP_MOTION_SENSOR_Enable(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  for(uint8_t i = 0; i < 10; i++)
  {
    memset(&angular_velocity, 0, sizeof(MOTION_SENSOR_Axes_t));
    ret = BSP_MOTION_SENSOR_GetAxes(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO, &angular_velocity);
    if (ret != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    printf("\tGyro (x = %ld, y = %ld, z = %ld\n", angular_velocity.x, angular_velocity.y, angular_velocity.z);

    HAL_Delay(1000);
  }
  
  ret = BSP_MOTION_SENSOR_Disable(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
 
  ret = BSP_MOTION_SENSOR_DeInit(MOTION_SENSOR_ISM330DHCX_0);
  if (ret != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
#endif /* USE_MOTION_SENSOR_ISM330DHCX_0 */
}

/**
  * @}
  */

/**
  * @}
  */
