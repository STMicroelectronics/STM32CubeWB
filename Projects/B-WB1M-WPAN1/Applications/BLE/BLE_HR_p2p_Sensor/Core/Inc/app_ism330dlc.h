/**
  ******************************************************************************
  * @file    app_ism330dlc.h 
  * @author  MCD Application Team
  * @brief   Header for app_ism330dlc.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ISM330DLC_H
#define __APP_ISM330DLC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_nucleo_bus.h"
#include "stm32wbxx_nucleo.h"
#include "stm32wbxx_discovery_lcd.h"
#include "stm32_seq.h"
#include "app_common.h"
#include "ism330dlc_reg.h"
/* Defines -------------------------------------------------------------------*/

struct ISM330DLC_Acc_Value
{
  float x;
  float y;
  float z;
};
typedef struct ISM330DLC_Acc_Value ISM330DLC_Acc_Value;


/* Exported functions ------------------------------------------------------- */
void ISM330DLC_Init(void);
void ISM330DLC_getValue2g(void);
void ISM330DLC_getValue4g(void);
void ISM330DLC_PrintValue(void);
int32_t getAccX(void);
int32_t getAccY(void);
int32_t getAccZ(void);
int32_t getGyroX(void);
int32_t getGyroY(void);
int32_t getGyroZ(void);
void ISM330DLC_SetFullScale_2g(void);
void ISM330DLC_SetFullScale_4g(void);
float getAccX_float(void);
float getAccY_float(void);
float getAccZ_float(void);
float getGyroX_float(void);
float getGyroY_float(void);
float getGyroZ_float(void);
void ISM330DLC_getTempValue(float *value);
void ISM330DLC_getAccValue(ISM330DLC_Acc_Value *value,int nb_g);
void ISM330DLC_getGyroValue(ISM330DLC_Acc_Value *value);
void ISM330DLC_getAccValueTest(int32_t *accX,int32_t *accY,int32_t *accZ,int nb_g);
void ISM330DLC_getGyroValueTest(int32_t *gyroX,int32_t *gyroY,int32_t *gyroZ);
#endif /* __APP_ISM330DLC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/