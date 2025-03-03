/*****************************************************************************
 * @file    ble_legacy.h
 *
 * @brief   This file contains legacy definitions used for BLE.
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2025 STMicroelectronics.
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

#define OOB_AUTH_DATA_ABSENT                       0x00U
#define OOB_AUTH_DATA_PRESENT                      0x01U

#define BLE_STATUS_SEC_DB_FULL                     0x5DU
#define BLE_STATUS_INSUFFICIENT_ENC_KEYSIZE        0x5FU
#define BLE_STATUS_CHARAC_ALREADY_EXISTS           0x63U

#define GAP_NAME_DISCOVERY_PROC                    0x04U

/* Deprecated names for ACI/HCI commands and events
 */

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

#define ACI_HAL_FW_ERROR_VSEVT_CODE \
        ACI_HAL_WARNING_VSEVT_CODE

typedef __PACKED_STRUCT
{
  uint8_t FW_Error_Type;
  uint8_t Data_Length;
  uint8_t Data[(BLE_EVT_MAX_PARAM_LEN - 2) - 2];
} aci_hal_fw_error_event_rp0;

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

#define OOB_NOT_AVAILABLE                 REASON_OOB_NOT_AVAILABLE
#define AUTH_REQ_CANNOT_BE_MET            REASON_AUTHENTICATION_REQ
#define CONFIRM_VALUE_FAILED              REASON_CONFIRM_VALUE_FAILED
#define PAIRING_NOT_SUPPORTED             REASON_PAIRING_NOT_SUPPORTED
#define INSUFF_ENCRYPTION_KEY_SIZE        REASON_ENCRYPTION_KEY_SIZE
#define CMD_NOT_SUPPORTED                 REASON_COMMAND_NOT_SUPPORTED
#define UNSPECIFIED_REASON                REASON_UNSPECIFIED_REASON
#define VERY_EARLY_NEXT_ATTEMPT           REASON_REPEATED_ATTEMPTS
#define SM_INVALID_PARAMS                 REASON_INVALID_PARAMETERS
#define SMP_SC_DHKEY_CHECK_FAILED         REASON_DHKEY_CHECK_FAILED
#define SMP_SC_NUMCOMPARISON_FAILED       REASON_NUM_COMPARISON_FAILED

#define CONFIG_DATA_PUBADDR_OFFSET        CONFIG_DATA_PUBLIC_ADDRESS_OFFSET
#define CONFIG_DATA_PUBADDR_LEN           CONFIG_DATA_PUBLIC_ADDRESS_LEN

#define FW_L2CAP_RECOMBINATION_ERROR                 0x01U
#define FW_GATT_UNEXPECTED_PEER_MESSAGE              0x02U
#define FW_NVM_LEVEL_WARNING                         0x03U
#define FW_COC_RX_DATA_LENGTH_TOO_LARGE              0x04U
#define FW_ECOC_CONN_RSP_ALREADY_ASSIGNED_DCID       0x05U

/* Deprecated commands
 */

/**
 * @brief ACI_GAP_RESOLVE_PRIVATE_ADDR
 * This command tries to resolve the address provided with the IRKs present in
 * its database. If the address is resolved successfully with any one of the
 * IRKs present in the database, it returns success and also the corresponding
 * public/static random address stored with the IRK in the database.
 * 
 * @param Address Address to be resolved
 * @param[out] Actual_Address The public or static random address of the peer
 *        device, distributed during pairing phase.
 * @return Value indicating success or error code.
 */
__STATIC_INLINE
tBleStatus aci_gap_resolve_private_addr( const uint8_t* Address,
                                         uint8_t* Actual_Address )
{
  uint8_t type;
  return aci_gap_check_bonded_device( 1, Address, &type, Actual_Address );
}

/**
 * @brief ACI_GAP_IS_DEVICE_BONDED
 * The command finds whether the device, whose address is specified in the
 * command, is present in the bonding table. If the device is found, the
 * command returns "Success".
 * Note: the specified address can be a RPA. In this case, even if privacy is
 * not enabled, this address is resolved to check the presence of the peer
 * device in the bonding table.
 * 
 * @param Peer_Address_Type The address type of the peer device.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 * @param Peer_Address Public Device Address or Random Device Address of the
 *        peer device
 * @return Value indicating success or error code.
 */
__STATIC_INLINE
tBleStatus aci_gap_is_device_bonded( uint8_t Peer_Address_Type,
                                     const uint8_t* Peer_Address )
{
  uint8_t type, address[6];
  return aci_gap_check_bonded_device( Peer_Address_Type, Peer_Address,
                                      &type, address );
}


#endif /* BLE_LEGACY_H__ */
