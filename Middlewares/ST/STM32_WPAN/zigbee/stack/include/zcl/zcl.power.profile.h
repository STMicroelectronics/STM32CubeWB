/* Copyright [2019 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_PWR_PROF_H
#define ZCL_PWR_PROF_H

#include "zcl/zcl.h"

/* Attribute Identifiers */
enum {
    ZCL_POWER_PROF_SVR_ATTR_TOTAL_PROFILENUM = 0x0000,
    ZCL_POWER_PROF_SVR_ATTR_MULTIPLE_SCHED = 0x0001,
    ZCL_POWER_PROF_SVR_ATTR_ENERGY_FORMAT = 0x0002,
    ZCL_POWER_PROF_SVR_ATTR_ENERGY_REMOTE = 0x0003,
    ZCL_POWER_PROF_SVR_ATTR_SCHEDULE_MODE = 0x0004
};

/* Client Generated Commands */
enum {
    ZCL_PWR_PROF_CLI_PROFILE_REQ = 0x00,
    ZCL_PWR_PROF_CLI_STATE_REQ = 0x01,
    ZCL_PWR_PROF_CLI_PRICE_RSP = 0x02,
    ZCL_PWR_PROF_CLI_SCHED_PRICE_RSP = 0x03,
    ZCL_PWR_PROF_CLI_PHASES_NOTIFY = 0x04,
    ZCL_PWR_PROF_CLI_PHASES_RSP = 0x05,
    ZCL_PWR_PROF_CLI_SCHED_CONS_REQ = 0x06,
    ZCL_PWR_PROF_CLI_PHASES_SCHED_STATE_REQ = 0x07,
    ZCL_PWR_PROF_CLI_PRICE_EXT_RSP = 0x08
};

/* Server Generated Commands */
enum {
    ZCL_PWR_PROF_SVR_PROFILE_NOTIFY = 0x00,
    ZCL_PWR_PROF_SVR_PROFILE_RSP = 0x01,
    ZCL_PWR_PROF_SVR_STATE_RSP = 0x02,
    ZCL_PWR_PROF_SVR_GET_PRICE = 0x03,
    ZCL_PWR_PROF_SVR_STATE_NOTIFY = 0x04,
    ZCL_PWR_PROF_SVR_GET_SCHED_PRICE = 0x05,
    ZCL_PWR_PROF_SVR_PHASES_REQ = 0x06,
    ZCL_PWR_PROF_SVR_PHASES_RSP = 0x07,
    ZCL_PWR_PROF_SVR_PHASES_NOTIFY = 0x08,
    ZCL_PWR_PROF_SVR_CONSTRAINTS_NOTIFY = 0x09,
    ZCL_PWR_PROF_SVR_CONSTRAINTS_RSP = 0x0a,
    ZCL_PWR_PROF_SVR_GET_PRICE_EXT = 0x0b,
};

/* Energy Formatting Mask (ZCL_POWER_PROF_SVR_ATTR_ENERGY_FORMAT) */
#define ZCL_PWR_PROF_ENERGY_FORMAT_DIGITS_TRAIL_MASK    0x07U /* num trailing digits */
#define ZCL_PWR_PROF_ENERGY_FORMAT_DIGITS_TRAIL_OFFSET  0U
#define ZCL_PWR_PROF_ENERGY_FORMAT_DIGITS_LEAD_MASK     0x78U /* num leading digits */
#define ZCL_PWR_PROF_ENERGY_FORMAT_DIGITS_LEAD_OFFSET   3U
#define ZCL_PWR_PROF_ENERGY_FORMAT_SUPPRESS_LEADING     0x80U /* suppress leading zeroes */

/* ScheduleMode Mask (ZCL_POWER_PROF_SVR_ATTR_SCHEDULE_MODE) */
#define ZCL_PWR_PROF_SCHED_MODE_CHEAPEST                0x01U
#define ZCL_PWR_PROF_SCHED_MODE_GREENEST                0x02U

/* BUGBUG - review these values */
#define ZCL_PWR_PROF_MAX_PROFILES               16U /* theoretical max is 0xfe (254) */
#define ZCL_PWR_PROF_MAX_ENERGY_PHASES          16U /* theoretical max is 0xfe (254) */

enum ZbZclPowerProfState {
    ZCL_PWR_PROF_STATE_PROFILE_IDLE = 0x00,
    ZCL_PWR_PROF_STATE_PROFILE_PROGRAMMED = 0x01,
    ZCL_PWR_PROF_STATE_PHASE_RUNNING = 0x03,
    ZCL_PWR_PROF_STATE_PHASE_PAUSED = 0x04,
    ZCL_PWR_PROF_STATE_PHASE_WAITING_START = 0x05,
    ZCL_PWR_PROF_STATE_PHASE_WAITING_PAUSED = 0x06,
    ZCL_PWR_PROF_STATE_PROFILE_ENDED = 0x07
};

struct ZbZclPowerProfPhase {
    uint8_t energy_phase_id;
    uint8_t macro_phase_id;
    uint16_t expect_duration;
    uint16_t peak_power;
    uint16_t energy;
    uint16_t max_activation_delay;
};

/* PowerProfileNotification and PowerProfileResponse share the same payload format. */
struct ZbZclPowerProfSvrProfileRsp {
    uint8_t total_profile_num;
    uint8_t power_profile_id;
    uint8_t num_transferred_phases;
    struct ZbZclPowerProfPhase phase_list[ZCL_PWR_PROF_MAX_ENERGY_PHASES];
};

struct ZbZclPowerProfileRecord {
    uint8_t power_profile_id;
    uint8_t energy_phase_id;
    uint8_t remote_control;
    enum ZbZclPowerProfState state;
};

/* PowerProfileStateResponse */
struct ZbZclPowerProfSvrStateRsp {
    uint8_t profile_count;
    struct ZbZclPowerProfileRecord record_list[ZCL_PWR_PROF_MAX_PROFILES];
};

/* PowerProfileRequest */
struct ZbZclPowerProfCliProfileReq {
    uint8_t profile_id;
};

struct ZbZclPowerProfSchedPhase {
    uint8_t energy_phase_id;
    uint16_t sched_time;
};

struct ZbZclPowerProfCliPhasesNotify {
    uint8_t power_profile_id;
    uint8_t num_phases;
    struct ZbZclPowerProfSchedPhase sched_list[ZCL_PWR_PROF_MAX_ENERGY_PHASES];
};

struct ZbZclPowerProfCliPriceRsp {
    uint8_t power_profile_id;
    uint16_t currency;
    uint32_t price;
    uint8_t trailing_digit;
};

struct ZbZclPowerProfCliSchedPriceRsp {
    uint16_t currency;
    uint32_t price;
    uint8_t trailing_digit;
};

/* ZCL_PWR_PROF_SVR_PHASES_RSP / ZCL_PWR_PROF_SVR_PHASES_NOTIFY */
struct ZbZclPowerProfSvrPhasesRsp {
    uint8_t power_profile_id;
    uint8_t num_sched_energy_phases;
};

/* ZCL_PWR_PROF_SVR_CONSTRAINTS_NOTIFY */
struct ZbZclPowerProfSvrConstraintsNotify {
    uint8_t power_profile_id;
    uint16_t start_after;
    uint16_t stop_before;
};

/* GetPowerProfilePriceExtended Command options field */
#define ZCL_PWR_PROF_PRICE_EXT_OPT_START_TIME_PRESENT   0x01U
#define ZCL_PWR_PROF_PRICE_EXT_OPT_EST_PRICE            0x02U

/* ZCL_PWR_PROF_SVR_GET_PRICE_EXT */
struct ZbZclPowerProfSvrGetPriceExtReq {
    uint8_t options; /* e.g. ZCL_PWR_PROF_PRICE_EXT_OPT_START_TIME_PRESENT */
    uint8_t power_profile_id;
    uint16_t start_time; /* optional (ZCL_PWR_PROF_PRICE_EXT_OPT_START_TIME_PRESENT) */
};

/* Power Profile Client Callbacks
 * If application sends a response, the callback return code should be set to
 * ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE to prevent a Default Response from also being sent.
 */
struct ZbZclPowerProfClientCallbacks {
    /* ZCL_PWR_PROF_SVR_PROFILE_NOTIFY */
    enum ZclStatusCodeT (*profile_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrProfileRsp *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_SVR_GET_PRICE */
    enum ZclStatusCodeT (*get_price)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_SVR_STATE_NOTIFY */
    enum ZclStatusCodeT (*state_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrStateRsp *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_SVR_GET_SCHED_PRICE */
    enum ZclStatusCodeT (*get_sched_price)(struct ZbZclClusterT *cluster,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_SVR_PHASES_REQ */
    enum ZclStatusCodeT (*phases_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_SVR_PHASES_NOTIFY */
    enum ZclStatusCodeT (*phases_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrPhasesRsp *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_SVR_CONSTRAINTS_NOTIFY */
    enum ZclStatusCodeT (*constraints_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrConstraintsNotify *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_SVR_GET_PRICE_EXT */
    enum ZclStatusCodeT (*get_price_ext)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrGetPriceExtReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
};

struct ZbZclClusterT * ZbZclPowerProfClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPowerProfClientCallbacks *callbacks, void *arg);

/* ZCL_PWR_PROF_CLI_PROFILE_REQ */
enum ZclStatusCodeT ZbZclPowerProfClientProfileReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfCliProfileReq *req, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_STATE_REQ */
enum ZclStatusCodeT ZbZclPowerProfClientStateReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_PRICE_RSP */
enum ZclStatusCodeT ZbZclPowerProfClientPriceRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfCliPriceRsp *rsp,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_SCHED_PRICE_RSP */
enum ZclStatusCodeT ZbZclPowerProfClientSchedPriceRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfCliSchedPriceRsp *rsp,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_PHASES_NOTIFY */
enum ZclStatusCodeT ZbZclPowerProfClientPhasesNotify(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfCliPhasesNotify *notify, struct ZbApsAddrT *dst,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_PHASES_RSP */
enum ZclStatusCodeT ZbZclPowerProfClientPhasesResponse(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfCliPhasesNotify *notify, struct ZbZclAddrInfoT *dst,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_SCHED_CONS_REQ */
enum ZclStatusCodeT ZbZclPowerProfClientSchedConsReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfCliProfileReq *req, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_PHASES_SCHED_STATE_REQ */
enum ZclStatusCodeT ZbZclPowerProfClientPhasesSchedStateReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfCliProfileReq *req, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PWR_PROF_CLI_PRICE_EXT_RSP */
enum ZclStatusCodeT ZbZclPowerProfClientPriceExtRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfCliPriceRsp *rsp,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* Power Profile Server Callbacks
 * If application sends a response, the callback return code should be set to
 * ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE to prevent a Default Response from also being sent.
 */
struct ZbZclPowerProfServerCallbacks {
    /* ZCL_PWR_PROF_CLI_PROFILE_REQ */
    enum ZclStatusCodeT (*profile_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_CLI_STATE_REQ */
    enum ZclStatusCodeT (*state_req)(struct ZbZclClusterT *cluster,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_CLI_PHASES_NOTIFY */
    enum ZclStatusCodeT (*phases_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliPhasesNotify *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_CLI_SCHED_CONS_REQ */
    enum ZclStatusCodeT (*sched_contraints_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    /* ZCL_PWR_PROF_CLI_PHASES_SCHED_STATE_REQ */
    enum ZclStatusCodeT (*phases_sched_state_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
};

struct ZbZclClusterT * ZbZclPowerProfServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPowerProfServerCallbacks *callbacks, void *arg);

/* ZCL_PWR_PROF_SVR_PROFILE_NOTIFY */
enum ZclStatusCodeT ZbZclPowerProfServerProfileNotify(struct ZbZclClusterT *clusterPtr,
    struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrProfileRsp *notify,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_PROFILE_RSP */
enum ZclStatusCodeT ZbZclPowerProfServerProfileRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrProfileRsp *rsp,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_STATE_RSP */
enum ZclStatusCodeT ZbZclPowerProfServerStateRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrStateRsp *rsp,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_GET_PRICE */
enum ZclStatusCodeT ZbZclPowerProfServerGetPriceReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfCliProfileReq *req, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_STATE_NOTIFY */
enum ZclStatusCodeT ZbZclPowerProfServerStateNotify(struct ZbZclClusterT *clusterPtr,
    struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrStateRsp *notify,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_GET_SCHED_PRICE */
enum ZclStatusCodeT ZbZclPowerProfServerGetSchedPriceReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_PHASES_REQ */
enum ZclStatusCodeT ZbZclPowerProfServerPhasesReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfCliProfileReq *req, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_PHASES_RSP */
enum ZclStatusCodeT ZbZclPowerProfServerPhasesRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrPhasesRsp *rsp,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_PHASES_NOTIFY */
enum ZclStatusCodeT ZbZclPowerProfServerPhasesNotify(struct ZbZclClusterT *clusterPtr,
    struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrPhasesRsp *notify,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_CONSTRAINTS_NOTIFY */
enum ZclStatusCodeT ZbZclPowerProfServerConstraintsNotify(struct ZbZclClusterT *clusterPtr,
    struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrConstraintsNotify *notify,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_CONSTRAINTS_RSP */
enum ZclStatusCodeT ZbZclPowerProfServerConstraintsRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrConstraintsNotify *rsp,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZCL_PWR_PROF_SVR_GET_PRICE_EXT */
enum ZclStatusCodeT ZbZclPowerProfServerGetPriceReqExtReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclPowerProfSvrGetPriceExtReq *req, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_PWR_PROF_H */
