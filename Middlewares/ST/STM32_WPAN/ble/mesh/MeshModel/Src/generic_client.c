/**
******************************************************************************
* @file    generic_client.c
* @author  BLE Mesh Team
* @brief   Generic model client middleware file
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
#include "mesh_cfg.h"
#include "generic.h"
#include "generic_client.h"
#include "common.h"
#include "models_if.h"
#include <string.h>
#include "compiler.h"


/** @addtogroup MODEL_CLIENT_GENERIC
*  @{
*/

/** @addtogroup Generic_Model_Client_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern Model_Tid_t Model_Tid;;
const MODEL_OpcodeTableParam_t Generic_Client_Opcodes_Table[] = {
  /* Generic OnOff Client */
/* model_id                                         opcode,                                    reliable,    min_payload_size, max_payload_size, response_opcode,                        min_response_size, max_response_size
    Here in this array, Handler is not defined; */
#ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF  
  {GENERIC_ONOFF_CLIENT_MODEL_ID,                   GENERIC_ON_OFF_GET,                        MOBLE_TRUE,  0,                0,                GENERIC_ON_OFF_STATUS,                  1,                 3},
  {GENERIC_ONOFF_CLIENT_MODEL_ID,                   GENERIC_ON_OFF_SET_ACK,                    MOBLE_TRUE,  2,                4,                GENERIC_ON_OFF_STATUS,                  1,                 3},  
  {GENERIC_ONOFF_CLIENT_MODEL_ID,                   GENERIC_ON_OFF_SET_UNACK,                  MOBLE_FALSE, 2,                4,                GENERIC_ON_OFF_STATUS,                  1,                 3}, 
  {GENERIC_ONOFF_CLIENT_MODEL_ID,                   GENERIC_ON_OFF_STATUS,                     MOBLE_FALSE, 1,                3,                0,                                      1,                 3},
#endif                                                                                                                                                                                                           
#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL                                                                                                                                                                         
  /* Generic Level Server */                                                                                                                                                                                     
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_LEVEL_GET,                         MOBLE_TRUE,   0,                0,               GENERIC_LEVEL_STATUS,                   2,                 5}, 
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_LEVEL_SET_ACK,                     MOBLE_TRUE,   3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_LEVEL_SET_UNACK,                   MOBLE_FALSE,  3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5}, 
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_DELTA_SET,                         MOBLE_TRUE,   5,                7,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_DELTA_SET_UNACK,                   MOBLE_FALSE,  5,                7,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_MOVE_SET,                          MOBLE_TRUE,   3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_MOVE_SET_UNACK,                    MOBLE_FALSE,  3,                5,               GENERIC_LEVEL_STATUS,                   2,                 5},
  {GENERIC_LEVEL_CLIENT_MODEL_ID,                   GENERIC_LEVEL_STATUS,                      MOBLE_FALSE,  2,                5,               0,                                      2,                 5},
#endif                                                                                                                                                                                                           
#ifdef ENABLE_GENERIC_MODEL_CLIENT_POWER_ONOFF                                                                                                                                                                   
  {GENERIC_POWER_ONOFF_CLIENT_MODEL_ID,             GENERIC_POWER_ON_OFF_GET ,                 MOBLE_TRUE,   0,                0,               GENERIC_POWER_ON_OFF_STATUS,            1,                 1},
  {GENERIC_POWER_ONOFF_CLIENT_MODEL_ID,             GENERIC_POWER_ON_OFF_SET,                  MOBLE_TRUE,   1,                1,               GENERIC_POWER_ON_OFF_STATUS,            1,                 1},
  {GENERIC_POWER_ONOFF_CLIENT_MODEL_ID,             GENERIC_POWER_ON_OFF_SET_UNACK,            MOBLE_FALSE,  1,                1,               0,                                      1,                 1},
  {GENERIC_POWER_ONOFF_CLIENT_MODEL_ID,             GENERIC_POWER_ON_OFF_STATUS ,              MOBLE_FALSE,  1,                1,               0,                                      1,                 1},
#endif                                                                                                                                                                                                           
#ifdef ENABLE_GENERIC_MODEL_CLIENT_DEFAULT_TRANSITION_TIME                                                                                                                                                       
  /* Generic Default Transition Time Server Model  */                                                                                                                                                            
  {GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID, GENERIC_DEFAULT_TRANSITION_TIME_GET,       MOBLE_TRUE,  0,                0,                GENERIC_DEFAULT_TRANSITION_TIME_STATUS, 1,                 1}, 
  {GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID, GENERIC_DEFAULT_TRANSITION_TIME_SET,       MOBLE_TRUE,  1,                1,                GENERIC_DEFAULT_TRANSITION_TIME_STATUS, 1,                 1},
  {GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID, GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK, MOBLE_FALSE, 1,                1,                GENERIC_DEFAULT_TRANSITION_TIME_STATUS, 1 ,                1}, 
  {GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID, GENERIC_DEFAULT_TRANSITION_TIME_STATUS,    MOBLE_FALSE, 1,                1,                0,                                      1,                 1},
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

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF  
/******************************************************************************/
/**
* @brief  GenericClient_OnOff_Get: This function is called to send Generic OnOff Get message
* @param  elementIndex : Index of the element
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_OnOff_Get(MOBLEUINT8 elementIndex) 

{
  /* 3.2.1.1 Generic OnOff Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_OnOff_Get Client Message \r\n");  
  
  msg_opcode = GENERIC_ON_OFF_GET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_ONOFF_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, 0,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  GenericClient_OnOff_Set_Ack: This function is called to send Generic OnOff Set message Acknowledged
* @param  elementIndex : Index of the element
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_OnOff_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pOnOff_param, 
                                     MOBLEUINT32 length) 

{
  
  /* 3.2.1.2 Generic OnOff Set 
  OnOff: 1B The target value of the Generic OnOff state 
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 millisecond steps (C.1)
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_OnOff_Set Client Message \r\n");  
  pOnOff_param[1] = Model_Tid.TidSend;
  msg_buff = pOnOff_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_ON_OFF_SET_ACK;
  
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_ONOFF_CLIENT_MODEL_ID, 
                                           elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  /* 
  Manage the TID 
  */
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  GenericClient_OnOff_Set_Unack: This function is called to send Generic OnOff Set message UnAcknowledged
* @param  elementIndex : Index of the element
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_OnOff_Set_Unack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pOnOff_param, 
                                     MOBLEUINT32 length) 

{
  
  /* 3.2.1.3 Generic OnOff Set unack 
  OnOff: 1B The target value of the Generic OnOff state 
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 millisecond steps (C.1)
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "Generic_OnOff_Set Client Message \r\n");  
  pOnOff_param[1] = Model_Tid.TidSend;
  msg_buff = pOnOff_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = GENERIC_ON_OFF_SET_UNACK;
  
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_ONOFF_CLIENT_MODEL_ID, 
                                           elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  /* 
  Manage the TID
  */
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}
/******************************************************************************/
#endif /// ENABLE_GENERIC_MODEL_CLIENT_ONOFF  
/******************************************************************************/


/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL  
/******************************************************************************/
/**
* @brief  GenericClient_Level_Get: This function is called to send Generic Level Get
* @param  elementIndex : Index of the element
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Level_Get(MOBLEUINT8 elementIndex) 

{
  /* 
  3.2.2.1 Generic Level Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_Level_Get Client Message \r\n");  
  
  msg_opcode = GENERIC_LEVEL_GET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_LEVEL_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, 0,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  

  return result;
}

/**
* @brief  GenericClient_Level_Set_Ack: This function is called to send Generic Level Set message Acknowledged
* @param  elementIndex : Index of the element
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Level_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *plevel_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.2.2 Generic Level Set
  Level: 2B The target value of the Generic Level state
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_Level_Set Client Message \r\n");  
  plevel_param[2] = Model_Tid.TidSend;
  msg_buff = plevel_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_LEVEL_SET_ACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_LEVEL_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  /* 
  Manage the TID
  */  
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_Level_Set_Unack: This function is called to send Generic Level Set message UnAcknowledged
* @param  elementIndex:  Index of the element
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Level_Set_Unack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8* plevel_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.2.3 Generic Level Set unack
  Level: 2B The target value of the Generic Level state
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_Level_Set Client Message \r\n");  
  plevel_param[2] = Model_Tid.TidSend;
  msg_buff = plevel_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = GENERIC_LEVEL_SET_UNACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_LEVEL_CLIENT_MODEL_ID, 
                                           elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  /* 
  Manage the TID
  */
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  GenericClient_Delta_Set_Ack: This function is called to send Generic Delta Level Set message UnAcknowledged
* @param  elementIndex : Index of the element
* @param  pdeltalevel_param: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Delta_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pdeltalevel_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.2.4 Generic Delta Level Set
  Delta Level: 4B The Delta change of the Generic Level state
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_Delta_Set Client Message \r\n");  
  pdeltalevel_param[4] = Model_Tid.TidSend;
  msg_buff = pdeltalevel_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_DELTA_SET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_LEVEL_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  /* 
  Manage the TID
  */
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_Delta_Set_Unack: This function is called to send Generic Delta Level Set message UnAcknowledged
* @param  elementIndex : Index of the element
* @param  pdeltalevel_param: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Delta_Set_Unack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pdeltalevel_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.2.5 Generic Delta Level Set UnAcnowedged
  Delta Level: 4B The Delta change of the Generic Level state
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_Delta_Set Client Message \r\n");  
  pdeltalevel_param[4] = Model_Tid.TidSend;
  msg_buff = pdeltalevel_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = GENERIC_DELTA_SET_UNACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_LEVEL_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  /* 
  Manage the TID
  */
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_Move_Set_Ack: This function is called to send Generic Delta Level Set message UnAcknowledged
* @param  elementIndex : Index of the element
* @param  plevelmove_param: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Move_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *plevelmove_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.2.6 Generic Move Set
  Delta Level: 2B The Delta Level step to calculate Move speed for the Generic Level state.
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_Move_Set Client Message \r\n");  
  plevelmove_param[2] = Model_Tid.TidSend;
  msg_buff = plevelmove_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_MOVE_SET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_LEVEL_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  /* 
  Manage the TID
  */
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_Move_Set_Unack: This function is called to send Generic Delta Level Set message UnAcknowledged
* @param  elementIndex : Index of the element
* @param  plevelmove_param: Pointer to the parameters message
* @param  OptionalValid: Flag to inform about the validity of optional parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Move_Set_Unack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *plevelmove_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.2.7 Generic Move Set UnAcnowedged
  Delta Level: 2B The Delta Level step to calculate Move speed for the Generic Level state.
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_Move_Set Client Message \r\n");  
  plevelmove_param[2] = Model_Tid.TidSend;
  msg_buff = plevelmove_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = GENERIC_MOVE_SET_UNACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_LEVEL_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  /* 
  Manage the TID
  */
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/******************************************************************************/
#endif /// ENABLE_GENERIC_MODEL_CLIENT_LEVEL  
/******************************************************************************/


/******************************************************************************/
#ifdef ENABLE_GENERIC_MODEL_CLIENT_POWER_ONOFF 
/******************************************************************************/
/**
* @brief  GenericClient_PowerOnOff_Get: This function is called to send Generic Power On Off Get
* @param  elementIndex : Index of the element
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_PowerOnOff_Get(MOBLEUINT8 elementIndex) 

{
  /* 
3.2.4.1 Generic On Power Up Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_PowerOnOff_Get Client Message \r\n");  
  
  msg_opcode = GENERIC_POWER_ON_OFF_GET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_POWER_ONOFF_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, 0,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_PowerOnOff_Set_Ack: This function is called to send Generic Level Set message Acknowledged
* @param  elementIndex : Index of the element
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_PowerOnOff_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *ppoweronoff_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.4.2 Generic On Power Up Set
  OnPowerUp: 1B The value of the Generic OnPowerUp state.
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "Generic_PowerOnOff_Set Client Message \r\n");  
  msg_buff = ppoweronoff_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_POWER_ON_OFF_SET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_POWER_ONOFF_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);

  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_PowerOnOff_Set_Unack: This function is called to send Generic Level Set message UnAcknowledged
* @param  elementIndex : Index of the element
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_PowerOnOff_Set_Unack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8* ppoweronoff_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.4.3 Generic On Power Up Set Unack
  OnPowerUp: 1B The value of the Generic OnPowerUp state.
*/    
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "Generic_PowerOnOff_Set Client Message \r\n");  
  msg_buff = ppoweronoff_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = GENERIC_POWER_ON_OFF_SET_UNACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_POWER_ONOFF_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
 
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/******************************************************************************/
#endif /// ENABLE_GENERIC_MODEL_CLIENT_POWER_ONOFF 
/******************************************************************************/


/******************************************************************************/    
#ifdef ENABLE_GENERIC_MODEL_CLIENT_DEFAULT_TRANSITION_TIME 
/******************************************************************************/
/**
* @brief  GenericClient_DefaultTransitionTime_Get: This function is called to send Generic Transition Time Get
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_DefaultTransitionTime_Get(MOBLEUINT8 elementIndex) 

{
  /* 
3.2.3.1 Generic Default Transition Time Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_DefaultTransitionTime Client Message \r\n");  
  
  msg_opcode = GENERIC_DEFAULT_TRANSITION_TIME_GET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, 0,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_DefaultTransitionTime_Set_Ack: This function is called to send Generic Level Set message Acknowledged
* @param  elementIndex : Index of the element
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_DefaultTransitionTime_Set_Ack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8 *pdefaulttransitiontime_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.3.2 Generic Default Transition Time Set
  Transition Time: 1B The value of the Generic Default Transition Time state.
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_DefaultTransitionTime_Set_Ack Client Message \r\n");  
  msg_buff = pdefaulttransitiontime_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_DEFAULT_TRANSITION_TIME_SET;
  
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID, 
                                            elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);

  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}

/**
* @brief  GenericClient_DefaultTransitionTime_Set_Unack: This function is called to send Generic Level Set message UnAcknowledged
* @param  elementIndex : Index of the element
* @param  plevel_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_DefaultTransitionTime_Set_Unack(MOBLEUINT8 elementIndex, 
                                     MOBLEUINT8* pdefaulttransitiontime_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.3.3 Generic Default Transition Time Set Unack
  Transition Time: 1B The value of the Generic Default Transition Time state.
*/  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT_M, "GenericClient_DefaultTransitionTime_Set_Unack Client Message \r\n");  
  msg_buff = pdefaulttransitiontime_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = GENERIC_DEFAULT_TRANSITION_TIME_SET_UNACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID, 
                                           elementIndex,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
 
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}
/******************************************************************************/    
#endif /// ENABLE_GENERIC_MODEL_CLIENT_DEFAULT_TRANSITION_TIME 
/******************************************************************************/

  /**
* @brief Generic_Client_OnOff_Status: Function called when status of the model 
*        received on the client.
* @param pOnOff_status: Pointer to the parameters received for message
* @param plength: Length of the parameters received for message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_OnOff_Status(MOBLEUINT8 const *pOnOff_status,\
                                         MOBLEUINT32 plength, MOBLEUINT16 dstPeer,\
                                              MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_GENERIC_CLIENT_M, ">>>\r\n");
  GenericAppli_cb.OnOff_Status_cb(pOnOff_status , plength, dstPeer, elementIndex);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  GenericClient_Level_Set_unack: This function is called for Acknowledged 
* @param  elementIndex:  Index of the element
* @param  plevel_paramr: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 

/**
* @brief Generic_Client_Level_Status: Function called when status of the model 
*        received on the client.
* @param plevel_status: ointer to the parameters received for message
* @param plength: Length of the parameters received for message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_Level_Status(MOBLEUINT8 const *plevel_status,\
                                         MOBLEUINT32 plength, MOBLEUINT16 dstPeer,\
                                              MOBLEUINT8 elementIndex)
{
  
  TRACE_M(TF_GENERIC_CLIENT_M,"Generic_Level_Status received \r\n");
  GenericAppli_cb.Level_Status_cb(plevel_status , plength, dstPeer, elementIndex);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief Generic_Client_DefaultTransitionTime_Status: Function called when status of the model 
*        received on the client.
* @param pTransition_status: ointer to the parameters received for message
* @param plength: Length of the parameters received for message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status,\
                                                MOBLEUINT32 plength, MOBLEUINT16 dstPeer,\
                                                MOBLEUINT8 elementIndex) 
{  
  
  TRACE_M(TF_GENERIC_CLIENT_M,"Generic_DefaultTransitionTime_Status received \r\n");
  GenericAppli_cb.GenericDefaultTransition_Status_cb(pTransition_status, plength, dstPeer,\
                                                                    elementIndex); 
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief Generic_Client_PowerOnOff_Status: Function called when status of the model 
*        received on the client.
* @param powerOnOff_status: ointer to the parameters received for message
* @param plength: Length of the parameters received for message
* @param  dstPeer: destination send by peer for this node. It can be a
*                     unicast or group address 
* @param  elementIndex: index of the element received from peer for this node which
*                     is elementNumber-1
* return MOBLE_RESULT_SUCCESS.
*/
MOBLE_RESULT Generic_Client_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status, \
                                              MOBLEUINT32 plength, MOBLEUINT16 dstPeer,\
                                              MOBLEUINT8 elementIndex) 
{  

  TRACE_M(TF_GENERIC_CLIENT_M,"Generic_PowerOnOff_Status received \r\n"); 
  GenericAppli_cb.GenericPowerOnOff_Status_cb(powerOnOff_status, plength, dstPeer,\
                                                                    elementIndex); 
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief   GenericModelServer_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{
  *data = Generic_Client_Opcodes_Table;
  *length = sizeof(Generic_Client_Opcodes_Table)/sizeof(Generic_Client_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  GenericModelClient_GetStatusRequestCb : This function is call-back 
*         from the library to send response to the message from peer
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelClient_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam,
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response)

{
  TRACE_M(TF_GENERIC_CLIENT_M, "response status enable \n\r");
  
  return MOBLE_RESULT_SUCCESS;    
}

/**
* @brief  GenericModelClient_ProcessMessageCb: This is a callback function from
*         the library whenever a Generic Model message is received
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericModelClient_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam,
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response)
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  TRACE_M(TF_GENERIC_CLIENT_M, 
          "elementIndex = %.2x, dst_peer = %.2X, peer_add = %.2X,opcode= %.2X ,response= %.2X\r\n", 
          pmsgParam->elementIndex, 
          pmsgParam->dst_peer,
          pmsgParam->peer_addr, 
          opcode, 
          response);
  
  switch(opcode)
  { 
  case GENERIC_ON_OFF_STATUS:
    {     
      Generic_Client_OnOff_Status(pRxData, dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);          
      break;
    } 
  case GENERIC_LEVEL_STATUS:
    {     
      Generic_Client_Level_Status(pRxData, dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);          
      break;
    } 
  case GENERIC_DEFAULT_TRANSITION_TIME_STATUS:
    {     
      Generic_Client_DefaultTransitionTime_Status(pRxData, dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);         
      break;
    } 
  case GENERIC_POWER_ON_OFF_STATUS:
    {     
      Generic_Client_PowerOnOff_Status(pRxData, dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);         
      break;
    } 
  default:
    {
      break;
    }
  }
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @}
*/

/**
* @}
*/



