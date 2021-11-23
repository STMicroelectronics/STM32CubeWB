/**
  ******************************************************************************
* @file    proximity.c 
* @author  MCD Application Team
* @brief   This example code shows how to use the TOF sensor supported by the 
*          STM32WB5MM-DK board
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
#include "stm32wb5mm_dk_bus.h"

#include "vl53l0x_def.h"
#include "vl53l0x_api.h"
#include "vl53l0x_tof.h"

/** @addtogroup STM32WBxx_HAL_Examples
* @{
*/

/** @addtogroup BSP
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PROXIMITY_I2C_ADDRESS            0x53U
#define VL53L0X_ID                       0xEEAAU
#define VL53L0X_XSHUT_PIN                GPIO_PIN_6
#define VL53L0X_XSHUT_GPIO_PORT          GPIOB
#define VL53L0X_XSHUT_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define VL53L0X_XSHUT_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static VL53L0X_Dev_t Dev =
{
  .I2cHandle = &hbus_i2c3,
  .I2cDevAddr = PROXIMITY_I2C_ADDRESS
};

/* Private function prototypes -----------------------------------------------*/
static void VL53L0X_PROXIMITY_Init(void);
static uint16_t VL53L0X_PROXIMITY_GetDistance(void);
static void VL53L0X_PROXIMITY_MspInit(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Test of VL53L0X sensor.
  */
void Proximity_Test(void)
{
  char prox[64];
  uint16_t proximity;
  proximity = 0;
  
  VL53L0X_PROXIMITY_Init();
  while (1)
  {
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);
    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Proximity Measure ", CENTER_MODE);
    UTIL_LCD_DrawRect(0, 0, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    proximity = VL53L0X_PROXIMITY_GetDistance();
    sprintf(prox,"proximity=%d mm", proximity);
    UTIL_LCD_DisplayStringAt(0, 37, (uint8_t *)prox, LEFT_MODE);
    BSP_LCD_Refresh(0);
    HAL_Delay(500);
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      return;
    }
  }      
}

/**
  * @brief  VL53L0X proximity sensor Initialization.
  */
static void VL53L0X_PROXIMITY_Init(void)
{
  uint16_t vl53l0x_id = 0; 
  VL53L0X_DeviceInfo_t VL53L0X_DeviceInfo;
  
  /* Initialize IO interface */
  STM32WB5MM_DK_I2C_Init();
  
  VL53L0X_PROXIMITY_MspInit();
  
  memset(&VL53L0X_DeviceInfo, 0, sizeof(VL53L0X_DeviceInfo_t));
  
  if (VL53L0X_ERROR_NONE == VL53L0X_GetDeviceInfo(&Dev, &VL53L0X_DeviceInfo))
  {  
    if (VL53L0X_ERROR_NONE == VL53L0X_RdWord(&Dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, (uint16_t *) &vl53l0x_id))
    {
      if (vl53l0x_id == VL53L0X_ID)
      {
        if (VL53L0X_ERROR_NONE == VL53L0X_DataInit(&Dev))
        {
          Dev.Present = 1;
          SetupSingleShot(Dev);
        }
        else
        { 
          printf("VL53L0X Time of Flight Failed to send its ID!\n");
        }
      }
    }
    else
    {
      printf("VL53L0X Time of Flight Failed to Initialize!\n");
    }
  }
  else
  {
    printf("VL53L0X Time of Flight Failed to get infos!\n");
  }  
}

/**
  * @brief  Get distance from VL53L0X proximity sensor.
  * @retval Distance in mm
  */
static uint16_t VL53L0X_PROXIMITY_GetDistance(void)
{
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  
  VL53L0X_PerformSingleRangingMeasurement(&Dev, &RangingMeasurementData);
  
  return RangingMeasurementData.RangeMilliMeter;  
}

/**
  * @brief  VL53L0X proximity sensor Msp Initialization.
  */
static void VL53L0X_PROXIMITY_MspInit(void)
{
  GPIO_InitTypeDef gpio_config = {0};
  
  VL53L0X_XSHUT_GPIO_CLK_ENABLE();
  
  /* Configure GPIO pin : VL53L0X_XSHUT_PIN */
  gpio_config.Pin   = VL53L0X_XSHUT_PIN;
  gpio_config.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_config.Pull  = GPIO_PULLUP;
  gpio_config.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(VL53L0X_XSHUT_GPIO_PORT, &gpio_config);
  
  HAL_GPIO_WritePin(VL53L0X_XSHUT_GPIO_PORT, VL53L0X_XSHUT_PIN, GPIO_PIN_SET);
  
  HAL_Delay(1000);  
}
/**
  * @}
  */ 

/**
  * @}
  */
 
