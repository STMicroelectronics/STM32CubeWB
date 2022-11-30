/**
 * @file zcl.price.h
 * @heading Price
 * @brief ZCL Price cluster header
 * ZCL 7 section 10.2
 * ZCL 8 section 10.2
 * @copyright Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved.
 */

/* EXEGIN - removed '@'PICS escape sequence, since these are not the
 * official PICS codes. These were added with the assumption that
 * SE clusters are being brought into the Zigbee 3.0 fold and will
 * use the same PICS codes and PICS format style. However, this has
 * yet to occur. */

/* ZCL.Price
 * SEPR.S | Server | True
 * SEPR.C | Client | True
 *
 * Server Attributes
 * SEPR.S.A0000 | Tier(1-48)PriceLabel | False | Optional
 * SEPR.S.A0100 | Block(1-48)Threshold | False | Optional
 * SEPR.S.A0200 | StartofBlockPeriod | False | Optional
 * SEPR.S.A0201 | BlockPeriodDuration | False | Optional
 * SEPR.S.A0300 | CommodityType | False | Optional
 * SEPR.S.A0301 | StandingCharge | False | Optional
 * SEPR.S.A0400 | NoTierBlock(1-6)Price | False | Optional
 * SEPR.S.A0410 | Tier(1-15)Block(1-16)Price | False | Optional
 * SEPR.S.A0700 | CurrentBillingPeriodStart | False | Optional
 * SEPR.S.A0701 | CurrentBillingPeriodDuration | False | Optional
 * SEPR.S.A0702 | LastBillingPeriodStart | False | Optional
 * SEPR.S.A0703 | LastBillingPeriodDuration | False | Optional
 * SEPR.S.A0704 | LastBillingPeriodConsolidatedBill | False | Optional
 * SEPR.S.Afffd | ClusterRevision | True
 * SEPR.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEPR.S.C00.Rsp | Get Current Price | False
 * SEPR.S.C01.Rsp | Get Scheduled Prices | False
 * SEPR.S.C02.Rsp | Price Acknowledgement | False
 * SEPR.S.C03.Rsp | Get Block Period(s) | False
 * SEPR.S.C04.Rsp | GetConversionFactor | False
 * SEPR.S.C05.Rsp | GetCalorificValue | False
 * SEPR.S.C06.Rsp | GetTariffInformation | False
 * SEPR.S.C07.Rsp | GetPriceMatrix | False
 * SEPR.S.C08.Rsp | GetBlockThresholds | False
 * SEPR.S.C09.Rsp | GetCO2Value | False
 * SEPR.S.C0a.Rsp | GetTierLabels | False
 * SEPR.S.C0b.Rsp | GetBillingPeriod | False
 * SEPR.S.C0c.Rsp | GetConsolidatedBill | False
 * SEPR.S.C0d.Rsp | CPPEventResponse | False
 * SEPR.S.C0e.Rsp | GetCreditPayment | False
 * SEPR.S.C0f.Rsp | GetCurrencyConversion | False
 * SEPR.S.C10.Rsp | GetTariffCancellation | False
 *
 * Commands Generated
 * SEPR.S.C00.Tx | Publish Price | True
 * SEPR.S.C01.Tx | Publish Block Period | False
 * SEPR.S.C02.Tx | Publish Conversion Factor | False
 * SEPR.S.C03.Tx | Publish Calorific Value | False
 * SEPR.S.C04.Tx | PublishTariffInformation | True
 * SEPR.S.C05.Tx | PublishPriceMatrix | True
 * SEPR.S.C06.Tx | PublishBlockThresholds | True
 * SEPR.S.C07.Tx | PublishCO2Value | False
 * SEPR.S.C08.Tx | PublishTierLabels | False
 * SEPR.S.C09.Tx | PublishBillingPeriod | False
 * SEPR.S.C0a.Tx | PublishConsolidatedBill | False
 * SEPR.S.C0b.Tx | PublishCPPEvent | False
 * SEPR.S.C0c.Tx | PublishCreditPayment | False
 * SEPR.S.C0d.Tx | PublishCurrencyConversion | False
 * SEPR.S.C0e.Tx | CancelTariff | False
 *
 * Client Attributes
 * SEPR.C.A0000 | PriceIncreaseRandomizeMinutes | False | Optional
 * SEPR.C.A0001 | PriceDecreaseRandomizeMinutes | False | Optional
 * SEPR.C.A0002 | CommodityType | False | Optional
 * SEPR.C.Afffd | ClusterRevision | True
 * SEPR.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * SEPR.C.C00.Rsp | Publish Price | False
 * SEPR.C.C01.Rsp | Publish Block Period | False
 * SEPR.C.C02.Rsp | Publish Conversion Factor | False
 * SEPR.C.C03.Rsp | Publish Calorific Value | False
 * SEPR.C.C04.Rsp | PublishTariffInformation | False
 * SEPR.C.C05.Rsp | PublishPriceMatrix | False
 * SEPR.C.C06.Rsp | PublishBlockThresholds | False
 * SEPR.C.C07.Rsp | PublishCO2Value | False
 * SEPR.C.C08.Rsp | PublishTierLabels | False
 * SEPR.C.C09.Rsp | PublishBillingPeriod | False
 * SEPR.C.C0a.Rsp | PublishConsolidatedBill | False
 * SEPR.C.C0b.Rsp | PublishCPPEvent | False
 * SEPR.C.C0c.Rsp | PublishCreditPayment | False
 * SEPR.C.C0d.Rsp | PublishCurrencyConversion | False
 * SEPR.C.C0e.Rsp | CancelTariff | False
 *
 * Commands Generated
 * SEPR.C.C00.Tx | Get Current Price | True
 * SEPR.C.C01.Tx | Get Scheduled Prices | True
 * SEPR.C.C02.Tx | Price Acknowledgement | True
 * SEPR.C.C03.Tx | Get Block Period(s) | False
 * SEPR.C.C04.Tx | GetConversionFactor | False
 * SEPR.C.C05.Tx | GetCalorificValue | False
 * SEPR.C.C06.Tx | GetTariffInformation | True
 * SEPR.C.C07.Tx | GetPriceMatrix | True
 * SEPR.C.C08.Tx | GetBlockThresholds | True
 * SEPR.C.C09.Tx | GetCO2Value | False
 * SEPR.C.C0a.Tx | GetTierLabels | False
 * SEPR.C.C0b.Tx | GetBillingPeriod | False
 * SEPR.C.C0c.Tx | GetConsolidatedBill | False
 * SEPR.C.C0d.Tx | CPPEventResponse | False
 * SEPR.C.C0e.Tx | GetCreditPayment | False
 * SEPR.C.C0f.Tx | GetCurrencyConversion | False
 * SEPR.C.C10.Tx | GetTariffCancellation | False
 */

#ifndef ZCL_PRICE_H
# define ZCL_PRICE_H

#include "zcl/zcl.h"
#include "zcl/se/zcl.meter.h" /* ZbZclMeterUnitsT */

/** Price Server Attribute IDs */
enum ZbZclPriceSvrAttrT {
    /* Tier Label (Delivered) Set (0x00) */
    ZCL_PRICE_SVR_ATTR_TIER1_LABEL = 0x0000,
    /**< Tier1PriceLabel (Optional)
     * ZCL_PRICE_SVR_ATTR_TIERN_LABEL(1)
     * For all the tiers, use the ZCL_PRICE_SVR_ATTR_TIERN_LABEL(tier) macro. */

    /* Block Threshold (Delivered) Set (0x01) */
    ZCL_PRICE_SVR_ATTR_BLOCK1_THRESHOLD = 0x0100,
    /**< Block1Threshold (Optional)
     * ZCL_PRICE_SVR_ATTR_BLOCKN_THRESHOLD(1)
     * For all the blocks, use the ZCL_PRICE_SVR_ATTR_BLOCKN_THRESHOLD(block) macro. */

    /* Block Period (Delivered) Set (0x02) */
    ZCL_PRICE_SVR_ATTR_START_OF_BLOCK_PERIOD = 0x0200, /**< StartofBlockPeriod (Optional) */
    ZCL_PRICE_SVR_ATTR_BLOCK_PERIOD_DURATION = 0x0201, /**< BlockPeriodDuration (Optional) */

    /* Commodity Set (0x03) */
    ZCL_PRICE_SVR_ATTR_COMMODITY_TYPE = 0x0300, /**< CommodityType (Optional) */
    ZCL_PRICE_SVR_ATTR_STANDING_CHARGE = 0x0301, /**< StandingCharge (Optional) */

    /* Block Price Information (Delivered) Set (0x04) */
    ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCK1_PRICE = 0x0400,
    /**< NoTierBlock1Price (Optional)
     * ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCKN_PRICE(1)
     * For all the blocks, use the ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCKN_PRICE(block) macro. */

    ZCL_PRICE_SVR_ATTR_TIER1_BLOCK1_PRICE = 0x0401,
    /**< Tier1Block1Price (Optional)
     * ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(1,1)
     * For all the tiers and blocks, use the ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(tier, block) macro. */

    /* Extended Price Information (Delivered) Set (0x05) */
    /* Tariff Information (Delivered) Attribute Set (0x06) */

    /* Billing Information (Delivered) Attribute Set (0x07) */
    ZCL_PRICE_SVR_ATTR_CBP_START = 0x0700, /**< CurrentBillingPeriodStart (Optional) */
    ZCL_PRICE_SVR_ATTR_CBP_DURATION = 0x0701, /**< CurrentBillingPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_LBP_START = 0x0702, /**< LastBillingPeriodStart (Optional) */
    ZCL_PRICE_SVR_ATTR_LBP_DURATION = 0x0703, /**< LastBillingPeriodDuration (Optional) */
    ZCL_PRICE_SVR_ATTR_LBP_CONSOLIDATED_BILL = 0x0704 /**< LastBillingPeriodConsolidatedBill (Optional) */

        /* Credit Payment Attribute Set (0x08) */
        /* Received Tier Label Set (0x80) */
        /* Received Block Threshold Set (0x81) */
        /* Received Block Period Set (0x82) */
        /* Received Block Price Information Set (0x83) */
        /* Received Extended Price Information Set (0x84) */
        /* Received Tariff Information Set (0x85) */
        /* Received Billing Information Set (0x86) */
};

/* Price Cluster Attribute identifiers. */
/* Tiers start at 1 and go to a max of 48. */
#define ZCL_PRICE_SVR_ATTR_TIERN_LABEL(tier)                           (0x0000U + ((tier) - 1U))
#define ZCL_PRICE_SVR_ATTR_BLOCKN_THRESHOLD(block)                     (0x0100U + ((block) - 1U))

#define ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(tier, block)             (0x0400U + ((block) - 1U) + (0x10U * (tier)))
#define ZCL_PRICE_SVR_ATTR_NO_TIER_BLOCKN_PRICE(block)                 ZCL_PRICE_SVR_ATTR_TIERN_BLOCKN_PRICE(0U, block)

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

/* Currency, from ISO 4217 */
enum ZbZclPriceCurrencyCodeT {
    ZCL_PRICE_CURRENCY_CODE_AUD = 36, /* Australian dollar */
    ZCL_PRICE_CURRENCY_CODE_CAD = 124, /* Canadian dollar */
    ZCL_PRICE_CURRENCY_CODE_CNY = 156, /* Chinese yuan */
    ZCL_PRICE_CURRENCY_CODE_EUR = 978, /* Euro */
    ZCL_PRICE_CURRENCY_CODE_GBP = 826, /* Pound sterling */
    ZCL_PRICE_CURRENCY_CODE_HKD = 344, /* Hong Kong dollar */
    ZCL_PRICE_CURRENCY_CODE_INR = 356, /* Indian rupee */
    ZCL_PRICE_CURRENCY_CODE_RUB = 643, /* Russian ruble */
    ZCL_PRICE_CURRENCY_CODE_USD = 840, /* United States dollar */
    ZCL_PRICE_CURRENCY_CODE_XTS = 963, /* Code reserved for testing */
    ZCL_PRICE_CURRENCY_CODE_XXX = 999 /* No currency */
};

/* Price Cluster Structures */

/* ZCL_PRICE_SVR_CMD_PUB_PRICE */
#define ZCL_PRICE_PUBLISH_MIN_SIZE               28U /* assume rate_lable is only 1 byte */
#define ZCL_PRICE_PUBLISH_MAX_SIZE               59U

/* Table D-100 Price Control Field BitMap */
#define ZCL_PRICE_CTRL_MASK_PRICE_ACK               0x01 /* Price Acknowledgement required */
#define ZCL_PRICE_CTRL_MASK_TOTAL_TIERS_GT_15       0x02 /* Total Tiers exceeds the 15 specified in the command */

/** Publish Price command structure */
struct ZbZclPriceServerPublishPriceT {
    uint32_t provider_id; /**< Provider ID */
    char rate_lable[ZCL_PRICE_LABEL_MAX_LENGTH + 1U]; /**< Rate Label array */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t current_Time; /**< Current Time - UTC Time */
    enum ZbZclMeterUnitsT unit_of_measure; /**< Unit of Measure */
    enum ZbZclPriceCurrencyCodeT currency; /**< Currency */
    uint8_t trailing_digit_and_price_tier; /**< Price Trailing Digit & Price Tier */
    uint8_t num_price_tiers; /**< Number of Price Tiers & Register Tier */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint16_t duration; /**< Duration in Minutes */
    uint32_t price; /**< Price */
    /* ZCL_PRICE_PUBLISH_MIN_SIZE */

    /* Optional parameters.
     * Value set to all 1's to indicate not present.
     * First parameter to be all 1's will cut-off the
     * response at that point. */
    uint8_t price_ratio; /**< Price Ratio (Optional) */
    uint32_t generation_price; /**< Generation Price (Optional) */
    uint8_t generation_price_ratio; /**< Generation Price Ratio (Optional) */

    uint32_t alternate_cost_delivered; /**< Alternate Cost Delivered (Optional) */
    uint8_t alternate_cost_units; /**< Alternate Cost Unit (Optional) */
    uint8_t alternate_cost_trail_digits; /**< Alternate Cost Trailing Digit (Optional) */
    uint8_t num_block_thresholds; /**< Number of Block Thresholds (Optional) */
    uint8_t price_control; /**< Price Control (Optional) */

    uint8_t number_of_gen_tiers; /**< Number of Generation Tiers (Optional) */
    uint8_t gen_tier; /**< Generation Tier (Optional) */
    uint8_t ext_number_price_tiers; /**< Extended Number of Price Tiers (Optional) */
    uint8_t ext_price_tier; /**< Extended Price Tier (Optional) */
    uint8_t ext_register_tier; /**< Extended Register Tier (Optional) */
};

/** Publish Tariff Info command structure */
struct ZbZclPriceServerPublishTariffInfoT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint8_t tariff_type_and_charging_scheme; /**< Tariff Type/Charging Scheme */
    char tariff_label[ZCL_PRICE_TARIFF_LABEL_MAX_LENGTH + 1U]; /**< Tariff Label array */
    uint8_t number_of_price_tiers_in_use; /**< Number of Price Tiers in Use */
    uint8_t number_of_block_thresh_in_use; /**< Number of Block Thresholds in Use */
    uint8_t unit_of_measure; /**< Unit of Measure */
    uint16_t currency; /**< Currency */
    uint8_t price_trailing_digit; /**< Price Trailing Digit */
    uint32_t standing_charge; /**< Standing Charge */
    uint8_t tier_block_mode; /**< TierBlockMode */
    uint32_t block_thresh_multiplier; /**< Block Threshold Multiplier - 24-bit */
    uint32_t block_thresh_divisor; /**< Block Threshold Divisor - 24-bit */
};

/* ZCL_PRICE_SVR_CMD_PUB_PRICE_MATRIX */
#define ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX       15U
#define ZCL_PRICE_SVR_PRICE_MATRIX_MAX_SIZE                  (19U + (5U * ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX))

/** Price Matrix Entry structure */
struct ZbZclPriceServerPriceMatrixEntryT {
    uint8_t tier_block_id; /**< Tier/Block ID */
    uint32_t price; /**< Price */
};

/** Publish Price Matrix command structure */
struct ZbZclPriceServerPublishPriceMatrixT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
    uint8_t command_index; /**< Command Index */
    uint8_t total_number_commands; /**< Total Number of Commands */
    uint8_t sub_payload_control; /**< Sub-payload Control */
    uint8_t num_sub_paylad; /**< Number of items in Price Matrix Sub-payload */
    struct ZbZclPriceServerPriceMatrixEntryT sub_payload[ZCL_PRICE_SVR_PRICE_MATRIX_NUM_SUB_PAYLOAD_MAX];
    /**< Price Matrix Sub-payload */
};

/* ZCL_PRICE_SVR_CMD_PUB_BLK_THRESHOLDS */
/* max tiers */
#define ZCL_PRICE_SVR_BLOCK_THRESH_NUM_SUB_PAYLOAD_MAX       15U

/** Block Threshold Entry structure */
struct ZbZclPriceServerBlockThreshEntryT {
    uint8_t tier; /**< Tier */
    uint64_t *block_thresh; /**< Block Threshold */
};

/** Publish Block Thresholds command structure */
struct ZbZclPriceServerPublishBlockThresholdsT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t start_time; /**< Start Time - UTC Time */
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID*/
    uint8_t command_index; /**< Command Index */
    uint8_t total_number_commands; /**< Total Number of Commands */
    uint8_t sub_payload_control; /**< Sub-payload Control */
    uint8_t num_sub_paylad; /**< Number of items in Block Threshold Sub-payload */
    struct ZbZclPriceServerBlockThreshEntryT sub_payload[ZCL_PRICE_SVR_BLOCK_THRESH_NUM_SUB_PAYLOAD_MAX];
    /**< Block Threshold Sub-payload */
};

/** Get Current Price command structure */
struct ZbZclPriceClientGetCurrentPriceT {
    uint8_t command_options; /**< Command Options */
};

/** Price Acknowledge command structure */
struct ZbZclPriceClientPriceAckT {
    uint32_t provider_id; /**< Provider ID */
    uint32_t issuer_event_id; /**< Issuer Event ID */
    uint32_t price_ack_time; /**< Price Ack Time */
    uint8_t control; /**< Control. E.g. ZCL_PRICE_CTRL_MASK_PRICE_ACK */
};

/** Get Scheduled Prices command structure */
struct ZbZclPriceClientGetScheduledPricesT {
    uint32_t startTime; /**< StartTime */
    uint8_t maxPrices; /**< NumberofEvents */
};

/** Get Tariff Information command structure */
struct ZbZclPriceClientGetTariffInfoT {
    uint32_t earliest_start_time; /**< Earliest Start Time */
    uint32_t min_issuer_event_id; /**< Min. Issuer Event ID */
    uint8_t num_commands; /**< Number of Commands */
    uint8_t tariff_type; /**< Tariff Type */
};

/** Get Price Matrix command structure */
struct ZbZclPriceClientGetPriceMatrixT {
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
};

/** Get Block Thresholds command structure */
struct ZbZclPriceClientGetBlockThresholdsT {
    uint32_t issuer_tariff_id; /**< Issuer Tariff ID */
};

/* Price Server */

/** Price Server callbacks configuration */
struct ZbZclPriceServerCallbacksT {
    enum ZclStatusCodeT (*get_current_price)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetCurrentPriceT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Current command.
     * Application should return ZCL_STATUS_SUCCESS to include in the Default Response.
     * The application may call ZbZclPriceServerSendPublishPrice to send a
     * PublishPrice command.
     */

    enum ZclStatusCodeT (*price_ack)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientPriceAckT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Price Acknowledgement command. */

    enum ZclStatusCodeT (*get_tariff_info)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetTariffInfoT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Tariff Information command.
     * Application should return ZCL_STATUS_SUCCESS to include in the Default Response.
     * The application may call ZbZclPriceServerSendPublishTariffInfo to send a
     * PublishTariffInfo command. */

    enum ZclStatusCodeT (*get_price_matrix)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetPriceMatrixT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Price Matrix command.
     * Application should return ZCL_STATUS_SUCCESS to include in the Default Response.
     * The application may call ZbZclPriceServerSendPublishMatrix to send a
     * PublishMatrix command. */

    enum ZclStatusCodeT (*get_block_thresholds)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceClientGetBlockThresholdsT *req, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Get Block Thresholds command.
     * Application should return ZCL_STATUS_SUCCESS to include in the Default Response.
     * The application may call ZbZclPriceServerSendPublishBlockThresholds to send a
     * PublishBlockThresholds command. */

    /* Optional commands not parsed into a struct or specific callback */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
    /**< Callback to application, invoked on receipt of optional Client commands. */
};

/**
 * Create a new instance of the Price Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPriceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPriceServerCallbacksT *callbacks, void *arg);

/**
 * Initialize Publish Price information
 * @param rsp Publish Price command structure
 * @return Void
 */
void ZbZclPriceServerPublishPriceInit(struct ZbZclPriceServerPublishPriceT *rsp);

/**
 * Send a Publish Price command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param notify Publish Price command structure
 * @param callback Callback function that will be invoked later when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishPrice(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishPriceT *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Tariff Information command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param notify Publish Tariff Info command structure
 * @param callback Callback function that will be invoked later when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishTariffInfo(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishTariffInfoT *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Price Matrix command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param notify Publish Price Matrix command structure
 * @param callback Callback function that will be invoked later when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishMatrix(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishPriceMatrixT *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Publish Block Thresholds command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for command, including sequence number and tx options
 * @param notify Publish Block Thresholds command structure
 * @param callback Callback function that will be invoked later when response
 * is received, if one is expected. If broadcasting, then this should be set
 * to NULL since no response is expected
 * @param arg Pointer to application data that will later be provided back
 * to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceServerSendPublishBlockThresholds(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceServerPublishBlockThresholdsT *notify,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Price Client */
/** Price Client callbacks configuration */
struct ZbZclPriceClientCallbacksT {
    enum ZclStatusCodeT (*publish_price)(struct ZbZclClusterT *cluster, void *arg,
        struct ZbZclPriceServerPublishPriceT *price, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to Server, invoked on receipt of Publish Price command. */

    /* Optional commands not parsed into a struct or specific callback */
    enum ZclStatusCodeT (*optional)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr);
    /**< Callback to Server, invoked on receipt of optional Server commands. */
};

/**
 * Create a new instance of the Price Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPriceClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclPriceClientCallbacksT *callbacks, void *arg);

/**
 * Send a Get Current Price command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetCurrentPriceReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Scheduled Prices command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Scheduled Prices command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetScheduledPricesReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetScheduledPricesT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Price Acknowledgement command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Price Acknowledge command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandPriceAckReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientPriceAckT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Tariff Information command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Tariff Information command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetTariffInfoReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetTariffInfoT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Price Matrix command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Price Matrix command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetPriceMatrixReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetPriceMatrixT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Block Thresholds command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Block Thresholds command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclPriceClientCommandGetBlockThresholdsReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclPriceClientGetBlockThresholdsT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif
