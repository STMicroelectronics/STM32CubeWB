/**
 * @file zigbee.zdo.h
 * @brief ZDO header file
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZIGBEE_ZDO_H
# define ZIGBEE_ZDO_H

/*---------------------------------------------------------------
 * Misc. Definitions and Structures
 *---------------------------------------------------------------
 */
/* Profile, and Device Identifiers for the ZDO. */
#define ZB_ZDO_PROFILE_ID                           0x0000U
#define ZB_ZDO_DEVICE_ID                            0x0000U
#define ZB_ZDO_DEVICE_VER                           0x00U

#define ZB_ZDO_ADDRRSP_DEVLIST_MAX                  32U

/* NWK_addr_req / IEEE_addr_req definitions */
enum ZbZdoAddrReqTypeT {
    ZB_ZDO_ADDR_REQ_TYPE_SINGLE = 0x00,
    ZB_ZDO_ADDR_REQ_TYPE_EXTENDED = 0x01
};

/* NWK_Update_Req special duration values. */
#define ZB_ZDP_NWK_UPDATE_MAX_DURATION              0x05U
#define ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH            0xfeU
#define ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS        0xffU

/* Maximum Response Sizes */
/* EXEGIN (sal) was 64, but end up allocating too much on the stack */
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

/* ZigBee Node Descriptor - Section 2.3.2.3 */
typedef struct ZbNodeDescriptorT {
    uint8_t logicalType;
    uint8_t complexDesc;
    uint8_t userDesc;
    uint8_t apsFlags;
    uint8_t freqBands;
    uint8_t macCapability;
    uint16_t mfrCode;
    uint8_t maxBuffer;
    uint16_t maxIncomingTransfer;
    uint16_t serverMask;
    uint16_t maxOutgoingTransfer;
    uint8_t descCapability;
} ZbNodeDescriptorT;

/* ZigBee Power Descriptor - Section 2.3.2.4. */
typedef struct ZbPowerDescriptorT {
    uint8_t currentMode;
    uint8_t availableSources;
    uint8_t currentSource;
    uint8_t currentLevel;
} ZbPowerDescriptorT;

/* ZigBee Simple (Application) Descriptor. */
typedef struct ZbSimpleDescriptorT {
    uint8_t endpoint;
    uint16_t profileId;
    uint16_t deviceId;
    uint8_t version;
    uint8_t inputClusterCount;
    uint16_t inputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ];
    uint8_t outputClusterCount;
    uint16_t outputClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ];
} ZbSimpleDescriptorT;

/* Network Descriptor */
typedef struct ZbZdoNetworkDescT {
    uint64_t epid;
    uint8_t logicalChannel;
    uint8_t version;
    uint8_t stackProfile;
    uint8_t superframeOrder;
    uint8_t beaconOrder;
    uint8_t permitJoin;
} ZbZdoNetworkDescT;

/* Neighbor Descriptor */
typedef struct ZbZdoNeighborDescT {
    uint64_t epid;
    uint64_t extAddr;
    uint16_t nwkAddr;
    uint8_t deviceType;
    uint8_t rxOnWhenIdle;
    uint8_t relationship;
    uint8_t permitJoin;
    uint8_t depth;
    uint8_t lqi;
} ZbZdoNeighborDescT;

/* Routing Descriptor */
typedef struct ZbZdoRoutingDescT {
    uint16_t destAddr;
    uint8_t status; /* EXEGIN - convert to an enum type? */
    uint8_t constrained;
    uint8_t manyToOne;
    uint8_t recordRequired;
    uint16_t nextHopAddr;
} ZbZdoRoutingDescT;

/* Binding Descriptor */
typedef struct ZbZdoBindingDescT {
    uint64_t srcAddr;
    uint8_t srcEndpt;
    uint16_t clusterId;
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst;
} ZbZdoBindingDescT;

/*---------------------------------------------------------------
 * ZDP Discovery Requests
 *---------------------------------------------------------------
 */
/* NWK_addr_req */
typedef struct ZbZdoNwkAddrReqT {
    uint16_t dstNwkAddr; /* typically set to ZB_NWK_ADDR_BCAST_RXON (0xfffd) */
    uint64_t extAddr;
    enum ZbZdoAddrReqTypeT reqType;
    uint8_t startIndex;
} ZbZdoNwkAddrReqT;

/* NWK_addr_rsp */
typedef struct ZbZdoNwkAddrRspT {
    enum ZbStatusCodeT status;
    uint64_t extAddr;
    uint16_t nwkAddr;
    uint8_t deviceCount;
    uint8_t startIndex;
    uint16_t deviceList[ZB_ZDO_ADDRRSP_DEVLIST_MAX];
} ZbZdoNwkAddrRspT;

/* IEEE_addr_req */
typedef struct ZbZdoIeeeAddrReqT {
    uint16_t dstNwkAddr;
    uint16_t nwkAddrOfInterest;
    enum ZbZdoAddrReqTypeT reqType;
    uint8_t startIndex;
} ZbZdoIeeeAddrReqT;

/* IEEE_addr_rsp */
typedef struct ZbZdoIeeeAddrRspT {
    enum ZbStatusCodeT status;
    uint64_t extAddr;
    uint16_t nwkAddr;
    uint8_t deviceCount;
    uint8_t startIndex;
    uint16_t deviceList[ZB_ZDO_ADDRRSP_DEVLIST_MAX];
} ZbZdoIeeeAddrRspT;

/* Node_Desc_req */
typedef struct ZbZdoNodeDescReqT {
    uint16_t dstNwkAddr;
    uint16_t nwkAddrOfInterest;
} ZbZdoNodeDescReqT;

/* Node_Desc_rsp */
typedef struct ZbZdoNodeDescRspT {
    enum ZbStatusCodeT status;
    uint16_t nwkAddr;
    struct ZbNodeDescriptorT nodeDesc;
} ZbZdoNodeDescRspT;

/* Power_Desc_req */
typedef struct ZbZdoPowerDescReqT {
    uint16_t dstNwkAddr;
    uint16_t nwkAddrOfInterest;
} ZbZdoPowerDescReqT;

/* Power_Desc_rsp */
typedef struct ZbZdoPowerDescRspT {
    enum ZbStatusCodeT status;
    uint16_t nwkAddr;
    struct ZbPowerDescriptorT powerDesc;
} ZbZdoPowerDescRspT;

/* Simple_Desc_req */
typedef struct ZbZdoSimpleDescReqT {
    uint16_t dstNwkAddr;
    uint16_t nwkAddrOfInterest;
    uint8_t endpt;
} ZbZdoSimpleDescReqT;

/* Simple_Desc_rsp */
typedef struct ZbZdoSimpleDescRspT {
    enum ZbStatusCodeT status;
    uint16_t nwkAddr;
    struct ZbSimpleDescriptorT simpleDesc;
} ZbZdoSimpleDescRspT;

/* Discovery_Cache_rsp */
typedef struct ZbZdoDiscCacheRspT {
    enum ZbStatusCodeT status;
    /* Added by Exegin */
    uint16_t nwkAddr;
} ZbZdoDiscCacheRspT;

/* Active_EP_req */
typedef struct ZbZdoActiveEpReqT {
    uint16_t dstNwkAddr;
    uint16_t nwkAddrOfInterest;
} ZbZdoActiveEpReqT;

/* Active_EP_rsp */
typedef struct ZbZdoActiveEpRspT {
    enum ZbStatusCodeT status;
    uint16_t nwkAddr;
    uint8_t activeEpCount;
    uint8_t activeEpList[ZB_ZDO_ENDPOINT_LIST_MAXSZ];
} ZbZdoActiveEpRspT;

/* Match_Desc_req */
typedef struct ZbZdoMatchDescReqT {
    uint16_t dstNwkAddr;
    uint16_t nwkAddrOfInterest;
    uint16_t profileId;
    uint8_t numInClusters;
    uint16_t inClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ];
    uint8_t numOutClusters;
    uint16_t outClusterList[ZB_ZDO_CLUSTER_LIST_MAX_SZ];
} ZbZdoMatchDescReqT;

/* Match_Desc_rsp */
typedef struct ZbZdoMatchDescRspT {
    enum ZbStatusCodeT status;
    uint16_t nwkAddr;
    uint8_t matchLength;
    uint8_t matchList[ZB_ZDO_ENDPOINT_LIST_MAXSZ];
} ZbZdoMatchDescRspT;

/* Device_annce (ZB_ZDO_DEVICE_ANNCE) */
typedef struct ZbZdoDeviceAnnceT {
    uint16_t nwkAddr;
    uint64_t extAddr;
    uint8_t capability;
} ZbZdoDeviceAnnceT;

/* Bind_req */
typedef struct ZbZdoBindReqT {
    /* Destination for the ZDO Bind Request message */
    uint16_t target;
    /* Binding Source */
    uint64_t srcExtAddr;
    uint8_t srcEndpt;
    /* Binding Cluster ID */
    uint16_t clusterId;
    /* Binding Destination (not the destination of the message)
     * dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst;
} ZbZdoBindReqT;

/* Bind_rsp */
typedef struct ZbZdoBindRspT {
    enum ZbStatusCodeT status;
} ZbZdoBindRspT;

/* Mgmt_Lqi_req */
typedef struct ZbZdoLqiReqT {
    uint16_t destAddr;
    uint8_t startIndex;
} ZbZdoLqiReqT;

/* Mgmt_Lqi_rsp */
typedef struct ZbZdoLqiRspT {
    enum ZbStatusCodeT status;
    uint16_t respAddr;
    uint8_t neighborTblSz;
    uint8_t startIndex;
    uint8_t neighborListSz;
    struct ZbZdoNeighborDescT neighborList[ZB_ZDO_NEIGHBOR_LIST_MAX_SZ];
} ZbZdoLqiRspT;

/* Mgmt_Rtg_req */
typedef struct ZbZdoRtgReqT {
    uint16_t destAddr;
    uint8_t startIndex;
} ZbZdoRtgReqT;

/* Mgmt_Rtg_rsp */
typedef struct ZbZdoRtgRspT {
    enum ZbStatusCodeT status;
    uint16_t respAddr;
    uint8_t routeTblSz;
    uint8_t startIndex;
    uint8_t routeListSz;
    struct ZbZdoRoutingDescT routeList[ZB_ZDO_ROUTE_LIST_MAX_SZ];
} ZbZdoRtgRspT;

/* Mgmt_Bind_req */
typedef struct ZbZdoMgmtBindReqT {
    uint16_t destAddr;
    uint8_t startIndex;
} ZbZdoMgmtBindReqT;

/* Mgmt_Bind_rsp */
typedef struct ZbZdoMgmtBindRspT {
    enum ZbStatusCodeT status;
    uint8_t bindTblSz; /* Total number */
    uint8_t startIndex; /* Starting index being reported here */
    uint8_t bindListSz; /* Number reported here */
    struct ZbZdoBindingDescT bindList[ZB_ZDO_BIND_LIST_MAX_SZ];
} ZbZdoMgmtBindRspT;

/* Mgmt_Permit_Join_req */
typedef struct ZbZdoPermitJoinReqT {
    uint16_t destAddr;
    uint8_t duration;
    uint8_t tcSignificance;
} ZbZdoPermitJoinReqT;

/* Mgmt_Permit_Join_rsp */
typedef struct ZbZdoPermitJoinRspT {
    enum ZbStatusCodeT status;
} ZbZdoPermitJoinRspT;

/* Mgmt_Nwk_Update_req */
typedef struct ZbZdoNwkUpdateReqT {
    uint16_t destAddr;
    uint32_t channelMask;
    uint8_t scanDuration; /* or ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH, or ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
    uint8_t scanCount;
    uint8_t updateId; /* only for ZB_ZDP_NWK_UPDATE_CHANNEL_SWITCH or ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
    uint16_t managerAddr; /* only for ZB_ZDP_NWK_UPDATE_MANAGER_PARAMETERS */
} ZbZdoNwkUpdateReqT;

/* Mgmt_Nwk_Enhanced_Update_req */
struct ZbZdoNwkEnhUpdateReqT {
    uint16_t destAddr;
    struct ZbChannelListT channelList;
    uint8_t scanDuration;
    uint8_t scanCount;
    uint8_t updateId;
    uint16_t managerAddr;
};

/* Mgmt_Nwk_Update_notify */
typedef struct ZbZdoNwkUpdateNotifyT {
    enum ZbStatusCodeT status;
    uint32_t scannedChannels;
    uint16_t txTotal;
    uint16_t txFails;
    uint8_t channelListSz;
    uint8_t channelList[ZB_ZDO_CHANNEL_LIST_MAXSZ]; /* Channel energy list */
} ZbZdoNwkUpdateNotifyT;

#define ZB_ZDO_MGMT_NWK_LEAVE_FLAG_REJOIN                   0x80U
#define ZB_ZDO_MGMT_NWK_LEAVE_FLAG_REMOVE_CHILDREN          0x40U

/* Mgmt_Nwk_Leave_req */
typedef struct ZbZdoLeaveReqT {
    uint16_t destAddr;
    uint64_t deviceAddr;
    uint8_t flags;
} ZbZdoLeaveReqT;

/* Mgmt_Nwk_Leave_rsp */
typedef struct ZbZdoLeaveRspT {
    enum ZbStatusCodeT status;
} ZbZdoLeaveRspT;

#define ZDP_JOINING_LIST_MAX_LEN            9U

/* Mgmt_Nwk_Ieee_Joining_List_req */
struct ZbZdoNwkIeeeJoinListReqT {
    uint16_t destAddr;
    uint8_t startIndex;
};

/* Mgmt_Nwk_Ieee_Joining_List_rsp */
struct ZbZdoNwkIeeeJoinListRspT {
    enum ZbStatusCodeT status;
    uint16_t respAddr; /* not part of ZDP payload */
    uint8_t updateId;
    enum WpanJoinPolicyT joiningPolicy;
    uint8_t ieeeJoiningListTotal;
    uint8_t startIndex;
    uint8_t ieeeJoiningListCount;
    uint64_t ieeeJoiningList[ZDP_JOINING_LIST_MAX_LEN];
};

/*---------------------------------------------------------------
 * ZDO
 *---------------------------------------------------------------
 */
/* Get the next ZDO sequence number */
uint8_t ZbZdoGetNextSeqNum(struct ZigBeeT *zb);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkAddrReq(struct ZigBeeT *zb, struct ZbZdoNwkAddrReqT *req,
    void (*callback)(struct ZbZdoNwkAddrRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoIeeeAddrReq(struct ZigBeeT *zb, struct ZbZdoIeeeAddrReqT *req,
    void (*callback)(struct ZbZdoIeeeAddrRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNodeDescReq(struct ZigBeeT *zb, struct ZbZdoNodeDescReqT *req,
    void (*callback)(struct ZbZdoNodeDescRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoPowerDescReq(struct ZigBeeT *zb, struct ZbZdoPowerDescReqT *req,
    void (*callback)(struct ZbZdoPowerDescRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoSimpleDescReq(struct ZigBeeT *zb, struct ZbZdoSimpleDescReqT *req,
    void (*callback)(struct ZbZdoSimpleDescRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoActiveEpReq(struct ZigBeeT *zb, struct ZbZdoActiveEpReqT *req,
    void (*callback)(struct ZbZdoActiveEpRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMatchDescReq(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req,
    void (*callback)(struct ZbZdoMatchDescRspT *rsp, void *cb_arg), void *arg);

/* ZbZdoMatchDescMulti:
 * Returns ZB_ZDP_STATUS_SUCCESS for received responses.
 * Returns ZB_ZDP_STATUS_TABLE_FULL if there's a problem starting the request.
 * Returns ZB_ZDP_STATUS_TIMEOUT when the stack decides to stop receiving responses. */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMatchDescMulti(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req,
    void (*callback)(struct ZbZdoMatchDescRspT *rsp, void *cb_arg), void *arg);

void ZbZdoDeviceAnnce(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *deviceAnncePtr);
void ZbZdoDeviceAnnceAlias(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *deviceAnncePtr);
int ZbZdoParseDeviceAnnce(struct ZbZdoDeviceAnnceT *structPtr, const uint8_t *buf, unsigned int len);

/* API to register a filter in the ZDO for the application to
 * receive Device_Annce messages. */
struct ZbZdoDeviceAnnceFilterT;
struct ZbZdoDeviceAnnceFilterT * ZbZdoDeviceAnnceFilterRegister(struct ZigBeeT *zb,
    void (*callback)(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *annce, uint8_t seqno, void *arg), void *arg);
void ZbZdoDeviceAnnceFilterRemove(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceFilterT *handler);

/*---------------------------------------------------------------
 * ZDP Binding Requests
 *---------------------------------------------------------------
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoBindReq(struct ZigBeeT *zb, struct ZbZdoBindReqT *req,
    void (*callback)(struct ZbZdoBindRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoUnbindReq(struct ZigBeeT *zb, struct ZbZdoBindReqT *req,
    void (*callback)(struct ZbZdoBindRspT *rsp, void *cb_arg), void *arg);

/*---------------------------------------------------------------
 * ZDP Management Requests
 *---------------------------------------------------------------
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoLqiReq(struct ZigBeeT *zb, struct ZbZdoLqiReqT *req,
    void (*callback)(struct ZbZdoLqiRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoRtgReq(struct ZigBeeT *zb, struct ZbZdoRtgReqT *req,
    void (*callback)(struct ZbZdoRtgRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoMgmtBindReq(struct ZigBeeT *zb, struct ZbZdoMgmtBindReqT *req,
    void (*callback)(struct ZbZdoMgmtBindRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoPermitJoinReq(struct ZigBeeT *zb, struct ZbZdoPermitJoinReqT *req,
    void (*callback)(struct ZbZdoPermitJoinRspT *rsp, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkUpdateReq(struct ZigBeeT *zb, struct ZbZdoNwkUpdateReqT *req,
    void (*callback)(struct ZbZdoNwkUpdateNotifyT *reqPtr, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkEnhUpdateReq(struct ZigBeeT *zb, struct ZbZdoNwkEnhUpdateReqT *req,
    void (*callback)(struct ZbZdoNwkUpdateNotifyT *reqPtr, void *cb_arg), void *arg);

enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkUpdateNotify(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *reqPtr);

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
enum ZbStatusCodeT ZB_WARN_UNUSED ZbZdoNwkIeeeJoinListReq(struct ZigBeeT *zb, struct ZbZdoNwkIeeeJoinListReqT *req,
    void (*callback)(struct ZbZdoNwkIeeeJoinListRspT *rsp, void *cb_arg), void *arg);
unsigned int ZbZdoNwkIeeeJoinListRsp(struct ZigBeeT *zb, uint16_t dstNwkAddr,
    uint8_t startIndex, uint8_t seqnum, bool fromRequest);
/* Helper to broadcast the entire list to the network */
unsigned int ZbZdoNwkIeeeJoinListBcastAll(struct ZigBeeT *zb);

#endif /* ZIGBEE_ZDO_H */
