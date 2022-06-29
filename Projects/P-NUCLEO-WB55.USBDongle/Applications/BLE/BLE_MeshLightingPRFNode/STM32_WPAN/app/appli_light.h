/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_light.h
  * @author  MCD Application Team
  * @brief   Application interface for Light Mesh Model
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APPLI_LIGHT_H
#define APPLI_LIGHT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "light.h"

/* Exported macro ------------------------------------------------------------*/
#define HUE_UPPER_LIMIT 360
/* Exported variables  -------------------------------------------------------*/

/* Light Lightness set */

#pragma pack(push, 1)

typedef struct
{
  MOBLEUINT16 PresentState16; 
  MOBLEUINT16 LastLightness16;
  MOBLEUINT16 LightnessDefault;
  MOBLEUINT8  StatusCode;
  MOBLEUINT16 RangeMin;     
  MOBLEUINT16 RangeMax;
}Appli_Light_lightnessSet;


/* Light Lightness Linear set */
typedef struct
{
  MOBLEUINT16 PresentState16;
}Appli_Light_lightnessLinearSet;

/* Light Ctl Set */
typedef struct
{
  MOBLEUINT16 PresentLightness16;
  MOBLEUINT16 PresentTemperature16;
  MOBLEINT16 PresentCtlDelta16;
  MOBLEUINT16 TargetLightness16;
  MOBLEUINT16 TargetTemperature16;
  MOBLEINT16 TargetCtlDelta16;
}Appli_Light_CtlSet;

/* Light Ctl Temperature Range Set */
typedef struct
{
  MOBLEUINT8  StatusCode;
  MOBLEUINT16 RangeMin;     
  MOBLEUINT16 RangeMax;
}Appli_Light_CtlTemperatureRangeSet;

/* Light Ctl Default set */
typedef struct
{
  MOBLEUINT16 CtlDefaultLightness16;
  MOBLEUINT16 CtlDefaultTemperature16;
  MOBLEINT16 CtlDefaultDeltaUv;    
}Appli_Light_CtlDefaultSet;

/* Light Hsl Hue set */
typedef struct
{
  MOBLEUINT16 HslLightness16;
  MOBLEUINT16 HslHueLightness16;
  MOBLEUINT16 HslSaturation16; 
  MOBLEUINT16 TargetHslLightness16;
  MOBLEUINT16 TargetHslHueLightness16;
  MOBLEUINT16 TargetHslSaturation16; 
}Appli_Light_HslSet;

/* Light Hsl Hue set */
typedef struct
{
  MOBLEUINT16 HslDefaultLightness16;
  MOBLEUINT16 HslDefaultHueLightness16;
  MOBLEUINT16 HslDefaultSaturation16; 
}Appli_Light_HslDefaultSet;

typedef struct
{
  MOBLEUINT16 Red_Value;
  MOBLEUINT16 Green_Value;
  MOBLEUINT16 Blue_Value; 
}Appli_Light_RGBSet;

typedef struct
{
  MOBLEUINT8 StatusCode;
  MOBLEUINT16 HslHueMinRange16;
  MOBLEUINT16 HslHueMaxRange16;
  MOBLEUINT16 HslMinSaturation16; 
  MOBLEUINT16 HslMaxSaturation16; 
}Appli_Light_HslRangeSet;

typedef struct
{
  MOBLEUINT16 IntensityValue;
  MOBLEUINT16 PwmRedValue;
  MOBLEUINT16 PwmGreenValue;
  MOBLEUINT16 PwmBlueValue;
  MOBLEUINT16 PwmWarmValue;
  MOBLEUINT16 PwmCoolValue; 
}Appli_LightPwmValue_t;

#pragma pack(pop)

/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Appli_Light_Lightness_Set(Light_LightnessStatus_t*, 
                                       MOBLEUINT8 OptionalValid,
                                       MOBLEUINT16 dstPeer, 
                                       MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Lightness_Linear_Set(Light_LightnessStatus_t* pLight_LightnessLinearParam,
                                              MOBLEUINT8 OptionalValid,
                                              MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Lightness_Default_Set(Light_LightnessDefaultParam_t* pLight_LightnessDefaultParam,
                                               MOBLEUINT8 OptionalValid,
                                               MOBLEUINT16 dstPeer, 
                                               MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Lightness_Last_Set(Light_LightnessStatus_t* pLight_LightnessParam,                                               
                                            MOBLEUINT8 elementIndex);
                                              
MOBLE_RESULT Appli_Light_Lightness_Range_Set(Light_LightnessRangeParam_t* pLight_LightnessRangeParam,
                                             MOBLEUINT8 OptionalValid,
                                             MOBLEUINT16 dstPeer, 
                                             MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Ctl_Set(Light_CtlStatus_t* pLight_CtlParam,
                                 MOBLEUINT8 OptionalValid,
                                 MOBLEUINT16 dstPeer, 
                                 MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_CtlTemperature_Set(Light_CtlStatus_t* pLight_CtltempParam,
                                            MOBLEUINT8 OptionalValid,
                                            MOBLEUINT16 dstPeer, 
                                            MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_CtlTemperature_Range_Set(Light_CtlTemperatureRangeParam_t* pLight_CtlTempRangeParam,
                                                  MOBLEUINT8 OptionalValid,
                                                  MOBLEUINT16 dstPeer, 
                                                  MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_CtlDefault_Set(Light_CtlDefaultParam_t* pLight_CtlDefaultParam,
                                        MOBLEUINT8 OptionalValid,
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_Hsl_Set(Light_HslStatus_t* pLight_HslParam,
                                 MOBLEUINT8 OptionalValid,
                                 MOBLEUINT16 dstPeer, 
                                 MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslHue_Set(Light_HslStatus_t* pLight_HslHueParam,
                                    MOBLEUINT8 OptionalValid,
                                    MOBLEUINT16 dstPeer, 
                                    MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslSaturation_Set(Light_HslStatus_t* pLight_HslSaturationParam,
                                           MOBLEUINT8 OptionalValid,
                                           MOBLEUINT16 dstPeer, 
                                           MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslDefault_Set(Light_HslDefaultParam_t* pLight_HslDefaultParam,
                                        MOBLEUINT8 OptionalValid,
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_HslRange_Set(Light_HslRangeParam_t* pLight_HslDefaultParam,
                                      MOBLEUINT8 OptionalValid,
                                      MOBLEUINT16 dstPeer, 
                                      MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetLightnessStatus(MOBLEUINT8* lLightnessState,
                                            MOBLEUINT16 dstPeer, 
                                            MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetLightnessLinearStatus(MOBLEUINT8* lLightnessState,
                                                  MOBLEUINT16 dstPeer, 
                                                  MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetLightnessDefaultStatus(MOBLEUINT8* lDefaultState,
                                                   MOBLEUINT16 dstPeer, 
                                                   MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetLightnessLastStatus(MOBLEUINT8* lLastState,
                                                MOBLEUINT16 dstPeer, 
                                                MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetLightnessRangeStatus(MOBLEUINT8* lRangeState,
                                                 MOBLEUINT16 dstPeer, 
                                                 MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetCtlLightStatus(MOBLEUINT8* lCtlLightState,
                                           MOBLEUINT16 dstPeer,  
                                           MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetCtlTargetStatus(MOBLEUINT8* lCtlLightState, 
                                            MOBLEUINT16 dstPeer, 
                                            MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetCtlTemperatureStatus(MOBLEUINT8* lCtlTempState,
                                                 MOBLEUINT16 dstPeer, 
                                                 MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetCtlTemperatureRange(MOBLEUINT8* lCtlTempRange,
                                                MOBLEUINT16 dstPeer, 
                                                MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetCtlDefaultStatus(MOBLEUINT8* lCtlDefaultState,
                                             MOBLEUINT16 dstPeer, 
                                             MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetHslStatus(MOBLEUINT8* lHslState,
                                      MOBLEUINT16 dstPeer, 
                                      MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetHslTargetStatus(MOBLEUINT8* lHslState, 
                                            MOBLEUINT16 dstPeer, 
                                            MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetHslHueStatus(MOBLEUINT8* lHslHueState,
                                         MOBLEUINT16 dstPeer, 
                                         MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetHslSaturationStatus(MOBLEUINT8* lHslSaturationState,
                                                MOBLEUINT16 dstPeer, 
                                                MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetHslHueRange(MOBLEUINT8* lHslHueRange,
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetHslSatRange(MOBLEUINT8* lHslSatRange,
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex);

MOBLE_RESULT Appli_Light_GetHslDefaultStatus(MOBLEUINT8* lHslDefaultState,
                                             MOBLEUINT16 dstPeer, 
                                             MOBLEUINT8 elementIndex);

void Appli_Light_PwmInit(void);
void HSL2RGB_Conversion(MOBLEUINT8 elementIndex);
void Ctl_LedOffState(void);
void Rgb_LedOffState(void);
void RgbF_Create(MOBLEUINT16 value1, 
                 MOBLEUINT16 value2, 
                 MOBLEUINT16 value3, 
                 MOBLEUINT8 elementIndex);
void Light_UpdateLedValue(MOBLEUINT8 state,
                          Appli_LightPwmValue_t light_state);

/******************************************************************************/
#endif /* APPLI_LIGHT_H */
/******************************************************************************/


