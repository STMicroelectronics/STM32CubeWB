/**
 * @file zigbee.zdo.h
 * @brief Zigbee ZDO header file
 * @author Exegin Technologies Limited
 * @copyright Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved.
 *
 * This file provides the declaration of all the public API's and macros used by the ZDO layer.
 */

#ifndef ZIGBEE_ZDO_H
# define ZIGBEE_ZDO_H

struct ZbZdoFilterT;

/*---------------------------------------------------------------
 * Misc. Definitions and Structures
 *---------------------------------------------------------------
 */
/* Profile, and Device Identifiers for the ZDO. */
#define ZB_ZDO_PROFILE_ID                           0x0000U
#define ZB_ZDO_DEVICE_ID                            0x0000U
#define ZB_ZDO_DEVICE_VER                           0x00U

#define ZB_ZDO_ADDRRSP_DEVLIST_MAX                  32U

/** NWK_addr_req / IEEE_addr_req definitions */
enum ZbZdoAddrReqTypeT {
    ZB_ZDO_ADDR_REQ_TYPE_SINGLE = 0x00, /**< Single address request - Returns the information requested only */
    ZB_ZDO_ADDR_REQ_TYPE_EXTENDED = 0x01 /**< Extended address request - Returns the information requested and appends the devices in its network table */
};

/* NWK_Update_Req special duration values. */
#define ZB_ZDP_NWK_UPDATE_MAX_DURATION              0x05U
#define ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH            0xfeU
#define ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS        0xffU

/* Maximum Response Sizes */
/* NOTE was 64, but end up allocating too much on the stack */
#define ZB_ZDO_CLUSTER_LIST_MAX_SZ                  16U
#define ZB_ZDO_NETWORK_LIST_MAX_SZ                  8U
#define ZB_ZDO_NEIGHBOR_LIST_MAX_SZ                 4U
#define ZB_ZDO_ROUTE_LIST_MAX_SZ                    16U
/* Each entry in the ZDO message is either 14 or 21 bytes long.
 * It's only possible to return up to 4 entries at a time. */
#define ZB_ZDO_BIND_LIST_MAX_SZ                     4U
#define ZB_ZDO_ENDPOINT_LIST_MAXSZ                  32U
#define ZB_ZDO_CHANNEL_LIST_MAXSZ                   (8U * sizeof(uint32_t))
#define ZB_ZDO_CHILD_LIST_MAXSZ                     8U

/* Node Descriptor Definitions. */
#define ZB_ZDP_NODE_TYPE_COORDINATOR                (uint8_t)0x00U
#define ZB_ZDP_NODE_TYPE_ROUTER                     (uint8_t)0x01U
#define ZB_ZDP_NODE_TYPE_END_DEVICE                 (uint8_t)0x02U
#define ZB_ZDP_NODE_FREQ_868MHz                     (uint8_t)0x01U
#define ZB_ZDP_NODE_FREQ_900MHz                     (uint8_t)0x04U
#define ZB_ZDP_NODE_FREQ_2400MHz                    (uint8_t)0x08U
#define ZB_ZDP_NODE_CAPABILITY_EXT_EPLIST           (uint8_t)0x01U
#define ZB_ZDP_NODE_CAPABILITY_EXT_SIMPLE           (uint8_t)0x02U

/* Server Mask Bits. */
#define ZB_ZDP_SERVER_PRIMARY_TRUST                 (uint16_t)0x0001U
#define ZB_ZDP_SERVER_BACKUP_TRUST                  (uint16_t)0x0002U
#define ZB_ZDP_SERVER_PRIMARY_BIND_CACHE            (uint16_t)0x0004U
#define ZB_ZDP_SERVER_BACKUP_BIND_CACHE             (uint16_t)0x0008U
#define ZB_ZDP_SERVER_PRIMARY_DISC_CACHE            (uint16_t)0x0010U
#define ZB_ZDP_SERVER_BACKUP_DISC_CACHE             (uint16_t)0x0020U
#define ZB_ZDP_SERVER_NETWORK_MGR                   (uint16_t)0x0040U
#define ZB_ZDP_SERVER_RESERVED_MASK                 (uint16_t)0x0180U /* Reserved bits */
#define ZB_ZDP_SERVER_REVISION_SHIFT                9U
#define ZB_ZDP_SERVER_REVISION_MASK                 (uint16_t)0xFE00U /* (0x007f << ZB_ZDP_SERVER_REVISION_SHIFT) */

/* Power Descriptor Definitions. */
#define ZB_ZDP_POWER_MODE_ON                        (uint8_t)0x00U
#define ZB_ZDP_POWER_MODE_PERIODIC                  (uint8_t)0x01U
#define ZB_ZDP_POWER_MODE_REQUEST                   (uint8_t)0x02U
#define ZB_ZDP_POWER_SOURCE_CONSTANT                (uint8_t)0x01U
#define ZB_ZDP_POWER_SOURCE_RECHARGEABLE            (uint8_t)0x02U
#define ZB_ZDP_POWER_SOURCE_DISPOSEABLE             (uint8_t)0x04U
#define ZB_ZDP_POWER_LEVEL_CRITICAL                 (uint8_t)0x00U
#define ZB_ZDP_POWER_LEVEL_33                       (uint8_t)0x04U
#define ZB_ZDP_POWER_LEVEL_66                       (uint8_t)0x08U
#define ZB_ZDP_POWER_LEVEL_100                      (uint8_t)0x0cU

/* struct ZbNodeDescriptorT Mask Flag - Section 2.3.2.3  */
#define ZB_ZDP_NODE_OPT_MASK_TYPE                   (uint16_t)0x0007U
#define ZB_ZDP_NODE_OPT_MASK_COMPLEX                (uint16_t)0x0008U
#define ZB_ZDP_NODE_OPT_MASK_USER                   (uint16_t)0x0010U
#define ZB_ZDP_NODE_OPT_MASK_APSFLAG                (uint16_t)0x0700U
#define ZB_ZDP_NODE_OPT_MASK_FREQBAND               (uint16_t)0xf800U
#define ZB_ZDP_NODE_OPT_MASK_RESERVED               (uint16_t)0x00e0U

#define ZB_ZDP_NODE_OPT_OFFSET_TYPE                 0U
#define ZB_ZDP_NODE_OPT_OFFSET_COMPLEX              3U
#define ZB_ZDP_NODE_OPT_OFFSET_USER                 4U
#define ZB_ZDP_NODE_OPT_OFFSET_APSFLAG              8U
#define ZB_ZDP_NODE_OPT_OFFSET_FREQBAND             11U

/* Node Descriptor Definitions */
#define ZB_ZDP_NODE_OPT_TYPE_COORDINATOR            0x00U
#define ZB_ZDP_NODE_OPT_TYPE_ROUTER                 0x01U
#define ZB_ZDP_NODE_OPT_TYPE_END_DEVICE             0x02U
#define ZB_ZDP_NODE_OPT_TYPE(_nodetype_)            (_nodetype_)

#define ZB_ZDP_NODE_OPT_FREQBAND_868MHZ_BPSK        0x01U
#define ZB_ZDP_NODE_OPT_FREQBAND_915MHZ_BPSK        0x04U
#define ZB_ZDP_NODE_OPT_FREQBAND_2400MHZ            0x08U
#define ZB_ZDP_NODE_OPT_FREQBAND_EU_FSK_SUBGHZ      0x10U /* 868 MHz and 915 MHz */
#define ZB_ZDP_NODE_OPT_BAND(_freqband_)            (((uint16_t)_freqband_) << ZB_ZDP_NODE_OPT_OFFSET_FREQBAND)

/* struct ZbPowerDescriptorT Mask Flag - Section 2.3.2.4 */
#define ZB_ZDP_POWER_MASK_MODE                      0x000fU
#define ZB_ZDP_POWER_MASK_AVAILABLE                 0x00f0U
#define ZB_ZDP_POWER_MASK_SOURCE                    0x0f00U
#define ZB_ZDP_POWER_MASK_LEVEL                     0xf000U

#define ZB_ZDP_POWER_OFFSET_MODE                    0U
#define ZB_ZDP_POWER_OFFSET_AVAILABLE               4U
#define ZB_ZDP_POWER_OFFSET_SOURCE                  8U
#define ZB_ZDP_POWER_OFFSET_LEVEL                   12U

/* Complex descriptor tag values. */
#define ZB_ZDP_COMPLEX_TAG_LANGUAGE                 0x01U
#define ZB_ZDP_COMPLEX_TAG_MANUFACTURFER            0x02U
#define ZB_ZDP_COMPLEX_TAG_MODEL                    0x03U
#define ZB_ZDP_COMPLEX_TAG_SERIAL                   0x04U
#define ZB_ZDP_COMPLEX_TAG_DEVICE_URL               0x05U
#define ZB_ZDP_COMPLEX_TAG_ICON                     0x06U
#define ZB_ZDP_COMPLEX_TAG_ICON_URL                 0x07U

/* Network Descriptor Flags. */
#define ZB_ZDP_NETWORK_MASK_PROFILE                 0x0fU
#define ZB_ZDP_NETWORK_MASK_VERSION                 0xf0U
#define ZB_ZDP_NETWORK_MASK_BEACON                  0x0fU
#define ZB_ZDP_NETWORK_MASK_SUPERFRAME              0xf0U
#define ZB_ZDP_NETWORK_MASK_PERMIT_JOIN             0x01U

#define ZB_ZDP_NETWORK_OFFSET_PROFILE               0U
#define ZB_ZDP_NETWORK_OFFSET_VERSION               4U
#define ZB_ZDP_NETWORK_OFFSET_BEACON                0U
#define ZB_ZDP_NETWORK_OFFSET_SUPERFRAME            4U
#define ZB_ZDP_NETWORK_OFFSET_PERMIT_JOIN           0U

/* Neighbor Descriptor Flags */
#define ZB_ZDP_NEIGHBOR_MASK_TYPE                   0x03U
#define ZB_ZDP_NEIGHBOR_MASK_RX_ON_IDLE             0x0cU
#define ZB_ZDP_NEIGHBOR_MASK_RELATIONSHIP           0x70U
#define ZB_ZDP_NEIGHBOR_MASK_PERMIT_JOIN            0x03U

#define ZB_ZDP_NEIGHBOR_OFFSET_TYPE                 0U
#define ZB_ZDP_NEIGHBOR_OFFSET_RX_ON_IDLE           2U
#define ZB_ZDP_NEIGHBOR_OFFSET_RELATIONSHIP         4U
#define ZB_ZDP_NEIGHBOR_OFFSET_PERMIT_JOIN          0U

/* Neighbor Descriptor Definitions */
#define ZB_ZDP_NEIGHBOR_TYPE_COORDINATOR            0x00U
#define ZB_ZDP_NEIGHBOR_TYPE_ROUTER                 0x01U
#define ZB_ZDP_NEIGHBOR_TYPE_END_DEVICE             0x02U
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_PARENT         0x00U /* ZB_NWK_NEIGHBOR_REL_PARENT */
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_CHILD          0x01U /* ZB_NWK_NEIGHBOR_REL_CHILD */
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_SIBLING        0x02U /* ZB_NWK_NEIGHBOR_REL_SIBLING */
#define ZB_ZDP_NEIGHBOR_RELATIONSHIP_NONE           0x03U /* ZB_NWK_NEIGHBOR_REL_NONE */
#define ZD_ZDP_NEIGHBOR_RELATIONSHIP_PREVIOUS       0x04U /* ZB_NWK_NEIGHBOR_REL_PREV_CHILD */
#define ZB_ZDP_NEIGHBOR_PERMIT_JOIN_DISABLED        0x00U
#define ZB_ZDP_NEIGHBOR_PERMIT_JOIN_ENABLED         0x01U
#define ZB_ZDP_NEIGHBOR_PERMIT_JOIN_UNKNOWN         0x02U

/* Routing Descriptor Flags */
#define ZB_ZDP_ROUTE_MASK_STATUS                    0x07U
#define ZB_ZDP_ROUTE_MASK_CONSTRAINED               0x08U
#define ZB_ZDP_ROUTE_MASK_MANY_TO_ONE               0x10U
#define ZB_ZDP_ROUTE_MASK_RECORD_REQUIRED           0x20U

#define ZB_ZDP_ROUTE_OFFSET_STATUS                  0U
#define ZB_ZDP_ROUTE_OFFSET_CONSTRAINED             3U
#define ZB_ZDP_ROUTE_OFFSET_MANY_TO_ONE             4U
#define ZB_ZDP_ROUTE_OFFSET_RECORD_REQUIRED         5U

/* Complex Descriptor */
#define ZB_ZDP_COMPLEX_DESC_MAX_DESC_LEN            256U

/* User Descriptor */
#define ZB_ZDP_USER_DESC_MAX_DESC_LEN               0x10U

/*
 * ZigBee Device Object cluster definitions.
 */
/* This bit in the cluster ID denotes a response or request. */
#define ZB_ZDO_CLUSTER_RESPONSE_BIT                 0x8000U

/* ZDP Request IDs (Responses are the same, but with ZB_ZDO_CLUSTER_RESPONSE_BIT set) */
#define ZB_ZDO_NWK_ADDR_REQ                         0x0000U
#define ZB_ZDO_IEEE_ADDR_REQ                        0x0001U
#define ZB_ZDO_NODE_DESC_REQ                        0x0002U
#define ZB_ZDO_POWER_DESC_REQ                       0x0003U
#define ZB_ZDO_SIMPLE_DESC_REQ                      0x0004U
#define ZB_ZDO_ACTIVE_EP_REQ                        0x0005U
#define ZB_ZDO_MATCH_DESC_REQ                       0x0006U
#define ZB_ZDO_COMPLEX_DESC_REQ                     0x0010U
#define ZB_ZDO_USER_DESC_REQ                        0x0011U
#define ZB_ZDO_DISCOVERY_CACHE_REQ                  0x0012U
#define ZB_ZDO_DEVICE_ANNCE                         0x0013U
#define ZB_ZDO_USER_DESC_SET                        0x0014U
#define ZB_ZDO_SYSTEM_SERVER_DISC_REQ               0x0015U
#define ZB_ZDO_DISCOVERY_STORE_REQ                  0x0016U
#define ZB_ZDO_NODE_DESC_STORE_REQ                  0x0017U
#define ZB_ZDO_POWER_DESC_STORE_REQ                 0x0018U
#define ZB_ZDO_ACTIVE_EP_STORE_REQ                  0x0019U
#define ZB_ZDO_SIMPLE_DESC_STORE_REQ                0x001aU
#define ZB_ZDO_REMOVE_NODE_CACHE_REQ                0x001bU
#define ZB_ZDO_FIND_NODE_CACHE_REQ                  0x001cU
#define ZB_ZDO_EXT_SIMPLE_DESC_REQ                  0x001dU
#define ZB_ZDO_EXT_ACTIVE_EP_REQ                    0x001eU
#define ZB_ZDO_PARENT_ANNCE                         0x001fU
#define ZB_ZDO_END_DEVICE_BIND_REQ                  0x0020U
#define ZB_ZDO_BIND_REQ                             0x0021U
#define ZB_ZDO_UNBIND_REQ                           0x0022U
/* ZB_ZDO_MGMT_NWK_DISC_REQ depreciated             0x0030U */
#define ZB_ZDO_MGMT_LQI_REQ                         0x0031U
#define ZB_ZDO_MGMT_RTG_REQ                         0x0032U
#define ZB_ZDO_MGMT_BIND_REQ                        0x0033U
#define ZB_ZDO_MGMT_LEAVE_REQ                       0x0034U
#define ZB_ZDO_MGMT_DIRECT_JOIN_REQ                 0x0035U
#define ZB_ZDO_MGMT_PERMIT_JOINING_REQ              0x0036U
#define ZB_ZDO_MGMT_CACHE_REQ                       0x0037U
#define ZB_ZDO_MGMT_NWK_UPDATE_REQ                  0x0038U
#define ZB_ZDO_MGMT_NWK_ENHANCED_UPDATE_REQ         0x0039U
#define ZB_ZDO_MGMT_IEEE_JOINING_LIST_REQ           0x003aU
#define ZB_ZDO_MGMT_UNSOLICITED_UPDATE_NOTIFY       0x003bU

/** ZigBee Node Descriptor */
struct ZbNodeDescriptorT {
    uint8_t logicalType; /**< Logical type */
    uint8_t complexDesc;
    uint8_t userDesc;
    uint8_t apsFlags; /**< APS flags */
    uint8_t freqBands; /**< Frequency band */
    uint8_t macCapability; /**< MAC capability flags */
    uint16_t mfrCode; /**< Manufacturer code */
    uint8_t maxBuffer; /**< Maximum buffer size */
    uint16_t maxIncomingTransfer; /**< Maximum incoming transfer size */
    uint16_t serverMask; /**< Server mask */
    uint16_t maxOutgoingTransfer; /**< Maximum outgoing transfer size */
    uint8_t descCapability; /**< Descriptor capability field */
};

/** ZigBee Power Descriptor */
struct ZbPowerDescriptorT {
    uint8_t currentMode; /**< Current power mode */
    uint8_t availableSources; /**< Available power sources */
    uint8_t currentSource; /**< Current power source */
    uint8_t currentLevel; /**< Current power source level */
};

/** ZigBee Simple (Application) Descriptor. */
struct ZbSimpleDescriptorT {
    uint8_t endpoint; /**< Endpoint */
    uint16_t profileId; /**< Application profile identifier */
    uint16_t deviceId; /**< Application device identifier */
    uint8_t version; /**< Application device version */
    uint8_t inputClusterCount; /**< Application input cluster count */
    uint16_t inputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< Application input cluster list */
    uint8_t outputClusterCount; /**< Application output cluster count */
    uint16_t outputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< Application output cluster list */
};

/** Network Descriptor */
struct ZbZdoNetworkDescT {
    uint64_t epid; /**< ExtendedPANId */
    uint8_t logicalChannel; /**< LogicalChannel */
    uint8_t version; /**< ZigBeeVersion */
    uint8_t stackProfile; /**< StackProfile */
    uint8_t superframeOrder; /**< SuperframeOrder */
    uint8_t beaconOrder; /**< BeaconOrder */
    uint8_t permitJoin; /**< PermitJoining */
};

/** Neighbor Descriptor */
struct ZbZdoNeighborDescT {
    uint64_t epid; /**< Extended PAN ID */
    uint64_t extAddr; /**< Extended address */
    uint16_t nwkAddr; /**< Network address */
    uint8_t deviceType; /**< Device type */
    uint8_t rxOnWhenIdle; /**< RxOnWhenIdle */
    uint8_t relationship; /**< Relationship */
    uint8_t permitJoin; /**< Permit joining */
    uint8_t depth; /**< Depth */
    uint8_t lqi; /**< LQI */
};

/** Routing Descriptor */
struct ZbZdoRoutingDescT {
    uint16_t destAddr; /**< Destination address */
    uint8_t status; /**< Status */ /* EXEGIN - convert to an enum type? */
    uint8_t constrained; /**< Memory Constrained */
    uint8_t manyToOne; /**< Many-to-one */
    uint8_t recordRequired; /**< Route record required */
    uint16_t nextHopAddr; /**< Next-hop address */
};

/** Binding Descriptor */
struct ZbZdoBindingDescT {
    uint64_t srcAddr; /**< SrcAddr */
    uint8_t srcEndpt; /**< SrcEndpoint */
    uint16_t clusterId; /**< ClusterId */
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst; /**< DstAddr */
};

/*---------------------------------------------------------------
 * ZDP Discovery Requests
 *---------------------------------------------------------------
 */
/** NWK_addr_req */
struct ZbZdoNwkAddrReqT {
    uint16_t dstNwkAddr; /**<  Destination network address, not in spec */ /* typically set to ZB_NWK_ADDR_BCAST_RXON (0xfffd) */
    uint64_t extAddr; /**< IEEEAddr */
    enum ZbZdoAddrReqTypeT reqType; /**< RequestType */
    uint8_t startIndex; /**< StartIndex */
};

/** NWK_addr_rsp */
struct ZbZdoNwkAddrRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint64_t extAddr; /**< IEEEAddrRemoteDev */
    uint16_t nwkAddr; /**< NWKAddrRemoteDev */
    uint8_t deviceCount; /**< NumAssocDev */
    uint8_t startIndex; /**< StartIndex */
    uint16_t deviceList[ZB_ZDO_ADDRRSP_DEVLIST_MAX]; /**< NWKAddrAssocDevList */
};

/** IEEE_addr_req */
struct ZbZdoIeeeAddrReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
    enum ZbZdoAddrReqTypeT reqType; /**< RequestType */
    uint8_t startIndex; /**< StartIndex */
};

/** IEEE_addr_rsp */
struct ZbZdoIeeeAddrRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint64_t extAddr; /**< IEEEAddrRemoteDev */
    uint16_t nwkAddr; /**< NWKAddrRemoteDev */
    uint8_t deviceCount; /**< NumAssocDev */
    uint8_t startIndex; /**< StartIndex */
    uint16_t deviceList[ZB_ZDO_ADDRRSP_DEVLIST_MAX]; /**< NWKAddrAssocDevList */
};

/** Node_Desc_req */
struct ZbZdoNodeDescReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
};

/** Node_Desc_rsp */
struct ZbZdoNodeDescRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    struct ZbNodeDescriptorT nodeDesc; /**< NodeDescriptor */
};

/** Power_Desc_req */
struct ZbZdoPowerDescReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
};

/** Power_Desc_rsp */
struct ZbZdoPowerDescRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    struct ZbPowerDescriptorT powerDesc; /**< PowerDescriptor */
};

/** Simple_Desc_req */
struct ZbZdoSimpleDescReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
    uint8_t endpt; /**< Endpoint */
};

/** Simple_Desc_rsp */
struct ZbZdoSimpleDescRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    struct ZbSimpleDescriptorT simpleDesc; /**< SimpleDescriptor */
};

/* Discovery_Cache_rsp */
typedef struct ZbZdoDiscCacheRspT {
    enum ZbStatusCodeT status;
    /* Added by Exegin */
    uint16_t nwkAddr;
} ZbZdoDiscCacheRspT;

/** Active_EP_req */
struct ZbZdoActiveEpReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
};

/** Active_EP_rsp */
struct ZbZdoActiveEpRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    uint8_t activeEpCount; /**< ActiveEPCount */
    uint8_t activeEpList[ZB_ZDO_ENDPOINT_LIST_MAXSZ]; /**< ActiveEPList */
};

/** Match_Desc_req */
struct ZbZdoMatchDescReqT {
    uint16_t dstNwkAddr; /**< Destination network address */
    uint16_t nwkAddrOfInterest; /**< NWKAddrOfInterest */
    uint16_t profileId; /**< ProfileID */
    uint8_t numInClusters; /**< NumInClusters */
    uint16_t inClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< InClusterList */
    uint8_t numOutClusters; /**< NumOutClusters */
    uint16_t outClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ]; /**< OutClusterList */
};

/** Match_Desc_rsp */
struct ZbZdoMatchDescRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t nwkAddr; /**< NWKAddrOfInterest */
    uint8_t matchLength; /**< MatchLength */
    uint8_t matchList[ZB_ZDO_ENDPOINT_LIST_MAXSZ]; /**< MatchList */
};

/** Device_annce (ZB_ZDO_DEVICE_ANNCE) */
struct ZbZdoDeviceAnnceT {
    uint16_t nwkAddr; /**< NWKAddr */
    uint64_t extAddr; /**< IEEEAddr */
    uint8_t capability; /**< Capability */
};

/** Bind_req */
struct ZbZdoBindReqT {
    uint16_t target; /**< Destination for the ZDO Bind Request message */
    /* Binding Source */
    uint64_t srcExtAddr; /**< SrcAddress */
    uint8_t srcEndpt; /**< SrcEndp */
    /* Binding Cluster ID */
    uint16_t clusterId; /**< ClusterID */
    /* Binding Destination (not the destination of the message)
     * dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst; /**< DstAddrMode, DstAddress, DstEndp */
};

/** Bind_rsp */
struct ZbZdoBindRspT {
    enum ZbStatusCodeT status; /**< Status */
};

/** Mgmt_Lqi_req */
struct ZbZdoLqiReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Lqi_rsp */
struct ZbZdoLqiRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t respAddr; /**< Response address */
    uint8_t neighborTblSz; /**< NeighborTableEntries */
    uint8_t startIndex; /**< StartIndex */
    uint8_t neighborListSz; /**< NeighborTableListCount */
    struct ZbZdoNeighborDescT neighborList[ZB_ZDO_NEIGHBOR_LIST_MAX_SZ]; /**< NeighborTableList */
};

/** Mgmt_Rtg_req */
struct ZbZdoRtgReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Rtg_rsp */
struct ZbZdoRtgRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t respAddr; /**< Response address */
    uint8_t routeTblSz; /**< RoutingTableEntries */
    uint8_t startIndex; /**< StartIndex */
    uint8_t routeListSz; /**< RoutingTableListCount */
    struct ZbZdoRoutingDescT routeList[ZB_ZDO_ROUTE_LIST_MAX_SZ]; /**< RoutingTableList */
};

/** Mgmt_Bind_req */
struct ZbZdoMgmtBindReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Bind_rsp */
struct ZbZdoMgmtBindRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint8_t bindTblSz; /**< BindingTableEntries */ /* Total number */
    uint8_t startIndex; /**< StartIndex */ /* Starting index being reported here */
    uint8_t bindListSz; /**< BindingTableListCount */ /* Number reported here */
    struct ZbZdoBindingDescT bindList[ZB_ZDO_BIND_LIST_MAX_SZ]; /**< BindingTableList */
};

/** ZDO Mgmt_Permit_Join_req */
struct ZbZdoPermitJoinReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t duration; /**< PermitDuration */
    uint8_t tcSignificance; /**< TC_Significance */
};

/** ZDO Mgmt_Permit_Join_rsp */
struct ZbZdoPermitJoinRspT {
    enum ZbStatusCodeT status; /**< Status */
};

/** Mgmt_Nwk_Update_req */
struct ZbZdoNwkUpdateReqT {
    uint16_t destAddr; /**< Destination address */
    uint32_t channelMask; /**< ScanChannels */
    uint8_t scanDuration; /**< ScanDuration */ /* or ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH, or ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
    uint8_t scanCount; /**< ScanCount */
    uint8_t updateId; /**< nwkUpdateId */ /* only for ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH or ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
    uint16_t managerAddr; /**< nwkManagerAddr */ /* only for ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
};

/** Mgmt_Nwk_Enhanced_Update_req */
struct ZbZdoNwkEnhUpdateReqT {
    uint16_t destAddr; /**< Destination address */
    struct ZbChannelListT channelList; /**< ScanChannelsListStructure */
    uint8_t scanDuration; /**< ScanDuration */
    uint8_t scanCount; /**< ScanCount */
    uint8_t updateId; /**< nwkUpdateId */
    uint16_t managerAddr; /**< nwkManagerAddr */
};

/** Mgmt_Nwk_Update_notify */
struct ZbZdoNwkUpdateNotifyT {
    enum ZbStatusCodeT status; /**< Status */
    uint32_t scannedChannels; /**< ScannedChannels */
    uint16_t txTotal; /**< TotalTransmissions */
    uint16_t txFails; /**< TransmissionFailures */
    uint8_t channelListSz; /**< ScannedChannelsListCount */
    uint8_t channelList[ZB_ZDO_CHANNEL_LIST_MAXSZ]; /**< Channel energy list */
};

#define ZB_ZDO_MGMT_NWK_LEAVE_FLAG_REJOIN                   0x80U
#define ZB_ZDO_MGMT_NWK_LEAVE_FLAG_REMOVE_CHILDREN          0x40U

/** Mgmt_Nwk_Leave_req */
struct ZbZdoLeaveReqT {
    uint16_t destAddr; /**< Destination address */
    uint64_t deviceAddr; /**< Device address */
    uint8_t flags; /**< Remove Children, Rejoin */
};

/** Mgmt_Nwk_Leave_rsp */
struct ZbZdoLeaveRspT {
    enum ZbStatusCodeT status; /**< Status */
};

#define ZDP_JOINING_LIST_MAX_LEN            9U

/** Mgmt_Nwk_Ieee_Joining_List_req */
struct ZbZdoNwkIeeeJoinListReqT {
    uint16_t destAddr; /**< Destination address */
    uint8_t startIndex; /**< StartIndex */
};

/** Mgmt_Nwk_Ieee_Joining_List_rsp */
struct ZbZdoNwkIeeeJoinListRspT {
    enum ZbStatusCodeT status; /**< Status */
    uint16_t respAddr; /**< Response address */ /* not part of ZDP payload */
    uint8_t updateId; /**< IeeeJoiningListUpdateID */
    enum WpanJoinPolicyT joiningPolicy; /**< JoiningPolicy */
    uint8_t ieeeJoiningListTotal; /**< IeeeJoiningListTotal */
    uint8_t startIndex; /**< StartIndex */
    uint8_t ieeeJoiningListCount; /**< IeeeJoiningCount */
    uint64_t ieeeJoiningList[ZDP_JOINING_LIST_MAX_LEN]; /**< IeeeJoiningList */
};

/*---------------------------------------------------------------
 * ZDO
 *---------------------------------------------------------------
 */

/**
 * Return and increment the next ZDO sequence number.
 * @param zb Zigbee stack instance
 * @return Returns ZDO sequence number.
 */
uint8_t ZbZdoGetNextSeqNum(struct ZigBeeT *zb);

/**
 * Retrieve the 16-bit short address for a device if given an extended address.
 * @param zb Zigbee stack instance
 * @param req NWK_Addr_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkAddrReq(struct ZigBeeT *zb, struct ZbZdoNwkAddrReqT *req,
    void (*callback)(struct ZbZdoNwkAddrRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the 64-bit extended address for a device if given a short network address.
 * @param zb Zigbee stack instance
 * @param req IEEE_Addr_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoIeeeAddrReq(struct ZigBeeT *zb, struct ZbZdoIeeeAddrReqT *req,
    void (*callback)(struct ZbZdoIeeeAddrRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the node descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Node_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNodeDescReq(struct ZigBeeT *zb, struct ZbZdoNodeDescReqT *req,
    void (*callback)(struct ZbZdoNodeDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the power descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Power_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoPowerDescReq(struct ZigBeeT *zb, struct ZbZdoPowerDescReqT *req,
    void (*callback)(struct ZbZdoPowerDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the simple descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Simple_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoSimpleDescReq(struct ZigBeeT *zb, struct ZbZdoSimpleDescReqT *req,
    void (*callback)(struct ZbZdoSimpleDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the active endpoint list for a device.
 * @param zb Zigbee stack instance
 * @param req Active_ep_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoActiveEpReq(struct ZigBeeT *zb, struct ZbZdoActiveEpReqT *req,
    void (*callback)(struct ZbZdoActiveEpRspT *rsp, void *cb_arg), void *arg);

/**
 * Retrieve the mach descriptor for a device.
 * @param zb Zigbee stack instance
 * @param req Match_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMatchDescReq(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req,
    void (*callback)(struct ZbZdoMatchDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a ZDO Match-Desc request and receive multiple responses.
 * The responses, if any, are received by the callback.
 * Each callback contains the result of a callback from a single device.
 * The callback will be called each time a response is received.
 * After a timeout period, the callback is called with a status of ZB_ZDP_STATUS_TIMEOUT to indicate the internal response filter
 * is removed and any additional responses for this request are not processed.
 * @param zb Zigbee stack instance
 * @param req Match_desc_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZB_ZDP_STATUS_SUCCESS for received responses, ZB_ZDP_STATUS_TABLE_FULL if there's a problem starting the request,
 * ZB_ZDP_STATUS_TIMEOUT when the stack decides to stop receiving responses, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMatchDescMulti(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req,
    void (*callback)(struct ZbZdoMatchDescRspT *rsp, void *cb_arg), void *arg);

/**
 * Send a Device_annce message to the network. Automatically called by the stack after joining.
 * @param zb Zigbee stack instance
 * @param deviceAnncePtr Pointer to Device_annce structure
 * @return Returns void
 */
void ZbZdoDeviceAnnce(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *deviceAnncePtr);

/**
 * Send a Device_annce message to the network using aliasing.
 * @param zb Zigbee stack instance
 * @param deviceAnncePtr Pointer to Device_annce structure
 * @return Returns void
 */
void ZbZdoDeviceAnnceAlias(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *deviceAnncePtr);

/**
 * Parse the device announce received.
 * @param structPtr Pointer to structure
 * @param buf Packet buffer
 * @param len Packet length
 * @return size of the device announce size, which will be 11, or other value on error
 */
int ZbZdoParseDeviceAnnce(struct ZbZdoDeviceAnnceT *structPtr, const uint8_t *buf, unsigned int len);

/**
 * Register a filter in the ZDO for the application to receive Device_Annce messages.
 * Freed by calling ZbZdoFilterRemove.
 * @param zb Zigbee stack instance
 * @param callback Function to call upon reception of a valid ZDO Device-Annce message.
 * Callback can return ZB_APS_FILTER_CONTINUE to process other filters,
 * or ZB_APS_FILTER_DISCARD if all processing of this message should stop.
 * @param arg Callback argument
 * @return Pointer to filter structure
 */
struct ZbZdoFilterT * ZbZdoDeviceAnnceFilterRegister(struct ZigBeeT *zb,
    int (*callback)(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *annce, uint8_t seqno, void *arg), void *arg);

/*---------------------------------------------------------------
 * ZDP Binding Requests
 *---------------------------------------------------------------
 */

/**
 * Perform a ZDO Bind operation.
 * @param zb Zigbee stack instance
 * @param req Bind_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoBindReq(struct ZigBeeT *zb, struct ZbZdoBindReqT *req,
    void (*callback)(struct ZbZdoBindRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a ZDP Unbind Bind operation.
 * @param zb Zigbee stack instance
 * @param req Unbind_req
 * @param Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoUnbindReq(struct ZigBeeT *zb, struct ZbZdoBindReqT *req,
    void (*callback)(struct ZbZdoBindRspT *rsp, void *cb_arg), void *arg);

/*---------------------------------------------------------------
 * ZDP Management Requests
 *---------------------------------------------------------------
 */

/**
 * Perform a Mgmt_Lqi_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Lqi_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoLqiReq(struct ZigBeeT *zb, struct ZbZdoLqiReqT *req,
    void (*callback)(struct ZbZdoLqiRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Rtg_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Rtg_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoRtgReq(struct ZigBeeT *zb, struct ZbZdoRtgReqT *req,
    void (*callback)(struct ZbZdoRtgRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Bind_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_bind_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMgmtBindReq(struct ZigBeeT *zb, struct ZbZdoMgmtBindReqT *req,
    void (*callback)(struct ZbZdoMgmtBindRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_permit_joining_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_permit_joining_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoPermitJoinReq(struct ZigBeeT *zb, struct ZbZdoPermitJoinReqT *req,
    void (*callback)(struct ZbZdoPermitJoinRspT *rsp, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Nwk_update_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Nwk_update_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkUpdateReq(struct ZigBeeT *zb, struct ZbZdoNwkUpdateReqT *req,
    void (*callback)(struct ZbZdoNwkUpdateNotifyT *reqPtr, void *cb_arg), void *arg);

/**
 * Perform a Mgmt_Nwk_Enhanced_Update_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Nwk_Enhanced_Update_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkEnhUpdateReq(struct ZigBeeT *zb, struct ZbZdoNwkEnhUpdateReqT *req,
    void (*callback)(struct ZbZdoNwkUpdateNotifyT *reqPtr, void *cb_arg), void *arg);

/**
 * Send an unsolicited Mgmt_Nwk_Update_notify command to the network manager. Does not wait for any response.
 * @param zb Zigbee stack instance
 * @param reqPtr Mgmt_Nwk_Update_notify
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkUpdateNotify(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *reqPtr);

/**
 * Register a filter in the ZDO for the application to receive Mgmt_Nwk_Update_notify messages.
 * Freed by calling ZbZdoFilterRemove.
 * @param zb Zigbee stack instance
 * @param callback Function to call upon reception of a valid ZDO NWK-Update-Notify message.
 * Callback can return ZB_APS_FILTER_CONTINUE to process other filters,
 * or ZB_APS_FILTER_DISCARD if all processing of this message should stop.
 * @param arg Callback argument
 * @return Pointer to filter structure
 */
struct ZbZdoFilterT * ZbZdoNwkUpdateNotifyFilterRegister(struct ZigBeeT *zb,
    int (*callback)(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *msg, uint8_t seqno, void *arg), void *arg);

/**
 * Perform a Mgmt_leave_req command.
 * @param zb Zigbee stack instance
 * @param req Mgmt_Leave_req
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoLeaveReq(struct ZigBeeT *zb, struct ZbZdoLeaveReqT *req,
    void (*callback)(struct ZbZdoLeaveRspT *rsp, void *cb_arg), void *arg);

/*---------------------------------------------------------------
 * Receive incoming ZDO messages
 *---------------------------------------------------------------
 */
/* Configures the callback to receive incoming ZDO messages in your application. */
typedef int (*ZbZdoPreHandlerFuncT)(struct ZigBeeT *zb, struct ZbApsdeDataIndT *dataIndPtr, uint8_t seqnum);
void ZbZdoSetPreHandler(struct ZigBeeT *zb, ZbZdoPreHandlerFuncT func);

/*---------------------------------------------------------------
 * Complex Descriptor
 *---------------------------------------------------------------
 */
/* Internal complex descriptor format */
typedef struct ZbZdoComplexDescT {
    char manufacturerName[16];
    char modelName[16];
    char serialNumber[16];
} ZbZdoComplexDescT;

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoSetComplexDesc(struct ZigBeeT *zb, struct ZbZdoComplexDescT *structPtr);

/*---------------------------------------------------------------
 * IEEE Joining List
 *---------------------------------------------------------------
 */

/**
 * Send an IEEE-Joining-List request.
 * @param zb Zigbee stack instance
 * @param IEEE-Joining-List.request Structure
 * @param callback Function to call on completion
 * @param arg Callback argument
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkIeeeJoinListReq(struct ZigBeeT *zb, struct ZbZdoNwkIeeeJoinListReqT *req,
    void (*callback)(struct ZbZdoNwkIeeeJoinListRspT *rsp, void *cb_arg), void *arg);

/**
 * Send an IEEE-Joining-List.response.
 * Used primarily by GB868 applications.
 * @param zb Zigbee stack instance
 * @param dstNwkAddr Destination network address
 * @param startIndex Start index
 * @param seqnum Sequence number
 * @param fromRequest Use start index from request
 * @return Number of entries sent in response
 */
unsigned int ZbZdoNwkIeeeJoinListRsp(struct ZigBeeT *zb, uint16_t dstNwkAddr,
    uint8_t startIndex, uint8_t seqnum, bool fromRequest);
/* Helper to broadcast the entire list to the network */

/**
 * Send an IEEE-Joining-List.response broadcast message for all entries in the IEEE Join List.
 * Used primarily by GB868 applications.
 * @param zb Zigbee stack instance
 * @return Returns number of entries sent
 */
unsigned int ZbZdoNwkIeeeJoinListBcastAll(struct ZigBeeT *zb);

/**
 * Remove a ZDO filter.
 * @param zb Zigbee stack instance
 * @param filter Filter instance to remove and free
 * @return Returns void
 */
void ZbZdoFilterRemove(struct ZigBeeT *zb, struct ZbZdoFilterT *filter);

#endif
