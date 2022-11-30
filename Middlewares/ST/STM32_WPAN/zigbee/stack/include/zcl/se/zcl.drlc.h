/**
 * @file zcl.drlc.h
 * @heading Demand Response and Load Control
 * @brief ZCL Demand Response and Load Control cluster header
 * ZCL 7 Section 10.3
 * ZCL 8 Section 10.3
 * @copyright Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved.
 */

/* EXEGIN - removed '@'PICS escape sequence, since these are not the
 * official PICS codes. These were added with the assumption that
 * SE clusters are being brought into the Zigbee 3.0 fold and will
 * use the same PICS codes and PICS format style. However, this has
 * yet to occur. */

/* ZCL.DRLC
 * DRLC.S | Server | True
 * DRLC.C | Client | True
 *
 * Server Attributes
 * DRLC.S.Afffd | ClusterRevision | True
 * DRLC.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * DRLC.S.C00.Rsp | Report Event Status | True
 * DRLC.S.C01.Rsp | Get Scheduled Events | True
 *
 * Commands Generated
 * DRLC.S.C00.Tx | Load Control Event | True
 * DRLC.S.C01.Tx | Cancel Load Control Event | True
 * DRLC.S.C02.Tx | Cancel All Load Control Events | True
 *
 * Client Attributes
 * DRLC.C.A0000 | UtilityEnrollmentGroup | True
 * DRLC.C.A0001 | StartRandomizeMinutes | True
 * DRLC.C.A0002 | StopRandomizeMinutes | True
 * DRLC.C.A0003 | DeviceClassValue | True
 * DRLC.C.Afffd | ClusterRevision | True
 * DRLC.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * DRLC.C.C00.Rsp | Load Control Event | True
 * DRLC.C.C01.Rsp | Cancel Load Control Event | True
 * DRLC.C.C02.Rsp | Cancel All Load Control Events | True
 *
 * Commands Generated
 * DRLC.C.C00.Tx | Report Event Status | True
 * DRLC.C.C01.Tx | Get Scheduled Events | True
 */

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

/* DRLC Server Attribute IDs */
/* none */

/** DRLC Client Attribute IDs */
enum ZbZclDrlcCliAttrT {
    ZCL_DRLC_CLI_ATTR_UTILITY_ENROL_GRP = 0x0000, /**< UtilityEnrollmentGroup */
    ZCL_DRLC_CLI_ATTR_START_RAND_MINS = 0x0001, /**< StartRandomizationMinutes */
    ZCL_DRLC_CLI_ATTR_DURATION_RAND_MINS = 0x0002, /**< DurationRandomizationMinutes */
    ZCL_DRLC_CLI_ATTR_DEVICE_CLASS = 0x0003 /**< DeviceClassValue */
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

/** Load Control Event command structure */
struct ZbZclDrlcEventT {
    uint32_t issuer_id; /**< Issuer Event ID */
    uint16_t device_class; /**< Device Class */
    uint8_t util_enrol_group; /**< Utility Entrollment Group */
    uint32_t start_time; /**< Start Time - UTC, or 0 = now */
    uint16_t duration; /**< Duration in Minutes */
    enum ZbZclDrlcCriticalityLevelT criticality; /**< Criticality Level */
    uint8_t cool_offset; /**< Cooling Temperature Offset (Optional) */
    uint8_t heat_offset; /**< Heating Temperature Offset (Optional) */
    int16_t cool_setpoint; /**< Cooling Temperature Set Point (Optional) */
    int16_t heat_setpoint; /**< Heating Temperature Set Point (Optional) */
    int8_t avg_load_adj; /**< Average Load Adjustment Percentage (Optional) */
    uint8_t dutycycle; /**< Duty Cycle (Optional) */
    uint8_t event_control; /**< Event Control - e.g. ZCL_DRLC_EVENT_CTRL_RAND_START */
};

/** Cancel Load Control Event command structure */
struct ZbZclDrlcCancelT {
    uint32_t issuer_id; /**< Issuer Event ID */
    uint16_t device_class; /**< Device Class */
    uint8_t util_enrol_group; /**< Utility Enrollment Group */
    uint8_t cancel_control; /**< Cancel Control - e.g. ZCL_DRLC_CANCEL_CONTROL_GRACEFUL */
    /* effectiveTime is now deprecated - must be zero */
    /* uint32_t effectiveTime; */
};

/** Report Event Status command structure */
struct ZbZclDrlcStatusT {
    uint32_t issuer_id; /**< Issuer Event ID */
    enum ZbZclDrlcEventStatusT status; /**< Event Status */
    uint32_t status_time; /**< Event Status Time */
    enum ZbZclDrlcCriticalityLevelT crit_level_applied; /**< Criticality Level Applied */
    uint16_t cool_setpoint_applied; /**< Cooling Temperature Set Point Applied (Optional) - Set to ZCL_DRLC_COOL_SETPOINT_IGNORED if not
    used */
    uint16_t heat_setpoint_applied; /**< Heating Temperature Set Point Applied (Optional) - Set to ZCL_DRLC_HEAT_SETPOINT_IGNORED if not
    used */
    int8_t avg_load_adj_applied; /**< Average Load Adjustment Percentage Applied (Optional) - Set to ZCL_DRLC_AVG_LOAD_ADJ_IGNORED if not
    used */
    uint8_t dutycycle_applied; /**< Duty Cycle Applied (Optional) - Set to ZCL_DRLC_DUTYCYCLE_IGNORED if not used */
    uint8_t event_control; /**< Event Control */
    enum ZbZclDrlcSignatureT sig_type; /**< Signature Type */
    uint8_t sig_data[ZCL_DRLC_SIGNATURE_LENGTH]; /**< Signature (Optional) */
};

/** Get Scheduled Events command structure */
struct ZbZclDrlcGetEventsReqT {
    uint32_t start_time; /**< Start Time */
    uint8_t num_events; /**< Number of Events */
    uint32_t issuer_id; /**< Issuer Event ID (Optional) */
};

/*---------------------------------------------------------------
 * DRLC Server Functions
 *---------------------------------------------------------------
 */

/** DRLC Server callbacks configuration */
struct ZbZclDrlcServerCallbacksT {
    void (*report_status)(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo,
        struct ZbZclDrlcStatusT *status, void *arg);
    /**< Callback to application, invoked on receipt of Report Event Status command */

    enum ZclStatusCodeT (*get_events)(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo,
        struct ZbZclDrlcGetEventsReqT *req, void *arg);
    /**< Callback to application, invoked on receipt of Get Scheduled Events command. The get_events callback handler in the application
     * should return ZCL_STATUS_SUCCESS, or ZCL_STATUS_NOT_FOUND if no events are found. Events are re-issued by calling
     * ZbZclDrlcServerCommandEventReq() */
};

/**
 * Create a new instance of the DRLC Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDrlcServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclDrlcServerCallbacksT *callbacks, void *arg);

/* Server Commands */

/**
 * Send a Load Control Event command
 * @param cluster Cluster instance from which to send this command
 * @param eventPtr Load Control Event command request structure
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDrlcServerCommandEventReq(struct ZbZclClusterT *cluster, struct ZbZclDrlcEventT *eventPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Cancel Load Control Event command
 * @param cluster Cluster instance from which to send this command
 * @param eventPtr Cancel Load Control Event command request structure
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDrlcServerCommandCancelReq(struct ZbZclClusterT *cluster,
    struct ZbZclDrlcCancelT *cancelInfoPtr, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Cancel All Load Control Event command
 * @param cluster Cluster instance from which to send this command
 * @param ctrl Cancel control bitmap, used to determine method of cancellation - e.g. ZCL_DRLC_CANCEL_CONTROL_GRACEFUL
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDrlcServerCommandCancelAllReq(struct ZbZclClusterT *cluster, uint8_t ctrl,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/*---------------------------------------------------------------
 * DRLC Client Functions
 *---------------------------------------------------------------
 */

/** DRLC Client callbacks configuration */
struct ZbZclDrlcClientCallbacksT {
    /* Should return true if started successfully, or false otherwise. */

    bool (*start)(void *arg, struct ZbZclDrlcEventT *event);
    /**< Callback to application, invoked on the start of an event */

    void (*stop)(void *arg, struct ZbZclDrlcEventT *event);
    /**< Callback to application, invoked on the end of an event */
};

/**
 * Create a new instance of the DRLC Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param time_server Time server cluster instance used to retrieve timing information of events
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param cb_arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDrlcClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, struct ZbZclClusterT *time_server,
    struct ZbZclDrlcClientCallbacksT *callbacks, void *cb_arg);

/**
 * Send a Get Event List command
 * @param cluster Cluster instance from which to send this command
 * @param eventList Holds a pointer to the event list if successful
 * @param maxEntries Max event list entries
 * @return Event list size if successful, or 0 on error
 */
unsigned int ZbZclDrlcClientGetEventList(struct ZbZclClusterT *cluster,
    struct ZbZclDrlcEventT *eventList, unsigned int maxEntries);

/* Client Commands */

/**
 * Send a Report Event Status Command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param statusPtr Report Event Status command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDrlcClientCommandReportStatusReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclDrlcStatusT *statusPtr,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Get Scheduled Events
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Scheduled Events command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclDrlcClientCommandGetEventsReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclDrlcGetEventsReqT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif /* __ZCL_DRLC_H */
