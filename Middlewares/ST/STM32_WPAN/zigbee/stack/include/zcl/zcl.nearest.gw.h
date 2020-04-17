/* Copyright [2019 - 2019] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the Nearest Gateway cluster.
 *--------------------------------------------------------------------------
 */

#ifndef ZCL_NEAREST_GW_H
#define ZCL_NEAREST_GW_H

#include "zcl/zcl.h"

/* Attribute Identifiers */
enum {
    ZCL_NEAREST_GW_ATTR_NEAREST_GW = 0x0000,
    ZCL_NEAREST_GW_ATTR_NEW_MOBILE_NODE = 0x0001,
};

#define ZCL_NEAREST_GW_DEFAULT_NEAREST_GW             0x0000
#define ZCL_NEAREST_GW_DEFAULT_NEW_MOBILE_NODE        0x0000

/* Allocation Functions */
struct ZbZclClusterT * ZbZclNearestGwClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclNearestGwServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* ZCL_NEAREST_GW_H */
