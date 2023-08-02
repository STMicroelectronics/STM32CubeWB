/**
******************************************************************************
* @file    time_scene.c
* @author  BLE Mesh Team
* @brief   Time and Scene model middleware file
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
#include "common.h"
#include <string.h>
#include "compiler.h"
//#include "math.h"
#include "time_scene.h"

/** @addtogroup MODEL_TIME_SCENE
*  @{
*/

/** @addtogroup Time_Scene_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

const MODEL_OpcodeTableParam_t Time_Scene_Opcodes_Table[] = {
  /*MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
#ifdef ENABLE_TIME_MODEL_SERVER     
  {TIME_MODEL_SERVER_MODEL_ID   ,TIME_SET,                    MOBLE_TRUE,  10, 10,             TIME_STATUS ,             10, 10},
  {TIME_MODEL_SERVER_MODEL_ID   ,TIME_ZONE_SET,               MOBLE_TRUE,  6, 6,               TIME_ZONE_STATUS ,        7, 7}, 
  {TIME_MODEL_SERVER_MODEL_ID   ,TAI_UTC_DELTA_SET,           MOBLE_TRUE,  7, 7,               TAI_UTC_DELTA_STATUS ,    9, 9},
  {TIME_MODEL_SERVER_MODEL_ID   ,TIME_ROLE_GET,               MOBLE_TRUE,  0, 0,               TIME_ROLL_STATUS ,        1, 1},    
  {TIME_MODEL_SERVER_MODEL_ID   ,TIME_ROLL_SET,               MOBLE_TRUE,  1, 1,               TIME_ROLL_STATUS ,        1, 1},
  {TIME_MODEL_SERVER_MODEL_ID   ,TIME_ROLL_STATUS,            MOBLE_FALSE,  1, 1,              NULL ,                   1, 1},
#endif  
#ifdef ENABLE_TIME_MODEL_SERVER_SETUP 
  {TIME_MODEL_SERVER_SETUP_MODEL_ID  ,TIME_GET,                    MOBLE_TRUE,   0, 0,               TIME_STATUS ,             10, 10},
  {TIME_MODEL_SERVER_SETUP_MODEL_ID  ,TIME_STATUS,                 MOBLE_FALSE,  10, 10,             NULL ,                    10, 10},
  {TIME_MODEL_SERVER_SETUP_MODEL_ID  ,TIME_ZONE_GET,               MOBLE_TRUE,   0, 0,               TIME_ZONE_STATUS ,        7, 7},
  {TIME_MODEL_SERVER_SETUP_MODEL_ID  ,TIME_ZONE_STATUS,            MOBLE_FALSE,  7, 7,               NULL ,        7, 7},
  {TIME_MODEL_SERVER_SETUP_MODEL_ID  ,TAI_UTC_DELTA_GET,           MOBLE_TRUE,   0, 0,               TAI_UTC_DELTA_STATUS ,    9, 9},  
  {TIME_MODEL_SERVER_SETUP_MODEL_ID  ,TAI_UTC_DELTA_STATUS,        MOBLE_FALSE,  9, 9,               NULL ,    9, 9},  
  
#endif
#ifdef ENABLE_SCENE_MODEL_SERVER
  {SCENE_MODEL_SERVER_MODEL_ID   ,SCENE_GET,                   MOBLE_TRUE,     0, 0,               SCENE_STATUS ,            3, 6},
  {SCENE_MODEL_SERVER_MODEL_ID   ,SCENE_STATUS,                MOBLE_FALSE,    3, 6,               NULL ,            3, 6},
  {SCENE_MODEL_SERVER_MODEL_ID   ,SCENE_REGISTER_GET,          MOBLE_TRUE,     0, 0,               SCENE_REGISTER_STATUS ,   5, 8},
  {SCENE_MODEL_SERVER_MODEL_ID   ,SCENE_REGISTER_STATUS,       MOBLE_FALSE,     5, 8,              NULL ,   5, 8},
  {SCENE_MODEL_SERVER_MODEL_ID   ,SCENE_RECALL,                MOBLE_TRUE,     3, 5,               SCENE_STATUS ,            3, 6},   
  {SCENE_MODEL_SERVER_MODEL_ID   ,SCENE_RECALL_UNACK,          MOBLE_FALSE,    3, 5,               SCENE_STATUS ,            3, 6},     
#endif
#ifdef ENABLE_SCENE_MODEL_SERVER_SETUP  
  {SCENE_MODEL_SERVER_SETUP_MODEL_ID  ,SCENE_STORE,                 MOBLE_TRUE,     2, 2,               SCENE_REGISTER_STATUS ,   5, 8},                  
  {SCENE_MODEL_SERVER_SETUP_MODEL_ID  ,SCENE_STORE_UNACK,           MOBLE_FALSE,    2, 2,               SCENE_REGISTER_STATUS ,   5, 8},
  {SCENE_MODEL_SERVER_SETUP_MODEL_ID  ,SCENE_DELETE,                MOBLE_TRUE,     2, 2,               SCENE_REGISTER_STATUS ,   5, 8},
  {SCENE_MODEL_SERVER_SETUP_MODEL_ID  ,SCENE_DELETE_UNACK,          MOBLE_FALSE,    2, 2,               SCENE_REGISTER_STATUS ,   5, 8},
#endif
  {0}
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Time_SceneModelServer_GetOpcodeTableCb: This function is call-back 
          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Light Model opcode array 
* @param  length: Pointer to the Length of Light Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Time_SceneModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                               MOBLEUINT16 *length)
{
  *data = Time_Scene_Opcodes_Table;
  *length = sizeof(Time_Scene_Opcodes_Table)/sizeof(Time_Scene_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Time_SceneModelServer_GetStatusRequestCb : This function is call-back 
          from the library to send response to the message from peer
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Time_SceneModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
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
  case TIME_STATUS:
    {
      break;
    }
  case TIME_ROLL_STATUS:
    {
      break;
    }
  case TIME_ZONE_STATUS:
    {
      break;
    }
  case TAI_UTC_DELTA_STATUS:
    {
      break;
    }
  case SCENE_STATUS:
    {
      break;
    }
  case SCENE_REGISTER_STATUS:
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
* @brief  Time_SceneModelServer_ProcessMessageCb: This is a callback function from
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
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Time_SceneModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                               MOBLE_ADDRESS dst_peer, 
                                               MOBLEUINT16 opcode, 
                                               MOBLEUINT8 const *pRxData, 
                                               MOBLEUINT32 dataLength, 
                                               MOBLEBOOL response
                                                 )
{  
  switch(opcode)
  {
  case TIME_SET:
    {
      break;
    }
  case TIME_ROLL_SET:
    {
      break;
    }
  case TIME_ZONE_SET:
    {
      break;
    }
  case TAI_UTC_DELTA_SET:
    {
      break;
    }     
  case SCENE_RECALL:
  case SCENE_RECALL_UNACK:
    {
      break;
    }
  case SCENE_STORE:
  case SCENE_STORE_UNACK:
    {
      break;
    }
  case SCENE_DELETE:
  case SCENE_DELETE_UNACK: 
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




