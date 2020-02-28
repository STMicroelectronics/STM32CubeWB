/**
******************************************************************************
* @file    generic.c
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Generic model middleware file
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
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "common.h"
#include <string.h>
#include "compiler.h"


/** @addtogroup MODEL_GENERIC
*  @{
*/

/** @addtogroup Generic_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static Generic_TemporaryStatus_t Generic_TemporaryStatus;

static Generic_TimeParam_t Generic_TimeParam;

static Generic_LevelStatus_t Generic_LevelStatus;
 
static Generic_OnOffStatus_t Generic_OnOffStatus;

Generic_DefaultTransitionParam_t Generic_DefaultTransitionParam = {0x06};

static Generic_ModelFlag_t Generic_ModelFlag;

extern MOBLEUINT16 CommandStatus;
MOBLEUINT8 GenericUpdateFlag = 0;
MOBLEUINT8 Generic_Trnsn_Cmplt = 0;
MOBLEUINT16 Generic_Rx_Opcode;
MOBLEUINT8 OptionalValid = 0;
MOBLEUINT8 PowerOnOff_Flag = 0;
MOBLEUINT8 TidValue = 0;
MOBLEUINT16 Model_ID = 0;
MOBLE_ADDRESS Dst_Peer;
extern MOBLEUINT8 TidSend;

const MODEL_OpcodeTableParam_t Generic_Opcodes_Table[] = {
  /* Generic OnOff Server */
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
    MOBLEUINT16 max_payload_size;
    Here in this array, Handler is not defined; */
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF  
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_GET,                                    MOBLE_TRUE,  0, 0,              GENERIC_ON_OFF_STATUS , 1, 3},
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_SET_ACK,                                MOBLE_TRUE,  2, 4,              GENERIC_ON_OFF_STATUS , 1, 3},  
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_SET_UNACK,                              MOBLE_FALSE, 2, 4,              GENERIC_ON_OFF_STATUS , 1, 3},  // null replaced from GENERIC_ON_OFF_STATUS
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_STATUS,                                 MOBLE_FALSE, 1, 3,              0 , 1, 3},
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  /* Generic Level Server */
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_GET,                                     MOBLE_TRUE,   0, 0,             GENERIC_LEVEL_STATUS , 2 , 5}, 
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_SET_ACK,                                 MOBLE_TRUE,   3, 5,             GENERIC_LEVEL_STATUS , 2 , 5},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_SET_UNACK,                               MOBLE_FALSE,  3, 5,             GENERIC_LEVEL_STATUS , 2 , 5}, //null is replaced with GENERIC_LEVEL_STATUS
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_DELTA_SET,                               MOBLE_TRUE,   5, 7,             GENERIC_LEVEL_STATUS , 2 , 5},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_DELTA_SET_UNACK,                         MOBLE_FALSE,  5, 7,             0, 0 , 0},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_DELTA_MOVE_SET,                          MOBLE_TRUE,   3, 5,             GENERIC_LEVEL_STATUS , 2 , 5},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_DELTA_MOVE_SET_UNACK,                    MOBLE_FALSE,  3, 5,             0, 0 , 0},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_STATUS,                                  MOBLE_FALSE,  2, 5,             0 , 2 , 5},
#endif  
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF  
  {GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP_MODEL_ID   ,GENERIC_POWER_ON_OFF_SET,                              MOBLE_TRUE,   1, 1,             GENERIC_POWER_ON_OFF_STATUS, 1,1},
  {GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP_MODEL_ID   ,GENERIC_POWER_ON_OFF_SET_UNACK,                        MOBLE_FALSE,  1, 1,             0, 1,1},
  {GENERIC_MODEL_SERVER_POWER_ONOFF_MODEL_ID   ,GENERIC_POWER_ON_OFF_GET ,                             MOBLE_TRUE,   0, 0,             GENERIC_POWER_ON_OFF_STATUS, 1,1},
  {GENERIC_MODEL_SERVER_POWER_ONOFF_MODEL_ID   ,GENERIC_POWER_ON_OFF_STATUS ,                          MOBLE_FALSE,  1, 1,             0, 1,1},
#endif  
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME     
  /* Generic Default Transition Time Server Model  */
  {GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID   ,GENERIC_DEFAULT_TRANSITION_TIME_GET,                   MOBLE_TRUE,  0, 0,              GENERIC_DEFAULT_TRANSITION_TIME_STATUS , 1, 1}, 
  {GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID   ,GENERIC_DEFAULT_TRANSITION_TIME_SET,                   MOBLE_TRUE,  1, 1,              GENERIC_DEFAULT_TRANSITION_TIME_STATUS , 1, 1},
  {GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID   ,GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK,             MOBLE_FALSE, 1, 1,              GENERIC_DEFAULT_TRANSITION_TIME_STATUS  ,0 ,0}, // //null is replaced with GENERIC_DEFAULT_TRANSITION_TIME_STATUS
  {GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID   ,GENERIC_DEFAULT_TRANSITION_TIME_STATUS,                MOBLE_FALSE, 1, 1,              0  ,1, 1},
#endif  
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_BATTERY     
  /* Generic Battery Server Model  */
  {GENERIC_MODEL_SERVER_BATTERY_MODEL_ID   GENERIC_BATTERY_GET,                                   MOBLE_TRUE,  0, 0,              GENERIC_BATTERY_STATUS , 8 , 8},         
#endif
  {0}
};

/* Private function prototypes -----------------------------------------------*/
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t* pGeneric_OnOffParam, 
                                                    MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t* plevelParam, 
                                                    MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_LevelDelta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                                         MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_LevelMove_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                                        MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Set(Generic_PowerOnOffParam_t* pPowerOnOffParam, 
                                                        MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Set(Generic_DefaultTransitionParam_t* pDefaultTimeParam, 
                                                                    MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (void Appli_Generic_Restore_PowerOn_Value(MOBLEUINT8 restoreValue));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffValue(MOBLEUINT8* pOnOff_Value) );
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetPowerOnOffStatus(MOBLEUINT8* pLevel_Status));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetDefaultTransitionStatus(MOBLEUINT8* pTransition_Status));
WEAK_FUNCTION(MOBLE_RESULT Appli_GenericClient_Level_Set_Unack(void));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Status(MOBLEUINT8 const *pOnOff_status, MOBLEUINT32 plength));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Status(MOBLEUINT8 const *plevel_status, MOBLEUINT32 plength));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status , MOBLEUINT32 plength));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , MOBLEUINT32 plength));

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Generic_OnOff_Set: This function is called for both Acknowledged and 
          unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_OnOff_Set(MOBLEUINT8 const *pOnOff_param, MOBLEUINT32 length) 
{
  
  /* 3.2.1.2 Generic OnOff Set 
  OnOff: 1B The target value of the Generic OnOff state 
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 millisecond steps (C.1)
  */
  
  TRACE_M(TF_GENERIC,"Generic_OnOff_Set callback received \r\n");  
  
  Generic_OnOffParam_t Generic_OnOffParam; 
  Generic_OnOffParam.TargetOnOffState = pOnOff_param[0];
  Generic_OnOffParam.Generic_TID = pOnOff_param[1];
  CommandStatus = pOnOff_param[0];
  
  if(length > 2)
  {
    /* Transition_Time & Delay_Time Present */
    Generic_OnOffParam.Transition_Time = pOnOff_param[2];
    Generic_OnOffParam.Delay_Time = pOnOff_param[3];
      
    /* Copy the received data in status message which needs
       to be set in application messages
    */
    Generic_OnOffStatus.Target_OnOff =  Generic_OnOffParam.TargetOnOffState;
    Generic_OnOffStatus.RemainingTime = Generic_OnOffParam.Transition_Time;
    /* copy status parameters in Temporary parameters for transition 
       process
    */
    if(Generic_OnOffParam.TargetOnOffState == 1)
    {
      Generic_TemporaryStatus.TargetValue16 = PWM_TIME_PERIOD;
      Generic_OnOffStatus.Present_OnOff_Value = 1;
    }
    else
    {
      Generic_TemporaryStatus.TargetValue16 = 1;
    }    
    Generic_TemporaryStatus.RemainingTime = Generic_OnOffStatus.RemainingTime;
    /* Function to calculate time parameters, step resolution
      step size for transition state machine
    */
    Generic_GetStepValue(pOnOff_param[2]);
    /*option parameter flag, enable to sent all required parameter in status.*/      
    Generic_ModelFlag.GenericOptionalParam = 1;
    /*Flag to enable the on Off transition state machine */
    Generic_ModelFlag.GenericTransitionFlag = GENERIC_ON_OFF_TRANSITION_START;
    OptionalValid = IN_TRANSITION;
  } 
  else
  {
    /* when default transition time enabled,and the target time is not given by client
       the transition time will be used from default value
      */
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME     
    if(Generic_OnOffParam.TargetOnOffState == 1) 
    {
      Generic_TemporaryStatus.TargetValue16 = PWM_TIME_PERIOD;     
      Generic_OnOffStatus.Present_OnOff_Value = 1;
    }
    else
    {
      Generic_TemporaryStatus.TargetValue16 = 1;
    }
    Generic_OnOffDefaultTransitionValue();
    OptionalValid = DEFAULT_TRANSITION;
#else
    /* When no optional parameter received, target value will be set as present
       value in application.
    */  
    OptionalValid = NO_TRANSITION;
#endif    
  }
   Generic_OnOffStatus.Present_OnOff_State = Generic_OnOffParam.TargetOnOffState;
   /* Application Callback */
  (GenericAppli_cb.OnOff_Set_cb)(&Generic_OnOffStatus, OptionalValid);
#ifdef ENABLE_MODEL_BINDING    
   /* Binding of data b/w Generic on off and Light lightness Actual model */
   GenericOnOff_LightActualBinding(&Generic_OnOffParam);
#endif
      
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_OnOff_Status
* @param  pOnoff_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_OnOff_Status(MOBLEUINT8* pOnOff_status, MOBLEUINT32 *plength)
{
  /* 
  Following is the status message:
  Present OnOff The present value of the Generic OnOff state. 
  Target OnOff The target value of the Generic OnOff state (optional).
  Remaining Time is transition time. 
  */
   MOBLEUINT8 Generic_GetBuff[1] ; 
  
  TRACE_M(TF_GENERIC,"Generic_OnOff_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8201! \n\r");
  
   /* Function call back to get the values from application*/
    (Appli_GenericState_cb.GetOnOffStatus_cb)(Generic_GetBuff);
  Generic_OnOffStatus.Present_OnOff_State = Generic_GetBuff[0];
   
   if((Generic_ModelFlag.GenericOptionalParam == 1) || (Generic_TimeParam.StepValue != 0))
   {   
    *pOnOff_status = Generic_OnOffStatus.Present_OnOff_State;
       *(pOnOff_status+1) = Generic_OnOffStatus.Target_OnOff;
       *(pOnOff_status+2) = Generic_OnOffStatus.RemainingTime;
       *plength = 3; 
       Generic_ModelFlag.GenericOptionalParam = 0;
   }
   else
   {   /* When no optional parameter received, target value will
         be sent in status message.
      */
    *pOnOff_status = Generic_OnOffStatus.Present_OnOff_State;
       *plength = 1;
   }
 
      return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Level_Set: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Level_Set(const MOBLEUINT8* plevel_param, MOBLEUINT32 length)
{
  /*
  3.2.2.2 Generic Level Set
  Level: 2B The target value of the Generic Level state
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
  */
  
  TRACE_M(TF_GENERIC,"Generic_Level_Set callback received \r\n");
  
  Generic_LevelParam_t Generic_LevelParam;
  MOBLEINT16 setValue;
  
  Generic_LevelParam.TargetLevel = *(MOBLEUINT16*) plevel_param;
  Generic_LevelParam.Generic_TID = plevel_param[2]; 
  
  setValue =  Generic_LevelParam.TargetLevel;
  /* Check for Optional Parameters. */ 
  if(length > 3)
  {
     Generic_LevelParam.Transition_Time = plevel_param[3];
     Generic_LevelParam.Delay_Time = plevel_param[4];
     /* Copy the data into status message which needs to be update in 
       application message.
     */
     Generic_LevelStatus.Target_Level16 = setValue;
     Generic_LevelStatus.RemainingTime = Generic_LevelParam.Transition_Time;
     /* copy status parameters in Temporary parameters for transition 
         process.
     */
     Generic_TemporaryStatus.TargetValue16 = Generic_LevelStatus.Target_Level16;
     Generic_TemporaryStatus.RemainingTime = Generic_LevelStatus.RemainingTime;
     /* Function to calculate time parameters, step resolution
        step size for transition state machine
     */
     Generic_GetStepValue(plevel_param[3]); 
     /*option parameter flag, enable to sent all required parameter in status.*/
    Generic_ModelFlag.GenericOptionalParam = 1;
     /*transition process enable flag. */
    Generic_ModelFlag.GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
  }
  else
  {     
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
    
    Generic_LevelDefaultTransitionValue(setValue);
#else
     /* When no optional parameter received, target value will
         be set as present value in application.
     */
     Generic_LevelStatus.Present_Level16= setValue; 
#endif    
  }  
     Generic_LevelStatus.Last_Present_Level16 = Generic_LevelStatus.Present_Level16;
 
     /* Application Callback */
     (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus, 0); 
#ifdef ENABLE_MODEL_BINDING       
     /* Binding of Generic level with light lightnes actual */
      GenericLevel_LightActualBinding(&Generic_LevelParam);
     /* Binding of Generic Level data with ctl temperature data */ 
      GenericLevel_CtlTempBinding(&Generic_LevelParam);
     /* Generic Level binding with Hsl Hue Value */
      GenericLevel_HslHueBinding(&Generic_LevelParam);
     /* Generic Level Binding with Hsl Saturation */
      GenericLevel_HslSaturationBinding(&Generic_LevelParam);
#endif 
        
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_LevelDelta_Set: This function is called for both Acknowledged 
*         and unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_LevelDelta_Set(const MOBLEUINT8* plevel_param, MOBLEUINT32 length)
{
  /*
  3.2.2.4 Generic Level Delta Set
  Delta Level: 4B The Delta change of the Generic Level state
  TID:   1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 milliseconds steps (C.1)
  */
  
  TRACE_M(TF_GENERIC,"Generic_LevelDelta_Set callback received \r\n");
  
  Generic_DeltaLevelParam_t Generic_DeltaLevelParam ;
  MOBLEUINT32 delta;
  
  /* Copy the 4Bytes data to local variable */
  delta = (plevel_param[3] << 24);
  delta |= (plevel_param[2] << 16);
  delta |= (plevel_param[1] << 8);
  delta |= (plevel_param[0]);
  Generic_DeltaLevelParam.TargetDeltaLevel32 = delta;
  
  Generic_DeltaLevelParam.Generic_TID = plevel_param[4];
  
  /* Check for Optional Parameters */   
  if(length > 5)
  {
       Generic_DeltaLevelParam.Transition_Time = plevel_param[5];
       Generic_DeltaLevelParam.Delay_Time = plevel_param[6];
      /* Copy the data into status message which needs to be update in 
         application message.
      */ 
       Generic_LevelStatus.Target_Level16 = Generic_LevelStatus.Present_Level16 + 
                                                Generic_DeltaLevelParam.TargetDeltaLevel32;
       Generic_LevelStatus.RemainingTime = Generic_DeltaLevelParam.Transition_Time;
       
       /* copy status parameters in Temporary parameters for transition 
         process.
       */
       Generic_TemporaryStatus.TargetValue16 = Generic_LevelStatus.Target_Level16;
       Generic_TemporaryStatus.RemainingTime = Generic_LevelStatus.RemainingTime;
       /* Function to calculate time parameters, step resolution
        step size for transition state machine.
       */
       Generic_GetStepValue(plevel_param[5]);
       
       /*option parameter flag, enable to sent all required parameter in status.*/ 
       Generic_ModelFlag.GenericOptionalParam = 1;
       
       /*transition process enable flag. */
    Generic_ModelFlag.GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
  }
  else
    {   
      if(Generic_LevelStatus.Last_Level_TID == Generic_DeltaLevelParam.Generic_TID)
      {
         Generic_LevelStatus.Present_Level16 =  Generic_LevelStatus.Last_Present_Level16 
                                                  + Generic_DeltaLevelParam.TargetDeltaLevel32;
      }
      else
      {         
         Generic_LevelStatus.Present_Level16 += Generic_DeltaLevelParam.TargetDeltaLevel32;  
      }
    }
         Generic_LevelStatus.Last_Level_TID = Generic_DeltaLevelParam.Generic_TID;   
      
    /* EME: need to update the value for next operation */
    Generic_LevelStatus.Last_Present_Level16 = Generic_LevelStatus.Present_Level16;

    /* Application Callback */
   (GenericAppli_cb.LevelDelta_Set_cb)(&Generic_LevelStatus, 0);
   
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_LevelMove_Set: This function is called for both 
*         Acknowledged and unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_LevelMove_Set(const MOBLEUINT8* plevel_param, MOBLEUINT32 length)
{
  /*
  3.2.2.6 Generic Level Move Set
  Level: 2B The target value of the Generic Level state
  TID:   1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 milliseconds steps (C.1)
  */
  TRACE_M(TF_GENERIC,"Generic_LevelMove_Set callback received \r\n");
  
    Generic_LevelMoveParam_t  Generic_LevelMoveParam;
  
   Generic_LevelMoveParam.TargetMoveLevel16  = (plevel_param[1] << 8);
   Generic_LevelMoveParam.TargetMoveLevel16 |= (plevel_param[0]);
   Generic_LevelMoveParam.Generic_TID = plevel_param[2];
  
  /* Check for Optional Parameters */   
  if(length > 3)
  {
     Generic_LevelMoveParam.Transition_Time = plevel_param[3];
     Generic_LevelMoveParam.Delay_Time = plevel_param[4];
     /* Copy the data into status message which needs to be update in 
        application message.
    */ 
     Generic_LevelStatus.Target_Level16 = Generic_LevelStatus.Present_Level16 + 
                                              Generic_LevelMoveParam.TargetMoveLevel16;
     Generic_LevelStatus.RemainingTime = Generic_LevelMoveParam.Transition_Time;
     /* Function to calculate time parameters, step resolution
        step size for transition state machine.
      */
     Generic_GetStepValue(plevel_param[3]);
     /*option parameter flag, enable to sent all required parameter in status.*/
     Generic_ModelFlag.GenericOptionalParam = 1;
     /*transition process enable flag. */
    Generic_ModelFlag.GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
  }
  else
  {   
    if(Generic_LevelStatus.Last_Level_TID == Generic_LevelMoveParam.Generic_TID)
    {
       Generic_LevelStatus.Present_Level16 =  Generic_LevelStatus.Last_Present_Level16 
                                                + Generic_LevelMoveParam.TargetMoveLevel16;
    }
    else
    {
       Generic_LevelStatus.Present_Level16 += Generic_LevelMoveParam.TargetMoveLevel16;  
    }
  }    Generic_LevelStatus.Last_Level_TID = Generic_LevelMoveParam.Generic_TID; 
 
   /* Application Callback */
  (GenericAppli_cb.LevelDeltaMove_Set_cb)(&Generic_LevelStatus, 0); 
     
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Level_Status
* @param  plevel_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Level_Status(MOBLEUINT8* plevel_status, MOBLEUINT32 *plength)
{
  /* 
  3.2.2.8 Generic Level Status
  Following is the status message:
  Present Level: 2B The present value of the Generic Level state. 
  Target Level: 2B The target value of the Generic Level state (Optional). 
  Remaining Time: 1B Format as defined in Section 3.1.3 (C.1).
  
  */
      MOBLEUINT8 Generic_GetBuff[2] ;   
  
  TRACE_M(TF_GENERIC,"Generic_Level_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8205! \n\r");
  
      /* Function call back to get the values from application*/
      (Appli_GenericState_cb.GetLevelStatus_cb)(Generic_GetBuff);
       Generic_LevelStatus.Present_Level16 = Generic_GetBuff[1] << 8;
       Generic_LevelStatus.Present_Level16 |= Generic_GetBuff[0];
      
       /* checking the transition is in process.
        checking for remaining time is not equal to zero.
       */

   if((Generic_ModelFlag.GenericOptionalParam ==1) || (Generic_TimeParam.StepValue != 0))
   {
      *(plevel_status) = Generic_LevelStatus.Present_Level16;
      *(plevel_status+1) = Generic_LevelStatus.Present_Level16 >> 8;
      *(plevel_status+2) = Generic_LevelStatus.Target_Level16;
      *(plevel_status+3) = Generic_LevelStatus.Target_Level16 >> 8;
      *(plevel_status+4) = Generic_LevelStatus.RemainingTime;
      *plength = 5;
       Generic_ModelFlag.GenericOptionalParam = 0;    
   }
  else
  {
      *(plevel_status) = Generic_LevelStatus.Present_Level16;
      *(plevel_status+1) = Generic_LevelStatus.Present_Level16 >> 8;
      *plength = 2;             
  }
     
  return MOBLE_RESULT_SUCCESS;   
}

/**
* @brief  Generic_PowerOnOff_Set: This function is called for both 
*         Acknowledged and unacknowledged message
* @param  powerOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_PowerOnOff_Set(const MOBLEUINT8 *powerOnOff_param , MOBLEUINT32 length) 
{
   /* 
  3.2.4.2 Generic Power On Off Time
  Following is the set message:
  powerOnOff_param:1B parameter is received to set the power on off model.  
  */
  TRACE_M(TF_GENERIC,"Generic_PowerOnOff_Set callback received \r\n");
  
  Generic_PowerOnOffParam_t Generic_PowerOnOffParam;
  Generic_PowerOnOffParam.PowerOnOffState = powerOnOff_param[0];
  
  /* Application Callback */
  (GenericAppli_cb.GenericPowerOnOff_cb)(&Generic_PowerOnOffParam, length);
   return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Generic_PowerOnOff_Status
* @param  powerOnOff_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_PowerOnOff_Status(MOBLEUINT8 *powerOnOff_status , MOBLEUINT32 *plength) 
{  
  /* 
  3.2.4.4 Generic OnPowerUp Status
  Following is the status message:
  powerOnOff_status: 1B is the status parameter of the Power on off model. 
  */  
  MOBLEUINT8 Generic_GetBuff[2] ;
  TRACE_M(TF_GENERIC,"Generic_PowerOnOff_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8211! \n\r");
  
  /* Function call back to get the values from application*/
  (Appli_GenericState_cb.GetPowerOnOffStatus_cb)(Generic_GetBuff);
   
   *(powerOnOff_status) = Generic_GetBuff[0];
   *plength = 1;
 
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Generic_DefaultTransitionTime_Set: This function is called for both 
*         Acknowledged and unacknowledged message
* @param  defaultTransition_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_DefaultTransitionTime_Set(const MOBLEUINT8 *defaultTransition_param , MOBLEUINT32 length) 
{
   /* 
  3.2.3.2 Generic Default Transition Time Set
  Following is the set message:
  defaultTime_param:1B parameter is received to set the Default transition time model.  
  */
  TRACE_M(TF_GENERIC,"Generic_DefaultTransitionTime_Set callback received \r\n"); 
  
  Generic_DefaultTransitionParam.DefaultTransitionTime = defaultTransition_param[0];
  /* Application Callback */
  (GenericAppli_cb.GenericDefaultTransition_cb)(&Generic_DefaultTransitionParam, length);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Generic_DefaultTransitionTime_Status
* @param  pTransition_status: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_DefaultTransitionTime_Status(MOBLEUINT8 *pTransition_status , MOBLEUINT32 *plength) 
{  
  /* 
  3.2.3.4 Generic Default Transition Time Status
  Following is the status message:
  powerOnOff_status: 1B is the status parameter of the Default transition time model. 
  */  
  MOBLEUINT8 Generic_GetBuff[2] ;
  TRACE_M(TF_GENERIC,"Generic_DefaultTransitionTime_Status callback received \r\n");
  /* Function call back to get the values from application*/
  (Appli_GenericState_cb.GetDefaultTransitionStatus_cb)(Generic_GetBuff);
   
   *(pTransition_status) = Generic_GetBuff[0];
   *plength = 1;
 
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
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pRxData,
                                    MOBLEUINT32 dataLength,
                                    MOBLEBOOL response)

{
  TRACE_M(TF_GENERIC,"response status enable \n\r");
  switch(opcode)
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF      
  case GENERIC_ON_OFF_STATUS:
    {
      Generic_OnOff_Status(pResponsedata, plength);      
      break;
    }
#endif 
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL      
  case GENERIC_LEVEL_STATUS:
    {
      Generic_Level_Status(pResponsedata, plength);
      break;
    }
#endif      
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF    
  case GENERIC_POWER_ON_OFF_STATUS:
    {
      Generic_PowerOnOff_Status(pResponsedata, plength);
      break;
    }
#endif
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME    
  case GENERIC_DEFAULT_TRANSITION_TIME_STATUS:
    {
      Generic_DefaultTransitionTime_Status(pResponsedata, plength);
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
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message 
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                )
{

  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
//  tClockTime delay_t = Clock_Time();
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 elementNumber;
  MOBLEUINT8 modelStateChangeFlag = MOBLE_FALSE;  
  Generic_Rx_Opcode = opcode;
  Dst_Peer = dst_peer;
  TRACE_M(TF_GENERIC,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X ,response= %.2X \r\n  ",
                                                       dst_peer, peer_addr, opcode , response);
  Generic_Rx_Opcode = opcode; 
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
          /*
           when device is working as proxy and is a part of node
           delay will be included in the toggelinf of led.
          */
          if(!MOBLE_FAILED(result = Chk_TidValidity(peer_addr,dst_peer,pRxData[1])))
          {
            Generic_OnOff_Set(pRxData,dataLength);   
            Model_ID = (MOBLEUINT16)GENERIC_MODEL_SERVER_ONOFF_MODEL_ID;
            modelStateChangeFlag = MOBLE_TRUE;
          }
        }
        
        break;
      }
  case GENERIC_ON_OFF_STATUS:
    {
      Generic_Client_OnOff_Status(pRxData,dataLength);
      break;
    }
#endif 
      
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
  
    case GENERIC_LEVEL_SET_ACK:
    case GENERIC_LEVEL_SET_UNACK: 
      {   
       
      result = Chk_ParamMinMaxValidity(LEVEL_MIN_VALID_RANGE ,pRxData , LEVEL_MAX_VALID_RANGE );        
        if(result == MOBLE_RESULT_SUCCESS)
        {
        if(!MOBLE_FAILED(result = Chk_TidValidity(peer_addr,dst_peer,pRxData[2])))
          {
          Generic_Level_Set(pRxData,dataLength);   
            Model_ID = (MOBLEUINT16)GENERIC_MODEL_SERVER_LEVEL_MODEL_ID;
            modelStateChangeFlag = MOBLE_TRUE;
          }
      }
       
        break;
      }
    
    case GENERIC_LEVEL_DELTA_SET:
    case GENERIC_LEVEL_DELTA_SET_UNACK:
      { 
        /*
        Delta Level 4 The Delta change of the Generic Level state 
        TID 1 Transaction Identifier 
        Transition Time 1 Format as defined in Section 3.1.3. (Optional) 
        Delay 1 Message execution delay in 5 milliseconds steps (C.1)
        */
      if(!MOBLE_FAILED(result = Chk_TidValidity(peer_addr,dst_peer,pRxData[4])))
      {
        Generic_LevelDelta_Set(pRxData,dataLength);
      }
        break;
      }
      
    case GENERIC_LEVEL_DELTA_MOVE_SET:
    case GENERIC_LEVEL_DELTA_MOVE_SET_UNACK:
      {    
       
      result = Chk_ParamMinMaxValidity(LEVEL_MIN_VALID_RANGE ,pRxData , LEVEL_MAX_VALID_RANGE );       
        if(result == MOBLE_RESULT_SUCCESS)
        {
         if(!MOBLE_FAILED(result = Chk_TidValidity(peer_addr,dst_peer,pRxData[2])))
         {
        Generic_LevelMove_Set(pRxData, dataLength); 
        }
      }
        break;
      }
  case GENERIC_LEVEL_STATUS:
    {       
      Generic_Client_Level_Status(pRxData,dataLength);     
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
        Generic_PowerOnOff_Set(pRxData, dataLength);
      }
      break;
    }
  case GENERIC_POWER_ON_OFF_STATUS:
    {
      Generic_Client_PowerOnOff_Status(pRxData,dataLength);
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
        Generic_DefaultTransitionTime_Set(pRxData, dataLength);
      }
      break;
    }
  case GENERIC_DEFAULT_TRANSITION_TIME_STATUS:
    {
      Generic_Client_DefaultTransitionTime_Status(pRxData, dataLength);
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
    Model_SendResponse(peer_addr, dst_peer,opcode, pRxData,dataLength);
  }
  
  /*
     Publish the status to publish address if the publication is set by client 
     publication is independent of the response of the message.if thek condition 
     for publication is full filled as per specification then the status will be 
     published.
  */
    elementNumber = BLE_GetElementNumber();
    publishAddress = BLEMesh_GetPublishAddress(elementNumber,Model_ID);
    
  if((result == MOBLE_RESULT_SUCCESS) && (publishAddress != 0x0000) && (modelStateChangeFlag == MOBLE_TRUE))
  {
    Model_SendResponse(publishAddress,dst_peer,opcode,pRxData,dataLength);
    
    modelStateChangeFlag = MOBLE_FALSE;   
  }
  return MOBLE_RESULT_SUCCESS;
}


/* @Brief  Generic_TransitionBehaviourSingle_Param: Generic On Off Transition behaviour 
*          used for the Generic On Off model when transition time is received in
*          message.        
* @param GetValue Pointer of the array
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Generic_TransitionBehaviourSingle_Param(MOBLEUINT8 *GetValue)
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
   /* Values from application are copied into Temporary vaiables for processing */
  
  Generic_TemporaryStatus.PresentValue16  = GetValue[1] << 8;
  Generic_TemporaryStatus.PresentValue16 |= GetValue[0];
    /*if condition to wait untill the time is equal to the given resolution time */
   if(((Clock_Time()- Check_time) >= Generic_TimeParam.Res_Value))
   {     
      if(Generic_TimeParam.StepValue == 0)
      {
        Generic_TimeParam.StepValue = 1;
      }
    
     if(Generic_TemporaryStatus.TargetValue16 > Generic_TemporaryStatus.PresentValue16)
    {
      Generic_OnOffStatus.Present_OnOff_State = 1;
      /* target range = total range to be covered */
      targetRange = Generic_TemporaryStatus.TargetValue16 - Generic_TemporaryStatus.PresentValue16; 
      /*target slot = time to cover in single step */
      targetSlot = targetRange/Generic_TimeParam.StepValue; 
      /* target slot added to present value to achieve target value */
      Generic_TemporaryStatus.PresentValue16 += targetSlot;             
    }              
    else if(Generic_TemporaryStatus.TargetValue16 < Generic_TemporaryStatus.PresentValue16)
    {  
      Generic_OnOffStatus.Present_OnOff_State = 0;
      /* condition execute when transition is negative */
      /* target range = total range to be covered */ 
      targetRange = Generic_TemporaryStatus.PresentValue16 - Generic_TemporaryStatus.TargetValue16;
      /*target slot = time to cover in single step */
      targetSlot = targetRange/Generic_TimeParam.StepValue;
      /*target slot = time to cover in single step */
      Generic_TemporaryStatus.PresentValue16 -= targetSlot;
    }     
    else
    {
      
    }
        Generic_TimeParam.StepValue--;
        /* updating the remaining time after each step covered*/
        Generic_TemporaryStatus.RemainingTime = Generic_TimeParam.StepValue | (Generic_TimeParam.ResBitValue << 6) ;
     
        Check_time = 0;
        Clockflag = 0;
    GenericUpdateFlag = VALUE_UPDATE_SET;
        /* when transition is completed, disable the transition by disabling 
           transition flag
        */
        if(Generic_TimeParam.StepValue <= 0)
        {
      Generic_ModelFlag.GenericTransitionFlag = GENERIC_TRANSITION_STOP; 
      Generic_Trnsn_Cmplt = MOBLE_TRUE;
        }
    TRACE_M(TF_GENERIC,"Inside virtual application at %ld, Current state 0x%.2x, Target state 0x%.2x, Remaining Time 0x%.2x \n\r",
             Clock_Time(), Generic_TemporaryStatus.PresentValue16,Generic_TemporaryStatus.TargetValue16,Generic_TemporaryStatus.RemainingTime);  
    }
return MOBLE_RESULT_SUCCESS;       

} 


/* Generic_TransitionBehaviourMulti_Param: This funtion is used for the    
*  for multi parameter in model when transition time is received in message.                             
* @param GetValue Pointer of the array
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Generic_TransitionBehaviourMulti_Param(MOBLEUINT8 *GetValue)
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
   /* Values from application are copied into Temporary vaiables for processing */
    Generic_TemporaryStatus.PresentValue16  = GetValue[1] << 8;
    Generic_TemporaryStatus.PresentValue16 |= GetValue[0];
   /*if condition to wait untill the time is equal to the given resolution time */
   if(((Clock_Time()- Check_time) >= Generic_TimeParam.Res_Value) )
   {   
      if(Generic_TimeParam.StepValue == 0)
      {
        Generic_TimeParam.StepValue = 1;
      }
      if(Generic_TemporaryStatus.TargetValue16 > Generic_TemporaryStatus.PresentValue16)
      {
         /* target range = total range to be covered */
         targetRange = Generic_TemporaryStatus.TargetValue16 - Generic_TemporaryStatus.PresentValue16; 
         /*target slot = time to cover in single step */
         targetSlot = targetRange/Generic_TimeParam.StepValue; 
         /* target slot added to present value to achieve target value */
         Generic_TemporaryStatus.PresentValue16 += targetSlot;             
      }              
      else
      {  
        /* condition execute when transition is negative */
        /* target range = total range to be covered */ 
         targetRange = Generic_TemporaryStatus.PresentValue16 - Generic_TemporaryStatus.TargetValue16;
         /*target slot = time to cover in single step */
         targetSlot = targetRange/Generic_TimeParam.StepValue;
         /*target slot = time to cover in single step */
         Generic_TemporaryStatus.PresentValue16 -= targetSlot;
      }     
         Generic_TimeParam.StepValue--;
         /* updating the remaining time after each step covered*/
         Generic_TemporaryStatus.RemainingTime  = Generic_TimeParam.StepValue | (Generic_TimeParam.ResBitValue << 6) ;
                                                        
         Check_time = 0;
         Clockflag = 0;
    GenericUpdateFlag = VALUE_UPDATE_SET;
        /* when transition is completed, disable the transition by disabling 
         transition flag
        */
        if(Generic_TimeParam.StepValue <= 0)
        {
      Generic_ModelFlag.GenericTransitionFlag = GENERIC_TRANSITION_STOP; 
      Generic_Trnsn_Cmplt = MOBLE_TRUE;
        }
    TRACE_M(TF_GENERIC,"Inside virtual level application at %ld, Current state 0x%.2x , target state 0x%.2x , Remaining Time 0x%.2x \n\r",
                Clock_Time(),Generic_TemporaryStatus.PresentValue16,Generic_TemporaryStatus.TargetValue16,               
                    Generic_TemporaryStatus.RemainingTime);
    }

return MOBLE_RESULT_SUCCESS;         
} 


/** 
* @brief Generic_GetStepValue: This function calculates values for transition time
* @param stepParam: Transition time set value of particular model message.
* retval void
*/
void Generic_GetStepValue(MOBLEUINT8 stepParam)
{
  /*
     Two MSB bit of transition time is dedicated to resolution.
     00 = resolution is 100 ms.
     01 = resolution is 1000 ms. 
     10 = resolution is 10000 ms.
     11 = resolution is 600000 ms. 
    Last bits from 0 to 5th index is step number.
  */
  
   Generic_TimeParam.ResBitValue = stepParam >> 6 ;
   Generic_TimeParam.Res_Value = Get_StepResolutionValue(Generic_TimeParam.ResBitValue);
   Generic_TimeParam.StepValue = stepParam & 0x3F ;
  
  if(Generic_TimeParam.Res_Value >= 100)
  {
    Generic_TimeParam.Res_Value = Generic_TimeParam.Res_Value/TRANSITION_SCALER;
    Generic_TimeParam.StepValue = (Generic_TimeParam.StepValue * TRANSITION_SCALER);
  }
  
  TRACE_M(TF_GENERIC," step resolution 0x%.2lx, number of step 0x%.2x \r\n",
          Generic_TimeParam.Res_Value , Generic_TimeParam.StepValue );
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
  MOBLEUINT8 elementNumber;
  MOBLEUINT8 const pRxData[8] = {0};
  MOBLEUINT32 dataLength = 0;
#if defined ENABLE_GENERIC_MODEL_SERVER_ONOFF || defined ENABLE_GENERIC_MODEL_SERVER_LEVEL  
     MOBLEUINT8 Generic_GetBuff[8]; 
#endif     
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF   
  if(Generic_ModelFlag.GenericTransitionFlag == GENERIC_ON_OFF_TRANSITION_START)
  {   
    (Appli_GenericState_cb.GetOnOffValue_cb)(Generic_GetBuff);
      Generic_TransitionBehaviourSingle_Param(Generic_GetBuff);
    if(GenericUpdateFlag == VALUE_UPDATE_SET)
    {
      GenericOnOffStateUpdate_Process();
      (GenericAppli_cb.OnOff_Set_cb)(&Generic_OnOffStatus, OptionalValid);  
      GenericUpdateFlag = VALUE_UPDATE_RESET;     
    }
  }    
#endif 
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL       
  if(Generic_ModelFlag.GenericTransitionFlag == GENERIC_LEVEL_TRANSITION_START)
  {    
     (Appli_GenericState_cb.GetLevelStatus_cb)(Generic_GetBuff);
      Generic_TransitionBehaviourMulti_Param(Generic_GetBuff);
    if(GenericUpdateFlag == VALUE_UPDATE_SET)
    {
      GenericLevelStateUpdate_Process();
     (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus, 0);                             
      GenericUpdateFlag = VALUE_UPDATE_RESET;
    }
                               
  }   
#endif
  
  if(Generic_Trnsn_Cmplt == MOBLE_TRUE)
  {         
    elementNumber = BLE_GetElementNumber();
    publishAddress = BLEMesh_GetPublishAddress(elementNumber,Model_ID);
    if(publishAddress != 0x00)
    {
      Model_SendResponse(publishAddress,Dst_Peer,Generic_Rx_Opcode,pRxData,dataLength);
    }
    Generic_Trnsn_Cmplt = MOBLE_FALSE;
  }
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
  generic_Buff[1] = TidSend;
  
  result = BLEMesh_SetRemotePublication(GENERIC_MODEL_SERVER_ONOFF_MODEL_ID, srcAddress ,
                            GENERIC_ON_OFF_SET_UNACK, 
                            generic_Buff, 2,
                            MOBLE_FALSE, MOBLE_FALSE);
  TidSend++;
  if(TidSend >= MAX_TID_VALUE)
  {
    TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC,"Publication Error \r\n");
  }
  
    CommandStatus = generic_Buff[0];
}


/**
* @brief GenericOnOffStateUpdate_Process:Function to update the parametes of 
*        Generic On Off model in application file from Temporary parameter in model file.
* @param void
* return MOBLE_RESULT.
*/
MOBLE_RESULT GenericOnOffStateUpdate_Process(void)
{
  Generic_OnOffStatus.Present_OnOff_Value = Generic_TemporaryStatus.PresentValue16;
   Generic_OnOffStatus.RemainingTime = Generic_TemporaryStatus.RemainingTime;
   
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief GenericLevelStateUpdate_Process:function to update the parametes of Generic 
*        Level model in application file from Temporary parameter in model file.
* @param void
* return MOBLE_RESULT.
*/
MOBLE_RESULT GenericLevelStateUpdate_Process(void)
{
   Generic_LevelStatus.Present_Level16 = Generic_TemporaryStatus.PresentValue16;
   Generic_LevelStatus.Target_Level16  = Generic_TemporaryStatus.TargetValue16;
   Generic_LevelStatus.RemainingTime   = Generic_TemporaryStatus.RemainingTime;
   
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  GenericOnOff_LightActualBinding: Data binding b/w Generic On Off and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic on off set. 
* @param onOff_param: Pointer to the data which needs to be checked.
* return void.
*/
void GenericOnOff_LightActualBinding(Generic_OnOffParam_t* onOff_param)
{
  /*
   6.1.2.2.3 - Binding of actual light lightness with Generic on off.
               As generic on off state changes, the actual lightness value will
               change.
  */
  MOBLEUINT8 Generic_GetBuff[4]; 
  
  Light_LightnessStatus_t bLight_ActualParam ;
  Light_LightnessDefaultParam_t bLight_DefaultParam;
  Light_LightnessLastParam_t bLight_LastParam;
  
   /* Get the last saved value of light lightness actual from application */
   (Appli_Light_GetStatus_cb.GetLightLightness_cb)(Generic_GetBuff);
   
    bLight_LastParam.LightnessLastStatus = Generic_GetBuff[3] << 8;
    bLight_LastParam.LightnessLastStatus |= Generic_GetBuff[2];
   
   /* Get the default value of light lightness actual */ 
   (Appli_Light_GetStatus_cb.GetLightLightnessDefault_cb)(Generic_GetBuff);
   
    bLight_DefaultParam.LightnessDefaultStatus = Generic_GetBuff[1] << 8;
    bLight_DefaultParam.LightnessDefaultStatus |= Generic_GetBuff[0];
  
  /* condition is depends on the generic on off state */  
  if(onOff_param->TargetOnOffState == 0x00)
  {
     bLight_ActualParam.PresentValue16 = 0x00;    
  }
  else if((onOff_param->TargetOnOffState == 0x01) && 
              (bLight_DefaultParam.LightnessDefaultStatus == 0x00))
  {
     bLight_ActualParam.PresentValue16 = bLight_LastParam.LightnessLastStatus;
  }
  else if((onOff_param->TargetOnOffState == 0x01) && 
              (bLight_DefaultParam.LightnessDefaultStatus != 0x000))
  {
     bLight_ActualParam.PresentValue16 = bLight_DefaultParam.LightnessDefaultStatus;
  }
  else
  {
    /* no condition to Execute */
  }
     /* Application callback for setting the light lightness actual value in application
        level
     */
    (LightAppli_cb.Lightness_Set_cb)(&bLight_ActualParam, 0);
    /* implicit binding of lightness linear with generic on off set.
       generic on off set -> actual lightness -> linear lightness set.
    */   
  Light_Linear_ActualImplicitBinding(BINDING_LIGHT_LIGHTNESS_ACTUAL_SET , 0);
}


/*
* @Brief  GenericLevel_LightActualBinding: Data binding b/w Generic Level and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic Level set. 
* @param gLevel_param: Pointer to the data which needs to be checked.
* return void.
*/
void GenericLevel_LightActualBinding(Generic_LevelParam_t* gLevel_param)
{
  /*
    6.1.2.2.2 - Binding of actual light lightness with generic level
                As generic Level changes, the actual lightness value will
                change.
  */
  Light_LightnessStatus_t bLight_ActualParam ; 
  
  bLight_ActualParam.PresentValue16 = gLevel_param->TargetLevel + 32768;  
 (LightAppli_cb.Lightness_Set_cb)(&bLight_ActualParam, 0);
  /* application callback for the actual lightness to get the value.
    Actual lightness is directly bounded with generic on off and generic level
    which implicitly changed the value of linear lightness with generic on    
    off state and generic level changes.
 */     
  Light_Actual_LinearBinding();
 
}


/**
* @brief GenericLevel_CtlTempBinding: Data binding b/w Generic level and Ctl
*        Temperature set.
* @param bLevelParam: pointer to the structure, which should be set.
* return void.
*/
void GenericLevel_CtlTempBinding(Generic_LevelParam_t * bLevelParam)
{ 
     /* 6.1.3.1.1 Binding with the Generic Level state.
       Light CTL Temperature = T_MIN + (Generic Level + 32768) * (T_MAX - T_MIN) / 65535
       T_MIN = minimum range of ctl temperature
       T_MAX = maximum range of ctl temperature
     */
   Light_CtlStatus_t bCtlTempstatus;
   MOBLEUINT32 productValue;
      
  productValue = (bLevelParam->TargetLevel + 32768) * (MAX_CTL_TEMP_RANGE - MIN_CTL_TEMP_RANGE) ;
  bCtlTempstatus.PresentCtlTemperature16 = MIN_CTL_TEMP_RANGE + ((productValue) /65535);
      /* Application callback */
     (LightAppli_cb.Light_CtlTemperature_Set_cb)(&bCtlTempstatus, 0);
}


/*
* @brief GenericLevel_HslHueBinding: Data binding b/w Generic level and Hsl
*        Hue set.
* @param bLevelParam: pointer to the structure, which should be set.
* return void.
*/
void GenericLevel_HslHueBinding(Generic_LevelParam_t * bLevelParam)
{ 
  /*  6.1.4.1.1 Binding with the Generic Level state
      Light HSL Hue = Generic Level + 32768
  */
   Light_HslStatus_t bHslHuestatus; 
 
   bHslHuestatus.PresentHslHueLightness16 = bLevelParam->TargetLevel + 32768;   
   /* Application callback */
   (LightAppli_cb.Light_HslHue_Set_cb)(&bHslHuestatus, 0);
}


/**
* @brief GenericLevel_HslSaturationBinding: Data binding b/w Generic level and Hsl
*        Hue set.
* @param bLevelParam: pointer to the structure, which should be set.
* return void.
*/
void GenericLevel_HslSaturationBinding(Generic_LevelParam_t * bLevelParam)
{ 
  /*  6.1.4.1.1 Binding with the Generic Level state
      Light HSL Hue = Generic Level + 32768
  */
   Light_HslStatus_t bHslSatstatus; 
 
   bHslSatstatus.PresentHslSaturation16 = bLevelParam->TargetLevel + 32768;   
   /* Application callback */
   (LightAppli_cb.Light_HslSaturation_Set_cb)(&bHslSatstatus, 0);
}

/**
* @brief function to assign the Pwm value to the target value of the generic on off 
*        saved states.
* @param void: 
* return void.
*/
void Generic_OnOffDefaultTransitionValue(void)
{
  Generic_TemporaryStatus.RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime; 
  Generic_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime);
  Generic_ModelFlag.GenericTransitionFlag = GENERIC_ON_OFF_TRANSITION_START;
  Generic_ModelFlag.GenericOptionalParam = 1;
}

/**
* @brief function called in generic level when the default transition time is enabled.
* @param levelValue: generic level target value
* return void.
*/
void Generic_LevelDefaultTransitionValue(MOBLEUINT16 levelValue)
{
  Generic_TemporaryStatus.TargetValue16 = levelValue;   
  Generic_TemporaryStatus.RemainingTime = Generic_DefaultTransitionParam.DefaultTransitionTime; 
  Generic_GetStepValue((MOBLEUINT8)Generic_DefaultTransitionParam.DefaultTransitionTime);
  Generic_ModelFlag.GenericTransitionFlag = GENERIC_LEVEL_TRANSITION_START;
  Generic_ModelFlag.GenericOptionalParam = 1;
}

/**
* @brief Generic_Client_OnOff_Status: Function called when status of the model 
received on the client.
* @param pOnOff_status: ointer to the parameters received for message
* @param plength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_OnOff_Status(MOBLEUINT8 const *pOnOff_status, MOBLEUINT32 plength)
{
  TRACE_M(TF_GENERIC_CLIENT,"Generic_OnOff_Status received \r\n");
  GenericAppli_cb.OnOff_Status_cb(pOnOff_status , plength);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief Generic_Client_Level_Status: Function called when status of the model 
received on the client.
* @param plevel_status: ointer to the parameters received for message
* @param plength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_Level_Status(MOBLEUINT8 const *plevel_status, MOBLEUINT32 plength)
{
  
  TRACE_M(TF_GENERIC_CLIENT,"Generic_Level_Status received \r\n");
  GenericAppli_cb.Level_Status_cb(plevel_status , plength);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief Generic_Client_PowerOnOff_Status: Function called when status of the model 
received on the client.
* @param powerOnOff_status: ointer to the parameters received for message
* @param plength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status , MOBLEUINT32 plength) 
{  
  
  TRACE_M(TF_GENERIC_CLIENT,"Generic_PowerOnOff_Status received \r\n"); 
  GenericAppli_cb.GenericPowerOnOff_Status_cb(powerOnOff_status, plength);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief Generic_Client_DefaultTransitionTime_Status: Function called when status of the model 
received on the client.
* @param pTransition_status: ointer to the parameters received for message
* @param plength: Length of the parameters received for message
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , MOBLEUINT32 plength) 
{  
  
  TRACE_M(TF_GENERIC_CLIENT,"Generic_DefaultTransitionTime_Status received \r\n");
  GenericAppli_cb.GenericDefaultTransition_Status_cb(pTransition_status, plength);
  return MOBLE_RESULT_SUCCESS;
}

/**
* Weak function are defined to support the original function if they are not
   included in firmware.
   There is no use of this function for application development purpose.
*/
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t* pGeneric_OnOffParam, 
                                     MOBLEUINT8 OptionalValid))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t* plevelParam, 
                                     MOBLEUINT8 OptionalValid))
{   return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_LevelDelta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                                         MOBLEUINT8 OptionalValid))
{   return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_LevelMove_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                     MOBLEUINT8 OptionalValid))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Set(Generic_PowerOnOffParam_t* pPowerOnOffParam, 
                                                        MOBLEUINT8 OptionalValid))
{ return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (void Appli_Generic_Restore_PowerOn_Value(MOBLEUINT8 restoreValue))
{}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Set(Generic_DefaultTransitionParam_t* pDefaultTimeParam, 
                                               MOBLEUINT8 OptionalValid))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Status(MOBLEUINT8 const *pOnOff_status, MOBLEUINT32 plength))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Status(MOBLEUINT8 const *plevel_status, MOBLEUINT32 plength))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status , MOBLEUINT32 plength))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , MOBLEUINT32 plength))
{  return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffStatus(MOBLEUINT8* pOnOff_Status))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetOnOffValue(MOBLEUINT8* pOnOff_Value) )
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetLevelStatus(MOBLEUINT8* pLevel_Status))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetPowerOnOffStatus(MOBLEUINT8* pPower_Status))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_GetDefaultTransitionStatus(MOBLEUINT8* pTransition_Status))
{return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLE_RESULT Appli_GenericClient_Level_Set_Unack(void))
{return MOBLE_RESULT_SUCCESS;}

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

