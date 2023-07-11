/**
 * @file zigbee.nwk.h
 * @brief NWK header file
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZIGBEE_NWK_H
# define ZIGBEE_NWK_H

/*lint -e621 "Identifier clash [MISRA Rule 5.1 (REQUIRED), MISRA Rule 5.2 (REQUIRED), MISRA Rule 5.4 (REQUIRED), MISRA Rule 5.5 (REQUIRED)]" */

/*---------------------------------------------------------------
 * Misc. Definitions and Structures
 *---------------------------------------------------------------
 */
#define ZbNwkAddrIsBcast(_addr_) (((_addr_) >= (uint16_t)ZB_NWK_ADDR_BCAST_MIN) && ((_addr_) != (uint16_t)ZB_NWK_ADDR_USE_EXT))

/* Special NWK addresses. */
#define ZB_NWK_ADDR_COORDINATOR                     (uint16_t)0x0000U
#define ZB_NWK_ADDR_BCAST_MIN                       (uint16_t)0xfff8U
#define ZB_NWK_ADDR_BCAST_LOW_POWER_ROUTERS         (uint16_t)0xfffbU
#define ZB_NWK_ADDR_BCAST_ROUTERS                   (uint16_t)0xfffcU
#define ZB_NWK_ADDR_BCAST_RXON                      (uint16_t)0xfffdU
#define ZB_NWK_ADDR_USE_EXT                         (uint16_t)0xfffeU /* IEEE802154_ADDR_SHORT_NONE */
#define ZB_NWK_ADDR_BCAST_ALL                       (uint16_t)0xffffU /* IEEE802154_ADDR_SHORT_BCAST */
#define ZB_NWK_ADDR_UNDEFINED                       (uint16_t)0xffffU /* IEEE802154_ADDR_SHORT_BCAST */

/* Default delay to wait before generating a response or performing some other
 * task to allow the stack to send an APS ACK first, if required, or prevent
 * other contention on the RF due to simultaneous bi-directional Zigbee traffic. */
#define ZB_NWK_RSP_DELAY_DEFAULT                    200U /* ms */

/* ZigBee Stack Profiles (StackProfile or nwkStackProfile) */
enum {
    ZB_NWK_STACK_PROFILE_NWKSPFC = 0x00,
    ZB_NWK_STACK_PROFILE_HOME = 0x01,
    ZB_NWK_STACK_PROFILE_PRO = 0x02,
    ZB_NWK_STACK_PROFILE_PLANTCTRL = 0x03
};

/* Address allocation methods (nwkAddrAlloc) */
enum  nwkAddrAllocMethod {
    ZB_NWK_ADDR_ALLOC_DISTRIB = 0x00,
    ZB_NWK_ADDR_ALLOC_RESERVED = 0x01,
    ZB_NWK_ADDR_ALLOC_STOCHASTIC = 0x02
};

/* Status codes for network status indications. */
enum ZbNwkNetworkStatusCodeT {
    ZB_NWK_STATUS_CODE_NO_ROUTE_AVAILABLE = 0x00, /**< No route available */
    ZB_NWK_STATUS_CODE_TREE_LINK_FAILURE = 0x01, /**< Tree link failure */
    ZB_NWK_STATUS_CODE_NON_TREE_LINK_FAILURE = 0x02, /**< Non-tree link failure */
    ZB_NWK_STATUS_CODE_LOW_BATTERY = 0x03, /**< Low battery level */
    ZB_NWK_STATUS_CODE_NO_ROUTING_CAPACITY = 0x04, /**< No routing capacity */
    ZB_NWK_STATUS_CODE_NO_INDIRECT_CAPACITY = 0x05, /**< No indirect capacity */
    ZB_NWK_STATUS_CODE_INDIRECT_EXPIRY = 0x06, /**< Indirect transaction expiry */
    ZB_NWK_STATUS_CODE_TARGET_UNAVAILABLE = 0x07, /**< Target device unavailable */
    ZB_NWK_STATUS_CODE_TARGET_UNALLOCATED = 0x08, /**< Target address unallocated */
    ZB_NWK_STATUS_CODE_PARENT_LINK_FAILURE = 0x09, /**< Parent link failure */
    ZB_NWK_STATUS_CODE_VALIDATE_ROUTE = 0x0a, /**< Validate route */
    ZB_NWK_STATUS_CODE_SOURCE_ROUTE_FAILURE = 0x0b, /**< Source route failure */
    ZB_NWK_STATUS_CODE_MANY_TO_ONE_FAILURE = 0x0c, /**< Many-to-one route failure */
    ZB_NWK_STATUS_CODE_ADDRESS_CONFLICT = 0x0d, /**< Address conflict */
    ZB_NWK_STATUS_CODE_VERIFY_ADDRESS = 0x0e, /**< Verify address */
    ZB_NWK_STATUS_CODE_PANID_UPDATE = 0x0f, /**< PAN identifier update */
    ZB_NWK_STATUS_CODE_ADDRESS_UPDATE = 0x10, /**< Network address update */
    ZB_NWK_STATUS_CODE_BAD_FRAME_COUNTER = 0x11, /**< Bad frame counter */
    ZB_NWK_STATUS_CODE_BAD_KEY_SEQNUM = 0x12, /**< Bad key sequence number */
    ZB_NWK_STATUS_CODE_UNKNOWN_COMMAND = 0x13, /**< Unknown Command */
    /* 0x14 - 0xff  =  Reserved */
    /* Custom Exegin status codes */
    ZB_NWK_STATUS_CODE_INCOMING_FRAME_COUNTER = 0xff /**< (Exegin custom) An incoming frame counter is greater than 0x80000000 */
};

/* NWK DstAddrMode values */
enum ZbNwkAddrModeT {
    ZB_NWK_ADDRMODE_NONE = 0,
    ZB_NWK_ADDRMODE_MCAST, /* 1 - deprecated by R23 */
    ZB_NWK_ADDRMODE_SHORT, /* 2 */
    ZB_NWK_ADDRMODE_EXT /* 3 - for InterPAN */
};

/* NWK IB Attributes */
enum ZbNwkNibAttrIdT {
    ZB_NWK_NIB_ID_PanId = 0x80, /* 0x80 ZigBee 2007+ */
    ZB_NWK_NIB_ID_SequenceNumber = 0x81,
    ZB_NWK_NIB_ID_PassiveAckTimeout = 0x82,
    ZB_NWK_NIB_ID_MaxBroadcastRetries = 0x83,
    ZB_NWK_NIB_ID_MaxChildren = 0x84,
    ZB_NWK_NIB_ID_MaxDepth = 0x85,
    ZB_NWK_NIB_ID_MaxRouters = 0x86,
    /* Warning, the stack implements the NNT as a sorted list by NWK Address.
     * If an entry is set through the NIB, it is inserted into the list sorted.
     * It is generally a bad idea for an application to modify the NNT directly.
     * Use ZbNwkNeighborClearAll to clear all entries (or all except parent, e.g.
     * before end-device rejoin).
     * EXEGIN - This NIB should be made read-only. */
    ZB_NWK_NIB_ID_NeighborTable = 0x87,
    /* Time duration in seconds (Note, the Spec defines this as OctetDurations) */
    ZB_NWK_NIB_ID_NetworkBroadcastDeliveryTime = 0x88,
    ZB_NWK_NIB_ID_ReportConstantCost = 0x89,
    ZB_NWK_NIB_ID_RouteDiscoveryRetriesPermitted = 0x8a,
    ZB_NWK_NIB_ID_RouteTable = 0x8b,
    ZB_NWK_NIB_ID_TimeStamp = 0x8c,
    ZB_NWK_NIB_ID_TxTotal = 0x8d, /* EXEGIN - make TxTotal a per interface thing */
    ZB_NWK_NIB_ID_SymLink = 0x8e,
    ZB_NWK_NIB_ID_CapabilityInformation = 0x8f,
    ZB_NWK_NIB_ID_AddrAlloc = 0x90,
    /* was ZB_NWK_NIB_ID_UseTreeRouting = 0x91 */
    ZB_NWK_NIB_ID_ManagerAddr = 0x92,
    ZB_NWK_NIB_ID_MaxSourceRoute = 0x93,
    ZB_NWK_NIB_ID_UpdateId = 0x94,
    ZB_NWK_NIB_ID_TransactionPersistenceTime = 0x95,
    /* ZigBee 2006+ Attributes */
    ZB_NWK_NIB_ID_NetworkAddress = 0x96,
    ZB_NWK_NIB_ID_StackProfile = 0x97,
    ZB_NWK_NIB_ID_BroadcastTransactionTable = 0x98,
    ZB_NWK_NIB_ID_GroupIdTable = 0x99,
    ZB_NWK_NIB_ID_ExtendedPanId = 0x9a,
    /* ZigBee 2007+ Attributes */
    ZB_NWK_NIB_ID_RouteRecordTable = 0x9c,
    ZB_NWK_NIB_ID_IsConcentrator = 0x9d,
    ZB_NWK_NIB_ID_ConcentratorRadius = 0x9e,
    ZB_NWK_NIB_ID_ConcentratorDiscoveryTime = 0x9f, /* units: seconds */
    /* ZigBee Security Attributes */
    ZB_NWK_NIB_ID_SecurityLevel = 0xa0,
    ZB_NWK_NIB_ID_SecurityMaterialSet = 0xa1,
    ZB_NWK_NIB_ID_ActiveKeySeqNumber = 0xa2,
    /* nwkAllFresh has been deprecated (0xa3) */
    /* ...discontinuity... */
    ZB_NWK_NIB_ID_SecureAllFrames = 0xa5,
    /* ZigBee 2007+ Attributes (cont'd). */
    ZB_NWK_NIB_ID_LinkStatusPeriod = 0xa6,
    ZB_NWK_NIB_ID_RouterAgeLimit = 0xa7,
    ZB_NWK_NIB_ID_UniqueAddr = 0xa8,
    ZB_NWK_NIB_ID_AddressMap = 0xa9,
    /* ...continued in zigbee.aps.h with ZB_APS_IB_ID_DEVICE_KEY_PAIR_SET... */
    ZB_NWK_NIB_ID_DiscoveryTable = 0xb4, /* ZbNwkDiscoveryInfoT - Get only. */

    /* 0x400 to 0x4ff reserved for custom NIBs. */
    ZB_NWK_NIB_ID_Depth = 0x0400,
    ZB_NWK_NIB_ID_FrameCounterSet = 0x0401,
    /* discontinuity */
    ZB_NWK_NIB_ID_FastPollPeriod = 0x0403, /* Only used as sleepy end devices to set the fast polling interval. */
    ZB_NWK_NIB_ID_FrameCounterCooldown = 0x0404, /* Cooldown timer (in seconds) to apply to frame counter resets. */
    ZB_NWK_NIB_ID_OutgoingCounter = 0x0405, /* Global outgoing frame counter. */
    ZB_NWK_NIB_ID_PersistCounter = 0x0406, /* Persisted outgoing frame counter. */

    /* R21+ attributes */
    ZB_NWK_NIB_ID_LeaveRequestAllowed = 0x0407,
    ZB_NWK_NIB_ID_ParentInformation = 0x0408,
    ZB_NWK_NIB_ID_EndDeviceTimeoutDefault = 0x0409,
    ZB_NWK_NIB_ID_EdkaFailThreshold = 0x040a,
    /**< Number of consecutive EDKA request failures before triggering a PARENT_LINK_FAILURE.
     * Valid range is 1 to 4. Default is 1. */
    ZB_NWK_NIB_ID_LeaveRequestWithoutRejoinAllowed = 0x040b,
    ZB_NWK_NIB_ID_DisablePeriodicTimers = 0x040c,
    /**< If set, NWK layer disables automatic EDKA and Link Power Negotiation timers.
     * Default is 0 (automatic timers are enabled). If automatic EDKA is disabled,
     * the application can call ZbNwkSendEdkaReq() periodically itself to refresh
     * the timeout on its Parent. */

    /* R22+ attributes */
    ZB_NWK_NIB_ID_TxPowerMgmtSupported = 0x040d, /* not affected by nwk_reset_nib, keeps value. */
    ZB_NWK_NIB_ID_LinkPowerDeltaPeriod = 0x040e,

    /* R22 MAC PIB Attributes that are maintained by our NWK layer, since we support multiple interfaces. */
    ZB_NWK_NIB_ID_JoiningListUpdateId = 0x040f, /* IeeeJoiningListUpdateID [uint8_t] */
    ZB_NWK_NIB_ID_JoiningPolicy = 0x0410, /* mibJoiningPolicy [uint8_t] */
    ZB_NWK_NIB_ID_JoiningListTotal = 0x0411, /* IeeeJoiningListTotal [uint8_t] */
    ZB_NWK_NIB_ID_JoiningListExpiryInterval = 0x0412, /* mibIeeeExpiryInterval minutes [uint16_t] */
    /* mibIeeeExpiryIntervalCountdown is not externally accessible */

    ZB_NWK_NIB_ID_ActiveChannelList = 0x0413, /* struct ZbChannelListT - Get only. */
    ZB_NWK_NIB_ID_PermitJoinCounter = 0x0414, /* uint8_t - Get only. Set through ZbNlmePermitJoinReq */
    ZB_NWK_NIB_ID_PassiveAckEnabled = 0x0415, /* uint8_t - Enable/Disable Broadcast Passive ACK */

    /* discontinuity */
    /* Exegin Custom Attributes */
    ZB_NWK_NIB_ID_AllowLostParentFlag = 0x04a3,
    /**< If true, then a stack internal flag will be set when ZB_NWK_STATUS_CODE_PARENT_LINK_FAILURE
     * is generated via NLME-NWK-STATUS.indication. When the flag is set, the NLDE-DATA.request
     * will return ZB_NWK_STATUS_INVALID_REQUEST if attempting to send packet to, or through,
     * the parent. Note that all APSDE-DATA.request API calls will subsequently call the
     * NLDE-DATA.request API to the network layer. By setting this flag, it gives the application
     * an immediate error response if there's a problem communicating the parent, rather than
     * waiting for the approximately 10 seconds for the APS retry mechanism to complete and
     * ultimately fail. The flag is cleared once a packet is received by the parent, or if
     * this attribute's value is set to 0. */

    ZB_NWK_NIB_ID_SedTimeoutLeaveDuration = 0x04a4,
    /**< How long in seconds to keep trying to send a Network Leave Request indirectly to
     * Sleepy End Device (SED) after it has timed-out from the NNT.
     * Minimum duration is 30 seconds, and maximum duration is 1 hour.
     * Default is 5 minutes. */

    ZB_NWK_NIB_ID_SedLeaveRspDuringRejoin = 0x04a5,
    /**< This flag controls whether we are we allowed to send a Network Leave Response
     * for a Leave Request while we're in the middle of a rejoin. By default,
     * and since R22.2 especially, the SED should not send a Leave Response. */

    ZB_NWK_NIB_ID_Max = 0x04ff
};

/* Possible values for ZbNlmeJoinReqT.rejoinNetwork */
enum ZbNwkRejoinTypeT {
    ZB_NWK_REJOIN_TYPE_ASSOC = 0x00,
    ZB_NWK_REJOIN_TYPE_ORPHAN,
    ZB_NWK_REJOIN_TYPE_NWKREJOIN
};

/* R22 (GB-868) Joining Policies */
enum WpanJoinPolicyT {
    WPAN_JOIN_POLICY_ALL = 0, /* 0x00 = ALL_JOIN */
    WPAN_JOIN_POLICY_IEEELIST, /* 0x01 = IEEELIST_JOIN */
    WPAN_JOIN_POLICY_NONE /* 0x02 = NO_JOIN */
};

/* Maximum link cost. */
#define ZB_NWK_LINK_COST_MAX                        7U

/* NWK Constants */
#define ZB_NWK_CONST_SECURITY_LEVEL                 0x05U
#define ZB_NWK_CONST_DISCOVERY_RETRY_LIMIT          0x03U
#define ZB_NWK_CONST_MAX_DEPTH                      0x0fU
#define ZB_NWK_CONST_MIN_HEADER_OVERHEAD            0x08U
#define ZB_NWK_CONST_PROTOCOL_VERSION               ZB_PROTOCOL_VERSION_2007
#define ZB_NWK_CONST_WAIT_BEFORE_VALIDATION         0x500U /* In ms. */
#define ZB_NWK_CONST_REPAIR_THRESHOLD               0x03U
#define ZB_NWK_CONST_ROUTE_DISCOVERY_TIME           0x2710U /* In ms. (10 seconds) */
#define ZB_NWK_CONST_MAX_BROADCAST_JITTER           0x40U /* In ms. */
#define ZB_NWK_CONST_INITIAL_RREQ_RETRIES           0x03U
#define ZB_NWK_CONST_RREQ_RETRIES                   0x02U
#define ZB_NWK_CONST_RREQ_RETRY_INTERVAL            0xfeU /* 254 mS */
#define ZB_NWK_CONST_MIN_RREQ_JITTER                0x01U /* In 2 ms slots. */
#define ZB_NWK_CONST_MAX_RREQ_JITTER                0x40U /* In 2 ms slots. */
#define ZB_NWK_CONST_MAC_FRAME_OVERHEAD             0x0bU /* See D.4 of 053474r17. */
/* The following are added by Exegin */
#define ZB_NWK_CONST_SECURITY_OVERHEAD              (14U + ZB_SEC_MIC_LENGTH_5)
/* 127 - 8 - 11 = 108 bytes */
#define ZB_NWK_CONST_MAX_PAYLOAD_SIZE               (WPAN_CONST_MAX_PHY_PACKET_SIZE - ZB_NWK_CONST_MIN_HEADER_OVERHEAD - ZB_NWK_CONST_MAC_FRAME_OVERHEAD)

#define ZB_NWK_BCNPAYLOAD_MIN_SIZE                  15U
#define ZB_NWK_ENH_BCNPAYLOAD_SIZE                  19U

/* Upper bound on nwkMaxSourceRoute */
#define ZB_NWK_CONST_MAX_SOURCE_ROUTE               12U /* 0x0c */
/* The smaller the potential hop count, the larger packets we can send. */
#define ZB_NWK_CONST_DEFAULT_SOURCE_ROUTE           6U

#define ZB_NWK_CONST_ENDDEV_TIMEOUT_MAX             14U
#define ZB_NWK_CONST_ENDDEV_TIMEOUT_DEFAULT         8U
#define ZB_NWK_CONST_ENDDEV_TIMEOUT_DISABLED        0xffU

/* NWK Security Material Set (ZB_NWK_NIB_ID_SecurityMaterialSet) */
struct ZbNwkSecMaterialT {
    bool valid;
    uint8_t keySeqNumber;
    uint8_t key[ZB_SEC_KEYSIZE];
    enum ZbSecKeyTypeT keyType;
};

/* Frame Counter Attribute Set (ZB_NWK_NIB_ID_FrameCounterSet) */
struct ZbNwkFrameCounterT {
    uint8_t keySeqNumber;
    uint64_t senderAddr;
    uint32_t counter;
    ZbUptimeT cooldown; /* Cooldown timer for counter resets. */
    ZbUptimeT lastUsed; /* Timer for LRU evictions. */
    uint32_t persistCounter; /* Last saved counter value */
};

struct ZbNwkRouteRecordInfoT {
    unsigned int recordsSent;
    bool doSendNext; /* Send Route Record before next tx */
    bool doRenew; /* Whether to renew sending a Route Record after renewTime. */
    ZbUptimeT renewTime;
};

/* NNT Discovery Parameters (ZB_NWK_NIB_ID_DiscoveryTable) */
struct ZbNwkDiscoveryInfoT {
    uint16_t panId;
    uint64_t epid;
    uint8_t channelPage;
    uint16_t logicalChannel;
    uint8_t protocolVersion;
    uint8_t stackProfile;
    uint8_t nwkDepth;
    bool useSecurity;
    bool permitJoin;
    bool routerCapacity;
    bool endDevCapacity;
    bool potentialParent;
    uint8_t updateId;
    uint8_t lqi; /* mirror of struct ZbNwkNeighborT.lqi value */

    /* Exegin add-ons */
    uint8_t beaconOrder;
    uint8_t superframeOrder;
    /* Duplicates of struct ZbNwkNeighborT. Only used by ZB_NWK_NIB_ID_DiscoveryTable */
    uint16_t nwkAddr; /* Entry is empty if == ZB_NWK_ADDR_UNDEFINED */
};

/*
 * Neighbor Table Entry
 *
 * Contains all the mandatory fields that are used during normal
 * network operation. This is maintained in the NIB, all entries
 * are assumed to be on the same network as the local device.
 */

/* Device Type definitions. */
enum ZbNwkNeighborTypeT {
    ZB_NWK_NEIGHBOR_TYPE_COORD = 0x00,
    ZB_NWK_NEIGHBOR_TYPE_ROUTER = 0x01,
    ZB_NWK_NEIGHBOR_TYPE_END_DEV = 0x02,
    ZB_NWK_NEIGHBOR_TYPE_UNKNOWN = 0x03
};

/* Device Relationship definitions. */
enum ZbNwkNeighborRelT {
    ZB_NWK_NEIGHBOR_REL_PARENT = 0x00,
    ZB_NWK_NEIGHBOR_REL_CHILD = 0x01,
    ZB_NWK_NEIGHBOR_REL_SIBLING = 0x02,
    ZB_NWK_NEIGHBOR_REL_NONE = 0x03,
    ZB_NWK_NEIGHBOR_REL_PREV_CHILD = 0x04,
    /* Exegin Custom Values follow here */
    ZB_NWK_NEIGHBOR_REL_UNAUTH_CHILD = 0x05,
    /* Temporary values to use during association. */
    ZB_NWK_NEIGHBOR_REL_PEND_ASSOCIATE = 0x06,
    ZB_NWK_NEIGHBOR_REL_PEND_ORPHAN = 0x07
};

/* Parent Information is one byte. We extend it to two bytes for internal state. */
#define ZB_NWK_PARENT_INFO_MASK                     0x0007U
#define ZB_NWK_PARENT_INFO_DATA_POLL                0x0001U /* MAC Data Poll Keepalive Supported */
#define ZB_NWK_PARENT_INFO_KEEP_ALIVE               0x0002U /* End Device Timeout Request Keepalive Supported */
#define ZB_NWK_PARENT_INFO_POWER_NEGOT              0x0004U /* Power Negotiation Supported (R22: Link Power Delta) */
/* Exegin add-on to track the status of parent info */
#define ZB_NWK_PARENT_INFO_START                    0x0100U
/* Exegin add-on to track a manual EDKA request. */
#define ZB_NWK_PARENT_INFO_MANUAL_EDKA_REQ          0x0200U

#define ZB_NWK_NEIGHBOR_TIMEOUT_MAX                 14U

#define ZB_NWK_NEIGHBOR_IFINDEX_UNKNOWN             0xffU

struct ZbNwkNeighborT {
    /*** Mandatory Neighbor Table Entries */
    uint64_t extAddr;
    uint16_t nwkAddr; /* Set to ZB_NWK_ADDR_UNDEFINED to invalidate entry */
    uint8_t capability;
    enum ZbNwkNeighborTypeT deviceType;
    enum ZbNwkNeighborRelT relationship;
    uint8_t txFailure;
    uint8_t lqi; /* Average LQI. */
    int8_t unicastRssi; /* Average RSSI when sent as unicast. For power control. */
    uint8_t outgoingCost; /* ZigBee 2007. */
    uint8_t age; /* ZigBee 2007. */
    uint8_t interval; /* R21 draft. */
    ZbUptimeT timeout; /* R21 draft. End-device keep-alive timeout. Disabled if zero. */
    uint8_t ifc_index; /* R22 - set to ZB_NWK_NEIGHBOR_IFINDEX_UNKNOWN if not known (e.g. after persistence) */
};

/*
 * Routing Table Entry
 */

/* Route status values (2.7.3.2) */
enum ZbNwkRouteStatusT {
    ZB_NWK_ROUTE_STATUS_ACTIVE = 0x00,
    ZB_NWK_ROUTE_STATUS_DISCOVERY_UNDERWAY = 0x01,
    ZB_NWK_ROUTE_STATUS_DISCOVERY_FAILED = 0x02,
    ZB_NWK_ROUTE_STATUS_INACTIVE = 0x03
};

#define ZB_NWK_ROUTE_RECORD_RENEWAL_TIMEOUT         (60U * 1000U) /* ms */
#define ZB_NWK_ROUTE_RECORD_POST_SLEEP              (20U) /* ms */

struct ZbNwkRouteEntryT {
    enum ZbNwkRouteStatusT status;
    bool noCache; /* flag indicating destination doesn't store source routes. */
    bool isManyToOne; /* flag indicating if destination is a concentrator */
    struct ZbNwkRouteRecordInfoT routeRecord;
    uint16_t destAddr;
    uint16_t nextAddr;
    ZbUptimeT lastUsed; /* Used to measure route table ageing. */
    ZbUptimeT lastUpdate;
    uint8_t cost; /* Currently not being used. */
};

/*---------------------------------------------------------------
 * NLDE
 *---------------------------------------------------------------
 */
/* NLDE-DATA.request */
struct ZbNldeDataReqT {
    enum ZbNwkAddrModeT dstAddrMode;
    uint8_t nonMemberRadius;
    uint16_t dstAddr;
    const void *nsdu;
    uint8_t nsduLength;
    uint32_t handle;
    uint8_t radius;
    bool discoverRoute;
    bool security;
    /* Alias */
    bool useAlias;
    uint16_t aliasAddr;
    uint8_t aliasSeqnum;
};

/* NLDE-DATA.confirm */
struct ZbNldeDataConfT {
    uint32_t handle;
    enum ZbStatusCodeT status;
};

/* NLDE-DATA.indication */
struct ZbNldeDataIndT {
    enum ZbNwkAddrModeT dstAddrMode;
    uint16_t dstAddr;
    uint16_t srcAddr;
    uint16_t linkAddr; /* Exegin add-on for Intra-PAN portability. */
    uint8_t nsdu[ZB_NWK_CONST_MAX_PAYLOAD_SIZE];
    uint8_t nsduLength;
    uint8_t linkQuality;
    int8_t rssi;
    bool useSecurity;
};

enum ZbStatusCodeT ZB_WARN_UNUSED ZbNldeDataReqCallback(struct ZigBeeT *zb, struct ZbNldeDataReqT *req,
    void (*callback)(struct ZbNldeDataConfT *dataConf, void *cb_arg), void *arg);

/* NLDE-FRAME.request */
/* Exegin addon for RAW MAC frame transmission. */
struct ZbNldeFrameReqT {
    enum ZbNwkAddrModeT dstAddrMode;
    enum ZbNwkAddrModeT srcAddrMode;
    uint16_t dstAddr16;
    uint64_t dstAddr64;
    uint16_t dstPanId;
    uint8_t txOptions;
    const void *msdu;
    uint8_t msduLength;
    uint32_t handle;
};

enum ZbStatusCodeT ZB_WARN_UNUSED ZbNldeFrameReqCallback(struct ZigBeeT *zb, struct ZbNldeFrameReqT *req,
    void (*callback)(struct ZbNldeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * NLME
 *---------------------------------------------------------------
 */
/* NLME-GET.request */
struct ZbNlmeGetReqT {
    enum ZbNwkNibAttrIdT attrId;
    void *attr; /* Buffer for attribute data */
    unsigned int attrLength; /* Max length of input, actual length returned */
    unsigned int attrIndex;
};

/* NLME-GET.confirm */
struct ZbNlmeGetConfT {
    enum ZbStatusCodeT status;
    enum ZbNwkNibAttrIdT attrId;
};

/* NLME-SET.request */
struct ZbNlmeSetReqT {
    enum ZbNwkNibAttrIdT attrId;
    const void *attr;
    unsigned int attrLength;
    unsigned int attrIndex;
};

/* NLME-SET.confirm */
struct ZbNlmeSetConfT {
    enum ZbStatusCodeT status;
    enum ZbNwkNibAttrIdT attrId;
};

/* NLME-RESET.request */
struct ZbNlmeResetReqT {
    bool warmStart;
};

/* NLME-RESET.confirm */
struct ZbNlmeResetConfT {
    enum ZbStatusCodeT status;
};

/* NLME-SYNC.request */
struct ZbNlmeSyncReqT {
    bool track;
};

/* NLME-SYNC.confirm */
struct ZbNlmeSyncConfT {
    enum ZbStatusCodeT status;
};

/* NLME-SYNC.indication */
struct ZbNlmeSyncIndT {
    void *voidPtr; /* empty */
};

/* NLME-NETWORK-FORMATION.request */
struct ZbNlmeNetFormReqT {
    /* A network form always uses the standard MCP_SCAN_ACTIVE scan type
     * in order to elicit all Beacons to check for existing PAN Ids. */
    struct ZbChannelListT scanChannels;
    uint8_t scanDuration;
    uint8_t beaconOrder;
    uint8_t superframeOrder;
    uint8_t batteryLifeExtension;
    bool distributed;
};

/* NLME-NETWORK-FORMATION.confirm */
struct ZbNlmeNetFormConfT {
    enum ZbStatusCodeT status;
};

/* NLME-NETWORK-DISCOVERY.request */
struct ZbNlmeNetDiscReqT {
    struct ZbChannelListT scanChannels;
    uint8_t scanDuration;
};

/* NLME-NETWORK-DISCOVERY.confirm */
struct ZbNlmeNetDiscConfT {
    enum ZbStatusCodeT status;
    /* Use ZB_NWK_NIB_ID_DiscoveryTable to retrieve the discovery table entries. */
};

/* NLME-PERMIT-JOIN.request */
struct ZbNlmePermitJoinReqT {
    uint8_t permitDuration;
};

/* NLME-PERMIT-JOIN.confirm */
struct ZbNlmePermitJoinConfT {
    enum ZbStatusCodeT status;
};

/* NLME-JOIN.request */
struct ZbNlmeJoinReqT {
    uint64_t epid;
    enum ZbNwkRejoinTypeT rejoinNetwork;
    struct ZbChannelListT scanChannels;
    uint8_t scanDuration;
    uint8_t capabilityInformation; /* e.g. MCP_ASSOC_CAP_DEV_TYPE */
    bool securityEnable;
};

/* NLME-JOIN.confirm */
struct ZbNlmeJoinConfT {
    enum ZbStatusCodeT status;
    uint16_t nwkAddr;
};

/* NLME-JOIN.indication */
struct ZbNlmeJoinIndT {
    uint16_t nwkAddr;
    uint64_t extAddr;
    uint8_t capabilityInfo;
    enum ZbNwkRejoinTypeT rejoinNetwork;
    bool secureRejoin;
};

/* NLME-DIRECT-JOIN.request */
struct ZbNlmeDirectJoinReqT {
    uint64_t deviceAddr;
    uint16_t nwkAddr; /* Exegin addon - set to ZB_NWK_ADDR_UNDEFINED if not used. */
    uint8_t capabilityInfo;
    uint8_t interval;
};

/* NLME-DIRECT-JOIN.confirm */
struct ZbNlmeDirectJoinConfT {
    uint64_t deviceAddr;
    enum ZbStatusCodeT status;
};

/* NLME-LEAVE.request */
struct ZbNlmeLeaveReqT {
    uint64_t deviceAddr;
    bool removeChildren;
    bool rejoin;
};

/* NLME-LEAVE.confirm */
struct ZbNlmeLeaveConfT {
    uint64_t deviceAddr;
    enum ZbStatusCodeT status;
};

/* NLME-LEAVE.indication */
struct ZbNlmeLeaveIndT {
    uint64_t deviceAddr;
    bool rejoin;
    /* Exegin extension */
    uint16_t shortAddr;
    /* Is one of our children leaving? By the time the NHLE has received this, the device
     * has likely been removed as one of our children from the NNT. */
    enum ZbNwkNeighborRelT relationship;
};

/* NLME-START-ROUTER.request */
struct ZbNlmeStartRouterReqT {
    uint8_t beaconOrder;
    uint8_t superframeOrder;
    uint8_t battLifeExtension;
    struct ZbChannelListT scanChannels;
};

/* NLME-START-ROUTER.confirm */
struct ZbNlmeStartRouterConfT {
    enum ZbStatusCodeT status;
};

/* NLME-ED-SCAN.request */
struct ZbNlmeEdScanReqT {
    uint32_t channelMask; /* Channel Page Structure [PAGE|MASK] */
    uint8_t scanDuration;
};

/* NLME-ED-SCAN.confirm */
struct ZbNlmeEdScanConfT {
    uint32_t unscannedChannels;
    uint8_t energyDetectList[WPAN_PAGE_CHANNELS_MAX];
    enum ZbStatusCodeT status;
};

/* NLME-ROUTE-DISCOVERY.request */
struct ZbNlmeRouteDiscReqT {
    enum ZbNwkAddrModeT dstAddrMode;
    uint16_t dstAddr;
    uint8_t radius;
    uint8_t noRouteCache;
};

/* NLME-ROUTE-DISCOVERY.confirm */
struct ZbNlmeRouteDiscConfT {
    enum ZbStatusCodeT status;
};

/* NLME-NETWORK-STATUS.indication */
struct ZbNlmeNetworkStatusIndT {
    uint16_t shortAddr;
    enum ZbNwkNetworkStatusCodeT status;
};

/* NLME-SET-INTERFACE.request */
struct ZbNlmeSetInterfaceReqT {
    uint8_t ifcIndex;
    uint8_t state;
    uint32_t channelInUse;
    struct ZbChannelListT supportedChannels;
    bool routersAllowed;
};

struct ZbNlmeSetInterfaceConfT {
    enum ZbStatusCodeT status;
};

/* NLME-GET-INTERFACE.request */
struct ZbNlmeGetInterfaceReqT {
    uint8_t ifcIndex;
};

struct ZbNlmeGetInterfaceConfT {
    enum ZbStatusCodeT status;
    uint8_t ifcIndex;
    uint8_t state;
    uint32_t channelInUse;
    struct ZbChannelListT supportedChannels;
    bool routersAllowed;
    bool powerNegotSupported;
    /* Exegin add-on */
    struct WpanPublicT *mac;
};

/* Broadcast transaction table entry */
struct ZbNwkBttEntryT {
    uint16_t srcAddr;
    uint8_t seqnum;
    /* EXEGIN - replace pAckCount with a list of router neighbors */
    uint8_t pAckCount; /* passive ack count for flood limiting. */
    ZbUptimeT expireTime; /* expiration time relative to ZbUptime. */
};

/** Commissioning configuration. */
struct ZbNwkCommissioningInfo {
    uint8_t ifc_index; /**< NWK interface index. Starts at 0. */
    uint16_t nwk_addr; /**< Network Address to configure (default is ZB_NWK_ADDR_UNDEFINED) */
    uint16_t pan_id; /**< PAN Id to configure (default is ZB_NWK_ADDR_UNDEFINED) */
    uint8_t rx_on; /**< Configures RxOnWhenIdle to MAC */
    uint8_t page; /**< Channge Page to configure */
    uint8_t channel; /**< Channel to configure */
};

/**
 * Configures the MAC interface to be able to send and receive commissioning InterPAN packets.
 * This API is provided so the stack doesn't need to expose the MLME-SET API.
 * This should be called before the stack has been started (i.e. ZbStartup).
 * @param zb Zigbee instance.
 * @param commission_info Pointer to commissioning configuration data structure.
 * @return True if configuration was successfully applied, false otherwise.
 */
bool ZbNwkCommissioningConfig(struct ZigBeeT *zb, struct ZbNwkCommissioningInfo *commission_info);

/* NLME-GET.request */
void ZbNlmeGetReq(struct ZigBeeT *zb, struct ZbNlmeGetReqT *getReqPtr, struct ZbNlmeGetConfT *getConfPtr);

/* NLME-SET.request */
void ZbNlmeSetReq(struct ZigBeeT *zb, struct ZbNlmeSetReqT *setReqPtr, struct ZbNlmeSetConfT *setConfPtr);

/* NLME-RESET.request */
void ZbNlmeResetReq(struct ZigBeeT *zb, struct ZbNlmeResetReqT *resetReqPtr, struct ZbNlmeResetConfT *resetConfPtr);

/* NLME-SYNC.request */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbNlmeSyncReq(struct ZigBeeT *zb, struct ZbNlmeSyncReqT *syncReqPtr,
    void (*callback)(struct ZbNlmeSyncConfT *syncConfPtr, void *arg), void *arg);

/* NLME-NETWORK-DISCOVERY.request */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbNlmeNetDiscReq(struct ZigBeeT *zb, struct ZbNlmeNetDiscReqT *req,
    void (*callback)(struct ZbNlmeNetDiscConfT *conf, void *cbarg), void *cbarg);

/* NLME-NETWORK-FORMATION.request */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbNlmeNetFormReq(struct ZigBeeT *zb, struct ZbNlmeNetFormReqT *req,
    void (*callback)(struct ZbNlmeNetFormConfT *formConf, void *arg), void *cbarg);

/* NLME-PERMIT-JOIN.request */
void ZbNlmePermitJoinReq(struct ZigBeeT *zb, struct ZbNlmePermitJoinReqT *permitReq, struct ZbNlmePermitJoinConfT *permitConf);

/* Exegin Custom API to manage the IEEE Joining List in the NWK layer.
 * Let the NWK layer manage configuring the individual MACs. */
enum WpanJoinPolicyT ZbNlmeJoiningPolicyGet(struct ZigBeeT *zb);
bool ZbNlmeJoiningPolicyConfigure(struct ZigBeeT *zb, enum WpanJoinPolicyT policy, uint64_t *extAddrList,
    unsigned int numExtAddr, uint8_t *updateIdOverride);
void ZbNlmeJoinPolicyTimeoutRefresh(struct ZigBeeT *zb);
void ZbNlmeIeeeJoiningListClear(struct ZigBeeT *zb);
bool ZbNlmeIeeeJoiningListRemove(struct ZigBeeT *zb, uint64_t extAddr);
bool ZbNwkIeeeJoiningListEnabled(struct ZigBeeT *zb);

/* NLME-JOIN.request */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbNlmeJoinReq(struct ZigBeeT *zb, struct ZbNlmeJoinReqT *joinReqPtr,
    void (*callback)(struct ZbNlmeJoinConfT *joinConf, void *arg), void *cbarg);

/* NLME-DIRECT-JOIN.request */
void ZbNlmeDirectJoinReq(struct ZigBeeT *zb, struct ZbNlmeDirectJoinReqT *directJoinReqPtr, struct ZbNlmeDirectJoinConfT
    *directJoinConfPtr);

/* NLME-START-ROUTER.request */
void ZbNlmeStartRouterReq(struct ZigBeeT *zb, struct ZbNlmeStartRouterReqT *req, struct ZbNlmeStartRouterConfT *conf);

/* NLME-ED-SCAN.request */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbNlmeEdScanReq(struct ZigBeeT *zb, struct ZbNlmeEdScanReqT *req,
    void (*callback)(struct ZbNlmeEdScanConfT *scanConf, void *arg), void *cbarg);

/* NLME-LEAVE.request. */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbNlmeLeaveReq(struct ZigBeeT *zb, struct ZbNlmeLeaveReqT *leaveReqPtr,
    void (*callback)(struct ZbNlmeLeaveConfT *leaveConfPtr, void *arg), void *cbarg);

/* NLME-ROUTE-DISCOVERY.request */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbNlmeRouteDiscReq(struct ZigBeeT *zb, struct ZbNlmeRouteDiscReqT *routeDiscReqPtr,
    void (*callback)(struct ZbNlmeRouteDiscConfT *discConf, void *cbarg), void *arg);

/* NLME-SET-INTERFACE.request */
void ZbNlmeSetInterface(struct ZigBeeT *zb, struct ZbNlmeSetInterfaceReqT *req, struct ZbNlmeSetInterfaceConfT *conf);
/* NLME-GET-INTERFACE.request */
void ZbNlmeGetInterface(struct ZigBeeT *zb, struct ZbNlmeGetInterfaceReqT *req, struct ZbNlmeGetInterfaceConfT *conf);

/* Helper to enable/disable interface */
bool ZbNwkIfToggleByName(struct ZigBeeT *zb, const char *name, bool enable);

bool ZbNwkIfSetTxPower(struct ZigBeeT *zb, const char *name, int8_t tx_power);
bool ZbNwkIfGetTxPower(struct ZigBeeT *zb, const char *name, int8_t *tx_power);

/* ZbNwkToggleDutyCycle - Enable or disable Duty Cycle management in the MAC.
 * Disabling duty cycle will also clear the duty cycle history and set the status to
 * MCP_DUTYCYCLE_STATUS_NORMAL.
 * This function would typically be used in conjunction with ZB_NWK_NIB_ID_TxPowerMgmtSupported,
 * which configures TX Power Control in the NWK and MAC. */
bool ZbNwkToggleDutyCycle(struct ZigBeeT *zb, bool enable);

/* Network end device keep alive request command. Can only be used if
 * ZB_NWK_NIB_ID_DisablePeriodicTimers is set to 1 (automatic EDKA is disabled). */
bool ZbNwkSendEdkaReq(struct ZigBeeT *zb);

/* Network Link power delta request & notify commands. */
bool ZbNwkSendLinkPowerDeltaReq(struct ZigBeeT *zb);
bool ZbNwkSendLinkPowerDeltaNotify(struct ZigBeeT *zb);

/*---------------------------------------------------------------
 * NLME-GET and NLME-SET Helper Functions
 *---------------------------------------------------------------
 */
/* ZbNwkGet and ZbNwkSet return the confirm status byte (SUCCESS == 0x00) */
enum ZbStatusCodeT ZbNwkGet(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz);
enum ZbStatusCodeT ZbNwkSet(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz);

enum ZbStatusCodeT ZbNwkGetIndex(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz, unsigned int attrIndex);
enum ZbStatusCodeT ZbNwkSetIndex(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz, unsigned int attrIndex);

/*---------------------------------------------------------------
 * NIB Attribute Helper Functions
 *---------------------------------------------------------------
 */
unsigned int ZbNwkAddrMapFreeSpace(struct ZigBeeT *zb);
bool ZbNwkAddrStoreMap(struct ZigBeeT *zb, uint16_t nwkAddr, uint64_t extAddr, bool resolve_now);
void ZbNwkAddrClearMap(struct ZigBeeT *zb, bool isShortAddr, uint64_t addr);

/* Returns the network address that corresponds to the extended address
 * if known, otherwise returns ZB_NWK_ADDR_UNDEFINED. */
uint16_t ZbNwkAddrLookupNwk(struct ZigBeeT *zb, uint64_t extAddr);

/* Returns the extended address that corresponds to the network address
 * if known, otherwise returns 0. */
uint64_t ZbNwkAddrLookupExt(struct ZigBeeT *zb, uint16_t nwkAddr);

bool ZbNwkGetSecMaterial(struct ZigBeeT *zb, uint8_t keySeqno, struct ZbNwkSecMaterialT *material);
bool ZbNwkSetFrameCounter(struct ZigBeeT *zb, uint8_t keySeqno, uint64_t srcAddr, uint32_t newFrameCount);
bool ZbNwkGetActiveKey(struct ZigBeeT *zb, struct ZbNwkSecMaterialT *active_key);
bool ZbNwkClearActiveKey(struct ZigBeeT *zb);

/*---------------------------------------------------------------
 * NNT Helper Functions
 *---------------------------------------------------------------
 */
const char * ZbNwkNeighborRelationshipToStr(enum ZbNwkNeighborRelT relationship);
const char * ZbNwkNeighborDeviceTypeToStr(enum ZbNwkNeighborTypeT deviceType);

/* Parent Functions */
uint64_t ZbNwkGetParentExtAddr(struct ZigBeeT *zb);
uint16_t ZbNwkGetParentShortAddr(struct ZigBeeT *zb);

/* Clear the NNT. For example, as a zigbee end-device performing a
 * network discovery scan before rejoin, it needs to first clear the
 * NNT except for it's parent entry (keep_parent = true). This way,
 * only potential parents that responded to the network discovery
 * scan will be attempted to rejoin the network to. */
void ZbNwkNeighborClearAll(struct ZigBeeT *zb, bool keep_parent, bool keep_children);

/*---------------------------------------------------------------
 * Fast Polling
 *---------------------------------------------------------------
 */
/* NLME-SYNC.request parent polling */

/* The default delay before sending the first NLME-SYNC */
#define ZB_NWK_SYNC_DEFAULT_DELAY_MS        50U

/**
 * Make the stack begin (or continue) fast polling. Fast polling period is defined by
 * ZB_NWK_NIB_ID_FastPollPeriod.
 * @param zb Zigbee instance
 * @param delay Delay in milliseconds before sending the first NLME-SYNC
 * (e.g. ZB_NWK_SYNC_DEFAULT_DELAY_MS)
 * @param timeout Timeout in milliseconds for this fast polling interval. If 0,
 * then interval is indefinite and only stopped by calling ZbNwkFastPollRelease
 * @return Fast polling entry pointer, to be used as a handle to track this polling request.
 */
struct nwk_fastpoll_entry_t * ZbNwkFastPollRequest(struct ZigBeeT *zb,
    unsigned int delay, unsigned int timeout);

/**
 * Release a fast polling request. If this is the last fast polling request, then
 * fast polling will be stopped.
 * @param zb Zigbee instance
 * @param entry Fast polling entry pointer (handle) returned by ZbNwkFastPollRequest
 * @return True if found and removed, false otherwise.
 */
bool ZbNwkFastPollRelease(struct ZigBeeT *zb, struct nwk_fastpoll_entry_t *handle);

/**
 * Get the current number of fast polling requests to the stack.
 * @param zb Zigbee instance
 * @return Number of outstanding fast polling requests.
 */
unsigned int ZbNwkFastPollResourceCount(struct ZigBeeT *zb);

#endif /* ZIGBEE_NWK_H */
