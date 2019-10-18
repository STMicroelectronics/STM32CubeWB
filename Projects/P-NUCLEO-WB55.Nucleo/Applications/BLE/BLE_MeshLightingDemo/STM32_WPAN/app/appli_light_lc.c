/**
  ******************************************************************************
  * @file    appli_light_ctrl.c
  * @author  BLE Mesh Team
  * @brief   Application interface for light LC Mesh Models 
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
#include "appli_generic.h"
#include "appli_light.h"
#include "common.h"
#include "mesh_cfg_usr.h"
#include "appli_light_lc.h"

/** @addtogroup BlueNRG_Mesh
*  @{
*/

/** @addtogroup models_BlueNRG2
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/

Appli_LightLC_Set_t Appli_LightLC_set;
Appli_Light_LC_PropertySet_t Appli_LightLC_PropertySet; 

MOBLEUINT16 AmbientLuxLevel;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Appli_Light_LCMode_Set: This function is callback for Application
when Light LC mode Set message is received
* @param  pLight_LC_Param: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightLC_Mode_Set(Light_LC_Param_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid)
{
  Appli_LightLC_set.LC_mode = pLight_LC_Param->LC_mode;
  return MOBLE_RESULT_SUCCESS;
}
  
/**
* @brief  Appli_LightLC_OM_Set: This function is callback for Application
when Light LC mode Occupancy Model Set message is received
* @param  pLight_LC_Param: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightLC_OM_Set(Light_LC_Param_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid)
{
  Appli_LightLC_set.LC_OM = pLight_LC_Param->LC_OM;
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Appli_LightLC_OnOff_Set: This function is callback for Application
when Light LC On Off Set message is received
* @param  pLight_LC_Param: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightLC_OnOff_Set(Light_LC_OnOffState_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid)
{
  Appli_LightLC_set.Light_OnOffState = pLight_LC_Param->Present_OnOff_State;
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief Get_AmbientLuxLevelOutput: This function is given to calculate the Ambient 
         Lux level output from the ambient sensor.
* @param void:
* @retval MOBLEUINT16:    
**/
MOBLEUINT16 Appli_LightLC_Get_AmbientLuxLevelOutput(void)
{
  /* Use AmbientLuxLevel global variable to store the value from ambient lux sensor
     and then use this value to copmpare the lux level output from state machine.
   */
  
  return AmbientLuxLevel;
}

/**
* @brief Light_LC_LuxLevelPIRegulator: This function will calculate all the parameter
         Kid,kpu,kiu,kpd and return the value Light Lightness Linear.           
* @param void:
* @retval MOBLEUINT16:    
**/
MOBLEUINT16 Appli_Light_LC_PIRegulatorOutput(MOBLEUINT16 tableLuxLevel,MOBLEUINT16 ambientLuxLevel)
{
  MOBLEUINT16 luxLevel = 0;
  /* User can write their code for the calculation */
  
  
  return luxLevel;
}

/**
* @brief  Appli_LightLC_Get_ModeStatus: This function is callback for Application
to get the application values in middleware used for transition change.
* @param  lcModeState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightLC_Get_ModeStatus(MOBLEUINT8* plcModeState)
{  
  *(plcModeState) = Appli_LightLC_set.LC_mode;
  TRACE_M(TF_SERIAL_CTRL,"Get Mode Status: %d\n\r",
          Appli_LightLC_set.LC_mode);
  
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Appli_LightLC_Get_OMModeStatus: This function is callback for Application
to get the application values in middleware used for transition change.
* @param  lcOM_ModeState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightLC_Get_OMModeStatus(MOBLEUINT8* plcOM_ModeState)
{  
  *(plcOM_ModeState) = Appli_LightLC_set.LC_OM;
  TRACE_M(TF_SERIAL_CTRL,"Get OM Mode Status: %d\n\r",
          Appli_LightLC_set.LC_OM);
  
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Appli_LightLC_Get_OnOffStatus: This function is callback for Application
to get the application values in middleware used for transition change.
* @param  lcOnOffState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightLC_Get_OnOffStatus(MOBLEUINT8* plcOnOffState)
{  
  *(plcOnOffState) = Appli_LightLC_set.Light_OnOffState;
  TRACE_M(TF_SERIAL_CTRL,"Get On Off Status: %d\n\r",
          Appli_LightLC_set.Light_OnOffState);
  
  
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Appli_LightLC_Get_PropertyStatus: This function is callback for Application
to get the application values in middleware used for transition change.
* @param  plcPropertyState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_LightLC_Get_PropertyStatus(MOBLEUINT8* plcPropertyState)
{  
  return MOBLE_RESULT_SUCCESS;
}  


/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

