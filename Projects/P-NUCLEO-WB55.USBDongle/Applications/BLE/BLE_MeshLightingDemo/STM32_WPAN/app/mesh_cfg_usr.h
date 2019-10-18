/**
******************************************************************************
* @file    mesh_cfg_usr.h
* @author  BLE Mesh Team
* @version V1.10.000
* @date    15-Jan-2019
* @brief   Header file for mesh_usr_cfg.c 
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
#ifndef __MESH_CFG_USR_H
#define __MESH_CFG_USR_H

/* Includes ------------------------------------------------------------------*/
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/

/* Macros Used for user defined serial print data for models. 
Either use 0 to disable or 1 to enable
@  TF_GENERIC is responsible for the Generic model traces.
@  TF_LIGHT is responsible for the Light model traces.
@  TF_SENSOR is responsible for the Sensor model traces.
@  TF_VENDOR is responsible for the vendor model traces.
@  TF_NEIGHBOUR is responsible for the neighbour function traces.
@  TF_PROVISION is responsible for the Provision related function traces.
@  TF_LPN_FRND is responsible for the Low power nodef function traces.
@  TF_ELEMENTS is responsible for the element selection function traces.
@  TF_ADDRESS is responsible for the address of element traces.
@  TF_HANDLER is responsible for the interrupt file traces.
@  TF_INIT is responsible for the main function traces.
@  TF_MISC is responsible for the other type traces.

*/
#define TF_GENERIC                                                             0
#define TF_LIGHT                                                               0
#define TF_LIGHT_LC                                                            0
#define TF_SENSOR                                                              0
#define TF_VENDOR                                                              0
#define TF_NEIGHBOUR                                                           0
#define TF_LPN_FRND                                                            0
#define TF_ELEMENTS                                                            0
#define TF_ADDRESS                                                             0
#define TF_PROVISION                                                           0
#define TF_HANDLER                                                             0
#define TF_INIT                                                                0
#define TF_MISC                                                                0
#define TF_SERIAL_CTRL                                                         0

/*******************************************************************************
*** Following section helps to select right configuration of Models  ***********
*******************************************************************************/
/******* Define the following Macros to enable the usage of the  Models  ******/

/* Define the following Macros to enable the usage of the Server Generic Models  */
#define ENABLE_GENERIC_MODEL_SERVER_ONOFF 
#define ENABLE_GENERIC_MODEL_SERVER_LEVEL
#define ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
#define ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP
//#define ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME

/* The Following Models are not available in this version, will be developed in 
   next version.
*/
/*
#define ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL
#define ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP
#define ENABLE_GENERIC_MODEL_SERVER_BATTERY
#define ENABLE_GENERIC_MODEL_SERVER_LOCATION
#define ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP
#define ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY
#define ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY
#define ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY
#define ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY
*/

/* Define the following Macros to enable the usage of the Client Generic Models  */
/*
#define ENABLE_GENERIC_MODEL_CLIENT_ONOFF 
#define ENABLE_GENERIC_MODEL_CLIENT_LEVEL
*/

/******************************************************************************/
/******* Define the following Macros to enable the usage of the Light Models  */
/******************************************************************************/

#define ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
#define ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_CTL
#define ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
#define ENABLE_LIGHT_MODEL_SERVER_HSL 
#define ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP  
#define ENABLE_LIGHT_MODEL_SERVER_HSL_HUE 
#define ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION 

/* 
   The following Models are managed in different file light_lc.c in middleware
*/
#define ENABLE_LIGHT_MODEL_SERVER_LC 
#define ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 

/* 
   The Following Models are not available in this version
*/
//#define ENABLE_LIGHT_MODEL_SERVER_XYL 
//#define ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP 


/* Following Macro helps to know if the Fixed functions are needed or not 
   DO NOT change or add any space at the end of the file */
#if defined(ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_CTL) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_HSL) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_HSL_HUE) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_LC) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_LC_SETUP) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_XYL) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP)
  #define  ENABLE_LIGHT_MODEL_SERVER  
#endif
      
#if defined(ENABLE_LIGHT_MODEL_SERVER_LC) \
    || defined(ENABLE_LIGHT_MODEL_SERVER_LC_SETUP)
   #define  ENABLE_LIGHT_LC_MODEL_SERVER  
#endif      
/******************************************************************************/
/*
Define the Macros for Enabling/disabling the binding of data between the Generic 
and Light model.
@ define the Macro for enabling the binding
@ Undefine or comment the macro for disabling the binding.
*/
/******************************************************************************/
//#define ENABLE_MODEL_BINDING


/******************************************************************************/
/* Define the following Macros to enable the usage of the Sensor Models  */
/******************************************************************************/

#define ENABLE_SENSOR_MODEL_SERVER
#define ENABLE_SENSOR_MODEL_SERVER_SETUP

/* Define the macros for the numbers of sensor present.*/
#define NUMBER_OF_SENSOR                   2
#define ENABLE_OCCUPANCY_SENSOR
/*
Macro is responsible for enabling and desabling the sensor publication.
Comment this macro to disable the publication
*/
//#define ENABLE_SENSOR_PUBLICATION

/******************************************************************************/
/* Define the following Macros to enable the usage of the time and scene Models  */
/******************************************************************************/

//#define ENABLE_TIME_MODEL_SERVER
//#define ENABLE_TIME_MODEL_SERVER_SETUP
//#define ENABLE_SCENE_MODEL_SERVER
//#define ENABLE_SCENE_MODEL_SERVER_SETUP

/******************************************************************************/
/*
Macros are defined to enable the setting for the PWM. these Macros are given for 
eval board of BlueNRG-1 and BlueNRG-2, costom board and STEVAL-BLUEMIC-1 board
and default transition inserted in generic on off.
IMPORTATNT NOTE-  STEVAL_BLUENRG_1_BOARD_PWM_SELECTION
                  STEVAL_BLUENRG_2_BOARD_PWM_SELECTION
                  STEVAL_BLUEMIC_1_BOARD_PWM_SELECTION
                  CUSTOM_BOARD_PWM_SELECTION
                  GENERIC_ONOFF_DEFAULT_TRANSITION_ENABLE
Only one macro must be enabled at one time from list of mocros provided here,
otherwise get confliction and firmware will not work properly.
*/
/******************************************************************************/
//#define STEVAL_BLUENRG_1_BOARD_PWM_SELECTION
//#define STEVAL_BLUENRG_2_BOARD_PWM_SELECTION
//#define STEVAL_BLUEMIC_1_BOARD_PWM_SELECTION
//#define CUSTOM_BOARD_PWM_SELECTION


/******************************************************************************/
/*
Define the Macro for enabling/disabling the Publishing with Generic on off
or by Vendor Model.
@ define Macro for Vendor Publishing
@ Undefine or comment Macro for Generic On Off Publishing
*/
//#define VENDOR_MODEL_PUBLISH
 
/******************************************************************************/


/* Maximum Time period value of PWM */
#define PWM_TIME_PERIOD                    31990U

/******************************************************************************/
/***** MACROS for POWER ON-OFF CYCLE BASED UNPROVISIONING *********************/
/******************************************************************************/

/*
Define the following Macro to check the Power-OnOff Cycles
5 Continous cycles of OnOff with Ontime <2 sec will cause unprovisioning
*/
//#define ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE

#define POWER_ON_OFF_CYCLES_FOR_UNPROVISIONING 5
#define MASK_BITS_FOR_POWER_ON_OFF_CYCLES  (0xFFFFFFFF << POWER_ON_OFF_CYCLES_FOR_UNPROVISIONING)      
      /* 0xFFFFFFE0 */
#define ON_TIME_IN_SECONDS_FOR_POWER_CYCLING 2
#define BLINK_TIMES_FOR_FACTORY_RESET 5

/* 
Define the following Macro to save Generic and Light Model states in NVM
This may result into excessive flash erase operations, this should be avoided to ensure flash longevity
*/
#define ENABLE_SAVE_MODEL_STATE_NVM

/* Only one Macro will be enable at one time */
//#define SAVE_MODEL_STATE_FOR_ALL_MESSAGES         
#define SAVE_MODEL_STATE_POWER_FAILURE_DETECTION

/* Macros defined for the number of bytes saved, number of bytes dedicated for ganeric 
  model and light model.
*/     
#define APP_NVM_GENERIC_MODEL_SIZE        16U
#define APP_NVM_LIGHT_MODEL_SIZE          16U
#define APP_NVM_MODEL_SIZE                40U
      
/*Macros are defined for the selection of the number of led and type of lighting 
  used for the application.
*if board has one led, uncomment USER_BOARD_1LED 
*if board has cool warm leds, uncomment USER_BOARD_COOL_WHITE_LED
*if board has RGB leds, uncomment USER_BOARD_RGB_LED
*if board has RGB leds as well as cool warm leds, uncomment USER_BOARD_COOL_WHITE_LED
  and USER_BOARD_RGB_LED
*/
/* comment ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME when using 
   USER_BOARD_COOL_WHITE_LED ,USER_BOARD_RGB_LED
   Default transition is provided only for one led.
*/      
//#define  USER_BOARD_1LED      
//#define  USER_BOARD_COOL_WHITE_LED
#define  USER_BOARD_RGB_LED 

#if defined(STEVAL_BLUEMIC_1_BOARD_PWM_SELECTION)      
   #define SINGLE_LED      PWM1
#endif    

/*******************************************************************************
*** Following section helps to configure the LEDs of Application of Mesh     ***********
*******************************************************************************/

#if defined STEVAL_BLUENRG_1_BOARD_PWM_SELECTION || defined STEVAL_BLUENRG_2_BOARD_PWM_SELECTION
  #define SINGLE_LED   PWM4
  #define COOL_LED     PWM0
  #define WARM_LED     PWM1
  #define RED_LED      PWM2
  #define GREEN_LED    PWM3
  #define BLUE_LED     PWM4 
#endif   

#if defined CUSTOM_BOARD_PWM_SELECTION || defined USE_STM32WBXX_NUCLEO || defined USE_STM32WBXX_USB_DONGLE
  #define SINGLE_LED   PWM4    
  #define COOL_LED     PWM0  
  #define WARM_LED     PWM1
  #define RED_LED      PWM2
  #define GREEN_LED    PWM3
  #define BLUE_LED     PWM4 
#endif

#ifdef STEVAL_BLUEMIC_1_BOARD_PWM_SELECTION
  #define SINGLE_LED   PWM1   
  #define COOL_LED     PWM0  
  #define WARM_LED     PWM1
  #define RED_LED      PWM2
  #define GREEN_LED    PWM3
  #define BLUE_LED     PWM4 
#endif      
/*******************************************************************************
*** Following section helps to configure the Application of Mesh     ***********
*******************************************************************************/

#define APPLICATION_NUMBER_OF_ELEMENTS                                         1

/* Contains a 16-bit company identifier assigned by the Bluetooth SIG
(the list is available @ https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers )
For STMicroelectronics : it is 0x0030 */
#define COMPANY_ID                                                        0x0030

/* Contains a 16-bit vendor-assigned product identifier */
#define PRODUCT_ID                                                        0x0001

/* Contains a 16-bit vendor-assigned product version ID */
#define PRODUCT_VERSION_ID                                                0x0001

#define MAX_APPLICATION_PACKET_SIZE                                          160

/*******************************************************************************
********** MAC Address Configuration *******************************************
*******************************************************************************/

/*
*  Define to enable external MAC handling, 
*  DONT change the Values 0x01, 0x02, 0x03 
*  SELECT ONLY One of the Option 
*/
//#define GENERATE_STATIC_RANDOM_MAC                                          0x01   
//#define EXTERNAL_MAC_ADDR_MGMT                                              0x02
#define INTERNAL_UNIQUE_NUMBER_MAC                                          0x03

/* Declare this value as 0x01 if the External Address is PUBLIC Address */
/* For example like this: #define EXTERNAL_MAC_IS_PUBLIC_ADDR 0x1       */
/* Else, by default, the external address will be treated as RANDOM     */
#define EXTERNAL_MAC_IS_PUBLIC_ADDR                                          0x0

/******************************************************************************/


/*******************************************************************************
****** Following section helps to select right configuration of node ***********
*******************************************************************************/
/* 
*  Minimum gap between successive transmissions
*  varies from 10 to 65535 
*/
#define TR_GAP_BETWEEN_TRANSMISSION                                          50U


/*
*  Different provision bearer supported by BlueNRG-Mesh. Define according to application.
*      Atleast one of PB-ADV and PB-GATT should be defined
*/
#define ENABLE_PB_ADV
#define ENABLE_PB_GATT
#define ENABLE_PUB_KEY_TYPE_OOB
#define ENABLE_AUTH_TYPE_STATIC_OOB
#define ENABLE_AUTH_TYPE_OUTPUT_OOB
//#define ENABLE_AUTH_TYPE_INPUT_OOB

/* Static OOB Configurations */
#ifdef ENABLE_AUTH_TYPE_STATIC_OOB
#define STATIC_OOB_SIZE                                                      16U 
#else
#define STATIC_OOB_SIZE                                                       0U 
#endif
/* Output OOB Configurations */
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
#define OUTPUT_OOB_SIZE                              1U 
#define OUTPUT_OOB_ACTION                            OUTPUT_OOB_ACTION_BIT_BLINK 
#else
#define OUTPUT_OOB_SIZE                              0U 
#define OUTPUT_OOB_ACTION                            OUTPUT_OOB_ACTION_BIT_BLINK 
#endif

/* Input OOB Configurations */
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
#define INPUT_OOB_SIZE                    1U 
#define INPUT_OOB_ACTION                  INPUT_OOB_ACTION_BIT_PUSH 
#else
#define INPUT_OOB_SIZE                    0U 
#define INPUT_OOB_ACTION                  INPUT_OOB_ACTION_BIT_PUSH 
#endif

/*
*  Different features supported by BlueNRG-Mesh. Uncomment according to application.
*      Low power feature enabled node do not support other features. 
*      Do not define any other feature if Low Power feature is defined
*/
#define ENABLE_RELAY_FEATURE
#define ENABLE_PROXY_FEATURE
#define ENABLE_FRIEND_FEATURE
/* #define ENABLE_LOW_POWER_FEATURE */


/*
* Friend node receive window size is fixed at 50 ms
*/

/* 
* Friend node subscription list size is fixed at 4
*/

/* 
* Friend node maximum number of messages is fixed at 16
*/

/* 
*  Number of Low power nodes that can be associated with Friend node
*  varies from 1 to 10
*/
#define FN_NO_OF_LPNS                                                         2U

/* 
*  For prioritizing friendship offer with good RSSI link
*  varies from 0 to 3
*  Ref @Mesh_v1.0
*/
#define LPN_RSSI_FACTOR_LEVEL                                                 1U

/* 
*  For prioritizing friendship offer with good receive window factor
*  varies from 0 to 3
*  Ref @Mesh_v1.0
*/
#define LPN_RECIVE_WINDOW_FACTOR_LEVEL                                        1U

/* 
*  Minimum packets queue size required
*  varies from 1 to 7
*  Ref @Mesh_v1.0
*/
#define LPN_MINIMUM_QUEUE_SIZE_LOG                                            2U

/* 
*  (unit ms)
*  varies from 0x0A to 0xFF
*  Ref @Mesh_v1.0
*/
#define LPN_RECEIVE_DELAY                                                   150U

/* 
*  Poll timeout value after which friendship cease to exist (unit 100ms)
*  varies from 0x00000A to 0x34BBFF
*  Ref @Mesh_v1.0
*/
#define LPN_POLL_TIMEOUT                                                   2000U

/* 
*  Maximum receive window size acceptable to low power node (unit ms)
*  varies from 10 to 255
*  Ref @Mesh_v1.0
*/
#define LPN_RECEIVE_WINDOW_SIZE                                              55U

/* 
*  Minimum friend's subscription list size capability required by lpn
*  varies from 1 to 5
*  Ref @Mesh_v1.0
*/
#define LPN_SUBSCR_LIST_SIZE                                                  2U

/* 
*  Frequency at which low power node would send friend request (unit 100ms)
*  varies from 0 to 255
*/
#define LPN_FRIEND_REQUEST_FREQUENCY                                         50U

/* 
*  Frequency at which low power node would poll friend node (unit 100ms)
*  should be less than poll timeout
*/
#define LPN_FRIEND_POLL_FREQUENCY                                            25U

/* 
*  Minimum RSSI required by low power node
*  should be less than equal to -60
*/
#define LPN_MINIMUM_RSSI                                                    -100

/* 
*  Retries to be made by lpn before termination of friendship
*  varies from 2 to 10
*/
#define LPN_NO_OF_RETRIES                                                    10U


/*
*  Enable or disable neighbor table
*  if defined -> enabled
*    If MAC address of neighbor changes it appears as new neighbor
*    Neighbor table update can be triggered (configurable) in following cases
*      On receiving Unprovisioned device beacon
*      On receiving Secure network beacon
*      On receiving Mesh message
*        Message with any TTL
*        Message with TTL = 0
*  if not defined -> disabled
*/
#define ENABLE_NEIGHBOR_TABLE

/*
*  Size of neighbor table
*/
#define NEIGHBOR_COUNT                                                        5U

/* 
*  (unit s)
*  Duration (wrt appear or last refresh) for which neighbor exists in neighbor table
*/
#define NEIGHBOR_ALIVE_TIME                                                  20U

/* 
*  Enable/disable neighbor table update with unprovisioned device beacon
*  0: Disable neighbor table update with unprovisioned device beacon
*  1: Enable neighbor table update with unprovisioned device beacon
*/
#define NEIGHBOR_UNPRVND_DEV_BEACON_NTU                                       0U

/* 
*  Enable/disable neighbor table update with secure network beacon
*  0: Disable neighbor table update with secure network beacon
*  1: Enable neighbor table update with secure network beacon
*/
#define NEIGHBOR_SECURE_NET_BEACON_NTU                                        1U

/* 
*  Enable/disable neighbor table update with TTL 0 message
*  0: Disable neighbor table update messages
*  1: Enable neighbor table update with messages with 0 TTL
*  2: Enable neighbor table update with messages with any TTL
*/
#define NEIGHBOR_MSG_TTLX_NTU                                                 1U

/* Exported variables  -------------------------------------------------------*/
/* Exported Functions Prototypes ---------------------------------------------*/

#endif /* __MESH_CFG_USR_H */
