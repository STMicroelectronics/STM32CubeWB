/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_vl53l0x.c
 * @author  MCD Application Team
 * @brief   Proximity Application
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
#include "app_vl53l0x.h"
#include "stm32_seq.h"
#include "stm32wb5mm_dk.h"
#include "stm32wb5mm_dk_lcd.h"
#include "stm32_lcd.h"
#include "stm32wb5mm_dk_bus.h"

/* Private defines -----------------------------------------------------------*/ 
#define PROXIMITY_UPDATE_PERIOD       (uint32_t)(0.5*1000*1000/CFG_TS_TICK_VAL) /*500ms*/
#define DISTANCE_MAX_PROXIMITY        2000  /* 2m */

#define PROXIMITY_I2C_ADDRESS            0x53U
#define VL53L0X_XSHUT_PIN                GPIO_PIN_6
#define VL53L0X_XSHUT_GPIO_PORT          GPIOB
#define VL53L0X_XSHUT_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define VL53L0X_XSHUT_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

/* Private variables ---------------------------------------------------------*/   

/* Proximity */ 
VL53L0X_Dev_t Dev =
{
  .I2cHandle = &hbus_i2c3,
  .I2cDevAddr = PROXIMITY_I2C_ADDRESS
};

uint8_t VL53L0X_PROXIMITY_Update_Timer_Id;

/* Private function prototypes -----------------------------------------------*/
static void VL53L0X_PROXIMITY_Update_Timer_Callback(void);
static void VL53L0X_PROXIMITY_MspInit(void);

/**
  * @brief  VL53L0X proximity sensor Initialization.
  */
void VL53L0X_PROXIMITY_Init(void)
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
          while(1){}  // VL53L0X Time of Flight Failed to send its ID!
        }
      }
    }
    else
    {
      while(1){} // VL53L0X Time of Flight Failed to Initialize!
    }
  }
  else
  {
    while(1){} // VL53L0X Time of Flight Failed to get infos!
  } 
  UTIL_SEQ_RegTask( 1<<CFG_TASK_GET_MEASURE_TOF_ID, UTIL_SEQ_RFU, VL53L0X_PROXIMITY_PrintValue);
  /* Create timer to get the measure of TOF */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR,
        &VL53L0X_PROXIMITY_Update_Timer_Id,
        hw_ts_Repeated,
        VL53L0X_PROXIMITY_Update_Timer_Callback);
}

               
void VL53L0X_Start_Measure(void)
{
  /* Start the timer used to update the proximity value */
  HW_TS_Start(VL53L0X_PROXIMITY_Update_Timer_Id, PROXIMITY_UPDATE_PERIOD);
}
  
void VL53L0X_Stop_Measure(void)
{
  /* Stop the timer used to update the proximity value */
  HW_TS_Stop(VL53L0X_PROXIMITY_Update_Timer_Id);
}  
        
/**
 * @brief  On timeout, trigger the task
 *         to update the proximity value
 * @param  None
 * @retval None
 */
static void VL53L0X_PROXIMITY_Update_Timer_Callback(void)
{
  UTIL_SEQ_SetTask(1<<CFG_TASK_GET_MEASURE_TOF_ID, CFG_SCH_PRIO_0);
}

/**
  * @brief  Get distance from VL53L0X proximity sensor.
  * @param  None
  * @retval Distance in mm
  */
uint16_t VL53L0X_PROXIMITY_GetDistance(void)
{
  VL53L0X_RangingMeasurementData_t RangingMeasurementData;
  
  VL53L0X_PerformSingleRangingMeasurement(&Dev, &RangingMeasurementData);
  
  return RangingMeasurementData.RangeMilliMeter;  
}

/**
  * @brief  Print distance measure from VL53L0X proximity sensor on the OLED screen.
  * @param  None
  * @retval None
  */
void VL53L0X_PROXIMITY_PrintValue(void){
      UTIL_LCD_ClearStringLine(2);
      char distanceText[18];
      uint16_t prox_value = 0;
      uint16_t distance = 0;
      prox_value = VL53L0X_PROXIMITY_GetDistance();
      if(prox_value < DISTANCE_MAX_PROXIMITY){
        distance = prox_value / 10;
        sprintf(distanceText,"Distance : %3d cm",distance);
        UTIL_LCD_DisplayStringAtLine(2,(uint8_t*)distanceText);
      }else{
        UTIL_LCD_DisplayStringAtLine(2,(uint8_t*)"Distance > 200 cm");
      }
      BSP_LCD_Refresh(0);
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

