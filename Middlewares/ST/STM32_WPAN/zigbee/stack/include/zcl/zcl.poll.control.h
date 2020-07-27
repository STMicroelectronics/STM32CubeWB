/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_POLL_CONTROL_H
# define ZCL_POLL_CONTROL_H

/* PICS.ZCL.Poll.Control
 * POLL.S | True
 * POLL.C | True
 *
 * Server Attributes
 * POLL.S.A0000 | True
 * POLL.S.A0001 | True
 * POLL.S.A0002 | True
 * POLL.S.A0003 | True
 * POLL.S.A0004 | True
 * POLL.S.A0005 | True
 * POLL.S.A0006 | True
 * POLL.S.AFFFD | True
 * POLL.S.AFFFE | False
 *
 * Commands received
 * POLL.S.C00.Rsp | True
 * POLL.S.C01.Rsp | True
 * POLL.S.C02.Rsp | True
 * POLL.S.C03.Rsp | True
 *
 * Commands sent
 * POLL.S.C00.Tx | True
 *
 * Client Attributes
 * POLL.C.AFFFD | True
 * POLL.C.AFFFE | False
 *
 * Commands received
 * POLL.C.C00.Rsp | True
 *
 * Commands sent
 * POLL.C.C00.Tx | True
 * POLL.C.C01.Tx | True
 * POLL.C.C02.Tx | True
 * POLL.C.C03.Tx | True
 */

#include "zcl/zcl.h"

/** Poll Control Server Attribute IDs */
enum ZbZclPollControlSvrAttrT {
    ZCL_POLL_CHECK_IN_INTERVAL = 0x0000,
    ZCL_POLL_LONG_POLL_INTERVAL = 0x0001,
    ZCL_POLL_SHORT_POLL_INTERVAL = 0x0002,
    ZCL_POLL_FAST_POLL_TIMEOUT = 0x0003,
    ZCL_POLL_CHECK_IN_INTERVAL_MIN = 0x0004,
    ZCL_POLL_LONG_POLL_INTERVAL_MIN = 0x0005,
    ZCL_POLL_FAST_POLL_TIMEOUT_MAX = 0x0006,

};

/* Client Generated Commands */
enum {
    ZCL_POLL_CTRL_CLI_CHECK_IN_RSP = 0x00, /* mandatory */
    ZCL_POLL_CTRL_CLI_FAST_POLL_STOP = 0x01, /* mandatory */
    ZCL_POLL_CTRL_CLI_SET_LONG_POLL_INTERVAL = 0x02,
    ZCL_POLL_CTRL_CLI_SET_SHOR_POLL_INTERVAL = 0x03
};

/* Server Generated Commands */
enum {
    ZCL_POLL_CTRL_SVR_CHECK_IN = 0x00 /* mandatory */
};

struct zcl_poll_checkin_rsp_t {
    enum ZclStatusCodeT status;
    bool start_fast_poll;
    uint16_t fast_poll_timeout;
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */
struct ZbZclPollControlServerCallbackT {
    void (*checkin_rsp)(struct ZbZclClusterT *clusterPtr,
        struct zcl_poll_checkin_rsp_t *rsp_info, struct ZbZclAddrInfoT *srcInfo, void *arg);
};

/**
 * Allocate the Poll Control Server cluster
 * @param zb Zigbee instance
 * @param endpoint Endpoint to allocate cluster on
 * @return Cluster instance
 */
struct ZbZclClusterT * zcl_poll_server_alloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPollControlServerCallbackT *callbacks, void *arg);

/**
 * Get the Poll Control Server to send a Check-in Request to any bound Clients.
 * @param cluster Cluster instance
 * @return ZCL Status Code
 */
enum ZclStatusCodeT zcl_poll_server_send_checkin(struct ZbZclClusterT *cluster);

/**
 * Change the long polling interval. Default is disabled (ZCL_INVALID_UNSIGNED_32BIT = 0xffffffff).
 * @param clusterPtr
 * @param long_poll_intvl Polling interval in quarter-seconds (1 = 250 mS)
 * @return ZCL Status Code
 */
enum ZclStatusCodeT zcl_poll_server_write_long_poll_intvl(struct ZbZclClusterT *clusterPtr,
    uint32_t long_poll_intvl);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */
struct ZbZclPollControlClientCheckinInfo {
    struct ZbApsAddrT dst;
    bool start_fast_poll;
    uint16_t fast_poll_timeout;
};

struct ZbZclPollControlClientStopReq {
    struct ZbApsAddrT dst;
};

struct ZbZclPollControlClientSetLongReq {
    struct ZbApsAddrT dst;
    uint32_t interval;
};

struct ZbZclPollControlClientSetShortReq {
    struct ZbApsAddrT dst;
    uint16_t interval;
};

struct ZbZclPollControlClientCallbackT {
    /* Callback limitation note:
     * If timeout returned to the callback is zero, that means that the server is fast polling
     * for the amount of time it's fast poll timeout attribute is set to.
     *
     * We can only assume that the server is fast polling if the default response status is
     * success, since there is no way for the client to know if the server is actually fast
     * polling from the check-in response due to various reasons (i.e. already fast polling,
     * fails to start fast poll after sending the default response, or stopped fast polling).
     *
     * If the polling boolean is false, there is a chance that the server is fast polling
     * still. This could be due to other Poll Clients the server is bound to.
     * */
    enum ZclStatusCodeT (*checkin_rsp_callback)(struct ZbZclClusterT *clusterPtr,
        struct zcl_poll_checkin_rsp_t *rsp_info, struct ZbZclAddrInfoT *srcInfo, void *arg);
};

struct ZbZclClusterT * zcl_poll_client_alloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPollControlClientCallbackT *callbacks, void *arg);

enum ZclStatusCodeT zcl_poll_client_set_checkin_rsp(struct ZbZclClusterT *cluster,
    struct ZbZclPollControlClientCheckinInfo *info);

enum ZclStatusCodeT zcl_poll_client_stop_fastpoll_req(struct ZbZclClusterT *cluster,
    struct ZbZclPollControlClientStopReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_poll_client_set_long_intvl_req(struct ZbZclClusterT *cluster,
    struct ZbZclPollControlClientSetLongReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_poll_client_set_short_intvl_req(struct ZbZclClusterT *cluster,
    struct ZbZclPollControlClientSetShortReq *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_POLL_CONTROL_H */
