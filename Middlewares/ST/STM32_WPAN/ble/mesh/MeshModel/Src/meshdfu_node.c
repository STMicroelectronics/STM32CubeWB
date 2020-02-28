/**
******************************************************************************
* @file    meshdfu_node.c
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   BLE-Mesh Device Firmware Upgrade / FOTA over the mesh implementation 
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
* Initial BlueNRG-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "common.h"
#include <string.h>
#include "compiler.h"
#include "Math.h"
#include "meshdfu_node.h"


/** @addtogroup Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const MODEL_OpcodeTableParam_t MeshDfuNode_Opcodes_Table[] = {
  /*MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
#ifdef ENABLE_MESHNODEUPDATE_MODEL_SERVER     
  {FIRMWARE_INFORMATION_GET,        MOBLE_TRUE,  0,   0,   FIRMWARE_INFORMATION_STATUS ,      1, 255},
  {FIRMWARE_UPDATE_GET,             MOBLE_TRUE,  0,   0,   FIRMWARE_UPDATE_STATUS ,           3, 255},
  {FIRMWARE_VALIDATION_DATA_CHECK,  MOBLE_TRUE,  1, 255,   FIRMWARE_VALIDATION_DATA_STATUS ,  1, 255},
  {FIRMWARE_UPDATE_CHECK,           MOBLE_TRUE,  0,   0,   FIRMWARE_UPDATE_STATUS ,           3, 255},
  {FIRMWARE_UPDATE_START,           MOBLE_TRUE, 10, 255,   FIRMWARE_UPDATE_STATUS ,           3, 255},
  {FIRMWARE_UPDATE_CANCEL,          MOBLE_TRUE,  0,   0,   FIRMWARE_UPDATE_STATUS ,           3, 255},
  {FIRMWARE_UPDATE_APPLY,           MOBLE_TRUE,  0,   0,   FIRMWARE_UPDATE_STATUS ,           3, 255},
  {FIRMWARE_INFORMATION_STATUS,     MOBLE_FALSE, 0,   0,     0 ,                                1, 255},
  {FIRMWARE_VALIDATION_DATA_STATUS, MOBLE_FALSE, 0,   0,     0 ,                                1, 255},
  {FIRMWARE_UPDATE_STATUS,          MOBLE_FALSE, 0,   0,     0 ,                                3, 255},
#endif
  {0}
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  MeshDfuNode_ModelServer_GetOpcodeTableCb: This function is call-back 
          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Light Model opcode array 
* @param  length: Pointer to the Length of Light Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT MeshDfuNode_ModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                               MOBLEUINT16 *length)
{
  *data = MeshDfuNode_Opcodes_Table;
  *length = sizeof(MeshDfuNode_Opcodes_Table)/sizeof(MeshDfuNode_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  MeshDfuNode_ModelServer_GetStatusRequestCb : This function is call-back 
          from the library to send response to the message from peer
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
MOBLE_RESULT MeshDfuNode_ModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                  MOBLE_ADDRESS dst_peer, 
                                                  MOBLEUINT16 opcode, 
                                                  MOBLEUINT8 *pResponsedata, 
                                                  MOBLEUINT32 *plength, 
                                                  MOBLEUINT8 const *pRxData,
                                                  MOBLEUINT32 dataLength,
                                                  MOBLEBOOL response)
{
  switch(opcode)
  {
  case FIRMWARE_INFORMATION_STATUS: 
    {
      break;
    }
  case FIRMWARE_VALIDATION_DATA_STATUS: 
    {
      break;
    }
  case FIRMWARE_UPDATE_STATUS: 
    {
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
* @brief  MeshDfuNode_ModelServer_ProcessMessageCb: This is a callback function from
          the library whenever a Light Model message is received
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
MOBLE_RESULT MeshDfuNode_ModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                               MOBLE_ADDRESS dst_peer, 
                                               MOBLEUINT16 opcode, 
                                               MOBLEUINT8 const *pRxData, 
                                               MOBLEUINT32 dataLength, 
                                               MOBLEBOOL response
                                                 )
{  
  switch(opcode)
  {
  case FIRMWARE_INFORMATION_GET: 
    {
      break;
    }
  case FIRMWARE_UPDATE_GET: 
    {
      break;
    }
  case FIRMWARE_VALIDATION_DATA_CHECK: 
    {
      break;
    }
  case FIRMWARE_UPDATE_CHECK: 
    {
      break;
    }
  case FIRMWARE_UPDATE_START:
    {
      break;
    }
  case FIRMWARE_UPDATE_CANCEL: 
    {
      break;
    }
    
  case FIRMWARE_UPDATE_APPLY: 
    {
      break;
    }
    
  default:
    {
      break;
    }    
  } /* Switch ends */
  return MOBLE_RESULT_SUCCESS;
}


/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

