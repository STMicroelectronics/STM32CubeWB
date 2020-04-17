/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Keep Alive cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_KEEPALIVE_H
# define ZCL_KEEPALIVE_H

#include "zcl/zcl.h"

/* Attributes */
enum {
    ZCL_KEEPALIVE_ATTR_BASE = 0x0000, /* minutes (valid range is from 0x01 to 0xff, but not enforced by cluster, for testing) */
    ZCL_KEEPALIVE_ATTR_JITTER = 0x0001 /* seconds (valid range is from 0x0000 to 0x0200) */
};

#define ZCL_KEEPALIVE_BASE_MIN              0x01U
#define ZCL_KEEPALIVE_BASE_MAX              0xffU
#define ZCL_KEEPALIVE_BASE_DEFAULT          10U /* minutes */

#define ZCL_KEEPALIVE_JITTER_MIN            0x0000U
#define ZCL_KEEPALIVE_JITTER_MAX            0x0200U
#define ZCL_KEEPALIVE_JITTER_DEFAULT        300U /* seconds */

/* Server */
struct ZbZclClusterT * ZbZclKeepAliveServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Client */
/* Allocate the Keepalive Client cluster.
 * The tcso_callback is called when the TCSO is ever started (ZB_TCSO_STATUS_DISCOVERY_UNDERWAY),
 * and when it completes, with the resultant status. */
struct ZbZclClusterT * ZbZclKeepAliveClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    void (*tcso_callback)(enum ZbTcsoStatusT status, void *arg), void *tcso_arg);

void ZbZclKeepAliveClientStart(struct ZbZclClusterT *clusterPtr);
void ZbZclKeepAliveClientStop(struct ZbZclClusterT *clusterPtr);

#endif /* __ZCL_KEEPALIVE_H */
