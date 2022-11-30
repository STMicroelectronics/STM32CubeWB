/**
 * @file zcl.power.profile.h
 * @heading Power Profile
 * @brief ZCL Power Profile cluster header
 * ZCL 7 section 3.17
 * ZCL 8 section 3.17
 * @copyright Copyright [2019 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_PWR_PROF_H
#define ZCL_PWR_PROF_H

/* NOTE: The following PICS statement is for internal Exegin use only, and is not a product of the Zigbee Alliance. */
/* @PICS.ZCL.Power.Profile
 * PWR.S | Server | True
 * PWR.C | Client | True
 *
 * Server Attributes
 * PWR.S.A0000 | TotalProfileNum | True
 * PWR.S.A0001 | MultipleScheduling | True
 * PWR.S.A0002 | EnergyFormatting | True
 * PWR.S.A0003 | EnergyRemote | True
 * PWR.S.A0004 | ScheduleMode | True
 * PWR.S.A0004.Report.Tx  | ScheduleMode | True
 * PWR.S.Afffd | ClusterRevision | True
 * PWR.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * PWR.S.C00.Rsp | PowerProfileRequest | True
 * PWR.S.C01.Rsp | PowerProfileStateRequest | True
 * PWR.S.C02.Rsp | GetPowerProfilePriceResponse | True
 * PWR.S.C03.Rsp | GetOverallSchedulePriceResponse | True
 * PWR.S.C04.Rsp | EnergyPhasesScheduleNotification | True
 * PWR.S.C05.Rsp | EnergyPhasesScheduleResponse | True
 * PWR.S.C06.Rsp | PowerProfileScheduleConstraintsRequest | True
 * PWR.S.C07.Rsp | EnergyPhasesScheduleStateRequest | True
 * PWR.S.C08.Rsp | GetPowerProfilePriceExtendedResponse | True
 *
 * Commands Generated
 * PWR.S.C00.Tx | PowerProfileNotification | True
 * PWR.S.C01.Tx | PowerProfileResponse | True
 * PWR.S.C02.Tx | PowerProfileStateResponse | True
 * PWR.S.C03.Tx | GetPowerProfilePrice | True
 * PWR.S.C04.Tx | PowerProfilesStateNotification | True
 * PWR.S.C05.Tx | GetOverallSchedulePrice | True
 * PWR.S.C06.Tx | EnergyPhasesScheduleRequest | True
 * PWR.S.C07.Tx | EnergyPhasesScheduleStateResponse | True
 * PWR.S.C08.Tx | EnergyPhasesScheduleStateNotification | True
 * PWR.S.C09.Tx | EnergyPhasesScheduleStateNotification | True
 * PWR.S.C0a.Tx | PowerProfileScheduleConstraintsResponse | True
 * PWR.S.C0b.Tx | GetPowerProfilePriceExtended | True
 *
 * Client Attributes
 * PWR.C.A0004.Report.Rsp | ScheduleMode | False
 * PWR.C.Afffd | ClusterRevision | True
 * PWR.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * PWR.C.C00.Rsp | PowerProfileNotification | True
 * PWR.C.C01.Rsp | PowerProfileResponse | True
 * PWR.C.C02.Rsp | PowerProfileStateResponse | True
 * PWR.C.C03.Rsp | GetPowerProfilePrice | True
 * PWR.C.C04.Rsp | PowerProfilesStateNotification | True
 * PWR.C.C05.Rsp | GetOverallSchedulePrice | True
 * PWR.C.C06.Rsp | EnergyPhasesScheduleRequest | True
 * PWR.C.C07.Rsp | EnergyPhasesScheduleStateResponse | True
 * PWR.C.C08.Rsp | EnergyPhasesScheduleStateNotification | True
 * PWR.C.C09.Rsp | EnergyPhasesScheduleStateNotification | True
 * PWR.C.C0a.Rsp | PowerProfileScheduleConstraintsResponse | True
 * PWR.C.C0b.Rsp | GetPowerProfilePriceExtended | True
 *
 * Commands Generated
 * PWR.C.C00.Tx | PowerProfileRequest | True
 * PWR.C.C01.Tx | PowerProfileStateRequest | True
 * PWR.C.C02.Tx | GetPowerProfilePriceResponse | True
 * PWR.C.C03.Tx | GetOverallSchedulePriceResponse | True
 * PWR.C.C04.Tx | EnergyPhasesScheduleNotification | True
 * PWR.C.C05.Tx | EnergyPhasesScheduleResponse | True
 * PWR.C.C06.Tx | PowerProfileScheduleConstraintsRequest | True
 * PWR.C.C07.Tx | EnergyPhasesScheduleStateRequest | True
 * PWR.C.C08.Tx | GetPowerProfilePriceExtendedResponse | True
 */

#include "zcl/zcl.h"

/** Power Profile Server Attribute IDs */
enum ZbZclPowerProfileSvrAttrT {
    ZCL_POWER_PROF_SVR_ATTR_TOTAL_PROFILENUM = 0x0000, /**< TotalProfileNum */
    ZCL_POWER_PROF_SVR_ATTR_MULTIPLE_SCHED = 0x0001, /**< MultipleScheduling */
    ZCL_POWER_PROF_SVR_ATTR_ENERGY_FORMAT = 0x0002, /**< EnergyFormatting */
    ZCL_POWER_PROF_SVR_ATTR_ENERGY_REMOTE = 0x0003, /**< EnergyRemote */
    ZCL_POWER_PROF_SVR_ATTR_SCHEDULE_MODE = 0x0004 /**< ScheduleMode */
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

/* These are arbitrary limits to save memory. Theoretical max is 0xfe (254) */
#define ZCL_PWR_PROF_MAX_PROFILES                       16U
#define ZCL_PWR_PROF_MAX_ENERGY_PHASES                  16U

enum ZbZclPowerProfState {
    ZCL_PWR_PROF_STATE_PROFILE_IDLE = 0x00,
    ZCL_PWR_PROF_STATE_PROFILE_PROGRAMMED = 0x01,
    ZCL_PWR_PROF_STATE_PHASE_RUNNING = 0x03,
    ZCL_PWR_PROF_STATE_PHASE_PAUSED = 0x04,
    ZCL_PWR_PROF_STATE_PHASE_WAITING_START = 0x05,
    ZCL_PWR_PROF_STATE_PHASE_WAITING_PAUSED = 0x06,
    ZCL_PWR_PROF_STATE_PROFILE_ENDED = 0x07
};

/** Phase structure */
struct ZbZclPowerProfPhase {
    uint8_t energy_phase_id; /**< Energy PhaseID */
    uint8_t macro_phase_id; /**< Macro PhaseID */
    uint16_t expect_duration; /**< ExpectedDuration */
    uint16_t peak_power; /**< Peak Power */
    uint16_t energy; /**< Energy */
    uint16_t max_activation_delay; /**< MaxActivationDelay */
};

/** PowerProfileNotification and PowerProfileResponse command structure */
struct ZbZclPowerProfSvrProfileRsp {
    uint8_t total_profile_num; /**< Total Profile Num */
    uint8_t profile_id; /**< Power ProfileID */
    uint8_t num_transferred_phases; /**< Num of Transferred Phases */
    struct ZbZclPowerProfPhase phase_list[ZCL_PWR_PROF_MAX_ENERGY_PHASES]; /**< Phase List */
};

/** Record structure */
struct ZbZclPowerProfileRecord {
    uint8_t profile_id; /**< Power Profile ID */
    uint8_t energy_phase_id; /**< Energy Phase ID */
    uint8_t remote_control; /**< PowerProfileRemoteControl */
    enum ZbZclPowerProfState state; /**< PowerProfile State */
};

/** PowerProfileStateResponse command structure */
struct ZbZclPowerProfSvrStateRsp {
    uint8_t profile_count; /**< Power Profile Count */
    struct ZbZclPowerProfileRecord record_list[ZCL_PWR_PROF_MAX_PROFILES]; /**< Record List */
};

/** PowerProfileRequest command structure */
struct ZbZclPowerProfCliProfileReq {
    uint8_t profile_id; /**< PowerProfileID */
};

/** Schedule Phase structure */
struct ZbZclPowerProfSchedPhase {
    uint8_t energy_phase_id; /**< Energy PhaseID */
    uint16_t sched_time; /**< Scheduled Time */
};

/** Energy Phases Schedule Notification command structure */
struct ZbZclPowerProfCliPhasesNotify {
    uint8_t profile_id; /**< Power ProfileID */
    uint8_t num_phases; /**< Num of Scheduled Phases */
    struct ZbZclPowerProfSchedPhase sched_list[ZCL_PWR_PROF_MAX_ENERGY_PHASES]; /**< Schedule List */
};

/** GetPowerProfilePriceResponse command structure */
struct ZbZclPowerProfCliPriceRsp {
    uint8_t profile_id; /**< Power Profile ID */
    uint16_t currency; /**< Currency */
    uint32_t price; /**< Price */
    uint8_t trailing_digit; /**< Price Trailing Digit */
};

/** GetOverallSchedulePriceResponse command structure */
struct ZbZclPowerProfCliSchedPriceRsp {
    uint16_t currency; /**< Currency */
    uint32_t price; /**< Price */
    uint8_t trailing_digit; /**< Price Trailing Digit */
};

/** Energy Phases Schedule Notification and EnergyPhasesScheduleResponse command structure */
struct ZbZclPowerProfSvrPhasesRsp {
    uint8_t profile_id; /**< Power ProfileID */
    uint8_t num_sched_energy_phases; /**< Num of Scheduled Phases */
};

/** PowerProfileScheduleConstraintsNotification command structure */
struct ZbZclPowerProfSvrConstraintsNotify {
    uint8_t profile_id; /**< PowerProfileID */
    uint16_t start_after; /**< Start After */
    uint16_t stop_before; /**< Stop Before */
};

/* GetPowerProfilePriceExtended Command options field */
#define ZCL_PWR_PROF_PRICE_EXT_OPT_START_TIME_PRESENT   0x01U
#define ZCL_PWR_PROF_PRICE_EXT_OPT_EST_PRICE            0x02U

/** GetPowerProfilePriceExtended command structure */
struct ZbZclPowerProfSvrGetPriceExtReq {
    uint8_t options; /**< Options - e.g. ZCL_PWR_PROF_PRICE_EXT_OPT_START_TIME_PRESENT */
    uint8_t profile_id; /**< PowerProfileID */
    uint16_t start_time; /**< PowerProfileStartTime - optional (ZCL_PWR_PROF_PRICE_EXT_OPT_START_TIME_PRESENT) */
};

/** Power Profile Client callbacks configuration */
struct ZbZclPowerProfClientCallbacks {
    enum ZclStatusCodeT (*profile_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrProfileRsp *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of PowerProfileNotification command */

    enum ZclStatusCodeT (*get_price)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of GetPowerProfilePrice command */

    enum ZclStatusCodeT (*state_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrStateRsp *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of PowerProfileStateNotification command */

    enum ZclStatusCodeT (*get_sched_price)(struct ZbZclClusterT *cluster,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of GetOverallSchedulePrice command */

    enum ZclStatusCodeT (*phases_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of EnergyPhasesScheduleRequest command */

    enum ZclStatusCodeT (*phases_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrPhasesRsp *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of EnergyPhasesScheduleStateNotification command */

    enum ZclStatusCodeT (*constraints_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrConstraintsNotify *notify, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of PowerProfileScheduleConstraintsNotification command */

    enum ZclStatusCodeT (*get_price_ext)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfSvrGetPriceExtReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of GetPowerProfilePriceExtended command */
};

/**
 * Create a new instance of the Power Profile Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPowerProfClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPowerProfClientCallbacks *callbacks, void *arg);

/**
 * Send a PowerProfileRequest command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req PowerProfileRequest command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientProfileReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfCliProfileReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a PowerProfileStateRequest command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientStateReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a GetPowerProfilePriceResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp GetPowerProfilePriceResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientPriceRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfCliPriceRsp *rsp,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send a GetOverallSchedulePriceResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp GetOverallSchedulePriceResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientSchedPriceRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfCliSchedPriceRsp *rsp,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send an Energy Phases Schedule Notification command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param notify Energy Phases Schedule Notification command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientPhasesNotify(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfCliPhasesNotify *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a EnergyPhasesScheduleResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp EnergyPhasesScheduleResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientPhasesResponse(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfCliPhasesNotify *notify,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send a PowerProfileScheduleConstraintsRequest command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req PowerProfileScheduleConstraintsRequest command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientSchedConsReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfCliProfileReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a EnergyPhasesScheduleStateRequest command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req EnergyPhasesScheduleStateRequest command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientPhasesSchedStateReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfCliProfileReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a GetPowerProfilePriceExtendedResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp GetPowerProfilePriceExtendedResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfClientPriceExtRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfCliPriceRsp *rsp,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/** Power Profile Server callbacks configuration */
struct ZbZclPowerProfServerCallbacks {
    enum ZclStatusCodeT (*profile_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of PowerProfileRequest command */

    enum ZclStatusCodeT (*state_req)(struct ZbZclClusterT *cluster,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of PowerProfileStateRequest command */

    enum ZclStatusCodeT (*phases_notify)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliPhasesNotify *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Energy Phases Schedule Notification command */

    enum ZclStatusCodeT (*sched_contraints_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of PowerProfileScheduleConstraintsRequest command */

    enum ZclStatusCodeT (*phases_sched_state_req)(struct ZbZclClusterT *cluster,
        struct ZbZclPowerProfCliProfileReq *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of EnergyPhasesScheduleStateRequest command */
};

/**
 * Create a new instance of the Power Profile Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPowerProfServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPowerProfServerCallbacks *callbacks, void *arg);

/** Send a PowerProfileNotification Command. It is sent as a ZCL request with a unique
 * sequence number, and can receive a Default Response if applicable
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param notify PowerProfileNotification command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerProfileNotify(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrProfileRsp *notify,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a PowerProfileResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp PowerProfileResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerProfileRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrProfileRsp *rsp,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send a PowerProfileStateResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp PowerProfileStateResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerStateRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrStateRsp *rsp,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send a GetPowerProfilePrice command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req GetPowerProfilePrice command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerGetPriceReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfCliProfileReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Send a PowerProfileStateNotification Command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param notify PowerProfileStateNotification command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerStateNotify(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrStateRsp *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a GetOverallSchedulePrice command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req GetOverallSchedulePrice command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerGetSchedPriceReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a EnergyPhasesScheduleRequest command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req EnergyPhasesScheduleRequest command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerPhasesReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfCliProfileReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a EnergyPhasesScheduleStateResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp EnergyPhasesScheduleStateResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerPhasesRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrPhasesRsp *rsp,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/** Send a EnergyPhasesScheduleStateNotification Command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param notify EnergyPhasesScheduleStateNotification command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerPhasesNotify(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrPhasesRsp *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Send a PowerProfileScheduleConstraintsNotification Command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param notify PowerProfileScheduleConstraintsNotification command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerConstraintsNotify(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrConstraintsNotify *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a PowerProfileScheduleConstraintsResponse command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp PowerProfileScheduleConstraintsResponse command response structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerConstraintsRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclPowerProfSvrConstraintsNotify *rsp,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/**
 * Send a GetPowerProfilePriceExtended command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req GetPowerProfilePriceExtended command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPowerProfServerGetPriceReqExtReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPowerProfSvrGetPriceExtReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_PWR_PROF_H */
