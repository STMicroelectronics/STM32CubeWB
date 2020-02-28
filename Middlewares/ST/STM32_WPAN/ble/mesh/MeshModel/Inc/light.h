/**
******************************************************************************
* @file    light.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Header file for the Lighting Model file 
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
* Initial BLE-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIGHT_H
#define __LIGHT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"
#include <string.h>
/* Exported macro ------------------------------------------------------------*/

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/

/* 7.1 Messages summary Page 298 */
/* Light Lightness Server Model Opcodes */
#define LIGHT_LIGHTNESS_GET                     0x824B
#define LIGHT_LIGHTNESS_SET                     0x824C
#define LIGHT_LIGHTNESS_SET_UNACK               0x824D
#define LIGHT_LIGHTNESS_STATUS                  0x824E
#define LIGHT_LIGHTNESS_LINEAR_GET              0x824F
#define LIGHT_LIGHTNESS_LINEAR_SET              0x8250
#define LIGHT_LIGHTNESS_LINEAR_SET_UNACK        0x8251
#define LIGHT_LIGHTNESS_LINEAR_STATUS           0x8252
#define LIGHT_LIGHTNESS_LAST_GET                0x8253
#define LIGHT_LIGHTNESS_LAST_STATUS             0x8254
#define LIGHT_LIGHTNESS_DEFAULT_GET             0x8255
#define LIGHT_LIGHTNESS_DEFAULT_STATUS          0x8256
#define LIGHT_LIGHTNESS_RANGE_GET               0x8257
#define LIGHT_LIGHTNESS_RANGE_STATUS            0x8258
#define LIGHT_LIGHTNESS_DEFAULT_SET             0x8259            
#define LIGHT_LIGHTNESS_DEFAULT_SET_UNACK       0x825A
#define LIGHT_LIGHTNESS_RANGE_SET               0x825B
#define LIGHT_LIGHTNESS_RANGE_SET_UNACK         0x825C
#define LIGHT_CTL_GET                           0x825D
#define LIGHT_CTL_SET                           0x825E
#define LIGHT_CTL_SET_UNACK                     0x825F
#define LIGHT_CTL_STATUS                        0x8260
#define LIGHT_CTL_TEMPERATURE_GET               0x8261
#define LIGHT_CTL_TEMPERATURE_RANGE_GET         0x8262
#define LIGHT_CTL_TEMPERATURE_RANGE_STATUS      0x8263
#define LIGHT_CTL_TEMPERATURE_SET               0x8264
#define LIGHT_CTL_TEMPERATURE_SET_UNACK         0x8265
#define LIGHT_CTL_TEMPERATURE_STATUS            0x8266
#define LIGHT_CTL_DEFAULT_GET                   0x8267
#define LIGHT_CTL_DEFAULT_STATUS                0x8268
#define LIGHT_CTL_DEFAULT_SET                   0x8269
#define LIGHT_CTL_DEFAULT_SET_UNACK             0x826A
#define LIGHT_CTL_TEMPERATURE_RANGE_SET        0x826B
#define LIGHT_CTL_TEMPERATURE_RANGE_SET_UNACK  0x826C
#define LIGHT_HSL_GET                           0x826D
#define LIGHT_HSL_HUE_GET                       0x826E
#define LIGHT_HSL_HUE_SET                       0x826F
#define LIGHT_HSL_HUE_SET_UNACK                 0x8270
#define LIGHT_HSL_HUE_STATUS                    0x8271  
#define LIGHT_HSL_SATURATION_GET                0x8272
#define LIGHT_HSL_SATURATION_SET                0x8273  
#define LIGHT_HSL_SATURATION_SET_UNACK          0x8274
#define LIGHT_HSL_SATURATION_STATUS             0x8275
#define LIGHT_HSL_SET                           0x8276
#define LIGHT_HSL_SET_UNACK                     0x8277
#define LIGHT_HSL_STATUS                        0x8278
#define LIGHT_HSL_TARGET_GET                    0x8279
#define LIGHT_HSL_TARGET_STATUS                 0x827A
#define LIGHT_HSL_DEFAULT_GET                   0x827B
#define LIGHT_HSL_DEFAULT_STATUS                0x827C
#define LIGHT_HSL_RANGE_GET                     0x827D
#define LIGHT_HSL_RANGE_STATUS                  0x827E
#define LIGHT_HSL_DEFAULT_SET                   0x827F
#define LIGHT_HSL_DEFAULT_SET_UNACK             0x8280
#define LIGHT_HSL_RANGE_SET                     0x8281
#define LIGHT_HSL_RANGE_SET_UNACK               0x8282
#define LIGHT_XYL_GET                           0x8283
#define LIGHT_XYL_SET                           0x8284
#define LIGHT_XYL_SET_UNACK                     0x8285 
#define LIGHT_XYL_STATUS                        0x8286
#define LIGHT_XYL_TARGET_GET                    0x8287
#define LIGHT_XYL_TARGET_STATUS                 0x8288
#define LIGHT_XYL_DEFAULT_GET                   0x8289
#define LIGHT_XYL_DEFAULT_STATUS                0x828A
#define LIGHT_XYL_RANGE_GET                     0x828B
#define LIGHT_XYL_RANGE_STATUS                  0x828C
#define LIGHT_XYL_DEFAULT_SET                   0x828D
#define LIGHT_XYL_DEFAULT_SET_UNACK             0x828E
#define LIGHT_XYL_RANGE_SET                     0x828F
#define LIGHT_XYL_RANGE_SET_UNACK               0x8290

/******************************************************************************/
/********** Following Section defines the SIG MODEL IDs            ************/
/******************************************************************************/

#define LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID       0x1300
#define LIGHT_MODEL_SERVER_LIGHTNESS_SETUP_MODEL_ID 0x1301
#define LIGHT_MODEL_SERVER_CTL_MODEL_ID             0x1303
#define LIGHT_MODEL_SERVER_CTL_SETUP_MODEL_ID       0x1304
#define LIGHT_MODEL_SERVER_CTL_TEMPERATURE_MODEL_ID 0x1306
#define LIGHT_MODEL_SERVER_HSL_MODEL_ID             0x1307
#define LIGHT_MODEL_SERVER_HSL_SETUP_MODEL_ID       0x1308
#define LIGHT_MODEL_SERVER_HSL_HUE_MODEL_ID         0x130A
#define LIGHT_MODEL_SERVER_HSL_SATURATION_MODEL_ID  0x130B
#define LIGHT_MODEL_SERVER_XYL_MODEL_ID             0x130C
#define LIGHT_MODEL_SERVER_XYL_SETUP_MODEL_ID       0x130D

/******************************************************************************/
/********** SIG MODEL IDs ends                                     ************/
/******************************************************************************/

/****************************************************/
/* Macros for Light Lightness Range value*/
#define MIN_RANGE_INVALID_STATUS_CODE       0x01
#define MAX_RANGE_INVALID_STATUS_CODE       0x02
#define SUCCESS_STATUS_CODE                 0x00
#define MIN_VALID_RANGE                     0x00
#define MAX_VALID_RANGE                     0xFFFF
#define RANGE_INVALID_VALUE                 0x00

/* Macros for Ctl Temperature range value */
#define MIN_CTL_TEMP_RANGE      0X0320
#define MAX_CTL_TEMP_RANGE      0X4E20
#define UNKNOWN_CTL_TEMP_VALUE  0XFFFF
#define MAX_CTL_DELTA_VALUE     0X7FFF
#define MIN_CTL_DELTA_VALUE     0X8000

/* Model Binding Macros Flag */
#define BINDING_LIGHT_LIGHTNESS_ACTUAL_SET     0X01
#define BINDING_LIGHT_LIGHTNESS_LINEAR_SET     0X00
#define BINDING_LIGHT_CTL_LIGHTNESS_SET        0X00

/* Macros used for light restore value */
#define LIGHT_LIGHTNESS_NVM_FLAG               0X03
#define LIGHT_CTL_NVM_FLAG                     0X04
#define LIGHT_HSL_NVM_FLAG                     0X05

/* Define the following Macros to change the step resolution and step count value */
#define TRANSITION_SCALER                  1
#define PWM_DEFAULT_VALUE                  10000U
#define PWM_VALUE_OFF                      1
/******************************************************************************/
/* Macros are used to update the PWM state according to the condition. */
#define         RESUME_STATE       1
#define         RESET_STATE        2
#define         DEFAULT_STATE      3
#define         LOAD_STATE         4

/* Macros for the Light model transition flag */
#define LIGHT_TRANSITION_STOP                   0X00
#define LIGHT_LIGHTNESS_TRANSITION_START        0X01
#define LIGHT_LINEAR_TRANSITION_START           0X02
#define LIGHT_CTL_TRANSITION_START              0X03
#define LIGHT_TEMPERATURE_TRANSITION_START      0X04
#define LIGHT_HSL_TRANSITION_START              0X05
#define LIGHT_HSL_HUE_TRANSITION_START          0X06
#define LIGHT_HSL_SATURATION_TRANSITION_START   0X07
/* Exported variables  ------------------------------------------------------- */
/* Transition flag of models*/
#pragma pack(1)
typedef struct
{
  MOBLEUINT8 LightTransitionFlag;
  MOBLEUINT8 LightOptionalParam; 
}Light_ModelFlag_t;
/**************************************/

/* Transitiojn time variables*/
typedef struct
{
  MOBLEINT8   StepValue ;
  MOBLEUINT32 Res_Value;
  MOBLEUINT8  ResBitValue;
}Light_TimeParam_t;
/**************************************/

/* Light Lightness message parameters*/
typedef struct
{
  MOBLEUINT16 TargetLightnessStatus;
  MOBLEUINT16 TargetLightnessLinearStatus;
  MOBLEUINT8 TID;
  MOBLEUINT8 TransitionTime;
  MOBLEUINT8 Delay_Time;
} Light_LightnessParam_t;

typedef struct
{
  MOBLEUINT16 PresentValue16;
  MOBLEUINT16 TargetValue16;
  MOBLEUINT8  RemainingTime;
} Light_LightnessStatus_t;

typedef struct
{
  MOBLEUINT16 LightnessLastStatus;
} Light_LightnessLastParam_t;

typedef struct
{
  MOBLEUINT16 LightnessDefaultStatus;
} Light_LightnessDefaultParam_t;

typedef struct
{
  MOBLEUINT8  StatusCode;
  MOBLEUINT16 MinRangeStatus;
  MOBLEUINT16 MaxRangeStatus;
} Light_LightnessRangeParam_t;
/**************************************/

/* Light Ctl message parameters*/
typedef struct
{
  MOBLEUINT16 CTL_Lightness;
  MOBLEUINT16 CTL_Temperature;
  MOBLEINT16 CTL_DeltaUv;
  MOBLEUINT8  CTL_TID;
  MOBLEUINT8  CTL_TransitionTime;
  MOBLEUINT8  CTL_Delay;
  MOBLEUINT16 Last_Lightness;
  MOBLEUINT16 Last_Temperature;
}Light_CtlParam_t;

typedef struct
{
  MOBLEUINT16 PresentCtlLightness16;
  MOBLEUINT16 PresentCtlTemperature16;
  MOBLEINT16 PresentCtlDelta16;
  MOBLEUINT16 TargetCtlLightness16;
  MOBLEUINT16 TargetCtlTemperature16;
  MOBLEINT16 TargetCtlDeltaUv16;
  MOBLEUINT8 RemainingTime;
}Light_CtlStatus_t;

typedef struct
{
  MOBLEUINT16 PresentParam_1;
  MOBLEUINT16 PresentParam_2;
  MOBLEUINT16 PresentParam_3;
  MOBLEUINT16 TargetParam_1;
  MOBLEUINT16 TargetParam_2;
  MOBLEUINT16 TargetParam_3;
  MOBLEUINT8 RemainingTime;
}Light_TemporaryStatus_t;

/* Light CTL Temperature Range Parameter */
typedef struct
{
  MOBLEUINT8  StatusCode;
  MOBLEUINT16 MinRangeStatus;
  MOBLEUINT16 MaxRangeStatus;
}Light_CtlTemperatureRangeParam_t;

/* Light CTL Default Parameter */
typedef struct
{
  MOBLEUINT16 CtlDefaultLightness16;
  MOBLEUINT16 CtlDefaultTemperature16;
  MOBLEINT16 CtlDefaultDeltaUv;    
}Light_CtlDefaultParam_t;  

/* Light Hsl Hue Parameter */
typedef struct
{
  MOBLEUINT16 HslLightness16;
  MOBLEUINT16 HslHueLightness16;
  MOBLEUINT16 HslSaturation16; 
  MOBLEUINT8  Hsl_TID;
  MOBLEUINT8  Hsl_TransitionTime;
  MOBLEUINT8  Hsl_Delay;
}Light_HslParam_t;

typedef struct
{
  MOBLEUINT16 PresentHslLightness16;
  MOBLEUINT16 PresentHslHueLightness16;
  MOBLEUINT16 PresentHslSaturation16;
  MOBLEUINT16 TargetHslLightness16;
  MOBLEUINT16 TargetHslHueLightness16;
  MOBLEUINT16 TargetHslSaturation16;
  MOBLEUINT8 RemainingTime;
}Light_HslStatus_t;
  
typedef struct
{
  MOBLEUINT16 HslHueDefault16;
  MOBLEUINT16 HslSaturationDefault16;
  MOBLEUINT16 HslLightnessDefualt16;
}Light_HslDefault_t;
 
typedef struct
{
  MOBLEUINT16 HslHueMinRange16;
  MOBLEUINT16 HslHueMaxRange16;
  MOBLEUINT16 HslMinSaturation16; 
  MOBLEUINT16 HslMaxSaturation16; 
}Light_HslRangeParam_t;
/**************************************/
typedef struct
{
  /* Pointer to the function Appli_Light_Lightness_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Lightness_Set_cb)(Light_LightnessStatus_t*, MOBLEUINT8);  
  
  MOBLE_RESULT (*Lightness_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_Lightness_Linear_Set used for callback 
     from the middle layer to Application layer
  */  
  MOBLE_RESULT (*Lightness_Linear_Set_cb)(Light_LightnessStatus_t*, MOBLEUINT8);  
   
  MOBLE_RESULT (*Lightness_Linear_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
   
   
  /* Pointer to the function Appli_Light_Lightness_Default_Set used for callback 
     from the middle layer to Application layer
  */  
  MOBLE_RESULT (*Lightness_Default_Set_cb)(Light_LightnessDefaultParam_t*, MOBLEUINT8);
  
   MOBLE_RESULT (*Lightness_Default_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_Lightness_Range_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Lightness_Range_Set_cb)(Light_LightnessRangeParam_t*, MOBLEUINT8);
 
  MOBLE_RESULT (*Lightness_Range_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
 
  /* Pointer to the function Appli_Light_Ctl_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_Ctl_Set_cb)(Light_CtlStatus_t*, MOBLEUINT8);
  
  MOBLE_RESULT (*Light_Ctl_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_CtlTemperature_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_CtlTemperature_Set_cb)(Light_CtlStatus_t*, MOBLEUINT8);
  
  MOBLE_RESULT (*Light_CtlTemperature_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
       
  /* Pointer to the function Appli_Light_CtlTemperature_Range_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_CtlTemperature_Range_Set_cb)(Light_CtlTemperatureRangeParam_t*, MOBLEUINT8);
  
  MOBLE_RESULT (*Light_CtlTemperature_Range_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_CtlDefault_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_CtlDefault_Set_cb)(Light_CtlDefaultParam_t*, MOBLEUINT8);
    
  MOBLE_RESULT (*Light_CtlDefault_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
    
  /* Pointer to the function Appli_Light_Hsl_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_Hsl_Set_cb)(Light_HslStatus_t*, MOBLEUINT8);
  
  MOBLE_RESULT (*Light_Hsl_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_HslHue_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_HslHue_Set_cb)(Light_HslStatus_t*, MOBLEUINT8);
  
  MOBLE_RESULT (*Light_HslHue_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_HslSaturation_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_HslSaturation_Set_cb)(Light_HslStatus_t*, MOBLEUINT8);
  
   MOBLE_RESULT (*Light_HslSaturation_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_HslDefault_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_HslDefault_Set_cb)(Light_HslStatus_t*, MOBLEUINT8);
  
  MOBLE_RESULT (*Light_HslDefault_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
  
  /* Pointer to the function Appli_Light_HslRange_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Light_HslRange_Set_cb)(Light_HslRangeParam_t*, MOBLEUINT8);
  
  MOBLE_RESULT (*Light_HslRange_Status_cb)(MOBLEUINT8 const *, MOBLEUINT32);
} Appli_Light_cb_t;
 
typedef struct
{ 
  
  MOBLE_RESULT (*GetLightLightness_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightLightnessLinear_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightLightnessDefault_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightLightnessRange_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightCtl_cb)(MOBLEUINT8*); 
  MOBLE_RESULT (*GetLightCtlTemp_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightCtlTempRange_cb)(MOBLEUINT8*); 
  MOBLE_RESULT (*GetLightCtlDefault_cb)(MOBLEUINT8*); 
  MOBLE_RESULT (*GetLightHsl_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightHslHue_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightHslSaturation_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightHslHueRange_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightHslSatRange_cb)(MOBLEUINT8*);
  MOBLE_RESULT (*GetLightHslDefault_cb)(MOBLEUINT8*);
}Appli_Light_GetStatus_cb_t; 
#pragma pack(4)

extern const Appli_Light_GetStatus_cb_t Appli_Light_GetStatus_cb;

extern const Appli_Light_cb_t LightAppli_cb;

/* Exported Functions Prototypes ---------------------------------------------*/

void BLEMesh_LightModelAppliCb (Appli_Light_cb_t* map );

MOBLE_RESULT Light_Lightness_Set(const MOBLEUINT8* plightness_param, MOBLEUINT32 length);
MOBLE_RESULT Light_Lightness_Status(MOBLEUINT8* pLightness_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_Lightness_Linear_Set(const MOBLEUINT8* plightnessLinear_param, MOBLEUINT32 length);
MOBLE_RESULT Light_Lightness_Linear_Status(MOBLEUINT8* pLightnessLinear_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_Lightness_Last_Status(MOBLEUINT8* pLightnessLast_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_Lightness_Default_Set(const MOBLEUINT8* plightnessDefault_param, MOBLEUINT32 length);
MOBLE_RESULT Light_Lightness_Default_Status(MOBLEUINT8* pLightnessDefault_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_Lightness_Range_Set(const MOBLEUINT8* plightnessRange_param, MOBLEUINT32 length);
MOBLE_RESULT Light_Lightness_Range_Status(MOBLEUINT8* pLightnessRange_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_Ctl_Set(const MOBLEUINT8* pLightCtl_param, MOBLEUINT32 length);
MOBLE_RESULT Light_CtlTemperature_Set(const MOBLEUINT8* pLightCtlTemp_param, MOBLEUINT32 length);
MOBLE_RESULT Light_CtlTemperature_Range_Set(const MOBLEUINT8* plightCtlTempRange_param, MOBLEUINT32 length);
MOBLE_RESULT Light_CtlTemperature_Range_Status(MOBLEUINT8* pCtlTempRange_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_CtlDefault_Set(const MOBLEUINT8* pCtlDefault_param, MOBLEUINT32 length);
MOBLE_RESULT Light_CtlDefault_Status(MOBLEUINT8* pCtlDefault_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_Hsl_Set(const MOBLEUINT8* pHsl_param, MOBLEUINT32 length);
MOBLE_RESULT Light_Hsl_Status(MOBLEUINT8* pHsl_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_HslHue_Set(const MOBLEUINT8* pHslHue_param, MOBLEUINT32 length);
MOBLE_RESULT Light_HslHue_Status(MOBLEUINT8* pHslHue_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_HslSaturation_Set(const MOBLEUINT8* pHslSaturation_param, MOBLEUINT32 length);
MOBLE_RESULT Light_HslSaturation_Status(MOBLEUINT8* pHslSaturation_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_HslDefault_Set(const MOBLEUINT8* pHslDefault_param, MOBLEUINT32 length);
MOBLE_RESULT Light_HslDefault_Status(MOBLEUINT8* pHslDefault_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_HslRange_Set(const MOBLEUINT8* pHslRange_param, MOBLEUINT32 length);
MOBLE_RESULT Light_HslRange_Status(MOBLEUINT8* pHslRange_status, MOBLEUINT32 *pLength);
MOBLE_RESULT Light_HslTarget_Status(MOBLEUINT8* pHslTarget_status, MOBLEUINT32 *pLength);


MOBLE_RESULT LightModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);

MOBLE_RESULT LightModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pData,
                                    MOBLEUINT32 length,
                                    MOBLEBOOL response);


MOBLE_RESULT LightModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *data, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response
                                    );
void Lighting_Process(void);
MOBLE_RESULT BLEMesh_AddLightingModels(void);

void Light_BindingCtlToLightness_Actual(MOBLEUINT8 bindingFlag);
void Light_BindingTemperatureToTemperatureRange(void);

void LightActual_GenericOnOffBinding(Light_LightnessParam_t* lightActual);
void LightActual_GenericLevelBinding(Light_LightnessParam_t* lightActual);
void Light_CtlTemp_GenericLevelBinding(Light_CtlParam_t* bCtlTempParam);
void Light_Linear_ActualImplicitBinding(MOBLEUINT8 bindingFlag ,MOBLEUINT32 length);
MOBLEUINT16 Light_Actual_LinearBinding(void);
MOBLEUINT16 Light_Linear_ActualBinding(MOBLEUINT32 length);
void Light_Actual_RangeBinding(Light_LightnessParam_t* lightActual);
void Light_CtlTemperature_TempRangeBinding(Light_CtlParam_t* ctlTemperature);
void Light_HslHue_GenericLevelBinding(Light_HslParam_t* bHslHueParam);
void Light_HslHue_RangeBinding(Light_HslParam_t* bHslHueParam);
void Light_HslSaturation_GenericLevelBinding(Light_HslParam_t* bHslSatParam);
void Light_HslSaturation_RangeBinding(Light_HslParam_t* bHslSatParam);
void Light_HslLightness_LightnessActualBinding(Light_HslParam_t* bHslLightParam);
void Light_ActualLightness_HslLightnessBinding(Light_LightnessParam_t* bActualLightParam);

MOBLE_RESULT Light_TransitionBehaviourSingle_Param(MOBLEUINT8 *GetValue);
MOBLE_RESULT Light_TransitionBehaviourMulti_Param(MOBLEUINT8 *GetValue , MOBLEUINT8 param_Count);

void Light_GetStepValue(MOBLEUINT8 stepParam);

MOBLE_RESULT LightLightnessStateUpdate_Process(void);
MOBLE_RESULT LightLinearStateUpdate_Process(void);
MOBLE_RESULT LightCtlStateUpdate_Process(void);
MOBLE_RESULT LightCtlTemperatureStateUpdate_Process(void);
MOBLE_RESULT LightHslStateUpdate_Process(void);
MOBLE_RESULT LightHslHueStateUpdate_Process(void);
MOBLE_RESULT LightHslSaturationStateUpdate_Process(void);
void Light_HslHue_RangeBinding(Light_HslParam_t* bHslHueParam);

void Light_RestoreStates(MOBLEUINT8 const *pModelState_Load, MOBLEUINT8 size);

void Light_LightnessDefaultTransitionValue(Light_LightnessParam_t* pLightnessValue);
void Light_CTLDefaultTransitionValue(Light_CtlParam_t* pCTLValue);
void Light_CTLTemperatureDefaultTransitionValue(Light_CtlParam_t* pCTLValue);

MOBLE_RESULT Light_Client_Lightness_Status(MOBLEUINT8 const *pLightness_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_Lightness_Linear_Status(MOBLEUINT8 const *pLightnessLinear_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_Lightness_Last_Status(MOBLEUINT8 const *pLightnessLast_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_Lightness_Default_Status(MOBLEUINT8 const *pLightnessDefault_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_Lightness_Range_Status(MOBLEUINT8 const *pLightnessRange_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_Ctl_Status(MOBLEUINT8 const *pLightCtl_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_CtlTemperature_Range_Status(MOBLEUINT8 const *pCtlTempRange_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_CtlDefault_Status(MOBLEUINT8 const *pCtlDefault_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_CtlTemperature_Status(MOBLEUINT8 const *pLightCtlTemp_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_Hsl_Status(MOBLEUINT8 const *pHsl_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_HslDefault_Status(MOBLEUINT8 const *pHslDefault_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_HslRange_Status(MOBLEUINT8 const *pHslRange_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_HslTarget_Status(MOBLEUINT8 const *pHslTarget_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_HslHue_Status(MOBLEUINT8 const *pHslHue_status, MOBLEUINT32 pLength);
MOBLE_RESULT Light_Client_HslSaturation_Status(MOBLEUINT8 const *pHslSaturation_status, MOBLEUINT32 pLength);

#endif /* __LIGHT_MODEL_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

