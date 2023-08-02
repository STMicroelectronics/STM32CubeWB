/**
******************************************************************************
* @file    generic.c
* @author  BLE Mesh Team
* @brief   Generic model middleware file
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "ble_mesh.h"
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "light_lc.h"
#include "common.h"
#include "models_if.h"
#include <string.h>
#include "compiler.h"
#include <stdint.h>
#include "math.h"


/** @addtogroup MODEL_GENERIC
*  @{
*/

/** @addtogroup Generic_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static Generic_TemporaryStatus_t Generic_TemporaryStatus[APPLICATION_NUMBER_OF_ELEMENTS];
static Generic_TimeParam_t Generic_TimeParam[APPLICATION_NUMBER_OF_ELEMENTS];
extern Publication1SecFlag_t Publication1SecFlag;
/* initialize the array with minimum level value and other parameter as zero */
static Generic_LevelStatus_t Generic_LevelStatus[APPLICATION_NUMBER_OF_ELEMENTS];
static Generic_OnOffStatus_t Generic_OnOffStatus[APPLICATION_NUMBER_OF_ELEMENTS];
 
/* Initialize the default transition value 0x00 
   Default Transition Step Resolution is 100 milliseconds
   Generic Default Transition Time is immediate.
*/
Generic_DefaultTransitionParam_t Generic_DefaultTransitionParam = {0x00};
Generic_ModelFlag_t Generic_ModelFlag[APPLICATION_NUMBER_OF_ELEMENTS];

extern MOBLEUINT16 CommandStatus; /* Current on/off status, shared with Vendor model, used to publish status  */
MOBLEUINT8 GenericUpdateFlag = 0;
MOBLEUINT8 OptionalParam = 0;

extern Model_Tid_t Model_Tid;
/*Variables used for the publishing of binded data */
extern Model_Binding_Var_t Model_Binding_Var;

const MODEL_OpcodeTableParam_t Generic_Opcodes_Table[] = 
{
  /* Generic OnOff Server */
/* model_id                                         opcode,                                    reliable,    min_payload_size, max_payload_size, response_opcode,                        min_response_size, max_response_size
    Here in this array, Handler is not defined; */
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF  
  {GENERIC_ONOFF_SERVER_MODEL_ID,                   GENERIC_ON_OFF_GET,                        MOBLE_TRUE,  0,                0,                GENERIC_ON_OFF_STATUS,                  1,                 3},
  {GENERIC_ONOFF_SERVER_MODEL_ID,                   GENERIC_ON_OFF_SET_ACK,                    MOBLE_TRUE,  2,                4,                GENERIC_ON_OFF_STATUS,                  1,                 3},  
  {GENERIC_ONOFF_SERVER_MODEL_ID,                   GENERIC_ON_OFF_SET_UNACK,                  MOBLE_FALSE, 2,                4,                GENERIC_ON_OFF_STATUS,                  1,                 3}, 
#endif                                                                                                                                                                                                           
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL                                                                                                                                                                         
  /* Generic Level Server */                                                                                                                                                                                     
  {GENERIC_LEVEL_SERVER_MODEL_ID,                   GENERIC_LEVEL_GET,                         MOBLE_TRUE,   0,                0,               GENERIC_LEVEL_STATUS,                   2,                 5}, 
  {GENERIC_LEVEL_SERVER_MODEL_ID,                   GENERIC_LEVEL_SET_ACK,                     MOBLE_TRUE,   3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_SERVER_MODEL_ID,                   GENERIC_LEVEL_SET_UNACK,                   MOBLE_FALSE,  3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5}, 
  {GENERIC_LEVEL_SERVER_MODEL_ID,                   GENERIC_DELTA_SET,                         MOBLE_TRUE,   5,                7,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_SERVER_MODEL_ID,                   GENERIC_DELTA_SET_UNACK,                   MOBLE_FALSE,  5,                7,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_SERVER_MODEL_ID,                   GENERIC_MOVE_SET,                          MOBLE_TRUE,   3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_SERVER_MODEL_ID,                   GENERIC_MOVE_SET_UNACK,                    MOBLE_FALSE,  3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5},
#endif                                                                                                                                                                                                           
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF                                                                                                                                                                   
  {GENERIC_POWER_ONOFF_SETUP_SERVER_MODEL_ID,       GENERIC_POWER_ON_OFF_SET,                  MOBLE_TRUE,   1,                1,               GENERIC_POWER_ON_OFF_STATUS,            1,                 1},
  {GENERIC_POWER_ONOFF_SETUP_SERVER_MODEL_ID,       GENERIC_POWER_ON_OFF_SET_UNACK,            MOBLE_FALSE,  1,                1,               0,                                      1,                 1},
  {GENERIC_POWER_ONOFF_SERVER_MODEL_ID,             GENERIC_POWER_ON_OFF_GET ,                 MOBLE_TRUE,   0,                0,               GENERIC_POWER_ON_OFF_STATUS,            1,                 1},
#endif                                                                                                                                                                                                           
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME                                                                                                                                                       
  /* Generic Default Transition Time Server Model  */                                                                                                                                                            
  {GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_ID, GENERIC_DEFAULT_TRANSITION_TIME_GET,       MOBLE_TRUE,  0,                0,                GENERIC_DEFAULT_TRANSITION_TIME_STATUS, 1,                 1}, 
  {GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_ID, GENERIC_DEFAULT_TRANSITION_TIME_SET,       MOBLE_TRUE,  1,                1,                GENERIC_DEFAULT_TRANSITION_TIME_STATUS, 1,                 1},
  {GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_ID, GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK, MOBLE_FALSE, 1,                1,                GENERIC_DEFAULT_TRANSITION_TIME_STATUS, 1 ,                1}, 
#endif                                                                                                                                                                                                           
  
  {0}
};

/* Private function prototypes -----------------------------------------------*/
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t* pGeneric_OnOffParam, 
                                                    MOBLEUINT8 OptionalValid, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t* plevelParam, 
                                                    MOBLEUINT8 OptionalValid, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Delta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                                    MOBLEUINT8 OptionalValid, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Move_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                                   MOBLEUINT8 OptionalValid, 
                                                   uint16_t dstPeer, 
                                                   uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Set(Generic_PowerOnOffParam_t* pPowerOnOffParam, 
                                                         MOBLEUINT8 OptionalValid, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex));
WEAK_FUNCTION (void Appli_Generic_Restore_PowerOn_Value(MOBLEUINT8 restoreValue, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Set(Generic_DefaultTransitionParam_t* pDefaultTimeParam, 
                                                                    MOBLEUINT8 OptionalValid, 
                                                                    uint16_t dstPeer, 
                                                                    uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Status(MOBLEUINT8 const *pOnOff_status, 
                                                       MOBLEUINT32 plength, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Status(MOBLEUINT8 const *plevel_status, 
                                                       MOBLEUINT32 plength, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status , 
                                                            MOBLEUINT32 plength, 
                                                            uint16_t dstPeer, 
                                                            uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , 
                                                                       MOBLEUINT32 plength, 
                                                                       uint16_t dstPeer, 
                                                                       uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffValue(MOBLEUINT8* pOnOff_Value, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetPowerOnOffStatus(MOBLEUINT8* pPower_Status, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetDefaultTransitionStatus(MOBLEUINT8* pTransition_Status, 
                                                                     uint16_t dstPeer, 
                                                                     uint8_t elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_GenericClient_Level_Set_Unack(void));

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Generic_OnOff_Set: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_OnOff_Set(MOBLEUINT8 const *pOnOff_param, 
                               MOBLEUINT32 length,
                               MODEL_MessageHeader_t *pmsgParam)  
{
  
  /* 3.2.1.2 Generic OnOff Set 
  OnOff: 1B The target value of the Generic OnOff state 
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 millisecond steps (C.1)
  */
  
  TRACE_M(TF_GENERIC_M, "Generic_OnOff_Set callback received \r\n");  
  
  Generic_OnOffParam_t Generic_OnOffParam; 
  Generic_OnOffParam.TargetOnOffState = pOnOff_param[0];
  Generic_OnOffParam.Generic_TID = pOnOff_param[1];
  Generic_OnOffParam.Transition_Time = 0;
  Generic_OnOffParam.Delay_Time = 0;

  /*  
  Checking for optional parameters
  length > 2 , 4 values  received(OnOff status, TID, Trasmisition time(optional),
  Delay(optional)),length < 2 OnOff status and TID   
  */
  
  if((length > 2) && (pOnOff_param[2] !=0))
  {
    /* Transition_Time & Delay_Time Present */
    Generic_OnOffParam.Transition_Time = pOnOff_param[2];
    Generic_OnOffParam.Delay_Time = pOnOff_param[3];
      
    /* 3.1.1.1 Binary state transitions: Because binary states cannot support transitions, 
       when changing to 0x01 (On), the Generic OnOff state shall change immediately 
       when the transition starts, and when changing to 0x00, the state shall
       change when the transition finishes.*/
    if(pOnOff_param[0] == 1)
    {
      Generic_OnOffStatus[pmsgParam->elementIndex].Present_OnOff_State = pOnOff_param[0];
    }
    Generic_OnOffStatus[pmsgParam->elementIndex].Target_OnOff = Generic_OnOffParam.TargetOnOffState;
    Generic_OnOffStatus[pmsgParam->elementIndex].RemainingTime = Generic_OnOffParam.Transition_Time;   
    
    Generic_TemporaryStatus[pmsgParam->elementIndex].RemainingTime = Generic_OnOffStatus[pmsgParam->elementIndex].RemainingTime;
    /* Function to calculate time parameters, step resolution
      step size for transition state machine
    */
    Generic_GetStepValue(pOnOff_param[2], pmsgParam->elementIndex);   
    /*option parameter flag, enable to send optional parameters in status.*/         
    Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam = 1;
    /*Flag to enable the on Off transition state machine */
    Generic_ModelFlag[pmsgParam->elementIndex].GenericTransitionFlag = GENERIC_ON_OFF_TRANSITION_START;
    /* flag is used for the application to get the information about the transition 
       time parameter is included or not in the received message.
    */
    OptionalParam = IN_TRANSITION;
  } 
  else
  {
    /* when default transition time enabled,and the target time is not given by client
       the transition time will be used from default value
      */
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME     
    Generic_OnOffDefaultTransitionValue(pmsgParam->elementIndex);
/* EME BEGIN: management of Present_OnOff_Value and Target_OnOff on immediate transition */
    if(Generic_TimeParam[pmsgParam->elementIndex].StepValue == 0)
    {
      /* Immediate transition */
      if(Generic_OnOffStatus[pmsgParam->elementIndex].Present_OnOff_State > 0)
      {
        Generic_OnOffStatus[pmsgParam->elementIndex].Present_OnOff_Value = PWM_TIME_PERIOD;
        Generic_OnOffStatus[pmsgParam->elementIndex].Target_OnOff = PWM_TIME_PERIOD;
      }
      else
      {
        Generic_OnOffStatus[pmsgParam->elementIndex].Present_OnOff_Value = 0;
        Generic_OnOffStatus[pmsgParam->elementIndex].Target_OnOff = 0;
      }
      Generic_OnOffStatus[pmsgParam->elementIndex].RemainingTime = Generic_OnOffParam.Transition_Time;   
    }
/* EME END: management of Present_OnOff_Value and Target_OnOff on immediate transition */
#else
    /* When no optional parameter received, target value will be set as present
       value in application.
    */  
    OptionalParam = NO_TRANSITION;
    Generic_TransitionParameterReset(pmsgParam->elementIndex);
#endif       
    
    Generic_OnOffStatus[pmsgParam->elementIndex].Present_OnOff_State = Generic_OnOffParam.TargetOnOffState;
  }
  
  /* Application Callback */
  (GenericAppli_cb.OnOff_Set_cb)(&Generic_OnOffStatus[pmsgParam->elementIndex], OptionalParam, pmsgParam->dst_peer, pmsgParam->elementIndex);
#ifdef ENABLE_MODEL_BINDING    

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
  /* Binding of data b/w Generic on off and Light lightness Actual model */
  GenericOnOff_LightActualBinding(&Generic_OnOffStatus[pmsgParam->elementIndex],pmsgParam->elementIndex,
           Generic_ModelFlag[pmsgParam->elementIndex].GenericTransitionFlag,Generic_ModelFlag[pmsgParam->elementIndex].Generic_Trnsn_Cmplt);
#endif

  /* For corresponding element which supports Light LC Server */
  Binding_GenericOnOff_LightLcLightOnOff(pmsgParam->elementIndex,
                                         Generic_OnOffParam.TargetOnOffState,
                                          0, 0, 0);
  
#endif
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_OnOff_Status
* @param  pOnoff_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @param  dstPeer: *pmsgParam Pointer to structure of message header for parameters:
*                  elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset

* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_OnOff_Status(MOBLEUINT8* pOnOff_status, 
                                  MOBLEUINT32 *plength,
                                  MODEL_MessageHeader_t *pmsgParam) 
{
  /* 
  Following is the status message:
  Present OnOff The present value of the Generic OnOff state. 
  Target OnOff The target value of the Generic OnOff state (optional).
  Remaining Time is transition time. 
  */
  
  TRACE_M(TF_GENERIC_M, ">>>\r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8201! \n\r");
  /* 
  Default value of GenericOptionalParam=0, 
  GenericOptionalParam set equal to 1 in Generic_OnOff_Set for Generic_OnOff_Status 
  */
  if((Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam == 1) || (Generic_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {   
    /*  
    When optional parameter received present value,targert value, remaining time be sent in status message
    length of received data is equal to 4B
    */
    TRACE_M(TF_GENERIC_M, "Generic_OnOff_Status sent with Transition \r\n"); 

    *(pOnOff_status+1) = Generic_OnOffStatus[pmsgParam->elementIndex].Target_OnOff;
    *(pOnOff_status+2) = Generic_OnOffStatus[pmsgParam->elementIndex].RemainingTime;
       *plength = 3; 
    Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam = 0;
  }
  else
  { /* When no optional parameter received, target value will
       be sent in status message.
       length of received data is equal to 2B
    */
    TRACE_M(TF_GENERIC_M, "Generic_OnOff_Status sent without Transition \r\n"); 
    TRACE_M(TF_GENERIC_M, "%d \r\n",*pOnOff_status);
    *plength = 1;
  }
  *pOnOff_status = Generic_OnOffStatus[pmsgParam->elementIndex].Present_OnOff_State; 
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Level_Set: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*         elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Level_Set(const MOBLEUINT8* plevel_param, 
                               MOBLEUINT32 length,
                               MODEL_MessageHeader_t *pmsgParam) 
{
  /*
  3.2.2.2 Generic Level Set
  Level: 2B The target value of the Generic Level state
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
  */
  
  TRACE_M(TF_GENERIC_M, "Generic_Level_Set callback received \r\n");
  
  Generic_LevelParam_t Generic_LevelParam;
  MOBLEINT16 setValue;
  
  Generic_LevelParam.TargetLevel = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)plevel_param);
  Generic_LevelParam.Generic_TID = plevel_param[2]; 
  
  setValue =  Generic_LevelParam.TargetLevel;
  /* Check for Optional Parameters. 
     length > 3  plevel_param has level,TID,Transition Time, Delay      
     length < 3  plevel_param has level,TID
  */ 
  if((length > 3) && (plevel_param[3] !=0))
  {
     Generic_LevelParam.Transition_Time = plevel_param[3];
     Generic_LevelParam.Delay_Time = plevel_param[4];
     /* Copy the data into status message which needs to be update in 
       application message.
     */
    Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16 = setValue;
    Generic_LevelStatus[pmsgParam->elementIndex].RemainingTime = Generic_LevelParam.Transition_Time;
    /* copy status parameters in Temporary parameters for transition 
       process.
    */
    Generic_TemporaryStatus[pmsgParam->elementIndex].TargetValue16 = Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16;
    Generic_TemporaryStatus[pmsgParam->elementIndex].RemainingTime = Generic_LevelStatus[pmsgParam->elementIndex].RemainingTime;
    /* Function to calculate time parameters, step resolution
      step size for transition state machine
    */
    Generic_GetStepValue(plevel_param[3], pmsgParam->elementIndex); 
    /*option parameter flag, enable to sent all required parameter in status.*/
    Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam = 1;
    /*transition process enable flag. */
    Generic_ModelFlag[pmsgParam->elementIndex].GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
  }
  else
  {     
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
    
    Generic_LevelDefaultTransitionValue(pmsgParam->elementIndex, setValue);

#else
     /* When no optional parameter received, target value will
         be set as present value in application.
     */
    Generic_TransitionParameterReset(pmsgParam->elementIndex);
    Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16= setValue;
    Generic_TimeParam[pmsgParam->elementIndex].StepValue = 0;
#endif    
  }  
  Generic_LevelStatus[pmsgParam->elementIndex].Last_Present_Level16 = Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16;
 
  /* Application Callback */
  (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus[pmsgParam->elementIndex], 0, pmsgParam->dst_peer, pmsgParam->elementIndex);
#ifdef ENABLE_MODEL_BINDING       
  /* Binding of Generic level with light lightnes actual */
  GenericLevel_LightBinding(&Generic_LevelStatus[pmsgParam->elementIndex],BINDING_GENERIC_LEVEL_SET, pmsgParam->elementIndex,
        Generic_ModelFlag[pmsgParam->elementIndex].GenericTransitionFlag,Generic_ModelFlag[pmsgParam->elementIndex].Generic_Trnsn_Cmplt);
#endif 
        
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Delta_Set: This function is called for both Acknowledged 
*         and unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Delta_Set(const MOBLEUINT8* plevel_param, MOBLEUINT32 length,\
                               MODEL_MessageHeader_t *pmsgParam) 
{
  /*
  3.2.2.4 Generic Delta Set
  Delta Level: 4B The Delta change of the Generic Level state
  TID:   1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 milliseconds steps (C.1)
  */
  
  TRACE_M(TF_GENERIC_M, "Generic_Delta_Set callback received \r\n");
  
  Generic_DeltaLevelParam_t Generic_DeltaLevelParam ;
  MOBLEUINT32 delta;
  
  /* Copy the 4Bytes data to local variable */
  delta = (plevel_param[3] << 24);
  delta |= (plevel_param[2] << 16);
  delta |= (plevel_param[1] << 8);
  delta |= (plevel_param[0]);
  Generic_DeltaLevelParam.TargetDeltaLevel32 = delta;
  
  TRACE_M(TF_GENERIC_M,"Generic delta value %.2lx \r\n",
          Generic_DeltaLevelParam.TargetDeltaLevel32);
  
  Generic_DeltaLevelParam.Generic_TID = plevel_param[4];
  /* 
     Check for Optional Parameters 
     length > 5  Delata level,TID, Trasition Time(optional),Delay(optional)
     length < 5  Delta level,TID
  */    
  if((length > 5) && (plevel_param[5] !=0))
  {
    Generic_DeltaLevelParam.Transition_Time = plevel_param[5];
    Generic_DeltaLevelParam.Delay_Time = plevel_param[6];
    /* Copy the data into status message which needs to be update in 
     application message.
    */ 
    Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16 = Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16 + 
                                                Generic_DeltaLevelParam.TargetDeltaLevel32;
    Generic_LevelStatus[pmsgParam->elementIndex].RemainingTime = Generic_DeltaLevelParam.Transition_Time;
       
    /* copy status parameters in Temporary parameters for transition 
     process.
    */
    Generic_TemporaryStatus[pmsgParam->elementIndex].TargetValue16 = Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16;
    Generic_TemporaryStatus[pmsgParam->elementIndex].RemainingTime = Generic_LevelStatus[pmsgParam->elementIndex].RemainingTime;
    /* Function to calculate time parameters, step resolution
    step size for transition state machine.
    */
    Generic_GetStepValue(plevel_param[5], pmsgParam->elementIndex);   
       
    /*option parameter flag, enable to sent all required parameter in status.*/ 
    Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam = 1;
       
    /*transition process enable flag. */
    Generic_ModelFlag[pmsgParam->elementIndex].GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
  }
  else
  {   
		
    Generic_TransitionParameterReset(pmsgParam->elementIndex);
    
    if(Generic_LevelStatus[pmsgParam->elementIndex].Last_Level_TID == Generic_DeltaLevelParam.Generic_TID)
    {
        if(Generic_DeltaLevelParam.TargetDeltaLevel32 > Generic_LevelStatus[pmsgParam->elementIndex].Last_delta_level)
      {
          Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16 += (Generic_DeltaLevelParam.TargetDeltaLevel32  
          - Generic_LevelStatus[pmsgParam->elementIndex].Last_delta_level);     
      }
      else
      {
          Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16 -= (Generic_LevelStatus[pmsgParam->elementIndex].Last_delta_level
            -Generic_DeltaLevelParam.TargetDeltaLevel32) ;           
      }       
    }
    else
    {    /*If TID value is different from the last TID, then new transaction has been started*/
        Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16 += Generic_DeltaLevelParam.TargetDeltaLevel32;
    }
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
    
    Generic_DeltaDefaultTransitionValue(pmsgParam->elementIndex, delta);
    
#else
    /* When no optional parameter received, target value will
    be set as present value in application.
    */
	Generic_TransitionParameterReset(pmsgParam->elementIndex);
#endif   
  }
  
  TRACE_M(TF_GENERIC_M, "Generic Level value %.2x \r\n" ,Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16);
  
  Generic_LevelStatus[pmsgParam->elementIndex].Last_delta_level = Generic_DeltaLevelParam.TargetDeltaLevel32;
  Generic_LevelStatus[pmsgParam->elementIndex].Last_Level_TID = Generic_DeltaLevelParam.Generic_TID; 
      
  /* Application Callback */
  (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus[pmsgParam->elementIndex], 0, pmsgParam->dst_peer, pmsgParam->elementIndex);
#ifdef ENABLE_MODEL_BINDING    
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
  /* Binding of Generic level with light lightnes actual */
  GenericLevel_LightBinding(&Generic_LevelStatus[pmsgParam->elementIndex],BINDING_GENERIC_LEVEL_SET,pmsgParam->elementIndex,
         Generic_ModelFlag[pmsgParam->elementIndex].GenericTransitionFlag,Generic_ModelFlag[pmsgParam->elementIndex].Generic_Trnsn_Cmplt);
#endif  
#endif  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Move_Set: This function is called for both 
*         Acknowledged and unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Move_Set(const MOBLEUINT8* plevel_param, 
                              MOBLEUINT32 length,
                              MODEL_MessageHeader_t *pmsgParam) 
{
  /*
  3.2.2.6 Generic Level Move Set
  Level: 2B The target value of the Generic Level state
  TID:   1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 milliseconds steps (C.1)
  */
  TRACE_M(TF_GENERIC_M, "Generic_Move_Set callback received \r\n");
  
  Generic_LevelMoveParam_t  Generic_LevelMoveParam;
  
  Generic_LevelMoveParam.TargetMoveLevel16  = (plevel_param[1] << 8);
  Generic_LevelMoveParam.TargetMoveLevel16 |= (plevel_param[0]);
  Generic_LevelMoveParam.Generic_TID = plevel_param[2];
  
  /* Check for Optional Parameters 
  
    length > 3 plevel_param has level,TID,Trasition Time,Delay
    length < 3 plevel_param has level,TID
  */     
  if((length > 3) && (plevel_param[3] !=0))
  {
    Generic_LevelMoveParam.Transition_Time = plevel_param[3];
    Generic_LevelMoveParam.Delay_Time = plevel_param[4];
    
    /* Copy the data into status message which needs to be update in 
      application message.
    */ 
    /* The assumption here is that the move command starts transition with each 
       step of size delta, which terminates only at max or min value */
    if (Generic_LevelMoveParam.TargetMoveLevel16 <= 0x7FFF)
    {
        Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16 = 0x7FFF;
    }
    else
    {
        Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16 = 0x8000;
    }
    Generic_TemporaryStatus[pmsgParam->elementIndex].TargetValue16 = Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16;
    Generic_TemporaryStatus[pmsgParam->elementIndex].RemainingTime = UNDEFSTEPVAL;
    Generic_LevelStatus[pmsgParam->elementIndex].RemainingTime = UNDEFSTEPVAL;
    
    /* Function to calculate time parameters, step resolution
      step size for transition state machine.
    */
    Generic_GetStepValue(plevel_param[3], pmsgParam->elementIndex);   
    /* option parameter flag, enable to sent all required parameter in status.*/
    Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam = 1;  
    /* transition process enable flag. */
    Generic_ModelFlag[pmsgParam->elementIndex].GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
  }
  else
  {   
    Generic_TransitionParameterReset(pmsgParam->elementIndex);
    
    if(Generic_LevelStatus[pmsgParam->elementIndex].Last_Level_TID == Generic_LevelMoveParam.Generic_TID)
    {
      Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16 =  Generic_LevelStatus[pmsgParam->elementIndex].Last_Present_Level16 
        + Generic_LevelMoveParam.TargetMoveLevel16;
    }
    else
    {
      
    }
    }
    
  Generic_LevelStatus[pmsgParam->elementIndex].Last_Level_TID = Generic_LevelMoveParam.Generic_TID; 
 
  /* Application Callback */
  (GenericAppli_cb.LevelDeltaMove_Set_cb)(&Generic_LevelStatus[pmsgParam->elementIndex], 0, pmsgParam->dst_peer, pmsgParam->elementIndex);
     
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Level_Status
* @param  plevel_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Level_Status(MOBLEUINT8* plevel_status, 
                                  MOBLEUINT32 *plength,
                                  MODEL_MessageHeader_t *pmsgParam) 
{
  /* 
  3.2.2.8 Generic Level Status
  Following is the status message:
  Present Level: 2B The present value of the Generic Level state. 
  Target Level: 2B The target value of the Generic Level state (Optional). 
  Remaining Time: 1B Format as defined in Section 3.1.3 (C.1).
  
  */
  
  TRACE_M(TF_GENERIC_M, "Generic_Level_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8205! \n\r");
  
  /* checking the transition is in process.
  checking for remaining time is not equal to zero.
  */
  
  if((Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam ==1) || (Generic_TimeParam[pmsgParam->elementIndex].StepValue != 0))
  {
    TRACE_M(TF_GENERIC_M, "Generic_Level_Status sent with Transition \r\n"); 

    *(plevel_status+2) = Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16;
    *(plevel_status+3) = Generic_LevelStatus[pmsgParam->elementIndex].Target_Level16 >> 8;
    *(plevel_status+4) = Generic_LevelStatus[pmsgParam->elementIndex].RemainingTime;
    *plength = 5;
    Generic_ModelFlag[pmsgParam->elementIndex].GenericOptionalParam = 0;    
  }
  else
  {
    TRACE_M(TF_GENERIC_M, "Generic_Level_Status sent without Transition \r\n"); 
    *plength = 2;             
  }
     
  *(plevel_status) = Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16;
  *(plevel_status+1) = Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16 >> 8;
  TRACE_M(TF_GENERIC_M, "%d \r\n", Generic_LevelStatus[pmsgParam->elementIndex].Present_Level16); 
  return MOBLE_RESULT_SUCCESS;   
}

/**
* @brief  Generic_PowerOnOff_Set: This function is called for both 
*         Acknowledged and unacknowledged message
* @param  powerOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_PowerOnOff_Set(const MOBLEUINT8 *powerOnOff_param , MOBLEUINT32 length,\
                                   MODEL_MessageHeader_t *pmsgParam) 
{
  /* 
  3.2.4.2 Generic Power On Off Time
  Following is the set message:
  powerOnOff_param:1B parameter is received to set the power on off model.  
  */
  TRACE_M(TF_GENERIC_M, "Generic_PowerOnOff_Set callback received \r\n");
  TRACE_M(TF_GENERIC_M,"Generic_PowerOnOff_Set is %d\r\n", powerOnOff_param[0]);
  
  Generic_PowerOnOffParam_t Generic_PowerOnOffParam[APPLICATION_NUMBER_OF_ELEMENTS];
  
  Generic_PowerOnOffParam[pmsgParam->elementIndex].PowerOnOffState = powerOnOff_param[0];
  
  /* Application Callback */
  (GenericAppli_cb.GenericPowerOnOff_cb)(&Generic_PowerOnOffParam[pmsgParam->elementIndex], length, pmsgParam->dst_peer,\
                                                     pmsgParam->elementIndex);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Generic_PowerOnOff_Status
* @param  powerOnOff_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_PowerOnOff_Status(MOBLEUINT8 *powerOnOff_status , 
                                       MOBLEUINT32 *plength,
                                       MODEL_MessageHeader_t *pmsgParam) 
{  
  /* 
  3.2.4.4 Generic OnPowerUp Status
  Following is the status message:
  powerOnOff_status: 1B is the status parameter of the Power on off model. 
  */  
  MOBLEUINT8 Generic_GetBuff[2] ;
  TRACE_M(TF_GENERIC_M, "Generic_PowerOnOff_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8211! \n\r");
  
  /* Function call back to get the values from application*/
  (Appli_GenericState_cb.GetPowerOnOffStatus_cb)(Generic_GetBuff, pmsgParam->dst_peer,\
                                                        pmsgParam->elementIndex);
   
  *(powerOnOff_status) = Generic_GetBuff[0];
  *plength = 1;
 
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_DefaultTransitionTime_Set: This function is called for both 
*         Acknowledged and unacknowledged message
* @param  defaultTransition_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_DefaultTransitionTime_Set(const MOBLEUINT8 *defaultTransition_param,
                                               MOBLEUINT32 length, 
                                               MODEL_MessageHeader_t *pmsgParam)  
{
  /* 
  3.2.3.2 Generic Default Transition Time Set
  Following is the set message:
  defaultTime_param:1B parameter is received to set the Default transition time model.  
  */
  TRACE_M(TF_GENERIC_M, "Generic_DefaultTransitionTime_Set callback received \r\n"); 
  TRACE_M(TF_GENERIC_M,"Generic_DefaultTransitionTime is = %.2x \r\n", defaultTransition_param[0]); 
  Generic_DefaultTransitionParam.DefaultTransitionTime = defaultTransition_param[0];

  /* Application Callback */
  (GenericAppli_cb.GenericDefaultTransition_cb)(&Generic_DefaultTransitionParam,
                                length, pmsgParam->dst_peer, pmsgParam->elementIndex);
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_DefaultTransitionTime_Status
* @param  pTransition_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_DefaultTransitionTime_Status(MOBLEUINT8 *pTransition_status,\
                          MOBLEUINT32 *plength, MODEL_MessageHeader_t *pmsgParam)
{  
  /* 
  3.2.3.4 Generic Default Transition Time Status
  Following is the status message:
  powerOnOff_status: 1B is the status parameter of the Default transition time model. 
  */  
  MOBLEUINT8 Generic_GetBuff[2] ;
  TRACE_M(TF_GENERIC_M, "Generic_DefaultTransitionTime_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#820D! \n\r");
  
  /* Function call back to get the values from application*/
  (Appli_GenericState_cb.GetDefaultTransitionStatus_cb)(Generic_GetBuff, pmsgParam->dst_peer,\
                                                         pmsgParam->elementIndex);
   
  *(pTransition_status) = Generic_GetBuff[0];
  *plength = 1;
 
   TRACE_M(TF_GENERIC_M,"Generic_DefaultTransitionTime_Status = %.2x \r\n", Generic_GetBuff[0]);
   
 
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief   GenericModelServer_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{
  *data = Generic_Opcodes_Table;
  *length = sizeof(Generic_Opcodes_Table)/sizeof(Generic_Opcodes_Table[0]);

  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  GenericModelServer_GetStatusRequestCb : This function is call-back 
*         from the library to send response to the message from peer
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelServer_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam,
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response)

{
  TRACE_M(TF_GENERIC_M, "response status enable \n\r");
  switch(opcode)
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF      
    case GENERIC_ON_OFF_STATUS:
    {
      Generic_OnOff_Status(pResponsedata, plength, pmsgParam);       
      break;
    }
#endif 
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL      
    case GENERIC_LEVEL_STATUS:
    {
      Generic_Level_Status(pResponsedata, plength, pmsgParam);     
      break;
    }
#endif      
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF    
    case GENERIC_POWER_ON_OFF_STATUS:
    {
      Generic_PowerOnOff_Status(pResponsedata, plength, pmsgParam);
      break;
    }
#endif
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME    
    case GENERIC_DEFAULT_TRANSITION_TIME_STATUS:
    {
      Generic_DefaultTransitionTime_Status(pResponsedata, plength, pmsgParam);
    }
#endif    
    default:
    {
      break;
    }
  }
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief  GenericModelServer_ProcessMessageCb: This is a callback function from
*         the library whenever a Generic Model message is received
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged message or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelServer_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam,
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response)
{
  Model_Binding_Var.Dst_Peer = pmsgParam->dst_peer;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 modelStateChangeFlag = MOBLE_FALSE; 
  
  TRACE_M(TF_GENERIC_M, "elementIndex %.2x dst_peer %.2X peer_add %.2X opcode %.2X response %.2X\r\n",
          pmsgParam->elementIndex, pmsgParam->dst_peer, pmsgParam->peer_addr, opcode, response);   
                                                      
  switch(opcode)
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
    
    case GENERIC_ON_OFF_SET_ACK:
    case GENERIC_ON_OFF_SET_UNACK:
    {
      result = Chk_ParamValidity(pRxData[0], 1); 
      /* 3.1.1 Generic OnOff 0x02–0xFF Prohibited */
      /* 3.2.1.2 Generic OnOff Set If the Transition Time field is present, 
      the Delay field shall also be present; otherwise these fields shall 
      not be present*/
      
      /* 3.2.1.2 Generic OnOff Set 
      Check if Transition Time field is present or Not,
      If present, Only values of 0x00 through 0x3E shall be used to specify 
      the value of the Transition Number of Steps field. */

      result |= Chk_OptionalParamValidity (dataLength, 2, (pRxData[2]&0x3F), 0x3E );        
    
      if(result == MOBLE_RESULT_SUCCESS)
      {
        /* when device is working as proxy and is a part of node
           delay is included in LED toggle */              
        if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr,\
                                             pmsgParam->dst_peer, pRxData[1])))
        {
          Generic_OnOff_Set(pRxData, dataLength, pmsgParam);  
          Model_Binding_Var.Model_ID = (MOBLEUINT16)GENERIC_ONOFF_SERVER_MODEL_ID;
          Model_Binding_Var.Model_Rx_Opcode = opcode;
          modelStateChangeFlag = MOBLE_TRUE;
          Publication1SecFlag.count = 0;
        }
      }
        
      break;
    }
#endif 
      
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
  
    case GENERIC_LEVEL_SET_ACK:
    case GENERIC_LEVEL_SET_UNACK: 
    {   
       
      result = Chk_ParamMinMaxIntValidity(INT16_MIN ,pRxData , INT16_MAX );        
      if(result == MOBLE_RESULT_SUCCESS)
      {
        if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr, 
                                                  pmsgParam->dst_peer,
                                                  pRxData[2])))
        {
          Generic_Level_Set(pRxData, dataLength, pmsgParam);  
          Model_Binding_Var.Model_ID = (MOBLEUINT16)GENERIC_LEVEL_SERVER_MODEL_ID;
          Model_Binding_Var.Model_Rx_Opcode = opcode;
          modelStateChangeFlag = MOBLE_TRUE;
          Publication1SecFlag.count = 0;
        }
      }
       
      break;
    }
    
    case GENERIC_DELTA_SET:
    case GENERIC_DELTA_SET_UNACK:
    { 
      /*
        Delta Level 4 The Delta change of the Generic Level state 
        TID 1 Transaction Identifier 
        Transition Time 1 Format as defined in Section 3.1.3. (Optional) 
        Delay 1 Message execution delay in 5 milliseconds steps (C.1)
      */
      Generic_Delta_Set(pRxData,dataLength, pmsgParam);   
      Model_Binding_Var.Model_ID = (MOBLEUINT16)GENERIC_LEVEL_SERVER_MODEL_ID;
      Model_Binding_Var.Model_Rx_Opcode = opcode;
      modelStateChangeFlag = MOBLE_TRUE;
      Publication1SecFlag.count = 0;
      break;
    }
      
    case GENERIC_MOVE_SET:
    case GENERIC_MOVE_SET_UNACK:
    {    
       
      result = Chk_ParamMinMaxIntValidity(INT16_MIN ,pRxData , INT16_MAX );       
      if(result == MOBLE_RESULT_SUCCESS)
      {
        if(!MOBLE_FAILED(result = Chk_TidValidity(pmsgParam->peer_addr, 
                                                  pmsgParam->dst_peer,
                                                  pRxData[2])))
        {
          Generic_Move_Set(pRxData, dataLength, pmsgParam);  
        }
      }
      break;
    }
#endif        
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
    
    case GENERIC_POWER_ON_OFF_SET: 
    case GENERIC_POWER_ON_OFF_SET_UNACK:
    {
      result = Chk_ParamValidity(pRxData[0], GENERIC_POWER_RESTORE_STATE); 
      if(result == MOBLE_RESULT_SUCCESS)
      {
        Generic_PowerOnOff_Set(pRxData, dataLength, pmsgParam); 
        Model_Binding_Var.Model_ID = (MOBLEUINT16)GENERIC_POWER_ONOFF_SERVER_MODEL_ID;
        Model_Binding_Var.Model_Rx_Opcode = opcode;
        modelStateChangeFlag = MOBLE_TRUE;
        Publication1SecFlag.count = 0;
      }
      break;
    }
#endif 

#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME 
    
    case GENERIC_DEFAULT_TRANSITION_TIME_SET:
    case GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK:
    {
      result = Chk_ParamValidity(pRxData[0], TRANSITION_MAX_VALID_RANGE); 
      if(result == MOBLE_RESULT_SUCCESS)
      {
        Generic_DefaultTransitionTime_Set(pRxData, dataLength, pmsgParam);  
        Model_Binding_Var.Model_ID = (MOBLEUINT16)GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_ID;
        modelStateChangeFlag = MOBLE_TRUE;
      }
      break;
    }
  
#endif    
    
    default:
    {
      break;
    }          
  } /* Switch ends */
          
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    if(ADDRESS_IS_UNICAST(pmsgParam->dst_peer))
    {
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
    }
    else
    {

      pmsgParam->dst_peer = BLEMesh_GetAddress();       // Replace group address by the single node address for response
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
    }
  }
  /*
     Publish the status to publish address if the publication is set by client 
     publication is independent of the response of the message.if thek condition 
     for publication is full filled as per specification then the status will be 
     published.
  */
  publishAddress = BLEMesh_GetPublishAddress(pmsgParam->elementIndex, Model_Binding_Var.Model_ID);
    
  if((result == MOBLE_RESULT_SUCCESS) && 
     (publishAddress != 0x0000) && 
       (modelStateChangeFlag == MOBLE_TRUE))
  {
    if(ADDRESS_IS_UNICAST(pmsgParam->dst_peer))
    {
      pmsgParam->peer_addr = publishAddress;
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
    }
        else
    {

      pmsgParam->peer_addr = publishAddress;
      pmsgParam->dst_peer = BLEMesh_GetAddress();       // Replace group address by the single node address for response
      Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
    }
                
      modelStateChangeFlag = MOBLE_FALSE;   
      TRACE_I(TF_GENERIC_M,"Publishing state when change to the address %.2X \r\n",publishAddress);
  }
  return MOBLE_RESULT_SUCCESS;
}


/* @Brief  Generic_TransitionBehaviour: Generic On Off Transition behaviour 
*          used for the Generic On Off model when transition time is received in
*          message.        
* @param GetValue Pointer of the array
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Generic_TransitionBehaviour(MOBLEUINT8 *GetValue, 
                                         MOBLEUINT8 elementIndex)
{
  
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;
  MOBLEUINT16 targetRange;
  MOBLEUINT16 targetSlot;
  
  /* Taking the time stamp for particular time */
  if(Clockflag == 0)
  {
    Check_time = Clock_Time();
    Clockflag = 1;
  }
  
  /* Publication after 1 second if total transition time is greater than 2 sec.*/
  if(Generic_TimeParam[elementIndex].TotalTime >2000)
  {
    Punblication_OneSecTimer();
  }
  /* Values from application are copied into Temporary variables for processing */
  
  Generic_TemporaryStatus[elementIndex].PresentValue16  = GetValue[1] << 8;
  Generic_TemporaryStatus[elementIndex].PresentValue16 |= GetValue[0];
  /* if condition to wait until the time is equal to the given resolution time */
  if(((Clock_Time()- Check_time) >= Generic_TimeParam[elementIndex].Res_Value))
  {     
    if(Generic_TimeParam[elementIndex].StepValue == 0)
    {
      Generic_TimeParam[elementIndex].StepValue = 1;
    }

    if(Generic_TemporaryStatus[elementIndex].TargetValue16 > Generic_TemporaryStatus[elementIndex].PresentValue16)
    {
      Generic_OnOffStatus[elementIndex].Present_OnOff_State = 1;
      /* target range = total range to be covered */
      targetRange = 
        Generic_TemporaryStatus[elementIndex].TargetValue16 - Generic_TemporaryStatus[elementIndex].PresentValue16;
      
      /* target slot = time to cover in single step */
      targetSlot = targetRange / Generic_TimeParam[elementIndex].StepValue;
      /* target slot added to present value to achieve target value */
      Generic_TemporaryStatus[elementIndex].PresentValue16 += targetSlot;      
    }              
    else if(Generic_TemporaryStatus[elementIndex].TargetValue16 < Generic_TemporaryStatus[elementIndex].PresentValue16)
    {  
      Generic_OnOffStatus[elementIndex].Present_OnOff_State = 0;
      /* condition execute when transition is negative */
      /* target range = total range to be covered */ 
      targetRange = Generic_TemporaryStatus[elementIndex].PresentValue16 - Generic_TemporaryStatus[elementIndex].TargetValue16;
      /* target slot = time to cover in single step */
      targetSlot = targetRange / Generic_TimeParam[elementIndex].StepValue;
      /* target slot = time to cover in single step */
      Generic_TemporaryStatus[elementIndex].PresentValue16 -= targetSlot; 
    }     
    else
    {
      
    }
    Generic_TimeParam[elementIndex].StepValue--;
    /* updating the remaining time after each step covered*/
    Generic_TemporaryStatus[elementIndex].RemainingTime = Generic_TimeParam[elementIndex].StepValue | (Generic_TimeParam[elementIndex].ResBitValue << 6) ;
   
    Check_time = 0;
    Clockflag = 0;
    GenericUpdateFlag = VALUE_UPDATE_SET;
    /* when transition is completed, disable the transition by disabling 
       transition flag
    */
    if(Generic_TimeParam[elementIndex].StepValue <= 0)
    {
      if((Generic_OnOffStatus[elementIndex].Present_OnOff_State == 1) && (Generic_OnOffStatus[elementIndex].Target_OnOff == 0))
      {
        Generic_OnOffStatus[elementIndex].Present_OnOff_State=0;
      }
      Generic_ModelFlag[elementIndex].GenericTransitionFlag = MODEL_TRANSITION_STOP; 
      Generic_ModelFlag[elementIndex].Generic_Trnsn_Cmplt = MOBLE_TRUE;
      Publication1SecFlag.TimeStampFlag = MOBLE_FALSE;  
    }
    TRACE_M(TF_GENERIC_M, "Inside virtual application at %ld, Current state 0x%.2x, Target state 0x%.2x, Remaining Time 0x%.2x \n\r",
            Clock_Time(), Generic_TemporaryStatus[elementIndex].PresentValue16,Generic_TemporaryStatus[elementIndex].TargetValue16,Generic_TemporaryStatus[elementIndex].RemainingTime);                     
  }
  return MOBLE_RESULT_SUCCESS;       

} 


/** 
* @brief Generic_GetStepValue: This function calculates values for transition time
* @param stepParam: Transition time set value of particular model message.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* retval void
*/
void Generic_GetStepValue(MOBLEUINT8 stepParam, 
                          MOBLEUINT8 elementIndex)
{
  /*
    Two MSB bit of transition time is dedicated to resolution.
    00 = resolution is 100 ms.
    01 = resolution is 1000 ms. 
    10 = resolution is 10000 ms.
    11 = resolution is 600000 ms. 
    Last bits from 0 to 5th index is step number.
  */
  
  Generic_TimeParam[elementIndex].ResBitValue = stepParam >> 6 ;
  Generic_TimeParam[elementIndex].Res_Value = Get_StepResolutionValue(Generic_TimeParam[elementIndex].ResBitValue);
  Generic_TimeParam[elementIndex].StepValue = stepParam & 0x3F ;
  Generic_TimeParam[elementIndex].TotalTime = (Generic_TimeParam[elementIndex].StepValue * Generic_TimeParam[elementIndex].Res_Value);
  
  if(Generic_TimeParam[elementIndex].Res_Value >= 100)
  {
    Generic_TimeParam[elementIndex].Res_Value = Generic_TimeParam[elementIndex].Res_Value/TRANSITION_SCALER;
    Generic_TimeParam[elementIndex].StepValue = (Generic_TimeParam[elementIndex].StepValue * TRANSITION_SCALER);
  }
  
  TRACE_M(TF_GENERIC_M," step resolution 0x%.2lx, number of step 0x%.2x \r\n",
          Generic_TimeParam[elementIndex].Res_Value , Generic_TimeParam[elementIndex].StepValue );   
}


/**
* @brief  Generic_Process: Function to execute the transition state machine for
*         particular Generic Model
* @param  void
* @retval void
*/ 
void Generic_Process(void)
{       
  MOBLE_ADDRESS publishAddress;
  MOBLE_ADDRESS my_Address = BLEMesh_GetAddress();
  MOBLEUINT8 elementIndex;
  MOBLEUINT8 const pRxData[8] = {0};
  MOBLEUINT32 dataLength = 0;
  
#if defined ENABLE_GENERIC_MODEL_SERVER_ONOFF || defined ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  MOBLEUINT8 Generic_GetBuff[8]; 
#endif     
  
  for(elementIndex = 0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++)
  {
    my_Address = my_Address+elementIndex;
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF   
    if(Generic_ModelFlag[elementIndex].GenericTransitionFlag == GENERIC_ON_OFF_TRANSITION_START)
    {   
      /* Fetching the state value from the application layer to middle layer */
      (Appli_GenericState_cb.GetOnOffValue_cb)(Generic_GetBuff, my_Address, elementIndex); 
      
      Generic_TemporaryStatus[elementIndex].TargetValue16  = Generic_GetBuff[3] << 8;
      Generic_TemporaryStatus[elementIndex].TargetValue16 |= Generic_GetBuff[2];
      
      /* Transition function called to change the state in transition */
      Generic_TransitionBehaviour(Generic_GetBuff, elementIndex); 
      if(GenericUpdateFlag == VALUE_UPDATE_SET)
      {
        /* updating the state value after the each transition step complete */
        Generic_OnOffStateUpdate_Process(elementIndex);
        /* Application callback */
        (GenericAppli_cb.OnOff_Set_cb)(&Generic_OnOffStatus[elementIndex], OptionalParam, my_Address, elementIndex);  
        /*binding of the generic on off with light model */
        GenericOnOff_LightActualBinding(&Generic_OnOffStatus[elementIndex],
                                        elementIndex,
                                        Generic_ModelFlag[elementIndex].GenericTransitionFlag,
                                        Generic_ModelFlag[elementIndex].Generic_Trnsn_Cmplt);
        GenericUpdateFlag = VALUE_UPDATE_RESET;     
      }
    }    
#endif 
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL       
    if(Generic_ModelFlag[elementIndex].GenericTransitionFlag == GENERIC_LEVEL_TRANSITION_START)
    {    
      /* Fetching the state value from the application layer to middle layer */
      (Appli_GenericState_cb.GetLevelStatus_cb)(Generic_GetBuff, my_Address, elementIndex); 
      /* Transition function called to change the state in transition */
      Generic_TransitionBehaviour(Generic_GetBuff, elementIndex); 
      if(GenericUpdateFlag == VALUE_UPDATE_SET)
      {
        /* updating the state value after the each transition step complete */
        Generic_LevelStateUpdate_Process(elementIndex);
        /* Application callback */
        (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus[elementIndex], 0, my_Address, elementIndex);  
        /*binding of the generic Level with light model */
        GenericLevel_LightBinding(&Generic_LevelStatus[elementIndex],BINDING_GENERIC_LEVEL_SET,
                                  elementIndex,
                                  Generic_ModelFlag[elementIndex].GenericTransitionFlag,
                                  Generic_ModelFlag[elementIndex].Generic_Trnsn_Cmplt);
        GenericUpdateFlag = VALUE_UPDATE_RESET;
      }
      
    }   
#endif
  
    if(Generic_ModelFlag[elementIndex].Generic_Trnsn_Cmplt == MOBLE_TRUE)
    {         
      publishAddress = BLEMesh_GetPublishAddress(elementIndex,Model_Binding_Var.Model_ID);
      if((publishAddress != 0x00))
      {

         MODEL_MessageHeader_t msgHdr;
         msgHdr.dst_peer = my_Address;
         msgHdr.elementIndex = elementIndex;
         msgHdr.peer_addr = publishAddress;
         Model_SendResponse(&msgHdr, Model_Binding_Var.Model_Rx_Opcode, pRxData, dataLength);
         TRACE_M(TF_LIGHT_M, "Publishing the status after transition complete opcode %.2x model id %.2x \r\n",
         Model_Binding_Var.Model_Rx_Opcode,Model_Binding_Var.Model_ID);
      }
      Generic_ModelFlag[elementIndex].Generic_Trnsn_Cmplt = MOBLE_FALSE;
    }
  }
}

/**
* @brief Generic_OnOffStateUpdate_Process:Function to update the parameters of 
*        Generic On Off model in application file from Temporary parameter in model file.
* @param void
* return MOBLE_RESULT.
*/
MOBLE_RESULT Generic_OnOffStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Generic_OnOffStatus[elementIndex].Present_OnOff_Value = Generic_TemporaryStatus[elementIndex].PresentValue16;
  if(Generic_TemporaryStatus[elementIndex].TargetValue16 == 0x01)
  {
    Generic_OnOffStatus[elementIndex].Present_OnOff_State = 0x01;
  }
  else
  {
    if(Generic_OnOffStatus[elementIndex].Present_OnOff_Value == 0x00)
    {
      Generic_OnOffStatus[elementIndex].Present_OnOff_State = 0x00;
    }
  }
  Generic_OnOffStatus[elementIndex].RemainingTime = Generic_TemporaryStatus[elementIndex].RemainingTime;
  
  return MOBLE_RESULT_SUCCESS;
}


 /**
* @brief  Generic_Publish: Publish command for Generic Model used while long prees
*         button.
* @param  srcAddress: Source Address of the node 
* @retval void
*/ 
void Generic_Publish(MOBLE_ADDRESS srcAddress)
{
  MOBLEUINT8 generic_Buff[2]; 
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* changes the LED status on other nodes in the network */
  if(CommandStatus == (MOBLEUINT16)APPLI_LED_ON)
  {
    generic_Buff[0] = APPLI_LED_OFF;
  }
  else
  {
    generic_Buff[0] = APPLI_LED_ON;
  }
  generic_Buff[1] = Model_Tid.TidSend;
  
  result = BLEMesh_SetRemotePublication(GENERIC_ONOFF_SERVER_MODEL_ID, 
                                        srcAddress ,
                                        GENERIC_ON_OFF_SET_UNACK, 
                                        generic_Buff, 2,
                                        MOBLE_FALSE, MOBLE_FALSE);
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_M, "Publication Error \r\n");
  }
  
  CommandStatus = generic_Buff[0];
}


/**
* @brief Generic_LevelStateUpdate_Process:function to update the parameters of Generic 
*        Level model in application file from Temporary parameter in model file.
* @param void
* return MOBLE_RESULT.
*/
MOBLE_RESULT Generic_LevelStateUpdate_Process(MOBLEUINT8 elementIndex)
{
  Generic_LevelStatus[elementIndex].Present_Level16 = Generic_TemporaryStatus[elementIndex].PresentValue16;
  Generic_LevelStatus[elementIndex].Target_Level16  = Generic_TemporaryStatus[elementIndex].TargetValue16;
  Generic_LevelStatus[elementIndex].RemainingTime   = Generic_TemporaryStatus[elementIndex].RemainingTime;
   
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  LightActual_GenericOnOffBinding: Reverse Data binding b/w Generic On Off and 
*         light lightness Actual. this function will set the On Off status of
*         Light when the light lightness Actual is set.
* @param lightActual: used to select the binding and reverse binding 
* return void.
*/
void LightActual_GenericOnOffBinding(Light_LightnessStatus_t* lightActual,
                                     MOBLEUINT8 elementIndex)
{
  /*
  6.1.2.2.3 - Binding of actual light lightness with generic on off,
  As actual lightness will set which changes the on off state in 
  generic model
  */

  if(lightActual->LightnessPresentValue16 == 0x00)
  {
    Generic_OnOffStatus[elementIndex].Present_OnOff_State = 0x00;
  }
  else
  {
    Generic_OnOffStatus[elementIndex].Present_OnOff_State = 0x01;
  }
  
  TRACE_M(TF_LIGHT_M, 
          "Light Actual binding with Generic On Off - Present OnOff value 0x%.2x \r\n", 
          Generic_OnOffStatus[elementIndex].Present_OnOff_State);    
 
}


/**
* @brief  LightActual_GenericLevelBinding: Data binding b/w Generic Level and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic Level set. 
* @param lightActual: Pointer to the data which needs to be checked.
* return void.
*/
void LightActual_GenericLevelBinding(Light_LightnessStatus_t* lightActual,
                                     MOBLEUINT8 elementIndex)
{ 
  /*
  6.1.2.2.2 - Binding of actual light lightness with generic level
  As Actual lightness changes, the generic level value will
  change.
     */

  Generic_LevelStatus[elementIndex].Present_Level16 = lightActual->LightnessPresentValue16 - 32768;  
 
  TRACE_M(TF_LIGHT_M, 
          "Light Actual binding with Generic Level - Present Level value 0x%.2x \r\n", 
          Generic_LevelStatus[elementIndex].Present_Level16); 
      
}


/**
* @brief  Light_CtlTemp_GenericLevelBinding: Data binding b/w Generic Level and 
*         light Ctl temperature . this function will set the generic Level
*         value at the time of  Ctl temperature value set. 
* @param bCtlTempParam: Pointer to the data which needs to be checked.
* return void.
*/
void Light_CtlTemp_GenericLevelBinding(Light_CtlStatus_t* bCtlTempParam, MOBLEUINT8 elementIndex,MOBLEUINT8 trnsnFlag,MOBLEUINT8 trnsnCmplt)
{ 
  /* 6.1.3.1.1 Binding with the Generic Level state.
  Generic Level = (Light CTL Temperature - T _MIN) * 65535 / (T_MAX - T_MIN) - 32768
  T_MIN = minimum range of ctl temperature
  T_MAX = maximum range of ctl temperature
  */
  MOBLEUINT32 productValue;
  productValue = (bCtlTempParam->PresentCtlTemperature16 - MIN_CTL_TEMP_RANGE) * 65535;
  TRACE_M(TF_GENERIC_M, "\r\n ******** PresentCtlTemperature16 = %d,  ********\r\n\r\n", bCtlTempParam->PresentCtlTemperature16);
  TRACE_M(TF_GENERIC_M, "\r\n ******** productValue = %ld,  ********\r\n\r\n", productValue); 
 
  Generic_LevelStatus[elementIndex].Present_Level16 = (MOBLEINT16)(round((productValue / (float)(MAX_CTL_TEMP_RANGE - MIN_CTL_TEMP_RANGE))) - 32768);
  TRACE_M(TF_GENERIC_M, "\r\n ********, Present Level value %d ********\r\n\r\n" ,Generic_LevelStatus[elementIndex].Present_Level16); 
  GenericLevel_LightBinding(&Generic_LevelStatus[elementIndex] , BINDING_LIGHT_CTL_TEMP_SET,elementIndex,
                                                          trnsnFlag,trnsnCmplt);
}

/**
* @brief  Light_HslHue_GenericLevelBinding: Data binding b/w Generic Level and 
*         light Hsl . this function will set the Generic Level
*         value at the time of  Hsl Hue value set. 
* @param bHslHueParam: Pointer to the data which needs to be checked.
* return void.
*/
void Light_HslHue_GenericLevelBinding(Light_HslStatus_t* bHslHueParam,
                                      MOBLEUINT8 elementIndex)
{ 
  /*  6.1.4.1.1 Binding with the Generic Level state
  Generic Level = Light HSL Hue - 32768
  */
  Generic_LevelStatus[elementIndex].Present_Level16 = (MOBLEINT16)(bHslHueParam->PresentHslHueLightness16 - 32768);
 
  GenericLevel_LightBinding(&Generic_LevelStatus[elementIndex], BINDING_LIGHT_HSL_HUE_SET, elementIndex,
                            Generic_ModelFlag[elementIndex].GenericTransitionFlag,Generic_ModelFlag[elementIndex].Generic_Trnsn_Cmplt);
}


/**
* @brief  Light_HslSaturation_GenericLevelBinding: Data binding b/w Generic Level and 
*         light Hsl . this function will set the Generic Level
*         value at the time of  Hsl Saturation value set. 
* @param bHslSatParam: Pointer to the data which needs to be checked.
* return void.
*/
void Light_HslSaturation_GenericLevelBinding(Light_HslStatus_t* bHslSatParam,
                                             MOBLEUINT8 elementIndex)
{
  /* 6.1.4.4.1 Binding with the Generic Level state
  Generic Level = Light HSL Saturation – 32768
  */ 
 
  Generic_LevelStatus[elementIndex].Present_Level16 = (MOBLEINT16)(bHslSatParam->PresentHslSaturation16 - 32768);
  
  GenericLevel_LightBinding(&Generic_LevelStatus[elementIndex], BINDING_LIGHT_HSL_SATURATION_SET, elementIndex,
                            Generic_ModelFlag[elementIndex].GenericTransitionFlag,Generic_ModelFlag[elementIndex].Generic_Trnsn_Cmplt);
 
}


/**
* @brief function to assign the Pwm value to the target value of the generic on off 
*        saved states.
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Generic_OnOffDefaultTransitionValue(MOBLEUINT8 elementIndex)

{
  Generic_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime, elementIndex);
  TRACE_M(TF_GENERIC_M, "Generic_OnOff_DefaultTransition StepValue is = %.2x \r\n", Generic_TimeParam[elementIndex].StepValue); 

  if(Generic_TimeParam[elementIndex].StepValue != 0x00)
  {
    Generic_TemporaryStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime; 
    Generic_ModelFlag[elementIndex].GenericOptionalParam = 1;
    Generic_ModelFlag[elementIndex].GenericTransitionFlag = GENERIC_ON_OFF_TRANSITION_START;
    OptionalParam = DEFAULT_TRANSITION;
  }
  else
  {
    Generic_TimeParam[elementIndex].StepValue = 0;
    Generic_ModelFlag[elementIndex].GenericOptionalParam = 0;
    OptionalParam = NO_TRANSITION;
    Generic_ModelFlag[elementIndex].GenericTransitionFlag = MODEL_TRANSITION_STOP;
  }

}


/**
* @brief function called in generic level when the default transition time is enabled.
* @param levelValue: generic level target value
* @param elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Generic_LevelDefaultTransitionValue(MOBLEUINT8 elementIndex, MOBLEUINT16 levelValue)
{
   Generic_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime, elementIndex);
   TRACE_M(TF_GENERIC_M, "Generic_Level_DefaultTransition StepValue is =%.2x \r\n", Generic_TimeParam[elementIndex].StepValue);

   if(Generic_TimeParam[elementIndex].StepValue != 0x00){
    Generic_TemporaryStatus[elementIndex].TargetValue16 = levelValue;
    Generic_TemporaryStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime;
    Generic_ModelFlag[elementIndex].GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
    Generic_ModelFlag[elementIndex].GenericOptionalParam = 1;
  }
  else{
    Generic_TimeParam[elementIndex].StepValue = 0;
    Generic_ModelFlag[elementIndex].GenericOptionalParam = 0;
    Generic_LevelStatus[elementIndex].Present_Level16= levelValue;
    Generic_ModelFlag[elementIndex].GenericTransitionFlag = MODEL_TRANSITION_STOP;
  }
}


/**
* @brief function called in generic delta when the default transition time is enabled.
* @param deltaValue: generic delta value
* @param elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return void.
*/
void Generic_DeltaDefaultTransitionValue(MOBLEUINT8 elementIndex, MOBLEUINT32 deltaValue)
{
   Generic_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime, elementIndex);
   TRACE_M(TF_GENERIC_M, "Generic_Delta_DefaultTransition StepValue is =%.2x \r\n", Generic_TimeParam[elementIndex].StepValue);

   if(Generic_TimeParam[elementIndex].StepValue != 0x00){

    Generic_TemporaryStatus[elementIndex].TargetValue16 = Generic_LevelStatus[elementIndex].Present_Level16 + 
                                                                     deltaValue;
    Generic_TemporaryStatus[elementIndex].RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime;
    Generic_ModelFlag[elementIndex].GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
    Generic_ModelFlag[elementIndex].GenericOptionalParam = 1;
  }
  else{
    Generic_TimeParam[elementIndex].StepValue = 0;
    Generic_ModelFlag[elementIndex].GenericOptionalParam = 0;
    Generic_ModelFlag[elementIndex].GenericTransitionFlag = MODEL_TRANSITION_STOP;
  }
}

/**
* @brief Function called when message received without optional parameter, stop the running transition.
* @param void
* @param void
*/
void Generic_TransitionParameterReset(MOBLEUINT8 elementIndex)
{
   Generic_ModelFlag[elementIndex].GenericTransitionFlag = MODEL_TRANSITION_STOP;
   Generic_TimeParam[elementIndex].Res_Value = 0x00;
   Generic_TimeParam[elementIndex].StepValue = 0X00;
}

/**
  * @brief  Update Generic OnOff state of given element index
  * @param  Element index
  * @param  OnOff state
  * @retval None
  */
void Generic_OnOffUpdate(MOBLEUINT8 elementIndex, MOBLEUINT8 onOff)
{
  TRACE_M(TF_GENERIC_M, "Element index %d onOff %d", elementIndex, onOff);
  
  Generic_OnOffStatus[elementIndex].Present_OnOff_State = onOff;
  
  BLEMesh_SetRemotePublication(GENERIC_ONOFF_SERVER_MODEL_ID,
                                   BLEMesh_GetAddress() + elementIndex,
                                   (MOBLEUINT16)GENERIC_ON_OFF_STATUS,
                                   &Generic_OnOffStatus[elementIndex].Present_OnOff_State,
                                   1,
                                   MOBLE_FALSE,
                                   MOBLE_FALSE);
}

/**
* @brief function used to restore the states of the generic model.
* @param pointer to the array
* return void.
*/
void Generic_RestoreModelStates(void *model_State , MOBLEUINT8 elementIndex)
{
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF    
  Generic_OnOffStatus[elementIndex].Present_OnOff_State = ((Models_ParamNvm_t *)model_State)->Generic_OnOff;
  Generic_OnOffStatus[elementIndex].Target_OnOff = ((Models_ParamNvm_t *)model_State)->GenericTarget;
#endif
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL 
  if(((Models_ParamNvm_t *)model_State)->Generic_Level == 0x00)
  {
    Generic_LevelStatus[elementIndex].Present_Level16 = LEVEL_MIN_VALID_RANGE;
  }
  else
  {
    Generic_LevelStatus[elementIndex].Present_Level16 = ((Models_ParamNvm_t *)model_State)->Generic_Level;
  }
#endif 

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF  
  Generic_PowerOnOffParam_t Generic_PowerOnOffParam[APPLICATION_NUMBER_OF_ELEMENTS];
  Generic_PowerOnOffParam[elementIndex].PowerOnOffState = ((Models_ParamNvm_t *)model_State)->PowerOnOff;
  (GenericAppli_cb.GenericPowerOnOff_cb)(&Generic_PowerOnOffParam[elementIndex], 1, 0,elementIndex);
                                                     
#endif  
}

/**
* @brief function used to save the states of the generic model.
* @param pointer to the array
* return void.
*/
void Generic_SaveModelStates(void *model_State,MOBLEUINT8 elementIndex)
{
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF    
  ((Models_ParamNvm_t *)model_State)->Generic_OnOff = Generic_OnOffStatus[elementIndex].Present_OnOff_State;
  ((Models_ParamNvm_t *)model_State)->GenericTarget = Generic_OnOffStatus[elementIndex].Target_OnOff;
#endif
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  ((Models_ParamNvm_t *)model_State)->Generic_Level = Generic_LevelStatus[elementIndex].Present_Level16;
#endif 
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF  
  MOBLEUINT8 powerOnOff_State[APPLICATION_NUMBER_OF_ELEMENTS];
  (Appli_GenericState_cb.GetPowerOnOffStatus_cb)(&powerOnOff_State[elementIndex],0,elementIndex);
  ((Models_ParamNvm_t *)model_State)->PowerOnOff = powerOnOff_State[elementIndex];
#endif  
  ((Models_ParamNvm_t *)model_State)->GenericTransitionStatus = Generic_ModelFlag[elementIndex].GenericTransitionFlag;
  
}
/**
* Weak function are defined to support the original function if they are not
   included in firmware.
   There is no use of this function for application development purpose.
*/
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t* pGeneric_OnOffParam, 
                                                    MOBLEUINT8 OptionalValid, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t* plevelParam, 
                                                    MOBLEUINT8 OptionalValid, 
                                                    uint16_t dstPeer, 
                                                    uint8_t elementIndex))
{   return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Delta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                                         MOBLEUINT8 OptionalValid, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex))
{   return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Move_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                                        MOBLEUINT8 OptionalValid, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Set(Generic_PowerOnOffParam_t* pPowerOnOffParam, 
                                                        MOBLEUINT8 OptionalValid, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex))
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (void Appli_Generic_Restore_PowerOn_Value(MOBLEUINT8 restoreValue, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex))
{}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Set(Generic_DefaultTransitionParam_t* pDefaultTimeParam, 
                                                                    MOBLEUINT8 OptionalValid, 
                                                                    uint16_t dstPeer, 
                                                                    uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Status(MOBLEUINT8 const *pOnOff_status, 
                                                       MOBLEUINT32 plength, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Status(MOBLEUINT8 const *plevel_status, 
                                                       MOBLEUINT32 plength, 
                                                       uint16_t dstPeer, 
                                                       uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status, 
                                                            MOBLEUINT32 plength, 
                                                            uint16_t dstPeer, 
                                                            uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status,
                                                                       MOBLEUINT32 plength, 
                                                                       uint16_t dstPeer, 
                                                                       uint8_t elementIndex))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffValue(MOBLEUINT8* pOnOff_Value, 
                                                        uint16_t dstPeer, 
                                                        uint8_t elementIndex))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status, 
                                                         uint16_t dstPeer, 
                                                         uint8_t elementIndex))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetPowerOnOffStatus(MOBLEUINT8* pPower_Status, 
                                                              uint16_t dstPeer, 
                                                              uint8_t elementIndex))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetDefaultTransitionStatus(MOBLEUINT8* pTransition_Status, 
                                                                     uint16_t dstPeer, 
                                                                     uint8_t elementIndex))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLE_RESULT Appli_GenericClient_Level_Set_Unack(void))
{return MOBLE_RESULT_SUCCESS;}

/**
* @}
*/

/**
* @}
*/



