/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZIGBEE_STARTUP_H
# define ZIGBEE_STARTUP_H

#include "zcl/zcl.touchlink.h"

/* Startup control-codes as per the ZCL. */
enum ZbStartType {
    ZbStartTypePreconfigured = 0x00,
    ZbStartTypeForm = 0x01,
    ZbStartTypeRejoin = 0x02,
    ZbStartTypeJoin = 0x03,
    /* Exegin add-ons (Internal use only) */
    ZbStartTypeTouchlink = 0xfd,
    ZbStartTypeFindBind = 0xfe,
    ZbStartTypeNone = 0xff
};

struct ZbStartupCbkeT {
    uint8_t endpoint; /* e.g. ZB_ENDPOINT_MAX */
    uint16_t deviceId; /* e.g. ZCL_DEVICE_METER */
    uint16_t suite_mask; /* e.g. ZCL_KEY_SUITE_CBKE_ECMQV */
    struct ZbZclCbkeInfoT cbke_v1; /* if (suite & ZCL_KEY_SUITE_CBKE_ECMQV) */
    struct ZbZclCbke2InfoT cbke_v2; /* if (suite & ZCL_KEY_SUITE_CBKE2_ECMQV) */

    /* Keep Alive Server or Client */
    bool tc_keepalive_server_enable; /* true for server, false for client */
    /* Keep Alive Server attributes */
    uint8_t tc_keepalive_base; /* minutes. If zero, use stack defaults for BASE and JITTER. */
    uint16_t tc_keepalive_jitter; /* seconds */

    /* Trust Center Swap Out status callback, so application is informed of any TCSO process
     * initiated by the Keep Alive Client cluster. */
    void (*tcso_callback)(enum ZbTcsoStatusT status, void *arg);
    void *tcso_arg;
};

/* The set of parameters to form or join a ZigBee Network Stack.
 * Should be initialized using ZbStartupConfigGetProDefaults or ZbStartupConfigGetProSeDefaults. */
struct ZbStartupT {
    /* shortAddress is only used if:
     * startupControl = ZbStartTypePreconfigured or ZbStartTypeRejoin */
    uint16_t shortAddress;
    /* panId and networkManagerAddress are only used if:
     * startupControl = ZbStartTypePreconfigured */
    uint16_t panId;
    uint16_t networkManagerAddress;
    /* Extended PAN ID. See note1 below. */
    uint64_t extendedPanId;

    /* Specify the channel masks to use. If no channel masks are specified,
     * ZB_BDB_PrimaryChannelSet and ZB_BDB_SecondaryChannelSet are used instead. */
    struct ZbChannelListT channelList;

    /* Stack Profile.
     * If not ZB_NWK_STACK_PROFILE_PRO, application must configure the following
     * NIB parameters before calling ZbStartup:
     *      nwkMaxDepth, nwkMaxChildren, nwkReportConstantCost, nwkLinkStatusPeriod,
     *      nwkTransactionPersistenceTime, nwkPassiveAckTimeout, nwkMaxBroadcastRetries,
     *      nwkSecureAllFrames, nwkSecurityLevel */
    uint8_t stackProfile;

    /* BDB Commissioning Mode - see BDB_COMMISSION_MODE_MASK */
    uint8_t bdbCommissioningMode;

    /* Startup Type (used if not BDB_COMMISSION_MODE_TOUCHLINK)
     * Notes:
     *  ZbStartTypePreconfigured    - extendedPanId must be provided.
     *  ZbStartTypeForm             - if extendedPanId = 0 then the stack will use the EUI as the Extended PAN ID.
     *  ZbStartTypeRejoin           - extendedPanId must be provided.
     *  ZbStartTypeJoin             - if extendedPanId = 0 then the stack will try join any available network.
     */
    enum ZbStartType startupControl;

    /* Security Parameters (security level, preconfigured keys, etc) */
    struct {
        uint8_t level;
        bool useInsecureRejoin;
        uint64_t trustCenterAddress;
        uint8_t preconfiguredLinkKey[ZB_SEC_KEYSIZE]; /* 16 bytes */
        uint8_t distributedGlobalKey[ZB_SEC_KEYSIZE]; /* 16 bytes */
        uint8_t networkKey[ZB_SEC_KEYSIZE]; /* 16 bytes */
        uint8_t networkKeySeqNum;
        enum ZbSecKeyTypeT networkKeyType;

        struct ZbStartupCbkeT cbke;
    } security;

    /* The set of parameters related to joining a network. */
    struct {
        /* scanAttempts removed, use ZB_APS_IB_ID_SCAN_COUNT instead */
        uint16_t timeBetweenScans; /* in milliseconds */
        uint16_t rejoinInterval; /* in seconds */
        uint16_t maxRejoinInterval; /* in seconds */
    } join;

    /* Device Capability (FFD vs RFD, RX-ON) */
    uint8_t capability; /* e.g. MCP_ASSOC_CAP_RXONIDLE */

    /* End-device timeout is only used by end-devices. It configures the
     * time used to periodically update the Parent device so this
     * device is not removed from the Parent's NWK Neighbor Table.
     * Configures ZB_NWK_NIB_ID_EndDeviceTimeoutDefault.
     *
     * Timeout = (60 * 2^n) seconds for n > 0. If n = 0, timeout = 10 seconds.
     * Setting to ZB_NWK_CONST_ENDDEV_TIMEOUT_DISABLED (0xff) disables end-device timeout.*/
    uint8_t endDeviceTimeout;
    uint16_t fastPollPeriod;

    /* Touchlink (requires BDB_COMMISSION_MODE_TOUCHLINK) */
    struct {
        uint8_t tl_endpoint; /* Endpoint for the Touchlink Cluster (e.g. ) */
        uint8_t bind_endpoint; /* Endpoint to use when binding clusters from Initiator to Target. */
        uint16_t deviceId; /* e.g. ZCL_DEVICE_ONOFF_SWITCH */
        uint8_t zb_info; /* e.g. ZCL_TL_ZBINFO_TYPE_ROUTER */
        uint8_t flags; /* e.g. ZCL_TL_FLAGS_IS_TARGET */
        const void *persist_buf; /* if ZCL_TL_ZBINFO_USE_PERSIST */
        unsigned int persist_len;
    } touchlink;
};

/* Non-blocking startup function */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartup(struct ZigBeeT *zb, struct ZbStartupT *configPtr, void (*callback)(enum ZbStatusCodeT status, void *cb_arg), void *arg);

/* Blocking version of ZbStartup. Not supported on all platforms. */
enum ZbStatusCodeT ZbStartupWait(struct ZigBeeT *zb, struct ZbStartupT *config);

/* If Touchlink Target was started with ZbStartup, this API can be used to stop it. */
enum ZbStatusCodeT ZbStartupTouchlinkTargetStop(struct ZigBeeT *zb);

/* Manually start Finding & Binding. F&B is also started automatically after joining
 * the network. */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartupFindBindStart(struct ZigBeeT *zb,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg);

/* Same as ZbStartupFindBindStart, but only for a single endpoint. */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartupFindBindStartEndpoint(struct ZigBeeT *zb, uint8_t endpoint,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg);

/* ZbStartupRejoin is a wrapper for ZbNlmeJoinReq(ZB_NWK_REJOIN_TYPE_NWKREJOIN).
 * Use ZbStartupRejoin instead, because the internal startup handler will restart
 * any timers we need to maintain our parent.
 *
 * Must already be connected to a network. If not on a network and want to
 * rejoin as way to connect, use ZbStartup with ZbStartTypeRejoin.*/
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartupRejoin(struct ZigBeeT *zb, void (*callback)(ZbNlmeJoinConfT *conf, void *arg), void *cbarg);

/* Blocking version. Not supported on all platforms. */
enum ZbStatusCodeT ZbStartupRejoinWait(struct ZigBeeT *zb);

/* Trust Center Rejoin - unsecured rejoin (already joined to network, but
 * missed a NWK key update). */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbTrustCenterRejoin(struct ZigBeeT *zb, void (*callback)(enum ZbStatusCodeT status, void *arg), void *cbarg);

/* Try starting stack using persistence data provided. */
enum ZbStatusCodeT ZbStartupPersist(struct ZigBeeT *zb, const void *pdata, unsigned int plen, struct ZbStartupCbkeT *cbke_config);

/* Get the default configuration for a PRO network. */
void ZbStartupConfigGetProDefaults(struct ZbStartupT *configPtr);
/* Same as ZbStartupConfigGetProDefaults, but clears the preconfigured global link keys. */
void ZbStartupConfigGetProSeDefaults(struct ZbStartupT *configPtr);

/* The application can call ZbStartupTcsoStart if it thinks it as lost communication with the
 * Trust Center. The Trust Center Swap Out process will be performed.
 * The callback status is set to ZB_STATUS_SUCCESS if the stack is operational. */
bool ZB_WARN_UNUSED ZbStartupTcsoStart(struct ZigBeeT *zb, void (*callback)(enum ZbTcsoStatusT status, void *arg), void *arg);
void ZbStartupTcsoAbort(struct ZigBeeT *zb);

#endif /* ZIGBEE_STARTUP_H */
