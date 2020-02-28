/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the SE Messaging cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_MESSAGE_H
# define ZCL_MESSAGE_H

#include "zcl/zcl.h"

/* Server Generated Commands */
enum {
    ZCL_MESSAGE_SVR_CMD_DISPLAY_MESSAGE = 0x00,
    ZCL_MESSAGE_SVR_CMD_CANCEL_MESSAGE = 0x01,
    ZCL_MESSAGE_SVR_CMD_DISPLAY_PROTECTED_MESSAGE = 0x02,
    ZCL_MESSAGE_SVR_CMD_CANCEL_ALL_MESSAGES = 0x03
};

/* Client Generated Commands */
enum {
    ZCL_MESSAGE_CLI_CMD_GET_LAST_MESSAGE = 0x00,
    ZCL_MESSAGE_CLI_CMD_MESSAGE_CONFIRM = 0x01,
    ZCL_MESSAGE_CLI_CMD_GET_MESSAGE_CANCELLATION = 0x02
};

enum {
    ZCL_MESSAGE_CTRL_TRANS_NORMAL = 0,
    ZCL_MESSAGE_CTRL_TRANS_BOTH = 1,
    ZCL_MESSAGE_CTRL_TRANS_INTERPAN = 2
};

enum {
    ZCL_MESSAGE_CTRL_IMPTNCE_LOW = 0,
    ZCL_MESSAGE_CTRL_IMPTNCE_MEDIUM = 1,
    ZCL_MESSAGE_CTRL_IMPTNCE_HIGH = 2,
    ZCL_MESSAGE_CTRL_IMPTNCE_CRITICAL = 3
};

#define ZCL_MESSAGE_MAX_LENGTH                     59U /* D.5.2.3.1.1.1 */

struct ZbZclMsgMessageT {
    uint32_t message_id;
    uint32_t start_time; /* UTC Seconds */
    uint16_t duration; /* Minutes */
    uint8_t message_control; /* ZbZclMessageControlSet - transmission, importance, confirm required, confirm enhanced */
    char message_str[ZCL_MESSAGE_MAX_LENGTH + 1U]; /* UTF-8 */
    uint8_t extended_control;
};

#define ZCL_MESSAGE_CTRL_EXTENDED_CONFIRMED       0x01U // message has been confirmed

/* Message Control fields. */
#define ZCL_MESSAGE_CTRL_TRANS_MASK        0x03U
#define ZCL_MESSAGE_CTRL_TRANS_OFFSET      0U
#define ZCL_MESSAGE_CTRL_TRANS_GET(_c_)    (((_c_) & ZCL_MESSAGE_CTRL_TRANS_MASK) >> ZCL_MESSAGE_CTRL_TRANS_OFFSET)

#define ZCL_MESSAGE_CTRL_IMPTNCE_MASK          0x0cU
#define ZCL_MESSAGE_CTRL_IMPTNCE_OFFSET        2U
#define ZCL_MESSAGE_CTRL_IMPTNCE_GET(_c_)      (((_c_) & ZCL_MESSAGE_CTRL_IMPTNCE_MASK) >> ZCL_MESSAGE_CTRL_IMPTNCE_OFFSET)

#define ZCL_MESSAGE_CTRL_ENH_CONF_MASK            0x20U
#define ZCL_MESSAGE_CTRL_ENH_CONF_OFFSET          5U
#define ZCL_MESSAGE_CTRL_ENH_CONF_GET(_c_)        (((_c_) & ZCL_MESSAGE_CTRL_ENH_CONF_MASK) >> ZCL_MESSAGE_CTRL_ENH_CONF_OFFSET)

#define ZCL_MESSAGE_CTRL_CONFIRMATION_MASK        0x80U
#define ZCL_MESSAGE_CTRL_CONFIRMATION_OFFSET      7U
#define ZCL_MESSAGE_CTRL_CONFIRMATION_GET(_c_)    (((_c_) & ZCL_MESSAGE_CTRL_CONFIRMATION_MASK) >> ZCL_MESSAGE_CTRL_CONFIRMATION_OFFSET)

/* Message Control "Set" Helper */
static inline void
ZbZclMessageControlSet(struct ZbZclMsgMessageT *msg, uint8_t transmission,
    uint8_t importance, uint8_t confirm, uint8_t enh_confirm)
{
    msg->message_control = 0;
    msg->message_control |= ((transmission << ZCL_MESSAGE_CTRL_TRANS_OFFSET) & ZCL_MESSAGE_CTRL_TRANS_MASK);
    msg->message_control |= ((importance << ZCL_MESSAGE_CTRL_IMPTNCE_OFFSET) & ZCL_MESSAGE_CTRL_IMPTNCE_MASK);
    msg->message_control |= ((confirm << ZCL_MESSAGE_CTRL_CONFIRMATION_OFFSET) & ZCL_MESSAGE_CTRL_CONFIRMATION_MASK);
    msg->message_control |= ((enh_confirm << ZCL_MESSAGE_CTRL_ENH_CONF_OFFSET) & ZCL_MESSAGE_CTRL_ENH_CONF_MASK);
}

/* Cancel Message Descriptor */
struct ZbZclMsgMessageCancelT {
    uint32_t message_id;
    uint8_t control;
};

/* Cancel All Messages Descriptor */
struct ZbZclMsgMessageCancelAllT {
    uint32_t implementation_time;
};

/* Message Confirmation Descriptor */
struct ZbZclMsgConfirmT {
    uint32_t message_id;
    uint32_t confirm_time;
};

/* Message Confirmation Extended Optional */
/* EXEGIN - where in which spec is this message defined? */
#define ZCL_MESSAGE_CONFIRM_ENH_RSP_MAX_LEN         32U

struct ZbZclMsgConfirmEnhT {
    uint32_t message_id;
    uint32_t confirm_time;
    uint8_t confirm_control;
    char confirm_response[ZCL_MESSAGE_CONFIRM_ENH_RSP_MAX_LEN + 1U];
};

#define ZCL_MESSAGE_CONF_RSP_LEN 21U

struct ZbZclMsgMessageConfT {
    uint32_t message_id;
    uint32_t confirm_time;
    bool has_confirm_control;
    uint8_t confirm_control;
    bool has_confirm_response;
    uint8_t confirm_response[ZCL_MESSAGE_CONF_RSP_LEN];
};

struct ZbZclMsgGetMsgCancellationT {
    uint32_t earliest_impl_time;
};

/*
 * Server
 */
struct ZbZclMsgServerCallbacksT {
    enum ZclStatusCodeT (*get_last_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*message_confirmation)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageConfT *conf, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*get_message_cancellation)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgGetMsgCancellationT *req, struct ZbZclAddrInfoT *source);
};

struct ZbZclClusterT * ZbZclMsgServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMsgServerCallbacksT *callbacks, void *arg);

enum ZclStatusCodeT ZbZclMsgServerDisplayMessageReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageT *msg,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclMsgServerDisplayProtectedMsgReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageT *msg,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclMsgServerCancelMessageReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageCancelT *cancel, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclMsgServerCancelAllReq(struct ZbZclClusterT *clusterPtr, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageCancelAllT *cancel_all, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*
 * Client
 */
struct ZbZclMsgClientCallbacksT {
    enum ZclStatusCodeT (*display_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageT *msg, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*cancel_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageCancelT *cancel, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*cancel_all_messages)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageCancelAllT *cancel_all, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*display_protected_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageT *msg, struct ZbZclAddrInfoT *srcInfo);
};

struct ZbZclClusterT * ZbZclMsgClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMsgClientCallbacksT *callbacks, void *arg);

enum ZclStatusCodeT ZbZclMsgClientGetLastReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclMsgClientConfReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgConfirmT *msg_conf,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclMsgClientGetMsgCancelReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, uint32_t earliestTime,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_MESSAGE_H */
