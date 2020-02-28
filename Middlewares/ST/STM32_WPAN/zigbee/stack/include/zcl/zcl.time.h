/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_TIME_H
# define ZCL_TIME_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Time cluster.
 *--------------------------------------------------------------------------
 */
/* PICS.ZCL.Time
 * T.S | True
 * T.C | True
 *
 * Server Attributes
 * T.S.A0000 | True
 * T.S.A0001 | True
 * T.S.A0002 | True
 * T.S.A0003 | True
 * T.S.A0004 | True
 * T.S.A0005 | True
 * T.S.A0006 | True
 * T.S.A0007 | True
 * T.S.A0008 | True
 * T.S.A0009 | True
 * T.S.AFFFD | True
 * 
 * Client Attributes
 * T.C.AFFFD | True
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */
/* Time cluster attributes */
enum {
    ZCL_TIME_ATTR_TIME = 0x0000,
    ZCL_TIME_ATTR_STATUS = 0x0001,
    ZCL_TIME_ATTR_TIME_ZONE = 0x0002,
    ZCL_TIME_ATTR_DST_START = 0x0003,
    ZCL_TIME_ATTR_DST_END = 0x0004,
    ZCL_TIME_ATTR_DST_SHIFT = 0x0005,
    ZCL_TIME_ATTR_STANDARD_TIME = 0x0006,
    ZCL_TIME_ATTR_LOCAL_TIME = 0x0007,
    ZCL_TIME_ATTR_LAST_SET_TIME = 0x0008,
    ZCL_TIME_ATTR_VALID_UNTIL_TIME = 0x0009
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

/*---------------------------------------------------------------
 * Time Server
 *---------------------------------------------------------------
 */
struct ZbZclTimeServerCallbacks {
    uint32_t (*get_time)(struct ZbZclClusterT *clusterPtr, void *arg);
    void (*set_time)(struct ZbZclClusterT *clusterPtr, uint32_t time_val, void *arg);
};

struct ZbZclClusterT * ZbZclTimeServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclTimeServerCallbacks *callbacks, void *arg);

/*---------------------------------------------------------------
 * Time Server Helper Functions
 *---------------------------------------------------------------
 */
/* Wrappers to read/write the server's ZCL_TIME_ATTR_STATUS attribute */
void ZbZclTimeServerSetSynchronized(struct ZbZclClusterT *clusterPtr, bool isSynchronized);
bool ZbZclTimeServerIsSynchronized(struct ZbZclClusterT *clusterPtr);
void ZbZclTimeServerSetStatusMaster(struct ZbZclClusterT *clusterPtr, bool enable);

/* Wrapper to get/set the current time. This is only applicable if ZbZclTimeServerAlloc() was called with use_stack_uptime=true. */
void ZbZclTimeServerSetTime(struct ZbZclClusterT *clusterPtr, uint32_t currentTime);
uint32_t ZbZclTimeServerCurrentTime(struct ZbZclClusterT *clusterPtr);

/*---------------------------------------------------------------
 * Time Client
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclTimeClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_TIME_H */
