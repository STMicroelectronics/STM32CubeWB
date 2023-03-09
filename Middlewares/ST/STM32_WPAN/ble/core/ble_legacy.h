/*****************************************************************************
 * @file    ble_legacy.h
 * @author  MDG
 * @brief   This file contains legacy definitions used for BLE.
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#ifndef BLE_LEGACY_H__
#define BLE_LEGACY_H__

/* All previous Legacy Defines have been removed or moved in other MW header files */
/* ----------------------------------------------------------------------- */

/* Various obsolete definitions
 */

#define PERIPHERAL_PRIVACY_FLAG_UUID             0x2A02U
#define RECONNECTION_ADDR_UUID                   0x2A03U

/* Deprecated names for HCI commands and events
 */

#define hci_le_read_remote_used_features \
        hci_le_read_remote_features
#define hci_le_read_remote_used_features_complete_event_rp0 \
        hci_le_read_remote_features_complete_event_rp0
#define hci_le_read_advertising_channel_tx_power \
        hci_le_read_advertising_physical_channel_tx_power
#define hci_le_start_encryption \
        hci_le_enable_encryption
#define hci_le_long_term_key_requested_negative_reply \
        hci_le_long_term_key_request_negative_reply
#define hci_le_set_advertise_enable \
        hci_le_set_advertising_enable
#define hci_le_enhanced_receiver_test \
        hci_le_receiver_test_v2
#define hci_le_enhanced_transmitter_test \
        hci_le_transmitter_test_v2
#define hci_le_read_white_list_size \
        hci_le_read_filter_accept_list_size
#define hci_le_clear_white_list \
        hci_le_clear_filter_accept_list
#define hci_le_add_device_to_white_list \
        hci_le_add_device_to_filter_accept_list
#define hci_le_remove_device_from_white_list \
        hci_le_remove_device_from_filter_accept_list
#define hci_le_direct_advertising_report_event \
        hci_le_directed_advertising_report_event
#define hci_le_direct_advertising_report_event_rp0 \
        hci_le_directed_advertising_report_event_rp0


#endif /* BLE_LEGACY_H__ */
