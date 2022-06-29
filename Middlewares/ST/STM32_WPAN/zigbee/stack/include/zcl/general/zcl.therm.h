/**
 * @file zcl.therm.h
 * @heading Thermostat
 * @brief ZCL Thermostat cluster header
 * ZCL 7 section 6.3
 * ZCL 8 section 6.3
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_THERM_H
#define ZCL_THERM_H

/* @PICS.ZCL.Thermostat
 * TSTAT.S | Server | True
 * TSTAT.C | Client | True
 *
 * Server Attributes
 * TSTAT.S.A0000 | LocalTemperature | True
 * TSTAT.S.A0000.Report.Tx | LocalTemperature | True
 * TSTAT.S.A0001 | OutdoorTemperature | True | Optional
 * TSTAT.S.A0002 | Occupancy | False | Optional
 * TSTAT.S.A0003 | AbsMinHeatSetpointLimit | False | Optional
 * TSTAT.S.A0004 | AbsMaxHeatSetpointLimit | False | Optional
 * TSTAT.S.A0005 | AbsMinCoolSetpointLimit | False | Optional
 * TSTAT.S.A0006 | AbsMaxCoolSetpointLimit | False | Optional
 * TSTAT.S.A0007 | PICoolingDemand | False | Optional
 * TSTAT.S.A0007.Report.Tx | PICoolingDemand | False | Optional
 * TSTAT.S.A0008 | PIHeatingDemand | False | Optional
 * TSTAT.S.A0008.Report.Tx | PIHeatingDemand | False | Optional
 * TSTAT.S.A0009 | HVACSystemTypeConfiguration | False | Optional
 * TSTAT.S.A0010 | LocalTemperatureCalibration | False | Optional
 * TSTAT.S.A0011 | OccupiedCoolingSetpoint | True
 * TSTAT.S.A0011.Scene | OccupiedCoolingSetpoint | True
 * TSTAT.S.A0012 | OccupiedHeatingSetpoint | True
 * TSTAT.S.A0012.Scene | OccupiedHeatingSetpoint | True
 * TSTAT.S.A0013 | UnoccupiedCoolingSetpoint | False | Optional
 * TSTAT.S.A0014 | UnoccupiedHeatingSetpoint | False | Optional
 * TSTAT.S.A0015 | MinHeatSetpointLimit | False | Optional
 * TSTAT.S.A0016 | MaxHeatSetpointLimit | False | Optional
 * TSTAT.S.A0017 | MinCoolSetpointLimit | False | Optional
 * TSTAT.S.A0018 | MaxCoolSetpointLimit | False | Optional
 * TSTAT.S.A0019 | MinSetpointDeadBand | False | Optional
 * TSTAT.S.A001A | RemoteSensing | False | Optional
 * TSTAT.S.A001B | ControlSequenceOfOperation | True
 * TSTAT.S.A001C | SystemMode | True
 * TSTAT.S.A001C.Scene | SystemMode | True
 * TSTAT.S.A001D | AlarmMask | False | Optional
 * TSTAT.S.A001E | ThermostatRunningMode | False | Optional
 * TSTAT.S.A0020 | StartOfWeek | False | Optional
 * TSTAT.S.A0021 | NumberOfWeeklyTransitions | False | Optional
 * TSTAT.S.A0022 | NumberOfDailyTransitions | False| Optional
 * TSTAT.S.A0023 | TemperatureSetpointHold | False | Optional
 * TSTAT.S.A0024 | TemperatureSetpointHoldDuration | False | Optional
 * TSTAT.S.A0025 | ThermostatProgrammingOperationMode | False | Optional
 * TSTAT.S.A0029 | ThermostatRunningState | False | Optional
 * TSTAT.S.A0030 | SetpointChangeSource | False | Optional
 * TSTAT.S.A0031 | SetpointChangeAmount | False | Optional
 * TSTAT.S.A0032 | SetpointChangeSourceTimestamp | False | Optional
 * TSTAT.S.A0034 | OccupiedSetback | False | Optional
 * TSTAT.S.A0035 | OccupiedSetbackMin | False | Optional
 * TSTAT.S.A0036 | OccupiedSetbackMax | False | Optional
 * TSTAT.S.A0037 | UnoccupiedSetback | False | Optional
 * TSTAT.S.A0038 | UnoccupiedSetbackMin | False | Optional
 * TSTAT.S.A0039 | UnoccupiedSetbackMax | False | Optional
 * TSTAT.S.A003a | EmergencyHeatDelta | False | Optional
 * TSTAT.S.A0040 | ACType | False | Optional
 * TSTAT.S.A0041 | ACCapacity | False | Optional
 * TSTAT.S.A0042 | ACRefrigerantType | False | Optional
 * TSTAT.S.A0043 | ACCompressorType | False | Optional
 * TSTAT.S.A0044 | ACErrorCode | False | Optional
 * TSTAT.S.A0045 | ACLouverPosition | False | Optional
 * TSTAT.S.A0046 | ACCoilTemperature | False | Optional
 * TSTAT.S.A0047 | ACCapacityFormat | False | Optional
 * TSTAT.S.Afffd | ClusterRevision | True
 * TSTAT.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * TSTAT.S.C00.Rsp | Setpoint Raise/Lower | True
 * TSTAT.S.C01.Rsp | Set Weekly Schedule | True
 * TSTAT.S.C02.Rsp | Get Weekly Schedule | True
 * TSTAT.S.C03.Rsp | Clear Weekly Schedule | True
 * TSTAT.S.C04.Rsp | Get Relay Status Log | True
 *
 * Commands Generated
 * TSTAT.S.C00.Tx | Get Weekly Schedule Response | True
 * TSTAT.S.C01.Tx | Get Relay Status Log Response | True
 *
 * Client Attributes
 * TSTAT.C.A0000.Report.Rsp | False
 * TSTAT.C.A0007.Report.Rsp | False
 * TSTAT.C.A0008.Report.Rsp | False
 * TSTAT.C.Afffd | ClusterRevision | True
 * TSTAT.C.Afffd | AttributeReportingStatus | True
 *
 * Commands Received
 * TSTAT.C.C00.Rsp | Get Weekly Schedule Response | True
 * TSTAT.C.C01.Rsp | Get Relay Status Log Response | True
 *
 * Commands Generated
 * TSTAT.C.C00.Tx | Setpoint Raise/Lower | True
 * TSTAT.C.C01.Tx | Set Weekly Schedule | True
 * TSTAT.C.C02.Tx | Get Weekly Schedule | True
 * TSTAT.C.C03.Tx | Clear Weekly Schedule | True
 * TSTAT.C.C04.Tx | Get Relay Status Log | True
 */

#include "zcl/zcl.h"

/** Thermostat Attribute IDs */
enum ZbZclThermAttrT {
    /* Thermostat Info - 0x000x */
    ZCL_THERM_SVR_ATTR_LOCAL_TEMP = 0x0000, /**< LocalTemperature */
    ZCL_THERM_SVR_ATTR_OUTDOOR_TEMP = 0x0001, /**< OutdoorTemperature (Optional) */
    ZCL_THERM_SVR_ATTR_OCCUPANCY = 0x0002, /**< Occupancy (Optional) */
    ZCL_THERM_SVR_ATTR_ABS_MIN_HEAT_SETPOINT_LIMIT = 0x0003, /**< AbsMinHeatSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_ABS_MAX_HEAT_SETPOINT_LIMIT = 0x0004, /**< AbsMaxHeatSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_ABS_MIN_COOL_SETPOINT_LIMIT = 0x0005, /**< AbsMinCoolSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_ABS_MAX_COOL_SETPOINT_LIMIT = 0x0006, /**< AbsMaxCoolSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_PI_COOLING_DEMAND = 0x0007, /**< PICoolingDemand (Optional) */
    ZCL_THERM_SVR_ATTR_PI_HEATING_DEMAND = 0x0008, /**< PIHeatingDemand (Optional) */
    ZCL_THERM_SVR_ATTR_HVAC_SYSTYPE_CONFIG = 0x0009, /**< HVACSystemTypeConfiguration (Optional) */

    /* Thermostat Settings - 0x001x */
    ZCL_THERM_SVR_ATTR_LOCAL_TEMP_CALIB = 0x0010, /**< LocalTemperatureCalibration (Optional) */
    ZCL_THERM_SVR_ATTR_OCCUP_COOL_SETPOINT = 0x0011, /**< OccupiedCoolingSetpoint */
    ZCL_THERM_SVR_ATTR_OCCUP_HEAT_SETPOINT = 0x0012, /**< OccupiedHeatingSetpoint */
    ZCL_THERM_SVR_ATTR_UNOCCUP_COOL_SETPOINT = 0x0013, /**< UnoccupiedCoolingSetpoint (Optional) */
    ZCL_THERM_SVR_ATTR_UNOCCUP_HEAT_SETPOINT = 0x0014, /**< UnoccupiedHeatingSetpoint (Optional) */
    ZCL_THERM_SVR_ATTR_MIN_HEAT_SETPOINT = 0x0015, /**< MinHeatSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_MAX_HEAT_SETPOINT = 0x0016, /**< MaxHeatSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_MIN_COOL_SETPOINT = 0x0017, /**< MinCoolSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_MAX_COOL_SETPOINT = 0x0018, /**< MaxCoolSetpointLimit (Optional) */
    ZCL_THERM_SVR_ATTR_MIN_SETPOINT_DEADBAND = 0x0019, /**< MinSetpointDeadBand (Optional) */
    ZCL_THERM_SVR_ATTR_RMT_SENSE = 0x001A, /**< RemoteSensing (Optional) */
    ZCL_THERM_SVR_ATTR_CONTROL_SEQ_OPER = 0x001B, /**< ControlSequenceOfOperation */
    ZCL_THERM_SVR_ATTR_SYSTEM_MODE = 0x001C, /**< SystemMode */
    ZCL_THERM_SVR_ATTR_ALARM_MASK = 0x001D, /**< AlarmMask (Optional) */
    ZCL_THERM_SVR_ATTR_RUNNING_MODE = 0x001E, /**< ThermostatRunningMode (Optional) */

    /* Thermostat Schedule - 0x002x */
    ZCL_THERM_SVR_ATTR_START_OF_WEEK = 0x0020, /**< StartOfWeek (Optional) */
    ZCL_THERM_SVR_ATTR_NUM_WEEKLY_TRANSITIONS = 0x0021, /**< NumberOfWeeklyTransitions (Optional) */
    ZCL_THERM_SVR_ATTR_NUM_DAILY_TRANSITIONS = 0x0022, /**< NumberOfDailyTransitions (Optional) */
    ZCL_THERM_SVR_ATTR_TEMP_SETPOINT_HOLD = 0x0023, /**< TemperatureSetpointHold (Optional) */
    ZCL_THERM_SVR_ATTR_TEMP_SETPOINT_HOLD_DUR = 0x0024, /**< TemperatureSetpointHoldDuration (Optional) */
    ZCL_THERM_SVR_ATTR_PROG_OPER_MODE = 0x0025, /**< ThermostatProgrammingOperationMode (Optional) */
    ZCL_THERM_SVR_ATTR_RUNNING_STATE = 0x0029, /**< ThermostatRunningState (Optional) */

    /* Thermostat Setpoint Change Tracking - 0x003x */
    ZCL_THERM_SVR_ATTR_SETPOINT_CHANGE_SRC = 0x0030, /**< SetpointChangeSource */
    ZCL_THERM_SVR_ATTR_SETPOINT_CHANGE_AMT = 0x0031, /**< SetpointChangeAmount */
    ZCL_THERM_SVR_ATTR_SETPOINT_CHANGE_TIMESTAMP = 0x0032, /**< SetpointChangeSourceTimestamp */
    /* Reserved - 0x0033 */
    ZCL_THERM_SVR_ATTR_OCCUP_SETBACK = 0x0034, /**< OccupiedSetback (Optional) */
    ZCL_THERM_SVR_ATTR_OCCUP_SETBACK_MIN = 0x0035, /**< OccupiedSetbackMin (Optional) */
    ZCL_THERM_SVR_ATTR_OCCUP_SETBACK_MAX = 0x0036, /**< OccupiedSetbackMax (Optional) */
    ZCL_THERM_SVR_ATTR_UNOCCUP_SETBACK = 0x0037, /**< UnoccupiedSetback (Optional) */
    ZCL_THERM_SVR_ATTR_UNOCCUP_SETBACK_MIN = 0x0038, /**< UnoccupiedSetbackMin (Optional) */
    ZCL_THERM_SVR_ATTR_UNOCCUP_SETBACK_MAX = 0x0039, /**< UnoccupiedSetbackMax (Optional) */
    ZCL_THERM_SVR_ATTR_EMERGENCY_HEAT_DELTA = 0x003a, /**< EmergencyHeatDelta (Optional) */

    /* AC Information - 0x004x */
    ZCL_THERM_SVR_ATTR_AC_TYPE = 0x0040, /**< ACType */
    ZCL_THERM_SVR_ATTR_AC_CAPACITY = 0x0041, /**< ACCapacity */
    ZCL_THERM_SVR_ATTR_AC_REFIGERANT_TYPE = 0x0042, /**< ACRefrigerantType */
    ZCL_THERM_SVR_ATTR_AC_COMPRESSOR_TYPE = 0x0043, /**< ACCompressorType */
    ZCL_THERM_SVR_ATTR_AC_ERROR_CODE = 0x0044, /**< ACErrorCode */
    ZCL_THERM_SVR_ATTR_AC_LOUVER_POSITION = 0x0045, /**< ACLouverPosition */
    ZCL_THERM_SVR_ATTR_AC_COIL_TEMP = 0x0046, /**< ACCoilTemperature */
    ZCL_THERM_SVR_ATTR_AC_CAPACITY_FORMAT = 0x0047 /**< ACCapacityFormat */
};

#define ZCL_THERM_TEMP_MIN                          (-27315) /* 0x954d = -27315 */
#define ZCL_THERM_TEMP_MAX                          (32767) /* 0x7fff = 32797 */
#define ZCL_THERM_TEMP_LOCAL_DEFAULT                0x00ff /* 0x00ff = 2.55 deg C */

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

/* Thermostat Client Commands */
enum {
    ZCL_THERM_CLI_SETPOINT_RAISE_LOWER = 0x00, /* mandatory */
    ZCL_THERM_CLI_SET_WEEK_SCHED = 0x01,
    ZCL_THERM_CLI_GET_WEEK_SCHED = 0x02,
    ZCL_THERM_CLI_CLEAR_WEEK_SCHED = 0x03,
    ZCL_THERM_CLI_GET_RELAY_LOG = 0x04
};

/* Thermostat Server Commands */
enum {
    ZCL_THERM_SVR_GET_WEEK_RSP = 0x00,
    ZCL_THERM_SVR_GET_RELAY_LOG_RSP = 0x01
};

/* Thermostat Client */

#define ZCL_THERM_NUM_TRANSITIONS_MAX               10U

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

/**
 * Create a new instance of the Scenes Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclThermClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/** Thermostat Setpoint structure */
struct ZbZclThermCliSetpointT {
    uint8_t mode; /**< Mode */
    int8_t amount; /**< Amount */
};

/**
 * Send a Setpoint Raise/Lower command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Thermostat Setpoint structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclThermClientSetpointRaiseLower(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclThermCliSetpointT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Thermostat Transition structure */
struct ZbZclThermTransitionsT {
    uint16_t transition_time; /**< Transition Time */
    int16_t heat_set_point; /**< Heat Set Point (optional) */
    int16_t cool_set_point; /**< Cool Set Point (optional) */
};

/** Thermostat Weekly Schedule structure */
struct ZbZclThermWeeklySchedT {
    uint8_t num_transitions; /**< Number of Transitions for Sequence */
    uint8_t day_of_week_seq; /**< Day of the Week for Sequence - e.g. ZCL_THERM_DAY_OF_WEEK_SUNDAY */
    uint8_t mode_for_seq; /**< Mode for Sequence - e.g. ZCL_THERM_MODE_HEAT_SETPOINT_PRESENT */
    struct ZbZclThermTransitionsT transitions[ZCL_THERM_NUM_TRANSITIONS_MAX]; /**< List of transitions */
};

/**
 * Send a Set Weekly Schedule command (Optional)
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Thermostat Weekly Schedule structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclThermClientSetWeeklySched(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclThermWeeklySchedT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Thermostat Get Weekly Schedule structure */
struct ZbZclThermCliGetWeeklyT {
    uint8_t days_to_return; /**< Days to Return */
    uint8_t mode_to_return; /**< Mode to Return */
};

/**
 * Send a Get Weekly Schedule command (Optional)
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Thermostat Get Weekly Schedule info
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclThermClientGetWeeklySched(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclThermCliGetWeeklyT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Clear Weekly Schedule command (Optional)
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclThermClientClearWeeklySched(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Relay Status Log command (Optional)
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclThermClientGetRelayStatusLog(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Thermostat Server */

/** Thermostat Server callbacks configuration */
struct ZbZclThermServerCallbacksT {
    enum ZclStatusCodeT (*setpoint_raise_lower)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclThermCliSetpointT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to handle Setpoint Raise/Lower command response */

    enum ZclStatusCodeT (*set_weekly)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclThermWeeklySchedT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to handle Set Weekly Schedule command response */

    enum ZclStatusCodeT (*get_weekly)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclThermCliGetWeeklyT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to handle Get Weekly Schedule command response */

    enum ZclStatusCodeT (*clear_weekly)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to handle Clear Weekly Schedule command response */

    enum ZclStatusCodeT (*get_relay_status_log)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to handle Get Relay Status Log command response */
};

/**
 * Create a new instance of the Thermostat Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclThermServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclThermServerCallbacksT *callbacks, void *arg);

/**
 * Send a Get Weekly Schedule Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param rsp Server Clear Get Weekly Schedule response
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclThermServerGetWeeklySchedRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclThermWeeklySchedT *rsp);

/** Get Relay Status Log Response structure */
struct ZbZclThermSvrGetRelayStatusLogRspT {
    uint16_t time_of_day; /**< Time of Day */
    uint16_t relay_status; /**< Relay Status */
    int16_t local_temp; /**< Local Temperature */
    uint8_t humidity_percent; /**< Humidity in Percentage */
    int16_t set_point; /*8< Set Point */
    uint16_t unread_entries; /**< Unread Entries */
};

/**
 * Send a Get Relay Status Log Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param rsp Get Relay Status Log Response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclThermServerGetRelayStatusLogRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclThermSvrGetRelayStatusLogRspT *rsp);

#endif
