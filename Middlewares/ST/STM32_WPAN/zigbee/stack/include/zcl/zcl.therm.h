/* Copyright [2017 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_THERM_H
#define ZCL_THERM_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      ZCL Thermostat cluster definitions
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Thermostat
 * TSTAT.S | True
 * TSTAT.C | True
 *
 * Server Attributes
 * TSTAT.S.A0000 | True
 * TSTAT.S.A0000.Report.Tx | True
 * TSTAT.S.A0001 | False
 * TSTAT.S.A0002 | False
 * TSTAT.S.A0003 | False
 * TSTAT.S.A0004 | False
 * TSTAT.S.A0005 | False
 * TSTAT.S.A0006 | False
 * TSTAT.S.A0007 | False
 * TSTAT.S.A0007.Report.Tx | False
 * TSTAT.S.A0008 | False
 * TSTAT.S.A0008.Report.Tx | False
 * TSTAT.S.A0010 | False
 * TSTAT.S.A0011 | True
 * TSTAT.S.A0011.Scene | True
 * TSTAT.S.A0012 | True
 * TSTAT.S.A0012.Scene | True
 * TSTAT.S.A0013 | False
 * TSTAT.S.A0014 | False
 * TSTAT.S.A0015 | False
 * TSTAT.S.A0016 | False
 * TSTAT.S.A0017 | False
 * TSTAT.S.A0018 | False
 * TSTAT.S.A0019 | False
 * TSTAT.S.A001A | False
 * TSTAT.S.A001B | True
 * TSTAT.S.A001C | True
 * TSTAT.S.A001C.Scene | True
 * TSTAT.S.A001D | False
 * TSTAT.S.A001E | False
 * TSTAT.S.A0020 | False
 * TSTAT.S.A0021 | False
 * TSTAT.S.A0022 | False
 * TSTAT.S.A0023 | False
 * TSTAT.S.A0024 | False
 * TSTAT.S.A0025 | False
 * TSTAT.S.A0029 | False
 * TSTAT.S.A0030 | False
 * TSTAT.S.A0031 | False
 * TSTAT.S.A0032 | False
 * TSTAT.S.A0040 | False
 * TSTAT.S.A0041 | False
 * TSTAT.S.A0042 | False
 * TSTAT.S.A0043 | False
 * TSTAT.S.A0044 | False
 * TSTAT.S.A0045 | False
 * TSTAT.S.A0046 | False
 * TSTAT.S.A0047 | False
 * TSTAT.S.Afffd | True
 * TSTAT.S.Afffe | False
 *
 * Commands Received
 * TSTAT.S.C00.Rsp | True
 * TSTAT.S.C01.Rsp | True
 * TSTAT.S.C02.Rsp | True
 * TSTAT.S.C03.Rsp | True
 * TSTAT.S.C04.Rsp | True
 *
 * Commands Generated
 * TSTAT.S.C00.Tx | True
 * TSTAT.S.C01.Tx | True
 *
 *
 * Client Attributes
 * TSTAT.C.A0000.Report.Rsp | False
 * TSTAT.C.A0007.Report.Rsp | False
 * TSTAT.C.A0008.Report.Rsp | False
 * TSTAT.C.Afffd | True
 * TSTAT.C.Afffd | False
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */
/* Attribute Identifiers */
enum {
    /* Thermostat Info - 0x000x */
    ZCL_THERM_SVR_ATTR_LOCAL_TEMP = 0x0000, /* int16 [0x954d,0x7fff] - MANDATORY */
    ZCL_THERM_SVR_ATTR_OUTDOOR_TEMP = 0x0001, /* int16 */
    ZCL_THERM_SVR_ATTR_OCCUPANCY = 0x0002, /* map8 */
    ZCL_THERM_SVR_ATTR_ABS_MIN_HEAT_SETPOINT_LIMIT = 0x0003, /* int16 [0x954d,0x7fff] */
    ZCL_THERM_SVR_ATTR_ABS_MAX_HEAT_SETPOINT_LIMIT = 0x0004, /* int16 */
    ZCL_THERM_SVR_ATTR_ABS_MIN_COOL_SETPOINT_LIMIT = 0x0005, /* int16 */
    ZCL_THERM_SVR_ATTR_ABS_MAX_COOL_SETPOINT_LIMIT = 0x0006, /* int16 */
    ZCL_THERM_SVR_ATTR_PI_COOLING_DEMAND = 0x0007, /* uint8 [0x00,0x64] */
    ZCL_THERM_SVR_ATTR_PI_HEATING_DEMAND = 0x0008, /* uint8 [0x00,0x64] */
    ZCL_THERM_SVR_ATTR_HVAC_SYSTYPE_CONFIG = 0x0009, /* map8 */

    /* Thermostat Settings - 0x001x */
    ZCL_THERM_SVR_ATTR_LOCAL_TEMP_CALIB = 0x0010, /* int8 [0xe7,0x19] */
    ZCL_THERM_SVR_ATTR_OCCUP_COOL_SETPOINT = 0x0011, /* int16 - MANDATORY* */
    ZCL_THERM_SVR_ATTR_OCCUP_HEAT_SETPOINT = 0x0012, /* int16 - MANDATORY* */
    ZCL_THERM_SVR_ATTR_UNOCCUP_COOL_SETPOINT = 0x0013, /* int16 */
    ZCL_THERM_SVR_ATTR_UNOCCUP_HEAT_SETPOINT = 0x0014, /* int16 */
    ZCL_THERM_SVR_ATTR_MIN_HEAT_SETPOINT = 0x0015, /* int16 [0x954d,0x7fff] */
    ZCL_THERM_SVR_ATTR_MAX_HEAT_SETPOINT = 0x0016, /* int16 [0x954d,0x7fff] */
    ZCL_THERM_SVR_ATTR_MIN_COOL_SETPOINT = 0x0017, /* int16 [0x954d,0x7fff] */
    ZCL_THERM_SVR_ATTR_MAX_COOL_SETPOINT = 0x0018, /* int16 [0x954d,0x7fff] */
    ZCL_THERM_SVR_ATTR_MIN_SETPOINT_DEADBAND = 0x0019, /* int8 [0x0a,0x19] */
    ZCL_THERM_SVR_ATTR_RMT_SENSE = 0x001A, /* map8 */
    ZCL_THERM_SVR_ATTR_CONTROL_SEQ_OPER = 0x001B, /* enum8 [00,05] - MANDATORY */
    ZCL_THERM_SVR_ATTR_SYSTEM_MODE = 0x001C, /* enum - MANDATORY */
    ZCL_THERM_SVR_ATTR_ALARM_MASK = 0x001D, /* map8 */
    ZCL_THERM_SVR_ATTR_RUNNING_MODE = 0x001E, /* enum [0,4] */

    /* Thermostat Schedule - 0x002x */
    ZCL_THERM_SVR_ATTR_START_OF_WEEK = 0x0020,
    ZCL_THERM_SVR_ATTR_NUM_WEEKLY_TRANSITIONS = 0x0021,
    ZCL_THERM_SVR_ATTR_NUM_DAILY_TRANSITIONS = 0x0022,
    ZCL_THERM_SVR_ATTR_TEMP_SETPOINT_HOLD = 0x0023,
    ZCL_THERM_SVR_ATTR_TEMP_SETPOINT_HOLD_DUR = 0x0024,
    ZCL_THERM_SVR_ATTR_PROG_OPER_MODE = 0x0025,
    ZCL_THERM_SVR_ATTR_RUNNING_STATE = 0x0029,

    /* Thermostat Setpoint Change Tracking - 0x003x */
    ZCL_THERM_SVR_ATTR_SETPOINT_CHANGE_SRC = 0x0030,
    ZCL_THERM_SVR_ATTR_SETPOINT_CHANGE_AMT = 0x0031,
    ZCL_THERM_SVR_ATTR_SETPOINT_CHANGE_TIMESTAMP = 0x0032,
    /* Reserved - 0x0033 */
    ZCL_THERM_SVR_ATTR_OCCUP_SETBACK = 0x0034,
    ZCL_THERM_SVR_ATTR_OCCUP_SETBACK_MIN = 0x0035,
    ZCL_THERM_SVR_ATTR_OCCUP_SETBACK_MAX = 0x0036,
    ZCL_THERM_SVR_ATTR_UNOCCUP_SETBACK = 0x0037,
    ZCL_THERM_SVR_ATTR_UNOCCUP_SETBACK_MIN = 0x0038,
    ZCL_THERM_SVR_ATTR_UNOCCUP_SETBACK_MAX = 0x0039,
    ZCL_THERM_SVR_ATTR_EMERGENCY_HEAT_DELTA = 0x003a,

    /* AC Information - 0x004x */
    ZCL_THERM_SVR_ATTR_AC_TYPE = 0x0040,
    ZCL_THERM_SVR_ATTR_AC_CAPACITY = 0x0041,
    ZCL_THERM_SVR_ATTR_AC_REFIGERANT_TYPE = 0x0042,
    ZCL_THERM_SVR_ATTR_AC_COMPRESSOR_TYPE = 0x0043,
    ZCL_THERM_SVR_ATTR_AC_ERROR_CODE = 0x0044,
    ZCL_THERM_SVR_ATTR_AC_LOUVER_POSITION = 0x0045,
    ZCL_THERM_SVR_ATTR_AC_COIL_TEMP = 0x0046,
    ZCL_THERM_SVR_ATTR_AC_CAPACITY_FORMAT = 0x0047
};

#define ZCL_THERM_TEMP_MIN                          (-27315) /* 0x954d = -27315 */
#define ZCL_THERM_TEMP_MAX                          (32767) /* 0x7fff = 32797 */

/* Meaning for CNTRL_SEQ */
#define ZCL_THERM_CONTROL_OPERCOOL_ONLY             0x00
#define ZCL_THERM_CONTROL_OPERCOOL_REHEAT           0x01
#define ZCL_THERM_CONTROL_OPERHEAT_ONLY             0x02
#define ZCL_THERM_CONTROL_OPERHEAT_REHEAT           0x03
#define ZCL_THERM_CONTROL_OPERCOOL_HEAT             0x04
#define ZCL_THERM_CONTROL_OPERCOOL_HEAT_REHEAT      0x05

/* Meaning for SYS_MODE */
#define ZCL_THERM_SYSMODE_OFF                       0x00
#define ZCL_THERM_SYSMODE_AUTO                      0x01
#define ZCL_THERM_SYSMODE_COOL                      0x03
#define ZCL_THERM_SYSMODE_HEAT                      0x04
#define ZCL_THERM_SYSMODE_EHEAT                     0x05
#define ZCL_THERM_SYSMODE_PRECOOL                   0x06
#define ZCL_THERM_SYSMODE_FAN                       0x07
#define ZCL_THERM_SYSMODE_DRY                       0x08
#define ZCL_THERM_SYSMODE_SLEEP                     0x09

/* Client Generated Commands */
enum {
    ZCL_THERM_CLI_SETPOINT_RAISE_LOWER = 0x00, /* mandatory */
    ZCL_THERM_CLI_SET_WEEK_SCHED = 0x01,
    ZCL_THERM_CLI_GET_WEEK_SCHED = 0x02,
    ZCL_THERM_CLI_CLEAR_WEEK_SCHED = 0x03,
    ZCL_THERM_CLI_GET_RELAY_LOG = 0x04
};

/* Server Generated Commands */
enum {
    ZCL_THERM_SVR_GET_WEEK_RSP = 0x00,
    ZCL_THERM_SVR_GET_RELAY_LOG_RSP = 0x01
};

struct ZbZclThermCliSetpointT {
    uint8_t mode;
    int8_t amount;
};

#define ZCL_THERM_NUM_TRANSITIONS_MAX               10U

struct ZbZclThermTransitionsT {
    uint16_t transition_time;
    int16_t heat_set_point; /* optional */
    int16_t cool_set_point; /* optional */
};

#define ZCL_THERM_DAY_OF_WEEK_SUNDAY                0x01
#define ZCL_THERM_DAY_OF_WEEK_MONDAY                0x02
#define ZCL_THERM_DAY_OF_WEEK_TUESDAY               0x04
#define ZCL_THERM_DAY_OF_WEEK_WEDNESDAY             0x08
#define ZCL_THERM_DAY_OF_WEEK_THURSDAY              0x10
#define ZCL_THERM_DAY_OF_WEEK_FRIDAY                0x20
#define ZCL_THERM_DAY_OF_WEEK_SATURDAY              0x40
#define ZCL_THERM_DAY_OF_WEEK_VACATION              0x80

#define ZCL_THERM_MODE_HEAT_SETPOINT_PRESENT        0x01
#define ZCL_THERM_MODE_COOL_SETPOINT_PRESENT        0x02

struct ZbZclThermWeeklySchedT {
    uint8_t num_transitions;
    uint8_t day_of_week_seq; /* e.g. ZCL_THERM_DAY_OF_WEEK_SUNDAY */
    uint8_t mode_for_seq; /* e.g. ZCL_THERM_MODE_HEAT_SETPOINT_PRESENT */
    struct ZbZclThermTransitionsT transitions[ZCL_THERM_NUM_TRANSITIONS_MAX];
};

struct ZbZclThermCliGetWeeklyT {
    uint8_t days_to_return;
    uint8_t mode_to_return;
};

/* Thermostat Client */
struct ZbZclClusterT * ZbZclThermClientAlloc(struct ZigBeeT *zb, uint8_t ep);

/* ZCL_THERM_CLI_SETPOINT_RAISE_LOWER */
enum ZclStatusCodeT ZbZclThermClientSetpointRaiseLower(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclThermCliSetpointT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_THERM_CLI_SET_WEEK_SCHED */
enum ZclStatusCodeT ZbZclThermClientSetWeeklySched(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclThermWeeklySchedT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_THERM_CLI_GET_WEEK_SCHED */
enum ZclStatusCodeT ZbZclThermClientGetWeeklySched(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclThermCliGetWeeklyT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_THERM_CLI_CLEAR_WEEK_SCHED */
enum ZclStatusCodeT ZbZclThermClientClearWeeklySched(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_THERM_CLI_GET_RELAY_LOG */
enum ZclStatusCodeT ZbZclThermClientGetRelayStatusLog(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Thermostat Server */

struct ZbZclThermSvrGetRelayStatusLogRspT {
    uint16_t time_of_day;
    uint16_t relay_status;
    int16_t local_temp;
    uint8_t humidity_percent;
    int16_t set_point;
    uint16_t unread_entries;
};

struct ZbZclThermServerCallbacksT {
    enum ZclStatusCodeT (*setpoint_raise_lower)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclThermCliSetpointT *req, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*set_weekly)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclThermWeeklySchedT *req, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*get_weekly)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclThermCliGetWeeklyT *req, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*clear_weekly)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*get_relay_status_log)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
};

struct ZbZclClusterT * ZbZclThermServerAlloc(struct ZigBeeT *zb, uint8_t ep,
    struct ZbZclThermServerCallbacksT *callbacks, void *arg);

/* ZCL_THERM_SVR_GET_WEEK_RSP */
enum ZclStatusCodeT ZbZclThermServerGetWeeklySchedRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclThermWeeklySchedT *rsp);

/* ZCL_THERM_SVR_GET_RELAY_LOG_RSP */
enum ZclStatusCodeT ZbZclThermServerGetRelayStatusLogRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclThermSvrGetRelayStatusLogRspT *rsp);

#endif /* __ZCL_TERM__H */
