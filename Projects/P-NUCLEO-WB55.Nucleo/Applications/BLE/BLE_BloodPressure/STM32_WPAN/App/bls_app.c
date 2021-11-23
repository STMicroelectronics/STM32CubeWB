/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    bls_app.c
  * @author  MCD Application Team
  * @brief   Blood Pressure Service Application
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
#include "bls_app.h"
#include "ble.h"
#include "app_ble.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t connHandleIndex;
  BLS_Value_t BloodPressureMeasurementChar;
/*#if(BLE_CFG_BLS_MEASUREMENT_STATUS_VALUE_STATIC == 1)*/
/*  BLS_Measurement_Status_t MeasurementStatusChar;*/
/*#endif*/
#if(BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
  BLS_Value_t IntermediateCuffPressureChar;
  uint8_t TimerIntCufPressure_Id;
  uint8_t NotIntCufPressureEnabled;
#endif
#if(BLE_CFG_BLS_SUPPORTED_FEATURES == 1)
  uint16_t BloodPressureFeatureChar;
#endif
  uint8_t IndMeasurementEnabled;
  uint8_t TimerMeasurement_Id;
} BLSAPP_Context_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
#define DEFAULT_BLS_MEASUREMENTVALUE_SYSTOLIC                             0X0072
#define DEFAULT_BLS_MEASUREMENTVALUE_DIASTOLIC                            0X0072
#define DEFAULT_BLS_MEASUREMENTVALUE_MEAN                                 0X0072
#define DEFAULT_BLS_MEASUREMENTVALUE_PULSE_RATE                               80
#define DEFAULT_BLS_MEASUREMENTVALUE_USER_ID                                0X01
#define DEFAULT_BLS_TIME_STAMP_YEAR                                         2015
#define DEFAULT_BLS_TIME_STAMP_MONTH                                          10
#define DEFAULT_BLS_TIME_STAMP_DAY                                            26
#define DEFAULT_BLS_TIME_STAMP_HOURS                                          19
#define DEFAULT_BLS_TIME_STAMP_MINUTES                                        42
#define DEFAULT_BLS_TIME_STAMP_SECONDS                                        50
#define DEFAULT_BLS_MEASUREMENT_STATUS_BODY_MOVEMENT_DETECTION_FLAG         0X01
#define DEFAULT_BLS_MEASUREMENT_STATUS_CUFF_FIT_DETECTION_FLAG              0X01
#define DEFAULT_BLS_MEASUREMENT_STATUS_IRREGULAR_PULSE_DETECTION_FLAG       0X01
#define DEFAULT_BLS_MEASUREMENT_STATUS_PULSE_RATE_EXCEEDS_DETECTION_FLAG    0X01
#define DEFAULT_BLS_MEASUREMENT_STATUS_PULSE_RATE_LOWER_DETECTION_FLAG      0X01
#define DEFAULT_BLS_MEASUREMENT_STATUS_MEASUREMENT_POSITION_DETECTION_FLAG  0X01
#define DEFAULT_BLS_BODY_MOVEMENT_DETECTION_SUPPORT_BIT                     0x01
#define DEFAULT_BLS_CUFF_FIT_DETECTION_SUPPORT_BIT                          0x01
#define DEFAULT_BLS_IRREGULAR_PULSE_DETECTION_SUPPORT_BIT                   0x01
#define DEFAULT_BLS_PULSE_RATE_RANGE_DETECTION_SUPPORT_BIT                  0x01
#define DEFAULT_BLS_MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT              0x01
#define DEFAULT_BLS_MULTIPLE_BOND_SUPPORT_BIT                               0x00
#define DEFAULT_FEATURES_SUPPORTED                                          0X1F
#define DEFAULT_ONE_SECOND_INTERVAL                    (1000000/CFG_TS_TICK_VAL)  /**< 1s */
#define BODY_MOVEMENT_DETECTION_SUPPORT_BIT                                    1
#define CUFF_FIT_DETECTION_SUPPORT_BIT                                         2
#define IRREGULAR_PULSE_DETECTION_SUPPORT_BIT                                  4
#define PULSE_RATE_RANGE_DETECTION_SUPPORT_BIT                                 8
#define MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT                            16
#define MULTIPLE_BOND_SUPPORT_BIT                                             32
#define MEASUREMENT                                                            0
#define INT_CUFF_PRESS_MEASUREMENT                                             1
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static BLSAPP_Context_t BLSAPP_Context;
static BLS_Value_t aMeasurement[100];
static int8_t storeIndex;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLSAPP_UpdateMeasurement( void );
static void BLSAPP_UpdateIntCuffPressure( void );
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
static void BLSAPP_Store(void)
{
  memcpy(&aMeasurement[storeIndex++],
              &(BLSAPP_Context.BloodPressureMeasurementChar),
              sizeof(BLS_Value_t));
   memcpy(&aMeasurement[storeIndex++],
          &(BLSAPP_Context.BloodPressureMeasurementChar),
          sizeof(BLS_Value_t));
 if(storeIndex == 100)
    storeIndex = 0;
}

static void BLSAPP_Suppress(void)
{
  uint8_t i;

  BLS_Update_Char(BLOOD_PRESSURE_MEASUREMENT_CHAR_UUID,
                  (uint8_t *)&aMeasurement[0]);
  for(i = 1; i < storeIndex; i++)
  {
    memcpy(&aMeasurement[i-1],
                &aMeasurement[i],
                sizeof(BLS_Value_t));
    memcpy(&aMeasurement[i-1],
           &aMeasurement[i],
           sizeof(BLS_Value_t));
  }
  storeIndex--;
}

static void BLSAPP_UpdateMeasurement( void )
{
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_BLS_MEAS_REQ_ID,CFG_SCH_PRIO_0);

  return;
}

static void BLSAPP_UpdateIntCuffPressure( void )
{
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask( 1<<CFG_TASK_BLS_INT_CUFF_PRESSURE_REQ_ID, CFG_SCH_PRIO_0);

  return;
}

void BLSAPP_Measurement(void)
{
#if(BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 0)
  uint16_t systolic_measurement;
  uint16_t diastolic_measurement;
  uint16_t mean_measurement;

  systolic_measurement  = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_SYSTOLIC);
  diastolic_measurement = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_DIASTOLIC);
  mean_measurement      = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_MEAN);

  BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Systolic  = systolic_measurement;
  BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Diastolic = diastolic_measurement;
  BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Mean      = mean_measurement;
#if( BLE_CFG_BLS_TIME_STAMP_FLAG == 1)
  BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds += 4;
  if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds > 59)
  {
    BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds = 0;
    BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes += 1;
    if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes > 59)
    {
      BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes = 0;
      BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours += 1;
      if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours > 23)
      {
        BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours = 0;
        BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day += 1;
        if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day > 31)
          BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day = 1;
      }
    }
  }
#endif
#else
  if(BLSAPP_Context.NotIntCufPressureEnabled == 1)
  {
    BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Systolic  =
      BLSAPP_Context.IntermediateCuffPressureChar.MeasurementValue_Systolic;
    BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Diastolic =
      BLSAPP_Context.IntermediateCuffPressureChar.MeasurementValue_Diastolic;
    BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Mean      =
      BLSAPP_Context.IntermediateCuffPressureChar.MeasurementValue_Mean;
#if( BLE_CFG_BLS_TIME_STAMP_FLAG == 1)
    BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds =
      BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Seconds;
    BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes =
      BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Minutes;
    BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours   =
      BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Hours;
    BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day     =
      BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Day;
#endif
  }
  else
  {
    uint16_t systolic_measurement;
    uint16_t diastolic_measurement;
    uint16_t mean_measurement;

    systolic_measurement  = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_SYSTOLIC);
    diastolic_measurement = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_DIASTOLIC);
    mean_measurement      = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_MEAN);

    BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Systolic  = systolic_measurement;
    BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Diastolic = diastolic_measurement;
    BLSAPP_Context.BloodPressureMeasurementChar.MeasurementValue_Mean      = mean_measurement;
#if( BLE_CFG_BLS_TIME_STAMP_FLAG == 1)
    BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds += 4;
    if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds > 59)
    {
      BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds = 0;
      BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes += 1;
      if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes > 59)
      {
        BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes = 0;
        BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours += 1;
        if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours > 23)
        {
          BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours = 0;
          BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day += 1;
          if(BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day > 31)
            BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day = 1;
        }
      }
    }
#endif
  }
#endif
  if((APP_BLE_Get_Server_Connection_Status() == APP_BLE_CONNECTED_SERVER) &&
     (BLSAPP_Context.IndMeasurementEnabled == 1))
  {
    if(storeIndex >= 0)
    {
      BLE_DBG_APP_MSG ("BLSAPP_Measurement(): send stored measurement %d\n", storeIndex);
      BLSAPP_Suppress();
      HW_TS_Stop(BLSAPP_Context.TimerMeasurement_Id);
      HW_TS_Start(BLSAPP_Context.TimerMeasurement_Id, DEFAULT_ONE_SECOND_INTERVAL);
    }
    else
    {
      HW_TS_Stop(BLSAPP_Context.TimerMeasurement_Id);
      HW_TS_Start(BLSAPP_Context.TimerMeasurement_Id, DEFAULT_ONE_SECOND_INTERVAL*4);
      BLS_Update_Char(BLOOD_PRESSURE_MEASUREMENT_CHAR_UUID,
                      (uint8_t *)&BLSAPP_Context.BloodPressureMeasurementChar);
    }
  }
  else
    BLSAPP_Store();
  return;
}

void BLSAPP_IntCuffPressure(void)
{
#if(BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
  uint16_t systolic_measurement;
  uint16_t diastolic_measurement;
  uint16_t mean_measurement;

  systolic_measurement  = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_SYSTOLIC);
  diastolic_measurement = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_DIASTOLIC);
  mean_measurement      = (uint16_t)((rand()&0x1F)+DEFAULT_BLS_MEASUREMENTVALUE_MEAN);

  BLSAPP_Context.IntermediateCuffPressureChar.MeasurementValue_Systolic  = systolic_measurement;
  BLSAPP_Context.IntermediateCuffPressureChar.MeasurementValue_Diastolic = diastolic_measurement;
  BLSAPP_Context.IntermediateCuffPressureChar.MeasurementValue_Mean      = mean_measurement;
#if( BLE_CFG_BLS_TIME_STAMP_FLAG == 1)
  BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Seconds += 1;
  if(BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Seconds > 59)
  {
    BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Seconds = 0;
    BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Minutes += 1;
    if(BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Minutes > 59)
    {
      BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Minutes = 0;
      BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Hours += 1;
      if(BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Hours > 23)
      {
        BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Hours = 0;
        BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Day += 1;
        if(BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Day > 31)
          BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Day = 1;
      }
    }
  }
#endif

  if((APP_BLE_Get_Server_Connection_Status() == APP_BLE_CONNECTED_SERVER) &&
     (BLSAPP_Context.NotIntCufPressureEnabled == 1))
    BLS_Update_Char(INTERMEDIATE_CUFF_PRESSURE_CHAR_UUID,
                    (uint8_t *)&BLSAPP_Context.IntermediateCuffPressureChar);
#endif

  return;
}

/* Public functions ----------------------------------------------------------*/
void BLS_App_Notification(BLS_App_Notification_evt_t *pNotification)
{
  switch(pNotification->BLS_Evt_Opcode)
  {
    case BLS_MEASUREMENT_IND_ENABLED_EVT:
      {
        BLSAPP_Context.IndMeasurementEnabled = 1;
        HW_TS_Stop(BLSAPP_Context.TimerMeasurement_Id);
        HW_TS_Start(BLSAPP_Context.TimerMeasurement_Id, DEFAULT_ONE_SECOND_INTERVAL*4);
      }
      break;

    case BLS_MEASUREMENT_IND_DISABLED_EVT:
      {
        BLSAPP_Context.IndMeasurementEnabled = 0;
        HW_TS_Stop(BLSAPP_Context.TimerMeasurement_Id);
      }
      break;

#if( BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
    case BLS_INTERMEDIATE_CUFF_PRESSURE_NOTIF_ENABLED_EVT:
      {
        BLSAPP_Context.NotIntCufPressureEnabled = 1;
        HW_TS_Stop(BLSAPP_Context.TimerIntCufPressure_Id);
        HW_TS_Start(BLSAPP_Context.TimerIntCufPressure_Id, DEFAULT_ONE_SECOND_INTERVAL);
      }
      break;

    case BLS_INTERMEDIATE_CUFF_PRESSURE_NOTIF_DISABLED_EVT:
      {
        HW_TS_Stop(BLSAPP_Context.TimerIntCufPressure_Id);
        BLSAPP_Context.NotIntCufPressureEnabled = 0;
      }
      break;
#endif

    default:
      break;
  }

  return;
}

void BLSAPP_Init(void)
{
  UTIL_SEQ_RegTask( 1<< CFG_TASK_BLS_MEAS_REQ_ID, UTIL_SEQ_RFU, BLSAPP_Measurement );
  UTIL_SEQ_RegTask( 1<< CFG_TASK_BLS_INT_CUFF_PRESSURE_REQ_ID, UTIL_SEQ_RFU, BLSAPP_IntCuffPressure );

  /**
   * Initialise Flags
   */
  BLSAPP_Context.BloodPressureMeasurementChar.Flags = (uint8_t)NO_FLAG;
#if( BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
  BLSAPP_Context.IntermediateCuffPressureChar.Flags = (uint8_t)NO_FLAG;
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BLSAPP_Context.TimerMeasurement_Id),
               hw_ts_Repeated, BLSAPP_UpdateIntCuffPressure);
  BLSAPP_Context.NotIntCufPressureEnabled = 0;
#endif

  /**
   *
   */
#if( BLE_CFG_BLS_TIME_STAMP_FLAG == 1)
  BLSAPP_Context.BloodPressureMeasurementChar.Flags |= TIME_STAMP_PRESENT;
  BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Year = DEFAULT_BLS_TIME_STAMP_YEAR;
  BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Month = DEFAULT_BLS_TIME_STAMP_MONTH;
  BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Day = DEFAULT_BLS_TIME_STAMP_DAY;
  BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Hours = DEFAULT_BLS_TIME_STAMP_HOURS;
  BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Minutes = DEFAULT_BLS_TIME_STAMP_MINUTES;
  BLSAPP_Context.BloodPressureMeasurementChar.TimeStamp.Seconds = DEFAULT_BLS_TIME_STAMP_SECONDS;
  storeIndex = 0;
#if( BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
  BLSAPP_Context.IntermediateCuffPressureChar.Flags            |= TIME_STAMP_PRESENT;
  BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Year    = DEFAULT_BLS_TIME_STAMP_YEAR;
  BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Month   = DEFAULT_BLS_TIME_STAMP_MONTH;
  BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Day     = DEFAULT_BLS_TIME_STAMP_DAY;
  BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Hours   = DEFAULT_BLS_TIME_STAMP_HOURS;
  BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Minutes = DEFAULT_BLS_TIME_STAMP_MINUTES;
  BLSAPP_Context.IntermediateCuffPressureChar.TimeStamp.Seconds = DEFAULT_BLS_TIME_STAMP_SECONDS;
#endif
#endif

  /**
   * Set initial Pulse Rate
   */
#if( BLE_CFG_BLS_PULSE_RATE_FLAG == 1)
  BLSAPP_Context.BloodPressureMeasurementChar.Flags |= PULSE_RATE_PRESENT;
  BLSAPP_Context.BloodPressureMeasurementChar.PulseRate = DEFAULT_BLS_MEASUREMENTVALUE_PULSE_RATE;
#if( BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
  BLSAPP_Context.IntermediateCuffPressureChar.Flags |= PULSE_RATE_PRESENT;
  BLSAPP_Context.IntermediateCuffPressureChar.PulseRate = DEFAULT_BLS_MEASUREMENTVALUE_PULSE_RATE;
#endif
#endif
  /**
   * Set initial User Id
   */
#if( BLE_CFG_BLS_USER_ID_FLAG == 1)
  BLSAPP_Context.BloodPressureMeasurementChar.Flags |= USER_ID_PRESENT;
  BLSAPP_Context.BloodPressureMeasurementChar.UserID = DEFAULT_BLS_MEASUREMENTVALUE_USER_ID;
#if( BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
  BLSAPP_Context.IntermediateCuffPressureChar.Flags |= USER_ID_PRESENT;
  BLSAPP_Context.IntermediateCuffPressureChar.UserID    = DEFAULT_BLS_MEASUREMENTVALUE_USER_ID;
#endif
#endif

#if ( BLE_CFG_BLS_MEASUREMENT_STATUS_FLAG == 1)
  BLSAPP_Context.BloodPressureMeasurementChar.Flags |= MEASUREMENT_STATUS_PRESENT;
  BLSAPP_Context.BloodPressureMeasurementChar.MeasurementStatus = 0;
#if( BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
  BLSAPP_Context.IntermediateCuffPressureChar.Flags |= MEASUREMENT_STATUS_PRESENT;
  BLSAPP_Context.IntermediateCuffPressureChar.MeasurementStatus = 0;
  BLS_Update_Char(INTERMEDIATE_CUFF_PRESSURE_CHAR_UUID,
                  (uint8_t *)&BLSAPP_Context.IntermediateCuffPressureChar);
#endif
#endif
  BLS_Update_Char(BLOOD_PRESSURE_MEASUREMENT_CHAR_UUID,
                  (uint8_t *)&BLSAPP_Context.BloodPressureMeasurementChar);

#if( BLE_CFG_BLS_SUPPORTED_FEATURES == 1)
  BLSAPP_Context.BloodPressureFeatureChar = 0;
  BLSAPP_Context.BloodPressureFeatureChar = (BODY_MOVEMENT_DETECTION_SUPPORT_BIT       |
                                                  CUFF_FIT_DETECTION_SUPPORT_BIT             |
                                                  IRREGULAR_PULSE_DETECTION_SUPPORT_BIT      |
                                                  PULSE_RATE_RANGE_DETECTION_SUPPORT_BIT     |
                                                  MEASUREMENT_POSITION_DETECTION_SUPPORT_BIT |
                                                  MULTIPLE_BOND_SUPPORT_BIT);
  BLS_Update_Char(BLOOD_PRESSURE_FEATURE_CHAR_UUID,
                  (uint8_t *)&BLSAPP_Context.BloodPressureFeatureChar);
#endif

   /**
   * Create timer for Blood Pressure Measurement
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BLSAPP_Context.TimerMeasurement_Id), hw_ts_Repeated, BLSAPP_UpdateMeasurement);
  BLSAPP_Context.IndMeasurementEnabled = 0;

  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */
