/**
 * @file zcl.commission.h
 * @heading Commissioning
 * @brief ZCL Commissioning cluster header
 * ZCL 7 section 13.2
 * ZCL 8 section 13.2
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_COMMISSION_H
# define ZCL_COMMISSION_H

#include "zcl/zcl.h"

/* @PICS.ZCL.Commissioning
 * CS.S | Server | True
 * CS.C | Client | True
 *
 * Server Attributes
 * CS.S.A0000 | ShortAddress | True
 * CS.S.A0001 | ExtendedPANId | True
 * CS.S.A0002 | PANId | True
 * CS.S.A0003 | Channelmask | True
 * CS.S.A0004 | ProtocolVersion | True
 * CS.S.A0005 | StackProfile | True
 * CS.S.A0006 | StartupControl | True
 * CS.S.A0010 | TrustCenterAddress | True
 * CS.S.A0011 | TrustCenterMasterKey | False | Optional
 * CS.S.A0012 | NetworkKey | True
 * CS.S.A0013 | UseInsecureJoin | True
 * CS.S.A0014 | PreconfiguredLinkKey | True
 * CS.S.A0015 | NetworkKeySeqNum | True
 * CS.S.A0016 | NetworkKeyType | True
 * CS.S.A0017 | NetworkManagerAddress | True
 * CS.S.A0020 | ScanAttempts | True | Optional
 * CS.S.A0021 | TimeBetweenScans | False | Optional
 * CS.S.A0022 | RejoinInterval | False | Optional
 * CS.S.A0023 | MaxRejoinInterval | False | Optional
 * CS.S.A0030 | IndirectPollRate | False | Optional
 * CS.S.A0031 | ParentRetryThreshold | False | Optional
 * CS.S.A0040 | ConcentratorFlag | False | Optional
 * CS.S.A0041 | ConcentratorRadius | False | Optional
 * CS.S.A0042 | ConcentratorDiscoveryTime | False | Optional
 * CS.S.Afffd | ClusterRevision | True
 * CS.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * CS.S.C00.Rsp | Restart Device | Depends on customer application
 * CS.S.C01.Rsp | Save Startup Parameters | Depends on customer application
 * CS.S.C02.Rsp | Restore Startup Parameters | Depends on customer application
 * CS.S.C03.Rsp | Reset Startup Parameters | Depends on customer application
 *
 * Commands Generated
 * CS.S.C00.Tx | Restart Device Response | Depends on customer application
 * CS.S.C01.Tx | Save Startup Parameters Response | Depends on customer application
 * CS.S.C02.Tx | Restore Startup Parameters Response | Depends on customer application
 * CS.S.C03.Tx | Reset Startup Parameters Response | Depends on customer application
 *
 * Client Attributes
 * CS.C.Afffd | ClusterRevision | True
 * CS.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * CS.C.C00.Rsp | Restart Device Response | Depends on customer application
 * CS.C.C01.Rsp | Save Startup Parameters | Depends on customer application
 * CS.C.C02.Rsp | Restore Startup Parameters Response | Depends on customer application
 * CS.C.C03.Rsp | Reset Startup Parameters Response | Depends on customer application
 *
 * Commands Generated
 * CS.C.C00.Tx | Restart Device | True
 * CS.C.C01.Tx | Save Startup Parameters | True
 * CS.C.C02.Tx | Restore Startup Parameters | True
 * CS.C.C03.Tx | Reset Startup Parameters | True
 */

#define ZCL_COMMISSION_TXOPTIONS_UNSECURE \
    (ZB_APSDE_DATAREQ_TXOPTIONS_ACK | ZB_APSDE_DATAREQ_TXOPTIONS_FRAG)

#define ZCL_COMMISSION_TXOPTIONS_SECURE \
    (ZCL_COMMISSION_TXOPTIONS_UNSECURE | ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY)

#define ZCL_COMMISSION_RESTART_DEVICE_DELAY_MS      100U

enum ZbZclCommissionServerCommandsT {
    ZCL_COMMISSION_SVR_CMD_RESTART_DEVICE_RSP = 0x00,
    ZCL_COMMISSION_SVR_CMD_SAVE_STARTUP_RSP = 0x01,
    ZCL_COMMISSION_SVR_CMD_RESTORE_STARTUP_RSP = 0x02,
    ZCL_COMMISSION_SVR_CMD_RESET_STARTUP_RSP = 0x03
};

enum ZbZclCommissionClientCommandsT {
    ZCL_COMMISSION_CLI_CMD_RESTART_DEVICE = 0x00,
    ZCL_COMMISSION_CLI_CMD_SAVE_STARTUP = 0x01,
    ZCL_COMMISSION_CLI_CMD_RESTORE_STARTUP = 0x02,
    ZCL_COMMISSION_CLI_CMD_RESET_STARTUP = 0x03
};

/** Commissioning Server Attribute IDs */
enum ZbZclCommissionServerAttrT {
    /* Startup Parameters */
    ZCL_COMMISSION_SVR_ATTR_SHORT_ADDR = 0x0000,
    /**< ShortAddress. This should only be set if StartupControl is equal to 0x00 (ZbStartTypePreconfigured)
     * or 0x02 (ZbStartTypeRejoin). */
    ZCL_COMMISSION_SVR_ATTR_EPID = 0x0001, /**< ExtendedPANId */
    ZCL_COMMISSION_SVR_ATTR_PANID = 0x0002,
    /**< PANId. This should only be set if StartupControl is equal to 0x00 (ZbStartTypePreconfigured)
     * or 0x02 (ZbStartTypeRejoin). */
    ZCL_COMMISSION_SVR_ATTR_CHANNELMASK = 0x0003, /**< Channelmask */
    ZCL_COMMISSION_SVR_ATTR_PROTOCOLVER = 0x0004, /**< ProtocolVersion. Default value is 0x0002 (ZB_PROTOCOL_VERSION_2007) */
    ZCL_COMMISSION_SVR_ATTR_STACKPROFILE = 0x0005, /**< StackProfile. Default value is 0x02 (ZB_NWK_STACK_PROFILE_PRO) */
    ZCL_COMMISSION_SVR_ATTR_STARTUPCONTROL = 0x0006,
    /**< StartupControl.
     * ZbStartTypePreconfigured = 0x00,
     * ZbStartTypeForm = 0x01,
     * ZbStartTypeRejoin = 0x02,
     * ZbStartTypeJoin = 0x03 */
    ZCL_COMMISSION_SVR_ATTR_TCADDR = 0x0010,
    /**< TrustCenterAddress. This should only be set if StartupControl is equal to 0x00 (ZbStartTypePreconfigured).
     * Otherwise it should be zero to allow the Transport Key to be decrypted and processed correctly during joining. */
    ZCL_COMMISSION_SVR_ATTR_TCMASTER = 0x0011, /**< TrustCenterMasterKey (Optional) */
    ZCL_COMMISSION_SVR_ATTR_NWKKEY = 0x0012,
    /**< NetworkKey. This should only be set if StartupControl is equal to 0x00 (ZbStartTypePreconfigured). */
    ZCL_COMMISSION_SVR_ATTR_USEINSECJOIN = 0x0013, /**< UseInsecureJoin */
    ZCL_COMMISSION_SVR_ATTR_PRECONFLINKKEY = 0x0014, /**< PreconfiguredLinkKey */
    ZCL_COMMISSION_SVR_ATTR_NWKKEYSEQNUM = 0x0015,
    /**< NetworkKeySeqNum. This should only be set if StartupControl is equal to 0x00 (ZbStartTypePreconfigured). */
    ZCL_COMMISSION_SVR_ATTR_NWKKEYTYPE = 0x0016,
    /**< NetworkKeyType. This should only be set if StartupControl is equal to 0x00 (ZbStartTypePreconfigured). */
    ZCL_COMMISSION_SVR_ATTR_NWKMGRADDR = 0x0017,
    /**< NetworkManagerAddress. This should only be set if StartupControl is equal to 0x00 (ZbStartTypePreconfigured)
     * or 0x02 (ZbStartTypeRejoin). */

    /* Join Parameters */
    ZCL_COMMISSION_SVR_ATTR_SCANATTEMPTS = 0x0020, /**< ScanAttempts (Optional) */
    ZCL_COMMISSION_SVR_ATTR_TIMEBTWSCANS = 0x0021, /**< TimeBetweenScans (Optional) */
    ZCL_COMMISSION_SVR_ATTR_REJOININTERVAL = 0x0022, /**< RejoinInterval (Optional) */
    ZCL_COMMISSION_SVR_ATTR_MAXREJOININTERVAL = 0x0023, /**< MaxRejoinInterval (Optional) */

    /* End Device Parameters */
    ZCL_COMMISSION_SVR_ATTR_POLLRATE = 0x0030, /**< IndirectPollRate (Optional) */
    ZCL_COMMISSION_SVR_ATTR_PARENTRETRYTHRESH = 0x0031, /**< ParentRetryThreshold (Optional) */

    /* Concentrator Parameters */
    ZCL_COMMISSION_SVR_ATTR_CONCFLAG = 0x0040, /**< ConcentratorFlag (Optional) */
    ZCL_COMMISSION_SVR_ATTR_CONCRADIUS = 0x0041, /**< ConcentratorRadius (Optional) */
    ZCL_COMMISSION_SVR_ATTR_CONCDISCTIME = 0x0042 /**< ConcentratorDiscoveryTime (Optional) */
};

/* Client Commands */

/* Restart Device Options Field */
#define ZCL_COMMISS_RESTART_OPTS_MODE_MASK          0x07U
#define ZCL_COMMISS_RESTART_OPTS_MODE_USE_STARTUP   0x00U /* Current set of startup parameters */
#define ZCL_COMMISS_RESTART_OPTS_MODE_USE_STACK     0x01U /* Current state of device / stack parameters */
/* Reserved Mode bits: 2 */
#define ZCL_COMMISS_RESTART_OPTS_IMMEDIATE          0x08U
/* Reserved Options Field bits: 4..7 */

/** Restart Device command structure */
struct ZbZclCommissionClientRestartDev {
    uint8_t options; /**< Options - e.g. ZCL_COMMISS_RESTART_OPTS_MODE_MASK */
    uint8_t delay; /**< Delay (seconds) */
    uint8_t jitter; /**< Jitter - RAND(jitter * 80) milliseconds */
};

/** Save Startup Parameters command structure */
struct ZbZclCommissionClientSaveStartup {
    uint8_t options; /**< Options (Reserved) */
    uint8_t index; /**< Index */
};

/** Restore Startup Parameters command structure */
struct ZbZclCommissionClientRestoreStartup {
    uint8_t options; /**< Options (Reserved) */
    uint8_t index; /**< Index */
};

/* Reset Device Options Field */
#define ZCL_COMMISS_RESET_OPTS_RESET_CURR           0x01U
#define ZCL_COMMISS_RESET_OPTS_RESET_ALL            0x02U
#define ZCL_COMMISS_RESET_OPTS_RESET_ERASE          0x04U
/* Reserved Options Field bits: 3..7 */

/** Reset Startup Parameters command structure */
struct ZbZclCommissionClientResetStartup {
    uint8_t options; /**< Options - e.g. ZCL_COMMISS_RESET_OPTS_RESET_CURR */
    uint8_t index; /**< Index */
};

/* Server Commands */
/** Restart Device Response command structure */
struct ZbZclCommissionServerRestartDevRsp {
    enum ZclStatusCodeT status; /**< ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error. */
};

/** Save Startup Parameters Response command structure */
struct ZbZclCommissionServerSaveStartupRsp {
    enum ZclStatusCodeT status; /**< ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error. */
};

/** Restore Startup Parameters Response command structure */
struct ZbZclCommissionServerRestoreStartupRsp {
    enum ZclStatusCodeT status; /**< ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error. */
};

/** Reset Startup Parameters Response command structure */
struct ZbZclCommissionServerResetStartupRsp {
    enum ZclStatusCodeT status; /**< ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error. */
};

/** Commissioning Server Enable Information structure */
struct ZbZclCommissionServerEnableInfoT {
    uint8_t page; /**< Page */
    uint8_t channel; /**< Channel */
};

/** Commissioning Client Enable Information structure */
struct ZbZclCommissionClientEnableInfoT {
    uint8_t page; /**< Page */
    uint8_t channel; /**< Channel */
};

/* Commissioning Server */

/** Commissioning Server callbacks configuration */
struct ZbZclCommissionServerCallbacksT {
    enum ZclStatusCodeT (*restart_device)(struct ZbZclClusterT *cluster,
        struct ZbZclCommissionClientRestartDev *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Restart Device command.
     * Should call ZbZclCommissionServerSendRestartRsp to send response.
     * Since the application will end up calling ZbStartup or similar, the
     * application must wait and let the stack send the response before something like ZbStartup
     * is called. 100 milliseconds should be sufficient (ZCL_COMMISSION_RESTART_DEVICE_DELAY_MS).
     */

    enum ZclStatusCodeT (*save_startup)(struct ZbZclClusterT *cluster,
        struct ZbZclCommissionClientSaveStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Save Startup Parameters command. */

    enum ZclStatusCodeT (*restore_startup)(struct ZbZclClusterT *cluster,
        struct ZbZclCommissionClientRestoreStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Restore Startup Parameters command. */

    enum ZclStatusCodeT (*reset_startup)(struct ZbZclClusterT *cluster,
        struct ZbZclCommissionClientResetStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Reset Startup Parameters command. */
};

/**
 * Create a new instance of the Commissioning Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Set to ZB_ENDPOINT_BCAST if using Inter-PAN for communicating ZCL messages.
 * Otherwise, set a valid ZCL endpoint.
 * @param profile Profile ID for this cluster (e.g. ZCL_PROFILE_HOME_AUTOMATION)
 * @param aps_secured APS Security - true if APS Security enabled, else false
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclCommissionServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profile,
    bool aps_secured, struct ZbZclCommissionServerCallbacksT *callbacks, void *arg);

/**
 * Reset startup configurations cluster attributes back to defaults
 * @param cluster Cluster instance from which to send this command
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionServerResetStartup(struct ZbZclClusterT *cluster);

/**
 * Load startup configuration from Cluster Server's attributes to the stack's ZbStartupT structure
 * @param cluster Cluster instance from which to send this command
 * @param config Zigbee Stack Startup Configuration structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionServerGetStartup(struct ZbZclClusterT *cluster, struct ZbStartupT *config);

/**
 * Enable the Commissioning Server by configuring the MAC layer to listen for packets.
 * If enable is false, then Commissioning Server will stop processing any received Commissioning packets.
 * @param cluster Cluster instance from which to send this command
 * @param enable Enable or disable the ability to receive and process Commissioning commands.
 * @param info Commissioning Server Enable Information structure. Contains information needed
 * to start listening for Commissioning commands on a given channel. Optional and only applicable
 * if enable is true. This may be NULL if already configured and operational on a network.
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionServerEnable(struct ZbZclClusterT *cluster, bool enable,
    struct ZbZclCommissionServerEnableInfoT *info);

/**
 * Send a Restart Device Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Restart Device Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionServerSendRestartRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclCommissionServerRestartDevRsp *rsp);

/**
 * Send a Save Startup Parameters Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Save Startup Parameters Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionServerSendSaveStartupRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclCommissionServerSaveStartupRsp *rsp);

/**
 * Send a Restore Startup Parameters Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Restore Startup Parameters Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionServerSendRestoreStartupRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclCommissionServerRestoreStartupRsp *rsp);

/**
 * Send a Reset Startup Parameters Response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for response, including sequence number and tx options
 * @param rsp Reset Startup Parameters Response command structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionServerSendResetStartupRsp(struct ZbZclClusterT *cluster,
    struct ZbZclAddrInfoT *dst, struct ZbZclCommissionServerResetStartupRsp *rsp);

/* Commissioning Client */

/**
 * Create a new instance of the Commissioning Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Set to ZB_ENDPOINT_BCAST if using Inter-PAN for communicating ZCL messages.
 * Otherwise, set a valid ZCL endpoint.
 * @param profile Profile ID for this cluster (e.g. ZCL_PROFILE_HOME_AUTOMATION)
 * @param aps_secured APS Security - true if APS Security enabled, else false
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclCommissionClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profile, bool aps_secured);

/**
 * Enable Commissioning Client by configuring MAC layer to listen for packets.
 * @param cluster Cluster instance from which to send this command
 * @param info Commissioning Client Enable Information structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionClientEnable(struct ZbZclClusterT *cluster, struct ZbZclCommissionClientEnableInfoT *info);

/**
 * Send a Restart Device command
 * @param cluster Cluster instance from which to send this command
 * @param dst_ext Extended address of the device to send this command
 * @param dst_ep If cluster is not using Inter-PAN, this is the destination endpoint to send the command to.
 * @param req Restart Device command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionClientSendRestart(struct ZbZclClusterT *cluster, uint64_t dst_ext, uint8_t dst_ep,
    struct ZbZclCommissionClientRestartDev *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Save Startup Parameters command
 * @param cluster Cluster instance from which to send this command
 * @param dst_ext Extended address of the device to send this command
 * @param dst_ep If cluster is not using Inter-PAN, this is the destination endpoint to send the command to.
 * @param req Save Startup Parameters command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionClientSendSaveStartup(struct ZbZclClusterT *cluster, uint64_t dst_ext, uint8_t dst_ep,
    struct ZbZclCommissionClientSaveStartup *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Restore Startup Parameters command
 * @param cluster Cluster instance from which to send this command
 * @param dst_ext Extended address of the device to send this command
 * @param dst_ep If cluster is not using Inter-PAN, this is the destination endpoint to send the command to.
 * @param req Restore Startup Parameters command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionClientSendRestoreStartup(struct ZbZclClusterT *cluster, uint64_t dst_ext, uint8_t dst_ep,
    struct ZbZclCommissionClientRestoreStartup *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Reset Startup Parameters command
 * @param cluster Cluster instance from which to send this command
 * @param dst_ext Extended address of the device to send this command
 * @param dst_ep If cluster is not using Inter-PAN, this is the destination endpoint to send the command to.
 * @param req Reset Startup Parameters command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclCommissionClientSendResetStartup(struct ZbZclClusterT *cluster, uint64_t dst_ext, uint8_t dst_ep,
    struct ZbZclCommissionClientResetStartup *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif
