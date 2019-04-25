/**
  ******************************************************************************
  * @file    appli_sensor.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Lighting Mesh Models 
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

/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "types.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "sensors.h"
#include "appli_light.h"
#include "appli_generic.h"
#include "appli_sensor.h"
#include "common.h"
#include "mesh_cfg.h"

/** @addtogroup BLE_Mesh
 *  @{
 */

/** @addtogroup models_BLE
 *  @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
   
/* Sensor Cadence set */

typedef struct
{
  MOBLEUINT16 Prop_ID;
  MOBLEUINT16 PositiveTolenrance;
  MOBLEUINT16 NegativeTolenrance;
  MOBLEUINT8 SamplingFunction;
  MOBLEUINT8 MeasurementPeriod;
  MOBLEUINT8 UpdateInterval; 
}Appli_Sensor_DescriptorStatus_t; 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef ENABLE_SENSOR_MODEL_SERVER
/**
* @brief  Appli_Sensor_Data_Status: This function is callback for Application
           when sensor get message is received
* @param  sensor_Data: Pointer to the parameters to be send in message
* @param  pLength: Length of the parameters to be sent in response
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Sensor_Data_Status(MOBLEUINT8* sensor_Data , MOBLEUINT32* pLength, MOBLEUINT16 prop_ID)
{
                                     
      
  return MOBLE_RESULT_SUCCESS;
}

                                    
/**
* @brief  Appli_Sensor_Descriptor_Status: This function is callback for Application
           when sensor get message is received
* @param  sensor_Descriptor: Pointer to the parameters to be send in message
* @param  pLength: Length of the parameters to be sent in response
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Sensor_Descriptor_Status(MOBLEUINT8* sensor_Descriptor , MOBLEUINT32* pLength)
{
   Appli_Sensor_DescriptorStatus_t Appli_Sensor_DescriptorStatus1 = {TEMPERATURE_PID,0x00,0x00,0x00,0x00,0x00};
  
    *(sensor_Descriptor) = Appli_Sensor_DescriptorStatus1.Prop_ID;
    *(sensor_Descriptor+1) = Appli_Sensor_DescriptorStatus1.Prop_ID >> 8;
    *(sensor_Descriptor+2) = Appli_Sensor_DescriptorStatus1.PositiveTolenrance;
    *(sensor_Descriptor+3) = Appli_Sensor_DescriptorStatus1.PositiveTolenrance >> 8;
    *(sensor_Descriptor+4) = Appli_Sensor_DescriptorStatus1.NegativeTolenrance;
    *(sensor_Descriptor+5) = Appli_Sensor_DescriptorStatus1.NegativeTolenrance >> 8;
    *(sensor_Descriptor+7) = Appli_Sensor_DescriptorStatus1.SamplingFunction;
    *(sensor_Descriptor+8) = Appli_Sensor_DescriptorStatus1.MeasurementPeriod;
    *(sensor_Descriptor+9) = Appli_Sensor_DescriptorStatus1.UpdateInterval;
   
    *pLength = 8;
   
  return MOBLE_RESULT_SUCCESS;
}

#endif

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

