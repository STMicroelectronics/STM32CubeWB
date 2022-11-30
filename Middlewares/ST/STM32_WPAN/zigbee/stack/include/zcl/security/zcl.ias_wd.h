/**
 * @file zcl.ias_wd.h
 * @heading IAS WD
 * @brief ZCL Security IAS Warning Device cluster header
 * ZCL 7 section 8.4
 * ZCL 8 section 8.4
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_IAS_WD_H
#define ZCL_IAS_WD_H

/* @PICS.ZCL.IASWD
 *
 * IASWD.S | Server | True
 * IASWD.C | Client | True
 *
 * Server Attributes
 * IASWD.S.A0000 | MaxDuration | True
 * IASWD.S.Afffd | ClusterRevision | True
 * IASWD.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * IASWD.S.C00.Rsp | Start warning | True
 * IASWD.S.C01.Rsp | Squawk | True
 *
 * Client Attributes
 * IASWD.C.Afffd | ClusterRevision | True
 * IASWD.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Generated
 * IASWD.C.C00.Tx | Start warning | True
 * IASWD.C.C01.Tx | Squawk | True
 */

#include "zcl/zcl.h"

/** IAS Warning Device Server Attribute IDs */
enum ZbZclIasWdSvrAttrT {
    ZCL_IAS_WD_SVR_ATTR_MAX_DURATION = 0x0000, /**< MaxDuration */
};

/* IAS Warning Device Client Generated Commands */
enum ZbZclIasWdCliCommandsT {
    ZCL_IAS_WD_CLI_CMD_START_WARNING = 0x00,
    ZCL_IAS_WD_CLI_CMD_SQUAWK = 0x01,
};

/** IAS Warning Device Warning Modes */
enum ZbZclIasWdWarningModeT {
    ZCL_IAS_WD_WARNING_MODE_STOP = 0, /**< Stop (no warning) */
    ZCL_IAS_WD_WARNING_MODE_BURGLAR = 1, /**< Burglar */
    ZCL_IAS_WD_WARNING_MODE_FIRE = 2, /**< Fire */
    ZCL_IAS_WD_WARNING_MODE_EMERGENCY = 3, /**< Emergency */
    ZCL_IAS_WD_WARNING_MODE_POLICE_PANIC = 4, /**< Police panic */
    ZCL_IAS_WD_WARNING_MODE_FIRE_PANIC = 5, /**< Fire panic */
    ZCL_IAS_WD_WARNING_MODE_EMERGENCY_PANIC = 6, /**< Emergency Panic (i.e., medical issue) */
};

/** IAS Warning Device Strobe Field */
enum ZbZclIasWdStrobeT {
    ZCL_IAS_WD_STROBE_OFF = 0, /**< No strobe */
    ZCL_IAS_WD_STROBE_ON = 1, /**< Use strobe in parallel to warning */
};

/** IAS Warning Device Siren Level Field Values */
enum ZbZclIasWdLevelT {
    ZCL_IAS_WD_LEVEL_LOW = 0, /**< Low level sound */
    ZCL_IAS_WD_LEVEL_MEDIUM = 1, /**< Medium level sound */
    ZCL_IAS_WD_LEVEL_HIGH = 2, /**< High level sound */
    ZCL_IAS_WD_LEVEL_VERY_HIGH = 3, /**< Very high level sound */
};

/** IAS Warning Device Squawk Mode Field */
enum ZbZclIasWdSquawkModeT {
    ZCL_IAS_WD_SQUAWK_MODE_ARMED = 0, /**< Notification sound for “System is armed” */
    ZCL_IAS_WD_SQUAWK_MODE_DISARMED = 1, /**< Notification sound for "System is disarmed" */
};

/** IAS Warning Device Client Start Warning command structure */
struct ZbZclIasWdClientStartWarningReqT {
    enum ZbZclIasWdWarningModeT warning_mode; /**< Warning Mode */
    enum ZbZclIasWdStrobeT strobe; /**< Strobe */
    enum ZbZclIasWdLevelT siren_level; /**< Siren Level */
    uint16_t warning_duration; /**< Warning Duration */
    uint8_t strobe_dutycycle; /**< Strobe Duty Cycle */
    enum ZbZclIasWdLevelT strobe_level; /**< Strobe Level */
};

/** IAS Warning Device Client Squawk command structure */
struct ZbZclIasWdClientSquawkReqT {
    enum ZbZclIasWdSquawkModeT squawk_mode; /**< Squawk mode */
    enum ZbZclIasWdStrobeT strobe; /**< Strobe */
    enum ZbZclIasWdLevelT squawk_level; /**< Squawk level */
};

/* IAS Warning Device Server */

/** IAS Warning Server callbacks configuration */
struct ZbZclIasWdServerCallbacksT {
    enum ZclStatusCodeT (*start_warning)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclIasWdClientStartWarningReqT *warn_req);
    /**< Callback to handle Start Warning command.
     * ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */

    enum ZclStatusCodeT (*squawk)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclIasWdClientSquawkReqT *squawk_req);
    /**< Callback to handle Squawk command.
     * ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error.
     */
};

/**
 * Create a new instance of the IAS Warning Device Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIasWdServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclIasWdServerCallbacksT *callbacks, void *arg);

/* IAS Warning Device Client */

/**
 * Create a new instance of the IAS Warning Device Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIasWdClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, void *arg);

/**
 * Send a Start Warning command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param warning_req IAS Warning Device Client Start Warning command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasWdClientStartWarningReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    struct ZbZclIasWdClientStartWarningReqT *warning_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Squawk command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param squawk_req IAS Warning Device Client Squawk command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasWdClientSquawkReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    struct ZbZclIasWdClientSquawkReqT *squawk_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif
