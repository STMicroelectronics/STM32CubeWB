/**
******************************************************************************
* @file   time_scene.h
* @author  BLE Mesh Team
* @brief   Header file for the user application file 
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



