/**
  ******************************************************************************
  * @file    appli_light.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Lighting Mesh Models 
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
#include "PWM_handlers.h"
#include "PWM_config.h"
#include "appli_nvm.h"
#include "math.h"

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

MOBLE_RESULT Appli_Light_GetLightnessLastStatus(MOBLEUINT8* lLastState,
                                                MOBLEUINT16 dstPeer, 
                                                MOBLEUINT8 elementIndex);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


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
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
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

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
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
  
Appli_LightPwmValue_t Appli_LightPwmValue;
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
                                       MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  MOBLEUINT16 duty;
  static MOBLEUINT16 previousIntensity = 0;
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Set callback received for element %d \r\n", elementIndex);
    TRACE_M(TF_SERIAL_CTRL,"#824C!\r\n");
  
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
  
#if 0
  /* For demo, if Level is more than 100, switch ON the LED */
  if(pLight_LightnessParam->LightnessPresentValue16 >= 0x8000)
  {
    BSP_LED_On(LED_BLUE);
  }
  else
  {
    BSP_LED_Off(LED_BLUE);
  }
#endif

  TRACE_M(TF_SERIAL_CTRL,"#8206%04hx!\n\r",
          ApplilightnessSet[elementIndex].PresentState16);

  duty = PwmValueMapping(ApplilightnessSet[elementIndex].PresentState16 , 0xfFFF ,0); 
  Appli_LightPwmValue.IntensityValue = duty;
  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);

  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_LIGHTNESS_NVM_FLAG, elementIndex);
  
  return MOBLE_RESULT_SUCCESS;
  
}

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS */
/******************************************************************************/

/**
* @brief  Appli_Light_Lightness_Status: This function is callback for Application
*           when Light Lightness status message is received
* @param  pLightness_status: Pointer to the parameters received for message
* @param  pLength: length of data 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Lightness_Status(MOBLEUINT8 const *pLightness_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Status callback received for element %d \r\n", elementIndex);
    TRACE_M(TF_SERIAL_CTRL,"#824E!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pLightness_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Lightness: 0x%x (%d)\r\n",
              value | pLightness_status[i] << 8,
              value | pLightness_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Lightness: 0x%x (%d)\r\n",
              value | pLightness_status[i] << 8,
              value | pLightness_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\r\n",
              pLightness_status[i],
              pLightness_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


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
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Linear_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8250!\r\n");
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  */
/******************************************************************************/

/**
* @brief  Appli_Light_Lightness_Linear_Status: This function is callback for Application
*         when Light Lightness Linear status message is received
* @param  pLightnessLinear_status: Pointer to the parameters received for message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @param  pLength: length of data
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Linear_Status(MOBLEUINT8 const *pLightnessLinear_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Linear_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8252!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pLightnessLinear_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Lightness: 0x%x (%d)\r\n",
              value | pLightnessLinear_status[i] << 8,
              value | pLightnessLinear_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Lightness: 0x%x (%d)\r\n",
              value | pLightnessLinear_status[i] << 8,
              value | pLightnessLinear_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\r\n",
              pLightnessLinear_status[i],
              pLightnessLinear_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


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
#if 0
  if(pLight_LightnessDefaultParam->LightnessDefaultStatus > 0)
  {
    BSP_LED_On(LED_BLUE);
  }
  else
  {
    BSP_LED_Off(LED_BLUE);
  }
#endif
  
  ApplilightnessSet[elementIndex].LightnessDefault = 
    pLight_LightnessDefaultParam->LightnessDefaultStatus;
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Default_Set callback received for element %d \r\n", elementIndex);
    TRACE_M(TF_SERIAL_CTRL,"#8259! \r\n");
  
  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_LIGHTNESS_NVM_FLAG,elementIndex);
   
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP */
/******************************************************************************/

/**
* @brief  Appli_Light_Lightness_Default_Status: This function is callback for Application
*         when Light Lightness Default status message is received
* @param  pLightnessDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Default_Status(MOBLEUINT8 const *pLightnessDefault_status, MOBLEUINT32 pLength
                                                  , MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Default_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8256!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      value = pLightnessDefault_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness Default: 0x%x (%d)\r\n",
              value | pLightnessDefault_status[i] << 8,
              value | pLightnessDefault_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Light_Lightness_Last_Status: This function is callback for Application
*         when Light Lightness Last status message is received
* @param  pLightnessDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Last_Status(MOBLEUINT8 const *pLightnessLast_status, 
                                               MOBLEUINT32 pLength, 
                                               MOBLEUINT16 dstPeer, 
                                               MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
 
  TRACE_M(TF_LIGHT,"Light_Lightness_Last_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8256!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      value = pLightnessLast_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness Last: 0x%x (%d)\r\n",
              value | pLightnessLast_status[i] << 8,
              value | pLightnessLast_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


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
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Range_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#825B!\r\n");
  
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP */
/******************************************************************************/

/**
* @brief  Appli_Light_Lightness_Range_Status: This function is callback for Application
*         when Light Lightness range ststus message is received
* @param  pLightnessRange_status: Pointer to the parameters received for message
* @param  pLength: length of data 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Lightness_Range_Status(MOBLEUINT8 const *pLightnessRange_status, MOBLEUINT32 pLength
                                                , MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Lightness_Range_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8258!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      TRACE_M(TF_LIGHT,"Status Code: 0x%x (%d)\r\n",
              pLightnessRange_status[i],
              pLightnessRange_status[i]);
    }
    else if((i == 1) || (i == 3))
    {
      value = pLightnessRange_status[i];
    }
    else if(i == 2)
    {
      TRACE_M(TF_LIGHT,"Range Min: 0x%x (%d)\r\n",
              value | pLightnessRange_status[i] << 8,
              value | pLightnessRange_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Range Max: 0x%x (%d)\r\n",
              value | pLightnessRange_status[i] << 8,
              value | pLightnessRange_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


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
  
  TRACE_M(TF_LIGHT,"Appli_Light_Ctl_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#825E!\r\n");
  
  AppliCtlSet[elementIndex].PresentLightness16  = pLight_CtlParam->PresentCtlLightness16;
  AppliCtlSet[elementIndex].PresentTemperature16 = pLight_CtlParam->PresentCtlTemperature16;
  AppliCtlSet[elementIndex].PresentCtlDelta16 = pLight_CtlParam->PresentCtlDelta16;
  
  /* Save target parameters if transition is available */
  AppliCtlSet[elementIndex].TargetLightness16  = pLight_CtlParam->TargetCtlLightness16;
  AppliCtlSet[elementIndex].TargetTemperature16 = pLight_CtlParam->TargetCtlTemperature16;
  AppliCtlSet[elementIndex].TargetCtlDelta16 = pLight_CtlParam->TargetCtlDeltaUv16;
  
  colourRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentTemperature16 ,MAX_CTL_TEMP_RANGE ,MIN_CTL_TEMP_RANGE);
  
  brightRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentLightness16 ,0XFFFF ,0);
  
  Appli_LightPwmValue.PwmCoolValue = PWM_CoolValue(colourRatio , brightRatio); 
  Appli_LightPwmValue.PwmWarmValue = PWM_WarmValue(colourRatio , brightRatio);
   
  Rgb_LedOffState();
  
  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
  
  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_CTL_NVM_FLAG, elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL */
/******************************************************************************/

/**
* @brief  Appli_Light_Ctl_Status: This function is callback for Application
*         when Light CTL status message is received
* @param  pLightCtl_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_Ctl_Status(MOBLEUINT8 const *pLightCtl_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif

  TRACE_M(TF_LIGHT,"Light_Ctl_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8260!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4) || (i == 6))
    {
      value = pLightCtl_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present CTL Lightness: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Present CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Target CTL Lightness: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 7)
    {
      TRACE_M(TF_LIGHT,"Target CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtl_status[i] << 8,
              value | pLightCtl_status[i] << 8);
    }
    else if(i == 8)
    {
      TRACE_M(TF_LIGHT,"Remaining Time:0x%x (%d)\r\n",
              pLightCtl_status[i],
              pLightCtl_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


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
  
  TRACE_M(TF_LIGHT,"Appli_Light_CtlTemperature_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8264!\r\n");
  
  AppliCtlSet[elementIndex].PresentTemperature16 = pLight_CtltempParam->PresentCtlTemperature16;
  AppliCtlSet[elementIndex].PresentCtlDelta16 = pLight_CtltempParam->PresentCtlDelta16;
  
  /* Made a copy of target in case of transition values change */
  AppliCtlSet[elementIndex].TargetTemperature16 = pLight_CtltempParam->TargetCtlTemperature16;
  AppliCtlSet[elementIndex].TargetCtlDelta16 = pLight_CtltempParam->TargetCtlDeltaUv16;
  
  colourRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentTemperature16 ,MAX_CTL_TEMP_RANGE ,MIN_CTL_TEMP_RANGE);
  
  brightRatio = Ratio_CalculateValue(AppliCtlSet[elementIndex].PresentLightness16 ,0XFFFF ,0);
  
  Appli_LightPwmValue.PwmCoolValue = PWM_CoolValue(colourRatio , brightRatio); 
  Appli_LightPwmValue.PwmWarmValue = PWM_WarmValue(colourRatio , brightRatio);
   
  Rgb_LedOffState();

  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_CTL_NVM_FLAG, elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE */
/******************************************************************************/

/**                                                  
* @brief  Appli_Light_CtlTemperature_Status: This function is callback for Application
*         when Light CTL temperature status message is received
* @param  pLightCtlTemp_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Status(MOBLEUINT8 const *pLightCtlTemp_status, MOBLEUINT32 pLength
                                               , MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_CtlTemperature_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8266!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4) || (i == 6))
    {
      value = pLightCtlTemp_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Present CTL Delta UV: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Target CTL Temperature: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 7)
    {
      TRACE_M(TF_LIGHT,"Target CTL Delta UV: 0x%x (%d)\r\n",
              value | pLightCtlTemp_status[i] << 8,
              value | pLightCtlTemp_status[i] << 8);
    }
    else if(i == 8)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\r\n",
              pLightCtlTemp_status[i],
              pLightCtlTemp_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}



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

  TRACE_M(TF_LIGHT,"Light_CtlTemperature_Range_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#826B!\r\n");
  
  return MOBLE_RESULT_SUCCESS;
}


/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP */
/******************************************************************************/

/**                                                  
* @brief  Appli_Light_CtlTemperature_Range_Set: This function is callback for Application
*         when Light CTL temperature range status message is received
* @param  pCtlTempRange_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlTemperature_Range_Status(MOBLEUINT8 const *pCtlTempRange_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value;
#endif
  
  TRACE_M(TF_LIGHT,"Light_CtlTemperature_Range_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8263!\r\n");
  
  
  RestoreFlag = LIGHT_CTL_NVM_FLAG;
  AppliNvm_SaveMessageParam();
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      TRACE_M(TF_LIGHT,"Status Code: 0x%x (%d)\r\n",
              pCtlTempRange_status[i],
              pCtlTempRange_status[i]);
    }
    else if((i == 1) || (i == 3))
    {
      value = pCtlTempRange_status[i];
    }
    else if(i == 2)
    {
      TRACE_M(TF_LIGHT,"Range Min: 0x%x (%d)\r\n",
              value | pCtlTempRange_status[i] << 8,
              value | pCtlTempRange_status[i] << 8 );
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Range Max: 0x%x (%d)\r\n",
              value | pCtlTempRange_status[i] << 8,
              value | pCtlTempRange_status[i] << 8 );
    }
  }
#endif

  return MOBLE_RESULT_SUCCESS;
}


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
  
  TRACE_M(TF_LIGHT,"Light_CtlDefault_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8269!\r\n");
  
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif   /* #ifdef ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP */
/******************************************************************************/

/**                                                  
* @brief  Appli_Light_CtlDefault_Status: This function is callback for Application
*         when Light CTL Default status message is received
* @param  pCtlDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Light_CtlDefault_Status(MOBLEUINT8 const *pCtlDefault_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_Ctl_DefaultStatus callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8268!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4))
    {
      value = pCtlDefault_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness: 0x%x (%d)\n\r",
              value | pCtlDefault_status[i] << 8,
              value | pCtlDefault_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Temperature: 0x%x (%d)\n\r",
              value | pCtlDefault_status[i] << 8,
              value | pCtlDefault_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Delta UV: 0x%x (%d)\n\r",
              value | pCtlDefault_status[i] << 8,
              value | pCtlDefault_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
} 


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
  TRACE_M(TF_LIGHT,"Appli_Light_Hsl_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8276!\r\n");
  
  AppliHslSet[elementIndex].HslLightness16 = pLight_HslParam->PresentHslLightness16;
  AppliHslSet[elementIndex].HslHueLightness16 = pLight_HslParam->PresentHslHueLightness16;
  AppliHslSet[elementIndex].HslSaturation16 = pLight_HslParam->PresentHslSaturation16;
  
  /* Save target parameters if transition is available */
  AppliHslSet[elementIndex].TargetHslLightness16  = pLight_HslParam->TargetHslLightness16;
  AppliHslSet[elementIndex].TargetHslHueLightness16 = pLight_HslParam->TargetHslHueLightness16;
  AppliHslSet[elementIndex].TargetHslSaturation16 = pLight_HslParam->TargetHslSaturation16;
   
  /* Function to convert HSL values in RGB values */
  HSL2RGB_Conversion(elementIndex);
  
  Appli_LightPwmValue.PwmRedValue = PwmValueMapping(Appli_RGBParam[elementIndex].Red_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmGreenValue = PwmValueMapping(Appli_RGBParam[elementIndex].Green_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmBlueValue = PwmValueMapping(Appli_RGBParam[elementIndex].Blue_Value ,0xFFFF ,0x00); 
  
  /* when HSL is set, make CTL pwm will bw zero */
  Ctl_LedOffState();
  
  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif     /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL */
/******************************************************************************/

/**                                                  
* @brief  Appli_Light_Hsl_Status: This function is callback for Application
*         when Light HSL status message is received
* @param  pHsl_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_Hsl_Status(MOBLEUINT8 const *pHsl_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif

  TRACE_M(TF_LIGHT,"Light_Hsl_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8278!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4))
    {
      value = pHsl_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"HSL Lightness: 0x%x (%d)\n\r",
              value | pHsl_status[i] << 8,
              value | pHsl_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"HSL Hue: 0x%x (%d)\n\r",
              value | pHsl_status[i] << 8,
              value | pHsl_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"HSL Saturation: 0x%x (%d)\n\r",
              value | pHsl_status[i] << 8,
              value | pHsl_status[i] << 8);
    }
    else if(i == 6)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\n\r",
              pHsl_status[i],
              pHsl_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


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
  TRACE_M(TF_LIGHT,"Light_HslHue_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#826F!\r\n");	
  
  AppliHslSet[elementIndex].HslHueLightness16 = pLight_HslHueParam->PresentHslHueLightness16; 
  
  HSL2RGB_Conversion(elementIndex);
  
  Appli_LightPwmValue.PwmRedValue = PwmValueMapping(Appli_RGBParam[elementIndex].Red_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmGreenValue = PwmValueMapping(Appli_RGBParam[elementIndex].Green_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmBlueValue = PwmValueMapping(Appli_RGBParam[elementIndex].Blue_Value ,0xFFFF ,0x00); 
  
  Ctl_LedOffState();

  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
   
}

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_HUE */
/******************************************************************************/

/**                                                  
* @brief  Appli_Light_HslHue_Status: This function is callback for Application
*         when Light HSL HUE status message is received
* @param  pHslHue_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslHue_Status(MOBLEUINT8 const *pHslHue_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslHue_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8271!\r\n");
    
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pHslHue_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Hue: 0x%x (%d)\n\r",
              value | pHslHue_status[i] << 8,
              value | pHslHue_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Hue: 0x%x (%d)\n\r",
              value | pHslHue_status[i] << 8,
              value | pHslHue_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\n\r",
              pHslHue_status[i],
              pHslHue_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
} 


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
  TRACE_M(TF_LIGHT,"Appli_Light_HslSaturation_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8273!\r\n");	
  
  AppliHslSet[elementIndex].HslSaturation16 = pLight_HslSaturationParam->PresentHslSaturation16;
  
  HSL2RGB_Conversion(elementIndex);
  
  Appli_LightPwmValue.PwmRedValue = PwmValueMapping(Appli_RGBParam[elementIndex].Red_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmGreenValue = PwmValueMapping(Appli_RGBParam[elementIndex].Green_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmBlueValue = PwmValueMapping(Appli_RGBParam[elementIndex].Blue_Value ,0xFFFF ,0x00); 
  
  Ctl_LedOffState();

  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif   /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION */
/******************************************************************************/

/**                                                  
* @brief  Appli_Light_HslSaturation_Status: This function is callback for Application
*         when Light HSL Saturation status message is received
* @param  pHslSaturation_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslSaturation_Status(MOBLEUINT8 const *pHslSaturation_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslSaturation_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8275!\r\n");
  
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2))
    {
      value = pHslSaturation_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Present Saturation: 0x%x (%d)\n\r",
              value | pHslSaturation_status[i] << 8,
              value | pHslSaturation_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Target Saturation: 0x%x (%d)\n\r",
              value | pHslSaturation_status[i] << 8,
              value | pHslSaturation_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Remaining Time: 0x%x (%d)\n\r",
              pHslSaturation_status[i],
              pHslSaturation_status[i]);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


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
MOBLE_RESULT Appli_Light_HslDefault_Set(Light_HslStatus_t* pLight_HslDefaultParam,
                                        MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_LIGHT,"Appli_Light_HslDefault_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#827F!\r\n");	
  
  Appli_HslDefaultSet[elementIndex].HslDefaultLightness16 = pLight_HslDefaultParam->PresentHslLightness16;
  Appli_HslDefaultSet[elementIndex].HslDefaultHueLightness16 = pLight_HslDefaultParam->PresentHslHueLightness16;
  Appli_HslDefaultSet[elementIndex].HslDefaultSaturation16 = pLight_HslDefaultParam->PresentHslSaturation16;  
  
  AppliHslSet[elementIndex].HslLightness16 = Appli_HslDefaultSet[elementIndex].HslDefaultLightness16;
  AppliHslSet[elementIndex].HslHueLightness16 = Appli_HslDefaultSet[elementIndex].HslDefaultHueLightness16;
  AppliHslSet[elementIndex].HslSaturation16 = Appli_HslDefaultSet[elementIndex].HslDefaultSaturation16;
  
  HSL2RGB_Conversion(elementIndex);
  
  Appli_LightPwmValue.PwmRedValue = PwmValueMapping(Appli_RGBParam[elementIndex].Red_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmGreenValue = PwmValueMapping(Appli_RGBParam[elementIndex].Green_Value ,0xFFFF ,0x00); 
  Appli_LightPwmValue.PwmBlueValue = PwmValueMapping(Appli_RGBParam[elementIndex].Blue_Value ,0xFFFF ,0x00); 
  
  Ctl_LedOffState();

  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
  /* set the flag value for NVM store */
  NvmStatePowerFlag_Set(LIGHT_HSL_NVM_FLAG, elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif    /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP */
/******************************************************************************/
              
/**                                                  
* @brief  Appli_Light_HslDefault_Status: This function is callback for Application
*          when Light HSL Default status  message is received
* @param  pHslDefault_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslDefault_Status(MOBLEUINT8 const *pHslDefault_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslDefault_Status callback received for element %d \r\n", elementIndex);  
  TRACE_M(TF_SERIAL_CTRL,"#827C!\r\n");

#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if((i == 0) || (i == 2) || (i == 4))
    {
      value = pHslDefault_status[i];
    }
    else if(i == 1)
    {
      TRACE_M(TF_LIGHT,"Lightness: 0x%x (%d)\n\r",
              value | pHslDefault_status[i] << 8,
              value | pHslDefault_status[i] << 8);
    }
    else if(i == 3)
    {
      TRACE_M(TF_LIGHT,"Hue: 0x%x (%d)\n\r",
              value | pHslDefault_status[i] << 8,
              value | pHslDefault_status[i] << 8);
    }
    else if(i == 5)
    {
      TRACE_M(TF_LIGHT,"Saturation: 0x%x (%d)\n\r",
              value | pHslDefault_status[i] << 8,
              value | pHslDefault_status[i] << 8);
    }
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}

              
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
  TRACE_M(TF_LIGHT,"Light_HslRange_Set callback received for element %d \r\n", elementIndex);  
  TRACE_M(TF_SERIAL_CTRL,"#8281!\r\n"); 	
  
  AppliHslRangeSet[elementIndex].HslHueMinRange16 = pLight_HslRangeParam->HslHueMinRange16;
  AppliHslRangeSet[elementIndex].HslHueMaxRange16 = pLight_HslRangeParam->HslHueMaxRange16;
  AppliHslRangeSet[elementIndex].HslMinSaturation16 = pLight_HslRangeParam->HslMinSaturation16;
  AppliHslRangeSet[elementIndex].HslMaxSaturation16 = pLight_HslRangeParam->HslMaxSaturation16;
   
   return MOBLE_RESULT_SUCCESS;
} 

/******************************************************************************/
#endif               /* #ifdef ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP */
/******************************************************************************/

/**                                                  
* @brief  Appli_Light_HslRange_Status: This function is callback for Application
*         when Light HSL range status message is received
* @param  pHslRange_status: Pointer to the parameters received for message
* @param  pLength: length of data
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Light_HslRange_Status(MOBLEUINT8 const *pHslRange_status, MOBLEUINT32 pLength, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT32 i;
  MOBLEUINT16 value = 0;
#endif
  
  TRACE_M(TF_LIGHT,"Light_HslRange_Status callback received for element %d \r\n", elementIndex); 
  TRACE_M(TF_SERIAL_CTRL,"#827E!\r\n");
  
    
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < pLength; i++)
  {
    if(i == 0)
    {
      TRACE_M(TF_LIGHT,"Status Code: 0x%x (%d)\n\r",
              pHslRange_status[i],
              pHslRange_status[i]);
    }
    else if((i == 1) || (i == 3) || (i == 5) || (i == 7))
    {
      value = pHslRange_status[i];
    }
    else if(i == 2)
    {
      TRACE_M(TF_LIGHT,"Hue Range Min: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
    else if(i == 4)
    {
      TRACE_M(TF_LIGHT,"Hue Range Max: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
    else if(i == 6)
    {
      TRACE_M(TF_LIGHT,"Saturation Range Min: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
    else if(i == 8)
    {
      TRACE_M(TF_LIGHT,"Saturation Range Max: 0x%x (%d)\n\r",
              value | pHslRange_status[i] << 8,
              value | pHslRange_status[i] << 8);
    }
  }
#endif

  return MOBLE_RESULT_SUCCESS;
}



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

/* This Function used to initialise the PWM . This is used for the RGB board */
void Appli_Light_PwmInit()
{
   Appli_LightPwmValue.IntensityValue = PWM_VALUE_OFF;
   Appli_LightPwmValue.PwmCoolValue = 0;
   Appli_LightPwmValue.PwmWarmValue = 0;
   Appli_LightPwmValue.PwmRedValue = PWM_VALUE_OFF;
   Appli_LightPwmValue.PwmGreenValue = PWM_VALUE_OFF;
   Appli_LightPwmValue.PwmBlueValue = PWM_VALUE_OFF;
   Light_UpdateLedValue(RESET_STATE , Appli_LightPwmValue);
}

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
    
    /* Conditons applied for the different angle of hue value */
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
* @brief  Rgb_LedOffState: This function is called while using CTL, makes all the RGB 
*         PWM off state for Application.        
* @param  void: 
* @retval void
*/ 
void Rgb_LedOffState(void)
{
  Appli_LightPwmValue.PwmRedValue = PWM_VALUE_OFF;
  Appli_LightPwmValue.PwmGreenValue = PWM_VALUE_OFF;
  Appli_LightPwmValue.PwmBlueValue = PWM_VALUE_OFF; 
  Appli_LightPwmValue.IntensityValue = PWM_VALUE_OFF;
}

/**
* @brief  Ctl_LedOffState: This function is called while using HSL, makes all the  
*         cool Warm PWM off state for Application.        
* @param  void: 
* @retval void
*/ 
void Ctl_LedOffState(void)
{
  Appli_LightPwmValue.PwmWarmValue = PWM_VALUE_OFF;
  Appli_LightPwmValue.PwmCoolValue = PWM_VALUE_OFF;
  Appli_LightPwmValue.IntensityValue = PWM_VALUE_OFF;
}

/**
* @brief  Function to set the values for PWM for RGB board.
* @param  state: decided the state of PWM according to the condition.
* @retval void
*/
void Light_UpdateLedValue(MOBLEUINT8 state ,Appli_LightPwmValue_t light_state)
{
  
#ifndef USER_BOARD_1LED
  if(light_state.IntensityValue > 0)
  {
#ifdef  USER_BOARD_COOL_WHITE_LED
    if((light_state.PwmCoolValue == 0) && (light_state.PwmWarmValue == 0))
      light_state.PwmWarmValue = light_state.IntensityValue;
#endif
#ifdef  USER_BOARD_RGB_LED
    if((light_state.PwmRedValue == PWM_VALUE_OFF) && 
       (light_state.PwmGreenValue == PWM_VALUE_OFF) && 
         (light_state.PwmBlueValue == PWM_VALUE_OFF))
      light_state.PwmBlueValue = light_state.IntensityValue;
#endif
  }
#endif
  
  if(state == RESUME_STATE)
  {
#ifdef USER_BOARD_1LED
    Modify_PWM(SINGLE_LED, light_state.IntensityValue); 
#endif

#ifdef  USER_BOARD_COOL_WHITE_LED

    Modify_PWM(COOL_LED, light_state.PwmCoolValue); 
    Modify_PWM(WARM_LED, light_state.PwmWarmValue); 
#endif

#ifdef USER_BOARD_RGB_LED

    Modify_PWM(RED_LED, light_state.PwmRedValue); 
    Modify_PWM(GREEN_LED, light_state.PwmGreenValue); 
    Modify_PWM(BLUE_LED, light_state.PwmBlueValue);  
#endif

  }
  else if(state == RESET_STATE)
  {
#ifdef USER_BOARD_1LED
    Modify_PWM(SINGLE_LED, PWM_VALUE_OFF);     
#endif
#ifdef  USER_BOARD_COOL_WHITE_LED
    Modify_PWM(COOL_LED, 0); 
    Modify_PWM(WARM_LED, 0); 
#endif
#ifdef  USER_BOARD_RGB_LED
    Modify_PWM(RED_LED, PWM_VALUE_OFF); 
    Modify_PWM(GREEN_LED, PWM_VALUE_OFF); 
    Modify_PWM(BLUE_LED, PWM_VALUE_OFF);
#endif
  }
  else if(state == LOAD_STATE)
  {
#ifdef USER_BOARD_1LED
    Modify_PWM(SINGLE_LED, light_state.IntensityValue); 
#endif
    
#ifdef  USER_BOARD_COOL_WHITE_LED    
    Modify_PWM(COOL_LED, light_state.PwmCoolValue); 
    Modify_PWM(WARM_LED, light_state.PwmWarmValue); 
#endif
#ifdef  USER_BOARD_RGB_LED    
    Modify_PWM(RED_LED, light_state.PwmRedValue); 
    Modify_PWM(GREEN_LED, light_state.PwmGreenValue); 
    Modify_PWM(BLUE_LED, light_state.PwmBlueValue); 
#endif    
  }
  else
  {
#ifdef USER_BOARD_1LED
    Modify_PWM(SINGLE_LED, light_state.IntensityValue); 
#endif
    
#ifdef  USER_BOARD_COOL_WHITE_LED        
    light_state.PwmCoolValue = PWM_DEFAULT_VALUE;
    
    Modify_PWM(COOL_LED, light_state.PwmCoolValue); 
    Modify_PWM(WARM_LED, light_state.PwmWarmValue); 
#endif
#ifdef  USER_BOARD_RGB_LED    
    Modify_PWM(RED_LED, light_state.PwmRedValue); 
    Modify_PWM(GREEN_LED, light_state.PwmGreenValue); 
    Modify_PWM(BLUE_LED, light_state.PwmBlueValue); 
#endif    
  } 
}

/**
* @}
*/

/**
* @}
*/
/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

