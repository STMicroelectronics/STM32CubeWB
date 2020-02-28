/**
******************************************************************************
* @file    meshdfu_node.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   BLE-Mesh Device Firmware Upgrade/ FOTA over the mesh header file  
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
#ifndef __MESHDFU_NODE_H
#define __MESHDFU_NODE_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "bluenrg_mesh.h"

/* Exported macro ------------------------------------------------------------*/

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/
#define FIRMWARE_INFORMATION_GET         0XB601
#define FIRMWARE_INFORMATION_STATUS      0XB602
#define FIRMWARE_UPDATE_GET              0XB603         
#define FIRMWARE_VALIDATION_DATA_CHECK   0XB604
#define FIRMWARE_UPDATE_CHECK            0XB60A  /* Error here in Doc */
#define FIRMWARE_VALIDATION_DATA_STATUS  0XB605
#define FIRMWARE_UPDATE_START            0XB606
#define FIRMWARE_UPDATE_CANCEL           0XB607
#define FIRMWARE_UPDATE_APPLY            0XB608
#define FIRMWARE_UPDATE_STATUS           0XB609


#define FIRMWARE_DISTRIBUTION_GET	 0XB60A
#define FIRMWARE_DISTRIBUTION_START	 0XB60B
#define FIRMWARE_DISTRIBUTION_CANCEL	 0XB60C
#define FIRMWARE_DISTRIBUTION_STATUS	 0XB60D
#define FIRMWARE_DISTRIBUTION_RESULTS_NODES_GET	          0XB60E
#define FIRMWARE_DISTRIBUTION_RESULTS_STATUSNODES_LIST	  0XB60F
#define FIRMWARE_DISTRIBUTION_NODE_LISTS_ADD	          0XB610
#define FIRMWARE_DISTRIBUTION_NODE_LISTS_CLEARDELETE_ALL  0XB611
#define FIRMWARE_DISTRIBUTION_NODE_LISTS_STATUS	          0XB612
#define FIRMWARE_DISTRIBUTION_CAPABILITIES_GET	          0XB613
#define FIRMWARE_DISTRIBUTION_CAPABILITIES_STATUS	  0XB614    
#define FIRMWARE_DISTRIBUTION_UPLOADFIRMWARE_GET	  0XB615
#define FIRMWARE_DISTRIBUTION_FIRMWARE_ADDUPLOAD_START	  0XB616
#define FIRMWARE_DISTRIBUTION_FIRMWARE_ADD_OOBUPLOAD_OOB_START	0XB617
#define FIRMWARE_DISTRIBUTION_FIRMWARE_DELETEUPLOAD_CANCEL	0XB618
#define FIRMWARE_DISTRIBUTION_UPLOAD_FIRMWARE_STATUS	  0XB619    
#define FIRMWARE_DISTRIBUTION_FIRMWARE_LIST_GET	          0XB61A
#define FIRMWARE_DISTRIBUTION_FIRMWARE_LISTSTATUS_STATUS  0XB61B    
#define FIRMWARE_DISTRIBUTION_FIRMWARE_GET_BY_INDEX	  0XB61C
#define FIRMWARE_DISTRIBUTION_FIRMWARE_DELETE	          0XB61D
#define FIRMWARE_DISTRIBUTION_FIRMWARE_DELETE_ALL	  0XB61E




/******************************************************************************/
/********** Following Section defines the SIG MODEL IDs            ************/
/******************************************************************************/
#define FIRMWARE_UPDATE_SERVER             0xFE00
#define FIRMWARE_UPDATE_CLIENT             0xFE01
#define FIRMWARE_DISTRIBUTION_SERVER       0xFE02
#define FIRMWARE_DISTRIBUTION_CLIENT       0xFE03


/******************************************************************************/
/** Following Section defines the Firmware Update Phases : in a 3-bit value  **/
/******************************************************************************/
#define MESHDFU_NODE_IDLE_STATE                  0x00
#define MESHDFU_NODE_TRANSFER_ERROR_STATE        0x01
#define MESHDFU_NODE_INPROGRESS_STATE            0x02
#define MESHDFU_NODE_DFU_READY_STATE             0x03
#define MESHDFU_NODE_VERIFICATION_FAILED_STATE   0x04
#define MESHDFU_NODE_RFU_MIN_STATE               0x05
#define MESHDFU_NODE_RFU_MAX_STATE               0x07

/******************************************************************************/
/** Firmware Update Additional Information : state is 5-bit wide bitfield *****/
/******************************************************************************/

#define MESHDFU_NODE_PROVISIONING_NEEDED_INFO          0x01
#define MESHDFU_NODE_COMPOSITION_CHANGE_EXPECTED_INFO  0x02  /* 1 << 2 */

/******************************************************************************/
/** 10.1.5.2	Out-of-Band Retrieval Supported                           *****/
/******************************************************************************/

#define OOB_RETRIEVAL_NOT_SUPPORTED  0x00
#define OOB_RETRIEVAL_OOB_SUPPORTED  0x01
#define OOB_RETRIEVAL_PROHIBITED_MIN 0x02
#define OOB_RETRIEVAL_PROHIBITED_MAX 0xFF

/******************************************************************************/
/** 10.1.6.1	Distribution Phase                                        *****/
/******************************************************************************/

#define DISTRIBUTION_PHASE_NOT_ACTIVE        0x00 
#define DISTRIBUTION_PHASE_IS_ACTIVE         0x01
#define DISTRIBUTION_PHASE_IS_COMPLETE       0x02
#define DISTRIBUTION_PHASE_IS_PROHIBITED_MIN 0x03
#define DISTRIBUTION_PHASE_IS_PROHIBITED_MAX 0xFF

/******************************************************************************/
/** 10.1.6.2	Update Policy                                             *****/
/******************************************************************************/

#define UPDATE_POLICY_VERIFY_ONLY        0x00
#define UPDATE_POLICY_VERIFY_AND_APPLY   0x01
#define UPDATE_POLICY_PROHIBITED_MIN     0x02
#define UPDATE_POLICY_PROHIBITED_MAX     0xFF

/******************************************************************************/
/** 10.1.7.2	Upload Phase   
     The Upload Phase state indicates the phase of a firmware image delivery to 
     a Firmware Distribution Server. The firmware binary can be delivered 
     via OOB mechanism or BLOB Transfer Server model                      *****/
/******************************************************************************/

#define UPLOAD_PHASE_IDLE 0x00
#define UPLOAD_PHASE_IN_PROGRESS 0x01
#define UPLOAD_TRANSFER_ERROR    0x02
#define UPLOAD_PHASE_PROHIBITED_MIN 0x03
#define UPLOAD_PHASE_PROHIBITED_MAX 0xFF


/******************************************************************************/
/**           10.2.4 Distributor:      Firmware Update Status Code  *****/
/******************************************************************************/
#define MESHDFU_SUCCESS_STATUS            0x00
#define MESHDFU_VALIDATION_FAILED_STATUS  0x01
#define MESHDFU_INVALID_ID_STATUS         0x02
#define MESHDFU_DISTRIBUTOR_BUSY_STATUS   0x03
#define MESHDFU_OUT_OF_RESOURCES_STATUS   0x04
#define MESHDFU_BLOB_TRANSFER_BUSY_STATUS 0x05
                /* 0x06 is missing */ 
#define MESHDFU_FIRMWARE_ID_STATUS        0x07
#define MESHDFU_TEMPORARILY_UNABLE_STATUS 0x08
#define MESHDFU_INVALID_COMMAND_STATUS    0x09
#define MESHDFU_PROHIBITED_MIN_STATUS     0x0A
#define MESHDFU_PROHIBITED_MAX_STATUS     0xFF


/******************************************************************************/
/********** SIG MODEL IDs ends                                     ************/
/******************************************************************************/ 
MOBLE_RESULT MeshDfuNode_ModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                        MOBLEUINT16 *length);

MOBLE_RESULT MeshDfuNode_ModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                      MOBLE_ADDRESS dst_peer, 
                                                      MOBLEUINT16 opcode, 
                                                      MOBLEUINT8 *pResponsedata, 
                                                      MOBLEUINT32 *plength, 
                                                      MOBLEUINT8 const *pRxData,
                                                      MOBLEUINT32 dataLength,
                                                      MOBLEBOOL response);

MOBLE_RESULT MeshDfuNode_ModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                    MOBLE_ADDRESS dst_peer, 
                                                    MOBLEUINT16 opcode, 
                                                    MOBLEUINT8 const *pRxData, 
                                                    MOBLEUINT32 dataLength, 
                                                    MOBLEBOOL response);
                                                 

   
#endif /* __MESHDFU_NODE_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

