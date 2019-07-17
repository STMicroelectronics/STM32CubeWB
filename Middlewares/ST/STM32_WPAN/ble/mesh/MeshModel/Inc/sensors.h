/**
******************************************************************************
* @file    sensor.h
* @author  BLE Mesh Team
* @version V1.10.000
* @date    15-Jan-2019
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
#ifndef __SENSOR_H
#define __SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/
/* Sensors Property ID */
#define TEMPERATURE_PID                    0X0071
#define PRESSURE_PID                       0X2A6D

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

/* 
 structure for the Property id for the sensors Present inside the firmware.
*/
typedef struct 
{
    MOBLEUINT16 Property_ID;
    
} MODEL_Property_IDTableParam_t;

/* Sensor Cadence Parameters */
typedef struct 
{
 MOBLEUINT16 Property_ID; 
 MOBLEUINT8 FastCadenceDevisor;
 MOBLEUINT8 StatusTriggerType; 
 MOBLEUINT8 triggerDeltaDown;
 MOBLEUINT8 triggerDeltaUp;
 MOBLEUINT8 StatusMinInterval;
 float FastCadenceLow;
 float FastCadenceHigh;
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
 MOBLEUINT16 RowValueX; 
 MOBLEUINT16 RowValueWidth;
 MOBLEUINT16 RowValueY;
}Sensor_ColumnParam_t;

/** \brief Callback map for application from middle layer .
    this will call the function related to the function pointer in the 
    model_if.c file
   const Appli_Sensor_cb_t SensorAppli_cb = 
  {
    Appli_Sensor_Cadence_Set,
    Appli_Sensor_Data_Status,
    Appli_Sensor_Descriptor_Status ,
    Appli_Sensor_Setting_Set
  };
    
**/ 
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
  MOBLE_RESULT (*Sensor_Descriptor_cb)(MOBLEUINT8*, MOBLEUINT32*);  
  
  /* Pointer to the function Appli_Sensor_Setting_Set used for callback 
     from the middle layer to Application layer
  */
  MOBLE_RESULT (*Sensor_Setting_Set_cb)(Sensor_SettingParam_t*, MOBLEUINT8); 
  
} Appli_Sensor_cb_t;


/* function pointer for application to get the value from application to middle 
   layer file
*/
typedef struct
{ 
  MOBLE_RESULT (*GetSettingStatus_cb)(MOBLEUINT8*);
  
  MOBLE_RESULT (*GetSetting_IDStatus_cb)(MOBLEUINT8*);
  
}Appli_Sensor_GetStatus_cb_t;

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
MOBLE_RESULT Sensor_Descriptor_Status(MOBLEUINT8* pSensorDiscriptor_param, MOBLEUINT32* plength);
MOBLE_RESULT Sensor_Setting_Set(const MOBLEUINT8* pSetting_param, MOBLEUINT32 length);
MOBLE_RESULT Sensor_Setting_Status_PID(MOBLEUINT8* pSetting_param, MOBLEUINT32 *plength, 
                                                 const MOBLEUINT8 *pData,MOBLEUINT32 length);
MOBLE_RESULT Sensor_Setting_Status_SettingID(MOBLEUINT8* pSetting_param, MOBLEUINT32 *plength, 
                                                       const MOBLEUINT8 *pData,MOBLEUINT32 length);
MOBLE_RESULT Sensor_Cadence_Status(MOBLEUINT8* pCadencestatus_param, MOBLEUINT32 *plength,
                                   MOBLEUINT8 const *pData, MOBLEUINT32 length);
   
#endif /* __Sensor_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

