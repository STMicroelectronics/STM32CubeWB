/**
  ******************************************************************************
  * @file    appli_generic.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Generic Mesh Models 
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
extern Appli_LightPwmValue_t Appli_LightPwmValue[APPLICATION_NUMBER_OF_ELEMENTS];

Appli_Generic_OnOffSet AppliOnOffSet[APPLICATION_NUMBER_OF_ELEMENTS];
Appli_Generic_LevelSet AppliLevelSet[APPLICATION_NUMBER_OF_ELEMENTS];
Appli_Generic_PowerOnOffSet AppliPowerOnSet[APPLICATION_NUMBER_OF_ELEMENTS];
Appli_Generic_DefaultTransitionSet AppliDefaultTransitionSet[APPLICATION_NUMBER_OF_ELEMENTS];

aPwmLedGsData_TypeDef aPwmLedGsData_generic = {PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_0_7};

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
                                     MOBLEUINT8 OptionalValid,
                                     MOBLEUINT16 dstPeer,
                                     MOBLEUINT8 elementIndex)
{
  /* LED control only for main element */
  if(elementIndex == GENERIC_SERVER_MAIN_ELEMENT_INDEX)
  {
    /*User need to modify the parameters as per the number of elements per node*/
    AppliOnOffSet[elementIndex].Present_OnOff = pGeneric_OnOffParam->Present_OnOff_State;
    AppliOnOffSet[elementIndex].Present_OnOffValue = pGeneric_OnOffParam->Present_OnOff_Value; 

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
    
    /* This condition is applicable when user want to on off the light with some 
       default transition value, or optionalValid =IN_TRANSITION ,
       transition is in progress.
    */
    if((OptionalValid == DEFAULT_TRANSITION) || (OptionalValid == IN_TRANSITION))
    {
      Appli_LightPwmValue[elementIndex].IntensityValue = AppliOnOffSet[elementIndex].Present_OnOffValue;
      AppliOnOffSet[elementIndex].TargetValue = pGeneric_OnOffParam->Target_OnOff; 

      if(AppliOnOffSet[elementIndex].Present_OnOffValue == AppliOnOffSet[elementIndex].TargetValue)
      {
        if(AppliOnOffSet[elementIndex].Present_OnOffValue > 0)
        {
          aPwmLedGsData_generic[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue   * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
          aPwmLedGsData_generic[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
          aPwmLedGsData_generic[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue  * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
          LED_On(aPwmLedGsData_generic);
        }
        else
        {
          LED_Off();
        }
      }
    }  
    else
    {
      if((AppliOnOffSet[elementIndex].Present_OnOff == APPLI_LED_ON) && (OptionalValid == NO_TRANSITION))
      { 
        aPwmLedGsData_generic[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue   * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
        aPwmLedGsData_generic[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
        aPwmLedGsData_generic[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue  * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF; //PWM_LED_GSDATA_7_0;
        LED_On(aPwmLedGsData_generic);
      }
      else
      {  
          LED_Off();
      } 
    }
  }
  
  TRACE_M(TF_GENERIC, "Appli_Generic_OnOff_Set callback received for elementIndex %d \r\n", elementIndex);           
  TRACE_M(TF_SERIAL_CTRL, "#8202!for elementIndex %d \r\n", elementIndex);

  NvmStatePowerFlag_Set(GENERIC_ON_OFF_NVM_FLAG, elementIndex);

  return MOBLE_RESULT_SUCCESS;
}

/******************************************************************************/
#endif  /* #ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF */
/******************************************************************************/

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
  
  //Manage Default Color
  if((Appli_LightPwmValue[elementIndex].PwmRedValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmGreenValue == 0) &&
       (Appli_LightPwmValue[elementIndex].PwmBlueValue == 0))
  {
	  Appli_LightPwmValue[elementIndex].PwmBlueValue=0xFF;
  }
  
  //Manage intensity
  Appli_LightPwmValue[elementIndex].IntensityValue = (AppliLevelSet[elementIndex].Present_Level16 * 0xFFFF)/0x7FFF; //Convert generic Level into Intensity

  aPwmLedGsData_generic[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue   * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF;
  aPwmLedGsData_generic[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF;
  aPwmLedGsData_generic[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue  * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF;
  LED_On(aPwmLedGsData_generic);
        
  TRACE_M(TF_GENERIC,"Appli_Generic_Level_Set callback received for elementIndex %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8206!for elementIndex %d \r\n", elementIndex);
  
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
MOBLE_RESULT Appli_Generic_Delta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                          MOBLEUINT8 OptionalValid,MOBLEUINT16 dstPeer, 
                                              MOBLEUINT8 elementIndex)
{
  AppliLevelSet[elementIndex].Present_Level16 = pdeltalevelParam->Present_Level16;
  Appli_LightPwmValue[elementIndex].IntensityValue = (AppliLevelSet[elementIndex].Present_Level16 * 0xFFFF)/0x7FFF; //Convert generic Level into Intensity

  //Manage Default Color
  if((Appli_LightPwmValue[elementIndex].PwmRedValue == 0) &&
     (Appli_LightPwmValue[elementIndex].PwmGreenValue == 0) &&
       (Appli_LightPwmValue[elementIndex].PwmBlueValue == 0))
  {
    Appli_LightPwmValue[elementIndex].PwmBlueValue=0xFF;
  }

  aPwmLedGsData_generic[PWM_LED_RED]   = (Appli_LightPwmValue[elementIndex].PwmRedValue   * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF;
  aPwmLedGsData_generic[PWM_LED_GREEN] = (Appli_LightPwmValue[elementIndex].PwmGreenValue * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF;
  aPwmLedGsData_generic[PWM_LED_BLUE]  = (Appli_LightPwmValue[elementIndex].PwmBlueValue  * Appli_LightPwmValue[elementIndex].IntensityValue)/0xFFFF;
  LED_On(aPwmLedGsData_generic);
  
  TRACE_M(TF_GENERIC,"Generic_LevelDelta_Set callback received for element %d \r\n", elementIndex);
  TRACE_M(TF_SERIAL_CTRL,"#8209!\r\n");

  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Appli_Generic_Move_Set: This function is callback for Application
*          when Generic Level Move message is received
* @param  pdeltaMoveParam: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Appli_Generic_Move_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
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
  TRACE_M(TF_SERIAL_CTRL,"#8213! for elementIndex %d \r\n", elementIndex);
  TRACE_M(TF_GENERIC,"Appli_Generic_PowerOnOff_Set callback received for elementIndex %d \r\n", elementIndex);

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


