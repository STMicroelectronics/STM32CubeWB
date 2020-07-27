/**
******************************************************************************
* @file    sensors.c
* @author  BLE Mesh Team
* @brief   Sensors model middleware file
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "sensors.h"
#include "common.h"
#include "light_lc.h"
#include <string.h>
#include "compiler.h"
//#include "math.h"
#include "mesh_cfg_usr.h"

/** @addtogroup MODEL_SENSOR
*  @{
*/

/** @addtogroup Sensor_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


Sensor_SettingParam_t Sensor_SettingParam[NUMBER_OF_SENSOR];
/*  */
//Sensor_ColumnParam_t Sensor_ColumnParam[NUMBER_OF_SENSOR];

//Sensor_Series_Param_t Sensor_SeriesParam;

Sensor_SettingParam_t Sensor_SettingParameter;
Sensor_SeriesParam_t Sensor_SeriesParam;
/*  */
Sensor_CadenceParam_t Sensor_CadenceParam[NUMBER_OF_SENSOR];
/*
=
{
  {0x0071 , 0x2 , 2 , 2 ,2 ,1 ,0X05 , 0x64},
  {0x2A6D , 0x2 , 1 , 1 , 1, 1, 0X258 , 0x3ED},
  {0X2A7F ,0x2,1,1,1,0,0x10, 0x20}
};*/


const MODEL_OpcodeTableParam_t Sensor_Opcodes_Table[] = {
/* model_id                      opcode                    reliable     min_payload_size max_payload_size response_opcode           min_response_size max_response_size */
  
#ifdef ENABLE_SENSOR_MODEL_SERVER       
  {SENSOR_SERVER_MODEL_ID,       SENSOR_DESCRIPTOR_GET,    MOBLE_FALSE, 0,               2,               SENSOR_DESCRIPTOR_STATUS, 2,                75},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_DESCRIPTOR_STATUS, MOBLE_FALSE, 2,               75,              0,                        1,                1},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_GET,               MOBLE_FALSE, 0,               2,               SENSOR_STATUS,            0,                65},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_STATUS,            MOBLE_FALSE, 0,               65,              0,                        1,                1},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_COLUMN_GET,        MOBLE_FALSE, 3,               6,               SENSOR_COLUMN_STATUS,     2,                14},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_COLUMN_STATUS,     MOBLE_FALSE, 2,               14,              0,                        1,                1},
#endif                                                                                                                              
                                                                                                                                    
#ifdef ENABLE_SENSOR_MODEL_CLIENT       
  {SENSOR_CLIENT_MODEL_ID,       SENSOR_DESCRIPTOR_GET,    MOBLE_FALSE, 0,               2,               SENSOR_DESCRIPTOR_STATUS, 2,                75},
  {SENSOR_CLIENT_MODEL_ID,       SENSOR_DESCRIPTOR_STATUS, MOBLE_FALSE, 2,               75,              0,                        1,                1},
  {SENSOR_CLIENT_MODEL_ID,       SENSOR_GET,               MOBLE_FALSE, 0,               2,               SENSOR_STATUS,            0,                65},
  {SENSOR_CLIENT_MODEL_ID,       SENSOR_STATUS,            MOBLE_FALSE, 0,               65,              0,                        1,                1},
  {SENSOR_CLIENT_MODEL_ID,       SENSOR_COLUMN_GET,        MOBLE_FALSE, 3,               6,               SENSOR_COLUMN_STATUS,     2,                14},
  {SENSOR_CLIENT_MODEL_ID,       SENSOR_COLUMN_STATUS,     MOBLE_FALSE, 2,               14,              0,                        1,                1},
#endif                                                                                                                              
                                                                                                                                    
#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP                                                                                             
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_CADENCE_GET,       MOBLE_FALSE, 2,               2,               SENSOR_CADENCE_STATUS,    2,                2},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_CADENCE_SET,       MOBLE_FALSE, 8,               20,              0,                        2,                2},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_CADENCE_SET_UNACK, MOBLE_FALSE, 8,               20,              0,                        2,                2},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_CADENCE_STATUS,    MOBLE_FALSE, 2,               2,               0,                        1,                1},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SETTINGS_GET,      MOBLE_FALSE, 2,               2,               SENSOR_SETTINGS_STATUS,   2,                2},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SETTINGS_STATUS,   MOBLE_FALSE, 2,               2,               0,                        1,                1},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SETTING_GET,       MOBLE_FALSE, 4,               4,               SENSOR_SETTING_STATUS,    2,                2},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SETTING_SET,       MOBLE_FALSE, 5,               8,               0,                        2,                2},  
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SETTING_SET_UNACK, MOBLE_FALSE, 5,               8,               0,                        2,                2},  
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SETTING_STATUS,    MOBLE_FALSE, 2,               2,               0,                        1,                1},
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SERIES_GET,        MOBLE_TRUE,  2,               10,              SENSOR_SERIES_STATUS,     2,                2},  
  {SENSOR_SETUP_SERVER_MODEL_ID, SENSOR_SERIES_STATUS,     MOBLE_FALSE, 2,               2,               0,                        1,                1},
#endif
  {0}
};

int Property_ID_TableSet[NUMBER_OF_SENSOR] = 
{
  TEMPERATURE_PID,PRESSURE_PID,TIME_OF_FLIGHT_PID
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
MOBLE_RESULT Sensor_Column_Status(MOBLEUINT8* pSensorColumn_param, MOBLEUINT32* plength,
                                  MOBLEUINT8 const *pData, MOBLEUINT32 length);
MOBLE_RESULT  Sensor_Series_Status(MOBLEUINT8* pSensorSeries_param, 
                                   MOBLEUINT32* plength,
                                   MOBLEUINT8 const *pData, 
                                   MOBLEUINT32 length);

/* Private functions ---------------------------------------------------------*/

#ifdef ENABLE_SENSOR_MODEL_SERVER

/**
* @brief  This function is called for both Acknowledged and unacknowledged message
*         //
* @param  pSensorData_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message.
* @param  pData:Pointer of data coming in packet.
* @param  length: lenth of the data in packet.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Data_Status(MOBLEUINT8* pSensorData_param, 
                                MOBLEUINT32* plength ,
                                MOBLEUINT8 const *pData, 
                                MOBLEUINT32 length)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 prop_ID = 0x0000;
  
  TRACE_M(TF_SENSOR_M, "Sensor_Data_Status received \r\n");
  
  TRACE_M(TF_SENSOR_M, "Length of Received Data %lu:", length);
  
  if(length > 0)  
  {
    /* Extract Property ID */
    prop_ID = pData[1] << 8;
    prop_ID |= pData[0]; 
  }
  
  /* Application Callback */   
  if (SensorAppli_cb.Sensor_Data_cb != NULL)
  {
    (SensorAppli_cb.Sensor_Data_cb)(pSensorData_param,plength,prop_ID,length);
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }
  
  return result;
}


/**
* @brief  Sensor_Descriptor_Status: This function is called for both Acknowledged and 
*         unacknowledged message
* @param  pSensorDescriptor_param: Pointer to the parameters received for message
* @param  plength: Length of the parameters received for message
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT Sensor_Descriptor_Status(MOBLEUINT8* pSensorDescriptor_param, MOBLEUINT32* plength,
                                      MOBLEUINT8 const *pData, MOBLEUINT32 length)
{
  MOBLEUINT16 prop_ID = 0x0000;
  
  TRACE_M(TF_SENSOR_M,"Sensor_Descriptor_Status received \r\n");
  if(length>0){  
    prop_ID = pData[1]<<8;
    prop_ID |= pData[0];
  }
  printf("plength %ld, length %ld, Prop Id 0x%.4x \r\n", *plength, length, prop_ID);
  
  
  /* Application Callback */ 
  (SensorAppli_cb.Sensor_Descriptor_cb)(pSensorDescriptor_param,plength,prop_ID,length); 
  /*
  
  
  if (prop_ID == 0x0000)
  {
  TRACE_M(TF_SENSOR_M, "Property ID Prohibited \r\n");
}
  else  
  {
  (SensorAppli_cb.Sensor_Descriptor_cb)(pSensorDescriptor_param,plength,prop_ID,length);  
}
  */
  return MOBLE_RESULT_SUCCESS;
}


/**
* @brief  This function is called for both Acknowledged and unacknowledged message
*         //
* @param  pSensorData_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message.
* @param  pData: Pointer of data (parameters) coming in packet.
* @param  length: lenth of the data in packet.
* @retval MOBLE_RESULT
*/ 

MOBLE_RESULT Sensor_Column_Status(MOBLEUINT8* pSensorColumn_param, MOBLEUINT32* plength,
                                  MOBLEUINT8 const *pData, MOBLEUINT32 length)
{
  MOBLEUINT16 prop_ID = 0x0000;
  //  MOBLEUINT16 rawvaluex=0x0000;
  MOBLEUINT8 getBuff[8];
  
  TRACE_M(TF_SENSOR_M,"Sensor_Column_Status received \r\n");
  
  
  if(length > 0){
    
    prop_ID = pData[1] << 8;
    prop_ID |= pData[0];
    (SensorAppli_cb.Sensor_Column_cb)(getBuff,plength,prop_ID,length);
  }
  
  
  // (SensorAppli_cb.Sensor_Column_cb)(getBuff,plength,prop_ID,length);
  
  *pSensorColumn_param = pData[0];
  *(pSensorColumn_param+1) = pData[1];
  
  if(*plength == 4 )
  {
    *pSensorColumn_param = pData[0];
    *(pSensorColumn_param+1) = pData[1];
    
    *(pSensorColumn_param+2)=pData[2];
    *(pSensorColumn_param+3)=pData[3];
    //memcpy(pSensorColumn_param , getBuff,2);
    
    *plength = 4;
  }
  else 
  {
    if( pData[2] == getBuff[2]  && pData[3] == getBuff[3])
    {
      memcpy(pSensorColumn_param , getBuff,8);
    }
    else
    {
      
    }
    
  }
  
  TRACE_M(TF_SENSOR_M,"plength Value: %ld\r\n",*plength);
  return MOBLE_RESULT_SUCCESS;
}

/**
       
* @brief  This function is called for both Acknowledged and unacknowledged message
*         
* @param  pSensorData_param: Pointer to the status message, which needs to be updated
* @param  plength: Pointer to the Length of the Status message.
* @param  pData: Pointer of data (parameters) coming in packet.
* @param  length: lenth of the data in packet.
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT  Sensor_Series_Status(MOBLEUINT8* pSensorSeries_param, 
                                   MOBLEUINT32* plength,
                                   MOBLEUINT8 const *pData, 
                                   MOBLEUINT32 length)
{
  
  
  MOBLEUINT8 getBuff[2+6*SENSOR_SERIES_VALUE];
  
  TRACE_M(TF_SENSOR_M,"Sensor_Series_Get callback received \r\n");
  
  MOBLEUINT16 prop_ID=0x0000;
  MOBLEUINT16 RawValueX1=0x0000;
  MOBLEUINT16 RawValueX2=0x0000;
  MOBLEUINT8 z =0;
  MOBLEUINT8 i =0;
  
  if(length>0){
    prop_ID = pData[1] << 8;
    prop_ID |= pData[0];
    if(length>2)
    {
      RawValueX1 = pData[2];
      RawValueX1 |=pData[3]<< 8;
      
      RawValueX2 = pData[4];
      RawValueX2|=pData[5] << 8;
      
    }
  }
  for(i=0;i<NUMBER_OF_SENSOR;i++)
  {  
    if(Property_ID_TableSet[i] ==  prop_ID)
    { 
      (SensorAppli_cb.Sensor_Series_cb)(getBuff,plength,prop_ID,length);
      break;
    }
  }
  
  if(i<NUMBER_OF_SENSOR)
  {
    if(length ==2){
    memcpy(pSensorSeries_param,getBuff,*plength);
    }
    else if(length == 6){
    
      for(int y=0;y<SENSOR_SERIES_VALUE;y++)
      {
      MOBLEUINT16 RawpDataX1 = getBuff[2+6*y]<<8 | getBuff[3+6*y];
      if( RawpDataX1 <= RawValueX1    && RawpDataX1 >=RawValueX2)
      {
        
        *(pSensorSeries_param+2+6*z) = getBuff[2+6*y];
        *(pSensorSeries_param+3+6*z) = getBuff[3+6*y];
        *(pSensorSeries_param+4+6*z) = getBuff[4+6*y];
        *(pSensorSeries_param+5+6*z) = getBuff[5+6*y];
        *(pSensorSeries_param+6+6*z) = getBuff[6+6*y];
        *(pSensorSeries_param+7+6*z) = getBuff[7+6*y];
        z=z+1;
        
      }
    
    }
    
  }
  }
  else if( prop_ID != 0x0000)
  {
    *pSensorSeries_param=prop_ID>>8;
    *(pSensorSeries_param+1) = prop_ID;
    *plength=2;
  }
  else
  {
    //*plength =2;
  }
  
  TRACE_M(TF_SENSOR_M,"plength Value: %ld",*plength);
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
  MOBLEUINT16 prop_ID = 0x0000; 
  
  TRACE_M(TF_SENSOR_M,"Sensor_Cadence_Set callback received \r\n");
  
  if(length > 0)  
  {
    prop_ID = pCadence_param[1] << 8;
    prop_ID |= pCadence_param[0]; 
  }
  
for(int i=0;i<NUMBER_OF_SENSOR;i++)
  {
  
  if(prop_ID == Property_ID_TableSet[i])
  {
    Sensor_CadenceParam[i].Property_ID = pCadence_param[1] << 8;
    Sensor_CadenceParam[i].Property_ID |= pCadence_param[0];
    Sensor_CadenceParam[i].FastCadenceDevisor = (pCadence_param[2] && 0xFE) ;
    Sensor_CadenceParam[i].StatusTriggerType = (pCadence_param[2] && 0x01) ;
    Sensor_CadenceParam[i].triggerDeltaDown = pCadence_param[3];
    Sensor_CadenceParam[i].triggerDeltaUp = pCadence_param[4];
    Sensor_CadenceParam[i].StatusMinInterval = pCadence_param[5];
    Sensor_CadenceParam[i].FastCadenceLow = pCadence_param[6];
    Sensor_CadenceParam[i].FastCadenceHigh = pCadence_param[7];
  }
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
  MOBLEUINT16 propery_ID = 0x00;
  TRACE_M(TF_SENSOR_M,"Sensor_Cadence_Get callback received \r\n");
  
  MOBLEUINT8 flag = 0;
//  MOBLEUINT8 Sensor_GetBuff[8];
  // MOBLEUINT16 propery_ID;
  
  propery_ID = pData[1] << 8;
  propery_ID |= pData[0];   
  
  // (SensorAppli_cb.Sensor_Cadence_Get_cb)(Sensor_GetBuff,propery_ID,length);
  
  for (int i=0 ;i< NUMBER_OF_SENSOR ; i++){  
    
    if(propery_ID == Sensor_CadenceParam[i].Property_ID)
  {
      flag =1;
      *(pCadencestatus_param) = Sensor_CadenceParam[i].Property_ID;
      *(pCadencestatus_param+1) = Sensor_CadenceParam[i].Property_ID >> 8;
      //  *(pCadencestatus_param+2) = Sensor_CadenceParam[0].FastCadenceDevisor;
      *(pCadencestatus_param+2) = (Sensor_CadenceParam[i].FastCadenceDevisor << 0x07)|(Sensor_CadenceParam[i].StatusTriggerType & 0x01);
      *(pCadencestatus_param+3) = Sensor_CadenceParam[i].triggerDeltaDown;
      *(pCadencestatus_param+4) = Sensor_CadenceParam[i].triggerDeltaUp;
      *(pCadencestatus_param+5) = Sensor_CadenceParam[i].StatusMinInterval;
        *(pCadencestatus_param+6) = Sensor_CadenceParam[i].FastCadenceLow;
         *(pCadencestatus_param+7) = Sensor_CadenceParam[i].FastCadenceHigh;
      //    memcpy(&pCadencestatus_param[6],(void*)&Sensor_CadenceParam[0].FastCadenceLow,4);
   //   memcpy(&pCadencestatus_param[10],(MOBLEUINT8*)&Sensor_CadenceParam[i].FastCadenceHigh,4);
   //   memcpy(&pCadencestatus_param[10],(void*)&Sensor_CadenceParam[i].FastCadenceHigh,4);
      *plength = 8;
    
  
    }}
  
  if(flag == 0)
  {
    * pCadencestatus_param = propery_ID;
    *(pCadencestatus_param+1) = propery_ID >> 8;
    *plength=2;
  }
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
  
  TRACE_M(TF_SENSOR_M,"Sensor_Setting_Set callback received \r\n");
  MOBLEUINT8 flag=0;
  
  
  MOBLEUINT16 prop_ID = 0x0000; 
  
  if(length > 0)  
  {
    prop_ID = pSetting_param[1] << 8;
    prop_ID |= pSetting_param[0]; 
  }
  
  for(int i=0 ; i< NUMBER_OF_SENSOR ;i++)
  {
    
    if(prop_ID == Property_ID_TableSet[i])
    {
      Sensor_SettingParameter.Property_ID = pSetting_param[1]<<8 ;
      Sensor_SettingParameter.Property_ID|= pSetting_param[0];
      Sensor_SettingParameter.Sensor_Setting_ID = pSetting_param[3] << 8; 
      Sensor_SettingParameter.Sensor_Setting_ID |= pSetting_param[2];
      Sensor_SettingParameter.Sensor_Setting_Value = pSetting_param[5] << 8;
      Sensor_SettingParameter.Sensor_Setting_Value |= pSetting_param[4];
      
  
      Sensor_SettingParam[i].Property_ID = pSetting_param[1] << 8;
      Sensor_SettingParam[i].Property_ID |= pSetting_param[0];
      Sensor_SettingParam[i].Sensor_Setting_ID = pSetting_param[3] << 8; 
      Sensor_SettingParam[i].Sensor_Setting_ID |= pSetting_param[2];
      Sensor_SettingParam[i].Sensor_Setting_Value = pSetting_param[5] << 8;
      Sensor_SettingParam[i].Sensor_Setting_Value |= pSetting_param[4];
      flag=1;
      break;
    }
  
  }
  /* Application Callback */ 
  if(flag==1)
  {
    (SensorAppli_cb.Sensor_Setting_Set_cb)(&Sensor_SettingParameter,0,prop_ID );
  }
  
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
MOBLE_RESULT Sensor_Setting_Status_PID(MOBLEUINT8* pSetting_param, 
                                       MOBLEUINT32 *plength,
                                       const MOBLEUINT8 *pData,
                                       MOBLEUINT32 length)
{
  
  MOBLEUINT16 propery_ID = 0x0000;
  MOBLEUINT16 sensor_setting_propertID = 0x0000;
  
  
  MOBLEUINT8 flag =1;
  TRACE_M(TF_SENSOR_M,"Sensor_Setting_Status callback received \r\n");
  
  // (Appli_Sensor_GetStatus_cb.GetSettingStatus_cb)(Sensor_GetBuff);
  
  propery_ID = pData[1] << 8;
  propery_ID |= pData[0];
  
  sensor_setting_propertID=pData[3]<<8;
  sensor_setting_propertID=pData[2];
  
  // result = Check_Property_ID(Property_ID_Table , propery_ID);
  
  
  //  (Appli_Sensor_GetStatus_cb.GetSettingStatus_cb)(Sensor_GetBuff);
  
  for(int i=0;i<NUMBER_OF_SENSOR;i++)  
  
  {
    
    if(propery_ID == Sensor_SettingParam[i].Property_ID) 
    {
      flag=0;
      *pSetting_param = Sensor_SettingParam[i].Property_ID;
      *(pSetting_param+1) = Sensor_SettingParam[i].Property_ID >> 8;
      
      /* Comapre received setting property Id with existing value */
      if(sensor_setting_propertID == Sensor_SettingParam[i].Sensor_Setting_ID)
      {
        *(pSetting_param+2) = Sensor_SettingParam[i].Sensor_Setting_ID;
        *(pSetting_param+3) = Sensor_SettingParam[i].Sensor_Setting_ID >> 8;
    
    *plength = 4;
  }
      else /* setting property id not matched */
      {
        *plength = 2;
      }
      break;
    }
  }
  
  /* unknown property Id */
  if(flag ==1){
    *pSetting_param = propery_ID>>8;
    *(pSetting_param+1) = propery_ID;
    *plength = 2 ;
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
  MOBLEUINT8 sensor_GetBuff[7];
  MOBLEUINT16 propery_ID= 0x00;
  MOBLEUINT16 sensor_setting_propertID=0x00;
  MOBLEUINT8 flag=0;
  
  TRACE_M(TF_SENSOR_M,"Sensor_Setting_Status with setting id callback received \r\n");
  
  if(length >= 4){
  
    propery_ID = pData[1] << 8;
    propery_ID |= pData[0] ;
  
    sensor_setting_propertID = pData[3]<<8;
    sensor_setting_propertID |= pData[2];
  
  
    for(int i=0; i< NUMBER_OF_SENSOR ; i++){
  
      if(propery_ID == Sensor_SettingParam[i].Property_ID)
      { 
        if(sensor_setting_propertID == Sensor_SettingParam[i].Sensor_Setting_ID)
  {
          flag=1; 
          (Appli_Sensor_GetStatus_cb.GetSetting_IDStatus_cb)(sensor_GetBuff , propery_ID);
        }
        else
        {
          *pSetting_param =  Sensor_SettingParam[i].Property_ID >>8 ;
          *(pSetting_param+1) =Sensor_SettingParam[i].Property_ID ;
          *(pSetting_param+2) = Sensor_SettingParam[i].Sensor_Setting_ID>>8;
          *(pSetting_param+3) = Sensor_SettingParam[i].Sensor_Setting_ID ;
          *plength=4;
        } 
        break;
      }
      
    } 
    
  }
  
  if(flag==1)
  {
    if(sensor_GetBuff[4] == 0x03){
      memcpy(pSetting_param , sensor_GetBuff,7);
    *plength = 7;
  }
    else if(sensor_GetBuff[4] == 0x01)
    {
      //memcpy(pSetting_param , sensor_GetBuff,5);
      *pSetting_param =  sensor_GetBuff[0];
      *(pSetting_param+1) =sensor_GetBuff[1];
      *(pSetting_param+2) = sensor_GetBuff[2];
      *(pSetting_param+3) = sensor_GetBuff[3];
      *(pSetting_param+4)= sensor_GetBuff[4];
  
      *plength=5;
    }
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


#endif

/**
* @brief  SensorModelServer_GetOpcodeTableCb: This function is call-back 
*         from the library to send Model Opcode Table info to library
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
MOBLE_RESULT SensorModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                                  MOBLE_ADDRESS dst_peer, 
                                                  MOBLEUINT16 opcode, 
                                                  MOBLEUINT8 *pResponsedata, 
                                                  MOBLEUINT32 *plength, 
                                                  MOBLEUINT8 const *pRxData,
                                                  MOBLEUINT32 dataLength,
                                                  MOBLEBOOL response)
{
  MOBLEUINT16 property_ID = 0;
  property_ID = pRxData[1] << 8;
  property_ID |= pRxData[0];
  switch(opcode)
  {
#ifdef ENABLE_SENSOR_MODEL_SERVER
  case SENSOR_DESCRIPTOR_STATUS:
    {
      Sensor_Descriptor_Status(pResponsedata ,plength,pRxData,dataLength);
      break;
    }
  case SENSOR_STATUS:
    {
      Sensor_Data_Status(pResponsedata ,plength,pRxData,dataLength);
      break;
    }
    
  case SENSOR_COLUMN_STATUS:
    {
      if((dataLength > 0) && (property_ID == 0x00))
      {             
        return MOBLE_RESULT_FALSE;
      }   
    
      Sensor_Column_Status(pResponsedata,plength,pRxData,dataLength);
      break;
    }
  case SENSOR_SERIES_STATUS:
    {
       if((dataLength > 0) && (property_ID == 0x00))
      {             
        return MOBLE_RESULT_FALSE;
      }  
      Sensor_Series_Status(pResponsedata,plength,pRxData,dataLength); 
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
      Sensor_Setting_Status_SettingID(pResponsedata,plength,pRxData,dataLength);
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
*         the library whenever a sensor Model message is received
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
  
  TRACE_M(TF_SENSOR_M,"dst_peer = %.2X , peer_add = %.2X \r\n ",dst_peer, peer_addr);
  switch(opcode)
  {
#ifdef ENABLE_SENSOR_MODEL_SERVER
  case SENSOR_DESCRIPTOR_GET:
    {
      if((dataLength > 0) && (property_ID == 0x00))
      {             
        return MOBLE_RESULT_FALSE;
      }   
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
  case SENSOR_CADENCE_GET:
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
    
  case SENSOR_COLUMN_GET:
    {
      
      if(property_ID == 0x00)
      {
        result = MOBLE_RESULT_FALSE;
      }
      break;
      
    }
     case SENSOR_SERIES_GET:
    {
      
      if(property_ID == 0x00)
      {
        result = MOBLE_RESULT_FALSE;
      }
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
      MOBLEUINT8 index;
      
      TRACE_M(TF_SENSOR,"Sensor Descritpor Status:\r\n");
      for(index = 0; index < dataLength; index++)
      {
        TRACE_M(TF_SENSOR,"Value: %d\r\n", pRxData[index]);
      }
      break;
    }
  case SENSOR_STATUS:
    {
      MOBLEUINT8 index;

      if((dataLength > 0) && (property_ID == 0x00))
      {
        return MOBLE_RESULT_FALSE;
      }
      else
      {
        TRACE_M(TF_SENSOR,"Sensor Status:\r\n");
        for(index = 0; index < dataLength; index++)
        {
          TRACE_M(TF_SENSOR,"Value: %d\r\n", pRxData[index]);
        }
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
      MOBLEUINT8 index;
      
      TRACE_M(TF_SENSOR,"Sensor Series Status:\r\n");
      for(index = 0; index < dataLength; index++)
      {
        TRACE_M(TF_SENSOR,"Value: %d\r\n", pRxData[index]);
      }
      break;
    }
  case SENSOR_CADENCE_STATUS:
    {
      MOBLEUINT8 index;
      
      TRACE_M(TF_SENSOR,"Sensor Cadence Status:\r\n");
      for(index = 0; index < dataLength; index++)
      {
        TRACE_M(TF_SENSOR,"Value: %d\r\n", pRxData[index]);
      }
      break;
    }
  case SENSOR_SETTING_STATUS_PID:
    {
      MOBLEUINT8 index;
      
      TRACE_M(TF_SENSOR,"Sensor Settings Status:\r\n");
      for(index = 0; index < dataLength; index++)
      {
        TRACE_M(TF_SENSOR,"Value: %d\r\n", pRxData[index]);
      }
      break;
    }
  case SENSOR_SETTING_STATUS_SETTING_ID:
    {
      MOBLEUINT8 index;
      
      TRACE_M(TF_SENSOR,"Sensor Setting Status:\r\n");
      for(index = 0; index < dataLength; index++)
      {
        TRACE_M(TF_SENSOR,"Value: %d\r\n", pRxData[index]);
      }
      break;
    } 
#endif		
  default:
    {
      break;
    }
    
  } /* Switch ends */
  
  
  if((result == MOBLE_RESULT_SUCCESS) && (response == MOBLE_TRUE) && (ADDRESS_IS_UNICAST(dst_peer)))
  {
    Model_SendResponse(peer_addr, dst_peer,opcode,pRxData,dataLength);
  }
  return MOBLE_RESULT_SUCCESS;
}


#if 0
/**
* @brief  floatToInt: This function is used to convert the float to integer 
* @param  in: sensor data value
* @param  out_value: pointer to the structure
* @param dec_prec: decimal point resolution
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
#endif

/**
Weak function are defined to support the original function if they are not
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
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Cadence_Get(MOBLEUINT8* sensor_DataCadence, 
                                                     MOBLEUINT16 property_ID, MOBLEUINT32 length) );
//WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Setting_Set(Sensor_SettingParam_t* pSensor_SettingParam,
//                                                                       MOBLEUINT8 OptionalValid,MOBLEUINT16 prop_ID));

WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_Setting_Set(Sensor_SettingParam_t* pSensor_SettingParam,
                                                     MOBLEUINT8 OptionalValid)); //,MOBLEUINT16 prop_ID));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_GetSettingStatus(MOBLEUINT8* pSetting_Status));
//WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_GetSetting_IDStatus(MOBLEUINT8* pSetting_Status, MOBLEUINT16 prop_ID, MOBLEUINT16 Sensor_PropertyID , MOBLEUINT8* plength));
WEAK_FUNCTION (MOBLE_RESULT Appli_Sensor_GetSetting_IDStatus(MOBLEUINT8* pSetting_Status));
/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

