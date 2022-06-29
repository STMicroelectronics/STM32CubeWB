/**
 ******************************************************************************
 * @file    802_15_4_mac_errors.h
 * @author  MCD Application Team
 * @brief   Contains STM32WB specificities requested to control the 802.15.4
 *          MAC interface.
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
#include <stdint.h>
#ifndef STM32WB_M4_FIRMWARE_FIRMWARE_MIDDLEWARES_ST_STM32_WPAN_MAC_802_15_4_CORE_INC_802_15_4_MAC_ERRORS_H_
#define STM32WB_M4_FIRMWARE_FIRMWARE_MIDDLEWARES_ST_STM32_WPAN_MAC_802_15_4_CORE_INC_802_15_4_MAC_ERRORS_H_
typedef enum MAC_Enum_Tag {
    /*! -Indicates mac operation is completed successfully */
    g_MAC_SUCCESS_c = (uint8_t) 0x00,

    /*! -Indicates beacon was lost following a synchronization request */
    g_MAC_BEACON_LOSS_c = (uint8_t) 0xe0,

    /* -Indicates  CSMA-CA mechanism has failed */
    g_MAC_CHANNEL_ACCESS_FAILURE_c = (uint8_t) 0xe1,

    /* -Indicates  frame counter purportedly applied by the originator of the
     received frame is invalid */
    g_MAC_COUNTER_ERROR_c = (uint8_t) 0xdb,

    /* -Indicates GTS request has been denied by the PAN coordinator */
    g_MAC_DENIED_c = (uint8_t) 0xe2,

    /* -The attempt to disable the transceiver has failed */
    g_MAC_DISABLE_TRX_FAILURE_c = (uint8_t) 0xe3,

    /* -Indicates that frame resulting from processing has a length that is
     greater than aMaxPHYPacketSize */
    g_MAC_FRAME_TOO_LONG_c = (uint8_t) 0xe5,

    /* -Indicates that key purportedly applied by the originator of the received
     frame is not allowed to be used */
    g_MAC_IMPROPER_KEY_TYPE_c = (uint8_t) 0xdc,

    /* -Indicates that the security level purportedly applied by the originator
     of the received frame does not meet the minimum security level  */
    g_MAC_IMPROPER_SECURITY_LEVEL_c = (uint8_t) 0xdd,

    /* -Indicates that request to send data was unsuccessful,neither the source
     address parameters nor the destination address parameters were resent. */
    g_MAC_INVALID_ADDRESS_c = (uint8_t) 0xf5,

    /* -Indicates that requested GTS transmission failed because the specified
     GTS was not defined */
    g_MAC_INVALID_GTS_c = (uint8_t) 0xe6,

    /* -Indicates that purge an MSDU from the transaction queue was
     made using an MSDU handle that was not found in the transaction table */
    g_MAC_INVALID_HANDLE_c = (uint8_t) 0xe7,

    /* -Indicates that table index is out of range */
    g_MAC_INVALID_INDEX_c = (uint8_t) 0xf9,

    /* -Indicates that parameter in the primitive is either not supported or is
     out of the valid range */
    g_MAC_INVALID_PARAMETER_c = (uint8_t) 0xe8,

    /* -Indicates that PAN descriptors stored reached an implementation-specified
     maximum */
    g_MAC_LIMIT_REACHED_c = (uint8_t) 0xfa,

    /* -No acknowledgment was received after macMaxFrameRetries */
    g_MAC_NO_ACK_c = (uint8_t) 0xe9,

    /* -A scan operation failed to find any network beacons */
    g_MAC_NO_BEACON_c = (uint8_t) 0xea,

    /* -No response data were available following a requeste */
    g_MAC_NO_DATA_c = (uint8_t) 0xeb,

    /* -IThe operation failed because a 16bit short address was not allocated. */
    g_MAC_NO_SHORT_ADDRESS_c = (uint8_t) 0xec,

    /* -specified a number of symbols that are longer than the beacon interval */
    g_MAC_ON_TIME_TOO_LONG_c = (uint8_t) 0xf6,

    /* -Indicates that receiver request is not able to completed within the CAP */
    g_MAC_OUT_OF_CAP_c = (uint8_t) 0xed,

    /* -A PAN identifier conflict has been detected and communicated to the PAN
     coordinator */
    g_MAC_PAN_ID_CONFLICT_c = (uint8_t) 0xee,

    /* -Indicates that mac parameter is read only */
    g_MAC_READ_ONLY_c = (uint8_t) 0xfb,

    /* -Indicates that coordinator realignment command has been received */
    g_MAC_REALIGNMENT_c = (uint8_t) 0xef,

    /* -Indicates that scan is in progress */
    g_MAC_SCAN_IN_PROGRESS_c = (uint8_t) 0xfc,

    /* -Cryptographic processing of the received secured frame failed */
    g_MAC_SECURITY_ERROR_c = (uint8_t) 0xe4,

    /* -Indicates that device is not currently tracking the beacon of its
     coordinator */
    g_MAC_TRACKING_OFF_c = (uint8_t) 0xf8,

    /* -The transaction has expired and its information was discarded */
    g_MAC_TRANSACTION_EXPIRED_c = (uint8_t) 0xf0,

    /* -There is no capacity to store the transaction. */
    g_MAC_TRANSACTION_OVERFLOW_c = (uint8_t) 0xf1,

    /* -Indicates that  transceiver was in the transmitter enabled state when the
     receiver was requested to be enabled */
    g_MAC_TX_ACTIVE_c = (uint8_t) 0xf2,

    /* -Indicates thate key purportedly used by the originator of the received
     frame is not available*/
    g_MAC_UNAVAILABLE_KEY_c = (uint8_t) 0xf3,

    /* -Indicates that requested mac attribute is not supported */
    g_MAC_UNSUPPORTED_ATTRIBUTE_c = (uint8_t) 0xf4,

    /* - Security purportedly applied by the originator of the received frame is
     not supported. */
    g_MAC_UNSUPPORTED_SECURITY_c = (uint8_t) 0xdf,

    /* - Indicate that the request can not be processed in current MAC state.
     This is the lone proprietery status value in this enum. So always
     keep it at last */
    g_MAC_INVALID_REQUEST_c = (uint8_t) 0xc0
} MAC_Enum_t;


#endif /* STM32WB_M4_FIRMWARE_FIRMWARE_MIDDLEWARES_ST_STM32_WPAN_MAC_802_15_4_CORE_INC_802_15_4_MAC_ERRORS_H_ */
