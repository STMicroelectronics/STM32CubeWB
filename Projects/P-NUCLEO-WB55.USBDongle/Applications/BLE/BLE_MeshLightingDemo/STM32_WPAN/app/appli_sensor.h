/**
  ******************************************************************************
  * @file    appli_sensor.h
  * @author  BLE Mesh Team
  * @brief   Application interface for Light Mesh Model
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLI_SENSOR_MODEL_H
#define __APPLI_SENSOR_MODEL_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported variables  -------------------------------------------------------*/
/* 
 structure for the Property id for the sensors Present inside the firmware.
*/


/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Appli_Sensor_Cadence_Set(Sensor_CadenceParam_t* pCadence_param, MOBLEUINT16 property_ID,
                                                                           MOBLEUINT32 length); 
MOBLE_RESULT Appli_Sensor_Data_Status(MOBLEUINT8* sensor_Data , MOBLEUINT32* pLength, 
                                              MOBLEUINT16 prop_ID , MOBLEUINT32 length);
MOBLE_RESULT Appli_Sensor_Descriptor_Status(MOBLEUINT8* sensor_Discriptor , 
                                                           MOBLEUINT32* pLength);
MOBLE_RESULT Appli_Sensor_Setting_Set(Sensor_SettingParam_t* pSensor_SettingParam,
                                                           MOBLEUINT8 OptionalValid);                                      

void Sensor_Publication_Process(float* , MODEL_Property_IDTableParam_t*);
void SensorDataPublish(MOBLEUINT32 * , MOBLEUINT16*);
void Read_Sensor_Data(float *);
MOBLE_RESULT Check_Property_ID(const MODEL_Property_IDTableParam_t prop_ID_Table[] 
                                                         , MOBLEUINT16 prop_ID);

MOBLE_RESULT Appli_Sensor_GetSettingStatus(MOBLEUINT8* pSetting_Status);
MOBLE_RESULT Appli_Sensor_GetSetting_IDStatus(MOBLEUINT8* pSetting_Status);
MOBLE_RESULT Appli_Sensor_Init(void); 
void Sensor_Process(void);

#endif /* __APPLI_SENSOR_MODEL_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

