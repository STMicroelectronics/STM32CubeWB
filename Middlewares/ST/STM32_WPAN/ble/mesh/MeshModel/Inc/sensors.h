/**
******************************************************************************
* @file    sensors.h
* @author  BLE Mesh Team
* @brief   Header file for the user application file 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORS_H
#define __SENSORS_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/
/* Sensors Property ID */
#define TEMPERATURE_PID         0x0071
#define PRESSURE_PID            0x2A6D
#define HUMIDITY_PID            0x2A6F
#define TIME_OF_FLIGHT_PID      0X2A7F
#define MAGNETO_METER_PID       0x2AA1
#define ACCELERO_METER_PID      0x2BA1
#define GYROSCOPE_PID           0x2BA2
#define VOLTAGE_PID             0x0005
#define CURRENT_PID             0x0004
#define POWER_FACTOR_PID        0x0072
#define ACTIVE_POWER_PID        0x0073
#define REACTIVE_POWER_PID      0x0074
#define APPARENT_POWER_PID      0x0075
#define ACTIVE_ENERGY_PID       0x0083
#define REACTIVE_ENERGY_PID     0x0084
#define APPARENT_ENERGY_PID     0x0085


/* 7.1 Messages summary Page 300 */
/* Sensor Server Model Opcode */
#define SENSOR_DESCRIPTOR_GET       0x8230U      
#define SENSOR_DESCRIPTOR_STATUS    0X51U
#define SENSOR_GET                  0X8231U
#define SENSOR_STATUS               0X52U
#define SENSOR_COLUMN_GET           0X8232U
#define SENSOR_COLUMN_STATUS        0X53U
#define SENSOR_SERIES_GET           0X8233U
#define SENSOR_SERIES_STATUS        0X54U

/* Sensor Setup Server Model Opcode */
#define SENSOR_CADENCE_GET          0X8234U
#define SENSOR_CADENCE_SET          0X55U
#define SENSOR_CADENCE_SET_UNACK    0X56U
#define SENSOR_CADENCE_STATUS       0X57U
#define SENSOR_SETTING_GET          0X8235U
#define SENSOR_SETTING_STATUS_PID           0X58U
#define SENSOR_SETTING_GET_SETTING_ID   0X8236U
#define SENSOR_SETTING_SET              0X59U
#define SENSOR_SETTING_SET_UNACK        0X5AU
#define SENSOR_SETTING_STATUS_SETTING_ID  0X5BU

/******************************************************************************/
/********** Following Section defines the SIG MODEL IDs            ************/
/******************************************************************************/

#define SENSOR_SERVER_MODEL_ID                0x1100U     /* sensor server  */
#define SENSOR_SETUP_SERVER_MODEL_ID          0x1101U    /* sensor setup server*/
#define SENSOR_CLIENT_MODEL_ID                0x1102U   /* Sensor Client */

/******************************************************************************/
/********** SIG MODEL IDs ends                                     ************/
/******************************************************************************/ 
/***********Publsh Period For the Sensor **************************************/
#define SENSOR_PUBLISH_PERIOD    10000
#define SENSOR_SERIES_VALUE      4
/* 
 structure for the Property id for the sensors Present inside the firmware.
*/
#pragma pack(4)
typedef struct 
{
    MOBLEUINT16 Property_ID;
} MODEL_Property_IDTableParam_t;

#pragma pack(4)
/* Sensor Cadence Parameters */
typedef struct 
{
 MOBLEUINT16 Property_ID;
 MOBLEUINT8 FastCadenceDevisor;
 MOBLEUINT8 StatusTriggerType; 
 MOBLEUINT8 triggerDeltaDown;
 MOBLEUINT8 triggerDeltaUp;
 MOBLEUINT8 StatusMinInterval;
 MOBLEUINT16 FastCadenceLow;
 MOBLEUINT16 FastCadenceHigh;  
}Sensor_CadenceParam_t;

/* Sensor Setting Parameters */
#pragma pack(1)
typedef struct 
{
 MOBLEUINT16 Property_ID; 
 MOBLEUINT16 Sensor_Setting_ID; 
 MOBLEUINT8 Sensor_Setting_Access;
 MOBLEUINT16 Sensor_Setting_Value;
}Sensor_SettingParam_t;

/* Sensor Coloumn Parameters */
#pragma pack(1)
typedef struct 
{
 MOBLEUINT16 Property_ID; 
 MOBLEUINT16 RawValueX; 
 MOBLEUINT16 RawValueWidth;
 MOBLEUINT16 RawValueY;
}Sensor_ColumnParam_t;

/* Sensor Series Parameters*/
typedef struct
{
  MOBLEUINT16 Property_ID;
  MOBLEUINT16 RawValueX1;
  MOBLEUINT16 RawValueX2;
  
}Sensor_SeriesParam_t ;



typedef struct
{
  /* Pointer to the function Appli_Sensor_Cadence_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Sensor_Cadence_Set_cb)(Sensor_CadenceParam_t*, MOBLEUINT16 ,MOBLEUINT32);
  
  /* Pointer to the function Appli_Sensor_Data_Status used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Sensor_Data_cb)(MOBLEUINT8*, MOBLEUINT32*, MOBLEUINT16 , MOBLEUINT32);
  
  /* Pointer to the function Appli_Sensor_Descriptor_Status used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Sensor_Descriptor_cb)(MOBLEUINT8*, MOBLEUINT32* , MOBLEUINT16 , MOBLEUINT32);
  

  
  /* Pointer to the function Appli_Sensor_Setting_Set used for callback 
     from the middle layer to Application layer
  */
 // MOBLE_RESULT (*Sensor_Setting_Set_cb)(Sensor_SettingParam_t*, MOBLEUINT8,MOBLEUINT16); 
  MOBLE_RESULT (*Sensor_Setting_Set_cb)(Sensor_SettingParam_t*,MOBLEUINT8,MOBLEUINT16);
  
  
    
  MOBLE_RESULT (*Sensor_Column_cb)(MOBLEUINT8*,MOBLEUINT32*,MOBLEUINT16,MOBLEUINT32);
  
  MOBLE_RESULT (*Sensor_Series_cb)(MOBLEUINT8*,MOBLEUINT32*,MOBLEUINT16,MOBLEUINT32);
   
  
} Appli_Sensor_cb_t;


/* function pointer for application to get the value from application to middle 
   layer file
*/

typedef struct
{ 
//  MOBLE_RESULT (*GetSettingStatus_cb)(MOBLEUINT8*);
  
  MOBLE_RESULT (*GetSetting_IDStatus_cb)(MOBLEUINT8*  , MOBLEUINT16);
 // MOBLE_RESULT (*GetSetting_IDStatus_cb)(MOBLEUINT8* ,);
}Appli_Sensor_GetStatus_cb_t;
#pragma pack(4)

extern const Appli_Sensor_GetStatus_cb_t Appli_Sensor_GetStatus_cb;
extern const Appli_Sensor_cb_t SensorAppli_cb;

/* Exported Functions Prototypes ---------------------------------------------*/

void BLEMesh_SensorModelAppliCb (Appli_Sensor_cb_t* map );
MOBLE_RESULT SensorModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                    MOBLEUINT16 *length);
MOBLE_RESULT SensorModelServer_ProcessMessageCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 const *pData, 
                                    MOBLEUINT32 length, 
                                    MOBLEBOOL response
                                    );
MOBLE_RESULT SensorModelServer_GetStatusRequestCb(MOBLE_ADDRESS peer_addr, 
                                    MOBLE_ADDRESS dst_peer, 
                                    MOBLEUINT16 opcode, 
                                    MOBLEUINT8 *pResponsedata, 
                                    MOBLEUINT32 *plength, 
                                    MOBLEUINT8 const *pData,
                                    MOBLEUINT32 length,
                                    MOBLEBOOL response);

MOBLE_RESULT Sensor_Cadence_Set(const MOBLEUINT8* pCadence_param, MOBLEUINT32 length);
MOBLE_RESULT Sensor_Data_Status(MOBLEUINT8* pSensorData_param, MOBLEUINT32* plength ,
                                                      MOBLEUINT8 const *pData, MOBLEUINT32 length);
MOBLE_RESULT Sensor_Descriptor_Status(MOBLEUINT8* pSensorDiscriptor_param, MOBLEUINT32* plength,MOBLEUINT8 const *pData, MOBLEUINT32 length);
MOBLE_RESULT Sensor_Setting_Set(const MOBLEUINT8* pSetting_param, MOBLEUINT32 length);
MOBLE_RESULT Sensor_Setting_Status_PID(MOBLEUINT8* pSetting_param, MOBLEUINT32 *plength, 
                                                 const MOBLEUINT8 *pData,MOBLEUINT32 length);
MOBLE_RESULT Sensor_Setting_Status_SettingID(MOBLEUINT8* pSetting_param, MOBLEUINT32 *plength, 
                                                       const MOBLEUINT8 *pData,MOBLEUINT32 length);
   
MOBLE_RESULT Check_Property_ID(const MODEL_Property_IDTableParam_t prop_ID_Table[] 
                                                         , MOBLEUINT16 prop_ID);
MOBLE_RESULT Appli_Sensor_Column_Status(MOBLEUINT8* sensor_Column , MOBLEUINT32* pLength,MOBLEUINT16 prop_ID , MOBLEUINT32 length);

#endif /* __SENSORS_H */

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

