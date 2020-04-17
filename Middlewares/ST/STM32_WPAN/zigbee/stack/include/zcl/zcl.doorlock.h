/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_DOORLOCK_H
#define ZCL_DOORLOCK_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL OnOff cluster.
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Doorlock
 * DRLK.S | True
 * DRLK.C | True
 *
 * Server Attributes
 * DRLK.S.A0000 | True
 * DRLK.S.A0001 | True
 * DRLK.S.A0002 | True
 * DRLK.S.A0003 | False
 * DRLK.S.A0004 | False
 * DRLK.S.A0005 | False
 * DRLK.S.A0006 | False
 * DRLK.S.A0010 | False
 * DRLK.S.A0011 | False
 * DRLK.S.A0012 | False
 * DRLK.S.A0013 | False
 * DRLK.S.A0014 | False
 * DRLK.S.A0015 | False
 * DRLK.S.A0016 | False
 * DRLK.S.A0017 | True
 * DRLK.S.A0018 | True
 * DRLK.S.A0019 | True
 * DRLK.S.A001a | True
 * DRLK.S.A0020 | False
 * DRLK.S.A0021 | True
 * DRLK.S.A0022 | False
 * DRLK.S.A0023 | False
 * DRLK.S.A0024 | False
 * DRLK.S.A0025 | False
 * DRLK.S.A0026 | True
 * DRLK.S.A0027 | False
 * DRLK.S.A0028 | True
 * DRLK.S.A0029 | False
 * DRLK.S.A002a | False
 * DRLK.S.A002b | False
 * DRLK.S.A0030 | False
 * DRLK.S.A0031 | False
 * DRLK.S.A0032 | False
 * DRLK.S.A0033 | False
 * DRLK.S.A0034 | False
 * DRLK.S.A0040 | False
 * DRLK.S.A0041 | False
 * DRLK.S.A0042 | False
 * DRLK.S.A0043 | False
 * DRLK.S.A0044 | False
 * DRLK.S.A0045 | False
 * DRLK.S.A0046 | False
 * DRLK.S.A0047 | False
 * DRLK.S.Afffd | True
 * DRLK.S.Afffe | False
 *
 * Commands Received
 * DRLK.S.C00.Rsp | True
 * DRLK.S.C01.Rsp | True
 * DRLK.S.C02.Rsp | True
 * DRLK.S.C03.Rsp | False
 * DRLK.S.C04.Rsp | False
 * DRLK.S.C05.Rsp | False
 * DRLK.S.C06.Rsp | False
 * DRLK.S.C07.Rsp | False
 * DRLK.S.C08.Rsp | False
 * DRLK.S.C09.Rsp | False
 * DRLK.S.C0a.Rsp | False
 * DRLK.S.C0b.Rsp | False
 * DRLK.S.C0c.Rsp | False
 * DRLK.S.C0d.Rsp | False
 * DRLK.S.C0e.Rsp | False
 * DRLK.S.C0f.Rsp | False
 * DRLK.S.C10.Rsp | False
 * DRLK.S.C11.Rsp | False
 * DRLK.S.C12.Rsp | False
 * DRLK.S.C13.Rsp | False
 * DRLK.S.C14.Rsp | False
 * DRLK.S.C15.Rsp | False
 * DRLK.S.C16.Rsp | False
 * DRLK.S.C17.Rsp | False
 * DRLK.S.C18.Rsp | False
 * DRLK.S.C19.Rsp | False
 *
 * Commands Generated
 * DRLK.S.C00.Tx | True
 * DRLK.S.C01.Tx | True
 * DRLK.S.C02.Tx | True
 * DRLK.S.C03.Tx | False
 * DRLK.S.C04.Tx | False
 * DRLK.S.C05.Tx | False
 * DRLK.S.C06.Tx | False
 * DRLK.S.C07.Tx | False
 * DRLK.S.C08.Tx | False
 * DRLK.S.C09.Tx | False
 * DRLK.S.C0a.Tx | False
 * DRLK.S.C0b.Tx | False
 * DRLK.S.C0c.Tx | False
 * DRLK.S.C0d.Tx | False
 * DRLK.S.C0e.Tx | False
 * DRLK.S.C0f.Tx | False
 * DRLK.S.C10.Tx | False
 * DRLK.S.C11.Tx | False
 * DRLK.S.C12.Tx | False
 * DRLK.S.C13.Tx | False
 * DRLK.S.C14.Tx | False
 * DRLK.S.C15.Tx | False
 * DRLK.S.C16.Tx | False
 * DRLK.S.C17.Tx | False
 * DRLK.S.C18.Tx | False
 * DRLK.S.C19.Tx | False
 * DRLK.S.C20.Tx | False
 * DRLK.S.C21.Tx | False
 *
 *
 * Client Attributes
 * DRLK.C.Afffd | True
 * DRLK.C.Afffe | False
 *
 * Commands Received
 * DRLK.C.C00.Rsp | True
 * DRLK.C.C01.Rsp | True
 * DRLK.C.C02.Rsp | True
 * DRLK.C.C03.Rsp | False
 * DRLK.C.C04.Rsp | False
 * DRLK.C.C05.Rsp | False
 * DRLK.C.C06.Rsp | False
 * DRLK.C.C07.Rsp | False
 * DRLK.C.C08.Rsp | False
 * DRLK.C.C09.Rsp | False
 * DRLK.C.C0a.Rsp | False
 * DRLK.C.C0b.Rsp | False
 * DRLK.C.C0c.Rsp | False
 * DRLK.C.C0d.Rsp | False
 * DRLK.C.C0e.Rsp | False
 * DRLK.C.C0f.Rsp | False
 * DRLK.C.C10.Rsp | False
 * DRLK.C.C11.Rsp | False
 * DRLK.C.C12.Rsp | False
 * DRLK.C.C13.Rsp | False
 * DRLK.C.C14.Rsp | False
 * DRLK.C.C15.Rsp | False
 * DRLK.C.C16.Rsp | False
 * DRLK.C.C17.Rsp | False
 * DRLK.C.C18.Rsp | False
 * DRLK.C.C19.Rsp | False
 * DRLK.C.C20.Rsp | False
 * DRLK.C.C21.Rsp | False
 *
 * Commands Generated
 * DRLK.C.C00.Tx | True
 * DRLK.C.C01.Tx | True
 * DRLK.C.C02.Tx | True
 * DRLK.C.C03.Tx | False
 * DRLK.C.C04.Tx | False
 * DRLK.C.C05.Tx | False
 * DRLK.C.C06.Tx | False
 * DRLK.C.C07.Tx | False
 * DRLK.C.C08.Tx | False
 * DRLK.C.C09.Tx | False
 * DRLK.C.C0a.Tx | False
 * DRLK.C.C0b.Tx | False
 * DRLK.C.C0c.Tx | False
 * DRLK.C.C0d.Tx | False
 * DRLK.C.C0e.Tx | False
 * DRLK.C.C0f.Tx | False
 * DRLK.C.C10.Tx | False
 * DRLK.C.C11.Tx | False
 * DRLK.C.C12.Tx | False
 * DRLK.C.C13.Tx | False
 * DRLK.C.C14.Tx | False
 * DRLK.C.C15.Tx | False
 * DRLK.C.C16.Tx | False
 * DRLK.C.C17.Tx | False
 * DRLK.C.C18.Tx | False
 * DRLK.C.C19.Tx | False
 */

/* DoorLock Cluster Attributes */
enum {
    ZCL_DOORLOCK_ATTR_LOCKSTATE = 0x0000, /* R- mandatory! */
    ZCL_DOORLOCK_ATTR_LOCKTYPE = 0x0001, /* R- mandatory! */
    ZCL_DOORLOCK_ATTR_ACT_ENABLED = 0x0002, /* R- mandatory! */
    ZCL_DOORLOCK_ATTR_DOORSTATE = 0x0003, /* R- Optional. */
    ZCL_DOORLOCK_ATTR_OPENEVENTS = 0x0004, /* R/W- Optional*/
    ZCL_DOORLOCK_ATTR_CLOSEDEVENTS = 0x0005, /* R/W- Optional*/
    ZCL_DOORLOCK_ATTR_OPENPERIOD = 0x0006, /* R/W- Optional*/
    /* User/PIN/Schedule/Log attributes - all optional. */
    ZCL_DOORLOCK_ATTR_NUM_LOGRECORDS = 0x0010, /* R */
    ZCL_DOORLOCK_ATTR_NUM_TOTALUSERS = 0x0011, /* R */
    ZCL_DOORLOCK_ATTR_NUM_PINUSERS = 0x0012, /* R */
    ZCL_DOORLOCK_ATTR_NUM_RFIDUSERS = 0x0013, /* R */
    ZCL_DOORLOCK_ATTR_NUM_WD_SCHEDULES = 0x0014, /* R - weekday schedule*/
    ZCL_DOORLOCK_ATTR_NUM_YD_SCHEDULES = 0x0015, /* R - yearday schedule*/
    ZCL_DOORLOCK_ATTR_NUM_HD_SCHEDULES = 0x0016, /* R - holiday schedule*/
    ZCL_DOORLOCK_ATTR_MAX_PIN_LEN = 0x0017, /* R - max pin code len. */
    ZCL_DOORLOCK_ATTR_MIN_PIN_LEN = 0x0018, /* R - min pin code len. */
    ZCL_DOORLOCK_ATTR_MAX_RFID_LEN = 0x0019, /* R - min pin code len. */
    ZCL_DOORLOCK_ATTR_MIN_RFID_LEN = 0x001A, /* R - min pin code len. */
    /* Operational Settings. - all optional. */
    ZCL_DOORLOCK_ATTR_LOGGING = 0x0020, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_LANGUAGE = 0x0021, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_LED_SETTINGS = 0x0022, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_AUTO_RELOCK = 0x0023, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_VOLUME = 0x0024, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_MODE = 0x0025, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_SUPPORTED_MODES = 0x0026, /* R */
    ZCL_DOORLOCK_ATTR_DEF_CFG = 0x0027, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_LOCAL_PROG = 0x0028, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_ONETOUCH_LOCK = 0x0029, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_INSIDE_STATUS = 0x002A, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_PRIV_BUTTON = 0x002B, /* R/W-Report */
    /* Security Settings. - all optional. */
    ZCL_DOORLOCK_ATTR_WRONGCODE_LIMIT = 0x0030, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_TEMP_DISABLE = 0x0031, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_PIN_OTA = 0x0032, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_PIN_FOR_RF = 0x0033, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_SEC_LEVEL = 0x0034, /* R/W-Report */
    /* Alarms and Event masks. - all optional. */
    ZCL_DOORLOCK_ATTR_ALARM_MASK = 0x0040, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_KEYPAD_OP_EVENT_MASK = 0x0041, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_RF_OP_EVENT_MASK = 0x0042, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_MANUAL_OP_EVENT_MASK = 0x0043, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_RFID_EVENT_MASK = 0x0044, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_KEYPAD_EVENT_MASK = 0x0045, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_RF_PROG_EVENT_MASK = 0x0046, /* R/W-Report */
    ZCL_DOORLOCK_ATTR_RFID_PROG_EVENT_MASK = 0x0047 /* R/W-Report */
};

/* Default Values */
#define DOORLOCK_LOGRECORDS_DEFAULT          0
#define DOORLOCK_TOTALUSERS_DEFAULT          0
#define DOORLOCK_PINUSERS_DEFAULT            0
#define DOORLOCK_RFIDUSERS_DEFAULT           0
#define DOORLOCK_WD_SCHEDULES_DEFAULT        0
#define DOORLOCK_YD_SCHEDULES_DEFAULT        0
#define DOORLOCK_HD_SCHEDULES_DEFAULT        0
#define DOORLOCK_AUTO_RELOCK_DEFAULT         0
#define DOORLOCK_MODE_DEFAULT                0
#define DOORLOCK_BADCODE_LIMIT_DEFAULT       0
#define DOORLOCK_TEMP_DISABLE_DEFAULT        0
#define DOORLOCK_PIN_FOR_RF_DEFAULT          0
#define DOORLOCK_SEC_LEVEL_DEFAULT           0
#define DOORLOCK_ALARM_DEFAULT               0

/* LOCKSTATE attribute values */
#define DOORLOCK_LOCKSTATE_NOTFULLYLOCKED    0
#define DOORLOCK_LOCKSTATE_LOCKED            1
#define DOORLOCK_LOCKSTATE_UNLOCKED          2
#define DOORLOCK_LOCKSTATE_UNDEFINED         0xFF

/* LOCKTYPE attributes... many of these set yours accordingly. */
#define DOORLOCK_LOCKTYPE                    0x00

/* ACT_ENABLED attribute values */
#define DOORLOCK_ACT_ENABLED_DISABLED        0
#define DOORLOCK_ACT_ENABLED_ENABLED         1

/* Min and Max Values */
#define DOORLOCK_MAX_PIN                     0x08
#define DOORLOCK_MIN_PIN                     0x04
#define DOORLOCK_MAX_RFID                    0x14
#define DOORLOCK_MIN_RFID                    0x08
#define DOORLOCK_MAX_USERS                   0x10
#define DOORLOCK_MAX_PIN_USERS               0x08
#define DOORLOCK_MAX_RFID_USERS              0x08
#define DOORLOCK_WD_SCHED_SUPP               0x02
#define DOORLOCK_YD_SCHED_SUPP               0x02
#define DOORLOCK_HD_SCHED_SUPP               0x02

/* User Status Values */
#define DOORLOCK_USER_STATUS_AVAILABLE       (uint8_t)0x00
#define DOORLOCK_USER_STATUS_OCC_ENABLED     (uint8_t)0x01
#define DOORLOCK_USER_STATUS_OCC_DISABLED    (uint8_t)0x03
#define DOORLOCK_USER_STATUS_NOT_SUPP        (uint8_t)0xFF

/* User Type Values */
#define DOORLOCK_USER_TYPE_UNRESTRICTED      (uint8_t)0x00
#define DOORLOCK_USER_TYPE_YD_SCHEDULE       (uint8_t)0x01
#define DOORLOCK_USER_TYPE_WD_SCHEDULE       (uint8_t)0x02
#define DOORLOCK_USER_TYPE_MASTER            (uint8_t)0x03
#define DOORLOCK_USER_TYPE_NON_ACCESS        (uint8_t)0x04
#define DOORLOCK_USER_TYPE_NOT_SUPPORTED     (uint8_t)0xFF

/* Response Status Values */
#define DOORLOCK_STATUS_SUCCESS              (uint8_t)0x0
#define DOORLOCK_STATUS_GENERAL_FAILURE      (uint8_t)0x1
#define DOORLOCK_STATUS_MEM_FULL             (uint8_t)0x2
#define DOORLOCK_STATUS_DUPLICATE            (uint8_t)0x3

/* Schedule Values */
#define DOORLOCK_HOURS_LIMIT                 (uint8_t)0x17
#define DOORLOCK_MINUTES_LIMIT               (uint8_t)0x3B

/* Security Settings */
#define DOORLOCK_PIN_REQUIRED                1
#define DOORLOCK_PIN_NOT_REQUIRED            0

/* Client Generated Commands */
enum {
    ZCL_DOORLOCK_CLI_LOCK = 0x00, /* Mandatory */
    ZCL_DOORLOCK_CLI_UNLOCK = 0x01, /* Mandatory */
    ZCL_DOORLOCK_CLI_TOGGLE = 0x02,
    ZCL_DOORLOCK_CLI_UNLOCK_TIMEOUT = 0x03,
    ZCL_DOORLOCK_CLI_GET_LOG = 0x04,
    ZCL_DOORLOCK_CLI_SETPIN = 0x05,
    ZCL_DOORLOCK_CLI_GETPIN = 0x06,
    ZCL_DOORLOCK_CLI_CLRPIN = 0x07,
    ZCL_DOORLOCK_CLI_CLR_ALL_PINS = 0x08,
    ZCL_DOORLOCK_CLI_SETUSER_STATUS = 0x09,
    ZCL_DOORLOCK_CLI_GETUSER_STATUS = 0x0a,
    ZCL_DOORLOCK_CLI_SETWD_SCHED = 0x0b,
    ZCL_DOORLOCK_CLI_GETWD_SCHED = 0x0c,
    ZCL_DOORLOCK_CLI_CLRWD_SCHED = 0x0d,
    ZCL_DOORLOCK_CLI_SETYD_SCHED = 0x0e,
    ZCL_DOORLOCK_CLI_GETYD_SCHED = 0x0f,
    ZCL_DOORLOCK_CLI_CLRYD_SCHED = 0x10,
    ZCL_DOORLOCK_CLI_SETHD_SCHED = 0x11,
    ZCL_DOORLOCK_CLI_GETHD_SCHED = 0x12,
    ZCL_DOORLOCK_CLI_CLRHD_SCHED = 0x13,
    ZCL_DOORLOCK_CLI_SET_USERTYPE = 0x14,
    ZCL_DOORLOCK_CLI_GET_USERTYPE = 0x15,
    ZCL_DOORLOCK_CLI_SET_RFID = 0x16,
    ZCL_DOORLOCK_CLI_GET_RFID = 0x17,
    ZCL_DOORLOCK_CLI_CLR_RFID = 0x18,
    ZCL_DOORLOCK_CLI_CLR_ALL_RFIDS = 0x19
};

/* Server Generated Commands */
enum {
    ZCL_DOORLOCK_SVR_LOCK_RSP = 0x00, /* Mandatory */
    ZCL_DOORLOCK_SVR_UNLOCK_RSP = 0x01, /* Mandatory */
    ZCL_DOORLOCK_SVR_TOGGLE_RSP = 0x02,
    ZCL_DOORLOCK_SVR_UNLOCK_TO_RSP = 0x03,
    ZCL_DOORLOCK_SVR_GET_LOG_RSP = 0x04,
    ZCL_DOORLOCK_SVR_SETPIN_RSP = 0x05,
    ZCL_DOORLOCK_SVR_GETPIN_RSP = 0x06,
    ZCL_DOORLOCK_SVR_CLRPIN_RSP = 0x07,
    ZCL_DOORLOCK_SVR_CLR_ALL_PINS_RSP = 0x08,
    ZCL_DOORLOCK_SVR_SETUSER_STATUS_RSP = 0x09,
    ZCL_DOORLOCK_SVR_GETUSER_STATUS_RSP = 0x0a,
    ZCL_DOORLOCK_SVR_SETWD_SCHED_RSP = 0x0b,
    ZCL_DOORLOCK_SVR_GETWD_SCHED_RSP = 0x0c,
    ZCL_DOORLOCK_SVR_CLRWD_SCHED_RSP = 0x0d,
    ZCL_DOORLOCK_SVR_SETYD_SCHED_RSP = 0x0e,
    ZCL_DOORLOCK_SVR_GETYD_SCHED_RSP = 0x0f,
    ZCL_DOORLOCK_SVR_CLRYD_SCHED_RSP = 0x10,
    ZCL_DOORLOCK_SVR_SETHD_SCHED_RSP = 0x11,
    ZCL_DOORLOCK_SVR_GETHD_SCHED_RSP = 0x12,
    ZCL_DOORLOCK_SVR_CLRHD_SCHED_RSP = 0x13,
    ZCL_DOORLOCK_SVR_SET_USERTYPE_RSP = 0x14,
    ZCL_DOORLOCK_SVR_GET_USERTYPE_RSP = 0x15,
    ZCL_DOORLOCK_SVR_SET_RFID_RSP = 0x16,
    ZCL_DOORLOCK_SVR_GET_RFID_RSP = 0x17,
    ZCL_DOORLOCK_SVR_CLR_RFID_RSP = 0x18,
    ZCL_DOORLOCK_SVR_CLR_ALL_RFIDS_RSP = 0x19,
    /* Warning, discontinuity from 0x19 to 0x20 (someone can't count in hex?) */
    /* Notifications */
    ZCL_DOORLOCK_SVR_OPER_EVENT_NOTIFY = 0x20,
    ZCL_DOORLOCK_SVR_PROG_EVENT_NOTIFY = 0x21
};

/*---------------------------------------------------------------
 * Client
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclDoorLockClientAlloc(struct ZigBeeT *zb, uint8_t ep);

/*---------------------------------------------------------------
 * Client Lock/Unlock Helper
 *---------------------------------------------------------------
 */

/* One of:
 *      ZCL_DOORLOCK_CLI_LOCK
 *      ZCL_DOORLOCK_CLI_UNLOCK
 *      ZCL_DOORLOCK_CLI_TOGGLE
 *      ZCL_DOORLOCK_CLI_UNLOCK_TIMEOUT
 */

/* ZCL_DOORLOCK_CLI_LOCK */
struct ZbZclDoorLockLockDoorReqT {
    uint8_t pin[DOORLOCK_MAX_PIN];
    uint8_t pin_len;
};

enum ZclStatusCodeT ZbZclDoorLockClientLockReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockLockDoorReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

struct ZbZclDoorLockLockDoorRspT {
    uint8_t status;
};

enum ZclStatusCodeT ZbZclDoorLockServerSendLockRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockLockDoorRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_DOORLOCK_CLI_UNLOCK */
struct ZbZclDoorLockUnlockDoorReqT {
    uint8_t pin[DOORLOCK_MAX_PIN];
    uint8_t pin_len;
};

enum ZclStatusCodeT ZbZclDoorLockClientUnlockReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockUnlockDoorReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

struct ZbZclDoorLockUnlockDoorRspT {
    uint8_t status;
};

enum ZclStatusCodeT ZbZclDoorLockServerSendUnlockRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockUnlockDoorRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_DOORLOCK_CLI_TOGGLE */
struct ZbZclDoorLockToggleReqT {
    uint8_t pin[DOORLOCK_MAX_PIN];
    uint8_t pin_len;
};

enum ZclStatusCodeT ZbZclDoorLockClientToggleReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockToggleReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

struct ZbZclDoorLockToggleRspT {
    uint8_t status;
};

enum ZclStatusCodeT ZbZclDoorLockServerSendToggleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockToggleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_DOORLOCK_CLI_UNLOCK_TIMEOUT */
struct ZbZclDoorLockUnlockTimeoutReqT {
    uint16_t timeout;
    uint8_t pin[DOORLOCK_MAX_PIN];
    uint8_t pin_len;
};

enum ZclStatusCodeT ZbZclDoorLockClientUnlockTimeoutReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclDoorLockUnlockTimeoutReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

struct ZbZclDoorLockUnlockTimeoutRspT {
    uint8_t status;
};

enum ZclStatusCodeT ZbZclDoorLockServerSendUnlockTimeoutRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockUnlockTimeoutRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get Log Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetLogReqT {
    uint16_t log_index;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetLogReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetLogReqT *get_log_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetLogRspT {
    uint16_t log_entry_id;
    uint32_t time_stamp;
    uint8_t event_type;
    uint8_t source;
    uint8_t alarm_code;
    uint16_t user_id;
    uint8_t pin[DOORLOCK_MAX_PIN];
    uint8_t pin_len;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetLogRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetLogRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set Pin Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockSetPinReqT {
    uint16_t user_id;
    uint8_t user_status; /* e.g. DOORLOCK_USER_STATUS_AVAILABLE */
    uint8_t user_type; /* e.g. DOORLOCK_USER_TYPE_UNRESTRICTED */
    uint8_t pin[DOORLOCK_MAX_PIN];
    uint8_t pin_len;
};
enum ZclStatusCodeT ZbZclDoorLockClientSetPinReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetPinReqT *set_pin_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockSetPinRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendSetPinRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get Pin Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetPinReqT {
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetPinReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetPinReqT *get_pin_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetPinRspT {
    uint16_t user_id;
    uint8_t user_status;
    uint8_t user_type;
    uint8_t pin[DOORLOCK_MAX_PIN];
    uint8_t pin_len;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetPinRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear Pin Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockClrPinReqT {
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientClrPinReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrPinReqT *clr_pin_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockClrPinRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendClrPinRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear All Pin Helper
 *---------------------------------------------------------------
 */

enum ZclStatusCodeT ZbZclDoorLockClientClrAllPinReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockClrAllPinRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendClrAllPinRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrAllPinRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set User Status Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockSetUserStatusReqT {
    uint16_t user_id;
    uint8_t user_status;
};
enum ZclStatusCodeT ZbZclDoorLockClientSetUserStatusReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetUserStatusReqT *set_user_status_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockSetUserStatusRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendSetUserStatusRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetUserStatusRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get User Status Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetUserStatusReqT {
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetUserStatusReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetUserStatusReqT *get_user_status_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetUserStatusRspT {
    uint16_t user_id;
    uint8_t user_status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetUserStatusRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetUserStatusRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set WD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockSetWDScheduleReqT {
    uint8_t schedule_id;
    uint16_t user_id;
    uint8_t days_mask;
    uint8_t start_hour;
    uint8_t start_minute;
    uint8_t end_hour;
    uint8_t end_minute;
};
enum ZclStatusCodeT ZbZclDoorLockClientSetWDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetWDScheduleReqT *set_wd_sched_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockSetWDScheduleRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendSetWDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetWDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get WD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetWDScheduleReqT {
    uint8_t schedule_id;
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetWDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetWDScheduleReqT *get_wd_sched_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetWDScheduleRspT {
    uint8_t schedule_id;
    uint16_t user_id;
    uint8_t status;
    uint8_t days_mask;
    uint8_t start_hour;
    uint8_t start_minute;
    uint8_t end_hour;
    uint8_t end_minute;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetWDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetWDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear WD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockClrWDScheduleReqT {
    uint8_t schedule_id;
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientClrWDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrWDScheduleReqT *clr_wd_schedule_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockClrWDScheduleRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendClrWDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrWDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set YD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockSetYDScheduleReqT {
    uint8_t schedule_id;
    uint16_t user_id;
    uint32_t local_start_time;
    uint32_t local_end_time;
};
enum ZclStatusCodeT ZbZclDoorLockClientSetYDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetYDScheduleReqT *set_yd_sched_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockSetYDScheduleRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendSetYDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetYDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get YD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetYDScheduleReqT {
    uint8_t schedule_id;
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetYDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetYDScheduleReqT *get_yd_sched_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetYDScheduleRspT {
    uint8_t schedule_id;
    uint16_t user_id;
    uint8_t status;
    uint32_t local_start_time;
    uint32_t local_end_time;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetYDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetYDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear YD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockClrYDScheduleReqT {
    uint8_t schedule_id;
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientClrYDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrYDScheduleReqT *clr_yd_schedule_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockClrYDScheduleRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendClrYDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrYDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set HD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockSetHDScheduleReqT {
    uint8_t schedule_id;
    uint32_t local_start_time;
    uint32_t local_end_time;
    uint8_t operating_mode;
};
enum ZclStatusCodeT ZbZclDoorLockClientSetHDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetHDScheduleReqT *set_hd_sched_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockSetHDScheduleRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendSetHDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetHDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get HD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetHDScheduleReqT {
    uint8_t schedule_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetHDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetHDScheduleReqT *get_hd_sched_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetHDScheduleRspT {
    uint8_t schedule_id;
    uint8_t status;
    uint32_t local_start_time;
    uint32_t local_end_time;
    uint8_t operating_mode;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetHDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetHDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear HD Schedule Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockClrHDScheduleReqT {
    uint8_t schedule_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientClrHDScheduleReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrHDScheduleReqT *clr_hd_schedule_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockClrHDScheduleRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendClrHDScheduleRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrHDScheduleRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set User Type Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockSetUserTypeReqT {
    uint16_t user_id;
    uint8_t user_type;
};
enum ZclStatusCodeT ZbZclDoorLockClientSetUserTypeReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetUserTypeReqT *set_user_type_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockSetUserTypeRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendSetUserTypeRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetUserTypeRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get User Type Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetUserTypeReqT {
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetUserTypeReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetUserTypeReqT *get_user_type_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetUserTypeRspT {
    uint16_t user_id;
    uint8_t user_type;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetUserTypeRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetUserTypeRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Set RFID Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockSetRfidReqT {
    uint16_t user_id;
    uint8_t user_status;
    uint8_t user_type;
    uint8_t rfid[DOORLOCK_MAX_RFID];
    uint8_t rfid_len;
};
enum ZclStatusCodeT ZbZclDoorLockClientSetRfidReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockSetRfidReqT *set_rfid_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockSetRfidRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendSetRfidRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockSetRfidRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Get RFID Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockGetRfidReqT {
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientGetRfidReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockGetRfidReqT *get_rfid_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockGetRfidRspT {
    uint16_t user_id;
    uint8_t user_status;
    uint8_t user_type;
    uint8_t rfid[DOORLOCK_MAX_RFID];
    uint8_t rfid_len;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendGetRfidRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockGetRfidRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear RFID Helper
 *---------------------------------------------------------------
 */

struct ZbZclDoorLockClrRfidReqT {
    uint16_t user_id;
};
enum ZclStatusCodeT ZbZclDoorLockClientClrRfidReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct ZbZclDoorLockClrRfidReqT *clr_rfid_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockClrRfidRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendClrRfidRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct ZbZclDoorLockClrRfidRspT *rsp, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client Clear All RFID Helper
 *---------------------------------------------------------------
 */

enum ZclStatusCodeT ZbZclDoorLockClientClrAllRfidReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclDoorLockClrAllRfidRspT {
    uint8_t status;
};
enum ZclStatusCodeT ZbZclDoorLockServerSendClrAllRfidRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
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
struct ZbZclDoorLockServerCallbacksT {
    enum ZclStatusCodeT (*lock)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockLockDoorReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*unlock)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockUnlockDoorReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*toggle)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockToggleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*unlock_timeout)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockUnlockTimeoutReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_log)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetLogReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*set_pin)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetPinReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_pin)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetPinReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*clr_pin)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockClrPinReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*clr_all_pins)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*set_user_status)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetUserStatusReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_user_status)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetUserStatusReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*set_wd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetWDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_wd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetWDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*clr_wd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockClrWDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*set_yd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetYDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_yd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetYDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*clr_yd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockClrYDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*set_hd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetHDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_hd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetHDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*clr_hd_sched)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockClrHDScheduleReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*set_user_type)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetUserTypeReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_user_type)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetUserTypeReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*set_rfid)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockSetRfidReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*get_rfid)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockGetRfidReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*clr_rfid)(struct ZbZclClusterT *clusterPtr, struct ZbZclDoorLockClrRfidReqT *cmd_req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*clr_all_rfids)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);
};

struct ZbZclClusterT * ZbZclDoorLockServerAlloc(struct ZigBeeT *zb, uint8_t ep,
    struct ZbZclDoorLockServerCallbacksT *callbacks, void *arg);

#endif /* ZCL_DOORLOCK_H */
