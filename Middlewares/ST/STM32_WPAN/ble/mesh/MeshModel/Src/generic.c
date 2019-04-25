/**
******************************************************************************
* @file    generic.c
* @author  BLE Mesh Team
* @version V1.09.000
* @date    15-Oct-2018
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
#include "ble_mesh.h"
#include "models_if.h"
#include "appli_generic.h"
#include "common.h"
#include "vendor.h"
#include "mesh_cfg.h"
#include "compiler.h"
#include <string.h>


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

static Generic_ModelFlag_t Generic_ModelFlag;

 const MODEL_OpcodeTableParam_t generic_on_off_opcodes[] = {
  /* Generic OnOff Server */
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
    MOBLEUINT16 max_payload_size;
    Here in this array, Handler is not defined; */
       
    {GENERIC_ON_OFF_GET,                                    MOBLE_TRUE,  0, 0,              GENERIC_ON_OFF_STATUS , 1, 3},
    {GENERIC_ON_OFF_SET_ACK,                                MOBLE_TRUE,  2, 4,              GENERIC_ON_OFF_STATUS , 1, 3},  
    {GENERIC_ON_OFF_SET_UNACK,                              MOBLE_FALSE, 2, 4,              NULL, 0, 0},
     
    /* Generic Level Server */
    {GENERIC_LEVEL_GET,                                     MOBLE_TRUE,  0, 0,              GENERIC_LEVEL_STATUS , 2 , 5}, 
    {GENERIC_LEVEL_SET_ACK,                                 MOBLE_TRUE,  3, 5,              GENERIC_LEVEL_STATUS , 2 , 5},
    {GENERIC_LEVEL_SET_UNACK,                               MOBLE_FALSE,  3, 5,             NULL,  0 , 0},
    {GENERIC_LEVEL_DELTA_SET,                               MOBLE_TRUE,  5, 7,              GENERIC_LEVEL_STATUS , 2 , 5},
    {GENERIC_LEVEL_DELTA_SET_UNACK,                         MOBLE_FALSE,  5, 7,             NULL,  0, 0},
    {GENERIC_LEVEL_DELTA_MOVE_SET,                          MOBLE_TRUE,  3, 5,              GENERIC_LEVEL_STATUS , 2 , 5},
    {GENERIC_LEVEL_DELTA_MOVE_SET_UNACK,                    MOBLE_FALSE,  3, 5,             NULL,  0 , 0},
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME     
    /* Generic Default Transition Time Server Model  */
    {GENERIC_DEFAULT_TRANSITION_TIME_GET,                   MOBLE_TRUE, 0, 0,               GENERIC_DEFAULT_TRANSITION_TIME_STATUS , 1, 1}, 
    {GENERIC_DEFAULT_TRANSITION_TIME_SET,                   MOBLE_TRUE, 1, 1,               GENERIC_DEFAULT_TRANSITION_TIME_STATUS , 1, 1},
    {GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK,             MOBLE_FALSE, 1, 1,              NULL ,0 ,0},
#endif  
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_BATTERY     
    /* Generic Battery Server Model  */
    {GENERIC_BATTERY_GET,                                   MOBLE_TRUE,  0, 0,              GENERIC_BATTERY_STATUS , 8 , 8},      
#endif
    {0}
};

extern MOBLEUINT16 CommandStatus;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Generic_OnOff_Set: This function is called for both Acknowledged and 
          unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Generic_OnOff_Set(const MOBLEUINT8* pOnOff_param, MOBLEUINT32 length)
{
  
  /* 3.2.1.2 Generic OnOff Set 
  OnOff: 1B The target value of the Generic OnOff state 
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 millisecond steps (C.1)
  */
#if !defined(DISABLE_TRACES)
  printf("Generic_OnOff_Set callback received \r\n");
#endif

  Generic_OnOffParam_t Generic_OnOffParam; 
  
  Generic_OnOffParam.TargetOnOffState = pOnOff_param[0];
  Generic_OnOffParam.Generic_TID = pOnOff_param[1];
  
  if(length > 2)
  {
      /* Transition_Time & Delay_Time Present */
      Generic_OnOffParam.Transition_Time = pOnOff_param[2];
      Generic_OnOffParam.Delay_Time = pOnOff_param[3];
      
      /* Copy the received data in status message which needs
        to be set in application messages
     */
      Generic_OnOffStatus.Target_OnOff =  Generic_OnOffParam.TargetOnOffState;;
      Generic_OnOffStatus.RemainingTime = Generic_OnOffParam.Transition_Time;
      
      /* copy status parameters in Temporary parameters for transition 
         process
      */
      Generic_TemporaryStatus.TargetValue16 = Generic_OnOffStatus.Target_OnOff;
      Generic_TemporaryStatus.RemainingTime = Generic_OnOffStatus.RemainingTime;
      /* Function to calculate time parameters, step resolution
        step size for transition state machine
      */
      Generic_GetStepValue(pOnOff_param[2]);
      
      /*option parameter flag, enable to sent all required parameter in status.*/      
      Generic_ModelFlag.GenericOptionalParam = 1;
      /*Flag to enable the on Off transition state machine */
      Generic_ModelFlag.GenericTransitionFlag = 1;   
  } 
  else
  {
      /* When no optional parameter received, target value will
         be set as present value in application.
      */
       Generic_OnOffStatus.Present_OnOff = Generic_OnOffParam.TargetOnOffState;
  }
     /* Application Callback */
     (GenericAppli_cb.OnOff_Set_cb)(&Generic_OnOffStatus, 0);
     /* Binding of data b/w Generic on off and Light lightness Actual model */
     GenericOnOff_LightActualBinding(&Generic_OnOffParam);
      
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
#if !defined(DISABLE_TRACES)
  printf("Generic_OnOff_Status callback received \r\n");
#endif
   /* Function call back to get the values from application*/
    (Appli_GenericState_cb.GetOnOffStatus_cb)(Generic_GetBuff);
    Generic_OnOffStatus.Present_OnOff = Generic_GetBuff[0];
   
   if((Generic_ModelFlag.GenericOptionalParam == 1) || (Generic_TimeParam.StepValue != 0))
   {   
       *pOnOff_status = Generic_OnOffStatus.Present_OnOff;
       *(pOnOff_status+1) = Generic_OnOffStatus.Target_OnOff;
       *(pOnOff_status+2) = Generic_OnOffStatus.RemainingTime;
       *plength = 3; 
       Generic_ModelFlag.GenericOptionalParam = 0;
   }
   else
   {   /* When no optional parameter received, target value will
         be sent in status message.
      */
       *pOnOff_status = Generic_OnOffStatus.Present_OnOff;
       *plength = 1;
   }
      return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_Level_Set: This function is called for both Acknowledged and 
          unacknowledged message
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
#if !defined(DISABLE_TRACES)
  printf("Generic_Level_Set callback received \r\n");
#endif
  Generic_LevelParam_t Generic_LevelParam;
  MOBLEUINT32 setValue;
  
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
 
     /*transition process enable flag. */
     Generic_ModelFlag.GenericTransitionFlag = 2;
  }
  else
  {     
     /* When no optional parameter received, target value will
         be set as present value in application.
     */
     Generic_LevelStatus.Present_Level16= setValue; 
  }  
     Generic_LevelStatus.Last_Present_Level16 = Generic_LevelStatus.Present_Level16;
 
     /* Application Callback */
     (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus, 0); 
     
     /* Binding of Generic level with light lightnes actual */
      GenericLevel_LightActualBinding(&Generic_LevelParam);
     /* Binding of Generic Level data with ctl temperature data */ 
      GenericLevel_CtlTempBinding(&Generic_LevelParam);
     /* Generic Level binding with Hsl Hue Value */
      GenericLevel_HslHueBinding(&Generic_LevelParam);
     /* Generic Level Binding with Hsl Saturation */
      GenericLevel_HslSaturationBinding(&Generic_LevelParam);
        
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_LevelDelta_Set: This function is called for both Acknowledged 
          and unacknowledged message
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
#if !defined(DISABLE_TRACES)
    printf("Generic_LevelDelta_Set callback received \r\n");
#endif
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
       Generic_ModelFlag.GenericTransitionFlag = 2;
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
      
    /* Application Callback */
   (GenericAppli_cb.LevelDelta_Set_cb)(&Generic_LevelStatus, 0);
   
   return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Generic_LevelMove_Set: This function is called for both 
           Acknowledged and unacknowledged message
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
#if !defined(DISABLE_TRACES)
    printf("Generic_LevelMove_Set callback received \r\n");
#endif
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
     Generic_ModelFlag.GenericTransitionFlag = 2;
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
#if !defined(DISABLE_TRACES)
  printf("Generic_Level_Status callback received \r\n");
#endif
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


   /* 
* @brief  BLEMesh_AddGenericModels
* @param  Function adds all the sub model of generic model. this function 
*         uses Mid of the particular model for adding.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLEMesh_AddGenericModels(void)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF   
  if(MOBLE_FAILED(GenericModel_Add_Server(GENERIC_MODEL_SERVER_ONOFF_MODEL_ID)))
  {
#if !defined(DISABLE_TRACES)
    printf("Failed to add Generic On Off Model \n\r");
#endif
    result = MOBLE_RESULT_FAIL;
  } 
#endif    

#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL   
  if(MOBLE_FAILED(GenericModel_Add_Server(GENERIC_MODEL_SERVER_LEVEL_MODEL_ID)))
  {
#if !defined(DISABLE_TRACES)
    printf("Failed to add Generic Level Model \n\r");
#endif		
    result = MOBLE_RESULT_FAIL;
  }
#endif   

     return result;    
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
  *data = generic_on_off_opcodes;
  *length = sizeof(generic_on_off_opcodes)/sizeof(generic_on_off_opcodes[0]);

  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  GenericModelServer_GetStatusRequestCb : This function is call-back 
          from the library to send response to the message from peer
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEBOOL response)
{
  switch(opcode)
  {
  case GENERIC_ON_OFF_STATUS:
    {
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF       
      Generic_OnOff_Status(pResponsedata, plength);      
#endif   
      break;
    }
    
  case GENERIC_LEVEL_STATUS:
    {
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL    
      Generic_Level_Status(pResponsedata, plength);
#endif      
      break;
    }
  default:
    {
      break;
    }
  }
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief  GenericModelServer_ProcessMessageCb: This is a callback function from
           the library whenever a Generic Model message is received
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *pData, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response
                                    )
{

  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  tClockTime delay_t = Clock_Time();
#if !defined(DISABLE_TRACES)   
  printf("dst_peer = %.2X , peer_add = %.2X \r\n ",dst_peer, peer_addr);
#endif  
  switch(opcode)
  {
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
    
    case GENERIC_ON_OFF_SET_ACK:
    case GENERIC_ON_OFF_SET_UNACK:
      {

        result = Chk_ParamValidity(pData[0], 1); 
        /* 3.1.1 Generic OnOff 0x02–0xFF Prohibited */
        /* 3.2.1.2 Generic OnOff Set If the Transition Time field is present, 
        the Delay field shall also be present; otherwise these fields shall 
        not be present*/
      
        /* 3.2.1.2 Generic OnOff Set 
        Check if Transition Time field is present or Not,
        If present, Only values of 0x00 through 0x3E shall be used to specify 
        the value of the Transition Number of Steps field. */

        result |= Chk_OptionalParamValidity (length, 2, (pData[2]&0x3F), 0x3E );        
        if(result == MOBLE_RESULT_SUCCESS)
        {
          /*
           when device is working as proxy and is a part of node
           delay will be included in the toggelinf of led.
          */
            Generic_OnOff_Set(pData, length);   
        }
        
        break;
      }
#endif 
      
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
  
    case GENERIC_LEVEL_SET_ACK:
    case GENERIC_LEVEL_SET_UNACK: 
      {   
       
        result = Chk_ParamMinMaxValidity(MIN_VALID_RANGE ,pData , MAX_VALID_RANGE );        
        if(result == MOBLE_RESULT_SUCCESS)
        {
          Generic_Level_Set(pData, length);   
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
       
        result = Chk_ParamMinMaxValidity(MIN_VALID_RANGE ,pData , MAX_VALID_RANGE );        
        if(result == MOBLE_RESULT_SUCCESS)
        {
          Generic_LevelDelta_Set(pData, length);
        }
        break;
      }
      
    case GENERIC_LEVEL_DELTA_MOVE_SET:
    case GENERIC_LEVEL_DELTA_MOVE_SET_UNACK:
      {    
       
        result = Chk_ParamMinMaxValidity(MIN_VALID_RANGE ,pData , MAX_VALID_RANGE );       
        if(result == MOBLE_RESULT_SUCCESS)
        {
        Generic_LevelMove_Set(pData, length); 
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
    Generic_SendResponse(peer_addr, dst_peer,opcode, length);
  }
  return MOBLE_RESULT_SUCCESS;
}


/* @Brief  Generic_TransitionBehaviourSingle_Param: Generic On Off Transition behaviour 
*          used for the Generic On Off model when transition time is received in
           message.        
* @param GetValue Pointer of the array
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Generic_TransitionBehaviourSingle_Param(MOBLEUINT8 *GetValue)
{
  
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;
  
  /* Taking the time stamp for particular time */
  if(Clockflag == 0)
  {
    Check_time = Clock_Time();
    Clockflag = 1;
  }
   /* Values from application are copied into Temporary vaiables for processing */
  
   Generic_TemporaryStatus.PresentValue16 = GetValue[0];
  
    /*if condition to wait untill the time is equal to the given resolution time */
   if(((Clock_Time()- Check_time) >= Generic_TimeParam.Res_Value))
   {     
        
      if(Generic_TimeParam.StepValue == 0)
      {
        Generic_TimeParam.StepValue = 1;
      }
        Generic_TimeParam.StepValue--;
        
        Generic_TemporaryStatus.PresentValue16 = Generic_TemporaryStatus.TargetValue16;
        
        /* updating the remaining time after each step covered*/
        Generic_TemporaryStatus.RemainingTime = Generic_TimeParam.StepValue | (Generic_TimeParam.ResBitValue << 6) ;
     
        Check_time = 0;
        Clockflag = 0;
        /* when transition is completed, disable the transition by disabling 
           transition flag
        */
        if(Generic_TimeParam.StepValue <= 0)
        {
          Generic_ModelFlag.GenericTransitionEnd = Generic_ModelFlag.GenericTransitionFlag; 
          Generic_ModelFlag.GenericTransitionFlag = 0;           
        }
#if !defined(DISABLE_TRACES)         
      printf("Inside virtual application at %d, Current state 0x%.2x, Target state 0x%.2x, Remaining Time 0x%.2x \n\r", 
             Clock_Time(), Generic_TemporaryStatus.PresentValue16,Generic_TemporaryStatus.TargetValue16,Generic_TemporaryStatus.RemainingTime);  
#endif
    }
return MOBLE_RESULT_SUCCESS;       

} 


/* Generic_TransitionBehaviourMulti_Param: This funtion is used for the Generic Level 
*                                    and Power Level model when transition time is 
*                                    received in message.
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
        /* when transition is completed, disable the transition by disabling 
         transition flag
        */
        if(Generic_TimeParam.StepValue <= 0)
        {
          Generic_ModelFlag.GenericTransitionFlag = 0;           
        }
#if !defined(DISABLE_TRACES)         
      printf("Inside virtual level application at %d, Current state 0x%.2x ,target state 0x%.2x , Remaining Time 0x%.2x \n\r",
                Clock_Time(),Generic_TemporaryStatus.PresentValue16,Generic_TemporaryStatus.TargetValue16,               
                    Generic_TemporaryStatus.RemainingTime);
#endif
    }

return MOBLE_RESULT_SUCCESS;         
} 


/** 
* @brief Generic_GetStepValue: This function calculates parameters for transition time
* @param stepParam: Transition time set value of particular model
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
   Generic_TimeParam.TotalTime = (Generic_TimeParam.Res_Value * Generic_TimeParam.StepValue);
#if !defined(DISABLE_TRACES)     
   printf(" step resolution 0x%.2x, number of step 0x%.2x \r\n",
          Generic_TimeParam.Res_Value , Generic_TimeParam.StepValue );
#endif
}


/**
* @brief  Generic_Process: Function to execute the transition state machine for
          particular Generic Model
* @param  void
* @retval void
*/ 
void Generic_Process(void)
{       
#if defined ENABLE_GENERIC_MODEL_SERVER_ONOFF || defined ENABLE_GENERIC_MODEL_SERVER_LEVEL  
     MOBLEUINT8 Generic_GetBuff[8]; 
#endif     
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF   
 if(Generic_ModelFlag.GenericTransitionFlag == 1)
  {   
     (Appli_GenericState_cb.GetOnOffStatus_cb)(Generic_GetBuff);
      Generic_TransitionBehaviourSingle_Param(Generic_GetBuff);
      GenericOnOffStateUpdate_Process();
     (GenericAppli_cb.OnOff_Set_cb)(&Generic_OnOffStatus, 0);
  }    
#endif 
 
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL       
  if(Generic_ModelFlag.GenericTransitionFlag == 2)
  {    
     (Appli_GenericState_cb.GetLevelStatus_cb)(Generic_GetBuff);
      Generic_TransitionBehaviourMulti_Param(Generic_GetBuff);
      GenericLevelStateUpdate_Process();
     (GenericAppli_cb.Level_Set_cb)(&Generic_LevelStatus, 0);                             
  }   
#endif
    
}


 /**
* @brief  Generic_Publish: Publish command for Generic Model
* @param  publishAddress: Publish Address of the message 
* @param  elementIndex: index of the element
* @retval void
*/ 
void Generic_Publish(MOBLE_ADDRESS publishAddress, MOBLEUINT8 elementIndex)
{
    MOBLEUINT8 generic_Buff[2]; 
  
  /* changes the LED status on other nodes in the network */
  if(CommandStatus == (MOBLEUINT16)APPLI_LED_ON)
  {
    generic_Buff[0] = APPLI_LED_OFF;
  }
  else
  {
    generic_Buff[0] = APPLI_LED_ON;
  }
  
  BLEMesh_SetRemoteData(publishAddress, elementIndex,
                            GENERIC_ON_OFF_SET_UNACK, 
                            generic_Buff, 2,
                            MOBLE_FALSE, MOBLE_FALSE);
  
    CommandStatus = generic_Buff[0];
}


/*
* @brief GenericOnOffStateUpdate_Process:Function to update the parametes of 
*        Generic On Off model in application file from Temporary parameter in model file.
* @param void
* return MOBLE_RESULT.
*/
MOBLE_RESULT GenericOnOffStateUpdate_Process(void)
{
   Generic_OnOffStatus.Present_OnOff = Generic_TemporaryStatus.PresentValue16;
   Generic_OnOffStatus.Target_OnOff  = Generic_TemporaryStatus.TargetValue16;
   Generic_OnOffStatus.RemainingTime = Generic_TemporaryStatus.RemainingTime;
   
  return MOBLE_RESULT_SUCCESS;
}


/*
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


/*
* @Brief  GenericOnOff_LightActualBinding: Data binding b/w Generic On Off and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic on off set. 
* @param onOff_param Pointer to the data which needs to be checked.
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
    /* No Condition to Execute */
  }
     /* Application callback for setting the light lightness actual value in application
        level
     */
    (LightAppli_cb.Lightness_Set_cb)(&bLight_ActualParam, 0);
    
    /* implicit binding of lightness linear with generic on off set.
       generic on off set -> actual lightness -> linear lightness set.
    */   
    Light_Linear_ActualImplicitBinding(BINDING_LIGHT_LIGHTNESS_ACTUAL_SET);
}


/*
* @Brief  GenericLevel_LightActualBinding: Data binding b/w Generic Level and 
*         light lightness Actual. this function will set the actual light lightness
*         value at the time of  generic Level set. 
* @param gLevel_param Pointer to the data which needs to be checked.
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


/*
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
      
      productValue = (bLevelParam->TargetLevel + 32768) * (0x4E20 - 0x0320) ;
      bCtlTempstatus.PresentCtlTemperature16 = 0x0320 + ((productValue) /65535);
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


/*
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

/* Weak function are defined to support the original function if they are not
   included in firmware.
   There is no use of this function for application development purpose.
*/

WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_OnOff_Set(Generic_OnOffStatus_t* pGeneric_OnOffParam, 
                                     MOBLEUINT8 OptionalValid))
{
               
  return MOBLE_RESULT_SUCCESS;
}


WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_Level_Set(Generic_LevelStatus_t* plevelParam, 
                                     MOBLEUINT8 OptionalValid))
{
               
  return MOBLE_RESULT_SUCCESS;
}


WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_LevelDelta_Set(Generic_LevelStatus_t* pdeltalevelParam, 
                                     MOBLEUINT8 OptionalValid))
{
               
  return MOBLE_RESULT_SUCCESS;
}


WEAK_FUNCTION (MOBLE_RESULT Appli_Generic_LevelMove_Set(Generic_LevelStatus_t* pdeltaMoveParam, 
                                               MOBLEUINT8 OptionalValid))
{
               
  return MOBLE_RESULT_SUCCESS;
}
/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

