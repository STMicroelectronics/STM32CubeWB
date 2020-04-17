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
#include "light_lc.h"
#include "common.h"
#include <string.h>
#include "compiler.h"
#include <stdint.h>


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

static Generic_LevelStatus_t Generic_LevelStatus = {0x8000,0x00,0x00,0x00,0x00,0x00};
 
static Generic_OnOffStatus_t Generic_OnOffStatus;

Generic_DefaultTransitionParam_t Generic_DefaultTransitionParam = {0x06};

static Generic_ModelFlag_t Generic_ModelFlag;

extern MOBLEUINT16 CommandStatus;
MOBLEUINT8 GenericUpdateFlag = 0;
MOBLEUINT16 Model_Rx_Opcode;
MOBLEUINT8 OptionalValid = 0;
MOBLEUINT8 PowerOnOff_Flag = 0;
MOBLEUINT8 TidValue = 0;
MOBLEUINT16 Model_ID = 0;
extern MOBLEUINT8 TidSend;
extern MOBLE_ADDRESS Dst_Peer;

const MODEL_OpcodeTableParam_t Generic_Opcodes_Table[] = {
  /* Generic OnOff Server */
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
    MOBLEUINT16 max_payload_size;
    Here in this array, Handler is not defined; */
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF  
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_GET,                                    MOBLE_TRUE,  0, 0,              GENERIC_ON_OFF_STATUS , 1, 3},
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_SET_ACK,                                MOBLE_TRUE,  2, 4,              GENERIC_ON_OFF_STATUS , 1, 3},  
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_SET_UNACK,                              MOBLE_FALSE, 2, 4,              GENERIC_ON_OFF_STATUS , 1, 3}, 
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID   ,GENERIC_ON_OFF_STATUS,                                 MOBLE_FALSE, 1, 3,              0 , 1, 3},
#endif
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  /* Generic Level Server */
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_GET,                                     MOBLE_TRUE,   0, 0,             GENERIC_LEVEL_STATUS , 2 , 5}, 
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_SET_ACK,                                 MOBLE_TRUE,   3, 5,             GENERIC_LEVEL_STATUS , 2 , 5},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_LEVEL_SET_UNACK,                               MOBLE_FALSE,  3, 5,             GENERIC_LEVEL_STATUS , 2 , 5}, 
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_DELTA_SET,                                     MOBLE_TRUE,   5, 7,             GENERIC_LEVEL_STATUS , 2 , 5},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_DELTA_SET_UNACK,                               MOBLE_FALSE,  5, 7,             GENERIC_LEVEL_STATUS, 2 , 5},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_MOVE_SET,                                      MOBLE_TRUE,   3, 5,             GENERIC_LEVEL_STATUS , 2 , 5},
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID   ,GENERIC_MOVE_SET_UNACK,                                 MOBLE_FALSE,  3, 5,             GENERIC_LEVEL_STATUS, 2 , 5},
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
  {GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID   ,GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK,             MOBLE_FALSE, 1, 1,              GENERIC_DEFAULT_TRANSITION_TIME_STATUS  ,1 ,1}, 
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
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Delta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                                         MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Move_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                     MOBLEUINT8 OptionalValid));

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Generic_OnOff_Set: This function is called for both Acknowledged and 
*         unacknowledged message
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
      
    /* Copy the received data in status message which needs
       to be set in application messages
    */
    Generic_OnOffStatus.Target_OnOff =  Generic_OnOffParam.TargetOnOffState;
    Generic_OnOffStatus.RemainingTime = Generic_OnOffParam.Transition_Time;

    Generic_TemporaryStatus.RemainingTime = Generic_OnOffStatus.RemainingTime;
    /* Function to calculate time parameters, step resolution
      step size for transition state machine
    */
    Generic_GetStepValue(pOnOff_param[2]);
    /*option parameter flag, enable to sent all required parameter in status.*/      
    Generic_ModelFlag.GenericOptionalParam = 1;
    /*Flag to enable the on Off transition state machine */
    Generic_ModelFlag.GenericTransitionFlag = GENERIC_ON_OFF_TRANSITION_START;
    /* flag is used for the application to get the information about the transiotn 
       time parameter is included or not in the received message.
    */
    OptionalValid = IN_TRANSITION;
  } 
  else
  {
    /* when default transition time enabled,and the target time is not given by client
       the transition time will be used from default value
      */
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME     
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

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
   /* Binding of data b/w Generic on off and Light lightness Actual model */
  GenericOnOff_LightActualBinding(&Generic_OnOffStatus);
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC 
  GenericOnOff_Light_LC_Binding(&Generic_OnOffStatus);
#endif
  
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
  
  TRACE_M(TF_GENERIC,"Generic_OnOff_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8201! \n\r");
   /* 
  Default value of GenericOptionalParam=0, 
  GenericOptionalParam set equal to 1 in Generic_OnOff_Set for Generic_OnOff_Status 
  */
   if((Generic_ModelFlag.GenericOptionalParam == 1) || (Generic_TimeParam.StepValue != 0))
   {   
/*  
     When optional parameter received present value,targert value, remaing time be sent in status message
     length of received data is equal to 4B
    */
    *pOnOff_status = Generic_OnOffStatus.Present_OnOff_State;
       *(pOnOff_status+1) = Generic_OnOffStatus.Target_OnOff;
       *(pOnOff_status+2) = Generic_OnOffStatus.RemainingTime;
       *plength = 3; 
       Generic_ModelFlag.GenericOptionalParam = 0;
   }
   else
   {   /* When no optional parameter received, target value will
         be sent in status message.
    length of received data is equal to 2B
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

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
     /* Binding of Generic level with light lightnes actual */
  GenericLevel_LightBinding(&Generic_LevelStatus,BINDING_GENERIC_LEVEL_SET);
#endif 
        
#endif   /* ENABLE_MODEL_BINDING */
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Delta_Set: This function is called for both Acknowledged 
*         and unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Delta_Set(const MOBLEUINT8* plevel_param, MOBLEUINT32 length)
{
  /*
  3.2.2.4 Generic Delta Set
  Delta Level: 4B The Delta change of the Generic Level state
  TID:   1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 milliseconds steps (C.1)
  */
  
  TRACE_M(TF_GENERIC,"Generic_Delta_Set callback received \r\n");
  
  Generic_DeltaLevelParam_t Generic_DeltaLevelParam ;
  MOBLEUINT32 delta;
  
  /* Copy the 4Bytes data to local variable */
  delta = (plevel_param[3] << 24);
  delta |= (plevel_param[2] << 16);
  delta |= (plevel_param[1] << 8);
  delta |= (plevel_param[0]);
  Generic_DeltaLevelParam.TargetDeltaLevel32 = delta;
  
  TRACE_M(TF_GENERIC,"Generic delta value %.2lx \r\n"
                      ,Generic_DeltaLevelParam.TargetDeltaLevel32);
  
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
        if(Generic_DeltaLevelParam.TargetDeltaLevel32 > Generic_LevelStatus.Last_delta_level)
        {
          Generic_LevelStatus.Present_Level16 += (Generic_DeltaLevelParam.TargetDeltaLevel32  
          - Generic_LevelStatus.Last_delta_level);     
        }
        else
        {
          Generic_LevelStatus.Present_Level16 -= (Generic_LevelStatus.Last_delta_level
            -Generic_DeltaLevelParam.TargetDeltaLevel32) ;           
        }       
      }
      else
      {         
       if(Generic_DeltaLevelParam.TargetDeltaLevel32 > Generic_LevelStatus.Last_delta_level)
       {
         Generic_LevelStatus.Present_Level16 += Generic_DeltaLevelParam.TargetDeltaLevel32;  
      }
       else
       {
          Generic_LevelStatus.Present_Level16 -= Generic_DeltaLevelParam.TargetDeltaLevel32;
       }
    }
    }
  
  TRACE_M(TF_GENERIC,"Generic Level value %.2x \r\n" ,Generic_LevelStatus.Present_Level16);
  
  Generic_LevelStatus.Last_delta_level = Generic_DeltaLevelParam.TargetDeltaLevel32;
         Generic_LevelStatus.Last_Level_TID = Generic_DeltaLevelParam.Generic_TID;   
      
    /* Application Callback */
   (GenericAppli_cb.LevelDelta_Set_cb)(&Generic_LevelStatus, 0);
#ifdef ENABLE_MODEL_BINDING    
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS  
  /* Binding of Generic level with light lightnes actual */
  GenericLevel_LightBinding(&Generic_LevelStatus,BINDING_GENERIC_LEVEL_SET);
#endif  
#endif  
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Move_Set: This function is called for both 
*         Acknowledged and unacknowledged message
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_Move_Set(const MOBLEUINT8* plevel_param, MOBLEUINT32 length)
{
  /*
  3.2.2.6 Generic Level Move Set
  Level: 2B The target value of the Generic Level state
  TID:   1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 milliseconds steps (C.1)
  */
  TRACE_M(TF_GENERIC,"Generic_Move_Set callback received \r\n");
  
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
  
  TRACE_M(TF_GENERIC,"Generic_Level_Status callback received \r\n");
  TRACE_M(TF_SERIAL_CTRL,"#8205! \n\r");
  
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
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 elementNumber;
  MOBLEUINT8 modelStateChangeFlag = MOBLE_FALSE;  
  MOBLE_ADDRESS my_Address;
  Model_Rx_Opcode = opcode;
  
  my_Address = BLEMesh_GetAddress();
  Dst_Peer = dst_peer;
  Model_Rx_Opcode = opcode; 
  
  TRACE_M(TF_GENERIC,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X ,response= %.2X \r\n  ",
                                                       dst_peer, peer_addr, opcode , response);
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
       
      result = Chk_ParamMinMaxIntValidity(INT16_MIN ,pRxData , INT16_MAX );        
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
    
  case GENERIC_DELTA_SET:
  case GENERIC_DELTA_SET_UNACK:
      { 
        /*
        Delta Level 4 The Delta change of the Generic Level state 
        TID 1 Transaction Identifier 
        Transition Time 1 Format as defined in Section 3.1.3. (Optional) 
        Delay 1 Message execution delay in 5 milliseconds steps (C.1)
        */
        Generic_Delta_Set(pRxData,dataLength);   
        Model_ID = (MOBLEUINT16)GENERIC_MODEL_SERVER_LEVEL_MODEL_ID;
        modelStateChangeFlag = MOBLE_TRUE;
        break;
      }
      
  case GENERIC_MOVE_SET:
  case GENERIC_MOVE_SET_UNACK:
      {    
       
      result = Chk_ParamMinMaxIntValidity(INT16_MIN ,pRxData , INT16_MAX );       
        if(result == MOBLE_RESULT_SUCCESS)
        {
         if(!MOBLE_FAILED(result = Chk_TidValidity(peer_addr,dst_peer,pRxData[2])))
         {
           Generic_Move_Set(pRxData, dataLength);   
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
          Model_ID = (MOBLEUINT16)GENERIC_MODEL_SERVER_POWER_ONOFF_MODEL_ID;
          modelStateChangeFlag = MOBLE_TRUE;
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
        Model_ID = (MOBLEUINT16)GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME_MODEL_ID;
        modelStateChangeFlag = MOBLE_TRUE;
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
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE) && (ADDRESS_IS_UNICAST(dst_peer)))
  {
    Model_SendResponse(peer_addr,my_Address,opcode,pRxData,dataLength);
  }
  
  /*
     Publish the status to publish address if the publication is set by client 
     publication is independent of the response of the message.if thek condition 
     for publication is full filled as per specification then the status will be 
     published.
  */
    elementNumber = BLE_GetElementNumber();
    publishAddress = BLEMesh_GetPublishAddress(elementNumber,Model_ID);
    
  if((result == MOBLE_RESULT_SUCCESS) && (publishAddress != 0x0000) && (modelStateChangeFlag == MOBLE_TRUE)
      && (ADDRESS_IS_UNICAST(dst_peer)))
  {
    Model_SendResponse(publishAddress,my_Address,opcode,pRxData,dataLength);
    
    modelStateChangeFlag = MOBLE_FALSE;   
    TRACE_I(TF_GENERIC,"Publishing state when change to the address %.2X \r\n",publishAddress);
  }
  return MOBLE_RESULT_SUCCESS;
}


/* @Brief  Generic_TransitionBehaviour: Generic On Off Transition behaviour 
*          used for the Generic On Off model when transition time is received in
*          message.        
* @param GetValue Pointer of the array
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Generic_TransitionBehaviour(MOBLEUINT8 *GetValue)
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
      Generic_ModelFlag.Generic_Trnsn_Cmplt = MOBLE_TRUE;
        }
    TRACE_M(TF_GENERIC,"Inside virtual application at %ld, Current state 0x%.2x, Target state 0x%.2x, Remaining Time 0x%.2x \n\r",
             Clock_Time(), Generic_TemporaryStatus.PresentValue16,Generic_TemporaryStatus.TargetValue16,Generic_TemporaryStatus.RemainingTime);  
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
  MOBLE_ADDRESS my_Address;
  MOBLEUINT8 elementNumber;
  MOBLEUINT8 const pRxData[8] = {0};
  MOBLEUINT32 dataLength = 0;
  
#if defined ENABLE_GENERIC_MODEL_SERVER_ONOFF || defined ENABLE_GENERIC_MODEL_SERVER_LEVEL  
     MOBLEUINT8 Generic_GetBuff[8]; 
#endif     
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF   
  if(Generic_ModelFlag.GenericTransitionFlag == GENERIC_ON_OFF_TRANSITION_START)
  {   
    /* Fetching the state value from the applcation layer to middle layer */
    (Appli_GenericState_cb.GetOnOffValue_cb)(Generic_GetBuff);
     Generic_TemporaryStatus.TargetValue16  = Generic_GetBuff[3] << 8;
     Generic_TemporaryStatus.TargetValue16 |= Generic_GetBuff[2];
    /* Transition function called to change the state in transition */
    Generic_TransitionBehaviour(Generic_GetBuff);
    if(GenericUpdateFlag == VALUE_UPDATE_SET)
    {
      /* updating the state value after the each transition step complete */
      GenericOnOffStateUpdate_Process();
      /* Application callback */
      (GenericAppli_cb.OnOff_Set_cb)(&Generic_OnOffStatus, OptionalValid);  
      /*binding of the generic on off with light model */
      GenericOnOff_LightActualBinding(&Generic_OnOffStatus);
      GenericUpdateFlag = VALUE_UPDATE_RESET;     
    }
  }    
#endif 
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL       
  if(Generic_ModelFlag.GenericTransitionFlag == GENERIC_LEVEL_TRANSITION_START)
  {    
    /* Fetching the state value from the applcation layer to middle layer */
     (Appli_GenericState_cb.GetLevelStatus_cb)(Generic_GetBuff);
    /* Transition function called to change the state in transition */
    Generic_TransitionBehaviour(Generic_GetBuff);
    if(GenericUpdateFlag == VALUE_UPDATE_SET)
    {
      /* updating the state value after the each transition step complete */
      GenericLevelStateUpdate_Process();
      /* Application callback */
     (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus, 0);                             
      /*binding of the generic Level with light model */
      GenericLevel_LightBinding(&Generic_LevelStatus,BINDING_GENERIC_LEVEL_SET);
      GenericUpdateFlag = VALUE_UPDATE_RESET;
    }
                               
  }   
#endif
  
  if(Generic_ModelFlag.Generic_Trnsn_Cmplt == MOBLE_TRUE)
  {         
    my_Address = BLEMesh_GetAddress();
    elementNumber = BLE_GetElementNumber();
    publishAddress = BLEMesh_GetPublishAddress(elementNumber,Model_ID);
    if(publishAddress != 0x00)
    {
      Model_SendResponse(publishAddress,my_Address,Model_Rx_Opcode,pRxData,dataLength);
    }
    Generic_ModelFlag.Generic_Trnsn_Cmplt = MOBLE_FALSE;
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
* @brief  LightActual_GenericOnOffBinding: Reverse Data binding b/w Generic On Off and 
*         light lightness Actual. this function will set the On Off status of
*         Light when the light lightness Actual is set.
* @param lightActual: used to select the binding and reverse binding 
* return void.
*/
void LightActual_GenericOnOffBinding(Light_LightnessStatus_t* lightActual)
{
  /*
  6.1.2.2.3 - Binding of actual light lightness with generic on off,
  As actual lightness will set which changes the on off state in 
  generic model
  */
  if(lightActual->LightnessPresentValue16 == 0x00)
  {
    Generic_OnOffStatus.Present_OnOff_State = 0x00;
  }
  else
  {
    Generic_OnOffStatus.Present_OnOff_State = 0x01;
  }
  
  TRACE_M(TF_LIGHT, "LightActual_GenericOnOffBinding Function - Present OnOff value 0x%.2x \r\n" 
         , Generic_OnOffStatus.Present_OnOff_State);    
 
}

/**
* @brief  LightActual_GenericLevelBinding: Data binding b/w Generic Level and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic Level set. 
* @param lightActual: Pointer to the data which needs to be checked.
* return void.
*/
void LightActual_GenericLevelBinding(Light_LightnessStatus_t* lightActual)
{ 
  /*
  6.1.2.2.2 - Binding of actual light lightness with generic level
  As Actual lightness changes, the generic level value will
  change.
     */
  Generic_LevelStatus.Present_Level16 = lightActual->LightnessPresentValue16 - 32768;  
 
   TRACE_M(TF_LIGHT, "LightActual_GenericLevelBinding Function - Present Level value 0x%.2x \r\n" 
         , Generic_LevelStatus.Present_Level16); 
      
}


/**
* @brief  Light_CtlTemp_GenericLevelBinding: Data binding b/w Generic Level and 
*         light Ctl temperature . this function will set the generic Level
*         value at the time of  Ctl temperature value set. 
* @param bCtlTempParam: Pointer to the data which needs to be checked.
* return void.
*/
void Light_CtlTemp_GenericLevelBinding(Light_CtlStatus_t* bCtlTempParam)
{ 
  /* 6.1.3.1.1 Binding with the Generic Level state.
  Generic Level = (Light CTL Temperature - T _MIN) * 65535 / (T_MAX - T_MIN) - 32768
  T_MIN = minimum range of ctl temperature
  T_MAX = maximum range of ctl temperature
  */
  MOBLEUINT32 productValue;
  
  productValue = (bCtlTempParam->PresentCtlTemperature16 - MIN_CTL_TEMP_RANGE) * 65535;
  Generic_LevelStatus.Present_Level16 = (productValue / (MAX_CTL_TEMP_RANGE - MIN_CTL_TEMP_RANGE)) - 32768;
 
  GenericLevel_LightBinding(&Generic_LevelStatus , BINDING_LIGHT_CTL_TEMP_SET);
}

/**
* @brief  Light_HslHue_GenericLevelBinding: Data binding b/w Generic Level and 
*         light Hsl . this function will set the Generic Level
*         value at the time of  Hsl Hue value set. 
* @param bHslHueParam: Pointer to the data which needs to be checked.
* return void.
*/
void Light_HslHue_GenericLevelBinding(Light_HslStatus_t* bHslHueParam)
{ 
  /*  6.1.4.1.1 Binding with the Generic Level state
  Generic Level = Light HSL Hue – 32768
  */
  Generic_LevelStatus.Present_Level16 = bHslHueParam->PresentHslHueLightness16- 32768;
 
  GenericLevel_LightBinding(&Generic_LevelStatus , BINDIG_LIGHT_HSL_HUE_SET);
}

/**
* @brief  Light_HslSaturation_GenericLevelBinding: Data binding b/w Generic Level and 
*         light Hsl . this function will set the Generic Level
*         value at the time of  Hsl Saturation value set. 
* @param bHslSatParam: Pointer to the data which needs to be checked.
* return void.
*/
void Light_HslSaturation_GenericLevelBinding(Light_HslStatus_t* bHslSatParam)
{
  /* 6.1.4.4.1 Binding with the Generic Level state
  Generic Level = Light HSL Saturation – 32768
  */ 
  Generic_LevelStatus.Present_Level16 = bHslSatParam->PresentHslSaturation16 - 32768;
 
  GenericLevel_LightBinding(&Generic_LevelStatus , BINDIG_LIGHT_HSL_SATURATION_SET);
 
}

/**
* @brief  Light_LC_GenericOnOffBinding: Reverse Data binding b/w Generic On Off and 
*         light LC on off. this function will set the Generic On Off status of
*         Light when the light LC on off is set.
* @param lightActual: used to select the binding and reverse binding 
* return void.
*/
void Light_LC_GenericOnOffBinding(Light_LC_Param_t* light_LC)
{
  if(light_LC->Present_Light_OnOff == 0x00)
  {
    Generic_OnOffStatus.Present_OnOff_State = 0x00;
  }
  else
  {
    Generic_OnOffStatus.Present_OnOff_State = 0x01;
  }
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
*        received on the client.
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
*        received on the client.
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
*        received on the client.
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
*        received on the client.
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

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Delta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                                         MOBLEUINT8 OptionalValid))
{   return MOBLE_RESULT_SUCCESS;}

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Move_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
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

