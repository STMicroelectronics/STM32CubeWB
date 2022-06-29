/**
  ******************************************************************************
* @file    sensor_cfg_usr.h
* @author  BLE Mesh Team
* @brief   sensor server initialization parameters 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SENSOR_USR_CFG_H
#define SENSOR_USR_CFG_H

/**
* Maximum count of settings that can be supported by a sensor
* E.g., 5 sensors 
*  1st sensor has 2 settings
*  2nd sensor has 3 settings
*  3rd sensor has 2 settings
*  4th sensor has 1 setting
*  5th sensor has 2 settings
*  This value is max(2, 3, 2, 1, 2) = 3
*  value is >=1 
*/
#define SENSOR_MAX_SETTINGS_COUNT                                              2

/**
* Maximum count of series column that is supported by a sensor
* E.g., 2 sensors supporting series column
*  One sensor supports 2 columns while other sensor supports 20 columns
*  This value is max(2, 20) = 20
*  value is >=1
*/
#define SENSOR_MAX_SERIES_COUNT                                                1

/** 
* Total sensors count on all elements
* It is sum of sensors count on all elements
* Sensor init fails in case of mismatch with sensor server initialization parameters
*/
#define TOTAL_SENSORS_COUNT                                                    1

/** 
* Sum of sensor settings on all sensors on all elements
* Sensor init fails in case of mismatch with sensor server initialization parameters
*/
#define TOTAL_SENSOR_SETTINGS_COUNT                                            4

/** 
* Sum of sensor series columns on all sensors on all elements
* Sensor init fails in case of mismatch with sensor server initialization parameters
*/
#define TOTAL_SENSOR_SERIES_COLUMN_COUNT                                       0


/**
* This structure contains sensor setting initialization parameters
*/ 
typedef struct
{
    uint16_t settingPropertyId;
    uint8_t settingAccess;
    uint32_t settingRaw;
}sensor_settings_init_params_t;


/**
* This structure contains sensor series column initialization parameters
*/ 
typedef struct
{
    uint32_t rawX;
    uint32_t columnWidth;
}sensor_series_column_init_params_t;


/**
* This structure contains sensor initialization parameters
*/ 
typedef struct
{
    uint8_t elementIdx;
    uint16_t propertyId;
    uint16_t positiveTolerance;
    uint16_t negativeTolerance;
    uint8_t samplingFunction;
    uint8_t measurementPeriod;
    uint8_t updateInterval;
    uint8_t dataLength;
    uint8_t cadenceState;
    uint32_t valuesRange;
    uint8_t settingsCount;
    sensor_settings_init_params_t settings[SENSOR_MAX_SETTINGS_COUNT];
    uint16_t seriesCount;
    sensor_series_column_init_params_t seriesColumn[SENSOR_MAX_SERIES_COUNT];
}sensor_init_params_t;


/**
* This structure contains sensor server initialization parameters
*/ 
typedef struct
{
    uint8_t sensorsCount;
    sensor_init_params_t sensorInitParams[TOTAL_SENSORS_COUNT];
} sensor_server_init_params_t;


/**
* Below section represents initialization parameters of sensors supported
* Define sensors in ascending order of element index followed by ascending 
* order of Property IDs else initialization of sensor structure would fail
* Single element can support one instance of sensor PID, there can't be multiple
* instances of same PID on same element 
* For e.g. 10 sensors with PID (PID1 < PIDn ... < PID7) supported on 3 elements
* with element index (0, 1 and 2) in below fashion
* Element index 0 supports sensors corresponding to PID3, PID4, PID6 and PID7
* Element index 1 supports sensors corresponding to PID2, PID4, PID5 and PID6
* Element index 2 supports sensors corresponding to PID1, PID5
* Corrector order of naming sensors (SENSORX) is
* Element index 0 -> SENSOR1(PID3), SENSOR2(PID4), SENSOR3(PID6) and SENSOR4(PID7)
* Element index 1 -> SENSOR5(PID2), SENSOR6(PID4), SENSOR7(PID5), and SENSOR8(PID6)
* Element index 2 -> SENSOR9(PID1), and SENSOR10(PID5)
*/

/* Sensor 1 initialization */

/**
* Element index for SENSOR1
* varies from 0 to n-1 (n = number of elements)
*/
#define SENSOR1_ELEMENT_IDX                                                    0

/**
* Property ID of sensor, identifies device characteristics and other features
* Defined by Mesh Device Properties or a custom value
* 16 bit value
* 0x0000 - Prohibited
*/
#define SENSOR1_PROPERTY_ID                    PRESENT_AMBIENT_TEMPERATURE_PID
#define SENSOR1_POSITIVE_TOLERANCE             SENSOR_POSITIVE_TOLERANCE_UNSPECIFIED
#define SENSOR1_NEGATIVE_TOLERANCE             SENSOR_NEGATIVE_TOLERANCE_UNSPECIFIED
#define SENSOR1_SAMPLING_FUNCTION              SENSOR_SAMPLING_FUNC_UNSPECIFIED
#define SENSOR1_MEASUREMENT_PERIOD             SENSOR_MEASUREMENT_PERIOD_NA
#define SENSOR1_UPDATE_INTERVAL                SENSOR_UPDATE_INTERVAL_NA
#define SENSOR1_DATA_LENGTH                    1
#define SENSOR1_CADENCE_STATE                  SENSOR_CADENCE_SUPPORTED
#define SENSOR1_VALUES_RANGE                   254
#define SENSOR1_SETTINGS_COUNT                 2
#define SENSOR1_SETTING1_PROPERTY_ID           0x00BB
#define SENSOR1_SETTING1_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR1_SETTING1_RAW                   0
#define SENSOR1_SETTING2_PROPERTY_ID           0x00AD
#define SENSOR1_SETTING2_ACCESS                SENSOR_SETTING_ACCESS_READ_WRITE
#define SENSOR1_SETTING2_RAW                   0
#define SENSOR1_SERIES_COUNT                   0

#define SENSOR1_SETTINGS1_INIT_PARAMS \
{\
  SENSOR1_SETTING1_PROPERTY_ID,\
  SENSOR1_SETTING1_ACCESS,\
  SENSOR1_SETTING1_RAW \
}

#define SENSOR1_SETTINGS2_INIT_PARAMS \
{\
  SENSOR1_SETTING2_PROPERTY_ID,\
  SENSOR1_SETTING2_ACCESS,\
  SENSOR1_SETTING2_RAW \
}

#define SENSOR1_INIT_PARAMS \
{\
  SENSOR1_ELEMENT_IDX,\
  SENSOR1_PROPERTY_ID,\
  SENSOR1_POSITIVE_TOLERANCE,\
  SENSOR1_NEGATIVE_TOLERANCE,\
  SENSOR1_SAMPLING_FUNCTION,\
  SENSOR1_MEASUREMENT_PERIOD,\
  SENSOR1_UPDATE_INTERVAL,\
  SENSOR1_DATA_LENGTH,\
  SENSOR1_CADENCE_STATE,\
  SENSOR1_VALUES_RANGE,\
  SENSOR1_SETTINGS_COUNT,\
  {\
      SENSOR1_SETTINGS1_INIT_PARAMS,\
      SENSOR1_SETTINGS2_INIT_PARAMS\
  },\
  SENSOR1_SERIES_COUNT,\
  {\
    {0}\
  }\
}


#if 0
/* Sensor 2 initialization */

#define SENSOR2_ELEMENT_IDX                    0
#define SENSOR2_PROPERTY_ID                    PRESSURE_PID
#define SENSOR2_POSITIVE_TOLERANCE             SENSOR_POSITIVE_TOLERANCE_UNSPECIFIED
#define SENSOR2_NEGATIVE_TOLERANCE             SENSOR_NEGATIVE_TOLERANCE_UNSPECIFIED
#define SENSOR2_SAMPLING_FUNCTION              SENSOR_SAMPLING_FUNC_UNSPECIFIED
#define SENSOR2_MEASUREMENT_PERIOD             SENSOR_MEASUREMENT_PERIOD_NA
#define SENSOR2_UPDATE_INTERVAL                SENSOR_UPDATE_INTERVAL_NA
#define SENSOR2_DATA_LENGTH                    4
#define SENSOR2_CADENCE_STATE                  SENSOR_CADENCE_NOT_SUPPORTED
#define SENSOR2_VALUES_RANGE                   500
#define SENSOR2_SETTINGS_COUNT                 2
#define SENSOR2_SETTING1_PROPERTY_ID           0x0AAA
#define SENSOR2_SETTING1_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR2_SETTING1_RAW                   35666
#define SENSOR2_SETTING2_PROPERTY_ID           0x0AAC
#define SENSOR2_SETTING2_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR2_SETTING2_RAW                   33666
#define SENSOR2_SERIES_COUNT                   0

#define SENSOR2_SETTINGS1_INIT_PARAMS \
{\
  SENSOR2_SETTING1_PROPERTY_ID,\
  SENSOR2_SETTING1_ACCESS,\
  SENSOR2_SETTING1_RAW\
}

#define SENSOR2_SETTINGS2_INIT_PARAMS \
{\
  SENSOR2_SETTING2_PROPERTY_ID,\
  SENSOR2_SETTING2_ACCESS,\
  SENSOR2_SETTING2_RAW\
}

#define SENSOR2_INIT_PARAMS \
{\
  SENSOR2_ELEMENT_IDX,\
  SENSOR2_PROPERTY_ID,\
  SENSOR2_POSITIVE_TOLERANCE,\
  SENSOR2_NEGATIVE_TOLERANCE,\
  SENSOR2_SAMPLING_FUNCTION,\
  SENSOR2_MEASUREMENT_PERIOD,\
  SENSOR2_UPDATE_INTERVAL,\
  SENSOR2_DATA_LENGTH,\
  SENSOR2_CADENCE_STATE,\
  SENSOR2_VALUES_RANGE,\
  SENSOR2_SETTINGS_COUNT,\
  {\
      SENSOR2_SETTINGS1_INIT_PARAMS,\
      SENSOR2_SETTINGS2_INIT_PARAMS\
  },\
  SENSOR2_SERIES_COUNT,\
  {\
    {0}\
  }\
}

/**
* Combined defined of all sensors initialization parameters
*/

#define SENSOR_SERVER_INIT_PARAMS \
{\
  TOTAL_SENSORS_COUNT,\
  {\
    SENSOR1_INIT_PARAMS,\
    SENSOR2_INIT_PARAMS,\
  }\
}
#else
/**
* Combined defined of all sensors initialization parameters
*/

#define SENSOR_SERVER_INIT_PARAMS \
{\
  TOTAL_SENSORS_COUNT,\
  {\
    SENSOR1_INIT_PARAMS,\
  }\
}
#endif
#endif /* SENSOR_USR_CFG_H */

/*

Example for multiple sensors with series column support

#define SENSOR_MAX_SETTINGS_COUNT                                           3
#define SENSOR_MAX_SERIES_COUNT                                            20
#define TOTAL_SENSORS_COUNT                                                 5
#define TOTAL_SENSOR_SETTINGS_COUNT                                         9
#define TOTAL_SENSOR_SERIES_COLUMN_COUNT                                   22

#define SENSOR1_ELEMENT_IDX                                                  0
#define SENSOR1_PROPERTY_ID                                   PEOPLE_COUNT_PID
#define SENSOR1_POSITIVE_TOLERANCE       SENSOR_POSITIVE_TOLERANCE_UNSPECIFIED
#define SENSOR1_NEGATIVE_TOLERANCE       SENSOR_NEGATIVE_TOLERANCE_UNSPECIFIED
#define SENSOR1_SAMPLING_FUNCTION             SENSOR_SAMPLING_FUNC_UNSPECIFIED
#define SENSOR1_MEASUREMENT_PERIOD                SENSOR_MEASUREMENT_PERIOD_NA
#define SENSOR1_UPDATE_INTERVAL                      SENSOR_UPDATE_INTERVAL_NA
#define SENSOR1_DATA_LENGTH                                                2
#define SENSOR1_CADENCE_STATE                  SENSOR_CADENCE_SUPPORTED
#define SENSOR1_VALUES_RANGE                   1000
#define SENSOR1_SETTINGS_COUNT                 3
#define SENSOR1_SETTING1_PROPERTY_ID           0x000A
#define SENSOR1_SETTING1_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR1_SETTING1_RAW                   234
#define SENSOR1_SETTING2_PROPERTY_ID           0x000C
#define SENSOR1_SETTING2_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR1_SETTING2_RAW                   246
#define SENSOR1_SETTING3_PROPERTY_ID           0x0010
#define SENSOR1_SETTING3_ACCESS                SENSOR_SETTING_ACCESS_READ_WRITE
#define SENSOR1_SETTING3_RAW                   278
#define SENSOR1_SERIES_COUNT                   0

#define SENSOR1_SETTINGS1_INIT_PARAMS \
{\
  SENSOR1_SETTING1_PROPERTY_ID,\
  SENSOR1_SETTING1_ACCESS,\
  SENSOR1_SETTING1_RAW\
}

#define SENSOR1_SETTINGS2_INIT_PARAMS \
{\
  SENSOR1_SETTING2_PROPERTY_ID,\
  SENSOR1_SETTING2_ACCESS,\
  SENSOR1_SETTING2_RAW\
}

#define SENSOR1_SETTINGS3_INIT_PARAMS \
{\
  SENSOR1_SETTING3_PROPERTY_ID,\
  SENSOR1_SETTING3_ACCESS,\
  SENSOR1_SETTING3_RAW\
}

#define SENSOR1_INIT_PARAMS \
{\
  SENSOR1_ELEMENT_IDX,\
  SENSOR1_PROPERTY_ID,\
  SENSOR1_POSITIVE_TOLERANCE,\
  SENSOR1_NEGATIVE_TOLERANCE,\
  SENSOR1_SAMPLING_FUNCTION,\
  SENSOR1_MEASUREMENT_PERIOD,\
  SENSOR1_UPDATE_INTERVAL,\
  SENSOR1_DATA_LENGTH,\
  SENSOR1_CADENCE_STATE,\
  SENSOR1_VALUES_RANGE,\
  SENSOR1_SETTINGS_COUNT,\
  {\
    SENSOR1_SETTINGS1_INIT_PARAMS,\
    SENSOR1_SETTINGS2_INIT_PARAMS,\
    SENSOR1_SETTINGS3_INIT_PARAMS\
  },\
  SENSOR1_SERIES_COUNT,\
  {\
    {0}\
  }\
}

#define SENSOR2_ELEMENT_IDX                                                  0
#define SENSOR2_PROPERTY_ID                    DESIRED_AMBIENT_TEMPERATURE_PID
#define SENSOR2_POSITIVE_TOLERANCE             0xabc
#define SENSOR2_NEGATIVE_TOLERANCE             0xdef
#define SENSOR2_SAMPLING_FUNCTION              SENSOR_SAMPLING_FUNC_RMS
#define SENSOR2_MEASUREMENT_PERIOD             0x04
#define SENSOR2_UPDATE_INTERVAL                0x05
#define SENSOR2_DATA_LENGTH                    1
#define SENSOR2_CADENCE_STATE                  SENSOR_CADENCE_SUPPORTED
#define SENSOR2_VALUES_RANGE                   100
#define SENSOR2_SETTINGS_COUNT                 2
#define SENSOR2_SETTING1_PROPERTY_ID           0x00BB
#define SENSOR2_SETTING1_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR2_SETTING1_RAW                   345
#define SENSOR2_SETTING2_PROPERTY_ID           0x00AD
#define SENSOR2_SETTING2_ACCESS                SENSOR_SETTING_ACCESS_READ_WRITE
#define SENSOR2_SETTING2_RAW                   435
#define SENSOR2_SERIES_COUNT                   0

#define SENSOR2_SETTINGS1_INIT_PARAMS \
{\
  SENSOR2_SETTING1_PROPERTY_ID,\
  SENSOR2_SETTING1_ACCESS,\
  SENSOR2_SETTING1_RAW\
}

#define SENSOR2_SETTINGS2_INIT_PARAMS \
{\
  SENSOR2_SETTING2_PROPERTY_ID,\
  SENSOR2_SETTING2_ACCESS,\
  SENSOR2_SETTING2_RAW \
}

#define SENSOR2_INIT_PARAMS \
{\
  SENSOR2_ELEMENT_IDX,\
  SENSOR2_PROPERTY_ID,\
  SENSOR2_POSITIVE_TOLERANCE,\
  SENSOR2_NEGATIVE_TOLERANCE,\
  SENSOR2_SAMPLING_FUNCTION,\
  SENSOR2_MEASUREMENT_PERIOD,\
  SENSOR2_UPDATE_INTERVAL,\
  SENSOR2_DATA_LENGTH,\
  SENSOR2_CADENCE_STATE,\
  SENSOR2_VALUES_RANGE,\
  SENSOR2_SETTINGS_COUNT,\
  {\
    SENSOR2_SETTINGS1_INIT_PARAMS,\
    SENSOR2_SETTINGS2_INIT_PARAMS\
  },\
  SENSOR2_SERIES_COUNT,\
  {\
    {0}\
  }\
}

#define SENSOR3_ELEMENT_IDX                    0
#define SENSOR3_PROPERTY_ID                    PRESSURE_PID
#define SENSOR3_POSITIVE_TOLERANCE             SENSOR_POSITIVE_TOLERANCE_UNSPECIFIED
#define SENSOR3_NEGATIVE_TOLERANCE             SENSOR_NEGATIVE_TOLERANCE_UNSPECIFIED
#define SENSOR3_SAMPLING_FUNCTION              SENSOR_SAMPLING_FUNC_UNSPECIFIED
#define SENSOR3_MEASUREMENT_PERIOD             SENSOR_MEASUREMENT_PERIOD_NA
#define SENSOR3_UPDATE_INTERVAL                SENSOR_UPDATE_INTERVAL_NA
#define SENSOR3_DATA_LENGTH                    2
#define SENSOR3_CADENCE_STATE                  SENSOR_CADENCE_NOT_SUPPORTED
#define SENSOR3_VALUES_RANGE                  500
#define SENSOR3_SETTINGS_COUNT                 2
#define SENSOR3_SETTING1_PROPERTY_ID           0x0AAA
#define SENSOR3_SETTING1_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR3_SETTING1_RAW                  35666
#define SENSOR3_SETTING2_PROPERTY_ID           0x0AAC
#define SENSOR3_SETTING2_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR3_SETTING2_RAW                  33666
#define SENSOR3_SERIES_COUNT                   20
#define SENSOR3_SERIES1_RAWX                   0
#define SENSOR3_SERIES1_CW                     10
#define SENSOR3_SERIES2_RAWX                   23
#define SENSOR3_SERIES2_CW                     10
#define SENSOR3_SERIES3_RAWX                   34
#define SENSOR3_SERIES3_CW                     10
#define SENSOR3_SERIES4_RAWX                   35
#define SENSOR3_SERIES4_CW                     10
#define SENSOR3_SERIES5_RAWX                   36
#define SENSOR3_SERIES5_CW                     10
#define SENSOR3_SERIES6_RAWX                   40
#define SENSOR3_SERIES6_CW                     10
#define SENSOR3_SERIES7_RAWX                   41
#define SENSOR3_SERIES7_CW                     10
#define SENSOR3_SERIES8_RAWX                   42
#define SENSOR3_SERIES8_CW                     10
#define SENSOR3_SERIES9_RAWX                   43
#define SENSOR3_SERIES9_CW                     10
#define SENSOR3_SERIES10_RAWX                  45
#define SENSOR3_SERIES10_CW                    10
#define SENSOR3_SERIES11_RAWX                  46
#define SENSOR3_SERIES11_CW                    10
#define SENSOR3_SERIES12_RAWX                  47
#define SENSOR3_SERIES12_CW                    10
#define SENSOR3_SERIES13_RAWX                  48
#define SENSOR3_SERIES13_CW                    10
#define SENSOR3_SERIES14_RAWX                  49
#define SENSOR3_SERIES14_CW                    10
#define SENSOR3_SERIES15_RAWX                  50
#define SENSOR3_SERIES15_CW                    10
#define SENSOR3_SERIES16_RAWX                  51
#define SENSOR3_SERIES16_CW                    10
#define SENSOR3_SERIES17_RAWX                  52
#define SENSOR3_SERIES17_CW                    10
#define SENSOR3_SERIES18_RAWX                  53
#define SENSOR3_SERIES18_CW                    10
#define SENSOR3_SERIES19_RAWX                  55
#define SENSOR3_SERIES19_CW                    10
#define SENSOR3_SERIES20_RAWX                  89
#define SENSOR3_SERIES20_CW                    10

#define SENSOR3_SETTINGS1_INIT_PARAMS \
{\
  SENSOR3_SETTING1_PROPERTY_ID,\
  SENSOR3_SETTING1_ACCESS,\
  SENSOR3_SETTING1_RAW\
}

#define SENSOR3_SETTINGS2_INIT_PARAMS \
{\
  SENSOR3_SETTING2_PROPERTY_ID,\
  SENSOR3_SETTING2_ACCESS,\
  SENSOR3_SETTING2_RAW\
}

#define SENSOR3_COLUMN1_INIT_PARAMS {SENSOR3_SERIES1_RAWX,SENSOR3_SERIES1_CW}
#define SENSOR3_COLUMN2_INIT_PARAMS  {SENSOR3_SERIES2_RAWX,SENSOR3_SERIES2_CW}
#define SENSOR3_COLUMN3_INIT_PARAMS  {SENSOR3_SERIES3_RAWX,SENSOR3_SERIES3_CW}
#define SENSOR3_COLUMN4_INIT_PARAMS  {SENSOR3_SERIES4_RAWX,SENSOR3_SERIES4_CW}
#define SENSOR3_COLUMN5_INIT_PARAMS  {SENSOR3_SERIES5_RAWX,SENSOR3_SERIES5_CW}
#define SENSOR3_COLUMN6_INIT_PARAMS  {SENSOR3_SERIES6_RAWX,SENSOR3_SERIES6_CW}
#define SENSOR3_COLUMN7_INIT_PARAMS  {SENSOR3_SERIES7_RAWX,SENSOR3_SERIES7_CW}
#define SENSOR3_COLUMN8_INIT_PARAMS  {SENSOR3_SERIES8_RAWX,SENSOR3_SERIES8_CW}
#define SENSOR3_COLUMN9_INIT_PARAMS  {SENSOR3_SERIES9_RAWX,SENSOR3_SERIES9_CW}
#define SENSOR3_COLUMN10_INIT_PARAMS {SENSOR3_SERIES10_RAWX,SENSOR3_SERIES10_CW}
#define SENSOR3_COLUMN11_INIT_PARAMS {SENSOR3_SERIES11_RAWX,SENSOR3_SERIES11_CW}
#define SENSOR3_COLUMN12_INIT_PARAMS {SENSOR3_SERIES12_RAWX,SENSOR3_SERIES12_CW}
#define SENSOR3_COLUMN13_INIT_PARAMS {SENSOR3_SERIES13_RAWX,SENSOR3_SERIES13_CW}
#define SENSOR3_COLUMN14_INIT_PARAMS {SENSOR3_SERIES14_RAWX,SENSOR3_SERIES14_CW}
#define SENSOR3_COLUMN15_INIT_PARAMS {SENSOR3_SERIES15_RAWX,SENSOR3_SERIES15_CW}
#define SENSOR3_COLUMN16_INIT_PARAMS {SENSOR3_SERIES16_RAWX,SENSOR3_SERIES16_CW}
#define SENSOR3_COLUMN17_INIT_PARAMS {SENSOR3_SERIES17_RAWX,SENSOR3_SERIES17_CW}
#define SENSOR3_COLUMN18_INIT_PARAMS {SENSOR3_SERIES18_RAWX,SENSOR3_SERIES18_CW}
#define SENSOR3_COLUMN19_INIT_PARAMS {SENSOR3_SERIES19_RAWX,SENSOR3_SERIES19_CW}
#define SENSOR3_COLUMN20_INIT_PARAMS {SENSOR3_SERIES20_RAWX,SENSOR3_SERIES20_CW}

#define SENSOR3_INIT_PARAMS \
{\
  SENSOR3_ELEMENT_IDX,\
  SENSOR3_PROPERTY_ID,\
  SENSOR3_POSITIVE_TOLERANCE,\
  SENSOR3_NEGATIVE_TOLERANCE,\
  SENSOR3_SAMPLING_FUNCTION,\
  SENSOR3_MEASUREMENT_PERIOD,\
  SENSOR3_UPDATE_INTERVAL,\
  SENSOR3_DATA_LENGTH,\
  SENSOR3_CADENCE_STATE,\
  SENSOR3_VALUES_RANGE,\
  SENSOR3_SETTINGS_COUNT,\
  {\
    SENSOR3_SETTINGS1_INIT_PARAMS,\
    SENSOR3_SETTINGS2_INIT_PARAMS\
  },\
  SENSOR3_SERIES_COUNT,\
  {\
    SENSOR3_COLUMN1_INIT_PARAMS,\
    SENSOR3_COLUMN2_INIT_PARAMS,\
    SENSOR3_COLUMN3_INIT_PARAMS,\
    SENSOR3_COLUMN4_INIT_PARAMS,\
    SENSOR3_COLUMN5_INIT_PARAMS,\
    SENSOR3_COLUMN6_INIT_PARAMS,\
    SENSOR3_COLUMN7_INIT_PARAMS,\
    SENSOR3_COLUMN8_INIT_PARAMS,\
    SENSOR3_COLUMN9_INIT_PARAMS,\
    SENSOR3_COLUMN10_INIT_PARAMS,\
    SENSOR3_COLUMN11_INIT_PARAMS,\
    SENSOR3_COLUMN12_INIT_PARAMS,\
    SENSOR3_COLUMN13_INIT_PARAMS,\
    SENSOR3_COLUMN14_INIT_PARAMS,\
    SENSOR3_COLUMN15_INIT_PARAMS,\
    SENSOR3_COLUMN16_INIT_PARAMS,\
    SENSOR3_COLUMN17_INIT_PARAMS,\
    SENSOR3_COLUMN18_INIT_PARAMS,\
    SENSOR3_COLUMN19_INIT_PARAMS,\
    SENSOR3_COLUMN20_INIT_PARAMS}\
}

#define SENSOR4_ELEMENT_IDX                    0
#define SENSOR4_PROPERTY_ID                    HUMIDITY_PID
#define SENSOR4_POSITIVE_TOLERANCE             SENSOR_POSITIVE_TOLERANCE_UNSPECIFIED
#define SENSOR4_NEGATIVE_TOLERANCE             SENSOR_NEGATIVE_TOLERANCE_UNSPECIFIED
#define SENSOR4_SAMPLING_FUNCTION              SENSOR_SAMPLING_FUNC_UNSPECIFIED
#define SENSOR4_MEASUREMENT_PERIOD             SENSOR_MEASUREMENT_PERIOD_NA
#define SENSOR4_UPDATE_INTERVAL                SENSOR_UPDATE_INTERVAL_NA
#define SENSOR4_DATA_LENGTH                    2
#define SENSOR4_CADENCE_STATE                  SENSOR_CADENCE_SUPPORTED
#define SENSOR4_VALUES_RANGE                   200
#define SENSOR4_SETTINGS_COUNT                 1
#define SENSOR4_SETTING1_PROPERTY_ID           0xAAAA
#define SENSOR4_SETTING1_ACCESS                SENSOR_SETTING_ACCESS_READ_WRITE
#define SENSOR4_SETTING1_RAW                   35666
#define SENSOR4_SERIES_COUNT                   0

#define SENSOR4_SETTINGS_INIT_PARAMS \
{\
  SENSOR4_SETTING1_PROPERTY_ID,\
  SENSOR4_SETTING1_ACCESS,\
  SENSOR4_SETTING1_RAW\
}

#define SENSOR4_INIT_PARAMS \
{\
  SENSOR4_ELEMENT_IDX,\
  SENSOR4_PROPERTY_ID,\
  SENSOR4_POSITIVE_TOLERANCE,\
  SENSOR4_NEGATIVE_TOLERANCE,\
  SENSOR4_SAMPLING_FUNCTION,\
  SENSOR4_MEASUREMENT_PERIOD,\
  SENSOR4_UPDATE_INTERVAL,\
  SENSOR4_DATA_LENGTH,\
  SENSOR4_CADENCE_STATE,\
  SENSOR4_VALUES_RANGE,\
  SENSOR4_SETTINGS_COUNT,\
  {\
    SENSOR4_SETTINGS_INIT_PARAMS\
  },\
  SENSOR4_SERIES_COUNT,\
  {\
    {0}\
  }\
}

#define SENSOR5_ELEMENT_IDX                    0
#define SENSOR5_PROPERTY_ID                    TIME_OF_FLIGHT_PID
#define SENSOR5_POSITIVE_TOLERANCE             SENSOR_POSITIVE_TOLERANCE_UNSPECIFIED
#define SENSOR5_NEGATIVE_TOLERANCE             SENSOR_NEGATIVE_TOLERANCE_UNSPECIFIED
#define SENSOR5_SAMPLING_FUNCTION              SENSOR_SAMPLING_FUNC_UNSPECIFIED
#define SENSOR5_MEASUREMENT_PERIOD             SENSOR_MEASUREMENT_PERIOD_NA
#define SENSOR5_UPDATE_INTERVAL                SENSOR_UPDATE_INTERVAL_NA
#define SENSOR5_DATA_LENGTH                    2
#define SENSOR5_CADENCE_STATE                  SENSOR_CADENCE_NOT_SUPPORTED
#define SENSOR5_VALUES_RANGE                   0
#define SENSOR5_SETTINGS_COUNT                 1
#define SENSOR5_SETTING1_PROPERTY_ID           0xAAFA
#define SENSOR5_SETTING1_ACCESS                SENSOR_SETTING_ACCESS_READ
#define SENSOR5_SETTING1_RAW                   35666
#define SENSOR5_SERIES_COUNT                   2
#define SENSOR5_SERIES1_RAWX                   20
#define SENSOR5_SERIES1_CW                     30
#define SENSOR5_SERIES2_RAWX                   50
#define SENSOR5_SERIES2_CW                     40

#define SENSOR5_SETTINGS_INIT_PARAMS \
{\
  SENSOR5_SETTING1_PROPERTY_ID,\
  SENSOR5_SETTING1_ACCESS,\
  SENSOR5_SETTING1_RAW\
}

#define SENSOR5_COLUMN1_INIT_PARAMS  {SENSOR5_SERIES1_RAWX,SENSOR5_SERIES1_CW}
#define SENSOR5_COLUMN2_INIT_PARAMS  {SENSOR5_SERIES2_RAWX,SENSOR5_SERIES2_CW}

#define SENSOR5_INIT_PARAMS \
{\
  SENSOR5_ELEMENT_IDX,\
  SENSOR5_PROPERTY_ID,\
  SENSOR5_POSITIVE_TOLERANCE,\
  SENSOR5_NEGATIVE_TOLERANCE,\
  SENSOR5_SAMPLING_FUNCTION,\
  SENSOR5_MEASUREMENT_PERIOD,\
  SENSOR5_UPDATE_INTERVAL,\
  SENSOR5_DATA_LENGTH,\
  SENSOR5_CADENCE_STATE,\
  SENSOR5_VALUES_RANGE,\
  SENSOR5_SETTINGS_COUNT,\
  {\
    SENSOR5_SETTINGS_INIT_PARAMS\
  },\
  SENSOR5_SERIES_COUNT,\
  {\
    SENSOR5_COLUMN1_INIT_PARAMS,\
    SENSOR5_COLUMN2_INIT_PARAMS\
  }\
}

#define SENSOR6_ELEMENT_IDX                    0
#define SENSOR6_PROPERTY_ID                    PRESENCE_PID
#define SENSOR6_POSITIVE_TOLERANCE             SENSOR_POSITIVE_TOLERANCE_UNSPECIFIED
#define SENSOR6_NEGATIVE_TOLERANCE             SENSOR_NEGATIVE_TOLERANCE_UNSPECIFIED
#define SENSOR6_SAMPLING_FUNCTION              SENSOR_SAMPLING_FUNC_UNSPECIFIED
#define SENSOR6_MEASUREMENT_PERIOD             SENSOR_MEASUREMENT_PERIOD_NA
#define SENSOR6_UPDATE_INTERVAL                SENSOR_UPDATE_INTERVAL_NA
#define SENSOR6_DATA_LENGTH                    1
#define SENSOR6_CADENCE_STATE                  SENSOR_CADENCE_NOT_SUPPORTED
#define SENSOR6_VALUES_RANGE                   0
#define SENSOR6_SETTINGS_COUNT                 0
#define SENSOR6_SERIES_COUNT                   0

#define SENSOR6_SETTINGS_INIT_PARAMS \
{\
  {\
    {0}\
  }\
}

#define SENSOR6_INIT_PARAMS \
{\
  SENSOR6_ELEMENT_IDX,\
  SENSOR6_PROPERTY_ID,\
  SENSOR6_POSITIVE_TOLERANCE,\
  SENSOR6_NEGATIVE_TOLERANCE,\
  SENSOR6_SAMPLING_FUNCTION,\
  SENSOR6_MEASUREMENT_PERIOD,\
  SENSOR6_UPDATE_INTERVAL,\
  SENSOR6_DATA_LENGTH,\
  SENSOR6_CADENCE_STATE,\
  SENSOR6_VALUES_RANGE,\
  SENSOR6_SETTINGS_COUNT,\
  {\
    {0}\
  },\
  SENSOR6_SERIES_COUNT,\
  {\
    {0}\
  }\
}

#define SENSOR_SERVER_INIT_PARAMS \
{\
  TOTAL_SENSORS_COUNT,\
  {\
    SENSOR1_INIT_PARAMS,\
    SENSOR2_INIT_PARAMS,\
    SENSOR3_INIT_PARAMS,\
    SENSOR4_INIT_PARAMS,\
    SENSOR5_INIT_PARAMS\
  }\
}
*/
