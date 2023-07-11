/**
 * @file zcl.message.h
 * @heading Messaging
 * @brief ZCL Messaging cluster header
 * ZCL 7 Section 10.5
 * ZCL 8 Section 10.5
 * @copyright Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved.
 */

/* EXEGIN - removed '@'PICS escape sequence, since these are not the
 * official PICS codes. These were added with the assumption that
 * SE clusters are being brought into the Zigbee 3.0 fold and will
 * use the same PICS codes and PICS format style. However, this has
 * yet to occur. */

/* ZCL.Message
 * SEMS.S | Server | True
 * SEMS.C | Client | True
 *
 * Server Attributes
 * SEMS.S.Afffd | ClusterRevision | True
 * SEMS.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEMS.S.C00.Rsp | Get Last Message | False
 * SEMS.S.C01.Rsp | Message Confirmation | False
 * SEMS.S.C02.Rsp | GetMessageCancellation | False
 *
 * Commands Generated
 * SEMS.S.C00.Tx | Display Message | True
 * SEMS.S.C01.Tx | Cancel Message | True
 * SEMS.S.C02.Tx | Display Protected Message | True
 * SEMS.S.C03.Tx | Cancel All Messages | True
 *
 * Client Attributes
 * SEMS.C.Afffd | ClusterRevision | True
 * SEMS.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEMS.C.C00.Rsp | Display Message | False
 * SEMS.C.C01.Rsp | Cancel Message | False
 * SEMS.C.C02.Rsp | Display Protected Message | False
 * SEMS.C.C03.Rsp | Cancel All Messages | False
 *
 * Commands Generated
 * SEMS.C.C00.Tx | Get Last Message | True
 * SEMS.C.C01.Tx | Message Confirmation | True
 * SEMS.C.C02.Tx | GetMessageCancellation | True
 */

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

#define ZCL_MESSAGE_MAX_LENGTH                     59U /* D.5.2.3.1.1.1 */

/** Display Message/Display Protected Message command structure */
struct ZbZclMsgMessageT {
    uint32_t message_id; /**< Message ID */
    uint32_t start_time; /**< Start Time - UTC Seconds */
    uint16_t duration; /**< Duration In Minutes */
    uint8_t message_control; /**< Message Control */
    char message_str[ZCL_MESSAGE_MAX_LENGTH + 1U]; /**< Message - UTF-8 */
    uint8_t extended_control; /**< Extended Message Control */
};

#define ZCL_MESSAGE_CTRL_EXTENDED_CONFIRMED       0x01U // message has been confirmed

/* Message Control bit masks */
#define ZCL_MESSAGE_CTRL_TRANS_NORMAL           0x00 /* Normal transmission only */
#define ZCL_MESSAGE_CTRL_TRANS_NORMAL_AND_IP    0x01 /* Normal and InterPAN transmission */
#define ZCL_MESSAGE_CTRL_TRANS_IP_ONLY          0x02 /* InterPAN only */
#define ZCL_MESSAGE_CTRL_TRANS_MASK             0x03
#define ZCL_MESSAGE_CTRL_IMPORT_LOW             0x00 /* Message importance = low */
#define ZCL_MESSAGE_CTRL_IMPORT_MED             0x04 /* Message importance = medium */
#define ZCL_MESSAGE_CTRL_IMPORT_HIGH            0x08 /* Message importance = high */
#define ZCL_MESSAGE_CTRL_IMPORT_CRITICAL        0x0c /* Message importance = critical */
#define ZCL_MESSAGE_CTRL_IMPORT_MASK            0x0c
#define ZCL_MESSAGE_CTRL_ENH_CONF               0x20 /* Enhanced confirmation required */
#define ZCL_MESSAGE_CTRL_CONF                   0x80 /* Message confirmation required */

/** Cancel Message command structure */
struct ZbZclMsgMessageCancelT {
    uint32_t message_id; /**< Message ID */
    uint8_t control; /**< Message Control */
};

/** Cancel All Messages command structure */
struct ZbZclMsgMessageCancelAllT {
    uint32_t implementation_time; /**< Implementation Date/Time */
};

/** Message Confirmation command structure */
struct ZbZclMsgConfirmT {
    uint32_t message_id; /**< Message ID */
    uint32_t confirm_time; /**< Confirmation Time */
};

/* Message Confirmation Extended Optional */
/* EXEGIN - where in which spec is this message defined? */
#define ZCL_MESSAGE_CONFIRM_ENH_RSP_MAX_LEN         32U

/** Enhanced Message Confirmation command structure */
struct ZbZclMsgConfirmEnhT {
    uint32_t message_id; /**< Message ID */
    uint32_t confirm_time; /**< Confirmation Time */
    uint8_t confirm_control; /**< Message Confirmation Control */
    char confirm_response[ZCL_MESSAGE_CONFIRM_ENH_RSP_MAX_LEN + 1U]; /**< Message Confirmation Response */
};

#define ZCL_MESSAGE_CONF_RSP_LEN 21U

/** Message Confirmation callback structure */
struct ZbZclMsgMessageConfT {
    uint32_t message_id; /**< Message ID */
    uint32_t confirm_time; /**< Confirmation Time */
    bool has_confirm_control; /**< Has Message Confirmation Control */
    uint8_t confirm_control; /**< Message Confirmation Control */
    bool has_confirm_response; /**< Has Message Confirmation Response */
    uint8_t confirm_response[ZCL_MESSAGE_CONF_RSP_LEN]; /**< Message Confirmation Response */
};

/** Get Message Cancellation command structure */
struct ZbZclMsgGetMsgCancellationT {
    uint32_t earliest_impl_time; /**< Earliest Implementation Time */
};

/*
 * Server
 */

/** Messaging Server callbacks configuration */
struct ZbZclMsgServerCallbacksT {
    enum ZclStatusCodeT (*get_last_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Last Message command */

    enum ZclStatusCodeT (*message_confirmation)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageConfT *conf, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Message Confirmation command */

    enum ZclStatusCodeT (*get_message_cancellation)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgGetMsgCancellationT *req, struct ZbZclAddrInfoT *source);
    /**< Callback to application, invoked on receipt of Get Message Cancellation command */
};

/**
 * Create a new instance of the Messaging Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclMsgServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMsgServerCallbacksT *callbacks, void *arg);

/**
 * Send a Display Message command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param msg Display Message command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMsgServerDisplayMessageReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageT *msg, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Display Protected Message command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param msg Display Protected Message command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMsgServerDisplayProtectedMsgReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageT *msg, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Cancel Message command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cancel Cancel Message command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMsgServerCancelMessageReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageCancelT *cancel, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Cancel All Messages command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cancel_all Cancel All Messages command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMsgServerCancelAllReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgMessageCancelAllT *cancel_all, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*
 * Client
 */

/** Messaging Client callbacks configuration */
struct ZbZclMsgClientCallbacksT {
    enum ZclStatusCodeT (*display_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageT *msg, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Display Message command */

    enum ZclStatusCodeT (*cancel_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageCancelT *cancel, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Cancel Message command */

    enum ZclStatusCodeT (*cancel_all_messages)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageCancelAllT *cancel_all, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Cancel All Messages command */

    enum ZclStatusCodeT (*display_protected_message)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclMsgMessageT *msg, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Display Protected Message command */
};

/**
 * Create a new instance of the Messaging Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclMsgClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclMsgClientCallbacksT *callbacks, void *arg);

/**
 * Send a Get Last Message command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMsgClientGetLastReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Message Confirmation command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param msg_conf Message Confirmation command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMsgClientConfReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct ZbZclMsgConfirmT *msg_conf,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Message Cancellation command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param earliestTime UTC Timestamp indicating the earliest implementation time of a Cancel All Messages command to be returned
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclMsgClientGetMsgCancelReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, uint32_t earliestTime,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_MESSAGE_H */
