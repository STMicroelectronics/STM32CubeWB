/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Price cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_PRICE_H
# define ZCL_PRICE_H

#include "zcl/zcl.h"

enum ZbZclPriceServerAttrT {
    ZCL_PRICE_SVR_ATTR_START_OF_BLOCK_PERIOD = 0x0200,
    ZCL_PRICE_SVR_ATTR_BLOCK_PERIOD_DURATION = 0x0201,

    ZCL_PRICE_SVR_ATTR_COMMODITY_TYPE = 0x0300,
    ZCL_PRICE_SVR_ATTR_STANDING_CHARGE = 0x0301,

    /* Current Bill Period (CBP) */
    ZCL_PRICE_SVR_ATTR_CBP_START = 0x0700,
    ZCL_PRICE_SVR_ATTR_CBP_DURATION = 0x0701,
    /* Last Bill Period (LBP) */
    ZCL_PRICE_SVR_ATTR_LBP_START = 0x0702,
    ZCL_PRICE_SVR_ATTR_LBP_DURATION = 0x0703,
    ZCL_PRICE_SVR_ATTR_LBP_CONSOLIDATED_BILL = 0x0704
};

/* Price Cluster Attribute identifiers. */
#define ZCL_PRICE_SVR_ATTR_TIERN_LABEL(_x_)                          (0x0000U + ((_x_) - 1U))
#define ZCL_PRICE_SVR_ATTR_BLOCKN_THRESHOLD(_x_)                     (0x0100U + ((_x_) - 1U))

#define ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(_tier_, _blk_)         (0x0400U + ((_blk_) - 1U) + (0x10U * (_tier_)))
#define ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCKN_PRICE(_blk_)               ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(0U, _blk_)

/* Price Server Generated Commands */
enum ZbZclPriceServerCommandsT {
    ZCL_PRICE_SVR_CMD_PUB_PRICE = 0x00, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_BLK_PERIOD = 0x01,
    ZCL_PRICE_SVR_CMD_PUB_CONVERSION_FACTOR = 0x02,
    ZCL_PRICE_SVR_CMD_PUB_CALORIFIC_VALUE = 0x03,
    ZCL_PRICE_SVR_CMD_PUB_TARIFF_INFORMATION = 0x04, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_PRICE_MATRIX = 0x05, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_BLK_THRESHOLDS = 0x06, /* mandatory for GBCS ESI */
    ZCL_PRICE_SVR_CMD_PUB_CO2_VALUE = 0x07,
    ZCL_PRICE_SVR_CMD_PUB_TIER_LABELS = 0x08,
    ZCL_PRICE_SVR_CMD_PUB_BILLING_PERIOD = 0x09,
    ZCL_PRICE_SVR_CMD_PUB_CONSOLIDATED_BILL = 0x0a,
    ZCL_PRICE_SVR_CMD_PUB_CPP_EVENT = 0x0b,
    ZCL_PRICE_SVR_CMD_PUB_CREDIT_PAYMENT = 0x0c,
    ZCL_PRICE_SVR_CMD_PUB_CURRENCY_CONVERSION = 0x0d,
    ZCL_PRICE_SVR_CMD_PUB_CANCEL_TARIFF = 0x0e
};

/* Price Client Generated Commands */
enum ZbZclPriceClientCommandsT {
    ZCL_PRICE_CLI_CMD_GET_CURRENT_PRICE = 0x00, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_SCHEDULED_PRICES = 0x01,
    ZCL_PRICE_CLI_CMD_PRICE_ACKNOWLEDGEMENT = 0x02, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_BLOCK_PERIODS = 0x03,
    ZCL_PRICE_CLI_CMD_GET_CONVERSION_FACTOR = 0x04,
    ZCL_PRICE_CLI_CMD_GET_CALORIFIC_VALUE = 0x05,
    ZCL_PRICE_CLI_CMD_GET_TARIFF_INFORMATION = 0x06, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_PRICE_MATRIX = 0x07, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_BLOCK_THRESHOLDS = 0x08, /* mandatory for GBCS ESI */
    ZCL_PRICE_CLI_CMD_GET_CO2_VALUE = 0x09,
    ZCL_PRICE_CLI_CMD_GET_TIER_LABELS = 0x0a,
    ZCL_PRICE_CLI_CMD_GET_BILLING_PERIOD = 0x0b,
    ZCL_PRICE_CLI_CMD_GET_CONSOLIDATED_BILL = 0x0c,
    ZCL_PRICE_CLI_CMD_CPP_EVENT_RESPONSE = 0x0d,
    ZCL_PRICE_CLI_CMD_GET_CREDIT_PAYMENT = 0x0e,
    ZCL_PRICE_CLI_CMD_GET_CURRENCY_CONVERSION = 0x0f,
    ZCL_PRICE_CLI_CMD_GET_TARIFF_CANCELLATION = 0x10
};

/* Price Cluster get current flags. */
#define ZCL_PRICE_GET_FLAGS_RX_ON_IDLE           0x01U

/* Price tier label string lengths. */
#define ZCL_PRICE_LABEL_MAX_LENGTH               12U

#define ZCL_PRICE_TARIFF_LABEL_MAX_LENGTH        24U

/* Infinite price duration */
#define ZCL_PRICE_DURATION_INFINITE              0xffff

/* Alternate Cost units. */
#define ZCL_PRICE_ALTERNATE_KG_CO2               0x01

/*---------------------------------------------------------------
 * Structures
 *---------------------------------------------------------------
 */

/* ZCL_PRICE_SVR_CMD_PUB_PRICE */
#define ZCL_PRICE_PUBLISH_MIN_SIZE               28U /* assume rate_lable is only 1 byte */
#define ZCL_PRICE_PUBLISH_MAX_SIZE               59U

struct ZbZclPriceServerPublishPriceT {
    uint32_t provider_id;
    char rate_lable[ZCL_PRICE_LABEL_MAX_LENGTH + 1U];
    uint32_t issuer_event_id;
    uint32_t current_Time; /* UTC Time */
    uint8_t unit_of_measure;
    uint16_t currency;
    uint8_t trailing_digit_and_price_tier;
    uint8_t num_price_tiers;
    uint32_t start_time; /* UTC Time */
    uint16_t duration; /* minutes */
    uint32_t price;
    /* ZCL_PRICE_PUBLISH_MIN_SIZE */

    /* Optional parameters.
     * Value set to all 1's to indicate not present.
     * First parameter to be all 1's will cut-off the
     * response at that point. */
    uint8_t price_ratio;
    uint32_t generation_price;
    uint8_t generation_price_ratio;

    uint32_t alternate_cost_delivered;
    uint8_t alternate_cost_units;
    uint8_t alternate_cost_trail_digits;
    uint8_t num_block_thresholds;
    uint8_t price_control;

    uint8_t number_of_gen_tiers;
    uint8_t gen_tier;
    uint8_t ext_number_price_tiers;
    uint8_t ext_price_tier;
    uint8_t ext_register_tier;
};

/* ZCL_PRICE_SVR_CMD_PUB_TARIFF_INFORMATION */
struct ZbZclPriceServerPublishTariffInfoT {
    uint32_t provider_id;
    uint32_t issuer_event_id;
    uint32_t issuer_tariff_id;
    uint32_t start_time; /* UTC Time */
    uint8_t tariff_type_and_charging_scheme;
    char tariff_label[ZCL_PRICE_TARIFF_LABEL_MAX_LENGTH + 1U];
    uint8_t number_of_price_tiers_in_use;
    uint8_t number_of_block_thresh_in_use;
    uint8_t unit_of_measure;
    uint16_t currency;
    uint8_t price_trailing_digit;
    uint32_t standing_charge;
    uint8_t tier_block_mode;
    uint32_t block_thresh_multiplier; /* 24-bit */
    uint32_t block_thresh_divisor; /* 24-bit */
};

/* ZCL_PRICE_SVR_CMD_PUB_PRICE_MATRIX */
#define ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX       15U
#define ZCL_PRICE_SVR_PRICE_MATRIX_MAX_SIZE                  (19U + (5U * ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX))

struct ZbZclPriceServerPriceMatrixEntryT {
    uint8_t tier_block_id;
    uint32_t price;
};

struct ZbZclPriceServerPublishPriceMatrixT {
    uint32_t provider_id;
    uint32_t issuer_event_id;
    uint32_t start_time; /* UTC Time */
    uint32_t issuer_tariff_id;
    uint8_t command_index;
    uint8_t total_number_commands;
    uint8_t sub_payload_control;
    uint8_t num_sub_paylad;
    struct ZbZclPriceServerPriceMatrixEntryT sub_payload[ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX];
};

/* ZCL_PRICE_SVR_CMD_PUB_BLK_THRESHOLDS */
/* max tiers */
#define ZCL_PRICE_SVR_BLOCK_THRESH_NUM_SUB_PAYLOAD_MAX       15U

struct ZbZclPriceServerBlockThreshEntryT {
    uint8_t tier;
    uint64_t *block_thresh; /* array of 48-bit integers */
};

struct ZbZclPriceServerPublishBlockThresholdsT {
    uint32_t provider_id;
    uint32_t issuer_event_id;
    uint32_t start_time; /* UTC Time */
    uint32_t issuer_tariff_id;
    uint8_t command_index;
    uint8_t total_number_commands;
    uint8_t sub_payload_control;
    uint8_t num_sub_paylad;
    struct ZbZclPriceServerBlockThreshEntryT sub_payload[ZCL_PRICE_SVR_BLOCK_THRESH_NUM_SUB_PAYLOAD_MAX];
};

struct ZbZclPriceClientGetCurrentPriceT {
    uint8_t command_options;
};

struct ZbZclPriceClientPriceAckT {
    uint32_t provider_id;
    uint32_t issuer_event_id;
    uint32_t price_ack_time;
    uint8_t control;
};

struct ZbZclPriceClientGetTariffInfoT {
    uint32_t earliest_start_time;
    uint32_t min_issuer_event_id;
    uint8_t num_commands;
    uint8_t tariff_type;
};

struct ZbZclPriceClientGetPriceMatrixT {
    uint32_t issuer_tariff_id;
};

struct ZbZclPriceClientGetBlockThresholdsT {
    uint32_t issuer_tariff_id;
};

/*---------------------------------------------------------------
 * API Callback Types
 *---------------------------------------------------------------
 */
struct ZbZclPriceServerCallbacksT {
    enum ZclStatusCodeT (*get_current_price)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclPriceClientGetCurrentPriceT *req, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*price_ack)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclPriceClientPriceAckT *req, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*get_tariff_info)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclPriceClientGetTariffInfoT *req, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*get_price_matrix)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclPriceClientGetPriceMatrixT *req, struct ZbZclAddrInfoT *srcInfo);

    enum ZclStatusCodeT (*get_block_thresholds)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclPriceClientGetBlockThresholdsT *req, struct ZbZclAddrInfoT *srcInfo);

    /* Optional commands not parsed into a struct or specific callback */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
};

/*---------------------------------------------------------------
 * Price Server
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclPriceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPriceServerCallbacksT *callbacks, void *arg);

/* ZCL_PRICE_SVR_CMD_PUB_PRICE */
void ZbZclPriceServerPublishPriceInit(struct ZbZclPriceServerPublishPriceT *rsp);

enum ZclStatusCodeT ZbZclPriceServerSendPublishPrice(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclPriceServerPublishPriceT *rsp);

/* ZCL_PRICE_SVR_CMD_PUB_TARIFF_INFORMATION */
enum ZclStatusCodeT ZbZclPriceServerSendPublishTariffInfo(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclPriceServerPublishTariffInfoT *rsp);

/* ZCL_PRICE_SVR_CMD_PUB_PRICE_MATRIX */
enum ZclStatusCodeT ZbZclPriceServerSendPublishMatrix(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclPriceServerPublishPriceMatrixT *rsp);

/* ZCL_PRICE_SVR_CMD_PUB_BLK_THRESHOLDS */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBlockThresholds(struct ZbZclClusterT *clusterPtr,
    struct ZbZclAddrInfoT *dstInfo, struct ZbZclPriceServerPublishBlockThresholdsT *rsp);

/*---------------------------------------------------------------
 * Price Client
 *---------------------------------------------------------------
 */
struct ZbZclPriceClientCallbacksT {
    enum ZclStatusCodeT (*publish_price)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclPriceServerPublishPriceT *price, struct ZbZclAddrInfoT *srcInfo);

    /* Optional commands not parsed into a struct or specific callback */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
};

struct ZbZclClusterT * ZbZclPriceClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPriceClientCallbacksT *callbacks, void *arg);

/* ZCL_PRICE_CLI_CMD_GET_CURRENT_PRICE */
enum ZclStatusCodeT ZbZclPriceClientCommandGetCurrentPriceReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PRICE_CLI_CMD_GET_SCHEDULED_PRICES */
enum ZclStatusCodeT ZbZclPriceClientCommandGetScheduledPricesReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    uint32_t startTime, uint8_t maxPrices,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PRICE_CLI_CMD_PRICE_ACKNOWLEDGEMENT */
enum ZclStatusCodeT ZbZclPriceClientCommandPriceAckReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientPriceAckT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PRICE_CLI_CMD_GET_TARIFF_INFORMATION */
enum ZclStatusCodeT ZbZclPriceClientCommandGetTariffInfoReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetTariffInfoT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PRICE_CLI_CMD_GET_PRICE_MATRIX */
enum ZclStatusCodeT ZbZclPriceClientCommandGetPriceMatrixReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetPriceMatrixT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* ZCL_PRICE_CLI_CMD_GET_BLOCK_THRESHOLDS */
enum ZclStatusCodeT ZbZclPriceClientCommandGetBlockThresholdsReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetBlockThresholdsT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_PRICE_H */
