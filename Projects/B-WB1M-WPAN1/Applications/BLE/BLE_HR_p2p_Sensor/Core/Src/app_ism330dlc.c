/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_ism330dlc.c
 * @author  MCD Application Team
 * @brief   3D Accelerometer and 3D Gyroscope Application
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
#include "app_ism330dlc.h"

/* Private defines -----------------------------------------------------------*/ 
#define ISM330DLC_UPDATE_PERIOD       (uint32_t)(0.5*1000*1000/CFG_TS_TICK_VAL) /*500ms*/

/* Private variables ---------------------------------------------------------*/   
extern I2C_HandleTypeDef hi2c3;

uint8_t ISM330DLC_Update_Timer_Id;

/* Acc & Gyro */ 
typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;

typedef union{
  int16_t i16bit;
  uint8_t u8bit[2];
} axis1bit16_t;

static axis3bit16_t data_raw_acceleration;
static axis3bit16_t data_raw_angular_rate;
static axis1bit16_t data_raw_temperature;
static float acceleration_mg[3];
static float angular_rate_mdps[3];
static float temperature_degC;
static uint8_t whoamI, rst;
stmdev_ctx_t dev_ctx;

char accXText[7];
char accYText[7];
char accZText[7];
char tempText[7];
/* Private function prototypes -----------------------------------------------*/
//static void ISM330DLC_Update_Timer_Callback(void);

static int32_t ISM330DLC_write(void *handle, uint8_t Reg, uint8_t *Bufp,uint16_t len)
{
  BSP_I2C3_WriteReg(ISM330DLC_I2C_ADD_H, (uint16_t) Reg, Bufp, len);
  return 0;
}

static int32_t ISM330DLC_read(void *handle, uint8_t Reg, uint8_t *Bufp,uint16_t len)
{
  BSP_I2C3_ReadReg(ISM330DLC_I2C_ADD_H, (uint16_t) Reg, Bufp, len);
  return 0;
}

/**
  * @brief  ISM330DLC sensor Initialization.
  */
void ISM330DLC_Init(void){
  //BSP_I2C3_Init();
  /*
   *  Initialize mems driver interface
   */
  dev_ctx.write_reg = ISM330DLC_write;
  dev_ctx.read_reg = ISM330DLC_read;
  dev_ctx.handle = &hi2c3; 
   /*
   *  Check device ID
   */
  //uint8_t testID = 0;
  whoamI = 0;
  ism330dlc_device_id_get(&dev_ctx, &whoamI);
  //testID = whoamI;
  if ( whoamI != ISM330DLC_ID )
    while(1); /*manage here device not found */
  /*
   *  Restore default configuration
   */
  ism330dlc_reset_set(&dev_ctx, PROPERTY_ENABLE);
  do {
    ism330dlc_reset_get(&dev_ctx, &rst);
  } while (rst);
  /*
   *  Enable Block Data Update
   */
  ism330dlc_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
  /*
   * Set Output Data Rate
   */
  ism330dlc_xl_data_rate_set(&dev_ctx, ISM330DLC_XL_ODR_12Hz5);
  ism330dlc_gy_data_rate_set(&dev_ctx, ISM330DLC_GY_ODR_12Hz5);
  /*
   * Set full scale
   */ 
  ism330dlc_xl_full_scale_set(&dev_ctx, ISM330DLC_2g);
  ism330dlc_gy_full_scale_set(&dev_ctx, ISM330DLC_2000dps);
 
  /*
   * Configure filtering chain(No aux interface)
   */ 
  /* Accelerometer - analog filter */
  ism330dlc_xl_filter_analog_set(&dev_ctx, ISM330DLC_XL_ANA_BW_400Hz);
 
  /* Accelerometer - LPF1 path ( LPF2 not used )*/
  //ism330dlc_xl_lp1_bandwidth_set(&dev_ctx, ISM330DLC_XL_LP1_ODR_DIV_4);
 
  /* Accelerometer - LPF1 + LPF2 path */  
  ism330dlc_xl_lp2_bandwidth_set(&dev_ctx, ISM330DLC_XL_LOW_NOISE_LP_ODR_DIV_100);
 
  /* Accelerometer - High Pass / Slope path */
  //ism330dlc_xl_reference_mode_set(&dev_ctx, PROPERTY_DISABLE);
  //ism330dlc_xl_hp_bandwidth_set(&dev_ctx, ISM330DLC_XL_HP_ODR_DIV_100);
 
  /* Gyroscope - filtering chain */
  ism330dlc_gy_band_pass_set(&dev_ctx, ISM330DLC_HP_260mHz_LP1_STRONG);
  
//  UTIL_SEQ_RegTask( 1<<CFG_TASK_GET_MEASURE_ACC_ID, UTIL_SEQ_RFU, ISM330DLC_PrintValue);
//  /* Create timer to get the measure of TOF */
//  HW_TS_Create(CFG_TIM_PROC_ID_ISR,
//        &ISM330DLC_Update_Timer_Id,
//        hw_ts_Repeated,
//        ISM330DLC_Update_Timer_Callback);
//  
//  /* Start the timer used to update the proximity value */
//  HW_TS_Start(ISM330DLC_Update_Timer_Id, ISM330DLC_UPDATE_PERIOD);
}  

void ISM330DLC_getValue2g(void){
  /*
  * Read output only if new value is available
  */
  ism330dlc_reg_t reg;
  ism330dlc_status_reg_get(&dev_ctx, &reg.status_reg);
  //float accx = 0,accy = 0,accz = 0,gyrox = 0,gyroy = 0,gyroz = 0;
  
  if (reg.status_reg.xlda)
  {
      
    memset(data_raw_acceleration.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_acceleration_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
    acceleration_mg[0] = ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[0]);
    acceleration_mg[1] = ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[1]);
    acceleration_mg[2] = ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[2]);
//    accx = acceleration_mg[0];
//    accy = acceleration_mg[1];
//    accz = acceleration_mg[2];
  }
  if (reg.status_reg.gda)
  {
     
    memset(data_raw_angular_rate.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
    angular_rate_mdps[0] = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[0]);
    angular_rate_mdps[1] = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[1]);
    angular_rate_mdps[2] = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[2]);
//    gyrox = angular_rate_mdps[0];
//    gyroy = angular_rate_mdps[1];
//    gyroz = angular_rate_mdps[2];
  }
  
  if (reg.status_reg.tda)
  {  
    /* Read temperature data */
    memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
    ism330dlc_temperature_raw_get(&dev_ctx, data_raw_temperature.u8bit);
    temperature_degC = ism330dlc_from_lsb_to_celsius( data_raw_temperature.i16bit );
  }
}

void ISM330DLC_getAccValue(ISM330DLC_Acc_Value *value,int nb_g){
  /*
  * Read output only if new value is available
  */
  ism330dlc_reg_t reg;
  ism330dlc_status_reg_get(&dev_ctx, &reg.status_reg);
  
  if (reg.status_reg.xlda)
  {
    memset(data_raw_acceleration.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_acceleration_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
    if(nb_g == 2){
      value->x = ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[0]);
      value->y = ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[1]);
      value->z = ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[2]);
    }else if(nb_g == 4){
      value->x = ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[0]);
      value->y = ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[1]);
      value->z = ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[2]);
    }
  }
}

void ISM330DLC_getAccValueTest(int32_t *accX,int32_t *accY,int32_t *accZ,int nb_g){
  /*
  * Read output only if new value is available
  */
  ism330dlc_reg_t reg;
  ism330dlc_status_reg_get(&dev_ctx, &reg.status_reg);
  
  if (reg.status_reg.xlda)
  {
    memset(data_raw_acceleration.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_acceleration_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
    if(nb_g == 2){
      *accX = (int32_t)ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[0]);
      *accY = (int32_t)ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[1]);
      *accZ = (int32_t)ism330dlc_from_fs2g_to_mg( data_raw_acceleration.i16bit[2]);
    }else if(nb_g == 4){
      *accX = (int32_t)ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[0]);
      *accY = (int32_t)ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[1]);
      *accZ = (int32_t)ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[2]);
    }
  }
}

void ISM330DLC_getGyroValueTest(int32_t *gyroX,int32_t *gyroY,int32_t *gyroZ){
  /*
  * Read output only if new value is available
  */
  ism330dlc_reg_t reg;
  ism330dlc_status_reg_get(&dev_ctx, &reg.status_reg);
  
  if (reg.status_reg.gda)
  {
    memset(data_raw_angular_rate.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
    *gyroX = (int32_t)ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[0]);
    *gyroY = (int32_t)ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[1]);
    *gyroZ = (int32_t)ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[2]);
  }
}  

void ISM330DLC_getGyroValue(ISM330DLC_Acc_Value *value){
  /*
  * Read output only if new value is available
  */
  ism330dlc_reg_t reg;
  ism330dlc_status_reg_get(&dev_ctx, &reg.status_reg);
  
  if (reg.status_reg.gda)
  {
    memset(data_raw_angular_rate.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
    value->x = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[0]);
    value->y = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[1]);
    value->z = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[2]);
  }
}               

void ISM330DLC_getTempValue(float *value){
  ism330dlc_reg_t reg;
  ism330dlc_status_reg_get(&dev_ctx, &reg.status_reg);
  if (reg.status_reg.tda)
  {  
    /* Read temperature data */
    memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
    ism330dlc_temperature_raw_get(&dev_ctx, data_raw_temperature.u8bit);
    temperature_degC = ism330dlc_from_lsb_to_celsius( data_raw_temperature.i16bit );
    *value = temperature_degC;
  }
}

void ISM330DLC_getValue4g(void){
  /*
  * Read output only if new value is available
  */
  ism330dlc_reg_t reg;
  ism330dlc_status_reg_get(&dev_ctx, &reg.status_reg);
  //float accx = 0,accy = 0,accz = 0,gyrox = 0,gyroy = 0,gyroz = 0;
  
  if (reg.status_reg.xlda)
  {
      
    memset(data_raw_acceleration.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_acceleration_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
    acceleration_mg[0] = ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[0]);
    acceleration_mg[1] = ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[1]);
    acceleration_mg[2] = ism330dlc_from_fs4g_to_mg( data_raw_acceleration.i16bit[2]);
//    accx = acceleration_mg[0];
//    accy = acceleration_mg[1];
//    accz = acceleration_mg[2];
  }
  if (reg.status_reg.gda)
  {
     
    memset(data_raw_angular_rate.u8bit, 0x00, 3*sizeof(int16_t));
    ism330dlc_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
    angular_rate_mdps[0] = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[0]);
    angular_rate_mdps[1] = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[1]);
    angular_rate_mdps[2] = ism330dlc_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[2]);
//    gyrox = angular_rate_mdps[0];
//    gyroy = angular_rate_mdps[1];
//    gyroz = angular_rate_mdps[2];
  }
}

int32_t getAccX(void){
    return (int32_t) acceleration_mg[0];
}  

int32_t getAccY(void){
    return (int32_t) acceleration_mg[1];
}  

int32_t getAccZ(void){
    return (int32_t) acceleration_mg[2];
}  

int32_t getGyroX(void){
    return (int32_t) angular_rate_mdps[0];
}  

int32_t getGyroY(void){
    return (int32_t) angular_rate_mdps[1];
}  

int32_t getGyroZ(void){
    return (int32_t) angular_rate_mdps[2];
}  

float getAccX_float(void){
    return (-1.0)*acceleration_mg[0];
}  

float getAccY_float(void){
    return (-1.0)*acceleration_mg[1];
}  

float getAccZ_float(void){
    return acceleration_mg[2];
}  

float getGyroX_float(void){
    return angular_rate_mdps[0];
}  

float getGyroY_float(void){
    return angular_rate_mdps[1];
}  

float getGyroZ_float(void){
    return angular_rate_mdps[2];
}  

void ISM330DLC_SetFullScale_4g(void){
    ism330dlc_xl_full_scale_set(&dev_ctx, ISM330DLC_4g);
}

void ISM330DLC_SetFullScale_2g(void){
    ism330dlc_xl_full_scale_set(&dev_ctx, ISM330DLC_2g);
}               
                 
void ISM330DLC_PrintValue(void){
      BSP_LCD_ClearStringLine(2);
      BSP_LCD_ClearStringLine(3);
      BSP_LCD_ClearStringLine(4);
      ISM330DLC_getValue2g();
//      sprintf(accXText,"%.000f",acceleration_mg[0]);
//      sprintf(accYText,"%.000f",acceleration_mg[1]);
//      sprintf(accZText,"%.000f",acceleration_mg[2]);
      sprintf(accXText,"%4.2f",angular_rate_mdps[0]/1000);
      sprintf(accYText,"%4.2f",angular_rate_mdps[1]/1000);
      sprintf(accZText,"%4.2f",angular_rate_mdps[2]/1000);  
      sprintf(tempText,"%6.2f",temperature_degC);  
      //BSP_LCD_DisplayStringAtLine(2,(uint8_t*)accXText);
      //BSP_LCD_DisplayStringAtLine(3,(uint8_t*)accYText);
      BSP_LCD_DisplayStringAtLine(3,(uint8_t*)tempText);
      //BSP_LCD_DisplayStringAtLine(4,(uint8_t*)accZText);
      BSP_LCD_Refresh();
}

///**
// * @brief  On timeout, trigger the task
// *         to update the ISM330DLC value
// * @param  None
// * @retval None
// */
//static void ISM330DLC_Update_Timer_Callback(void)
//{
//  UTIL_SEQ_SetTask(1<<CFG_TASK_GET_MEASURE_ACC_ID, CFG_SCH_PRIO_0);
//}