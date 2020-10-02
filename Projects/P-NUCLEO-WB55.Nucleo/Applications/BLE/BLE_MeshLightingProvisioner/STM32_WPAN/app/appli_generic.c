/**
  ******************************************************************************
  * @file    appli_generic.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Generic Mesh Models 
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
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "appli_generic.h"
#include "appli_light.h"
#include "common.h"
#include "mesh_cfg_usr.h"
#include "appli_nvm.h"

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
   
extern MOBLEUINT8 RestoreFlag;
extern MOBLEUINT16 IntensityValue;
extern MOBLEUINT8 IntensityFlag;
extern MOBLEUINT8 PowerOnOff_flag;
extern Appli_LightPwmValue_t Appli_LightPwmValue;

Appli_Generic_OnOffSet AppliOnOffSet[APPLICATION_NUMBER_OF_ELEMENTS];
Appli_Generic_LevelSet AppliLevelSet[APPLICATION_NUMBER_OF_ELEMENTS];
Appli_Generic_PowerOnOffSet AppliPowerOnSet[APPLICATION_NUMBER_OF_ELEMENTS];
Appli_Generic_DefaultTransitionSet AppliDefaultTransitionSet[APPLICATION_NUMBER_OF_ELEMENTS];

/* Private function prototypes -----------------------------------------------*/
MOBLE_RESULT Appli_Generic_LevelDelta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                          MOBLEUINT8 OptionalValid,
                                          MOBLEUINT16 dstPeer, 
                                          MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Generic_LevelMove_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                         MOBLEUINT8 OptionalValid, 
                                         MOBLEUINT16 dstPeer, 
                                         MOBLEUINT8 elementIndex);
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
/******************************************************************************/

/**
* @brief  Appli_Generic_OnOff_Set: This function is callback for Application
*          when Generic OnOff message is received
* @param  pGeneric_OnOffParam: Pointer to the parameters received for message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t* pGeneric_OnOffParam, 
                                     MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                     MOBLEUINT8 elementIndex)
{
  /*User need to modify the parameters as per the number of elements per node*/
  AppliOnOffSet[elementIndex].Present_OnOff = pGeneric_OnOffParam->Present_OnOff_State;
  AppliOnOffSet[elementIndex].Present_OnOffValue = pGeneric_OnOffParam->Present_OnOff_Value; 

  /* This condition is applicable when user want to on off the light with some 
    default transition value, or optionalValid =IN_TRANSITION ,
    transition is in progress.
  */
  if((OptionalValid == DEFAULT_TRANSITION) || (OptionalValid == IN_TRANSITION))
  {
    Appli_LightPwmValue.IntensityValue = AppliOnOffSet[elementIndex].Present_OnOffValue;
    Light_UpdateLedValue(LOAD_STATE ,Appli_LightPwmValue);

  #if 0 /* EME */
    if(AppliOnOffSet[elementIndex].Present_OnOff == APPLI_LED_ON)
    {
      AppliOnOffSet[elementIndex].TargetValue = PWM_TIME_PERIOD;
    }
    else
    {
      AppliOnOffSet[elementIndex].TargetValue = APPLI_LED_OFF;
    }
  #else
    AppliOnOffSet[elementIndex].TargetValue = pGeneric_OnOffParam->Target_OnOff; 
  #endif /* EME */

    if(AppliOnOffSet[elementIndex].Present_OnOffValue == AppliOnOffSet[elementIndex].TargetValue)
    {
      if(AppliOnOffSet[elementIndex].Present_OnOffValue > 0)
      {
        BSP_LED_On(LED_BLUE);
      }
      else
      {
        BSP_LED_Off(LED_BLUE);
      }
    }
  }  
  else
  {
    if((AppliOnOffSet[elementIndex].Present_OnOff == APPLI_LED_ON) && (OptionalValid == NO_TRANSITION))
    { 
      Appli_LightPwmValue.IntensityValue = PWM_TIME_PERIOD;
      Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
      BSP_LED_On(LED_BLUE);
    }
    else
    {  
      Appli_LightPwmValue.IntensityValue = PWM_VALUE_OFF;
      Light_UpdateLedValue(RESET_STATE , Appli_LightPwmValue);
      BSP_LED_Off(LED_BLUE);
    } 
  }
  
  TRACE_M(TF_GENERIC,"Generic_OnOff_Set callback received for element %d \r\n", elementIndex);           
  TRACE_M(TF_SERIAL_CTRL,"#8202%02hx!\n\r",AppliOnOffSet[elementIndex].Present_OnOff);

  NvmStatePowerFlag_Set(GENERIC_ON_OFF_NVM_FLAG, elementIndex);

  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* #ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF */
/******************************************************************************/


/**
* @brief  Appli_Generic_OnOff_Status: This function is callback for Application
*          when Generic OnOff message is received
* @param  pOnOff_status: Pointer to the parameters received for message
* @param  plength: length of the data 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_OnOff_Status(MOBLEUINT8 const *pOnOff_status, MOBLEUINT32 plength, 
                                        MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex)
{
  MOBLEUINT8 i;
  
  TRACE_M(TF_GENERIC,"Generic_OnOff_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8204!\r\n");
  
  for(i = 0; i < plength; i++)
  {
    if(i == 0)
      TRACE_M(TF_SERIAL_CTRL,"Present OnOff value: %d\n\r", pOnOff_status[i]);
    else if(i == 1)
      TRACE_M(TF_SERIAL_CTRL,"Target OnOff value: %d\n\r", pOnOff_status[i]);
    else if(i == 2)
      TRACE_M(TF_SERIAL_CTRL,"Remaining Time value: %d\n\r", pOnOff_status[i]);
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
/******************************************************************************/

/**
* @brief  Appli_Generic_Level_Set: This function is callback for Application
*          when Generic Level message is received
* @param  plevelParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t* plevelParam, 
                                     MOBLEUINT8 OptionalValid,MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  MOBLEUINT16 duty;
  static MOBLEUINT16 previousIntensity = 0;
   
  AppliLevelSet[elementIndex].Present_Level16= plevelParam->Present_Level16;
  if(AppliLevelSet[elementIndex].Present_Level16 <= 0)
  {
    AppliLevelSet[elementIndex].Present_Level16 = 0;
  }
  
  IntensityValue =  AppliLevelSet[elementIndex].Present_Level16;
  
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
  
  if(AppliLevelSet[elementIndex].Present_Level16 < 0x00)
  {
     AppliLevelSet[elementIndex].Present_Level16 = 0;
  }
  duty = PwmValueMapping(AppliLevelSet[elementIndex].Present_Level16 , 0x7FFF ,0x0000); 
  Appli_LightPwmValue.IntensityValue = duty;
  Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);

  TRACE_M(TF_GENERIC,"Generic_Level_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8206!\r\n");
  
  NvmStatePowerFlag_Set(GENERIC_LEVEL_NVM_FLAG, elementIndex);
    
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Generic_Delta_Set: This function is callback for Application
*          when Generic Level Delta message is received
* @param  pdeltalevelParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_LevelDelta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                          MOBLEUINT8 OptionalValid,MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  AppliLevelSet[elementIndex].Present_Level16 = pdeltalevelParam->Present_Level16;
  
  /* For demo, if Level is more than 50, switch ON the LED */
  if (AppliLevelSet[elementIndex].Present_Level16 >= 50)
  {
    Appli_LightPwmValue.IntensityValue = PWM_TIME_PERIOD;
    Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
    BSP_LED_On(LED_BLUE);
  }
  else
  {
    Light_UpdateLedValue(RESET_STATE , Appli_LightPwmValue);
    BSP_LED_Off(LED_BLUE);
  }
  
  TRACE_M(TF_GENERIC,"Generic_LevelDelta_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8209!\r\n");

  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Generic_LevelMove_Set: This function is callback for Application
*          when Generic Level Move message is received
* @param  pdeltaMoveParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_LevelMove_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                         MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                         MOBLEUINT8 elementIndex)
{
//  if(OptionalValid == 1)
//  {
    AppliLevelSet[elementIndex].Last_delta_level     = pdeltaMoveParam->Last_delta_level;
    AppliLevelSet[elementIndex].Last_Level_TID       = pdeltaMoveParam->Last_Level_TID;
    AppliLevelSet[elementIndex].Last_Present_Level16 = pdeltaMoveParam->Last_Present_Level16;
    AppliLevelSet[elementIndex].Present_Level16      = pdeltaMoveParam->Present_Level16;   
    AppliLevelSet[elementIndex].RemainingTime        = pdeltaMoveParam->RemainingTime;
    AppliLevelSet[elementIndex].Target_Level16       = pdeltaMoveParam->Target_Level16;
//  }
  
    TRACE_M(TF_GENERIC,"Generic_LevelMove_Set callback received for element %d \r\n", elementIndex);
    TRACE_M(TF_SERIAL_CTRL,"#820B! \r\n");
    return MOBLE_RESULT_SUCCESS;
  
}

/******************************************************************************/
#endif   /* ENABLE_GENERIC_MODEL_SERVER_LEVEL */
/******************************************************************************/

  /**
* @brief  Appli_Generic_Level_Status: This function is callback for Application
*          when Generic Level Move message is received
* @param  plevel_status: Pointer to the parameters message
* @param  plength: length of data 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Generic_Level_Status(MOBLEUINT8 const *plevel_status, 
                                        MOBLEUINT32 plength,
                                        MOBLEUINT16 dstPeer, 
                                        MOBLEUINT8 elementIndex)
{
#if ( CFG_DEBUG_TRACE != 0 )
  MOBLEUINT8 i;
  MOBLEUINT8 level = 0;
#endif
  
  TRACE_M(TF_GENERIC,"Generic_Level_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8208! \r\n");
      
#if ( CFG_DEBUG_TRACE != 0 )
  for(i = 0; i < plength; i++)
  {
    if((i == 0) || (i == 2))
      level = plevel_status[i];
    else if(i == 1)
      TRACE_M(TF_SERIAL_CTRL,"Present Level value: %d\n\r", 
              (plevel_status[i]<<8)|level);
    else if(i == 3)
      TRACE_M(TF_SERIAL_CTRL,"Target Level value: %d\n\r", 
              (plevel_status[i]<<8)|level);
    else if(i == 4)
      TRACE_M(TF_SERIAL_CTRL,"Remaining Time value: %d\n\r", plevel_status[i]);
  }
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
/******************************************************************************/
 
/**
* @brief  Appli_Generic_PowerOnOff_Set: This function is callback for Application
*           when Generic Power on off set message is received
* @param  pPowerOnOffParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_PowerOnOff_Set(Generic_PowerOnOffParam_t* pPowerOnOffParam, 
                                         MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{ 
  AppliPowerOnSet[elementIndex].PowerOnState = pPowerOnOffParam->PowerOnOffState;

  TRACE_M(TF_GENERIC,"Generic_PowerOnOff_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8213!\r\n");

  if(AppliPowerOnSet[elementIndex].PowerOnState == GENERIC_POWER_ON_STATE)
  {
    /* set the flag value for NVM store */
    RestoreFlag = GENERIC_ON_OFF_NVM_FLAG;
  }

  AppliNvm_SaveMessageParam();
    
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF */
/******************************************************************************/

/**
* @brief  Appli_Generic_PowerOnOff_Set: This function is callback for Application
*           when Generic Power on off set message is received
* @param   powerOnOff_status: Pointer to the parameters message
* @param  plength: length of data 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status,\
                 MOBLEUINT32 plength, MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex) 
{  
  MOBLEUINT8 i;

  TRACE_M(TF_GENERIC,"Generic_PowerOnOff_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8212!\r\n");
  
  for(i = 0; i < plength; i++)
  {
    if(i == 0)
      TRACE_M(TF_SERIAL_CTRL,"On Power up value: %d\n\r", 
              powerOnOff_status[i]);
  }

  return MOBLE_RESULT_SUCCESS;
}


/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
/******************************************************************************/

/**
* @brief  Appli_Generic_DefaultTransitionTime_Set: This function is callback for Application
*          when Generic Power on off set message is received
* @param  pDefaultTimeParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Set(Generic_DefaultTransitionParam_t* pDefaultTimeParam, 
                                         MOBLEUINT8 OptionalValid, MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex)
{
  
  AppliDefaultTransitionSet[elementIndex].DefaultTransitionTime = pDefaultTimeParam->DefaultTransitionTime;
  
  TRACE_M(TF_GENERIC,"Generic_DefaultTransitionTime_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#820E!\r\n");
  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif   /* ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME */
/******************************************************************************/

/**
* @brief  Appli_Generic_DefaultTransitionTime_Status: This function is callback for Application
*          when Generic Power on off set message is received
* @param  pTransition_status: Pointer to the parameters message
* @param  plength: length of data 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , MOBLEUINT32 plength,MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex) 
{  

  TRACE_M(TF_GENERIC,"Generic_DefaultTransitionTime_Status callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8210!\r\n");
  
  return MOBLE_RESULT_SUCCESS;
}



/**
* @brief  Appli_Generic_GetOnOffState: This function is callback for Application
*          when Generic on off status message is to be provided
* @param  pOnOff_status: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status,MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex)                                        
{
  
  *pOnOff_Status = AppliOnOffSet[elementIndex].Present_OnOff;
  return MOBLE_RESULT_SUCCESS; 
}


/**
* @brief  Appli_Generic_GetOnOffValue: This function is callback for Application
          to get the PWM value for the generic on off
* @param  pOnOff_Value: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_GetOnOffValue(MOBLEUINT8* pOnOff_Value, MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex)                                        
{
  
  *pOnOff_Value = AppliOnOffSet[elementIndex].Present_OnOffValue;
  *(pOnOff_Value+1) = AppliOnOffSet[elementIndex].Present_OnOffValue >> 8;
  *(pOnOff_Value+2) = AppliOnOffSet[elementIndex].TargetValue;
  *(pOnOff_Value+3) = AppliOnOffSet[elementIndex].TargetValue >> 8;
  return MOBLE_RESULT_SUCCESS; 
}


/**
* @brief  Appli_Generic_GetLevelStatus: This function is callback for Application
           when Generic Level status message is to be provided
* @param  pLevel_status: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status, MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex) 
{ 
   *pLevel_Status = AppliLevelSet[elementIndex].Present_Level16;
   *(pLevel_Status+1) = AppliLevelSet[elementIndex].Present_Level16 >> 8;
   *(pLevel_Status+2) = AppliLevelSet[elementIndex].Target_Level16;
   *(pLevel_Status+3) = AppliLevelSet[elementIndex].Target_Level16 >> 8;
  return MOBLE_RESULT_SUCCESS; 
}


/**
* @brief  Appli_Generic_GetPowerOnOffStatus: This function is callback for Application
*          when Generic Get Power status message is to be provided
* @param  pLevel_status: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_GetPowerOnOffStatus(MOBLEUINT8* pPower_Status, MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex) 
{ 

  *pPower_Status = AppliPowerOnSet[elementIndex].PowerOnState;
  
  return MOBLE_RESULT_SUCCESS; 
}


/**
* @brief  Appli_Generic_GetDefaultTransitionStatus: This function is callback for 
*           Application when Generic Level status message is to be provided
* @param  pTransition_Status: Pointer to the status message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_GetDefaultTransitionStatus(MOBLEUINT8* pTransition_Status,MOBLEUINT16 dstPeer, MOBLEUINT8 elementIndex) 
{ 

  *pTransition_Status = AppliDefaultTransitionSet[elementIndex].DefaultTransitionTime;
  
  return MOBLE_RESULT_SUCCESS; 
}


/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

