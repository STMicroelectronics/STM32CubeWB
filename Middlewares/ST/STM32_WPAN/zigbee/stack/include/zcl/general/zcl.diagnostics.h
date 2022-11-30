/**
 * @file zcl.diagnostics.h
 * @heading Diagnostics
 * @brief ZCL Diagnostics cluster header
 * ZCL 7 section 3.15
 * ZCL 8 section 3.15
 * @copyright Copyright [2019 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_DIAGNOSTICS_H
#define ZCL_DIAGNOSTICS_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Diagnostics Cluster
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Diagnostics
 * DIAG.S | Server | True
 * DIAG.C | Client | True
 *
 * Server Attributes
 * DIAG.S.A0000 | NumberOfResets | False | Optional
 * DIAG.S.A0001 | PersistentMemoryWrites | False | Optional
 * DIAG.S.A0100 | MacRxBcast | True | Optional
 * DIAG.S.A0101 | MacTxBcast | True | Optional
 * DIAG.S.A0102 | MacRxUcast | True | Optional
 * DIAG.S.A0103 | MacTxUcast | True | Optional
 * DIAG.S.A0104 | MacTxUcastRetry | True | Optional
 * DIAG.S.A0105 | MacTxUcastFail | True | Optional
 * DIAG.S.A0106 | APSRxBcast | True | Optional
 * DIAG.S.A0107 | APSTxBcast | True | Optional
 * DIAG.S.A0108 | APSRxUcast | True | Optional
 * DIAG.S.A0109 | APSTxUcastSuccess | True | Optional
 * DIAG.S.A010a | APSTxUcastRetry | True | Optional
 * DIAG.S.A010b | APSTxUcastFail | True | Optional
 * DIAG.S.A010c | RouteDiscInitiated | True | Optional
 * DIAG.S.A010d | NeighborAdded | True | Optional
 * DIAG.S.A010e | NeighborRemoved | True | Optional
 * DIAG.S.A010f | NeighborStale | True | Optional
 * DIAG.S.A0110 | JoinIndication | True | Optional
 * DIAG.S.A0111 | ChildMoved | True | Optional
 * DIAG.S.A0112 | NWKFCFailure | True | Optional
 * DIAG.S.A0113 | APSFCFailure | True | Optional
 * DIAG.S.A0114 | APSUnauthorizedKey | True | Optional
 * DIAG.S.A0115 | NWKDecryptFailures | True | Optional
 * DIAG.S.A0116 | APSDecryptFailures | True | Optional
 * DIAG.S.A0117 | PacketBufferAllocateFailures | True | Optional
 * DIAG.S.A0118 | RelayedUcast | True | Optional
 * DIAG.S.A0119 | PhytoMACqueuelimitreached | False | Optional
 * DIAG.S.A011a | PacketValidatedropcount | True | Optional
 * DIAG.S.A011b | AverageMACRetryPerAPSMessageSent | False | Optional
 * DIAG.S.A011c | LastMessageLQI | True | Optional
 * DIAG.S.A011d | LastMessageRSSI | True | Optional
 * DIAG.S.Afffd | ClusterRevision | True
 * DIAG.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * DIAG.C.Afffd | ClusterRevision | True
 * DIAG.C.Afffe | AttributeReportingStatus | True
 */

/*lint -save -e621 "Identifier clash (symbol 'ZCL_DIAG_SVR_ATTR_APS_TX_UCAST_FAIL'
 [MISRA Rule 5.1 REQUIRED, MISRA Rule 5.2 REQUIRED, MISRA Rule 5.4 REQUIRED, MISRA Rule 5.5 REQUIRED]" */

/** Diagnostics Server Attribute IDs */
enum ZbZclDiagSvrAttrT {
    ZCL_DIAG_SVR_ATTR_RESETS = 0x0000, /**< NumberOfResets (Optional) */
    ZCL_DIAG_SVR_ATTR_PERSIST_WRITES = 0x0001, /**< PersistentMemoryWrites (Optional) */
    ZCL_DIAG_SVR_ATTR_MAC_RX_BCAST = 0x0100, /**< MacRxBcast (Optional) */
    ZCL_DIAG_SVR_ATTR_MAC_TX_BCAST = 0x0101, /**< MacTxBcast (Optional) */
    ZCL_DIAG_SVR_ATTR_MAC_RX_UCAST = 0x0102, /**< MacRxUcast (Optional) */
    ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST = 0x0103, /**< MacTxUcast (Optional) */
    ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST_RETRY = 0x0104, /**< MacTxUcastRetry (Optional) */
    ZCL_DIAG_SVR_ATTR_MAC_TX_UCAST_FAIL = 0x0105, /**< MacTxUcastFail (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_RX_BCAST = 0x0106, /**< APSRxBcast (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_TX_BCAST = 0x0107, /**< APSTxBcast (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_RX_UCAST = 0x0108, /**< APSRxUcast (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_TX_UCAST_SUCCESS = 0x0109, /**< APSTxUcastSuccess (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_TX_UCAST_RETRY = 0x010a, /**< APSTxUcastRetry (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_TX_UCAST_FAIL = 0x010b, /**< APSTxUcastFail (Optional) */
    ZCL_DIAG_SVR_ATTR_ROUTE_DISC_INIT = 0x010c, /**< RouteDiscInitiated (Optional) */
    ZCL_DIAG_SVR_ATTR_NEIGHBOR_ADDED = 0x010d, /**< NeighborAdded (Optional) */
    ZCL_DIAG_SVR_ATTR_NEIGHBOUR_REMOVED = 0x010e, /**< NeighborRemoved (Optional) */
    ZCL_DIAG_SVR_ATTR_NEIGHBOUR_STALE = 0x010f, /**< NeighborStale (Optional) */
    ZCL_DIAG_SVR_ATTR_JOIN_IND = 0x0110, /**< JoinIndication (Optional) */
    ZCL_DIAG_SVR_ATTR_CHILD_MOVED = 0x0111, /**< ChildMoved (Optional) */
    ZCL_DIAG_SVR_ATTR_NWK_FC_FAILURE = 0x0112, /**< NWKFCFailure (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_FC_FAILURE = 0x0113, /**< APSFCFailure (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_UNAUTH_KEY = 0x0114, /**< APSUnauthorizedKey (Optional) */
    ZCL_DIAG_SVR_ATTR_NWK_DECRYPT_FAILS = 0x0115, /**< NWKDecryptFailures (Optional) */
    ZCL_DIAG_SVR_ATTR_APS_DECRYPT_FAILS = 0x0116, /**< APSDecryptFailures (Optional) */
    ZCL_DIAG_SVR_ATTR_PACKET_BUF_ALLOC_FAILS = 0x0117, /**< PacketBufferAllocateFailures (Optional) */
    ZCL_DIAG_SVR_ATTR_RELAYED_UCAST = 0x0118, /**< RelayedUcast (Optional) */
    ZCL_DIAG_SVR_ATTR_PHY_MAC_QUEUE_LIM = 0x0119, /**< PhytoMACqueuelimitreached (Optional) */
    ZCL_DIAG_SVR_ATTR_PACKET_VAL_DROP_COUNT = 0x011a, /**< PacketValidatedropcount (Optional) */
    ZCL_DIAG_SVR_ATTR_AVG_MAC_RETRY_PER_APS_MSG = 0x011b, /**< AverageMACRetryPerAPSMessageSent (Optional) */
    ZCL_DIAG_SVR_ATTR_LAST_MSG_LQI = 0x011c, /**< LastMessageLQI (Optional) */
    ZCL_DIAG_SVR_ATTR_LAST_MSG_RSSI = 0x011d /**< LastMessageRSSI (Optional) */
};
/*lint -restore */

/*---------------------------------------------------------------
 * Diagnostics Client
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Diagnostics Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDiagClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/*---------------------------------------------------------------
 * Diagnostics Server
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Diagnostics Server cluster. Only one Diagnostics Server can be allocated on the device
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param profileId Profile ID setting, unless set to ZCL_PROFILE_WILDCARD
 * @param minSecurity Minimum security level can be either: ZB_APS_STATUS_UNSECURED, ZB_APS_STATUS_SECURED_NWK_KEY, or
 *  ZB_APS_STATUS_SECURED_LINK_KEY
 * @return True on success, false otherwise
 */
bool ZbZclDiagServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId, enum ZbStatusCodeT minSecurity);

#endif /* __ZCL_DIAGNOSTICS_H */
