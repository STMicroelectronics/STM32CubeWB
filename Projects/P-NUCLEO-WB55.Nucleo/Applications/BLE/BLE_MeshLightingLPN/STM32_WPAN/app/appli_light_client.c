/**
******************************************************************************
* @file    appli_light_client.c
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
#include "appli_generic.h"
#include "appli_light.h"
#include "appli_light_lc.h"
#include "common.h"
#include "mesh_cfg_usr.h"
#include "appli_nvm.h"
#include "appli_mesh.h"
#include "generic_client.h"
#include "appli_light_client.h"
#include "light_client.h"


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

extern MOBLEUINT8 Tid_Client;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
* @brief  Appli_LightClient_API: This function is a Wrapper to call Light Client API depending on opcode received in input
* @param  elementIndex: Index of the element
* @param  msg_opcode: opcode of the desired API
* @param  msg_params: buffer containing the desired API parameters
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightClient_API(MOBLEUINT8 elementIndex, MOBLEUINT16 msg_opcode, MOBLEUINT8 *msg_params) 
{ 
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
    
  switch(msg_opcode){
/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS  
/******************************************************************************/
  case LIGHT_LIGHTNESS_GET:
    {
      result = LightClient_Lightness_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_SET:
    {
      result = LightClient_Lightness_Set_Ack(elementIndex,
                                             msg_params, 
                                             (sizeof(Light_LightnessParam_t)-2));       //Remove the size of Linear Lightness params
      break;
    }
  case LIGHT_LIGHTNESS_SET_UNACK:
    {  
      result = LightClient_Lightness_Set_Unack(elementIndex,
                                               msg_params, 
                                               (sizeof(Light_LightnessParam_t)-2));     //Remove the size of Linear Lightness params
      break;
    }
    
  case LIGHT_LIGHTNESS_LINEAR_GET:
    {
      result = LightClient_Lightness_Linear_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_LINEAR_SET:
    {
      result = LightClient_Lightness_Linear_Set_Ack(elementIndex,
                                                    msg_params, 
                                                    (sizeof(Light_LightnessParam_t)-2));        //Remove the size of  Lightness params
      break;
    }
  case LIGHT_LIGHTNESS_LINEAR_SET_UNACK:
    {     
      result = LightClient_Lightness_Linear_Set_Unack(elementIndex,
                                                      msg_params, 
                                                      (sizeof(Light_LightnessParam_t)-2));      //Remove the size of  Lightness params
      break;
    }
    
  case LIGHT_LIGHTNESS_DEFAULT_GET:
    {
      result = LightClient_Lightness_Default_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_DEFAULT_SET:
    {
      result = LightClient_Lightness_Default_Set_Ack(elementIndex,
                                                     msg_params, 
                                                     sizeof(Light_LightnessDefaultParam_t));
      break;
    }
  case LIGHT_LIGHTNESS_DEFAULT_SET_UNACK:
    {     
      result = LightClient_Lightness_Default_Set_Unack(elementIndex,
                                                       msg_params, 
                                                       sizeof(Light_LightnessDefaultParam_t));
      break;
    }
    
    
  case LIGHT_LIGHTNESS_RANGE_GET:
    {
      result = LightClient_Lightness_Range_Get(elementIndex);
      break;
    }
  case LIGHT_LIGHTNESS_RANGE_SET:
    {
      result = LightClient_Lightness_Range_Set_Ack(elementIndex,
                                                     msg_params, 
                                                     sizeof(Light_LightnessRangeParam_t)-1);    //Remove the size of StatusCode
      break;
    }
  case LIGHT_LIGHTNESS_RANGE_SET_UNACK:
    {     
      result = LightClient_Lightness_Range_Set_Unack(elementIndex,
                                                       msg_params, 
                                                       sizeof(Light_LightnessRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }


/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS */
/******************************************************************************/
    
/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_CTL  
/******************************************************************************/
  case LIGHT_CTL_GET:
    {
      result = LightClient_Ctl_Get(elementIndex);
      break;
    }
  case LIGHT_CTL_SET:
    {
      result = LightClient_Ctl_Set_Ack(elementIndex,
                                       msg_params, 
                                       (sizeof(Light_CtlParam_t)-4) ); //Remove the size of Last_Lightness and Last_Temperature
      break;
    }
  case LIGHT_CTL_SET_UNACK:
    {  
      result = LightClient_Ctl_Set_Unack(elementIndex,
                                         msg_params, 
                                         (sizeof(Light_CtlParam_t)-4) ); //Remove the size of Last_Lightness and Last_Temperature
      break;
    }
    
  case LIGHT_CTL_TEMPERATURE_GET:
    {
      result = LightClient_Ctl_Temperature_Get(elementIndex);
      break;
    }
  case LIGHT_CTL_TEMPERATURE_SET:
    {
      result = LightClient_Ctl_Temperature_Set_Ack(elementIndex,
                                                   msg_params, 
                                                   sizeof(Light_CtlTemperatureParam_t));
      break;
    }
  case LIGHT_CTL_TEMPERATURE_SET_UNACK:
    {  
      result = LightClient_Ctl_Temperature_Set_Unack(elementIndex,
                                                     msg_params, 
                                                     sizeof(Light_CtlTemperatureParam_t));
      break;
    }
                                                     
    case LIGHT_CTL_TEMPERATURE_RANGE_GET:
      {
        result = LightClient_Ctl_Temperature_Range_Get(elementIndex);
        break;
      }
    case LIGHT_CTL_TEMPERATURE_RANGE_SET:
      {
        result = LightClient_Ctl_Temperature_Range_Set_Ack(elementIndex,
                                                           msg_params, 
                                                           sizeof(Light_CtlTemperatureRangeParam_t)-1);  //Remove the size of StatusCode
        break;
      }
  case LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK:
    {  
      result = LightClient_Ctl_Temperature_Range_Set_Unack(elementIndex,
                                                           msg_params, 
                                                           sizeof(Light_CtlTemperatureRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }
      
    case LIGHT_CTL_DEFAULT_GET:
      {
        result = LightClient_Ctl_Default_Get(elementIndex);
        break;
      }
    case LIGHT_CTL_DEFAULT_SET:
      {
        result = LightClient_Ctl_Default_Set_Ack(elementIndex,
                                                 msg_params, 
                                                 sizeof(Light_CtlDefaultParam_t));
        break;
      }
  case LIGHT_CTL_DEFAULT_SET_UNACK:
    {  
      result = LightClient_Ctl_Default_Set_Unack(elementIndex,
                                                 msg_params, 
                                                 sizeof(Light_CtlDefaultParam_t));
      break;
    }
/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_CTL */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_HSL  
/******************************************************************************/
  case LIGHT_HSL_GET:
    {
      result = LightClient_Hsl_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_SET:
    {
      result = LightClient_Hsl_Set_Ack(elementIndex,
                                       msg_params, 
                                       sizeof(Light_HslParam_t) ); 
      break;
    }
  case LIGHT_HSL_SET_UNACK:
    {  
      result = LightClient_Hsl_Set_Unack(elementIndex,
                                         msg_params, 
                                         sizeof(Light_HslParam_t) );
      break;
    }
    
  case LIGHT_HSL_DEFAULT_GET:
    {
      result = LightClient_Hsl_Default_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_DEFAULT_SET:
    {
      result = LightClient_Hsl_Default_Set_Ack(elementIndex,
                                       msg_params, 
                                       sizeof(Light_HslDefault_t) ); 
      break;
    }
  case LIGHT_HSL_DEFAULT_SET_UNACK:
    {  
      result = LightClient_Hsl_Default_Set_Unack(elementIndex,
                                         msg_params, 
                                         sizeof(Light_HslDefault_t) );
      break;
    }
    
  case LIGHT_HSL_RANGE_GET:
    {
      result = LightClient_Hsl_Range_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_RANGE_SET:
    {
      result = LightClient_Hsl_Range_Set_Ack(elementIndex,
                                               msg_params, 
                                               sizeof(Light_HslRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }
  case LIGHT_HSL_RANGE_SET_UNACK:
    {  
      result = LightClient_Hsl_Range_Set_Unack(elementIndex,
                                                 msg_params, 
                                                 sizeof(Light_HslRangeParam_t)-1);  //Remove the size of StatusCode
      break;
    }
    
  case LIGHT_HSL_HUE_GET:
    {
      result = LightClient_Hsl_Hue_Get(elementIndex);
      break;
    }
  case LIGHT_HSL_HUE_SET:
    {
      result = LightClient_Hsl_Hue_Set_Ack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_HslHueParam_t));
      break;
    }
  case LIGHT_HSL_HUE_SET_UNACK:
    {  
      result = LightClient_Hsl_Hue_Set_Unack(elementIndex,
                                               msg_params, 
                                               sizeof(Light_HslHueParam_t));
      break;
    }
    
    case LIGHT_HSL_SATURATION_GET:
      {
        result = LightClient_Hsl_Saturation_Get(elementIndex);
        break;
      }
    case LIGHT_HSL_SATURATION_SET:
      {
        result = LightClient_Hsl_Saturation_Set_Ack(elementIndex,
                                                    msg_params, 
                                                    sizeof(Light_HslSaturationParam_t));
        break;
      }
    case LIGHT_HSL_SATURATION_SET_UNACK:
      {  
        result = LightClient_Hsl_Saturation_Set_Unack(elementIndex,
                                                      msg_params, 
                                                      sizeof(Light_HslSaturationParam_t));
        break;
    }
/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_HSL */
/******************************************************************************/
    
/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LC  
/******************************************************************************/   
  case LIGHT_LC_MODE_GET:
    {
      result = LightClient_LC_Mode_Get(elementIndex);
      break;
    }
  case LIGHT_LC_MODE_SET:
    {
      result = LightClient_LC_Mode_Set_Ack(elementIndex,
                                           msg_params, 
                                           sizeof(Light_LC_ModeParam_t));
      break;
    }
  case LIGHT_LC_MODE_SET_UNACK:
    {  
      result = LightClient_LC_Mode_Set_Unack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_LC_ModeParam_t));
      break;
    }
  case LIGHT_LC_OM_GET:
    {
      result = LightClient_LC_OM_Get(elementIndex);
      break;
    }
  case LIGHT_LC_OM_SET:
    {
      result = LightClient_LC_OM_Set_Ack(elementIndex,
                                           msg_params, 
                                           sizeof(Light_LC_OccupancyModeParam_t));
      break;
    }
  case LIGHT_LC_OM_SET_UNACK:
    {  
      result = LightClient_LC_OM_Set_Unack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_LC_OccupancyModeParam_t));
      break;
    }
  case LIGHT_LC_ON_OFF_GET:
    {
      result = LightClient_LC_OnOff_Get(elementIndex);
      break;
    }
  case LIGHT_LC_ON_OFF_SET:
    {
      result = LightClient_LC_OnOff_Set_Ack(elementIndex,
                                           msg_params, 
                                           sizeof(Light_LC_OnOffParam_t));
      break;
    }
  case LIGHT_LC_ON_OFF_SET_UNACK:
    {  
      result = LightClient_LC_OnOff_Set_Unack(elementIndex,
                                             msg_params, 
                                             sizeof(Light_LC_OnOffParam_t));
      break;
    }
    
    case LIGHT_LC_PROPERTY_GET:
      {
        result = LightClient_LC_Property_Get(elementIndex, msg_params);
        break;
      }
    case LIGHT_LC_PROPERTY_SET:
      {
        //TODO: Set dynamic data size depending on Property DATA LENTGH:  length = 2 + PropertyDataLength
        result = LightClient_LC_Property_Set_Ack(elementIndex,
                                              msg_params, 
                                              5);
        break;
      }
  case LIGHT_LC_PROPERTY_SET_UNACK:
    {  
      //TODO: Set dynamic data size depending on Property DATA LENTGH:  length = 2 + PropertyDataLength
      result = LightClient_LC_Property_Set_Unack(elementIndex,
                                              msg_params, 
                                              5);
      break;
    }
/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_CLIENT_LC */
/******************************************************************************/
  default:
    {
      TRACE_M(TF_LIGHT, "OpCode value invalid %d \r\n", msg_opcode);
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

