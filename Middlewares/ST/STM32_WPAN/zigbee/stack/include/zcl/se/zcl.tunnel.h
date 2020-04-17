/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Tunneling cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_TUNNEL_H
# define ZCL_TUNNEL_H

#include "zcl/zcl.h"

/* Tunneling cluster attributes */
enum {
    ZCL_TUNNEL_ATTR_TIMEOUT = 0x0000
};

/* Client-to-Server commands */
enum {
    ZCL_TUNNEL_SVR_CMD_REQUEST = 0x00,
    ZCL_TUNNEL_SVR_CMD_CLOSE = 0x01,
    ZCL_TUNNEL_SVR_CMD_DATA = 0x02,
    ZCL_TUNNEL_SVR_CMD_ERROR = 0x03,
    ZCL_TUNNEL_SVR_CMD_ACK = 0x04, /* for flow control */
    ZCL_TUNNEL_SVR_CMD_READY = 0x05, /* for flow control */
    ZCL_TUNNEL_SVR_CMD_SUPPORTED_REQ = 0x06
};

/* Server-to-Client commands. */
enum {
    ZCL_TUNNEL_CLI_CMD_RESPONSE = 0x00,
    ZCL_TUNNEL_CLI_CMD_DATA = 0x01,
    ZCL_TUNNEL_CLI_CMD_ERROR = 0x02,
    ZCL_TUNNEL_CLI_CMD_ACK = 0x03,
    ZCL_TUNNEL_CLI_CMD_READY = 0x04,
    ZCL_TUNNEL_CLI_CMD_SUPPORTED_RSP = 0x05,
    ZCL_TUNNEL_CLI_CMD_CLOSE_NOTIFY = 0x06
};

/* Tunnel protocol ID enumerations */
enum ZbZclTunnelProtocolT {
    ZCL_TUNNEL_PROTO_DLMS_COSEM = 0,
    ZCL_TUNNEL_PROTO_IEC_61107 = 1,
    ZCL_TUNNEL_PROTO_ANSI_C12 = 2,
    ZCL_TUNNEL_PROTO_M_BUS = 3,
    ZCL_TUNNEL_PROTO_SML = 4,
    ZCL_TUNNEL_PROTO_CLIMATE_TALK = 5,
    ZCL_TUNNEL_PROTO_GB_HRGP = 6,
    ZCL_TUNNEL_PROTO_IPV6 = 7,
    ZCL_TUNNEL_PROTO_IPV4 = 8,
    ZCL_TUNNEL_PROTO_NULL = 9,
    ZCL_TUNNEL_PROTO_TEST = 199,
    ZCL_TUNNEL_PROTO_MANUFACTURER = 200,
    ZCL_TUNNEL_PROTO_RESERVED = 0xff
};

/* Tunnel Status Codes.
 * These are used with the RequestTunnelResponse
 * (ZCL_TUNNEL_CLI_CMD_RESPONSE). */
enum ZbZclTunnelStatusT {
    ZCL_TUNNEL_STATUS_SUCCESS = 0x00,
    ZCL_TUNNEL_STATUS_BUSY = 0x01,
    ZCL_TUNNEL_STATUS_NO_RESOURCES = 0x02,
    ZCL_TUNNEL_STATUS_PROTO_UNSUPPORTED = 0x03,
    ZCL_TUNNEL_STATUS_FLOW_UNSUPPORTED = 0x04
};

/* Transfer Data Status codes.
 * These are used with the TransferDataError (ZCL_TUNNEL_SVR_CMD_ERROR and
 * ZCL_TUNNEL_CLI_CMD_ERROR) commands. */
enum ZbZclTunnelXferStatusT {
    /* ugh, why use 0x00 for an error code?
     * And why couldn't these status codes be combined with the
     * Tunnel Status Codes? */
    ZCL_TUNNEL_XFER_STATUS_NO_TUNNEL = 0x00,
    ZCL_TUNNEL_XFER_STATUS_WRONG_DEVICE = 0x01,
    ZCL_TUNNEL_XFER_STATUS_OVERFLOW = 0x02,

    /* Exegin add-on. If zcl_tunnel_handle_data returns
     * ZCL_TUNNEL_XFER_STATUS_NO_RESPONSE, don't send a response. */
    ZCL_TUNNEL_XFER_STATUS_NO_RESPONSE = 0xff
};

struct ZbZclTunnelStateT;

/*---------------------------------------------------------------
 * Server
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclTunnelServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

enum ZclStatusCodeT ZbZclTunnelServerAddProto(struct ZbZclClusterT *clusterPtr,
    enum ZbZclTunnelProtocolT protocol, /* Protocol ID enumeration. */
    uint16_t mfr, /* Manufacturer ID. */
    uint16_t mtu, /* Protocol's MTU. */
    /* request - mandatory */
    enum ZbZclTunnelStatusT (*request)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv),
    /* input - mandatory */
    void (*input)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv),
    /* close - optional (required if application needs to keep track of open tunnels, e.g. for persistence) */
    void (*close)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv),
    /* error - optional. To handle ZCL_TUNNEL_SVR_CMD_ERROR. Return false if tunnel should be closed */
    bool (*error)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv, enum ZbZclTunnelXferStatusT status),
    void *priv);

struct ZbZclTunnelStateT * ZbZclTunnelServerStateFindById(struct ZbZclClusterT *clusterPtr, uint16_t tunnel_id);

enum ZclStatusCodeT ZbZclTunnelServerSendto(struct ZbZclClusterT *clusterPtr,
    struct ZbZclTunnelStateT *state, const uint8_t *data, unsigned int len, enum ZbZclDirectionT direction,
    void (*callback)(struct ZbZclCommandRspT *rspPtr, void *arg), void *arg);

void ZbZclTunnelServerSendAllMatch(struct ZbZclClusterT *clusterPtr, uint64_t eui, void *data, unsigned int len);

/*---------------------------------------------------------------
 * Client
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclTunnelClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

enum ZclStatusCodeT ZbZclTunnelClientAddProto(struct ZbZclClusterT *clusterPtr,
    enum ZbZclTunnelProtocolT protocol, /* Protocol ID enumeration. */
    uint16_t mfr, /* Manufacturer ID. */
    uint16_t mtu, /* Protocol's MTU. */
    /* input - mandatory */
    void (*input)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv),
    /* close - optional */
    void (*close)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv),
    /* error - optional. To handle ZCL_TUNNEL_CLI_CMD_ERROR. Return false if tunnel should be closed */
    bool (*error)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *statePtr, void *priv, enum ZbZclTunnelXferStatusT status),
    void *priv);

enum ZclStatusCodeT ZbZclTunnelClientConnectReq(struct ZbZclClusterT *clusterPtr, uint64_t dst_addr, uint8_t dst_ep,
    void (*callback)(struct ZbZclClusterT *clusterPtr, struct ZbZclTunnelStateT *state, enum ZbZclTunnelStatusT status, void *arg),
    void *arg);

enum ZclStatusCodeT ZbZclTunnelClientCloseReq(struct ZbZclClusterT *clusterPtr,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/* For testing only - close the tunnel without informing the server. */
enum ZclStatusCodeT ZbZclTunnelClientCloseQuietReq(struct ZbZclClusterT *clusterPtr);

enum ZclStatusCodeT ZbZclTunnelClientSendReq(struct ZbZclClusterT *clusterPtr, const uint8_t *data, uint16_t len,
    void (*callback)(struct ZbZclCommandRspT *rspPr, void *arg), void *arg);

/*---------------------------------------------------------------
 * Misc Helpers
 *---------------------------------------------------------------
 */
uint16_t ZbZclTunnelStateGetId(struct ZbZclTunnelStateT *state);
enum ZbZclTunnelProtocolT ZbZclTunnelStateGetProtocol(struct ZbZclTunnelStateT *state);
uint8_t * ZbZclTunnelStateGetDataPtr(struct ZbZclTunnelStateT *state);
uint32_t ZbZclTunnelStateGetDataLen(struct ZbZclTunnelStateT *state, bool clear_data);
/* For testing only - change the ID of the current tunnel. */
bool zcl_tuncli_test_change_id(struct ZbZclClusterT *clusterPtr, uint16_t tunnel_id);

#endif /* __ZCL_TUNNEL_H */
