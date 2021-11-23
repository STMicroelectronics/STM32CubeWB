/**
******************************************************************************
* @file    common.h
* @author  BLE Mesh Team
* @brief   Model middleware file
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
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/
/* Macros defined for the number of bytes saved, number of bytes dedicated 
   for ganeric model and light model */
//#define APP_NVM_GENERIC_MODEL_SIZE                                            16U
//#define APP_NVM_LIGHT_MODEL_SIZE                                              32U
#define APP_NVM_LIGHT_LC_MODEL_SIZE                                           4U
#if 0
#define APP_NVM_MODELDATA_PER_ELEMENT_SIZE  (APP_NVM_GENERIC_MODEL_SIZE + \
                                             APP_NVM_LIGHT_MODEL_SIZE + \
                                             APP_NVM_LIGHT_LC_MODEL_SIZE)
#endif
#define APP_NVM_MODEL_SIZE   (APP_NVM_MODELDATA_PER_ELEMENT_SIZE * APPLICATION_NUMBER_OF_ELEMENTS)

#define GENERIC_VALID_FLAG                                                  0xAA

#define TESTING_BYTE                                                         240
/* 16 bytes are reserved for generic */
#define GENERIC_VALID_FLAG_OFFSET                                              0
#define GENERIC_ON_OFF_NVM_OFFSET                                              1
#define GENERIC_LEVEL_NVM_OFFSET                                               2
#define GENERIC_POWER_ON_OFF_NVM_OFFSET                                        4
/* 16 bytes are reserved for light
*  Light model nvm offset is 16 bytes ahead of generic model */
#define LIGHT_VALID_FLAG_OFFSET                                                0
#define LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET                                     1      
#define LIGHT_LIGHTNESS_TARGET_NVM_OFFSET                                      3      
#define LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET                                     5
#define LIGHT_LIGHTNESS_LAST_NVM_OFFSET                                        7
#define LIGHT_TRANSITION_STATUS                                                9
#define LIGHT_CTL_NVM_OFFSET                                                  10
#define LIGHT_CTL_DEFAULT_NVM_OFFSET                                          16
#define LIGHT_HSL_NVM_OFFSET                                                  22
#define LIGHT_HSL_DEFAULT_NVM_OFFSET                                          28
#define LIGHT_LC_MODE_NVM_OFFSET                                              48

/*  Light model nvm offset used for local array*/
#define LIGHT_DEFAULT_OFFSET                                                   6
/* Buffer index limit for the generic data */
#define GENERIC_DATA_LIMIT                                                    15

#define No_NVM_FLAG                                                         0XFE
#define GENERIC_ON_OFF_NVM_FLAG                                             0X01
#define GENERIC_LEVEL_NVM_FLAG                                              0X02
#define LIGHT_LIGHTNESS_NVM_FLAG                                            0X03
#define LIGHT_CTL_NVM_FLAG                                                  0X04
#define LIGHT_HSL_NVM_FLAG                                                  0X05
#define LIGHT_LC_NVM_FLAG                                                   0X06
                                               
#define GENERIC_POWER_OFF_STATE                                             0X00
#define GENERIC_POWER_ON_STATE                                              0X01
#define GENERIC_POWER_RESTORE_STATE                                         0X02
                                               
#define FLAG_SET                                                               1
#define FLAG_RESET                                                             0

#define VALUE_UPDATE_SET                                                    0X01
#define VALUE_UPDATE_RESET                                                  0X00

#define CLK_FLAG_ENABLE                                                        1 
#define CLK_FLAG_DISABLE                                                       0 

#define PWM_ZERO_VALUE                                                         1
#define INTENSITY_LEVEL_ZERO                                                0X00
#define INTENSITY_LEVEL_FULL                                              31990U

#define MAX_TID_VALUE                                                       0XFF

#define STEP_HEX_VALUE_0                                                    0X00
#define STEP_HEX_VALUE_1                                                    0X01
#define STEP_HEX_VALUE_2                                                    0X02
#define STEP_HEX_VALUE_3                                                    0X03

#define STEP_RESOLUTION_100MS                                                100
#define STEP_RESOLUTION_1S                                                  1000
#define STEP_RESOLUTION_10S                                                10000
#define STEP_RESOLUTION_10M                                               600000

#define MODEL_TRANSITION_STOP                                               0X00

#define DEFAULT_RANDOM_DELAY_VALUE                                           300
#define PUBLISH_1SEC_TIME                                                   1000

typedef MOBLE_RESULT (*APPLI_SAVE_MODEL_STATE_CB)(MOBLEUINT8* stateBuff, MOBLEUINT16 size);
typedef MOBLE_RESULT (*APPLI_SAVE_MODEL_TEST_STATE_CB)(MOBLEUINT8* stateBuff, MOBLEUINT16 size);
typedef MOBLE_RESULT (*APPLI_RETRIEVE_MODEL_TEST_STATE_CB)(MOBLEUINT8* stateBuff, MOBLEUINT16 size);

/******************************************************************************/
/********** Following Section defines the SIG MODEL IDs            ************/
/******************************************************************************/
#define GENERIC_ONOFF_SERVER_MODEL_ID                                    0x1000U
#define GENERIC_ONOFF_CLIENT_MODEL_ID                                    0x1001U
#define GENERIC_LEVEL_SERVER_MODEL_ID                                    0x1002U
#define GENERIC_LEVEL_CLIENT_MODEL_ID                                    0x1003U
#define GENERIC_DEFAULT_TRANSITION_TIME_SERVER_MODEL_ID                  0x1004U
#define GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL_ID                  0x1005U
#define GENERIC_POWER_ONOFF_SERVER_MODEL_ID                              0x1006U
#define GENERIC_POWER_ONOFF_SETUP_SERVER_MODEL_ID                        0x1007U
#define GENERIC_POWER_ONOFF_CLIENT_MODEL_ID                              0x1008U
#define GENERIC_POWER_LEVEL_SERVER_MODEL_ID                              0x1009U
#define GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL_ID                        0x100AU
#define GENERIC_POWER_LEVEL_CLIENT_MODEL_ID                              0x100BU
#define GENERIC_BATTERY_SERVER_MODEL_ID                                  0x100CU
#define GENERIC_BATTERY_CLIENT_MODEL_ID                                  0x100DU
#define GENERIC_LOCATION_SERVER_MODEL_ID                                 0x100EU
#define GENERIC_LOCATION_SETUP_SERVER_MODEL_ID                           0x100FU
#define GENERIC_LOCATION_CLIENT_MODEL_ID                                 0x1010U
#define GENERIC_ADMIN_PROPERTY_SERVER_MODEL_ID                           0x1011U
#define GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL_ID                    0x1012U
#define GENERIC_USER_PROPERTY_SERVER_MODEL_ID                            0x1013U
#define GENERIC_CLIENT_PROPERTY_SERVER_MODEL_ID                          0x1014U

#define SENSOR_SERVER_MODEL_ID                                           0x1100U  
#define SENSOR_SETUP_SERVER_MODEL_ID                                     0x1101U
#define SENSOR_CLIENT_MODEL_ID                                           0x1102U  

#define TIME_MODEL_SERVER_MODEL_ID                                       0x1200U
#define TIME_MODEL_SETUP_SERVER_MODEL_ID                                 0x1201U
#define SCENE_MODEL_SERVER_MODEL_ID                                      0x1203U
#define SCENE_MODEL_SETUP_SERVER_MODEL_ID                                0x1204U

#define LIGHT_LIGHTNESS_SERVER_MODEL_ID                                  0x1300U
#define LIGHT_LIGHTNESS_SETUP_SERVER_MODEL_ID                            0x1301U
#define LIGHT_LIGHTNESS_CLIENT_MODEL_ID                                  0x1302U
#define LIGHT_CTL_SERVER_MODEL_ID                                        0x1303U
#define LIGHT_CTL_SETUP_SERVER_MODEL_ID                                  0x1304U
#define LIGHT_CTL_CLIENT_MODEL_ID                                        0x1305U
#define LIGHT_CTL_TEMPERATURE_SERVER_MODEL_ID                            0x1306U
#define LIGHT_HSL_SERVER_MODEL_ID                                        0x1307U
#define LIGHT_HSL_SETUP_SERVER_MODEL_ID                                  0x1308U
#define LIGHT_HSL_CLIENT_MODEL_ID                                        0x1309U
#define LIGHT_HSL_HUE_SERVER_MODEL_ID                                    0x130AU
#define LIGHT_HSL_SATURATION_SERVER_MODEL_ID                             0x130BU
#define LIGHT_XYL_SERVER_MODEL_ID                                        0x130CU
#define LIGHT_XYL_SETUP_SERVER_MODEL_ID                                  0x130DU
#define LIGHT_XYL_CLIENT_MODEL_ID                                        0x130EU
#define LIGHT_LC_SERVER_MODEL_ID                                         0x130FU
#define LIGHT_LC_SETUP_SERVER_MODEL_ID                                   0x1310U
#define LIGHT_LC_CLIENT_MODEL_ID                                         0x1311U

/* Sensor Server model opcode */
#define SENSOR_DESCRIPTOR_GET                                            0x8230U      
#define SENSOR_DESCRIPTOR_STATUS                                           0X51U
#define SENSOR_GET                                                       0X8231U
#define SENSOR_STATUS                                                      0X52U
#define SENSOR_COLUMN_GET                                                0X8232U
#define SENSOR_COLUMN_STATUS                                               0X53U
#define SENSOR_SERIES_GET                                                0X8233U
#define SENSOR_SERIES_STATUS                                               0X54U
/* Sensor Setup Server model opcode */                     
#define SENSOR_CADENCE_GET                                               0X8234U
#define SENSOR_CADENCE_SET                                                 0X55U
#define SENSOR_CADENCE_SET_UNACK                                           0X56U
#define SENSOR_CADENCE_STATUS                                              0X57U
#define SENSOR_SETTINGS_GET                                              0X8235U
#define SENSOR_SETTINGS_STATUS                                             0X58U
#define SENSOR_SETTING_GET                                               0X8236U
#define SENSOR_SETTING_SET                                                 0X59U
#define SENSOR_SETTING_SET_UNACK                                           0X5AU
#define SENSOR_SETTING_STATUS                                              0X5BU
/* Light LC Server model opcode */
#define LIGHT_LC_MODE_GET                                                0X8291U
#define LIGHT_LC_MODE_SET                                                0X8292U
#define LIGHT_LC_MODE_SET_UNACK                                          0X8293U
#define LIGHT_LC_MODE_STATUS                                             0X8294U
#define LIGHT_LC_OM_GET                                                  0X8295U
#define LIGHT_LC_OM_SET                                                  0X8296U
#define LIGHT_LC_OM_SET_UNACK                                            0X8297U
#define LIGHT_LC_OM_STATUS                                               0X8298U
#define LIGHT_LC_ON_OFF_GET                                              0X8299U
#define LIGHT_LC_ON_OFF_SET                                              0X829AU
#define LIGHT_LC_ON_OFF_SET_UNACK                                        0X829BU
#define LIGHT_LC_ON_OFF_STATUS                                           0X829CU
/* Light LC Setup server model opcode */
#define LIGHT_LC_PROPERTY_GET                                            0X829DU
#define LIGHT_LC_PROPERTY_SET                                              0X62U
#define LIGHT_LC_PROPERTY_SET_UNACK                                        0X63U
#define LIGHT_LC_PROPERTY_STATUS                                           0X64U

#define GENERIC_SERVER_MAIN_ELEMENT_INDEX            0
/* Element index of main element which supports Light Lightness Server
   and Light Lightness Linear is binded with Light LC Linear Out */
#define LIGHT_LIGHTNESS_SERVER_MAIN_ELEMENT_INDEX    0

              /* Mesh Device Properties and Property IDs  */
/* Temperature 8 In A Period Of Day */
#define AVERAGE_AMBIENT_TEMPERATURE_IN_A_PERIOD_OF_DAY_PID 	  	  0x0001
/* Average Current */
#define AVERAGE_INPUT_CURRENT_PID 	                        	  0x0002
/* Average_Voltage */
#define AVERAGE_INPUT_VOLTAGE_PID 	  	                          0x0003
/* Average Current */
#define AVERAGE_OUTPUT_CURRENT_PID 	  	                          0x0004
/* Average_Voltage */
#define AVERAGE_OUTPUT_VOLTAGE_PID 	  	                          0x0005
/* Luminous Intensity */
#define CENTER_BEAM_INTENSITY_AT_FULL_POWER _PID	  	          0x0006
/* Chromaticity Tolerance */
#define CHROMATICITY_TOLERANCE_PID 	  	                          0x0007
/* Cie 13.3-1995 Color Rendering Index */
#define COLOR_RENDERING_INDEX_R9_PID 	  	                          0x0008
/* Cie 13.3-1995 Color Rendering Index */
#define COLOR_RENDERING_INDEX_RA_PID 	  	                          0x0009
/* Gap.Appearance */
#define DEVICE_APPEARANCE_PID 	  	                                  0x000A
/* Country Code */
#define DEVICE_COUNTRY_OF_ORIGIN_PID 	                          	  0x000B
/* Date Utc */
#define DEVICE_DATE_OF_MANUFACTURE_PID 	                   	          0x000C
/* Energy */
#define DEVICE_ENERGY_USE_SINCE_TURN_ON_PID 	                  	  0x000D
/* Fixed_String 8 */
#define DEVICE_FIRMWARE_REVISION_PID 	                        	  0x000E
/* Global Trade Item_Number */
#define DEVICE_GLOBAL_TRADE_ITEM_NUMBER_PID 	                 	  0x000F
/* Fixed_String 16 */
#define DEVICE_HARDWARE_REVISION _PID	  	                          0x0010
/* Fixed_String 36 */
#define DEVICE_MANUFACTURER_NAME_PID 	                         	  0x0011
/* Fixed_String 24 */
#define DEVICE_MODEL_NUMBER_PID 	                        	  0x0012
/* Temperature Range */
#define DEVICE_OPERATING_TEMPERATURE_RANGE_SPECIFICATION_PID              0x0013
/* Temperature_Statistics */
#define DEVICE_OPERATING_TEMPERATURE_STATISTICAL_VALUES_PID               0x0014
/* Event_Statistics */
#define DEVICE_OVER_TEMPERATURE_EVENT_STATISTICS_PID                      0x0015
/* Power_Specification */
#define DEVICE_POWER_RANGE_SPECIFICATION_PID	                 	  0x0016
/* Time_Hour 24 */
#define DEVICE_RUNTIME_SINCE_TURN_ON_PID 	                 	  0x0017
/* Time_Hour 24 */
#define DEVICE_RUNTIME_WARRANTY_PID 	  	                          0x0018
/* Fixed_String 16 */
#define DEVICE_SERIAL_NUMBER_PID 	  	                          0x0019
/* Fixed_String 8 */
#define DEVICE_SOFTWARE_REVISION_PID 	  	                          0x001A
/* Event_Statistics */
#define DEVICE_UNDER_TEMPERATURE_EVENT_STATISTICS_PID	          	  0x001B
/* Temperature 8_Statistics */
#define INDOOR_AMBIENT_TEMPERATURE_STATISTICAL_VALUES_PID 	  	  0x001C
/* Chromaticity Coordinates */
#define INITIAL_CIE 1931_CHROMATICITY_COORDINATES_PID 	                  0x001D
/* Correlated Color Temperature */
#define INITIAL_CORRELATED_COLOR_TEMPERATURE_PID	  	          0x001E
/* Luminous_Flux */
#define INITIAL_LUMINOUS_FLUX_PID 	  	                          0x001F
/* Chromatic Distance_From_Planckian */
#define INITIAL_PLANCKIAN_DISTANCE_PID 	  	                          0x0020
/* Electric Current_Specification */
#define INPUT_CURRENT_RANGE_SPECIFICATION_PID	  	                  0x0021
/* Electric Current_Statistics */
#define INPUT_CURRENT_STATISTICS_PID 	  	                          0x0022
/* Event_Statistics */
#define INPUT_OVER_CURRENT_EVENT_STATISTICS_PID	  	                  0x0023
/* Event_Statistics */
#define INPUT_OVER_RIPPLE_VOLTAGE_EVENT_STATISTICS_PID	                  0x0024
/* Event_Statistics */
#define INPUT_OVER_VOLTAGE_EVENT_STATISTICS_PID	  	                  0x0025
/* Event_Statistics */
#define INPUT_UNDER_CURRENT_EVENT_STATISTICS_PID	           	  0x0026
/* Event_Statistics */
#define INPUT_UNDER_VOLTAGE_EVENT_STATISTICS_PID	          	  0x0027
/* Voltage_Specification */
#define INPUT_VOLTAGE_RANGE_SPECIFICATION_PID 	          	          0x0028
/* Percentage 8 */
#define INPUT_VOLTAGE_RIPPLE_SPECIFICATION_PID	  	                  0x0029
/* Voltage Statistics */
#define INPUT_VOLTAGE_STATISTICS_PID 	  	                          0x002A
/* Illuminance */
#define LIGHT_CONTROL_AMBIENT_LUXLEVEL_ON_PID	                  	  0x002B
/* Illuminance */
#define LIGHT_CONTROL_AMBIENT_LUXLEVEL_PROLONG_PID	         	  0x002C
/* Illuminance */
#define LIGHT_CONTROL_AMBIENT_LUXLEVEL_STANDBY_PID                        0x002D
/* Perceived_Lightness */
#define LIGHT_CONTROL_LIGHTNESS_ON_PID 	           	                  0x002E
/* Perceived_Lightness */
#define LIGHT_CONTROL_LIGHTNESS_PROLONG_PID 	                 	  0x002F
/* Perceived_Lightness */
#define LIGHT_CONTROL_LIGHTNESS_STANDBY_PID 	                 	  0x0030
/* Percentage 8 */
#define LIGHT_CONTROL_REGULATOR_ACCURACY_PID 	                 	  0x0031
/* Coefficient */
#define LIGHT_CONTROL_REGULATOR_KID_PID 	                 	  0x0032
/* Coefficient */                                                         
#define LIGHT_CONTROL_REGULATOR_KIU_PID 	                 	  0x0033
/* Coefficient */                                                         
#define LIGHT_CONTROL_REGULATOR_KPD_PID 	                 	  0x0034
/* Coefficient */                                                         
#define LIGHT_CONTROL_REGULATOR_KPU_PID 	                 	  0x0035
/* Time Millisecond 24 */                                                 
#define LIGHT_CONTROL_TIME_FADE_PID 	  	                          0x0036
/* Time Millisecond 24 */
#define LIGHT_CONTROL_TIME_FADE_ON_PID 	                                  0x0037
/* Time Millisecond 24 */
#define LIGHT_CONTROL_TIME_FADE_STANDBY_AUTO_PID	          	  0x0038
/* Time Millisecond 24 */                                                 
#define LIGHT_CONTROL_TIME_FADE_STANDBY_MANUAL_PID 	          	  0x0039
/* Time Millisecond 24 */
#define LIGHT_CONTROL_TIME_OCCUPANCY_DELAY_PID 	  	                  0x003A
/* Time Millisecond 24 */                                                 
#define LIGHT_CONTROL_TIME_PROLONG_PID 	         	                  0x003B
/* Time Millisecond 24 */
#define LIGHT_CONTROL_TIME_RUN_ON_PID 	  	                          0x003C
/* Percentage 8 */
#define LUMEN_MAINTENANCE_FACTOR_PID 	                             	  0x003D
/* Luminous_Efficacy */
#define LUMINOUS_EFFICACY_PID 	  	                                  0x003E
/* Luminous_Energy */
#define LUMINOUS_ENERGY_SINCE_TURN_ON_PID            	        	  0x003F
/* Luminous_Exposure */
#define LUMINOUS_EXPOSURE_PID 	  	                                  0x0040
/* Luminous_Flux Range */
#define LUMINOUS_FLUX_RANGE_PID 	                         	  0x0041
/* Percentage 8 */
#define MOTION_SENSED_PID 	  	                                  0x0042
/* Percentage 8 */                                                        
#define MOTION_THRESHOLD_PID 	  	                                  0x0043
/* Event_Statistics */
#define OPEN_CIRCUIT_EVENT_STATISTICS_PID      	          	          0x0044
/* Temperature 8_Statistics */
#define OUTDOOR_STATISTICAL_VALUES_PID 	                                  0x0045
/* Electric Current Range */
#define OUTPUT_CURRENT_RANGE_PID 	                              	  0x0046
/* Electric Current_Statistics */                                         
#define OUTPUT_CURRENT_STATISTICS_PID 	  	                          0x0047
/* Percentage 8 */
#define OUTPUT_RIPPLE_VOLTAGE_SPECIFICATION_PID	  	                  0x0048
/* Voltage_Specification */ 
#define OUTPUT_VOLTAGE_RANGE_PID                                          0x0049
/* Voltage_Statistics */
#define OUTPUT_VOLTAGE_STATISTICS_PID 	                    	          0x004A
/* Event_Statistics */
#define OVER_OUTPUT_RIPPLE_VOLTAGE_EVENT_STATISTICS_PID	                  0x004B
/* Count 16 */
#define PEOPLE_COUNT_PID 	                              	          0x004C
/* Boolean */                                                             
#define PRESENCE_DETECTED_PID 	                              	          0x004D
/* Illuminance */                                                         
#define PRESENT_AMBIENT_LIGHT_LEVEL_PID 	  	                  0x004E
/* Temperature 8 */                                                       
#define PRESENT_AMBIENT_TEMPERATURE_PID 	  	                  0x004F
/* Chromaticity Coordinates */
#define PRESENT_CIE 1931_CHROMATICITY_COORDINATES_PID 	                  0x0050
/* Correlated Color Temperature */
#define PRESENT_CORRELATED_COLOR_TEMPERATURE_PID	               	  0x0051
/* Power */
#define PRESENT_DEVICE_INPUT_POWER_PID 	  	                          0x0052
/* Percentage 8 */                                                        
#define PRESENT_DEVICE_OPERATING_EFFICIENCY_PID	               	          0x0053
/* Temperature */
#define PRESENT_DEVICE_OPERATING_TEMPERATURE_PID             	  	  0x0054
/* Illuminance */                                                         
#define PRESENT_ILLUMINANCE_PID 	  	                          0x0055
/* Temperature 8 */
#define PRESENT_INDOOR_AMBIENT_TEMPERATURE_PID	              	          0x0056
/* Electric Current */
#define PRESENT_INPUT_CURRENT_PID 	  	                          0x0057
/* Percentage 8 */                                                        
#define PRESENT_INPUT_RIPPLE_VOLTAGE_PID 	                 	  0x0058
/* Voltage */                                                             
#define PRESENT_INPUT_VOLTAGE_PID 	  	                          0x0059
/* Luminous_Flux */                                                       
#define PRESENT_LUMINOUS_FLUX_PID 	  	                          0x005A
/* Temperature 8 */
#define PRESENT_OUTDOOR_AMBIENT_TEMPERATURE_PID	           	          0x005B
/* Electric Current */
#define PRESENT_OUTPUT_CURRENT_PID 	  	                          0x005C
/* Voltage */                                                             
#define PRESENT_OUTPUT_VOLTAGE_PID 	                         	  0x005D
/* Chromatic Distance_From_Planckian */
#define PRESENT_PLANCKIAN_DISTANCE_PID                                    0x005E
/* Percentage 8 */
#define PRESENT_RELATIVE_OUTPUT_RIPPLE_VOLTAGE_PID          	          0x005F
/* Energy In_A_Period Of Day */                                           
#define RELATIVE_DEVICE_ENERGY_USE_IN_A_PERIOD_OF_DAY_PID	          0x0060
/* Relative Runtime In_A_Generic_Level Range */                           
#define RELATIVE_DEVICE_RUNTIME_IN_A_GENERIC_LEVEL_RANGE_PID	          0x0061
/* Relative_Value In_An Illuminance Range */                              
#define RELATIVE_EXPOSURE_TIME_IN_AN_ILLUMINANCE_RANGE_PID  	          0x0062
/* Luminous_Energy */
#define RELATIVE_RUNTIME_IN_A_CORRELATED_COLOR_TEMPERATURE_RANGE_PID      0x0063
/* Relative_Value In_A Temperature Range */                               
#define RELATIVE_RUNTIME_IN_A_DEVICE_OPERATING_TEMPERATURE_RANGE_PID      0x0064
/* Relative Runtime In_A Current Range */
#define RELATIVE_RUNTIME_IN_AN_INPUT_CURRENT_RANGE_PID 	    	          0x0065
/* Relative_Value In_A_Voltage Range */                                   
#define RELATIVE_RUNTIME_IN_AN_INPUT_VOLTAGE_RANGE_PID 	                  0x0066
/* Event_Statistics */
#define SHORT_CIRCUIT_EVENT_STATISTICS_PID 	         	          0x0067
/* Time_Second 16 */                                                      
#define TIME_SINCE_MOTION_SENSED_PID 	  	                          0x0068
/* Time Second 16 */                                                      
#define TIME_SINCE_PRESENCE_DETECTED_PID 	                      	  0x0069
/* Energy */                                                              
#define TOTAL_DEVICE_ENERGY_USE_PID 	  	                          0x006A
/* Count 24 */
#define TOTAL_DEVICE_OFF_ON_CYCLES_PID 	  	                          0x006B
/* Count 24 */ 
#define TOTAL_DEVICE_POWER_ON_CYCLES_PID 	                      	  0x006C
/* Time_Hour 24 */
#define TOTAL_DEVICE_POWER_ON_TIME_PID 	  	                          0x006D
/* Time_Hour 24 */
#define TOTAL_DEVICE_RUNTIME_PID 	  	                          0x006E
/* Time_Hour 24 */                                                        
#define TOTAL_LIGHT_EXPOSURE_TIME_PID 	  	                          0x006F
/* Luminous_Energy */                                                     
#define TOTAL_LUMINOUS_ENERGY_PID 	  	                          0x0070
/* Temperature 8 */                                                       
#define DESIRED_AMBIENT_TEMPERATURE_PID 	 	                  0x0071

#define PRESSURE_PID                                                      0x2A6D
#define HUMIDITY_PID                                                      0x2A6F
#define TIME_OF_FLIGHT_PID                                                0X2A7F
#define MAGNETO_METER_PID                                                 0x2AA1
#define ACCELERO_METER_PID                                                0x2BA1
#define GYROSCOPE_PID                                                     0x2BA2
#define VOLTAGE_PID                                                       0x0005
#define CURRENT_PID                                                       0x0004
#define POWER_FACTOR_PID                                                  0x0072
#define ACTIVE_POWER_PID                                                  0x0073
#define REACTIVE_POWER_PID                                                0x0074
#define APPARENT_POWER_PID                                                0x0075
#define ACTIVE_ENERGY_PID                                                 0x0083
#define REACTIVE_ENERGY_PID                                               0x0084
#define APPARENT_ENERGY_PID                                               0x0085

#define PROPERTY_ID_LENGTH                                                     2

#define ONE_BYTE_VALUE                                                         1
#define TWO_BYTE_VALUE                                                         2
#define THREE_BYTE_VALUE                                                       3
#define FOUR_BYTE_VALUE                                                        4

#define PROPERTY_ID_PROHIBITED                                            0x0000


/** @addtogroup MODEL_GENERIC
*  @{
*/

/** @addtogroup Generic_Model_Callbacks
*  @{
*/

/* Exported types ------------------------------------------------------------*/


/**
  * transition status enum
 */
typedef enum
{
  TRANSITION_STATUS_STOP = 0,
  TRANSITION_STATUS_DELAY,
  TRANSITION_STATUS_RUNNING
}transition_status_e;


/**
  * transition parameters struct
 */
typedef struct
{
  MOBLEUINT32 stepResolutionMs;
  MOBLEUINT32 trTimeMs;
  MOBLEUINT32 trBeginTick;
  MOBLEUINT32 trEndTick;
  MOBLEUINT32 trNextActionTick;
  transition_status_e trStatus;
  MOBLEUINT8 publishEventTrig;
  MOBLEUINT8 res[2];
} transition_params_t;


/**
  * status send (in reply to get, set & setunack messages) enum
 */
typedef enum
{
  STATUS_SEND_REPLY = 0,
  STATUS_SEND_PUBLISH,
  STATUS_SEND_REPLY_PUBLISH
} status_send_e;


/**
  * transition event enum
 */
typedef enum
{
  TRANSITION_EVENT_NO = 0,
  TRANSITION_EVENT_ABORT,
  TRANSITION_EVENT_DELAY,
  TRANSITION_EVENT_TIMER_START,
  TRANSITION_EVENT_TIMER_TRIG,
  TRANSITION_EVENT_PUBLISH,
  TRANSITION_EVENT_TIMER_STOP
}transition_event_e;


/**
  * Model tid value structure
 */
typedef struct
{
  MOBLEUINT8 Tid_Value;
  MOBLEUINT8 TidSend;
  MOBLE_ADDRESS Peer_Addrs;
  MOBLE_ADDRESS Dst_Addrs;
}Model_Tid_t;


/**
  * TID params struct
 */
typedef struct
{
  MOBLEUINT32 tidTick;
  MOBLE_ADDRESS src;
  MOBLE_ADDRESS dst;
  MOBLEUINT8 tid;
  MOBLEUINT8 res[3];
}tid_param_t;


/**
  * variable used for binding status
 */
typedef struct
{
  MOBLEUINT16 Model_Rx_Opcode;  
  MOBLEUINT16 Model_ID;
  MOBLE_ADDRESS Dst_Peer;
}Model_Binding_Var_t;

typedef struct
{
  MOBLEUINT8 Restore_Flag;
  MOBLEUINT8 LightTransitionStatus;
  MOBLEUINT8 GenericTransitionStatus;
  MOBLEUINT8 PowerOnOff;
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF  
  MOBLEUINT8 Generic_OnOff; 
  MOBLEUINT8 GenericTarget;
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL  
  MOBLEUINT16 Generic_Level;
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
  MOBLEUINT16 LightLightness;
  MOBLEUINT16 LightTarget;
  MOBLEUINT16 LightDefault;
  MOBLEUINT16 LightLast;
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL
  MOBLEUINT16 CtlLightness;
  MOBLEUINT16 CtlTemperature;
  MOBLEUINT16 CtlDelta;
  MOBLEUINT16 CtlDefaultLightness;
  MOBLEUINT16 CtlDefaultTemperature;
  MOBLEUINT16 CtlDefaultDelta;
  MOBLEUINT16 CtlLightTarget;
  MOBLEUINT16 CtlTempTarget;
  MOBLEUINT16 CtlDeltaTarget;
  
#endif  

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL
  MOBLEUINT16 HslLightness;
  MOBLEUINT16 HslHue;
  MOBLEUINT16 HslSaturation; 
  MOBLEUINT16 HslLightnessDefault;
  MOBLEUINT16 HslHueDefault;
  MOBLEUINT16 HslSaturationDefault;
  MOBLEUINT16 HslLightTarget;
  MOBLEUINT16 HslHueTarget;
  MOBLEUINT16 HslSatTarget;
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC 
  MOBLEUINT8 LightLCBuff[APP_NVM_LIGHT_LC_MODEL_SIZE]; 
#endif

}Models_ParamNvm_t;
/* Exported functions ------------------------------------------------------- */
MOBLE_RESULT Chk_OptionalParamValidity(MOBLEUINT8 param_length, MOBLEUINT8
                                        mandatory_length, MOBLEUINT8 param,
                                                  MOBLEUINT8 max_param_value  );
MOBLE_RESULT Chk_ParamMinMaxValidity(MOBLEUINT16 min_param_value, 
                                                 const MOBLEUINT8* param,
                                                 MOBLEUINT16 max_param_value );

MOBLE_RESULT Chk_ParamValidity(MOBLEUINT8 param, MOBLEUINT8 max_param_val );
 
MOBLE_RESULT Chk_RangeValidity(MOBLEUINT16 min_param_value, const MOBLEUINT8* param, 
                                                     MOBLEUINT16 max_param_value );
MOBLE_RESULT  Chk_HslRangeValidity(const MOBLEUINT8* param,MOBLEUINT16 min_param_value_1, 
                                   MOBLEUINT16 max_param_value_1,MOBLEUINT16 min_param_value_2,
                                     MOBLEUINT16 max_param_value_2);
MOBLE_RESULT  Chk_TwoParamValidity(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                        const MOBLEUINT8* param);
MOBLE_RESULT  Chk_MultiParamValidity(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                      MOBLEINT16 min_param_range3, MOBLEUINT16 max_param_range3,
                                        const MOBLEUINT8* param);
MOBLE_RESULT  Chk_MultiParamValidityAllUnsigned(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                      MOBLEINT16 min_param_range3, MOBLEUINT16 max_param_range3,
                                        const MOBLEUINT8* param);
MOBLE_RESULT Chk_ParamMinMaxIntValidity(MOBLEINT16 min_param_value, const MOBLEUINT8* param, 
                                                     MOBLEINT16 max_param_value );
MOBLE_RESULT Chk_TidValidity(MOBLE_ADDRESS peer_Addrs,MOBLE_ADDRESS dst_Addrs,MOBLEUINT8 tidValue);
MOBLEUINT8 Tid_CheckAndUpdate(MOBLEUINT8 currentMsgTid,
                             MOBLE_ADDRESS currentMsgSrc,
                             MOBLE_ADDRESS currentMsgDst,
                             tid_param_t* pLastMsgTidParams);
MOBLE_RESULT TimeDttFGet(MOBLEUINT32 timeMs, MOBLEUINT8* timeDttF);
MOBLEUINT32 Get_StepResolutionValue(MOBLEUINT8 time_param);

float Ratio_CalculateValue(MOBLEUINT16 setValue , MOBLEUINT16 maxRange , MOBLEINT16 minRange);
void TraceHeader(const char* func_name, int mode);
#ifdef ENABLE_SAVE_MODEL_STATE_NVM
MOBLE_RESULT SaveModelsStateNvm(MOBLEUINT8* flag);
#endif
MOBLEUINT8 BLE_GetElementNumber(void);

void Test_Process(void);
void ModelSave_Process(void);
void BLEMesh_PacketResponseTime(MOBLEUINT8 *testFunctionParm);
MOBLEUINT8 Time_Conversion(MOBLEUINT32 lc_Time);
void Model_RestoreStates(MOBLEUINT8 const *pModelState_Load, MOBLEUINT8 size);

void Generic_OnOffPowerOnValue(Models_ParamNvm_t *state_Value, MODEL_MessageHeader_t *pmsgParam);
MOBLEUINT16 Light_lightnessPowerOnValue(Models_ParamNvm_t *state_Value, MODEL_MessageHeader_t *pmsgParam);                                       
void Light_CtlPowerOnValue(Models_ParamNvm_t *state_Value, MODEL_MessageHeader_t *pmsgParam);
void Light_HslPowerOnValue(Models_ParamNvm_t *state_Value, MODEL_MessageHeader_t *pmsgParam);

void MemoryDumpHex(const MOBLEUINT8* memory_addr, int size);
void PutLittleEndian(MOBLEUINT8* stream, MOBLEUINT32 value, MOBLEUINT8 octets);
void GetLittleEndian(MOBLEUINT8 const *stream, MOBLEUINT8 octets, MOBLEUINT32* result);

void CopyU8LittleEndienArray_fromU16word (MOBLEUINT8* pArray, MOBLEUINT16 inputWord);
void CopyU8LittleEndienArray_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord);
void CopyU8LittleEndienArray_2B_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord);
MOBLEUINT16 CopyU8LittleEndienArrayToU16word (MOBLEUINT8* pArray);
MOBLEUINT32 CopyU8LittleEndienArrayToU32word (MOBLEUINT8* pArray);
void NvmStatePowerFlag_Set(MOBLEUINT8 nvmModelFlag, MOBLEUINT8 elementIndex);
#if 0 //#ifdef ENABLE_NVM_TEST
void Device_FlashTesting(void);
#endif
transition_event_e Transition_Stop(transition_status_e* status);
transition_event_e Transition_Sm(transition_params_t* transitionParams,
                                MOBLEUINT32 delayMs);
MOBLEUINT32 Transition_RemainingTimeGet(transition_params_t* transitionParams);
MOBLEUINT32 Transition_TimerGet(transition_params_t* pTrParams);
MOBLEUINT32 Transition_TimeToNextActionGet(transition_params_t* pTrParams);
MOBLEUINT32 Transition_StateValueGet(MOBLEUINT32 finalState, 
                                    MOBLEUINT32 initialState, 
                                    MOBLEUINT32 timer, 
                                    MOBLEUINT32 transitionTime);
MOBLE_RESULT ExtractPropertyId(const MOBLEUINT8* data,
                                      MOBLEUINT16* pPropertyId);
MOBLE_RESULT Binding_GenericOnOff_LightLcLightOnOff(MOBLEUINT8 genericElementIndex, 
                                                    MOBLEUINT8 genericOnOff,
                                                    MOBLEUINT8 optionalParams,
                                                    MOBLEUINT32 delayMs,
                                                    MOBLEUINT8 transitionParam);
MOBLE_RESULT Binding_LightLcLightOnOff_GenericOnOff(MOBLEUINT8 lcElementIndex, MOBLEUINT8 lcOnOff);
MOBLE_RESULT Binding_LcLinearOut_LightLightnessLinear(MOBLEUINT8 lcElementIndex, MOBLEUINT16 lcLinearOut);
void BindingDisable_LcLinearOut_LightLightnessLinear(MOBLEUINT8 lightnessLinearElementIndex);
MOBLEBOOL Wait_RandomTime(void);

#endif /* __COMMON_H */



