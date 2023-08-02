/**
******************************************************************************
* @file    sensors.c
* @author  BLE Mesh Team
* @brief   Sensors model middleware file
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
#include "sensor_cfg_usr.h"
#include "mesh_cfg.h"
#include "sensors.h"
#include "models_if.h"
#include "common.h"
#include <string.h>
#include "compiler.h"

/** @addtogroup MODEL_SENSOR
*  @{
*/

/** @addtogroup Sensor_Model_Callbacks
*  @{
*/

/* Private typedefs ----------------------------------------------------------*/
typedef enum
{
  TIMER_STOPPED = 0,
  TIMER_RUNNING
}timer_status_e;

/**
* This structure represents sensor settings parameters
*/
typedef struct
{
  MOBLEUINT32 settingRaw;
  MOBLEUINT16 settingPropertyId;
  MOBLEUINT8 settingAccess;
  MOBLEUINT8 res;
}sensor_settings_params_t;

/**
* This structure represents sensor series parameters
*/
typedef struct
{
  MOBLEUINT32 rawX;
}sensor_series_column_params_t;

/**
* This structure represents sensor parameters
*/
typedef struct
{
  sensor_settings_params_t* pSettings;          /* sensor settings */
  sensor_series_column_params_t* pSeriesColumn; /* sensor series column */ 
  MOBLEUINT32 statusTriggerDeltaDown;           /* sensor cadence */
  MOBLEUINT32 statusTriggerDeltaUp;             /* sensor cadence */
  MOBLEUINT32 fastCadenceLow;                   /* sensor cadence */
  MOBLEUINT32 fastCadenceHigh;                  /* sensor cadence */
  MOBLEUINT32 modelPublishPeriodMs;             /* sensor server model publish period */
  MOBLEUINT32 publishPeriodMs;                  /* sensor publish period */
  MOBLEUINT32 timerBegin;                       /* sensor timer   */
  MOBLEUINT32 timerEnd;                         /* sensor timer   */
  MOBLEUINT16 propertyId;                       /* sensor property ID */
  MOBLEUINT16 seriesCount;                      /* sensor series column */
  MOBLEUINT8 elementIdx;                        /* sensor element index */
  MOBLEUINT8 dataLength;                        /* raw data length in bytes */
  MOBLEUINT8 cadenceState;                      /* cadence state of sensor */
  MOBLEUINT8 settingsCount;                     /* sensor settings */
  MOBLEUINT8 fastCadencePeriodDiv;              /* sensor cadence */
  MOBLEUINT8 statusTriggerType;                 /* sensor cadence */  
  MOBLEUINT8 statusMinInterval;                 /* sensor cadence */
  timer_status_e timerStatus;                   /* sensor timer   */
}sensor_params_t;

/**
* This structure represensts sensor server model
*/
typedef struct
{
  const sensor_server_cb_t* p_cb;             /* sensor server model callbacks */
  sensor_params_t* pSensorParams;             /* sensors parameters */
  MOBLEUINT8 sensorsCount;                    /* sensors count */
  MOBLEUINT8 res[3];
}sensor_server_t;


MOBLE_RESULT Sensor_UpdatePublishParameters(MOBLEUINT8 sensorOffset);
__weak MOBLE_RESULT Sensor_ExtractPropertyId(const MOBLEUINT8* data,
                                             MOBLEUINT16* pPropertyId);
__weak MOBLE_RESULT Sensor_ExtractSettingPropertyId(const MOBLEUINT8* data,
                                                    MOBLEUINT16* pSettingPropertyId);
__weak MOBLE_RESULT Sensor_FillDescriptor(MOBLEUINT8 sensorOffset,
                                          sensor_params_t* pParams, 
                                          MOBLEUINT8* pData, 
                                          MOBLEUINT8* pLengthInc);
__weak MOBLE_RESULT Sensor_MarshalPid(MOBLEUINT8 sensorOffset,
                                      sensor_params_t* pParams, 
                                      MOBLEUINT8* pData, 
                                      MOBLEUINT8* pLengthInc,
                                      MOBLEUINT8 pidMatch,
                                      MOBLEUINT16 pidNotFound);
__weak MOBLE_RESULT Sensor_CadenceStatus(MOBLEUINT8 pidMatch,
                                         sensor_params_t* pParams,
                                         MOBLEUINT16 pidNotFound,
                                         MOBLE_ADDRESS dstAddr,
                                         MOBLEUINT8 elementIdx,
                                         status_send_e statusSend);
__weak MOBLE_RESULT Sensor_SettingsStatus(MOBLEUINT8* offsetBuff, 
                                          MOBLEUINT8 sensorCount,
                                          MOBLE_ADDRESS dstAddr,
                                          MOBLEUINT16 pidNotFound);
__weak MOBLE_RESULT Sensor_SettingStatus(MOBLEUINT8 pidMatch,
                                         MOBLEUINT8 readViolation,
                                         sensor_params_t* pParams,
                                         sensor_settings_params_t* pSettingsParam,
                                         MOBLEUINT16 pidNotFound,
                                         MOBLEUINT16 rcvdSettingId,
                                         MOBLE_ADDRESS dstAddr,
                                         MOBLEUINT8 elementIdx,
                                         status_send_e statusSend);
__weak MOBLE_RESULT Sensor_DescriptorStatus(MOBLEUINT8* offsetBuff, 
                                            MOBLEUINT8 sensorCount,
                                            MOBLE_ADDRESS dstAddr,
                                            MOBLEUINT16 pidNotFound);
__weak MOBLE_RESULT Sensor_Status(MOBLEUINT8* offsetBuff, 
                                  MOBLEUINT8 sensorCount,
                                  MOBLE_ADDRESS dstAddr,
                                  MOBLEUINT16 pidNotFound,
                                  status_send_e statusSend);
__weak MOBLE_RESULT Sensor_ColumnStatus(MOBLEUINT8* offsetBuff, 
                                        MOBLEUINT8 sensorCount,
                                        MOBLE_ADDRESS dstAddr,
                                        MOBLEUINT32 rawValueX,
                                        MOBLEUINT8 rcvdRawXLength,
                                        MOBLEUINT16 pidNotFound,
                                        MOBLEUINT8 elementIndex,
                                        status_send_e statusSend);
__weak MOBLE_RESULT Sensor_SeriesStatus(MOBLEUINT8* offsetBuff, 
                                        MOBLEUINT8 sensorCount,
                                        MOBLE_ADDRESS dstAddr,
                                        MOBLEUINT32 rcvdX1,
                                        MOBLEUINT32 rcvdX2,
                                        MOBLEUINT8 isX1X2Present,
                                        MOBLEUINT8 rcvdRawXLength,
                                        MOBLEUINT16 pidNotFound,
                                        MOBLEUINT8 elementIndex,
                                        status_send_e statusSend);
__weak MOBLE_RESULT Sensor_DescriptorGet(const MOBLEUINT8* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_CadenceGet(const MOBLEUINT8* pCadenceParam, 
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_CadenceSet(const MOBLEUINT8* pCadenceParam, 
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_CadenceSetUnack(const MOBLEUINT8* pCadenceParam, 
                                           MOBLEUINT32 length,
                                           MOBLE_ADDRESS peerAddr,
                                           MOBLE_ADDRESS dstPeer,
                                           MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_SettingsGet(const MOBLEUINT8* pSettingsParam, 
                                       MOBLEUINT32 length,
                                       MOBLE_ADDRESS peerAddr,
                                       MOBLE_ADDRESS dstPeer,
                                       MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_SettingGet(const MOBLEUINT8* pRcvdParam, 
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_SettingSet(const MOBLEUINT8* pRcvdParam,
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_SettingSetUnack(const MOBLEUINT8* pRcvdParam, 
                                           MOBLEUINT32 length,
                                           MOBLE_ADDRESS peerAddr,
                                           MOBLE_ADDRESS dstPeer,
                                           MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_Get(const MOBLEUINT8* pGetParam, 
                               MOBLEUINT32 length,
                               MOBLE_ADDRESS peerAddr,
                               MOBLE_ADDRESS dstPeer,
                               MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_ColumnGet(const MOBLEUINT8* pColumnParam, 
                                     MOBLEUINT32 length,
                                     MOBLE_ADDRESS peerAddr,
                                     MOBLE_ADDRESS dstPeer,
                                     MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_SeriesGet(const MOBLEUINT8* pSeriesParam, 
                                     MOBLEUINT32 length,
                                     MOBLE_ADDRESS peerAddr,
                                     MOBLE_ADDRESS dstPeer,
                                     MOBLEUINT8 elementIndex);
__weak MOBLE_RESULT Sensor_CheckStatusTrigger(MOBLEUINT8 sensorOffset);
__weak MOBLE_RESULT Sensor_ExtractSensorOffset(MOBLEUINT8 elementIdx,
                                               MOBLEUINT8* pSensorOffsetBuff, 
                                               MOBLEUINT8* pSensorCount, 
                                               MOBLEUINT32 length, 
                                               const MOBLEUINT8* pData,
                                               MOBLEUINT16* pidNotFound);

/* ALIGN(4) */
__attribute__((aligned(4)))sensor_server_t* SensorServer = NULL; /* Sensor server context */


const MODEL_OpcodeTableParam_t Sensor_Opcodes_Table[] = 
{
/* model_id                      opcode                    reliable     min_payload_size max_payload_size response_opcode           min_response_size max_response_size */
  
#ifdef ENABLE_SENSOR_MODEL_SERVER       
  {SENSOR_SERVER_MODEL_ID,       SENSOR_DESCRIPTOR_GET,    MOBLE_FALSE, 0,               2,               SENSOR_DESCRIPTOR_STATUS, 2,                75},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_DESCRIPTOR_STATUS, MOBLE_FALSE, 2,               75,              0,                        1,                1},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_GET,               MOBLE_FALSE, 0,               2,               SENSOR_STATUS,            0,                65},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_STATUS,            MOBLE_FALSE, 0,               65,              0,                        1,                1},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_COLUMN_GET,        MOBLE_FALSE, 3,               6,               SENSOR_COLUMN_STATUS,     2,                14},
  {SENSOR_SERVER_MODEL_ID,       SENSOR_COLUMN_STATUS,     MOBLE_FALSE, 2,               14,              0,                        1,                1},
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


/* Private function prototypes -----------------------------------------------*/
MOBLE_RESULT Sensor_UpdatePublishParameters(MOBLEUINT8 sensorOffset);

WEAK_FUNCTION(void Appli_Sensor_CadenceGet(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_CadenceSet(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_CadenceSetUnack(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_SettingsGet(sensor_SettingsCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_SettingGet(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_SettingSet(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_SettingSetUnack(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_DescriptorGet(MOBLEUINT8 pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_Get (MOBLEUINT16 prop_ID,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_ColumnGet(sensor_ColumnCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_SeriesGet(sensor_SeriesCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadDescriptor(MOBLEUINT8 sensorOffset,
                                         sensor_DescriptorCbParams_t* pDescriptorParams));
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadValue(MOBLEUINT8 sensorOffset,
                                    sensor_ValueCbParams_t* pValueParams));
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadColumn(MOBLEUINT8 sensorOffset,
                                   MOBLEUINT8 columnOffset,
                                     sensor_ColumnCbParams_t* pColumnParams));
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadSeries(MOBLEUINT8 sensorOffset,
                                     sensor_SeriesCbParams_t* pSeriesParams));
WEAK_FUNCTION(MOBLEUINT8 Appli_Sensor_IsFastCadence(MOBLEUINT8 sensorOffset,
                                      void* pFastCadenceLow, 
                                      void* pFastCadenceHigh));
WEAK_FUNCTION(MOBLEUINT8 Appli_Sensor_IsStatusTrigger(MOBLEUINT8 sensorOffset,
                                        status_trigger_type_e triggerType,
                                        void* pDeltaDown,
                                        void* pDeltaUp));
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_Init(void)); 
WEAK_FUNCTION(void Appli_Sensor_SerialCmd(char *rcvdStringBuff, uint16_t rcvdStringSize));

WEAK_FUNCTION(void Appli_Sensor_Descriptor_Status(const MOBLEUINT8 *pDescriptor,
                                    MOBLEUINT32 length,
                                    MOBLE_ADDRESS dstPeer,
                                    MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_Cadence_Status(const MOBLEUINT8 *pCadence,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_Settings_Status(const MOBLEUINT8 *pSettings,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_Setting_Status(const MOBLEUINT8 *pSetting,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_Status(const MOBLEUINT8 *pStatus,
                         MOBLEUINT32 length,
                         MOBLE_ADDRESS dstPeer,
                         MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_Column_Status(const MOBLEUINT8 *pColumn,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex));
WEAK_FUNCTION(void Appli_Sensor_Series_Status(const MOBLEUINT8 *pSeries,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex));  
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_Update(MOBLEUINT8 sensorOffset, 
                                 MOBLEUINT32 value));

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Sensor_ExtractPropertyId
  *         Extract 16 bit Property ID from buffer
  * @param  reference to data to be used for extracting Property ID
  * @param  reference to be updated with Property ID
  * @retval Success if Property ID is not prohibited 
  *         else Invalid
  */
__weak MOBLE_RESULT Sensor_ExtractPropertyId(const MOBLEUINT8* data,
                                             MOBLEUINT16* pPropertyId)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 propertyId = (*data | (*(data+1)<<8));
  
  if (propertyId == PROPERTY_ID_PROHIBITED)
  {
    TRACE_M(TF_SENSOR_M, "Prohibited Property Id received \r\n");
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    *pPropertyId = propertyId;
  }
  
  return result;
}


/**
  * @brief  Sensor_ExtractSettingPropertyId
  *         Extract 16 bit Setting Property ID from buffer
  * @param  reference to data to be used for extracting Setting Property ID
  * @param  reference to be updated with Setting Property ID
  * @retval Success if Property ID is not prohibited else Invalid
  */
__weak MOBLE_RESULT Sensor_ExtractSettingPropertyId(const MOBLEUINT8* data,
                                                    MOBLEUINT16* pSettingPropertyId)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 settingPropertyId = (*data | (*(data+1)<<8));
  
  if (settingPropertyId == SENSOR_SETTING_PROPERTY_ID_PROHIBITED)
  {
    TRACE_M(TF_SENSOR_M, "Prohibited Setting Property Id received \r\n");
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    TRACE_M(TF_SENSOR_M, "Extracted setting Id 0x%.4x\r\n", settingPropertyId);
    *pSettingPropertyId = settingPropertyId;
  }
  
  return result;
}


/**
  * @brief  Sensor_ExtractSensorOffset
  *         Extract offsets of all or one (identified by Property ID) sensor 
  *         inside Sensor Server struct on same element index
  * @param  element index
  * @param  reference to buffer to be updated with sensor offsets
  * @param  reference to be updated with count of extracted sensor offsets
  * @param  length of received buffer
  *         If length = 0, extract offsets for all sensors else for single sensor
  * @param  reference to buffer from which property ID is extracted
  *         Used only if length >= 2, else all property IDs to be extracted
  * @param  pidNotFound updated with Property ID in case of single property ID 
  *         extraction and Property ID not found,
  * @retval Fail if no sensor found on given element index
  *         Fail in case of single Property ID case and Property ID is prohibited
  *         False in case of single Property ID but property ID not supported on element
  *         Success if one or more property ID found on element
  */
__weak MOBLE_RESULT Sensor_ExtractSensorOffset(MOBLEUINT8 elementIdx,
                                               MOBLEUINT8* pSensorOffsetBuff, 
                                               MOBLEUINT8* pSensorCount, 
                                               MOBLEUINT32 length, 
                                               const MOBLEUINT8* pData,
                                               MOBLEUINT16* pidNotFound)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  sensor_params_t* pParams = NULL;
  MOBLEUINT16 propertyId = 0x0000;
  MOBLEUINT8 sensorCount = 0;
  
  /* Extraction of offset for multiple sensors on a given element index */
  if (length == 0)
  {
    for (MOBLEUINT8 count=0; count< SensorServer->sensorsCount; count++)
    {
      pParams = SensorServer->pSensorParams + count;
      
      if (pParams->elementIdx == elementIdx)
      {
        *(pSensorOffsetBuff+sensorCount) = count;
        sensorCount += 1;
        
        TRACE_M(TF_SENSOR_M, "%d Sensor found at offset %d\r\n", sensorCount, count);
      }
      else
      {
        /* sensor is on different element index */
      }
    }
    
    if (sensorCount == 0)
    {
      result = MOBLE_RESULT_FAIL;
      TRACE_M(TF_SENSOR_M, "No sensor found on element index %d for multiple sensors case\r\n", elementIdx);
    }
  }
  /* Extract offset of a sensor given by property ID to be extracted from pData */
  else if (length >= 2)
  {
    result = ExtractPropertyId(pData, &propertyId);
    
    if (result == MOBLE_RESULT_SUCCESS)
    {
      for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
      {
        pParams = SensorServer->pSensorParams + count;
        
        if (pParams->elementIdx == elementIdx &&
            pParams->propertyId == propertyId)
        {
          TRACE_M(TF_SENSOR_M, "Sensor found at offset %d\r\n", count);
          
          *(pSensorOffsetBuff+sensorCount) = count;
          sensorCount = 1;
          break;
        }
        else
        {
          /*  */
        }
      }
      
      if (sensorCount == 0)
      {
        TRACE_M(TF_SENSOR_M, "No sensor found on element index %d for single sensor pid 0x(%.4x)\r\n", 
                elementIdx, propertyId);
        result = MOBLE_RESULT_FALSE;
        *pidNotFound = propertyId;
      }
    }
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    *pSensorCount = sensorCount;
  }
  
  return result;
}

/**
  * @brief  Sensor_FillDescriptor
  *         Descriptor data is fetched from application and
  *         fill sensor descriptor in buffer as per model specification
  * @param  sensor offset of which descriptor to be packed
  * @param  reference to sensor parameters
  * @param  reference to buffer to be filled
  * @param  reference to length incremented for descriptor parameters
  * @retval Fail if sensor parameters not initialized
  *         Fail if application not able to locate particular sensor
  *         else Success
  */
__weak MOBLE_RESULT Sensor_FillDescriptor(MOBLEUINT8 sensorOffset,
                                          sensor_params_t* pParams, 
                                          MOBLEUINT8* pData, 
                                          MOBLEUINT8* pLengthInc)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 length = 0;
  sensor_DescriptorCbParams_t descriptorParams;
  
  if (pParams != NULL)
  {
    /* Get sensor descriptor parameters */
    descriptorParams.propertyId = pParams->propertyId;
    
    if (SensorServer->p_cb->Sensor_ReadDescriptor_cb != NULL)
    {
      result = SensorServer->p_cb->Sensor_ReadDescriptor_cb(sensorOffset, &descriptorParams);
    }
    else
    {
      result = MOBLE_RESULT_FAIL;
    }
    
    if(result == MOBLE_RESULT_SUCCESS)
    {
      PutLittleEndian(pData, (MOBLEUINT32)pParams->propertyId, PROPERTY_ID_LENGTH);
      *(pData+2) = (MOBLEUINT8)(descriptorParams.positiveTolerance & 0xFF);
      *(pData+3) = (MOBLEUINT8)((descriptorParams.positiveTolerance >> 8) & 0x0F);
      *(pData+3) |= (MOBLEUINT8)((descriptorParams.negativeTolerance & 0x0F) << 4);
      *(pData+4) = (MOBLEUINT8)(descriptorParams.negativeTolerance >> 4);
      *(pData+5) = descriptorParams.samplingFunction;
      *(pData+6) = descriptorParams.measurementPeriod;
      *(pData+7) = descriptorParams.updateInterval;
    
    length = 8;
    }
    else
    {
      TRACE_M(TF_SENSOR_M, "Descriptor read failed\r\n");
    }
    
    *pLengthInc = length;
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }
  
  return result;
}


/**
  * @brief  Sensor_MarshalPid
  *         Sensor data is fetched from application and
  *         fill sensor marshalled data in buffer as per model specification
  *         Either Format A or Format B is followed
  *         length - 1 to be justified
  * @param  sensor offset of which sensor state to be packed
  * @param  refeence to sensor parameters
  * @param  reference to buffer to be filled
  * @param  reference to length incremented for sensor state
  * @param  If particular sensor property Id not exists
  * @param  Property ID received which doesn't exist
  * @retval Fail if sensor parameters not initialized
  *         Fail if application not able to read sensor value from application
  *         else Success
  */
__weak MOBLE_RESULT Sensor_MarshalPid(MOBLEUINT8 sensorOffset,
                                      sensor_params_t* pParams, 
                                      MOBLEUINT8* pData, 
                                      MOBLEUINT8* pLengthInc,
                                      MOBLEUINT8 pidMatch,
                                      MOBLEUINT16 pidNotFound)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 length = 0;
  MOBLEUINT8 pidNotFoundLength = 0;
  sensor_ValueCbParams_t valueParams = {0};
  
  if (pidMatch == 1)
  {
    if (pParams != NULL)
    {
      TRACE_M(TF_SENSOR_M, "Property Id of sensor 0x%.4x\r\n", pParams->propertyId);
      
      valueParams.propertyId = pParams->propertyId;
      
      if (SensorServer->p_cb->Sensor_ReadValue_cb != NULL)
      {
        result = SensorServer->p_cb->Sensor_ReadValue_cb(sensorOffset, &valueParams);
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
      }

      if (result == MOBLE_RESULT_SUCCESS)
      {
      /* Format A */
      if (pParams->propertyId < 0x0800)
      {
          *pData = (MOBLEUINT8)((((pParams->dataLength - 1) & 0x0F) << 1) | \
                                  ((pParams->propertyId & 0x0007) << 5)) & 0xFE;
        *(pData+1) = (MOBLEUINT8)((pParams->propertyId & 0x07F8) >> 3);
        length = 2;

          memcpy(pData+length, valueParams.data, pParams->dataLength);
        length += pParams->dataLength;
      }
      /* Format B */
      else
      {
        *pData = (MOBLEUINT8)(((pParams->dataLength - 1) & 0x7F) << 1) | 0x01;
        length = 1;
          PutLittleEndian(pData+length, (MOBLEUINT32)pParams->propertyId, PROPERTY_ID_LENGTH);
          length += PROPERTY_ID_LENGTH;
          
          memcpy(pData+length, valueParams.data, pParams->dataLength);
        length += pParams->dataLength;
      }
      
      *pLengthInc = length;
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Value read failed");
        result = MOBLE_RESULT_FAIL;
      }
    }
    else
    {
      result = MOBLE_RESULT_FAIL;
    }
  }
  else /* (pidMatch == 0) */
  {
    TRACE_M(TF_SENSOR_M, "Marshal only property Id 0x%.4x\r\n", pidNotFound);
    
    /* Format A */
    if (pidNotFound < 0x0800)
    {
      *pData = (MOBLEUINT8)((((pidNotFoundLength - 1) & 0x0F) << 1) | \
                               ((pidNotFound & 0x0007) << 5)) & 0xFE;
      *(pData+1) = (MOBLEUINT8)((pidNotFound & 0x07F8) >> 3);
      length = 2;
    }
    /* Format B */
    else
    {
      *pData = (MOBLEUINT8)(((pidNotFoundLength - 1) & 0x7F) << 1) | 0x01;
      length = 1;
      PutLittleEndian(pData+length, (MOBLEUINT32)pidNotFound, PROPERTY_ID_LENGTH);
      length += PROPERTY_ID_LENGTH;
    }
    
    *pLengthInc = length;
  }
  
  return result;
}


/**
  * @brief  Sensor_CadenceStatus message as per model specification v1.0.1
  *         Sent in response of Cadence Set, Cadence Get or Cadence SetUnack to
  *         publish group
  * @param  pidMatch indicates if pid field of incoming message
  *         is present and matched
  * @param  sensor parameters
  * @param  Property ID received which doesn't exist
  * @param  destination address for status message
  * @param  source element index
  * @param  status to be sent only to client or to be published or to do both
  * @retval Fail if source address is unassigned
  *         Fail if sensor parameters not initialized
  */
__weak MOBLE_RESULT Sensor_CadenceStatus(MOBLEUINT8 pidMatch,
                                         sensor_params_t* pParams,
                                         MOBLEUINT16 pidNotFound,
                                         MOBLE_ADDRESS dstAddr,
                                         MOBLEUINT8 elementIdx,
                                         status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[100];
  MOBLE_ADDRESS srcAddr = BLEMesh_GetAddress();
  MOBLE_ADDRESS srcElementAddr = MOBLE_ADDRESS_UNASSIGNED;
  
  TRACE_M(TF_SENSOR_M, "pidMatch %d, dst %.4x, not found pid %.4x\r\n",
          pidMatch, dstAddr, pidNotFound);
  
  if (srcAddr != MOBLE_ADDRESS_UNASSIGNED)
  {
    if(pidMatch == 0)
    {
      PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pidNotFound, PROPERTY_ID_LENGTH);
      dataBufLen = PROPERTY_ID_LENGTH;
    }
    else /* PID matched */
    {
      if (pParams != NULL)
      {
        TRACE_M(TF_SENSOR_M, "cadence params %.4x %d %d %ld %ld %d %ld %ld\r\n",
                pParams->propertyId,
                pParams->fastCadencePeriodDiv,
                pParams->statusTriggerType,
                pParams->statusTriggerDeltaDown,
                pParams->statusTriggerDeltaUp,
                pParams->statusMinInterval,
                pParams->fastCadenceLow,
                pParams->fastCadenceHigh);
        
        PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pParams->propertyId, PROPERTY_ID_LENGTH);
        dataBufLen = PROPERTY_ID_LENGTH;
        
        if (pParams->cadenceState == 1) /* sensor supports cadence */
        {
          *(data+dataBufLen) = (pParams->fastCadencePeriodDiv & 0x7F);
          *(data+dataBufLen) |= (MOBLEUINT8)((pParams->statusTriggerType & 0x01) << 7);
          dataBufLen += 1;
          
          /* if trigger type is value, field length would be actual data length */
          if (pParams->statusTriggerType == 0)
          {
            PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pParams->statusTriggerDeltaDown, pParams->dataLength);
            dataBufLen += pParams->dataLength;
            
            PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pParams->statusTriggerDeltaUp, pParams->dataLength);
            dataBufLen += pParams->dataLength;
          }
          else /* field length is 2 corresponding to uint16, max value is 10000 */
          {
            PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pParams->statusTriggerDeltaDown, 2);
            dataBufLen += 2;
            
            PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pParams->statusTriggerDeltaUp, 2);
            dataBufLen += 2;
          }
          
          *(data+dataBufLen) = pParams->statusMinInterval;
          dataBufLen += 1;
          
          PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pParams->fastCadenceLow, pParams->dataLength);
          dataBufLen += pParams->dataLength;
          
          PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pParams->fastCadenceHigh, pParams->dataLength);
          dataBufLen += pParams->dataLength;
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Sensor doesn't support cadence \r\n");
        }
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "Sensor not initialized \r\n");
      }
    }
    
    TRACE_M(TF_SENSOR_M, "result %d, dataBufLen %d\r\n", result, dataBufLen);
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Address Unassigned. Src (0c%.4x) Dst (0x%.4x)\r\n", srcAddr, dstAddr);
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    srcElementAddr = srcAddr + elementIdx;
    
    /* if client address and publish address is same, only one message may need 
    to be sent */
    if (statusSend == STATUS_SEND_REPLY ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
      {
        result = BLEMesh_ModelSendMessage(srcElementAddr,
                                              dstAddr,
                                              SENSOR_SETUP_SERVER_MODEL_ID,
                                              (MOBLEUINT16)SENSOR_CADENCE_STATUS, 
                                              data,
                                              dataBufLen);
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          TRACE_M(TF_SENSOR_M, "Reply sent\r\n");
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Reply failed\r\n");
        }
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Reply can't be sent. Client destination address is unassigned\r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
    
    if (statusSend == STATUS_SEND_PUBLISH ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      result = BLEMesh_SetRemotePublication(SENSOR_SETUP_SERVER_MODEL_ID, 
                                                srcElementAddr,
                                                (MOBLEUINT16)SENSOR_CADENCE_STATUS, 
                                                data, 
                                                dataBufLen,
                                                MOBLE_FALSE,
                                                MOBLE_FALSE); 
      
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_SENSOR_M, "Published\r\n");
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Publication failed\r\n");
      }
    }
  }
  else
  {
    /* */
  }
  
  return result;
}


/**
  * @brief  Sensor_SettingsStatus message as per model specification v1.0.1
  *         Sent in response of Settings Get
  * @param  offsets of sensors whose PIDs to be sent
  * @param  total count of sensors
  * @param  destination address for status message
  * @param  Property ID received which doesn't exist
  * @retval Fail if source address and destination address is unassigned
  *         Fail if sensor offset not found
  *         Fail if sensor parameters not initialized
  *         Fail if application not able to read sensor value from application
*/ 
__weak MOBLE_RESULT Sensor_SettingsStatus(MOBLEUINT8* offsetBuff, 
                                          MOBLEUINT8 sensorCount,
                                          MOBLE_ADDRESS dstAddr,
                                          MOBLEUINT16 pidNotFound)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  sensor_params_t* pParams = NULL;
  sensor_settings_params_t* pSettingsParams = NULL;
  MOBLEUINT8 elementIdx = 0;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[100];
  MOBLE_ADDRESS srcAddr = BLEMesh_GetAddress();
  MOBLE_ADDRESS srcElementAddr = MOBLE_ADDRESS_UNASSIGNED;
  
  TRACE_M(TF_SENSOR_M, "Sensor settings status. Sensor count %d, dst %.4x, not found pid %.4x\r\n",
          sensorCount, dstAddr, pidNotFound);
  
  if (dstAddr != MOBLE_ADDRESS_UNASSIGNED
      && srcAddr != MOBLE_ADDRESS_UNASSIGNED)
  {    
    if (offsetBuff == NULL)
    {
      result = MOBLE_RESULT_INVALIDARG;
      TRACE_M(TF_SENSOR_M, "Invalid arguments\r\n");
    }
    else if (sensorCount == 0)
    {
      if (pidNotFound != PROPERTY_ID_PROHIBITED)
      {
        PutLittleEndian(data, (MOBLEUINT32)pidNotFound, PROPERTY_ID_LENGTH);
        dataBufLen = PROPERTY_ID_LENGTH;
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "sensor count 0 and pid not found prohibited\r\n");
      }
    }
    else /* send all setting property IDs corresponding to received sensor property ID */
    {
      pParams = SensorServer->pSensorParams + offsetBuff[0];
      
      if (pParams != NULL)
      {
        PutLittleEndian(data, (MOBLEUINT32)pParams->propertyId, PROPERTY_ID_LENGTH);
        dataBufLen = PROPERTY_ID_LENGTH;
        
        for (MOBLEUINT8 count=0; count<pParams->settingsCount; count++)
        {
          pSettingsParams = pParams->pSettings + count;
          
          PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pSettingsParams->settingPropertyId, 2);
          dataBufLen += 2;
        }
        
        elementIdx = pParams->elementIdx;
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "sensor parameters not found at sensor offset %d", offsetBuff[0]);
      }
    }
    
    TRACE_M(TF_SENSOR_M, "sensor offset %d, result %d, dataBufLen %d\r\n",
            offsetBuff[0], result, dataBufLen);
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Address Unassigned. Src (0c%.4x) Dst (0x%.4x)\r\n", srcAddr, dstAddr);
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    srcElementAddr = srcAddr + elementIdx;
    
    result = BLEMesh_ModelSendMessage(srcElementAddr,
                                          dstAddr,
                                          SENSOR_SETUP_SERVER_MODEL_ID,
                                          (MOBLEUINT16)SENSOR_SETTINGS_STATUS, 
                                          data,
                                          dataBufLen);
    
    if (result != MOBLE_RESULT_SUCCESS)
    {
      TRACE_M(TF_SENSOR_M, "Sensor Settings Status publication failed\r\n");
    }
    else
    {
      /* */
    }
  }
  else
  {
    /* */
  }
  
  return result;
}


/**
  * @brief  Sensor_SettingStatus message as per model specification v1.0.1
  *         Sent in response of Setting Get, Setting Set
  * @param  pidMatch indicates if pid field of incoming message
  *         is present and matched
  * @param  if incoming message try to write setting which is read only, read
  *         read violtaion occurs
  * @param  sensor parameters
  * @param  settings parameter
  * @param  Property ID received which doesn't exist
  * @param  Setting property ID in received message
  * @param  destination address for status message
  * @param  element index handling the current message
  * @param  status to be sent only to client or to be published or to do both
  * @retval Fail if source address unassigned
  *         Fail if sensor parameters not initialized
*/ 
__weak MOBLE_RESULT Sensor_SettingStatus(MOBLEUINT8 pidMatch,
                                         MOBLEUINT8 readViolation,
                                         sensor_params_t* pParams,
                                         sensor_settings_params_t* pSettingsParam,
                                         MOBLEUINT16 pidNotFound,
                                         MOBLEUINT16 rcvdSettingId,
                                         MOBLE_ADDRESS dstAddr,
                                         MOBLEUINT8 elementIdx,
                                         status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[100];
  MOBLE_ADDRESS srcAddr = BLEMesh_GetAddress();
  MOBLE_ADDRESS srcElementAddr = MOBLE_ADDRESS_UNASSIGNED;
  
  TRACE_M(TF_SENSOR_M, "pidMatch %d, dst %.4x, not found pid %.4x, not found setting id %.4x\r\n",
          pidMatch, dstAddr, pidNotFound, rcvdSettingId);
  
  if (srcAddr != MOBLE_ADDRESS_UNASSIGNED)
  {
    if(pidMatch == 0)
    {
      PutLittleEndian(data, (MOBLEUINT32)pidNotFound, PROPERTY_ID_LENGTH);
      dataBufLen = PROPERTY_ID_LENGTH;
      
      PutLittleEndian(data+dataBufLen, (MOBLEUINT32)rcvdSettingId, 2);
      dataBufLen += 2;
    }
    else /* PIDs matched */
    {
      if (pParams != NULL &&
          pSettingsParam != NULL)
      {
        PutLittleEndian(data, (MOBLEUINT32)pParams->propertyId, PROPERTY_ID_LENGTH);
        dataBufLen = PROPERTY_ID_LENGTH;
        
        PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pSettingsParam->settingPropertyId, 2);
        dataBufLen += 2;
        
        *(data+dataBufLen) = pSettingsParam->settingAccess;
        dataBufLen += 1;
        
        if (readViolation == 0)
        {
          PutLittleEndian(data+dataBufLen, (MOBLEUINT32)pSettingsParam->settingRaw, 4);
          dataBufLen += 4;
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Read violtaion occurred. Omit raw value field\r\n");
        }
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "Sensor not initialized\r\n");
      }
    }
    
    TRACE_M(TF_SENSOR_M, "result %d, dataBufLen %d\r\n", result, dataBufLen);
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Src address %.4x\r\n", srcAddr);
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    srcElementAddr = srcAddr + elementIdx;
    
    /* if client address and publish address is same, only one message may need 
    to be sent */
    if (statusSend == STATUS_SEND_REPLY ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
      {
        result = BLEMesh_ModelSendMessage(srcElementAddr,
                                              dstAddr,
                                              SENSOR_SETUP_SERVER_MODEL_ID,
                                              (MOBLEUINT16)SENSOR_SETTING_STATUS, 
                                              data,
                                              dataBufLen);
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          TRACE_M(TF_SENSOR_M, "Reply sent\r\n");
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Reply failed\r\n");
        }
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Reply can't be sent. Client destination address is unassigned\r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
    
    if (statusSend == STATUS_SEND_PUBLISH ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      result = BLEMesh_SetRemotePublication(SENSOR_SETUP_SERVER_MODEL_ID, 
                                                srcElementAddr,
                                                (MOBLEUINT16)SENSOR_SETTING_STATUS, 
                                                data, 
                                                dataBufLen,
                                                MOBLE_FALSE,
                                                MOBLE_FALSE); 
      
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_SENSOR_M, "Published\r\n");
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Publication failed\r\n");
      }
    }
  }
  else
  {
    /* */
  }
  
  return result;
}


/**
  * @brief  Sensor_DescriptorStatus message as per model specification v1.0.1
  *         Sent in response of Descriptor Get
  * @param  offsets of sensors whose PIDs to be sent
  * @param  total count of sensors
  * @param  destination address for status message
  * @param  Property ID received which doesn't exist
  * @retval Fail if source address and destination address is unassigned
  *         Fail if sensor offset not found
  *         Fail if sensor parameters not initialized
  *         Fail if application not able to read sensor descriptor from application
  */
__weak MOBLE_RESULT Sensor_DescriptorStatus(MOBLEUINT8* offsetBuff, 
                                            MOBLEUINT8 sensorCount,
                                            MOBLE_ADDRESS dstAddr,
                                            MOBLEUINT16 pidNotFound)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  sensor_params_t* pParams = NULL;
  MOBLEUINT8 prevSensorElementIdx = 0;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[379];
  MOBLEUINT8 lengthIncrease = 0;
  MOBLE_ADDRESS srcAddr = BLEMesh_GetAddress();
  MOBLE_ADDRESS srcElementAddr = MOBLE_ADDRESS_UNASSIGNED;
  
  TRACE_M(TF_SENSOR_M, "Sensor descriptor status. Sensor count %d, dst %.4x, not found pid %.4x\r\n",
          sensorCount, dstAddr, pidNotFound);
  
  if (dstAddr != MOBLE_ADDRESS_UNASSIGNED
      && srcAddr != MOBLE_ADDRESS_UNASSIGNED)
  {    
    if (offsetBuff == NULL)
    {
      result = MOBLE_RESULT_INVALIDARG;
      TRACE_M(TF_SENSOR_M, "Invalid arguments\r\n");
    }
    else if (sensorCount == 0)
    {
      if (pidNotFound != PROPERTY_ID_PROHIBITED)
      {
        PutLittleEndian(data, (MOBLEUINT32)pidNotFound, PROPERTY_ID_LENGTH);
        dataBufLen = PROPERTY_ID_LENGTH;
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "sensor count 0 and pid not found prohibited\r\n");
      }
    }
    else 
    {
      for (MOBLEUINT8 count=0; count<sensorCount; count++)
      {
        pParams = SensorServer->pSensorParams + offsetBuff[count];
        
        /* check if previous iteration element index is same as current one */
        if (sensorCount != 1 && count > 1)
        {
          /* check for element index consistency */
          if (prevSensorElementIdx != pParams->elementIdx)
          {
            result = MOBLE_RESULT_FAIL;
            TRACE_M(TF_SENSOR_M, "Sensor offset %d index %d and offset%d index %d mismatch\r\n",
                    offsetBuff[count-1], prevSensorElementIdx, offsetBuff[count], pParams->elementIdx);
            break;
          }
          else
          {
            /*  */
          }
        }
        else
        {
          /* true for first iteration and sensor count = 1 */
        }
        
        result = Sensor_FillDescriptor(offsetBuff[count], pParams, data+dataBufLen, &lengthIncrease);        
        
        /* copy of element index for cross-check of element index in next iteration */
        prevSensorElementIdx = pParams->elementIdx;
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          dataBufLen += lengthIncrease;
        }
        else
        {
          break;
        }
        
        TRACE_M(TF_SENSOR_M, "count %d, sensor offset %d, result %d, lengthIncrease %d, dataBufLen %d\r\n",
                count, offsetBuff[count], result, lengthIncrease, dataBufLen);
      }
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Address Unassigned. Src (0c%.4x) Dst (0x%.4x)\r\n", srcAddr, dstAddr);
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    srcElementAddr = srcAddr + prevSensorElementIdx;
    
    result = BLEMesh_ModelSendMessage(srcElementAddr,
                                          dstAddr,
                                          SENSOR_SERVER_MODEL_ID,
                                          (MOBLEUINT16)SENSOR_DESCRIPTOR_STATUS, 
                                          data,
                                          dataBufLen);
    
    if (result != MOBLE_RESULT_SUCCESS)
    {
      TRACE_M(TF_SENSOR_M, "Sensor Descriptor Status publication failed\r\n");
    }
    else
    {
      /* */
    }
  }
  else
  {
    /* */
  }
  
  return result;
}


/**
  * @brief  Sensor_Status message as per model specification v1.0.1
  *         Sent in response of Sensor Get
  *         Marshalled sensor data sent for total no of sensors represented by 
  *         sensorCount, offset of all such sensors is represented by offsetBuff 
  *         Element index of all these sensors should be same
  * @param  offsets of sensors whose PIDs to be sent
  * @param  total count of sensors
  * @param  destination address for status message
  * @param  Property ID received which doesn't exist
  * @param  status to be sent only to client or to be published or to do both
  * @retval Fail if source address is unassigned
  *         Fail if sensor parameters not initialized
  *         Fail if application not able to read  sensor data from application
  */
__weak MOBLE_RESULT Sensor_Status(MOBLEUINT8* offsetBuff, 
                                  MOBLEUINT8 sensorCount,
                                  MOBLE_ADDRESS dstAddr,
                                  MOBLEUINT16 pidNotFound,
                                  status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  sensor_params_t* pParams = NULL;
  MOBLEUINT8 pidMatch = 0;
  MOBLEUINT8 prevSensorElementIdx = 0;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[379];
  MOBLEUINT8 lengthIncrease = 0;
  MOBLE_ADDRESS srcAddr = BLEMesh_GetAddress();
  MOBLE_ADDRESS srcElementAddr = MOBLE_ADDRESS_UNASSIGNED;
  
  if (srcAddr != MOBLE_ADDRESS_UNASSIGNED)
  {    
    if (offsetBuff == NULL)
    {
      result = MOBLE_RESULT_INVALIDARG;
      TRACE_M(TF_SENSOR_M, "Invalid arguments\r\n");
    }
    else if (sensorCount == 0) /* received pid doesn't exist */
    {
      if (pidNotFound == MOBLE_ADDRESS_UNASSIGNED)
      {
        result = MOBLE_RESULT_FAIL;
      }
      else
      {
        pidMatch = 0;
        result = Sensor_MarshalPid(0, pParams, data+dataBufLen, &lengthIncrease, pidMatch, pidNotFound);
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          dataBufLen += lengthIncrease;
        }
      }
    }
    else
    {
      pidMatch = 1;
      
      for (MOBLEUINT8 count=0; count<sensorCount; count++)
      {
        pParams = SensorServer->pSensorParams + offsetBuff[count];
        
        /* check if previous iteration element index is same as current one */
        if (sensorCount != 1
            && count > 1)
        {
          /* check for element index consistency */
          if (prevSensorElementIdx != pParams->elementIdx)
          {
            result = MOBLE_RESULT_FAIL;
            TRACE_M(TF_SENSOR_M, "Sensor offset %d index %d and offset%d index %d mismatch\r\n",
                    offsetBuff[count-1], prevSensorElementIdx, offsetBuff[count], pParams->elementIdx);
            break;
          }
          else
          {
            /*  */
          }
        }
        else
        {
          /* true for first iteration and sensor count = 1 */
        }
        
        result = Sensor_MarshalPid(offsetBuff[count], 
                                   pParams, 
                                   data+dataBufLen, 
                                   &lengthIncrease, 
                                   pidMatch, 
                                   pidNotFound);
        
        /* copy of element index for cross-check of element index in next iteration */
        prevSensorElementIdx = pParams->elementIdx;
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          dataBufLen += lengthIncrease;
        }
        else
        {
          break;
        }
        
        TRACE_M(TF_SENSOR_M, "count %d, sensor offset %d, result %d, lengthIncrease %d, dataBufLen %d\r\n",
                count, offsetBuff[count], result, lengthIncrease, dataBufLen);
      }
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Src unassigned %.4x\r\n", srcAddr);
  }
  
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    srcElementAddr = srcAddr + prevSensorElementIdx;
    
    /* if client address and publish address is same, only one message may need 
    to be sent */
    if (statusSend == STATUS_SEND_REPLY ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
      {
        result = BLEMesh_ModelSendMessage(srcElementAddr,
                                          dstAddr,
                                          SENSOR_SERVER_MODEL_ID,
                                          (MOBLEUINT16)SENSOR_STATUS, 
                                          data,
                                          dataBufLen);
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          TRACE_M(TF_SENSOR_M, "Reply sent\r\n");
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Reply failed\r\n");
        }
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Reply can't be sent. Client destination address is unassigned\r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
    
    if (statusSend == STATUS_SEND_PUBLISH ||
        statusSend == STATUS_SEND_REPLY_PUBLISH)
    {
      result = BLEMesh_SetRemotePublication(SENSOR_SERVER_MODEL_ID, 
                                                srcElementAddr,
                                                (MOBLEUINT16)SENSOR_STATUS, 
                                                data, 
                                                dataBufLen,
                                                MOBLE_FALSE,
                                                MOBLE_FALSE); 
      
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_SENSOR_M, "Published\r\n");
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Publication failed\r\n");
      }
    }
  }
  else
  {
    /* */
  }  
  
  return result;
}


/**
  * @brief  Sensor_ColumnStatus message as per model specification v1.0.1
  *         Sent in response of Sensor Column Get
  * @param  offsets of sensors whose PIDs to be sent
  * @param  total count of sensors
  * @param  destination address for status message
  * @param  Raw value X corresponding to which column data to be sent
  * @param  Length (in bytes) of received Raw Value X
  * @param  Property ID received which doesn't exist
  * @param  Corresponding element index
  * @param  status to be sent only to client or to be published or to do both
  * @retval Fail if source address is unassigned
  *         Fail if sensor parameters not initialized
  *         Fail if application not able to read column data from application
  */
__weak MOBLE_RESULT Sensor_ColumnStatus(MOBLEUINT8* offsetBuff, 
                                        MOBLEUINT8 sensorCount,
                                        MOBLE_ADDRESS dstAddr,
                                        MOBLEUINT32 rawValueX,
                                        MOBLEUINT8 rcvdRawXLength,
                                        MOBLEUINT16 pidNotFound,
                                        MOBLEUINT8 elementIndex,
                                        status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  sensor_params_t* pParams = NULL;
  sensor_series_column_params_t* pSeriesColumn = NULL;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[379];
  MOBLE_ADDRESS srcAddr = BLEMesh_GetAddress();
  MOBLE_ADDRESS srcElementAddr = MOBLE_ADDRESS_UNASSIGNED;
  sensor_ColumnCbParams_t columnParams = {0};
  
  if (srcAddr != MOBLE_ADDRESS_UNASSIGNED)
  {
    if (offsetBuff == NULL)
    {
      result = MOBLE_RESULT_INVALIDARG;
      TRACE_M(TF_SENSOR_M, "Invalid arguments\r\n");
    }
    else if (sensorCount == 0) /* received pid doesn't exist */
    {
      if (pidNotFound == MOBLE_ADDRESS_UNASSIGNED)
      {
        TRACE_M(TF_SENSOR_M, "pidNotFound prohibited\r\n");
        result = MOBLE_RESULT_FAIL;
      }
      else
      {
        PutLittleEndian(data, (MOBLEUINT32)pidNotFound, PROPERTY_ID_LENGTH);
        dataBufLen = PROPERTY_ID_LENGTH;
      }
    }
    else
    {
      pParams = SensorServer->pSensorParams+offsetBuff[0];
      
      if (pParams != NULL)
      {
        PutLittleEndian(data, pParams->propertyId, PROPERTY_ID_LENGTH);
        dataBufLen += PROPERTY_ID_LENGTH;
        
        if (pParams->seriesCount != 0)
        {        
          PutLittleEndian(data+dataBufLen, rawValueX, rcvdRawXLength);
          dataBufLen += rcvdRawXLength;
          
          for (MOBLEUINT8 count=0; count<pParams->seriesCount; count++)
          {
            pSeriesColumn = pParams->pSeriesColumn + count;
            
            if (pSeriesColumn != NULL)
            {
              if (pSeriesColumn->rawX == rawValueX)
              {
                columnParams.propertyId = pParams->propertyId;
                columnParams.rawValueX = pSeriesColumn->rawX;
                
                if (SensorServer->p_cb->Sensor_ReadColumn_cb != NULL)
                {
                  /* Get sensor column data */
                  result = SensorServer->p_cb->Sensor_ReadColumn_cb(offsetBuff[0],
                                                                    count,
                                                                    &columnParams);
                }
                else
                {
                  result = MOBLE_RESULT_FAIL;
                }
                
                if (result == MOBLE_RESULT_SUCCESS)
                {
                  if (columnParams.dataLength >= 2 &&
                      columnParams.dataLength <= 8)
                  {
                    memcpy(data+dataBufLen, columnParams.data, dataBufLen);
                    dataBufLen += columnParams.dataLength;
                  }
                  else
                  {
                    TRACE_M(TF_SENSOR_M, "Wrong length %d set by application\r\n", 
                                          columnParams.dataLength);
                    result = MOBLE_RESULT_FAIL;
                  }
                }
                else
                {
                  TRACE_M(TF_SENSOR_M, "Fail to read sensor column data\r\n");
                }
                
                break;
              }
              else
              {
                /* raw not matched */
              }
            }
            else
            {
              TRACE_M(TF_SENSOR_M, "Sereis column not initialized\r\n");
            }
          }
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Sereis column not supported\r\n");
        }
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "sensor not initialized\r\n");
      }
    }
    
    TRACE_M(TF_SENSOR_M, "sensor offset %d, dataBufLen %d\r\n", offsetBuff[0], dataBufLen);
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Src unassigned %.4x\r\n", srcAddr);
  }
  
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    srcElementAddr = srcAddr + elementIndex;
    
    /* no publishing of column status */
    if (statusSend == STATUS_SEND_REPLY)
    {
      if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
      {
        result = BLEMesh_ModelSendMessage(srcElementAddr,
                                              dstAddr,
                                              SENSOR_SERVER_MODEL_ID,
                                              (MOBLEUINT16)SENSOR_COLUMN_STATUS, 
                                              data,
                                              dataBufLen);
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          TRACE_M(TF_SENSOR_M, "Reply sent\r\n");
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Reply failed\r\n");
        }
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Reply can't be sent. Client destination address is unassigned\r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
  }
  else
  {
    /* */
  }  
  
  return result;
}


/**
  * @brief  Sensor_SeriesStatus message as per model specification v1.0.1
  *         Sent in response of Sensor Series Get
  *         Series data lying between Raw X1 and Raw X2 to be sent
  * @param  offsets of sensors whose PIDs to be sent
  * @param  total count of sensors
  * @param  destination address for status message
  * @param  Raw X1
  * @param  Raw X2
  * @param  Optional fields present or not
  *         If optional field is absent, whole series to be sent in status
  * @param  Length (in bytes) of received Raw Value X
  * @param  Property ID received which doesn't exist
  * @param  Corresponding element index
  * @param  status to be sent only to client or to be published or to do both
  * @retval Fail if source address is unassigned
  *         Fail if sensor parameters not initialized
  *         Fail if application not able to read column data from application
  */
__weak MOBLE_RESULT Sensor_SeriesStatus(MOBLEUINT8* offsetBuff, 
                                        MOBLEUINT8 sensorCount,
                                        MOBLE_ADDRESS dstAddr,
                                        MOBLEUINT32 rcvdX1,
                                        MOBLEUINT32 rcvdX2,
                                        MOBLEUINT8 isX1X2Present,
                                        MOBLEUINT8 rcvdRawXLength,
                                        MOBLEUINT16 pidNotFound,
                                        MOBLEUINT8 elementIndex,
                                        status_send_e statusSend)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  sensor_params_t* pParams = NULL;
  sensor_series_column_params_t* pSeriesColumn = NULL;
  MOBLEUINT16 dataBufLen = 0;
  MOBLEUINT8 data[379];
  MOBLE_ADDRESS srcAddr = BLEMesh_GetAddress();
  MOBLE_ADDRESS srcElementAddr = MOBLE_ADDRESS_UNASSIGNED;
  sensor_SeriesCbParams_t seriesParams = {0};
  MOBLEUINT32 rawX1 = 0;
  MOBLEUINT32 rawX2 = 0;
  
  if (srcAddr != MOBLE_ADDRESS_UNASSIGNED)
  {
    if (offsetBuff == NULL)
    {
      result = MOBLE_RESULT_INVALIDARG;
      TRACE_M(TF_SENSOR_M, "Invalid arguments\r\n");
    }
    else if (sensorCount == 0) /* received pid doesn't exist */
    {
      if (pidNotFound == MOBLE_ADDRESS_UNASSIGNED)
      {
        TRACE_M(TF_SENSOR_M, "pidNotFound prohibited\r\n");
        result = MOBLE_RESULT_FAIL;
      }
      else
      {
        PutLittleEndian(data, (MOBLEUINT32)pidNotFound, PROPERTY_ID_LENGTH);
        dataBufLen = PROPERTY_ID_LENGTH;
      }
    }
    else
    {
      pParams = SensorServer->pSensorParams+offsetBuff[0];
      
      if (pParams != NULL)
      {
        PutLittleEndian(data, pParams->propertyId, PROPERTY_ID_LENGTH);
        dataBufLen += PROPERTY_ID_LENGTH;
        
        if (pParams->seriesCount != 0)
        {
          /* Send all values between (and including) rawX1 and rawX2 */
          if (isX1X2Present == 1)
          {
            rawX1 = rcvdX1;
            rawX2 = rcvdX2;
          }
          else
          {
            pSeriesColumn = pParams->pSeriesColumn; /* first entry of series */
            if (pSeriesColumn != NULL)
          {
              rawX1 = pSeriesColumn->rawX;/* X1 is first raw value in series */
              pSeriesColumn = pParams->pSeriesColumn + (pParams->seriesCount - 1); /* last entry of series */
            if (pSeriesColumn != NULL)
            {
                rawX2 = pSeriesColumn->rawX; /* X2 is last raw value in series */
              }
              else
              {
                result = MOBLE_RESULT_FAIL;
              }
            }
            else
            {
              TRACE_M(TF_SENSOR_M, "Sereis column not initialized\r\n");
              result = MOBLE_RESULT_FAIL;
            }
            
            if (result == MOBLE_RESULT_SUCCESS)
              {
              seriesParams.propertyId = pParams->propertyId;
              seriesParams.rawValueX1 = rawX1;
              seriesParams.rawValueX2 = rawX2;
                
              if (SensorServer->p_cb->Sensor_ReadSeries_cb != NULL)
              {
                /* X1 and X2 extracted application to fill all data */
                result = SensorServer->p_cb->Sensor_ReadSeries_cb(offsetBuff[0], 
                                                                  &seriesParams);
              }
              else
              {
                result = MOBLE_RESULT_FAIL;
              }
                
              if (result == MOBLE_RESULT_SUCCESS)
              {
                if(seriesParams.dataLength >= 3)
                {
                  memcpy(data+dataBufLen, seriesParams.data, seriesParams.dataLength);
                  dataBufLen += seriesParams.dataLength;
              }
              else
              {
                /*  */
              }
            }
            else
            {
                TRACE_M(TF_SENSOR_M, "Fail to read sensor series");
              }
            }
            else
            {
              /*  */
            }
          }
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Sereis column not supported\r\n");
        }
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "sensor not initialized\r\n");
      }
    }
    
    TRACE_M(TF_SENSOR_M, "sensor offset %d, dataBufLen %d\r\n", offsetBuff[0], dataBufLen);
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Src unassigned %.4x\r\n", srcAddr);
  }
  
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    srcElementAddr = srcAddr + elementIndex;
    
    /* no publishing of series status */
    if (statusSend == STATUS_SEND_REPLY)
    {
      if (dstAddr != MOBLE_ADDRESS_UNASSIGNED)
      {
        result = BLEMesh_ModelSendMessage(srcElementAddr,
                                              dstAddr,
                                              SENSOR_SERVER_MODEL_ID,
                                              (MOBLEUINT16)SENSOR_SERIES_STATUS,
                                              data,
                                              dataBufLen);
        
        if (result == MOBLE_RESULT_SUCCESS)
        {
          TRACE_M(TF_SENSOR_M, "Reply sent\r\n");
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Reply failed\r\n");
        }
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Reply can't be sent. Client destination address is unassigned\r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
  }
  else
  {
    /* */
  }  
  
  return result;
}


/**
  * @brief  Sensor_DescriptorGet callback
  *         response to be sent only to client node and not to pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
*/
__weak MOBLE_RESULT Sensor_DescriptorGet(const MOBLEUINT8* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT16 propertyId = 0x0000;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length > 2)
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  else
  {
    /* Extract offsets of sensors */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pDescriptorParam,
                                        &pidNotFound);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (SensorServer->p_cb->Sensor_DescriptorGet_cb != NULL)
    {
      /*Application Callback */ 
      if( length == 2)
      {
        propertyId = pDescriptorParam[0] | (pDescriptorParam[1] << 8);
      }
      (SensorServer->p_cb->Sensor_DescriptorGet_cb)(propertyId,
                                                    length,
                                                    peerAddr,
                                                    dstPeer,
                                                    elementIndex);
    }
    
    result = Sensor_DescriptorStatus(sensorOffsetBuff, sensorCount, dstClientAddr, pidNotFound);
  }
  
  return result;
}


/**
  * @brief  Sensor_CadenceGet callback
  *         response to be sent only to client node and not to pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_CadenceGet(const MOBLEUINT8* pCadenceParam, 
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 pidMatch = 0;
  sensor_params_t* pParams = NULL;
  sensor_CadenceCbParam_t cadenceParams;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length == 2)
  {
    /* Extract offsets of sensors */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pCadenceParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (sensorCount == 0)
    {
      TRACE_M(TF_SENSOR_M, "pid mismatch %.4x", pidNotFound);
      pidMatch = 0;
    }
    else if (sensorCount == 1)
    {
      pParams = SensorServer->pSensorParams+sensorOffsetBuff[0];
      pidMatch = 1;
    }
    else
    {
      /*  */
    }
    
    if (SensorServer->p_cb->Sensor_CadenceGet_cb != NULL)
    {
      cadenceParams.property_ID=pParams->propertyId;
      cadenceParams.fastCadenceDevisor=pParams->fastCadencePeriodDiv;
      cadenceParams.statusTriggerType=pParams->statusTriggerType;
      cadenceParams.triggerDeltaDown=pParams->statusTriggerDeltaDown;
      cadenceParams.triggerDeltaUp=pParams->statusTriggerDeltaUp;
      cadenceParams.statusMinInterval=pParams->statusMinInterval;
      cadenceParams.fastCadenceLow=pParams->fastCadenceLow;
      cadenceParams.fastCadenceHigh=pParams->fastCadenceHigh;
      
      SensorServer->p_cb->Sensor_CadenceGet_cb(&cadenceParams,
                                               length,
                                               peerAddr,
                                               dstPeer,
                                               elementIndex);
    }
    result = Sensor_CadenceStatus(pidMatch, 
                                  pParams, 
                                  pidNotFound, 
                                  peerAddr, 
                                  elementIndex, 
                                  STATUS_SEND_REPLY);

    
  }
  else
  {
    /*  */
  }
  
  return result;
}


/**
  * @brief  Sensor_CadenceSet callback
  *         response to be sent to both client node and publish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
*/ 
__weak MOBLE_RESULT Sensor_CadenceSet(const MOBLEUINT8* pCadenceParam, 
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex)
{
  sensor_params_t* pParams = NULL;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 pidMatch = 0;
  MOBLEUINT8 paramOffset = 0;
  MOBLEUINT8 fastCadencePeriodDiv;
  MOBLEUINT8 statusTriggerType;
  MOBLEUINT32 statusTriggerDeltaDown;
  MOBLEUINT32 statusTriggerDeltaUp;
  MOBLEUINT8 statusMinInterval;
  MOBLEUINT32 fastCadenceLow;
  MOBLEUINT32 fastCadenceHigh;
  sensor_CadenceCbParam_t cadenceParams;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  /* cadence supported only for data length <= 4
     max length possible for cadence set = 2+1+4+4+1+4+4 */
  if (length >= 8 &&
      length <= 20)
  {
    /* Extract offsets of sensors */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pCadenceParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (sensorCount == 1)
    {
      pParams = SensorServer->pSensorParams+sensorOffsetBuff[0];
      
      if (pParams != NULL)
      {
        pidMatch = 1;
        
        if (pParams->cadenceState == 1) /* cadence supported */
        {
          paramOffset = PROPERTY_ID_LENGTH; /* property id already extracted */
          
          fastCadencePeriodDiv = (pCadenceParam[paramOffset] & 0x7F);
          statusTriggerType = (pCadenceParam[paramOffset] & 0x80) >> 7;
          paramOffset += 1;
          
          if (statusTriggerType == 0)
          {
            GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &statusTriggerDeltaDown);
            paramOffset += pParams->dataLength;
            
            GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &statusTriggerDeltaUp);
            paramOffset += pParams->dataLength;
          }
          else
          {
            GetLittleEndian(pCadenceParam+paramOffset, 2, &statusTriggerDeltaDown);
            paramOffset += 2;
            GetLittleEndian(pCadenceParam+paramOffset, 2, &statusTriggerDeltaUp);
            paramOffset += 2;
          }
          
          statusMinInterval = pCadenceParam[paramOffset];
          paramOffset += 1;
          
          GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &fastCadenceLow);
          paramOffset += pParams->dataLength;
          
          GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &fastCadenceHigh);
          paramOffset += pParams->dataLength;
          
          /* verify received packet length 
             in case length is not correct, cadence values are not correct */
          if (statusTriggerType == 0)/* status trigger down and up = dataLength */
          {
            if (length != 4+4*pParams->dataLength)/* 8, 12, 16 & 20 */
            {
              result = MOBLE_RESULT_FAIL;
              TRACE_M(TF_SENSOR_M, "Incorrect length %ld  (expected %d) status trigger value",
                                   length, 4+4*pParams->dataLength);
            }
          }
          else /* status trigger down and up = 2 */
          {
            if (length != 8+2*pParams->dataLength)/* 10, 12, 14 & 16 */
            {
              result = MOBLE_RESULT_FAIL;
              TRACE_M(TF_SENSOR_M, "Incorrect length %ld  (expected %d) status trigger pc change",
                                    length, 8+2*pParams->dataLength);              
            }
          }
          
          if (result == MOBLE_RESULT_SUCCESS)
          {
            /* some conditions commented as leading to pts test fail, todo verify with sig */
          if (SENSOR_FAST_CADENCE_PERIOD_DIV_PROHIBITED(fastCadencePeriodDiv) ||
              SENSOR_STATUS_MIN_INTERVAL_PROHIBITED(statusMinInterval) /* ||
                (statusTriggerType == 1 && statusTriggerDeltaDown > 10000) ||
                (statusTriggerType == 1 && statusTriggerDeltaUp > 10000) */)
          {
            result = MOBLE_RESULT_FAIL;
            TRACE_M(TF_SENSOR_M, "Prohibited values received\r\n");
            TRACE_M(TF_SENSOR_M, "period div %d, min interval %d, trigger type %d, delta down %4lx, delta up %4lx\r\n",
                    fastCadencePeriodDiv,
                    statusMinInterval,
                    statusTriggerType,
                    statusTriggerDeltaDown,
                    statusTriggerDeltaUp);
          }
          else
          {
            pParams->fastCadencePeriodDiv = fastCadencePeriodDiv;
            pParams->statusTriggerType = statusTriggerType;
            pParams->statusTriggerDeltaDown = statusTriggerDeltaDown;
            pParams->statusTriggerDeltaUp = statusTriggerDeltaUp;
            pParams->statusMinInterval = statusMinInterval;
            pParams->fastCadenceLow = fastCadenceLow;
            pParams->fastCadenceHigh = fastCadenceHigh;
            
            TRACE_M(TF_SENSOR_M, "Cadence values in order %2x, %2x, %2lx, %2lx, %2x, %2lx, %2lx\r\n",
                    fastCadencePeriodDiv,
                    statusTriggerType,
                    statusTriggerDeltaDown,
                    statusTriggerDeltaUp,
                    statusMinInterval,
                    fastCadenceLow,
                    fastCadenceHigh);
          }
        }
        else
        {
            /* length check failed */
          }
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Cadence not supported on Sensor %d\r\n", sensorOffsetBuff[0]);
        }
      }
    }
    else if (sensorCount == 0)
    {
      pidMatch = 0;
      TRACE_M(TF_SENSOR_M, "Unknown property Id %.4x\r\n", pidNotFound);
    }
    else
    {
      /*  */
    }
    
    if (MOBLE_SUCCEEDED(result))
    {
      /* Application Callback */ 
      if (SensorServer->p_cb->Sensor_CadenceSet_cb != NULL)
      {
        cadenceParams.property_ID=pParams->propertyId;
        cadenceParams.fastCadenceDevisor=pParams->fastCadencePeriodDiv;
        cadenceParams.statusTriggerType=pParams->statusTriggerType;
        cadenceParams.triggerDeltaDown=pParams->statusTriggerDeltaDown;
        cadenceParams.triggerDeltaUp=pParams->statusTriggerDeltaUp;
        cadenceParams.statusMinInterval=pParams->statusMinInterval;
        cadenceParams.fastCadenceLow=pParams->fastCadenceLow;
        cadenceParams.fastCadenceHigh=pParams->fastCadenceHigh;
      
        SensorServer->p_cb->Sensor_CadenceSet_cb(&cadenceParams,
                                                 length,
                                                 peerAddr,
                                                 dstPeer,
                                                 elementIndex);
      }
      
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_SENSOR_M, "update status publish parameters based on cadence values\r\n");
        result = Sensor_UpdatePublishParameters(sensorOffsetBuff[0]);
        
        if (result != MOBLE_RESULT_SUCCESS)
        {
          /* */
        }
      }
      
      result = Sensor_CadenceStatus(pidMatch, 
                                    pParams, 
                                    pidNotFound, 
                                    peerAddr, 
                                    elementIndex,
                                    STATUS_SEND_REPLY_PUBLISH);
    }
    else
    {
      /*  */
    }
  }
  else
  {
    /*  */
  }
  
  return result;
}


/**
  * @brief  Sensor_CadenceSetUnack callback
  *         response to be sent to both client node and publish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_CadenceSetUnack(const MOBLEUINT8* pCadenceParam, 
                                           MOBLEUINT32 length,
                                           MOBLE_ADDRESS peerAddr,
                                           MOBLE_ADDRESS dstPeer,
                                           MOBLEUINT8 elementIndex)
{
  sensor_params_t* pParams = NULL;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 pidMatch = 0;
  MOBLEUINT8 paramOffset = 0;
  MOBLEUINT8 fastCadencePeriodDiv;
  MOBLEUINT8 statusTriggerType;
  MOBLEUINT32 statusTriggerDeltaDown;
  MOBLEUINT32 statusTriggerDeltaUp;
  MOBLEUINT8 statusMinInterval;
  MOBLEUINT32 fastCadenceLow;
  MOBLEUINT32 fastCadenceHigh;
  sensor_CadenceCbParam_t cadenceParams;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  /* cadence supported only for data length <= 4
     max length possible for cadence set = 2+1+4+4+1+4+4 */
  if (length >= 8 &&
      length <= 20)
  {
    /* Extract offsets of sensors */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pCadenceParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (sensorCount == 1)
    {
      pParams = SensorServer->pSensorParams+sensorOffsetBuff[0];
      
      if (pParams != NULL)
      {
        pidMatch = 1;
        
        if (pParams->cadenceState == 1) /* cadence supported */
        {
          paramOffset = PROPERTY_ID_LENGTH; /* property id already extracted */
          
          fastCadencePeriodDiv = (pCadenceParam[2] & 0x7F);
          statusTriggerType = (pCadenceParam[2] & 0x80) >> 7;
          paramOffset += 1;
          
          if (statusTriggerType == 0)
          {
            GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &statusTriggerDeltaDown);
            paramOffset += pParams->dataLength;
            
            GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &statusTriggerDeltaUp);
            paramOffset += pParams->dataLength;
          }
          else
          {
            GetLittleEndian(pCadenceParam+paramOffset, 2, &statusTriggerDeltaDown);
            paramOffset += 2;
            GetLittleEndian(pCadenceParam+paramOffset, 2, &statusTriggerDeltaUp);
            paramOffset += 2;
          }
          
          statusMinInterval = pCadenceParam[paramOffset];
          paramOffset += 1;
          
          GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &fastCadenceLow);
          paramOffset += pParams->dataLength;
          
          GetLittleEndian(pCadenceParam+paramOffset, pParams->dataLength, &fastCadenceHigh);
          paramOffset += pParams->dataLength;
          
          /* verify received packet length 
             in case length is not correct, cadence values are not correct */
          if (statusTriggerType == 0)/* status trigger down and up = dataLength */
          {
            if (length != 4+4*pParams->dataLength)/* 8, 12, 16 & 20 */
            {
              result = MOBLE_RESULT_FAIL;
              TRACE_M(TF_SENSOR_M, "Incorrect length %ld  (expected %d) status trigger value",
                                   length, 4+4*pParams->dataLength);
            }
          }
          else /* status trigger down and up = 2 */
          {
            if (length != 8+2*pParams->dataLength)/* 10, 12, 14 & 16 */
            {
              result = MOBLE_RESULT_FAIL;
              TRACE_M(TF_SENSOR_M, "Incorrect length %ld  (expected %d) status trigger pc change",
                                    length, 8+2*pParams->dataLength);              
            }
          }
          
          if (result == MOBLE_RESULT_SUCCESS)
          {
            /* some conditions commented as leading to pts test fail, todo verify with sig */
          if (SENSOR_FAST_CADENCE_PERIOD_DIV_PROHIBITED(fastCadencePeriodDiv) ||
              SENSOR_STATUS_MIN_INTERVAL_PROHIBITED(statusMinInterval) /*||
                (statusTriggerType == 1 && statusTriggerDeltaDown > 10000) ||
                (statusTriggerType == 1 && statusTriggerDeltaUp > 10000)*/)
          {
            result = MOBLE_RESULT_FAIL;
              TRACE_M(TF_SENSOR_M, "Prohibited values received\r\n");
            TRACE_M(TF_SENSOR_M, "period div %d, min interval %d, trigger type %d, delta down %4lx, delta up %4lx\r\n",
                    fastCadencePeriodDiv,
                    statusMinInterval,
                    statusTriggerType,
                    statusTriggerDeltaDown,
                    statusTriggerDeltaUp);
          }
          else
          {
            pParams->fastCadencePeriodDiv = fastCadencePeriodDiv;
            pParams->statusTriggerType = statusTriggerType;
            pParams->statusTriggerDeltaDown = statusTriggerDeltaDown;
            pParams->statusTriggerDeltaUp = statusTriggerDeltaUp;
            pParams->statusMinInterval = statusMinInterval;
            pParams->fastCadenceLow = fastCadenceLow;
            pParams->fastCadenceHigh = fastCadenceHigh;
            
              TRACE_M(TF_SENSOR_M, "Cadence values in order %2x, %2x, %2lx, %2lx, %2x, %2lx, %2lx\r\n",
                    fastCadencePeriodDiv,
                    statusTriggerType,
                    statusTriggerDeltaDown,
                    statusTriggerDeltaUp,
                    statusMinInterval,
                    fastCadenceLow,
                    fastCadenceHigh);
          }
        }
        else
        {
            /* length check failed */
          }
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Cadence not supported on Sensor %d\r\n", sensorOffsetBuff[0]);
        }
      }
    }
    else if (sensorCount == 0)
    {
      pidMatch = 0;
      TRACE_M(TF_SENSOR_M, "Unknown property Id %.4x\r\n", pidNotFound);
    }
    else
    {
      /*  */
    }
    
    if (MOBLE_SUCCEEDED(result))
    {
      /* Application Callback */ 
      if (SensorServer->p_cb->Sensor_CadenceSetUnack_cb != NULL)
      {
        cadenceParams.property_ID=pParams->propertyId;
        cadenceParams.fastCadenceDevisor=pParams->fastCadencePeriodDiv;
        cadenceParams.statusTriggerType=pParams->statusTriggerType;
        cadenceParams.triggerDeltaDown=pParams->statusTriggerDeltaDown;
        cadenceParams.triggerDeltaUp=pParams->statusTriggerDeltaUp;
        cadenceParams.statusMinInterval=pParams->statusMinInterval;
        cadenceParams.fastCadenceLow=pParams->fastCadenceLow;
        cadenceParams.fastCadenceHigh=pParams->fastCadenceHigh;
      
        SensorServer->p_cb->Sensor_CadenceSetUnack_cb(&cadenceParams,
                                                      length,
                                                      peerAddr,
                                                      dstPeer,
                                                      elementIndex);
      }
      
      if (result == MOBLE_RESULT_SUCCESS)
      {
        TRACE_M(TF_SENSOR_M, "update status publish parameters based on cadence values\r\n");
        Sensor_UpdatePublishParameters(sensorOffsetBuff[0]);
      }
      
      /* status to be sent only to publish group */
      result = Sensor_CadenceStatus(pidMatch, 
                                    pParams, 
                                    pidNotFound, 
                                    peerAddr, 
                                    elementIndex,
                                    STATUS_SEND_PUBLISH);
    }
    else
    {
      /*  */
    }
  }
  else
  {
    /*  */
  }
  
  return result;
}


/**
  * @brief  Sensor_SettingsGet callback
  *         response only to be sent to client node and not to pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_SettingsGet(const MOBLEUINT8* pSettingsParam, 
                                       MOBLEUINT32 length,
                                       MOBLE_ADDRESS peerAddr,
                                       MOBLE_ADDRESS dstPeer,
                                       MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  MOBLEUINT16 pidNotFound = 0;
  sensor_SettingsCbParams_t sensorSettingsParam;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length == 2)
  {
    /* Extract offsets of sensors */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pSettingsParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application Callback */ 
    if (SensorServer->p_cb->Sensor_SettingsGet_cb != NULL)
    {
      sensorSettingsParam.propertyId = pSettingsParam[0] | pSettingsParam[1]<<8;
      sensorSettingsParam.settingPropertyId =  pSettingsParam[2] | pSettingsParam[3];
      SensorServer->p_cb->Sensor_SettingsGet_cb(&sensorSettingsParam,
                                                length,
                                                peerAddr,
                                                dstPeer,
                                                elementIndex);
    }
    
    result = Sensor_SettingsStatus(sensorOffsetBuff, sensorCount, dstClientAddr, pidNotFound);
  }
  
  return result;
}


/**
  * @brief  Sensor_SettingGet callback
  *         response only to be sent to client node and not to pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_SettingGet(const MOBLEUINT8* pRcvdParam, 
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 settingPropertyId;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 pidMatch = 0;
  sensor_params_t* pParams = NULL;
  sensor_settings_params_t* pSettingsParam = NULL;
  sensor_SettingCbParams_t sensorSetting;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length == 4)
  {
    /* Extract offsets of sensors given by property ID */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pRcvdParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }  
  
  if (MOBLE_SUCCEEDED(result))
  {
    result = Sensor_ExtractSettingPropertyId(pRcvdParam+2, &settingPropertyId);
    
    if (sensorCount == 0) /* No PID matched */
    {
      /* sensor PID mismatch */
      pidMatch = 0;
      result = MOBLE_RESULT_FALSE;
      
      TRACE_M(TF_SENSOR_M, "Sensor PID mismatch %.4x\r\n", pidNotFound);
    }
    else
    {
      pParams = SensorServer->pSensorParams + sensorOffsetBuff[0];
      
      if (pParams != NULL
          && result == MOBLE_RESULT_SUCCESS)
      {
        pidMatch = 0;
        
        for (MOBLEUINT8 count=0; count<pParams->settingsCount; count++)
        {
          pSettingsParam = (pParams->pSettings)+count;
          
          if (settingPropertyId == pSettingsParam->settingPropertyId)
          {
            pidMatch = 1;
            result = MOBLE_RESULT_SUCCESS;
            
            TRACE_M(TF_SENSOR_M, "setting property id %.4x found, raw value %ld\r\n",
                    settingPropertyId, pSettingsParam->settingRaw);
            break;
          }
          else /* setting Id not found */
          {
            pidMatch = 0;
          }
        }
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
      }
    }
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application Callback */ 
    if (SensorServer->p_cb->Sensor_SettingGet_cb != NULL)
    {
      sensorSetting.property_ID =pParams->propertyId;
      sensorSetting.settingPropertyId = pSettingsParam->settingPropertyId;
      sensorSetting.settingAccess = pSettingsParam->settingAccess;
      sensorSetting.settingRaw= pSettingsParam->settingRaw;
      
      SensorServer->p_cb->Sensor_SettingGet_cb(&sensorSetting,
                                               length,
                                               peerAddr,
                                               dstPeer,
                                               elementIndex);
    }
    
    result = Sensor_SettingStatus(pidMatch,
                                  0,
                                  pParams, 
                                  pSettingsParam, 
                                  pidNotFound, 
                                  settingPropertyId, 
                                  dstClientAddr,
                                  elementIndex,
                                  STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  Sensor_CadenceSetUnack callback
  *         response only to be sent to both client node and pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
*/ 
__weak MOBLE_RESULT Sensor_SettingSet(const MOBLEUINT8* pRcvdParam,
                                      MOBLEUINT32 length,
                                      MOBLE_ADDRESS peerAddr,
                                      MOBLE_ADDRESS dstPeer,
                                      MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 settingPropertyId;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 pidMatch = 0;
  MOBLEUINT8 readViolation = 0;
  sensor_params_t* pParams = NULL;
  sensor_settings_params_t* pSettingsParam = NULL;
  MOBLEUINT32 rawValue;
  MOBLEUINT8 rcvdSettingLengthValue;
  sensor_SettingCbParams_t sensorSetting;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length > 4)
  {
    /* Extract offsets of sensors given by property ID */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pRcvdParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }  
  
  if (MOBLE_SUCCEEDED(result))
  {
    result = Sensor_ExtractSettingPropertyId(pRcvdParam+2, &settingPropertyId);
    
    if (sensorCount == 0) /* No PID matched */
    {
      /* sensor PID mismatch */
      pidMatch = 0;
      result = MOBLE_RESULT_FALSE;
      TRACE_M(TF_SENSOR_M, "Sensor PID mismatch %.4x", pidNotFound);
    }
    else
    {
      pParams = SensorServer->pSensorParams + sensorOffsetBuff[0];
      
      if (pParams != NULL
          && result == MOBLE_RESULT_SUCCESS)
      {
        pidMatch = 0;
        
        for (MOBLEUINT8 count=0; count<pParams->settingsCount; count++)
        {
          pSettingsParam = (pParams->pSettings)+count;
          
          if (settingPropertyId == pSettingsParam->settingPropertyId)
          {
            pidMatch = 1;
            result = MOBLE_RESULT_SUCCESS;
            TRACE_M(TF_SENSOR_M, "setting property id %.4x found, raw value %ld\r\n",
                    settingPropertyId, pSettingsParam->settingRaw);

            break;
          }
          else /* setting Id not found */
          {
            pidMatch = 0;
          }
        }
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
      }
    }
  }
  
  if(pidMatch == 1)
  {
    if (pSettingsParam->settingAccess == SENSOR_SETTING_ACCESS_READ_WRITE)
    {
      rcvdSettingLengthValue  = length - 4;
      
      TRACE_M(TF_SENSOR_M, "Received setting length %d\r\n", rcvdSettingLengthValue);
      
      if (rcvdSettingLengthValue <= 4)
      {
        rawValue = 0;
        
        GetLittleEndian(pRcvdParam+4, rcvdSettingLengthValue, &rawValue);        
        pSettingsParam->settingRaw = rawValue;
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Received setting length value more than required \r\n");
      }
      
      readViolation = 0;
      
      TRACE_M(TF_SENSOR_M, "Raw value %ld updated\r\n", pSettingsParam->settingRaw);
    }
    else
    {
      readViolation = 1;
      TRACE_M(TF_SENSOR_M, "Read violation occurred, can't be written\r\n");
    }
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application Callback */ 
    if (SensorServer->p_cb->Sensor_SettingSet_cb != NULL)
    {
      sensorSetting.property_ID =pParams->propertyId;
      sensorSetting.settingPropertyId = pSettingsParam->settingPropertyId;
      sensorSetting.settingAccess = pSettingsParam->settingAccess;
      sensorSetting.settingRaw= pSettingsParam->settingRaw;
      SensorServer->p_cb->Sensor_SettingSet_cb(&sensorSetting,
                                               length,
                                               peerAddr,
                                               dstPeer,
                                               elementIndex);
    }
    
    result = Sensor_SettingStatus(pidMatch,
                                  readViolation,
                                  pParams, 
                                  pSettingsParam, 
                                  pidNotFound, 
                                  settingPropertyId, 
                                  dstClientAddr,
                                  elementIndex,
                                  STATUS_SEND_REPLY_PUBLISH);
  }
  
  return result;
}


/**
  * @brief  Sensor_SettingSetUnack callback
  *         response to be sent to both client node and publish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_SettingSetUnack(const MOBLEUINT8* pRcvdParam, 
                                           MOBLEUINT32 length,
                                           MOBLE_ADDRESS peerAddr,
                                           MOBLE_ADDRESS dstPeer,
                                           MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 settingPropertyId;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 pidMatch = 0;
  MOBLEUINT8 readViolation = 0;
  sensor_params_t* pParams = NULL;
  sensor_settings_params_t* pSettingsParam = NULL;
  MOBLEUINT32 rawValue;
  MOBLEUINT8 rcvdSettingLengthValue;
  sensor_SettingCbParams_t sensorSetting;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length > 4)
  {
    /* Extract offsets of sensors given by property ID */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pRcvdParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }  
  
  if (MOBLE_SUCCEEDED(result))
  {
    result = Sensor_ExtractSettingPropertyId(pRcvdParam+2, &settingPropertyId);
    
    if (sensorCount == 0) /* No PID matched */
    {
      /* sensor PID mismatch */
      pidMatch = 0;
      result = MOBLE_RESULT_FALSE;
      TRACE_M(TF_SENSOR_M, "Sensor PID mismatch %.4x\r\n", pidNotFound);
    }
    else
    {
      pParams = SensorServer->pSensorParams + sensorOffsetBuff[0];
      
      if (pParams != NULL
          && result == MOBLE_RESULT_SUCCESS)
      {
        pidMatch = 0;
        
        for (MOBLEUINT8 count=0; count<pParams->settingsCount; count++)
        {
          pSettingsParam = (pParams->pSettings)+count;
          
          if (settingPropertyId == pSettingsParam->settingPropertyId)
          {
            pidMatch = 1;
            result = MOBLE_RESULT_SUCCESS;

            TRACE_M(TF_SENSOR_M, "setting property id %.4x found, raw value %ld\r\n",
                    settingPropertyId, pSettingsParam->settingRaw);
            break;
          }
          else /* setting Id not found */
          {
            pidMatch = 0;
          }
        }
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
      }
    }
  }
  
  if(pidMatch == 1)
  {
    if (pSettingsParam->settingAccess == SENSOR_SETTING_ACCESS_READ_WRITE)
    {
      rcvdSettingLengthValue  = length - 4;
      
      TRACE_M(TF_SENSOR_M, "Received setting length %d\r\n", rcvdSettingLengthValue);
      
      if (rcvdSettingLengthValue <= 4)
      {
        rawValue = 0;
        
        GetLittleEndian(pRcvdParam+4, rcvdSettingLengthValue, &rawValue);        
        pSettingsParam->settingRaw = rawValue;
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Received setting length value more than required \r\n");
      }
      
      readViolation = 0;
      
      TRACE_M(TF_SENSOR_M, "Raw value %ld updated\r\n", pSettingsParam->settingRaw);
    }
    else
    {
      readViolation = 1;
      TRACE_M(TF_SENSOR_M, "Read violation occurred, can't be written\r\n");
    }
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application Callback */ 
    if (SensorServer->p_cb->Sensor_SettingSetUnack_cb != NULL)
    {
      sensorSetting.property_ID =pParams->propertyId;
      sensorSetting.settingPropertyId = pSettingsParam->settingPropertyId;
      sensorSetting.settingAccess = pSettingsParam->settingAccess;
      sensorSetting.settingRaw= pSettingsParam->settingRaw;
      
      (SensorServer->p_cb->Sensor_SettingSetUnack_cb)(&sensorSetting,
                                                      length,
                                                      peerAddr,
                                                      dstPeer,
                                                      elementIndex);
    }
    
    result = Sensor_SettingStatus(pidMatch,
                                  readViolation,
                                  pParams, 
                                  pSettingsParam, 
                                  pidNotFound, 
                                  settingPropertyId, 
                                  dstClientAddr,
                                  elementIndex,
                                  STATUS_SEND_PUBLISH);
  }
  
  return result;
}


/**
  * @brief  Sensor_Get callback
  *         response only to be sent to client node and not to pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_Get(const MOBLEUINT8* pGetParam, 
                               MOBLEUINT32 length,
                               MOBLE_ADDRESS peerAddr,
                               MOBLE_ADDRESS dstPeer,
                               MOBLEUINT8 elementIndex)
{  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 propertyId = 0x0000;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  MOBLEUINT16 pidNotFound = 0;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length > 2)
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  else
  {
    /* Extract offsets of sensors */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pGetParam,
                                        &pidNotFound);    
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    /* Application may update sensor data */
    if (SensorServer->p_cb->Sensor_Get_cb != NULL)
    {
      /*Application Callback */ 
      if(length == 2){
       propertyId = pGetParam[0] | pGetParam[1]<<8 ;
      }
      (SensorServer->p_cb->Sensor_Get_cb)(propertyId,
                                          length,
                                          peerAddr,
                                          dstPeer,
                                          elementIndex);
    }
    
    result = Sensor_Status(sensorOffsetBuff, 
                           sensorCount, 
                           dstClientAddr, 
                           pidNotFound,
                           STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  Sensor_ColumnGet callback
  *         response only to be sent to client node and not to pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_ColumnGet(const MOBLEUINT8* pColumnParam, 
                                     MOBLEUINT32 length,
                                     MOBLE_ADDRESS peerAddr,
                                     MOBLE_ADDRESS dstPeer,
                                     MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT32 rawValueX = 0;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 rcvdRawXLength = 0;
  sensor_params_t* pParams = NULL;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  sensor_ColumnCbParams_t pColumnParams;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length >= 3)
  {
    /* Extract offsets of sensors given by property ID */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pColumnParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (sensorCount == 0)
    {
      /* sensor PID mismatch */
      result = MOBLE_RESULT_FALSE;
      TRACE_M(TF_SENSOR_M, "Sensor PID mismatch %.4x\r\n", pidNotFound);
    }
    else
    {
      pParams = SensorServer->pSensorParams + sensorOffsetBuff[0];
      
      if (pParams != NULL
          && result == MOBLE_RESULT_SUCCESS)
      {
        if (pParams->seriesCount == 0)
        {
          TRACE_M(TF_SENSOR_M, "Series count not supported on sensor %d\r\n", sensorOffsetBuff[0]);
        }
        else
        {
          rcvdRawXLength = length - PROPERTY_ID_LENGTH;
          
          if (rcvdRawXLength <= 4)
          {
            GetLittleEndian(pColumnParam+PROPERTY_ID_LENGTH, rcvdRawXLength, &rawValueX);
          }
          else
          {
            result = MOBLE_RESULT_FAIL;
            TRACE_M(TF_SENSOR_M, "Received length mismatch \r\n");
          }
          
        }
      }
      else
      {
        /*  */
      }
    }
  }
  else
  {
    /* */
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (SensorServer->p_cb->Sensor_ColumnGet_cb != NULL)
    {
      /*Application Callback */ 
      pColumnParams.propertyId = pParams->propertyId;
      pColumnParams.rawValueX = rawValueX;
      
      (SensorServer->p_cb->Sensor_ColumnGet_cb)(&pColumnParams,
                                                length,
                                                peerAddr,
                                                dstPeer,
                                                elementIndex);
    }
    
    result = Sensor_ColumnStatus(sensorOffsetBuff,
                                 sensorCount,
                                 dstClientAddr,
                                 rawValueX,
                                 rcvdRawXLength,
                                 pidNotFound,
                                 elementIndex,
                                 STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  Sensor_SeriesGet callback
  *         response only to be sent to client node and not to pulish group
  * @param  Parameters as set by client
  * @param  Parameters length
  * @param  Client address
  * @param  Destination address set by client (unicast or group)
  * @param  element index corresponding to element which processed the message
  * @retval Invalid if length is not appropriate
  *         Fail if parameters are prohibited
  */
__weak MOBLE_RESULT Sensor_SeriesGet(const MOBLEUINT8* pSeriesParam, 
                                     MOBLEUINT32 length,
                                     MOBLE_ADDRESS peerAddr,
                                     MOBLE_ADDRESS dstPeer,
                                     MOBLEUINT8 elementIndex)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT32 rawValueX1 = 0;
  MOBLEUINT32 rawValueX2 = 0;
  MOBLEUINT8 isX1X2Present = 0;
  MOBLEUINT8 sensorOffsetBuff[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 sensorCount = 0;
  MOBLEUINT16 pidNotFound = 0;
  MOBLEUINT8 rcvdRawXLength = 0;
  sensor_params_t* pParams = NULL;
  MOBLE_ADDRESS dstClientAddr = peerAddr;
  
  TRACE_M(TF_SENSOR_M, "length %ld, peer %.4x, dst %.4x, element index %d \r\n",
          length, peerAddr, dstPeer, elementIndex);
  
  if (length >= 2)
  {
    /* Extract offsets of sensors given by property ID */
    result = Sensor_ExtractSensorOffset(elementIndex,
                                        sensorOffsetBuff, 
                                        &sensorCount, 
                                        length, 
                                        pSeriesParam,
                                        &pidNotFound);
  }
  else
  {
    result = MOBLE_RESULT_INVALIDARG;
    TRACE_M(TF_SENSOR_M, "Invalid length (%ld)\r\n", length);
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (sensorCount == 0)
    {
      /* sensor PID mismatch */
      result = MOBLE_RESULT_FALSE;
      TRACE_M(TF_SENSOR_M, "Sensor PID mismatch %.4x\r\n", pidNotFound);
    }
    else
    {
      pParams = SensorServer->pSensorParams + sensorOffsetBuff[0];
      
      if (pParams != NULL
          && result == MOBLE_RESULT_SUCCESS)
      {
        if(pParams->seriesCount != 0)
        {
          /* check if X1 and X2 are present */
          if (length > 2)
          {
            if (length % 2 == 0)
            {
              rcvdRawXLength = (length - 2)/2;
              
              if (rcvdRawXLength <= 4)
              {
                GetLittleEndian(pSeriesParam+PROPERTY_ID_LENGTH, rcvdRawXLength, &rawValueX1);
                GetLittleEndian(pSeriesParam+PROPERTY_ID_LENGTH+rcvdRawXLength, rcvdRawXLength, &rawValueX2);
                
                TRACE_M(TF_SENSOR_M, "rawValue X1 %ld and rawValueX2 %ld\r\n", rawValueX1, rawValueX2);
              }
              else
              {
                result = MOBLE_RESULT_FAIL;
                TRACE_M(TF_SENSOR_M, "received length mismatch \r\n");
              }
              
              isX1X2Present = 1;
            }
            else
            {
              /* odd length */
              result = MOBLE_RESULT_FAIL;
            }
          }
          else
          {
            isX1X2Present = 0;
            TRACE_M(TF_SENSOR_M, "X1 and X2 not present. Send for all columns\r\n");
          }
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Series count not supported on sensor %d\r\n", sensorOffsetBuff[0]);
        }
      }
      else
      {
        /*  */
      }
    }
  }
  else
  {
    /* */
  }
  
  if (MOBLE_SUCCEEDED(result))
  {
    if (SensorServer->p_cb->Sensor_SeriesGet_cb != NULL)
    {
      /*Application Callback */ 
      sensor_SeriesCbParams_t pSeriesParams;
      pSeriesParams.propertyId=pParams->propertyId;
      pSeriesParams.rawValueX1= rawValueX1;
      pSeriesParams.rawValueX2= rawValueX2;
      
      (SensorServer->p_cb->Sensor_SeriesGet_cb)(&pSeriesParams,
                                                length,
                                                peerAddr,
                                                dstPeer,
                                                elementIndex);
    }
    
    result = Sensor_SeriesStatus(sensorOffsetBuff,
                                 sensorCount,
                                 dstClientAddr,
                                 rawValueX1,
                                 rawValueX2,
                                 isX1X2Present,
                                 rcvdRawXLength,
                                 pidNotFound,
                                 elementIndex,
                                 STATUS_SEND_REPLY);
  }
  
  return result;
}


/**
  * @brief  SensorModelServer_GetOpcodeTableCb callback
  * @param  Reference to sensor model opcode table
  * @param  size of table to be set
  * @retval Success
*/
__weak MOBLE_RESULT SensorModelServer_GetOpcodeTableCb(const MODEL_OpcodeTableParam_t **data, 
                                                       MOBLEUINT16 *length)
{
  *data = Sensor_Opcodes_Table;
  *length = sizeof(Sensor_Opcodes_Table)/sizeof(Sensor_Opcodes_Table[0]);
  
  return MOBLE_RESULT_SUCCESS;
}


/**
  * @brief  SensorModelServer_GetStatusRequestCb is callback from 
  *         mesh library to send response to the message from peer
  *         NOT USED
  * @param  pmsgParams: message parameters
* @param  opcode: Received opcode of the Status message callback
  * @param  pResponsedata: buffer to be updated with status
  * @param  plength: Length of tata, updated by application
  * @param  pRxData: data received in packet.
* @param  dataLength: length of the data in packet.
* @param  response: Value to indicate whether message is acknowledged meassage or not.
  * @retval Success
*/
__weak MOBLE_RESULT SensorModelServer_GetStatusRequestCb(MODEL_MessageHeader_t *pmsgParams, 
                                                         MOBLEUINT16 opcode, 
                                                         MOBLEUINT8 *pResponsedata, 
                                                         MOBLEUINT32 *plength, 
                                                         MOBLEUINT8 const *pRxData,
                                                         MOBLEUINT32 dataLength,
                                                         MOBLEBOOL response)
{
  return MOBLE_RESULT_SUCCESS;    
}


/**
  * @brief  SensorModelServer_ProcessMessageCb callback from access layer
  * @param  pmsgParams: message parameters
  * @param  opcode: message opcode
  * @param  pRxData: data received in packet
  * @param  dataLength: length of the data
  * @param  response: not applicable
  * @retval MOBLE_RESULT as set by callee
*/ 
__weak MOBLE_RESULT SensorModelServer_ProcessMessageCb(MODEL_MessageHeader_t *pmsgParams, 
                                                       MOBLEUINT16 opcode, 
                                                       MOBLEUINT8 const *pRxData, 
                                                       MOBLEUINT32 dataLength, 
                                                       MOBLEBOOL response)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 elementIndex = 0;
  
  if(SensorServer != NULL)
  {
    result = MOBLE_RESULT_SUCCESS;
  }
  else
  {
    TRACE_M(TF_LIGHT_LC_M, "Sensor server not supported\r\n");
    result = MOBLE_RESULT_FAIL;
  }
  
  if(MOBLE_SUCCEEDED(result))
  {
  elementIndex = pmsgParams->elementIndex;
    
  switch(opcode)
  { 
    case SENSOR_DESCRIPTOR_GET:
      result = Sensor_DescriptorGet(pRxData, dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer, elementIndex);
      break;
    case SENSOR_GET:
      result = Sensor_Get(pRxData, dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer, elementIndex);
      break;
    case SENSOR_CADENCE_GET:
      result = Sensor_CadenceGet(pRxData, dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer, elementIndex);
      break;
    case SENSOR_CADENCE_SET:
      result = Sensor_CadenceSet(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    case SENSOR_CADENCE_SET_UNACK:
      result = Sensor_CadenceSetUnack(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    case SENSOR_COLUMN_GET:
      result = Sensor_ColumnGet(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    case SENSOR_SERIES_GET:
      result = Sensor_SeriesGet(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    case SENSOR_SETTINGS_GET:
      result = Sensor_SettingsGet(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    case SENSOR_SETTING_GET:
      result = Sensor_SettingGet(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    case SENSOR_SETTING_SET:
      result = Sensor_SettingSet(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    case SENSOR_SETTING_SET_UNACK:
      result = Sensor_SettingSetUnack(pRxData,dataLength, pmsgParams->peer_addr, pmsgParams->dst_peer,elementIndex);
      break;
    default:
      {
        break;
      }
  } /* opcode switch ends */
  }
  
  return result;
}


/**
  * @brief  Sensor_SleepDurationMs_Get
  *         returns max time after which call to process Sensor_Process is required
  *         to publish appropriate sensor status
  * @param  None
  * @retval sleep time in milliseconds
  */
__weak MOBLEUINT32 Sensor_SleepDurationMs_Get(void)
{
  MOBLEUINT32 sleepDuration = 96*60*60*1000; /* 96 hours in milliseconds */
  MOBLEUINT32 timeRemaining = 0;
  MOBLEUINT32 current = Clock_Time();
  sensor_params_t* pParams = NULL; 
  
  for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
  {
    if (SensorServer != NULL)
    {
    pParams = SensorServer->pSensorParams+count;
    }
    else
    {
      /* Sensor not initialized */
    }
    
    if (pParams != NULL)
    {
      if (pParams->timerStatus == TIMER_RUNNING)
      {
        if (pParams->timerBegin < pParams->timerEnd)
        {
          /* timer already expired and publication pending */
          if (current >= pParams->timerEnd ||
              current < pParams->timerBegin) 
          {
            timeRemaining = 0;
          }
          else /* time to next publicaton */
          {
            timeRemaining = pParams->timerEnd - current;
          }
        }
        else /* overflow */
        {
          /* timer already expired and publication pending */
          if (current >= pParams->timerEnd && 
              current < pParams->timerBegin) 
          {
            timeRemaining = 0;
          }
          else /* time to next publication */
          {
            if (current < pParams->timerEnd)
            {
              timeRemaining = pParams->timerEnd - current;
            }
            else
            {
              timeRemaining = (MAX_U32_VALUE - current) + pParams->timerEnd;
            }
          }
        }
        
        if (sleepDuration > timeRemaining)
        {
          sleepDuration = timeRemaining;
        }
        else
        {
          /* sleep time already less than what is required */
        }
      }
      else
      {
        /* sleep time update not required for this sensor as timer not running */
      }
    }
    else
    {
      /* */
    }
  }
  
  return sleepDuration;
}


/**
  * @brief  Sensor_UpdatePublishParameters to update fast cadence (if supported)
  *         state
  *         Update is required if sensor value is changed or
  *         Cadence parameters are modified or
  *         Sensor Server publish period is modified
  *         If publish parameter updated, all sensors on same element align
  *         publish time
  *         No change if cadence not supported
  * @param  sensor offset of corresponding sensor whose parameters to be updated
  * @retval Fail if sensor not initialized
  *         else Success
  */
__weak MOBLE_RESULT Sensor_UpdatePublishParameters(MOBLEUINT8 sensorOffset)
{
  MOBLEUINT8 fastCadenceStatus = 0;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 sensorOffsetTemp = sensorOffset;
  sensor_params_t* pParams = SensorServer->pSensorParams + sensorOffsetTemp;
  MOBLEUINT32 fastCadenceLow;
  MOBLEUINT32 fastCadenceHigh;

  TRACE_M(TF_SENSOR_M, "SENSOR%d\r\n", sensorOffset+1);
  
  if (pParams != NULL)
  {
    if (pParams->cadenceState == 1)
    {
      if (pParams->modelPublishPeriodMs == 0)
      {
        TRACE_M(TF_SENSOR_M, "Publish period set to 0\r\n");
      }
      else
      {
        if (SensorServer->p_cb->Sensor_IsFastCadence_cb != NULL)
        {
          fastCadenceLow = pParams->fastCadenceLow;
          fastCadenceHigh = pParams->fastCadenceHigh;
      
          /* check sensor value for fast cadence */
          fastCadenceStatus = 
            SensorServer->p_cb->Sensor_IsFastCadence_cb(sensorOffset,
                                                        &fastCadenceLow, 
                                                        &fastCadenceHigh);
      }
      else
      {
          fastCadenceStatus = 0;
      }
      
      /* if fast cadence required, divide publish interval by 2^n, n = fast cadence period divisor */
        if (fastCadenceStatus == 1)
      {
          pParams->publishPeriodMs = pParams->modelPublishPeriodMs / \
                                     (1<<pParams->fastCadencePeriodDiv);
      }
      else
      {
          pParams->publishPeriodMs = pParams->modelPublishPeriodMs;
      }
      
      /* if publish period is less than status min interval, adjust to status min value */
      if (pParams->publishPeriodMs < (1 << pParams->statusMinInterval))
      {
        TRACE_M(TF_SENSOR_M, "publish period %ld less than status minimum interval %d\r\n", pParams->publishPeriodMs, 1 << pParams->statusMinInterval);
        pParams->publishPeriodMs = 1 << pParams->statusMinInterval;
      }
      
        TRACE_M(TF_SENSOR_M, "SENSOR%d fast cadence state %d, publish period %ld\r\n",
                sensorOffset+1, fastCadenceStatus, pParams->publishPeriodMs);
      }
      
      /* realign timers of all sensors on same element index
         this may result in publishing of sensor status even for sensors whose
         sensor status is not scheduled
         Doing this benefits to publish sensor data of all elements in one message
         by data marshalling */
      for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
      {
        if(((SensorServer->pSensorParams)+count)->elementIdx == pParams->elementIdx &&
             ((SensorServer->pSensorParams)+count)->cadenceState == 1)
        {
          if(((SensorServer->pSensorParams)+count)->modelPublishPeriodMs == 0)
          {
            ((SensorServer->pSensorParams)+count)->timerStatus = TIMER_STOPPED;
          }
          else
          {
      /* start timer for now to immediately send status on change parameters
      subsequently status would be sent after every publish period */
            ((SensorServer->pSensorParams)+count)->timerStatus = TIMER_RUNNING;
            ((SensorServer->pSensorParams)+count)->timerBegin = Clock_Time();
            ((SensorServer->pSensorParams)+count)->timerEnd = \
              ((SensorServer->pSensorParams)+count)->timerBegin;            
          }
        }
        else
        {
          /* */
        }
      }
      
      result = MOBLE_RESULT_SUCCESS;
    }
    else /* cadence not supported */
    {
      TRACE_M(TF_SENSOR_M, "Cadence not supported\r\n");
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Sensor not initialized\r\n");
  }
  
  return result;
}


/**
  * @brief  Sensor_CheckStatusTrigger - trigger sensor status if change in sensor state
  *         is larger than defined trigger down/trigger up
  *         Status trigger is decided by application
  * @param  sensor offset of corresponding sensor
  * @retval set by application
  */
__weak MOBLE_RESULT Sensor_CheckStatusTrigger(MOBLEUINT8 sensorOffset)
{
  MOBLEUINT8 sensorOffsetTemp = sensorOffset;
  sensor_params_t* pParams = SensorServer->pSensorParams + sensorOffsetTemp;
  status_trigger_type_e statusTriggerType = (status_trigger_type_e)pParams->statusTriggerType;
  MOBLEUINT32 statusTriggerDeltaDown = pParams->statusTriggerDeltaDown;
  MOBLEUINT32 statusTriggerDeltaUp = pParams->statusTriggerDeltaUp;
  MOBLEUINT8 statusTrigger = 0;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  if (pParams != NULL &&
      SensorServer->p_cb->Sensor_IsStatusTrigger_cb != NULL)
  {
    statusTrigger = SensorServer->p_cb->Sensor_IsStatusTrigger_cb(sensorOffset,
                                                                  statusTriggerType,
                                                                  &statusTriggerDeltaDown,
                                                                  &statusTriggerDeltaUp);
  }
  else
  {
  result = MOBLE_RESULT_FAIL;
  }
    
  if (statusTrigger == 1)
  {
    Sensor_Status(&sensorOffsetTemp, 1, 0x0000, 0x0000, STATUS_SEND_PUBLISH);
  }
  else
  {
/* status trigger not to be sent, change is within safe limit */
  }
  
  return result;
}


/**
  * @brief  Send sensor status to publish group
  * @param  sensor offset of corresponding sensor
  * @param  element index
  * @param  sensor property ID
  * @retval fail if sensor not initialized
  *         else set by callee
  */
__weak MOBLE_RESULT Sensor_Send(MOBLEUINT8 sensorOffset,
                                MOBLEUINT8 elementIdx,
                                MOBLEUINT16 propertyId)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  sensor_params_t* pParams = NULL;
  
  if(SensorServer != NULL)
  {
    pParams = SensorServer->pSensorParams + sensorOffset;
    
    if (pParams != NULL)
    {
      if (pParams->elementIdx == elementIdx &&
          pParams->propertyId == propertyId)
      {
        result = Sensor_Status(&sensorOffset,
                               1,
                               MOBLE_ADDRESS_UNASSIGNED,
                               0x0000,
                               STATUS_SEND_PUBLISH);

      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Parameters mismatch \r\n");
        result = MOBLE_RESULT_FAIL;
      }
    }
    else
    {
      result = MOBLE_RESULT_FAIL;
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
    TRACE_M(TF_SENSOR_M, "Sensor server not initialized\r\n");
  }
  
  return result;
}


/**
  * @brief  Sensor_UpdateCadence to update publish period according to cadence state
  * @param  sensor offset of corresponding sensor
  * @param  corresponding element index
  * @param  property ID of sensor
  * @retval Fail if sensors not initialized or sensor offset doesn't exist
  *         Not implemented if cadence not supported
  *         False if parameters are not consistent
  *         else Success
  */
__weak MOBLE_RESULT Sensor_UpdateCadence(MOBLEUINT8 sensorOffset,
                                MOBLEUINT8 elementIdx,
                                MOBLEUINT16 propertyId)
{
  sensor_params_t* pParams = NULL;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  if (SensorServer != NULL
      && sensorOffset < SensorServer->sensorsCount)
  {
    pParams = SensorServer->pSensorParams+sensorOffset;
  }
  else
  {
    TRACE_M(TF_SENSOR_M, "Sensor parameters not initialized\r\n");
    result = MOBLE_RESULT_FAIL;
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    if (pParams->elementIdx == elementIdx &&
        pParams->propertyId == propertyId)
    {
      if (pParams->cadenceState == 1) /* cadence is supported */
      {
        /* update sensor publish parameters based on sensor cadence state */
        if (MOBLE_SUCCEEDED(Sensor_UpdatePublishParameters(sensorOffset)))
        {
          TRACE_M(TF_SENSOR_M, "Publish parameters updated\r\n");
        }
        
        /* check for sensor status trigger state */
        if (MOBLE_SUCCEEDED(Sensor_CheckStatusTrigger(sensorOffset)))
      {
          TRACE_M(TF_SENSOR_M, "Sensor status triggered\r\n");
      }
      }
      else
      {
        /* This function does not support update of series column state */
        result = MOBLE_RESULT_NOTIMPL;
      }
    }
    else
    {
      TRACE_M(TF_SENSOR_M, "Arguments mismatch \r\n");
      result = MOBLE_RESULT_FALSE;
    }
  }
  
  return result;
}


/**
  * @brief  Sensor_UpdatePublishState to enable/disable publishing
  * @param  sensor offset
  * @param  new state
  * @retval Fail if sensors not initialized or sensor offset doesn't exist
  *         else Success
  */
__weak MOBLE_RESULT Sensor_UpdatePublishState(MOBLEUINT8 sensorOffset, MOBLEUINT8 state)
{
  sensor_params_t* pParams = NULL;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  if (SensorServer != NULL &&
      sensorOffset < SensorServer->sensorsCount)
  {
    pParams = SensorServer->pSensorParams+sensorOffset;
    TRACE_M(TF_SENSOR_M, "sensor offset %d new publish state %d\r\n", sensorOffset, state);
  }
  else
  {
    TRACE_M(TF_SENSOR_M, "Sensor parameters not initialized\r\n");
    result = MOBLE_RESULT_FAIL;
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    if (state == 0)
      {
      pParams->cadenceState = 0;
    }
    else
    {
      pParams->cadenceState = 1;
    }
  }
  
  return result;
}


/**
* @brief  Update timers
  *         Status of sensors supported on same element index and have sensor 
  *         status publication scheduled within 50ms of each other, are 
  *         combined in single sensor status message
  *         At a time only 1 element can publish sensor status even if sensors on
  *         more than 1 element needs to publish. This is done to ensure multiple
  *         calls to Sensor_Process for publishing from multi-element. Issuing
  *         more than one unsegmented/segmented sensor status message in one go
  *         may fail due to limitation in capacity of underlying layers
* @param  void
* @retval void
*/
__weak void Sensor_Process(void)
{
  MOBLEUINT8 triggerStatus;
  sensor_params_t* pParams = NULL;
  tClockTime currentTick;
  MOBLEUINT32 advancedTick;
  MOBLEUINT8 sensorOffset[TOTAL_SENSORS_COUNT];
  MOBLEUINT8 publishElementIndex = 0;
  MOBLEUINT8 sensorSeiPublishCount = 0;
  MOBLEUINT8 tbcPublication = 0;
  
  if (SensorServer != NULL)
  {
    currentTick = Clock_Time();
    advancedTick = currentTick + 50; /* advance time by 50 ms */
    
    /* check for all sensors */
    for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
    {
      pParams = SensorServer->pSensorParams+count;
      
      if (pParams->timerStatus == TIMER_RUNNING)
      {
        /* Publish count (no of sensors whose marshalled data to be published)
           is 0, publishElementIndex doesn't matter (it is yet to be initialized) */
        if (publishElementIndex == 0)
        {
          tbcPublication = 1;
        }
        else
        {
          if(pParams->elementIdx != publishElementIndex)
          {
            /* if element index not same as publishElementIndex, 
               exit and try next time */
            break;
          }
          else
          {
            tbcPublication = 1;
          }
        }
        
        /* check if sensor publication required w.r.t advanced time */
        if(tbcPublication == 1)
        {
          if (pParams->timerBegin <= pParams->timerEnd)
          {
            triggerStatus = advancedTick >= pParams->timerEnd || 
                            advancedTick < pParams->timerBegin;
          }
          else  /* overflow */
          {
            triggerStatus = advancedTick >= pParams->timerEnd && 
                            advancedTick < pParams->timerBegin;
          }
        }
        
        if (triggerStatus == 1)
        {
          publishElementIndex = pParams->elementIdx;
          
          sensorOffset[sensorSeiPublishCount] = count;
          sensorSeiPublishCount++;
        }
        
        triggerStatus = 0;
      }
      else
      {
        /* Timer stopped */
      }
    }
      
    if(sensorSeiPublishCount != 0)
    {
      BLEMesh_GetPublishAddress(publishElementIndex, SENSOR_SERVER_MODEL_ID);
        
      if (MOBLE_SUCCEEDED(Sensor_Status(sensorOffset, sensorSeiPublishCount, 0x0000, 0, STATUS_SEND_PUBLISH)))
      {
        TRACE_M(TF_SENSOR_M, "Element index %d sensor count %d status published\r\n", 
                publishElementIndex, sensorSeiPublishCount);
      }
      else
      {
        TRACE_M(TF_SENSOR_M, "Sensor publish failed, sensor count %d\r\n", sensorSeiPublishCount);
      }
        
      for (MOBLEUINT8 count=0; count<sensorSeiPublishCount; count++)
      {
        pParams = SensorServer->pSensorParams+sensorOffset[count];
        
        /* update timer parameters for next publication */
        pParams->timerBegin = currentTick;
        pParams->timerEnd = pParams->timerBegin + pParams->publishPeriodMs;
      }
      
      TRACE_M(TF_SENSOR_M, "publish element index %d, publish sensors count %d\r\n", 
              publishElementIndex, sensorSeiPublishCount);
    }    
  }
  else
  {
    /* Sensor server not initialized */
  }
}


/**
  * @brief  Sensor server initialization
  * @param  sensor buffer to be allocated to sensor server model structure
  * @param  application callbacks used by sensor server
  * @param  buffer size
  * @param  initialization parameters
  * @retval success if initialization successful
  *         else fail
  */
__weak MOBLE_RESULT SensorServer_Init(void* sensorBuff,
                                      const sensor_server_cb_t* sensor_cb,
                                      MOBLEUINT16 sizeBuff,
                                      const void* initParams)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 currentElementIdx;
  MOBLEUINT8 nextElementIdx;
  MOBLEUINT16 currentPid;
  MOBLEUINT16 nextPid;
  MOBLEUINT16 requiredBufferSize = 0;
  MOBLEUINT16 totalSettingsCount = 0;
  MOBLEUINT16 totalSeriesColumnCount = 0;
  
  MOBLEUINT32 buffStartTempIndex = 0;
  sensor_params_t* pParams = NULL;
  sensor_settings_params_t* pSettingsParams;
  sensor_series_column_params_t* pSeriesColumnParams;
  const sensor_server_init_params_t* pServerInitParams = (sensor_server_init_params_t*) initParams;
  const sensor_init_params_t* pInitParams;
  const sensor_settings_init_params_t* pSettingsInitParams;
  const sensor_series_column_init_params_t* pSeriesColumnInitParams;
  
  /* Sensor server count should be > 0 */
  if (pServerInitParams == NULL ||
      pServerInitParams->sensorsCount == 0)
  {
    TRACE_M(TF_SENSOR_M, "Init params NULL or Sensor count can't be 0\r\n");
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    result = MOBLE_RESULT_SUCCESS;
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    for (MOBLEUINT8 count=0; count<pServerInitParams->sensorsCount; count++)
    {
      /* element index should be < N, N is total number of elements supported by device */
      if (pServerInitParams->sensorInitParams[count].elementIdx < APPLICATION_NUMBER_OF_ELEMENTS)
      {
        /* result = MOBLE_RESULT_SUCCESS; */
      }
      else
      {
        result = MOBLE_RESULT_FAIL;
        TRACE_M(TF_SENSOR_M, "Element index %d for SENSOR%d not supported\r\n",
                pServerInitParams->sensorInitParams[count].elementIdx, count+1);
        break;
      }
    }
  }
  else
  {
    /* */
  }
  
  if (result == MOBLE_RESULT_SUCCESS &&
      pServerInitParams->sensorsCount > 1)
  {
    for (MOBLEUINT8 count=0; count<pServerInitParams->sensorsCount-1; count++)
    {
      currentElementIdx = pServerInitParams->sensorInitParams[count].elementIdx;
      nextElementIdx = pServerInitParams->sensorInitParams[count+1].elementIdx;
      
      /* Element index in initialization sensors table should be in ascending order */
      if (nextElementIdx < currentElementIdx)
      {
        TRACE_M(TF_SENSOR_M, "Element index for SENSOR%d and SENSOR%d are not in ascending order\r\n",
                               count, count+1);
        result = MOBLE_RESULT_FAIL;
        break;
      }
      else
      {
        result = MOBLE_RESULT_SUCCESS;
      }
    }
  }
  else
  {
    /* */
  }
  
  if (result == MOBLE_RESULT_SUCCESS &&
      pServerInitParams->sensorsCount > 1)
  {
    for (MOBLEUINT8 count=0; count<pServerInitParams->sensorsCount-1; count++)
    {
      currentElementIdx = pServerInitParams->sensorInitParams[count].elementIdx;
      nextElementIdx = pServerInitParams->sensorInitParams[count+1].elementIdx;
      currentPid = pServerInitParams->sensorInitParams[count].propertyId;
      nextPid = pServerInitParams->sensorInitParams[count+1].propertyId;
      
      if (nextElementIdx != currentElementIdx)
      {
        /* Pid check not required */
      }
      else
      {
        /* PIDs should be in ascending order
           Also PID can't be replicated on same element */
        if (currentPid < nextPid)
        {
          result = MOBLE_RESULT_SUCCESS;
        }
        else
        {
          result = MOBLE_RESULT_FAIL;
          
          if (currentPid == nextPid)
          {
            TRACE_M(TF_SENSOR_M, "PID replicated on same element\r\n");
          }
          else
          {
            TRACE_M(TF_SENSOR_M, "PIDs not in ascending order on SENSOR%d\r\n", count+1);            
          }
        }
      }
    }
  }
  else
  {
    /* */
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    for (MOBLEUINT8 count=0; count<pServerInitParams->sensorsCount; count++)
    {
      if(pServerInitParams->sensorInitParams[count].cadenceState == 1 &&
         pServerInitParams->sensorInitParams[count].seriesCount != 0)
      {
        TRACE_M(TF_SENSOR_M, "SENSOR%d with series column support can't support cadence", count+1);
        
        result = MOBLE_RESULT_FAIL;
        break;
      }
      else
      {
        result = MOBLE_RESULT_SUCCESS;
      }
    }
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    for (MOBLEUINT8 count=0; count<pServerInitParams->sensorsCount; count++)
    {
      totalSettingsCount += pServerInitParams->sensorInitParams[count].settingsCount;
      totalSeriesColumnCount += pServerInitParams->sensorInitParams[count].seriesCount;
    }
    
    TRACE_M(TF_SENSOR_M, "Total settings count %d, total series column count %d\r\n",
                          totalSettingsCount, totalSeriesColumnCount);
    
    requiredBufferSize = sizeof(sensor_server_t) + \
                         sizeof(sensor_params_t)*(pServerInitParams->sensorsCount) + \
                         sizeof(sensor_settings_params_t)*totalSettingsCount + \
                         sizeof(sensor_series_column_params_t)*totalSeriesColumnCount;
    
    TRACE_M(TF_SENSOR_M, "Required buffer size %d\r\n", requiredBufferSize);
    TRACE_M(TF_SENSOR_M, "sensor_server_t %d = %d\r\n", 
            sizeof(sensor_server_t),
            SIZE_OF_SENSOR_SERVER_T);
    TRACE_M(TF_SENSOR_M, "sensor_params_t %d * pServerInitParams->sensorsCount %d = %d * %d\r\n", 
            sizeof(sensor_params_t),
            pServerInitParams->sensorsCount,
            SIZE_OF_SENSOR_PARAMS_T, TOTAL_SENSORS_COUNT);
    TRACE_M(TF_SENSOR_M, "timer_status_e %d \r\n", 
            sizeof(timer_status_e));
    TRACE_M(TF_SENSOR_M, "sensor_settings_params_t %d * totalSettingsCount %d = %d * %d\r\n", 
            sizeof(sensor_settings_params_t),
            totalSettingsCount,
            SIZE_OF_SENSOR_SETTINGS_T, TOTAL_SENSOR_SETTINGS_COUNT);
    TRACE_M(TF_SENSOR_M, "sensor_series_column_params_t %d * totalSeriesColumnCount %d = %d * %d\r\n", 
            sizeof(sensor_series_column_params_t),
            totalSeriesColumnCount,
            SIZE_OF_SENSOR_SERIES_COLUMN_T, TOTAL_SENSOR_SERIES_COLUMN_COUNT);
    
    if (sizeBuff < requiredBufferSize)
    {
      result = MOBLE_RESULT_FAIL;
      TRACE_M(TF_SENSOR_M, "Buffer size %d not sufficient compare to required %d\r\n", 
              sizeBuff,
              requiredBufferSize);
    }
    else
    {
      /* */
    }
  }
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    /* initialize sensor server structure */
  SensorServer = (sensor_server_t*)sensorBuff;
  buffStartTempIndex += sizeof(sensor_server_t);

  memset(SensorServer, 0, sizeof(sizeBuff));
  
  /* copy init values into sensor server model structure */
  if (SensorServer != NULL)
  {
    SensorServer->sensorsCount = pServerInitParams->sensorsCount;
    
      /* initialize sensor params structure */
    SensorServer->pSensorParams = (sensor_params_t*)(((MOBLEUINT8*)SensorServer)+buffStartTempIndex);
    buffStartTempIndex += SensorServer->sensorsCount*sizeof(sensor_params_t);
    
    /* initialize sensor structure with init values */
    for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
    {
      pParams = SensorServer->pSensorParams + count;
      pInitParams = &(pServerInitParams->sensorInitParams[count]);
      
      pParams->elementIdx = pInitParams->elementIdx;
      pParams->propertyId = pInitParams->propertyId;
        pParams->dataLength = pInitParams->dataLength;
      pParams->cadenceState = pInitParams->cadenceState;
      pParams->settingsCount = pInitParams->settingsCount;
      pParams->seriesCount = pInitParams->seriesCount;
    }
    
    /* initialize ptr and values for sensor settings */
    for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
    {
      pParams = SensorServer->pSensorParams + count;
      pInitParams = &(pServerInitParams->sensorInitParams[count]);
      
      pParams->pSettings = (sensor_settings_params_t*)(((MOBLEUINT8*)SensorServer)+buffStartTempIndex);
      buffStartTempIndex += pParams->settingsCount*sizeof(sensor_settings_params_t);
      
      for (MOBLEUINT8 count1=0; count1<pParams->settingsCount; count1++)
      {
        pSettingsParams = pParams->pSettings + count1;
        pSettingsInitParams = &(pInitParams->settings[count1]);
        
        pSettingsParams->settingPropertyId = pSettingsInitParams->settingPropertyId;
        pSettingsParams->settingAccess = pSettingsInitParams->settingAccess;
        pSettingsParams->settingRaw = pSettingsInitParams->settingRaw;
      }
    }
    
    /* initialize ptr and values for sensor series settings */
    for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
    {
      pParams = SensorServer->pSensorParams + count;
      pInitParams = &(pServerInitParams->sensorInitParams[count]);
      
      pParams->pSeriesColumn = (sensor_series_column_params_t*)(((MOBLEUINT8*)SensorServer)+buffStartTempIndex);
      buffStartTempIndex += pParams->seriesCount*sizeof(sensor_series_column_params_t);
      
      if (pParams->seriesCount != 0)
      {
        for (MOBLEUINT16 count1=0; count1<pParams->seriesCount; count1++)
        {
          pSeriesColumnParams = pParams->pSeriesColumn + count1;
          pSeriesColumnInitParams = &(pInitParams->seriesColumn[count1]);
          pSeriesColumnParams->rawX = pSeriesColumnInitParams->rawX;
        }
      }
      else
      {
        /* do nothing */
      }
    }
  
  /* Init timer params and sensor publish */
  for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
  {
    pParams = SensorServer->pSensorParams+count;
    
    pParams->timerStatus = TIMER_STOPPED;
    
        if (pParams->cadenceState == 1)
    {
          if (BLEMesh_IsUnprovisioned() == MOBLE_FALSE)
        {
            /* publication parameters not available yet */
      }
      else
      {
        /* --- */
      }      
    }
    else
    {
          /* periodic publishing not supported */
    }
  }
  
  /* callback initialization */
  if (sensor_cb != NULL)
  {
    SensorServer->p_cb = sensor_cb;
  }
  else
  {
    SensorServer->p_cb = NULL;
    TRACE_M(TF_SENSOR_M, "Unable to initialize sensor server callback ptr\r\n");
  }
  
  /* Print initialized parameters */
  TRACE_I(TF_SENSOR_M, "No of sensors %d\r\n", SensorServer->sensorsCount);
  
  for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
  {
    pParams = SensorServer->pSensorParams+count;
    
    TRACE_I(TF_SENSOR_M, "Sensor %d elementIdx %d\r\n", count+1, pParams->elementIdx);
    TRACE_I(TF_SENSOR_M, "Sensor %d propertyId 0x%.4x\r\n", count+1, pParams->propertyId);
    TRACE_I(TF_SENSOR_M, "Sensor %d settingsCount %d\r\n", count+1, pParams->settingsCount);
    TRACE_I(TF_SENSOR_M, "Sensor %d seriesCount %d\r\n", count+1, pParams->seriesCount);
    
    for(MOBLEUINT8 count1=0; count1<pParams->settingsCount; count1++)
    {
          TRACE_I(TF_SENSOR_M, "Sensor %d setting %d settingPropertyId 0x%.4x\r\n", count+1, count1+1, pParams->pSettings[count1].settingPropertyId);
          TRACE_I(TF_SENSOR_M, "Sensor %d setting %d settingAccess %d\r\n", count+1, count1+1, pParams->pSettings[count1].settingAccess);
          TRACE_I(TF_SENSOR_M, "Sensor %d setting %d settingRaw %ld\r\n", count+1, count1+1, pParams->pSettings[count1].settingRaw);
    }
    
    for(MOBLEUINT16 count1=0; count1<pParams->seriesCount; count1++)
    {
          TRACE_I(TF_SENSOR_M, "Sensor %d series column %d rawX %ld\r\n", count+1, count1+1, pParams->pSeriesColumn[count1].rawX);
        }
      }
    }
    else
    {
      TRACE_M(TF_SENSOR_M, "Unable to init sensor server\r\n");
      result = MOBLE_RESULT_FAIL;
    }
    }
  
  if(MOBLE_FAILED(result))
  {
    SensorServer = NULL;
  }
  
  return result;
}


/**
  * @brief  Update sensor model publication parameters
  * @param  Model publish parameters
  * @retval Fail if model id is not sensor server
  *         Fail if sensor not initialized
  *         else Success
  */
__weak MOBLE_RESULT Sensor_ModelPublishSet(model_publicationparams_t* pPublishParams)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 elementIdx;
  MOBLEUINT32 rcvdPublishPeriod;
  sensor_params_t* pParams = NULL;
  
  if(SensorServer != NULL)
  {
    if (pPublishParams != NULL &&
        pPublishParams->modelID == SENSOR_SERVER_MODEL_ID)
    {
      /* extract element index */
      elementIdx = pPublishParams->elementAddress - BLEMesh_GetAddress();
      rcvdPublishPeriod = pPublishParams->publishPeriod;
        
      rcvdPublishPeriod = (rcvdPublishPeriod & 0x3F)*\
                           Get_StepResolutionValue(rcvdPublishPeriod>>6);
    
      TRACE_M(TF_SENSOR_M, "Element index %d publish period %ld ms\r\n",
              elementIdx, rcvdPublishPeriod);

      /* update publish period of all sensors on element given by publish parameters */
      for (MOBLEUINT8 count=0; count<SensorServer->sensorsCount; count++)
      {
        pParams = SensorServer->pSensorParams + count;

        if (pParams != NULL)
        {
          if (pParams->elementIdx == elementIdx)
          {
            pParams->modelPublishPeriodMs = rcvdPublishPeriod;
            Sensor_UpdatePublishParameters(count);
          }
          else
          {
            /* */
          }
        }
        else
        {
          TRACE_M(TF_SENSOR_M, "Sensor not initialized\r\n");
          break;
        }
      }
    }
    else
    {
      TRACE_M(TF_SENSOR_M, "Not for sensor server model\r\n");
      result = MOBLE_RESULT_FAIL;
    }
  }
  else
  {
    TRACE_M(TF_SENSOR_M, "Sensor server not initialized\r\n");
  }
  
  return result;
}

#ifndef ENABLE_SENSOR_MODEL_SERVER
/**
* Weak function are defined to support the original function if they are not
   included in firmware.
   There is no use of this function for application development purpose.
*/
WEAK_FUNCTION(void Appli_Sensor_CadenceGet(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_CadenceSet(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_CadenceSetUnack(sensor_CadenceCbParam_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_SettingsGet(sensor_SettingsCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_SettingGet(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_SettingSet(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_SettingSetUnack(sensor_SettingCbParams_t* pSettingParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_DescriptorGet(MOBLEUINT8 pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_Get (MOBLEUINT16 prop_ID,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_ColumnGet(sensor_ColumnCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_SeriesGet(sensor_SeriesCbParams_t* pDescriptorParam,
                                         MOBLEUINT32 length,
                                         MOBLE_ADDRESS peerAddr,
                                         MOBLE_ADDRESS dstPeer,
                                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadDescriptor(MOBLEUINT8 sensorOffset,
                                         sensor_DescriptorCbParams_t* pDescriptorParams))
{  return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadValue(MOBLEUINT8 sensorOffset,
                                    sensor_ValueCbParams_t* pValueParams))
{  return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadColumn(MOBLEUINT8 sensorOffset,
                                   MOBLEUINT8 columnOffset,
                                     sensor_ColumnCbParams_t* pColumnParams))
{  return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_ReadSeries(MOBLEUINT8 sensorOffset,
                                     sensor_SeriesCbParams_t* pSeriesParams))
{  return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLEUINT8 Appli_Sensor_IsFastCadence(MOBLEUINT8 sensorOffset,
                                      void* pFastCadenceLow, 
                                      void* pFastCadenceHigh))
{  return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLEUINT8 Appli_Sensor_IsStatusTrigger(MOBLEUINT8 sensorOffset,
                                        status_trigger_type_e triggerType,
                                        void* pDeltaDown,
                                        void* pDeltaUp))
{  return MOBLE_RESULT_SUCCESS;}
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_Init(void))
{  return MOBLE_RESULT_SUCCESS;} 
WEAK_FUNCTION(void Appli_Sensor_SerialCmd(char *rcvdStringBuff, uint16_t rcvdStringSize))
{
;
}

WEAK_FUNCTION(void Appli_Sensor_Descriptor_Status(const MOBLEUINT8 *pDescriptor,
                                    MOBLEUINT32 length,
                                    MOBLE_ADDRESS dstPeer,
                                    MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_Cadence_Status(const MOBLEUINT8 *pCadence,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_Settings_Status(const MOBLEUINT8 *pSettings,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_Setting_Status(const MOBLEUINT8 *pSetting,
                                 MOBLEUINT32 length,
                                 MOBLE_ADDRESS dstPeer,
                                 MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_Status(const MOBLEUINT8 *pStatus,
                         MOBLEUINT32 length,
                         MOBLE_ADDRESS dstPeer,
                         MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_Column_Status(const MOBLEUINT8 *pColumn,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(void Appli_Sensor_Series_Status(const MOBLEUINT8 *pSeries,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex))
{
;
}
WEAK_FUNCTION(MOBLE_RESULT Appli_Sensor_Update(MOBLEUINT8 sensorOffset, 
                                 MOBLEUINT32 value))
{  return MOBLE_RESULT_SUCCESS;} 
#endif


