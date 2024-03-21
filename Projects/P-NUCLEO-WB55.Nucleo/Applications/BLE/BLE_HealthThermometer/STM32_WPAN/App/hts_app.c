/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hts_app.c
  * @author  MCD Application Team
  * @brief   Health Thermometer Service Application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "dbg_trace.h"
#include "app_ble.h"
#include "ble.h"
#include "hts_app.h"
#include <time.h>
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  HTS_TemperatureValue_t TemperatureMeasurementChar;
#if(BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 1)
  HTS_Temperature_Type_t TemperatureTypeChar;
#endif
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)
  HTS_TemperatureValue_t IntermediateTemperatureChar;
  uint8_t TimerIntTemp_Id;
  uint8_t IntTempEnabled;
#endif
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
  uint16_t  MeasurementIntervalChar;
  uint8_t   TimerMeasInt_Id;
  uint8_t   Indication_Status;
#endif
  uint8_t TimerMeasurement_Id;
  uint8_t TimerMeasurementStarted;
} HTSAPP_Context_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
#define DEFAULT_HTS_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */
#define DEFAULT_TEMPERATURE_TYPE          TT_Armpit
#define NB_SAVED_MEASURES                                                     10
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static HTSAPP_Context_t HTSAPP_Context;
static HTS_TemperatureValue_t HTSMeasurement[NB_SAVED_MEASURES];
static int8_t HTS_CurrentIndex, HTS_OldIndex;

/**
 * END of Section BLE_APP_CONTEXT
 */

 /* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void HTSAPP_Update_TimeStamp(void);
/* USER CODE BEGIN PFP */
static uint32_t HTSAPP_Get_Pseudo_Random_Sensor_Value ( void );
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
static void HTSAPP_UpdateMeasurement( void )
{
/* USER CODE BEGIN HTSAPP_UpdateMeasurement */
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_HTS_MEAS_REQ_ID,CFG_SCH_PRIO_0);

/* USER CODE END HTSAPP_UpdateMeasurement */
  return;
}

#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)
static void HTSAPP_UpdateIntermediateTemperature( void )
{
/* USER CODE BEGIN HTSAPP_UpdateIntermediateTemperature */
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_HTS_INTERMEDIATE_TEMPERATURE_REQ_ID, CFG_SCH_PRIO_0);

/* USER CODE END HTSAPP_UpdateIntermediateTemperature */
  return;
}
#endif

#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
static void HTSAPP_UpdateMeasurementInterval( void )
{
/* USER CODE BEGIN HTSAPP_UpdateMeasurementInterval */
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_HTS_MEAS_INTERVAL_REQ_ID, CFG_SCH_PRIO_0);

/* USER CODE END HTSAPP_UpdateMeasurementInterval */
  return;
}
#endif

#if(BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
static void HTSAPP_Update_TimeStamp(void)
{
/* USER CODE BEGIN HTSAPP_Update_TimeStamp */
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Seconds += 
    10 * HTSAPP_Context.MeasurementIntervalChar;
#else
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Seconds += 10;
#endif
  while(HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Seconds >= 60)
  {
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Seconds -= 60;
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Minutes += 1;
  }
  while(HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Minutes >= 60)
  {
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Minutes -= 60;
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Hours += 1;
  }
  while(HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Hours >= 24)
  {
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Hours -= 24;
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Day += 1;
  }
  while(HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Day >= 31)
  {
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Day -= 31;
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Month += 1;
  }
  while(HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Month >= 12)
  {
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Month -= 11;
    HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Year += 1;
  }
/* USER CODE END HTSAPP_Update_TimeStamp */
}
#endif

#if(BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
static void HTSAPP_Store(void)
{
/* USER CODE BEGIN HTSAPP_Store */
  HTS_CurrentIndex++;
  if(HTS_CurrentIndex == NB_SAVED_MEASURES)
  {
    HTS_CurrentIndex = 0;
  }
  if((HTS_CurrentIndex == HTS_OldIndex) && (HTSMeasurement[HTS_CurrentIndex].MeasurementValue > 0))
  {
    HTS_OldIndex = HTS_CurrentIndex + 1;
    if(HTS_OldIndex == NB_SAVED_MEASURES)
    {
      HTS_OldIndex = 0;
    }
  }
  APP_DBG_MSG ("Stored measurement %d, index of first measure saved: %d\n", 
               HTS_CurrentIndex, HTS_OldIndex);  
  memcpy(&HTSMeasurement[HTS_CurrentIndex], 
         &(HTSAPP_Context.TemperatureMeasurementChar), 
         sizeof(HTS_TemperatureValue_t)); 
/* USER CODE END HTSAPP_Store */
}

static void HTSAPP_Suppress(void)
{
/* USER CODE BEGIN HTSAPP_Suppress */
  HTS_Update_Char(TEMPERATURE_MEASUREMENT_CHAR_UUID, 
                  (uint8_t *)&HTSMeasurement[HTS_OldIndex]);
  
  HTSMeasurement[HTS_OldIndex].MeasurementValue = 0;
  HTS_OldIndex++;
  if(HTS_OldIndex == NB_SAVED_MEASURES)
  {
    HTS_OldIndex = 0;
  }
/* USER CODE END HTSAPP_Suppress */
}
#endif

/* Public functions ----------------------------------------------------------*/

void HTS_App_Notification(HTS_App_Notification_evt_t *pNotification)
{
/* USER CODE BEGIN HTS_App_Notification */
  switch(pNotification->HTS_Evt_Opcode)
  {
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
    case HTS_MEASUREMENT_INTERVAL_RECEIVED_EVT:
      {
        if(pNotification->RangeInterval != 0)
        {
          HTSAPP_Context.MeasurementIntervalChar = pNotification->RangeInterval;
          APP_DBG_MSG("HTS_MEASUREMENT_INTERVAL_RECEIVED_EVT: %d \n", 
                          pNotification->RangeInterval);
          if(HTSAPP_Context.Indication_Status != 0)
          {
            if(HTSAPP_Context.TimerMeasurementStarted == 0)
            {
              HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, 
                          (HTSAPP_Context.MeasurementIntervalChar)*DEFAULT_HTS_MEASUREMENT_INTERVAL);
              HTSAPP_Context.TimerMeasurementStarted = 1;
            }
            else
            {
              HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
              HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, 
                          (HTSAPP_Context.MeasurementIntervalChar)*DEFAULT_HTS_MEASUREMENT_INTERVAL);
            }
          }
        }
        else
        {
          HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
          HTSAPP_Context.TimerMeasurementStarted = 0;
        }
      }
      break;
#endif

    case HTS_MEASUREMENT_IND_ENABLED_EVT:
      {
        APP_DBG_MSG("HTS_MEASUREMENT_IND_ENABLED_EVT\n");
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
        HTSAPP_Context.Indication_Status = 1;
        if(HTSAPP_Context.TimerMeasurementStarted == 0)
        {
          HTSAPP_Measurement();
          HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
          HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, 
                      (HTSAPP_Context.MeasurementIntervalChar)*DEFAULT_HTS_MEASUREMENT_INTERVAL);
          HTSAPP_Context.TimerMeasurementStarted = 1;
        }
#else
        HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
        HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, 2*DEFAULT_HTS_MEASUREMENT_INTERVAL);
        HTSAPP_Context.TimerMeasurementStarted = 1;
#endif
      }
      break;

    case HTS_MEASUREMENT_IND_DISABLED_EVT:
      {
        APP_DBG_MSG("HTS_MEASUREMENT_IND_DISABLED_EVT\n");
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
        HTSAPP_Context.Indication_Status = 0;
#endif
        HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
        HTSAPP_Context.TimerMeasurementStarted = 0;
      }
      break;

#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
    case HTS_MEASUREMENT_INTERVAL_IND_ENABLED_EVT:
      {
        APP_DBG_MSG("HTS_MEASUREMENT_INTERVAL_IND_ENABLED_EVT\n");
        HW_TS_Stop(HTSAPP_Context.TimerMeasInt_Id);
        HW_TS_Start(HTSAPP_Context.TimerMeasInt_Id, 2*DEFAULT_HTS_MEASUREMENT_INTERVAL);
      }
      break;

    case HTS_MEASUREMENT_INTERVAL_IND_DISABLED_EVT:
      {
        APP_DBG_MSG("HTS_MEASUREMENT_INTERVAL_IND_DISABLED_EVT\n");
        HW_TS_Stop(HTSAPP_Context.TimerMeasInt_Id);
      }
      break;
#endif

#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)
    case HTS_INTERMEDIATE_TEMPERATURE_NOT_ENABLED_EVT:
      {
        APP_DBG_MSG("HTS_INTERMEDIATE_TEMPERATURE_NOT_ENABLED_EVT\n");
        HTSAPP_Context.IntTempEnabled = 1;
        HW_TS_Stop(HTSAPP_Context.TimerIntTemp_Id);
        HW_TS_Start(HTSAPP_Context.TimerIntTemp_Id, DEFAULT_HTS_MEASUREMENT_INTERVAL);
      }
      break;

    case HTS_INTERMEDIATE_TEMPERATURE_NOT_DISABLED_EVT:
      {
        APP_DBG_MSG("HTS_INTERMEDIATE_TEMPERATURE_NOT_DISABLED_EVT\n");
        HTSAPP_Context.IntTempEnabled = 0;
        HW_TS_Stop(HTSAPP_Context.TimerIntTemp_Id);
      }
      break;
#endif

    default:
      break;
  }

/* USER CODE END HTS_App_Notification */
  return;
}

void HTSAPP_Init(void)
{
/* USER CODE BEGIN HTSAPP_Init */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_HTS_MEAS_REQ_ID, UTIL_SEQ_RFU, HTSAPP_Measurement);
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)
  UTIL_SEQ_RegTask( 1<< CFG_TASK_HTS_INTERMEDIATE_TEMPERATURE_REQ_ID, UTIL_SEQ_RFU, HTSAPP_IntermediateTemperature);
#endif
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
  UTIL_SEQ_RegTask( 1<< CFG_TASK_HTS_MEAS_INTERVAL_REQ_ID, UTIL_SEQ_RFU, HTSAPP_MeasurementInterval);
#endif

  /**
   * Initialize Flags
   */
  HTSAPP_Context.TemperatureMeasurementChar.Flags = (uint8_t)NO_FLAGS;
#if(BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
  HTSAPP_Context.TemperatureMeasurementChar.Flags |= (uint8_t)SENSOR_TIME_STAMP_PRESENT;
#endif
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)
  HTSAPP_Context.IntermediateTemperatureChar.Flags = (uint8_t)NO_FLAGS;
#if(BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
    HTSAPP_Context.IntermediateTemperatureChar.Flags |= (uint8_t)SENSOR_TIME_STAMP_PRESENT;
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Year = 2017;
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Month = 4;
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Day = 1;
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Hours = 0;
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Minutes = 0;
  HTSAPP_Context.TemperatureMeasurementChar.TimeStamp.Seconds  = 0;
#endif
#endif

  /**
   * Initialize Measurement Interval
   */
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
  if(BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE == 0)
    HTSAPP_Context.MeasurementIntervalChar = BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE + 10;
  else
    HTSAPP_Context.MeasurementIntervalChar = BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE + 9;
    
  HTSAPP_Context.Indication_Status = 0;
  HTS_Update_Char(MEASUREMENT_INTERVAL_CHAR_UUID, 
                  (uint8_t *)&HTSAPP_Context.MeasurementIntervalChar);
  /**
   * Create timer for Health Temperature Measurement Interval
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(HTSAPP_Context.TimerMeasInt_Id), hw_ts_Repeated, HTSAPP_UpdateMeasurementInterval);
#endif

  /**
   * Set Temperature Measurement Type
   */
#if(BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 1)
  /**
   * Temperature Type shall not be present in the flags when it is static
   */
  HTSAPP_Context.TemperatureTypeChar = DEFAULT_TEMPERATURE_TYPE;
  HTS_Update_Char(TEMPERATURE_TYPE_CHAR_UUID, 
                  (uint8_t *)&HTSAPP_Context.TemperatureTypeChar);
#else
  /**
   * Temperature Type shall be present in the flags when it is NOT static
   */
  HTSAPP_Context.TemperatureMeasurementChar.TemperatureType = DEFAULT_TEMPERATURE_TYPE;
  HTSAPP_Context.TemperatureMeasurementChar.Flags |= (uint8_t)SENSOR_TEMPERATURE_TYPE_PRESENT;
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)
  HTSAPP_Context.IntermediateTemperatureChar.Flags |= (uint8_t)SENSOR_TEMPERATURE_TYPE_PRESENT;
  HTSAPP_Context.IntTempEnabled = 0;
#endif
#endif


  /**
   * Create timer for Health Temperature Measurement
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(HTSAPP_Context.TimerMeasurement_Id), hw_ts_Repeated, HTSAPP_UpdateMeasurement);
  HTSAPP_Context.TimerMeasurementStarted = 0;
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)
  /**
   * Create timer for Health Temperature Intermediate Temperature
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(HTSAPP_Context.TimerIntTemp_Id), hw_ts_Repeated, HTSAPP_UpdateIntermediateTemperature);
#endif

  HTS_CurrentIndex = -1;
  HTS_OldIndex = 0;

/* USER CODE END HTSAPP_Init */
return;
}

void HTSAPP_Measurement(void)
{
/* USER CODE BEGIN HTSAPP_Measurement */
  uint32_t measurement;
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE == 1)

  if(HTSAPP_Context.IntTempEnabled == 1)
    measurement = HTSAPP_Context.IntermediateTemperatureChar.MeasurementValue;
  else
  {
    measurement = ((HTSAPP_Get_Pseudo_Random_Sensor_Value()) & 0x03) + 37;
  }  
#else
  measurement = ((HTSAPP_Get_Pseudo_Random_Sensor_Value()) & 0x03) + 37;
#endif

  HTSAPP_Context.TemperatureMeasurementChar.MeasurementValue = measurement;
  APP_DBG_MSG("HTSAPP_Measurement: %d \n", (int)measurement);
#if(BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
  HTSAPP_Update_TimeStamp();
#endif

  if((APP_BLE_Get_Server_Connection_Status() == APP_BLE_CONNECTED_SERVER) &&
     (HTSAPP_Context.TimerMeasurementStarted == 1))
  {
#if(BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
    if(HTSMeasurement[HTS_OldIndex].MeasurementValue > 0)
    {
      APP_DBG_MSG ("Send stored measurement %d\n", HTS_OldIndex);  
      HTSAPP_Suppress();
      HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
      HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, DEFAULT_HTS_MEASUREMENT_INTERVAL);
    }
    else
    {
      HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
      HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, DEFAULT_HTS_MEASUREMENT_INTERVAL*10);
      HTS_Update_Char(TEMPERATURE_MEASUREMENT_CHAR_UUID, 
                      (uint8_t *)&HTSAPP_Context.TemperatureMeasurementChar);
    }
#else
    HTS_Update_Char(TEMPERATURE_MEASUREMENT_CHAR_UUID, 
                    (uint8_t *)&HTSAPP_Context.TemperatureMeasurementChar);
#endif
  }  
#if(BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
  else
  {
    HTSAPP_Store();
  }
#endif

/* USER CODE END HTSAPP_Measurement */
  return;
}

void HTSAPP_IntermediateTemperature(void)
{
/* USER CODE BEGIN HTSAPP_IntermediateTemperature */
  #if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE != 0)
  uint32_t measurement;
  measurement = ((HTSAPP_Get_Pseudo_Random_Sensor_Value()) & 0x03) + 37;

  HTSAPP_Context.IntermediateTemperatureChar.MeasurementValue = measurement;
  
  if(HTSAPP_Context.IntTempEnabled == 1)
  {
    APP_DBG_MSG("HTSAPP_IntermediateTemperature: %d \n", (int)measurement);
    
    HTS_Update_Char(INTERMEDIATE_TEMPERATURE_CHAR_UUID, 
                    (uint8_t *)&HTSAPP_Context.IntermediateTemperatureChar);
  }
#endif

/* USER CODE END HTSAPP_IntermediateTemperature */
  return;
}

#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL == 1)
void HTSAPP_MeasurementInterval(void)
{
/* USER CODE BEGIN HTSAPP_MeasurementInterval */
  if(HTSAPP_Context.MeasurementIntervalChar < BLE_CFG_HTS_TEMPERATURE_INTERVAL_MAX_VALUE)
    HTSAPP_Context.MeasurementIntervalChar += 1;
  else
    HTSAPP_Context.MeasurementIntervalChar = BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE;
  APP_DBG_MSG("Measurement Interval: %d \n", 
                  HTSAPP_Context.MeasurementIntervalChar);
    
  HTS_Update_Char(MEASUREMENT_INTERVAL_CHAR_UUID, 
                  (uint8_t *)&HTSAPP_Context.MeasurementIntervalChar);

/* USER CODE END HTSAPP_MeasurementInterval */
  return;
}
#endif

/**
 * @brief  Application service update characteristic
 * @param  None
 * @retval None
 */
void HTSAPP_Profile_UpdateChar(void)
{
/* USER CODE BEGIN HTSAPP_Profile_UpdateChar */
  #if(BLE_CFG_HTS_MEASUREMENT_INTERVAL != 0)
  uint16_t measurementInterval;
  
  HTS_SetMeasurementInterval_Menu (&measurementInterval);
  
  if(measurementInterval != 0)
  {
    HTSAPP_Context.MeasurementIntervalChar = measurementInterval;
    if(HTSAPP_Context.Indication_Status != 0)
    {
      if(HTSAPP_Context.TimerMeasurementStarted == 0)
      {
        HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, 
                    (HTSAPP_Context.MeasurementIntervalChar)*DEFAULT_HTS_MEASUREMENT_INTERVAL);
        HTSAPP_Context.TimerMeasurementStarted = 1;
      }
      else
      {
        HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
        HW_TS_Start(HTSAPP_Context.TimerMeasurement_Id, 
                    (HTSAPP_Context.MeasurementIntervalChar)*DEFAULT_HTS_MEASUREMENT_INTERVAL);
      }
    }
  }
  else
  {
    HW_TS_Stop(HTSAPP_Context.TimerMeasurement_Id);
    HTSAPP_Context.TimerMeasurementStarted = 0;
  }

  HTS_Update_Char(MEASUREMENT_INTERVAL_CHAR_UUID, 
                  (uint8_t *)&measurementInterval);
#endif

/* USER CODE END HTSAPP_Profile_UpdateChar */
}

/* USER CODE BEGIN FD */
static uint32_t HTSAPP_Get_Pseudo_Random_Sensor_Value ( void )
{
  return ((uint32_t)(READ_BIT(RTC->SSR, RTC_SSR_SS)));
}
/* USER CODE END FD */
