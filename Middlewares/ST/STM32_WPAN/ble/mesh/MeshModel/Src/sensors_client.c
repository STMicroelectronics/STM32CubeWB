/**
******************************************************************************
* @file    sensors_client.c
* @author  BLE Mesh Team
* @brief   Sensors model client middleware file
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
#include "sensors.h"
#include "sensors_client.h"
#include "common.h"
#include "models_if.h"
#include <string.h>
#include "compiler.h"


/** @addtogroup MODEL_CLIENT_SENSORS
*  @{
*/

/** @addtogroup Sensors_Model_Client_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern Model_Tid_t Model_Tid;;
const MODEL_OpcodeTableParam_t Sensors_Client_Opcodes_Table[] = {
  /* Sensors Client */
/* model_id                opcode,                   reliable,    min_payload_size, max_payload_size, response_opcode,          min_response_size, max_response_size
    Here in this array, Handler is not defined; */
#ifdef ENABLE_SENSOR_MODEL_CLIENT       
  {SENSOR_CLIENT_MODEL_ID, SENSOR_DESCRIPTOR_GET,    MOBLE_FALSE, 0,                2,                SENSOR_DESCRIPTOR_STATUS, 2,                 75},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_DESCRIPTOR_STATUS, MOBLE_FALSE, 2,                75,               0,                        1,                 1},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_CADENCE_GET,       MOBLE_FALSE, 2,                2,                SENSOR_CADENCE_STATUS,    2,                 2},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_CADENCE_SET,       MOBLE_FALSE, 8,                20,               0,                        2,                 2},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_CADENCE_SET_UNACK, MOBLE_FALSE, 8,                20,               0,                        2,                 2},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_CADENCE_STATUS,    MOBLE_FALSE, 2,                2,                0,                        1,                 1},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SETTINGS_GET,      MOBLE_FALSE, 2,                2,                SENSOR_SETTINGS_STATUS,   2,                 2},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SETTINGS_STATUS,   MOBLE_FALSE, 2,                2,                0,                        1,                 1},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SETTING_GET,       MOBLE_FALSE, 4,                4,                SENSOR_SETTING_STATUS,    2,                 2},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SETTING_SET,       MOBLE_FALSE, 5,                8,                0,                        2,                 2},  
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SETTING_SET_UNACK, MOBLE_FALSE, 5,                8,                0,                        2,                 2},  
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SETTING_STATUS,    MOBLE_FALSE, 2,                2,                0,                        1,                 1},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_GET,               MOBLE_FALSE, 0,                2,                SENSOR_STATUS,            0,                 65},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_STATUS,            MOBLE_FALSE, 0,                65,               0,                        1,                 1},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_COLUMN_GET,        MOBLE_FALSE, 3,                6,                SENSOR_COLUMN_STATUS,     2,                 14},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_COLUMN_STATUS,     MOBLE_FALSE, 2,                14,               0,                        1,                 1},
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SERIES_GET,        MOBLE_TRUE,  2,                10,               SENSOR_SERIES_STATUS,     2,                 2},  
  {SENSOR_CLIENT_MODEL_ID, SENSOR_SERIES_STATUS,     MOBLE_FALSE, 2,                2,                0,                        1,                 1},
#endif
  {0}
};

/* Private function prototypes -----------------------------------------------*/
MOBLE_RESULT SensorsClient_Column_Get(MOBLEUINT8 elementIndex, 
                                      MOBLEUINT8 * pparam,
                                      MOBLEUINT8 length);
MOBLE_RESULT SensorsClient_Series_Get(MOBLEUINT8 elementIndex, 
                                      MOBLEUINT8 * pparam,
                                      MOBLEUINT8 length);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  SensorsClient_Descriptor_Get: This function is called for Descriptor Get message
* @param  elementIndex : Index of the element
* @param  ppropertyId : pointer on property ID, if not present NULL
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Descriptor_Get(MOBLEUINT8 elementIndex, 
                                          MOBLEUINT8 * ppropertyId)
{
  
  /* 4.2.1 Sensor Descriptor Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;
  MOBLEUINT8 length = 0;
  
  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Descriptor_Get Client Message \r\n");  
  
  msg_opcode = SENSOR_DESCRIPTOR_GET;
  if(ppropertyId != NULL)
  {
    msg_buff = ppropertyId;
    length = 2;
  }
  
  /* 
     Manage the TID Here....
  */
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  Model_Tid.TidSend++;
  if(Model_Tid.TidSend >= MAX_TID_VALUE)
  {
    Model_Tid.TidSend = 0;
  }  
  
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Cadence_Get: This function is called for Descriptor Get message
* @param  elementIndex : Index of the element
* @param  ppropertyId : pointer on property ID
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Cadence_Get(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 * ppropertyId)
{
  
  /* 4.2.3 Sensor Cadence Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;
  MOBLEUINT8 length = 0;
  
  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Cadence_Get Client Message \r\n");  
  
  msg_opcode = SENSOR_CADENCE_GET;
  if(ppropertyId != NULL)
  {
    msg_buff = ppropertyId;
    length = 2;
  }
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Cadence_Set: This function is called for Cadence Set message
* @param  elementIndex : Index of the element
* @param  pCadence_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Cadence_Set(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 *pCadence_param, 
                                       MOBLEUINT32 length) 

{
  
  /* 4.2.4 Sensor Cadence Set 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Cadence_Set Client Message \r\n");  
  
  msg_buff = pCadence_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = SENSOR_CADENCE_SET;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Cadence_Set: This function is called for Cadence Set message
* @param  elementIndex : Index of the element
* @param  pCadence_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Cadence_Set_Unack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pCadence_param, 
                                             MOBLEUINT32 length) 

{
  
  /* 4.2.5 Sensor Cadence Set Unacknowledged 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Cadence_Set_Unack Client Message \r\n");  
  
  msg_buff = pCadence_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = SENSOR_CADENCE_SET_UNACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Settings_Get: This function is called for Settings Get message
* @param  elementIndex : Index of the element
* @param  ppropertyId : pointer on property ID
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Settings_Get(MOBLEUINT8 elementIndex, 
                                        MOBLEUINT8 * ppropertyId)
{
  
  /* 4.2.7 Sensor Settings Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;
  
  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Settings_Get Client Message \r\n");  
  
  msg_opcode = SENSOR_SETTINGS_GET;
  msg_buff = ppropertyId;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, 2,
                                            ack_flag, 
                                            MOBLE_FALSE);
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Setting_Get: This function is called for Setting Get message
* @param  elementIndex : Index of the element
* @param  ppropertyId : pointer on property ID
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Setting_Get(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 * pparam,
                                       MOBLEUINT8 length)
{
  
  /* 4.2.9 Sensor Setting Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;
  
  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Setting_Get Client Message \r\n");  
  
  msg_opcode = SENSOR_SETTING_GET;
  msg_buff = pparam;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Setting_Set: This function is called for Setting Set message
* @param  elementIndex : Index of the element
* @param  pSetting_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Setting_Set(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 *pSetting_param, 
                                       MOBLEUINT32 length) 

{
  
  /* 4.2.4 Sensor Cadence Set 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Setting_Set Client Message \r\n");  
  
  msg_buff = pSetting_param;
  ack_flag = MOBLE_TRUE;
  msg_opcode = SENSOR_SETTING_SET;
  
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Setting_Set_Unack: This function is called for Setting Set message
* @param  elementIndex : Index of the element
* @param  pSetting_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Setting_Set_Unack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pSetting_param, 
                                             MOBLEUINT32 length) 

{
  
  /* 4.2.11 Sensor Setting Set Unacknowledged 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 const *msg_buff; 
  MOBLEUINT16 msg_opcode;
  MOBLEBOOL ack_flag;

  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Setting_Set_Unack Client Message \r\n");  
  
  msg_buff = pSetting_param;
  ack_flag = MOBLE_FALSE;
  msg_opcode = SENSOR_SETTING_SET_UNACK;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Get: This function is called for Get message
* @param  elementIndex : Index of the element
* @param  ppropertyId : pointer on property ID, if not present NULL
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Get(MOBLEUINT8 elementIndex, 
                               MOBLEUINT8 * ppropertyId)
{
  
  /* 4.2.13 Sensor Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;
  MOBLEUINT8 length = 0;
  
  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Get Client Message \r\n");  
  
  msg_opcode = SENSOR_GET;
  if(ppropertyId != NULL)
  {
    msg_buff = ppropertyId;
    length = 2;
  }
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Column_Get: This function is called for Column Get message
* @param  elementIndex : Index of the element
* @param  pparam : pointer on parameters, Property ID and Raw Value X
* @param length :  length of the parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Column_Get(MOBLEUINT8 elementIndex, 
                               MOBLEUINT8 * pparam,
                               MOBLEUINT8 length)
{
  
  /* 4.2.15 Sensor Column Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;
  
  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Column_Get Client Message \r\n");  
  
  msg_opcode = SENSOR_COLUMN_GET;
  msg_buff = pparam;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief  SensorsClient_Series_Get: This function is called for Series Get message
* @param  elementIndex : Index of the element
* @param  pparam : pointer on parameters, Property ID, Raw Values X1 and X2
* @param length :  length of the parameters 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsClient_Series_Get(MOBLEUINT8 elementIndex, 
                               MOBLEUINT8 * pparam,
                               MOBLEUINT8 length)
{
  
  /* 4.2.15 Sensor Column Get 
  */
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 msg_opcode;
  MOBLEUINT8 const *msg_buff = NULL; 
  MOBLEBOOL ack_flag = MOBLE_FALSE;
  
  TRACE_M(TF_SENSOR_CLIENT_M, "SensorsClient_Series_Get Client Message \r\n");  
  
  msg_opcode = SENSOR_SERIES_GET;
  msg_buff = pparam;
  
  result = MeshClient_SetRemotePublication((MOBLEUINT32) SENSOR_CLIENT_MODEL_ID, 
                                            elementIndex ,
                                            msg_opcode , 
                                            msg_buff, length,
                                            ack_flag, 
                                            MOBLE_FALSE);
  
  if(result)
  {
    TRACE_M(TF_SENSOR_CLIENT_M, "Publication Error \r\n");
  }  
  
  return result;
}


/**
* @brief   SensorsModelServer_GetOpcodeTableCb: This function is call-back 
*          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Sensors Model opcode array 
* @param  length: Pointer to the Length of Sensors Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length)
{
  *data = Sensors_Client_Opcodes_Table;
  *length = sizeof(Sensors_Client_Opcodes_Table)/sizeof(Sensors_Client_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  SensorsModelClient_GetStatusRequestCb : This function is call-back 
*         from the library to send response to the message from peer
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsModelClient_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam,
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response)

{
  TRACE_M(TF_SENSOR_CLIENT_M, "response status enable \n\r");
  
  return MOBLE_RESULT_SUCCESS;    
}

/**
* @brief  SensorsModelClient_ProcessMessageCb: This is a callback function from
*         the library whenever a Sensors Model message is received
* @param  *pmsgParam Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & AppKey Offset
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorsModelClient_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam,
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response)
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  TRACE_M(TF_SENSOR_CLIENT_M, 
          "elementIndex = %.2x, dst_peer = %.2X, peer_add = %.2X, opcode= %.2X ,response= %.2X\r\n", 
          pmsgParam->elementIndex, 
          pmsgParam->dst_peer,
          pmsgParam->peer_addr, 
          opcode, 
          response);
  
  switch(opcode)
  { 
    case SENSOR_DESCRIPTOR_STATUS:
    {     
      SensorAppli_cb.Sensor_Descriptor_Status_cb(pRxData , dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);
      break;
    } 
    case SENSOR_CADENCE_STATUS:
    {     
      SensorAppli_cb.Sensor_Cadence_Status_cb(pRxData , dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);
      break;
    } 
    case SENSOR_SETTINGS_STATUS:
    {     
      SensorAppli_cb.Sensor_Settings_Status_cb(pRxData , dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);
      break;
    } 
    case SENSOR_SETTING_STATUS:
    {     
      SensorAppli_cb.Sensor_Setting_Status_cb(pRxData , dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);
      break;
    } 
    case SENSOR_STATUS:
    {     
      SensorAppli_cb.Sensor_Status_cb(pRxData , dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);
      break;
    } 
    case SENSOR_COLUMN_STATUS:
    {     
      SensorAppli_cb.Sensor_Column_Status_cb(pRxData , dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);
      break;
    } 
    case SENSOR_SERIES_STATUS:
    {     
      SensorAppli_cb.Sensor_Series_Status_cb(pRxData , dataLength, pmsgParam->dst_peer, pmsgParam->elementIndex);
      break;
    } 
    default:
    {
      break;
    }
  }
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    Model_SendResponse(pmsgParam, opcode, pRxData, dataLength);
  }
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @}
*/

/**
* @}
*/



