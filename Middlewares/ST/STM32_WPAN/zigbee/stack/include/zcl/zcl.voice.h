/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_VOICE_H
#define ZCL_VOICE_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the Voice Over Zigbee cluster.
 *--------------------------------------------------------------------------
 */

#include "zcl/zcl.h"

/* Attribute Identifiers */
enum {
    ZCL_VOICE_ATTR_CODEC_TYPE = 0x0000,
    ZCL_VOICE_ATTR_SAMP_FREQ = 0x0001,
    ZCL_VOICE_ATTR_CODECRATE = 0x0002,
    ZCL_VOICE_ATTR_ESTAB_TIMEOUT = 0x0003,
    ZCL_VOICE_ATTR_CODEC_TYPE_SUB_1 = 0x0004,
    ZCL_VOICE_ATTR_CODEC_TYPE_SUB_2 = 0x0005,
    ZCL_VOICE_ATTR_CODEC_TYPE_SUB_3 = 0x0006,
    ZCL_VOICE_ATTR_COMPRESSION_TYPE = 0x0007,
    ZCL_VOICE_ATTR_COMPRESSION_RATE = 0x0008,
    ZCL_VOICE_ATTR_OPTION_FLAGS = 0x0009,
    ZCL_VOICE_ATTR_THRESHOLD = 0x000a
};

/* Client Generated Commands */
enum {
    ZCL_VOICE_CLI_ESTAB_REQ = 0x00,
    ZCL_VOICE_CLI_VOICE_TX = 0x01,
    ZCL_VOICE_CLI_TX_COMPLETE = 0x02,
    ZCL_VOICE_CLI_CONTROL_RSP = 0x03
};

/* Server Generated Commands */
enum {
    ZCL_VOICE_SVR_ESTAB_RSP = 0x00,
    ZCL_VOICE_SVR_VOICE_TX_RSP = 0x01,
    ZCL_VOICE_SVR_CONTROL = 0x02
};

/* Voice of Zigbee Defined Values */
#define ZCL_VOICE_FLAG_CODEC_TYPE_SUB_1     0x01
#define ZCL_VOICE_FLAG_CODEC_TYPE_SUB_2     0x02
#define ZCL_VOICE_FLAG_CODEC_TYPE_SUB_3     0x04
#define ZCL_VOICE_FLAG_COMPRESSION          0x08

#define ZCL_VOICE_TX_RSP_ERROR_DECODE       0x00 /* Failure to decode voice data */
#define ZCL_VOICE_TX_RSP_ERROR_ORDER        0x01 /* Wrong order of voice data */

/* Request and Response Structures */
struct voice_estab_req_t {
    uint8_t flag;
    uint8_t codec_type;
    uint8_t samp_freq;
    uint8_t codec_rate;
    uint8_t service_type;
    uint8_t codec_type_s1;
    uint8_t codec_type_s2;
    uint8_t codec_type_s3;
    uint8_t comp_type;
    uint8_t comp_rate;
};

struct voice_estab_rsp_t {
    uint8_t ack_nak;
    uint8_t codec_type;
};

struct voice_voice_tx_t {
    uint8_t *voice_data;
    uint16_t voice_data_len;
};

struct voice_voice_tx_rsp_t {
    uint8_t error_flag; /* e.g. ZCL_VOICE_TX_RSP_ERROR_DECODE */
};

struct voice_control_t {
    uint8_t control_type;
};

struct voice_control_rsp_t {
    uint8_t ack_nak;
};

/* Callback Structures */
struct zcl_voice_server_callbacks_t {
    /* For voice_tx, the application should return ZCL_STATUS_SUCCESS or call ZbZclVoiceServerSendVoiceTxRsp
     * to send an error response and return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE */
    enum ZclStatusCodeT (*estab_req)(struct ZbZclClusterT *clusterPtr, struct voice_estab_req_t *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);

    enum ZclStatusCodeT (*voice_tx)(struct ZbZclClusterT *clusterPtr, struct voice_voice_tx_t *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);

    enum ZclStatusCodeT (*tx_complete)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclAddrInfoT *src_info, void *arg);
};

struct zcl_voice_client_callbacks_t {
    enum ZclStatusCodeT (*control)(struct ZbZclClusterT *clusterPtr, struct voice_control_t *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
};

/* Client API */
struct ZbZclClusterT * ZbZclVoiceClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct zcl_voice_client_callbacks_t *callbacks, void *arg);

enum ZclStatusCodeT ZbZclVoiceClientEstabReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct voice_estab_req_t *estab_req, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceVoiceTxReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct voice_voice_tx_t *voice_tx, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceTxCompletedReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceClientSendControlRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct voice_control_rsp_t *rsp);

/* Server API */
struct ZbZclClusterT * ZbZclVoiceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct zcl_voice_server_callbacks_t *callbacks, void *arg);

enum ZclStatusCodeT ZbZclVoiceServerControlReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct voice_control_t *control_cmd, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceServerSendEstabRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct voice_estab_rsp_t *rsp);

/* The application calls this to send the Voice Transmission Reponse Command if it ever encounters an
 * error processing a Voice Transmission packet */
enum ZclStatusCodeT ZbZclVoiceServerSendVoiceTxRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst,
    struct voice_voice_tx_rsp_t *rsp);

#endif /* ZCL_VOICE_H */
