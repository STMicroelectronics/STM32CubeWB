/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_COMMISSION_H
# define ZCL_COMMISSION_H

#include "zcl/zcl.h"

/* PICS.ZCL.Commissioning
 * CS.S | True
 * CS.C | True
 *
 * Server Attributes
 * CS.S.A0000 | True
 * CS.S.A0001 | True
 * CS.S.A0002 | True
 * CS.S.A0003 | True
 * CS.S.A0004 | True
 * CS.S.A0005 | True
 * CS.S.A0006 | True
 * CS.S.A0010 | True
 * CS.S.A0011 | False
 * CS.S.A0012 | True
 * CS.S.A0013 | True
 * CS.S.A0014 | True
 * CS.S.A0015 | True
 * CS.S.A0016 | True
 * CS.S.A0017 | True
 * CS.S.A0020 | True
 * CS.S.A0021 | False
 * CS.S.A0022 | False
 * CS.S.A0023 | False
 * CS.S.A0030 | False
 * CS.S.A0031 | False
 * CS.S.A0040 | False
 * CS.S.A0041 | False
 * CS.S.A0042 | False
 * CS.S.Afffd | True
 * CS.S.Afffe | False
 *
 * Commands received
 * CS.S.C00.Rsp |  depends on customer application
 * CS.S.C01.Rsp |  depends on customer application
 * CS.S.C02.Rsp |  depends on customer application
 * CS.S.C03.Rsp |  depends on customer application
 *
 * Commands generated
 * CS.S.C00.Tx |  depends on customer application
 * CS.S.C01.Tx |  depends on customer application
 * CS.S.C02.Tx |  depends on customer application
 * CS.S.C03.Tx |  depends on customer application
 *
 * Client attributes
 * CS.C.Afffd | True
 * CS.C.Afffe | False
 *
 * Commands received
 * CS.C.C00.Rsp |  depends on customer application
 * CS.C.C01.Rsp |  depends on customer application
 * CS.C.C02.Rsp |  depends on customer application
 * CS.C.C03.Rsp |  depends on customer application
 *
 * Commands generated
 * CS.C.C00.Tx |  True
 * CS.C.C01.Tx |  True
 * CS.C.C02.Tx |  True
 * CS.C.C03.Tx |  True
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

enum ZbZclCommissionServerAttrT {
    /* Startup Parameters */
    ZCL_COMMISSION_CLI_ATTR_SHORT_ADDR = 0x0000,
    ZCL_COMMISSION_CLI_ATTR_EPID = 0x0001,
    ZCL_COMMISSION_CLI_ATTR_PANID = 0x0002,
    ZCL_COMMISSION_CLI_ATTR_CHANNELMASK = 0x0003,
    ZCL_COMMISSION_CLI_ATTR_PROTOCOLVER = 0x0004,
    ZCL_COMMISSION_CLI_ATTR_STACKPROFILE = 0x0005,
    ZCL_COMMISSION_CLI_ATTR_STARTUPCONTROL = 0x0006,
    ZCL_COMMISSION_CLI_ATTR_TCADDR = 0x0010,
    ZCL_COMMISSION_CLI_ATTR_TCMASTER = 0x0011,
    ZCL_COMMISSION_CLI_ATTR_NWKKEY = 0x0012,
    ZCL_COMMISSION_CLI_ATTR_USEINSECJOIN = 0x0013,
    ZCL_COMMISSION_CLI_ATTR_PRECONFLINKKEY = 0x0014,
    ZCL_COMMISSION_CLI_ATTR_NWKKEYSEQNUM = 0x0015,
    ZCL_COMMISSION_CLI_ATTR_NWKKEYTYPE = 0x0016,
    ZCL_COMMISSION_CLI_ATTR_NWKMGRADDR = 0x0017,

    /* Join Parameters */
    ZCL_COMMISSION_CLI_ATTR_SCANATTEMPTS = 0x0020,
    ZCL_COMMISSION_CLI_ATTR_TIMEBTWSCANS = 0x0021,
    ZCL_COMMISSION_CLI_ATTR_REJOININTERVAL = 0x0022,
    ZCL_COMMISSION_CLI_ATTR_MAXREJOININTERVAL = 0x0023,

    /* End Device Parameters */
    ZCL_COMMISSION_CLI_ATTR_POLLRATE = 0x0030,
    ZCL_COMMISSION_CLI_ATTR_PARENTRETRYTHRESH = 0x0031,

    /* Concentrator Parameters */
    ZCL_COMMISSION_CLI_ATTR_CONCFLAG = 0x0040,
    ZCL_COMMISSION_CLI_ATTR_CONCRADIUS = 0x0041,
    ZCL_COMMISSION_CLI_ATTR_CONCDISCTIME = 0x0042
};

/*** Client Commands (Requests) */

/* Restart Device Options Field */
#define ZCL_COMMISS_RESTART_OPTS_MODE_MASK          0x07U
#define ZCL_COMMISS_RESTART_OPTS_MODE_USE_STARTUP   0x00U /* Current set of startup parameters */
#define ZCL_COMMISS_RESTART_OPTS_MODE_USE_STACK     0x01U /* Current state of device / stack parameters */
/* Reserved Mode bits: 2 */
#define ZCL_COMMISS_RESTART_OPTS_IMMEDIATE          0x08U
/* Reserved Options Field bits: 4..7 */

struct ZbZclCommissionClientRestartDev {
    uint8_t options; /* e.g. ZCL_COMMISS_RESTART_OPTS_MODE_MASK */
    uint8_t delay; /* seconds */
    uint8_t jitter; /* RAND(jitter * 80) milliseconds */
};

struct ZbZclCommissionClientSaveStartup {
    uint8_t options; /* all bits are reserved */
    uint8_t index;
};

struct ZbZclCommissionClientRestoreStartup {
    uint8_t options; /* all bits are reserved */
    uint8_t index;
};

/* Reset Device Options Field */
#define ZCL_COMMISS_RESET_OPTS_RESET_CURR           0x01U
#define ZCL_COMMISS_RESET_OPTS_RESET_ALL            0x02U
#define ZCL_COMMISS_RESET_OPTS_RESET_ERASE          0x04U
/* Reserved Options Field bits: 3..7 */

struct ZbZclCommissionClientResetStartup {
    uint8_t options; /* e.g. ZCL_COMMISS_RESET_OPTS_RESET_CURR */
    uint8_t index;
};

/*** Server Commands (Responses) */
struct ZbZclCommissionServerRestartDevRsp {
    enum ZclStatusCodeT status;
};

struct ZbZclCommissionServerSaveStartupRsp {
    enum ZclStatusCodeT status;
};

struct ZbZclCommissionServerRestoreStartupRsp {
    enum ZclStatusCodeT status;
};

struct ZbZclCommissionServerResetStartupRsp {
    enum ZclStatusCodeT status;
};

struct ZbZclCommissionServerEnableInfoT {
    uint8_t page;
    uint8_t channel;
};

struct ZbZclCommissionClientEnableInfoT {
    uint8_t page;
    uint8_t channel;
};

/*** Server API ***/

/**
 * Commissioning Server Cluster Callbacks
 */
struct ZbZclCommissionServerCallbacksT {
    enum ZclStatusCodeT (*restart_device)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclCommissionClientRestartDev *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Restart Device Request callback. Should call ZbZclCommissionServerSendRestartRsp
     * to send response. Since the application will end up calling ZbStartup or similar, the
     * application must wait and let the stack send the response before something like ZbStartup
     * is called. 100 milliseconds should be sufficient (ZCL_COMMISSION_RESTART_DEVICE_DELAY_MS). */

    enum ZclStatusCodeT (*save_startup)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclCommissionClientSaveStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Save Startup Request callback */

    enum ZclStatusCodeT (*restore_startup)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclCommissionClientRestoreStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Restore Startup Request callback */

    enum ZclStatusCodeT (*reset_startup)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclCommissionClientResetStartup *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Reset Startup Request callback */
};

struct ZbZclClusterT * ZbZclCommissionServerAlloc(struct ZigBeeT *zb, uint16_t profile, bool aps_secured,
    struct ZbZclCommissionServerCallbacksT *callbacks, void *arg);

/* Helper function to reset the startup configuration cluster attributes back to defaults. */
enum ZclStatusCodeT ZbZclCommissionServerResetStartup(struct ZbZclClusterT *clusterPtr);

/* Helper function to load the startup configuration from the Cluster Server's Attributes
 * into the ZbStartupT struct. */
enum ZclStatusCodeT ZbZclCommissionServerGetStartup(struct ZbZclClusterT *clusterPtr, struct ZbStartupT *config);

/* Helper to configure MAC layer to listen for packets
 * If info = NULL, then Commissioning Server will stop processing any received Commissioning
 * packets. */
enum ZclStatusCodeT ZbZclCommissionServerEnable(struct ZbZclClusterT *clusterPtr,
    struct ZbZclCommissionServerEnableInfoT *info);

enum ZclStatusCodeT ZbZclCommissionServerSendRestartRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCommissionServerRestartDevRsp *rsp);

enum ZclStatusCodeT ZbZclCommissionServerSendSaveStartupRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCommissionServerSaveStartupRsp *rsp);

enum ZclStatusCodeT ZbZclCommissionServerSendRestoreStartupRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCommissionServerRestoreStartupRsp *rsp);

enum ZclStatusCodeT ZbZclCommissionServerSendResetStartupRsp(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclCommissionServerResetStartupRsp *rsp);

/*** Client API */
struct ZbZclClusterT * ZbZclCommissionClientAlloc(struct ZigBeeT *zb, uint16_t profile, bool aps_secured);

/* Helper to configure MAC layer to listen for packets */
enum ZclStatusCodeT ZbZclCommissionClientEnable(struct ZbZclClusterT *clusterPtr,
    struct ZbZclCommissionClientEnableInfoT *info);

enum ZclStatusCodeT ZbZclCommissionClientSendRestart(struct ZbZclClusterT *clusterPtr, uint64_t ext_addr,
    struct ZbZclCommissionClientRestartDev *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclCommissionClientSendSaveStartup(struct ZbZclClusterT *clusterPtr, uint64_t ext_addr,
    struct ZbZclCommissionClientSaveStartup *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclCommissionClientSendRestoreStartup(struct ZbZclClusterT *clusterPtr, uint64_t ext_addr,
    struct ZbZclCommissionClientRestoreStartup *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclCommissionClientSendResetStartup(struct ZbZclClusterT *clusterPtr, uint64_t ext_addr,
    struct ZbZclCommissionClientResetStartup *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_COMMISSION_H */
