/* USER CODE BEGIN Header */
/**
  ******************************************************************************
 * @file    app_stts22h.c
 * @author  MCD Application Team
 * @brief   Temperature Application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "app_stts22h.h"
#include "stm32_lcd.h"
#include "stm32wb5mm_dk_lcd.h"

/* Private defines -----------------------------------------------------------*/
#define STTS22H_UPDATE_PERIOD       (uint32_t)(0.5*1000*1000/CFG_TS_TICK_VAL) /*500ms*/

/* Private variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hbus_i2c3;
static STTS22H_Object_t stts22h_obj_0;
uint8_t STTS22H_Update_Timer_Id;

/* Private function prototypes -----------------------------------------------*/
#ifdef THERMOMETER_NODE  
static void STTS22H_Update_Timer_Callback(void);
#endif

/* Private functions ---------------------------------------------------------*/
static int32_t STTS22H_write(void *handle, uint8_t reg, uint8_t *bufp,uint16_t len)
{
  BSP_I2C3_WriteReg(STTS22H_I2C_ADD_H, (uint16_t) reg, bufp, len);
  return 0;
}
static int32_t STTS22H_read(void *handle, uint8_t reg, uint8_t *bufp,uint16_t len)
{
  BSP_I2C3_ReadReg(STTS22H_I2C_ADD_H, (uint16_t) reg, bufp, len);
  return 0;
}

/**
 * @brief  STTS22H sensor Initialization.
 * @param  None
 * @retval BSP status
 */
int32_t STTS22H_Init_Sensor(void){
  STTS22H_IO_t            io_ctx;
  uint8_t              id;
  int32_t              ret = BSP_ERROR_NONE;

  /* Configure the environmental sensor driver */
  io_ctx.BusType     = STTS22H_I2C_BUS; /* I2C */
  io_ctx.Address     = STTS22H_I2C_ADD_H;
  io_ctx.Init        = BSP_I2C3_Init;
  io_ctx.DeInit      = BSP_I2C3_DeInit;
  io_ctx.ReadReg     = BSP_I2C3_ReadReg;
  io_ctx.WriteReg    = BSP_I2C3_WriteReg;
  io_ctx.GetTick     = BSP_GetTick;

  stts22h_obj_0.Ctx.write_reg = STTS22H_write;
  stts22h_obj_0.Ctx.read_reg = STTS22H_read;
  stts22h_obj_0.Ctx.handle = &hbus_i2c3;
  
  if (STTS22H_RegisterBusIO(&stts22h_obj_0, &io_ctx) != STTS22H_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (STTS22H_ReadID(&stts22h_obj_0, &id) != STTS22H_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != STTS22H_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (STTS22H_Init(&stts22h_obj_0) != STTS22H_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (STTS22H_TEMP_Enable(&stts22h_obj_0) != STTS22H_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
 #ifdef THERMOMETER_NODE   
    UTIL_SEQ_RegTask( 1 << CFG_TASK_GET_MEASURE_ENV_ID, UTIL_SEQ_RFU, STTS22H_PrintValues);
    
    /* Create timer to get the measure of environment data */
    HW_TS_Create(CFG_TIM_PROC_ID_ISR,
        &STTS22H_Update_Timer_Id,
        hw_ts_Repeated,
        STTS22H_Update_Timer_Callback);
#endif
  }
  return ret;
}

void STTS22H_Start_Measure(void)
{
  HW_TS_Start(STTS22H_Update_Timer_Id, STTS22H_UPDATE_PERIOD);
}

void STTS22H_Stop_Measure(void)
{
  HW_TS_Stop(STTS22H_Update_Timer_Id);
}

#ifdef THERMOMETER_NODE  
static void STTS22H_Update_Timer_Callback(void)
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_GET_MEASURE_ENV_ID, CFG_SCH_PRIO_0);
}
#endif

/**
 * @brief  Get the STTS22H temperature value
 * @param  Value pointer where the temperature value is written
 * @retval None
 */
void STTS22H_getTemperatureValue(float *value){
  float val;
  STTS22H_TEMP_GetTemperature(&stts22h_obj_0,&val);
  *value = val;
}

void STTS22H_PrintValues(void){
  float temp_val;
  char text[32];

  STTS22H_TEMP_GetTemperature(&stts22h_obj_0,&temp_val);

  sprintf(text,"T:%2.1f C",(double)temp_val);

  UTIL_LCD_DisplayStringAtLine(3,(uint8_t*)text);
#ifdef ENABLE_SENSOR_MODEL_SERVER
  Sensor_Send(0, 0, PRESENT_AMBIENT_TEMPERATURE_PID);  
#endif
  return;
}
