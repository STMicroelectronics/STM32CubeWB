/**
 * @file zcl.time.h
 * @heading Time
 * @brief ZCL Time cluster header
 * ZCL 7 section 3.12
 * ZCL 8 section 3.12
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_TIME_H
# define ZCL_TIME_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Time cluster.
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Time
 * T.S | Server | True
 * T.C | Client | True
 *
 * Server Attributes
 * T.S.A0000 | Time | True
 * T.S.A0001 | TimeStatus | True
 * T.S.A0002 | TimeZone | True | Optional
 * T.S.A0003 | DstStart | True | Optional
 * T.S.A0004 | DstEnd | True | Optional
 * T.S.A0005 | DstShift | True | Optional
 * T.S.A0006 | StandardTime | True | Optional
 * T.S.A0007 | LocalTime | True | Optional
 * T.S.A0008 | LastSetTime | True | Optional
 * T.S.A0009 | ValidUntilTime | True | Optional
 * T.S.Afffd | ClusterRevision | True
 * T.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * T.C.Afffd | ClusterRevision | True
 * T.C.Afffe | AttributeReportingStatus | True
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */

/** Time Server Attribute IDs */
enum ZbZclTimeSvrAttrT {
    ZCL_TIME_ATTR_TIME = 0x0000, /**< Time */
    ZCL_TIME_ATTR_STATUS = 0x0001, /**< TimeStatus */
    ZCL_TIME_ATTR_TIME_ZONE = 0x0002, /**< TimeZone (Optional) */
    ZCL_TIME_ATTR_DST_START = 0x0003, /**< DstStart (Optional) */
    ZCL_TIME_ATTR_DST_END = 0x0004, /**< DstEnd (Optional) */
    ZCL_TIME_ATTR_DST_SHIFT = 0x0005, /**< DstShift (Optional) */
    ZCL_TIME_ATTR_STANDARD_TIME = 0x0006, /**< StandardTime (Optional) */
    ZCL_TIME_ATTR_LOCAL_TIME = 0x0007, /**< LocalTime (Optional) */
    ZCL_TIME_ATTR_LAST_SET_TIME = 0x0008, /**< LastSetTime (Optional) */
    ZCL_TIME_ATTR_VALID_UNTIL_TIME = 0x0009 /**< ValidUntilTime (Optional) */
};

/* Time status bits */
#define ZCL_TIME_STATUS_MASTER               0x01U
#define ZCL_TIME_STATUS_SYNCHRONIZED         0x02U
#define ZCL_TIME_STATUS_ZONE_MASTER          0x04U
#define ZCL_TIME_STATUS_SUPERSEDING          0x08U

/* ZigBee Epoch in other time formats. */
#define ZCL_TIME_EPOCH_NTP                   0xBC17C20000000000ULL
#define ZCL_TIME_EPOCH_NTP_SECONDS           0xBC17C200UL

/* January 1, 2000, which equates to 946,684,800 seconds. */
#define ZCL_TIME_EPOCH_UNIX                  0x386D4380UL

#define ZCL_TIME_TIME_ZONE_MIN              (-86400)
#define ZCL_TIME_TIME_ZONE_MAX              (86400)

#define ZCL_TIME_DST_SHIFT_MIN              (-86400)
#define ZCL_TIME_DST_SHIFT_MAX              (86400)

/*---------------------------------------------------------------
 * Time Server
 *---------------------------------------------------------------
 */

/** Time Server callbacks configuration */
struct ZbZclTimeServerCallbacks {
    uint32_t (*get_time)(struct ZbZclClusterT *clusterPtr, void *arg);
    /**< Callback to application, invoked on receipt of Read Attribute request */

    void (*set_time)(struct ZbZclClusterT *clusterPtr, uint32_t time_val, void *arg);
    /**< Callback to application, invoked on receipt of Write Attribute request for the ZCL_TIME_ATTR_TIME attribute. The set_time app
     * callback should also set the ZCL_TIME_ATTR_LAST_SET_TIME attribute if successful. */
};

/**
 * Create a new instance of the Time Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclTimeServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclTimeServerCallbacks *callbacks, void *arg);

/* Wrapper to get/set the current time. This is only applicable if ZbZclTimeServerAlloc() was called with use_stack_uptime=true. */

/**
 * Call the set_time callback defined as part of the Time Server callbacks configuration
 * @param cluster Cluster instance from which to send this command
 * @param current_time New current time value to set
 * @return Void
 */
void ZbZclTimeServerSetTime(struct ZbZclClusterT *cluster, uint32_t current_time);

/**
 * Call the get_time callback defined as part of the Time Server callbacks configuration
 * @param cluster Cluster instance from which to send this command
 * @return Current time
 */
uint32_t ZbZclTimeServerCurrentTime(struct ZbZclClusterT *cluster);

/*---------------------------------------------------------------
 * Time Client
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Time Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclTimeClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_TIME_H */
