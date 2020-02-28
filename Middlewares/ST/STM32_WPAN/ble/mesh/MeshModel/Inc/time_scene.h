/**
******************************************************************************
* @file   time_scene.h
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
#ifndef __TIME_SCENE_H
#define __TIME_SCENE_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/
#define TIME_GET                0X8237
#define TIME_SET                0X5C
#define TIME_STATUS             0X5D
#define TIME_ROLE_GET           0X8238
#define TIME_ROLL_SET           0X8239
#define TIME_ROLL_STATUS        0X823A
#define TIME_ZONE_GET           0X823B
#define TIME_ZONE_SET           0X823C
#define TIME_ZONE_STATUS        0X823D
#define TAI_UTC_DELTA_GET       0X823E
#define TAI_UTC_DELTA_SET       0X823F
#define TAI_UTC_DELTA_STATUS    0X8240
#define SCENE_GET               0X8241 
#define SCENE_RECALL            0X8242
#define SCENE_RECALL_UNACK      0X8243
#define SCENE_STATUS            0X5E
#define SCENE_REGISTER_GET      0X8244
#define SCENE_REGISTER_STATUS   0X8245
#define SCENE_STORE             0X8246
#define SCENE_STORE_UNACK       0X8247
#define SCENE_DELETE            0X829E
#define SCENE_DELETE_UNACK      0X829F
/******************************************************************************/
/********** Following Section defines the SIG MODEL IDs            ************/
/******************************************************************************/
#define TIME_MODEL_SERVER_MODEL_ID             0X1200
#define TIME_MODEL_SERVER_SETUP_MODEL_ID       0X1201
#define SCENE_MODEL_SERVER_MODEL_ID            0X1203
#define SCENE_MODEL_SERVER_SETUP_MODEL_ID      0X1204

/******************************************************************************/
/********** SIG MODEL IDs ends                                     ************/
/******************************************************************************/ 
MOBLE_RESULT Time_SceneModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                        MOBLEUINT16 *length);

MOBLE_RESULT Time_SceneModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                      MOBLE_ADDRESS dst_peer, 
                                                      MOBLEUINT16 opcode, 
                                                      MOBLEUINT8 *pResponsedata, 
                                                      MOBLEUINT32 *plength, 
                                                      MOBLEUINT8 const *pRxData,
                                                      MOBLEUINT32 dataLength,
                                                      MOBLEBOOL response);

MOBLE_RESULT Time_SceneModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                    MOBLE_ADDRESS dst_peer, 
                                                    MOBLEUINT16 opcode, 
                                                    MOBLEUINT8 const *pRxData, 
                                                    MOBLEUINT32 dataLength, 
                                                    MOBLEBOOL response);
                                                 

   
#endif /* __TIME_SCENE_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

