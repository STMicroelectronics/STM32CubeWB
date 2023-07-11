/**
******************************************************************************
* @file    types.h
* @author  BLE Mesh Team
* @brief   Header file for various type declarations 
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
#ifndef _TYPES_H
#define _TYPES_H

/**
* \file types.h
* \brief This file defines Motorola Solutions Bluetooth LE Mesh Library types.
*
* This file contains data types defined in the ST BLE-Mesh Library . Please refer to the
* description of each type to get the information on it's purpose.
*/

#ifndef NULL
#define NULL 0
#endif

/** \brief A one octet signed data type
*
* A one octet signed data type
*/
typedef signed char     MOBLEINT8;

/** \brief A two octet signed data type
*
* A two octet signed data type
*/
typedef signed short    MOBLEINT16;

/** \brief A four octet signed data type
*
* A four octet signed data type
*/
typedef signed long     MOBLEINT32;

/** \brief A one octet unsigned data type
*
* A one octet unsigned data type
*/
typedef unsigned char   MOBLEUINT8;

/** \brief A two octet unsigned data type
*
* A two octet unsigned data type
*/
typedef unsigned short  MOBLEUINT16;

/** \brief A four octet unsigned data type
*
* A four octet unsigned data type
*/
typedef unsigned long   MOBLEUINT32;

/**
* \brief Enumeration representing boolean data type.
*
* Enumeration representing boolean data type
*/
typedef enum
{
  MOBLE_FALSE = 0, /**< False value */
  MOBLE_TRUE       /**< True value */
} MOBLEBOOL;

typedef MOBLEUINT16 MOBLE_ADDRESS;
#define MOBLE_ADDRESS_UNASSIGNED 0x0000
#define MOBLE_ADDRESS_ALL_NODES  0xFFFF

/**
* \brief Enumeration representing operation results.
*
* Enumeration representing operation results.
*/
typedef enum
{
  MOBLE_RESULT_SUCCESS = 0,       /**< Operation completed successfully */
  MOBLE_RESULT_FALSE,             /**< Operation was skipped or no action required */
  MOBLE_RESULT_FAIL,              /**< Operation failed */
  MOBLE_RESULT_INVALIDARG,        /**< Operation failed due to invalid argument */
  MOBLE_RESULT_OUTOFMEMORY,       /**< Operation failed due to resources limit */
  MOBLE_RESULT_NOTIMPL            /**< Operation failed due implementation is missed */
} MOBLE_RESULT;


/**
* \brief Macro to define if operation was completed successfully.
*
* Macro to define if operation was completed successfully.
* Successful results are MOBLE_RESULT_SUCCESS and MOBLE_RESULT_FALSE
*/
#define MOBLE_SUCCEEDED(a)  ((a) <= MOBLE_RESULT_FALSE)

/**
* \brief Macro to define if operation failed.
*
* Macro to define if operation failed.
* Unsuccessful results are MOBLE_RESULT_FAIL, MOBLE_RESULT_INVALIDARG and MOBLE_RESULT_OUTOFMEMORY
*/
#define MOBLE_FAILED(a)     ((a) >  MOBLE_RESULT_FALSE)

/**
* \brief Callback for transport heartbeat.
* \param src Source address of received Heartbeat message.
* \param dst Destination address of received Heartbeat message.
* \param initTTL Initial TTL value used when sending Heartbeat message.
* \param receivedTTL TTL of the Heartbeat message on its arrival.
* \param features corresponding features for Heartbeat message trigger from source.
*/
typedef MOBLE_RESULT (*MOBLE_HEARTBEAT_CB)(MOBLE_ADDRESS src, MOBLE_ADDRESS dst, MOBLEUINT8 initTTL, MOBLEUINT8 receivedTTL, MOBLEUINT16 features);
/**
* \brief Callback for Attention Timer Callback.
* \param void
*/
typedef MOBLE_RESULT (*MOBLE_ATTENTION_TIMER_CB)(void);

//typedef __packed struct
typedef struct __attribute__((packed))
{
  MOBLEUINT8 length;
  MOBLEUINT8 type;
  MOBLEUINT8 data[];
} LE_ADV;

/**
  * @brief  WEAK_FUNCTION
  *         Use the WEAK_FUNCTION macro to declare a weak function.
  *         Usage:  WEAK_FUNCTION(int my_weak_function(void))
  */
#define WEAK_FUNCTION(function)        __weak function


#endif /* _TYPES_H */


