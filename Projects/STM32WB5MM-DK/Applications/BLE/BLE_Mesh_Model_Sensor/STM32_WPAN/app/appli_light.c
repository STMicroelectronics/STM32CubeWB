/**
  ******************************************************************************
  * @file    appli_light.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Lighting Mesh Models 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "appli_nvm.h"
#include "math.h"

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
aPwmLedGsData_TypeDef aPwmLedGsData_light = {PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_0_7};

/*******************************************************************************
Following Variables are used for the LIGHTING Lightness MODEL 
*******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
/******************************************************************************/
Appli_Light_lightnessSet ApplilightnessSet[APPLICATION_NUMBER_OF_ELEMENTS] = {{1,0,0,0,0,0}};
Appli_Light_lightnessLinearSet ApplilightnessLinearSet[APPLICATION_NUMBER_OF_ELEMENTS];

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  */
/******************************************************************************/

/*******************************************************************************
Following Variables are used for the LIGHTING CTL MODEL 
*******************************************************************************/

/******************************************************************************/
#if defined (ENABLE_LIGHT_MODEL_SERVER_CTL) || defined (ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP) \
       || defined (ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE)
/******************************************************************************/
  Appli_Light_CtlSet AppliCtlSet[APPLICATION_NUMBER_OF_ELEMENTS];
  Appli_Light_CtlTemperatureRangeSet AppliCtlTemperatureRangeSet[APPLICATION_NUMBER_OF_ELEMENTS];
  Appli_Light_CtlDefaultSet AppliCtlDefaultSet[APPLICATION_NUMBER_OF_ELEMENTS];

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL */
/******************************************************************************/

/*******************************************************************************
Following Variables are used for the LIGHTING HSL MODEL 
*******************************************************************************/

#if defined (ENABLE_LIGHT_MODEL_SERVER_HSL) || defined (ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP) \
     || defined (ENABLE_LIGHT_MODEL_SERVER_HSL_HUE) || defined (ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION)
/******************************************************************************/       
  Appli_Light_HslSet AppliHslSet[APPLICATION_NUMBER_OF_ELEMENTS];
  Appli_Light_HslDefaultSet Appli_HslDefaultSet[APPLICATION_NUMBER_OF_ELEMENTS] = {{0x7FFF,0x7FFF,0x7FFF}};
  Appli_Light_RGBSet Appli_RGBParam[APPLICATION_NUMBER_OF_ELEMENTS];
  Appli_Light_HslRangeSet AppliHslRangeSet[APPLICATION_NUMBER_OF_ELEMENTS];

/*******************************************************************************/
#endif  /*End of the LIGHTING HSL MODEL variables */
/*******************************************************************************/

/*******************************************************************************
Following Variables are used for the RGB board. 
*******************************************************************************/
  
Appli_LightPwmValue_t Appli_LightPwmValue[APPLICATION_NUMBER_OF_ELEMENTS];
extern MOBLEUINT8 RestoreFlag;
extern MOBLEUINT8 PowerOnOff_flag;
extern MOBLEUINT8 IntensityFlag;
extern MOBLEUINT16 IntensityValue;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 
/******************************************************************************/

/**
* @brief  Appli_Light_Lightness_Set: This function is callback for Application
* when Light Lightness Set message is received
* @param  pLight_LightnessParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Set(Light_LightnessStatus_t* pLight_LightnessParam,
                                       MOBLEUINT8 OptionalValid,
                                       MOBLEUINT16 dstPeer,
                                       MOBLEUINT8 elementIndex)
{

  static MOBLEUINT16 previousIntensity = 0;
  
  TRACE_M(TF_SERIAL_CTRL, "#824C!\n\r");
  TRACE_M(TF_LIGHT, "Element index %d present 0x%.2x last 0x%.2x\r\n", 
          elementIndex,
          pLight_LightnessParam->LightnessPresentValue16,
          pLight_LightnessParam->LightnessLastStatus);  
  
  ApplilightnessSet[elementIndex].PresentState16 = pLight_LightnessParam->LightnessPresentValue16;
  ApplilightnessSet[elementIndex].LastLightness16 = pLight_LightnessParam->LightnessLastStatus;

  if(pLight_LightnessParam->LightnessPresentValue16 != 0x00)
  {
    ApplilightnessSet[elementIndex].LastLightness16 = pLight_LightnessParam->LightnessPresentValue16;
  }

  if(((IntensityValue > previousIntensity) && (IntensityValue <PWM_TIME_PERIOD)) ||
     IntensityValue == INTENSITY_LEVEL_ZERO)
  {
    IntensityFlag = MOBLE_FALSE;
  }
  else
  {
    IntensityFlag = MOBLE_TRUE;
  }
  previousIntensity = IntensityValue; 

  //Manage Default Color
  if((Appli_LightPwmValue[elementIndex].PwmRedValue  == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmGreenValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmBlueValue  == 0))
  {
	Appli_LightPwmValue[elementIndex].PwmBlueValue=0xFF;
  }
  
  //Manage intensity
  Appli_LightPwmValue[elementIndex].IntensityValue = ApplilightnessSet[elementIndex].PresentState16;
  TRACE_M(TF_LIGHT, "IntensityValue= 0x%.4x \r\n", Appli_LightPwmValue[elementIndex].IntensityValue); 
  
  aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  LED_On(aPwmLedGsData_light);

  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_LIGHTNESS_NVM_FLAG, elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
  
}

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 
/******************************************************************************/
                                          
/**
* @brief  Appli_Light_Lightness_Linear_Set: This function is callback for Application
* when Light Lightness Linear Set message is received
* @param  pLight_LightnessLinearParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Linear_Set(Light_LightnessStatus_t* pLight_LightnessLinearParam,
                                              MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  ApplilightnessLinearSet[elementIndex].PresentState16 = pLight_LightnessLinearParam->LinearPresentValue16; 
  
  TRACE_M(TF_LIGHT,"Light_LightnessLinear_Set callback received for elementIndex %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8250!\n\r");
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
/******************************************************************************/

/**
* @brief  Appli_Light_Lightness_Default_Set: This function is callback for Application
*         when Light Lightness Default Set message is received
* @param  pLight_LightnessDefaultParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Default_Set(Light_LightnessDefaultParam_t* pLight_LightnessDefaultParam,
                                               MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  //Manage Default Color
  if((Appli_LightPwmValue[elementIndex].PwmRedValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmGreenValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmBlueValue == 0))
  {
	Appli_LightPwmValue[elementIndex].PwmBlueValue=0xFF;	  
  }
  
  //Manage Default intensity
  if(Appli_LightPwmValue[elementIndex].IntensityValue == 0)
  {
    /* Intensity of 25% by default */
    Appli_LightPwmValue[elementIndex].IntensityValue=0x2000;
  }
  
  if(pLight_LightnessDefaultParam->LightnessDefaultStatus > 0)
  {
        aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue   * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
        aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
        aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue  * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
        LED_On(aPwmLedGsData_light);
  }
  
  else
  {
    LED_Off();
  }
  
  ApplilightnessSet[elementIndex].LightnessDefault = pLight_LightnessDefaultParam->LightnessDefaultStatus;
  
  TRACE_M(TF_SERIAL_CTRL,"#8259! \r\n");
  TRACE_M(TF_LIGHT,"Appli_Light_Lightness_Default_Set callback received for elementIndex %d \r\n", elementIndex);
  
  NvmStatePowerFlag_Set(LIGHT_LIGHTNESS_NVM_FLAG,elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_Lightness_Last_Set: This function is callback for Application
*         when Light Lightness Default Set message is received
* @param  pLight_LightnessDefaultParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Last_Set(Light_LightnessStatus_t* pLight_LightnessParam,                                             
                                              MOBLEUINT8 elementIndex)
{
  ApplilightnessSet[elementIndex].LastLightness16 = pLight_LightnessParam->LightnessLastStatus;
  
  TRACE_M(TF_SERIAL_CTRL,"#8259!\n\r");
  TRACE_M(TF_LIGHT,
          "Appli_Light_Lightness_Last_Set Light Last Value 0x%.2x \r\n", 
          ApplilightnessSet[elementIndex].LastLightness16);
   
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP */
/******************************************************************************/


/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
/******************************************************************************/

/**
* @brief  Appli_Light_Lightness_Range_Set: This function is callback for Application
*         when Light Lightness Range Set message is received
* @param  pLight_LightnessRangeParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Range_Set(Light_LightnessRangeParam_t* pLight_LightnessRangeParam,
                                             MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  ApplilightnessSet[elementIndex].StatusCode = pLight_LightnessRangeParam->StatusCode;
  ApplilightnessSet[elementIndex].RangeMin = pLight_LightnessRangeParam->MinRangeStatus; 
  ApplilightnessSet[elementIndex].RangeMax = pLight_LightnessRangeParam->MaxRangeStatus;
  
  TRACE_M(TF_SERIAL_CTRL,"#825B! \n\r");
  TRACE_M(TF_LIGHT,"Appli_Light_Lightness_Range_Set callback received for elementIndex %d \r\n", elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
/******************************************************************************/

/**
* @brief  Appli_Light_Ctl_Set: This function is callback for Application
*         when Light Ctl Set message is received
* @param  pLight_CtlParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Ctl_Set(Light_CtlStatus_t* pLight_CtlParam,
                                 MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  float colourRatio;
  float brightRatio;
  
  TRACE_M(TF_SERIAL_CTRL,"#825E! \n\r");
  
  AppliCtlSet[elementIndex].PresentLightness16  = pLight_CtlParam->PresentCtlLightness16;
  AppliCtlSet[elementIndex].PresentTemperature16 = pLight_CtlParam->PresentCtlTemperature16;
  AppliCtlSet[elementIndex].PresentCtlDelta16 = pLight_CtlParam->PresentCtlDelta16;
  
  /* Save target parameters if transition is available */
  AppliCtlSet[elementIndex].TargetLightness16  = pLight_CtlParam->TargetCtlLightness16;
  AppliCtlSet[elementIndex].TargetTemperature16 = pLight_CtlParam->TargetCtlTemperature16;
  AppliCtlSet[elementIndex].TargetCtlDelta16 = pLight_CtlParam->TargetCtlDeltaUv16;
  
  colourRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentTemperature16 ,MAX_CTL_TEMP_RANGE ,MIN_CTL_TEMP_RANGE);
  
  brightRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentLightness16 ,0XFFFF ,0);
  
  Appli_LightPwmValue[elementIndex].PwmCoolValue = PWM_CoolValue(colourRatio , brightRatio); 
  Appli_LightPwmValue[elementIndex].PwmWarmValue = PWM_WarmValue(colourRatio , brightRatio);
  
  //Manage Default Color
  if((Appli_LightPwmValue[elementIndex].PwmRedValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmGreenValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmBlueValue == 0))
  {
    Appli_LightPwmValue[elementIndex].PwmBlueValue=0xFF;	  
  }
  
  //Manage Default intensity
  if(Appli_LightPwmValue[elementIndex].IntensityValue == 0)
  {
    /* Intensity of 25% by default */
    Appli_LightPwmValue[elementIndex].IntensityValue=0x2000;
  }
  
  //Apply Temperature Ratio to colors
  Appli_LightPwmValue[elementIndex].PwmRedValue = (0xFF * (MOBLEUINT16) (colourRatio*100))/100; 
  Appli_LightPwmValue[elementIndex].PwmBlueValue = (0xFF * (MOBLEUINT16) ((1-colourRatio)*100))/100;

  aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  LED_On(aPwmLedGsData_light);

  NvmStatePowerFlag_Set(LIGHT_CTL_NVM_FLAG, elementIndex);
    
  TRACE_M(TF_LIGHT,"Appli_Light_Ctl_Set callback received for elementIndex %d \r\n", elementIndex); 
	                              
    
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE 
/******************************************************************************/

/**
* @brief  Appli_Light_CtlTemperature_Set: This function is callback for Application
*         when Light Ctl Temperature Set message is received
* @param  pLight_CtltempParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Set(Light_CtlStatus_t* pLight_CtltempParam,
                                            MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  float colourRatio;
  float brightRatio;
  TRACE_M(TF_SERIAL_CTRL,"#8264!\n\r");
  
  AppliCtlSet[elementIndex].PresentTemperature16 = pLight_CtltempParam->PresentCtlTemperature16;
  AppliCtlSet[elementIndex].PresentCtlDelta16 = pLight_CtltempParam->PresentCtlDelta16;
  
  /* Made a copy of target in case of transition values change */
  AppliCtlSet[elementIndex].TargetTemperature16 = pLight_CtltempParam->TargetCtlTemperature16;
  AppliCtlSet[elementIndex].TargetCtlDelta16 = pLight_CtltempParam->TargetCtlDeltaUv16;
  
  colourRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentTemperature16 ,MAX_CTL_TEMP_RANGE ,MIN_CTL_TEMP_RANGE);
  
  brightRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentLightness16 ,0XFFFF ,0);
  
  Appli_LightPwmValue[elementIndex].PwmCoolValue = PWM_CoolValue(colourRatio , brightRatio); 
  Appli_LightPwmValue[elementIndex].PwmWarmValue = PWM_WarmValue(colourRatio , brightRatio);

  //Manage Default Color
  if((Appli_LightPwmValue[elementIndex].PwmRedValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmGreenValue == 0) &&
       (Appli_LightPwmValue[elementIndex].PwmBlueValue == 0))
  {
    Appli_LightPwmValue[elementIndex].PwmBlueValue=0xFF;	  
  }
  
  //Manage Default intensity
  if(Appli_LightPwmValue[elementIndex].IntensityValue == 0)
  {
    /* Intensity of 25% by default */
    Appli_LightPwmValue[elementIndex].IntensityValue=0x2000;
  }
  
  //Apply Temperature Ratio to colors
  Appli_LightPwmValue[elementIndex].PwmRedValue = (0xFF * (MOBLEUINT16) (colourRatio*100))/100; 
  Appli_LightPwmValue[elementIndex].PwmBlueValue = (0xFF * (MOBLEUINT16) ((1-colourRatio)*100))/100;

  aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  LED_On(aPwmLedGsData_light);
  
  NvmStatePowerFlag_Set(LIGHT_CTL_NVM_FLAG, elementIndex);
    
  TRACE_M(TF_LIGHT,"Appli_Light_CtlTemperature_Set callback received for elementIndex %d \r\n", elementIndex); 
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
/******************************************************************************/

/**
* @brief  Appli_Light_CtlTemperature_Range_Set: This function is callback for Application
*         when Light Ctl Temperature range Set message is received
* @param  pLight_CtlTempRangeParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Range_Set(Light_CtlTemperatureRangeParam_t* pLight_CtlTempRangeParam,
                                                  MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  AppliCtlTemperatureRangeSet[elementIndex].RangeMin = pLight_CtlTempRangeParam->MinRangeStatus; 
  AppliCtlTemperatureRangeSet[elementIndex].RangeMax = pLight_CtlTempRangeParam->MaxRangeStatus;
  AppliCtlTemperatureRangeSet[elementIndex].StatusCode = pLight_CtlTempRangeParam->StatusCode;

  TRACE_M(TF_SERIAL_CTRL,"#826B!\n\r");
  TRACE_M(TF_LIGHT,"Light_CtlTemperature_Range_Set callback received for elementIndex %d \r\n", elementIndex);
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP
/******************************************************************************/

/**
* @brief  Appli_Light_CtlDefault_Set: This function is callback for Application
*         when Light Ctl Default Set message is received
* @param  pLight_CtlDefaultParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlDefault_Set(Light_CtlDefaultParam_t* pLight_CtlDefaultParam,
                                        MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  AppliCtlDefaultSet[elementIndex].CtlDefaultLightness16 = pLight_CtlDefaultParam->CtlDefaultLightness16; 
  AppliCtlDefaultSet[elementIndex].CtlDefaultTemperature16 = pLight_CtlDefaultParam->CtlDefaultTemperature16;
  AppliCtlDefaultSet[elementIndex].CtlDefaultDeltaUv = pLight_CtlDefaultParam->CtlDefaultDeltaUv;
  
  TRACE_M(TF_SERIAL_CTRL,"#8269!\n\r");
  TRACE_M(TF_LIGHT,"Light_CtlDefault_Set callback received for elementIndex %d \r\n", elementIndex);
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif   /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
/******************************************************************************/

/**
* @brief  Appli_Light_Hsl_Set: This function is callback for Application
*         when Light Hsl Set message is received
* @param  pLight_HslParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Hsl_Set(Light_HslStatus_t* pLight_HslParam,
                                 MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{ 
  TRACE_M(TF_SERIAL_CTRL,"#8276!\n\r");
  
  AppliHslSet[elementIndex].HslLightness16 = pLight_HslParam->PresentHslLightness16;
  AppliHslSet[elementIndex].HslHueLightness16 = pLight_HslParam->PresentHslHueLightness16;
  AppliHslSet[elementIndex].HslSaturation16 = pLight_HslParam->PresentHslSaturation16;
  
  /* Save target parameters if transition is available */
  AppliHslSet[elementIndex].TargetHslLightness16  = pLight_HslParam->TargetHslLightness16;
  AppliHslSet[elementIndex].TargetHslHueLightness16 = pLight_HslParam->TargetHslHueLightness16;
  AppliHslSet[elementIndex].TargetHslSaturation16 = pLight_HslParam->TargetHslSaturation16;
   
  /* Function to convert HSL values in RGB values */
  HSL2RGB_Conversion(elementIndex);
  
  //Manage Default intensity
  if(Appli_LightPwmValue[elementIndex].IntensityValue == 0)
  {
    /* Intensity of 25% by default */
    Appli_LightPwmValue[elementIndex].IntensityValue=0x2000;
  }
  
  Appli_LightPwmValue[elementIndex].PwmRedValue   = (Appli_RGBParam[elementIndex].Red_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmGreenValue = (Appli_RGBParam[elementIndex].Green_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmBlueValue  = (Appli_RGBParam[elementIndex].Blue_Value*0xFF)/0xFFFF;

  /** For PIR Sensor demonstration the light should not be switched on when HSL is set **/
  TRACE_M(TF_SERIAL_CTRL,"Red Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmRedValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Green Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmGreenValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Blue Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmBlueValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  
  aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  LED_On(aPwmLedGsData_light);
  
  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  TRACE_M(TF_LIGHT,"Appli_Light_Hsl_Set callback received for elementIndex %d \r\n", elementIndex); 
    
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif     /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE
/******************************************************************************/

/**
* @brief  Appli_Light_HslHue_Set: This function is callback for Application
* when Light Hsl Hue Set message is received
* @param  pLight_HslHueParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslHue_Set(Light_HslStatus_t* pLight_HslHueParam,
                                    MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SERIAL_CTRL, "#826F! \n\r");
  
  AppliHslSet[elementIndex].HslHueLightness16 = pLight_HslHueParam->PresentHslHueLightness16; 
  
  HSL2RGB_Conversion(elementIndex);
  
  //Manage Default intensity
  if(Appli_LightPwmValue[elementIndex].IntensityValue == 0)
  {
    /* Intensity of 25% by default */
    Appli_LightPwmValue[elementIndex].IntensityValue=0x2000;
  }
  

  Appli_LightPwmValue[elementIndex].PwmRedValue   = (Appli_RGBParam[elementIndex].Red_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmGreenValue = (Appli_RGBParam[elementIndex].Green_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmBlueValue  = (Appli_RGBParam[elementIndex].Blue_Value*0xFF)/0xFFFF;

  /** For PIR Sensor demonstration the light should not be switched on when HSL is set **/
  TRACE_M(TF_SERIAL_CTRL,"Red Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmRedValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Green Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmGreenValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Blue Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmBlueValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  
  aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;

  LED_On(aPwmLedGsData_light);

  
  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  TRACE_M(TF_LIGHT,"Appli_Light_HslHue_Set callback received for elementIndex %d \r\n", elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
   
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION
/******************************************************************************/

/**
* @brief  Appli_Light_HslSaturation_Set: This function is callback for Application
* when Light Hsl Saturation Set message is received
* @param  pLight_HslSaturationParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslSaturation_Set(Light_HslStatus_t* pLight_HslSaturationParam,
                                           MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SERIAL_CTRL,"#8273! \n\r");
  
  AppliHslSet[elementIndex].HslSaturation16 = pLight_HslSaturationParam->PresentHslSaturation16;
  
  HSL2RGB_Conversion(elementIndex);
  
  //Manage Default intensity
  if(Appli_LightPwmValue[elementIndex].IntensityValue == 0)
  {
    /* Intensity of 25% by default */
    Appli_LightPwmValue[elementIndex].IntensityValue=0x2000;
  }
  
  Appli_LightPwmValue[elementIndex].PwmRedValue   = (Appli_RGBParam[elementIndex].Red_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmGreenValue = (Appli_RGBParam[elementIndex].Green_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmBlueValue  = (Appli_RGBParam[elementIndex].Blue_Value*0xFF)/0xFFFF;

  /** For PIR Sensor demonstration the light should not be switched on when HSL is set **/
  TRACE_M(TF_SERIAL_CTRL,"Red Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmRedValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Green Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmGreenValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Blue Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmBlueValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  
  aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  LED_On(aPwmLedGsData_light);
  
  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  TRACE_M(TF_LIGHT,"Appli_Light_HslSaturation_Set callback received for elementIndex %d \r\n", elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif   /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
/******************************************************************************/

/**
* @brief  Appli_Light_HslDefault_Set: This function is callback for Application
* when Light Hsl Default Set message is received
* @param  pLight_HslDefaultParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslDefault_Set(Light_HslDefaultParam_t* pLight_HslDefaultParam,
                                        MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SERIAL_CTRL,"#827F! \n\r");
  
  Appli_HslDefaultSet[elementIndex].HslDefaultLightness16 = pLight_HslDefaultParam->HslLightnessDefault16;
  Appli_HslDefaultSet[elementIndex].HslDefaultHueLightness16 = pLight_HslDefaultParam->HslHueDefault16;
  Appli_HslDefaultSet[elementIndex].HslDefaultSaturation16 = pLight_HslDefaultParam->HslSaturationDefault16;
  
  AppliHslSet[elementIndex].HslLightness16 = (Appli_HslDefaultSet[elementIndex].HslDefaultLightness16;
  AppliHslSet[elementIndex].HslHueLightness16 = (Appli_HslDefaultSet[elementIndex].HslDefaultHueLightness16;
  AppliHslSet[elementIndex].HslSaturation16 = (Appli_HslDefaultSet[elementIndex].HslDefaultSaturation16;
  
  HSL2RGB_Conversion(elementIndex);
  
  //Manage Default intensity
  if(Appli_LightPwmValue[elementIndex].IntensityValue == 0)
  {
    /* Intensity of 25% by default */
    Appli_LightPwmValue[elementIndex].IntensityValue=0x2000;
  }
  
  Appli_LightPwmValue[elementIndex].PwmRedValue   = (Appli_RGBParam[elementIndex].Red_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmGreenValue = (Appli_RGBParam[elementIndex].Green_Value*0xFF)/0xFFFF;
  Appli_LightPwmValue[elementIndex].PwmBlueValue  = (Appli_RGBParam[elementIndex].Blue_Value*0xFF)/0xFFFF;
  
  /** For PIR Sensor demonstration the light should not be switched on when HSL is set **/
  TRACE_M(TF_SERIAL_CTRL,"Red Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmRedValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Green Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmGreenValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  TRACE_M(TF_SERIAL_CTRL,"Blue Value: (%04hx * %04hx)/0xFFFF = %04hx \n\r",
          Appli_LightPwmValue[elementIndex].PwmBlueValue,
          Appli_LightPwmValue[elementIndex].IntensityValue,
          (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF);
  
  aPwmLedGsData_light[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  aPwmLedGsData_light[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
  LED_On(aPwmLedGsData_light);

  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  TRACE_M(TF_LIGHT,"Appli_Light_HslDefault_Set callback received for elementIndex %d \r\n", elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP */
/******************************************************************************/
              
/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP
/******************************************************************************/

/**
* @brief  Appli_Light_HslRange_Set: This function is callback for Application
*         when Light Hsl Range Set message is received
* @param  pLight_HslRangeParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_HslRange_Set(Light_HslRangeParam_t* pLight_HslRangeParam,
                                      MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SERIAL_CTRL,"#8281! \n\r");
  
  AppliHslRangeSet[elementIndex].HslHueMinRange16 = pLight_HslRangeParam->HslHueMinRange16;
  AppliHslRangeSet[elementIndex].HslHueMaxRange16 = pLight_HslRangeParam->HslHueMaxRange16;
  AppliHslRangeSet[elementIndex].HslMinSaturation16 = pLight_HslRangeParam->HslMinSaturation16;
  AppliHslRangeSet[elementIndex].HslMaxSaturation16 = pLight_HslRangeParam->HslMaxSaturation16;
   
  TRACE_M(TF_LIGHT,"Appli_Light_HslRange_Set callback received for elementIndex %d \r\n", elementIndex);
   
   return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif               /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP */
/******************************************************************************/


/*******************************************************************************
Following Functions are used for the LIGHTING Lightness MODEL 
*******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS 
/******************************************************************************/

/**
* @brief  Appli_Light_GetLightnessStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lLightnessState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/  
MOBLE_RESULT Appli_Light_GetLightnessStatus(MOBLEUINT8* lLightnessState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lLightnessState) = ApplilightnessSet[elementIndex].PresentState16;
  *(lLightnessState+1) = ApplilightnessSet[elementIndex].PresentState16 >> 8;
  *(lLightnessState+2) = ApplilightnessSet[elementIndex].LastLightness16 ;
  *(lLightnessState+3) = ApplilightnessSet[elementIndex].LastLightness16 >> 8;
    
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Light_GetLightnessLinearStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lLightnessState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetLightnessLinearStatus(MOBLEUINT8* lLightnessState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lLightnessState) = ApplilightnessLinearSet[elementIndex].PresentState16;
  *(lLightnessState+1) = ApplilightnessLinearSet[elementIndex].PresentState16 >> 8;
  *(lLightnessState+2) = ApplilightnessSet[elementIndex].LastLightness16 ;
  *(lLightnessState+3) = ApplilightnessSet[elementIndex].LastLightness16 >> 8;
    
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Light_GetLightnessDefaultStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lDefaultState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetLightnessDefaultStatus(MOBLEUINT8* lDefaultState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lDefaultState) = ApplilightnessSet[elementIndex].LightnessDefault;
  *(lDefaultState+1) = ApplilightnessSet[elementIndex].LightnessDefault >> 8;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Light_GetLightnessLastStatus: This function is callback for Application
*         to get the application values in middleware used for transition change.
* @param  lLastState: Pointer to the status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetLightnessLastStatus(MOBLEUINT8* lLastState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lLastState) = ApplilightnessSet[elementIndex].LastLightness16;
  *(lLastState+1) = ApplilightnessSet[elementIndex].LastLightness16 >> 8;

  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_GetLightnessRangeStatus: This function is callback for Application
*         to get the application values in middleware used for transition change.
* @param  lRangeState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetLightnessRangeStatus(MOBLEUINT8* lRangeState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lRangeState) = ApplilightnessSet[elementIndex].StatusCode;
  *(lRangeState+1) = ApplilightnessSet[elementIndex].RangeMin;
  *(lRangeState+2) = ApplilightnessSet[elementIndex].RangeMin >> 8;
  *(lRangeState+3) = ApplilightnessSet[elementIndex].RangeMax;
  *(lRangeState+4) = ApplilightnessSet[elementIndex].RangeMax >> 8;
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  */
/******************************************************************************/

/*******************************************************************************
Following Functions are used for the LIGHTING CTL MODEL 
*******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
/******************************************************************************/

/**
* @brief  Appli_Light_GetCtlLightStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlLightState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlLightStatus(MOBLEUINT8* lCtlLightState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lCtlLightState) = AppliCtlSet[elementIndex].PresentLightness16;
  *(lCtlLightState+1) = AppliCtlSet[elementIndex].PresentLightness16 >> 8;
  *(lCtlLightState+2) = AppliCtlSet[elementIndex].PresentTemperature16;
  *(lCtlLightState+3) = AppliCtlSet[elementIndex].PresentTemperature16 >>8;
  *(lCtlLightState+4) = AppliCtlSet[elementIndex].PresentCtlDelta16;
  *(lCtlLightState+5) = AppliCtlSet[elementIndex].PresentCtlDelta16 >>8;
    
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Appli_Light_GetCtlTargetStatus: This function is callback for Application
*         to get the application values in middleware used for target state.
* @param  lCtlLightState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlTargetStatus(MOBLEUINT8* lCtlLightState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lCtlLightState) = AppliCtlSet[elementIndex].TargetLightness16;
  *(lCtlLightState+1) = AppliCtlSet[elementIndex].TargetLightness16 >> 8;
  *(lCtlLightState+2) = AppliCtlSet[elementIndex].TargetTemperature16;
  *(lCtlLightState+3) = AppliCtlSet[elementIndex].TargetTemperature16 >>8;
  *(lCtlLightState+4) = AppliCtlSet[elementIndex].TargetCtlDelta16;
  *(lCtlLightState+5) = AppliCtlSet[elementIndex].TargetCtlDelta16 >>8;

  return MOBLE_RESULT_SUCCESS;
}  
/**
* @brief  Appli_Light_GetCtlTeperatureStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlTempState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlTemperatureStatus(MOBLEUINT8* lCtlTempState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lCtlTempState) = AppliCtlSet[elementIndex].PresentTemperature16;
  *(lCtlTempState+1) = AppliCtlSet[elementIndex].PresentTemperature16 >> 8;
  *(lCtlTempState+2) = AppliCtlSet[elementIndex].PresentCtlDelta16;
  *(lCtlTempState+3) = AppliCtlSet[elementIndex].PresentCtlDelta16 >>8;
    
  return MOBLE_RESULT_SUCCESS;
}  


/**
* @brief  Appli_Light_GetCtlTemperatureRange: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlTempRange: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlTemperatureRange(MOBLEUINT8* lCtlTempRange, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{   
  *(lCtlTempRange) = AppliCtlTemperatureRangeSet[elementIndex].StatusCode;
  *(lCtlTempRange+1) = AppliCtlTemperatureRangeSet[elementIndex].RangeMin;
  *(lCtlTempRange+2) = AppliCtlTemperatureRangeSet[elementIndex].RangeMin >> 8;
  *(lCtlTempRange+3) = AppliCtlTemperatureRangeSet[elementIndex].RangeMax;
  *(lCtlTempRange+4) = AppliCtlTemperatureRangeSet[elementIndex].RangeMax >>8;
    
  return MOBLE_RESULT_SUCCESS;
}  


/**
* @brief  Appli_Light_GetCtlDefaultStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lCtlDefaultState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetCtlDefaultStatus(MOBLEUINT8* lCtlDefaultState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lCtlDefaultState) = AppliCtlDefaultSet[elementIndex].CtlDefaultLightness16;
  *(lCtlDefaultState+1) = AppliCtlDefaultSet[elementIndex].CtlDefaultLightness16 >> 8;
  *(lCtlDefaultState+2) = AppliCtlDefaultSet[elementIndex].CtlDefaultTemperature16;
  *(lCtlDefaultState+3) = AppliCtlDefaultSet[elementIndex].CtlDefaultTemperature16 >>8;
  *(lCtlDefaultState+4) = AppliCtlDefaultSet[elementIndex].CtlDefaultDeltaUv;
  *(lCtlDefaultState+5) = AppliCtlDefaultSet[elementIndex].CtlDefaultDeltaUv >> 8;

  return MOBLE_RESULT_SUCCESS;
}  

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL  */
/******************************************************************************/

/*******************************************************************************
Following Functions are used for the LIGHTING HSL MODEL 
*******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
/******************************************************************************/

/**
* @brief  Appli_Light_GetHslStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslStatus(MOBLEUINT8* lHslState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lHslState) = AppliHslSet[elementIndex].HslLightness16;
  *(lHslState+1) = AppliHslSet[elementIndex].HslLightness16 >> 8;
  *(lHslState+2) = AppliHslSet[elementIndex].HslHueLightness16;
  *(lHslState+3) = AppliHslSet[elementIndex].HslHueLightness16 >>8;
  *(lHslState+4) = AppliHslSet[elementIndex].HslSaturation16;
  *(lHslState+5) = AppliHslSet[elementIndex].HslSaturation16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}  
/**
* @brief  Appli_Light_GetHslTargetStatus: This function is callback for Application
*         to get the application values in middleware used for target state.
* @param  lHslState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslTargetStatus(MOBLEUINT8* lHslState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lHslState) = AppliHslSet[elementIndex].TargetHslLightness16;
  *(lHslState+1) = AppliHslSet[elementIndex].TargetHslLightness16 >> 8;
  *(lHslState+2) = AppliHslSet[elementIndex].TargetHslHueLightness16;
  *(lHslState+3) = AppliHslSet[elementIndex].TargetHslHueLightness16 >>8;
  *(lHslState+4) = AppliHslSet[elementIndex].TargetHslSaturation16;
  *(lHslState+5) = AppliHslSet[elementIndex].TargetHslSaturation16 >>8;
  
  return MOBLE_RESULT_SUCCESS;
}  
/**
* @brief  Appli_Light_GetHslHueStatus: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslHueState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslHueStatus(MOBLEUINT8* lHslHueState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lHslHueState) = AppliHslSet[elementIndex].HslHueLightness16;
  *(lHslHueState+1) = AppliHslSet[elementIndex].HslHueLightness16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}    


/**
* @brief  Appli_Light_GetHslSaturationStatus: This function is callback for Application
*         to get the application values in middleware used for transition change
* @param  lHslSaturationState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslSaturationStatus(MOBLEUINT8* lHslSaturationState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lHslSaturationState) = AppliHslSet[elementIndex].HslSaturation16;
  *(lHslSaturationState+1) = AppliHslSet[elementIndex].HslSaturation16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Appli_Light_GetHslDefaultStatus: This function is callback for Application
*         to get the application values in middleware used for transition change.
* @param  lHslDefaultState: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslDefaultStatus(MOBLEUINT8* lHslDefaultState, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  *(lHslDefaultState) = Appli_HslDefaultSet[elementIndex].HslDefaultLightness16;
  *(lHslDefaultState+1) = Appli_HslDefaultSet[elementIndex].HslDefaultLightness16 >> 8;
  *(lHslDefaultState+2) = Appli_HslDefaultSet[elementIndex].HslDefaultHueLightness16;
  *(lHslDefaultState+3) = Appli_HslDefaultSet[elementIndex].HslDefaultHueLightness16 >>8;
  *(lHslDefaultState+4) = Appli_HslDefaultSet[elementIndex].HslDefaultSaturation16;
  *(lHslDefaultState+5) = Appli_HslDefaultSet[elementIndex].HslDefaultSaturation16 >>8;
  
  return MOBLE_RESULT_SUCCESS;
}  

/**
* @brief  Appli_Light_GetHslSatRange: This function is callback for Application
*         to get the application values in middleware used for transition change
* @param  lHslSatRange: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_GetHslSatRange(MOBLEUINT8* lHslSatRange, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{   
  *(lHslSatRange) = AppliHslRangeSet[elementIndex].StatusCode;
  *(lHslSatRange+1) = AppliHslRangeSet[elementIndex].HslMinSaturation16;
  *(lHslSatRange+2) = AppliHslRangeSet[elementIndex].HslMinSaturation16 >> 8;
  *(lHslSatRange+3) = AppliHslRangeSet[elementIndex].HslMaxSaturation16;
  *(lHslSatRange+4) = AppliHslRangeSet[elementIndex].HslMaxSaturation16 >>8;
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Appli_Light_GetHslHueRange: This function is callback for Application
* to get the application values in middleware used for transition change.
* @param  lHslHueRange: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_GetHslHueRange(MOBLEUINT8* lHslHueRange, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{   
  *(lHslHueRange) = AppliHslRangeSet[elementIndex].StatusCode;
  *(lHslHueRange+1) = AppliHslRangeSet[elementIndex].HslHueMinRange16;
  *(lHslHueRange+2) = AppliHslRangeSet[elementIndex].HslHueMinRange16 >> 8;
  *(lHslHueRange+3) = AppliHslRangeSet[elementIndex].HslHueMaxRange16;
  *(lHslHueRange+4) = AppliHslRangeSet[elementIndex].HslHueMaxRange16 >>8;
    
    return MOBLE_RESULT_SUCCESS;
}  

/******************************************************************************/
#endif    /* ENABLE_LIGHT_MODEL_SERVER_HSL */
/******************************************************************************/

/******************************************************************************/
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
/******************************************************************************/

/**
* @brief  Function to convert the map the RGB values in real values.
* @param  value1: This value corresponds to red led.
* @param  value2: This value corresponds to green led .
* @param  value3: This value corresponds to blue led .
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval void
*/
void RgbF_Create(MOBLEUINT16 value1, MOBLEUINT16 value2, MOBLEUINT16 value3, MOBLEUINT8 elementIndex)
{
   Appli_RGBParam[elementIndex].Red_Value = (MOBLEUINT16)(65535 * value1/1000);
   Appli_RGBParam[elementIndex].Green_Value = (MOBLEUINT16)(65535 * value2/1000);
   Appli_RGBParam[elementIndex].Blue_Value = (MOBLEUINT16)(65535 * value3/1000);
}

/**
* @brief  Function to convert the HSL values in RGB values.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval void
*/
void HSL2RGB_Conversion(MOBLEUINT8 elementIndex)
{

  MOBLEUINT16 hueValue;
  float lightnessvalue;
  float saturationValue;
  MOBLEUINT16 cValue;  /* chroma */
  MOBLEUINT16 mValue;  /* intermediate value used for creating the r,g,b baseline */
  MOBLEUINT16 xValue;  /* intermediate value used for computing r,g,b */

  if(AppliHslSet[elementIndex].HslSaturation16 == 0)
  {
    Appli_RGBParam[elementIndex].Red_Value  = AppliHslSet[elementIndex].HslLightness16  ;
    Appli_RGBParam[elementIndex].Green_Value = AppliHslSet[elementIndex].HslLightness16 ;
    Appli_RGBParam[elementIndex].Blue_Value = AppliHslSet[elementIndex].HslLightness16 ;
  }
  else if((AppliHslSet[elementIndex].HslLightness16 == 0xFFFF) || (AppliHslSet[elementIndex].HslLightness16 == 0x00))
  {
    Appli_RGBParam[elementIndex].Red_Value  = AppliHslSet[elementIndex].HslLightness16  ;
    Appli_RGBParam[elementIndex].Green_Value = AppliHslSet[elementIndex].HslLightness16 ;
    Appli_RGBParam[elementIndex].Blue_Value = AppliHslSet[elementIndex].HslLightness16 ;
  }
  else
  {  
    /* Hue is calculated for angular value. so converting in between 0 to 360 degree */
    hueValue = (MOBLEUINT16)(360 * (float)AppliHslSet[elementIndex].HslHueLightness16/65535);
    lightnessvalue = (float)AppliHslSet[elementIndex].HslLightness16/65535;
    saturationValue = (float)AppliHslSet[elementIndex].HslSaturation16/65535;
    
    /* Formula to get the values used for RGB conversion */
    cValue = (MOBLEUINT16)(((1- fabs(2 * lightnessvalue-1))*saturationValue) * 1000);
    mValue = (MOBLEUINT16)((lightnessvalue * 1000) -  (cValue/2));
    xValue = (MOBLEUINT16)(cValue * (1 - fabs(fmod(hueValue / 60.0, 2.0) - 1)));
    
    /* Conditions applied for the different angle of hue value */
    if (hueValue > 0 && hueValue < (HUE_UPPER_LIMIT / 6))
    {
       RgbF_Create(cValue + mValue, xValue + mValue, mValue, elementIndex);
    }
    else if (hueValue >= (HUE_UPPER_LIMIT / 6) && hueValue < (HUE_UPPER_LIMIT / 3))
    {
       RgbF_Create(xValue + mValue, cValue + mValue, mValue, elementIndex);
    }
    else if (hueValue >= (HUE_UPPER_LIMIT / 3) && hueValue < (HUE_UPPER_LIMIT / 2))
    {
       RgbF_Create(mValue, cValue + mValue, xValue + mValue, elementIndex);
    }
    else if (hueValue >= (HUE_UPPER_LIMIT / 2)
                          && hueValue < (2 * HUE_UPPER_LIMIT / 3))
    {
       RgbF_Create(mValue, xValue + mValue, cValue + mValue, elementIndex);
    }
    else if (hueValue >= (2 * HUE_UPPER_LIMIT / 3)
                          && hueValue < (5 * HUE_UPPER_LIMIT / 6))
    {
       RgbF_Create(xValue + mValue, mValue, cValue + mValue, elementIndex);
    }
    else if ((hueValue >= (5 * HUE_UPPER_LIMIT / 6) && hueValue <= HUE_UPPER_LIMIT) || (hueValue == 0))
    {
       RgbF_Create(cValue + mValue, mValue, xValue + mValue, elementIndex);
    }
    else
    {
       RgbF_Create(mValue, mValue, mValue, elementIndex);
    } 
  }
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL */
/******************************************************************************/
/**
* @}
*/

/**
* @}
*/

