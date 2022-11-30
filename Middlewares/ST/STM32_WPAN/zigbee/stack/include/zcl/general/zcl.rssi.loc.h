/**
 * @file zcl.rssi_loc.h
 * @heading RSSI Location
 * @brief ZCL RSSI Location cluster header
 * ZCL 7 section 3.13
 * ZCL 8 section 3.13
 * @copyright Copyright [2019 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_RSSI_LOC_H
#define ZCL_RSSI_LOC_H

/* NOTE: The following PICS statement is for internal Exegin use only, and is not a product of the Zigbee Alliance. */
/* @PICS.ZCL.Rssi_Loc
 * RSSI.S | Server | True
 * RSSI.C | Client | True
 *
 * Server Attributes
 * RSSI.S.A0000 | LocationType | True
 * RSSI.S.A0001 | LocationMethod | True
 * RSSI.S.A0002 | LocationAge | True | Optional
 * RSSI.S.A0003 | QualityMeasure | True | Optional
 * RSSI.S.A0004 | NumberOfDevices | True | Optional
 * RSSI.S.A0010 | Coordinate1 | True
 * RSSI.S.A0011 | Coordinate2 | True
 * RSSI.S.A0012 | Coordinate3 | True | Optional
 * RSSI.S.A0013 | Power | True
 * RSSI.S.A0014 | PathLossExponent |  True
 * RSSI.S.A0015 | ReportingPeriod | True | Optional
 * RSSI.S.A0016 | CalculationPeriod | True | Optional
 * RSSI.S.A0017 | NumberRSSIMeasurements | True
 * RSSI.S.Afffd | ClusterRevision | True
 * RSSI.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * RSSI.S.C00.Rsp | Set Absolute Location | True
 * RSSI.S.C01.Rsp | Set Device Configuration | True
 * RSSI.S.C02.Rsp | Get Device Configuration | True
 * RSSI.S.C03.Rsp | Get Location Data | True
 * RSSI.S.C04.Rsp | RSSI Response | True
 * RSSI.S.C05.Rsp | Send Pings | True
 * RSSI.S.C06.Rsp | Anchor Node Announce | True
 *
 * Commands Generated
 * RSSI.S.C00.Tx | Device configuration response | True
 * RSSI.S.C01.Tx | Location data response | True
 * RSSI.S.C02.Tx | Location data notification | True
 * RSSI.S.C03.Tx | Compact location data notification | True
 * RSSI.S.C04.Tx | RSSI Ping | True
 * RSSI.S.C05.Tx | RSSI Request | True
 * RSSI.S.C06.Tx | Report RSSI Measurements | True
 * RSSI.S.C07.Tx | Request Own Location | True
 *
 * Client Attributes
 * RSSI.C.Afffd | ClusterRevision | True
 * RSSI.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * RSSI.C.C00.Rsp | Device configuration response | True
 * RSSI.C.C01.Rsp | Location data response | True
 * RSSI.C.C02.Rsp | Location data notification | True
 * RSSI.C.C03.Rsp | Compact location data notification | True
 * RSSI.C.C04.Rsp | RSSI Ping | True
 * RSSI.C.C05.Rsp | RSSI Request | True
 * RSSI.C.C06.Rsp | Report RSSI Measurements | True
 * RSSI.C.C07.Rsp | Request Own Location | True
 *
 * Commands Generated
 * RSSI.C.C00.Tx | Set Absolute Location | True
 * RSSI.C.C01.Tx | Set Device Configuration | True
 * RSSI.C.C02.Tx | Get Device Configuration | True
 * RSSI.C.C03.Tx | Get Location Data | True
 * RSSI.C.C04.Tx | RSSI Response | True
 * RSSI.C.C05.Tx | Send Pings | True
 * RSSI.C.C06.Tx | Anchor Node Announce | True
 */

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the RSSI Location cluster.
 *--------------------------------------------------------------------------
 */

/** Rssi Location Attribute IDs */
enum ZbZclRssiLocSvrAttrT {
    ZCL_RSSI_LOC_SVR_ATTR_LOCATION_TYPE = 0x0000, /**< LocationType */
    ZCL_RSSI_LOC_SVR_ATTR_LOCATION_METHOD = 0x0001, /**< LocationMethod */
    ZCL_RSSI_LOC_SVR_ATTR_LOCATION_AGE = 0x0002, /**< LocationAge (Optional) */
    ZCL_RSSI_LOC_SVR_ATTR_QUALITY_MEAS = 0x0003, /**< QualityMeasure (Optional) */
    ZCL_RSSI_LOC_SVR_ATTR_NUM_DEVICES = 0x0004, /**< NumberOfDevices (Optional) */
    ZCL_RSSI_LOC_SVR_ATTR_COORD1 = 0x0010, /**< Coordinate1 */
    ZCL_RSSI_LOC_SVR_ATTR_COORD2 = 0x0011, /**< Coordinate2 */
    ZCL_RSSI_LOC_SVR_ATTR_COORD3 = 0x0012, /**< Coordinate3 (Optional) */
    ZCL_RSSI_LOC_SVR_ATTR_POWER = 0x0013, /**< Power */
    ZCL_RSSI_LOC_SVR_ATTR_PATH_LOSS_EXP = 0x0014, /**< PathLossExponent */
    ZCL_RSSI_LOC_SVR_ATTR_REPORT_PERIOD = 0x0015, /**< ReportingPeriod (Optional) */
    ZCL_RSSI_LOC_SVR_ATTR_CALC_PERIOD = 0x0016, /**< CalculationPeriod (Optional) */
    ZCL_RSSI_LOC_SVR_ATTR_NUM_RSSI_MEAS = 0x0017 /**< NumberRSSIMeasurements */
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

/* LocationMethod (ZCL_RSSI_LOC_SVR_ATTR_LOCATION_METHOD) */
enum ZbZclRssiLocMethodT {
    ZCL_RSSI_LOC_METHOD_LATERATION = 0x00,
    ZCL_RSSI_LOC_METHOD_SIGNPOSTING = 0x01,
    ZCL_RSSI_LOC_METHOD_RF_FINGERPRINTING = 0x02,
    ZCL_RSSI_LOC_METHOD_OUT_OF_BAND = 0x03,
    ZCL_RSSI_LOC_METHOD_CENTRALIZED = 0x04,
    ZCL_RSSI_LOC_METHOD_MANUF_SPEC_MIN = 0x40,
    ZCL_RSSI_LOC_METHOD_MANUF_SPEC_MAX = 0xff
};

#define ZCL_RSSI_LOC_LOC_DATA_ABS_ONLY          0x01
#define ZCL_RSSI_LOC_LOC_DATA_RECALCULATE       0x02
#define ZCL_RSSI_LOC_LOC_DATA_BROAD_IND         0x04
#define ZCL_RSSI_LOC_LOC_DATA_BROAD_RSP         0x08
#define ZCL_RSSI_LOC_LOC_DATA_COMP_RSP          0x10

#define ZCL_RSSI_LOC_MAX_NEIGHBOURS             16U

/* Structures */

/** Set Absolute Location command structure */
struct rssi_loc_set_abs_loc {
    int16_t coord1; /**< Coordinate 1 */
    int16_t coord2; /**< Coordinate 2 */
    int16_t coord3; /**< Coordinate 3 */
    int16_t power; /**< Power */
    uint16_t path_loss_exp; /**< Path Loss Exponent */
};

/** Set Device Configuration command structure */
struct rssi_loc_set_dev_config {
    int16_t power; /**< Power */
    uint16_t path_loss_exp; /**< Path Loss Exponent */
    uint16_t calc_period; /**< Calculation Period */
    uint8_t num_rssi_meas; /**< Number Rssi Measurements */
    uint16_t report_period; /**< Reporting Period */
};

/** Get Device Configuration command structure */
struct rssi_loc_get_dev_config {
    uint64_t target_addr; /**< Target Address */
};

/** Get Location Data command structure */
struct rssi_loc_get_loc_data {
    uint8_t bitmap; /**< Bitmap */
    uint8_t num_responses; /**< Number Responses */
    uint64_t target_addr; /**< Target Address */
};

/** Rssi response structure */
struct rssi_loc_rssi_rsp {
    uint64_t replying_dev; /**< Replying Device */
    int16_t x; /**< X */
    int16_t y; /**< Y */
    int16_t z; /**< Z */
    int8_t rssi; /**< RSSI */
    uint8_t num_rssi_meas; /**< NumberRSSIMeasurements */
};

/** Send Pings command structure */
struct rssi_loc_send_pings {
    uint64_t target_addr; /**< Target Address */
    uint8_t num_rssi_meas; /**< NumberRSSIMeasurements */
    uint16_t calc_period; /**< CalculationPeriod */
};

/** Anchor Node Announce command structure */
struct rssi_loc_anchor_node_annc {
    uint64_t addr; /**< Anchor Node IEEE Address */
    int16_t x; /**< X */
    int16_t y; /**< Y */
    int16_t z; /**< Z */
};

/** Device Configuration response structure */
struct rssi_loc_dev_config_rsp {
    enum ZclStatusCodeT status; /**< Status */
    int16_t power; /**< Power */
    uint16_t path_loss_exp; /**< Path Loss Exponent */
    uint16_t calc_period; /**< Calculation Period */
    uint8_t num_rssi_meas; /**< Number RSSI Measurements */
    uint16_t report_period; /**< Reporting Period */
};

/** Location Data response structure */
struct rssi_loc_loc_data_rsp {
    enum ZclStatusCodeT status; /**< Status */
    uint8_t loc_type; /**< Location Type */
    int16_t coord1; /**< Coordinate 1 */
    int16_t coord2; /**< Coordinate 2 */
    int16_t coord3; /**< Coordinate 3 */
    int16_t power; /**< Power */
    uint16_t path_loss_exp; /**< Path Loss Exponent */
    uint8_t loc_method; /**< Location Method */
    uint8_t quality_meas; /**< Quality Measure */
    uint16_t loc_age; /**< Location Age */
};

/** Location Data Notification command structure */
struct rssi_loc_loc_data_notif {
    uint8_t loc_type; /**< Location Type */
    int16_t coord1; /**< Coordinate 1 */
    int16_t coord2; /**< Coordinate 2 */
    int16_t coord3; /**< Coordinate 3 */
    int16_t power; /**< Power */
    uint16_t path_loss_exp; /**< Path Loss Exponent */
    uint8_t loc_method; /**< Location Method */
    uint8_t quality_meas; /**< Quality Measure */
    uint16_t loc_age; /**< Location Age */
};

/** Compact Data Notification command structure */
struct rssi_loc_comp_data_notif {
    uint8_t loc_type; /**< Location Type */
    int16_t coord1; /**< Coordinate 1 */
    int16_t coord2; /**< Coordinate 2 */
    int16_t coord3; /**< Coordinate 3 */
    uint8_t quality_meas; /**< Quality Measure */
    uint16_t loc_age; /**< Location Age */
};

/** Rssi Ping command structure */
struct rssi_loc_rssi_ping {
    uint8_t loc_type; /**< Location Type */
};

/** Rssi Request structure */
struct rssi_loc_rssi_req {
    uint8_t rssi; /**< Rssi */
};

/** Neighbour Info structure */
struct rssi_loc_neighbour_info {
    uint64_t neighbour; /**< Neighbour */
    int16_t x; /**< X */
    int16_t y; /**< Y */
    int16_t z; /**< Z */
    int8_t rssi; /**< RSSI */
    uint8_t num_rssi_meas; /**< NumberRSSIMeasurements */
};

/** Report Rssi command structure */
struct rssi_loc_report_rssi {
    uint64_t measuring_dev; /**< Measuring Device */
    uint8_t n_neighbours; /**< N Neighbours */
    struct rssi_loc_neighbour_info neighbours_info[ZCL_RSSI_LOC_MAX_NEIGHBOURS]; /**< NeighboursInfo */
};

/** Request Own Location command structure */
struct rssi_loc_req_own_loc {
    uint64_t addr; /**< IEEE Address of the Blind Node */
};

/** Rssi Location Server callbacks configuration */
struct zcl_rssi_loc_server_callbacks_t {
    enum ZclStatusCodeT (*get_dev_config)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_get_dev_config *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Get Device Configuration command */
    enum ZclStatusCodeT (*get_loc_data)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_get_loc_data *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Get Location Data command */
    enum ZclStatusCodeT (*send_pings)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_send_pings *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Send Pings command */
    enum ZclStatusCodeT (*anchor_node_annc)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_anchor_node_annc *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Anchor Node Announce command */
};

/** Rssi Location Client callbacks configuration */
struct zcl_rssi_loc_client_callbacks_t {
    enum ZclStatusCodeT (*loc_data_notif)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_loc_data_notif *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Location Data Notification command */
    enum ZclStatusCodeT (*comp_data_notif)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_comp_data_notif *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Compact Data Notification command */
    enum ZclStatusCodeT (*rssi_ping)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_rssi_ping *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of RSSI Ping command */
    enum ZclStatusCodeT (*rssi_req)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_rssi_req *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of RSSI request. */
    enum ZclStatusCodeT (*report_rssi)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_report_rssi *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Report Rssi command */
    enum ZclStatusCodeT (*req_own_loc)(struct ZbZclClusterT *clusterPtr, struct rssi_loc_req_own_loc *cmd_req,
        struct ZbZclAddrInfoT *src_info, void *arg);
    /**< Callback to application, invoked on receipt of Request Own Location command */
};

/* Allocation Functions */

/**
 * Create a new instance of the RSSI Location Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclRssiLocClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct zcl_rssi_loc_client_callbacks_t *callbacks, void *arg);

/**
 * Create a new instance of the RSSI Location Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclRssiLocServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct zcl_rssi_loc_server_callbacks_t *callbacks, void *arg);

/* Client API */

/**
 * Send a Set Absolute Location command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param set_abs_loc Set Absolute Location command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocClientSetAbsLocation(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_set_abs_loc *set_abs_loc, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Set Device Configuration command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param set_dev_config Set Device Config command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocClientSetDevConfig(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_set_dev_config *set_dev_config, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Device Configuration command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param get_dev_config Get Device Configuration command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocClientGetDevConfig(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_get_dev_config *get_dev_config, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Location Data command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param get_loc_data Get Location Data command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocClientGetLocData(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_get_loc_data *get_loc_data, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a RSSI Response command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst_info Destination address for request
 * @param rsp Rssi Response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocClientSendRssiRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct rssi_loc_rssi_rsp *rsp);

/**
 * Send a Send Pings command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param send_pings Send Pings command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocClientSendPings(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_send_pings *send_pings, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Anchor Node Announce command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param anchor_node_annc Anchor Node Announce command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocClientAnchorNodeAnnc(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_anchor_node_annc *anchor_node_annc, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Server API */

/**
 * Send a Device Configuration Response command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst_info Destination address for request
 * @param rsp Send Device Configuration response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerSendDevConfigRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct rssi_loc_dev_config_rsp *rsp);

/**
 * Send a Location Data Response command
 * @param clusterPtr Cluster instance from which to send this command
 * @param dst_info Destination address for request
 * @param rsp Send Location Data response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerSendLocDataRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dst_info,
    struct rssi_loc_loc_data_rsp *rsp);

/**
 * Send a Location Data Notification command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerLocDataNotif(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Compact Data Notification command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerCompDataNotif(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a RSSI Ping command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerRssiPing(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a RSSI Request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerRssiReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Report RSSI command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param report_rssi Report Rssi command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerReportRssi(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_report_rssi *report_rssi, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Request Own Location command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req_own_loc Request Own Location command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclRssiLocServerReqOwnLoc(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    struct rssi_loc_req_own_loc *req_own_loc, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif
