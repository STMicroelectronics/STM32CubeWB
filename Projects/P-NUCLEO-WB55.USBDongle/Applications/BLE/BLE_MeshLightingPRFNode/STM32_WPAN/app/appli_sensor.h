/**
  ******************************************************************************
  * @file    appli_sensor.h
  * @author  BLE Mesh Team
  * @brief   Application interface for Light Mesh Model
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
#ifndef __APPLI_SENSOR_H
#define __APPLI_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "sensors.h"
#include "mesh_cfg.h"

/* Exported functions ------------------------------------------------------- */
void Appli_Sensor_CadenceGet(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_CadenceSet(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_CadenceSetUnack(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_SettingsGet(sensor_SettingsCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_SettingGet(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_SettingSet(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_SettingSetUnack(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_DescriptorGet(MOBLEUINT8 pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_Get (MOBLEUINT16 prop_ID,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_ColumnGet(sensor_ColumnCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
void Appli_Sensor_SeriesGet(sensor_SeriesCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
MOBLE_RESULT Appli_Sensor_ReadDescriptor(MOBLEUINT8 sensorOffset,
                                         sensor_DescriptorCbParams_t* pDescriptorParams);
MOBLE_RESULT Appli_Sensor_ReadValue(MOBLEUINT8 sensorOffset,
                                    sensor_ValueCbParams_t* pValueParams);
MOBLE_RESULT Appli_Sensor_ReadColumn(MOBLEUINT8 sensorOffset,
                                   MOBLEUINT8 columnOffset,
                                     sensor_ColumnCbParams_t* pColumnParams);
MOBLE_RESULT Appli_Sensor_ReadSeries(MOBLEUINT8 sensorOffset,
                                     sensor_SeriesCbParams_t* pSeriesParams);
MOBLEUINT8 Appli_Sensor_IsFastCadence(MOBLEUINT8 sensorOffset,
                                      void* pFastCadenceLow, 
                                      void* pFastCadenceHigh);
MOBLEUINT8 Appli_Sensor_IsStatusTrigger(MOBLEUINT8 sensorOffset,
                                        status_trigger_type_e triggerType,
                                        void* pDeltaDown,
                                        void* pDeltaUp);
MOBLE_RESULT Appli_Sensor_Init(void); 
//void Appli_Sensor_SerialCmd(char *rcvdStringBuff, uint16_t rcvdStringSize);
MOBLE_RESULT Appli_Sensor_Update(MOBLEUINT8 sensorOffset, MOBLEUINT32 value);

void Appli_Sensor_Descriptor_Status(const MOBLEUINT8 *pDescriptor,
                                    MOBLEUINT32 length,
                                    MOBLE_ADDRESS dstPeer,
                                    MOBLEUINT8 elementIndex);
void Appli_Sensor_Cadence_Status(const MOBLEUINT8 *pCadence,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex);
void Appli_Sensor_Settings_Status(const MOBLEUINT8 *pSettings,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex);
void Appli_Sensor_Setting_Status(const MOBLEUINT8 *pSetting,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex);
void Appli_Sensor_Status(const MOBLEUINT8 *pStatus,
                         MOBLEUINT32 length,
                         MOBLE_ADDRESS dstPeer,
                         MOBLEUINT8 elementIndex);
void Appli_Sensor_Column_Status(const MOBLEUINT8 *pColumn,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex);
void Appli_Sensor_Series_Status(const MOBLEUINT8 *pSeries,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex);  
MOBLE_RESULT Appli_Sensor_Update(MOBLEUINT8 sensorOffset, 
                                 MOBLEUINT32 value);
void Appli_Sensor_SerialCmd(char *rcvdStringBuff, 
                            uint16_t rcvdStringSize);
#endif /* __APPLI_SENSOR_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

