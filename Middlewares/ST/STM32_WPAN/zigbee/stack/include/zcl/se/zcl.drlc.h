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
enum {
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
    uint32_t eventId;
    uint16_t deviceClass;
    uint8_t enrolmentGroup;
    uint32_t startTime; /* UTC, or 0 = now */
    uint16_t duration; /* minutes */
    enum ZbZclDrlcCriticalityLevelT criticality;
    uint8_t coolingOffset;
    uint8_t heatingOffset;
    uint16_t coolingSetPoint;
    uint16_t heatingSetPoint;
    int8_t loadAdjustment;
    uint8_t dutyCycle;
    uint8_t ctrl; /* e.g. ZCL_DRLC_EVENT_CTRL_RAND_START */
};

/* Structure describing a cancel event command. */
struct ZbZclDrlcCancelT {
    uint32_t eventId;
    uint16_t deviceClass;
    uint8_t enrolmentGroup;
    uint8_t ctrl; /* e.g. ZCL_DRLC_CANCEL_CONTROL_GRACEFUL */
#if 0 /* effectiveTime is now deprecated - must be zero */
    uint32_t effectiveTime;
#endif
};

/* Structure describing an event status. */
struct ZbZclDrlcStatusT {
    uint32_t eventId;
    uint8_t status;
    uint32_t statusTime;
    uint8_t appliedCriticality;
    uint16_t appliedCooling;
    uint16_t appliedHeating;
    int8_t loadAdjustment;
    uint8_t appliedDutyCycle;
    uint8_t eventControl;
    enum ZbZclDrlcSignatureT signatureType;
    /*  */
    uint8_t signature[ZCL_DRLC_SIGNATURE_LENGTH];
};

struct ZbZclDrlcGetEventsReqT {
    uint32_t startTime;
    uint8_t maxEvents;
    uint32_t issuerId;
};

/*---------------------------------------------------------------
 * DRLC Server Functions
 *---------------------------------------------------------------
 */
struct ZbZclDrlcServerCallbacksT {
    void (*report_status)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo,
        struct ZbZclDrlcStatusT *status, void *arg);

    /* The get_events callback handler in the application should return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE
     * if successful and the application will call ZbZclDrlcServerCommandEventNotify in response.
     * Or, the callback should return ZCL_STATUS_NOT_FOUND if no events found and no response sent,
     * which causes a Default Response to be sent with the error status code. */
    enum ZclStatusCodeT (*get_events)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo,
        struct ZbZclDrlcGetEventsReqT *req, void *arg);
};

struct ZbZclClusterT * ZbZclDrlcServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclDrlcServerCallbacksT *callbacks, void *arg);

/* Server Commands */
enum ZclStatusCodeT ZbZclDrlcServerCommandEventReq(struct ZbZclClusterT *clusterPtr, struct ZbZclDrlcEventT *eventPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/* ZbZclDrlcServerCommandEventNotify is similar to ZbZclDrlcServerCommandEventReq, but it
 * sends the ZCL_DRLC_COMMAND_EVENT command as a response to a ZCL_DRLC_COMMAND_GET_SCHEDULED_EVENTS. */
enum ZclStatusCodeT ZbZclDrlcServerCommandEventNotify(struct ZbZclClusterT *clusterPtr,
    struct ZbZclDrlcEventT *eventPtr, struct ZbZclAddrInfoT *dstInfo);

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

enum ZclStatusCodeT ZbZclDrlcClientCommandGetEventsReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclDrlcGetEventsReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif /* __ZCL_DRLC_H */
