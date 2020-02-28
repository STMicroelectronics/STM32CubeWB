/**
******************************************************************************
* @file    blob.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   BLE-Mesh Block transfer Server implementation header file  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLOB_H
#define __BLOB_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "bluenrg_mesh.h"

/* Exported macro ------------------------------------------------------------*/

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/* 7.1.1	Numerical summary of opcodes                                 **/
/******************************************************************************/

#define BLOB_TRANSFER_GET	0XB701
#define BLOB_TRANSFER_START	0XB702
#define BLOB_TRANSFER_CANCEL	0XB703
#define BLOB_TRANSFER_STATUS	0XB704
#define BLOB_BLOCK_GET	        0XB707
#define BLOB_BLOCK_START	0XB705
#define BLOB_BLOCK_STATUS	  0X7E
#define BLOB_CHUNK_TRANSFER	  0X7D
#define BLOB_INFORMATION_GET	0XB70A
#define BLOB_INFORMATION_STATUS	0XB70B

/******************************************************************************/
/* 7.1.2	Summary of status codes                                      **/
/******************************************************************************/

typedef enum {
  BLOB_SUCCESS_STATUS,
  BLOB_OUT_OF_SEQUENCE_BLOCK_NUMBER_STATUS,
  BLOB_INVALID_BLOCK_NUMBER_STATUS,
  BLOB_WRONG_BLOCK_SIZE_STATUS,
  BLOB_WRONG_CHUNK_SIZE_STATUS,
  BLOB_INVALID_STATE_STATUS, 
  BLOB_INVALID_PARAMETER_STATUS, 
  BLOB_WRONG_BLOB_ID_STATUS,
  BLOB_STORAGE_LIMIT_STATUS,
  BLOB_PROHIBITED_MIN_STATUS,
  BLOB_PROHIBITED_MAX_STATUS = 0x3F,
} BLOB_Status_types_t;

//#define BLOB_SUCCESS_STATUS                      0x00
//#define BLOB_OUT_OF_SEQUENCE_BLOCK_NUMBER_STATUS 0x01
//#define BLOB_INVALID_BLOCK_NUMBER_STATUS         0x02
//#define BLOB_WRONG_BLOCK_SIZE_STATUS             0x03
//#define BLOB_WRONG_CHUNK_SIZE_STATUS             0x04
//#define BLOB_INVALID_STATE_STATUS                0x05
//#define BLOB_INVALID_PARAMETER_STATUS            0x06
//#define BLOB_WRONG_BLOB_ID_STATUS                0x07
//#define BLOB_STORAGE_LIMIT_STATUS                0x08
//#define BLOB_PROHIBITED_MIN_STATUS               0x09
//#define BLOB_PROHIBITED_MAX_STATUS               0x3F

/******************************************************************************/
/* 7.1.4.1	BLOB Transfer Phase                                         ***/     
/******************************************************************************/

//#define BLOB_IDLE_STATE                          0x00
//#define BLOB_WAITING_FOR_NEXT_BLOCK_STATE        0x01   
//#define BLOB_WAITING_FOR_NEXT_CHUNK_STATE        0x02
//#define BLOB_COMPLETE_STATE                      0x03
//#define BLOB_SUSPENDED_STATE                     0x04
//#define BLOB_INACTIVE_STATE                      0x05
//#define BLOB_PROHIBITED_MIN_STATE                0x06
//#define BLOB_PROHIBITED_MAX_STATE                0xFF

typedef enum {
 BLOB_IDLE_STATE,                          //0x00
 BLOB_WAITING_FOR_NEXT_BLOCK_STATE,        //0x01   
 BLOB_WAITING_FOR_NEXT_CHUNK_STATE,        //0x02
 BLOB_COMPLETE_STATE,                      //0x03
 BLOB_SUSPENDED_STATE,                     //0x04
 BLOB_INACTIVE_STATE,                      //0x05
 BLOB_PROHIBITED_MIN_STATE,                //0x06
 BLOB_PROHIBITED_MAX_STATE = 0xFF,         //0xFF
} BLOB_Transfer_Phase_types_t;

/******************************************************************************/
/********** Following Section defines the SIG MODEL IDs            ************/
/******************************************************************************/
#define BLOB_TRANSFER_MODEL_SERVER             0XFF00
#define BLOB_TRANSFER_MODEL_CLIENT             0XFF01

/******************************************************************************/
/********** SIG MODEL IDs ends                                     ************/
/******************************************************************************/ 

#define BLOB_ID_SIZE 8
#define BLOB_MAX_FILE_SIZE (256*1024)  /* This is for 256K Bytes */
#define BLOB_MIN_BLOCK_SIZE (1*1024)   /* 1024 means = 1K block size for simplicity */
#define BLOB_MAX_BLOCK_SIZE (1*1024)   /* Min and Max kept same for simplicity */
#define BLOB_MAX_BLOCK_NUMBER (BLOB_MAX_FILE_SIZE/BLOB_MIN_BLOCK_SIZE)
#define BLOB_MAX_BLOCK_BITMAP_BYTE_SIZE ((uint16_t) (BLOB_MAX_BLOCK_NUMBER+7)/ 8 ) /* Number of bits required to represent maximum block number */

#define BLOB_MAX_CHUNK_SIZE (128)
#define BLOB_MAX_CHUNK_NUMBER ( BLOB_MAX_BLOCK_SIZE/BLOB_MAX_CHUNK_SIZE )
#define BLOB_MAX_CHUNK_BITMAP_BYTE_SIZE ( (uint16_t) (BLOB_MAX_CHUNK_NUMBER+7)/8 )  /* Make it round number of bytes */

#define BLOB_MIN_BLOCK_SIZE_LOG 10  /* 2^10 = 1024 */
#define BLOB_MAX_BLOCK_SIZE_LOG 10  /* 2^10 = 1024 */

#define BLOB_NON_SEQUENTIAL_BLOCK_RECEPTION 0x00 /* If a bit is set, the 
         functionality is supported, for simplicity, we expect linear blocks */



typedef enum {
	ALL_CHUNKS_MISSING =0x00,
	NO_MISSING_CHUNKS = 0x01,
	SOME_CHUNKS_MISSING = 0x02,
	FORMAT_PROHIBITED =0x03,
} format_types_t;



#pragma pack(1)
typedef struct
{
  MOBLEUINT8  blob_id[BLOB_ID_SIZE];
  MOBLEUINT32 blob_size;
  MOBLEUINT8  blob_block_size_log;
  MOBLEINT16  mtu_size;
  MOBLEINT16  Timeout;
} _Blob_Transfer_Param_t;


#pragma pack(1)
typedef struct
{
  MOBLEINT16  Block_Number;
  MOBLEINT16  Chunk_Size;
} Blob_Block_Param_t;


typedef union  {
   _Blob_Transfer_Param_t uBlob_Transfer_param;
   MOBLEUINT8 pBlob_Transfer_Param[sizeof(_Blob_Transfer_Param_t)];
}Blob_Transfer_param_t;


typedef struct
{
   uint8_t Status  : 6;
   uint8_t Padding : 2; 
   uint8_t Phase; 
   uint8_t blob_id[BLOB_ID_SIZE];  /* Optional: C.1: If the BLOB ID field is 
                                      present, then the BLOB Size field may be 
                                      present; otherwise this field shall not be present. */
   uint32_t BLOB_Size;             /* C.2: If the BLOB Size field is present, 
                                      then the BLOB Block Size Log, Client MTU Size, 
                                      Timeout, and Blocks Not Received fields shall be present; 
                                      otherwise these fields shall not be present. */
   uint8_t BLOB_Block_Size_Log;
   uint16_t Client_MTU_Size;
   uint16_t Timeout; 
   uint8_t Blocks_Not_Received_Variable[BLOB_MAX_BLOCK_BITMAP_BYTE_SIZE]; 
} Transfer_Status_data_t;

typedef struct
{
  uint8_t Status	:6; //	Status code of the block transfer
  uint8_t Format	:2; //	Indicates the format used to report missing chunks
  uint16_t Block_Number;    //	16 Block’s number in a set of blocks
  uint16_t Chunk_Size;      //	16 Chunk Size in bytes for this block
  uint8_t  Missing_Chunks_Variable[BLOB_MAX_CHUNK_BITMAP_BYTE_SIZE]; //	Bit field of missing chunks for this block (C.1)
} BLOB_Block_Status_t; 


typedef struct
{	
  uint8_t Min_Block_Size_Log;  //1	Minimum block size: 2(Min Block Size Log)
  uint8_t Max_Block_Size_Log;  //1	Maximum block size: 2(Max Block Size Log)
  uint16_t Max_Chunks_Number;  //2	Maximum number of chunks in block 
  uint16_t Max_Chunk_Size;     //2	Maximum size of chunk supported by the server
  uint32_t Max_BLOB_Size;      //4	Maximum BLOB size supported by the server
  uint16_t MTU_size;           //2	Maximum payload size supported by the server
  uint8_t Functionalities;     //1	Bitmask of functionalities supported
} BLOB_Information_Status_t;

MOBLE_RESULT Mbt_ModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                        MOBLEUINT16 *length);

MOBLE_RESULT Mbt_ModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                      MOBLE_ADDRESS dst_peer, 
                                                      MOBLEUINT16 opcode, 
                                                      MOBLEUINT8 *pResponsedata, 
                                                      MOBLEUINT32 *plength, 
                                                      MOBLEUINT8 const *pRxData,
                                                      MOBLEUINT32 dataLength,
                                                      MOBLEBOOL response);

MOBLE_RESULT Mbt_ModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                    MOBLE_ADDRESS dst_peer, 
                                                    MOBLEUINT16 opcode, 
                                                    MOBLEUINT8 const *pRxData, 
                                                    MOBLEUINT32 dataLength, 
                                                    MOBLEBOOL response);
                                                 

MOBLE_RESULT BLOB_Transfer_Get(MOBLEUINT8 const*, MOBLEUINT32);  
MOBLE_RESULT BLOB_Transfer_Start(MOBLEUINT8 const*, MOBLEUINT32 );
MOBLE_RESULT BLOB_Transfer_Cancel(MOBLEUINT8 const*, MOBLEUINT32 );
MOBLE_RESULT BLOB_Block_Get(MOBLEUINT8 const*, MOBLEUINT32);  
MOBLE_RESULT BLOB_Block_Start(MOBLEUINT8 const*, MOBLEUINT32 );
MOBLE_RESULT BLOB_Chunk_Transfer(MOBLEUINT8 const* , MOBLEUINT32 );
MOBLE_RESULT BLOB_Information_Get(MOBLEUINT8 const*, MOBLEUINT32);      
MOBLE_RESULT BLOB_Transfer_Status(MOBLEUINT8 const *pMsgData, MOBLEUINT32* plength);
MOBLE_RESULT BLOB_Block_Status(MOBLEUINT8 const *pMsgData, MOBLEUINT32* plength);
MOBLE_RESULT BLOB_Information_Status(MOBLEUINT8 const *pMsgData, MOBLEUINT32* plength);

#endif /* __BLOB_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

