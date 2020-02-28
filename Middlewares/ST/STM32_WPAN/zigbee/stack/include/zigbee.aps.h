/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZIGBEE_APS_H
# define ZIGBEE_APS_H

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */
/* APS Constants */
/* Make sure apscMaxDescriptorSize never exceeds 0xff (length field is 1 byte) */
#define ZB_APS_CONST_MAX_DESCRIPTOR_SIZE            64U
#define ZB_APS_CONST_MIN_DUPLICATE_TABLE_SIZE       1U

/* I count 8 bytes as the minimum APS overhead, not 12 (0x0c)  */
#define ZB_APS_CONST_MIN_HEADER_OVERHEAD            8U

#define ZB_APS_CONST_SECURITY_OVERHEAD              (5U + ZB_SEC_MIC_LENGTH_5)

/* For embedded application using the zigbee mempool, the maximum size
 * we can allocate is slightly larger than 2000 bytes. */
#define ZB_APS_CONST_MAX_FRAG_SIZE                  2000U

/* Maximum APS payload size if no security at all. */
#define ZB_APS_CONST_MAX_UNSECURED_PAYLOAD_SIZE     (ZB_NWK_CONST_MAX_PAYLOAD_SIZE /* 108 */ - \
                                                     ZB_APS_CONST_MIN_HEADER_OVERHEAD /* 8 */) /* = 100 */

/* Maximum (unfragmented) APS payload, assuming a NWK secured packet and no many-to-one source routing.
 * Note, if fragmentation is allowed, subtract 2 for the APS extended header, which gives a value of 80. */
#define ZB_APS_CONST_MAX_PAYLOAD_SIZE               (ZB_NWK_CONST_MAX_PAYLOAD_SIZE /* 108 */ - \
                                                     ZB_NWK_CONST_SECURITY_OVERHEAD /* 18 */ - \
                                                     ZB_APS_CONST_MIN_HEADER_OVERHEAD /* 8 */) /* = 82 */

/* Maximum (unfragmented) APS payload, assuming a NWK secured packet
 * with many-to-one source routing (source routing level = 6). */
#define ZB_APS_CONST_SAFE_NWKSEC_PAYLOAD_SIZE       (ZB_NWK_CONST_MAX_PAYLOAD_SIZE /* 108 */ - \
                                                     ZB_NWK_CONST_SECURITY_OVERHEAD /* 18 */ - \
                                                     4U /* source router header */ - \
                                                     /* was using ZB_NWK_CONST_MAX_SOURCE_ROUTE */ \
                                                     (2U * ZB_NWK_CONST_DEFAULT_SOURCE_ROUTE) /* 12 */ - \
                                                     ZB_APS_CONST_MIN_HEADER_OVERHEAD /* 8 */) /* = 66 */

/* Maximum (unfragmented) APS payload, assuming an APS secured packet
 * with many-to-one source routing (source routing level = 6). */
#define ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE       (ZB_NWK_CONST_MAX_PAYLOAD_SIZE /* 108 */ - \
                                                     ZB_NWK_CONST_SECURITY_OVERHEAD /* 18 */ - \
                                                     4U /* source router header */ - \
                                                     /* was using ZB_NWK_CONST_MAX_SOURCE_ROUTE */ \
                                                     (2U * ZB_NWK_CONST_DEFAULT_SOURCE_ROUTE) /* 12 */ - \
                                                     ZB_APS_CONST_MIN_HEADER_OVERHEAD /* 8 */ - \
                                                     ZB_APS_CONST_SECURITY_OVERHEAD /* 9 */) /* = 57 */

/* "Assuming fragmentation is used there can be 256 such blocks
 * comprising a single maximum sized ASDU."
 * ZB_APS_CONST_MAX_PAYLOAD_SIZE:               20,992 bytes
 * ZB_APS_CONST_SAFE_NWKSEC_PAYLOAD_SIZE:       16,896 bytes
 * ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE:       14,592 bytes */
#define ZB_APS_CONST_MAX_ASDU_LENGTH                (256U * ZB_APS_CONST_MAX_PAYLOAD_SIZE)

/* apscMaxFrameRetries */
#define ZB_APS_CONST_MAX_FRAME_RETRIES              3U

#define ZB_APS_CONST_MAX_WINDOW_SIZE                8U

/* Endpoint Definitions */
#define ZB_ENDPOINT_MIN                             0x00U /*   0 */
#define ZB_ENDPOINT_ZDO                             0x00U /*   0 */
#define ZB_ENDPOINT_FINDBIND_MIN                    0x01U /*   1 */
#define ZB_ENDPOINT_FINDBIND_MAX                    0xefU /* 239 */
#define ZB_ENDPOINT_CBKE_DEFAULT                    0xf0U /* 240 */
#define ZB_ENDPOINT_GREENPOWER                      0xf2U /* 242 */
#define ZB_ENDPOINT_MAX                             0xfeU /* 254 */
#define ZB_ENDPOINT_BCAST                           0xffU /* 255 */

/* Internal endpoints used for special stack functions */
#define ZB_ENDPOINT_INTERNAL                        0x0100U
#define ZB_ENDPOINT_INTERPAN                        (ZB_ENDPOINT_INTERNAL + 0U)
#define ZB_ENDPOINT_GROUP                           (ZB_ENDPOINT_INTERNAL + 1U)

/* APS Command Frame Ids */
enum ZbApsCmdIdT {
#if 0 /* deprecated as of R21 */
    ZB_APS_CMD_SKKE_1 = 0x01,
    ZB_APS_CMD_SKKE_2 = 0x02,
    ZB_APS_CMD_SKKE_3 = 0x03,
    ZB_APS_CMD_SKKE_4 = 0x04,
#endif
    ZB_APS_CMD_TRANSPORT_KEY = 0x05,
    ZB_APS_CMD_UPDATE_DEVICE = 0x06,
    ZB_APS_CMD_REMOVE_DEVICE = 0x07,
    ZB_APS_CMD_REQUEST_KEY = 0x08,
    ZB_APS_CMD_SWITCH_KEY = 0x09,
#if 0 /* deprecated as of R21 */
    ZB_APS_CMD_EA_INIT_CHLNG = 0x0a,
    ZB_APS_CMD_EA_RESP_CHLNG = 0x0b,
    ZB_APS_CMD_EA_INIT_DATA = 0x0c,
    ZB_APS_CMD_EA_RESP_DATA = 0x0d,
#endif
    ZB_APS_CMD_TUNNEL = 0x0e,
    ZB_APS_CMD_VERIFY_KEY = 0x0f,
    ZB_APS_CMD_CONFIRM_KEY = 0x10
};

/*---------------------------------------------------------------
 * APSDE
 *---------------------------------------------------------------
 */
/* APSDE and Interpan Addressing Modes */
enum ZbApsAddrModeT {
    ZB_APSDE_ADDRMODE_NOTPRESENT = 0,
    ZB_APSDE_ADDRMODE_GROUP,
    ZB_APSDE_ADDRMODE_SHORT,
    ZB_APSDE_ADDRMODE_EXT,
    ZB_APSDE_ADDRMODE_IPGROUP /* InterPAN */
};

struct ZbApsAddrT {
    enum ZbApsAddrModeT mode;
    uint16_t endpoint;
    uint16_t panId; /* Not used by ZbApsdeDataReqCallback. Mainly for InterPan packets. */
    uint16_t nwkAddr;
    uint64_t extAddr;
};

/* Helper struct to send using binding */
extern const struct ZbApsAddrT *ZbApsAddrBinding;

/* APSDE Transmit Options */
#define ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY         0x0001U
#define ZB_APSDE_DATAREQ_TXOPTIONS_NWKKEY           0x0002U
#define ZB_APSDE_DATAREQ_TXOPTIONS_ACK              0x0004U
#define ZB_APSDE_DATAREQ_TXOPTIONS_FRAG             0x0008U
#define ZB_APSDE_DATAREQ_TXOPTIONS_NONCE            0x0010U /* TODO: CCB 1184 */
/* Exegin addons */
/*
 * When the VECTOR option is set, the ASDU points instead to an array of
 * ZbApsBufT structures that point to fragments of the message to assemble into
 * one ASDU, and the ASDU length is the number of buffer structures. This is
 * used for scatter-gather mode API calls to reduce heap activity.
 */
#define ZB_APSDE_DATAREQ_TXOPTIONS_VECTOR           0x0100U
/* Used in place of the R21 useAlias parameter. */
#define ZB_APSDE_DATAREQ_TXOPTIONS_ALIAS            0x0200U

/* APSDE-DATA.request */
typedef struct {
    struct ZbApsAddrT dst;
    uint16_t profileId;
    uint16_t clusterId;
    uint16_t srcEndpt;
    const void *asdu;
    uint16_t asduLength;
    uint32_t asduHandle;
    /*
     * You should not set ZB_APSDE_DATAREQ_TXOPTIONS_ACK if you are trying to
     * send to a sleepy device. The default timeout to wait for an APS ACK
     * is 1.5 seconds, which can be much shorter than the time a sleepy device
     * can be asleep for. APS transmission may still work, but you will get
     * a lot of APS retries and error messages.
     */
    uint16_t txOptions;
    /*
     * If you perform route discovery separately using ZbNlmeRouteDiscWait(),
     * then you can set discoverRoute to zero, decreasing the length of time
     * an APS data request may take if there is a problem sending the packet
     * to the target.
     */
    bool discoverRoute;
    uint8_t radius;
    uint16_t aliasAddr;
    uint8_t aliasSeqnum;
} ZbApsdeDataReqT;

/* Buffer descriptor for vectored/scatter-gather API */
typedef struct {
    const uint8_t *data;
    unsigned int len;
} ZbApsBufT;

/* APSDE-DATA.confirm */
typedef struct {
    struct ZbApsAddrT dst;
    uint8_t srcEndpt;
    uint32_t asduHandle;
    enum ZbStatusCodeT status;
} ZbApsdeDataConfT;

/* APSDE-DATA.indication */
typedef struct {
    struct ZbApsAddrT dst;
    struct ZbApsAddrT src;
    uint16_t profileId;
    uint16_t clusterId;
    uint8_t *asdu;
    uint16_t asduLength;
    /* securityStatus, one of:
     *      ZB_APS_STATUS_UNSECURED             (no decryption necessary)
     *      ZB_APS_STATUS_SECURED_NWK_KEY       (decrypted with the Network Key)
     *      ZB_APS_STATUS_SECURED_LINK_KEY      (decrypted with a Link Key) */
    enum ZbStatusCodeT securityStatus;
    uint8_t linkQuality;
    int8_t rssi;
    /* Exegin Addon for Intra-PAN portability. */
    uint16_t linkAddr;
    /* TO BE DONE :missing aps data- not implemented yet, needed for gateway */
    int rxTime;
} ZbApsdeDataIndT;

/* APSDE-DATA.request */
void ZbApsdeDataReqWait(struct ZigBeeT *zb, ZbApsdeDataReqT *dataReqPtr, ZbApsdeDataConfT *dataConfPtr);

/* ZbApsdeDataReq is deprecated, use ZbApsdeDataReqWait instead. */
#define ZbApsdeDataReq(_zb_, _req_, _conf_) ZbApsdeDataReqWait(_zb_, _req_, _conf_)

/* Non-blocking version of APSDE-DATA.request.
 * If the packet was queued, ZB_STATUS_SUCCESS is returned. Otherwise, an error status is returned. */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbApsdeDataReqCallback(struct ZigBeeT *zb, ZbApsdeDataReqT *req,
    void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg);

/*---------------------------------------------------------------
 * APSME
 *---------------------------------------------------------------
 */
/* APS IB Attributes */
enum ZbApsmeIbAttrIdT {
    ZB_APS_IB_ID_ADDRESS_MAP = 0xc0, /* 0xc0 Removed in ZigBee 2007 */
    ZB_APS_IB_ID_BINDING_TABLE, /* 0xc1 (ZbApsmeBindT) */
    ZB_APS_IB_ID_DESIGNATED_COORD, /* 0xc2 (uint8_t) */
    ZB_APS_IB_ID_CHANNEL_MASK, /* 0xc3 (struct ZbChannelListT) Converted to a list in R22. */
    ZB_APS_IB_ID_USE_EPID, /* 0xc4 (uint64_t) */
    ZB_APS_IB_ID_GROUP_TABLE, /* 0xc5 (ZbApsmeGroupT) */
    ZB_APS_IB_ID_NONMEMBER_RADIUS, /* 0xc6 (uint8_t) */
#if 0 /* REMOVED  */
    ZB_APS_IB_ID_PERMISSIONS_CONFIG, /* 0xc7 */
#endif
    ZB_APS_IB_ID_USE_INSECURE_JOIN = 0xc8, /* 0xc8 (uint8_t) */
    ZB_APS_IB_ID_INTERFRAME_DELAY, /* 0xc9 (uint8_t) */
    ZB_APS_IB_ID_LAST_CHANNEL_ENERGY, /* 0xca (uint8_t) */
    ZB_APS_IB_ID_LAST_CHANNEL_FAILRATE, /* 0xcb (uint8_t) */
    ZB_APS_IB_ID_CHANNEL_TIMER, /* 0xcc (ZbUptimeT) */
    ZB_APS_IB_ID_MAX_WINDOW_SIZE, /* 0xcd (uint8_t) */
#if 0 /* REMOVED  */
    ZB_APS_IB_ID_PARENT_ANNOUNCE_TIMER, /* 0xce */
#endif

    /*** Security attributes ***/
    ZB_APS_IB_ID_DEVICE_KEY_PAIR_SET = 0xaa, /* 0xaa (ZbApsmeKeyPairT) */
    ZB_APS_IB_ID_TRUST_CENTER_ADDRESS, /* 0xab (uint64_t) */
    ZB_APS_IB_ID_SECURITY_TIMEOUT_PERIOD, /* 0xac (uint16_t) */

    /* Trust center policy table bitmask (type = uint32_t, enum ZbApsmePolicyT).
     * Represents the following AIBs:
     *      allowJoins = 0xad
     *      useWhiteList = 0xae
     *      allowInstallCodes = 0xaf
     *      updateTrustCenterLinkKeysRequired = 0xb3
     *      allowRejoins = 0xb6
     *      allowTrustCenterLinkKeyRequests = 0xb7
     *      networkKeyUpdateMethod = 0xba
     *      allowApplicationKeyRequests = 0xbb
     *      allowRemoteTcPolicyChange = 0xbd */
    ZB_APS_IB_ID_TRUST_CENTER_POLICY, /* 0xad (type = uint32_t, enum ZbApsmePolicyT) */

    /*** Exegin extensions (0x500 to 0x5ff reserved for custom AIBs) ***/
    ZB_APS_IB_ID_SCAN_COUNT = 0x0500, /* (uint8_t) ZDO join parameter. Is not modified by ZbApsReset. */
    ZB_APS_IB_ID_LEAVE_REMOVE_CHILDREN, /* (uint8_t) ZDO leave parameter */
    /* apsPreconfiguredLinkKey -- Preconfigured Trust Center Link Key */
    ZB_APS_IB_ID_PRECONFIGURED_LINK_KEY, /* (uint8_t[ZB_SEC_KEYSIZE]) */
    /* apsDistributedGlobalKey */
    ZB_APS_IB_ID_DISTRIBUTED_GLOBAL_KEY, /* (uint8_t[ZB_SEC_KEYSIZE]) */
    /* KeyUpdatePeriod from the trust center policy table. */
    ZB_APS_IB_ID_KEY_UPDATE_PERIOD, /* (uint32_t) */
    ZB_APS_IB_ID_MANUFACTURER_ID, /* (uint16_t) Manufacturer ID */
    ZB_APS_IB_ID_SEND_PKT_COOLDOWN, /* (uint16_t) milliseconds */
    ZB_APS_IB_ID_BIND_ADDR_RESOLVE_PERIOD, /* (uint16_t) seconds, 0 = disabled */
    ZB_APS_IB_ID_FRAGMENTATION_THRESH /* apsFragmentationThresh */
};

/* APSME-GET.request */
typedef struct {
    enum ZbApsmeIbAttrIdT attrId;
    void *attr;
    unsigned int attrLength;
    unsigned int attrIndex;
} ZbApsmeGetReqT;

/* APSME-GET.confirm */
typedef struct {
    enum ZbStatusCodeT status;
    enum ZbApsmeIbAttrIdT attrId;
} ZbApsmeGetConfT;

/* APSME-SET.request */
typedef struct {
    enum ZbApsmeIbAttrIdT attrId;
    const void *attr;
    unsigned int attrLength;
    unsigned int attrIndex;
} ZbApsmeSetReqT;

/* APSME-SET.confirm */
typedef struct {
    enum ZbStatusCodeT status;
    enum ZbApsmeIbAttrIdT attrId;
} ZbApsmeSetConfT;

/* APSME-BIND.request */
typedef struct {
    uint64_t srcExtAddr;
    uint8_t srcEndpt;
    uint16_t clusterId;
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst;
} ZbApsmeBindReqT;

/* APSME-BIND.confirm */
typedef struct {
    enum ZbStatusCodeT status;
    uint64_t srcExtAddr;
    uint8_t srcEndpt;
    uint16_t clusterId;
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst;
} ZbApsmeBindConfT;

/* APSME-UNBIND.request */
typedef struct {
    uint64_t srcExtAddr;
    uint8_t srcEndpt;
    uint16_t clusterId;
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst;
} ZbApsmeUnbindReqT;

/* APSME-UNBIND.confirm */
typedef struct {
    enum ZbStatusCodeT status;
    uint64_t srcExtAddr;
    uint8_t srcEndpt;
    uint16_t clusterId;
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst;
} ZbApsmeUnbindConfT;

/* APSME-ADD-GROUP.request */
typedef struct {
    uint16_t groupAddr;
    uint8_t endpt;
} ZbApsmeAddGroupReqT;

/* APSME-ADD-GROUP.confirm */
typedef struct {
    enum ZbStatusCodeT status;
    uint16_t groupAddr;
    uint8_t endpt;
} ZbApsmeAddGroupConfT;

/* APSME-REMOVE-GROUP.request */
typedef struct {
    uint16_t groupAddr;
    uint8_t endpt;
} ZbApsmeRemoveGroupReqT;

/* APSME-REMOVE-GROUP.confirm */
typedef struct {
    enum ZbStatusCodeT status;
    uint16_t groupAddr;
    uint8_t endpt;
} ZbApsmeRemoveGroupConfT;

/* APSME-REMOVE-ALL-GROUPS.request */
typedef struct {
    uint8_t endpt;
} ZbApsmeRemoveAllGroupsReqT;

/* APSME-REMOVE-ALL-GROUPS.confirm */
typedef struct {
    enum ZbStatusCodeT status;
    uint8_t endpt;
} ZbApsmeRemoveAllGroupsConfT;

/* APSME-ADD-ENDPOINT.request - Exegin Custom */
typedef struct {
    /* Application Information */
    uint8_t endpoint;
    uint16_t profileId;
    uint16_t deviceId;
    uint8_t version;
    uint8_t inputClusterCount; /* Supported Server Clusters */
    const uint16_t *inputClusterList;
    uint8_t outputClusterCount; /* Supported Client Clusters */
    const uint16_t *outputClusterList;
    uint16_t bdbCommissioningGroupID; /* e.g. DEFAULT_EP_BDB_COMMISSION_GRP_ID; */
} ZbApsmeAddEndpointReqT;

/* APSME-ADD-ENDPOINT.confirm - Exegin Custom */
typedef struct {
    enum ZbStatusCodeT status;
} ZbApsmeAddEndpointConfT;

/* APSME-REMOVE-ENDPOINT.request - Exegin Custom */
typedef struct {
    uint8_t endpoint;
} ZbApsmeRemoveEndpointReqT;

/* APSME-REMOVE-ENDPOINT.confirm - Exegin Custom */
typedef struct {
    enum ZbStatusCodeT status;
} ZbApsmeRemoveEndpointConfT;

/* APS Binding Table entry. */
typedef struct {
    /* Source */
    uint64_t srcExtAddr; /* if zero, entry is invalid */
    uint8_t srcEndpt;
    /* Cluster */
    uint16_t clusterId;
    /* Destination */
    /* dst.panId not used, dst.nwkAddr is only for the group address,
     * and dst.endpoint only used if mode == ZB_APSDE_ADDRMODE_EXT */
    struct ZbApsAddrT dst;
} ZbApsmeBindT;

/* APS Group ID Table entry. */
typedef struct {
    uint16_t groupAddr;
    uint8_t endpoint;
} ZbApsmeGroupT;

void ZbApsmeGetReq(struct ZigBeeT *zb, ZbApsmeGetReqT *getReqPtr, ZbApsmeGetConfT *getConfPtr);
void ZbApsmeSetReq(struct ZigBeeT *zb, ZbApsmeSetReqT *setReqPtr, ZbApsmeSetConfT *setConfPtr);
void ZbApsmeBindReq(struct ZigBeeT *zb, ZbApsmeBindReqT *bindReqPtr, ZbApsmeBindConfT *bindConfPtr);
void ZbApsmeUnbindReq(struct ZigBeeT *zb, ZbApsmeUnbindReqT *unbindReqPtr, ZbApsmeUnbindConfT *unbindConfPtr);
void ZbApsUnbindAllReq(struct ZigBeeT *zb);
void ZbApsmeAddGroupReq(struct ZigBeeT *zb, ZbApsmeAddGroupReqT *r, ZbApsmeAddGroupConfT *c);
void ZbApsmeRemoveGroupReq(struct ZigBeeT *zb, ZbApsmeRemoveGroupReqT *r, ZbApsmeRemoveGroupConfT *c);
void ZbApsmeRemoveAllGroupsReq(struct ZigBeeT *zb, ZbApsmeRemoveAllGroupsReqT *r, ZbApsmeRemoveAllGroupsConfT *c);

uint8_t ZbApsGroupsGetCapacity(struct ZigBeeT *zb);
uint8_t ZbApsGroupsGetMembership(struct ZigBeeT *zb, uint8_t endpoint, uint16_t *group_list,
    uint8_t max_len);

void ZbApsmeAddEndpoint(struct ZigBeeT *zb, ZbApsmeAddEndpointReqT *r, ZbApsmeAddEndpointConfT *c);
void ZbApsmeRemoveEndpoint(struct ZigBeeT *zb, ZbApsmeRemoveEndpointReqT *r, ZbApsmeRemoveEndpointConfT *c);

/* Attach a callback to receive all APS messages for this endpoint that have
 * not matched any other filter rules. */
bool ZbApsmeEndpointConfigNoMatchCallback(struct ZigBeeT *zb, uint8_t endpoint,
    int (*callback)(ZbApsdeDataIndT *ind, void *cbarg), void *arg);

/* Add a cluster ID to the input cluster list of an existing endpoint */
bool ZbApsmeEndpointClusterListAppend(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t cluster_id, bool is_input);
bool ZbApsmeEndpointClusterListRemove(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t cluster_id, bool is_input);
bool ZbApsmeEndpointClusterPresent(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t cluster_id, bool is_input);

uint8_t ZbApsmeEndpointClusterListInputNum(struct ZigBeeT *zb, uint8_t endpoint);
uint8_t ZbApsmeEndpointClusterListOutputNum(struct ZigBeeT *zb, uint8_t endpoint);

/* Check if an endpoint already exists */
bool ZbApsEndpointExists(struct ZigBeeT *zb, uint8_t endpoint);
/* Get the Profile ID for this endpoint */
uint16_t ZbApsEndpointProfile(struct ZigBeeT *zb, uint8_t endpoint);

/*---------------------------------------------------------------
 * APSME - Security
 *---------------------------------------------------------------
 */
/* Device status values for APSME-UPDATE-DEVICE.request and indication. */
enum ZbApsmeDeviceStatusT {
    ZB_APSME_DEV_STD_SECURE_REJOIN = 0x00, /* 0x00 */
    ZB_APSME_DEV_STD_INSECURE_JOIN, /* 0x01 */
    ZB_APSME_DEV_LEFT, /* 0x02 */
    ZB_APSME_DEV_STD_INSECURE_REJOIN, /* 0x03 */
    ZB_APSME_DEV_HIGH_SECURE_REJOIN, /* HIGHSEC SECURE REJOIN 0x04 */
    ZB_APSME_DEV_HIGH_INSECURE_JOIN, /* HIGHSEC JOIN 0x05 */
    /* Discontinuity, 0x06 */
    ZB_APSME_DEV_HIGH_INSECURE_REJOIN = 0x07 /* HIGHSEC INSECURE REJOIN 0x07 */
};

/* EXEGIN - change the these values so the low nibble represents an
 * enumeration, and the high nibble is a mask representing the key type
 * (e.g. global, unique, unverified, derived, etc)
 *
 * Key attributes set in the device key pair set (bitmask)
 *
 * Verified keys are the default.
 *
 * Warning, the bitmask here does not match the ZigBee Spec
 * (R21: Table 4.29)*/

/* Unique Link Key (e.g. Install Code Generated Key) */
#define ZB_APSME_KEY_ATTR_VERIFIED          0x00U

/* Shared key (value stored in apsPreconfiguredLinkKey). */
/* Global Link Key */
#define ZB_APSME_KEY_ATTR_SHARED            0x01U

#if 0 /* provisional key is not used by stack. */
#define ZB_APSME_KEY_ATTR_PROVISIONAL       ZB_APSME_KEY_ATTR_SHARED
#endif

/* An unverified TCLK derived key. We should continue to use
 * apsPreconfiguredLinkKey until it gets verified by a
 * APSME-VERIFY-KEY.indication */
#define ZB_APSME_KEY_ATTR_UNVERIFIED        0x02U

/* ZCL KE/CBKE derived key. */
/* Unique Link Key */
#define ZB_APSME_KEY_ATTR_CBKE              0x04U

/* TCLK update derived key. Set when we receive a APSME-REQUEST-KEY.indication
 * with key-type set to TCLK. ZB_APSME_KEY_ATTR_UNVERIFIED is set at the
 * same time. */
#define ZB_APSME_KEY_ATTR_TCLK_DERIVED      0x08U

/* Distributed Security Global Key (value stored in apsDistributedGlobalKey) */
/* Global Link Key */
#define ZB_APSME_KEY_ATTR_DISTRIBUTED       0x10U

/* Trust Center Policy Flags (ZB_APS_IB_ID_TRUST_CENTER_POLICY)
 * See R22 section 4.7.3 Trust Center Policy Values */
enum ZbApsmePolicyT {
    /* allowJoins */
    ZB_APSME_POLICY_ALLOW_JOIN = 0x00000001,
    /* useWhiteList */
    ZB_APSME_POLICY_WHITELIST = 0x00000002,

    /* allowInstallCodes */
    ZB_APSME_POLICY_IC_MASK = 0x0000000C,
    /* 0x00 (do not support Install Codes) */
    ZB_APSME_POLICY_IC_NOT_SUPPORTED = 0x00000000,
    /* 0x01 (support but do not require Install Codes) */
    ZB_APSME_POLICY_IC_SUPPORTED = 0x00000004,
    /* 0x02 (require the use of Install Codes) */
    ZB_APSME_POLICY_IC_REQUIRED = 0x00000008,

    /* updateTrustCenterLinkKeysRequired */
    ZB_APSME_POLICY_TCLK_UPDATE_REQUIRED = 0x00000010,

    /* allowRejoins - "This value indicates if the trust center allows rejoins using well known
     * or default keys. A setting of FALSE means rejoins are only allowed with trust center
     * link keys where the KeyAttributes of the apsDeviceKeyPairSet entry indicates VERIFIED_KEY." */
    ZB_APSME_POLICY_ALLOW_REJOIN = 0x00000020,

    /* allowTrustCenterLinkKeyRequests */
    ZB_APSME_POLICY_TC_KEY_REQ_MASK = 0x000000C0,
    /* 0x01 (any) */
    ZB_APSME_POLICY_TC_KEY_REQ_ANY = 0x00000040,
    /* 0x02 (provisional) */
    ZB_APSME_POLICY_TC_KEY_REQ_PROV = 0x00000080,

#if 0 /* not used and has no effect in stack */
      /* networkKeyUpdateMethod */
    ZB_APSME_POLICY_NWK_KEY_UNICAST = 0x00000100,
#endif

    /* allowApplicationKeyRequests */
    ZB_APSME_POLICY_APP_KEY_REQ_MASK = 0x00000C00,
    /* 0x00 (never) */
    ZB_APSME_POLICY_APP_KEY_REQ_NEVER = ZB_APSME_POLICY_IC_NOT_SUPPORTED,
    /* 0x01 (any) */
    ZB_APSME_POLICY_APP_KEY_REQ_ANY = 0x00000400,
    /* 0x02 (provisional) */
    /* Provisional requires "applicationKeyRequestList" to be implemented. */
    ZB_APSME_POLICY_APP_KEY_REQ_PROV = 0x00000800,

    /* allowRemoteTcPolicyChange */
    ZB_APSME_POLICY_TC_POLICY_CHANGE = 0x00001000,

    /* For certification testing only */
    /* Authenticate using dummy keys only (preconfigured NWK key in use). */
    ZB_APSME_POLICY_DUMMY_KEY = 0x10000000
};

/* APSME-TRANSPORT-KEY.request */
typedef struct {
    /*
     * Under normal stack operation, destination address mode must be
     * ZB_APSDE_ADDRMODE_EXT. If dstAddrMode == ZB_APSDE_ADDRMODE_EXT
     * and dstExtAddr == 0, the transport key command will be sent to the
     * 0xffff broadcast address.
     *
     * For ZigBee Pro testing, we need to have finer control over the
     * broadcast address.  For test 14.24, we need to be able to
     * specify 0xffff as well as 0xfffd (rx-on only).
     */
    enum ZbApsAddrModeT dstAddrMode;
    /*lint -e658 [ !MISRA - Anonymous union assumed. ] */
    union {
        uint16_t dstNwkAddr;
        uint64_t dstExtAddr;
    };
    enum ZbSecKeyTypeT keyType;
    union {
        struct {
            uint64_t parentAddr;
            uint8_t key[ZB_SEC_KEYSIZE];
        } trustKey;
        struct {
            uint8_t keySeqNumber;
            uint8_t key[ZB_SEC_KEYSIZE];
            bool useParent;
            uint64_t parentAddr;
        } nwkKey;
        struct {
            uint64_t partnerAddr;
            bool initiator;
            uint8_t key[ZB_SEC_KEYSIZE];
        } linkKey;
    };
    /* Anonymous Union (keyData) */
} ZbApsmeTransportKeyReqT;

/* APSME-TRANSPORT-KEY.indication */
typedef struct {
    uint64_t srcExtAddr;
    enum ZbSecKeyTypeT keyType;
    union {
        struct {
            uint8_t key[ZB_SEC_KEYSIZE];
        } trustKey;
        struct {
            uint8_t keySeqNumber;
            uint8_t key[ZB_SEC_KEYSIZE];
        } nwkKey;
        struct {
            uint64_t partnerAddr;
            bool initiator; /* initiator field is missing from spec. */
            uint8_t key[ZB_SEC_KEYSIZE];
        } linkKey;
    };
    /*lint -restore */
    enum ZbSecEncryptT encryptKeyType; /* key used to encrypt the incoming command. */
} ZbApsmeTransKeyIndT;

/* APSME-UPDATE-DEVICE.request */
typedef struct {
    uint64_t dstExtAddr;
    uint64_t devExtAddr;
    enum ZbApsmeDeviceStatusT status;
    uint16_t devShortAddr;
} ZbApsmeUpdateDeviceReqT;

/* APSME-UPDATE-DEVICE.indication */
typedef struct {
    uint64_t srcExtAddr;
    uint64_t devExtAddr;
    uint16_t devShortAddr;
    enum ZbApsmeDeviceStatusT status;
    enum ZbSecEncryptT encryptKeyType; /* key used to encrypt the incoming command. */
} ZbApsmeUpdateDeviceIndT;

/* APSME-REMOVE-DEVICE.request */
typedef struct {
    uint64_t dstExtAddr;
    uint64_t targetAddr;
} ZbApsmeRemoveDeviceReqT;

/* APSME-REMOVE-DEVICE.indication */
typedef struct {
    uint64_t srcExtAddr;
    uint64_t targetAddr;
    enum ZbSecEncryptT encryptKeyType; /* key used to encrypt the incoming command. */
} ZbApsmeRemoveDeviceIndT;

/* R22 Table 4-19 RequestKeyType Values */
/* This table is slightly different than the values from enum ZbSecKeyTypeT, which all the other commands use. */
enum ZbApsRequestKeyTypeT {
    ZB_APS_REQKEY_KEYTYPE_APP_LINK = 0x02,
    ZB_APS_REQKEY_KEYTYPE_TC_LINK = 0x04
};

/* APSME-REQUEST-KEY.request */
typedef struct {
    uint64_t dstExtAddr;
    enum ZbApsRequestKeyTypeT keyType;
    uint64_t partnerAddr; /* only used with application keys. */
} ZbApsmeRequestKeyReqT;

/* APSME-REQUEST-KEY.indication */
typedef struct {
    uint64_t srcExtAddr;
    enum ZbApsRequestKeyTypeT keyType;
    uint64_t partnerAddr;
    enum ZbSecEncryptT encryptKeyType; /* key used to encrypt the incoming command. */
} ZbApsmeRequestKeyIndT;

/* APSME-SWITCH-KEY.request */
typedef struct {
    /*
     * Under normal stack operation, destination address mode must be
     * ZB_APSDE_ADDRMODE_EXT. If dstAddrMode == ZB_APSDE_ADDRMODE_EXT
     * and dstExtAddr == 0, the switch key command will be sent to the
     * 0xffff broadcast address.
     *
     * For ZigBee Pro testing, we need to have finer control over the
     * broadcast address.  For test 14.24, we need to be able to
     * specify 0xffff as well as 0xfffd (rx-on only).
     */
    enum ZbApsAddrModeT dstAddrMode;
    /*lint -e658 [ !MISRA - Anonymous union assumed. ] */
    union {
        uint16_t dstNwkAddr;
        uint64_t dstExtAddr;
    };
    /*lint -restore */
    uint8_t keySeqNum;
} ZbApsmeSwitchKeyReqT;

/* APSME-SWITCH-KEY.indication */
typedef struct {
    uint64_t srcExtAddr;
    uint8_t keySeqNum;
    enum ZbSecEncryptT encryptKeyType; /* key used to encrypt the incoming command. */
} ZbApsmeSwitchKeyIndT;

/* APSME-VERIFY-KEY.request */
typedef struct {
    uint64_t dstExtAddr;
    enum ZbSecKeyTypeT keyType;
} ZbApsmeVerifyKeyReqT;

/* APSME-VERIFY-KEY.indication */
typedef struct {
    uint64_t srcExtAddr;
    enum ZbSecKeyTypeT keyType;
    uint8_t hash[ZB_SEC_KEYSIZE];
    enum ZbSecEncryptT encryptKeyType; /* key used to encrypt the incoming command. */
} ZbApsmeVerifyKeyIndT;

/* APSME-CONFIRM-KEY.request */
typedef struct {
    enum ZbStatusCodeT status;
    uint64_t dstExtAddr;
    enum ZbSecKeyTypeT keyType;
} ZbApsmeConfirmKeyReqT;

/* APSME-CONFIRM-KEY.indication */
typedef struct {
    enum ZbStatusCodeT status;
    uint64_t srcExtAddr;
    enum ZbSecKeyTypeT keyType;
    enum ZbSecEncryptT encryptKeyType; /* key used to encrypt the incoming command. */
} ZbApsmeConfirmKeyIndT;

/* APSME-ADD-KEY.request - Exegin Custom */
typedef struct {
    enum ZbSecKeyTypeT keyType;
    uint8_t key[ZB_SEC_KEYSIZE];
    uint8_t keyAttribute;
    uint8_t keySeqNumber;
    uint64_t partnerAddr;
    uint32_t outgoingCounter; /* For link keys only. */
    uint32_t incomingCounter; /* For link keys only. */
} ZbApsmeAddKeyReqT;

/* APSME-ADD-KEY.confirm - Exegin Custom */
typedef struct {
    enum ZbStatusCodeT status;
} ZbApsmeAddKeyConfT;

/*STRUCTURE:-----------------------------------------------------
 *  NAME        ZbApsmeGetKeyReqT
 *  DESC        Exegin Custom
 *  PARAMS
 *      keyType         ;
 *      keySeqNumber    ; Only used with ZB_SEC_KEYTYPE_STANDARD_NWK
 *                        The active network key's sequence number can be retrieved from
 *                        the NIB attribute ZB_NWK_NIB_ID_ActiveKeySeqNumber.
 *      partnerAddr     ;
 *---------------------------------------------------------------
 */
typedef struct {
    enum ZbSecKeyTypeT keyType;
    uint8_t keySeqNumber;
    uint64_t partnerAddr;
} ZbApsmeGetKeyReqT;

/* APSME-GET-KEY.confirm - Exegin Custom */
typedef struct {
    enum ZbStatusCodeT status;
    uint8_t key[ZB_SEC_KEYSIZE];
    uint32_t outgoingCounter;
    uint32_t incomingCounter;
    uint8_t apsKeyAttribute; /* Only valid if req.keyType != ZB_SEC_KEYTYPE_STANDARD_NWK */
} ZbApsmeGetKeyConfT;

/* APSME-REMOVE-KEY.request - Exegin Custom */
typedef struct {
    enum ZbSecKeyTypeT keyType;
    uint64_t partnerAddr;
} ZbApsmeRemoveKeyReqT;

/* APSME-REMOVE-KEY.confirm - Exegin Custom */
typedef struct {
    enum ZbStatusCodeT status;
} ZbApsmeRemoveKeyConfT;

/* APS Key/Pair Descriptor - used with ZB_APS_IB_ID_DEVICE_KEY_PAIR_SET */
typedef struct {
    uint64_t deviceAddress;
    uint8_t linkKey[ZB_SEC_KEYSIZE];
    uint32_t outgoingFrameCounter;
    uint32_t incomingFrameCounter;
    uint32_t persistOutFrameCounter;
    uint32_t persistInFrameCounter;
    uint8_t keyAttribute;
    ZbUptimeT cooldown;
} ZbApsmeKeyPairT;

void ZbApsmeTransportKeyReq(struct ZigBeeT *zb, ZbApsmeTransportKeyReqT *req);
enum ZbSecHdrKeyIdT ZbApsmeUpdateDeviceReq(struct ZigBeeT *zb, ZbApsmeUpdateDeviceReqT *req);
void ZbApsmeRemoveDeviceReq(struct ZigBeeT *zb, ZbApsmeRemoveDeviceReqT *req);
enum ZbStatusCodeT ZbApsmeRequestKeyReq(struct ZigBeeT *zb, ZbApsmeRequestKeyReqT *req);
void ZbApsmeSwitchKeyReq(struct ZigBeeT *zb, ZbApsmeSwitchKeyReqT *req);
void ZbApsmeVerifyKeyReq(struct ZigBeeT *zb, ZbApsmeVerifyKeyReqT *req);
void ZbApsmeConfirmKeyReq(struct ZigBeeT *zb, ZbApsmeConfirmKeyReqT *req);
void ZbApsmeAddKeyReq(struct ZigBeeT *zb, ZbApsmeAddKeyReqT *req, ZbApsmeAddKeyConfT *conf);
/* For ZbApsmeGetKeyReq, key types ZB_SEC_KEYTYPE_TC_LINK and ZB_SEC_KEYTYPE_APP_LINK are treated
 * the same when searching, so  you can use either to get the link key for the requested EUI. */
void ZbApsmeGetKeyReq(struct ZigBeeT *zb, ZbApsmeGetKeyReqT *req, ZbApsmeGetKeyConfT *conf);
void ZbApsmeRemoveKeyReq(struct ZigBeeT *zb, ZbApsmeRemoveKeyReqT *req, ZbApsmeRemoveKeyConfT *conf);

/*---------------------------------------------------------------
 * Filter API
 *---------------------------------------------------------------
 */
#define ZB_APS_FILTER_RULES_MAX     16U

enum ZbApsFilterTypeT {
    ZB_APS_FILTER_TYPE_NORMAL = 0, /* handle normally, call callback on match. */
    ZB_APS_FILTER_TYPE_DROP, /* drop packet and don't process further. */
    ZB_APS_FILTER_TYPE_NO_MATCH /* call if there were no other matches for this packet */
};

/* Packet filter return values. Only used if type = ZB_APS_FILTER_TYPE_NORMAL */
#define ZB_APS_FILTER_CONTINUE              0 /* Continue processing any further filter callbacks. */
#define ZB_APS_FILTER_DISCARD               1 /* Stop processing further filter callbacks. */

/* APS packet filter entry. */
struct ZbApsFilterT {
    struct LinkListT link;
    /* Packet handler actions */
    enum ZbApsFilterTypeT type;
    int (*callback)(ZbApsdeDataIndT *dataInd, void *cb_arg);
    void *arg;
    /* Packet filter rules */
    uint32_t r[ZB_APS_FILTER_RULES_MAX];
};

/* Create an APS indication filter and callback for an endpoint, with no specific cluster being filtered. */
struct ZbApsFilterT * ZbApsFilterEndpointAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId,
    int (*callback)(ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg);
/* Create an APS indication filter and callback for an endpoint and a specific cluster ID. */
struct ZbApsFilterT * ZbApsFilterClusterAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t clusterId, uint16_t profileId,
    int (*callback)(ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg);
void ZbApsFilterClusterFree(struct ZigBeeT *zb, struct ZbApsFilterT *filter);

/*---------------------------------------------------------------
 * Helper Functions
 *---------------------------------------------------------------
 */
/* ZbApsGet and ZbApsSet return the confirm status byte (SUCCESS == 0x00) */
#define     ZbApsGet(_zb_, _id_, _ptr_, _sz_) ZbApsGetIndex(_zb_, _id_, _ptr_, _sz_, 0)
#define     ZbApsSet(_zb_, _id_, _ptr_, _sz_) ZbApsSetIndex(_zb_, _id_, _ptr_, _sz_, 0)
enum ZbStatusCodeT ZbApsGetIndex(struct ZigBeeT *zb, enum ZbApsmeIbAttrIdT attrId, void *attrPtr, unsigned int attrSz, unsigned int attrIndex);
enum ZbStatusCodeT ZbApsSetIndex(struct ZigBeeT *zb, enum ZbApsmeIbAttrIdT attrId, const void *attrPtr, unsigned int attrSz, unsigned int attrIndex);

bool ZbApsGroupIsMember(struct ZigBeeT *zb, uint16_t groupAddr, uint8_t endpoint);

bool ZbApsLinkKeyExists(struct ZigBeeT *zb, uint64_t partner);
ZbApsmeKeyPairT * ZbApsLookupKey(struct ZigBeeT *zb, ZbApsmeKeyPairT *key,
    uint64_t addr, unsigned int *idx);

bool ZbApsAddrIsBcast(struct ZbApsAddrT *addr);
bool ZbApsAddrIsLocal(struct ZigBeeT *zb, struct ZbApsAddrT *addr);

bool ZbApsCommandSecurityCheck(struct ZigBeeT *zb, enum ZbApsCmdIdT cmdId, uint64_t srcExtAddr, enum ZbSecEncryptT encryptKeyType);

/* Helpers for certification testing (TP/PRO/BV-43 and TP/PRO/BV-44) */
bool ZbApsFragDropTxAdd(struct ZigBeeT *zb, uint8_t blockNum);
void ZbApsFragDropTxClear(struct ZigBeeT *zb);

#endif /* ZIGBEE_APS_H */
