/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_ELEC_MEAS_H
#define ZCL_ELEC_MEAS_H

#include "zcl/zcl.h"

/* PICS.ZCL.Electrical.Measurement
 * EMR.S | True
 * EMR.C | True
 *
 * Server Attributes
 * EMR.S.A0000 | True
 * EMR.S.A0100 | False
 * EMR.S.A0101 | False
 * EMR.S.A0102 | False
 * EMR.S.A0103 | False
 * EMR.S.A0104 | False
 * EMR.S.A0105 | False
 * EMR.S.A0106 | False
 * EMR.S.A0107 | False
 * EMR.S.A0108 | False
 * EMR.S.A0200 | False
 * EMR.S.A0201 | False
 * EMR.S.A0202 | False
 * EMR.S.A0203 | False
 * EMR.S.A0204 | False
 * EMR.S.A0205 | False
 * EMR.S.A0300 | False
 * EMR.S.A0301 | False
 * EMR.S.A0302 | False
 * EMR.S.A0303 | False
 * EMR.S.A0304 | False
 * EMR.S.A0305 | False
 * EMR.S.A0306 | False
 * EMR.S.A0307 | False
 * EMR.S.A0308 | False
 * EMR.S.A0309 | False
 * EMR.S.A030a | False
 * EMR.S.A030b | False
 * EMR.S.A030c | False
 * EMR.S.A030d | False
 * EMR.S.A030e | False
 * EMR.S.A030f | False
 * EMR.S.A0310 | False
 * EMR.S.A0311 | False
 * EMR.S.A0312 | False
 * EMR.S.A0400 | False
 * EMR.S.A0401 | False
 * EMR.S.A0402 | False
 * EMR.S.A0403 | False
 * EMR.S.A0404 | False
 * EMR.S.A0405 | False
 * EMR.S.A0501 | False
 * EMR.S.A0502 | False
 * EMR.S.A0503 | False
 * EMR.S.A0505 | False
 * EMR.S.A0506 | False
 * EMR.S.A0507 | False
 * EMR.S.A0508 | False
 * EMR.S.A0509 | False
 * EMR.S.A050a | False
 * EMR.S.A050b | False
 * EMR.S.A050c | False
 * EMR.S.A050d | False
 * EMR.S.A050e | False
 * EMR.S.A050f | False
 * EMR.S.A0510 | False
 * EMR.S.A0511 | False
 * EMR.S.A0512 | False
 * EMR.S.A0513 | False
 * EMR.S.A0514 | False
 * EMR.S.A0515 | False
 * EMR.S.A0516 | False
 * EMR.S.A0517 | False
 * EMR.S.A0600 | False
 * EMR.S.A0601 | False
 * EMR.S.A0602 | False
 * EMR.S.A0603 | False
 * EMR.S.A0604 | False
 * EMR.S.A0605 | False
 * EMR.S.A0700 | False
 * EMR.S.A0701 | False
 * EMR.S.A0702 | False
 * EMR.S.A0800 | False
 * EMR.S.A0801 | False
 * EMR.S.A0802 | False
 * EMR.S.A0803 | False
 * EMR.S.A0804 | False
 * EMR.S.A0805 | False
 * EMR.S.A0806 | False
 * EMR.S.A0807 | False
 * EMR.S.A0808 | False
 * EMR.S.A0809 | False
 * EMR.S.A080a | False
 * EMR.S.A0901 | False
 * EMR.S.A0902 | False
 * EMR.S.A0903 | False
 * EMR.S.A0905 | False
 * EMR.S.A0906 | False
 * EMR.S.A0907 | False
 * EMR.S.A0908 | False
 * EMR.S.A0909 | False
 * EMR.S.A090a | False
 * EMR.S.A090b | False
 * EMR.S.A090c | False
 * EMR.S.A090d | False
 * EMR.S.A090e | False
 * EMR.S.A090f | False
 * EMR.S.A0910 | False
 * EMR.S.A0911 | False
 * EMR.S.A0912 | False
 * EMR.S.A0913 | False
 * EMR.S.A0914 | False
 * EMR.S.A0915 | False
 * EMR.S.A0916 | False
 * EMR.S.A0917 | False
 * EMR.S.A0a01 | False
 * EMR.S.A0a02 | False
 * EMR.S.A0a03 | False
 * EMR.S.A0a05 | False
 * EMR.S.A0a06 | False
 * EMR.S.A0a07 | False
 * EMR.S.A0a08 | False
 * EMR.S.A0a09 | False
 * EMR.S.A0a0a | False
 * EMR.S.A0a0b | False
 * EMR.S.A0a0c | False
 * EMR.S.A0a0d | False
 * EMR.S.A0a0e | False
 * EMR.S.A0a0f | False
 * EMR.S.A0a10 | False
 * EMR.S.A0a11 | False
 * EMR.S.A0a12 | False
 * EMR.S.A0a13 | False
 * EMR.S.A0a14 | False
 * EMR.S.A0a15 | False
 * EMR.S.A0a16 | False
 * EMR.S.A0a17 | False
 * EMR.S.Afffd | True
 * EMR.S.Afffe | False

 * Commands Received
 * EMR.S.C00.Rsp | depends on customer application
 * EMR.S.C01.Rsp | depends on customer application

 * Commands Generated
 * EMR.S.C00.Tx | depends on customer application
 * EMR.S.C01.Tx | depends on customer application
 *
 * Client Attributes
 * DRLK.C.Afffd | True
 * DRLK.C.Afffe | False
 *
 * Commands Received
 * EMR.C.C00.Rsp | depends on customer application
 * EMR.C.C01.Rsp | depends on customer application
 *
 * Commands Generated
 * EMR.C.C00.Tx | True
 * EMR.C.C01.Tx | True
 */

/** Electrical Measurement Server Attribute IDs */
enum ZbZclElecMeasSvrAttrT {
    ZCL_ELEC_MEAS_ATTR_MEAS_TYPE = 0x0000,
    ZCL_ELEC_MEAS_ATTR_DC_VOLT = 0x0100,
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_MIN = 0x0101,
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_MAX = 0x0102,
    ZCL_ELEC_MEAS_ATTR_DC_CURRENT = 0x0103,
    ZCL_ELEC_MEAS_ATTR_DC_CURRENT_MIN = 0x0104,
    ZCL_ELEC_MEAS_ATTR_DC_CURRENT_MAX = 0x0105,
    ZCL_ELEC_MEAS_ATTR_DC_POWER = 0x0106,
    ZCL_ELEC_MEAS_ATTR_DC_POWER_MIN = 0x0107,
    ZCL_ELEC_MEAS_ATTR_DC_POWER_MAX = 0x0108,
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_MULTIPLIER = 0x0200,
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_DIVISOR = 0x0201,
    ZCL_ELEC_MEAS_ATTR_DC_CURR_MULTIPLIER = 0x0202,
    ZCL_ELEC_MEAS_ATTR_DC_CURR_DIVISOR = 0x0203,
    ZCL_ELEC_MEAS_ATTR_DC_PWR_MULTIPLIER = 0x0204,
    ZCL_ELEC_MEAS_ATTR_DC_PWR_DIVISOR = 0x0205,
    ZCL_ELEC_MEAS_ATTR_AC_FREQ = 0x0300,
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_MIN = 0x0301,
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_MAX = 0x0302,
    ZCL_ELEC_MEAS_ATTR_NEUTRAL_CURR = 0x0303,
    ZCL_ELEC_MEAS_ATTR_TOTAL_ACTIVE_PWR = 0x0304,
    ZCL_ELEC_MEAS_ATTR_TOTAL_REACTIVE_PWR = 0x0305,
    ZCL_ELEC_MEAS_ATTR_TOTAL_APPARENT_PWR = 0x0306,
    ZCL_ELEC_MEAS_ATTR_1ST_HARM_CURR = 0x0307,
    ZCL_ELEC_MEAS_ATTR_3RD_HARM_CURR = 0x0308,
    ZCL_ELEC_MEAS_ATTR_5TH_HARM_CURR = 0x0309,
    ZCL_ELEC_MEAS_ATTR_7TH_HARM_CURR = 0x030a,
    ZCL_ELEC_MEAS_ATTR_9TH_HARM_CURR = 0x030b,
    ZCL_ELEC_MEAS_ATTR_11TH_HARM_CURR = 0x030c,
    ZCL_ELEC_MEAS_ATTR_PHASE_1ST_HARM_CURR = 0x030d,
    ZCL_ELEC_MEAS_ATTR_PHASE_3RD_HARM_CURR = 0x030e,
    ZCL_ELEC_MEAS_ATTR_PHASE_5TH_HARM_CURR = 0x030f,
    ZCL_ELEC_MEAS_ATTR_PHASE_7TH_HARM_CURR = 0x0310,
    ZCL_ELEC_MEAS_ATTR_PHASE_9TH_HARM_CURR = 0x0311,
    ZCL_ELEC_MEAS_ATTR_PHASE_11TH_HARM_CURR = 0x0312,
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_MULTIPLIER = 0x0400,
    ZCL_ELEC_MEAS_ATTR_AC_FREQ_DIVISOR = 0x0401,
    ZCL_ELEC_MEAS_ATTR_PWR_MULTIPLIER = 0x0402,
    ZCL_ELEC_MEAS_ATTR_PWR_DIVISOR = 0x0403,
    ZCL_ELEC_MEAS_ATTR_HARM_CURR_MULTIPLIER = 0x0404,
    ZCL_ELEC_MEAS_ATTR_PHASE_CURR_MULTIPLIER = 0x0405,
    ZCL_ELEC_MEAS_ATTR_LINE_CURR = 0x0501,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_CURR = 0x0502,
    ZCL_ELEC_MEAS_ATTR_REACTIVE_CURR = 0x0503,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT = 0x0505,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MIN = 0x0506,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MAX = 0x0507,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR = 0x0508,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MIN = 0x0509,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MAX = 0x050a,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR = 0x050b,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MIN = 0x050c,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MAX = 0x050d,
    ZCL_ELEC_MEAS_ATTR_REACTIVE_PWR = 0x050e,
    ZCL_ELEC_MEAS_ATTR_APPARENT_PWR = 0x050f,
    ZCL_ELEC_MEAS_ATTR_PWR_FACTOR = 0x0510,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_VOLT_PERIOD = 0x0511,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV_COUNT = 0x0512,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV_COUNT = 0x0513,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OVER_PERIOD = 0x0514,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UNDER_PERIOD = 0x0515,
    ZCL_ELEC_MEAS_ATTR_VOLT_SAG_PERIOD = 0x0516,
    ZCL_ELEC_MEAS_ATTR_VOLT_SWELL_PERIOD = 0x0517,
    ZCL_ELEC_MEAS_ATTR_AC_VOLT_MULTIPLIER = 0x0600,
    ZCL_ELEC_MEAS_ATTR_AC_VOLT_DIVISOR = 0x0601,
    ZCL_ELEC_MEAS_ATTR_AC_CURR_MULT = 0x0602,
    ZCL_ELEC_MEAS_ATTR_AC_CURR_DIVISOR = 0x0603,
    ZCL_ELEC_MEAS_ATTR_AC_PWR_MULTIPLIER = 0x0604,
    ZCL_ELEC_MEAS_ATTR_AC_PWR_DIVISOR = 0x0605,
    ZCL_ELEC_MEAS_ATTR_DC_OL_ALARMS_MASK = 0x0700,
    ZCL_ELEC_MEAS_ATTR_DC_VOLT_OL = 0x0701,
    ZCL_ELEC_MEAS_ATTR_DC_CURR_OL = 0x0702,
    ZCL_ELEC_MEAS_ATTR_AC_ALARMS_MASK = 0x0800,
    ZCL_ELEC_MEAS_ATTR_AC_VOLT_OL = 0x0801,
    ZCL_ELEC_MEAS_ATTR_AC_CURR_OL = 0x0802,
    ZCL_ELEC_MEAS_ATTR_AC_ACTIVE_PWR_OL = 0x0803,
    ZCL_ELEC_MEAS_ATTR_AC_REACTIVE_PWR_OL = 0x0804,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV = 0x0805,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV = 0x0806,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OV = 0x0807,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UV = 0x0808,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_SAG = 0x0809,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_SWELL = 0x080a,
    ZCL_ELEC_MEAS_ATTR_LINE_CURR_B = 0x0901,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_CURR_B = 0x0902,
    ZCL_ELEC_MEAS_ATTR_REACTIVE_CURR_B = 0x0903,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_B = 0x0905,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MIN_B = 0x0906,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MAX_B = 0x0907,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_B = 0x0908,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MIN_B = 0x0909,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MAX_B = 0x090a,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_B = 0x090b,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MIN_B = 0x090c,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MAX_B = 0x090d,
    ZCL_ELEC_MEAS_ATTR_REACTIVE_PWR_B = 0x090e,
    ZCL_ELEC_MEAS_ATTR_APPARENT_PWR_B = 0x090f,
    ZCL_ELEC_MEAS_ATTR_PWR_FACTOR_B = 0x0910,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_VOLT_PERIOD_B = 0x0911,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV_B = 0x0912,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV_B = 0x0913,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OVER_B = 0x0914,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UNDER_B = 0x0915,
    ZCL_ELEC_MEAS_ATTR_RMS_SAG_PERIOD_B = 0x0916,
    ZCL_ELEC_MEAS_ATTR_RMS_SWELL_PERIOD_B = 0x0917,
    ZCL_ELEC_MEAS_ATTR_LINE_CURR_C = 0x0a01,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_CURR_C = 0x0a02,
    ZCL_ELEC_MEAS_ATTR_REACTIVE_CURR_C = 0x0a03,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_C = 0x0a05,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MIN_C = 0x0a06,
    ZCL_ELEC_MEAS_ATTR_RMS_VOLT_MAX_C = 0x0a07,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_C = 0x0a08,
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MIN_C = 0x0a09, /**< RMSCurrentMinPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_RMS_CURR_MAX_C = 0x0a0a,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_C = 0x0a0b, /**< ActivePowerPhC (Optional) */
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MIN_C = 0x0a0c,
    ZCL_ELEC_MEAS_ATTR_ACTIVE_PWR_MAX_C = 0x0a0d,
    ZCL_ELEC_MEAS_ATTR_REACTIVE_PWR_C = 0x0a0e,
    ZCL_ELEC_MEAS_ATTR_APPARENT_PWR_C = 0x0a0f,
    ZCL_ELEC_MEAS_ATTR_PWR_FACTOR_C = 0x0a10,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_VOLT_PERIOD_C = 0x0a11,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_OV_C = 0x0a12,
    ZCL_ELEC_MEAS_ATTR_AVG_RMS_UV_C = 0x0a13,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_OVER_C = 0x0a14,
    ZCL_ELEC_MEAS_ATTR_RMS_EXT_UNDER_C = 0x0a15,
    ZCL_ELEC_MEAS_ATTR_RMS_SAG_PERIOD_C = 0x0a16,
    ZCL_ELEC_MEAS_ATTR_RMS_SWELL_PERIOD_C = 0x0a17,
};

/* Default Values */
#define ZCL_ELEC_MEAS_DEFAULT_MEAS_TYPE         0x00000000
#define ZCL_ELEC_MEAS_DEFAULT_DC_MEAS           ZCL_INVALID_SIGNED_16BIT
#define ZCL_ELEC_MEAS_DEFAULT_DC_FORMATTING     0x0001
#define ZCL_ELEC_MEAS_DEFAULT_DC_OL_ALARMS_MASK 0x00
#define ZCL_ELEC_MEAS_DEFAULT_DC_VOLT_OL        0xffff

/* Min and Max Values */
#define ZCL_ELEC_MEAS_MIN_DC_MEAS               (-32767)
#define ZCL_ELEC_MEAS_MAX_DC_MEAS               32767
#define ZCL_ELEC_MEAS_MIN_DC_FORMATTING         0x0001
#define ZCL_ELEC_MEAS_MAX_DC_FORMATTING         0xffff
#define ZCL_ELEC_MEAS_MIN_OL_ALARMS_MASK        0x00
#define ZCL_ELEC_MEAS_MAX_OL_ALARMS_MASK        0x03

/* Client Generated Commands */
enum {
    ZCL_ELEC_MEAS_CLI_GET_PROFILE_INFO = 0x00,
    ZCL_ELEC_MEAS_CLI_GET_MEAS_PROFILE = 0x01
};

/* Server Generated Commands */
enum {
    ZCL_ELEC_MEAS_SVR_GET_PROFILE_INFO_RSP = 0x00,
    ZCL_ELEC_MEAS_SVR_GET_MEAS_PROFILE_RSP = 0x01
};

struct ZbZclElecMeasClientGetMeasProfileReqT {
    uint16_t attr_id;
    uint32_t start_time;
    uint8_t num_intervals;
};

struct ZbZclElecMeasSvrGetProfileInfoRspT {
    /* Profile count is the total number for supported profile, and the list of
     * attributes is the list of attributes being profiled. This relationship has
     * been interpreted as profile count holding the number of attributes in
     * attribute list. */
    uint8_t profile_count;
    uint8_t profile_interval_period;
    uint8_t max_num_intervals;
    uint16_t *attr_list;
};

struct ZbZclElecMeasSvrGetMeasProfileRspT {
    uint32_t start_time;
    uint8_t status;
    uint8_t profile_interval_period;
    uint8_t num_intervals_delivered; /**< Number of intervals in interval_data. */
    uint16_t attr_id; /**< The attribute Id being profiled */
    uint8_t *interval_data;
    /**< Pointer to buffer of interval attribute data in the format as sent over the air
     * (endian conversion already done). If an interval is invalid, it should be set to
     * all f's (e.g. 0xffff for an int16 type). */
    uint16_t interval_len; /**< Octet length of interval_data. */
};

struct ZbZclElecMeasSvrCallbacksT {
    enum ZclStatusCodeT (*get_profile_info)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*get_meas_profile)(struct ZbZclClusterT *clusterPtr, struct ZbZclElecMeasClientGetMeasProfileReqT *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
};

/*---------------------------------------------------------------
 * Electrical Measurement Function Declarations
 *---------------------------------------------------------------
 */

/* Allocation Functions */
struct ZbZclClusterT * ZbZclElecMeasClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclElecMeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclElecMeasSvrCallbacksT *callbacks, void *arg);

/* Client Generated Commands */
enum ZclStatusCodeT ZbZclElecMeasClientGetProfileInfoReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);
enum ZclStatusCodeT ZbZclElecMeasClientGetMeasProfileReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclElecMeasClientGetMeasProfileReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Server Reponse Commands */
enum ZclStatusCodeT ZbZclElecMeasServerSendProfileInfoRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclElecMeasSvrGetProfileInfoRspT *rsp);
enum ZclStatusCodeT ZbZclElecMeasServerSendMeasProfileRsp(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *dst,
    struct ZbZclElecMeasSvrGetMeasProfileRspT *rsp);

#endif /* _ZCL_ELEC_MEAS_H */
