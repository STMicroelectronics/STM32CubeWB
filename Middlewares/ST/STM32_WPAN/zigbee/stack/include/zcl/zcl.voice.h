/* Copyright [2019 - 2019] Exegin Technologies Limited. All rights reserved. */

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
#define FLAG_CODEC_TYPE_SUB_1           0x01
#define FLAG_CODEC_TYPE_SUB_2           0x02
#define FLAG_CODEC_TYPE_SUB_3           0x04
#define FLAG_COMPRESSION_TYPE           0x08
#define FLAG_COMPRESSION_RATE           0x10

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
    uint8_t seq_no;
    uint8_t error_flag;
};

struct voice_control_t {
    uint8_t control_type;
};

struct voice_control_rsp_t {
    uint8_t ack_nak;
};

/* Callback Structures */
struct zcl_voice_server_callbacks_t {
    /* For voice_tx, the application should return ZCL_STATUS_SUCCESS for all received packets, which will send
     * a Default Response with SUCCESS. The ZCL7 spec is written such that it appears the server should not
     * send a response at all, and the application would return ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE and only
     * send ZbZclVoiceServerSendVoiceTxRsp() upon error. This doesn't follow the standard ZCL method, where all
     * requests should generate a reponse using the same ZCL sequence number. We'll leave this open to the
     * application developer to decide. */
    enum ZclStatusCodeT (*estab_req)(struct ZbZclClusterT *clusterPtr, struct voice_estab_req_t *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*voice_tx)(struct ZbZclClusterT *clusterPtr, struct voice_voice_tx_t *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*tx_complete)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *src_info, void *arg);
};

struct zcl_voice_client_callbacks_t {
    enum ZclStatusCodeT (*voice_tx_rsp)(struct ZbZclClusterT *clusterPtr, struct voice_voice_tx_rsp_t *cmd_rsp,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*control)(struct ZbZclClusterT *clusterPtr, struct voice_control_t *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
};

/* Allocation Functions */
struct ZbZclClusterT * ZbZclVoiceClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct zcl_voice_client_callbacks_t *callbacks, void *arg);
struct ZbZclClusterT * ZbZclVoiceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct zcl_voice_server_callbacks_t *callbacks, void *arg);

/* Client API */
enum ZclStatusCodeT ZbZclVoiceClientEstabReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct voice_estab_req_t *estab_req, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceVoiceTxReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct voice_voice_tx_t *voice_tx, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceTxCompletedReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceClientSendControlRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct voice_control_rsp_t *rsp);

/* Server API */
enum ZclStatusCodeT ZbZclVoiceServerControlReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    struct voice_control_t *control_cmd, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclVoiceServerSendEstabRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct voice_estab_rsp_t *rsp);

/* The application calls this to send the Voice Transmission Reponse Command if it ever encounters an
 * error processing a Voice Transmission packet */
enum ZclStatusCodeT ZbZclVoiceServerSendVoiceTxRsp(struct ZbZclClusterT *clusterPtr, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), struct voice_voice_tx_rsp_t *rsp, void *arg);

#endif /* ZCL_VOICE_H */
