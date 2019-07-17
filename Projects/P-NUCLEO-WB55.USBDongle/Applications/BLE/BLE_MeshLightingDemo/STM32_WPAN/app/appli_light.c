/**
  ******************************************************************************
  * @file    appli_light.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Lighting Mesh Models 
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
#include "app_conf.h"
#include "hal_common.h"
#include "types.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "light.h"
#include "appli_light.h"
#include "appli_generic.h"
#include "common.h"
#include "mesh_cfg.h"

/** @addtogroup BLE_Mesh
*  @{
*/

/** @addtogroup models_BLE
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/*******************************************************************************
Following Variables are used for the LIGHTING Lightness MODEL 
*******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS

/* Light Lightness set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT16 PresentState16;  
  MOBLEUINT16 LastLightness16;
}Appli_Light_lightnessSet;

Appli_Light_lightnessSet ApplilightnessSet;

/* Light Lightness Linear set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT16 PresentState16;
}Appli_Light_lightnessLinearSet;

Appli_Light_lightnessLinearSet ApplilightnessLinearSet;

/* Light Lightness Default set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT16 LightnessDefault;
}Appli_Light_lightnessDefaultSet;

Appli_Light_lightnessDefaultSet ApplilightnessDefaultSet;

/* Light Lightness Range set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT8  StatusCode;
  MOBLEUINT16 RangeMin;     
  MOBLEUINT16 RangeMax;
}Appli_Light_lightnessRangeSet;

Appli_Light_lightnessRangeSet ApplilightnessRangeSet = {0x00,0x01,0xFFFF};

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  */
/******************************************************************************/

/*******************************************************************************
Following Variables are used for the LIGHTING CTL MODEL 
*******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL

/* Light Ctl Set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT16 PresentLightness16;
  MOBLEUINT16 PresentTemperature16;
  MOBLEINT16 PresentCtlDelta16;
}Appli_Light_CtlSet;

Appli_Light_CtlSet AppliCtlSet;

/* Light Ctl Temperature Range Set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT8  StatusCode;
  MOBLEUINT16 RangeMin;     
  MOBLEUINT16 RangeMax;
}Appli_Light_CtlTemperatureRangeSet;

Appli_Light_CtlTemperatureRangeSet AppliCtlTemperatureRangeSet;

/* Light Ctl Default set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT16 CtlDefaultLightness16;
  MOBLEUINT16 CtlDefaultTemperature16;
  MOBLEINT16 CtlDefaultDeltaUv;    
}Appli_Light_CtlDefaultSet;

Appli_Light_CtlDefaultSet AppliCtlDefaultSet;

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL */
/******************************************************************************/

/*******************************************************************************
Following Variables are used for the LIGHTING HSL MODEL 
*******************************************************************************/


#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
/* Light Hsl Hue set */
#pragma pack(1)
typedef struct
{
  MOBLEUINT16 HslLightness16;
  MOBLEUINT16 HslHueLightness16;
  MOBLEUINT16 HslSaturation16; 
}Appli_Light_HslSet;

Appli_Light_HslSet AppliHslSet;

#pragma pack(1)
typedef struct
{
  MOBLEUINT8 StatusCode;
  MOBLEUINT16 HslHueMinRange16;
  MOBLEUINT16 HslHueMaxRange16;
  MOBLEUINT16 HslMinSaturation16; 
  MOBLEUINT16 HslMaxSaturation16; 
}Appli_Light_HslRangeSet;

Appli_Light_HslRangeSet AppliHslRangeSet;

/******************************************************************************/
#endif   /* ENABLE_LIGHT_MODEL_SERVER_HSL */
/******************************************************************************/

/*******************************************************************************
End of the LIGHTING HSL MODEL variables
*******************************************************************************/


MOBLEUINT8 OptionalLightParam = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 
/**
* @brief  Appli_Light_Lightness_Set: This function is callback for Application
* when Light Lightness Set message is received
* @param  pLight_LightnessParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Set(Light_LightnessStatus_t* pLight_LightnessParam,
                                       MOBLEUINT8 OptionalValid)
{
  ApplilightnessSet.PresentState16 = pLight_LightnessParam->PresentValue16;
  
  if(pLight_LightnessParam->PresentValue16 != 0x00)
  {
    ApplilightnessSet.LastLightness16 = pLight_LightnessParam->PresentValue16;
  }    
  
  return MOBLE_RESULT_SUCCESS;
}
#endif


#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 
/**
* @brief  Appli_Light_Lightness_Linear_Set: This function is callback for Application
* when Light Lightness Linear Set message is received
* @param  pLight_LightnessLinearParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Linear_Set(Light_LightnessStatus_t* pLight_LightnessLinearParam,
                                              MOBLEUINT8 OptionalValid)
{
  ApplilightnessLinearSet.PresentState16 = pLight_LightnessLinearParam->PresentValue16;
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif
/******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
/**
* @brief  Appli_Light_Lightness_Default_Set: This function is callback for Application
* when Light Lightness Linear Set message is received
* @param  pLight_LightnessDefaultParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Default_Set(Light_LightnessDefaultParam_t* pLight_LightnessDefaultParam,
                                               MOBLEUINT8 OptionalValid)
{
  if(pLight_LightnessDefaultParam->LightnessDefaultStatus > 0)
  {
    BSP_LED_On(LED_BLUE);
  }
  else
  {
    BSP_LED_Off(LED_BLUE);
  }
  
  ApplilightnessDefaultSet.LightnessDefault = pLight_LightnessDefaultParam->LightnessDefaultStatus;
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif
/******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
/**
* @brief  Appli_Light_Lightness_Range_Set: This function is callback for Application
* when Light Lightness Linear Set message is received
* @param  pLight_LightnessRangeParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Range_Set(Light_LightnessRangeParam_t* pLight_LightnessRangeParam,
                                             MOBLEUINT8 OptionalValid)
{
  ApplilightnessRangeSet.StatusCode = pLight_LightnessRangeParam->StatusCode;
  ApplilightnessRangeSet.RangeMin = pLight_LightnessRangeParam->MinRangeStatus; 
  ApplilightnessRangeSet.RangeMax = pLight_LightnessRangeParam->MaxRangeStatus;
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif
/******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
/**
* @brief  Appli_Light_Ctl_Set: This function is callback for Application
* when Light Lightness Linear Set message is received
* @param  pLight_CtlParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Ctl_Set(Light_CtlStatus_t* pLight_CtlParam,
                                 MOBLEUINT8 OptionalValid)
{
  
  AppliCtlSet.PresentLightness16 = pLight_CtlParam->PresentCtlLightness16;
  AppliCtlSet.PresentTemperature16 = pLight_CtlParam->PresentCtlTemperature16;
  AppliCtlSet.PresentCtlDelta16 = pLight_CtlParam->PresentCtlDelta16;
  return MOBLE_RESULT_SUCCESS;
}
#endif


#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE 
/**
* @brief  Appli_Light_CtlTemperature_Set: This function is callback for Application
* when Light Lightness Linear Set message is received
* @param  pLight_CtltempParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Set(Light_CtlStatus_t* pLight_CtltempParam,
                                            MOBLEUINT8 OptionalValid)
{
  AppliCtlSet.PresentTemperature16 = pLight_CtltempParam->PresentCtlTemperature16;
  AppliCtlSet.PresentCtlDelta16 = pLight_CtltempParam->PresentCtlDelta16;
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif
/******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
/**
* @brief  Appli_Light_CtlTemperature_Range_Set: This function is callback for Application
* when Light Lightness Linear Set message is received
* @param  pLight_CtlTempRangeParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Range_Set(Light_CtlTemperatureRangeParam_t* pLight_CtlTempRangeParam,
                                                  MOBLEUINT8 OptionalValid)
{
  AppliCtlTemperatureRangeSet.RangeMin = pLight_CtlTempRangeParam->MinRangeStatus; 
  AppliCtlTemperatureRangeSet.RangeMax = pLight_CtlTempRangeParam->MaxRangeStatus;
  AppliCtlTemperatureRangeSet.StatusCode = pLight_CtlTempRangeParam->StatusCode;

  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif
/******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
/**
* @brief  Appli_Light_CtlDefault_Set: This function is callback for Application
*         when Light Lightness Linear Set message is received
* @param  pLight_CtlDefaultParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlDefault_Set(Light_CtlDefaultParam_t* pLight_CtlDefaultParam,
                                     MOBLEUINT8 OptionalValid)
{
  AppliCtlDefaultSet.CtlDefaultLightness16 = pLight_CtlDefaultParam->CtlDefaultLightness16; 
  AppliCtlDefaultSet.CtlDefaultTemperature16 = pLight_CtlDefaultParam->CtlDefaultTemperature16;
  AppliCtlDefaultSet.CtlDefaultDeltaUv = pLight_CtlDefaultParam->CtlDefaultDeltaUv;
  
  return MOBLE_RESULT_SUCCESS;
} 
#endif


#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
/**
* @brief  Appli_Light_Hsl_Set: This function is callback for Application
*         when Light Hsl Set message is received
* @param  pLight_HslParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Hsl_Set(Light_HslStatus_t* pLight_HslParam,
                                             MOBLEUINT8 OptionalValid)
{ 
  AppliHslSet.HslLightness16 = pLight_HslParam->PresentHslLightness16;
  AppliHslSet.HslHueLightness16 = pLight_HslParam->PresentHslHueLightness16;
  AppliHslSet.HslSaturation16 = pLight_HslParam->PresentHslSaturation16;
  
  return MOBLE_RESULT_SUCCESS;
} 
#endif


#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE
/**
* @brief  Appli_Light_HslHue_Set: This function is callback for Application
* when Light Hsl Hue Set message is received
* @param  pLight_HslHueParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslHue_Set(Light_HslStatus_t* pLight_HslHueParam,
                                             MOBLEUINT8 OptionalValid)
{
  AppliHslSet.HslHueLightness16 = pLight_HslHueParam->PresentHslHueLightness16; 
  
  return MOBLE_RESULT_SUCCESS;
} 
#endif


#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION
/**
* @brief  Appli_Light_HslSaturation_Set: This function is callback for Application
* when Light Hsl Saturation Set message is received
* @param  pLight_HslSaturationParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslSaturation_Set(Light_HslStatus_t* pLight_HslSaturationParam,
                                             MOBLEUINT8 OptionalValid)
{
  AppliHslSet.HslSaturation16 = pLight_HslSaturationParam->PresentHslSaturation16;
  
  return MOBLE_RESULT_SUCCESS;
} 
#endif
              

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
/**
* @brief  Appli_Light_HslDefault_Set: This function is callback for Application
* when Light Hsl Default Set message is received
* @param  pLight_HslDefaultParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslDefault_Set(Light_HslStatus_t* pLight_HslDefaultParam,
                                             MOBLEUINT8 OptionalValid)
{
  AppliHslSet.HslLightness16 = pLight_HslDefaultParam->PresentHslLightness16;
  AppliHslSet.HslHueLightness16 = pLight_HslDefaultParam->PresentHslHueLightness16;
  AppliHslSet.HslSaturation16 = pLight_HslDefaultParam->PresentHslSaturation16;
  
  return MOBLE_RESULT_SUCCESS;
} 
#endif 
              

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
/**
* @brief  Appli_Light_HslRange_Set: This function is callback for Application
*         when Light Hsl Range Set message is received
* @param  pLight_HslRangeParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslRange_Set(Light_HslRangeParam_t* pLight_HslRangeParam,
                                             MOBLEUINT8 OptionalValid)
{
   AppliHslRangeSet.HslHueMinRange16 = pLight_HslRangeParam->HslHueMinRange16;
   AppliHslRangeSet.HslHueMaxRange16 = pLight_HslRangeParam->HslHueMaxRange16;
   AppliHslRangeSet.HslMinSaturation16 = pLight_HslRangeParam->HslMinSaturation16;
   AppliHslRangeSet.HslMaxSaturation16 = pLight_HslRangeParam->HslMaxSaturation16;
   
   return MOBLE_RESULT_SUCCESS;
} 
#endif            


/*******************************************************************************
Following Functions are used for the LIGHTING Lightness MODEL 
*******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 

/**
* @brief  Appli_Light_GetLightnessStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lLightnessState: Pointer to the status message
* @retval MOBLE_RESULT
*/  
MOBLE_RESULT Appli_Light_GetLightnessStatus(MOBLEUINT8* lLightnessState)
{
  *(lLightnessState) = ApplilightnessSet.PresentState16;
  *(lLightnessState+1) = ApplilightnessSet.PresentState16 >> 8;
  *(lLightnessState+2) = ApplilightnessSet.LastLightness16 ;
  *(lLightnessState+3) = ApplilightnessSet.LastLightness16 >> 8;
    
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Light_GetLightnessLinearStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lLightnessState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetLightnessLinearStatus(MOBLEUINT8* lLightnessState)
{
  *(lLightnessState) = ApplilightnessLinearSet.PresentState16;
  *(lLightnessState+1) = ApplilightnessLinearSet.PresentState16 >> 8;
  *(lLightnessState+2) = ApplilightnessSet.LastLightness16 ;
  *(lLightnessState+3) = ApplilightnessSet.LastLightness16 >> 8;
    
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Light_GetLightnessDefaultStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lDefaultState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetLightnessDefaultStatus(MOBLEUINT8* lDefaultState)
{
  *(lDefaultState) = ApplilightnessDefaultSet.LightnessDefault;
  *(lDefaultState+1) = ApplilightnessDefaultSet.LightnessDefault >> 8;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Light_GetLightnessRangeStatus: This function is callback for Application
to get the application values in middleware used for transition change.
* @param  lRangeState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetLightnessRangeStatus(MOBLEUINT8* lRangeState)
{
  *(lRangeState) = ApplilightnessRangeSet.StatusCode;
  *(lRangeState+1) = ApplilightnessRangeSet.RangeMin;
  *(lRangeState+2) = ApplilightnessRangeSet.RangeMin >> 8;
  *(lRangeState+3) = ApplilightnessRangeSet.RangeMax;
  *(lRangeState+4) = ApplilightnessRangeSet.RangeMax >> 8;
  
  return MOBLE_RESULT_SUCCESS;
}

#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  */

/*******************************************************************************
Following Functions are used for the LIGHTING CTL MODEL 
*******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL

/**
* @brief  Appli_Light_GetCtlLightStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlLightState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlLightStatus(MOBLEUINT8* lCtlLightState)
{
  *(lCtlLightState) = AppliCtlSet.PresentLightness16;
  *(lCtlLightState+1) = AppliCtlSet.PresentLightness16 >> 8;
  *(lCtlLightState+2) = AppliCtlSet.PresentTemperature16;
  *(lCtlLightState+3) = AppliCtlSet.PresentTemperature16 >>8;
    
  return MOBLE_RESULT_SUCCESS;
}  


/**
* @brief  Appli_Light_GetCtlTeperatureStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlTempState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlTemperatureStatus(MOBLEUINT8* lCtlTempState)
{
    *(lCtlTempState) = AppliCtlSet.PresentTemperature16;
    *(lCtlTempState+1) = AppliCtlSet.PresentTemperature16 >> 8;
    *(lCtlTempState+2) = AppliCtlSet.PresentCtlDelta16;
    *(lCtlTempState+3) = AppliCtlSet.PresentCtlDelta16 >>8;
    
  return MOBLE_RESULT_SUCCESS;
}  


/**
* @brief  Appli_Light_GetCtlTemperatureRange: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlTempRange: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlTemperatureRange(MOBLEUINT8* lCtlTempRange)
{   
  *(lCtlTempRange) = AppliCtlTemperatureRangeSet.StatusCode;
  *(lCtlTempRange+1) = AppliCtlTemperatureRangeSet.RangeMin;
  *(lCtlTempRange+2) = AppliCtlTemperatureRangeSet.RangeMin >> 8;
  *(lCtlTempRange+3) = AppliCtlTemperatureRangeSet.RangeMax;
  *(lCtlTempRange+4) = AppliCtlTemperatureRangeSet.RangeMax >>8;
    
  return MOBLE_RESULT_SUCCESS;
}  


/**
* @brief  Appli_Light_GetCtlDefaultStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlDefaultState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlDefaultStatus(MOBLEUINT8* lCtlDefaultState)
{
  *(lCtlDefaultState) = AppliCtlDefaultSet.CtlDefaultLightness16;
  *(lCtlDefaultState+1) = AppliCtlDefaultSet.CtlDefaultLightness16 >> 8;
  *(lCtlDefaultState+2) = AppliCtlDefaultSet.CtlDefaultTemperature16;
  *(lCtlDefaultState+3) = AppliCtlDefaultSet.CtlDefaultTemperature16 >>8;
  *(lCtlDefaultState+4) = AppliCtlDefaultSet.CtlDefaultDeltaUv;
  *(lCtlDefaultState+5) = AppliCtlDefaultSet.CtlDefaultDeltaUv >>8;

  return MOBLE_RESULT_SUCCESS;
}  

#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL  */

/*******************************************************************************
Following Functions are used for the LIGHTING HSL MODEL 
*******************************************************************************/

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
/**
* @brief  Appli_Light_GetHslStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslStatus(MOBLEUINT8* lHslState)
{
    *(lHslState) = AppliHslSet.HslLightness16;
    *(lHslState+1) = AppliHslSet.HslLightness16 >> 8;
    *(lHslState+2) = AppliHslSet.HslHueLightness16;
    *(lHslState+3) = AppliHslSet.HslHueLightness16 >>8;
    *(lHslState+4) = AppliHslSet.HslSaturation16;
    *(lHslState+5) = AppliHslSet.HslSaturation16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}  
  


/**
* @brief  Appli_Light_GetHslHueStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslHueState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslHueStatus(MOBLEUINT8* lHslHueState)
{
   
    *(lHslHueState) = AppliHslSet.HslHueLightness16;
    *(lHslHueState+1) = AppliHslSet.HslHueLightness16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}    


/**
* @brief  Appli_Light_GetHslSaturationStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslSaturationState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslSaturationStatus(MOBLEUINT8* lHslSaturationState)
{
   
    *(lHslSaturationState) = AppliHslSet.HslSaturation16;
    *(lHslSaturationState+1) = AppliHslSet.HslSaturation16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}  


/**
* @brief  Appli_Light_GetHslHueRange: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslHueRange: Pointer to the status message
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_GetHslHueRange(MOBLEUINT8* lHslHueRange)
{   
    *(lHslHueRange) = AppliHslRangeSet.StatusCode;
    *(lHslHueRange+1) = AppliHslRangeSet.HslHueMinRange16;
    *(lHslHueRange+2) = AppliHslRangeSet.HslHueMinRange16 >> 8;
    *(lHslHueRange+3) = AppliHslRangeSet.HslHueMaxRange16;
    *(lHslHueRange+4) = AppliHslRangeSet.HslHueMaxRange16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}  


/**
* @brief  Appli_Light_GetHslSatRange: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslSatRange: Pointer to the status message
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_GetHslSatRange(MOBLEUINT8* lHslSatRange)
{   
    *(lHslSatRange) = AppliHslRangeSet.StatusCode;
    *(lHslSatRange+1) = AppliHslRangeSet.HslMinSaturation16;
    *(lHslSatRange+2) = AppliHslRangeSet.HslMinSaturation16 >> 8;
    *(lHslSatRange+3) = AppliHslRangeSet.HslMaxSaturation16;
    *(lHslSatRange+4) = AppliHslRangeSet.HslMaxSaturation16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}  

/******************************************************************************/
#endif    /* ENABLE_LIGHT_MODEL_SERVER_HSL */
/******************************************************************************/





///**
// * @}
// */
//
///**
// * @}
// */
//
///******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
//
