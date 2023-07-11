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


/* Various obsolete definitions
 */

#define PERIPHERAL_PRIVACY_FLAG_UUID             0x2A02U
#define RECONNECTION_ADDR_UUID                   0x2A03U

/* Deprecated names for ACI/HCI commands and events
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
#define aci_gap_configure_whitelist \
        aci_gap_configure_filter_accept_list
#define aci_gap_slave_security_req \
        aci_gap_peripheral_security_req
#define aci_hal_set_slave_latency \
        aci_hal_set_peripheral_latency
#define aci_gap_slave_security_initiated_event \
        aci_gap_peripheral_security_initiated_event

#define Whitelist_Entry_t \
        Peer_Entry_t
#define Whitelist_Identity_Entry_t \
        Identity_Entry_t

#define ACI_GAP_SLAVE_SECURITY_INITIATED_VSEVT_CODE \
        ACI_GAP_PERIPHERAL_SECURITY_INITIATED_VSEVT_CODE

/* Other deprecated names
 */

#define HCI_ADV_FILTER_WHITELIST_SCAN \
        HCI_ADV_FILTER_ACC_LIST_USED_FOR_SCAN
#define HCI_ADV_FILTER_WHITELIST_CONNECT \
        HCI_ADV_FILTER_ACC_LIST_USED_FOR_CONNECT
#define HCI_ADV_FILTER_WHITELIST_SCAN_CONNECT \
        HCI_ADV_FILTER_ACC_LIST_USED_FOR_ALL
#define NO_WHITE_LIST_USE \
        HCI_ADV_FILTER_NO
#define WHITE_LIST_FOR_ONLY_SCAN \
        HCI_ADV_FILTER_ACC_LIST_USED_FOR_SCAN
#define WHITE_LIST_FOR_ONLY_CONN \
        HCI_ADV_FILTER_ACC_LIST_USED_FOR_CONNECT
#define WHITE_LIST_FOR_ALL \
        HCI_ADV_FILTER_ACC_LIST_USED_FOR_ALL

#define HCI_SCAN_FILTER_WHITELIST \
        HCI_SCAN_FILTER_ACC_LIST_USED
#define HCI_SCAN_FILTER_NO_EVEN_RPA  \
        HCI_SCAN_FILTER_NO_EXT
#define HCI_SCAN_FILTER_WHITELIST_BUT_RPA \
        HCI_SCAN_FILTER_ACC_LIST_USED_EXT

#define HCI_INIT_FILTER_WHITELIST \
        HCI_INIT_FILTER_ACC_LIST_USED

#define AD_TYPE_SLAVE_CONN_INTERVAL \
        AD_TYPE_PERIPHERAL_CONN_INTERVAL


#endif /* BLE_LEGACY_H__ */
