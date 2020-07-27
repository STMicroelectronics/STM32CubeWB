/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL DRLC cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_DRLC_H
# define ZCL_DRLC_H

/*---------------------------------------------------------------
 * Dependencies
 *---------------------------------------------------------------
 */
#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */

/* DRLC Server Commands */
enum {
    ZCL_DRLC_COMMAND_EVENT = 0x00,
    ZCL_DRLC_COMMAND_CANCEL_EVENT = 0x01,
    ZCL_DRLC_COMMAND_CANCEL_ALL = 0x02
};

/* DRLC Client Commands */
enum {
    ZCL_DRLC_COMMAND_REPORT_EVENT_STATUS = 0x00,
    ZCL_DRLC_COMMAND_GET_SCHEDULED_EVENTS = 0x01
};

#define ZCL_DRLC_ISSUER_ID_INVALID              0xffffffffU

/* DRLC Server attribute identifiers */
/* none */

/* DRLC Client attribute identifiers */
enum {
    ZCL_DRLC_CLI_ATTR_UTILITY_ENROL_GRP = 0x0000,
    ZCL_DRLC_CLI_ATTR_START_RAND_MINS = 0x0001,
    ZCL_DRLC_CLI_ATTR_STOP_RAND_MINS = 0x0002,
    ZCL_DRLC_CLI_ATTR_DEVICE_CLASS = 0x0003
};

/* DRLC Device Class bitmaps */
#define ZCL_DRLC_DEVICE_HVAC                    0x0001U
#define ZCL_DRLC_DEVICE_BASEBOARD_HEATER        0x0002U
#define ZCL_DRLC_DEVICE_WATER_HEATER            0x0004U
#define ZCL_DRLC_DEVICE_POOL_PUMP               0x0008U
#define ZCL_DRLC_DEVICE_APPLIANCE               0x0010U
#define ZCL_DRLC_DEVICE_IRRIGATION_PUMP         0x0020U
#define ZCL_DRLC_DEVICE_COMMERCIAL_LOAD         0x0040U
#define ZCL_DRLC_DEVICE_RESIDENTIAL_LOAD        0x0080U
#define ZCL_DRLC_DEVICE_EXTERIOR_LIGHTING       0x0100U
#define ZCL_DRLC_DEVICE_INTERIOR_LIGHTING       0x0200U
#define ZCL_DRLC_DEVICE_ELECTRIC_VEHICLE        0x0400U
#define ZCL_DRLC_DEVICE_GENERATOR               0x0800U

/* DRLC Criticality Levels */
enum ZbZclDrlcCriticalityLevelT {
    ZCL_DRLC_CRITICALITY_INVALID = 0,
    ZCL_DRLC_CRITICALITY_GREEN = 1,
    ZCL_DRLC_CRITICALITY_1 = 2,
    ZCL_DRLC_CRITICALITY_2 = 3,
    ZCL_DRLC_CRITICALITY_3 = 4,
    ZCL_DRLC_CRITICALITY_4 = 5,
    ZCL_DRLC_CRITICALITY_5 = 6,
    ZCL_DRLC_CRITICALITY_EMERGENCY = 7,
    ZCL_DRLC_CRITICALITY_PLANNED_OUTAGE = 8,
    ZCL_DRLC_CRITICALITY_DISCONNECT = 9,
    ZCL_DRLC_CRITICALITY_UTILITY_MIN = 0x0A,
    ZCL_DRLC_CRITICALITY_UTILITY_MAX = 0x0F
};

/* Event Control Flags */
#define ZCL_DRLC_EVENT_CTRL_RAND_START          0x01U
#define ZCL_DRLC_EVENT_CTRL_RAND_END            0x02U

/* Cancel Event Flags */
#define ZCL_DRLC_CANCEL_CONTROL_GRACEFUL        0x01U

/* Event Status Values */
enum ZbZclDrlcEventStatusT {
    ZCL_DRLC_STATUS_EVENT_RECEIVED = 0x01,
    ZCL_DRLC_STATUS_EVENT_STARTED = 0x02,
    ZCL_DRLC_STATUS_EVENT_COMPLETED = 0x03,
    ZCL_DRLC_STATUS_USER_OPT_OUT = 0x04,
    ZCL_DRLC_STATUS_USER_OPT_IN = 0x05,
    ZCL_DRLC_STATUS_EVENT_CANCELLED = 0x06,
    ZCL_DRLC_STATUS_EVENT_SUPERSEDED = 0x07,
    ZCL_DRLC_STATUS_EVENT_PARTOPTOUT = 0x08,
    ZCL_DRLC_STATUS_EVENT_PARTOPTIN = 0x09,
    ZCL_DRLC_STATUS_EVENT_COMPLETE_OPT_OUT = 0x0a,
    ZCL_DRLC_STATUS_RJCTD_CANCEL_COMMAND = 0xf8,
    ZCL_DRLC_STATUS_RJCTD_CANCEL_TIME = 0xf9,
    ZCL_DRLC_STATUS_RJCTD_AFTER_EXPIRE = 0xfb,
    ZCL_DRLC_STATUS_RJCTD_CANCEL_UNDEFINED = 0xfd,
    ZCL_DRLC_STATUS_RJCTD = 0xfe
};

#define ZCL_DRLC_COOL_SETPOINT_IGNORED      0x8000U
#define ZCL_DRLC_HEAT_SETPOINT_IGNORED      0x8000U
#define ZCL_DRLC_AVG_LOAD_ADJ_IGNORED       0x80U
#define ZCL_DRLC_DUTYCYCLE_IGNORED          0xffU

enum ZbZclDrlcSignatureT {
    ZCL_DRLC_SIGNATURE_TYPE_NONE = 0x00,
    ZCL_DRLC_SIGNATURE_TYPE_ECDSA = 0x01
};

/* Event status signature size. */
#define ZCL_DRLC_SIGNATURE_LENGTH               42U

/*---------------------------------------------------------------
 * Structures
 *---------------------------------------------------------------
 */
/* Structure describing a load control event. */
struct ZbZclDrlcEventT {
    uint32_t issuer_id;
    uint16_t device_class;
    uint8_t util_enrol_group;
    uint32_t start_time; /* UTC, or 0 = now */
    uint16_t duration; /* minutes */
    enum ZbZclDrlcCriticalityLevelT criticality;
    uint8_t cool_offset;
    uint8_t heat_offset;
    int16_t cool_setpoint;
    int16_t heat_setpoint;
    int8_t avg_load_adj;
    uint8_t dutycycle;
    uint8_t event_control; /* e.g. ZCL_DRLC_EVENT_CTRL_RAND_START */
};

/* Structure describing a cancel event command. */
struct ZbZclDrlcCancelT {
    uint32_t issuer_id;
    uint16_t device_class;
    uint8_t util_enrol_group;
    uint8_t cancel_control; /* e.g. ZCL_DRLC_CANCEL_CONTROL_GRACEFUL */
    /* effectiveTime is now deprecated - must be zero */
    /* uint32_t effectiveTime; */
};

/* Report Event Status Command data structure */
struct ZbZclDrlcStatusT {
    uint32_t issuer_id;
    enum ZbZclDrlcEventStatusT status;
    uint32_t status_time;
    enum ZbZclDrlcCriticalityLevelT crit_level_applied;
    uint16_t cool_setpoint_applied; /* Set to ZCL_DRLC_COOL_SETPOINT_IGNORED if not used */
    uint16_t heat_setpoint_applied; /* Set to ZCL_DRLC_HEAT_SETPOINT_IGNORED if not used */
    int8_t avg_load_adj_applied; /* Set to ZCL_DRLC_AVG_LOAD_ADJ_IGNORED if not used */
    uint8_t dutycycle_applied; /* Set to ZCL_DRLC_DUTYCYCLE_IGNORED if not used */
    uint8_t event_control;
    enum ZbZclDrlcSignatureT sig_type;
    uint8_t sig_data[ZCL_DRLC_SIGNATURE_LENGTH];
};

struct ZbZclDrlcGetEventsReqT {
    uint32_t start_time;
    uint8_t num_events;
    uint32_t issuer_id;
};

/*---------------------------------------------------------------
 * DRLC Server Functions
 *---------------------------------------------------------------
 */
struct ZbZclDrlcServerCallbacksT {
    void (*report_status)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo,
        struct ZbZclDrlcStatusT *status, void *arg);

    /* The get_events callback handler in the application should return ZCL_STATUS_SUCCESS,
     * or ZCL_STATUS_NOT_FOUND if no events are found. Events are re-issued by calling
     * ZbZclDrlcServerCommandEventReq(). */
    enum ZclStatusCodeT (*get_events)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo,
        struct ZbZclDrlcGetEventsReqT *req, void *arg);
};

struct ZbZclClusterT * ZbZclDrlcServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclDrlcServerCallbacksT *callbacks, void *arg);

/* Server Commands */
enum ZclStatusCodeT ZbZclDrlcServerCommandEventReq(struct ZbZclClusterT *clusterPtr, struct ZbZclDrlcEventT *eventPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclDrlcServerCommandCancelReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclDrlcCancelT *cancelInfoPtr, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/* ctrl ; e.g. ZCL_DRLC_CANCEL_CONTROL_GRACEFUL */
enum ZclStatusCodeT ZbZclDrlcServerCommandCancelAllReq(struct ZbZclClusterT *clusterPtr, uint8_t ctrl,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/*---------------------------------------------------------------
 * DRLC Client Functions
 *---------------------------------------------------------------
 */

struct ZbZclDrlcClientCallbacksT {
    /* Should return true if started successfully, or false otherwise. */
    bool (*start)(void *arg, struct ZbZclDrlcEventT *event);
    void (*stop)(void *arg, struct ZbZclDrlcEventT *event);
};

struct ZbZclClusterT * ZbZclDrlcClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, struct ZbZclClusterT *time_server,
    struct ZbZclDrlcClientCallbacksT *callbacks, void *cb_arg);

unsigned int ZbZclDrlcClientGetEventList(struct ZbZclClusterT *clusterPtr,
    struct ZbZclDrlcEventT *eventList, unsigned int maxEntries);

/* Client Commands */
enum ZclStatusCodeT ZbZclDrlcClientCommandReportStatusReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclDrlcStatusT *statusPtr,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/* This command is used to request that Load Control Events are re-issued to the requesting device. */
enum ZclStatusCodeT ZbZclDrlcClientCommandGetEventsReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclDrlcGetEventsReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif /* __ZCL_DRLC_H */
