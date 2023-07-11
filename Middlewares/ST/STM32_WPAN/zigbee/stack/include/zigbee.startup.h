/**
 * @file zigbee.startup.h
 * @heading Startup
 * @brief Zigbee startup header file
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZIGBEE_STARTUP_H
# define ZIGBEE_STARTUP_H

#include "zcl/zcl.touchlink.h"

/** Startup control-codes as per the ZCL Spec, Commissioning Cluster's StartupControl
 * attribute in the "Startup Parameters Attribute Set" */
enum ZbStartType {
    ZbStartTypePreconfigured = 0x00, /**< (0x00) Preconfigured. No explicit form, join or rejoin to be performed. */
    ZbStartTypeForm = 0x01, /**< (0x01) Form network */
    ZbStartTypeRejoin = 0x02, /**< (0x02) Rejoin network */
    ZbStartTypeJoin = 0x03, /**< (0x03) Join network */
    /* Exegin add-ons (Internal use only) */
    ZbStartTypeTouchlink = 0xfd, /**< [Internal stack use only] Touchlink */
    ZbStartTypeFindBind = 0xfe, /**< [Internal stack use only] Finding & Binding */
    ZbStartTypeNone = 0xff /**< [Internal stack use only] None */
};

/** CBKE configuration parameters for ZbStartup. This configuration is only
 * applicable if the 'suite_mask' is non-zero. */
struct ZbStartupCbkeT {
    uint8_t endpoint;
    /**< Endpoint to assign ZCL Key Exchange cluster. Default is ZB_ENDPOINT_CBKE_DEFAULT (240) */

    uint16_t deviceId;
    /**< Device Id to assign to the endpoint created for the ZCL Key Exchange cluster.
     * Default is ZCL_DEVICE_METER */

    uint16_t suite_mask;
    /**< The Key Exchange suite bitmask. E.g. ZCL_KEY_SUITE_CBKE2_ECMQV for CBKE version 2 (cbke_v2). */

    struct ZbZclCbkeInfoT cbke_v1;
    /**< CBKE version 1 certificate and security keys configuration.
     * Only applicable if ZCL_KEY_SUITE_CBKE_ECMQV is set in suite_mask. */

    struct ZbZclCbke2InfoT cbke_v2;
    /**< CBKE version 2 certificate and security keys configuration.
     * Only applicable if ZCL_KEY_SUITE_CBKE2_ECMQV is set in suite_mask. */

    /* Keep Alive Server or Client */
    bool tc_keepalive_server_enable;
    /**< If CBKE is enabled (suite_mask != 0), this flag determines whether to allocate the
     * Trust Center Keep Alive Server (true) or Client (false). The Trust Center should
     * set this flag to true, and all other joiners should set this flag to false. */

    /* Keep Alive Server attributes */
    uint8_t tc_keepalive_base;
    /**< Trust Center Keep Alive Server 'Base' attribute value in minutes.
     * If zero, let the stack choose a default value. */
    uint16_t tc_keepalive_jitter;
    /**< Trust Center Keep Alive Server 'Jitter' attribute value in seconds.
     * If zero, let the stack choose a default value. */

    void (*tcso_callback)(enum ZbTcsoStatusT status, void *arg);
    /**< Application callback that is called for any Trust Center Swap Out (TCSO) process
     * initiated by the Keep Alive Client cluster. This includes when a TCSO has been started
     * and the final outcome of a TCSO process. */

    void *tcso_arg; /** Application callback argument for 'tcso_callback' */
};

/**
 * The set of configuration parameters used to form or join a network using ZbStartup.
 * Should be first initialized using ZbStartupConfigGetProDefaults or
 * ZbStartupConfigGetProSeDefaults.
 */
struct ZbStartupT {
    uint16_t shortAddress;
    /**< Network short address. Only applicable if startupControl is
     * ZbStartTypePreconfigured or ZbStartTypeRejoin */

    uint16_t panId;
    /**< Network PAN Id. Only applicable if startupControl is
     * ZbStartTypePreconfigured or ZbStartTypeRejoin */

    uint16_t networkManagerAddress;
    /**< Network Manager Address. Only applicable if startupControl is
     * ZbStartTypePreconfigured or ZbStartTypeRejoin */

    uint64_t extendedPanId;
    /**< Extended PAN Id.
     *
     * If startupControl is ZbStartTypeForm and extendedPanId is zero, then the device's
     * extended address (EUI) will be used.
     *
     * If startupControl is ZbStartTypeRejoin a non-zero extendedPanId must be provided.
     *
     * If startupControl is ZbStartTypeJoin, the device will only attempt to join a
     * network matching the extendedPanId. If extendedPanId is zero, then the device will
     * attempt to join any available network, sorted by LQI.
     *
     * If startupControl is ZbStartTypePreconfigured a non-zero extendedPanId must be provided.
     */

    struct ZbChannelListT channelList;
    /**< Specify the channel mask(s) to use. If no channel masks are specified,
     * ZB_BDB_PrimaryChannelSet and ZB_BDB_SecondaryChannelSet are used instead. */

    uint8_t stackProfile;
    /**< Network Stack Profile. If not ZB_NWK_STACK_PROFILE_PRO, application must
     * configure the following NIB parameters before calling ZbStartup:
     *      nwkMaxDepth, nwkMaxChildren, nwkReportConstantCost, nwkLinkStatusPeriod,
     *      nwkTransactionPersistenceTime, nwkPassiveAckTimeout, nwkMaxBroadcastRetries,
     *      nwkSecureAllFrames, nwkSecurityLevel */

    uint8_t bdbCommissioningMode;
    /**< BDB Commissioning Mode mask. Set to 0 by default.
     * If BDB_COMMISSION_MODE_TOUCHLINK is set, then Touchlink will be used.
     * If BDB_COMMISSION_MODE_FIND_BIND is set, then Finding & Binding will be used after joining.
     */

    enum ZbStartType startupControl;
    /**< Startup Type. Not applicable if BDB_COMMISSION_MODE_TOUCHLINK commissioning mode is set.
     *
     * If startup type is ZbStartTypeJoin, the ZB_APS_IB_ID_SCAN_COUNT attribute is used to
     * control the number of scans to perform while searching for a network to join. Similarly,
     * the ZB_BDB_ScanDuration attribute is used to configure the MAC scan duration to use for
     * scans during network joining and forming.
     */

    struct {
        uint8_t level; /**< Security Level. Default is 0x05. */
        bool useInsecureRejoin; /**< Configures ZB_APS_IB_ID_USE_INSECURE_JOIN. */
        uint64_t trustCenterAddress;
        /**< Configures ZB_APS_IB_ID_TRUST_CENTER_ADDRESS. If forming the network and assuming the role of
         * Trust Center, the device's extended address will be used instead, unless this parameter has
         * been explicitly set to ZB_DISTRIBUTED_TC_ADDR. */

        uint8_t preconfiguredLinkKey[ZB_SEC_KEYSIZE]; /**< Preconfigured Link Key */
        uint8_t distributedGlobalKey[ZB_SEC_KEYSIZE]; /**< Preconfigured Distributed Global Link Key */

        uint8_t networkKey[ZB_SEC_KEYSIZE];
        /**< Configures the Network Key with key type set to ZB_SEC_KEYTYPE_STANDARD_NWK.
         * Only applicable if startupControl is ZbStartTypePreconfigured. */

        uint8_t networkKeySeqNum;
        /**< Configures the Network Key Sequence Number for the Network Key.
         * Also sets ZB_NWK_NIB_ID_ActiveKeySeqNumber to this value.
         * Only applicable if startupControl is ZbStartTypePreconfigured.
         */

        enum ZbSecKeyTypeT networkKeyType; /**< Deprecated and not used. */

        struct ZbStartupCbkeT cbke; /**< CBKE certificate configuration */
    } security;

    uint8_t capability;
    /**< Device capability mask. Default value includes:
     * MCP_ASSOC_CAP_PWR_SRC (mains power),
     * MCP_ASSOC_CAP_RXONIDLE (radio receiver is on, not sleepy),
     * MCP_ASSOC_CAP_ALLOC_ADDR (parent allocates network address),
     * MCP_ASSOC_CAP_DEV_TYPE (full function device) */

    uint8_t endDeviceTimeout;
    /**< End-device timeout is only used by end-devices. It configures the
     * time used to periodically update the Parent device so this
     * device is not removed from the Parent's NWK Neighbor Table.
     * Configures ZB_NWK_NIB_ID_EndDeviceTimeoutDefault.
     *
     * Timeout = (60 * 2^n) seconds for n > 0. If n = 0, timeout = 10 seconds.
     * Setting to ZB_NWK_CONST_ENDDEV_TIMEOUT_DISABLED (0xff) disables end-device timeout.*/

    uint16_t fastPollPeriod; /**< Configures ZB_NWK_NIB_ID_FastPollPeriod. */

    struct {
        uint8_t tl_endpoint; /**< Endpoint for the Touchlink Cluster (e.g. ) */
        uint8_t bind_endpoint; /**< Endpoint to use when binding clusters from Initiator to Target. */
        uint16_t deviceId; /**< e.g. ZCL_DEVICE_ONOFF_SWITCH */
        uint8_t zb_info; /**< e.g. ZCL_TL_ZBINFO_TYPE_ROUTER */
        uint8_t flags; /**< e.g. ZCL_TL_FLAGS_IS_TARGET */
        const void *persist_buf;
        /**< Pointer to persistence data. Only applicable if ZCL_TL_ZBINFO_USE_PERSIST
         * flag is set in zb_info. */
        unsigned int persist_len; /**< Length of persistence data. */
        struct ZbTouchlinkCallbacks callbacks;
        /**< Callback functions for Touchlink Controller Device Utility commands. */
        void *cb_arg; /**< Pointer to application data that will be used with Touchlink callbacks. */
    } touchlink; /**< Touchlink configuration. Only applicable if BDB_COMMISSION_MODE_TOUCHLINK. */
};

/**
 * Executes the startup procedure as described in section 2.5.5.5.6.2 of [R1].
 *
 * The startup code also handles callbacks from the stack to maintain the network
 * and security processes. For example, these include the handling of
 * APSME-UPDATE-DEVICE.indication messages when acting as a Coordinator Trust Center,
 * to handle the authentication procedure described in section 4.6.3.2 of Zigbee R22.
 *
 * The message callback  handlers can be overridden by the application by creating
 * message filters using ZbMsgFilterRegister.
 *
 * This function and the entire startup code can be bypassed by the application, and
 * the Network Layer APIs used directly, if so desired.
 *
 * @param zb Zigbee stack instance
 * @param configPtr Pointer to ZbStartupT configuration structure.
 * @param callback Application callback that is called with the final result
 * of the startup procedure.
 * @param arg Callback argument
 * @return Zigbee Status Code whether the startup procedure has been started.
 * If ZB_STATUS_SUCCESS, then the callback will be called with the final result.
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartup(struct ZigBeeT *zb, struct ZbStartupT *configPtr,
    void (*callback)(enum ZbStatusCodeT status, void *cb_arg), void *arg);

/**
 * If Touchlink Target was started with ZbStartup, this API can be used to stop it.
 * @param zb Zigbee stack instance
 * @return Zigbee Status Code whether Touchlink was successfully stopped or not.
 */
enum ZbStatusCodeT ZbStartupTouchlinkTargetStop(struct ZigBeeT *zb);

/**
 * Manually start Finding & Binding. F&B is also started automatically
 * after joining the network.
 * @param zb Zigbee stack instance
 * @param callback Application callback to call after Finding & Binding complete
 * @param arg Application callback argument
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartupFindBindStart(struct ZigBeeT *zb,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg);

/**
 * Same as ZbStartupFindBindStart, but only for a single endpoint.
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint to perform Finding & Binding from.
 * @param callback Application callback to call after Finding & Binding complete
 * @param arg Application callback argument
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartupFindBindStartEndpoint(struct ZigBeeT *zb, uint8_t endpoint,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg);

/**
 * ZbStartupRejoin is a wrapper for ZbNlmeJoinReq(ZB_NWK_REJOIN_TYPE_NWKREJOIN).
 * Use ZbStartupRejoin instead, because the internal startup handler will restart
 * any timers we need to maintain our parent.
 *
 * Must already be connected to a network. If not on a network and want to
 * rejoin as way to connect, use ZbStartup with ZbStartTypeRejoin.
 * @param zb Zigbee stack instance
 * @param callback Application callback to call after rejoin is complete,
 * indicating success or failure.
 * @param cbarg Application callback argument
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbStartupRejoin(struct ZigBeeT *zb,
    void (*callback)(struct ZbNlmeJoinConfT *conf, void *arg), void *cbarg);

/**
 * Trust Center Rejoin - unsecured rejoin (already joined to network, but
 * missed a NWK key update).
 * @param zb Zigbee stack instance
 * @param callback Application callback to call after Trust Center rejoin is complete.
 * If this function returns an error, then no callback will be generated.
 * @param cbarg Application callback argument
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZB_WARN_UNUSED ZbTrustCenterRejoin(struct ZigBeeT *zb,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *cbarg);

/**
 * Try starting stack using persistence data provided.
 * @param zb Zigbee stack instance
 * @param pdata Pointer to persistent data
 * @param plen Length of persistent data
 * @param cbke_config Optional pointer to CBKE configuration data structure.
 * @param callback Application callback to call after persistence is completed.
 * If this function returns an error, then no callback will be generated.
 * @param arg Application callback argument
 * @return Zigbee Status Code
 */
enum ZbStatusCodeT ZbStartupPersist(struct ZigBeeT *zb, const void *pdata, unsigned int plen,
    struct ZbStartupCbkeT *cbke_config,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg);

/**
 * Get the default configuration for a PRO network.
 * @param configPtr Startup configuration to initialize
 * @return None
 */
void ZbStartupConfigGetProDefaults(struct ZbStartupT *configPtr);

/**
 * Same as ZbStartupConfigGetProDefaults, but clears the preconfigured
 * global link keys.
 * @param configPtr Startup configuration to initialize
 * @return None
 */
void ZbStartupConfigGetProSeDefaults(struct ZbStartupT *configPtr);

/**
 * The application can call ZbStartupTcsoStart if it thinks it as lost
 * communication with the Trust Center. The Trust Center Swap Out process
 * will be performed. The callback status is set to ZB_STATUS_SUCCESS if
 * the stack is operational.
 * @param zb Zigbee stack instance
 * @param callback Application callback that is called after TCSO is complete.
 * @param arg Application callback argument
 * @return True if TCSO was started and to wait for callback to indicate
 * completion. False if TCSO wasn't started.
 */
bool ZB_WARN_UNUSED ZbStartupTcsoStart(struct ZigBeeT *zb,
    void (*callback)(enum ZbTcsoStatusT status, void *arg), void *arg);

/**
 * Abort a TCSO, if it's running. This function should not be necessary,
 * but an application may.
 * @param zb Zigbee stack instance
 * @return True if TCSO was aborted, false otherwise (i.e. wasn't running).
 */
bool ZbStartupTcsoAbort(struct ZigBeeT *zb);

#endif /* ZIGBEE_STARTUP_H */
