/**
  ******************************************************************************
  * @file    appli_light.h
  * @author  BLE Mesh Team
  * @brief   Application interface for Light Mesh Model
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLI_LIGHT_H
#define __APPLI_LIGHT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "light.h"

/* Exported macro ------------------------------------------------------------*/
#define HUE_UPPER_LIMIT 360
/* Exported variables  -------------------------------------------------------*/

/* Light Lightness set */
#pragma pack(1)
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
}Appli_Light_HslSet;

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
#pragma pack(4)

/* Exported Functions Prototypes ---------------------------------------------*/

MOBLE_RESULT Appli_Light_Lightness_Set(Light_LightnessStatus_t*, MOBLEUINT8 OptionalValid);

MOBLE_RESULT Appli_Light_Lightness_Linear_Set(Light_LightnessStatus_t* pLight_LightnessLinearParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_Lightness_Default_Set(Light_LightnessDefaultParam_t* pLight_LightnessDefaultParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_Lightness_Range_Set(Light_LightnessRangeParam_t* pLight_LightnessRangeParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_Ctl_Set(Light_CtlStatus_t* pLight_CtlParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_CtlTemperature_Set(Light_CtlStatus_t* pLight_CtltempParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_CtlTemperature_Range_Set(Light_CtlTemperatureRangeParam_t* pLight_CtlTempRangeParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_CtlDefault_Set(Light_CtlDefaultParam_t* pLight_CtlDefaultParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_Hsl_Set(Light_HslStatus_t* pLight_HslParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_HslHue_Set(Light_HslStatus_t* pLight_HslHueParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_HslSaturation_Set(Light_HslStatus_t* pLight_HslSaturationParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_HslDefault_Set(Light_HslStatus_t* pLight_HslDefaultParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_HslRange_Set(Light_HslRangeParam_t* pLight_HslDefaultParam,
                                     MOBLEUINT8 OptionalValid);
MOBLE_RESULT Appli_Light_GetLightnessStatus(MOBLEUINT8* lLightnessState);
MOBLE_RESULT Appli_Light_GetLightnessLinearStatus(MOBLEUINT8* lLightnessState);
MOBLE_RESULT Appli_Light_GetLightnessDefaultStatus(MOBLEUINT8* lDefaultState);
MOBLE_RESULT Appli_Light_GetLightnessRangeStatus(MOBLEUINT8* lRangeState);
MOBLE_RESULT Appli_Light_GetCtlLightStatus(MOBLEUINT8* lCtlLightState);
MOBLE_RESULT Appli_Light_GetCtlTemperatureStatus(MOBLEUINT8* lCtlTempState);
MOBLE_RESULT Appli_Light_GetCtlTemperatureRange(MOBLEUINT8* lCtlTempRange);
MOBLE_RESULT Appli_Light_GetCtlDefaultStatus(MOBLEUINT8* lCtlDefaultState);
MOBLE_RESULT Appli_Light_GetHslStatus(MOBLEUINT8* lHslState);
MOBLE_RESULT Appli_Light_GetHslHueStatus(MOBLEUINT8* lHslHueState);
MOBLE_RESULT Appli_Light_GetHslSaturationStatus(MOBLEUINT8* lHslSaturationState);
MOBLE_RESULT Appli_Light_GetHslHueRange(MOBLEUINT8* lHslHueRange);
MOBLE_RESULT Appli_Light_GetHslSatRange(MOBLEUINT8* lHslSatRange);
void Appli_Light_PwmInit(void);
void HSL2RGB_Conversion(void);
void Ctl_LedOffState(void);
void Rgb_LedOffState(void);
void RgbF_Create(MOBLEUINT16 value1, MOBLEUINT16 value2, MOBLEUINT16 value3);
void Light_UpdateLedValue(MOBLEUINT8 state , Appli_LightPwmValue_t light_state);


#endif /* __APPLI_LIGHT_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

