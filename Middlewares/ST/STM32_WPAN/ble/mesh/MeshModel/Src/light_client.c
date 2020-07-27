/**
******************************************************************************
* @file    light_client.c
* @author  BLE Mesh Team
* @brief   Generic model client middleware file
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under Ultimate Liberty license
* SLA0044, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
*                             www.st.com/SLA0044
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
const MODEL_OpcodeTableParam_t Light_Client_Opcodes_Table[] = 
{
  /* Light Lightness Client */
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
/* model_id                                     opcode                                 reliable     min_payload_size max_payload_size response_opcode                      min_response_size max_response_size */
                                                                                                                                                                           
#ifdef ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS                                                                                                                                 
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_GET,                   MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_STATUS,             2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_SET,                   MOBLE_TRUE,  3,               5,                LIGHT_LIGHTNESS_STATUS,             2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_SET_UNACK,             MOBLE_FALSE, 3,               5,                LIGHT_LIGHTNESS_STATUS,             2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_STATUS,                MOBLE_FALSE, 2,               5,                0,                                  2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_LINEAR_GET,            MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_LINEAR_STATUS,      2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_LINEAR_SET,            MOBLE_TRUE,  3,               5,                LIGHT_LIGHTNESS_LINEAR_STATUS,      2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_LINEAR_SET_UNACK,      MOBLE_FALSE, 3,               5,                LIGHT_LIGHTNESS_LINEAR_STATUS,      2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_LINEAR_STATUS,         MOBLE_FALSE, 2,               5,                0,                                  2,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_LAST_GET,              MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_LAST_STATUS,        2,                2},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_LAST_STATUS,           MOBLE_FALSE, 2,               2,                0,                                  2,                2},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_DEFAULT_GET,           MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_DEFAULT_STATUS,     2,                2},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_DEFAULT_STATUS,        MOBLE_FALSE, 2,               2,                0,                                  2,                2},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_RANGE_GET,             MOBLE_TRUE,  0,               0,                LIGHT_LIGHTNESS_RANGE_STATUS,       5,                5},
  {LIGHT_MODEL_CLIENT_LIGHTNESS_MODEL_ID,       LIGHT_LIGHTNESS_RANGE_STATUS,          MOBLE_FALSE, 5,               5,                0,                                  5,                5},
#endif                                                                                                                                                                     
                                                                                                                                                                           
#ifdef ENABLE_LIGHT_MODEL_CLIENT_CTL                                                                                                                                       
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_GET,                         MOBLE_TRUE,  0,               0,                LIGHT_CTL_STATUS,                   4,                9},     
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_SET,                         MOBLE_TRUE,  7,               9,                LIGHT_CTL_STATUS,                   4,                9},     
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_SET_UNACK,                   MOBLE_FALSE, 7,               9,                LIGHT_CTL_STATUS,                   4,                9},
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_STATUS,                      MOBLE_FALSE, 4,               9,                0,                                  4,                9},
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_TEMPERATURE_RANGE_GET,       MOBLE_TRUE,  0,               0,                LIGHT_CTL_TEMPERATURE_RANGE_STATUS, 5,                5},
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_TEMPERATURE_RANGE_STATUS,    MOBLE_FALSE, 5,               5,                0,                                  5,                5},
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_DEFAULT_GET,                 MOBLE_TRUE,  0,               0,                LIGHT_CTL_DEFAULT_STATUS,           6,                6},  
  {LIGHT_MODEL_CLIENT_CTL_MODEL_ID,             LIGHT_CTL_DEFAULT_STATUS,              MOBLE_TRUE,  6,               6,                0,                                  6,                6},   
#endif                                                                                                                                                                     
                                                                                                                                                                           
#ifdef ENABLE_LIGHT_MODEL_CLIENT_HSL
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_GET,                         MOBLE_TRUE,  0,               0,                LIGHT_HSL_STATUS,                   6,                7}, 
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_SET,                         MOBLE_TRUE,  7,               9,                LIGHT_HSL_STATUS,                   6,                7},   
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_SET_UNACK,                   MOBLE_FALSE, 7,               9,                LIGHT_HSL_STATUS,                   6,                7},    
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_STATUS,                      MOBLE_FALSE, 6,               7,                0,                                  6,                7},  
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_TARGET_GET,                  MOBLE_TRUE,  0,               0,                LIGHT_HSL_TARGET_STATUS,            6,                7}, 
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_TARGET_STATUS,               MOBLE_FALSE, 6,               7,                0,                                  6,                7},  
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_DEFAULT_GET,                 MOBLE_TRUE,  0,               0,                LIGHT_HSL_DEFAULT_STATUS,           6,                6},  
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_DEFAULT_STATUS,              MOBLE_FALSE, 6,               6,                0,                                  6,                6},  
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_RANGE_GET,                   MOBLE_TRUE,  0,               0,                LIGHT_HSL_RANGE_STATUS,             9,                9},
  {LIGHT_MODEL_CLIENT_HSL_MODEL_ID,             LIGHT_HSL_RANGE_STATUS,                MOBLE_TRUE,  9,               9,                0,                                  9,                9},
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
  
  TRACE_M(TF_LIGHT_CLIENT_M,"Light_Lightness_Set_Unack Client Message \r\n");  
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
    TRACE_M(TF_LIGHT_CLIENT_M,"Publication Error \r\n");
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
  TRACE_M(TF_LIGHT_CLIENT_M,"response status enable \n\r");
  
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
  
  TRACE_M(TF_LIGHT_CLIENT_M,"dst_peer = %.2X , peer_add = %.2X, opcode= %.2X ,response= %.2X \r\n  ",
          dst_peer, peer_addr, opcode , response);
  
  switch(opcode)
  { 
  case LIGHT_LIGHTNESS_STATUS:
    {     
      Light_Client_Lightness_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_LIGHTNESS_LINEAR_STATUS:
    {     
      Light_Client_Lightness_Linear_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_LIGHTNESS_LAST_STATUS:
    {     
      Light_Client_Lightness_Last_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_LIGHTNESS_DEFAULT_STATUS:
    {     
      Light_Client_Lightness_Default_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_LIGHTNESS_RANGE_STATUS:
    {     
      Light_Client_Lightness_Range_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_CTL_STATUS:
    {     
      Light_Client_Ctl_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_CTL_TEMPERATURE_RANGE_STATUS:
    {     
      Light_Client_CtlTemperature_Range_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_CTL_TEMPERATURE_STATUS:
    {     
      Light_Client_CtlTemperature_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_CTL_DEFAULT_STATUS:
    {     
      Light_Client_CtlDefault_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_HSL_HUE_STATUS:
    {     
      Light_Client_HslHue_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_HSL_SATURATION_STATUS:
    {     
      Light_Client_HslSaturation_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_HSL_STATUS:
    {     
      Light_Client_Hsl_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_HSL_TARGET_STATUS:
    {     
      Light_Client_HslTarget_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_HSL_DEFAULT_STATUS:
    {     
      Light_Client_HslDefault_Status(pRxData, dataLength);         
      break;
    } 
  case LIGHT_HSL_RANGE_STATUS:
    {     
      Light_Client_HslRange_Status(pRxData, dataLength);         
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

