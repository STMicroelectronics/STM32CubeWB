/**
******************************************************************************
* @file    appli_light_lc.h
* @author  BLE Mesh Team
* @brief   Application interface for Light Control Mesh Models  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLI_LIGHT_LC_H
#define __APPLI_LIGHT_LC_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "light_lc.h"


/* Exported macro ------------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/
/*******************************************************************************
Following Variables are used for the Light LC Mode MODEL 
*******************************************************************************/
/* Light Lightness set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT8 LC_mode;
  MOBLEUINT8 LC_OM;
  MOBLEUINT8 Light_OnOffState;
  MOBLEUINT16 Light_OnOffValue;
}Appli_LightLC_Set_t;

typedef struct
{
  MOBLEUINT16 Property_ID;
  MOBLEUINT16 Property_Value; 
}Appli_Light_LC_PropertySet_t;
#pragma pack(4)


MOBLE_RESULT Appli_LightLC_Mode_Set(Light_LC_Param_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_LightLC_OM_Set(Light_LC_Param_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_LightLC_OnOff_Set(Light_LC_Param_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_LightLC_Get_OnOffStatus(MOBLEUINT8* plcOnOffState);   
MOBLE_RESULT Appli_LightLC_Get_ModeStatus(MOBLEUINT8* plcModeState);
MOBLE_RESULT Appli_LightLC_Get_OMModeStatus(MOBLEUINT8* plcOM_ModeState);
MOBLEUINT16 Appli_LightLC_Get_AmbientLuxLevelOutput(void);
MOBLEUINT16 Appli_Light_LC_PIRegulatorOutput(MOBLEUINT16 tableLuxLevel,
                                               MOBLEUINT16 ambientLuxLevel);
#endif /* __APPLI_LIGHT_LC_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

