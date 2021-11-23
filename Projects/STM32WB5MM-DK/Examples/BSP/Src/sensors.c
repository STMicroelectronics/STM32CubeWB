/**
  ******************************************************************************
* @file    sensors.c 
* @author  MCD Application Team
* @brief   This example code shows how to use the sensors supported by the 
*          STM32WB55MM-DK board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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

/** @addtogroup STM32WLxx_HAL_Examples
* @{
*/

/** @addtogroup BSP
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CELSIUS_ASCII_CODE 248

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Test of STTS22H and LPS22HB temperature sensors.
  */
void Temperature_Test(void)
{
  char temp[64];
  float temperature;
  temperature = 0;
  BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
  BSP_LCD_Refresh(0);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Temperature", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 12, (uint8_t *)"Measure", CENTER_MODE);
  UTIL_LCD_DrawRect(0, 0, 127 , 23, SSD1315_COLOR_WHITE);
  BSP_LCD_Refresh(0);
  
  (void)BSP_ENV_SENSOR_Init(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
  (void)BSP_ENV_SENSOR_Enable(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
  while (1)
  {
    (void)BSP_ENV_SENSOR_GetValue(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE, &temperature);
    sprintf(temp,"Temp=%f", temperature);
    UTIL_LCD_DisplayStringAt(0, 37, (uint8_t *)temp, CENTER_MODE);
    BSP_LCD_Refresh(0);

    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      (void)BSP_ENV_SENSOR_Disable(ENV_SENSOR_STTS22H_0, ENV_TEMPERATURE);
      (void)BSP_ENV_SENSOR_DeInit(ENV_SENSOR_STTS22H_0);
      return;
    }
  }
}

/**
  * @brief  Test of LSM6DSL accelerometer sensor.
  */
void Accelero_Test(void)
{
  char acc_x[64];
  char acc_y[64];
  char acc_z[64];
  MOTION_SENSOR_Axes_t acceleration;

  (void)BSP_MOTION_SENSOR_Init(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO);
  (void)BSP_MOTION_SENSOR_Enable(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO);

  while (1)
  {
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Accelerometer", CENTER_MODE);
    UTIL_LCD_DisplayStringAt(0, 12, (uint8_t *)"Measure ", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 0, 127 , 23, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    memset(&acceleration, 0, sizeof(MOTION_SENSOR_Axes_t));
    (void)BSP_MOTION_SENSOR_GetAxes(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO, &acceleration);
    sprintf(acc_x," ACC_X = %ld", acceleration.x);
    sprintf(acc_y," ACC_Y = %ld", acceleration.y);
    sprintf(acc_z," ACC_Z = %ld", acceleration.z);
    UTIL_LCD_DisplayStringAt(0, 25, (uint8_t *)acc_x, LEFT_MODE);
    UTIL_LCD_DisplayStringAt(0, 37, (uint8_t *)acc_y, LEFT_MODE);
    UTIL_LCD_DisplayStringAt(0, 49, (uint8_t *)acc_z, LEFT_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(1000);

    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      (void)BSP_MOTION_SENSOR_Disable(MOTION_SENSOR_ISM330DHCX_0, MOTION_ACCELERO);
      (void)BSP_MOTION_SENSOR_DeInit(MOTION_SENSOR_ISM330DHCX_0);
      return;
    }
  }
}

/**
  * @brief  Test of ISM330DHCX gyroscope sensor.
  */
void Gyro_Test(void)
{
  char gyr_x[64];
  char gyr_y[64];
  char gyr_z[64];
  MOTION_SENSOR_Axes_t angular_velocity;
  
  (void)BSP_MOTION_SENSOR_Init(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO);
  (void)BSP_MOTION_SENSOR_Enable(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO);
  
  while (1)
  {
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Gyroscope Measure ", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 0, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    memset(&angular_velocity, 0, sizeof(MOTION_SENSOR_Axes_t));
    (void)BSP_MOTION_SENSOR_GetAxes(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO, &angular_velocity);
    sprintf(gyr_x," GYRO_X = %ld", angular_velocity.x);
    sprintf(gyr_y," GYRO_Y = %ld", angular_velocity.y);
    sprintf(gyr_z," GYRO_Z = %ld", angular_velocity.z);
    UTIL_LCD_DisplayStringAt(0, 25, (uint8_t *)gyr_x, LEFT_MODE);
    UTIL_LCD_DisplayStringAt(0, 37, (uint8_t *)gyr_y, LEFT_MODE);
    UTIL_LCD_DisplayStringAt(0, 49, (uint8_t *)gyr_z, LEFT_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(1000);
    
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      (void)BSP_MOTION_SENSOR_Disable(MOTION_SENSOR_ISM330DHCX_0, MOTION_GYRO);
      (void)BSP_MOTION_SENSOR_DeInit(MOTION_SENSOR_ISM330DHCX_0);
      return;
    }
  }
}

/**
  * @}
  */ 

/**
  * @}
  */
 
