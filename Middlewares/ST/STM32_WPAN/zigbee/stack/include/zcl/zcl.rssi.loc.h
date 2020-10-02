/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_RSSI_LOC_H
#define ZCL_RSSI_LOC_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the RSSI Location cluster.
 *--------------------------------------------------------------------------
 */

/* Attribute Identifiers */
enum ZbZclRssiLocSvrAttrT {
    ZCL_RSSI_LOC_SVR_ATTR_LOCATION_TYPE = 0x0000,
    ZCL_RSSI_LOC_SVR_ATTR_LOCATION_METHOD = 0x0001,
    ZCL_RSSI_LOC_SVR_ATTR_LOCATION_AGE = 0x0002,
    ZCL_RSSI_LOC_SVR_ATTR_QUALITY_MEAS = 0x0003,
    ZCL_RSSI_LOC_SVR_ATTR_NUM_DEVICES = 0x0004,
    ZCL_RSSI_LOC_SVR_ATTR_COORD1 = 0x0010,
    ZCL_RSSI_LOC_SVR_ATTR_COORD2 = 0x0011,
    ZCL_RSSI_LOC_SVR_ATTR_COORD3 = 0x0012,
    ZCL_RSSI_LOC_SVR_ATTR_POWER = 0x0013,
    ZCL_RSSI_LOC_SVR_ATTR_PATH_LOSS_EXP = 0x0014,
    ZCL_RSSI_LOC_SVR_ATTR_REPORT_PERIOD = 0x0015,
    ZCL_RSSI_LOC_SVR_ATTR_CALC_PERIOD = 0x0016,
    ZCL_RSSI_LOC_SVR_ATTR_NUM_RSSI_MEAS = 0x0017
};

/* Commands */
#define ZCL_RSSI_LOC_CLI_CMD_SET_ABS_LOC        0x00
#define ZCL_RSSI_LOC_CLI_CMD_SET_DEV_CONFIG     0x01
#define ZCL_RSSI_LOC_CLI_CMD_GET_DEV_CONFIG     0x02
#define ZCL_RSSI_LOC_CLI_CMD_GET_LOC_DATA       0x03
#define ZCL_RSSI_LOC_CLI_CMD_RSSI_RSP           0x04
#define ZCL_RSSI_LOC_CLI_CMD_SEND_PINGS         0x05
#define ZCL_RSSI_LOC_CLI_CMD_ANCHOR_NODE_ANNC   0x06

#define ZCL_RSSI_LOC_CLI_CMD_DEV_CONFIG_RSP     0x00
#define ZCL_RSSI_LOC_CLI_CMD_LOC_DATA_RSP       0x01
#define ZCL_RSSI_LOC_CLI_CMD_LOC_DATA_NOTIF     0x02
#define ZCL_RSSI_LOC_CLI_CMD_COMP_DATA_NOTIF    0x03
#define ZCL_RSSI_LOC_CLI_CMD_RSSI_PING          0x04
#define ZCL_RSSI_LOC_CLI_CMD_RSSI_REQUEST       0x05
#define ZCL_RSSI_LOC_CLI_CMD_REPORT_RSSI        0x06
#define ZCL_RSSI_LOC_CLI_CMD_REQ_OWN_LOC        0x07

/* Attribute Values */
#define ZCL_RSSI_LOC_LOC_TYPE_ABSOLUTE          0x01 /* 1 = Abs, 0 = Measured */
#define ZCL_RSSI_LOC_LOC_TYPE_2D                0x02 /* 1 = 2D, 0 = 3D */
#define ZCL_RSSI_LOC_LOC_TYPE_COORD_MASK        0x0c
#define ZCL_RSSI_LOC_LOC_TYPE_COORD_RECT        0x00

#define ZCL_RSSI_LOC_LOC_DATA_ABS_ONLY          0x01
#define ZCL_RSSI_LOC_LOC_DATA_RECALCULATE       0x02
#define ZCL_RSSI_LOC_LOC_DATA_BROAD_IND         0x04
#define ZCL_RSSI_LOC_LOC_DATA_BROAD_RSP         0x08
#define ZCL_RSSI_LOC_LOC_DATA_COMP_RSP          0x10

#define ZCL_RSSI_LOC_MAX_NEIGHBOURS             16U

/* Structures */
struct rssi_loc_set_abs_loc {
    int16_t coord1;
    int16_t coord2;
    int16_t coord3;
    int16_t power;
    uint16_t path_loss_exp;
};

struct rssi_loc_set_dev_config {
    int16_t power;
    uint16_t path_loss_exp;
    uint16_t calc_period;
    uint8_t num_rssi_meas;
    uint16_t report_period;
};

struct rssi_loc_get_dev_config {
    uint64_t target_addr;
};

struct rssi_loc_get_loc_data {
    uint8_t bitmap;
    uint8_t num_responses;
    uint64_t target_addr;
};

struct rssi_loc_rssi_rsp {
    uint64_t replying_dev;
    int16_t x;
    int16_t y;
    int16_t z;
    int8_t rssi;
    uint8_t num_rssi_meas;
};

struct rssi_loc_send_pings {
    uint64_t target_addr;
    uint8_t num_rssi_meas;
    uint16_t calc_period;
};

struct rssi_loc_anchor_node_annc {
    uint64_t addr;
    int16_t x;
    int16_t y;
    int16_t z;
};

struct rssi_loc_dev_config_rsp {
    enum ZclStatusCodeT status;
    int16_t power;
    uint16_t path_loss_exp;
    uint16_t calc_period;
    uint8_t num_rssi_meas;
    uint16_t report_period;
};

struct rssi_loc_loc_data_rsp {
    enum ZclStatusCodeT status;
    uint8_t loc_type;
    int16_t coord1;
    int16_t coord2;
    int16_t coord3;
    int16_t power;
    uint16_t path_loss_exp;
    uint8_t loc_method;
    uint8_t quality_meas;
    uint16_t loc_age;
};

struct rssi_loc_loc_data_notif {
    uint8_t loc_type;
    int16_t coord1;
    int16_t coord2;
    int16_t coord3;
    int16_t power;
    uint16_t path_loss_exp;
    uint8_t loc_method;
    uint8_t quality_meas;
    uint16_t loc_age;
};

struct rssi_loc_comp_data_notif {
    uint8_t loc_type;
    int16_t coord1;
    int16_t coord2;
    int16_t coord3;
    uint8_t quality_meas;
    uint16_t loc_age;
};

struct rssi_loc_rssi_ping {
    uint8_t loc_type;
};

struct rssi_loc_rssi_req {
    uint8_t rssi;
};

struct rssi_loc_neighbour_info {
    uint64_t neighbour;
    int16_t x;
    int16_t y;
    int16_t z;
    int8_t rssi;
    uint8_t num_rssi_meas;
};

struct rssi_loc_report_rssi {
    uint64_t measuring_dev;
    uint8_t n_neighbours;
    struct rssi_loc_neighbour_info neighbours_info[ZCL_RSSI_LOC_MAX_NEIGHBOURS];
};

struct rssi_loc_req_own_loc {
    uint64_t addr;
};

/* Callbacks */
struct zcl_rssi_loc_server_callbacks_t {
    enum ZclStatusCodeT (*get_dev_config)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_get_dev_config *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*get_loc_data)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_get_loc_data *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*send_pings)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_send_pings *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*anchor_node_annc)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_anchor_node_annc *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
};

struct zcl_rssi_loc_client_callbacks_t {
    enum ZclStatusCodeT (*loc_data_notif)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_loc_data_notif *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*comp_data_notif)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_comp_data_notif *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*rssi_ping)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_rssi_ping *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*rssi_req)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_rssi_req *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*report_rssi)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_report_rssi *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    enum ZclStatusCodeT (*req_own_loc)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_req_own_loc *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
};

/* Allocation Functions */
struct ZbZclClusterT * ZbZclRssiLocClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, struct zcl_rssi_loc_client_callbacks_t *callbacks, void *arg);
struct ZbZclClusterT * ZbZclRssiLocServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, struct zcl_rssi_loc_server_callbacks_t *callbacks, void *arg);

/* Client API */
enum ZclStatusCodeT ZbZclRssiLocClientSetAbsLocation(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_set_abs_loc *set_abs_loc, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocClientSetDevConfig(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_set_dev_config *set_dev_config, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocClientGetDevConfig(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_get_dev_config *get_dev_config, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocClientGetLocData(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_get_loc_data *get_loc_data, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocClientSendRssiRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct rssi_loc_rssi_rsp *rsp);

enum ZclStatusCodeT ZbZclRssiLocClientSendPings(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_send_pings *send_pings, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocClientAnchorNodeAnnc(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_anchor_node_annc *anchor_node_annc, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Server API */

enum ZclStatusCodeT ZbZclRssiLocServerSendDevConfigRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct rssi_loc_dev_config_rsp *rsp);

enum ZclStatusCodeT ZbZclRssiLocServerSendLocDataRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct rssi_loc_loc_data_rsp *rsp);

enum ZclStatusCodeT ZbZclRssiLocServerLocDataNotif(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocServerCompDataNotif(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocServerRssiPing(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocServerRssiReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocServerReportRssi(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_report_rssi *report_rssi, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclRssiLocServerReqOwnLoc(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_req_own_loc *req_own_loc, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_RSSI_LOC_H */
