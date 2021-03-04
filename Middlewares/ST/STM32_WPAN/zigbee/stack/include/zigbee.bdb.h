/**
 * @file zigbee.bdb.h
 * @brief BDB header file
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
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
    ZB_BDB_COMMISS_STATUS_NOT_PERMITTED, /**< NOT_PERMITTED - A touchlink (steal) attempt was made when a node is already connected to a centralized security network */
    ZB_BDB_COMMISS_STATUS_TCLK_EX_FAILURE /**< TCLK_EX_FAILURE - The Trust Center link key exchange procedure has failed attempting to join a centralized security network */
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

/* Touchlink Steal Flags */
#define TOUCHLINK_STEAL_START               0x01U /* Target is allowed to process a Touchlink Network Start Request */
#define TOUCHLINK_STEAL_JOIN                0x02U /* Target is allowed to process a Touchlink Join Request */
/* EXEGIN - Make ZB_BDB_TLDenyFactoryNew one of these flags */

/* BDB IB attributes */
enum ZbBdbAttrIdT {
    /* EXEGIN ZB_BDB_CommissioningGroupID ? = 0x1000 */
    ZB_BDB_CommissioningMode = 0x1001, /* bdbCommissioningMode - e.g. BDB_COMMISSION_MODE_MASK */
    ZB_BDB_JoiningNodeEui64 = 0x1002, /* for internal use only */
    ZB_BDB_JoiningNodeNewTCLinkKey = 0x1003, /* for internal use only */
    ZB_BDB_JoinUsesInstallCodeKey = 0x1004,
    ZB_BDB_NodeCommissioningCapability = 0x1005, /* bdbNodeCommissioningCapability - e.g. BDB_COMMISSION_CAP_MASK */
    ZB_BDB_NodeIsOnANetwork = 0x1006, /* Just checks nwkExtendedPanId if non-zero */
    ZB_BDB_NodeJoinLinkKeyType = 0x1007, /* e.g. BDB_JOINLINK_KEYTYPE_FLAG - Link key with which the node was able to decrypt the network key */
    ZB_BDB_PrimaryChannelSet = 0x1008,
    ZB_BDB_ScanDuration = 0x1009,
    ZB_BDB_SecondaryChannelSet = 0x100a,
    ZB_BDB_TCLK_ExchangeAttempts = 0x100b, /* TC link key   */
    ZB_BDB_TCLK_ExchangeAttemptsMax = 0x100c,
    ZB_BDB_TCLinkKeyExchangeMethod = 0x100d, /* enum ZbBdbLinkKeyExchMethodT */
    ZB_BDB_TrustCenterNodeJoinTimeout = 0x100e,
    ZB_BDB_TrustCenterRequiresKeyExchange = 0x100f, /* Modifies ZB_APSME_POLICY_TCLK_UPDATE_REQUIRED bit in ZB_APS_IB_ID_TRUST_CENTER_POLICY */
    ZB_BDB_AcceptNewUnsolicitedTCLinkKey = 0x1010, /* uint8_t */
    ZB_BDB_AcceptNewUnsolicitedApplicationLinkKey = 0x1011, /* uint8_t */

    /* Extra stuff not explicitly covered by the BDB spec. */
    ZB_BDB_JoiningNodeParent = 0x1100, /* EUI of parent of joining device (where to send APS Remove Request if necessary) */
    ZB_BDB_vDoPrimaryScan, /* 0x1101 - internal use only - boolean whether to use ZB_BDB_PrimaryChannelSet or ZB_BDB_SecondaryChannelSet */
    /* Address assignment */
    ZB_BDB_FreeNetAddrBegin, /* 0x1102 */
    ZB_BDB_FreeNetAddrCurrent, /* 0x1103 */
    ZB_BDB_FreeNetAddrEnd, /* 0x1104 */
    /* 0x1105 - was ZB_BDB_FreeGroupIDBegin */
    /* 0x1106 - was ZB_BDB_FreeGroupIDEnd */
    ZB_BDB_TLRssiMin = 0x1107, /* 0x1107 - RSSI threshold, int8_t value */
    ZB_BDB_TLTestFlags, /* 0x1108 - Touchlink test flags (enum ZbTlTestFlagsT) */
    ZB_BDB_UpdateDeviceKeyId, /* 0x1109 - enum ZbSecHdrKeyIdT (e.g. ZB_SEC_KEYID_NETWORK (default) or ZB_SEC_KEYID_LINK) */
    ZB_BDB_JoinScanType, /* 0x110a - MCP_SCAN_ACTIVE (default) or MCP_SCAN_ENHANCED */
    ZB_BDB_JoinIgnoreLqi, /* 0x110b - bool - Ignore LQI (link cost <= 3) of potential parent's beacon  */
    ZB_BDB_NlmeSyncFailNumBeforeError, /* 0x110c - uint8_t - Number of consecutive NLME-SYNC failures before reporting ZB_NWK_STATUS_CODE_PARENT_LINK_FAILURE */
    ZB_BDB_ZdoTimeout, /* 0x110d - ZDO response wait timeout in milliseconds - default is 6000 mS */
    ZB_BDB_TLStealFlags, /* 0x110e */
    ZB_BDB_JoinTclkNodeDescReqDelay, /* 0x110f */
    ZB_BDB_JoinTclkRequestKeyDelay, /* 0x1110 */
    ZB_BDB_TLDenyFactoryNew, /* 0x1111 */
    ZB_BDB_TLKey, /* 0x1112 */
    ZB_BDB_TLKeyIndex, /* 0x1113 - enum ZbBdbTouchlinkKeyIndexT */
    ZB_BDB_ZdoPermitJoinAfterJoin, /* 0x1114 - Default is enabled. */
    ZB_BDB_ZdoZigbeeProtocolRevision, /* 0x1115 - Default 22 (R22) - was ZB_PROTOCOL_REVISION. */
    ZB_BDB_NwkAllowRouterLeaveRejoin, /* 0x1116 - Default is disabled. */
    ZB_BDB_PersistTimeoutMs, /* 0x1117 */
    ZB_BDB_JoinAttemptsMax, /* 0x1118 - uint8_t - maximum number attempts to join a network. If an attempt fails, the EPID is added to a blacklist before the next attempt. */
    ZB_BDB_MaxConcurrentJoiners, /* 0x1119 - uint8_t - maximum number of concurrent joiners the coordinator supports */
    ZB_BDB_DisablePersistRejoin, /* 0x111a - boolean */
    ZB_BDB_ZdoBindCheckCluster, /* boolean */
    ZB_BDB_ApsInterpanDisabled, /* boolean */

    /* Constants which are accessible through a BDB GET IB request. */
    ZB_BDBC_MaxSameNetworkRetryAttempts = 0x1200,
    ZB_BDBC_MinCommissioningTime, /* seconds */
    ZB_BDBC_RecSameNetworkRetryAttempts,
    ZB_BDBC_TCLinkKeyExchangeTimeout, /* seconds */
    ZB_BDBC_TLInterPANTransIdLifetime, /* seconds */
    ZB_BDBC_TLMinStartupDelayTime, /* seconds */
    ZB_BDBC_TLRxWindowDuration, /* seconds */
    ZB_BDBC_TLScanTimeBaseDuration /* uint8_t - milliseconds */
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

/* Helpers for ZB_BDB_CommissioningMode bits */
/* ZbBdbCommissionModeBitSupported - Check if a BDB_COMMISSION_MODE_ bit or mask is
 * supported by bdbNodeCommissioningCapability. */
bool ZbBdbCommissionModeBitSupported(struct ZigBeeT *zb, uint8_t new_mode_bit);
enum ZbStatusCodeT ZbBdbCommissionModeBitSet(struct ZigBeeT *zb, uint8_t new_mode_bit);
enum ZbStatusCodeT ZbBdbCommissionModeBitClear(struct ZigBeeT *zb, uint8_t new_mode_bit);

enum ZbBdbCommissioningStatusT ZbBdbNwkStatusToBdbStatus(enum ZbStatusCodeT status);
enum ZbStatusCodeT ZbBdbStatusToNwkStatus(enum ZbBdbCommissioningStatusT status);

/* Returns the commissioning status for the given endpoint (same for all endpoints?).
 * If endpoint = ZB_ENDPOINT_BCAST, returns the status for the first endpoint found. */
enum ZbBdbCommissioningStatusT ZbBdbGetEndpointStatus(struct ZigBeeT *zb, uint8_t endpoint);

/* Configures the endpoint with the given commissioning status. Mostly for internal use only. */
void ZbBdbSetEndpointStatus(struct ZigBeeT *zb, enum ZbBdbCommissioningStatusT status, uint8_t endpoint);

#endif /* ZIGBEE_BDB_H */
