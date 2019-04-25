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

/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Appli_Sensor_Data_Status(MOBLEUINT8* data , MOBLEUINT32* pLength ,MOBLEUINT16 prop_ID);
MOBLE_RESULT Appli_Sensor_Descriptor_Status(MOBLEUINT8* sensor_Discriptor , MOBLEUINT32* pLength);




#endif /* __APPLI_SENSOR_MODEL_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

