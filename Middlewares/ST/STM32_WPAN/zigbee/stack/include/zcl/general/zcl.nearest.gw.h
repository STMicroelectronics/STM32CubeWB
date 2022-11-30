/**
 * @file zcl.nearest.gw.h
 * @heading Nearest Gateway
 * @brief ZCL Nearest Gateway cluster header
 * ZCL 7 section 14.5
 * ZCL 8 section 14.5
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_NEAREST_GW_H
#define ZCL_NEAREST_GW_H

/* NOTE: The following PICS statement is for internal Exegin use only, and is not a product of the Zigbee Alliance. */
/* @PICS.ZCL.nearest.gw
 * NEARGW.S | Server | True
 * NEARGW.C | Client | True
 *
 * Server Attributes
 * NEARGW.S.A0000 | Nearest Gateway | True
 * NEARGW.S.A0001 | New Mobile Node | True
 * NEARGW.S.Afffd | ClusterRevision | True
 * NEARGW.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * NEARGW.C.Afffd | ClusterRevision | True
 * NEARGW.C.Afffe | AttributeReportingStatus | True
 */

#include "zcl/zcl.h"

/* Nearest Gateway cluster interface definition */

/** Nearest Gateway Attribute Ids */
enum ZbZclNearestGwServerAttrT {
    ZCL_NEAREST_GW_SVR_ATTR_NEAREST_GW = 0x0000, /**< Nearest Gateway */
    ZCL_NEAREST_GW_SVR_ATTR_NEW_MOBILE_NODE = 0x0001, /**< New Mobile Node */
};

/* Nearest Gateway cluster defines */
#define ZCL_NEAREST_GW_DEFAULT_NEAREST_GW             0x0000
#define ZCL_NEAREST_GW_DEFAULT_NEW_MOBILE_NODE        0x0000

/* Nearest Gateway cluster function prototypes */
/**
 * Instantiate a new instance of the Nearest Gateway client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclNearestGwClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Instantiate a new instance of the Nearest Gateway server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclNearestGwServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif
