/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_DIAGNOSTICS_H
#define ZCL_DIAGNOSTICS_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Diagnostics Cluster
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Diagnostics
 * DIAG.S | True
 * DIAG.C | True
 *
 * Server Attributes
 * DIAG.S.A0000 | False
 * DIAG.S.A0001 | False
 * DIAG.S.A0100 | True
 * DIAG.S.A0101 | True
 * DIAG.S.A0102 | True
 * DIAG.S.A0103 | True
 * DIAG.S.A0104 | True
 * DIAG.S.A0105 | True
 * DIAG.S.A0106 | True
 * DIAG.S.A0107 | True
 * DIAG.S.A0108 | True
 * DIAG.S.A0109 | True
 * DIAG.S.A010a | True
 * DIAG.S.A010b | True
 * DIAG.S.A010c | True
 * DIAG.S.A010d | True
 * DIAG.S.A010e | True
 * DIAG.S.A010f | True
 * DIAG.S.A0110 | True
 * DIAG.S.A0111 | True
 * DIAG.S.A0112 | True
 * DIAG.S.A0113 | True
 * DIAG.S.A0114 | False
 * DIAG.S.A0115 | True
 * DIAG.S.A0116 | True
 * DIAG.S.A0117 | True
 * DIAG.S.A0118 | True
 * DIAG.S.A0119 | False
 * DIAG.S.A011a | True
 * DIAG.S.A011b | False
 * DIAG.S.A011c | True
 * DIAG.S.A011d | True
 * DIAG.S.Afffd | True
 * DIAG.S.Afffe | False
 *
 * Client Attributes
 * DIAG.C.Afffd | True
 * DIAG.C.Afffe | False
 */

/** Diagnostics Server Attribute IDs */
enum ZbZclDiagSvrAttrT {
    ZCL_DIAG_SVR_ATTR_RESETS = 0x0000,
    ZCL_DIAG_SVR_ATTR_PERSIST_WRITES = 0x0001,
    ZCL_DIAG_SVR_ATTR_MAC_RX_BCAST = 0x0100,
    ZCL_DIAG_SVR_ATTR_MAC_TX_BCAST = 0x0101,
    ZCL_DIAG_SVR_ATTR_MAC_RX_UCAST = 0x0102,
    ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST = 0x0103,
    ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST_RETRY = 0x0104,
    ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST_FAIL = 0x0105,
    ZCL_DIAG_SVR_ATTR_APS_RX_BCAST = 0x0106,
    ZCL_DIAG_SVR_ATTR_APS_TX_BCAST = 0x0107,
    ZCL_DIAG_SVR_ATTR_APS_RX_UCAST = 0x0108,
    ZCL_DIAG_SVR_ATTR_APS_TX_UCAST_SUCCESS = 0x0109,
    ZCL_DIAG_SVR_ATTR_APS_TX_UCAST_RETRY = 0x010a,
    ZCL_DIAG_SVR_ATTR_APS_TX_UCAST_FAIL = 0x010b,
    ZCL_DIAG_SVR_ATTR_ROUTE_DISC_INIT = 0x010c,
    ZCL_DIAG_SVR_ATTR_NEIGHBOR_ADDED = 0x010d,
    ZCL_DIAG_SVR_ATTR_NEIGHBOUR_REMOVED = 0x010e,
    ZCL_DIAG_SVR_ATTR_NEIGHBOUR_STALE = 0x010f,
    ZCL_DIAG_SVR_ATTR_JOIN_IND = 0x0110,
    ZCL_DIAG_SVR_ATTR_CHILD_MOVED = 0x0111,
    ZCL_DIAG_SVR_ATTR_NWK_FC_FAILURE = 0x0112,
    ZCL_DIAG_SVR_ATTR_APS_FC_FAILURE = 0x0113,
    ZCL_DIAG_SVR_ATTR_APS_UNAUTH_KEY = 0x0114,
    ZCL_DIAG_SVR_ATTR_NWK_DECRYPT_FAILS = 0x0115,
    ZCL_DIAG_SVR_ATTR_APS_DECRYPT_FAILS = 0x0116,
    ZCL_DIAG_SVR_ATTR_PACKET_BUF_ALLOC_FAILS = 0x0117,
    ZCL_DIAG_SVR_ATTR_RELAYED_UCAST = 0x0118,
    ZCL_DIAG_SVR_ATTR_PHY_MAC_QUEUE_LIM = 0x0119,
    ZCL_DIAG_SVR_ATTR_PACKET_VAL_DROP_COUNT = 0x011a,
    ZCL_DIAG_SVR_ATTR_AVG_MAC_RETRY_PER_APS_MSG = 0x011b,
    ZCL_DIAG_SVR_ATTR_LAST_MSG_LQI = 0x011c,
    ZCL_DIAG_SVR_ATTR_LAST_MSG_RSSI = 0x011d,
};

/*---------------------------------------------------------------
 * Diagnostics Client
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclDiagClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/*---------------------------------------------------------------
 * Diagnostics Server
 *---------------------------------------------------------------
 */
/* endpoint - set the Endpoint for this cluster */
/* profileId - set the Profile ID, unless set to ZCL_PROFILE_WILDCARD. */
/* minSecurity - can be either: ZB_APS_STATUS_UNSECURED, ZB_APS_STATUS_SECURED_NWK_KEY, or ZB_APS_STATUS_SECURED_LINK_KEY */
/* Note, only one diagnostics server can be allocated on the device. */
bool ZbZclDiagServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId, enum ZbStatusCodeT minSecurity);

#endif /* __ZCL_DIAGNOSTICS_H */
