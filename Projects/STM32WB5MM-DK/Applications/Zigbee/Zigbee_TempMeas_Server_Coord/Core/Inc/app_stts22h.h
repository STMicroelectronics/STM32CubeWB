/**
  ******************************************************************************
  * @file    app_stts221.h 
  * @author  MCD Application Team
  * @brief   Header for app_stts221.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017(-2021) STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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



/* Defines -------------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
int32_t STTS22H_Init_Sensor(void);
void STTS22H_getTemperatureValue(float *value);
void STTS22H_Start_Measure(void);
void STTS22H_Stop_Measure(void);
void STTS22H_PrintValues(void);

#endif /* __APP_STTS22H_H */
