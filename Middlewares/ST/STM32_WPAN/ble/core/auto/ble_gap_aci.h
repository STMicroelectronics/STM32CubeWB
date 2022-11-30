/*****************************************************************************
 * @file    ble_gap_aci.h
 * @author  MDG
 * @brief   STM32WB BLE API (gap_aci)
 *          Auto-generated file: do not edit!
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#ifndef BLE_GAP_ACI_H__
#define BLE_GAP_ACI_H__


#include "ble_types.h"

/**
 * @brief ACI_GAP_SET_NON_DISCOVERABLE
 * Put the device in non-discoverable mode. This command disables the LL
 * advertising.
 * Note: this command only supports legacy advertising.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_non_discoverable( void );

/**
 * @brief ACI_GAP_SET_LIMITED_DISCOVERABLE
 * Put the device in limited discoverable mode (as defined in Bluetooth spec.
 * v.5.3 [Vol 3, Part C, 9.2.3]). The device will be discoverable for maximum
 * period of TGAP (lim_adv_timeout) = 180 seconds (from errata). The
 * advertising can be disabled at any time by issuing
 * ACI_GAP_SET_NON_DISCOVERABLE command.
 * The Adv_Interval_Min and Adv_Interval_Max parameters are optional. If both
 * are set to 0, the GAP will use default values for adv intervals for limited
 * discoverable mode (250 ms and 500 ms respectively).
 * To allow a fast connection, the host can set Local_Name, Service_Uuid_List,
 * Slave_Conn_Interval_Min and Slave_Conn_Interval_Max. If provided, these data
 * will be inserted into the advertising packet payload as AD data. These
 * parameters are optional in this command. These values can be set in
 * advertised data using ACI_GAP_UPDATE_ADV_DATA command separately.
 * The total size of data in advertising packet cannot exceed 31 bytes.
 * With this command, the BLE Stack will also add automatically the following
 * standard AD types:
 * - AD Flags
 * - Power Level
 * When advertising timeout happens (i.e. limited discovery period has
 * elapsed), controller generates ACI_GAP_LIMITED_DISCOVERABLE_EVENT event.
 * Note: this command only supports legacy advertising.
 * 
 * @param Advertising_Type Advertising type
 *        Values:
 *        - 0x00: ADV_IND (Connectable undirected advertising)
 *        - 0x02: ADV_SCAN_IND (Scannable undirected advertising)
 *        - 0x03: ADV_NONCONN_IND (Non connectable undirected advertising)
 * @param Advertising_Interval_Min Minimum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Advertising_Interval_Max Maximum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Advertising_Filter_Policy Advertising filter policy: not applicable
 *        (the value of Advertising_Filter_Policy parameter is not used inside
 *        the Stack)
 * @param Local_Name_Length Length of the local_name field in octets.
 *        If length is set to 0x00, Local_Name parameter is not used.
 * @param Local_Name Local name of the device. First byte must be 0x08 for
 *        Shortened Local Name or 0x09 for Complete Local Name. No NULL
 *        character at the end.
 * @param Service_Uuid_length Length of the Service Uuid List in octets.
 *        If there is no service to be advertised, set this field to 0x00.
 * @param Service_Uuid_List This is the list of the UUIDs as defined in Volume
 *        3, Section 11 of GAP Specification. First byte is the AD Type.
 * @param Slave_Conn_Interval_Min Slave connection interval minimum value
 *        suggested by Peripheral.
 *        If Slave_Conn_Interval_Min and Slave_Conn_Interval_Max are not
 *        0x0000, Slave Connection Interval Range AD structure will be added in
 *        advertising data.
 *        Connection interval is defined in the following manner:
 *        connIntervalmin = Slave_Conn_Interval_Min x 1.25ms.
 *        Values:
 *        - 0x0000 (NaN)
 *        - 0xFFFF (NaN) : No specific minimum
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Slave_Conn_Interval_Max Slave connection interval maximum value
 *        suggested by Peripheral.
 *        If Slave_Conn_Interval_Min and Slave_Conn_Interval_Max are not
 *        0x0000, Slave Connection Interval Range AD structure will be added in
 *        advertising data.
 *        Connection interval is defined in the following manner:
 *        connIntervalmax = Slave_Conn_Interval_Max x 1.25ms
 *        Values:
 *        - 0x0000 (NaN)
 *        - 0xFFFF (NaN) : No specific maximum
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_limited_discoverable( uint8_t Advertising_Type,
                                             uint16_t Advertising_Interval_Min,
                                             uint16_t Advertising_Interval_Max,
                                             uint8_t Own_Address_Type,
                                             uint8_t Advertising_Filter_Policy,
                                             uint8_t Local_Name_Length,
                                             const uint8_t* Local_Name,
                                             uint8_t Service_Uuid_length,
                                             const uint8_t* Service_Uuid_List,
                                             uint16_t Slave_Conn_Interval_Min,
                                             uint16_t Slave_Conn_Interval_Max );

/**
 * @brief ACI_GAP_SET_DISCOVERABLE
 * Put the device in general discoverable mode (as defined in Bluetooth spec.
 * v.5.3 [Vol 3, Part C, 9.2.4]). The device will be discoverable until the
 * host issues the ACI_GAP_SET_NON_DISCOVERABLE command. The Adv_Interval_Min
 * and Adv_Interval_Max parameters are optional. If both are set to 0, the GAP
 * uses the default values for adv intervals for general discoverable mode.
 * When using connectable undirected advertising events:
 * - Adv_Interval_Min = 30 ms
 * - Adv_Interval_Max = 60 ms
 * When using non-connectable advertising events or scannable undirected
 * advertising events:
 * - Adv_Interval_Min = 100 ms
 * - Adv_Interval_Max = 150 ms
 * Host can set the Local Name, a Service UUID list and the Slave Connection
 * Interval Range.
 * If provided, these data will be inserted into the advertising packet payload
 * as AD data.
 * These parameters are optional in this command. These values can be also set
 * using ACI_GAP_UPDATE_ADV_DATA command separately.
 * The total size of data in advertising packet cannot exceed 31 bytes.
 * With this command, the BLE Stack will also add automatically the following
 * standard AD types:
 * - AD Flags
 * - TX Power Level
 * Note: this command only supports legacy advertising.
 * 
 * @param Advertising_Type Advertising type
 *        Values:
 *        - 0x00: ADV_IND (Connectable undirected advertising)
 *        - 0x02: ADV_SCAN_IND (Scannable undirected advertising)
 *        - 0x03: ADV_NONCONN_IND (Non connectable undirected advertising)
 * @param Advertising_Interval_Min Minimum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Advertising_Interval_Max Maximum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Advertising_Filter_Policy Advertising filter policy: not applicable
 *        (the value of Advertising_Filter_Policy parameter is not used inside
 *        the Stack)
 * @param Local_Name_Length Length of the local_name field in octets.
 *        If length is set to 0x00, Local_Name parameter is not used.
 * @param Local_Name Local name of the device. First byte must be 0x08 for
 *        Shortened Local Name or 0x09 for Complete Local Name. No NULL
 *        character at the end.
 * @param Service_Uuid_length Length of the Service Uuid List in octets.
 *        If there is no service to be advertised, set this field to 0x00.
 * @param Service_Uuid_List This is the list of the UUIDs as defined in Volume
 *        3, Section 11 of GAP Specification. First byte is the AD Type.
 * @param Slave_Conn_Interval_Min Slave connection interval minimum value
 *        suggested by Peripheral.
 *        If Slave_Conn_Interval_Min and Slave_Conn_Interval_Max are not
 *        0x0000, Slave Connection Interval Range AD structure will be added in
 *        advertising data.
 *        Connection interval is defined in the following manner:
 *        connIntervalmin = Slave_Conn_Interval_Min x 1.25ms.
 *        Values:
 *        - 0x0000 (NaN)
 *        - 0xFFFF (NaN) : No specific minimum
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Slave_Conn_Interval_Max Slave connection interval maximum value
 *        suggested by Peripheral.
 *        If Slave_Conn_Interval_Min and Slave_Conn_Interval_Max are not
 *        0x0000, Slave Connection Interval Range AD structure will be added in
 *        advertising data.
 *        Connection interval is defined in the following manner:
 *        connIntervalmax = Slave_Conn_Interval_Max x 1.25ms
 *        Values:
 *        - 0x0000 (NaN)
 *        - 0xFFFF (NaN) : No specific maximum
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_discoverable( uint8_t Advertising_Type,
                                     uint16_t Advertising_Interval_Min,
                                     uint16_t Advertising_Interval_Max,
                                     uint8_t Own_Address_Type,
                                     uint8_t Advertising_Filter_Policy,
                                     uint8_t Local_Name_Length,
                                     const uint8_t* Local_Name,
                                     uint8_t Service_Uuid_length,
                                     const uint8_t* Service_Uuid_List,
                                     uint16_t Slave_Conn_Interval_Min,
                                     uint16_t Slave_Conn_Interval_Max );

/**
 * @brief ACI_GAP_SET_DIRECT_CONNECTABLE
 * Set the device in direct connectable mode (as defined in Bluetooth spec.
 * v.5.3 [Vol 3, Part C, 9.3.3]). Device uses direct connectable mode to
 * advertise using High Duty cycle advertisement events or Low Duty cycle
 * advertisement events and the address as either what is specified in the Own
 * Address Type parameter. The command specifies the type of the advertising
 * used.
 * If the privacy is enabled, the Type parameter in reconnection address is
 * used for advertising, otherwise the address of the type specified in
 * Own_Address_Type is used.
 * The device stays in directed connectable mode only for 1.28 seconds. If no
 * connection is established within this duration, the device enters non
 * discoverable mode and advertising has to be again enabled explicitly.
 * The controller generates a HCI_LE_CONNECTION_COMPLETE_EVENT event with the
 * status set to HCI_ADVERTISING_TIMEOUT_ERR_CODE if the connection was not
 * established and BLE_STATUS_SUCCESS (0x00) if the connection was successfully
 * established.
 * Note: this command only supports legacy advertising.
 * 
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 * @param Directed_Advertising_Type Advertising type
 *        Values:
 *        - 0x01: High Duty Cycle Directed Advertising
 *        - 0x04: Low Duty Cycle Directed Advertising
 * @param Direct_Address_Type The address type of the peer device.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 * @param Direct_Address Initiator address
 * @param Advertising_Interval_Min Minimum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0006 (3.750 ms) : for High Duty Cycle Directed Advertising
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms) : for Low Duty Cycle
 *          Directed Advertising
 * @param Advertising_Interval_Max Maximum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0006 (3.750 ms) : for High Duty Cycle Directed Advertising
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms) : for Low Duty Cycle
 *          Directed Advertising
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_direct_connectable( uint8_t Own_Address_Type,
                                           uint8_t Directed_Advertising_Type,
                                           uint8_t Direct_Address_Type,
                                           const uint8_t* Direct_Address,
                                           uint16_t Advertising_Interval_Min,
                                           uint16_t Advertising_Interval_Max );

/**
 * @brief ACI_GAP_SET_IO_CAPABILITY
 * Set the IO capabilities of the device. This command has to be given only
 * when the device is not in a connected state.
 * 
 * @param IO_Capability IO capability of the device.
 *        Values:
 *        - 0x00: IO_CAP_DISPLAY_ONLY
 *        - 0x01: IO_CAP_DISPLAY_YES_NO
 *        - 0x02: IO_CAP_KEYBOARD_ONLY
 *        - 0x03: IO_CAP_NO_INPUT_NO_OUTPUT
 *        - 0x04: IO_CAP_KEYBOARD_DISPLAY
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_io_capability( uint8_t IO_Capability );

/**
 * @brief ACI_GAP_SET_AUTHENTICATION_REQUIREMENT
 * Set the authentication requirements for the device. If the OOB_Enable is set
 * to 0, the following 16 octets of OOB_Data will be ignored on reception. This
 * command has to be given only when the device is not in a connected state.
 * 
 * @param Bonding_Mode Bonding mode.
 *        Only if bonding is enabled (0x01), the bonding information is stored
 *        in flash
 *        Values:
 *        - 0x00: No-bonding mode
 *        - 0x01: Bonding mode
 * @param MITM_Mode MITM mode.
 *        Values:
 *        - 0x00: MITM protection not required
 *        - 0x01: MITM protection required
 * @param SC_Support LE Secure connections support
 *        Values:
 *        - 0x00: Secure Connections Pairing not supported
 *        - 0x01: Secure Connections Pairing supported but optional
 *        - 0x02: Secure Connections Pairing supported and mandatory (SC Only
 *          Mode)
 * @param KeyPress_Notification_Support Keypress notification support
 *        Values:
 *        - 0x00: Keypress notification not supported
 *        - 0x01: Keypress notification supported
 * @param Min_Encryption_Key_Size Minimum encryption key size to be used during
 *        pairing
 * @param Max_Encryption_Key_Size Maximum encryption key size to be used during
 *        pairing
 * @param Use_Fixed_Pin Use or not fixed pin. If set to 0x00, then during the
 *        pairing process the application will not be requested for a pin
 *        (Fixed_Pin will be used).
 *        If set to 0x01, then during pairing process if a passkey is required
 *        the application will be notified
 *        Values:
 *        - 0x00: use a fixed pin
 *        - 0x01: do not use a fixed pin
 * @param Fixed_Pin Fixed pin to be used during pairing if MIMT protection is
 *        enabled.
 *        Any random value between 0 to 999999
 *        Values:
 *        - 0 ... 999999
 * @param Identity_Address_Type Identity address type.
 *        Values:
 *        - 0x00: Public Identity Address
 *        - 0x01: Random (static) Identity Address
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_authentication_requirement( uint8_t Bonding_Mode,
                                                   uint8_t MITM_Mode,
                                                   uint8_t SC_Support,
                                                   uint8_t KeyPress_Notification_Support,
                                                   uint8_t Min_Encryption_Key_Size,
                                                   uint8_t Max_Encryption_Key_Size,
                                                   uint8_t Use_Fixed_Pin,
                                                   uint32_t Fixed_Pin,
                                                   uint8_t Identity_Address_Type );

/**
 * @brief ACI_GAP_SET_AUTHORIZATION_REQUIREMENT
 * Set the authorization requirements of the device. This command has to be
 * given when connected to a device if authorization is required to access
 * services which require authorization.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Authorization_Enable Enable the authorization in the device and when
 *        a remote device tries to read/write a characteristic with
 *        authorization requirements, the stack will send back an error
 *        response with "Insufficient authorization" error code. After pairing
 *        is complete an ACI_GAP_AUTHORIZATION_REQ_EVENT will be sent to the
 *        Host.
 *        Values:
 *        - 0x00: Authorization not required
 *        - 0x01: Authorization required
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_authorization_requirement( uint16_t Connection_Handle,
                                                  uint8_t Authorization_Enable );

/**
 * @brief ACI_GAP_PASS_KEY_RESP
 * This command should be sent by the host in response to
 * ACI_GAP_PASS_KEY_REQ_EVENT event. The command parameter contains the pass
 * key which will be used during the pairing process.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Pass_Key Pass key that will be used during the pairing process.
 *        Must be a six-digit decimal number.
 *        Values:
 *        - 0 ... 999999
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_pass_key_resp( uint16_t Connection_Handle,
                                  uint32_t Pass_Key );

/**
 * @brief ACI_GAP_AUTHORIZATION_RESP
 * Authorize a device to access attributes. This command should be sent by the
 * host in response to ACI_GAP_AUTHORIZATION_REQ_EVENT event.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Authorize Authorization response.
 *        Values:
 *        - 0x01: Authorize
 *        - 0x02: Reject
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_authorization_resp( uint16_t Connection_Handle,
                                       uint8_t Authorize );

/**
 * @brief ACI_GAP_INIT
 * Initialize the GAP layer. Register the GAP service with the GATT.
 * All the standard GAP characteristics will also be added:
 * - Device Name
 * - Appearance
 * - Peripheral Preferred Connection Parameters (peripheral role only).
 * Note that if the Peripheral Preferred Connection Parameters characteristic
 * is added, its handle is equal to the Appearance characteristic handle plus
 * 2.
 * Note also that if privacy is enabled, this command automatically unmasks the
 * HCI_LE_ENHANCED_CONNECTION_COMPLETE_EVENT event.
 * 
 * @param Role Bitmap of allowed roles.
 *        Flags:
 *        - 0x01: Peripheral
 *        - 0x02: Broadcaster
 *        - 0x04: Central
 *        - 0x08: Observer
 * @param privacy_enabled This parameter specifies if Privacy is enabled or
 *        not. N.B.: only Controller Privacy is supported.
 *        Values:
 *        - 0x00: Privacy disabled
 *        - 0x02: Privacy enabled
 * @param device_name_char_len Length of the device name characteristic
 * @param[out] Service_Handle Handle of the GAP service
 * @param[out] Dev_Name_Char_Handle Device Name Characteristic handle
 * @param[out] Appearance_Char_Handle Appearance Characteristic handle
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_init( uint8_t Role,
                         uint8_t privacy_enabled,
                         uint8_t device_name_char_len,
                         uint16_t* Service_Handle,
                         uint16_t* Dev_Name_Char_Handle,
                         uint16_t* Appearance_Char_Handle );

/**
 * @brief ACI_GAP_SET_NON_CONNECTABLE
 * Put the device into non connectable mode. This mode does not support
 * connection. The privacy setting done in the ACI_GAP_INIT command plays a
 * role in deciding the valid parameters for this command.
 * Advertiser filter policy is internally set to 0.
 * 
 * @param Advertising_Event_Type Advertising type
 *        Values:
 *        - 0x02: ADV_SCAN_IND (Scannable undirected advertising)
 *        - 0x03: ADV_NONCONN_IND (Non connectable undirected advertising)
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_non_connectable( uint8_t Advertising_Event_Type,
                                        uint8_t Own_Address_Type );

/**
 * @brief ACI_GAP_SET_UNDIRECTED_CONNECTABLE
 * Put the device into undirected connectable mode.
 * If privacy is enabled in the device, a resolvable private address is
 * generated and used as the advertiser's address. If not, the address of the
 * type specified in Own_Address_Type is used for advertising.
 * Note: this command only supports legacy advertising.
 * 
 * @param Advertising_Interval_Min Minimum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Advertising_Interval_Max Maximum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 * @param Adv_Filter_Policy Advertising filter policy.
 *        Values:
 *        - 0x00: Allow Scan Request from Any, Allow Connect Request from Any
 *        - 0x03: Allow Scan Request from White List Only, Allow Connect
 *          Request from White List Only
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_undirected_connectable( uint16_t Advertising_Interval_Min,
                                               uint16_t Advertising_Interval_Max,
                                               uint8_t Own_Address_Type,
                                               uint8_t Adv_Filter_Policy );

/**
 * @brief ACI_GAP_SLAVE_SECURITY_REQ
 * Send a slave security request to the master.
 * This command has to be issued to notify the master of the security
 * requirements of the slave. The master may encrypt the link, initiate the
 * pairing procedure, or reject the request.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_slave_security_req( uint16_t Connection_Handle );

/**
 * @brief ACI_GAP_UPDATE_ADV_DATA
 * This command can be used to update the advertising data for a particular AD
 * type. If the AD type specified does not exist, then it is added to the
 * advertising data. If the overall advertising data length is more than 31
 * octets after the update, then the command is rejected and the old data is
 * retained.
 * 
 * @param AdvDataLen Length of AdvData in octets
 * @param AdvData Advertising data used by the device while advertising.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_update_adv_data( uint8_t AdvDataLen,
                                    const uint8_t* AdvData );

/**
 * @brief ACI_GAP_DELETE_AD_TYPE
 * This command can be used to delete the specified AD type from the
 * advertisement data if present.
 * 
 * @param ADType One of the AD types as in Bluetooth spec. [Vol 3, Part C, 11].
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_delete_ad_type( uint8_t ADType );

/**
 * @brief ACI_GAP_GET_SECURITY_LEVEL
 * This command can be used to get the current security settings of the device.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param[out] Security_Mode Security mode.
 *        Values:
 *        - 0x01: Security Mode 1
 *        - 0x02: Security Mode 2
 * @param[out] Security_Level Security Level.
 *        Values:
 *        - 0x01: Security Level 1
 *        - 0x02: Security Level 2
 *        - 0x03: Security Level 3
 *        - 0x04: Security Level 4
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_get_security_level( uint16_t Connection_Handle,
                                       uint8_t* Security_Mode,
                                       uint8_t* Security_Level );

/**
 * @brief ACI_GAP_SET_EVENT_MASK
 * It allows masking events from the GAP. The default configuration is all the
 * events masked.
 * 
 * @param GAP_Evt_Mask GAP event mask. Default: 0xFFFF.
 *        Flags:
 *        - 0x0000: No events
 *        - 0x0001: ACI_GAP_LIMITED_DISCOVERABLE_EVENT
 *        - 0x0002: ACI_GAP_PAIRING_COMPLETE_EVENT
 *        - 0x0004: ACI_GAP_PASS_KEY_REQ_EVENT
 *        - 0x0008: ACI_GAP_AUTHORIZATION_REQ_EVENT
 *        - 0x0010: ACI_GAP_SLAVE_SECURITY_INITIATED_EVENT
 *        - 0x0020: ACI_GAP_BOND_LOST_EVENT
 *        - 0x0080: ACI_GAP_PROC_COMPLETE_EVENT
 *        - 0x0100: ACI_L2CAP_CONNECTION_UPDATE_REQ_EVENT
 *        - 0x0200: ACI_L2CAP_CONNECTION_UPDATE_RESP_EVENT
 *        - 0x0400: ACI_L2CAP_PROC_TIMEOUT_EVENT
 *        - 0x0800: ACI_GAP_ADDR_NOT_RESOLVED_EVENT
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_event_mask( uint16_t GAP_Evt_Mask );

/**
 * @brief ACI_GAP_CONFIGURE_WHITELIST
 * Add addresses of bonded devices into the controller's whitelist.
 * The command returns an error if it was unable to add the bonded devices into
 * the whitelist.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_configure_whitelist( void );

/**
 * @brief ACI_GAP_TERMINATE
 * Command the controller to terminate the connection. A
 * HCI_DISCONNECTION_COMPLETE_EVENT event is generated when the link is
 * disconnected.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Reason The reason for ending the connection.
 *        Values:
 *        - 0x05: Authentication Failure
 *        - 0x13: Remote User Terminated Connection
 *        - 0x14: Remote Device Terminated Connection due to Low Resources
 *        - 0x15: Remote Device Terminated Connection due to Power Off
 *        - 0x1A: Unsupported Remote Feature
 *        - 0x3B: Unacceptable Connection Parameters
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_terminate( uint16_t Connection_Handle,
                              uint8_t Reason );

/**
 * @brief ACI_GAP_CLEAR_SECURITY_DB
 * Clear the security database. All the devices in the security database are
 * removed.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_clear_security_db( void );

/**
 * @brief ACI_GAP_ALLOW_REBOND
 * Allows the security manager to complete the pairing procedure and re-bond
 * with the master. This command should be given by the application when it
 * receives the ACI_GAP_BOND_LOST_EVENT if it wants the re-bonding to happen
 * successfully. If this command is not given on receiving the event, the
 * bonding procedure will timeout.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_allow_rebond( uint16_t Connection_Handle );

/**
 * @brief ACI_GAP_START_LIMITED_DISCOVERY_PROC
 * Start the limited discovery procedure. The controller is commanded to start
 * active scanning.
 * When this procedure is started, only the devices in limited discoverable
 * mode are returned to the upper layers.
 * The procedure is terminated when either the upper layers issue a command to
 * terminate the procedure by issuing the command ACI_GAP_TERMINATE_GAP_PROC
 * with the procedure code set to 0x01 or a timeout happens (the timeout value
 * is fixed at 10.24 s.). When the procedure is terminated due to any of the
 * above  reasons, ACI_GAP_PROC_COMPLETE_EVENT event is returned with the
 * procedure code set to 0x01.
 * The device found when the procedure is ongoing is returned to the upper
 * layers through the event HCI_LE_ADVERTISING_REPORT_EVENT.
 * 
 * @param LE_Scan_Interval This is defined as the time interval from when the
 *        Controller started its last LE scan until it begins the subsequent LE
 *        scan.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms)
 * @param LE_Scan_Window Amount of time for the duration of the LE scan.
 *        LE_Scan_Window shall be less than or equal to LE_Scan_Interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms)
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Filter_Duplicates Enable/disable duplicate filtering.
 *        Values:
 *        - 0x00: Duplicate filtering disabled
 *        - 0x01: Duplicate filtering enabled
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_start_limited_discovery_proc( uint16_t LE_Scan_Interval,
                                                 uint16_t LE_Scan_Window,
                                                 uint8_t Own_Address_Type,
                                                 uint8_t Filter_Duplicates );

/**
 * @brief ACI_GAP_START_GENERAL_DISCOVERY_PROC
 * Start the general discovery procedure. The controller is commanded to start
 * active scanning. The procedure is terminated when  either the upper layers
 * issue a command to terminate the procedure by issuing the command
 * ACI_GAP_TERMINATE_GAP_PROC with the procedure code set to 0x02 or a timeout
 * happens (the timeout value is fixed at 10.24 s.). When the procedure is
 * terminated due to any of the above reasons, ACI_GAP_PROC_COMPLETE_EVENT
 * event is returned with the procedure code set to 0x02.
 * The devices found when the procedure is ongoing are returned via
 * HCI_LE_ADVERTISING_REPORT_EVENT (or via
 * HCI_LE_EXTENDED_ADVERTISING_REPORT_EVENT when the extended advertising
 * feature is supported).
 * 
 * @param LE_Scan_Interval This is defined as the time interval from when the
 *        Controller started its last LE scan until it begins the subsequent LE
 *        scan.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param LE_Scan_Window Amount of time for the duration of the LE scan.
 *        LE_Scan_Window shall be less than or equal to LE_Scan_Interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Filter_Duplicates Enable/disable duplicate filtering.
 *        Values:
 *        - 0x00: Duplicate filtering disabled
 *        - 0x01: Duplicate filtering enabled
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_start_general_discovery_proc( uint16_t LE_Scan_Interval,
                                                 uint16_t LE_Scan_Window,
                                                 uint8_t Own_Address_Type,
                                                 uint8_t Filter_Duplicates );

/**
 * @brief ACI_GAP_START_AUTO_CONNECTION_ESTABLISH_PROC
 * Start the auto connection establishment procedure. The devices specified are
 * added to the whitelist of the controller and a LE_Create_Connection call
 * will be made to the controller by GAP with the initiator filter policy set
 * to "use whitelist to determine which advertiser to connect to". When a
 * command is issued to terminate the procedure by upper layer, a
 * LE_Create_Connection_Cancel call will be made to the controller by GAP.
 * The procedure is terminated when either a connection is successfully
 * established with one of the specified devices in the whitelist or the
 * procedure is explicitly terminated by issuing the command
 * ACI_GAP_TERMINATE_GAP_PROC with the procedure code set to 0x08. A
 * ACI_GAP_PROC_COMPLETE_EVENT event is returned with the procedure code set to
 * 0x08.
 * If privacy is enabled and the peer device (advertiser) is in the resolving
 * list then the link layer generates a RPA, if it is not then the RPA
 * generated by the Host is used.
 * 
 * @param LE_Scan_Interval This is defined as the time interval from when the
 *        Controller started its last LE scan until it begins the subsequent LE
 *        scan.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param LE_Scan_Window Amount of time for the duration of the LE scan.
 *        LE_Scan_Window shall be less than or equal to LE_Scan_Interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 * @param Conn_Interval_Min Minimum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Interval_Max Maximum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Latency Maximum Peripheral latency for the connection in number
 *        of connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Supervision_Timeout Supervision timeout for the LE Link.
 *        It shall be a multiple of 10 ms and larger than (1 +
 *        connSlaveLatency) * connInterval * 2.
 *        Time = N * 10 ms.
 *        Values:
 *        - 0x000A (100 ms)  ... 0x0C80 (32000 ms)
 * @param Minimum_CE_Length Information parameter about the minimum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @param Maximum_CE_Length Information parameter about the maximum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @param Num_of_Whitelist_Entries Number of devices that have to be added to
 *        the whitelist.
 * @param Whitelist_Entry See @ref Whitelist_Entry_t
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_start_auto_connection_establish_proc( uint16_t LE_Scan_Interval,
                                                         uint16_t LE_Scan_Window,
                                                         uint8_t Own_Address_Type,
                                                         uint16_t Conn_Interval_Min,
                                                         uint16_t Conn_Interval_Max,
                                                         uint16_t Conn_Latency,
                                                         uint16_t Supervision_Timeout,
                                                         uint16_t Minimum_CE_Length,
                                                         uint16_t Maximum_CE_Length,
                                                         uint8_t Num_of_Whitelist_Entries,
                                                         const Whitelist_Entry_t* Whitelist_Entry );

/**
 * @brief ACI_GAP_START_GENERAL_CONNECTION_ESTABLISH_PROC
 * Start a general connection establishment procedure. The host enables
 * scanning in the controller with the scanner filter policy set to "accept all
 * advertising packets" and from the scanning results, all the devices are sent
 * to the upper layer using the event LE_Advertising_Report. The upper layer
 * then has to select one of the devices to which it wants to connect by
 * issuing the command ACI_GAP_CREATE_CONNECTION. If privacy is enabled, then
 * either a private resolvable address or a non-resolvable address, based on
 * the address type specified in the command is set as the scanner address but
 * the gap create connection always uses a private resolvable address if the
 * general connection establishment procedure is active.
 * The procedure is terminated when a connection is established or the upper
 * layer terminates the procedure by issuing the command
 * ACI_GAP_TERMINATE_GAP_PROC with the procedure code set to 0x10. On
 * completion of the procedure a ACI_GAP_PROC_COMPLETE_EVENT event is generated
 * with the procedure code set to 0x10.
 * If privacy is enabled and the peer device (advertiser) is in the resolving
 * list then the link layer will generate a RPA, if it is not then the RPA/NRPA
 * generated by the Host will be used.
 * 
 * @param LE_Scan_Type Passive or active scanning. With passive scanning, no
 *        scan request PDUs are sent.
 *        Values:
 *        - 0x00: Passive scanning
 *        - 0x01: Active scanning
 * @param LE_Scan_Interval This is defined as the time interval from when the
 *        Controller started its last LE scan until it begins the subsequent LE
 *        scan.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param LE_Scan_Window Amount of time for the duration of the LE scan.
 *        LE_Scan_Window shall be less than or equal to LE_Scan_Interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Scanning_Filter_Policy Scanning filter policy:
 *         - 0x00 Accept all advertisement packets. Directed advertising
 *        packets which are not addressed for this device shall be ignored.
 *         - 0x01 Ignore advertisement packets from devices not in the White
 *        List Only. Directed advertising packets which are not addressed for
 *        this device shall be ignored.
 *         - 0x02 Accept all undirected advertisement packets (it is allowed
 *        only if privacy is enabled). Directed advertisement packets where
 *        initiator address is an RPA and Directed advertisement packets
 *        addressed to this device shall be accepted.
 *         - 0x03 Accept all undirected advertisement packets from devices that
 *        are in the White List. Directed advertisement packets where initiator
 *        address is RPA and Directed advertisement packets addressed to this
 *        device shall be accepted.
 *         - NOTE: if privacy is enabled Scanning_Filter_Policy can only assume
 *        values 0x00 or 0x02.
 *        Values:
 *        - 0x00: Accept all
 *        - 0x01: Ignore devices not in the White List
 *        - 0x02: Accept all (use resolving list)
 *        - 0x03: Ignore devices not in the White List (use resolving list)
 * @param Filter_Duplicates Enable/disable duplicate filtering.
 *        Values:
 *        - 0x00: Duplicate filtering disabled
 *        - 0x01: Duplicate filtering enabled
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_start_general_connection_establish_proc( uint8_t LE_Scan_Type,
                                                            uint16_t LE_Scan_Interval,
                                                            uint16_t LE_Scan_Window,
                                                            uint8_t Own_Address_Type,
                                                            uint8_t Scanning_Filter_Policy,
                                                            uint8_t Filter_Duplicates );

/**
 * @brief ACI_GAP_START_SELECTIVE_CONNECTION_ESTABLISH_PROC
 * Start a selective connection establishment procedure. The GAP adds the
 * specified device addresses into whitelist and enables scanning in the
 * controller with the scanner filter policy set to "accept packets only from
 * devices in whitelist". All the devices found are sent to the upper layer by
 * the event HCI_LE_ADVERTISING_REPORT_EVENT (or by the event
 * HCI_LE_EXTENDED_ADVERTISING_REPORT_EVENT when the extended advertising
 * feature is supported). The upper layer then has to select one of the devices
 * to which it wants to connect by issuing the command
 * ACI_GAP_CREATE_CONNECTION.
 * On completion of the procedure a ACI_GAP_PROC_COMPLETE_EVENT event is
 * generated with the procedure code set to 0x20. The procedure is terminated
 * when a connection is established or the upper layer terminates the procedure
 * by issuing the command ACI_GAP_TERMINATE_GAP_PROC with the procedure code
 * set to 0x20.
 * If privacy is enabled and the peer device (advertiser) is in the resolving
 * list then the link layer will generate a RPA, if it is not then the RPA/NRPA
 * generated by the Host will be used.
 * 
 * @param LE_Scan_Type Passive or active scanning. With passive scanning, no
 *        scan request PDUs are sent.
 *        Values:
 *        - 0x00: Passive scanning
 *        - 0x01: Active scanning
 * @param LE_Scan_Interval This is defined as the time interval from when the
 *        Controller started its last LE scan until it begins the subsequent LE
 *        scan.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param LE_Scan_Window Amount of time for the duration of the LE scan.
 *        LE_Scan_Window shall be less than or equal to LE_Scan_Interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Scanning_Filter_Policy Scanning filter policy:
 *         - 0x00 Accept all advertisement packets. Directed advertising
 *        packets which are not addressed for this device shall be ignored.
 *         - 0x01 Ignore advertisement packets from devices not in the White
 *        List Only. Directed advertising packets which are not addressed for
 *        this device shall be ignored.
 *         - 0x02 Accept all undirected advertisement packets (it is allowed
 *        only if privacy is enabled). Directed advertisement packets where
 *        initiator address is an RPA and Directed advertisement packets
 *        addressed to this device shall be accepted.
 *         - 0x03 Accept all undirected advertisement packets from devices that
 *        are in the White List. Directed advertisement packets where initiator
 *        address is RPA and Directed advertisement packets addressed to this
 *        device shall be accepted.
 *         - NOTE: if privacy is enabled Scanning_Filter_Policy can only assume
 *        values 0x01 or 0x03.
 *        Values:
 *        - 0x00: Accept all
 *        - 0x01: Ignore devices not in the White List
 *        - 0x02: Accept all (use resolving list)
 *        - 0x03: Ignore devices not in the White List (use resolving list)
 * @param Filter_Duplicates Enable/disable duplicate filtering.
 *        Values:
 *        - 0x00: Duplicate filtering disabled
 *        - 0x01: Duplicate filtering enabled
 * @param Num_of_Whitelist_Entries Number of devices that have to be added to
 *        the whitelist.
 * @param Whitelist_Entry See @ref Whitelist_Entry_t
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_start_selective_connection_establish_proc( uint8_t LE_Scan_Type,
                                                              uint16_t LE_Scan_Interval,
                                                              uint16_t LE_Scan_Window,
                                                              uint8_t Own_Address_Type,
                                                              uint8_t Scanning_Filter_Policy,
                                                              uint8_t Filter_Duplicates,
                                                              uint8_t Num_of_Whitelist_Entries,
                                                              const Whitelist_Entry_t* Whitelist_Entry );

/**
 * @brief ACI_GAP_CREATE_CONNECTION
 * Start the direct connection establishment procedure. A LE_Create_Connection
 * call will be made to the controller by GAP with the initiator filter policy
 * set to "ignore whitelist and process connectable advertising packets only
 * for the specified device". The procedure can be terminated explicitly by the
 * upper layer by issuing the command ACI_GAP_TERMINATE_GAP_PROC. When a
 * command is issued to terminate the procedure by upper layer, a
 * HCI_LE_CREATE_CONNECTION_CANCEL call will be made to the controller by GAP.
 * On termination of the procedure, a HCI_LE_CONNECTION_COMPLETE_EVENT event is
 * returned. The procedure can be explicitly terminated by the upper layer by
 * issuing the command ACI_GAP_TERMINATE_GAP_PROC with the Procedure_Code set
 * to 0x40.
 * If privacy is enabled and the peer device (advertiser) is in the resolving
 * list then the link layer will generate a RPA, if it is not then the RPA
 * generated by the Host will be used.
 * 
 * @param LE_Scan_Interval This is defined as the time interval from when the
 *        Controller started its last LE scan until it begins the subsequent LE
 *        scan.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param LE_Scan_Window Amount of time for the duration of the LE scan.
 *        LE_Scan_Window shall be less than or equal to LE_Scan_Interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param Peer_Address_Type The address type of the peer device.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 * @param Peer_Address Public Device Address or Random Device Address of the
 *        device to be connected.
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 * @param Conn_Interval_Min Minimum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Interval_Max Maximum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Latency Maximum Peripheral latency for the connection in number
 *        of connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Supervision_Timeout Supervision timeout for the LE Link.
 *        It shall be a multiple of 10 ms and larger than (1 +
 *        connSlaveLatency) * connInterval * 2.
 *        Time = N * 10 ms.
 *        Values:
 *        - 0x000A (100 ms)  ... 0x0C80 (32000 ms)
 * @param Minimum_CE_Length Information parameter about the minimum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @param Maximum_CE_Length Information parameter about the maximum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_create_connection( uint16_t LE_Scan_Interval,
                                      uint16_t LE_Scan_Window,
                                      uint8_t Peer_Address_Type,
                                      const uint8_t* Peer_Address,
                                      uint8_t Own_Address_Type,
                                      uint16_t Conn_Interval_Min,
                                      uint16_t Conn_Interval_Max,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint16_t Minimum_CE_Length,
                                      uint16_t Maximum_CE_Length );

/**
 * @brief ACI_GAP_TERMINATE_GAP_PROC
 * Terminate the specified GAP procedure. An ACI_GAP_PROC_COMPLETE_EVENT event
 * is returned with the procedure code set to the corresponding procedure.
 * 
 * @param Procedure_Code GAP procedure bitmap.
 *        Values:
 *        - 0x00: No events
 *        - 0x01: GAP_LIMITED_DISCOVERY_PROC
 *        - 0x02: GAP_GENERAL_DISCOVERY_PROC
 *        - 0x08: GAP_AUTO_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x10: GAP_GENERAL_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x20: GAP_SELECTIVE_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x40: GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC
 *        - 0x80: GAP_OBSERVATION_PROC
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_terminate_gap_proc( uint8_t Procedure_Code );

/**
 * @brief ACI_GAP_START_CONNECTION_UPDATE
 * Start the connection update procedure (only when role is Master). A
 * HCI_LE_CONNECTION_UPDATE is called.
 * On completion of the procedure, an HCI_LE_CONNECTION_UPDATE_COMPLETE_EVENT
 * event is returned to the upper layer.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Conn_Interval_Min Minimum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Interval_Max Maximum value for the connection event interval.
 *        Time = N * 1.25 ms.
 *        Values:
 *        - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms)
 * @param Conn_Latency Maximum Peripheral latency for the connection in number
 *        of connection events.
 *        Values:
 *        - 0x0000 ... 0x01F3
 * @param Supervision_Timeout Supervision timeout for the LE Link.
 *        It shall be a multiple of 10 ms and larger than (1 +
 *        connSlaveLatency) * connInterval * 2.
 *        Time = N * 10 ms.
 *        Values:
 *        - 0x000A (100 ms)  ... 0x0C80 (32000 ms)
 * @param Minimum_CE_Length Information parameter about the minimum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @param Maximum_CE_Length Information parameter about the maximum length of
 *        connection needed for this LE connection.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0000 (0.000 ms)  ... 0xFFFF (40959.375 ms)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_start_connection_update( uint16_t Connection_Handle,
                                            uint16_t Conn_Interval_Min,
                                            uint16_t Conn_Interval_Max,
                                            uint16_t Conn_Latency,
                                            uint16_t Supervision_Timeout,
                                            uint16_t Minimum_CE_Length,
                                            uint16_t Maximum_CE_Length );

/**
 * @brief ACI_GAP_SEND_PAIRING_REQ
 * Send the SM pairing request to start a pairing process. The authentication
 * requirements and IO capabilities should be set before issuing this command
 * using the ACI_GAP_SET_IO_CAPABILITY and
 * ACI_GAP_SET_AUTHENTICATION_REQUIREMENT commands.
 * A ACI_GAP_PAIRING_COMPLETE_EVENT event is returned after the pairing process
 * is completed.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Force_Rebond If 1, Pairing request will be sent even if the device
 *        was previously bonded,
 *         otherwise pairing request is not sent.
 *        Values:
 *        - 0x00: NO
 *        - 0x01: YES
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_send_pairing_req( uint16_t Connection_Handle,
                                     uint8_t Force_Rebond );

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
tBleStatus aci_gap_resolve_private_addr( const uint8_t* Address,
                                         uint8_t* Actual_Address );

/**
 * @brief ACI_GAP_SET_BROADCAST_MODE
 * This command puts the device into broadcast mode. A privacy enabled device
 * uses either a resolvable private address or a non-resolvable private address
 * as specified in the Own_Address_Type parameter of the command.
 * Note: this command only supports legacy advertising.
 * 
 * @param Advertising_Interval_Min Minimum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Advertising_Interval_Max Maximum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Advertising_Type Advertising type
 *        Values:
 *        - 0x02: ADV_SCAN_IND (Scannable undirected advertising)
 *        - 0x03: ADV_NONCONN_IND (Non connectable undirected advertising)
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Adv_Data_Length Length of the advertising data in the advertising
 *        packet.
 * @param Adv_Data Advertising data used by the device while advertising.
 * @param Num_of_Whitelist_Entries Number of devices that have to be added to
 *        the whitelist.
 * @param Whitelist_Entry See @ref Whitelist_Entry_t
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_broadcast_mode( uint16_t Advertising_Interval_Min,
                                       uint16_t Advertising_Interval_Max,
                                       uint8_t Advertising_Type,
                                       uint8_t Own_Address_Type,
                                       uint8_t Adv_Data_Length,
                                       const uint8_t* Adv_Data,
                                       uint8_t Num_of_Whitelist_Entries,
                                       const Whitelist_Entry_t* Whitelist_Entry );

/**
 * @brief ACI_GAP_START_OBSERVATION_PROC
 * Starts an Observation procedure when the device is in Observer Role. The
 * host enables scanning in the controller. The advertising reports are sent to
 * the upper layer using standard LE Advertising Report Event.
 * If privacy is enabled and the peer device (advertiser) is in the resolving
 * list then the link layer will generate a RPA, if it is not then the RPA/NRPA
 * generated by the Host will be used.
 * 
 * @param LE_Scan_Interval This is defined as the time interval from when the
 *        Controller started its last LE scan until it begins the subsequent LE
 *        scan.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param LE_Scan_Window Amount of time for the duration of the LE scan.
 *        LE_Scan_Window shall be less than or equal to LE_Scan_Interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0004 (2.500 ms)  ... 0x4000 (10240.000 ms) : legacy advertising
 *        - 0x0004 (2.500 ms)  ... 0x5DC0 (15000.000 ms) : extended advertising
 *          with STM32WB
 *        - 0x0004 (2.500 ms)  ... 0xFFFF (40959.375 ms) : extended advertising
 *          with STM32WBA
 * @param LE_Scan_Type Passive or active scanning. With passive scanning, no
 *        scan request PDUs are sent.
 *        Values:
 *        - 0x00: Passive scanning
 *        - 0x01: Active scanning
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Filter_Duplicates Enable/disable duplicate filtering.
 *        Values:
 *        - 0x00: Duplicate filtering disabled
 *        - 0x01: Duplicate filtering enabled
 * @param Scanning_Filter_Policy Scanning filter policy:
 *         - 0x00 Accept all advertisement packets (it is allowed only if
 *        privacy is enabled). Directed advertising packets which are not
 *        addressed for this device shall be ignored.
 *         - 0x01 Ignore advertisement packets from devices not in the White
 *        List Only. Directed advertising packets which are not addressed for
 *        this device shall be ignored.
 *         - 0x02 Accept all undirected advertisement packets (it is allowed
 *        only if privacy is enabled). Directed advertisement packets where
 *        initiator address is an RPA and Directed advertisement packets
 *        addressed to this device shall be accepted.
 *         - 0x03 Accept all undirected advertisement packets from devices that
 *        are in the White List. Directed advertisement packets where initiator
 *        address is RPA and Directed advertisement packets addressed to this
 *        device shall be accepted.
 *        Values:
 *        - 0x00: Accept all
 *        - 0x01: Ignore devices not in the White List
 *        - 0x02: Accept all (use resolving list)
 *        - 0x03: Ignore devices not in the White List (use resolving list)
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_start_observation_proc( uint16_t LE_Scan_Interval,
                                           uint16_t LE_Scan_Window,
                                           uint8_t LE_Scan_Type,
                                           uint8_t Own_Address_Type,
                                           uint8_t Filter_Duplicates,
                                           uint8_t Scanning_Filter_Policy );

/**
 * @brief ACI_GAP_GET_BONDED_DEVICES
 * This command gets the list of the devices which are bonded. It returns the
 * number of addresses and the corresponding address types and values.
 * 
 * @param[out] Num_of_Addresses The number of bonded devices
 * @param[out] Bonded_Device_Entry See @ref Bonded_Device_Entry_t
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_get_bonded_devices( uint8_t* Num_of_Addresses,
                                       Bonded_Device_Entry_t* Bonded_Device_Entry );

/**
 * @brief ACI_GAP_IS_DEVICE_BONDED
 * The command finds whether the device, whose address is specified in the
 * command, is bonded. If the device is using a resolvable private address and
 * it has been bonded, then the command will return BLE_STATUS_SUCCESS.
 * 
 * @param Peer_Address_Type Identity address type.
 *        Values:
 *        - 0x00: Public Identity Address
 *        - 0x01: Random (static) Identity Address
 * @param Peer_Address Public or Random (static) Identity address of the peer
 *        device
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_is_device_bonded( uint8_t Peer_Address_Type,
                                     const uint8_t* Peer_Address );

/**
 * @brief ACI_GAP_NUMERIC_COMPARISON_VALUE_CONFIRM_YESNO
 * This command allows the User to validate/confirm or not the Numeric
 * Comparison value showed through the ACI_GAP_Numeric_Comparison_Value_Event.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Confirm_Yes_No 0 : The Numeric Values showed on both local and peer
 *        device are different!
 *        1 : The Numeric Values showed on both local and peer device are
 *        equal!
 *        Values:
 *        - 0x00: No
 *        - 0x01: Yes
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_numeric_comparison_value_confirm_yesno( uint16_t Connection_Handle,
                                                           uint8_t Confirm_Yes_No );

/**
 * @brief ACI_GAP_PASSKEY_INPUT
 * This command permits to signal to the Stack the input type detected during
 * Passkey input.
 * 
 * @param Connection_Handle Connection handle for which the command applies.
 *        Values:
 *        - 0x0000 ... 0x0EFF
 * @param Input_Type Passkey input type detected
 *        Values:
 *        - 0x00: Passkey entry started
 *        - 0x01: Passkey digit entered
 *        - 0x02: Passkey digit erased
 *        - 0x03: Passkey cleared
 *        - 0x04: Passkey entry completed
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_passkey_input( uint16_t Connection_Handle,
                                  uint8_t Input_Type );

/**
 * @brief ACI_GAP_GET_OOB_DATA
 * This command is sent by the User to get (i.e. to extract from the Stack) the
 * OOB data generated by the Stack itself.
 * 
 * @param OOB_Data_Type OOB Data type
 *        Values:
 *        - 0x00: TK (LP v.4.1)
 *        - 0x01: Random (SC)
 *        - 0x02: Confirm (SC)
 * @param[out] Address_Type Identity address type.
 *        Values:
 *        - 0x00: Public Identity Address
 *        - 0x01: Random (static) Identity Address
 * @param[out] Address Public or Random (static) address of this  device
 * @param[out] OOB_Data_Len Length of OOB Data
 * @param[out] OOB_Data Local Pairing Data intended to the remote device to be
 *        sent via OOB.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_get_oob_data( uint8_t OOB_Data_Type,
                                 uint8_t* Address_Type,
                                 uint8_t* Address,
                                 uint8_t* OOB_Data_Len,
                                 uint8_t* OOB_Data );

/**
 * @brief ACI_GAP_SET_OOB_DATA
 * This command is sent (by the User) to input the OOB data arrived via OOB
 * communication.
 * 
 * @param Device_Type OOB Device type
 *        Values:
 *        - 0x00: Local device
 *        - 0x01: Remote device
 * @param Address_Type Identity address type.
 *        Values:
 *        - 0x00: Public Identity Address
 *        - 0x01: Random (static) Identity Address
 * @param Address Public or Random (static) address of the peer device
 * @param OOB_Data_Type OOB Data type
 *        Values:
 *        - 0x00: TK (LP v.4.1)
 *        - 0x01: Random (SC)
 *        - 0x02: Confirm (SC)
 * @param OOB_Data_Len Length of OOB Data
 * @param OOB_Data Pairing Data received through OOB from remote device
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_set_oob_data( uint8_t Device_Type,
                                 uint8_t Address_Type,
                                 const uint8_t* Address,
                                 uint8_t OOB_Data_Type,
                                 uint8_t OOB_Data_Len,
                                 const uint8_t* OOB_Data );

/**
 * @brief ACI_GAP_ADD_DEVICES_TO_RESOLVING_LIST
 * This  command is used to add devices to the list of address translations
 * used to resolve Resolvable Private Addresses in the Controller.
 * 
 * @param Num_of_Resolving_list_Entries Number of devices that have to be added
 *        to the list.
 * @param Whitelist_Identity_Entry See @ref Whitelist_Identity_Entry_t
 * @param Clear_Resolving_List Clear the resolving list
 *        Values:
 *        - 0x00: Do not clear
 *        - 0x01: Clear before adding
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_add_devices_to_resolving_list( uint8_t Num_of_Resolving_list_Entries,
                                                  const Whitelist_Identity_Entry_t* Whitelist_Identity_Entry,
                                                  uint8_t Clear_Resolving_List );

/**
 * @brief ACI_GAP_REMOVE_BONDED_DEVICE
 * This command is used to remove a specified device from bonding table
 * 
 * @param Peer_Identity_Address_Type Identity address type.
 *        Values:
 *        - 0x00: Public Identity Address
 *        - 0x01: Random (static) Identity Address
 * @param Peer_Identity_Address Public or Random (static) Identity address of
 *        the peer device
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_remove_bonded_device( uint8_t Peer_Identity_Address_Type,
                                         const uint8_t* Peer_Identity_Address );

/**
 * @brief ACI_GAP_ADD_DEVICES_TO_LIST
 * This  command is used to add specific device addresses to the white and/or
 * resolving list.
 * 
 * @param Num_of_List_Entries Number of devices that have to be added to the
 *        list.
 * @param List_Entry See @ref List_Entry_t
 * @param Mode Mode used for adding devices in the lists.
 *        Values:
 *        - 0x00: Append to the resolving list only
 *        - 0x01: Clear and set the resolving list only
 *        - 0x02: Append to the white list only
 *        - 0x03: Clear and set the white list only
 *        - 0x04: Append to both resolving and white lists
 *        - 0x05: Clear and set both resolving and white lists
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_add_devices_to_list( uint8_t Num_of_List_Entries,
                                        const List_Entry_t* List_Entry,
                                        uint8_t Mode );

/**
 * @brief ACI_GAP_ADDITIONAL_BEACON_START
 * This command starts an advertising beacon. It allows additional advertising
 * packets to be transmitted independently of the packets transmitted with GAP
 * advertising commands such as ACI_GAP_SET_DISCOVERABLE or
 * ACI_GAP_SET_LIMITED_DISCOVERABLE.
 * 
 * @param Adv_Interval_Min Minimum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Adv_Interval_Max Maximum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x0020 (20.000 ms)  ... 0x4000 (10240.000 ms)
 * @param Adv_Channel_Map Advertising channel map.
 *        Flags:
 *        - 0x01: Channel 37 shall be used
 *        - 0x02: Channel 38 shall be used
 *        - 0x04: Channel 39 shall be used
 * @param Own_Address_Type Own address type: public or static random.
 *        Values:
 *        - 0x00: Public Device Address
 *        - 0x01: Random Device Address
 * @param Own_Address Public Device Address or Random Device Address.
 * @param PA_Level Power amplifier output level. Output power is indicative and
 *        depends on the PCB layout and associated components. Here the values
 *        are given at the STM32WB output.
 *        Values:
 *        - 0x00: -40 dBm
 *        - 0x01: -20.85 dBm
 *        - 0x02: -19.75 dBm
 *        - 0x03: -18.85 dBm
 *        - 0x04: -17.6 dBm
 *        - 0x05: -16.5 dBm
 *        - 0x06: -15.25 dBm
 *        - 0x07: -14.1 dBm
 *        - 0x08: -13.15 dBm
 *        - 0x09: -12.05 dBm
 *        - 0x0A: -10.9 dBm
 *        - 0x0B: -9.9 dBm
 *        - 0x0C: -8.85 dBm
 *        - 0x0D: -7.8 dBm
 *        - 0x0E: -6.9 dBm
 *        - 0x0F: -5.9 dBm
 *        - 0x10: -4.95 dBm
 *        - 0x11: -4 dBm
 *        - 0x12: -3.15 dBm
 *        - 0x13: -2.45 dBm
 *        - 0x14: -1.8 dBm
 *        - 0x15: -1.3 dBm
 *        - 0x16: -0.85 dBm
 *        - 0x17: -0.5 dBm
 *        - 0x18: -0.15 dBm
 *        - 0x19: 0 dBm
 *        - 0x1A: +1 dBm
 *        - 0x1B: +2 dBm
 *        - 0x1C: +3 dBm
 *        - 0x1D: +4 dBm
 *        - 0x1E: +5 dBm
 *        - 0x1F: +6 dBm
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_additional_beacon_start( uint16_t Adv_Interval_Min,
                                            uint16_t Adv_Interval_Max,
                                            uint8_t Adv_Channel_Map,
                                            uint8_t Own_Address_Type,
                                            const uint8_t* Own_Address,
                                            uint8_t PA_Level );

/**
 * @brief ACI_GAP_ADDITIONAL_BEACON_STOP
 * This command stops the advertising beacon started with
 * ACI_GAP_ADDITIONAL_BEACON_START.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_additional_beacon_stop( void );

/**
 * @brief ACI_GAP_ADDITIONAL_BEACON_SET_DATA
 * This command sets the data transmitted by the advertising beacon started
 * with ACI_GAP_ADDITIONAL_BEACON_START. If the advertising beacon is already
 * started, the new data is used in subsequent beacon advertising events.
 * 
 * @param Adv_Data_Length Length of Adv_Data in octets
 * @param Adv_Data Advertising data used by the device while advertising.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_additional_beacon_set_data( uint8_t Adv_Data_Length,
                                               const uint8_t* Adv_Data );

/**
 * @brief ACI_GAP_ADV_SET_CONFIGURATION
 * This command is used to set the extended advertising configuration for one
 * advertising set.
 * This command, in association with ACI_GAP_ADV_SET_SCAN_RESP_DATA,
 * ACI_GAP_ADV_SET_ADV_DATA and ACI_GAP_ADV_SET_ENABLE, enables to start
 * extended advertising. These commands must be used in replacement of
 * ACI_GAP_SET_DISCOVERABLE, ACI_GAP_SET_LIMITED_DISCOVERABLE,
 * ACI_GAP_SET_DIRECT_CONNECTABLE, ACI_GAP_SET_NON_CONNECTABLE,
 * ACI_GAP_SET_UNDIRECTED_CONNECTABLE and ACI_GAP_SET_BROADCAST_MODE that only
 * support legacy advertising.
 * If bit 0 of Adv_Mode is set, the Own_Address_Type parameter is ignored and
 * the own address shall be set with the ACI_GAP_ADV_SET_RANDOM_ADDRESS
 * command. This mode is only valid for non-connectable advertising.
 * 
 * @param Adv_Mode Bitmap of extended advertising modes
 *        Flags:
 *        - 0x01: Use specific random address
 * @param Advertising_Handle Used to identify an advertising set.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Adv_Event_Properties Type of advertising event.
 *        Flags:
 *        - 0x0001: Connectable advertising
 *        - 0x0002: Scannable advertising
 *        - 0x0004: Directed advertising
 *        - 0x0008: High Duty Cycle Directed Connectable advertising
 *        - 0x0010: Use legacy advertising PDUs
 *        - 0x0020: Anonymous advertising
 *        - 0x0040: Include TxPower in at least one advertising PDU
 * @param Primary_Adv_Interval_Min Minimum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x00000020 (20.000 ms)  ... 0x00FFFFFF (10485759.375 ms)
 * @param Primary_Adv_Interval_Max Maximum advertising interval.
 *        Time = N * 0.625 ms.
 *        Values:
 *        - 0x00000020 (20.000 ms)  ... 0x00FFFFFF (10485759.375 ms)
 * @param Primary_Adv_Channel_Map Advertising channel map.
 *        Flags:
 *        - 0x01: Channel 37 shall be used
 *        - 0x02: Channel 38 shall be used
 *        - 0x04: Channel 39 shall be used
 * @param Own_Address_Type Own address type: if Privacy is disabled, the
 *        address can be public or static random; otherwise, it can be a
 *        resolvable private address or a non-resolvable private address.
 *        Values:
 *        - 0x00: Public address
 *        - 0x01: Static random address
 *        - 0x02: Resolvable private address
 *        - 0x03: Non-resolvable private address
 * @param Peer_Address_Type Address type of the peer device.
 *        Values:
 *        - 0x00: Public Device Address or Public Identity Address
 *        - 0x01: Random Device Address or Random (static) Identity Address
 * @param Peer_Address Public Device Address, Random Device Address, Public
 *        Identity Address, or Random (static) Identity Address of the device
 *        to be connected.
 * @param Adv_Filter_Policy Advertising filter policy
 *        Values:
 *        - 0x00: Process scan and connection requests from all devices (i.e.,
 *          the White List is not in use)
 *        - 0x01: Process connection requests from all devices and scan
 *          requests only from devices that are in the White List.
 *        - 0x02: Process scan requests from all devices and connection
 *          requests only from devices that are in the White List.
 *        - 0x03: Process scan and connection requests only from devices in the
 *          White  List.
 * @param Adv_TX_Power Advertising TX power. Units: dBm.
 *        Values:
 *        - -127 ... 20
 * @param Secondary_Adv_Max_Skip Secondary advertising maximum skip.
 *        Values:
 *        - 0x00: AUX_ADV_IND shall be sent prior to the next advertising event
 *        - 0x01 ... 0xFF: Maximum advertising events the Controller can skip
 *          before sending the AUX_ADV_IND packets on the secondary advertising
 *          physical channel
 * @param Secondary_Adv_PHY Secondary advertising PHY.
 *        Values:
 *        - 0x01: Secondary advertisement PHY is LE 1M
 *        - 0x02: Secondary advertisement PHY is LE 2M
 *        - 0x03: Secondary advertisement PHY is LE Coded (not supported on
 *          STM32WB)
 * @param Adv_SID Value of the Advertising SID subfield in the ADI field of the
 *        PDU.
 *        Values:
 *        - 0x00 ... 0x0F
 * @param Scan_Req_Notification_Enable Scan request notifications.
 *        Values:
 *        - 0x00: Scan request notifications disabled
 *        - 0x01: Scan request notifications enabled
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_adv_set_configuration( uint8_t Adv_Mode,
                                          uint8_t Advertising_Handle,
                                          uint16_t Adv_Event_Properties,
                                          uint32_t Primary_Adv_Interval_Min,
                                          uint32_t Primary_Adv_Interval_Max,
                                          uint8_t Primary_Adv_Channel_Map,
                                          uint8_t Own_Address_Type,
                                          uint8_t Peer_Address_Type,
                                          const uint8_t* Peer_Address,
                                          uint8_t Adv_Filter_Policy,
                                          uint8_t Adv_TX_Power,
                                          uint8_t Secondary_Adv_Max_Skip,
                                          uint8_t Secondary_Adv_PHY,
                                          uint8_t Adv_SID,
                                          uint8_t Scan_Req_Notification_Enable );

/**
 * @brief ACI_GAP_ADV_SET_ENABLE
 * This command is used to request the Controller to enable or disable one or
 * more extended advertising sets.
 * 
 * @param Enable Enable/disable advertising.
 *        Values:
 *        - 0x00: Advertising is disabled
 *        - 0x01: Advertising is enabled
 * @param Num_Sets Number of advertising sets.
 *        Values:
 *        - 0x00: Disable all advertising sets
 *        - 0x01 ... 0x3F: Number of advertising sets to enable or disable
 * @param Adv_Set See @ref Adv_Set_t
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_adv_set_enable( uint8_t Enable,
                                   uint8_t Num_Sets,
                                   const Adv_Set_t* Adv_Set );

/**
 * @brief ACI_GAP_ADV_SET_ADV_DATA
 * This command is used to set the data used in extended advertising PDUs that
 * have a data field.
 * 
 * @param Advertising_Handle Used to identify an advertising set.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Operation Advertising operation.
 *        Values:
 *        - 0x00: Intermediate fragment of fragmented extended advertising data
 *        - 0x01: First fragment of fragmented extended advertising data
 *        - 0x02: Last fragment of fragmented extended advertising data
 *        - 0x03: Complete extended advertising data
 *        - 0x04: Unchanged data (just update the Advertising DID)
 * @param Fragment_Preference Fragment preference.
 *        Values:
 *        - 0x00: The Controller may fragment all data
 *        - 0x01: The Controller should not fragment or should minimize
 *          fragmentation of data
 * @param Advertising_Data_Length Length of Advertising_Data in octets
 * @param Advertising_Data Data formatted as defined in Bluetooth spec. v.5.3
 *        [Vol 3, Part C, 11].
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_adv_set_adv_data( uint8_t Advertising_Handle,
                                     uint8_t Operation,
                                     uint8_t Fragment_Preference,
                                     uint8_t Advertising_Data_Length,
                                     const uint8_t* Advertising_Data );

/**
 * @brief ACI_GAP_ADV_SET_SCAN_RESP_DATA
 * This command is used to provide scan response data used during extended
 * advertising.
 * 
 * @param Advertising_Handle Used to identify an advertising set.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Operation Scan response operation.
 *        Values:
 *        - 0x00: Intermediate fragment of fragmented scan response data
 *        - 0x01: First fragment of fragmented scan response data
 *        - 0x02: Last fragment of fragmented scan response data
 *        - 0x03: Complete scan response data
 * @param Fragment_Preference Fragment preference.
 *        Values:
 *        - 0x00: The Controller may fragment all data
 *        - 0x01: The Controller should not fragment or should minimize
 *          fragmentation of data
 * @param Scan_Response_Data_Length Length of Scan_Response_Data in octets
 * @param Scan_Response_Data Data formatted as defined in Bluetooth spec. v.5.3
 *        [Vol 3, Part C, 11].
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_adv_set_scan_resp_data( uint8_t Advertising_Handle,
                                           uint8_t Operation,
                                           uint8_t Fragment_Preference,
                                           uint8_t Scan_Response_Data_Length,
                                           const uint8_t* Scan_Response_Data );

/**
 * @brief ACI_GAP_ADV_REMOVE_SET
 * This command is used to remove an advertising set from the Controller.
 * 
 * @param Advertising_Handle Used to identify an advertising set.
 *        Values:
 *        - 0x00 ... 0xEF
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_adv_remove_set( uint8_t Advertising_Handle );

/**
 * @brief ACI_GAP_ADV_CLEAR_SETS
 * This command is used to remove all existing advertising sets from the
 * Controller.
 * 
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_adv_clear_sets( void );

/**
 * @brief ACI_GAP_ADV_SET_RANDOM_ADDRESS
 * This command is used to set the random device address of an advertising set
 * configured to use specific random address.
 * 
 * @param Advertising_Handle Used to identify an advertising set.
 *        Values:
 *        - 0x00 ... 0xEF
 * @param Random_Address Random Device Address.
 * @return Value indicating success or error code.
 */
tBleStatus aci_gap_adv_set_random_address( uint8_t Advertising_Handle,
                                           const uint8_t* Random_Address );


#endif /* BLE_GAP_ACI_H__ */
