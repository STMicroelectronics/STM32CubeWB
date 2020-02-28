/**
******************************************************************************
* @file    light_client.c
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
* Initial BLE-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "light.h"
#include "light_client.h"
#include "common.h"
#include "models_if.h"
#include <string.h>
#include "compiler.h"

/** @addtogroup MODEL_CLIENT_LIGHT
*  @{
*/

/** @addtogroup Light_Model_Client_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern MOBLEUINT8 TidSend;
const MODEL_OpcodeTableParam_t Light_Client_Opcodes_Table[] = {
  /* Light Lightness Client */
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS  
  {LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID      ,LIGHT_LIGHTNESS_STATUS,              MOBLE_FALSE, 2, 5, 0,        2, 5},
#endif
  {0}
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  LightClient_Lightness_Set_Unack: This function is called for Acknowledged message
* @param  element_number: number for the element present on the node
* @param  pLightness_param: pointer ot the function
* @param  length:length of the data received
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT LightClient_Lightness_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Light_LightnessParam *pLightness_param, 
                                     MOBLEUINT32 length) 

{

  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;
  
  TRACE_M(TF_LIGHT_CLIENT,"Light_Lightness_Set_Unack Client Message \r\n");  
  pLightness_param->a_Lightness_param[2] = TidSend;
  msg_buff = pLightness_param->a_Lightness_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = LIGHT_LIGHTNESS_SET_UNACK;
  
  /* 
  Manage the TID Here....
  */
  result = MeshClient_SetRemotePublication((MOBLEUINT32) LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID, 
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
    TRACE_M(TF_LIGHT_CLIENT,"Publication Error \r\n");
  }  
  
  return result;
}
 

/**
* @brief   LightModelClient_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Generic Model opcode array 
* @param  length: Pointer to the Length of Generic Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT LightModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{
  *data = Light_Client_Opcodes_Table;
  *length = sizeof(Light_Client_Opcodes_Table)/sizeof(Light_Client_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  LightModelClient_GetStatusRequestCb : This function is call-back 
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
MOBLE_RESULT LightModelClient_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                   MOBLE_ADDRESS dst_peer, 
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response)

{
  TRACE_M(TF_LIGHT_CLIENT,"response status enable \n\r");
  
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief LightModelClient_ProcessMessageCb: This is a callback function from
*        the library whenever a Generic Model message is received
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
MOBLE_RESULT LightModelClient_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   )
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  //tClockTime delay_t = Clock_Time();
  
  TRACE_M(TF_LIGHT_CLIENT,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X ,response= %.2X \r\n  ",
          dst_peer, peer_addr, opcode , response);
  
  switch(opcode)
  { 
  case LIGHT_LIGHTNESS_STATUS:
    {     
      Light_Client_Lightness_Status(pRxData, dataLength);         
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

