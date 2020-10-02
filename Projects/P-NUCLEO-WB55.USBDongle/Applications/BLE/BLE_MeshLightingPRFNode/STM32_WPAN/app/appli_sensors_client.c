/**
******************************************************************************
* @file    appli_sensors_client.c
* @author  BLE Mesh Team
* @brief   Application interface for Generic Mesh Models 
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "appli_sensor.h"
#include "appli_light.h"
#include "common.h"
#include "mesh_cfg_usr.h"
#include "appli_nvm.h"
#include "appli_mesh.h"
#include "sensors_client.h"
#include "appli_sensors_client.h"

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/**
* @brief  Appli_SensorsClient_API: This function is a Wrapper to call Sensors Client API depending on opcode received in input
* @param  elementIndex: Index of the element
* @param  msg_opcode: opcode of the desired API
* @param  msg_params: buffer containing the desired API parameters
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_SensorsClient_API(MOBLEUINT8 elementIndex, MOBLEUINT16 msg_opcode, MOBLEUINT8 *msg_params) 
{ 
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  switch(msg_opcode){
/******************************************************************************/
#ifdef ENABLE_SENSOR_MODEL_CLIENT 
/******************************************************************************/
  case SENSOR_DESCRIPTOR_GET:
    {
      result = SensorsClient_Descriptor_Get(elementIndex, msg_params);
      break;
    }
  case SENSOR_CADENCE_GET:
    {
      result = SensorsClient_Cadence_Get(elementIndex, msg_params);
      break;
    }
  case SENSOR_CADENCE_SET:
    {
      //TODO: Set dynamic data size depending on Sensor DATA LENTGH:  length = 4 + 4 + 2* SensorDataLength
      result = SensorsClient_Cadence_Set(elementIndex,
                              msg_params, 
                              10); //(sizeof(sensor_CadenceCbParam_t)-3)); 
      break;
    }
  case SENSOR_CADENCE_SET_UNACK:
    {  
      //TODO: Set dynamic data size depending on Sensor DATA LENTGH:  length = 4 + 4 + 2 * SensorDataLength
      result = SensorsClient_Cadence_Set_Unack(elementIndex,
                              msg_params, 
                              10); //(sizeof(sensor_CadenceCbParam_t)-3));
      break;
    }
  case SENSOR_SETTINGS_GET:
    {
      result = SensorsClient_Settings_Get(elementIndex, msg_params);
      break;
    }
    
      case SENSOR_SETTING_GET:
    {
      result = SensorsClient_Setting_Get(elementIndex, msg_params, sizeof(sensor_SettingsCbParams_t));
      break;
    }
  case SENSOR_SETTING_SET:
    {
      //TODO: Set dynamic data size depending on Sensor DATA LENTGH:  length = 4 + SensorDataLength
      result = SensorsClient_Setting_Set(elementIndex,
                              msg_params, 
                              6);
      break;
    }
  case SENSOR_SETTING_SET_UNACK:
    {  
      //TODO: Set dynamic data size depending on Sensor DATA LENTGH:  length = 4 + SensorDataLength
      result = SensorsClient_Setting_Set_Unack(elementIndex,
                              msg_params, 
                              6);
      break;
    }  
  case SENSOR_GET:
    {  
      result = SensorsClient_Get(elementIndex, msg_params);
      break;
    }
  case SENSOR_COLUMN_GET:
    {  
      result = SensorsClient_Column_Get(elementIndex, msg_params, 4);
      break;
    }
  case SENSOR_SERIES_GET:
    {  
      result = SensorsClient_Series_Get(elementIndex, msg_params, 4);
      break;
    }
/******************************************************************************/    
#endif /* #ifdef ENABLE_SENSOR_MODEL_CLIENT */
/******************************************************************************/   
    

  default:
    {
      TRACE_M(TF_SENSOR, "OpCode value invalid %d \r\n", msg_opcode);
      result = MOBLE_RESULT_FAIL;
    }
  }
  return result;
}

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

