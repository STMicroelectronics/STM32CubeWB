/**
******************************************************************************
* @file    sensors_client.h
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
#ifndef __SENSORS_CLIENT_H
#define __SENSORS_CLIENT_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"


/* Exported macro ------------------------------------------------------------*/
/* Variable-------------------------------------------------------------------*/
#pragma pack(1)
typedef union  
{
  sensor_CadenceCbParam_t sSensor_CadenceParam;
  MOBLEUINT8 a_Cadence_param[sizeof(sensor_CadenceCbParam_t)]; 
} _Sensor_CadenceParam;

typedef union  
{
  sensor_SettingCbParams_t sSensor_SettingParam;
  MOBLEUINT8 a_Setting_param[sizeof(sensor_SettingCbParams_t)]; 
} _Sensor_SettingParam;

/**
*  structure of sensor server function pointers callback
*/
typedef struct
{
  void (*Sensor_Descriptor_Status_cb)(const MOBLEUINT8 *pDescriptor,
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex);
  void (*Sensor_Cadence_Status_cb)(const MOBLEUINT8 *pCadence,
                                   MOBLEUINT32 length,
                                   MOBLE_ADDRESS dstPeer,
                                   MOBLEUINT8 elementIndex);
  void (*Sensor_Settings_Status_cb)(const MOBLEUINT8 *pSettings,
                                    MOBLEUINT32 length,
                                    MOBLE_ADDRESS dstPeer,
                                    MOBLEUINT8 elementIndex);
  void (*Sensor_Setting_Status_cb)(const MOBLEUINT8 *pSetting,
                                   MOBLEUINT32 length,
                                   MOBLE_ADDRESS dstPeer,
                                   MOBLEUINT8 elementIndex);
  void (*Sensor_Status_cb)(const MOBLEUINT8 *pStatus,
                           MOBLEUINT32 length,
                           MOBLE_ADDRESS dstPeer,
                           MOBLEUINT8 elementIndex);
  void (*Sensor_Column_Status_cb)(const MOBLEUINT8 *pColumn,
                                  MOBLEUINT32 length,
                                  MOBLE_ADDRESS dstPeer,
                                  MOBLEUINT8 elementIndex);
  void (*Sensor_Series_Status_cb)(const MOBLEUINT8 *pSeries,
                                  MOBLEUINT32 length,
                                  MOBLE_ADDRESS dstPeer,
                                  MOBLEUINT8 elementIndex);  
} sensor_client_cb_t;

extern const sensor_client_cb_t SensorAppli_cb;

/******************************************************************************/
/********** Following Section defines the Opcodes for the Messages ************/
/******************************************************************************/


/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT SensorsModelClient_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                 MOBLEUINT16 *length);

MOBLE_RESULT SensorsModelClient_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParam,
                                                   MOBLEUINT16 opcode, 
                                                   MOBLEUINT8 *pResponsedata, 
                                                   MOBLEUINT32 *plength, 
                                                   MOBLEUINT8 const *pRxData,
                                                   MOBLEUINT32 dataLength,
                                                   MOBLEBOOL response);

MOBLE_RESULT SensorsModelClient_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParam,
                                                 MOBLEUINT16 opcode, 
                                                 MOBLEUINT8 const *pRxData, 
                                                 MOBLEUINT32 dataLength, 
                                                 MOBLEBOOL response);

MOBLE_RESULT SensorsClient_Descriptor_Get(MOBLEUINT8 elementIndex, 
                                          MOBLEUINT8 * ppropertyId);

MOBLE_RESULT SensorsClient_Cadence_Get(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 * ppropertyId);

MOBLE_RESULT SensorsClient_Cadence_Set(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 *pCadence_param, 
                                       MOBLEUINT32 length);

MOBLE_RESULT SensorsClient_Cadence_Set_Unack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pCadence_param, 
                                             MOBLEUINT32 length); 

MOBLE_RESULT SensorsClient_Settings_Get(MOBLEUINT8 elementIndex, 
                                        MOBLEUINT8 * ppropertyId);

MOBLE_RESULT SensorsClient_Setting_Get(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 * pparam,
                                       MOBLEUINT8 length);

MOBLE_RESULT SensorsClient_Setting_Set(MOBLEUINT8 elementIndex, 
                                       MOBLEUINT8 *pSetting_param, 
                                       MOBLEUINT32 length); 

MOBLE_RESULT SensorsClient_Setting_Set_Unack(MOBLEUINT8 elementIndex, 
                                             MOBLEUINT8 *pSetting_param, 
                                             MOBLEUINT32 length); 

MOBLE_RESULT SensorsClient_Get(MOBLEUINT8 elementIndex, 
                               MOBLEUINT8 * ppropertyId);

MOBLE_RESULT SensorsClient_Column_Get(MOBLEUINT8 elementIndex, 
                               MOBLEUINT8 * pparam,
                               MOBLEUINT8 length);

MOBLE_RESULT SensorsClient_Series_Get(MOBLEUINT8 elementIndex, 
                               MOBLEUINT8 * pparam,
                               MOBLEUINT8 length);

#endif /* __SENSORS_CLIENT */



