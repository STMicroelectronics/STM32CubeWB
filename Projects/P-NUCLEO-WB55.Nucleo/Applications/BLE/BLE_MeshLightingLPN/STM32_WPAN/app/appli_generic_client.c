/**
******************************************************************************
* @file    appli_generic_client.c
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
#include "common.h"
#include "mesh_cfg_usr.h"
#include "appli_nvm.h"
#include "appli_mesh.h"
#include "generic_client.h"
#include "appli_generic_client.h"

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
* @brief  Appli_GenericClient_API: This function is a Wrapper to call Generic Client API depending on opcode received in input
* @param  elementIndex: Index of the element
* @param  msg_opcode: opcode of the desired API
* @param  msg_params: buffer containing the desired API parameters
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_GenericClient_API(MOBLEUINT8 elementIndex, MOBLEUINT16 msg_opcode, MOBLEUINT8 *msg_params) 
{ 
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  switch(msg_opcode){
/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF  
/******************************************************************************/
  case GENERIC_ON_OFF_GET:
    {
      result = GenericClient_OnOff_Get(elementIndex);
      break;
    }
  case GENERIC_ON_OFF_SET_ACK:
    {
      result = GenericClient_OnOff_Set_Ack(elementIndex,
                              msg_params, 
                              sizeof(Generic_OnOffParam_t) ); 
      break;
    }
  case GENERIC_ON_OFF_SET_UNACK:
    {  
      result = GenericClient_OnOff_Set_Unack(elementIndex,
                              msg_params, 
                              sizeof(Generic_OnOffParam_t) );
      break;
    }
/******************************************************************************/    
#endif /* #ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF */
/******************************************************************************/   
    
/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL 
/******************************************************************************/
  case GENERIC_LEVEL_GET:
    {
      result = GenericClient_Level_Get(elementIndex);
      break;
    }
  case GENERIC_LEVEL_SET_ACK:
    {
      result = GenericClient_Level_Set_Ack(elementIndex,
                                    msg_params, 
                                    sizeof(Generic_LevelParam_t));
      break;
    }
  case GENERIC_LEVEL_SET_UNACK:
    {
      result = GenericClient_Level_Set_Unack(elementIndex,
                                    msg_params, 
                                    sizeof(Generic_LevelParam_t));
      break;
    }
  case GENERIC_DELTA_SET:
    {

      result = GenericClient_Delta_Set_Ack(elementIndex,
                                    msg_params, 
                                    sizeof(Generic_DeltaLevelParam_t));
        break;
    }
  case GENERIC_DELTA_SET_UNACK:
    {
      result = GenericClient_Delta_Set_Unack(elementIndex,
                                    msg_params, 
                                    sizeof(Generic_DeltaLevelParam_t));
      break;
    }  
  case GENERIC_MOVE_SET:
    {
      result = GenericClient_Move_Set_Ack(elementIndex,
                                    msg_params, 
                                    sizeof(Generic_LevelMoveParam_t));
        break;
    }
  case GENERIC_MOVE_SET_UNACK:
    {
      result = GenericClient_Move_Set_Unack(elementIndex,
                                    msg_params, 
                                    sizeof(Generic_LevelMoveParam_t));
      break;
    }   
/******************************************************************************/    
#endif /* #ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL */
/******************************************************************************/   

/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_CLIENT_POWER_ONOFF 
/******************************************************************************/
  case GENERIC_POWER_ON_OFF_GET:
    {
      GenericClient_PowerOnOff_Get(elementIndex);
      break;
    }
  case GENERIC_POWER_ON_OFF_SET:
    {
      result = GenericClient_PowerOnOff_Set_Ack(elementIndex,
                                  msg_params, 
                                  sizeof(Generic_PowerOnOffParam_t));
      break;
    }
      case GENERIC_POWER_ON_OFF_SET_UNACK:
    {
      result = GenericClient_PowerOnOff_Set_Unack(elementIndex,
                                  msg_params, 
                                  sizeof(Generic_PowerOnOffParam_t));
      break;
    }
    
/******************************************************************************/    
#endif /* #ifdef ENABLE_GENERIC_MODEL_CLIENT_POWER_ONOFF */
/******************************************************************************/   

/******************************************************************************/    
#ifdef ENABLE_GENERIC_MODEL_CLIENT_DEFAULT_TRANSITION_TIME 
/******************************************************************************/
  case GENERIC_DEFAULT_TRANSITION_TIME_GET:
    {
      result = GenericClient_DefaultTransitionTime_Get(elementIndex);
      break;
    }
    
  case GENERIC_DEFAULT_TRANSITION_TIME_SET:
    {
      result = GenericClient_DefaultTransitionTime_Set_Ack(elementIndex,
                                  msg_params, 
                                  sizeof(Generic_DefaultTransitionParam_t));
      break;
    }
    
      case GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK:
    { 
      result = GenericClient_DefaultTransitionTime_Set_Unack(elementIndex,
                                  msg_params, 
                                  sizeof(Generic_DefaultTransitionParam_t));
      break;
    }
    
/******************************************************************************/    
#endif /* #ifdef ENABLE_GENERIC_MODEL_CLIENT_DEFAULT_TRANSITION_TIME */
/******************************************************************************/ 
 
  default:
    {
      TRACE_M(TF_GENERIC, "OpCode value invalid %d \r\n", msg_opcode);
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

