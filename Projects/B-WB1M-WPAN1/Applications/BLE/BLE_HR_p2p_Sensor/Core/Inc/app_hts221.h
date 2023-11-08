/**
  ******************************************************************************
  * @file    app_hts221.h 
  * @author  MCD Application Team
  * @brief   Header for app_hts221.c module
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
#ifndef __APP_HTS221_H
#define __APP_HTS221_H

/* Includes ------------------------------------------------------------------*/
#include "hts221_reg.h" 
#include "hts221.h" 
#include "stm32wbxx_nucleo_bus.h"    
#include "stm32_seq.h"
#include "app_common.h"    
/* Defines -------------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
int32_t HTS221_Init_Sensor(void);
void HTS221_getHumidityValue(float *value);
void HTS221_getTemperatureValue(float *value);

#endif /* __APP_HTS221_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/