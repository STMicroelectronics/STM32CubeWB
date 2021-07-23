/**
  ******************************************************************************
  * @file    app_stts221.h 
  * @author  MCD Application Team
  * @brief   Header for app_stts221.c module
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
#ifndef __APP_STTS22H_H
#define __APP_STTS22H_H

/* Includes ------------------------------------------------------------------*/
#include "stts22h_reg.h" 
#include "stts22h.h" 
//#include "stm32wb5M_DK_bus.h"
#include "stm32wb5mm_dk_bus.h"
//#include "stm32wbxx_discovery_lcd.h"
#include "stm32_seq.h"
#include "app_common.h"
#include "common.h"
#include "sensors.h"

/* Defines -------------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
int32_t STTS22H_Init_Sensor(void);
void STTS22H_getTemperatureValue(float *value);
void STTS22H_Start_Measure(void);
void STTS22H_Stop_Measure(void);
void STTS22H_PrintValues(void);

#endif /* __APP_STTS22H_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
