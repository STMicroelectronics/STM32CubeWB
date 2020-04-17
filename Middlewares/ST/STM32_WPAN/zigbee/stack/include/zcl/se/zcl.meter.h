/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_METER_H
# define ZCL_METER_H

#include "zcl/zcl.h"

/* EXEGIN - Fixed most of them. Some stragglers remain. */
/*lint -save -e621 [ identifier clash within 31 chars - 5.1 REQUIRED ] */

/*---------------------------------------------------------------------------
 *  Metering Attributes
 *--------------------------------------------------------------------------
 */
/*
 * "Attribute identifiers are encoded such that the most significant octet
 * specifies the attribute set and the least significant octet specifies
 * the attribute within the set."
 * */
enum {
    /* Reading Information Set (0x00) */
    ZCL_METER_SVR_ATTR_CURSUM_DELIV = 0x0000,
    ZCL_METER_SVR_ATTR_CURSUM_RECV = 0x0001,
    ZCL_METER_SVR_ATTR_MAX_DMND_DELIV = 0x0002,
    ZCL_METER_SVR_ATTR_MAX_DMND_RECV = 0x0003,
    ZCL_METER_SVR_ATTR_DFT_SUM = 0x0004,
    ZCL_METER_SVR_ATTR_DAILY_FREEZE_TIME = 0x0005,
    ZCL_METER_SVR_ATTR_POWER_FACTOR = 0x0006,
    ZCL_METER_SVR_ATTR_READ_SNAPSHOT_TIME = 0x0007,
    ZCL_METER_SVR_ATTR_MAX_DMND_DELIV_TIME = 0x0008,
    ZCL_METER_SVR_ATTR_MAX_DMND_RECV_TIME = 0x0009,
    ZCL_METER_SVR_ATTR_DEFAULT_UPDATE_PERIOD = 0x000A,
    ZCL_METER_SVR_ATTR_FAST_POLL_UPDATE_PERIOD = 0x000B,
    ZCL_METER_SVR_ATTR_BLOCK_CONSUMP_DELIV = 0x000C,
    ZCL_METER_SVR_ATTR_DAILY_CONSUMPTION_TARGET = 0x000D,
    ZCL_METER_SVR_ATTR_CURBLOCK = 0x000E,
    ZCL_METER_SVR_ATTR_PROFILE_INTERVAL_PERIOD = 0x000F,
    ZCL_METER_SVR_ATTR_INTERVAL_REPORTING_PERIOD = 0x0010,
    ZCL_METER_SVR_ATTR_PRESET_READING_TIME = 0x0011,
    ZCL_METER_SVR_ATTR_VOLUME_PER_REPORT = 0x0012,
    ZCL_METER_SVR_ATTR_FLOW_RESTRICTION = 0x0013,
    ZCL_METER_SVR_ATTR_SUPPLY_STATUS = 0x0014,
    ZCL_METER_SVR_ATTR_INLET_EC_SUMMATION = 0x0015,
    ZCL_METER_SVR_ATTR_OUTLET_EC_SUMMATION = 0x0016,
    ZCL_METER_SVR_ATTR_INLET_TEMPERATURE = 0x0017,
    ZCL_METER_SVR_ATTR_OUTLET_TEMPERATURE = 0x0018,
    ZCL_METER_SVR_ATTR_CONTROL_TEMPERATURE = 0x0019,
    ZCL_METER_SVR_ATTR_INLET_EC_DEMAND = 0x001A,
    ZCL_METER_SVR_ATTR_OUTLET_EC_DEMAND = 0x001B,
    ZCL_METER_SVR_ATTR_PREVIOUS_BLOCK_DELIV = 0x001C,
    ZCL_METER_SVR_ATTR_CURBLOCK_CONSUMPTION_RECIEVED = 0x001D,
    ZCL_METER_SVR_ATTR_CURBLOCK_RECV = 0x001E,
    ZCL_METER_SVR_ATTR_DFT_SUM_RECV = 0x001F,
    ZCL_METER_SVR_ATTR_ACTREGTIER_DELIV = 0x0020, /* ActiveRegisterTierDelivered */
    ZCL_METER_SVR_ATTR_ACTREGTIER_RECV = 0x0021, /* ActiveRegisterTierReceived */
    ZCL_METER_SVR_ATTR_LAST_BLOCK_SWITCH_TIME = 0x0022,

    /* TOU Information Set (0x01) */
    /* For ZCL_METER_SVR_ATTR_TIERN_, see below. (0x0100) */

    /* Meter Status (0x02) */
    ZCL_METER_SVR_ATTR_METER_STATUS = 0x0200, /* ZCL_METER_STATUS_ */
    ZCL_METER_SVR_ATTR_RMN_BATT_LIFE = 0x0201,
    ZCL_METER_SVR_ATTR_HOURS_IN_OPERATION = 0x0202,
    ZCL_METER_SVR_ATTR_HOURS_IN_FAULT = 0x0203,
    ZCL_METER_SVR_ATTR_EXTENDED_STATUS = 0x0204,
    ZCL_METER_SVR_ATTR_RMN_BATT_DAYS = 0x0205,
    ZCL_METER_SVR_ATTR_CURMETER_ID = 0x0206,
    ZCL_METER_SVR_ATTR_AMBIENT_CONSUMPTION_INDICATOR = 0x0207,

    /* Formatting (0x03) */
    ZCL_METER_SVR_ATTR_UNIT_OF_MEASURE = 0x0300,
    ZCL_METER_SVR_ATTR_MULTIPLIER = 0x0301,
    ZCL_METER_SVR_ATTR_DIVISOR = 0x0302,
    ZCL_METER_SVR_ATTR_SUMMATION_FORMAT = 0x0303,
    ZCL_METER_SVR_ATTR_DEMAND_FORMAT = 0x0304,
    ZCL_METER_SVR_ATTR_HISTORICAL_CONSUMPTION_FORMAT = 0x0305,
    ZCL_METER_SVR_ATTR_DEVICE_TYPE = 0x0306,
    ZCL_METER_SVR_ATTR_SITE_ID = 0x0307,
    ZCL_METER_SVR_ATTR_SERIAL_NUMBER = 0x0308,
    ZCL_METER_SVR_ATTR_EC_UNIT_OF_MEASURE = 0x0309,
    ZCL_METER_SVR_ATTR_EC_SUMMATION_FORMAT = 0x030A,
    ZCL_METER_SVR_ATTR_EC_DEMAND_FORMAT = 0x030B,
    ZCL_METER_SVR_ATTR_TEMP_UNIT_OF_MEASURE = 0x030C,
    ZCL_METER_SVR_ATTR_TEMP_FORMAT = 0x030D,
    ZCL_METER_SVR_ATTR_MODULE_SERIAL_NUMBER = 0x030E,
    ZCL_METER_SVR_ATTR_OPERTRFLBL_DELIV = 0x030F, /* OperatingTariffLabelDelivered */
    ZCL_METER_SVR_ATTR_OPERTRFLBL_RECIEVED = 0x0310, /* OperatingTariffLabelReceived */
    ZCL_METER_SVR_ATTR_CUSTOMER_ID_NUMBER = 0x0311, /* Octet String (len = 25) */
    ZCL_METER_SVR_ATTR_ALT_UNIT_OF_MEASURE = 0x0312,
    ZCL_METER_SVR_ATTR_ALT_DEMAND_FORMATTING = 0x0313,
    ZCL_METER_SVR_ATTR_ALT_CONSUMPTION_FORMATTING = 0x0314,

    /* Historical Consumption (0x04) */
    ZCL_METER_SVR_ATTR_INSTANTANEOUS_DEMAND = 0x0400,
    ZCL_METER_SVR_ATTR_CURDAY_DM_DELIV = 0x0401, /* CurrentDayConsumptionDelivered */
    ZCL_METER_SVR_ATTR_CURDAY_DM_RECV = 0x0402, /* CurrentDayConsumptionReceived */
    ZCL_METER_SVR_ATTR_PRVDAY_DM_DELIV = 0x0403, /* PreviousDayConsumptionDelivered */
    ZCL_METER_SVR_ATTR_PRVDAY_DM_RECV = 0x0404, /* PreviousDayConsumptionReceived */
    ZCL_METER_SVR_ATTR_CURPRTL_DELIV_START = 0x0405, /* CurrentPartialProfileIntervalStartTimeDelivered (UTC Time) */
    ZCL_METER_SVR_ATTR_CURPRTL_RECV_START = 0x0406, /* CurrentPartialProfileIntervalStartTimeReceived (UTC Time) */
    ZCL_METER_SVR_ATTR_CURPRTL_DELIV = 0x0407,
    ZCL_METER_SVR_ATTR_CURPRTL_RECV = 0x0408,
    ZCL_METER_SVR_ATTR_CURDAY_MAX_PRESSURE = 0x0409,
    ZCL_METER_SVR_ATTR_CURDAY_MIN_PRESSURE = 0x040A,
    ZCL_METER_SVR_ATTR_PRVDAY_MAX_PRESSURE = 0x040B,
    ZCL_METER_SVR_ATTR_PRVDAY_MIN_PRESSURE = 0x040C,
    ZCL_METER_SVR_ATTR_CURDAY_MAX_DEMAND = 0x040D,
    ZCL_METER_SVR_ATTR_PRVDAY_MAX_DEMAND = 0x040E,
    ZCL_METER_SVR_ATTR_CURMTH_MAX_DEMAND = 0x040F,
    ZCL_METER_SVR_ATTR_CURYR_MAX_DEMAND = 0x0410,
    ZCL_METER_SVR_ATTR_CURDAY_EC_MAX_DEMAND = 0x0411,
    ZCL_METER_SVR_ATTR_PRVDAY_EC_MAX_DEMAND = 0x0412,
    ZCL_METER_SVR_ATTR_CURMTH_EC_MAX_DEMAND = 0x0413,
    ZCL_METER_SVR_ATTR_CURMTH_EC_MIN_DEMAND = 0x0414,
    ZCL_METER_SVR_ATTR_CURYR_EC_MAX_DEMAND = 0x0415,
    ZCL_METER_SVR_ATTR_CURYR_EC_MIN_DEMAND = 0x0416,
    /* PreviousDay[2-8]Consumption... (0x0420 to 0x042D) */
#if 0
    ZCL_METER_SVR_ATTR_PRVDAY_N_CONSUMP_DELIV
    ZCL_METER_SVR_ATTR_PRVDAY_N_RECV_CONSUMP
    /* PreviousWeek[0-5]Consumption... (0x0430 to 0x043B) */
    ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_DELIV
    ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_RECV
    /* PreviousMonth[0-13]Consumption... (0x0440 to 0x045B) */
    ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_DELIV
    ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_RECV
#endif
    ZCL_METER_SVR_ATTR_HISTORICAL_FREEZE_TIME = 0x045C,

    /* Load Profile Configuration (0x05) */
    ZCL_METER_SVR_ATTR_MAX_PERIODS_DELIV = 0x0500,

    /* Supply Limit (0x06) */
    ZCL_METER_SVR_ATTR_CURDEMAND_DELIV = 0x0600,
    ZCL_METER_SVR_ATTR_DEMAND_LIMIT = 0x0601,
    ZCL_METER_SVR_ATTR_DEMAND_INTEGRATION_PERIOD = 0x0602,
    ZCL_METER_SVR_ATTR_DEMAND_SUBINTERVALS = 0x0603,
    ZCL_METER_SVR_ATTR_SUPPLY_LIMIT_ARM_DURATION = 0x0604,
    ZCL_METER_SVR_ATTR_LOAD_LIMIT_SUPPLY_STATE = 0x0605,
    ZCL_METER_SVR_ATTR_LOAD_LIMIT_COUNTER = 0x0606,
    ZCL_METER_SVR_ATTR_SUPPLY_TAMPER_STATE = 0x0607,
    ZCL_METER_SVR_ATTR_SUPPLY_DEPLETION_STATE = 0x0608,
    ZCL_METER_SVR_ATTR_SUPPLY_UNCONTROLLED_FLOW_STATE = 0x0609,

    /* Block Information (0x07) */
    /* For ZCL_METER_SVR_ATTR_TIERN_BLOCKN_, see below. */

    /* Alarms (0x08) */
    ZCL_METER_SVR_ATTR_GENERIC_ALARM_MASK = 0x0800, /* access = read/write */
    ZCL_METER_SVR_ATTR_ELECTRICITY_ALARM_MASK = 0x0801, /* access = read/write */
    ZCL_METER_SVR_ATTR_FLOW_PRESSURE_ALARM_MASK = 0x0802, /* access = read/write */
    ZCL_METER_SVR_ATTR_WATER_ALARM_MASK = 0x0803, /* access = read/write */
    ZCL_METER_SVR_ATTR_HEAT_COOLING_ALARM_MASK = 0x0804, /* access = read/write */
    ZCL_METER_SVR_ATTR_GAS_ALARM_MASK = 0x0805, /* access = read/write */
    ZCL_METER_SVR_ATTR_EXTENDED_GENERIC_ALARM_MASK = 0x0806, /* access = read/write */
    ZCL_METER_SVR_ATTR_MANUFACTURER_ALARM_MASK = 0x0807, /* access = read/write */

    /* Meter Billing (0x0A) */
    ZCL_METER_SVR_ATTR_BTD_DELIV = 0x0A00,
    ZCL_METER_SVR_ATTR_BTD_TIMESTAMP_DELIV = 0x0A01,
    ZCL_METER_SVR_ATTR_PROJ_BILL_DELIV = 0x0A02,
    ZCL_METER_SVR_ATTR_PROJ_BILL_TIMESTAMP_DELIV = 0x0A03,
    ZCL_METER_SVR_ATTR_BILL_DELIV_TRAILING_DIGIT = 0x0A04,
    ZCL_METER_SVR_ATTR_BTD_RECV = 0x0A10,
    ZCL_METER_SVR_ATTR_BTD_TIMESTAMP_RECV = 0x0A11,
    ZCL_METER_SVR_ATTR_PROJ_BILL_RECV = 0x0A12,
    ZCL_METER_SVR_ATTR_PROJ_BILL_TIMESTAMP_RECV = 0x0A13,
    ZCL_METER_SVR_ATTR_BILL_RECV_TRAILING_DIGIT = 0x0A14,

    /* Supply Control (0x0B) */
    ZCL_METER_SVR_ATTR_PROP_CHG_SUPPLY_IMPL_TIME = 0x0B00,
    ZCL_METER_SVR_ATTR_PROP_CHG_SUPPLY_STATUS = 0x0B01,
    ZCL_METER_SVR_ATTR_UNCTLFLOW_THRESHOLD = 0x0B10, /* Uncontrolled Flow Threshold */
    ZCL_METER_SVR_ATTR_UNCTLFLOW_UNIT_OF_MEASURE = 0x0B11, /* Uncontrolled Flow Threshold Unit of Measure */
    ZCL_METER_SVR_ATTR_UNCTLFLOW_MULTIPLIER = 0x0B12, /* Uncontrolled Flow Multiplier */
    ZCL_METER_SVR_ATTR_UNCTLFLOW_DIVISOR = 0x0B13, /* Uncontrolled Flow Divisor */
    ZCL_METER_SVR_ATTR_FLOW_STABILISATION_PERIOD = 0x0B14,
    ZCL_METER_SVR_ATTR_FLOW_MEASUREMENT_PERIOD = 0x0B15

        /* Alternative Historical Consumption (0x0C) */
        /* TBD */
};

/* Simple Metering Time of Use Information Attributes */

/* Tiers start at 1 and go to a max of 48.
 * They should have started at an index of 0, but oh well. */
#define ZCL_METER_SVR_ATTR_TIERN_MAX                                48U
#define ZCL_METER_SVR_ATTR_TIERN_DELIV(_x_)                         (0x0100U + (2U * ((_x_) - 1U)))
#define ZCL_METER_SVR_ATTR_TIERN_RECV(_x_)                          (0x0101U + (2U * ((_x_) - 1U)))

/* Simple Metering Block Information Attributes (Delivered) */
#define ZCL_METER_SVR_ATTR_TIERN_BLOCKN_DELIV(_tier_, _blk_)        (0x0700U + ((_blk_) - 1U) + (0x10U * (_tier_)))
#define ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_DELIV(_blk_)              ZCL_METER_SVR_ATTR_TIERN_BLOCKN_DELIV(0U, (_blk_))

/* Simple Metering ESP Historical Attributes */
#define ZCL_METER_SVR_ATTR_PRVDAY_N_DELIV_CONSUMP(_x_)              (0x0420U + (2U * ((_x_) - 2U)))
#define ZCL_METER_SVR_ATTR_PRVDAY_N_RECV_CONSUMP(_x_)               (0x0421U + (2U * ((_x_) - 2U)))

#define ZCL_METER_SVR_ATTR_WEEK_FIRST_CONSUMP_DELIV                 0U
#define ZCL_METER_SVR_ATTR_WEEK_MAX_CONSUMP_DELIV                   6U /* less than, i.e. 0..5 */
#define ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_DELIV(_x_)                (0x0430U + (2U * (_x_)))
#define ZCL_METER_SVR_ATTR_WEEK_N_CONSUMP_RECV(_x_)                 (0x0431U + (2U * (_x_)))
#define ZCL_METER_SVR_ATTR_MONTH_FIRST_CONSUMP_DELIV                0U
#define ZCL_METER_SVR_ATTR_MONTH_MAX_CONSUMP_DELIV                  14U /* less than, i.e. 0..13 */
#define ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_DELIV(_x_)               (0x0440U + (2U * (_x_)))
#define ZCL_METER_SVR_ATTR_MONTH_N_CONSUMP_RECV(_x_)                (0x0441U + (2U * (_x_)))

/* Simple Metering Block Information Attributes - Received */
#define ZCL_METER_SVR_ATTR_TIERN_BLOCKN_RECV(_tier_, _blk_)         (0x0900U + ((_blk_) - 1U) + (0x10U * (_tier_)))
#define ZCL_METER_SVR_ATTR_NO_TIER_BLOCKN_RECV(_blk_)               ZCL_METER_SVR_ATTR_TIERN_BLOCKN_RECV(0U, _blk_)

enum ZbZclMeterClientAttrT {
    /* Notification Set (0x00) */
    /* Notification Flags (ZCL_METER_CLI_NOTIF_FLG_) */
    ZCL_METER_CLI_ATTR_NOTIF_FLG_1 = 0x0000,
    ZCL_METER_CLI_ATTR_NOTIF_FLG_2, /* 0x0001 */
    ZCL_METER_CLI_ATTR_NOTIF_FLG_3, /* 0x0002 */
    ZCL_METER_CLI_ATTR_NOTIF_FLG_4, /* 0x0003 */
    ZCL_METER_CLI_ATTR_NOTIF_FLG_5, /* 0x0004 */
    ZCL_METER_CLI_ATTR_NOTIF_FLG_6, /* 0x0005 */
    ZCL_METER_CLI_ATTR_NOTIF_FLG_7, /* 0x0006 */
    ZCL_METER_CLI_ATTR_NOTIF_FLG_8 /* 0x0007 */
};

/* ZCL_METER_CLI_ATTR_NOTIF_FLG_ */
#define ZCL_METER_CLI_NOTIF_FLG_NEW_OTA_FW                      0x00000001U /* bit  0 */
#define ZCL_METER_CLI_NOTIF_FLG_CBKE_UPDATE_REQ                 0x00000002U /* bit  1 */
#define ZCL_METER_CLI_NOTIF_FLG_TIME_SYNC                       0x00000004U /* bit  2 */
/* Reserved - 0x00000008U */
#define ZCL_METER_CLI_NOTIF_FLG_STAY_AWAKE_HAN                  0x00000010U /* bit  4 */
#define ZCL_METER_CLI_NOTIF_FLG_STAY_AWAKE_WAN                  0x00000020U /* bit  5 */
#define ZCL_METER_CLI_NOTIF_FLG_PUSH_HIST_METER_MASK            0x000001C0U /* bits 6-8 */
#define ZCL_METER_CLI_NOTIF_FLG_PUSH_HIST_PREPAY_SET            0x00000E00U /* bits 9-11 */
#define ZCL_METER_CLI_NOTIF_FLG_PUSH_STATIC_BASIC               0x00001000U /* bit 12 */
#define ZCL_METER_CLI_NOTIF_FLG_PUSH_STATIC_METER               0x00002000U /* bit 13 */
#define ZCL_METER_CLI_NOTIF_FLG_PUSH_STATIC_PREPAY              0x00004000U /* bit 14 */
#define ZCL_METER_CLI_NOTIF_FLG_NETWORK_KEY_ACTIVE              0x00008000U /* bit 15 */
#define ZCL_METER_CLI_NOTIF_FLG_DISPLAY_MESSAGE                 0x00010000U /* bit 16 */
#define ZCL_METER_CLI_NOTIF_FLG_CANCEL_ALL_MESSAGES             0x00020000U /* bit 17 */
#define ZCL_METER_CLI_NOTIF_FLG_CHANGE_SUPPLY                   0x00040000U /* bit 18 */
#define ZCL_METER_CLI_NOTIF_FLG_LOCAL_CHANGE_SUPPLY             0x00080000U /* bit 19 */
#define ZCL_METER_CLI_NOTIF_FLG_SET_UNCONTROLLED_FLOW_THRESH    0x00100000U /* bit 20 */
#define ZCL_METER_CLI_NOTIF_FLG_TUNNEL_MESSAGE_PEND             0x00200000U /* bit 21 */
#define ZCL_METER_CLI_NOTIF_FLG_GET_SNAPSHOT                    0x00400000U /* bit 22 */
#define ZCL_METER_CLI_NOTIF_FLG_GET_SAMPLED_DATA                0x00800000U /* bit 23 */
#define ZCL_METER_CLI_NOTIF_FLG_NEW_SUBGHZ_CHANNEL_MASKS        0x01000000U /* bit 24 */
#define ZCL_METER_CLI_NOTIF_FLG_ENERGY_SCAN_PEND                0x02000000U /* bit 25 */
#define ZCL_METER_CLI_NOTIF_FLG_CHANNEL_CHANGE_PEND             0x04000000U /* bit 26 */

/* Meter Status flags */
#define ZCL_METER_STATUS_CHECK_METER                0x01U
#define ZCL_METER_STATUS_LOW_BATTERY                0x02U
#define ZCL_METER_STATUS_TAMPER_DETECT              0x04U
#define ZCL_METER_STATUS_POWER_FAILURE              0x08U
#define ZCL_METER_STATUS_PIPE_EMPTY                 0x08U
#define ZCL_METER_STATUS_TEMPERATURE_SENSOR         0x08U
#define ZCL_METER_STATUS_POWER_QUALITY              0x10U
#define ZCL_METER_STATUS_LOW_PRESSURE               0x10U
#define ZCL_METER_STATUS_BURST_DETECT               0x10U
#define ZCL_METER_STATUS_LEAK_DETECT                0x20U
#define ZCL_METER_STATUS_SERVICE_DISCONNECT         0x40U
#define ZCL_METER_STATUS_REVERSE_FLOW               0x80U
#define ZCL_METER_STATUS_FLOW_SENSOR                0x80U

#define ZCL_METER_FORMAT_FRACTIONAL                 0x07U
#define ZCL_METER_FORMAT_INTEGER                    0x78U
#define ZCL_METER_FORMAT_SUPPRESS_LEADING_ZERO      0x80U
#define ZCL_METER_FORMAT_OFFSET_FRACTIONAL          0U
#define ZCL_METER_FORMAT_OFFSET_INTEGER             3U

/* Simple Metering units of measure enumeration */
enum ZbZclMeterUnitsT {
    ZCL_METER_UNITS_BINARY_KWH = 0x00,
    ZCL_METER_UNITS_BINARY_CUBIC_METERS = 0x01,
    ZCL_METER_UNITS_BINARY_CUBIC_FEET = 0x02,
    ZCL_METER_UNITS_BINARY_CCF = 0x03,
    ZCL_METER_UNITS_BINARY_US_GALLON = 0x04,
    ZCL_METER_UNITS_BINARY_IMPERIAL_GALLON = 0x05,
    ZCL_METER_UNITS_BINARY_BTU = 0x06,
    ZCL_METER_UNITS_BINARY_LITERS = 0x07,
    ZCL_METER_UNITS_BINARY_RELATIVE_KPA = 0x08,
    ZCL_METER_UNITS_BINARY_ABSOLUTE_KPA = 0x09,
    ZCL_METER_UNITS_BINARY_MCF = 0x0A, /* SE 1.1 */
    ZCL_METER_UNITS_BINARY_UNITLE = 0x0B, /* SE 1.1 */
    ZCL_METER_UNITS_BINARY_MEGA_JOULE = 0x0C, /* SE 1.1 */
    ZCL_METER_UNITS_BINARY_KVAR = 0x0D, /* SE 1.2a */

    ZCL_METER_UNITS_BCD_KWH = 0x80,
    ZCL_METER_UNITS_BCD_CUBIC_METERS = 0x81,
    ZCL_METER_UNITS_BCD_CUBIC_FEET = 0x82,
    ZCL_METER_UNITS_BCD_CCF = 0x83,
    ZCL_METER_UNITS_BCD_US_GALLON = 0x84,
    ZCL_METER_UNITS_BCD_IMPERIAL_GALLON = 0x85,
    ZCL_METER_UNITS_BCD_BTU = 0x86,
    ZCL_METER_UNITS_BCD_LITERS = 0x87,
    ZCL_METER_UNITS_BCD_RELATIVE_KPA = 0x88,
    ZCL_METER_UNITS_BCD_ABSOLUTE_KPA = 0x89,
    ZCL_METER_UNITS_BCD_MCF = 0x8A, /* SE 1.1 */
    ZCL_METER_UNITS_BCD_UNITLE = 0x8B, /* SE 1.1 */
    ZCL_METER_UNITS_BCD_MEGA_JOULE = 0x8C, /* SE 1.1 */
    ZCL_METER_UNITS_BCD_KVAR = 0x8D /* SE 1.2a */
};

/* Simple Metering device type enumerations (ZCL_METER_SVR_ATTR_DEVICE_TYPE)
 *
 * Funny how the mirrored types start with GAS at 0x80 instead
 * of 0x81. Are we really correct to assume there will never be a
 * mirrored electric meter?
 *
 * Also, SE states that the metering device type is an 8-bit bitmask,
 * but it's actually an 8-bit enumeration.
 */
enum ZbZclMeterTypeT {
    ZCL_METER_TYPE_ELECTRIC = 0x00,
    ZCL_METER_TYPE_GAS = 0x01,
    ZCL_METER_TYPE_WATER = 0x02,
#if 0 /* Deprecated as of SE 1.1 */
    ZCL_METER_TYPE_THERMAL = 0x03,
#endif
    ZCL_METER_TYPE_PRESSURE = 0x04,
    ZCL_METER_TYPE_HEAT = 0x05,
    ZCL_METER_TYPE_COOLING = 0x06,

    ZCL_METER_TYPE_END_USE_METER = 0x07, /* SE 1.1b */
    ZCL_METER_TYPE_PV_GEN = 0x08, /* SE 1.1b */
    ZCL_METER_TYPE_WIND_GEN = 0x09, /* SE 1.1b */
    ZCL_METER_TYPE_WATER_GEN = 0x0A, /* SE 1.1b */
    ZCL_METER_TYPE_MICRO_GEN = 0x0B, /* SE 1.1b */
    ZCL_METER_TYPE_SOLAR_HW = 0x0C, /* SE 1.1b */
    ZCL_METER_TYPE_ELECTRIC_PHASE_1 = 0x0D, /* SE 1.1b */
    ZCL_METER_TYPE_ELECTRIC_PHASE_2 = 0x0E, /* SE 1.1b */
    ZCL_METER_TYPE_ELECTRIC_PHASE_3 = 0x0F, /* SE 1.1b */

    ZCL_METER_TYPE_MIRR_ELECTRIC = 0x7F,
    ZCL_METER_TYPE_MIRR_GAS = 0x80,
    ZCL_METER_TYPE_MIRR_WATER = 0x81,
#if 0 /* Deprecated as of SE 1.1 */
    ZCL_METER_TYPE_MIRR_THERMAL = 0x82,
#endif
    ZCL_METER_TYPE_MIRR_PRESSURE = 0x83,
    ZCL_METER_TYPE_MIRR_HEAT = 0x84,
    ZCL_METER_TYPE_MIRR_COOLING = 0x85,

    ZCL_METER_TYPE_MIRR_END_USE_METER = 0x86, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_PV_GEN = 0x87, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_WIND_GEN = 0x88, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_WATER_GEN = 0x89, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_MICRO_GEN = 0x8A, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_SOLAR_HW = 0x8B, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_ELECPHS_1 = 0x8C, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_ELECPHS_2 = 0x8D, /* SE 1.1b */
    ZCL_METER_TYPE_MIRR_ELECPHS_3 = 0x8E /* SE 1.1b */
};

/*lint -save -e849 [ !MISRA - multiple string with same enum value. ] */
/* In the infinite wisdom of zigbee, they use the same enumeration values
 *  for different meter types (e.g. alarm code 0x03 means power failure for
 *  electric meter , but means pipe empty for water meter).
 */
enum ZbZclMeterAlarmT {
    /* Simple Metering alarm codes. */
    ZCL_METER_ALARM_CHECK_METER = 0x00,
    ZCL_METER_ALARM_LOW_BATTERY = 0x01,
    ZCL_METER_ALARM_TAMPER_DETECT = 0x02,
    ZCL_METER_ALARM_POWER_FAILURE = 0x03, /* Electricity alarm only. */
    ZCL_METER_ALARM_PIPE_EMPTY = 0x03, /* Water alarm only.  */
    ZCL_METER_ALARM_TEMPERATURE_SENSOR = 0x03, /* Heating/Cooling alarm only. */
    ZCL_METER_ALARM_POWER_QUALITY = 0x04, /* Electricity alarm only. */
    ZCL_METER_ALARM_LOW_PRESSURE = 0x04, /* Gas and Water alarms only. */
    ZCL_METER_ALARM_BURST_DETECT = 0x04, /* Heating/Cooling alarm only. */
    ZCL_METER_ALARM_LEAK_DETECT = 0x05,
    ZCL_METER_ALARM_SERVICE_DISCONNECT = 0x06,
    ZCL_METER_ALARM_REVERSE_FLOW = 0x07, /* Gas and Water alarms only. */
    ZCL_METER_ALARM_FLOW_SENSOR = 0x07, /* Heating/Cooling alarm only. */

    /* Simple Metering Electricity alarm codes. */
    ZCL_METER_ALARM_LOW_VOLTAGE_L1 = 0x10,
    ZCL_METER_ALARM_HIGH_VOLTAGE_L1 = 0x11,
    ZCL_METER_ALARM_LOW_VOLTAGE_L2 = 0x12,
    ZCL_METER_ALARM_HIGH_VOLTAGE_L2 = 0x13,
    ZCL_METER_ALARM_LOW_VOLTAGE_L3 = 0x14,
    ZCL_METER_ALARM_HIGH_VOLTAGE_L4 = 0x15,
    ZCL_METER_ALARM_OVER_CURRENT_L1 = 0x16,
    ZCL_METER_ALARM_OVER_CURRENT_L2 = 0x17,
    ZCL_METER_ALARM_OVER_CURRENT_L3 = 0x18,
    ZCL_METER_ALARM_FREQ_LOW_L1 = 0x19,
    ZCL_METER_ALARM_FREQ_HIGH_L1 = 0x1a,
    ZCL_METER_ALARM_FREQ_LOW_L2 = 0x1b,
    ZCL_METER_ALARM_FREQ_HIGH_L2 = 0x1c,
    ZCL_METER_ALARM_FREQ_LOW_L3 = 0x1d,
    ZCL_METER_ALARM_FREQ_HIGH_L3 = 0x1e,
    ZCL_METER_ALARM_GROUND_FAULT = 0x1f,
    ZCL_METER_ALARM_ELECTRIC_TAMPER_DETECT = 0x20,

    /* Simple Metering Pressure/Flow alarm codes. */
    ZCL_METER_ALARM_PRESSURE_BURST_DETECT = 0x30,
    ZCL_METER_ALARM_PRESSURE_TOO_LOW = 0x31,
    ZCL_METER_ALARM_PRESSURE_TOO_HIGH = 0x32,
    ZCL_METER_ALARM_FLOW_COMM_ERROR = 0x33,
    ZCL_METER_ALARM_FLOW_MEASUREMENT_FAULT = 0x34,
    ZCL_METER_ALARM_FLOW_REVERSE = 0x35,
    ZCL_METER_ALARM_FLOW_AIR_DETECT = 0x36,
    ZCL_METER_ALARM_FLOW_PIPE_EMPTY = 0x37,

    /* Simple Metering Heat and Cooling alarm codes. */
    ZCL_METER_ALARM_INLET_TEMPERATURE_SENSOR = 0x50,
    ZCL_METER_ALARM_OUTLET_TEMPERATURE_SENSOR = 0x51
};
/*lint -restore */

/* Simple Metering profile intervals. */
enum ZbZclMeterProfileIntervalT {
    ZCL_METER_PRFL_PERIOD_DAILY = 0x00,
    ZCL_METER_PRFL_PERIOD_HOURLY = 0x01,
    ZCL_METER_PRFL_PERIOD_30MIN = 0x02,
    ZCL_METER_PRFL_PERIOD_15MIN = 0x03,
    ZCL_METER_PRFL_PERIOD_10MIN = 0x04,
    ZCL_METER_PRFL_PERIOD_450SEC = 0x05,
    ZCL_METER_PRFL_PERIOD_300SEC = 0x06,
    ZCL_METER_PRFL_PERIOD_150SEC = 0x07
};

/* Simple Metering Get Profile Response Status. */
enum ZbZclMeterGetProfileStatusT {
    ZCL_METER_PRFL_STATUS_SUCCESS = 0x00,
    ZCL_METER_PRFL_STATUS_CHAN_UNDEFINED = 0x01,
    ZCL_METER_PRFL_STATUS_CHAN_UNSUPPORTED = 0x02,
    ZCL_METER_PRFL_STATUS_INVALID_END_TIME = 0x03,
    ZCL_METER_PRFL_STATUS_TOO_MANY_INTERVALS = 0x04,
    ZCL_METER_PRFL_STATUS_NO_INTERVALS = 0x05
};

/* Simple Metering Server Generated Commands */
enum ZbZclMeterServerCommandsT {
    ZCL_METER_SVR_CMD_GET_PROFILE_RESPONSE = 0x00,
    ZCL_METER_SVR_CMD_REQUEST_MIRROR = 0x01,
    ZCL_METER_SVR_CMD_REMOVE_MIRROR = 0x02,
    ZCL_METER_SVR_CMD_FAST_POLL_RESPONSE = 0x03,
    ZCL_METER_SVR_CMD_SCHEDULE_SNAPSHOT_RESPONSE = 0x04,
    ZCL_METER_SVR_CMD_TAKE_SNAPSHOT_RESPONSE = 0x05,
    ZCL_METER_SVR_CMD_PUBLISH_SNAPSHOT = 0x06,
    ZCL_METER_SVR_CMD_GET_SAMPLED_DATA_RESPONSE = 0x07,
    ZCL_METER_SVR_CMD_CONFIGURE_MIRROR = 0x08,
    ZCL_METER_SVR_CMD_CONFIGURE_NOTIFICATION_SCHEME = 0x09,
    ZCL_METER_SVR_CMD_CONFIGURE_NOTIFICATION_FLAG = 0x0a,
    ZCL_METER_SVR_CMD_GET_NOTIFIED_MESSAGE = 0x0b,
    ZCL_METER_SVR_CMD_SUPPLY_STATUS_RESPONSE = 0x0c,
    ZCL_METER_SVR_CMD_START_SAMPLING_RESPONSE_RESERVED = 0x0d
};

/* Simple Metering Client Generated Commands */
enum ZbZclMeterClientCommandsT {
    ZCL_METER_CLI_CMD_GET_PROFILE = 0x00,
    ZCL_METER_CLI_CMD_REQUEST_MIRROR_RESPONSE = 0x01,
    ZCL_METER_CLI_CMD_MIRROR_REMOVED = 0x02,
    ZCL_METER_CLI_CMD_REQUEST_FAST_POLL_MODE = 0x03,
    ZCL_METER_CLI_CMD_SCHEDULE_SNAPSHOT = 0x04,
    ZCL_METER_CLI_CMD_TAKE_SNAPSHOT = 0x05,
    ZCL_METER_CLI_CMD_GET_SNAPSHOT = 0x06,
    ZCL_METER_CLI_CMD_START_SAMPLING = 0x07,
    ZCL_METER_CLI_CMD_GET_SAMPLED_DATA = 0x08,
    ZCL_METER_CLI_CMD_MIRROR_REPORT_ATTRIBUTE_RESPONSE = 0x09,
    ZCL_METER_CLI_CMD_RESET_LOADLIMIT_COUNTER = 0x0a,
    ZCL_METER_CLI_CMD_CHANGE_SUPPLY = 0x0b,
    ZCL_METER_CLI_CMD_LOCAL_CHANGE_SUPPLY = 0x0c,
    ZCL_METER_CLI_CMD_SET_SUPPLY_STATUS = 0x0d,
    ZCL_METER_CLI_CMD_SET_UNCONTROLLED_FLOW_THRESHOLD = 0x0e
};

/* Sample Type Enumerations. These are the same as Interval Channel Values */
enum ZbZclMeterSampleTypeT {
    ZCL_METER_SAMPLE_TYPE_CONSUMP_DELIV = 0x00,
    ZCL_METER_SAMPLE_TYPE_CONSUMP_RECV, /* 0x01 */
    ZCL_METER_SAMPLE_TYPE_REACTIVE_CONSUMP_DELIV, /* 0x02 */
    ZCL_METER_SAMPLE_TYPE_REACTIVE_CONSUMP_RECV, /* 0x03 */
    ZCL_METER_SAMPLE_TYPE_INSTANTANEOUS_DEMAND /* 0x04 */
};

#if 0 /* not used */
struct ZbZclMeterSampleT {
    unsigned long long sumDelivered;
    unsigned long long sumReceived;
    unsigned long long demandDelivered;
    unsigned long long demandReceived;
    signed char powerFactor;
    int currentTier;
    int currentBlock;
};
#endif

struct ZbZclMeterClientGetProfileReqT {
    enum ZbZclMeterSampleTypeT interval_channel;
    uint32_t end_time;
    uint8_t number_of_periods;
};

struct ZbZclMeterServerGetProfileRspT {
    uint32_t end_time;
    enum ZbZclMeterGetProfileStatusT status;
    uint8_t profile_interval_period;
    uint8_t number_of_periods;
    uint8_t *profile_data; /* ZbZclMeterFormSampledData */
    unsigned int profile_length;
};

struct ZbZclMeterClientGetSampledDataReqT {
    uint16_t sample_id;
    uint32_t earliest_sample_time;
    enum ZbZclMeterSampleTypeT sample_type;
    uint16_t number_of_samples;
};

struct ZbZclMeterServerGetSampledDataRspT {
    uint16_t sample_id;
    uint32_t sample_start_time;
    enum ZbZclMeterSampleTypeT sample_type;
    uint16_t sample_request_interval;
    uint16_t number_of_samples;
    uint8_t *sample_data; /* can use ZbZclMeterFormSampledData */
    unsigned int sample_length;
};

enum ZbZclMeterSupplyStatusT {
    ZCL_METER_SUPPLY_STATUS_OFF = 0x00,
    ZCL_METER_SUPPLY_STATUS_OFF_ARMED = 0x01,
    ZCL_METER_SUPPLY_STATUS_ON = 0x02
};

struct ZbZclMeterClientLocalChangeSupplyReqT {
    enum ZbZclMeterSupplyStatusT prop_supply_status;
};

/*---------------------------------------------------------------
 * Metering Server Functions
 *---------------------------------------------------------------
 */
struct ZbZclMeterServerCallbacksT {
    /* Configure callback to handle ZCL_METER_CLI_CMD_GET_PROFILE.
     * In handling of get_profile(), the application should call
     * ZbZclMeterServerSendGetProfileRsp to send the response if successful.
     * Otherwise, the application can return an error status, which will cause
     * a Default Response to be sent with that status. */
    enum ZclStatusCodeT (*get_profile)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclMeterClientGetProfileReqT *req, struct ZbZclAddrInfoT *srcInfo);

    /* Configure callback to handle ZCL_METER_CLI_CMD_GET_SAMPLED_DATA.
     * In handling of get_sampled_data(), the application should call
     * ZbZclMeterServerSendGetSampledDataRsp to send the response if successful.
     * Otherwise, the application can return an error status, which will cause
     * a Default Response to be sent with that status. */
    enum ZclStatusCodeT (*get_sampled_data)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclMeterClientGetSampledDataReqT *req, struct ZbZclAddrInfoT *srcInfo);

    /* Configure callback to handle ZCL_METER_CLI_CMD_LOCAL_CHANGE_SUPPLY.
     * In handling of local_change_supply, application returns the status code
     * to include in the Default Response. */
    enum ZclStatusCodeT (*local_change_supply)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclMeterClientLocalChangeSupplyReqT *req, struct ZbZclAddrInfoT *srcInfo);

    /* Configure callback to handle ZCL_METER_COMMAND_REQUEST_MIRROR command.
     * Callback will register an endpoint for the mirrored metering cluster,
     * and return the endpoint ID or 0xFFFF on error. */
    uint16_t (*request_mirror)(void *arg, struct ZbZclClusterT *clusterPtr,
        struct ZbZclAddrInfoT *srcInfo, struct ZbApsdeDataIndT *dataIndPtr);

    uint16_t (*remove_mirror)(void *arg, struct ZbZclClusterT *clusterPtr,
        struct ZbZclAddrInfoT *srcInfo, struct ZbApsdeDataIndT *dataIndPtr);

    /* Optional commands not parsed into a struct or specific callback. If this is NULL,
     * the cluster will return a Default Response with status of ZCL_STATUS_UNSUPP_CLUSTER_COMMAND.
     * The application can also return that status value to include in the Default Response, or
     * any other ZCL status (ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE means no Default Response). */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
};

/* Allocate the Metering Server Cluster.
 * After initialization, ZbZclClusterSetCallbackArg is called with 'arg'
 * to configure the application's callback argument. */
struct ZbZclClusterT * ZbZclMeterServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMeterServerCallbacksT *callbacks, void *arg);

/* Converts the profile interval period enumerated value to a time in seconds,
 * or -1 on error. */
int ZbZclMeterGetProfileIntervalPeriod(uint8_t profile_interval_id);

/* Helper function to send the Get Profile Response Command
 * (get_profile callback, id = ZCL_METER_SVR_CMD_GET_PROFILE_RESPONSE) */
enum ZclStatusCodeT ZbZclMeterServerSendGetProfileRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclMeterServerGetProfileRspT *rsp);

/* Helper function to convert an array of 24-bit integers to the format
 * that gets sent in the ZigBee frame. Returns the number of octets
 * written to sample_data. */
int ZbZclMeterFormSampledData(uint8_t *sample_data, unsigned int max_len, uint32_t *samples, uint16_t num_samples);

/* Helper function to send the Get Sampled Data Response Command
 * (get_sampled_data callback, id = ZCL_METER_SVR_CMD_GET_SAMPLED_DATA_RESPONSE) */
enum ZclStatusCodeT ZbZclMeterServerSendGetSampledDataRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclMeterServerGetSampledDataRspT *rsp);

/*---------------------------------------------------------------
 * Metering Server Mirror Functions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclMeterServerMirrorAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    uint64_t mirrorAddr, uint8_t mirrorEndpt);

/* ZbZclMeterServerMirrorConfig enables mirroring on this endpoint, allowing
 * reports from the srcExtAddr address to our dstEndPt endpoint to be
 * handled as updates to this mirroring cluster. */
void ZbZclMeterServerMirrorConfig(struct ZbZclClusterT *clusterPtr, uint64_t srcExtAddr, uint8_t dstEndpt);

/*---------------------------------------------------------------
 * Metering Client Functions
 *---------------------------------------------------------------
 */

struct ZbZclMeterClientCallbacksT {
    /* Optional commands not parsed into a struct or specific callback. If this is NULL,
     * the cluster will return a Default Response with status of ZCL_STATUS_UNSUPP_CLUSTER_COMMAND.
     * The application can also return that status value to include in the Default Response, or
     * any other ZCL status (ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE means no Default Response). */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
};

struct ZbZclClusterT * ZbZclMeterClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMeterClientCallbacksT *callbacks, void *arg);

uint8_t ZbZclMeterClientCommandGetProfileReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclMeterClientGetProfileReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

uint8_t ZbZclMeterClientCommandGetSampledDataReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclMeterClientGetSampledDataReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

uint8_t ZbZclMeterClientCommandLocalChangeSupplyReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclMeterClientLocalChangeSupplyReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclMeterClientAttrQueryT {
    uint16_t attributeId;
    uint8_t dataType;
    uint8_t status;
    union {
        /* Reading Information Set */
        uint64_t currentSummationDelivered;
        uint64_t currentSummationReceived;
        uint64_t currentMaxDemandDelivered;
        uint64_t currentMaxDemandReceived;
        uint64_t currentDftSummation;
        uint16_t dailyFreezeTime;
        int8_t powerFactor;
        uint32_t readingSnapshotTime;
        uint32_t currentMaxDemandDeliveredTime;
        uint32_t currentMaxDemandReceivedTime;
        uint64_t currentBlockPeriodConsumptionDelivered; /* HACK */
        /* Time-of-Use Information Set */
        uint64_t tierSummationDelivered;
        uint64_t tierSummationReceived;
        /* Meter Status Information Set */
        uint8_t meterStatus;
        /* Formatting Information Set */
        uint8_t unitOfMeasure;
        uint32_t multiplier;
        uint32_t divisor;
        uint8_t summationFormatting;
        uint8_t demandFormatting;
        uint8_t historicalFormatting;
        uint8_t meterType;
        /* Historical Information Set */
        int32_t instantaneousDemand;
        uint32_t dailyDemandDelivered;
        uint32_t dailyDemandReceived;
        uint32_t yesterdayDemandDelivered;
        uint32_t yesterdayDemandReceived;
        uint32_t partialDeliveredStartTime;
        uint32_t partialReceivedStartTime;
        uint32_t partialDemandDelivered;
        uint32_t partialDemandReceived;
        uint8_t maxPeriodsDelivered;
        /* Supply Limit Information Set */
        uint32_t currentDemandDelivered;
        uint32_t demandLimit;
        uint8_t demandIntegrationPeriod;
        uint8_t demandSubintervals;
    } data;
};

/*lint -restore */

#endif /* __ZCL_METER_H */
