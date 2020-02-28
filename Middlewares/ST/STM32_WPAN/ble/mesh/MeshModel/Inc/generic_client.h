/**
******************************************************************************
* @file    generic_client.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Header file for the user application file 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GENERIC_CLIENT_H
#define __GENERIC_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/
/* Variable-------------------------------------------------------------------*/
#pragma pack(1)
typedef union  
{
  Generic_OnOffParam_t sGeneric_OnOffParam;
  MOBLEUINT8 a_OnOff_param[sizeof(Generic_OnOffParam_t)]; 
} _Generic_OnOffParam;

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/


/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT GenericModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT GenericModelClient_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                   MOBLE_ADDRESS dst_peer, 
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response);

MOBLE_RESULT GenericModelClient_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                 MOBLE_ADDRESS dst_peer, 
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response
                                                   );

MOBLE_RESULT GenericClient_OnOff_Set(MOBLE_ADDRESS element_number, 
                                     _Generic_OnOffParam *pOnOff_param, 
                                     MOBLEUINT32 length); 

MOBLE_RESULT GenericClient_OnOff_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Generic_OnOffParam *pOnOff_param, 
                                     MOBLEUINT32 length); 
MOBLE_RESULT GenericClient_Level_Set_Unack(MOBLE_ADDRESS element_number, 
                                     _Generic_LevelParam *plevel_param, 
                                     MOBLEUINT32 length);


MOBLE_RESULT Generic_Client_OnOff_Status(MOBLEUINT8 const *pOnOff_status, MOBLEUINT32 plength);
MOBLE_RESULT Generic_Client_Level_Status(MOBLEUINT8 const *plevel_status, MOBLEUINT32 plength);
MOBLE_RESULT Generic_Client_PowerOnOff_Status(MOBLEUINT8 const *powerOnOff_status , MOBLEUINT32 plength);
MOBLE_RESULT Generic_Client_DefaultTransitionTime_Status(MOBLEUINT8 const *pTransition_status , MOBLEUINT32 plength);

#endif /* __GENERIC_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

