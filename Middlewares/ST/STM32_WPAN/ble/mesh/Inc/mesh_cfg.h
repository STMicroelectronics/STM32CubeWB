/**
  ******************************************************************************
  * @file    mesh_cfg.h
  * @author  BLE Mesh Team
  * @brief   Header file for mesh_usr_cfg.c 
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MESH_CFG_H
#define __MESH_CFG_H

/* Includes ------------------------------------------------------------------*/
#include "mesh_cfg_usr.h"
#include "app_conf.h"

/* Exported macro ------------------------------------------------------------*/
/* Flash parameters */
#if defined(STM32WB55xx)
#define PAGE_SIZE                          4096
#endif
#if defined(STM32WB15xx)
#define PAGE_SIZE                          2048
#endif
#define RESET_MANAGER_FLASH_BASE_ADDRESS   (0x10040000)


#define DEVICE_KEY_SIZE                         16U
#define APP_KEY_SIZE                            16U
#define UUID_SIZE                               16U

#define NO_MODEL_AVLBL                       0xFFFF

#define MAX_U32_VALUE                           0xFFFFFFFF        

/*
* TRACE_M includes function name and clock
*/
void TraceHeader(const char* func_name, int mode);
#define TraceB0(func_name, ...)
#if ( CFG_DEBUG_TRACE != 0 )
#define TraceB1(func_name, ...) do {TraceHeader(func_name, 0); printf( __VA_ARGS__);} while(0)
#else
#define TraceB1(func_name, ...)
#endif
#define TraceBX(flags, func_name, ...) TraceB ##flags(func_name, ##__VA_ARGS__)
#define TRACE_M(flag, ...)   TraceBX(flag, __func__, ##__VA_ARGS__)

/*
* TRACE_I does not include function name and clock
*/
#define TraceC0(...)
#if ( CFG_DEBUG_TRACE != 0 )
#define TraceC1(...) do { printf( __VA_ARGS__);} while(0)
#else
#define TraceC1(...)
#endif
//#define TraceCX(flags, ...) TraceC ##flags( ##__VA_ARGS__)
#define TraceCX(flags, ...) TraceC ##flags( __VA_ARGS__)
#define TRACE_I(flag, ...)   TraceCX(flag, ##__VA_ARGS__)

void MemoryDumpHex(const MOBLEUINT8* memory_addr, int size);


/* No OOB Type Authentication Configuration */
#ifdef ENABLE_AUTH_TYPE_NO_OOB
#define AUTH_TYPE_NO_OOB_ENABLE_STATE   MOBLE_TRUE
#else
#define AUTH_TYPE_NO_OOB_ENABLE_STATE   MOBLE_FALSE
#endif

/* Static OOB Configurations */
#ifdef ENABLE_AUTH_TYPE_STATIC_OOB
#define SIZE_STATIC_OOB                    STATIC_OOB_SIZE 
#else
#define SIZE_STATIC_OOB                    0U 
#endif

/* Output OOB Configurations */
#ifdef ENABLE_AUTH_TYPE_OUTPUT_OOB
#define SIZE_OUTPUT_OOB                    OUTPUT_OOB_SIZE 
#define OUTPUT_OOB_ACTION                  OUTPUT_OOB_ACTION_BIT_BLINK 
#else
#define SIZE_OUTPUT_OOB                    0U 
#define OUTPUT_OOB_ACTION                  OUTPUT_OOB_ACTION_BIT_BLINK 
#endif

/* Input OOB Configurations */
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB
#define SIZE_INPUT_OOB                    INPUT_OOB_SIZE 
#define INPUT_OOB_ACTION                  INPUT_OOB_ACTION_BIT_PUSH 
#else
#define SIZE_INPUT_OOB                    0U 
#define INPUT_OOB_ACTION                  INPUT_OOB_ACTION_BIT_PUSH 
#endif

#ifdef ENABLE_SENSOR_MODEL_SERVER
#define ENABLE_SENSOR_MODEL_SERVER_SETUP              ENABLE_SENSOR_MODEL_SERVER
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
  #ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP
    #if (ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP != ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF)
      #error "Generic Power OnOff Setup Server should be supported with Generic Power OnOff Server"
    #endif
  #else
    #error "Generic Power OnOff Setup Server should be supported with Generic Power OnOff Server"
  #endif

  #ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF
    #if (ENABLE_GENERIC_MODEL_SERVER_ONOFF == 0)
      #error "Generic OnOff Server should be supported with Generic Power OnOff Server"
    #endif
  #else
    #error "Generic OnOff Server should be supported with Generic Power OnOff Server"
  #endif
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
  #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP
    #if (ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS_SETUP != ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS)
      #error "Light Lightness Setup Server should be supported with Light Lightness Server"
    #endif
  #else
    #error "Light Lightness Setup Server should be supported with Light Lightness Server"
  #endif

  #ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF
    #if (ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF == 0)
      #error "Generic Power OnOff Server should be supported with Light Lightness Server"
    #endif
  #else
    #error "Generic Power OnOff Server should be supported with Light Lightness Server"
  #endif

  #ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL
    #if (ENABLE_GENERIC_MODEL_SERVER_LEVEL == 0)
      #error "Generic Level Server should be supported with Light Lightness Server"
    #endif
  #else
    #error "Generic Level Server should be supported with Light Lightness Server"
  #endif
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
  #define ENABLE_LIGHT_MODEL_SERVER_LC_SETUP          ENABLE_LIGHT_MODEL_SERVER_LC

  #if (APPLICATION_NUMBER_OF_ELEMENTS < 2)
    #error "Number of elements should be >= 2 with Light LC Server"
  #endif

  #ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS
    #if (ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS == 0)
      #error "Light Lightness Server should be supported with Light LC Server (on main element)"
    #endif
  #else
    #error "Light Lightness Server should be supported with Light LC Server (on main element)"
#endif
#endif

#ifdef EXTERNAL_MAC_ADDR_MGMT
  #define EXTERNAL_MAC_TYPE (uint8_t)(EXTERNAL_MAC_IS_PUBLIC_ADDR<<7)
#else
  #define EXTERNAL_MAC_TYPE (uint8_t)0
#endif

#if  (!(GENERATE_STATIC_RANDOM_MAC)) &&  (!(EXTERNAL_MAC_ADDR_MGMT)) && (!(INTERNAL_UNIQUE_NUMBER_MAC))
#error "Please select at least one MAC address Option"
#endif 

#ifdef ENABLE_UNPROVISIONING_BY_POWER_ONOFF_CYCLE
#define ENABLE_SAVE_MODEL_STATE_NVM
#endif

#ifdef ENABLE_SAVE_MODEL_STATE_NVM
#define SAVE_MODEL_STATE_NVM 1
#else
#define SAVE_MODEL_STATE_NVM 0
#endif

#define AD_TYPE_NAME                     0x09

#define DEVICE_NAME_PARAMS               \
{                                        \
    DEVICE_NAME_SIZE,                    \
    Device_Name                          \
}     

#define TRANSMIT_RECEIVE_PARAMS          \
{                                        \
    TR_GAP_BETWEEN_TRANSMISSION          \
}

#ifdef ENABLE_PB_ADV
#define PB_ADV_SUPPORTED 1
#else
#define PB_ADV_SUPPORTED 0
#endif

#ifdef ENABLE_PB_GATT
#define PB_GATT_SUPPORTED 1
#else
#define PB_GATT_SUPPORTED 0
#endif

#ifdef ENABLE_PUB_KEY_TYPE_OOB
#define PUB_KEY_TYPE_OOB        MOBLE_TRUE
#else
#define PUB_KEY_TYPE_OOB        MOBLE_FALSE
#endif


#if  (PB_ADV_SUPPORTED == 0 && PB_GATT_SUPPORTED == 0)
#error "At least one of PB-ADV and PB-GATT should be defined"
#endif

#define BLUENRG_MESH_PRVN_BEARER_INFO (PB_ADV_SUPPORTED << 0 | PB_GATT_SUPPORTED << 1)

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

#ifdef ENABLE_LOW_POWER_FEATURE
#define LOW_POWER_FEATURE 1
#else
#define LOW_POWER_FEATURE 0
#endif

#ifdef ENABLE_PROVISIONER_FEATURE
#define PROVISIONER_FEATURE 1
#else
#define PROVISIONER_FEATURE 0
#endif

#if (LOW_POWER_FEATURE && RELAY_FEATURE)
#error "Low power node can't be relay node"
#elif (LOW_POWER_FEATURE && PROXY_FEATURE)
#error "Low power node can't be proxy node"
#elif (LOW_POWER_FEATURE && FRIEND_FEATURE)
#error "Low power node can't be friend node"
#endif

#define BLUENRG_MESH_FEATURES (RELAY_FEATURE << 0 | PROXY_FEATURE << 1 | FRIEND_FEATURE << 2 | LOW_POWER_FEATURE << 3)

#define FRIEND_NODE_PARAMS               \
{                                        \
    FN_NO_OF_LPNS                        \
}

#define LOW_POWER_NODE_PARAMS            \
{                                        \
  LPN_RSSI_FACTOR_LEVEL,                 \
  LPN_RECIVE_WINDOW_FACTOR_LEVEL,        \
  LPN_MINIMUM_QUEUE_SIZE_LOG,            \
  LPN_RECEIVE_DELAY,                     \
  LPN_POLL_TIMEOUT,                      \
  LPN_FRIEND_REQUEST_FREQUENCY,          \
  LPN_FRIEND_POLL_FREQUENCY,             \
  LPN_RECEIVE_WINDOW_SIZE,               \
  LPN_SUBSCR_LIST_SIZE,                  \
  LPN_MINIMUM_RSSI,                      \
  LPN_NO_OF_RETRIES                      \
}

#define UNPROV_NODE_INFO_PARAMS          \
{                                        \
    PUB_KEY_TYPE_OOB,                    \
    PubKeyBuff,                          \
    PrivKeyBuff,                         \
    AUTH_TYPE_NO_OOB_ENABLE_STATE,       \
    SIZE_STATIC_OOB,                     \
    StaticOobBuff,                       \
    SIZE_OUTPUT_OOB,                     \
    OUTPUT_OOB_ACTION,                   \
    Appli_BleOutputOOBAuthCb,            \
    SIZE_INPUT_OOB,                      \
    INPUT_OOB_ACTION,                    \
    Appli_BleInputOOBAuthCb              \
}

#ifdef ENABLE_NEIGHBOR_TABLE
#define NEIGHBOR_TABLE_SUPPORTED 1
#else
#define NEIGHBOR_TABLE_SUPPORTED 0
#endif

#if  (NEIGHBOR_TABLE_SUPPORTED == 1)
#if (NEIGHBOR_COUNT == 0)
#error "Number of neighbors should be nonzero"
#endif /* NO_OF_NEIGHBORS */
#if (NEIGHBOR_ALIVE_TIME == 0)
#error "Number of neighbor alive time should be nonzero"
#endif /* NEIGHBOR_ALIVE_TIME */
#if ((NEIGHBOR_UNPRVND_DEV_BEACON_NTU == 0) && (NEIGHBOR_MSG_TTLX_NTU == 0) && (NEIGHBOR_SECURE_NET_BEACON_NTU == 0))
#error "At least one of Network Table Update trigger should be enabled"
#endif
#if (NEIGHBOR_UNPRVND_DEV_BEACON_NTU > 1)
#error "Invalid valid of NEIGHBOR_UNPRVND_DEV_BEACON_NTU. Allowed value: 0 and 1"
#endif /* NEIGHBOR_UNPRVND_DEV_BEACON_NTU */
#if (NEIGHBOR_SECURE_NET_BEACON_NTU > 1)
#error "Invalid valid of NEIGHBOR_SECURE_NET_BEACON_NTU. Allowed value: 0 and 1"
#endif /* NEIGHBOR_SECURE_NET_BEACON_NTU */
#if (NEIGHBOR_MSG_TTLX_NTU > 2)
#error "Invalid valid of NEIGHBOR_MSG_TTLX_NTU. Allowed value: 0, 1 and 2"
#endif /* NEIGHBOR_MSG_TTLX_NTU */
#endif /* NEIGHBOR_TABLE_SUPPORTED */

#ifdef ENABLE_NEIGHBOR_TABLE
#define NEIGHBOR_TABLE_PARAMS            \
{                                        \
    NEIGHBOR_COUNT,                      \
    NEIGHBOR_ALIVE_TIME,                 \
    NEIGHBOR_UNPRVND_DEV_BEACON_NTU,     \
    NEIGHBOR_SECURE_NET_BEACON_NTU,      \
    NEIGHBOR_MSG_TTLX_NTU                \
}
#else
#define NEIGHBOR_TABLE_PARAMS            {0, 0, 0, 0, 0}
#endif

#if FRIEND_FEATURE
#define FRIEND_BUFF_DYNAMIC_MEMORY_SIZE    FN_NO_OF_LPNS*820+4
#elif LOW_POWER_FEATURE
#define FRIEND_BUFF_DYNAMIC_MEMORY_SIZE    112U
#else
#define FRIEND_BUFF_DYNAMIC_MEMORY_SIZE    4U
#endif

#define DYNAMIC_MEMORY_SIZE                4096U

#define SEGMENTATION_COUNT             (MAX_APPLICATION_PACKET_SIZE / 12) + 2 
#define SAR_BUFFER_SIZE                ((uint8_t)SEGMENTATION_COUNT) * 40 
/* Added Interrupt handler for Uart */
#define SdkEvalComIOUartIrqHandler         UART_Handler  

#if NEIGHBOR_TABLE_SUPPORTED
#define NEIGHBOR_TABLE_DYNAMIC_MEMORY_SIZE 32U*NEIGHBOR_COUNT
#else
#define NEIGHBOR_TABLE_DYNAMIC_MEMORY_SIZE 0
#endif

#define SdkEvalComIOUartIrqHandler         UART_Handler /* Added Interrupt handler for Uart */


/* Following Macro helps to know if the Fixed functions are needed or not 
   DO NOT change or add any space at the end of the file */
#if defined (ENABLE_GENERIC_MODEL_CLIENT_ONOFF) \
    || defined (ENABLE_GENERIC_MODEL_CLIENT_LEVEL) 

 #define ENABLE_GENERIC_MODEL_CLIENT
#endif

#if defined (ENABLE_LIGHT_MODEL_CLIENT_LIGHTNESS) \
    || defined (ENABLE_LIGHT_MODEL_CLIENT_CTL)  \
    || defined (ENABLE_LIGHT_MODEL_CLIENT_HSL)  \
    || defined (ENABLE_LIGHT_MODEL_CLIENT_LC) 
         
 #define ENABLE_LIGHT_MODEL_CLIENT
#endif

#if defined (ENABLE_GENERIC_MODEL_SERVER_ONOFF) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_LEVEL) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_BATTERY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_LOCATION) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY) \
    && !defined (ENABLE_GENERIC_MODEL_SERVER)
 #define ENABLE_GENERIC_MODEL_SERVER                                         (1)
#endif

#if defined (ENABLE_GENERIC_MODEL_SERVER_ONOFF) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_LEVEL) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF_SETUP) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_DEFAULT_TRANSITION_TIME) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_POWER_LEVEL_SETUP) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_BATTERY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_LOCATION) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_LOCATION_SETUP) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_ADMIN_PROPERTY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_MANUFACTURER_PROPERTY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_USER_PROPERTY) \
    || defined (ENABLE_GENERIC_MODEL_SERVER_CLIENT_PROPERTY) \
    && !defined (GENERIC_SERVER_MODEL_ADD_CONFIGURATION)
 #define GENERIC_SERVER_MODEL_ADD_CONFIGURATION                              (1)

#endif

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
    || defined(ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP) \
    && !defined (ENABLE_LIGHT_MODEL_SERVER)
  #define  ENABLE_LIGHT_MODEL_SERVER                                         (1)
#endif  

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
    || defined(ENABLE_LIGHT_MODEL_SERVER_XYL_SETUP) \
    && !defined (LIGHT_SERVER_MODEL_ADD_CONFIGURATION)
  #define LIGHT_SERVER_MODEL_ADD_CONFIGURATION    
#endif  

#if defined(ENABLE_TIME_MODEL_SERVER) \
    || defined(ENABLE_TIME_MODEL_SERVER_SETUP)\
    || defined(ENABLE_SCENE_MODEL_SERVER)\
    || defined(ENABLE_SCENE_MODEL_SERVER_SETUP)
    && !defined (ENABLE_TIME_SCENE_MODEL_SERVER)
   #define ENABLE_TIME_SCENE_MODEL_SERVER                                    (1)
#endif

#if (ENABLE_SIG_MODELS_AT_COMMANDS == 1) \
    || (ENABLE_VENDOR_MODELS_AT_COMMANDS == 1)
  #define ENABLE_SERIAL_CONTROL 1
#endif         

#if (ENABLE_VENDOR_MODELS_AT_COMMANDS == 1)
  #define ENABLE_APPLI_TEST     1 /* Enable for Application Testing */
#endif      

#if (ENABLE_SIG_MODELS_AT_COMMANDS == 1) \
    || (ENABLE_VENDOR_MODELS_AT_COMMANDS == 1) \
    || (ENABLE_UT == 1)
  #define TF_SERIAL_PRINTS  1
#endif

/* Following Macros helps to align the Number Of Models set by User with the
   Application Settings. DO NOT change the value */
#define APPLICATION_SIG_MODELS_MAX_COUNT             USER_SIG_MODELS_MAX_COUNT+1
#define APPLICATION_VENDOR_MODELS_MAX_COUNT       USER_VENDOR_MODELS_MAX_COUNT+1      


/******************************************************************************/
/* 
Define the Macros for Enabling/disabling the binding of data between the Generic 
and Light model.
@ define the Macro for enabling the binding
@ Undefine or comment the macro for disabling the binding.
*/
/******************************************************************************/
#define ENABLE_MODEL_BINDING

/**
* Size of sensor server structure
*/
#define SIZE_OF_SENSOR_SERVER_T                                               12
/**
* Size of sensor parameters structure
*/
#define SIZE_OF_SENSOR_PARAMS_T                                               60
/**
* Size of sensor settings structure
*/
#define SIZE_OF_SENSOR_SETTINGS_T                                              8
/**
* Size of sensor series structure
*/
#define SIZE_OF_SENSOR_SERIES_COLUMN_T                                         4
/**
* Size of sensor buffer required for sensor server model
*/
#define TOTAL_SENSOR_SERVER_BUFF_SIZE          (SIZE_OF_SENSOR_SERVER_T + \
                                                SIZE_OF_SENSOR_PARAMS_T*TOTAL_SENSORS_COUNT + \
                                                SIZE_OF_SENSOR_SETTINGS_T*TOTAL_SENSOR_SETTINGS_COUNT + \
                                                SIZE_OF_SENSOR_SERIES_COLUMN_T*TOTAL_SENSOR_SERIES_COLUMN_COUNT)

/**
* Size of Light LC server structure
*/
#define SIZE_OF_LC_SERVER_T                                                    156

#if defined STEVAL_BLUENRG_1_BOARD_PWM_SELECTION || defined STEVAL_BLUENRG_2_BOARD_PWM_SELECTION
  #define SINGLE_LED   PWM4
  #define COOL_LED     PWM0
  #define WARM_LED     PWM1
  #define RED_LED      PWM2
  #define GREEN_LED    PWM3
  #define BLUE_LED     PWM4
#endif

#ifdef CUSTOM_BOARD_PWM_SELECTION 
  #define SINGLE_LED   PWM4    
  #define COOL_LED     PWM0
  #define WARM_LED     PWM1
  #define RED_LED      PWM2
  #define GREEN_LED    PWM3
  #define BLUE_LED     PWM4
#endif

#ifdef STEVAL_BLUEMIC_1_BOARD_PWM_SELECTION
  #define SINGLE_LED   PWM3
//  #define COOL_LED     PWM0
//  #define WARM_LED     PWM1
  #define RED_LED      PWM1
  #define GREEN_LED    PWM3
  #define BLUE_LED     PWM4
#endif

#ifdef BLUENRG_TILE
  #define SINGLE_LED   PWM3
//  #define COOL_LED     PWM0
//  #define WARM_LED     PWM1
  #define RED_LED      PWM1
  #define GREEN_LED    PWM3
  #define BLUE_LED     PWM4
#endif

/******************** Serial Interface Handling Control **********************/

/* Exported variables  ------------------------------------------------------- */
extern const device_name_params_t DeviceNameParams;
extern const DynBufferParam_t DynBufferParam;
extern const tr_params_t TrParams;
extern const lpn_params_t LpnParams;
extern const fn_params_t FnParams;
extern const prvn_params_t PrvnParams;
extern const neighbor_table_init_params_t NeighborTableParams;
extern MOBLEUINT8 BufferSensorServer[];
extern MOBLEUINT8 BufferLcServer[];

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLEUINT8 ApplicationSetNodeSigModelList(void);
MOBLE_RESULT ApplicationInitSigModelList(void);
MOBLE_RESULT ApplicationInitVendorModelList(void);
MOBLE_RESULT AppliCheck_EnabledModelsList(void);
MOBLE_RESULT Appli_Light_LCs_Init(void);
#endif /* __MESH_CFG_H */
