/**
******************************************************************************
* @file    appli_generic_client.c
* @author  BLE Mesh Team
* @brief   Application interface for Generic Mesh Models 
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

MOBLEUINT8 Led_Value;
/* Private function prototypes -----------------------------------------------*/
MOBLE_RESULT Appli_GenericClient_OnOff_Set(void);
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Appli_Generic_OnOff_Set: This function is callback for Application
*          when Generic OnOff message is called
* @param  void
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_GenericClient_OnOff_Set(void) 
{ 
  MOBLE_ADDRESS elementAddr = 0; 
  MOBLEUINT8 pGeneric_OnOffParam[2];
   
  Led_Value ^= APPLI_LED_ON;
  pGeneric_OnOffParam[0] = Led_Value;
  GenericClient_OnOff_Set_Unack(elementAddr,
                          (_Generic_OnOffParam*) pGeneric_OnOffParam, 
                          sizeof(pGeneric_OnOffParam) ); 
     
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL
/******************************************************************************/

/**
* @brief  Appli_Generic_Level_Set: This function is callback for Application
*          when Generic Level message is called
* @param  void: 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_GenericClient_Level_Set_Unack(void) 
{ 
  MOBLE_ADDRESS elementAddr = 0; 
  MOBLEUINT8 pGeneric_LevelParam[3];
  
  Appli_IntensityControlPublishing(pGeneric_LevelParam);
  GenericClient_Level_Set_Unack(elementAddr,
                          (_Generic_LevelParam*) pGeneric_LevelParam, 
                          sizeof(pGeneric_LevelParam));
    
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* #ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL */
/******************************************************************************/

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

