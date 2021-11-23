/**
  ******************************************************************************
  * @file    appli_sensor.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Sensor Mesh Models 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "types.h"
#include "sensor_cfg_usr.h"
#include "sensors.h"
#include "appli_sensor.h"
#include "appli_light_lc.h"
#include "mesh_cfg.h"
//#include "LPS25HB.h"
#include "string.h"
#include "common.h"
#include "math.h"
#include "app_stts22h.h"

/** @addtogroup ST_BLE_Mesh
 *  @{
 */

/** @addtogroup Application_Mesh_Models
 *  @{
 */

MOBLE_RESULT Appli_Sensor_ValueSet(MOBLEUINT8 sensorOffset, 
                                   MOBLEUINT32 value);

/* Private variables ---------------------------------------------------------*/

/* ALIGN(4) */
__attribute__((aligned(4)))const sensor_server_init_params_t SensorServerInitParams = SENSOR_SERVER_INIT_PARAMS;

/* ALIGN(4) */
__attribute__((aligned(4)))const sensor_server_cb_t AppliSensor_cb = 
{
  /* Sensor Model callbacks */
  Appli_Sensor_CadenceGet,
  Appli_Sensor_CadenceSet,
  Appli_Sensor_CadenceSetUnack,
  Appli_Sensor_SettingsGet,
  Appli_Sensor_SettingGet,
  Appli_Sensor_SettingSet,
  Appli_Sensor_SettingSetUnack,
  Appli_Sensor_DescriptorGet,
  Appli_Sensor_Get,
  Appli_Sensor_ColumnGet,
  Appli_Sensor_SeriesGet,
  Appli_Sensor_ReadDescriptor,
  Appli_Sensor_ReadValue,
  Appli_Sensor_ReadColumn,
  Appli_Sensor_ReadSeries,
  Appli_Sensor_IsFastCadence,
  Appli_Sensor_IsStatusTrigger
};

#if 0
/**
  * @brief Pressure sensor init
  */
__attribute__((aligned(4)))const PRESSURE_InitTypeDef Lps25InitParams =
{
  LPS25HB_ODR_1Hz,
  LPS25HB_BDU_READ, 
  LPS25HB_DIFF_ENABLE,  
  LPS25HB_SPI_SIM_3W,  
  LPS25HB_P_RES_AVG_32,
  LPS25HB_T_RES_AVG_16 
};
#endif

/**
  * @brief Variables for people 
  */
MOBLEUINT8 AppliSensorReadFromSensor = 0; /* Used for PTS testing */
MOBLEUINT8 PresentTemperatureValue = 0;
MOBLEUINT8 PreviousTemperatureValue = 0;
MOBLEUINT8 Occupancy_Flag = MOBLE_FALSE;
extern MOBLEUINT8 NumberOfElements;
extern MOBLEUINT8 ProvisionFlag;
MOBLEUINT8 Sensor_Setting_Access  = 0x01 ;
MOBLEUINT32 PresentPeopleCount = 0;
MOBLEUINT32 PreviousPeopleCount = 0;

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
#ifdef ENABLE_SENSOR_MODEL_SERVER
/******************************************************************************/


/**
  * @brief  Callback corresponding to Sensor_CadenceGet_cb
  * @param  Cadence parameters
* @retval None
*/ 
void Appli_Sensor_CadenceGet(sensor_CadenceCbParam_t* pCadenceParam,
                             MOBLEUINT32 length,
                             MOBLE_ADDRESS peerAddr,
                             MOBLE_ADDRESS dstPeer,
                             MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X  element index %X  peer addr %X dst peer %X\r\n",
                      pCadenceParam->property_ID, elementIndex, peerAddr, dstPeer);
}
  

/**
  * @brief  Callback corresponding to Sensor_CadenceSet_cb
  * @param  Cadence parameters
* @retval None
*/ 
void Appli_Sensor_CadenceSet(sensor_CadenceCbParam_t* pCadenceParam,
                             MOBLEUINT32 length,
                             MOBLE_ADDRESS peerAddr,
                             MOBLE_ADDRESS dstPeer,
                             MOBLEUINT8 elementIndex)                                    
{
  TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X dst peer %X\r\n",
                      pCadenceParam->property_ID, elementIndex, peerAddr, dstPeer); 
}


/**
  * @brief  Callback corresponding to Sensor_CadenceSetUnack_cb
  * @param  Cadence parameters
* @retval None
*/ 
void Appli_Sensor_CadenceSetUnack(sensor_CadenceCbParam_t* pCadenceParam,
                                  MOBLEUINT32 length,
                                  MOBLE_ADDRESS peerAddr,
                                  MOBLE_ADDRESS dstPeer,
                                  MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X dst peer %X\r\n",
                      pCadenceParam->property_ID, elementIndex, peerAddr, dstPeer);
}


/**
  * @brief  Callback corresponding to Sensor_SettingsGet_cb
  * @param  Setting parameters
* @retval None
*/ 
void Appli_Sensor_SettingsGet(sensor_SettingsCbParams_t* pSettingParam,
                              MOBLEUINT32 length,
                              MOBLE_ADDRESS peerAddr,
                              MOBLE_ADDRESS dstPeer,
                              MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X dst peer %X\r\n",
                      pSettingParam->propertyId, elementIndex, peerAddr, dstPeer); 
}
                                     
/**
  * @brief  Callback corresponding to Sensor_SettingGet_cb
  * @param  Setting parameters
* @retval None
      */
void Appli_Sensor_SettingGet(sensor_SettingCbParams_t* pSettingParam,
                             MOBLEUINT32 length,
                             MOBLE_ADDRESS peerAddr,
                             MOBLE_ADDRESS dstPeer,
                             MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X dst peer %X\r\n",
                      pSettingParam->property_ID, elementIndex, peerAddr, dstPeer);
}
      
      
/**
  * @brief  Callback corresponding to Sensor_SettingSet_cb
  * @param  Setting parameters
* @retval None
*/ 
void Appli_Sensor_SettingSet(sensor_SettingCbParams_t* pSettingParam,
                             MOBLEUINT32 length,
                             MOBLE_ADDRESS peerAddr,
                             MOBLE_ADDRESS dstPeer,
                             MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X dst peer %X\r\n",
                      pSettingParam->property_ID, elementIndex, peerAddr, dstPeer);
}  
        
      
/**
  * @brief  Callback corresponding to Sensor_SettingSetUnack_cb
  * @param  Setting parameters
* @retval None
*/
void Appli_Sensor_SettingSetUnack(sensor_SettingCbParams_t* pSettingParam,
                                  MOBLEUINT32 length,
                                  MOBLE_ADDRESS peerAddr,
                                  MOBLE_ADDRESS dstPeer,
                                  MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X dst peer %X\r\n",
                      pSettingParam->property_ID, elementIndex, peerAddr, dstPeer);
}

                                    
/**
  * @brief  Callback corresponding to Sensor_DescriptorGet_cb
  * @param  Descriptor parameters
* @retval None
*/ 
void Appli_Sensor_DescriptorGet(MOBLEUINT8 prop_ID,
                                MOBLEUINT32 length,
                                MOBLE_ADDRESS peerAddr,
                                MOBLE_ADDRESS dstPeer,
                                MOBLEUINT8 elementIndex)
{
  if(length == 0)
  {
    TRACE_M(TF_SENSOR, "Sensor Descriptor data for all sensors on element index %X peer addr %X, dst peer %X\r\n",
                        elementIndex, peerAddr, dstPeer);
  }
  else
  {
    TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X, dst peer %X\r\n",
                        prop_ID, elementIndex, peerAddr, dstPeer);
  }
}
      
      
/**
  * @brief  Callback corresponding to Sensor_Get_cb
  * @param  Get parameters
* @retval None
*/ 
void Appli_Sensor_Get(MOBLEUINT16 prop_ID,
                      MOBLEUINT32 length,
                      MOBLE_ADDRESS peerAddr,
                      MOBLE_ADDRESS dstPeer,
                      MOBLEUINT8 elementIndex)
{
  if(length == 0)
  {
    TRACE_M(TF_SENSOR, "Sensor Data for all sensors on element index %X peer addr %X dst peer %X\r\n",
                       elementIndex, peerAddr, dstPeer);
  }
  else
  {
    TRACE_M(TF_SENSOR, "Property ID %X element index %X peer addr %X dst peer %X \r\n",
                      prop_ID, elementIndex, peerAddr, dstPeer); 
  }
}
      
  
/**
  * @brief  Callback corresponding to Sensor_ColumnGet_cb
  * @param  Column parameters
* @retval None
*/
void Appli_Sensor_ColumnGet(sensor_ColumnCbParams_t* pColumnParam,
                            MOBLEUINT32 length,
                            MOBLE_ADDRESS peerAddr,
                            MOBLE_ADDRESS dstPeer,
                            MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X  element index %X peer addr %X dst peer %X\r\n",
                      pColumnParam->propertyId, elementIndex, peerAddr, dstPeer);
}


/**
  * @brief  Callback corresponding to Sensor_SeriesGet_cb
  * @param  Series parameters
* @retval None
*/ 
void Appli_Sensor_SeriesGet(sensor_SeriesCbParams_t* pSeriesParam,
                            MOBLEUINT32 length,
                            MOBLE_ADDRESS peerAddr,
                            MOBLE_ADDRESS dstPeer,
                            MOBLEUINT8 elementIndex)
{
  TRACE_M(TF_SENSOR, "Property ID %X  Raw Value X1 %ld  Raw Value X2 %ld\r\n",
                      pSeriesParam->propertyId,
                      pSeriesParam->rawValueX1,
                      pSeriesParam->rawValueX2);  
}


/**
  * @brief  Callback corresponding to Sensor_ReadDescriptor_cb
  *         Update SENSORX descriptor identified by sensorOffset
  *         Read from application to optimize RAM
  * @param  Sensor offset in sensor init structure
  * @param  descriptor parameters
  * @retval Fail if sensor doesn't exist or property ID mismatch occurs
  *         else Success
  */
MOBLE_RESULT Appli_Sensor_ReadDescriptor(MOBLEUINT8 sensorOffset,
                                         sensor_DescriptorCbParams_t* pDescriptorParams)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  const sensor_init_params_t* pSensorInitParams = NULL;
  
  if (sensorOffset < SensorServerInitParams.sensorsCount)
  {
    pSensorInitParams = &(SensorServerInitParams.sensorInitParams[sensorOffset]);
    
    if (pSensorInitParams->propertyId != pDescriptorParams->propertyId)
    {
      result = MOBLE_RESULT_FAIL;
    }
    else
    {
      /* */
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }
  
  if (pSensorInitParams != NULL &&
      pDescriptorParams != NULL)
  {
    pDescriptorParams->positiveTolerance = pSensorInitParams->positiveTolerance;
    pDescriptorParams->negativeTolerance = pSensorInitParams->negativeTolerance;
    pDescriptorParams->samplingFunction = pSensorInitParams->samplingFunction;
    pDescriptorParams->measurementPeriod = pSensorInitParams->measurementPeriod;
    pDescriptorParams->updateInterval = pSensorInitParams->updateInterval;
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }

  return result;
}


/**
  * @brief  Callback corresponding to Sensor_ReadValue_cb
  *         Read sensor value and update buffer
  *         data length <= 128
  *         PreviousTemperatureValue, PresentTemperatureValue  to be updated 
  *         every time sensor value is changed and call to Sensor_UpdateCadence
  * @param  Sensor offset in sensor init structure
  * @param  Value parameters
  * @retval Fail if sensor doesn't exist
  *         else Success
  */
MOBLE_RESULT Appli_Sensor_ReadValue(MOBLEUINT8 sensorOffset,
                                    sensor_ValueCbParams_t* pValueParams)
{
#if 0
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
//  PRESSURE_StatusTypeDef tempStatus = PRESSURE_OK;
  MOBLEINT16 temp = 0;
  MOBLEINT8 temperature8 = 0;
  float pressure = 0;
  
  /* sensor offset exist */
  if (sensorOffset < SensorServerInitParams.sensorsCount)
  {
    if (sensorOffset == 0) /* Present Ambient Temperature */
    {
      if(AppliSensorReadFromSensor == 0) /* Normal mode */
      {
        /* Temperature, Temperature8 format, M=1, d=0, b=-1 */
//        tempStatus = LPS25HB_I2C_ReadRawTemperature(&temp);
//        if (tempStatus == PRESSURE_OK)
        {
          TRACE_M(TF_SENSOR, "Temperature sensor raw value %d\r\n" , temp);
        
          /* Convert temperature raw value to Temperature8 format */
          temp = (temp/240) + 85;
        
          if (temp < -64*2)
          {
            temp = -64*2;
          }
          else if (temp > 63.5*2)
          {
            temp = 63.5*2;
          }
        
          temperature8 = temp;
        
          pValueParams->data[0] = (MOBLEUINT8)temperature8;

          if (pValueParams->data[0] == 0xFF)
          {
            /* 0xFF is unknown but here it is -1
            -1 is approximated to 0 */
            pValueParams->data[0] = 0x00;
          }
        
          TRACE_M(TF_SENSOR, "Temperature8 raw value %d, actual value %f\r\n", 
                              temperature8, (float)temperature8/2);
        }
#if 0
        else /* error */
        {
          pValueParams->data[0] = 0xFF;
        }
#endif
      }
      else /* Value not to be read from sensor */
      {
        pValueParams->data[0] = PresentTemperatureValue;
      }
    }
    else if (sensorOffset == 1) /* Pressure */
    {
//      tempStatus = LPS25HB_GetPressure(&pressure);
//      if (tempStatus == PRESSURE_OK)
      {
        TRACE_M(TF_SENSOR, "Pressure sensor value %f mbar\r\n" , pressure);
      
        memcpy(pValueParams->data, (void*)&pressure, 4);
      }
#if 0
      else /* error */
      {
        memset(pValueParams->data, 0, 4);
      }
#endif
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }
  
  return result;
#else
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  TRACE_M(TF_SENSOR_M, ">>> \r\n");

  switch(SensorServerInitParams.sensorInitParams[sensorOffset].propertyId)
  {
    case PRESENT_AMBIENT_TEMPERATURE_PID:
      {
        float temp_val;
        STTS22H_getTemperatureValue(&temp_val);
        pValueParams->data[0] = (MOBLEUINT8)round(temp_val*2);
      }
      break;
      
    default:
      break;
  }   

  return result;
#endif
}
  
#endif
  
      
/**
  * @brief  Callback corresponding to Sensor_ReadColumn_cb
  *         Fill column width and raw valye Y in data buffer
  *         data length <= 8
  * @param  Sensor offset in sensor init structure
  * @param  Series column parameters
  * @retval Fail if sensor doesn't exist
  *         else Success
*/  
MOBLE_RESULT Appli_Sensor_ReadColumn(MOBLEUINT8 sensorOffset,
                                     MOBLEUINT8 columnOffset,
                                     sensor_ColumnCbParams_t* pColumnParams)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  const sensor_init_params_t* pSensorInitParams = NULL;
  MOBLEUINT8 dataLength = 0;
  MOBLEUINT8* data = pColumnParams->data;
  
  if (sensorOffset < SensorServerInitParams.sensorsCount)
  {
    pSensorInitParams = &(SensorServerInitParams.sensorInitParams[sensorOffset]);
    
    /* fill sensor column data */
    memcpy(data+dataLength, 
           &(pSensorInitParams->seriesColumn[columnOffset].columnWidth), 
           pSensorInitParams->dataLength);
    dataLength = pSensorInitParams->dataLength;
    
    memset(data+dataLength, 0xAA, pSensorInitParams->dataLength);
    dataLength += pSensorInitParams->dataLength;
    
    pColumnParams->dataLength = dataLength;
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }
  
  return result;
}


/**
  * @brief  Callback corresponding to Sensor_ReadSeries_cb
  *         Fill sensor series state for all columns between and including X1 and X2
  *         Series data to be concatenated in triplet of raw value X, column width and raw value Y
  *         X[n] CW[n] Y[n] X[n+1] CW[n+1] Y[n+1] ...
  *         data length should be less than minimum of 379 or max application packet length supported
  * @param  Sensor offset in sensor init structure
  * @param  Series parameters
  * @retval Fail if sensor doesn't exist
  *         else Success
  */
MOBLE_RESULT Appli_Sensor_ReadSeries(MOBLEUINT8 sensorOffset,
                                     sensor_SeriesCbParams_t* pSeriesParams)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  const sensor_init_params_t* pSensorInitParams = NULL;
  MOBLEUINT16 dataLength = 0;
  MOBLEUINT8* data = pSeriesParams->data;
  
  if (sensorOffset < SensorServerInitParams.sensorsCount)
  {
    pSensorInitParams = &(SensorServerInitParams.sensorInitParams[sensorOffset]);
    
    for (MOBLEUINT8 count=0; count<pSensorInitParams->seriesCount; count++)
    {
      /* fill sensor series data */
      if (pSensorInitParams->seriesColumn[count].rawX >= pSeriesParams->rawValueX1 &&
          pSensorInitParams->seriesColumn[count].rawX <= pSeriesParams->rawValueX2)
      {
        memcpy(data+dataLength, 
               &(pSensorInitParams->seriesColumn[count].rawX), 
               pSensorInitParams->dataLength);
        dataLength += pSensorInitParams->dataLength;
        memcpy(data+dataLength, 
               &(pSensorInitParams->seriesColumn[count].columnWidth), 
               pSensorInitParams->dataLength);
        dataLength += pSensorInitParams->dataLength;
        memset(data+dataLength, 0xAA, pSensorInitParams->dataLength);
        dataLength += pSensorInitParams->dataLength;
        
        pSeriesParams->dataLength = dataLength;
      }
      else
      {
        /* */
      }
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  }
  
  return result;
}
  
  
/**
  * @brief  Callback corresponding to Sensor_IsFastCadence_cb
  *         To check if fast cadence to be used for current sensor state
  * @param  Sensor offset in sensor init structure
  * @param  Fast Cadence Low
  * @param  Fast Cadence High
  * @retval Trigger status
  */
MOBLEUINT8 Appli_Sensor_IsFastCadence(MOBLEUINT8 sensorOffset,
                                      void* pFastCadenceLow, 
                                      void* pFastCadenceHigh)
{
  MOBLEUINT8 fastCadenceStatus = 0;
  MOBLEUINT32 fastCadenceLow = *((MOBLEUINT32*)pFastCadenceLow);
  MOBLEUINT32 fastCadenceHigh = *((MOBLEUINT32*)pFastCadenceHigh);
  MOBLEUINT32 sensorValue = PresentTemperatureValue;
   
  if (sensorOffset == 0)
  {
    if (fastCadenceLow <= fastCadenceHigh)
  {
      fastCadenceStatus = sensorValue >= fastCadenceLow &&
                          sensorValue <= fastCadenceHigh;
    }
    else
    {
      fastCadenceStatus = sensorValue > fastCadenceLow ||
                          sensorValue < fastCadenceHigh;
  }
}
  else
  {
    /* Implemented only for sensor at offset 0 */
  }
  
  return fastCadenceStatus;
}


/**
  * @brief  Callback corresponding to Sensor_IsStatusTrigger_cb
  *         To check if sensor change in sensor state (delta) is more than
  *         given trigger state value 
  *         delta up values
  * @param  Sensor offset in sensor init structure
  * @param  Trigger type -> value or percent change
  * @param  Status trigger delta down
  * @param  Status trigger delta up
  * @retval Trigger status
*/ 
MOBLEUINT8 Appli_Sensor_IsStatusTrigger(MOBLEUINT8 sensorOffset,
                                        status_trigger_type_e triggerType,
                                        void* pDeltaDown,
                                        void* pDeltaUp)
{
  const sensor_init_params_t* pSensorInitParams = NULL;
  MOBLEUINT8 triggerStatus = 0;
  MOBLEUINT32 deltaDown = *((MOBLEUINT32*)pDeltaDown);
  MOBLEUINT32 deltaUp = *((MOBLEUINT32*)pDeltaUp);
  status_trigger_delta_e statusTriggerDelta;
  MOBLEUINT32 delta;
  
  if (sensorOffset == 0)
  {
    if (PresentTemperatureValue < PreviousTemperatureValue)
    {
      /* status trigger delta down */
      statusTriggerDelta = STATUS_TRIGGER_DELTA_DOWN;
      delta = PreviousTemperatureValue - PresentTemperatureValue;

      TRACE_M(TF_SENSOR, "Delta down value %ld\r\n", delta);
    }
    else
    {
      /* status trigger delta up */
      statusTriggerDelta = STATUS_TRIGGER_DELTA_UP;
      delta = PresentTemperatureValue - PreviousTemperatureValue;

      TRACE_M(TF_SENSOR, "Delta up value %ld\r\n", delta);
    }
  
    if (triggerType == STATUS_TRIGGER_TYPE_PC)
    {
      pSensorInitParams = &(SensorServerInitParams.sensorInitParams[sensorOffset]);
    
      if (pSensorInitParams->valuesRange != 0)
      {
        /* change delta to percentage change (of 0.01 % steps) */
        delta = (MOBLEUINT32)((delta*10000)/pSensorInitParams->valuesRange);
      }
      else
      {
        triggerStatus = 0;
      }
    }
  
    if ((statusTriggerDelta == STATUS_TRIGGER_DELTA_DOWN && delta >= deltaDown) ||
        (statusTriggerDelta == STATUS_TRIGGER_DELTA_UP && delta >= deltaUp))
    {
      triggerStatus = 1;
    }
    else
    {
      triggerStatus = 0;
    }
  }

  return triggerStatus;
}
  
  
/**
* @brief  Initialize hardware interfaces for sensors and mesh sensor model structures
*         Sensor init parameters to be defined in sensor_cfg_usr.h
*         Sensors are initialized in the order as defined in sensor_cfg_usr.h
  *         LPS25HB supports pressure and temperature sensor
* @param  void 
  * @retval 
  */
MOBLE_RESULT Appli_Sensor_Init(void)                                        
  {
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
//  PRESSURE_StatusTypeDef lps25Status;
    
  /* Hardware interface initialization */
#ifndef CUSTOM_BOARD_PWM_SELECTION  
//  PRESSURE_StatusTypeDef tempStatus;
//  PRESSURE_StatusTypeDef pressStatus;
//  int16_t temperature;
//  int32_t pressure;
  
#if 0
  /* Initiallization of sensors */
  lps25Status = LPS25HB_Init((PRESSURE_InitTypeDef*)(&Lps25InitParams));
        
  if (lps25Status != PRESSURE_OK)
  {
    TRACE_M(TF_SENSOR, "Error initializing LPS25HB. status (%d)\r\n", lps25Status);
  }
    
  tempStatus = LPS25HB_I2C_ReadRawTemperature(&temperature);
  pressStatus = LPS25HB_I2C_ReadRawPressure(&pressure);

  if (tempStatus == PRESSURE_OK &&
      pressStatus == PRESSURE_OK)
  {
    /* TRACE_M(TF_SENSOR, "Raw temperature (%d) and raw pressure (%d)\r\n",
                        temperature, pressure); */
  }
  else
  {
    TRACE_M(TF_SENSOR, "Error reading LPS25HB at init\r\n");
  }
#endif
  
#else
  TRACE_M(TF_SENSOR, "LPS25HB not initialized\r\n");
#endif

  /* initialize sensor server model */
  result = SensorServer_Init(&BufferSensorServer, 
                              &AppliSensor_cb,
                              TOTAL_SENSOR_SERVER_BUFF_SIZE, 
                              &SensorServerInitParams);
      
  if(MOBLE_FAILED(result))
  {
    TRACE_M(TF_SENSOR, "Sensor Server init failed\r\n");
  }
  
  return result; 
  }


/**
  * @brief  Handling of serial inputs to sensor model
  * @param  Sensor offset in sensor init structure
  * @param  Sensor value
  * @retval void
  */
MOBLE_RESULT Appli_Sensor_Update(MOBLEUINT8 sensorOffset, MOBLEUINT32 value)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  AppliSensorReadFromSensor = 1;
  
  /* Update previous with current and current with new */
  PreviousTemperatureValue = PresentTemperatureValue;
  PresentTemperatureValue = (MOBLEUINT8)value;
  
  /* To update cadence parameters
     results in either status trigger or (and) fast cadence based on change and present
     Application may decide not to update cadence parameters if senosr value is changing
     very frequently in some cases to save bandwidth */
  if (PresentTemperatureValue != PreviousTemperatureValue)
  {
    result = Sensor_UpdateCadence(sensorOffset, 
                  SensorServerInitParams.sensorInitParams[0].elementIdx, 
                                  SensorServerInitParams.sensorInitParams[0].propertyId);
  }
  
  return result;
      }


/**
  * @brief  Handling of serial inputs to sensor model
  *         Appli_Sensor_SerialCmd can be used for testing periodic publishing and 
  *         triggered publishing with PTS
  * @param  serial string
  * @param  serial string size
  * @retval void
  */
void Appli_Sensor_SerialCmd(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  MOBLE_RESULT result = MOBLE_RESULT_INVALIDARG;
  MOBLEUINT16 value = 0;
  MOBLEUINT8 sensorOffset = 0;
  
  if (!strncmp(rcvdStringBuff+6, "SETV", 4))
  {
    if (rcvdStringSize == 15)
    {
      sscanf(rcvdStringBuff+11, "%4hx", &value);
      
      /* Set SENSOR1 value at offset 0 */
      result = Appli_Sensor_Update(sensorOffset, value);
    }
    else
    {
      result = MOBLE_RESULT_FAIL;
      BLEMesh_PrintStringCb("Invalid size of string\r\n");
    }
  }
  else if (!strncmp(rcvdStringBuff+6, "PUBLISH", 7))
  {
    if (rcvdStringSize == 17)
    {
      sscanf(rcvdStringBuff+14, "%1hx", &value);
      
      sensorOffset = value;
      
      sscanf(rcvdStringBuff+16, "%1hx", &value);
      
      /* Enable / Disable publishing of sensor as identified by sensor offset */
      if(value == 0) /* Disable */
      {
        result = Sensor_UpdatePublishState(sensorOffset, 0);
      }
      else /* Enable */
      {
        result = Sensor_UpdatePublishState(sensorOffset, 1);
      }
    }
    else
    {
      result = MOBLE_RESULT_FAIL;
      BLEMesh_PrintStringCb("Invalid size of string\r\n");
    }
  }
  else
  {
    result = MOBLE_RESULT_FAIL;
  } 
  
  /* Check the result of command processing */
  if(result == MOBLE_RESULT_SUCCESS)
  {
    BLEMesh_PrintStringCb("Success\r\n");
  }
  else if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
    BLEMesh_PrintStringCb("Fail Out of memory\r\n");  
  }
  else if(result == MOBLE_RESULT_INVALIDARG)
  {
    BLEMesh_PrintStringCb("Fail Invalid Argument\r\n");  
  }
  else
  {       
    BLEMesh_PrintStringCb("Fail\r\n");   
  }
}


/**
 * @}
 */

/**
 * @}
 */


