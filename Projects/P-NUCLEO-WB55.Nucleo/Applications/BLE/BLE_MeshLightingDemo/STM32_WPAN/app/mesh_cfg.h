/**
  ******************************************************************************
  * @file    mesh_cfg.h
  * @author  BLE Mesh Team
  * @version V1.09.000
  * @date    15-Oct-2018
  * @brief   Header file for mesh_usr_cfg.c 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef __MESH_CFG_H
#define __MESH_CFG_H

/* Includes ------------------------------------------------------------------*/
#include "ble_mesh.h"

/* Exported macro ------------------------------------------------------------*/

#define APPLICATION_NUMBER_OF_ELEMENTS 1
/*******************************************************************************
*** Following section helps to configure the Application of Mesh     ***********
*******************************************************************************/
/* Contains a 16-bit company identifier assigned by the Bluetooth SIG
(the list is available @ https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers )
For STMicroelectronics : it is 0x0030 */
#define COMPANY_ID 0x0030

/* Contains a 16-bit vendor-assigned product identifier */
#define PRODUCT_ID 0x0001

/* Contains a 16-bit vendor-assigned product version ID */
#define PRODUCT_VERSION_ID 0x0001 


#define MAX_APPLICATION_PACKET_SIZE 160

/* 
*  Define to enable external MAC handling, 
*  DONT change the Values 0x01, 0x02, 0x03 
*  SELECT ONLY One of the Option 
*/
/* #define GENERATE_STATIC_RANDOM_MAC  0x01   */
/* #define EXTERNAL_MAC_ADDR_MGMT      0x02 */
#define INTERNAL_UNIQUE_NUMBER_MAC    0x03

#if  (!(GENERATE_STATIC_RANDOM_MAC)) &&  (!(EXTERNAL_MAC_ADDR_MGMT)) && (!(INTERNAL_UNIQUE_NUMBER_MAC))
#error "Please select atleast one Option"
#endif 

#define STATIC_OOB_SIZE                 16U
extern const MOBLEUINT8 StaticOobBuff[STATIC_OOB_SIZE];

/* Define the following Macros to change the step resolution and step count value */
#define TRANSITION_SCALER 4
#define PWM_TIME_PERIOD  32000U

/* Define the Macro for enabling/disabling the Publishing with Generic on off
   or by Vendor Model.
@ define Macro for Vendor Publishing
@ Undefine or comment Macro for Generic On Off Publishing
*/
#define VENDOR_MODEL_PUBLISH

/* Macros for the Property id for the sensors */
#define TEMPERATURE_PID  0X004F
#define PRESSURE_PID     0X2A6D

/*******************************************************************************
*** Following section helps to select right configuration of Models  ***********
*******************************************************************************/
/******* Define the following Macros to enable the usage of the  Models  ******/

/* Define the following Macros to enable the usage of the Server Generic Models  */
#define ENABLE_GENERIC_MODEL_SERVER_ONOFF 
#define ENABLE_GENERIC_MODEL_SERVER_LEVEL
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

/* Define the following Macros to enable the usage of the Client Generic Models  */
#define ENABLE_GENERIC_MODEL_CLIENT_ONOFF 
#define ENABLE_GENERIC_MODEL_CLIENT_LEVEL

/* Define the following Macros to enable the usage of the Light Models  */

#define ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
#define ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_CTL
#define ENABLE_LIGHT_MODEL_SERVER_CTL_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_CTL_TEMPERATURE
#define ENABLE_LIGHT_MODEL_SERVER_HSL 
#define ENABLE_LIGHT_MODEL_SERVER_HSL_SETUP  
#define ENABLE_LIGHT_MODEL_SERVER_HSL_HUE 
#define ENABLE_LIGHT_MODEL_SERVER_HSL_SATURATION 
#define ENABLE_LIGHT_MODEL_SERVER_XYL ;
#define ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP 
#define ENABLE_LIGHT_MODEL_SERVER_LC 
#define ENABLE_LIGHT_MODEL_SERVER_LC_SETUP 

/* Define the following Macros to enable the usage of the Sensor Models  */
#define ENABLE_SENSOR_MODEL_SERVER
#define ENABLE_SENSOR_MODEL_SERVER_SETUP


/*******************************************************************************
****** Following section helps to select right configuration of node ***********
*******************************************************************************/

#define ENABLE_RELAY_FEATURE
#define ENABLE_PROXY_FEATURE
/* #define ENABLE_FRIEND_FEATURE */

#ifdef ENABLE_RELAY_FEATURE
#define RELAY_FEATURE 1
#else
#define RELAY_FEATURE 0
#endif

#ifdef ENABLE_PROXY_FEATURE
#define PROXY_FEATURE 1
#else
#define PROXY_FEATURE 0
#endif

#ifdef ENABLE_FRIEND_FEATURE
#define FRIEND_FEATURE 1
#else
#define FRIEND_FEATURE 0
#endif

#if (PROXY_FEATURE && FRIEND_FEATURE)
#error "BLE Proxy node can't be friend node"
#endif

#define MESH_FEATURES (RELAY_FEATURE << 0 | PROXY_FEATURE << 1 | FRIEND_FEATURE << 2)

#define TR_GAP_BETWEEN_TRANSMISSION      20U    /* varies from 10 to 65535 */
#define TRANSMIT_RECEIVE_PARAMS          \
{                                        \
    TR_GAP_BETWEEN_TRANSMISSION          \
}

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
* Friend node receive window size is fixed at 50 ms
*/

/* 
*  Number of Low power nodes that can be associated with Friend node
*  varies from 1 to 10
*/
#define FN_NO_OF_LPNS                    2U
#define FRIEND_NODE_PARAMS               \
{                                        \
    FN_NO_OF_LPNS                        \
}

/* 
*  For prioritizing friendship offer with good RSSI link
*  varies from 0 to 3
*  Ref @Mesh_v1.0
*/
#define LPN_RSSI_FACTOR_LEVEL            1U

/* 
*  For prioritizing friendship offer with good receive window factor
*  varies from 0 to 3
*  Ref @Mesh_v1.0
*/
#define LPN_RECIVE_WINDOW_FACTOR_LEVEL   1U

/* 
*  Minimum packets queue size required
*  varies from 1 to 7
*  Ref @Mesh_v1.0
*/
#define LPN_MINIMUM_QUEUE_SIZE_LOG       2U

/* 
*  (unit ms)
*  varies from 0x0A to 0xFF
*  Ref @Mesh_v1.0
*/
#define LPN_RECEIVE_DELAY                150U

/* 
*  Poll timeout value after which friendship cease to exist (unit 100ms)
*  varies from 0x00000A to 0x34BBFF
*  Ref @Mesh_v1.0
*/
#define LPN_POLL_TIMEOUT                 2000U

/* 
*  Maximum receive window size acceptable to low power node, recommended value is 255
*  varies from 10 to 255
*  Ref @Mesh_v1.0
*/
#define LPN_RECEIVE_WINDOW_SIZE          55U

/* 
*  Minimum friend's subscription list size capability required by lpn
*  varies from 1 to 5
*  Ref @Mesh_v1.0
*/
#define LPN_SUBSCR_LIST_SIZE             2U

/* 
*  Frequency at which low power node would send friend request (unit 100ms)
*  varies from 0 to 255
*/
#define LPN_FRIEND_REQUEST_FREQUENCY     50U

/* 
*  Frequency at which low power node would poll friend node (unit 100ms)
*  should be less than poll timeout
*/
#define LPN_FRIEND_POLL_FREQUENCY        25U

/* 
*  Minimum RSSI required by low power node
*  should be less than equal to -60
*/
#define LPN_MINIMUM_RSSI                 -100

/* 
*  Retries to be made by lpn before termination of friendship
*  varies from 2 to 10
*/
#define LPN_NO_OF_RETRIES                10U

#define UNPROV_NODE_INFO_PARAMS          \
{                                        \
    STATIC_OOB_SIZE,                     \
    StaticOobBuff                        \
}

#if FRIEND_FEATURE
#define FRIEND_BUFF_DYNAMIC_MEMORY_SIZE    FN_NO_OF_LPNS*816+4
#elif LOW_POWER_FEATURE
#define FRIEND_BUFF_DYNAMIC_MEMORY_SIZE    112U
#else
#define FRIEND_BUFF_DYNAMIC_MEMORY_SIZE    1U
#endif

#define DYNAMIC_MEMORY_SIZE                4096U

#define SEGMENTATION_COUNT             (MAX_APPLICATION_PACKET_SIZE / 12) + 2 
#define SAR_BUFFER_SIZE                ((uint8_t)SEGMENTATION_COUNT) * 40 

#define SdkEvalComIOUartIrqHandler         UART_Handler /* Added Interrupt handler for Uart */

/* Exported variables  ------------------------------------------------------- */

extern const DynBufferParam_t DynBufferParam;
extern const tr_params_t TrParams;
extern const lpn_params_t LpnParams;
extern const fn_params_t FnParams;
extern const unprov_node_info_params_t UnprovNodeInfoParams;

/* Exported Functions Prototypes ---------------------------------------------*/

#endif /* __MESH_CFG_H */
