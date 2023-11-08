/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_hts221.c
 * @author  MCD Application Team
 * @brief   Temperature and Humidity Application
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "app_hts221.h"
 
/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c3;
static HTS221_Object_t hts221_obj_0;

/* Private functions ---------------------------------------------------------*/
static int32_t HTS221_write(void *handle, uint8_t reg, uint8_t *bufp,uint16_t len)
{
  BSP_I2C3_WriteReg(HTS221_I2C_ADDRESS, (uint16_t) reg, bufp, len);
  return 0;
}  
static int32_t HTS221_read(void *handle, uint8_t reg, uint8_t *bufp,uint16_t len)
{
  BSP_I2C3_ReadReg(HTS221_I2C_ADDRESS, (uint16_t) reg, bufp, len);
  return 0;
}

/**
 * @brief  HTS221 sensor Initialization.
 * @param  None
 * @retval BSP status
 */
int32_t HTS221_Init_Sensor(void){
  HTS221_IO_t            io_ctx;
  uint8_t              id;
  int32_t              ret = BSP_ERROR_NONE;
  
  /* Configure the environmental sensor driver */
  io_ctx.BusType     = HTS221_I2C_BUS; /* I2C */
  io_ctx.Address     = HTS221_I2C_ADDRESS;
  io_ctx.Init        = BSP_I2C3_Init;
  io_ctx.DeInit      = BSP_I2C3_DeInit;
  io_ctx.ReadReg     = BSP_I2C3_ReadReg;
  io_ctx.WriteReg    = BSP_I2C3_WriteReg;
  io_ctx.GetTick     = BSP_GetTick;
 
  hts221_obj_0.Ctx.write_reg = HTS221_write;
  hts221_obj_0.Ctx.read_reg = HTS221_read;
  hts221_obj_0.Ctx.handle = &hi2c3;
    
  if (HTS221_RegisterBusIO(&hts221_obj_0, &io_ctx) != HTS221_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (HTS221_ReadID(&hts221_obj_0, &id) != HTS221_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != HTS221_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (HTS221_Init(&hts221_obj_0) != HTS221_OK)
  {
	ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (HTS221_HUM_Enable(&hts221_obj_0) != HTS221_OK)
  {
	ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (HTS221_TEMP_Enable(&hts221_obj_0) != HTS221_OK)
  {
	ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
	ret = BSP_ERROR_NONE;
  }
  return ret;
}

/**
 * @brief  Get the HTS221 humidity value
 * @param  Value pointer where the humidity value is written
 * @retval None
 */
void HTS221_getHumidityValue(float *value){
        float val;
	HTS221_HUM_GetHumidity(&hts221_obj_0, &val);
        *value = val;
}

/**
 * @brief  Get the HTS221 temperature value
 * @param  Value pointer where the temperature value is written
 * @retval None
 */
void HTS221_getTemperatureValue(float *value){
        float val;
	HTS221_TEMP_GetTemperature(&hts221_obj_0,&val);
        *value = val;
}
	

