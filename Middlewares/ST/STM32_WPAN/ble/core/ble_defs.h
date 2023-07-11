/*****************************************************************************
 * @file    ble_defs.h
 * @author  MDG
 * @brief   This file contains definitions used for BLE Stack interface.
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

#ifndef BLE_DEFS_H__
#define BLE_DEFS_H__


/* ------------------------------------------------------------------------- */

/* Status codes */

/* Returned when the command has completed with success
 */
#define BLE_STATUS_SUCCESS                              0x00U

/* The remote device in in the Blacklist and the pairing operation it requested
 * cannot be performed.
 */
#define BLE_STATUS_DEV_IN_BLACKLIST                     0x59U

/* CSRK not found during validation of an incoming signed packet
 */
#define BLE_STATUS_CSRK_NOT_FOUND                       0x5AU

/* IRK not found (Currently not used)
 */
#define BLE_STATUS_IRK_NOT_FOUND                        0x5BU

/* A search for a specific remote device was unsuccessful because no entry
 * exists either into NVM Database or in volatile database. 
 */
#define BLE_STATUS_DEV_NOT_FOUND                        0x5CU

/* The security database is full and no more records can be added.
 */
#define BLE_STATUS_SEC_DB_FULL                          0x5DU

/* The remote device is not bonded, and no operations related to bonded devices
 * may be performed (e.g. writing Gatt Client data).
 */
#define BLE_STATUS_DEV_NOT_BONDED                       0x5EU

/* The encryption key size used for encrypting the link is insufficient\n
 */
#define BLE_STATUS_INSUFFICIENT_ENC_KEYSIZE             0x5FU

/* The attribute handle is invalid.
 */
#define BLE_STATUS_INVALID_HANDLE                       0x60U

/* There aren't sufficient Attributes handles available for allocation during
 * creation of Services, Characteristics or Descriptors.
 */
#define BLE_STATUS_OUT_OF_HANDLE                        0x61U

/* The requested GATT operation is not allowed in this context/status or using 
 * the provided parameters. 
 * This is a specific GATT error, different from generic Not Allowed error, 
 * because it refers to specific GATT specifications/rules.
 */
#define BLE_STATUS_INVALID_OPERATION                    0x62U

/* The characteristic has already been added to the database.
 */
#define BLE_STATUS_CHARAC_ALREADY_EXISTS                0x63U

/* The requested operation failed for a temporary lack of resources
 * (e.g. packet pool or timers), but it may be retried later when resources may
 * become available (packets or timers may have been released by other
 * consumers).
 */
#define BLE_STATUS_INSUFFICIENT_RESOURCES               0x64U

/* Notification/Indication can't be sent to the requested remote device because
 * it doesn't satisfy the needed security permission.
 */ 
#define BLE_STATUS_SEC_PERMISSION_ERROR                 0x65U

/* The address of the device could not be resolved using the IRK stored\n
 */
#define BLE_STATUS_ADDRESS_NOT_RESOLVED                 0x70U

/* Returned when no valid slots are available
 * (e.g. when there are no available state machines).
 */
#define BLE_STATUS_NO_VALID_SLOT                        0x82U

/* The only slot available is not long enough to satisfy scan window request.
 */
#define BLE_STATUS_SCAN_WINDOW_SHORT                    0x83U

/* Returned when the maximum requested interval to be allocated is shorter
 * then the current anchor period and there is no submultiple for the
 * current anchor period that is between the minimum and the maximum requested
 * intervals.
 */
#define BLE_STATUS_NEW_INTERVAL_FAILED                  0x84U

/* Returned when the maximum requested interval to be allocated is greater
 * than the current anchor period and there is no multiple of the anchor
 * period that is between the minimum and the maximum requested intervals.
 */
#define BLE_STATUS_INTERVAL_TOO_LARGE                   0x85U

/* Returned when the current anchor period or a new one can be found that
 * is compatible to the interval range requested by the new slot but the
 * maximum available length that can be allocated is less than the minimum
 * requested slot length.
 */
#define BLE_STATUS_LENGTH_FAILED                        0x86U

/* The Host failed while performing the requested operation.
 */
#define BLE_STATUS_FAILED                               0x91U

/* Invalid parameters in Host commands
 */
#define BLE_STATUS_INVALID_PARAMS                       0x92U

/* The Host is already processing another request received in advance.
 */
#define BLE_STATUS_BUSY                                 0x93U

/* The operation requested cannot be completed immediately by the Host
 * (usually because of lack of resources). 
 * The operation is generally put on hold by the caller and it's usually 
 * retried on later time.
 */
#define BLE_STATUS_PENDING                              0x95U

/* The requested operation violates the logic of the called layer/function or
 * the format of the data to be processed during the operation.
 */
#define BLE_STATUS_ERROR                                0x97U

/* The requested operation failed because of lack of memory.
 * Out of memory shall be returned for situations where memory will never 
 * become available again (e.g. ATT database)
 */
#define BLE_STATUS_OUT_OF_MEMORY                        0x98U

/* Returned when a timeout occurs at BLE application interface
 */
#define BLE_STATUS_TIMEOUT                              0xFFU

/* ------------------------------------------------------------------------- */

/* Characteristic value lengths
 */
#define DEVICE_NAME_CHARACTERISTIC_LEN                  8
#define APPEARANCE_CHARACTERISTIC_LEN                   2
#define PERIPHERAL_PRIVACY_CHARACTERISTIC_LEN           1
#define RECONNECTION_ADDR_CHARACTERISTIC_LEN            6
#define PERIPHERAL_PREF_CONN_PARAMS_CHARACTERISTIC_LEN  8

/* Adv. lengths
 */
#define MAX_ADV_DATA_LEN                              31U
#define BD_ADDR_SIZE                                   6U

/* AD types for adv. data and scan response data
 */
#define AD_TYPE_FLAGS                                0x01U
#define AD_TYPE_16_BIT_SERV_UUID                     0x02U
#define AD_TYPE_16_BIT_SERV_UUID_CMPLT_LIST          0x03U
#define AD_TYPE_32_BIT_SERV_UUID                     0x04U
#define AD_TYPE_32_BIT_SERV_UUID_CMPLT_LIST          0x05U
#define AD_TYPE_128_BIT_SERV_UUID                    0x06U
#define AD_TYPE_128_BIT_SERV_UUID_CMPLT_LIST         0x07U
#define AD_TYPE_SHORTENED_LOCAL_NAME                 0x08U
#define AD_TYPE_COMPLETE_LOCAL_NAME                  0x09U
#define AD_TYPE_TX_POWER_LEVEL                       0x0AU
#define AD_TYPE_CLASS_OF_DEVICE                      0x0DU
#define AD_TYPE_SEC_MGR_TK_VALUE                     0x10U
#define AD_TYPE_SEC_MGR_OOB_FLAGS                    0x11U
#define AD_TYPE_PERIPHERAL_CONN_INTERVAL             0x12U
#define AD_TYPE_SERV_SOLICIT_16_BIT_UUID_LIST        0x14U
#define AD_TYPE_SERV_SOLICIT_128_BIT_UUID_LIST       0x15U
#define AD_TYPE_SERVICE_DATA                         0x16U
#define AD_TYPE_APPEARANCE                           0x19U
#define AD_TYPE_ADVERTISING_INTERVAL                 0x1AU
#define AD_TYPE_LE_ROLE                              0x1CU
#define AD_TYPE_SERV_SOLICIT_32_BIT_UUID_LIST        0x1FU
#define AD_TYPE_URI                                  0x24U
#define AD_TYPE_MANUFACTURER_SPECIFIC_DATA           0xFFU

/* Flag bits for Flags AD Type
 */
#define FLAG_BIT_LE_LIMITED_DISCOVERABLE_MODE        0x01
#define FLAG_BIT_LE_GENERAL_DISCOVERABLE_MODE        0x02
#define FLAG_BIT_BR_EDR_NOT_SUPPORTED                0x04
#define FLAG_BIT_LE_BR_EDR_CONTROLLER                0x08
#define FLAG_BIT_LE_BR_EDR_HOST                      0x10

/* Appearance values
 */
#define GAP_APPEARANCE_UNKNOWN                                 0x0000
#define GAP_APPEARANCE_GENERIC_PHONE                           0x0040
#define GAP_APPEARANCE_GENERIC_COMPUTER                        0x0080
#define GAP_APPEARANCE_GENERIC_WATCH                           0x00C0
#define GAP_APPEARANCE_WATCH_SPORT_WATCH                       0x00C1
#define GAP_APPEARANCE_GENERIC_CLOCK                           0x0100
#define GAP_APPEARANCE_GENERIC_DISPLAY                         0x0140
#define GAP_APPEARANCE_GENERIC_REMOTE_CONTROL                  0x0180
#define GAP_APPEARANCE_GENERIC_EYE_GLASSES                     0x01C0
#define GAP_APPEARANCE_GENERIC_TAG                             0x0200
#define GAP_APPEARANCE_GENERIC_KEYRING                         0x0240
#define GAP_APPEARANCE_GENERIC_MEDIA_PLAYER                    0x0280
#define GAP_APPEARANCE_GENERIC_BARCODE_SCANNER                 0x02C0
#define GAP_APPEARANCE_GENERIC_THERMOMETER                     0x0300
#define GAP_APPEARANCE_THERMOMETER_EAR                         0x0301
#define GAP_APPEARANCE_GENERIC_HEART_RATE_SENSOR               0x0340
#define GAP_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT       0x0341
#define GAP_APPEARANCE_GENERIC_BLOOD_PRESSURE                  0x0380
#define GAP_APPEARANCE_BLOOD_PRESSURE_ARM                      0x0381
#define GAP_APPEARANCE_BLOOD_PRESSURE_WRIST                    0x0382
#define GAP_APPEARANCE_HUMAN_INTERFACE_DEVICE                  0x03C0
#define GAP_APPEARANCE_KEYBOARD                                0x03C1
#define GAP_APPEARANCE_MOUSE                                   0x03C2
#define GAP_APPEARANCE_JOYSTICK                                0x03C3
#define GAP_APPEARANCE_GAMEPAD                                 0x03C4
#define GAP_APPEARANCE_DIGITIZER_TABLET                        0x03C5
#define GAP_APPEARANCE_CARD_READER                             0x03C6
#define GAP_APPEARANCE_DIGITAL_PEN                             0x03C7
#define GAP_APPEARANCE_BARCODE_SCANNER                         0x03C8
#define GAP_APPEARANCE_GENERIC_GLUCOSE_METER                   0x0400
#define GAP_APPEARANCE_GENERIC_RUNNING_WALKING_SENSOR          0x0440
#define GAP_APPEARANCE_RUNNING_WALKING_IN_SHOE                 0x0441
#define GAP_APPEARANCE_RUNNING_WALKING_ON_SHOE                 0x0442
#define GAP_APPEARANCE_RUNNING_WALKING_ON_HIP                  0x0443
#define GAP_APPEARANCE_GENERIC_CYCLING                         0x0480
#define GAP_APPEARANCE_CYCLING_CYCLING_COMPUTER                0x0481
#define GAP_APPEARANCE_CYCLING_SPEED_SENSOR                    0x0482
#define GAP_APPEARANCE_CYCLING_CADENCE_SENSOR                  0x0483
#define GAP_APPEARANCE_CYCLING_POWER_SENSOR                    0x0484
#define GAP_APPEARANCE_CYCLING_SPEED_AND_CADENCE_SENSOR        0x0485
#define GAP_APPEARANCE_GENERIC_PULSE_OXYMETER                  0x0C40
#define GAP_APPEARANCE_FINGERTIP                               0x0C41
#define GAP_APPEARANCE_WRIST_WORN                              0x0C42
#define GAP_APPEARANCE_GENERIC_WEIGHT_SCALE                    0x0C80
#define GAP_APPEARANCE_GENERIC_OUTDOOR_SPORT_ACTIVITY          0x1440
#define GAP_APPEARANCE_LOCATION_DISPLAY_DEVICE                 0x1441
#define GAP_APPEARANCE_LOCATION_AND_NAVIGATION_DISPLAY_DEVICE  0x1442
#define GAP_APPEARANCE_LOCATION_POD                            0x1443
#define GAP_APPEARANCE_LOCATION_AND_NAVIGATION_POD             0x1444
#define GAP_APPEARANCE_GENERIC_ENVIRONMENTAL_SENSOR            0x1640

/* Privacy flag values
 */
#define PRIVACY_ENABLED                               0x02
#define PRIVACY_DISABLED                              0x00

/* Intervals in terms of 625 micro sec
 */
#define DIR_CONN_ADV_INT_MIN                        0x190U  /* 250 ms */
#define DIR_CONN_ADV_INT_MAX                        0x320U  /* 500 ms */
#define UNDIR_CONN_ADV_INT_MIN                      0x800U  /* 1.28 s */
#define UNDIR_CONN_ADV_INT_MAX                     0x1000U  /* 2.56 s */
#define LIM_DISC_ADV_INT_MIN                        0x190U  /* 250 ms */
#define LIM_DISC_ADV_INT_MAX                        0x320U  /* 500 ms */
#define GEN_DISC_ADV_INT_MIN                        0x800U  /* 1.28 s */
#define GEN_DISC_ADV_INT_MAX                       0x1000U  /* 2.56 s */

/* GAP Roles
 */
#define GAP_PERIPHERAL_ROLE                          0x01U
#define GAP_BROADCASTER_ROLE                         0x02U
#define GAP_CENTRAL_ROLE                             0x04U
#define GAP_OBSERVER_ROLE                            0x08U

/* GAP procedure codes
 * Procedure codes for ACI_GAP_PROC_COMPLETE_EVENT event
 * and ACI_GAP_TERMINATE_GAP_PROC command.
 */
#define GAP_LIMITED_DISCOVERY_PROC                   0x01U
#define GAP_GENERAL_DISCOVERY_PROC                   0x02U
#define GAP_NAME_DISCOVERY_PROC                      0x04U
#define GAP_AUTO_CONNECTION_ESTABLISHMENT_PROC       0x08U
#define GAP_GENERAL_CONNECTION_ESTABLISHMENT_PROC    0x10U
#define GAP_SELECTIVE_CONNECTION_ESTABLISHMENT_PROC  0x20U
#define GAP_DIRECT_CONNECTION_ESTABLISHMENT_PROC     0x40U
#define GAP_OBSERVATION_PROC                         0x80U

/* GAP Address Type
 */
#define GAP_PUBLIC_ADDR                              0x00U
#define GAP_STATIC_RANDOM_ADDR                       0x01U
#define GAP_RESOLVABLE_PRIVATE_ADDR                  0x02U
#define GAP_NON_RESOLVABLE_PRIVATE_ADDR              0x03U

/* Bitmap definitions for Mode of ACI_GAP_ADD_DEVICES_TO_LIST
 */
#define GAP_ADD_DEV_MODE_RESOLVING_LIST_ONLY         0x00U
#define GAP_ADD_DEV_MODE_CLEAR                       0x01U
#define GAP_ADD_DEV_MODE_FILTER_ACC_LIST_ONLY        0x02U
#define GAP_ADD_DEV_MODE_BOTH_LISTS                  0x04U 

/* ------------------------------------------------------------------------- */

/* IO capabilities
 */
#define IO_CAP_DISPLAY_ONLY                        0x00U
#define IO_CAP_DISPLAY_YES_NO                      0x01U
#define IO_CAP_KEYBOARD_ONLY                       0x02U
#define IO_CAP_NO_INPUT_NO_OUTPUT                  0x03U
#define IO_CAP_KEYBOARD_DISPLAY                    0x04U

/* Authentication requirements
 */
#define NO_BONDING                                 0x00U
#define BONDING                                    0x01U

/* MITM protection requirements
 */
#define MITM_PROTECTION_NOT_REQUIRED               0x00U
#define MITM_PROTECTION_REQUIRED                   0x01U

/* Out-Of-Band data
 */
#define OOB_AUTH_DATA_ABSENT                       0x00U
#define OOB_AUTH_DATA_PRESENT                      0x01U

/* Authorization requirements
 */
#define AUTHORIZATION_NOT_REQUIRED                 0x00U
#define AUTHORIZATION_REQUIRED                     0x01U

/* Connection authorization
 */
#define CONNECTION_AUTHORIZED                      0x01U
#define CONNECTION_REJECTED                        0x02U

/* Keypress notification support
 */
#define KEYPRESS_NOT_SUPPORTED                     0x00U
#define KEYPRESS_SUPPORTED                         0x01U

/* Use fixed pin
 */
#define USE_FIXED_PIN_FOR_PAIRING_ALLOWED          0x00U
#define USE_FIXED_PIN_FOR_PAIRING_FORBIDDEN        0x01U

/* SMP pairing status (ACI_GAP_PAIRING_COMPLETE_EVENT)
 */
#define SMP_PAIRING_STATUS_SUCCESS                 0x00U
#define SMP_PAIRING_STATUS_SMP_TIMEOUT             0x01U
#define SMP_PAIRING_STATUS_PAIRING_FAILED          0x02U
#define SMP_PAIRING_STATUS_ENCRYPT_FAILED          0x03U

/* SMP pairing failed reason code (ACI_GAP_PAIRING_COMPLETE_EVENT)
 */
#define OOB_NOT_AVAILABLE                          0x02
#define AUTH_REQ_CANNOT_BE_MET                     0x03
#define CONFIRM_VALUE_FAILED                       0x04
#define PAIRING_NOT_SUPPORTED                      0x05
#define INSUFF_ENCRYPTION_KEY_SIZE                 0x06
#define CMD_NOT_SUPPORTED                          0x07
#define UNSPECIFIED_REASON                         0x08
#define VERY_EARLY_NEXT_ATTEMPT                    0x09
#define SM_INVALID_PARAMS                          0x0A
#define SMP_SC_DHKEY_CHECK_FAILED                  0x0B
#define SMP_SC_NUMCOMPARISON_FAILED                0x0C

/* Passkey input type detected (ACI_GAP_PASSKEY_INPUT)
 */
#define PASSKEY_ENTRY_STARTED                      0x00U
#define PASSKEY_DIGIT_ENTERED                      0x01U
#define PASSKEY_DIGIT_ERASED                       0x02U
#define PASSKEY_CLEARED                            0x03U
#define PASSKEY_ENTRY_COMPLETED                    0x04U

/* Numeric Comparison Confirm Value
 * (ACI_GAP_NUMERIC_COMPARISON_VALUE_CONFIRM_YESNO)
 */
#define NUMERIC_COMPARISON_CONFIRM_NO              0x00U
#define NUMERIC_COMPARISON_CONFIRM_YES             0x01U

/* OOB Device Type (ACI_GAP_SET_OOB_DATA)
 */
#define OOB_DEVICE_TYPE_LOCAL                      0x00U
#define OOB_DEVICE_TYPE_REMOTE                     0x01U

/* OOB Data Type (ACI_GAP_GET_OOB_DATA / ACI_GAP_SET_OOB_DATA)
 */
#define OOB_DATA_TYPE_LP_TK                        0x00U
#define OOB_DATA_TYPE_SC_RANDOM                    0x01U
#define OOB_DATA_TYPE_SC_CONFIRM                   0x02U

/* ------------------------------------------------------------------------- */

/* GATT UUIDs
 */
#define GATT_SERVICE_UUID                        0x1801U
#define PRIMARY_SERVICE_UUID                     0x2800U
#define SECONDARY_SERVICE_UUID                   0x2801U
#define INCLUDE_SERVICE_UUID                     0x2802U
#define CHARACTERISTIC_UUID                      0x2803U
#define CHAR_EXTENDED_PROP_DESC_UUID             0x2900U
#define CHAR_USER_DESC_UUID                      0x2901U
#define CHAR_CLIENT_CONFIG_DESC_UUID             0x2902U
#define CHAR_SERVER_CONFIG_DESC_UUID             0x2903U
#define CHAR_FORMAT_DESC_UUID                    0x2904U
#define CHAR_AGGR_FMT_DESC_UUID                  0x2905U
#define SERVICE_CHANGED_UUID                     0x2A05U
#define CLIENT_SUPPORTED_FEATURES_UUID           0X2B29U
#define DATABASE_HASH_UUID                       0X2B2AU
#define SERVER_SUPPORTED_FEATURES_UUID           0X2B3AU

/* GAP UUIDs
 */
#define GAP_SERVICE_UUID                         0x1800U
#define DEVICE_NAME_UUID                         0x2A00U
#define APPEARANCE_UUID                          0x2A01U
#define PERIPHERAL_PREFERRED_CONN_PARAMS_UUID    0x2A04U
#define CENTRAL_ADDRESS_RESOLUTION_UUID          0x2AA6U
#define RESOLVABLE_PRIVATE_ADDRESS_ONLY_UUID     0x2AC9U
#define ENCRYPTED_DATA_KEY_MATERIAL_UUID         0x2B88U
#define LE_GATT_SECURITY_LEVELS_UUID             0x2BF5U

/* Access permissions for an attribute
 */
#define ATTR_NO_ACCESS                           0x00U
#define ATTR_ACCESS_READ_ONLY                    0x01U
#define ATTR_ACCESS_WRITE_REQ_ONLY               0x02U
#define ATTR_ACCESS_READ_WRITE                   0x03U
#define ATTR_ACCESS_WRITE_WITHOUT_RESPONSE       0x04U
#define ATTR_ACCESS_SIGNED_WRITE_ALLOWED         0x08U
#define ATTR_ACCESS_WRITE_ANY                    0x0EU
#define ATTR_ACCESS_ANY                          0x0FU

/* Characteristic properties
 */
#define CHAR_PROP_NONE                           0x00U
#define CHAR_PROP_BROADCAST                      0x01U
#define CHAR_PROP_READ                           0x02U
#define CHAR_PROP_WRITE_WITHOUT_RESP             0x04U
#define CHAR_PROP_WRITE                          0x08U
#define CHAR_PROP_NOTIFY                         0x10u
#define CHAR_PROP_INDICATE                       0x20U
#define CHAR_PROP_SIGNED_WRITE                   0x40U
#define CHAR_PROP_EXT                            0x80U

/* Security permissions for an attribute
 */
#define ATTR_PERMISSION_NONE           0x00U /* No security. */
#define ATTR_PERMISSION_AUTHEN_READ    0x01U /* Need authentication to read */
#define ATTR_PERMISSION_AUTHOR_READ    0x02U /* Need authorization to read */
#define ATTR_PERMISSION_ENCRY_READ     0x04U /* Need encryption to read */
#define ATTR_PERMISSION_AUTHEN_WRITE   0x08U /* Need authentication to write */
#define ATTR_PERMISSION_AUTHOR_WRITE   0x10U /* Need authorization to write */
#define ATTR_PERMISSION_ENCRY_WRITE    0x20U /* Need encryption to write */

/* Type of UUID (16 bit or 128 bit)
 */
#define UUID_TYPE_16                             0x01U
#define UUID_TYPE_128                            0x02U

/* Type of service (primary or secondary)
 */
#define PRIMARY_SERVICE                          0x01U
#define SECONDARY_SERVICE                        0x02U

/* Gatt Event Mask
 * Type of event generated by GATT server
 * See aci_gatt_add_char.
 */
#define GATT_DONT_NOTIFY_EVENTS                       0x00U
#define GATT_NOTIFY_ATTRIBUTE_WRITE                   0x01U
#define GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP  0x02U
#define GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP   0x04U
#define GATT_NOTIFY_NOTIFICATION_COMPLETION           0x08U

/* Type of characteristic length (see ACI_GATT_ADD_CHAR)
 */
#define CHAR_VALUE_LEN_CONSTANT                  0x00
#define CHAR_VALUE_LEN_VARIABLE                  0x01

/* Encryption key size
 */
#define MIN_ENCRY_KEY_SIZE                       7U
#define MAX_ENCRY_KEY_SIZE                      16U

/* Format
 */
#define FORMAT_UINT8                             0x04U
#define FORMAT_UINT16                            0x06U
#define FORMAT_SINT16                            0x0EU
#define FORMAT_SINT24                            0x0FU

/* Unit
 */
#define UNIT_UNITLESS                          0x2700
#define UNIT_TEMP_CELSIUS                      0x272F
#define UNIT_PRESSURE_BAR                      0x2780

/* Update_Type definitions for ACI_GATT_UPDATE_CHAR_VALUE_EXT
 */
#define GATT_CHAR_UPDATE_LOCAL_ONLY                   0x00U
#define GATT_CHAR_UPDATE_SEND_NOTIFICATION            0x01U
#define GATT_CHAR_UPDATE_SEND_INDICATION              0x02U

/* ------------------------------------------------------------------------- */

/* Advertising Type
 */
#define ADV_IND                                        0
#define ADV_DIRECT_IND                                 1
#define ADV_SCAN_IND                                   2
#define ADV_NONCONN_IND                                3
#define ADV_DIRECT_IND_LDC                             4
#define SCAN_RSP                                       4

/* Advertising channels
 */
#define ADV_CH_37                              0x01
#define ADV_CH_38                              0x02
#define ADV_CH_39                              0x04

/* ------------------------------------------------------------------------- */

/* Offset for configuration values (see ACI_HAL_WRITE_CONFIG_DATA)
 */
#define CONFIG_DATA_PUBADDR_OFFSET             0x00U
#define CONFIG_DATA_ER_OFFSET                  0x08U
#define CONFIG_DATA_IR_OFFSET                  0x18U
#define CONFIG_DATA_RANDOM_ADDRESS_OFFSET      0x2EU
#define CONFIG_DATA_GAP_ADD_REC_NBR_OFFSET     0x34U
#define CONFIG_DATA_SC_KEY_TYPE_OFFSET         0x35U
#define CONFIG_DATA_SMP_MODE_OFFSET            0xB0U
#define CONFIG_DATA_LL_SCAN_CHAN_MAP_OFFSET    0xC0U
#define CONFIG_DATA_LL_BG_SCAN_MODE_OFFSET     0xC1U

/* Length for configuration values (see ACI_HAL_WRITE_CONFIG_DATA)
 */
#define CONFIG_DATA_PUBADDR_LEN                6
#define CONFIG_DATA_ER_LEN                    16
#define CONFIG_DATA_IR_LEN                    16
#define CONFIG_DATA_RANDOM_ADDRESS_LEN         6
#define CONFIG_DATA_GAP_ADD_REC_NBR_LEN        1
#define CONFIG_DATA_SC_KEY_TYPE_LEN            1
#define CONFIG_DATA_SMP_MODE_LEN               1
#define CONFIG_DATA_LL_SCAN_CHAN_MAP_LEN       1
#define CONFIG_DATA_LL_BG_SCAN_MODE_LEN        1

/* ------------------------------------------------------------------------- */


#endif /* BLE_DEFS_H__ */
