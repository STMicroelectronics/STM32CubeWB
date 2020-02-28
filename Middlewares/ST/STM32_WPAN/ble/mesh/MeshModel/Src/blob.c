/**
******************************************************************************
* @file    blob.c
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   BLE-Mesh Block transfer Server implementation 
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
#include "blob.h"


/** @addtogroup Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#pragma pack(1)
Blob_Transfer_param_t Blob_Transfer_param;
Blob_Block_Param_t Blob_Block_Param;
uint8_t Blob_Blocks_Not_Received[BLOB_MAX_BLOCK_BITMAP_BYTE_SIZE]; 
uint8_t Missing_Chunks_Array[BLOB_MAX_CHUNK_BITMAP_BYTE_SIZE]; 

const MODEL_OpcodeTableParam_t Mbt_Opcodes_Table[] = {
  /*MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
#ifdef ENABLE_BLOB_MODEL_SERVER     
  {BLOB_TRANSFER_GET,           MOBLE_TRUE,   0,  0,    BLOB_TRANSFER_STATUS,     2, 23},
  {BLOB_TRANSFER_START,         MOBLE_TRUE,  17, 17,    BLOB_TRANSFER_STATUS,     2, 23},
  {BLOB_TRANSFER_CANCEL,        MOBLE_TRUE,   8,  8,    BLOB_TRANSFER_STATUS,     2, 23},
  {BLOB_BLOCK_GET,              MOBLE_TRUE,   0,  0,    BLOB_BLOCK_STATUS,        5,  7},  
  {BLOB_BLOCK_START,            MOBLE_TRUE,   4,  4,    BLOB_BLOCK_STATUS,        5,  7},  
  {BLOB_CHUNK_TRANSFER,         MOBLE_FALSE,  2,  258,  0,                        0,  0},
  {BLOB_INFORMATION_GET,        MOBLE_TRUE,   0,  0,    BLOB_INFORMATION_STATUS, 13, 13},

  /* Following status messages may need commenting */
  {BLOB_TRANSFER_STATUS,        MOBLE_FALSE, 17, 17,    0,                        0,  0}, 
  {BLOB_BLOCK_STATUS,           MOBLE_FALSE,  5,  7,    0,                        0,  0}, 
  {BLOB_INFORMATION_STATUS,     MOBLE_FALSE,  13,13,    0,                        0,  0},
#endif
  {0}
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Mbt_ModelServer_GetOpcodeTableCb: This function is call-back 
          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Light Model opcode array 
* @param  length: Pointer to the Length of Light Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Mbt_ModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                               MOBLEUINT16 *length)
{
  *data = Mbt_Opcodes_Table;
  *length = sizeof(Mbt_Opcodes_Table)/sizeof(Mbt_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Mbt_ModelServer_GetStatusRequestCb : This function is call-back 
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
MOBLE_RESULT Mbt_ModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
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
  case BLOB_TRANSFER_STATUS: 
    {
      BLOB_Transfer_Status(pResponsedata, plength);
      break;
    }
  case BLOB_BLOCK_STATUS: 
    {
      BLOB_Block_Status(pResponsedata, plength);
      break;
    }
  case BLOB_INFORMATION_STATUS: 
    {
      BLOB_Information_Status(pResponsedata, plength);
      break;
    }    
    
  default:
    {
      break;
    }    
  } /* Switch ends */
 return MOBLE_RESULT_SUCCESS;    
}

/**
* @brief  Mbt_ModelServer_ProcessMessageCb: This is a callback function from
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
MOBLE_RESULT Mbt_ModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                               MOBLE_ADDRESS dst_peer, 
                                               MOBLEUINT16 opcode, 
                                               MOBLEUINT8 const *pMsgData, 
                                               MOBLEUINT32 dataLength, 
                                               MOBLEBOOL response
                                                 )
{  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  
  switch(opcode)
  {
  case BLOB_TRANSFER_GET:
    {
      result = BLOB_Transfer_Get(pMsgData, dataLength);
               /* The Status shall be send from Model_SendResponse via 
                  Mbt_ModelServer_GetStatusRequestCb*/
      break;
    }
  case BLOB_TRANSFER_START:
    {
      result = BLOB_Transfer_Start(pMsgData, dataLength);
      break;
    }
  case BLOB_TRANSFER_CANCEL:
    {
      result = BLOB_Transfer_Cancel(pMsgData, dataLength);
      break;
    }
  case BLOB_BLOCK_GET:
    {
      result = BLOB_Block_Get(pMsgData, dataLength);
      break;
    }
  case BLOB_BLOCK_START:
    {
      result = BLOB_Transfer_Start(pMsgData, dataLength);
      break;
    }
  case BLOB_CHUNK_TRANSFER:
    {
      result = BLOB_Chunk_Transfer(pMsgData, dataLength);
      break;
    }
  case BLOB_INFORMATION_GET:
    {
      result = BLOB_Information_Get(pMsgData, dataLength);
      break;
    }

  default:
    {
      break;
    }    
  } /* Switch ends */
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    Model_SendResponse(peer_addr,dst_peer,opcode,pMsgData,dataLength);
  }
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  BLOB_Transfer_Get: This is a function which is called when the 
          client calls for BLOB Transfer Get
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Transfer_Get(MOBLEUINT8 const *pMsgData, MOBLEUINT32 length)
{
  /* BLOB Transfer Get is an acknowledged message used to get the current value 
  of a BLOB Transfer Server state.
  The response to the BLOB Transfer Get message is a 
  BLOB Transfer Status message.
*/
  
  if (length != 0 )
  {
    /* There are no parameters for this message */
    return MOBLE_RESULT_INVALIDARG;
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  BLOB_Transfer_Start: This is a function which is called when the 
          client calls for BLOB Transfer Start function
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Transfer_Start(MOBLEUINT8 const *pMsgData, MOBLEUINT32 length)
{
  /* BLOB Transfer Start is an acknowledged message used to start a 
      new BLOB transfer.
     The response to the BLOB Transfer Start message is a 
        BLOB Transfer Status message.
  */
  
  MOBLE_RESULT result;
 
  
  if (length != sizeof(Blob_Transfer_param_t) )
  {
    /* The message has no optional parameters, hence the lenght must match 
    with the expected size */
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    /* Copy all the data in the parameters */
    memcpy(Blob_Transfer_param.pBlob_Transfer_Param, pMsgData, length);
    result = MOBLE_RESULT_SUCCESS;
   }
     
  return result;
}

/**
* @brief  BLOB_Transfer_Cancel: This is a function which is called when the 
          client calls for BLOB Transfer CANCEL function
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Transfer_Cancel(MOBLEUINT8 const *pMsgData, MOBLEUINT32 length)
{
  /* BLOB Transfer Cancel is an acknowledged message used to cancel the ongoing 
      Transfer BLOB Procedure.
     The response to the BLOB Transfer Cancel message is a 
      BLOB Transfer Status message.
     The BLOB ID field shall indicate the ID of the BLOB that’s transfer is 
      supposed to be canceled.
  */
  
  MOBLE_RESULT result;
 
  
  if (length != BLOB_ID_SIZE )
  {
    /* The message shall receive the Blob_id to Cancel
       if the length is not matching, this is an error */
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if ( memcmp(Blob_Transfer_param.uBlob_Transfer_param.blob_id, pMsgData, length) != 0 )    
  { 
    /* If Blob id received for cancelling the message is not matching with 
       the Blob under progress, this is still an error */
     result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    /* Cancel the ongoing transfer, reset the state machine  */

    /* DO SOMETHING HERE  !!!! */
    result = MOBLE_RESULT_SUCCESS;
   }
     
  return result;
}

/**
* @brief  BLOB_Block_Get: This is a function which is called when the 
          client calls for BLOB Block Get
* @param  node: There are no parameters to this function
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Block_Get(MOBLEUINT8 const *pMsgData, MOBLEUINT32 length)
{
  /* The BLOB Block Get is an acknowledged message used to get the 
      status of the current block transfer.
     The response to the BLOB Block Get message is a BLOB Block Status message.
     There are no parameters for this message.

*/
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  BLOB_Block_Start: This is a function which is called when the 
          client calls for BLOB Block Start function
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Block_Start(MOBLEUINT8 const *pMsgData, MOBLEUINT32 length)
{
  /* 
    BLOB Block Start is an acknowledged message used to 
        start block transfer to the server.
    The response to the BLOB Block Start message is a BLOB Block Status message

  The Block Number field shall indicate the new value of the Block Number state (see Section 7.1.3.2.1).
  The Chunk Size field shall indicate the new value of the Chunk Size state (see Section 7.1.3.2.2).
  */
  
  MOBLE_RESULT result;
 
  
  if (length != sizeof(Blob_Block_Param_t) )
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    /* Copy all the data in the parameters */
    // memcpy(Blob_Block_Param , pMsgData, length);
    //Blob_Block_Param.Block_Number;
    Blob_Block_Param.Block_Number = *((MOBLEUINT16*)pMsgData);
    Blob_Block_Param.Chunk_Size = *((MOBLEUINT16*)pMsgData);
    result = MOBLE_RESULT_SUCCESS;
   }
     
  return result;
}



/**
* @brief  BLOB_Chunk_Transfer: This is a function which is called when the 
          client calls for BLOB CHUNK Transfer function
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Chunk_Transfer(MOBLEUINT8 const *pMsgData, MOBLEUINT32 length)
{
  /* 
    The BLOB Chunk Transfer is an unacknowledged message used to deliver a chunk 
    of the current block to a server.
  
    The Chunk Number field shall indicate the chunk’s number in a sequence of 
     chunks in a block.
    The Chunk Data field shall contain between 1 and Chunk Size state bytes of 
      the BLOB data. It shall be Chunk Size state bytes in size, except for the 
      last chunk in the block, which may be shorter if Chunk Size state is not a 
      divisor of Block Size.
  */
  
  //MOBLE_RESULT result;
//  MOBLEUINT8 *pChunkData;
//  MOBLEUINT16 chunk_index;
  
  /* Copy the Chunk data to the Application */     
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  BLOB_Information_Get: This is a function which is called when the 
          client calls for BLOB Information Get function
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Information_Get(MOBLEUINT8 const *pMsgData, MOBLEUINT32 length)
{
  /* The BLOB Information Get is an acknowledged message used to get the 
      BLOB transfer capabilities of the server.
     The response to the BLOB Information Get message is a 
      BLOB Information Status message.
*/

  if (length != 0 )
  {
    /* There are no parameters for this message */
    return MOBLE_RESULT_INVALIDARG;
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  BLOB_Transfer_Status: This is a function which is called when the 
          Server needs to send the Transfer Status message to Client
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Transfer_Status(MOBLEUINT8 const *pResponsedata, MOBLEUINT32 *plength)
{
  Transfer_Status_data_t* pTransfer_Status;
  
  pTransfer_Status = (Transfer_Status_data_t*)(pResponsedata);
  pTransfer_Status->Status = (uint8_t)(BLOB_SUCCESS_STATUS); //modify
  pTransfer_Status->Padding = 0;
  pTransfer_Status->Phase = (uint8_t)BLOB_IDLE_STATE; //modify
  
  memcpy( pTransfer_Status->blob_id, Blob_Transfer_param.pBlob_Transfer_Param, sizeof(_Blob_Transfer_Param_t) );
//  pTransfer_Status->BLOB_Size;
//  BLOB_Block_Size_Log;
//  Client_MTU_Size;
//  Timeout;
  memcpy (pTransfer_Status->Blocks_Not_Received_Variable, Blob_Blocks_Not_Received, BLOB_MAX_BLOCK_BITMAP_BYTE_SIZE);
  *plength = sizeof (Transfer_Status_data_t);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  BLOB_Block_Status: This is a function which is called when the 
          Server needs to send the BLOB Block Status message to Client
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Block_Status(MOBLEUINT8 const *pResponsedata, MOBLEUINT32* plength)
{
  BLOB_Block_Status_t* pblock_status;
  
  pblock_status = (BLOB_Block_Status_t*)(pResponsedata);
  pblock_status->Status = (uint8_t)(BLOB_SUCCESS_STATUS); //modify
  pblock_status->Format = 0; //modify
  pblock_status->Block_Number = 0; //modify
  pblock_status->Chunk_Size = 0; //modify  
  memcpy (pblock_status->Missing_Chunks_Variable, Missing_Chunks_Array, BLOB_MAX_CHUNK_BITMAP_BYTE_SIZE);
  *plength = sizeof (BLOB_Block_Status_t);
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  BLOB_Information_Status: This is a function which is called when the 
          Server needs to send the BLOB Information Status message to Client
* @param  pMsgData: Message pointer containing the received parameters
* @param  length: length of the message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLOB_Information_Status(MOBLEUINT8 const *pResponsedata, MOBLEUINT32* plength)
{
  BLOB_Information_Status_t* pblobInfoStatus;
  
  pblobInfoStatus = (BLOB_Information_Status_t*) pResponsedata;
  pblobInfoStatus->Min_Block_Size_Log = (uint8_t)BLOB_MIN_BLOCK_SIZE_LOG;
  pblobInfoStatus->Min_Block_Size_Log = (uint8_t)BLOB_MAX_BLOCK_SIZE_LOG;
  pblobInfoStatus->Max_Chunks_Number = (uint16_t)BLOB_MAX_CHUNK_NUMBER;
  pblobInfoStatus->Max_BLOB_Size = (uint32_t) BLOB_MAX_FILE_SIZE;
  pblobInfoStatus->MTU_size= (uint16_t)BLOB_MAX_CHUNK_SIZE;
  pblobInfoStatus->Functionalities= (uint8_t)BLOB_NON_SEQUENTIAL_BLOCK_RECEPTION;
  *plength = sizeof (BLOB_Information_Status_t);
  return MOBLE_RESULT_SUCCESS;
}


/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/



