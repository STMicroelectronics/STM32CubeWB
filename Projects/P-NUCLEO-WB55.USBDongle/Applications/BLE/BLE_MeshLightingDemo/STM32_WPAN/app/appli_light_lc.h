/**
******************************************************************************
* @file    appli_light_ctrl.h
* @author  BLE Mesh Team
* @version V1.10.000
* @date    15-Jan-2019
* @brief   Application interface for Light Control Mesh Models  
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Initial BlueNRG-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
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

#pragma pack(1)
typedef struct
{
  MOBLEUINT16 Property_ID;
  MOBLEUINT16 Property_Value; 
}Appli_Light_LC_PropertySet_t;


MOBLE_RESULT Appli_LightLC_Mode_Set(Light_LC_Param_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_LightLC_OM_Set(Light_LC_Param_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_LightLC_OnOff_Set(Light_LC_OnOffState_t* pLight_LC_Param,
                                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_LightLC_Get_OnOffStatus(MOBLEUINT8* plcOnOffState);   
MOBLE_RESULT Appli_LightLC_Get_ModeStatus(MOBLEUINT8* plcModeState);
MOBLE_RESULT Appli_LightLC_Get_OMModeStatus(MOBLEUINT8* plcOM_ModeState);
MOBLE_RESULT Appli_LightLC_Get_PropertyStatus(MOBLEUINT8* plcPropertyState);
MOBLEUINT16 Appli_LightLC_Get_AmbientLuxLevelOutput(void);
MOBLEUINT16 Appli_Light_LC_PIRegulatorOutput(MOBLEUINT16 tableLuxLevel,
                                               MOBLEUINT16 ambientLuxLevel);
#endif /* __APPLI_LIGHT_LC_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

