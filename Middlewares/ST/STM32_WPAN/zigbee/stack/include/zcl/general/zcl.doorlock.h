/**
 * @file zcl.doorlock.h
 * @heading Door Lock
 * @brief ZCL Door Lock cluster header
 * ZCL 7 section 7.3
 * ZCL 8 section 7.3
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_DRLK_H
#define ZCL_DRLK_H

#include "zcl/zcl.h"

/* @PICS.ZCL.Doorlock
 * DRLK.S | Server | True
 * DRLK.C | Client | True
 *
 * Server Attributes
 * DRLK.S.A0000 | LockState | True
 * DRLK.S.A0001 | LockType | True
 * DRLK.S.A0002 | ActuatorEnabled | True
 * DRLK.S.A0003 | DoorState | True | Optional
 * DRLK.S.A0004 | DoorOpenEvents | True | Optional
 * DRLK.S.A0005 | DoorClosedEvents | True | Optional
 * DRLK.S.A0006 | OpenPeriod | True | Optional
 * DRLK.S.A0010 | NumberOfLogRecordsSupported | True | Optional
 * DRLK.S.A0011 | NumberOfTotalUsersSupported | True | Optional
 * DRLK.S.A0012 | NumberOfPINUsersSupported | True | Optional
 * DRLK.S.A0013 | NumberOfRFIDUsersSupported | True | Optional
 * DRLK.S.A0014 | NumberOfWeekDaySchedulesSupportedPerUser | True | Optional
 * DRLK.S.A0015 | NumberOfYearDaySchedulesSupportedPerUser | True | Optional
 * DRLK.S.A0016 | NumberOfHolidaySchedulesSupported | True | Optional
 * DRLK.S.A0017 | MaxPINCodeLength | True | Optional
 * DRLK.S.A0018 | MinPINCodeLength | True | Optional
 * DRLK.S.A0019 | MaxRFIDCodeLength | True | Optional
 * DRLK.S.A001a | MinRFIDCodeLength | True | Optional
 * DRLK.S.A0020 | EnableLogging | False | Optional
 * DRLK.S.A0021 | Language | True | Optional
 * DRLK.S.A0022 | LEDSettings | False | Optional
 * DRLK.S.A0023 | AutoRelockTime | True | Optional
 * DRLK.S.A0024 | SoundVolume | False | Optional
 * DRLK.S.A0025 | OperatingMode | True | Optional
 * DRLK.S.A0026 | SupportedOperatingModes | True | Optional
 * DRLK.S.A0027 | DefaultConfigurationRegister | False | Optional
 * DRLK.S.A0028 | EnableLocalProgramming | True | Optional
 * DRLK.S.A0029 | EnableOneTouchLocking | False | Optional
 * DRLK.S.A002a | EnableInsideStatusLED | False | Optional
 * DRLK.S.A002b | EnablePrivacyModeButton | False | Optional
 * DRLK.S.A0030 | WrongCodeEntryLimit | True | Optional
 * DRLK.S.A0031 | UserCodeTemporaryDisableTime | True | Optional
 * DRLK.S.A0032 | SendPINOverTheAir | False | Optional
 * DRLK.S.A0033 | RequirePINforRFOperation | True | Optional
 * DRLK.S.A0034 | SecurityLevel | True | Optional
 * DRLK.S.A0040 | AlarmMask | True | Optional
 * DRLK.S.A0041 | KeypadOperationEventMask | False | Optional
 * DRLK.S.A0042 | RFOperationEventMask | False | Optional
 * DRLK.S.A0043 | ManualOperationEventMask | False | Optional
 * DRLK.S.A0044 | RFIDOperationEventMask | False | Optional
 * DRLK.S.A0045 | KeypadProgrammingEventMask | False | Optional
 * DRLK.S.A0046 | RFProgrammingEventMask | False | Optional
 * DRLK.S.A0047 | RFIDProgrammingEventMask | False | Optional
 * DRLK.S.Afffd | ClusterRevision | True
 * DRLK.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * DRLK.S.C00.Rsp | Lock Door | True
 * DRLK.S.C01.Rsp | Unlock Door | True
 * DRLK.S.C02.Rsp | Toggle | True
 * DRLK.S.C03.Rsp | Unlock with Timeout | False
 * DRLK.S.C04.Rsp | Get Log Record | False
 * DRLK.S.C05.Rsp | Set PIN Code | False
 * DRLK.S.C06.Rsp | Get PIN Code | False
 * DRLK.S.C07.Rsp | Clear PIN Code | False
 * DRLK.S.C08.Rsp | Clear All PIN Codes | False
 * DRLK.S.C09.Rsp | Set User Status | False
 * DRLK.S.C0a.Rsp | Get User Status | False
 * DRLK.S.C0b.Rsp | Set Weekday Schedule | False
 * DRLK.S.C0c.Rsp | Get Weekday Schedule | False
 * DRLK.S.C0d.Rsp | Clear Weekday Schedule | False
 * DRLK.S.C0e.Rsp | Set Year Day Schedule | False
 * DRLK.S.C0f.Rsp | Get Year Day Schedule | False
 * DRLK.S.C10.Rsp | Clear Year Day Schedule | False
 * DRLK.S.C11.Rsp | Set Holiday Schedule | False
 * DRLK.S.C12.Rsp | Get Holiday Schedule | False
 * DRLK.S.C13.Rsp | Clear Holiday Schedule | False
 * DRLK.S.C14.Rsp | Set User Type | False
 * DRLK.S.C15.Rsp | Get User Type | False
 * DRLK.S.C16.Rsp | Set RFID Code | False
 * DRLK.S.C17.Rsp | Get RFID Code | False
 * DRLK.S.C18.Rsp | Clear RFID Code | False
 * DRLK.S.C19.Rsp | Clear All RFID Codes | False
 *
 * Commands Generated
 * DRLK.S.C00.Tx | Lock Door Response | True
 * DRLK.S.C01.Tx | Unlock Door Response | True
 * DRLK.S.C02.Tx | Toggle Response | True
 * DRLK.S.C03.Tx | Unlock with Timeout Response | False
 * DRLK.S.C04.Tx | Get Log Record Response | False
 * DRLK.S.C05.Tx | Set PIN Code Response | False
 * DRLK.S.C06.Tx | Get PIN Code Response | False
 * DRLK.S.C07.Tx | Clear PIN Code Response | False
 * DRLK.S.C08.Tx | Clear All PIN Codes Response | False
 * DRLK.S.C09.Tx | Set User Status Response | False
 * DRLK.S.C0a.Tx | Get User Status Response | False
 * DRLK.S.C0b.Tx | Set Weekday Schedule Response | False
 * DRLK.S.C0c.Tx | Get Weekday Schedule Response | False
 * DRLK.S.C0d.Tx | Clear Weekday Schedule Response | False
 * DRLK.S.C0e.Tx | Set Year Day Schedule Response | False
 * DRLK.S.C0f.Tx | Get Year Day Schedule Response | False
 * DRLK.S.C10.Tx | Clear Year Day Schedule Response | False
 * DRLK.S.C11.Tx | Set Holiday Schedule Response | False
 * DRLK.S.C12.Tx | Get Holiday Schedule Response | False
 * DRLK.S.C13.Tx | Clear Holiday Schedule Response | False
 * DRLK.S.C14.Tx | Set User Type Response | False
 * DRLK.S.C15.Tx | Get User Type Response | False
 * DRLK.S.C16.Tx | Set RFID Code Response | False
 * DRLK.S.C17.Tx | Get RFID Code Response | False
 * DRLK.S.C18.Tx | Clear RFID Code Response | False
 * DRLK.S.C19.Tx | Clear All RFID Codes Response | False
 * DRLK.S.C20.Tx | Operating Event Notification | False
 * DRLK.S.C21.Tx | Programming Event Notification | False
 *
 *
 * Client Attributes
 * DRLK.C.Afffd | ClusterRevision | True
 * DRLK.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * DRLK.C.C00.Rsp | Lock Door Response | True
 * DRLK.C.C01.Rsp | Unlock Door Response | True
 * DRLK.C.C02.Rsp | Toggle Response | True
 * DRLK.C.C03.Rsp | Unlock with Timeout Response | False
 * DRLK.C.C04.Rsp | Get Log Record Response | False
 * DRLK.C.C05.Rsp | Set PIN Code Response | False
 * DRLK.C.C06.Rsp | Get PIN Code Response | False
 * DRLK.C.C07.Rsp | Clear PIN Code Response | False
 * DRLK.C.C08.Rsp | Clear All PIN Codes Response | False
 * DRLK.C.C09.Rsp | Set User Status Response | False
 * DRLK.C.C0a.Rsp | Get User Status Response | False
 * DRLK.C.C0b.Rsp | Set Weekday Schedule Response | False
 * DRLK.C.C0c.Rsp | Get Weekday Schedule Response | False
 * DRLK.C.C0d.Rsp | Clear Weekday Schedule Response | False
 * DRLK.C.C0e.Rsp | Set Year Day Schedule Response | False
 * DRLK.C.C0f.Rsp | Get Year Day Schedule Response | False
 * DRLK.C.C10.Rsp | Clear Year Day Schedule Response | False
 * DRLK.C.C11.Rsp | Set Holiday Schedule Response | False
 * DRLK.C.C12.Rsp | Get Holiday Schedule Response | False
 * DRLK.C.C13.Rsp | Clear Holiday Schedule Response | False
 * DRLK.C.C14.Rsp | Set User Type Response | False
 * DRLK.C.C15.Rsp | Get User Type Response | False
 * DRLK.C.C16.Rsp | Set RFID Code Response | False
 * DRLK.C.C17.Rsp | Get RFID Code Response | False
 * DRLK.C.C18.Rsp | Clear RFID Code Response | False
 * DRLK.C.C19.Rsp | Clear All RFID Codes Response | False
 * DRLK.C.C20.Rsp | Operating Event Notification | False
 * DRLK.C.C21.Rsp | Programming Event Notification | False
 *
 * Commands Generated
 * DRLK.C.C00.Tx | Lock Door | True
 * DRLK.C.C01.Tx | Unlock Door | True
 * DRLK.C.C02.Tx | Toggle | True
 * DRLK.C.C03.Tx | Unlock with Timeout | False
 * DRLK.C.C04.Tx | Get Log Record  | False
 * DRLK.C.C05.Tx | Set PIN Code | False
 * DRLK.C.C06.Tx | Get PIN Code | False
 * DRLK.C.C07.Tx | Clear PIN Code | False
 * DRLK.C.C08.Tx | Clear All PIN Codes | False
 * DRLK.C.C09.Tx | Set User Status | False
 * DRLK.C.C0a.Tx | Get User Status  | False
 * DRLK.C.C0b.Tx | Set Weekday Schedule | False
 * DRLK.C.C0c.Tx | Get Weekday Schedule | False
 * DRLK.C.C0d.Tx | Clear Weekday Schedule | False
 * DRLK.C.C0e.Tx | Set Year Day Schedule | False
 * DRLK.C.C0f.Tx | Get Year Day Schedule | False
 * DRLK.C.C10.Tx | Clear Year Day Schedule | False
 * DRLK.C.C11.Tx | Set Holiday Schedule | False
 * DRLK.C.C12.Tx | Get Holiday Schedule | False
 * DRLK.C.C13.Tx | Clear Holiday Schedule | False
 * DRLK.C.C14.Tx | Set User Type | False
 * DRLK.C.C15.Tx | Get User Type | False
 * DRLK.C.C16.Tx | Get RFID Code |False
 * DRLK.C.C17.Tx | Get RFID Code | False
 * DRLK.C.C18.Tx | Clear RFID Code | False
 * DRLK.C.C19.Tx | Clear All RFID Codes | False
 */

/** Door Lock Server Attribute IDs */
enum {
    ZCL_DRLK_ATTR_LOCKSTATE = 0x0000, /**< LockState */
    ZCL_DRLK_ATTR_LOCKTYPE = 0x0001, /**< LockType */
    ZCL_DRLK_ATTR_ACT_ENABLED = 0x0002, /**< ActuatorEnabled */
    ZCL_DRLK_ATTR_DOORSTATE = 0x0003, /**< DoorState (Optional) */
    ZCL_DRLK_ATTR_OPENEVENTS = 0x0004, /**< DoorOpenEvents (Optional) */
    ZCL_DRLK_ATTR_CLOSEDEVENTS = 0x0005, /**< DoorClosedEvents (Optional) */
    ZCL_DRLK_ATTR_OPENPERIOD = 0x0006, /**< OpenPeriod (Optional) */
    /* User/PIN/Schedule/Log attributes */
    ZCL_DRLK_ATTR_NUM_LOGRECORDS = 0x0010, /**< NumberOfLogRecordsSupported (Optional) */
    ZCL_DRLK_ATTR_NUM_TOTALUSERS = 0x0011, /**< NumberOfTotalUsersSupported (Optional) */
    ZCL_DRLK_ATTR_NUM_PINUSERS = 0x0012, /**< NumberOfPINUsersSupported (Optional) */
    ZCL_DRLK_ATTR_NUM_RFIDUSERS = 0x0013, /**< NumberOfRFIDUsersSupported (Optional) */
    ZCL_DRLK_ATTR_NUM_WD_SCHEDULES = 0x0014, /**< NumberOfWeekDaySchedulesSupportedPerUser (Optional) */
    ZCL_DRLK_ATTR_NUM_YD_SCHEDULES = 0x0015, /**< NumberOfYearDaySchedulesSupportedPerUser (Optional) */
    ZCL_DRLK_ATTR_NUM_HD_SCHEDULES = 0x0016, /**< NumberOfHolidaySchedulesSupported (Optional) */
    ZCL_DRLK_ATTR_MAX_PIN_LEN = 0x0017, /**< MaxPINCodeLength (Optional) */
    ZCL_DRLK_ATTR_MIN_PIN_LEN = 0x0018, /**< MinPINCodeLength (Optional) */
    ZCL_DRLK_ATTR_MAX_RFID_LEN = 0x0019, /**< MaxRFIDCodeLength (Optional) */
    ZCL_DRLK_ATTR_MIN_RFID_LEN = 0x001A, /**< MinRFIDCodeLength (Optional) */
    /* Operational Settings */
    ZCL_DRLK_ATTR_LOGGING = 0x0020, /**< EnableLogging (Optional) */
    ZCL_DRLK_ATTR_LANGUAGE = 0x0021, /**< Language (Optional) */
    ZCL_DRLK_ATTR_LED_SETTINGS = 0x0022, /**< LEDSettings (Optional) */
    ZCL_DRLK_ATTR_AUTO_RELOCK = 0x0023, /**< AutoRelockTime (Optional) */
    ZCL_DRLK_ATTR_VOLUME = 0x0024, /**< SoundVolume (Optional) */
    ZCL_DRLK_ATTR_MODE = 0x0025, /**< OperatingMode (Optional) */
    ZCL_DRLK_ATTR_SUPPORTED_MODES = 0x0026, /**< SupportedOperatingModes (Optional) */
    ZCL_DRLK_ATTR_DEF_CFG = 0x0027, /**< DefaultConfigurationRegister (Optional) */
    ZCL_DRLK_ATTR_LOCAL_PROG = 0x0028, /**< EnableLocalProgramming (Optional) */
    ZCL_DRLK_ATTR_ONETOUCH_LOCK = 0x0029, /**< EnableOneTouchLocking (Optional) */
    ZCL_DRLK_ATTR_INSIDE_STATUS = 0x002A, /**< EnableInsideStatusLED (Optional) */
    ZCL_DRLK_ATTR_PRIV_BUTTON = 0x002B, /**< EnablePrivacyModeButton (Optional) */
    /* Security Settings */
    ZCL_DRLK_ATTR_WRONGCODE_LIMIT = 0x0030, /**< WrongCodeEntryLimit (Optional) */
    ZCL_DRLK_ATTR_TEMP_DISABLE = 0x0031, /**< UserCodeTemporaryDisableTime (Optional) */
    ZCL_DRLK_ATTR_PIN_OTA = 0x0032, /**< SendPINOverTheAir (Optional) */
    ZCL_DRLK_ATTR_PIN_FOR_RF = 0x0033, /**< RequirePINforRFOperation (Optional) */
    ZCL_DRLK_ATTR_SEC_LEVEL = 0x0034, /**< SecurityLevel (Optional) */
    /* Alarms and Event masks */
    ZCL_DRLK_ATTR_ALARM_MASK = 0x0040, /**< AlarmMask (Optional) */
    ZCL_DRLK_ATTR_KEYPAD_OP_EVENT_MASK = 0x0041, /**< KeypadOperationEventMask (Optional) */
    ZCL_DRLK_ATTR_RF_OP_EVENT_MASK = 0x0042, /**< RFOperationEventMask (Optional) */
    ZCL_DRLK_ATTR_MANUAL_OP_EVENT_MASK = 0x0043, /**< ManualOperationEventMask (Optional) */
    ZCL_DRLK_ATTR_RFID_EVENT_MASK = 0x0044, /**< RFIDOperationEventMask (Optional) */
    ZCL_DRLK_ATTR_KEYPAD_EVENT_MASK = 0x0045, /**< KeypadProgrammingEventMask (Optional) */
    ZCL_DRLK_ATTR_RF_PROG_EVENT_MASK = 0x0046, /**< RFProgrammingEventMask (Optional) */
    ZCL_DRLK_ATTR_RFID_PROG_EVENT_MASK = 0x0047 /**< RFIDProgrammingEventMask (Optional) */
};

/* Default Values */
#define ZCL_DRLK_LOGRECORDS_DEFAULT                 0
#define ZCL_DRLK_TOTALUSERS_DEFAULT                 0
#define ZCL_DRLK_PINUSERS_DEFAULT                   0
#define ZCL_DRLK_RFIDUSERS_DEFAULT                  0
#define ZCL_DRLK_WD_SCHEDULES_DEFAULT               0
#define ZCL_DRLK_YD_SCHEDULES_DEFAULT               0
#define ZCL_DRLK_HD_SCHEDULES_DEFAULT               0
#define ZCL_DRLK_AUTO_RELOCK_DEFAULT                0
#define ZCL_DRLK_MODE_DEFAULT                       0
#define ZCL_DRLK_BADCODE_LIMIT_DEFAULT              0
#define ZCL_DRLK_TEMP_DISABLE_DEFAULT               0
#define ZCL_DRLK_PIN_FOR_RF_DEFAULT                 0
#define ZCL_DRLK_SEC_LEVEL_DEFAULT                  0
#define ZCL_DRLK_ALARM_DEFAULT                      0

/* LOCKSTATE attribute values */
#define ZCL_DRLK_LOCKSTATE_NOTFULLYLOCKED           0
#define ZCL_DRLK_LOCKSTATE_LOCKED                   1
#define ZCL_DRLK_LOCKSTATE_UNLOCKED                 2
#define ZCL_DRLK_LOCKSTATE_UNDEFINED                0xFF

/* LOCKTYPE attributes... many of these set yours accordingly. */
#define ZCL_DRLK_LOCKTYPE                           0x00

/* ACT_ENABLED attribute values */
#define ZCL_DRLK_ACT_ENABLED_DISABLED               0
#define ZCL_DRLK_ACT_ENABLED_ENABLED                1

/* ZCL_DRLK_ATTR_DOORSTATE */
#define ZCL_DRLK_DOORSTATE_OPEN                     0x00U
#define ZCL_DRLK_DOORSTATE_CLOSED                   0x01U
#define ZCL_DRLK_DOORSTATE_ERROR_JAMMED             0x02U
#define ZCL_DRLK_DOORSTATE_ERROR_FORCED             0x03U
#define ZCL_DRLK_DOORSTATE_ERROR_UNKNOWN            0x04U

/* ZCL_DRLK_ATTR_MODE */
#define ZCL_DRLK_MODE_NORMAL                        0x00U
#define ZCL_DRLK_MODE_VACATION                      0x01U
#define ZCL_DRLK_MODE_PRIVACY                       0x02U
#define ZCL_DRLK_MODE_NO_RF_LOCK                    0x03U
#define ZCL_DRLK_MODE_PASSAGE                       0x04U

/* ZCL_DRLK_ATTR_ALARM_MASK */
#define ZCL_DRLK_ALARM_CODE_DEADBOLT_JAMMED         0x0001U
#define ZCL_DRLK_ALARM_CODE_LOCK_RESET              0x0002U
#define ZCL_DRLK_ALARM_CODE_RF_POWER_CYCLED         0x0008U
#define ZCL_DRLK_ALARM_CODE_TAMPER_WRONG            0x0010U
#define ZCL_DRLK_ALARM_CODE_TAMPER_REMOVED          0x0020U
#define ZCL_DRLK_ALARM_CODE_FORCED_DOOR             0x0040U
#define ZCL_DRLK_ALARM_CODE_MASK                    0x007bU

/* Min and Max Values */
#define ZCL_DRLK_MAX_PIN_LEN                        0x08
#define ZCL_DRLK_MIN_PIN_LEN                        0x04
#define ZCL_DRLK_MAX_RFID_LEN                       0x14
#define ZCL_DRLK_MIN_RFID_LEN                       0x08

/* User Status Values */
#define ZCL_DRLK_USER_STATUS_AVAILABLE              0x00U
#define ZCL_DRLK_USER_STATUS_OCC_ENABLED            0x01U
#define ZCL_DRLK_USER_STATUS_OCC_DISABLED           0x03U
#define ZCL_DRLK_USER_STATUS_NOT_SUPP               0xffU

/* User Type Values */
#define ZCL_DRLK_USER_TYPE_UNRESTRICTED             0x00U
#define ZCL_DRLK_USER_TYPE_YD_SCHEDULE              0x01U
#define ZCL_DRLK_USER_TYPE_WD_SCHEDULE              0x02U
#define ZCL_DRLK_USER_TYPE_MASTER                   0x03U
#define ZCL_DRLK_USER_TYPE_NON_ACCESS               0x04U
#define ZCL_DRLK_USER_TYPE_NOT_SUPPORTED            0xffU

/* Response Status Values */
#define ZCL_DRLK_STATUS_SUCCESS                     0U
#define ZCL_DRLK_STATUS_FAIL                        1U
#define ZCL_DRLK_STATUS_MEM_FULL                    2U /* Only used with Set PIN/RFID Code Response Commands */
#define ZCL_DRLK_STATUS_DUPLICATE                   3U /* Only used with Set PIN/RFID Code Response Commands */

/* Schedule Values */
#define ZCL_DRLK_HOURS_LIMIT                        0x17U
#define ZCL_DRLK_MINUTES_LIMIT                      0x3BU

/* Security Settings */
#define ZCL_DRLK_PIN_REQUIRED                       1U
#define ZCL_DRLK_PIN_NOT_REQUIRED                   0U

/* Door Lock Client Generated Commands */
enum {
    ZCL_DRLK_CLI_LOCK = 0x00, /* Lock Door */
    ZCL_DRLK_CLI_UNLOCK = 0x01, /* Unlock Door */
    ZCL_DRLK_CLI_TOGGLE = 0x02, /* Toggle (Optional) */
    ZCL_DRLK_CLI_UNLOCK_TIMEOUT = 0x03, /* Unlock with Timeout (Optional) */
    ZCL_DRLK_CLI_GET_LOG = 0x04, /* Get Log Record (Optional) */
    ZCL_DRLK_CLI_SETPIN = 0x05, /* Set PIN Code (Optional) */
    ZCL_DRLK_CLI_GETPIN = 0x06, /* Get PIN Code (Optional) */
    ZCL_DRLK_CLI_CLRPIN = 0x07, /* Clear PIN Code (Optional) */
    ZCL_DRLK_CLI_CLR_ALL_PINS = 0x08, /* Clear All PIN Codes (Optional) */
    ZCL_DRLK_CLI_SETUSER_STATUS = 0x09, /* Set User Status (Optional) */
    ZCL_DRLK_CLI_GETUSER_STATUS = 0x0a, /* Get User Status (Optional) */
    ZCL_DRLK_CLI_SETWD_SCHED = 0x0b, /* Set Weekday Schedule (Optional) */
    ZCL_DRLK_CLI_GETWD_SCHED = 0x0c, /* Get Weekday Schedule (Optional) */
    ZCL_DRLK_CLI_CLRWD_SCHED = 0x0d, /* Clear Weekday Schedule (Optional) */
    ZCL_DRLK_CLI_SETYD_SCHED = 0x0e, /* Set Year Day Schedule (Optional) */
    ZCL_DRLK_CLI_GETYD_SCHED = 0x0f, /* Get Year Day Schedule (Optional) */
    ZCL_DRLK_CLI_CLRYD_SCHED = 0x10, /* Clear Year Day Schedule (Optional) */
    ZCL_DRLK_CLI_SETHD_SCHED = 0x11, /* Set Holiday Schedule (Optional) */
    ZCL_DRLK_CLI_GETHD_SCHED = 0x12, /* Get Holiday Schedule (Optional) */
    ZCL_DRLK_CLI_CLRHD_SCHED = 0x13, /* Clear Holiday Schedule (Optional) */
    ZCL_DRLK_CLI_SET_USERTYPE = 0x14, /* Set User Type (Optional) */
    ZCL_DRLK_CLI_GET_USERTYPE = 0x15, /* Get User Type (Optional) */
    ZCL_DRLK_CLI_SET_RFID = 0x16, /* Set RFID Code (Optional) */
    ZCL_DRLK_CLI_GET_RFID = 0x17, /* Get RFID Code (Optional) */
    ZCL_DRLK_CLI_CLR_RFID = 0x18, /* Clear RFID Code (Optional) */
    ZCL_DRLK_CLI_CLR_ALL_RFIDS = 0x19 /* Clear All RFID Codes (Optional) */
};

/* Door Lock Server Generated Commands */
enum {
    ZCL_DRLK_SVR_LOCK_RSP = 0x00, /* Lock Door Response */
    ZCL_DRLK_SVR_UNLOCK_RSP = 0x01, /* Unlock Door Response */
    ZCL_DRLK_SVR_TOGGLE_RSP = 0x02, /* Toggle Response (Optional) */
    ZCL_DRLK_SVR_UNLOCK_TO_RSP = 0x03, /* Unlock with Timeout Response (Optional) */
    ZCL_DRLK_SVR_GET_LOG_RSP = 0x04, /* Get Log Record Response (Optional) */
    ZCL_DRLK_SVR_SETPIN_RSP = 0x05, /* Set PIN Code Response (Optional) */
    ZCL_DRLK_SVR_GETPIN_RSP = 0x06, /* Get PIN Code Response (Optional) */
    ZCL_DRLK_SVR_CLRPIN_RSP = 0x07, /* Clear PIN Code Response (Optional) */
    ZCL_DRLK_SVR_CLR_ALL_PINS_RSP = 0x08, /* Clear All PIN Codes Response (Optional) */
    ZCL_DRLK_SVR_SETUSER_STATUS_RSP = 0x09, /* Set User Status Response (Optional) */
    ZCL_DRLK_SVR_GETUSER_STATUS_RSP = 0x0a, /* Get User Status Response (Optional) */
    ZCL_DRLK_SVR_SETWD_SCHED_RSP = 0x0b, /* Set Weekday Schedule Response (Optional) */
    ZCL_DRLK_SVR_GETWD_SCHED_RSP = 0x0c, /* Get Weekday Schedule Response (Optional) */
    ZCL_DRLK_SVR_CLRWD_SCHED_RSP = 0x0d, /* Clear Weekday Schedule Response (Optional) */
    ZCL_DRLK_SVR_SETYD_SCHED_RSP = 0x0e, /* Set Year Day Schedule Response (Optional) */
    ZCL_DRLK_SVR_GETYD_SCHED_RSP = 0x0f, /* Get Year Day Schedule Response (Optional) */
    ZCL_DRLK_SVR_CLRYD_SCHED_RSP = 0x10, /* Clear Year Day Schedule Response (Optional) */
    ZCL_DRLK_SVR_SETHD_SCHED_RSP = 0x11, /* Set Holiday Schedule Response (Optional) */
    ZCL_DRLK_SVR_GETHD_SCHED_RSP = 0x12, /* Get Holiday Schedule Response (Optional) */
    ZCL_DRLK_SVR_CLRHD_SCHED_RSP = 0x13, /* Clear Holiday Schedule Response (Optional) */
    ZCL_DRLK_SVR_SET_USERTYPE_RSP = 0x14, /* Set User Type Response (Optional) */
    ZCL_DRLK_SVR_GET_USERTYPE_RSP = 0x15, /* Get User Type Response (Optional) */
    ZCL_DRLK_SVR_SET_RFID_RSP = 0x16, /* Set RFID Code Response (Optional) */
    ZCL_DRLK_SVR_GET_RFID_RSP = 0x17, /* Get RFID Code Response (Optional) */
    ZCL_DRLK_SVR_CLR_RFID_RSP = 0x18, /* Clear RFID Code Response (Optional) */
    ZCL_DRLK_SVR_CLR_ALL_RFIDS_RSP = 0x19, /* Clear All RFID Codes Response (Optional) */
    /* Notifications */
    ZCL_DRLK_SVR_OPER_EVENT_NOTIFY = 0x20, /* Operating Event Notification (Optional) */
    ZCL_DRLK_SVR_PROG_EVENT_NOTIFY = 0x21 /* Programming Event Notification (Optional) */
};

/**
 * Create a new instance of the Door Lock Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDoorLockClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/*---------------------------------------------------------------
 * Client Lock/Unlock Helper
 *---------------------------------------------------------------
 */

/** Lock Door request structure */
struct ZbZclDoorLockLockDoorReqT {
    uint8_t pin[ZCL_DRLK_MAX_PIN_LEN]; /**< PIN/RFID Code */
    uint8_t pin_len; /**< Length of PIN/RFID Code */
};

/**
 * Send a Lock Door request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Lock Door request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientLockReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockLockDoorReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/** Lock Door response structure */
struct ZbZclDoorLockLockDoorRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Lock Door response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Lock Door response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendLockRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockLockDoorRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/** Unlock Door request structure */
struct ZbZclDoorLockUnlockDoorReqT {
    uint8_t pin[ZCL_DRLK_MAX_PIN_LEN]; /**< PIN/RFID Code */
    uint8_t pin_len; /**< Length of PIN/RFID Code */
};

/**
 * Send an Unlock Door request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Unlock Door request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientUnlockReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockUnlockDoorReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/** Unlock Door response structure */
struct ZbZclDoorLockUnlockDoorRspT {
    uint8_t status; /**< Status */
};

/**
 * Send an Unlock Door response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Unlock Door response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendUnlockRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockUnlockDoorRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/** Toggle request structure */
struct ZbZclDoorLockToggleReqT {
    uint8_t pin[ZCL_DRLK_MAX_PIN_LEN]; /**< PIN/RFID Code */
    uint8_t pin_len; /**< Length of PIN/RFID Code */
};

/**
 * Send Toggle request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Toggle request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientToggleReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockToggleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/** Toggle response structure */
struct ZbZclDoorLockToggleRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Toggle response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Toggle response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendToggleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockToggleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/** Unlock with Timeout request structure */
struct ZbZclDoorLockUnlockTimeoutReqT {
    uint16_t timeout; /**< Timeout in seconds */
    uint8_t pin[ZCL_DRLK_MAX_PIN_LEN]; /**< PIN/RFID Code */
    uint8_t pin_len; /**< Length of PIN/RFID Code */
};

/**
 * Send an Unlock with Timeout request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Unlock with Timeout request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientUnlockTimeoutReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockUnlockTimeoutReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/** Unlock with Timeout response structure */
struct ZbZclDoorLockUnlockTimeoutRspT {
    uint8_t status; /**< Status */
};

/**
 * Send an Unlock with Timeout response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Unlock with Timeout response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendUnlockTimeoutRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockUnlockTimeoutRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get Log Helper
 *---------------------------------------------------------------
 */
/** Get Log Record request structure */
struct ZbZclDoorLockGetLogReqT {
    uint16_t log_index; /**< Log Index */
};

/**
 * Send a Get Log Record request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get Log Record request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetLogReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetLogReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get Log Record response structure */
struct ZbZclDoorLockGetLogRspT {
    uint16_t log_entry_id; /**< Log Entry ID */
    uint32_t time_stamp; /**< Timestamp */
    uint8_t event_type; /**< Event Type */
    uint8_t source; /**< Source (see Operation Event Sources) */
    uint8_t alarm_code; /**< Event ID/Alarm Code (see Operation Event Codes) */
    uint16_t user_id; /**< User ID */
    uint8_t pin[ZCL_DRLK_MAX_PIN_LEN]; /**< PIN */
    uint8_t pin_len; /**< Length of PIN */
};

/**
 * Send a Get Log Record response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get Log Record response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetLogRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetLogRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set Pin Helper
 *---------------------------------------------------------------
 */
/** Set PIN Code request structure */
struct ZbZclDoorLockSetPinReqT {
    uint16_t user_id; /**< User ID */
    uint8_t user_status; /**< User Status */
    uint8_t user_type; /**< User Type */
    uint8_t pin[ZCL_DRLK_MAX_PIN_LEN]; /**< PIN */
    uint8_t pin_len; /**< Length of PIN */
};

/**
 * Send a Set PIN Code request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Set PIN Code request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientSetPinReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetPinReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Set PIN Code response structure */
struct ZbZclDoorLockSetPinRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Set PIN Code response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Set PIN Code response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendSetPinRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get Pin Helper
 *---------------------------------------------------------------
 */
/** Get PIN Code request structure */
struct ZbZclDoorLockGetPinReqT {
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Get PIN Code request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get PIN Code request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetPinReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetPinReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get PIN Code response structure */
struct ZbZclDoorLockGetPinRspT {
    uint16_t user_id; /**< User ID */
    uint8_t user_status; /**< User Status */
    uint8_t user_type; /**< User Type */
    uint8_t pin[ZCL_DRLK_MAX_PIN_LEN]; /**< Code */
    uint8_t pin_len; /**< Length of Code */
};

/**
 * Send a Get PIN Code response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get PIN Code response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetPinRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear Pin Helper
 *---------------------------------------------------------------
 */
/** Clear PIN Code request structure */
struct ZbZclDoorLockClrPinReqT {
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Clear PIN Code request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Clear PIN Code request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientClrPinReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrPinReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Clear PIN Code response structure */
struct ZbZclDoorLockClrPinRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Clear PIN Code response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Clear PIN Code response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendClrPinRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear All Pin Helper
 *---------------------------------------------------------------
 */
/**
 * Send a Clear All PIN Codes request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientClrAllPinReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Clear All PIN Codes response structure */
struct ZbZclDoorLockClrAllPinRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Clear All PIN Codes response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Clear All PIN Codes response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendClrAllPinRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrAllPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set User Status Helper
 *---------------------------------------------------------------
 */
/** Set User Status request structure */
struct ZbZclDoorLockSetUserStatusReqT {
    uint16_t user_id; /**< User ID */
    uint8_t user_status; /**< User Status */
};

/**
 * Send a Set User Status request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Set User Status request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientSetUserStatusReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetUserStatusReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Set User Status response structure */
struct ZbZclDoorLockSetUserStatusRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Set User Status response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Set User Status response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendSetUserStatusRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetUserStatusRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get User Status Helper
 *---------------------------------------------------------------
 */
/** Get User Status request structure */
struct ZbZclDoorLockGetUserStatusReqT {
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Get User Status request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get User Status request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetUserStatusReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetUserStatusReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get User Status response structure */
struct ZbZclDoorLockGetUserStatusRspT {
    uint16_t user_id; /**< User ID */
    uint8_t user_status; /**< User Status */
};

/**
 * Send a Get User Status response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get User Status response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetUserStatusRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetUserStatusRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set WD Schedule Helper
 *---------------------------------------------------------------
 */
/** Set Weekday Schedule request structure */
struct ZbZclDoorLockSetWDScheduleReqT {
    uint8_t schedule_id; /**< ScheduleID # */
    uint16_t user_id; /**< User ID */
    uint8_t days_mask; /**< Days Mask */
    uint8_t start_hour; /**< Start Hour */
    uint8_t start_minute; /**< Start Minute */
    uint8_t end_hour; /**< End Hour */
    uint8_t end_minute; /**< End Minute */
};

/**
 * Send a Set Weekday Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Set Weekday Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientSetWDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetWDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Set Weekday Schedule response structure */
struct ZbZclDoorLockSetWDScheduleRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Set Weekday Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Set Weekday Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendSetWDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetWDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get WD Schedule Helper
 *---------------------------------------------------------------
 */
/** Get Weekday Schedule request structure */
struct ZbZclDoorLockGetWDScheduleReqT {
    uint8_t schedule_id; /**< Schedule ID */
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Get Weekday Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get Weekday Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetWDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetWDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get Weekday Schedule response structure */
struct ZbZclDoorLockGetWDScheduleRspT {
    uint8_t schedule_id; /**< Schedule ID */
    uint16_t user_id; /**< User ID */
    uint8_t status; /**< Status */
    uint8_t days_mask; /**< Days Mask */
    uint8_t start_hour; /**< Start Hour */
    uint8_t start_minute; /**< Start Minute */
    uint8_t end_hour; /**< End Hour */
    uint8_t end_minute; /**< End Minute */
};

/**
 * Send a Get Weekday Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get Weekday Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetWDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetWDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear WD Schedule Helper
 *---------------------------------------------------------------
 */
/** Clear Weekday Schedule request structure */
struct ZbZclDoorLockClrWDScheduleReqT {
    uint8_t schedule_id; /**< Schedule ID */
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Clear Weekday Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Clear Weekday Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientClrWDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrWDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Clear Weekday Schedule response structure */
struct ZbZclDoorLockClrWDScheduleRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Clear Weekday Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Clear Weekday Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendClrWDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrWDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set YD Schedule Helper
 *---------------------------------------------------------------
 */
/** Set Year Day Schedule request structure */
struct ZbZclDoorLockSetYDScheduleReqT {
    uint8_t schedule_id; /**< Schedule ID */
    uint16_t user_id; /**< User ID */
    uint32_t local_start_time; /**< Local Start Time */
    uint32_t local_end_time; /**< Local End Time */
};

/**
 * Send a Set Year Day Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Set Year Day Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientSetYDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetYDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Set Year Day Schedule response structure */
struct ZbZclDoorLockSetYDScheduleRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Set Year Day Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Set Year Day Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendSetYDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetYDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get YD Schedule Helper
 *---------------------------------------------------------------
 */
/** Get Year Day Schedule request structure */
struct ZbZclDoorLockGetYDScheduleReqT {
    uint8_t schedule_id; /**< Schedule ID */
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Get Year Day Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get Year Day Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetYDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetYDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get Year Day Schedule response structure */
struct ZbZclDoorLockGetYDScheduleRspT {
    uint8_t schedule_id; /**< Schedule ID */
    uint16_t user_id; /**< User ID */
    uint8_t status; /**< Status */
    uint32_t local_start_time; /**< Local Start Time */
    uint32_t local_end_time; /**< Local End Time */
};

/**
 * Send a Get Year Day Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get Year Day Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetYDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetYDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear YD Schedule Helper
 *---------------------------------------------------------------
 */
/** Clear Year Day Schedule request structure */
struct ZbZclDoorLockClrYDScheduleReqT {
    uint8_t schedule_id; /**< Schedule ID */
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Clear Year Day Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Clear Year Day Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientClrYDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrYDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Clear Year Day Schedule response structure */
struct ZbZclDoorLockClrYDScheduleRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Clear Year Day Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Clear Year Day Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendClrYDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrYDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set HD Schedule Helper
 *---------------------------------------------------------------
 */
/** Set Holiday Schedule request structure */
struct ZbZclDoorLockSetHDScheduleReqT {
    uint8_t schedule_id; /**< Holiday Schedule ID */
    uint32_t local_start_time; /**< Local Start Time */
    uint32_t local_end_time; /**< Local End Time */
    uint8_t operating_mode; /**< Operating Mode During Holiday */
};

/**
 * Send a Set Holiday Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Set Holiday Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientSetHDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetHDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Set Holiday Schedule response structure */
struct ZbZclDoorLockSetHDScheduleRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Set Holiday Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Set Holiday Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendSetHDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetHDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get HD Schedule Helper
 *---------------------------------------------------------------
 */
/** Get Holiday Schedule request structure */
struct ZbZclDoorLockGetHDScheduleReqT {
    uint8_t schedule_id; /**< Holiday Schedule ID */
};

/**
 * Send a Get Holiday Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get Holiday Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetHDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetHDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get Holiday Schedule response structure */
struct ZbZclDoorLockGetHDScheduleRspT {
    uint8_t schedule_id; /**< Holiday Schedule ID */
    uint8_t status; /**< Status */
    uint32_t local_start_time; /**< Local Start Time */
    uint32_t local_end_time; /**< Local End Time */
    uint8_t operating_mode; /**< Operating Mode During Holiday */
};

/**
 * Send a Get Holiday Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get Holiday Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetHDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetHDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear HD Schedule Helper
 *---------------------------------------------------------------
 */
/** Clear Holiday Schedule request structure */
struct ZbZclDoorLockClrHDScheduleReqT {
    uint8_t schedule_id; /**< Holiday Schedule ID */
};

/**
 * Send a Clear Holiday Schedule request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Clear Holiday Schedule request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientClrHDScheduleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrHDScheduleReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Clear Holiday Schedule response structure */
struct ZbZclDoorLockClrHDScheduleRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Clear Holiday Schedule response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Clear Holiday Schedule response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendClrHDScheduleRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrHDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set User Type Helper
 *---------------------------------------------------------------
 */
/** Set User Type request structure */
struct ZbZclDoorLockSetUserTypeReqT {
    uint16_t user_id; /**< User ID */
    uint8_t user_type; /**< User Type */
};

/**
 * Send a Set User Type request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Set User Type request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientSetUserTypeReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetUserTypeReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Set User Type response structure */
struct ZbZclDoorLockSetUserTypeRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Set User Type response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Set User Type response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendSetUserTypeRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetUserTypeRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get User Type Helper
 *---------------------------------------------------------------
 */
/** Get User Type request structure */
struct ZbZclDoorLockGetUserTypeReqT {
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Get User Type request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get User Type request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetUserTypeReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetUserTypeReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get User Type response structure */
struct ZbZclDoorLockGetUserTypeRspT {
    uint16_t user_id; /**< User ID */
    uint8_t user_type; /**< User Type */
};

/**
 * Send a Get User Type response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get User Type response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetUserTypeRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetUserTypeRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set RFID Helper
 *---------------------------------------------------------------
 */
/** Set RFID Code request structure */
struct ZbZclDoorLockSetRfidReqT {
    uint16_t user_id; /**< User ID */
    uint8_t user_status; /**< User Status */
    uint8_t user_type; /**< User Type */
    uint8_t rfid[ZCL_DRLK_MAX_RFID_LEN]; /**< RFID Code */
    uint8_t rfid_len; /**< Length of RFID Code */
};

/**
 * Send a Set RFID Code request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Set RFID Code request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientSetRfidReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetRfidReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Set RFID Code response structure */
struct ZbZclDoorLockSetRfidRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Set RFID Code response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Set RFID Code response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendSetRfidRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetRfidRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get RFID Helper
 *---------------------------------------------------------------
 */
/** Get RFID Code request structure */
struct ZbZclDoorLockGetRfidReqT {
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Get RFID Code request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Get RFID Code request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientGetRfidReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetRfidReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get RFID Code response structure */
struct ZbZclDoorLockGetRfidRspT {
    uint16_t user_id; /**< User ID */
    uint8_t user_status; /**< User Status */
    uint8_t user_type; /**< User Type */
    uint8_t rfid[ZCL_DRLK_MAX_RFID_LEN]; /**< RFID Code */
    uint8_t rfid_len; /**< Length of RFID Code */
};

/**
 * Send a Get RFID Code response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Get RFID Code response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendGetRfidRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetRfidRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear RFID Helper
 *---------------------------------------------------------------
 */
/** Clear RFID Code request structure */
struct ZbZclDoorLockClrRfidReqT {
    uint16_t user_id; /**< User ID */
};

/**
 * Send a Clear RFID Code request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Clear RFID Code request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientClrRfidReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrRfidReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Clear RFID Code response structure */
struct ZbZclDoorLockClrRfidRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Clear RFID Code response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Clear RFID Code response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendClrRfidRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrRfidRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear All RFID Helper
 *---------------------------------------------------------------
 */

/**
 * Send a Clear All RFID Codes request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockClientClrAllRfidReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Clear All RFID Codes response structure */
struct ZbZclDoorLockClrAllRfidRspT {
    uint8_t status; /**< Status */
};

/**
 * Send a Clear All RFID Codes response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Clear All RFID Codes response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDoorLockServerSendClrAllRfidRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrAllRfidRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Server
 *---------------------------------------------------------------
 */

/* These callbacks are used to implemented command functionality in the application.
 * The callbacks should return ZCL_STATUS_SUCCESS or ZCL_STATUS_FAILURE, and that status
 * will be attached to the payload sent in cluster response. The response can either
 * be sent with ZbZclClusterCommandRsp() using srcInfo provided, or the app can call
 * ZbZclSendDefaultResponse2 to send a ZCL Default Response */

/** Door Lock Server callbacks configuration */
struct ZbZclDoorLockServerCallbacksT {
    enum ZclStatusCodeT (*lock)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockLockDoorReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Lock Door command. */

    enum ZclStatusCodeT (*unlock)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockUnlockDoorReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Unlock Door command. */

    enum ZclStatusCodeT (*toggle)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockToggleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Toggle command. */

    enum ZclStatusCodeT (*unlock_timeout)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockUnlockTimeoutReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Unlock with Timeout command. */

    enum ZclStatusCodeT (*get_log)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetLogReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get Log Record command. */

    enum ZclStatusCodeT (*set_pin)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockSetPinReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Set PIN Code command. */

    enum ZclStatusCodeT (*get_pin)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetPinReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get PIN Code command. */

    enum ZclStatusCodeT (*clr_pin)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockClrPinReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Clear PIN Code command. */

    enum ZclStatusCodeT (*clr_all_pins)(struct ZbZclClusterT *cluster,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Clear All PIN Codes command. */

    enum ZclStatusCodeT (*set_user_status)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockSetUserStatusReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Set User Status command. */

    enum ZclStatusCodeT (*get_user_status)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetUserStatusReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get User Status command. */

    enum ZclStatusCodeT (*set_wd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockSetWDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Set Weekday Schedule command. */

    enum ZclStatusCodeT (*get_wd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetWDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get Weekday Schedule command. */

    enum ZclStatusCodeT (*clr_wd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockClrWDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Clear Weekday Schedule command. */

    enum ZclStatusCodeT (*set_yd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockSetYDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Set Year Day Schedule command. */

    enum ZclStatusCodeT (*get_yd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetYDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get Year Day Schedule command. */

    enum ZclStatusCodeT (*clr_yd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockClrYDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Clear Year Day Schedule command. */

    enum ZclStatusCodeT (*set_hd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockSetHDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Set Holiday Schedule command. */

    enum ZclStatusCodeT (*get_hd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetHDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get Holiday Schedule command. */

    enum ZclStatusCodeT (*clr_hd_sched)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockClrHDScheduleReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Clear Holiday Schedule command. */

    enum ZclStatusCodeT (*set_user_type)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockSetUserTypeReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Set User Type command. */

    enum ZclStatusCodeT (*get_user_type)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetUserTypeReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get User Type command. */

    enum ZclStatusCodeT (*set_rfid)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockSetRfidReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Set RFID Code command. */

    enum ZclStatusCodeT (*get_rfid)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockGetRfidReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Get RFID Code command. */

    enum ZclStatusCodeT (*clr_rfid)(struct ZbZclClusterT *cluster,
        struct ZbZclDoorLockClrRfidReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Clear RFID Code command. */

    enum ZclStatusCodeT (*clr_all_rfids)(struct ZbZclClusterT *cluster,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Clear All RFID Codes command. */
};

/**
 * Create a new instance of the Door Lock Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDoorLockServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclDoorLockServerCallbacksT *callbacks, void *arg);

#endif
