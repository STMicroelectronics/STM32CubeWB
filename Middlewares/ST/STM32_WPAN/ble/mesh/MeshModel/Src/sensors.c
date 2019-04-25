/**
******************************************************************************
* @file    sensors.c
* @author  BLE Mesh Team
* @version V1.09.000
* @date    15-Oct-2018
* @brief   Sensors model middleware file
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
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "sensors.h"
#include "ble_mesh.h"
#include "models_if.h"
#include "appli_generic.h"
#include "common.h"
#include "vendor.h"
#include "mesh_cfg.h"
#include "compiler.h"
#include <string.h>

/** @addtogroup MODEL_SENSOR
*  @{
*/

/** @addtogroup Sensor_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
  
#define S_VARIABLE 2

/* Private variables ---------------------------------------------------------*/

 const MODEL_OpcodeTableParam_t Sensor_Opcodes_Table[] = {
  /*    MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
    MOBLEUINT16 max_payload_size;
    Here in this array, Handler is not defined; */
      
#ifdef ENABLE_SENSOR_MODEL_SERVER       
    {SENSOR_DESCRIPTOR_GET,                    MOBLE_TRUE,  0, 2,               SENSOR_DESCRIPTOR_STATUS , 2, 16},
    {SENSOR_GET,                               MOBLE_TRUE,  0, 2,               SENSOR_STATUS , 0,16 },  /* STATUS MESSAGE AS MARSHALLED DATA */
    {SENSOR_COLUMN_GET,                        MOBLE_TRUE,  3, 3,               SENSOR_COLUMN_STATUS , 4, 8},  /* GET VARIABLE TAKEN AS 1 (2+VARIABLE) */
#endif

#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP     
    {SENSOR_CADENCE_GET,                       MOBLE_TRUE,  2, 2,               SENSOR_CADENCE_STATUS , 2, 8},
    {SENSOR_CADENCE_SET,                       MOBLE_TRUE,  8, 8,               SENSOR_CADENCE_STATUS , 2, 8},
    {SENSOR_CADENCE_SET_UNACK,                 MOBLE_FALSE,  8, 8,              SENSOR_CADENCE_STATUS , 2, 8},
    {SENSOR_SETTING_GET,                       MOBLE_TRUE,  2, 2,               SENSOR_SETTING_STATUS , 4 , 4},
    {SENSOR_SETTING_GET_DIFF,                  MOBLE_TRUE,  4, 4,               SENSOR_SETTING_STATUS_DIFF , 5, 5},  /* STATUS VARIABLE  TAKEN AS 1 (4 + VARIABLE) */
    {SENSOR_SETTING_SET_DIFF,                  MOBLE_TRUE,  5, 5,               SENSOR_SETTING_STATUS_DIFF , 5, 5},  /* SET VARIABLE TAKEN AS 1  (4_VARIABLE) */
    {SENSOR_SETTING_SET_DIFF_UNACK,            MOBLE_FALSE,  5, 5,              SENSOR_SETTING_STATUS_DIFF , 5, 5},
    {SENSOR_SETTING_STATUS_DIFF,               MOBLE_TRUE,  5, 5,               SENSOR_SETTING_STATUS_DIFF , 5, 5},     
    {SENSOR_SERIES_GET,                        MOBLE_TRUE,  2, 6,               SENSOR_SERIES_STATUS , 8, 8},  /* GET VARIABLE TAKEN AS 4 (2+VARAIBLE) , 2 VARIABLE PARAMTER */
#endif    
    {0}
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Sensor_Data_Status: This function is called for both Acknowledged and 
          unacknowledged message
* @param  pSensorData_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Data_Status(MOBLEUINT8* pSensorData_param, MOBLEUINT32* plength ,
                                                      MOBLEUINT8 const *pData, MOBLEUINT32 length)
{
#if !defined(DISABLE_TRACES)
  printf("Sensor_Data_Status received \r\n");
#endif    
  
  MOBLEUINT16 prop_ID = 0x00;
  if(length > 0)  
  {
    prop_ID = pData[1] << 8;
    prop_ID |= pData[0]; 
  }
   /* Application Callback */   
  (SensorAppli_cb.Sensor_Data_cb)(pSensorData_param,plength,prop_ID);
  
 return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Sensor_Descriptor_Status: This function is called for both Acknowledged and 
          unacknowledged message
* @param  pSensorDescriptor_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Descriptor_Status(MOBLEUINT8* pSensorDescriptor_param, MOBLEUINT32* plength)
{
  
  
#if !defined(DISABLE_TRACES)
  printf("Sensor_Descriptor_Status received \r\n");
#endif    
     /* Application Callback */ 
     (SensorAppli_cb.Sensor_Descriptor_cb)(pSensorDescriptor_param,plength);
 return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  SensorModelServer_GetLightOpcodeTableCb: This function is call-back 
          from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the Light Model opcode array 
* @param  length: Pointer to the Length of Light Model opcode array
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length)
{
  *data = Sensor_Opcodes_Table;
  *length = sizeof(Sensor_Opcodes_Table)/sizeof(Sensor_Opcodes_Table[0]);

  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  SensorModelServer_GetStatusRequestCb : This function is call-back 
          from the library to send response to the message from peer
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pResponsedata: Pointer to the buffer to be updated with status
* @param  plength: Pointer to the Length of the data, to be updated by application
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pData,
                                    MOBLEUINT32 length,
                                    MOBLEBOOL response)
{
  switch(opcode)
  {
#ifdef ENABLE_SENSOR_MODEL_SERVER
      case SENSOR_DESCRIPTOR_STATUS:
      {
        Sensor_Descriptor_Status(pResponsedata ,plength);
        break;
      }
      case SENSOR_STATUS:
      {
        Sensor_Data_Status(pResponsedata ,plength,pData ,length);
        break;
      }
#endif      
      case SENSOR_COLUMN_STATUS:
      {
        break;
      }
      case SENSOR_SERIES_STATUS:
      {
        break;
      }
      case SENSOR_CADENCE_STATUS:
      {
        break;
      }
      case SENSOR_SETTING_STATUS:
      {
        break;
      }
      case SENSOR_SETTING_STATUS_DIFF:
      {
        break;
      }
      
      default:
      {
        
      } 
  }
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief  SensorModelServer_ProcessMessageCb: This is a callback function from
           the library whenever a Generic Model message is received
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  pData: Pointer to the buffer to be updated with status
* @param  length: Length of the parameters received 
* @param  response: if TRUE, the message is an acknowledged message 
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *pData, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response
                                    )
{

  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  tClockTime delay_t = Clock_Time();
   MOBLEUINT16 property_ID = 0x00;
#if !defined(DISABLE_TRACES) 
  printf("dst_peer = %.2X , peer_add = %.2X \r\n ",dst_peer, peer_addr);
#endif  
  switch(opcode)
  {
#ifdef ENABLE_SENSOR_MODEL_SERVER
    
    case SENSOR_DESCRIPTOR_GET:
    {
      break;
    }    
    case SENSOR_GET:
    {
    
      if(length > 0)
      {
        property_ID = pData[1] << 8;
        property_ID |= pData[0];
        if(property_ID == 0x00)
        {
          result = MOBLE_RESULT_FALSE;
        }
      }
        
      break;
    }
#endif
    
  case SENSOR_COLUMN_GET:
    break;

  case SENSOR_SERIES_GET:
    break;
    
  case SENSOR_CADENCE_GET:
    break;
    
    case SENSOR_CADENCE_SET:
      break;
      
  case SENSOR_CADENCE_SET_UNACK:
    break;
      
  case SENSOR_SETTING_GET:
      break;
    
  case SENSOR_SETTING_GET_DIFF:
    break;
    
  case SENSOR_SETTING_SET_DIFF:
    break;
    
  case SENSOR_SETTING_SET_DIFF_UNACK:
    break;
    
    default:
      break;
      
  } /* Switch ends */
  

  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    Sensor_SendResponse(peer_addr, dst_peer,opcode, length, &pData[0]);
  }
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Sensor model add function
* @param  function adds all the sub model of Light model.
          Function uses the mid of the model for adding.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT BLEMesh_AddSensorModels(void)
{
  MOBLE_RESULT res = MOBLE_RESULT_FAIL;
  
#ifdef ENABLE_SENSOR_MODEL_SERVER 
  
  if(MOBLE_FAILED(SensorModel_Add_Server(SENSOR_SERVER_MODEL_ID)))
  {
#if !defined(DISABLE_TRACES)
    printf("Failed to Add Sensor Server Model \n\r");
#endif
    res = MOBLE_RESULT_FAIL;
  }
#endif    
  
   return res;    
}


void Sensor_Process(void)
{
  
}

/* Weak function are defined to support the original function if they are not
   included in firmware.
   There is no use of this function for application development purpose.
*/

WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Descriptor_Status(MOBLEUINT8* sensor_Descriptor , MOBLEUINT32* pLength))
{
               
  return MOBLE_RESULT_SUCCESS;
}


WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Data_Status(MOBLEUINT8* sensor_Data , MOBLEUINT32* pLength))
{
               
  return MOBLE_RESULT_SUCCESS;
}  


/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

