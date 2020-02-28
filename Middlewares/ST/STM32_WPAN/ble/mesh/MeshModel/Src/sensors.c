/**
******************************************************************************
* @file    sensors.c
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
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
#include "common.h"
#include "light_lc.h"
#include <string.h>
#include "compiler.h"
#include "Math.h"

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

Sensor_SettingParam_t Sensor_SettingParam;
Sensor_ColumnParam_t Sensor_ColumnParam;
Sensor_CadenceParam_t Sensor_CadenceParam[2];

const MODEL_OpcodeTableParam_t Sensor_Opcodes_Table[] = {
  /*MOBLEUINT32 opcode, MOBLEBOOL reliable, MOBLEUINT16 min_payload_size, 
  MOBLEUINT16 max_payload_size;
  Here in this array, Handler is not defined; */
  
#ifdef ENABLE_SENSOR_MODEL_SERVER       
  {SENSOR_SERVER_MODEL_ID    ,SENSOR_DESCRIPTOR_GET,                    MOBLE_TRUE,   0, 2,               SENSOR_DESCRIPTOR_STATUS , 2, 75},
  {SENSOR_SERVER_MODEL_ID    ,SENSOR_DESCRIPTOR_STATUS,                 MOBLE_FALSE,  2, 75,               SENSOR_DESCRIPTOR_STATUS , 2, 75},
  {SENSOR_SERVER_MODEL_ID    ,SENSOR_GET,                               MOBLE_TRUE,   0, 2,               SENSOR_STATUS , 0,65 },  /* STATUS MESSAGE AS MARSHALLED DATA */
  {SENSOR_SERVER_MODEL_ID    ,SENSOR_STATUS,                            MOBLE_FALSE,  0, 65,              0 , 0,65 }, 
  {SENSOR_SERVER_MODEL_ID    ,SENSOR_COLUMN_GET,                        MOBLE_TRUE,   3, 3,               SENSOR_COLUMN_STATUS , 4, 8},  /* GET VARIABLE TAKEN AS 1 (2+VARIABLE) */
  {SENSOR_SERVER_MODEL_ID    ,SENSOR_COLUMN_STATUS,                     MOBLE_FALSE,  4, 8,               0 , 4, 8},
#endif
  
#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP     
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_CADENCE_GET,                       MOBLE_TRUE,   2, 2,               SENSOR_CADENCE_STATUS , 2, 8},
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_CADENCE_SET,                       MOBLE_TRUE,   8, 8,               SENSOR_CADENCE_STATUS , 2, 8},
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_CADENCE_SET_UNACK,                 MOBLE_FALSE,  8, 8,               SENSOR_CADENCE_STATUS , 2, 8},
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_CADENCE_STATUS,                    MOBLE_FALSE,  2, 8,               0 , 2, 8},
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SETTING_GET,                       MOBLE_TRUE,   2, 2,               SENSOR_SETTING_STATUS_PID , 4 , 4},
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SETTING_STATUS_PID,                MOBLE_FALSE,  4, 4,               0 , 4 , 4},
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SETTING_GET_SETTING_ID,            MOBLE_TRUE,   4, 4,               SENSOR_SETTING_STATUS_SETTING_ID , 5, 5},  /* STATUS VARIABLE  TAKEN AS 1 (4 + VARIABLE) */
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SETTING_SET,                       MOBLE_TRUE,   5, 5,               SENSOR_SETTING_STATUS_SETTING_ID , 5, 5},  /* SET VARIABLE TAKEN AS 1  (4_VARIABLE) */
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SETTING_SET_UNACK,                 MOBLE_FALSE,  5, 5,               SENSOR_SETTING_STATUS_SETTING_ID , 5, 5},  
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SETTING_STATUS_SETTING_ID,         MOBLE_FALSE,  5, 5,               0 , 5, 5},
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SERIES_GET,                        MOBLE_TRUE,   2, 6,               SENSOR_SERIES_STATUS , 8, 8},  /* GET VARIABLE TAKEN AS 4 (2+VARAIBLE) , 2 VARIABLE PARAMTER */
  {SENSOR_SETUP_SERVER_MODEL_ID    ,SENSOR_SERIES_STATUS,                     MOBLE_FALSE,  8, 8,               0 , 8, 8},
#endif
  {0}
};

/* Private function prototypes -----------------------------------------------*/
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Descriptor_Status(MOBLEUINT8* sensor_Descriptor , MOBLEUINT32* pLength));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Data_Status(MOBLEUINT8* sensor_Data , MOBLEUINT32* pLength));
WEAK_FUNCTION (void SensorDataPublish(MOBLEUINT32 *pSensor_Value , MOBLEUINT16* pProp_ID));
WEAK_FUNCTION (void Sensor_Publication_Process(float* pSensorData, MODEL_Property_IDTableParam_t* pProp_ID));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Cadence_Set(Sensor_CadenceParam_t* pCadence_param, 
                                                              MOBLEUINT16 property_ID, MOBLEUINT32 length) );
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Setting_Set(Sensor_SettingParam_t* pSensor_SettingParam,
                                                                       MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_GetSettingStatus(MOBLEUINT8* pSetting_Status));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_GetSetting_IDStatus(MOBLEUINT8* pSetting_Status));
MOBLE_RESULT Sensor_Cadence_Status(MOBLEUINT8* pCadencestatus_param, MOBLEUINT32 *plength,
                                   MOBLEUINT8 const *pData, MOBLEUINT32 length);

/* Private functions ---------------------------------------------------------*/

#ifdef ENABLE_SENSOR_MODEL_SERVER

/**
* @brief  Sensor_Data_Status: This function is called for both Acknowledged and 
unacknowledged message
* @param  pSensorData_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message.
* @param  pData:Pointer of data coming in packet.
* @param  length: lenth of the data in packet.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Data_Status(MOBLEUINT8* pSensorData_param, MOBLEUINT32* plength ,
                                MOBLEUINT8 const *pData, MOBLEUINT32 length)
{
  MOBLEUINT16 prop_ID = 0x00;
  
  TRACE_M(TF_SENSOR,"Sensor_Data_Status received \r\n");
  
  if(length > 0)  
  {
    prop_ID = pData[1] << 8;
    prop_ID |= pData[0]; 
  }
  /* Application Callback */   
  (SensorAppli_cb.Sensor_Data_cb)(pSensorData_param,plength,prop_ID,length);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Sensor_Descriptor_Status: This function is called for both Acknowledged and 
unacknowledged message
* @param  pSensorDescriptor_param: Pointer to the parameters received for message
* @param  plength: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Descriptor_Status(MOBLEUINT8* pSensorDescriptor_param, MOBLEUINT32* plength)
{
  
  TRACE_M(TF_SENSOR,"Sensor_Descriptor_Status received \r\n");
  
  /* Application Callback */ 
  (SensorAppli_cb.Sensor_Descriptor_cb)(pSensorDescriptor_param,plength);
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Sensor_Cadence_Set
* @param  pCadence_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Cadence_Set(const MOBLEUINT8* pCadence_param, MOBLEUINT32 length)
{
  MOBLEUINT16 prop_ID = 0x00; 
  
  TRACE_M(TF_SENSOR,"Sensor_Cadence_Set callback received \r\n");
  
  if(length > 0)  
  {
    prop_ID = pCadence_param[1] << 8;
    prop_ID |= pCadence_param[0]; 
  }
  
  if(prop_ID == (MOBLEUINT16)TEMPERATURE_PID)
  {
    Sensor_CadenceParam[0].Property_ID = pCadence_param[1] << 8;
    Sensor_CadenceParam[0].Property_ID = pCadence_param[0];
    Sensor_CadenceParam[0].FastCadenceDevisor = (pCadence_param[2] && 0xFE) ;
    Sensor_CadenceParam[0].StatusTriggerType = (pCadence_param[2] && 0x01) ;
    Sensor_CadenceParam[0].triggerDeltaDown = pCadence_param[3];
    Sensor_CadenceParam[0].triggerDeltaUp = pCadence_param[4];
    Sensor_CadenceParam[0].StatusMinInterval = pCadence_param[5];
    Sensor_CadenceParam[0].FastCadenceLow = pCadence_param[6];
    Sensor_CadenceParam[0].FastCadenceHigh = pCadence_param[7];
  }
  else if(prop_ID == (MOBLEUINT16)PRESSURE_PID)
  {
    Sensor_CadenceParam[1].Property_ID = pCadence_param[9] << 8;
    Sensor_CadenceParam[1].Property_ID = pCadence_param[8];
    Sensor_CadenceParam[1].FastCadenceDevisor = (pCadence_param[10] && 0xFE) ;
    Sensor_CadenceParam[1].StatusTriggerType = (pCadence_param[10] && 0x01) ;
    Sensor_CadenceParam[1].triggerDeltaDown = pCadence_param[11];
    Sensor_CadenceParam[1].triggerDeltaUp = pCadence_param[12];
    Sensor_CadenceParam[1].StatusMinInterval = pCadence_param[13];
    Sensor_CadenceParam[1].FastCadenceLow = pCadence_param[14];
    Sensor_CadenceParam[1].FastCadenceHigh = pCadence_param[15];
  }
  else
  {
    /* No Comments */
  }
  
  /* Application Callback */ 
  (SensorAppli_cb.Sensor_Cadence_Set_cb)(Sensor_CadenceParam,prop_ID,length);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Sensor_Cadence_Status
* @param  pCadencestatus_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @param  pData:Pointer of data coming in packet.
* @param  length: lenth of the data in packet.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Cadence_Status(MOBLEUINT8* pCadencestatus_param, MOBLEUINT32 *plength,
                                   MOBLEUINT8 const *pData, MOBLEUINT32 length)
{
  MOBLEUINT16 propery_ID;
  TRACE_M(TF_SENSOR,"Sensor_Cadence_Get callback received \r\n");
  
  propery_ID = pData[1] << 8;
  propery_ID |= pData[0];   
  
  if(propery_ID == (MOBLEUINT16)TEMPERATURE_PID)
  {
    *(pCadencestatus_param) = Sensor_CadenceParam[0].Property_ID;
    *(pCadencestatus_param+1) = Sensor_CadenceParam[0].Property_ID >> 8;
    *(pCadencestatus_param+2) = Sensor_CadenceParam[0].FastCadenceDevisor;
    *(pCadencestatus_param+3) = Sensor_CadenceParam[0].StatusTriggerType;
    *(pCadencestatus_param+4) = Sensor_CadenceParam[0].triggerDeltaDown;
    *(pCadencestatus_param+5) = Sensor_CadenceParam[0].triggerDeltaUp;
    
    memcpy(&pCadencestatus_param[6],(void*)&Sensor_CadenceParam[0].FastCadenceLow,4);
    memcpy(&pCadencestatus_param[10],(MOBLEUINT8*)&Sensor_CadenceParam[0].FastCadenceHigh,4);
  }
  if(propery_ID == (MOBLEUINT16)PRESSURE_PID)
  {
    *(pCadencestatus_param) = Sensor_CadenceParam[1].Property_ID;
    *(pCadencestatus_param+1) = Sensor_CadenceParam[1].Property_ID >> 8;
    *(pCadencestatus_param+2) = Sensor_CadenceParam[1].FastCadenceDevisor;
    *(pCadencestatus_param+3) = Sensor_CadenceParam[1].StatusTriggerType;
    *(pCadencestatus_param+4) = Sensor_CadenceParam[1].triggerDeltaDown;
    *(pCadencestatus_param+5) = Sensor_CadenceParam[1].triggerDeltaUp;
    
    memcpy(&pCadencestatus_param[6],(void*)&Sensor_CadenceParam[1].FastCadenceLow,4);
    memcpy(&pCadencestatus_param[10],(void*)&Sensor_CadenceParam[1].FastCadenceHigh,4);
  }
  
  *plength = 13;
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Sensor_Setting_Set
* @param  pSetting_param: Pointer to the parameters received for message
* @param  length: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Setting_Set(const MOBLEUINT8* pSetting_param, MOBLEUINT32 length)
{
  
  TRACE_M(TF_SENSOR,"Sensor_Setting_Set callback received \r\n");
  
  Sensor_SettingParam.Property_ID = pSetting_param[1] << 8;
  Sensor_SettingParam.Property_ID = pSetting_param[0];
  Sensor_SettingParam.Sensor_Setting_ID = pSetting_param[3] << 8; 
  Sensor_SettingParam.Sensor_Setting_ID = pSetting_param[2];
  Sensor_SettingParam.Sensor_Setting_Value = pSetting_param[5] << 8;
  Sensor_SettingParam.Sensor_Setting_Value = pSetting_param[4];
  
  /* Application Callback */ 
  (SensorAppli_cb.Sensor_Setting_Set_cb)(&Sensor_SettingParam,0);
  
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Sensor_Setting_Status_PID
* @param  pSetting_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @param  pData: Pointer of data coming in packet.
* @param  length: length of the data in packet.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Setting_Status_PID(MOBLEUINT8* pSetting_param, MOBLEUINT32 *plength 
                                       ,const MOBLEUINT8 *pData,MOBLEUINT32 length)
{
  MOBLEUINT8 Sensor_GetBuff[4];
  MOBLEUINT16 propery_ID;
  
  TRACE_M(TF_SENSOR,"Sensor_Setting_Status callback received \r\n");
  
  (Appli_Sensor_GetStatus_cb.GetSettingStatus_cb)(Sensor_GetBuff);
  
  propery_ID = pData[1] << 8;
  propery_ID = pData[0];
  
  if(Sensor_SettingParam.Property_ID == propery_ID)             
  {
    memcpy(pSetting_param , Sensor_GetBuff,4); 
    
    *plength = 4;
  }
  
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  Sensor_Setting_Status_SettingID with setting id
* @param  pSetting_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message
* @param  pData: Pointer of data coming in packet.
* @param  length: length of the data in packet.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Setting_Status_SettingID(MOBLEUINT8* pSetting_param, MOBLEUINT32 *plength 
                                             ,const MOBLEUINT8 *pData,MOBLEUINT32 length)
{
  MOBLEUINT8 Sensor_GetBuff[7];
  MOBLEUINT16 propery_ID;
  
  TRACE_M(TF_SENSOR,"Sensor_Setting_Status with setting id callback received \r\n");
  
  
  (Appli_Sensor_GetStatus_cb.GetSetting_IDStatus_cb)(Sensor_GetBuff);
  
  propery_ID = pData[1] << 8;
  propery_ID = pData[0];
  
  if(Sensor_SettingParam.Property_ID == propery_ID)             
  {
    memcpy(pSetting_param , Sensor_GetBuff,7); 
    
    *plength = 7;
  }
  
  return MOBLE_RESULT_SUCCESS;
}

#endif

/**
* @brief  SensorModelServer_GetOpcodeTableCb: This function is call-back 
from the library to send Model Opcode Table info to library
* @param  MODEL_OpcodeTableParam_t:  Pointer to the sensor Model opcode array 
* @param  length: Pointer to the Length of sensor Model opcode array
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
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
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
#ifdef ENABLE_SENSOR_MODEL_SERVER
  case SENSOR_DESCRIPTOR_STATUS:
    {
      Sensor_Descriptor_Status(pResponsedata ,plength);
      break;
    }
  case SENSOR_STATUS:
    {
      Sensor_Data_Status(pResponsedata ,plength,pRxData,dataLength);
      break;
    }
    
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
      Sensor_Cadence_Status(pResponsedata ,plength,pRxData,dataLength);
      break;
    }
  case SENSOR_SETTING_STATUS_PID:
    {
      Sensor_Setting_Status_PID(pResponsedata ,plength,pRxData,dataLength);
      break;
    }
  case SENSOR_SETTING_STATUS_SETTING_ID:
    {
      Model_SendResponse(peer_addr, dst_peer,opcode,pRxData,dataLength);
      break;
    }
#endif           
  default:
    {
      
    } 
  }
  return MOBLE_RESULT_SUCCESS;    
}


/**
* @brief  SensorModelServer_ProcessMessageCb: This is a callback function from
the library whenever a sensor Model message is received
* @param  peer_addr: Address of the peer
* @param  dst_peer: destination send by peer for this node. It can be a
*                                                     unicast or group address 
* @param  opcode: Received opcode of the Status message callback
* @param  response: if TRUE, the message is an acknowledged message 
* @param  pRxData: Pointer to the data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate wheather message is acknowledged meassage or not.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT SensorModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                                MOBLE_ADDRESS dst_peer, 
                                                MOBLEUINT16 opcode, 
                                                MOBLEUINT8 const *pRxData, 
                                                MOBLEUINT32 dataLength, 
                                                MOBLEBOOL response
                                                  )
{
  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
/*  tClockTime delay_t = Clock_Time(); */
#ifdef ENABLE_SENSOR_MODEL_SERVER 
    MOBLEUINT16 property_ID = 0;
    property_ID = pRxData[1] << 8;
    property_ID |= pRxData[0];
#endif
  
  TRACE_M(TF_SENSOR,"dst_peer = %.2X , peer_add = %.2X, opcode = %.2X, response= %.2X \r\n ",
          dst_peer, peer_addr, opcode, response);
  switch(opcode)
  {
#ifdef ENABLE_SENSOR_MODEL_SERVER
  case SENSOR_DESCRIPTOR_GET:
    {
      break;
    }    
  case SENSOR_GET:
    {
      if((dataLength > 0) && (property_ID == 0x00))
        {
          return MOBLE_RESULT_FALSE;
        }
      break;
    }
    
  case SENSOR_CADENCE_SET:
  case SENSOR_CADENCE_SET_UNACK:
    {
      if((dataLength > 0) && (property_ID == 0x00))
        {
          return MOBLE_RESULT_FALSE;
        }
      Sensor_Cadence_Set(pRxData,dataLength);
      break;
    }
    
  case SENSOR_SETTING_SET:
  case SENSOR_SETTING_SET_UNACK:
    {
      if((dataLength > 0) && (property_ID == 0x00))
      {
         return MOBLE_RESULT_FALSE;
      }   
      Sensor_Setting_Set(pRxData,dataLength);
      break;
    } 
  case SENSOR_DESCRIPTOR_STATUS:
    {
      break;
    }
  case SENSOR_STATUS:
    {
     if((dataLength > 0) && (property_ID == 0x00))
        {
          return MOBLE_RESULT_FALSE;
        }
     
     if(property_ID == PRESENCE_DETECTED_PROPERTY)
     {  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC        
        Light_LC_ModeSet(&pRxData[2],1);  
        Light_LC_OMSet(&pRxData[2],1);   
#endif        
     }
      break;
    }
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
  case SENSOR_SETTING_STATUS_PID:
    {
      break;
    }
  case SENSOR_SETTING_STATUS_SETTING_ID:
    {
      break;
    } 
#endif		
  default:
    {
      break;
    }
    
  } /* Switch ends */
  
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE))
  {
    Model_SendResponse(peer_addr, dst_peer,opcode,pRxData,dataLength);
  }
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  floatToInt: This function is used to convert the float to integer 
* @param  in: sensor data value
* @param  out_value: pointer to the structure
* @@param dec_prec: decimal point resolution
* @retval void
*/ 
void floatToInt(float in, displayFloatToInt_t *out_value, MOBLEINT32 dec_prec)
{
  if(in >= 0.0f)
  {
    out_value->sign = 0;
  }else
  {
    out_value->sign = 1;
    in = -in;
  }
  out_value->out_int = (int32_t)in;
  in = in - (float)(out_value->out_int);
  out_value->out_dec = (int32_t)trunc(in * pow(10, dec_prec));
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

WEAK_FUNCTION (void SensorDataPublish(MOBLEUINT32 *pSensor_Value , MOBLEUINT16* pProp_ID));
WEAK_FUNCTION (void Sensor_Publication_Process(float* pSensorData, MODEL_Property_IDTableParam_t* pProp_ID))
{
}
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Cadence_Set(Sensor_CadenceParam_t* pCadence_param, 
                                                              MOBLEUINT16 property_ID, MOBLEUINT32 length) );
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Setting_Set(Sensor_SettingParam_t* pSensor_SettingParam,
                                                                       MOBLEUINT8 OptionalValid));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_GetSettingStatus(MOBLEUINT8* pSetting_Status));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_GetSetting_IDStatus(MOBLEUINT8* pSetting_Status));

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

