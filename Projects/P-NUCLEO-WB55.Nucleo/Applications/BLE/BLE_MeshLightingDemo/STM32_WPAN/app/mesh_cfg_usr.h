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

/* Macros Used for user defined serial print data for models. Either use 0 to disable or 1 to enable
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
#define TF_GENERIC                         0
#define TF_LIGHT                           0
#define TF_SENSOR                          0
#define TF_VENDOR                          0
#define TF_NEIGHBOUR                       0
#define TF_PROVISION                       1
#define TF_LPN_FRND                        0
#define TF_ELEMENTS                        0
#define TF_ADDRESS                         0
#define TF_HANDLER                         1
#define TF_INIT                            1
#define TF_MISC                            0
#define TF_SERIAL_CTRL                     0

/*******************************************************************************
*** Following section helps to select right configuration of Models  ***********
*******************************************************************************/
/******* Define the following Macros to enable the usage of the  Models  ******/

/* Define the following Macros to enable the usage of the Server Generic Models  */
#define ENABLE_GENERIC_MODEL_SERVER_ONOFF 
#define ENABLE_GENERIC_MODEL_SERVER_LEVEL
/*
#define ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME
#define ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
#define ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP
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

/* Define the following Macros to enable the usage of the Light Models  */

#define ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
#define ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_CTL
#define ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
//#define ENABLE_LIGHT_MODEL_SERVER_HSL 
//#define ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP  
//#define ENABLE_LIGHT_MODEL_SERVER_HSL_HUE 
//#define ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION 

/*
#define ENABLE_LIGHT_MODEL_SERVER_XYL 
#define ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_LC 
#define ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 
*/

/* Define the following Macros to enable the usage of the Sensor Models  */
#define ENABLE_SENSOR_MODEL_SERVER
//#define ENABLE_SENSOR_MODEL_SERVER_SETUP

/*
Macros are defined to enable the setting for the PWM. these Macros are given for 
custom board of BLE, RGB board and default transition 
inserted in generic on off.
NOTE- Only one macro must be enabled at one time , otherwise get confliction and 
      firmware will not work properly.
*/
//#define STEVAL_BLUENRG_1_BOARD_PWM_SELECTION
//#define STEVAL_BLUENRG_2_BOARD_PWM_SELECTION
//#define STEVAL_BLUEMIC_1_BOARD_PWM_SELECTION
//#define CUSTOM_BOARD_PWM_SELECTION


/* Macro to enable default transition for generic on off message.*/
//#define GENERIC_ONOFF_DEFAULT_TRANSITION_ENABLE

/* 
Define the Macros for Enabling/disabling the binding of data between the Generic and 
Light model.
@ define the Macro for enabling the binding
@ Undefine or comment the macro for disabling the binding.
*/
//#define ENABLE_MODEL_BINDING

/*
Define the Macro for enabling/disabling the Publishing with Generic on off
or by Vendor Model.
@ define Macro for Vendor Publishing
@ Undefine or comment Macro for Generic On Off Publishing
*/
//#define VENDOR_MODEL_PUBLISH
 
/* Define the macros for the numbers of sensor present.*/
#define NUMBER_OF_SENSOR                   2

/*
Macro is responsible for enabling and desabling the sensor publication.
Comment this macro to disable the publication
*/
//#define ENABLE_SENSOR_PUBLICATION

/* Define the following Macros to change the step resolution and step count value */
#define TRANSITION_SCALER                  4
#define PWM_TIME_PERIOD                    31990U
//#define PWM_DEFAULT_VALUE                  10000U

/* Macros are used to update the PWM state according to the condition. */
//#define         RESUME_STATE       1
//#define         RESET_STATE        2
//#define         DEFAULT_STATE      3
//#define         LOAD_STATE         4

/* 
Define the following Macro to save Generic and Light Model states in NVM
This may result into excessive flash erase operations, this should be avoided to ensure flash longevity
*/
#define ENABLE_SAVE_MODEL_STATE_NVM

/*******************************************************************************
*** Following section helps to configure the Application of Mesh     ***********
*******************************************************************************/

#define APPLICATION_NUMBER_OF_ELEMENTS     1

/* Contains a 16-bit company identifier assigned by the Bluetooth SIG
(the list is available @ https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers )
For STMicroelectronics : it is 0x0030 */
#define COMPANY_ID                         0x0030

/* Contains a 16-bit vendor-assigned product identifier */
#define PRODUCT_ID                         0x0001

/* Contains a 16-bit vendor-assigned product version ID */
#define PRODUCT_VERSION_ID                 0x0001

#define MAX_APPLICATION_PACKET_SIZE        160

/*******************************************************************************
********** MAC Address Configuration *******************************************
*******************************************************************************/

/*
*  Define to enable external MAC handling, 
*  DONT change the Values 0x01, 0x02, 0x03 
*  SELECT ONLY One of the Option 
*/
//#define GENERATE_STATIC_RANDOM_MAC         0x01   
//#define EXTERNAL_MAC_ADDR_MGMT             0x02
#define INTERNAL_UNIQUE_NUMBER_MAC         0x03

/* Declare this value as 0x01 if the External Address is PUBLIC Address */
/* For example like this: #define EXTERNAL_MAC_IS_PUBLIC_ADDR 0x1       */
/* Else, by default, the external address will be treated as RANDOM     */
#define EXTERNAL_MAC_IS_PUBLIC_ADDR        0x0

/******************************************************************************/

#define STATIC_OOB_SIZE                    16U

/*******************************************************************************
****** Following section helps to select right configuration of node ***********
*******************************************************************************/
/* 
*  Minimum gap between successive transmissions
*  varies from 10 to 65535 
*/
#define TR_GAP_BETWEEN_TRANSMISSION      50U


/*
*  Different features supported by BLE-Mesh. Uncomment according to application.
*      Low power feature enabled node do not support other features. 
*      Do not define any other feature if Low Power feature is defined
*/
#define ENABLE_RELAY_FEATURE
#define ENABLE_PROXY_FEATURE
#define ENABLE_FRIEND_FEATURE
/* #define ENABLE_LOW_POWER_FEATURE */


/*
* Friend node receive window size is fixed at 255 ms
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
#define FN_NO_OF_LPNS                    2U


/*
*  Enable or disable neighbor table
*  if defined -> enabled
*  if not defined -> disabled
*/
#define ENABLE_NEIGHBOR_TABLE

/*
*  Size of neighbor table
*/
#define NEIGHBOR_COUNT                   5U

/* 
*  (unit s)
*  Duration (wrt appear or last refresh) for which neighbor exists in neighbor table
*/
#define NEIGHBOR_ALIVE_TIME              20U

/* 
*  Enable/disable neighbor table update with unprovisioned device beacon
*  0: Disable neighbor table update with unprovisioned device beacon
*  1: Enable neighbor table update with unprovisioned device beacon
*/
#define NEIGHBOR_UNPRVND_DEV_BEACON_NTU  0U

/* 
*  Enable/disable neighbor table update with secure network beacon
*  0: Disable neighbor table update with secure network beacon
*  1: Enable neighbor table update with secure network beacon
*/
#define NEIGHBOR_SECURE_NET_BEACON_NTU   1U

/* 
*  Enable/disable neighbor table update with TTL 0 message
*  0: Disable neighbor table update messages
*  1: Enable neighbor table update with messages with 0 TTL
*  2: Enable neighbor table update with messages with any TTL
*/
#define NEIGHBOR_MSG_TTLX_NTU            1U

/* Exported variables  ------------------------------------------------------- */
/* Exported Functions Prototypes ---------------------------------------------*/

#endif /* __MESH_CFG_USR_H */
