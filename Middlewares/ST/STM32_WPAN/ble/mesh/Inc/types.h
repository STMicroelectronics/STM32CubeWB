/**
******************************************************************************
* @file    types.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Header file for various type declarations 
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
#ifndef _TYPES_H
#define _TYPES_H

/**
* \file types.h
* \brief This file defines Motorola Solutions Bluetooth LE Mesh Library types.
*
* This file contains data types defined in the ST BLE-Mesh Library . Please refer to the
* desription of each type to get the information on it's purpose.
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

#endif /* _TYPES_H */
/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

