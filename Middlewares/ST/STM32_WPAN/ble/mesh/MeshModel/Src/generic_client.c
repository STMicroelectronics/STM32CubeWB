/**
******************************************************************************
* @file    generic_client.c
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Generic model client middleware file
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
* Initial BLE-Mesh is built over Motorola�s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
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
extern MOBLEUINT8 TidSend;
const MODEL_OpcodeTableParam_t Generic_Client_Opcodes_Table[] = {
  /* Generic OnOff Client */
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
#ifdef ENABLE_GENERIC_MODEL_CLIENT_ONOFF  
  {GENERIC_MODEL_SERVER_ONOFF_MODEL_ID     ,GENERIC_ON_OFF_STATUS,                                 MOBLE_FALSE, 1, 3,              0 , 1, 3},
#endif
#ifdef ENABLE_GENERIC_MODEL_CLIENT_LEVEL  
  {GENERIC_MODEL_SERVER_LEVEL_MODEL_ID     ,GENERIC_LEVEL_STATUS,                                  MOBLE_FALSE,  2, 5,             0 , 2 , 5},
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

/**
* @brief  Generic_OnOff_Set: This function is called for Acknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_OnOff_Set(MOBLE_ADDRESS element_number, 
                                     _Generic_OnOffParam *pOnOff_param, 
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

  TRACE_M(TF_GENERIC_CLIENT,"Generic_OnOff_Set Client Message \r\n");  
  
  msg_buff = pOnOff_param->a_OnOff_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_ON_OFF_SET_ACK;
  
  /* 
     Manage the TID Here....
  */
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_MODEL_SERVER_ONOFF_MODEL_ID, 
                                            element_number ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT,"Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  GenericClient_OnOff_Set_Unack: This function is called for unacknowledged message
* @param  pOnOff_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_OnOff_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Generic_OnOffParam *pOnOff_param, 
                                     MOBLEUINT32 length) 

{
  
  /* 3.2.1.2 Generic OnOff Set unack 
  OnOff: 1B The target value of the Generic OnOff state 
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay: 1B Message execution delay in 5 millisecond steps (C.1)
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT,"Generic_OnOff_Set Client Message \r\n");  
  pOnOff_param->a_OnOff_param[1] = TidSend;
  msg_buff = pOnOff_param->a_OnOff_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = GENERIC_ON_OFF_SET_UNACK;
  
  /* 
     Manage the TID Here....
  */
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_MODEL_CLIENT_ONOFF_MODEL_ID, 
                                            element_number ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  TidSend++;
  if(TidSend >= MAX_TID_VALUE)
  {
    TidSend = 0;
  }  
  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT,"Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  GenericClient_Level_Set_unack: This function is called for Acknowledged 
* @param  plevel_paramr: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT GenericClient_Level_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Generic_LevelParam *plevel_param, 
                                     MOBLEUINT32 length) 

{
/*  
  3.2.2.2 Generic Level Set unack
  Level: 2B The target value of the Generic Level state
  TID :  1B Transaction Identifier
  Transition Time: 1B Format as defined in Section 3.1.3. (Optional)
  Delay:1B Message execution delay in 5 milliseconds steps (C.1)
*/   
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_GENERIC_CLIENT,"Generic_Level_Set Client Message \r\n");  
  plevel_param->a_Level_param[2] = TidSend;
  msg_buff = plevel_param->a_Level_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = GENERIC_LEVEL_SET_UNACK;
  
  /* 
     Manage the TID Here....
  */
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) GENERIC_MODEL_SERVER_LEVEL_MODEL_ID, 
                                            element_number ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  TidSend++;
  if(TidSend >= MAX_TID_VALUE)
  {
    TidSend = 0;
  }  
  if(result)
  {
    TRACE_M(TF_GENERIC_CLIENT,"Publication Error \r\n");
  }  
  
  return result;
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
MOBLE_RESULT GenericModelClient_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                   MOBLE_ADDRESS dst_peer, 
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response)

{
  TRACE_M(TF_GENERIC_CLIENT,"response status enable \n\r");
  
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief  GenericModelClient_ProcessMessageCb: This is a callback function from
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
MOBLE_RESULT GenericModelClient_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   )
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  //tClockTime delay_t = Clock_Time();
  
  TRACE_M(TF_GENERIC_CLIENT,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X ,response= %.2X \r\n  ",
          dst_peer, peer_addr, opcode , response);
  
  switch(opcode)
  { 
  case GENERIC_ON_OFF_STATUS:
    {     
      Generic_Client_OnOff_Status(pRxData, dataLength);         
      break;
    } 
  case GENERIC_LEVEL_STATUS:
    {     
      Generic_Client_Level_Status(pRxData, dataLength);         
      break;
    } 
  default:
    {
      break;
    }
  }
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    Model_SendResponse(peer_addr,dst_peer,opcode,pRxData,dataLength);
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

