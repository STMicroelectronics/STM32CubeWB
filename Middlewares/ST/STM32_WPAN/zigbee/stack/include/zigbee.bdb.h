/**
 * @file zigbee.bdb.h
 * @brief BDB header file
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZIGBEE_BDB_H
#define ZIGBEE_BDB_H

/* Values of the bdbCommissioningStatus attribute.
 * These are not interchangeable with Zigbee Status Codes. */
/** bdbCommissioningStatus */
enum ZbBdbCommissioningStatusT {
    ZB_BDB_COMMISS_STATUS_SUCCESS = 0x00, /**< SUCCESS - The commissioning sub-procedure was successful */
    ZB_BDB_COMMISS_STATUS_IN_PROGRESS, /**< IN_PROGRESS - One of the commissioning sub-procedures has started but is not yet complete */
    ZB_BDB_COMMISS_STATUS_NOT_AA_CAPABLE, /**< NOT_AA_CAPABLE - The initiator is not address assignment capable during touchlink */
    ZB_BDB_COMMISS_STATUS_NO_NETWORK, /**< NO_NETWORK - A network has not been found during network steering or touchlink*/
    ZB_BDB_COMMISS_STATUS_TARGET_FAILURE, /**< TARGET_FAILURE - A node has not joined a network when requested during touchlink */
    ZB_BDB_COMMISS_STATUS_FORMATION_FAILURE, /**< FORMATION_FAILURE - A network could not be formed during network formation */
    ZB_BDB_COMMISS_STATUS_NO_IDENTIFY_QUERY_RESPONSE, /* NO_IDENTIFY_QUERY_RESPONSE - No response to an identify query command has been received during finding & binding */
    ZB_BDB_COMMISS_STATUS_BINDING_TABLE_FULL, /**< BINDING_TABLE_FULL - A binding table entry could not be created due to insufficient space in the binding table during finding & binding */
    ZB_BDB_COMMISS_STATUS_NO_SCAN_RESPONSE, /**< NO_SCAN_RESPONSE - No response to a scan request inter-PAN command has been received during touchlink */
    ZB_BDB_COMMISS_STATUS_NOT_PERMITTED, /**< NOT_PERMITTED - A touchlink (steal) attempt was made when a node is already connected to a centralized security network or when end node attempts to form network */
    ZB_BDB_COMMISS_STATUS_TCLK_EX_FAILURE, /**< TCLK_EX_FAILURE - The Trust Center link key exchange procedure has failed attempting to join a centralized security network */
    ZB_BDB_COMMISS_STATUS_NOT_ON_A_NETWORK, /**< NOT_ON_A_NETWORK - A commissioning procedure was forbidden since the node was not currently on a network. */
    ZB_BDB_COMMISS_STATUS_ON_A_NETWORK /**< ON_A_NETWORK - A commissioning procedure was forbidden since the node was currently on a network. */
};

#define BDB_DEFAULT_TC_NODE_JOIN_TIMEOUT        15 /* seconds */

#define DEFAULT_EP_BDB_COMMISSION_GRP_ID     0xffffU

/* Bits for ZB_BDB_CommissioningMode */
#define BDB_COMMISSION_MODE_MASK                0x0FU
#define BDB_COMMISSION_MODE_TOUCHLINK           0x01U
#define BDB_COMMISSION_MODE_NET_STEER           0x02U
#define BDB_COMMISSION_MODE_NET_FORM            0x04U /* Whether to form a network. Configured by ZbStartup (e.g. ZbStartTypeForm) */
#define BDB_COMMISSION_MODE_FIND_BIND           0x08U

/* Bits for bdbCommissioningCapability. */
#define BDB_COMMISSION_CAP_MASK                 0x0FU
#define BDB_COMMISSION_CAP_STEER                0x01U
#define BDB_COMMISSION_CAP_NETWORK              0x02U
#define BDB_COMMISSION_CAP_FIND_BIND            0x04U
#define BDB_COMMISSION_CAP_TL                   0x08U

/* Bits for bdbNodeJoinLinkKeyType */
#define BDB_JOINLINK_KEYTYPE_FLAG               0x0F
#define BDB_JOINLINK_KEYTYPE_GTC                0x01 /* global trust center*/
#define BDB_JOINLINK_KEYTYPE_DS                 0x02 /* distributed security global */
#define BDB_JOINLINK_KEYTYPE_IC                 0x04 /* install code preconfigured. */
#define BDB_JOINLINK_KEYTYPE_TC                 0x08 /* touchlink preconfigured. */

/* values for ZB_BDB_TCLinkKeyExchangeMethod / bdbTCLinkKeyExchangeMethod */
/** bdbTCLinkKeyExchangeMethod */
enum ZbBdbLinkKeyExchMethodT {
    BDB_LINKKEY_EXCHANGE_METHOD_APS = 0x00, /**< APS Request Key */
    BDB_LINKKEY_EXCHANGE_METHOD_CBKE = 0x01 /**< Certificate Based Key Exchange (CBKE) */
};

/* Touchlink Default Primary Channel Set (2.4 GHz, Page 0, based on WPAN_CHANNELMASK_2400MHZ)
 * Channels: 11, 15, 20, 25 */
#define BDBC_TL_PRIMARY_CHANNEL_SET             0x02108800U
/* Secondary = 0x05ef7000.
 * Note that scanning the full primary and secondary channels sets may
 * not get done before the Touchlink process times-out. This is an
 * inherent problem with the Touchlink spec. By default, the stack
 * uses a secondary mask equal to zero. */
#define BDBC_TL_SECONDARY_CHANNEL_SET           (WPAN_CHANNELMASK_2400MHZ ^ BDBC_TL_PRIMARY_CHANNEL_SET)

/* value for bdbRssiMin - used by touchLink ONLY */
#define BDB_TL_RSSI_MIN                         (-40)

/* Table 13-23 Key Encryption Algorithm
 * NOTE: This is what keyMask<->keyIndex translates to:
 * KeyMask   Key Description       Algorithm      Supported
 *   0       development key       13.3.4.10.4    NO - use 4 or 15 instead
 *   4       production key        13.3.4.10.5    YES (same as certification)
 *  15       certification key     13.3.4.10.5    YES
 */

/** ZbBdbTouchlinkKeyIndex */
enum ZbBdbTouchlinkKeyIndexT {
    TOUCHLINK_KEY_INDEX_DEVELOPMENT = 0, /**< Development key */
    TOUCHLINK_KEY_INDEX_PRODUCTION = 4, /**< Production key */
    TOUCHLINK_KEY_INDEX_CERTIFICATION = 15 /**< Certification key */
};

/*** Touchlink Steal Flags (ZB_BDB_TLStealFlags) ***/
/* Target is allowed to process a Touchlink Network Start Request */
#define TOUCHLINK_STEAL_START               0x01U
/* Target is allowed to process a Touchlink Join Request */
#define TOUCHLINK_STEAL_JOIN                0x02U
/* Target is allowed to process a Touchlink Network Start Request if already joined to network. */
#define TOUCHLINK_STEAL_ONNET_START         0x04U
/* Target is allowed to process a Touchlink Network Join Request if already joined to network. */
#define TOUCHLINK_STEAL_ONNET_JOIN          0x08U
/* Target is allowed to process a Touchlink Factory New (Reset) Request */
#define TOUCHLINK_STEAL_RESET               0x10U
/* Target is allowed to process a Touchlink Factory New (Reset) Request if currently acting as Coordinator. */
#define TOUCHLINK_STEAL_COORD_RESET         0x20U
/* Default is all flags enabled */
#define TOUCHLINK_STEAL_DEFAULT             0x3fU

/*** ZB_BDB_Flags ***/
/* A Zigbee device is supposed to only select potential parents with a cost of 3 or lower.
 * This flag disables that check and allows potential parents with higher cost. */
#define ZB_BDB_FLAG_IGNORE_COST_DURING_JOIN             0x00000001U

/* By default, a Zigbee device is supposed to send a ZDO Permit-Join broadcast to the
 * network after it joins, to keep the network open for other devices to join.
 * This flag disables that behaviour. */
#define ZB_BDB_FLAG_DISABLE_ZDO_PJOIN_AFTER_JOIN        0x00000002U

/* By default, the stack will perform a network rejoin after starting from persistence,
 * if it's an end-device. This flag disables that. */
#define ZB_BDB_FLAG_DISABLE_REJOIN_AFTER_PERSIST        0x00000004U

/* By default, a Zigbee device is supposed to send a ZDO Device-Annce broadcast to the
 * network after it joins and whenever it changes short addresses.
 * This flag disables that behaviour. */
#define ZB_BDB_FLAG_DISABLE_DEVICE_ANNCE                0x00000008U

/* Controls whether we allow the local device acting as a Router to perform a
 * Network Rejoin after receiving a Network Leave with the Rejoin flag set.
 * It normally doesn't make sense for this type of behaviour with a Router, only
 * an End-Device. */
#define ZB_BDB_FLAG_ALLOW_ROUTER_LEAVE_REJOIN           0x00000010U

/* ZDO Binding, ignore check if endpoint and cluster exist */
/* NOTE: removed. Checking was added as Z3.0, but since removed due to CCB 2126 */
/* #define ZB_BDB_FLAG_ZDO_BIND_IGNORE_CHECKS              0x00000020U */

/* Disable the reception and processing of APS InterPAN messages */
#define ZB_BDB_FLAG_APS_INTERPAN_DISABLE                0x00000040U

/* If set and (ZB_BDB_JoinScanType == MCP_SCAN_ENHANCED), don't try to do a standard
 * beacon request when trying to scan for a network to join. If cleared and
 * (ZB_BDB_JoinScanType == MCP_SCAN_ENHANCED), the startup joining process
 * will fallback to attempt MCP_SCAN_ACTIVE if no networks are found. */
#define ZB_BDB_FLAG_JOIN_SCAN_FALLBACK_DISABLE          0x00000080U

/* If set, disable the ZDO from periodically querying the NNT for TX failures and
 * starting an ED scan if it encounters a high failure rate. If the Coordinator
 * does not support or provide frequency agility to the network, this is a waste of
 * time. The ED scan also causes the device to go offline for a period of time,
 * which can cause further network problems. */
#define ZB_BDB_FLAG_ZDO_NWK_UPDATE_SCAN_DISABLE         0x00000100U

/* Used for debugging APS layer */
#define ZB_BDB_FLAG_DEBUG_APS                           0x00000200U

/** BDB IB Attributes */
enum ZbBdbAttrIdT {
    /* EXEGIN ZB_BDB_CommissioningGroupID ? = 0x1000 */
    ZB_BDB_CommissioningMode = 0x1001, /**< bdbCommissioningMode - BDB_COMMISSION_MODE_MASK */
    ZB_BDB_JoiningNodeEui64 = 0x1002,
    ZB_BDB_JoiningNodeNewTCLinkKey = 0x1003,
    ZB_BDB_JoinUsesInstallCodeKey = 0x1004, /**< bdbJoinUsesInstallCodeKey */
    ZB_BDB_NodeCommissioningCapability = 0x1005, /**< bdbNodeCommissioningCapability - BDB_COMMISSION_CAP_MASK */
    ZB_BDB_NodeIsOnANetwork = 0x1006, /* bdbNodeIsOnANetwork - Checks nwkExtendedPanId if non-zero */
    ZB_BDB_NodeJoinLinkKeyType = 0x1007, /* bdbNodeJoinLinkKeyType - BDB_JOINLINK_KEYTYPE_FLAG, Link key with which the node was able to decrypt the network key */
    ZB_BDB_PrimaryChannelSet = 0x1008, /**< bdbPrimaryChannelSet */
    ZB_BDB_ScanDuration = 0x1009, /**< bdbScanDuration */
    ZB_BDB_SecondaryChannelSet = 0x100a, /**< bdbSecondaryChannelSet */
    ZB_BDB_TCLK_ExchangeAttempts = 0x100b, /**< bdbTCLinkKeyExchangeAttempts */
    ZB_BDB_TCLK_ExchangeAttemptsMax = 0x100c, /**< bdbTCLinkKeyExchangeAttemptsMax */
    ZB_BDB_TCLinkKeyExchangeMethod = 0x100d, /**< bdbTCLinkKeyExchangeMethod - enum ZbBdbLinkKeyExchMethodT */
    ZB_BDB_TrustCenterNodeJoinTimeout = 0x100e, /**< bdbTrustCenterNodeJoinTimeout */
    ZB_BDB_TrustCenterRequiresKeyExchange = 0x100f, /**< bdbTrustCenterRequireKey-Exchange - Modifies ZB_APSME_POLICY_TCLK_UPDATE_REQUIRED bit in ZB_APS_IB_ID_TRUST_CENTER_POLICY */
    ZB_BDB_AcceptNewUnsolicitedTCLinkKey = 0x1010, /**< acceptNewUnsolicitedTrustCenterLinkKey */ /* uint8_t */
    ZB_BDB_AcceptNewUnsolicitedApplicationLinkKey = 0x1011, /**< acceptNewUnsolicitedApplicationLinkKey */ /* uint8_t*/

    /* Extra stuff not explicitly covered by the BDB spec. */
    ZB_BDB_JoiningNodeParent = 0x1100, /* EUI of parent of joining device (where to send APS Remove Request if necessary) */
    ZB_BDB_vDoPrimaryScan = 0x1101, /* boolean whether to use ZB_BDB_PrimaryChannelSet or ZB_BDB_SecondaryChannelSet */
    /* Address assignment */
    ZB_BDB_FreeNetAddrBegin = 0x1102,
    ZB_BDB_FreeNetAddrCurrent = 0x1103,
    ZB_BDB_FreeNetAddrEnd = 0x1104,
    /* discontinuity */
    ZB_BDB_TLRssiMin = 0x1107, /**< Minimum RSSI threshold for Touchlink commissioning */ /* int8_t value */
    ZB_BDB_TLTestFlags = 0x1108, /* Touchlink test flags, enum ZbTlTestFlagsT) */
    ZB_BDB_UpdateDeviceKeyId = 0x1109, /**< enum ZbSecHdrKeyIdT (e.g. ZB_SEC_KEYID_NETWORK (default) or ZB_SEC_KEYID_LINK) */
    ZB_BDB_JoinScanType = 0x110a, /**< MCP_SCAN_ACTIVE (default) or MCP_SCAN_ENHANCED */ /* uint8_t */
    /* discontinuity */
    ZB_BDB_NlmeSyncFailNumBeforeError = 0x110c,
    /**< Number of consecutive NLME-SYNC failures before reporting ZB_NWK_STATUS_CODE_PARENT_LINK_FAILURE */ /* uint8_t */
    ZB_BDB_ZdoTimeout = 0x110d, /**< ZDO response wait timeout in milliseconds - default is 6000 mS */
    ZB_BDB_TLStealFlags = 0x110e, /**< Touchlink Stealing Flags (TOUCHLINK_STEAL_* defines) */
    ZB_BDB_JoinTclkNodeDescReqDelay = 0x110f, /* mS */
    ZB_BDB_JoinTclkRequestKeyDelay = 0x1110, /* mS */
    /* discontinuity */
    ZB_BDB_TLKey = 0x1112, /**< Touchlink preconfigured link key */
    ZB_BDB_TLKeyIndex = 0x1113, /**< enum ZbBdbTouchlinkKeyIndexT - Touchlink key encryption algorithm key index */
    /* discontinuity */
    ZB_BDB_ZdoZigbeeProtocolRevision = 0x1115, /**< Default 23 (R23) */
    /* discontinuity */
    ZB_BDB_PersistTimeoutMs = 0x1117, /**< Minimum delay between persistence updates */ /* uint32_t */
    ZB_BDB_JoinAttemptsMax = 0x1118,
    /**< Maximum number attempts to join a network. If an attempt fails,
     * the EPID is added to a blacklist before the next attempt */ /* uint8_t */
    ZB_BDB_MaxConcurrentJoiners = 0x1119, /**< Maximum number of concurrent joiners the coordinator supports */ /* uint8_t  */
    /* discontinuity */
    ZB_BDB_Uptime = 0x111d, /**< Returns the current stack uptime in milliseconds (ZbUptime) */
    ZB_BDB_Flags = 0x111e, /**< e.g. ZB_BDB_FLAG_IGNORE_COST_DURING_JOIN */ /* uint32_t  */

    /* Constants which are accessible through a BDB GET IB request. */
    ZB_BDBC_MaxSameNetworkRetryAttempts = 0x1200,
    ZB_BDBC_MinCommissioningTime = 0x1201, /**< Seconds */
    ZB_BDBC_RecSameNetworkRetryAttempts = 0x1202,
    ZB_BDBC_TLInterPANTransIdLifetime = 0x1203, /**< Seconds */
    ZB_BDBC_TLMinStartupDelayTime = 0x1204, /**< Seconds */
    ZB_BDBC_TLRxWindowDuration = 0x1205, /**< Seconds */
    ZB_BDBC_TLScanTimeBaseDuration = 0x1206 /**< Milliseconds */ /* uint8_t  */
};

/** BDB-GET.request */
struct ZbBdbGetReqT {
    enum ZbBdbAttrIdT attrId; /**< Attribute ID */
    void *attr; /**< Pointer to attribute */
    unsigned int attrLength; /**< Attribute Length */
    unsigned int attrIndex; /**< Attribute Index */
};

/** BDB-GET.confirm */
struct ZbBdbGetConfT {
    enum ZbStatusCodeT status; /**< Status */
    enum ZbBdbAttrIdT attrId; /**< Attribute ID */
};

/** BDB-SET.request */
struct ZbBdbSetReqT {
    enum ZbBdbAttrIdT attrId; /**< Attribute ID */
    const void *attr; /**< Pointer to attribute */
    unsigned int attrLength; /**< Attribute Length */
    unsigned int attrIndex; /**< Attribute Index */
};

/** BDB-SET.confirm */
struct ZbBdbSetConfT {
    enum ZbStatusCodeT status; /**< Status */
    enum ZbBdbAttrIdT attrId; /**< Attribute ID */
};

#define ZbBdbGet(_zb_, _id_, _ptr_, _sz_) ZbBdbGetIndex(_zb_, _id_, _ptr_, _sz_, 0)
#define ZbBdbSet(_zb_, _id_, _ptr_, _sz_) ZbBdbSetIndex(_zb_, _id_, _ptr_, _sz_, 0)

/**
 * Read a BDB IB attribute.
 * @param zb Zigbee stack instance
 * @param attrId Attribute ID
 * @param attrPtr Pointer to the attribute
 * @param attrSz Attribute size
 * @param attrIndex Index attribute
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbBdbGetIndex(struct ZigBeeT *zb, enum ZbBdbAttrIdT attrId, void *attrPtr, unsigned int attrSz, unsigned int attrIndex);

/**
 * Write a BDB IB attribute.
 * @param zb Zigbee stack instance
 * @param attrId Attribute ID
 * @param attrPtr Pointer to the attribute
 * @param attrSz Attribute size
 * @param attrIndex Index attribute
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZbStatusCodeT ZbBdbSetIndex(struct ZigBeeT *zb, enum ZbBdbAttrIdT attrId, const void *attrPtr, unsigned int attrSz, unsigned int attrIndex);

/**
 * Send a BDB-GET.request.
 * @param zb Zigbee stack instance
 * @param getReqPtr Pointer to BDB-GET.request
 * @param getConfPtr Pointer to BDB-GET.confirm
 * @return Returns void
 */
void ZbBdbGetReq(struct ZigBeeT *zb, struct ZbBdbGetReqT *getReqPtr, struct ZbBdbGetConfT *getConfPtr);

/**
 * Send a BDB-SET.request.
 * @param zb Zigbee stack instance
 * @param setReqPtr Pointer to BDB-SET.request
 * @param setConfPtr Pointer to BDB-SET.confirm
 * @return Returns void
 */
void ZbBdbSetReq(struct ZigBeeT *zb, struct ZbBdbSetReqT *setReqPtr, struct ZbBdbSetConfT *setConfPtr);

/* Returns the commissioning status for the given endpoint (same for all endpoints?).
 * If endpoint = ZB_ENDPOINT_BCAST, returns the status for the first endpoint found. */
enum ZbBdbCommissioningStatusT ZbBdbGetEndpointStatus(struct ZigBeeT *zb, uint8_t endpoint);

/* Configures the endpoint with the given commissioning status. Mostly for internal use only. */
void ZbBdbSetEndpointStatus(struct ZigBeeT *zb, enum ZbBdbCommissioningStatusT status, uint8_t endpoint);

#endif /* ZIGBEE_BDB_H */
