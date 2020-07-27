/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_H
# define ZCL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Needs to come before zigbee.h. Should remove zigbee.h include from this file. */
struct ZbZclClusterT;

#include "zigbee.h"
#include "pletoh.h"

/*---------------------------------------------------------------
 * ZCL Debug Logging
 *---------------------------------------------------------------
 */
/*lint -save -e762 [ Redundantly declared symbol ] */
/*lint -save -e9004 [ previously declared - MISRA 2012 Rule 8.5, required ] */
extern void ZbLogPrintf(struct ZigBeeT *zb, uint32_t mask, const char *hdr, const char *fmt, ...);
/*lint -restore */
/*lint -restore */

#ifndef CONFIG_ZB_LOG_ALLOWED_MASK
/* If not defined, then disable debugging (minimal Flash size) */
# define CONFIG_ZB_LOG_ALLOWED_MASK         ZB_LOG_MASK_LEVEL_0
#endif

/*lint -emacro(506,ZCL_LOG_PRINTF) */
/* Info 774: Boolean within 'if' always evaluates to False */
/*lint -emacro(774,ZCL_LOG_PRINTF) */
/*lint -emacro(831,ZCL_LOG_PRINTF) */

#if ((ZB_LOG_MASK_ZCL & CONFIG_ZB_LOG_ALLOWED_MASK) != 0U)
#define ZCL_LOG_PRINTF(zb, hdr, ...) \
    do { \
        if ((zb) == NULL) { \
            break; \
        } \
        ZbLogPrintf(zb, ZB_LOG_MASK_ZCL, hdr, __VA_ARGS__); \
    } while (false)
#else
#define ZCL_LOG_PRINTF(zb, hdr, ...)    (void)zb
#endif

/*---------------------------------------------------------------
 * ZCL Header
 *---------------------------------------------------------------
 */
/* ZCL Frame Control Field Bits and macros.. */
#define ZCL_FRAMECTRL_TYPE                       0x03U
#define ZCL_FRAMECTRL_MANUFACTURER               0x04U
#define ZCL_FRAMECTRL_DIRECTION                  0x08U /* set for client, clear for server */
#define ZCL_FRAMECTRL_DISABLE_DEFAULT_RESP       0x10U
#define ZCL_FRAMECTRL_RESERVED                   0xe0U

/* ZCL Frame Type Values */
#define ZCL_FRAMETYPE_PROFILE                    0x00U /* Global */
#define ZCL_FRAMETYPE_CLUSTER                    0x01U /* Cluster Specific */

/* ZCL Direction Values */
enum ZbZclDirectionT {
    ZCL_DIRECTION_TO_SERVER = 0,
    ZCL_DIRECTION_TO_CLIENT = 1,
    ZCL_DIRECTION_ANY = 2 /* Only used internally with ZbZclClusterBind */
};

enum ZbZclNoDefaultResponseT {
    ZCL_NO_DEFAULT_RESPONSE_FALSE = 0,
    ZCL_NO_DEFAULT_RESPONSE_TRUE = 1
};

/* Lengths of the ZCL header. */
#define ZCL_HEADER_MIN_SIZE                      3U
#define ZCL_HEADER_MAX_SIZE                      5U /* with 2-byte manufacturer code */

/* Max safe ZCL payload size (with APS security and no fragmentation)
 * This value will be less with application security.
 * Assumes not manufacturer specific command. */
#define ZCL_PAYLOAD_UNFRAG_SAFE_SIZE             (ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE - ZCL_HEADER_MIN_SIZE) /* 54 */

/* Maximum ZSE Message size (after fragmentation). */
#define ZCL_ASDU_LENGTH_SMART_ENERGY             128U

/* ZCL Indicator Values */
#define ZCL_INDICATOR_DEPTH                      0x0fU
#define ZCL_INDICATOR_OPERATION                  0xf0U
#define ZCL_INDICATOR_REPLACE                    0x00U
#define ZCL_INDICATOR_INSERT                     0x10U
#define ZCL_INDICATOR_REMOVE                     0x20U
#define ZCL_INDICATOR_DEPTH_DECR(x)              (x = (x & ZCL_INDICATOR_OPERATION_MASK) | ((x - 1) & ZCL_INDICATOR_DEPTH))

/* ZCL Semi-Precision Floating Point Fields. */
#define ZCL_FLOAT_SEMI_MANTISSA                  0x03ffU
#define ZCL_FLOAT_SEMI_EXPONENT                  0x7c00U
#define ZCL_FLOAT_SEMI_SIGN                      0x8000U
#define ZCL_FLOAT_SEMI_HIDDEN_BIT                10U
/* ZCL Single-Precision Floating Point Fields. */
#define ZCL_FLOAT_SINGLE_MANTISSA                0x007fffffU
#define ZCL_FLOAT_SINGLE_EXPONENT                0x7f800000U
#define ZCL_FLOAT_SINGLE_SIGN                    0x80000000U
#define ZCL_FLOAT_SINGLE_HIDDEN_BIT              23U
/* ZCL Double-Precision Floating point Fields. */
#define ZCL_FLOAT_DOUBLE_MANTISSA                0x000fffffffffffffULL
#define ZCL_FLOAT_DOUBLE_EXPONENT                0x7ff0000000000000ULL
#define ZCL_FLOAT_DOUBLE_SIGN                    0x8000000000000000ULL
#define ZCL_FLOAT_DOUBLE_HIDDEN_BIT              52U

/* Floating Point Helper Macros */
#ifdef _MSC_VER
#include <float.h>
#define ZCL_FLOAT_INFINITY                       (DBL_MAX + DBL_MAX)
#define ZCL_FLOAT_NAN                            (ZCL_FLOAT_INFINITY - ZCL_FLOAT_INFINITY)
#else
#define ZCL_FLOAT_INFINITY                       (1.0 / 0.0)
#define ZCL_FLOAT_NAN                            (0.0 / 0.0)
#endif
#define ZCL_FLOAT_ISNAN(x)                       ((x) != (x))
#define ZCL_FLOAT_ISINF(x)                       (((x) == ZCL_FLOAT_INFINITY) || ((x) == -ZCL_FLOAT_INFINITY))

/* Smart Energy Preferred channels: 11, 14, 15, 19, 20, 24, 25 */
#define ZCL_CHANNELMASK_SMART_ENERGY             0x318c800UL

/* Profile IDs and Ranges */
enum {
    ZCL_PROFILE_ZB_STANDARD_MIN = 0x0000,
    /* Note: Legacy profile range does not include HA (0x0104) */
    ZCL_PROFILE_LEGACY_MIN = 0x0101,
    ZCL_PROFILE_HOME_AUTOMATION = 0x0104,
    ZCL_PROFILE_LEGACY_MAX = 0x0108,
    ZCL_PROFILE_SMART_ENERGY = 0x0109,
    ZCL_PROFILE_RETAIL_SERVICES = 0x010A,
    ZCL_PROFILE_TP2 = 0x7f01, /* ZigBee Test Profile #2 Profile Id */
    ZCL_PROFILE_ZB_STANDARD_MAX = 0x7fff,

    ZCL_PROFILE_MFR_SPECIFIC_MIN = 0xc000,
    ZCL_PROFILE_ZLL = 0xc05e,
    ZCL_PROFILE_C4 = 0xc25d,
    ZCL_PROFILE_WILDCARD = 0xffff /* Wildcard Profile ID */
};

/* Device ID Ranges */
enum {
    ZCL_DEVICE_ZB_STANDARD_MIN = 0x0000,
    ZCL_DEVICE_ZB_STANDARD_MAX = 0xbfff
};

/* Attribute ID Ranges */
enum {
    ZCL_ATTRIBUTE_ZB_STANDARD_MIN = 0x0000,
    ZCL_ATTRIBUTE_ZB_STANDARD_MAX = 0x3fff
};

/* Command ID Ranges */
enum {
    ZCL_COMMAND_ZB_STANDARD_MIN = 0x00,
    ZCL_COMMAND_ZB_STANDARD_MAX = 0x7f
};

/* Global Attributes */
enum {
    /* CLUSTER_REV attribute is automatically allocated and initialized by
     * a cluster's alloc function when it calls ZbZclClusterAlloc.
     * It does not need to be included in the cluster's attribute list
     * during initialization. The value may be modified by the cluster or
     * application after initialization (ZbZclAttrIntegerWrite). */
    ZCL_GLOBAL_ATTR_CLUSTER_REV = 0xfffd, /* Mandatory */
    ZCL_GLOBAL_ATTR_REPORTING_STATUS = 0xfffe /* Optional */
};

/* ZCL_GLOBAL_ATTR_CLUSTER_REV */
enum {
    ZCL_CLUSTER_REVISION_LEGACY = 0,
    ZCL_CLUSTER_REVISION_ZCL6 = 1,
    ZCL_CLUSTER_REVISION_ZCL7 = 2
};

/* ZCL Attribute Flags
 * These define characteristics for a particular attribute
 * (e.g. write permissions, is persisted, etc).  */
typedef uint16_t ZclAttrFlagT;
/* i.e. read-only, non-reportable, etc */
#define ZCL_ATTR_FLAG_NONE                  (ZclAttrFlagT)0x0000U
/* read-write */
#define ZCL_ATTR_FLAG_WRITABLE              (ZclAttrFlagT)0x0001U
/* attribute may be reported */
#define ZCL_ATTR_FLAG_REPORTABLE            (ZclAttrFlagT)0x0002U
/* attribute is persisted */
#define ZCL_ATTR_FLAG_PERSISTABLE           (ZclAttrFlagT)0x0004U
/* Which callbacks does the application support */
#define ZCL_ATTR_FLAG_CB_MASK               0x00f0U
#define ZCL_ATTR_FLAG_CB_READ               (ZclAttrFlagT)0x0010U /* ZCL_ATTR_CB_TYPE_READ */
#define ZCL_ATTR_FLAG_CB_WRITE              (ZclAttrFlagT)0x0020U /* ZCL_ATTR_CB_TYPE_WRITE */
#define ZCL_ATTR_FLAG_CB_NOTIFY             (ZclAttrFlagT)0x0040U /* ZCL_ATTR_CB_TYPE_NOTIFY */
/* This flag means the attribute is for internal use only. Not discoverable. */
#define ZCL_ATTR_FLAG_INTERNAL              (ZclAttrFlagT)0x8000U

/* ZCL Attribute Write-Mode Flags */
typedef uint16_t ZclWriteModeT;
/* Perform all sanity checks, and write if allowed. */
#define ZCL_ATTR_WRITE_FLAG_NORMAL          (ZclWriteModeT)0x0000U
/* Sanity-check only, don't modify attribute data. */
#define ZCL_ATTR_WRITE_FLAG_TEST            (ZclWriteModeT)0x0001U
/* If the attribute is read only, write anyways. */
#define ZCL_ATTR_WRITE_FLAG_FORCE           (ZclWriteModeT)0x0002U
/* Data is from persistence (sometimes the application needs to
 * know where the data is coming from). */
#define ZCL_ATTR_WRITE_FLAG_PERSIST         (ZclWriteModeT)0x0004U

/* ZCL Data Types */
enum ZclDataTypeT {
    ZCL_DATATYPE_NULL = 0x00,
    ZCL_DATATYPE_GENERAL_8BIT = 0x08,
    ZCL_DATATYPE_GENERAL_16BIT = 0x09,
    ZCL_DATATYPE_GENERAL_24BIT = 0x0a,
    ZCL_DATATYPE_GENERAL_32BIT = 0x0b,
    ZCL_DATATYPE_GENERAL_40BIT = 0x0c,
    ZCL_DATATYPE_GENERAL_48BIT = 0x0d,
    ZCL_DATATYPE_GENERAL_56BIT = 0x0e,
    ZCL_DATATYPE_GENERAL_64BIT = 0x0f,
    ZCL_DATATYPE_BOOLEAN = 0x10,
    ZCL_DATATYPE_BITMAP_8BIT = 0x18,
    ZCL_DATATYPE_BITMAP_16BIT = 0x19,
    ZCL_DATATYPE_BITMAP_24BIT = 0x1a,
    ZCL_DATATYPE_BITMAP_32BIT = 0x1b,
    ZCL_DATATYPE_BITMAP_40BIT = 0x1c,
    ZCL_DATATYPE_BITMAP_48BIT = 0x1d,
    ZCL_DATATYPE_BITMAP_56BIT = 0x1e,
    ZCL_DATATYPE_BITMAP_64BIT = 0x1f,
    ZCL_DATATYPE_UNSIGNED_8BIT = 0x20,
    ZCL_DATATYPE_UNSIGNED_16BIT = 0x21,
    ZCL_DATATYPE_UNSIGNED_24BIT = 0x22,
    ZCL_DATATYPE_UNSIGNED_32BIT = 0x23,
    ZCL_DATATYPE_UNSIGNED_40BIT = 0x24,
    ZCL_DATATYPE_UNSIGNED_48BIT = 0x25,
    ZCL_DATATYPE_UNSIGNED_56BIT = 0x26,
    ZCL_DATATYPE_UNSIGNED_64BIT = 0x27,
    ZCL_DATATYPE_SIGNED_8BIT = 0x28,
    ZCL_DATATYPE_SIGNED_16BIT = 0x29,
    ZCL_DATATYPE_SIGNED_24BIT = 0x2a,
    ZCL_DATATYPE_SIGNED_32BIT = 0x2b,
    ZCL_DATATYPE_SIGNED_40BIT = 0x2c,
    ZCL_DATATYPE_SIGNED_48BIT = 0x2d,
    ZCL_DATATYPE_SIGNED_56BIT = 0x2e,
    ZCL_DATATYPE_SIGNED_64BIT = 0x2f,
    ZCL_DATATYPE_ENUMERATION_8BIT = 0x30,
    ZCL_DATATYPE_ENUMERATION_16BIT = 0x31,
    ZCL_DATATYPE_FLOATING_SEMI = 0x38,
    ZCL_DATATYPE_FLOATING_SINGLE = 0x39,
    ZCL_DATATYPE_FLOATING_DOUBLE = 0x3a,
    ZCL_DATATYPE_STRING_OCTET = 0x41,
    ZCL_DATATYPE_STRING_CHARACTER = 0x42,
    ZCL_DATATYPE_STRING_LONG_OCTET = 0x43,
    ZCL_DATATYPE_STRING_LONG_CHARACTER = 0x44,
    ZCL_DATATYPE_ARRAY = 0x48,
    ZCL_DATATYPE_STRUCT = 0x4c,
    ZCL_DATATYPE_SET = 0x50,
    ZCL_DATATYPE_BAG = 0x51,
    ZCL_DATATYPE_TIME_OF_DAY = 0xe0,
    ZCL_DATATYPE_DATE = 0xe1,
    ZCL_DATATYPE_TIME_UTC = 0xe2,
    ZCL_DATATYPE_CLUSTER_ID = 0xe8,
    ZCL_DATATYPE_ATTRIBUTE_ID = 0xe9,
    ZCL_DATATYPE_BACNET_OID = 0xea,
    ZCL_DATATYPE_EUI64 = 0xf0,
    ZCL_DATATYPE_SECURITY_KEY128 = 0xf1,
    ZCL_DATATYPE_UNKNOWN = 0xff
};

/* ZCL Invalid Data Values */
#define ZCL_INVALID_BOOLEAN                      0xffU
#define ZCL_INVALID_UNSIGNED_8BIT                0xffU
#define ZCL_INVALID_UNSIGNED_16BIT               0xffffU
#define ZCL_INVALID_UNSIGNED_24BIT               0xffffffU
#define ZCL_INVALID_UNSIGNED_32BIT               0xffffffffU
#define ZCL_INVALID_UNSIGNED_40BIT               0xffffffffffULL
#define ZCL_INVALID_UNSIGNED_48BIT               0xffffffffffffULL
#define ZCL_INVALID_UNSIGNED_56BIT               0xffffffffffffffULL
#define ZCL_INVALID_UNSIGNED_64BIT               0xffffffffffffffffULL

#define ZCL_INVALID_SIGNED_8BIT                  0x80
#define ZCL_INVALID_SIGNED_16BIT                 0x8000
#define ZCL_INVALID_SIGNED_24BIT                 0x800000
#define ZCL_INVALID_SIGNED_32BIT                 0x80000000U
#define ZCL_INVALID_SIGNED_40BIT                 0x8000000000LL
#define ZCL_INVALID_SIGNED_48BIT                 0x800000000000LL
#define ZCL_INVALID_SIGNED_56BIT                 0x80000000000000LL
#define ZCL_INVALID_SIGNED_64BIT                 0x8000000000000000UL

#define ZCL_INVALID_ENUMERATION_8BIT             0xffU
#define ZCL_INVALID_ENUMERATION_16BIT            0xffffU

#define ZCL_INVALID_FLOATING                     (0.0 / 0.0)
/* Note, can't bit-or signed values (MISRA rule 10.1) */
#define ZCL_INVALID_FLOATING_SEMI                (ZCL_FLOAT_SEMI_MANTISSA + ZCL_FLOAT_SEMI_EXPONENT)
#define ZCL_INVALID_FLOATING_SINGLE              (ZCL_FLOAT_SINGLE_MANTISSA + ZCL_FLOAT_SINGLE_MANTISSA)
#define ZCL_INVALID_FLOATING_DOUBLE              (ZCL_FLOAT_DOUBLE_MANTISSA + ZCL_FLOAT_DOUBLE_MANTISSA)

#define ZCL_INVALID_STRING_OCTET                 0xffU
#define ZCL_INVALID_STRING_CHARACTER             0xffU
#define ZCL_INVALID_STRING_LONG_OCTET            0xffffU
#define ZCL_INVALID_STRING_LONG_CHARACTER        0xffffU

#define ZCL_INVALID_ARRAY                        0xffffU
#define ZCL_INVALID_STRUCT                       0xffffU
#define ZCL_INVALID_SET                          0xffffU
#define ZCL_INVALID_BAG                          0xffffU
#define ZCL_INVALID_TIME_OF_DAY                  0xffffffffU
#define ZCL_INVALID_DATE                         0xffffffffU
#define ZCL_INVALID_TIME_UTC                     0xffffffffU
#define ZCL_INVALID_CLUSTER_ID                   0xffffU
#define ZCL_INVALID_ATTRIBUTE_ID                 0xffffU
#define ZCL_INVALID_BACNET_OID                   0xffffffffU
#define ZCL_INVALID_EUI64                        0xffffffffffffffffULL

/* ZCL Valid Ranges */
#define ZCL_MIN_SIGNED_8BIT                      (-(ZCL_INVALID_SIGNED_8BIT - 1))
#define ZCL_MIN_SIGNED_16BIT                     (-(ZCL_INVALID_SIGNED_16BIT - 1))
#define ZCL_MIN_SIGNED_24BIT                     (-(ZCL_INVALID_SIGNED_24BIT - 1))
#define ZCL_MIN_SIGNED_32BIT                     (-(ZCL_INVALID_SIGNED_32BIT - 1))
#define ZCL_MIN_SIGNED_40BIT                     (-(ZCL_INVALID_SIGNED_40BIT - 1))
#define ZCL_MIN_SIGNED_48BIT                     (-(ZCL_INVALID_SIGNED_48BIT - 1))
#define ZCL_MIN_SIGNED_56BIT                     (-(ZCL_INVALID_SIGNED_56BIT - 1))
#define ZCL_MIN_SIGNED_64BIT                     (-(ZCL_INVALID_SIGNED_64BIT - 1))

#define ZCL_MAX_SIGNED_8BIT                      0x7f
#define ZCL_MAX_SIGNED_16BIT                     0x7fff
#define ZCL_MAX_SIGNED_24BIT                     0x7fffff
#define ZCL_MAX_SIGNED_32BIT                     0x7fffffff
#define ZCL_MAX_SIGNED_40BIT                     0x7fffffffffULL
#define ZCL_MAX_SIGNED_48BIT                     0x7fffffffffffULL
#define ZCL_MAX_SIGNED_56BIT                     0x7fffffffffffffULL
#define ZCL_MAX_SIGNED_64BIT                     0x7fffffffffffffffULL

#define ZCL_MAX_UNSIGNED_8BIT                    (ZCL_INVALID_UNSIGNED_8BIT - 1U)
#define ZCL_MAX_UNSIGNED_16BIT                   (ZCL_INVALID_UNSIGNED_16BIT - 1U)
#define ZCL_MAX_UNSIGNED_24BIT                   (ZCL_INVALID_UNSIGNED_24BIT - 1U)
#define ZCL_MAX_UNSIGNED_32BIT                   (ZCL_INVALID_UNSIGNED_32BIT - 1U)
#define ZCL_MAX_UNSIGNED_40BIT                   (ZCL_INVALID_UNSIGNED_40BIT - 1U)
#define ZCL_MAX_UNSIGNED_48BIT                   (ZCL_INVALID_UNSIGNED_48BIT - 1U)
#define ZCL_MAX_UNSIGNED_56BIT                   (ZCL_INVALID_UNSIGNED_56BIT - 1U)
#define ZCL_MAX_UNSIGNED_64BIT                   (ZCL_INVALID_UNSIGNED_64BIT - 1U)

/* Type sizes */
#define ZCL_SIZEOF_8BIT                          1U
#define ZCL_SIZEOF_16BIT                         2U
#define ZCL_SIZEOF_24BIT                         3U
#define ZCL_SIZEOF_32BIT                         4U
#define ZCL_SIZEOF_40BIT                         5U
#define ZCL_SIZEOF_48BIT                         6U
#define ZCL_SIZEOF_56BIT                         7U
#define ZCL_SIZEOF_64BIT                         8U
#define ZCL_SIZEOF_128BIT                        16U

/* ZCL Status Codes */
enum ZclStatusCodeT {
    ZCL_STATUS_SUCCESS = 0x00,
    ZCL_STATUS_FAILURE = 0x01,
    ZCL_STATUS_REQUEST_DENIED = 0x70,
    ZCL_STATUS_NOT_AUTHORIZED = 0x7e,
    ZCL_STATUS_MALFORMED_COMMAND = 0x80,
    ZCL_STATUS_UNSUPP_CLUSTER_COMMAND = 0x81,
    ZCL_STATUS_UNSUPP_GENERAL_COMMAND = 0x82,
    ZCL_STATUS_UNSUPP_MFR_CLUSTER_COMMAND = 0x83,
    ZCL_STATUS_UNSUPP_MFR_GENERAL_COMMAND = 0x84,
    ZCL_STATUS_INVALID_FIELD = 0x85,
    ZCL_STATUS_UNSUPP_ATTRIBUTE = 0x86,
    ZCL_STATUS_INVALID_VALUE = 0x87,
    ZCL_STATUS_READ_ONLY = 0x88,
    ZCL_STATUS_INSUFFICIENT_SPACE = 0x89,
    ZCL_STATUS_DUPLICATE_EXISTS = 0x8a,
    ZCL_STATUS_NOT_FOUND = 0x8b,
    ZCL_STATUS_UNREPORTABLE_ATTRIBUTE = 0x8c,
    ZCL_STATUS_INVALID_DATA_TYPE = 0x8d,
    ZCL_STATUS_INVALID_SELECTOR = 0x8e,
    ZCL_STATUS_WRITE_ONLY = 0x8f,
    ZCL_STATUS_INCONSISTENT_STARTUP_STATE = 0x90,
    ZCL_STATUS_DEFINED_OUT_OF_BAND = 0x91,
    ZCL_STATUS_INCONSISTENT = 0x92,
    ZCL_STATUS_ACTION_DENIED = 0x93,
    ZCL_STATUS_TIMEOUT = 0x94,
    ZCL_STATUS_ABORT = 0x95,
    ZCL_STATUS_INVALID_IMAGE = 0x96,
    ZCL_STATUS_WAIT_FOR_DATA = 0x97,
    ZCL_STATUS_NO_IMAGE_AVAILABLE = 0x98,
    ZCL_STATUS_REQUIRE_MORE_IMAGE = 0x99,
    ZCL_STATUS_NOTIFICATION_PENDING = 0x9A,
    ZCL_STATUS_HARDWARE_FAILURE = 0xc0,
    ZCL_STATUS_SOFTWARE_FAILURE = 0xc1,
    ZCL_STATUS_CALIBRATION_ERROR = 0xc2,
    ZCL_STATUS_UNSUPP_CLUSTER = 0xc3,
    ZCL_STATUS_LIMIT_REACHED = 0xc4,

    /* Exegin add-ons */
    ZCL_STATUS_ALLOC_FAIL = 0xfe,
    /* Exegin add-on: Used with cluster command callback to indicate
     * command was handled successfully, but don't send a Default Response
     * for success. I.e. a response has already been sent. */
    ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE = 0xff
        /* zcl_attr_integer.c relies on these enum constants to be =< 0xff since they're cast as uint8_t */
};

/* For SE 1.2a Attribute Reporting Status for Sleepy Devices */
#define ZCL_ATTR_REPORTING_STATUS               0xfffeU
#define ZCL_ATTR_REPORTING_STATUS_PENDING       0x00U
#define ZCL_ATTR_REPORTING_STATUS_COMPLETE      0x01U

/* Device IDs */
enum ZbZclDeviceIdT {
    ZCL_DEVICE_ONOFF_SWITCH = 0x0000,
    ZCL_DEVICE_LEVEL_SWITCH = 0x0001,
    ZCL_DEVICE_ONOFF_OUTPUT = 0x0002,
    ZCL_DEVICE_LEVEL_OUTPUT = 0x0003,
    ZCL_DEVICE_SCENE_SELECTOR = 0x0004,
    ZCL_DEVICE_CONFIG_TOOL = 0x0005,
    ZCL_DEVICE_REMOTE_CONTROL = 0x0006,
    ZCL_DEVICE_COMBINED_INTERFACE = 0x0007,
    ZCL_DEVICE_RANGE_EXTENDER = 0x0008,
    ZCL_DEVICE_MAINS_POWER_OUTLET = 0x0009,
    ZCL_DEVICE_DOOR_LOCK = 0x000a,
    ZCL_DEVICE_DOOR_LOCK_CONTROLLER = 0x000b,
    ZCL_DEVICE_SIMPLE_SENSOR = 0x000c,
    ZCL_DEVICE_DATA_COLLECTION_UNIT = 0x0010,
    ZCL_DEVICE_ZIGBEE_SIM_CARD = 0x0020,
    ZCL_DEVICE_ZIGBEE_MOBILE_TERMINAL = 0x0021,
    ZCL_DEVICE_ZIGBEE_GLOBAL_PLATFORM_CARD = 0x0026, /* same as  */
    ZCL_DEVICE_CUSTOMER_HANDHELD_DEVICE = 0x0030,
    ZCL_DEVICE_RETAIL_ASSOCIATE_HANDHELD_DEVICE = 0x0031,
    ZCL_DEVICE_INTELLIGENT_SHOPPING_CART = 0x0032,
    ZCL_DEVICE_ELECTRONIC_SHELF_LABEL = 0x0033,
    ZCL_DEVICE_CUSTOMER_INFORMATION_POINT = 0x0034,
    ZCL_DEVICE_CUSTOMER_CARD = 0x0035,
    ZCL_DEVICE_CONSTRUCTED_BACNET_DEVICE = 0x004a,
    ZCL_DEVICE_BACNET_TUNNELED_DEVICE = 0x004b,
    ZCL_DEVICE_HOME_GATEWAY = 0x0050,
    ZCL_DEVICE_SMART_PLUG = 0x0051,
    ZCL_DEVICE_WHITE_GOODS = 0x0052,
    ZCL_DEVICE_METER_INTERFACE = 0x0053,
    ZCL_DEVICE_ZGP_PROXY = 0x0060,
    ZCL_DEVICE_ZGP_PROXY_BASIC = 0x0061,
    ZCL_DEVICE_ZGP_TARGET_PLUS = 0x0062,
    ZCL_DEVICE_ZGP_TARGET = 0x0063,
    ZCL_DEVICE_ZGP_COMMISSIONING_TOOL = 0x0064,
    ZCL_DEVICE_ZGP_COMBO = 0x0065,
    ZCL_DEVICE_ZGP_COMBO_BASIC = 0x0066,
    ZCL_DEVICE_ENVIRONMENTAL_SENSOR = 0x0067,

    /* Lighting and Occupancy (0x0100) */
    ZCL_DEVICE_ONOFF_LIGHT = 0x0100,
    ZCL_DEVICE_DIMMABLE_LIGHT = 0x0101,
    ZCL_DEVICE_COLOR_DIMMABLE_LIGHT = 0x0102,
    ZCL_DEVICE_ONOFF_LIGHT_SWITCH = 0x0103,
    ZCL_DEVICE_ONOFF_DIMMER_SWITCH = 0x0104,
    ZCL_DEVICE_ONOFF_COLOR_DIMMER_SWITCH = 0x0105,
    ZCL_DEVICE_ONOFF_LIGHT_SENSOR = 0x0106,
    ZCL_DEVICE_ONOFF_OCCUPANCY_SENSOR = 0x0107,
    /* EXEGIN - add more from Lighting and Occupancy Device Spec */

    /* IAS (0x0400) */
    ZCL_DEVICE_IAS_CONTROL_AND_INDICATING_EQUIPMENT = 0x0400,
    ZCL_DEVICE_IAS_ANCILLARY_CONTROL_EQUIPMENT = 0x0401,
    ZCL_DEVICE_IAS_ZONE = 0x0402,
    ZCL_DEVICE_IAS_WARNING_DEVICE = 0x0403,

    /* Smart Energy device identifiers. (0x0500) */
    ZCL_DEVICE_ESP = 0x0500,
    ZCL_DEVICE_METER = 0x0501,
    ZCL_DEVICE_IN_HOME_DISPLAY = 0x0502,
    ZCL_DEVICE_PCT = 0x0503,
    ZCL_DEVICE_LOAD_CONTROL = 0x0504,
    ZCL_DEVICE_SMART_APPLIANCE = 0x0505,
    ZCL_DEVICE_PREPAYMENT_TERMINAL = 0x0506,
    ZCL_DEVICE_PHYSICAL_DEVICE = 0x0507,
    ZCL_DEVICE_REMOTE_COMMUNICATIONS_DEVICE = 0x0508,
    ZCL_DEVICE_ERL_INTERFACE = 0x0509,

    /* Lighting and Occupancy Continued (0x0800) */
    ZCL_DEVICE_COLOR_CONTROLLER = 0x0800,
    ZCL_DEVICE_COLOR_SCENE_CONTROLLER = 0x0810,
    ZCL_DEVICE_NON_COLOR_CONTROLLER = 0x0820,
    ZCL_DEVICE_NON_COLOR_SCENE_CONTROLLER = 0x0830,
    ZCL_DEVICE_CONTROL_BRIDGE = 0x0840,
    ZCL_DEVICE_ONOFF_SENSOR = 0x0850 /* aka Door Sensor? */
};

/* Cluster ID Ranges */
enum {
    ZCL_CLUSTER_ZB_STANDARD_MIN = 0x0000U,
    ZCL_CLUSTER_ZB_STANDARD_MAX = 0x7fffU,
    ZCL_CLUSTER_MFR_SPECIFIC_MIN = 0xfc00U,
    ZCL_CLUSTER_MFR_SPECIFIC_MAX = 0xffffU
};

/* Known Cluster IDs */
enum ZbZclClusterIdT {
    /* General cluster identifiers. */
    ZCL_CLUSTER_BASIC = 0x0000,
    ZCL_CLUSTER_POWER_CONFIG = 0x0001,
    ZCL_CLUSTER_DEVICE_TEMPERATURE = 0x0002,
    ZCL_CLUSTER_IDENTIFY = 0x0003,
    ZCL_CLUSTER_GROUPS = 0x0004,
    ZCL_CLUSTER_SCENES = 0x0005,
    ZCL_CLUSTER_ONOFF = 0x0006,
    ZCL_CLUSTER_ONOFF_CONFIG = 0x0007,
    ZCL_CLUSTER_LEVEL_CONTROL = 0x0008,
    ZCL_CLUSTER_ALARMS = 0x0009,
    ZCL_CLUSTER_TIME = 0x000a,
    ZCL_CLUSTER_RSSI_LOCATION = 0x000b,
    ZCL_CLUSTER_ANALOG_INPUT_BASIC = 0x000c,
    ZCL_CLUSTER_ANALOG_OUTPUT_BASIC = 0x000d,
    ZCL_CLUSTER_ANALOG_VALUE_BASIC = 0x000e,
    ZCL_CLUSTER_BINARY_INPUT_BASIC = 0x000f,
    ZCL_CLUSTER_BINARY_OUTPUT_BASIC = 0x0010,
    ZCL_CLUSTER_BINARY_VALUE_BASIC = 0x0011,
    ZCL_CLUSTER_MULTISTATE_INPUT_BASIC = 0x0012,
    ZCL_CLUSTER_MULTISTATE_OUTPUT_BASIC = 0x0013,
    ZCL_CLUSTER_MULTISTATE_VALUE_BASIC = 0x0014,
    ZCL_CLUSTER_COMMISSIONING = 0x0015,
    ZCL_CLUSTER_OTA_UPGRADE = 0x0019,
    ZCL_CLUSTER_POWER_PROFILE = 0x001a,
    ZCL_CLUSTER_POLL_CONTROL = 0x0020,
    ZCL_CLUSTER_GREEN_POWER = 0x0021,
    ZCL_CLUSTER_NEAREST_GATEWAY = 0x0024,
    ZCL_CLUSTER_KEEP_ALIVE = 0x0025,
    ZCL_CLUSTER_METER_ID = 0x0b01,
    ZCL_CLUSTER_DIAGNOSTICS = 0x0b05,

    /* Closures Cluster IDs */
    ZCL_CLUSTER_CLOSURE_SHADE_CONFIG = 0x0100,
    ZCL_CLUSTER_DOOR_LOCK = 0x0101,
    ZCL_CLUSTER_WINDOW_COVERING = 0x0102,

    /* HVAC Cluster IDs */
    ZCL_CLUSTER_HVAC_PUMP = 0x0200,
    ZCL_CLUSTER_HVAC_THERMOSTAT = 0x0201,
    ZCL_CLUSTER_HVAC_FAN = 0x0202,
    ZCL_CLUSTER_HVAC_DEHUMIDIFIER = 0x0203,
    ZCL_CLUSTER_HVAC_THERMOSTAT_UI = 0x0204,

    /* Lighting Cluster IDs */
    ZCL_CLUSTER_COLOR_CONTROL = 0x0300,
    ZCL_CLUSTER_BALLAST_CONTROL = 0x0301,

    /* Measurement Cluster IDs */
    ZCL_CLUSTER_MEAS_ILLUMINANCE = 0x0400,
    ZCL_CLUSTER_MEAS_ILLUMINANCE_LEVEL = 0x0401,
    ZCL_CLUSTER_MEAS_TEMPERATURE = 0x0402,
    ZCL_CLUSTER_MEAS_PRESSURE = 0x0403,
    ZCL_CLUSTER_MEAS_FLOW = 0x0404,
    ZCL_CLUSTER_MEAS_HUMIDITY = 0x0405,
    ZCL_CLUSTER_MEAS_OCCUPANCY = 0x0406,
    ZCL_CLUSTER_MEAS_LEAF_WETNESS = 0x0407,
    ZCL_CLUSTER_MEAS_SOIL_MOISTURE = 0x0408,
    ZCL_CLUSTER_MEAS_ELECTRICAL = 0x0b04,

    /* Security Cluster IDs */
    ZCL_CLUSTER_SECURITY_IAS_ZONE = 0x0500,
    ZCL_CLUSTER_SECURITY_IAS_ANCILLARY = 0x0501,
    ZCL_CLUSTER_SECURITY_IAS_WARNING = 0x0502,

    /* Protocol Cluster IDs */
    ZCL_CLUSTER_PROTOCOL_TUNNEL = 0x0600,
    ZCL_CLUSTER_BACNET_TUNNEL = 0x0601,
    ZCL_CLUSTER_BACNET_ANALOG_IN_REG = 0x0602,
    ZCL_CLUSTER_BACNET_ANALOG_IN_EXT = 0x0603,
    ZCL_CLUSTER_BACNET_ANALOG_OUT_REG = 0x0604,
    ZCL_CLUSTER_BACNET_ANALOG_OUT_EXT = 0x0605,
    ZCL_CLUSTER_BACNET_ANALOG_VAL_REG = 0x0606,
    ZCL_CLUSTER_BACNET_ANALOG_VAL_EXT = 0x0607,
    ZCL_CLUSTER_BACNET_BINARY_IN_REG = 0x0608,
    ZCL_CLUSTER_BACNET_BINARY_IN_EXT = 0x0609,
    ZCL_CLUSTER_BACNET_BINARY_OUT_REG = 0x060a,
    ZCL_CLUSTER_BACNET_BINARY_OUT_EXT = 0x060b,
    ZCL_CLUSTER_BACNET_BINARY_VAL_REG = 0x060c,
    ZCL_CLUSTER_BACNET_BINARY_VAL_EXT = 0x060d,
    ZCL_CLUSTER_BACNET_MULTI_IN_REG = 0x060e,
    ZCL_CLUSTER_BACNET_MULTI_IN_EXT = 0x060f,
    ZCL_CLUSTER_BACNET_MULTI_OUT_REG = 0x0610,
    ZCL_CLUSTER_BACNET_MULTI_OUT_EXT = 0x0611,
    ZCL_CLUSTER_BACNET_MULTI_VAL_REG = 0x0612,
    ZCL_CLUSTER_BACNET_MULTI_VAL_EXT = 0x0613,

    /* Smart Energy */
    ZCL_CLUSTER_PRICE = 0x0700,
    ZCL_CLUSTER_DRLC = 0x0701,
    ZCL_CLUSTER_SIMPLE_METERING = 0x0702,
    ZCL_CLUSTER_MESSAGING = 0x0703,
    ZCL_CLUSTER_TUNNELING = 0x0704,
    ZCL_CLUSTER_PREPAYMENT = 0x0705,
    ZCL_CLUSTER_ENERGY_MANAGEMENT = 0x0706, /* SE 1.2 */
    ZCL_CLUSTER_CALENDAR = 0x0707,
    ZCL_CLUSTER_DEVICE_MANAGEMENT = 0x0708, /* SE 1.2 */
    ZCL_CLUSTER_EVENTS = 0x0709, /* SE 1.2 */
    ZCL_CLUSTER_MDU_PAIRING = 0x070A, /* SE 1.2 */

    /* CBKE */
    ZCL_CLUSTER_KEY_ESTABLISHMENT = 0x0800,

    /* Telecommunication */
    ZCL_CLUSTER_VOICE_OVER_ZIGBEE = 0x0904,

    /* TOUCHLINK */
    ZCL_CLUSTER_TOUCHLINK = 0x1000,

    /* Custom, internal use only, for handling cluster persistence.
     * Used with ZCL_MANUF_CODE_INTERNAL. */
    ZCL_CLUSTER_PERSIST = 0xfffe,

    /* Custom, internal use only, for local loop-back commands we want to
     * send to all clusters. Used with ZCL_MANUF_CODE_INTERNAL. */
    ZCL_CLUSTER_WILDCARD = 0xffff
};

/* General ZCL Commands (ZCL_FRAMETYPE_PROFILE) */
enum {
    ZCL_COMMAND_READ = 0x00,
    ZCL_COMMAND_READ_RESPONSE = 0x01,
    ZCL_COMMAND_WRITE = 0x02,
    ZCL_COMMAND_WRITE_UNDIVIDED = 0x03,
    ZCL_COMMAND_WRITE_RESPONSE = 0x04,
    ZCL_COMMAND_WRITE_NO_RESPONSE = 0x05,
    ZCL_COMMAND_CONFIG_REPORTING = 0x06,
    ZCL_COMMAND_CONFIG_REPORTING_RESPONSE = 0x07,
    ZCL_COMMAND_READ_REPORTING = 0x08,
    ZCL_COMMAND_READ_REPORTING_RESPONSE = 0x09,
    ZCL_COMMAND_REPORT = 0x0a,
    ZCL_COMMAND_DEFAULT_RESPONSE = 0x0b,
    ZCL_COMMAND_DISCOVER_ATTR = 0x0c,
    ZCL_COMMAND_DISCOVER_ATTR_RSP = 0x0d,
    ZCL_COMMAND_READ_STRUCTURED = 0x0e,
    ZCL_COMMAND_WRITE_STRUCTURED = 0x0f,
    ZCL_COMMAND_WRITE_STRUCTURED_RESPONSE = 0x10,

    /* Exegin's Manufacturer Specific Global Commands
     * ZCL_FRAMECTRL_MANUFACTURER && ZCL_FRAMETYPE_PROFILE &&
     * (Manufacturer Code == ZCL_MANUF_CODE_INTERNAL) */

    /* For Scene Store Command (ZCL_SCENES_COMMAND_STORE_SCENE)
     * Payload for Request (to Server): None
     * Payload for Response: [CLUSTER(2) | EXT_LEN(1) | EXT_ATTR_DATA(N)] */
    ZCL_CMD_MANUF_INTERNAL_GET_SCENE_EXTDATA = 0xf0, /* get_scene_data() */

    /* For Scene Recall Command (ZCL_SCENES_COMMAND_RECALL_SCENE)
     * Payload for Request (to Server): [EXT_LEN(1) | EXT_ATTR_DATA(N)]
     * Payload for Response: [STATUS(1)] */
    ZCL_CMD_MANUF_INTERNAL_SET_SCENE_EXTDATA, /* set_scene_data() */

    /* ZCL Cluster Persistence: Profile-Wide Manufacturer-Specific command
     * sent to a cluster to restore persistence. */
    ZCL_CMD_MANUF_INTERNAL_ATTR_PERSIST_SET
};

struct ZbZclAddrInfoT {
    struct ZbApsAddrT addr;
    uint8_t seqnum;
    uint16_t tx_options;
};

/* ZCL Frame Control field. */
struct ZbZclFrameControlT {
    uint8_t frameType; /* ZCL_FRAMECTRL_TYPE */
    uint8_t manufacturer; /* TRUE or FALSE */
    enum ZbZclDirectionT direction;
    enum ZbZclNoDefaultResponseT noDefaultResp;
};

/* General ZCL Frame Structure */
struct ZbZclHeaderT {
    struct ZbZclFrameControlT frameCtrl;
    uint16_t manufacturerCode;
    uint8_t seqNum;
    uint8_t cmdId;
};

/*---------------------------------------------------------------
 * Cluster Commands
 *---------------------------------------------------------------
 */
typedef struct ZbZclCommandReqT {
    /* From APSDE-DATA.request */
    struct ZbApsAddrT dst;
    uint16_t profileId;
    enum ZbZclClusterIdT clusterId;
    uint16_t srcEndpt;
    uint16_t txOptions; /* e.g. ZB_APSDE_DATAREQ_TXOPTIONS_ACK */
    bool discoverRoute;
    uint8_t radius;
    /* ZCL command */
    struct ZbZclHeaderT hdr;
    /* if (txOptions & ZB_APSDE_DATAREQ_TXOPTIONS_VECTOR), then payload is
     * a pointer to list of struct ZbApsBufT, and length is the number of struct ZbApsBufT
     * items in the list. */
    const void *payload;
    unsigned int length;
} ZbZclCommandReqT;

/* ZCL Command Response
 *
 * Special case if src.mode == ZB_APSDE_ADDRMODE_NOTPRESENT (0), then
 *      - response generated internally.
 *      - src.endpoint = local endpoint that generated request
 *      - hdr.seqNum = request sequence number
 *      - hdr.cmdId = request command id
 */
struct ZbZclCommandRspT {
    enum ZbStatusCodeT aps_status; /**< APS-DATA.confirm status */
    enum ZclStatusCodeT status;
    /**< Status of the ZCL response. If Default Response, it is the status code found
     * within the response. If a cluster-specific response, it is set to ZB_STATUS_SUCCESS,
     * and the application must parse the payload, if any, for any embedded status. */
    struct ZbApsAddrT src;
    uint16_t profileId;
    enum ZbZclClusterIdT clusterId;
    uint8_t linkQuality;
    struct ZbZclHeaderT hdr;
    const uint8_t *payload;
    uint16_t length;
};

/* Send a ZCL command. The callback function is called when the
 * associated ZCL response is received, or there's an error. */
enum ZclStatusCodeT ZbZclCommandReq(struct ZigBeeT *zb, struct ZbZclCommandReqT *zclReq,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*---------------------------------------------------------------
 * Cluster Attributes
 *---------------------------------------------------------------
 */

#define ZCL_ATTRIBUTE_BUFFER_SIZE_MAX       256U

struct ZbZclAttrCbInfoT;

/**
 * The structure used to initialize a ZCL attribute when calling ZbZclAttrAppendList.
 */
struct ZbZclAttrT {
    uint16_t attributeId; /**< The attribute ID */

    enum ZclDataTypeT dataType;
    /**< The attribute data type (ZCL_DATATYPE_UNKNOWN if last entry) */

    ZclAttrFlagT flags; /**< Attribute flags (e.g. writeable, reportable, etc) */

    unsigned int customValSz;
    /**< A custom size is required if the data type is one of:
     *    ZCL_DATATYPE_STRING_OCTET,
     *    ZCL_DATATYPE_STRING_CHARACTER,
     *    ZCL_DATATYPE_STRING_LONG_OCTET,
     *    ZCL_DATATYPE_STRING_LONG_CHARACTER,
     *    ZCL_DATATYPE_ARRAY,
     *    ZCL_DATATYPE_STRUCT,
     *    ZCL_DATATYPE_SET,
     *    ZCL_DATATYPE_BAG
     *
     * A custom size is also required if both customRead and customWrite callbacks
     * are provided and ZCL_ATTR_FLAG_PERSISTABLE is set.
     *
     * The maximum size is the length returned by ZbZclClusterGetMaxAsduLength() or
     * the cluster's maxAsduLength parameter. It should not exceed
     * ZB_APS_CONST_MAX_FRAG_SIZE. */

    enum ZclStatusCodeT (*callback)(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrCbInfoT *info);
    /**< If flags ZCL_ATTR_FLAG_CB_READ or ZCL_ATTR_FLAG_CB_WRITE are set, then
     * this callback is called for attribute read or write commands. */

    struct {
        long long min;
        long long max;
    } integer_range;
    /**< Optional integer attribute value range. If both are set to zero, range checking is disabled. */

    struct {
        uint16_t interval_min; /* seconds */
        uint16_t interval_max; /* seconds */
    } reporting;
    /**< Default attribute minimum and maximum reporting intervals in seconds. */
};

#define ZCL_ATTR_LIST_LEN(_list_)        (sizeof(_list_) / sizeof(struct ZbZclAttrT))

/* Appends attributes to the cluster.
 * Important note: attrList memory is attached to the cluster, so
 * it cannot be freed until the cluster is freed. */
enum ZclStatusCodeT ZbZclAttrAppendList(struct ZbZclClusterT *clusterPtr,
    const struct ZbZclAttrT *attrList, unsigned int num_attrs);

/** Attribute Callback Types */
enum ZbZclAttrCbTypeT {
    ZCL_ATTR_CB_TYPE_READ,
    /**< Read attribute callback. Use to have the application handle
     * the ZCL Read Requests, rather than the cluster. */
    ZCL_ATTR_CB_TYPE_WRITE,
    /**< Write attribute callback. Used to have the application handle
     * the ZCL Write Requests, rather than the cluster. If not used, the
     * cluster will handle the incoming requests. */
    ZCL_ATTR_CB_TYPE_NOTIFY
    /**< Write notification callback. Can be used if ZCL_ATTR_CB_TYPE_WRITE
     * is not being used and the applicaiton needs to be notified of
     * ZCL Write Requests. */
};

/** Attribute Callback Information */
struct ZbZclAttrCbInfoT {
    const struct ZbZclAttrT *info;
    /**< Pointer to the original info struct used to create the attribute */

    enum ZbZclAttrCbTypeT type; /**< Callback type (read, write, etc) */

    uint8_t *zcl_data;
    /**< Incoming or outgoing ZCL attribute payload data.
     *
     * If callback type is ZCL_ATTR_CB_TYPE_WRITE, this is the source of the new value,
     * to be validated and written back to attr_data if applicable.
     *
     * If callback type is ZCL_ATTR_CB_TYPE_READ, this is the destination of the value
     * to return in the ZCL Read Response. */

    unsigned int zcl_len; /**< Maximum length of 'zcl_data' */

    ZclWriteModeT write_mode;
    /**< Write mode (e.g. normal, test, force, persist).
     * Only applicable if type == ZCL_ATTR_CB_TYPE_WRITE. */

    void *attr_data;
    /**< Pointer to local attribute storage.
     * Only applicable if type == ZCL_ATTR_CB_TYPE_WRITE.
     *
     * If NULL, then application is maintaining attribute value
     * (i.e. both ZCL_ATTR_FLAG_CB_READ and ZCL_ATTR_FLAG_CB_WRITE were set).
     *
     * If not NULL, current attribute value can be read from here, and
     * updated value from WRITE command to be written here.
     */

    const struct ZbApsAddrT *src;
    /**< Source of the command, if not locally generated. May be NULL. */

    void *app_cb_arg; /**< Application's defined callback argument */
};

/* Returns the length of an attribute, solely based on type. Or 0, if length is unknown. */
unsigned int ZbZclAttrTypeLength(enum ZclDataTypeT type);

/* Returns the attribute length, or -1 on error. */
int ZbZclAttrDefaultValue(enum ZclDataTypeT type, uint8_t *buf, unsigned int max_len);

/*---------------------------------------------------------------
 * Alarm Cluster Helpers
 *---------------------------------------------------------------
 */
/**
 * ZbZclClusterReverseBind is used by the Alarms Server so it can receive loopback
 * messages sent in the direction of the client (i.e. reverse).
 * @param clusterPtr
 * @return Filter pointer (handle)
 */
struct ZbApsFilterT * ZbZclClusterReverseBind(struct ZbZclClusterT *clusterPtr);

/**
 * Remove the reverse (loopback) filter and free it.
 * @param clusterPtr
 * @param filter Filter to remove and free
 * @return None
 */
void ZbZclClusterReverseUnbind(struct ZbZclClusterT *clusterPtr, struct ZbApsFilterT *filter);

/**
 * Send an alarm from a cluster as if it originated from the alarms cluster
 * on the same endpoint. Typically, src_endpoint = ZbZclClusterGetEndpoint(clusterPtr).
 * If the cluster appears on multiple endpoints (e.g. Basic Cluster), the src_endpoint
 * is required to know which one to actually send the Alarm Command from.
 * The alarm message is sent via APS binding(s).
 * @param clusterPtr
 * @param src_endpoint
 * @param alarm_code
 * @return None
 */
void ZbZclClusterSendAlarm(struct ZbZclClusterT *clusterPtr, uint8_t src_endpoint, uint8_t alarm_code);

/**
 * Register a callback handler for the given cluster to receive Alarm "Reset Alarm"
 * and "Reset all alarms" commands.
 * @param clusterPtr Cluster instance
 * @param callback Application callback to handle reset alarm(s) commands.
 * @return ZCL Status Code
 */
enum ZclStatusCodeT ZbZclClusterRegisterAlarmResetHandler(struct ZbZclClusterT *clusterPtr,
    enum ZclStatusCodeT (*callback)(struct ZbZclClusterT *clusterPtr, uint8_t alarm_code,
        uint16_t cluster_id, struct ZbApsdeDataIndT *data_ind, struct ZbZclHeaderT *hdr));
void ZbZclClusterRemoveAlarmResetHandler(struct ZbZclClusterT *clusterPtr);

/*---------------------------------------------------------------
 * Cluster Base Type
 *---------------------------------------------------------------
 */
/* This structure describes a ZCL cluster */
struct ZbZclClusterT {
    struct LinkListT link; /* Optional linking for application use. */
    struct ZigBeeT *zb; /* ZigBee stack reference. */

    /*-------------------------------------------
     * Cluster Basics
     *-------------------------------------------
     */
    enum ZbZclClusterIdT clusterId;
    uint8_t endpoint;
    uint16_t mfrCode;
    /* minSecurity is one of: ZB_APS_STATUS_UNSECURED, ZB_APS_STATUS_SECURED_NWK_KEY or ZB_APS_STATUS_SECURED_LINK_KEY */
    enum ZbStatusCodeT minSecurity;
    uint16_t profileId;
    uint16_t txOptions; /* e.g. ZB_APSDE_DATAREQ_TXOPTIONS_ACK */
    bool discoverRoute;
    uint8_t radius;
    uint16_t maxAsduLength; /* with fragmentation, if enabled in txOptions. */
    /* Direction of incoming commands. I.e. the type of cluster this is
     * (client or server). */
    enum ZbZclDirectionT direction;

    /*-------------------------------------------
     * Filters, Lists, Timers
     *-------------------------------------------
     */
    struct ZbApsFilterT *dataind_filter;
    struct ZbApsFilterT *alarm_reset_filter;
    enum ZclStatusCodeT (*alarm_reset_callback)(struct ZbZclClusterT *clusterPtr, uint8_t alarm_code,
        uint16_t cluster_id, struct ZbApsdeDataIndT *data_ind, struct ZbZclHeaderT *hdr);
    struct LinkListT attributeList;
    struct LinkListT reports;
    struct ZbTimerT *persist_timer;

    /*-------------------------------------------
     * Callbacks
     *-------------------------------------------
     */
    /* The application argument passed through the various callbacks
     * to the application (e.g. custom attribute read/write). */
    void *app_cb_arg;

    /* Return value is a ZCL_STATUS_ code. If not ZCL_STATUS_SUCCESS, a default
     * response with the status code is sent to the originator of the command. */
    enum ZclStatusCodeT (*command)(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr,
        struct ZbApsdeDataIndT *dataIndPtr);

    /* Configure reporting */
    void (*config)(struct ZbZclClusterT *clusterPtr, struct ZbApsdeDataIndT *dataIndPtr, uint16_t attributeId,
        enum ZclStatusCodeT status, uint8_t direction);

    void (*report)(struct ZbZclClusterT *clusterPtr, struct ZbApsdeDataIndT *dataIndPtr, uint16_t attributeId,
        enum ZclDataTypeT dataType, const uint8_t *in_payload, uint16_t in_len);

    /* ZCL Internal Callback for the Scenes Cluster to Get a cluster's attribute
     * extension set.
     * extBuf has the format: [cluster (2 octets) | length (1 octet) | attribute data (N octets)]
     *
     * Returns length written to extBuf.  */
    uint8_t (*get_scene_data)(struct ZbZclClusterT *clusterPtr, uint8_t *extBuf, uint8_t extMaxLen);

    /* ZCL Internal Callback for the Scenes Cluster to Set a cluster's attribute
     * extension set.#define ZCL_BUILD_ADD_UINT8(_payload_, _max_len_, _index_, _arg_)  \
        do {                                                             \
            if (((_index_) + sizeof(uint8_t)) > (_max_len_)) {           \
                return -1;                                               \
            }                                                            \
            if (_payload_) {                                             \
                (_payload_)[_index_] = _arg_;                            \
            }                                                            \
            _index_ += sizeof(uint8_t);                                  \
     * extData has the format: [attribute data (N octets)]
     * extLen is the length of extData
     * transition_tenths ; transition time in tenths of a second.
     *
     * Returns ZCL status code (enum ZclStatusCodeT) */
    enum ZclStatusCodeT (*set_scene_data)(struct ZbZclClusterT *clusterPtr, uint8_t *extData, uint8_t extLen, uint16_t transition_tenths);

    /* cleanup() is called to free any cluster-specific data.
     * ZbZclClusterFree will already free any attributes and APS filter. */
    void (*cleanup)(struct ZbZclClusterT *clusterPtr);
};

/*---------------------------------------------------------------
 * Cluster Initialization
 *---------------------------------------------------------------
 */
/* Helper to create a ZCL endpoint. Calls ZbApsmeAddEndpoint to create the endpoint. Sets up a
 * filter to catch any ZCL commands for a cluster that doesn't exist on the endpoint, and
 * returns ZCL_STATUS_UNSUPPORTED_CLUSTER. Sets up filter bindings for the Basic Server
 * cluster (global stack clusters) and register's their cluster IDs with the endpoint
 * (e.g. for ZDO Match_Desc_req)  */
void ZbZclAddEndpoint(struct ZigBeeT *zb, struct ZbApsmeAddEndpointReqT *req, struct ZbApsmeAddEndpointConfT *conf);
/* ZbZclAddEndpointNoBasic is the same as ZbZclAddEndpoint, except it does not allocate
 * a Basic Server cluster for the endpoint. */
void ZbZclAddEndpointNoBasic(struct ZigBeeT *zb, struct ZbApsmeAddEndpointReqT *addReqPtr, struct ZbApsmeAddEndpointConfT *addConfPtr);

/* Helper to remove a ZCL endpoint created by ZbZclAddEndpoint.
 * Calls ZbApsmeRemoveEndpoint and removes the ZCL filters created by ZbZclAddEndpoint. */
void ZbZclRemoveEndpoint(struct ZigBeeT *zb, struct ZbApsmeRemoveEndpointReqT *req, struct ZbApsmeRemoveEndpointConfT *conf);

/* Allocate and initialize a new cluster */
void * ZbZclClusterAlloc(struct ZigBeeT *zb, unsigned int alloc_sz, enum ZbZclClusterIdT cluster_id,
    uint8_t endpoint, enum ZbZclDirectionT direction);
/* Attach the cluster to the stack (after callbacks have been configured) */
enum ZclStatusCodeT ZbZclClusterAttach(struct ZbZclClusterT *clusterPtr);
/* Free and detach the cluster */
void ZbZclClusterFree(struct ZbZclClusterT *clusterPtr);

/* Register this cluster's ID on the endpoint if not already listed. */
bool ZbZclClusterEndpointRegister(struct ZbZclClusterT *clusterPtr);
bool ZbZclClusterEndpointRemove(struct ZbZclClusterT *clusterPtr);

/*---------------------------------------------------------------
 * Building and Parsing Frames
 *---------------------------------------------------------------
 */
/** Parses a ZCL Header from a received frame.
 * Returns Length of the ZCL header, or negative value (-1) on error. */
int ZbZclParseHeader(struct ZbZclHeaderT *zclHdrPtr, const uint8_t *buf, unsigned int len);

/** Builds and appends a ZCL Header to the start of a buffer.
 * Returns length of data written, or negative value (-1) on error. */
int ZbZclPrependHeader(struct ZbZclHeaderT *zclHdrPtr, uint8_t *data, unsigned int len);

/** Builds and appends a ZCL Header to the end of a buffer.
 * Returns Length of data written, or negative value (-1) on error. */
int ZbZclAppendHeader(struct ZbZclHeaderT *zclHdrPtr, uint8_t *data, unsigned int max_len);

/*---------------------------------------------------------------
 * Generic Attribute Functions
 *---------------------------------------------------------------
 */
int ZbZclAttrParseLength(enum ZclDataTypeT type, const uint8_t *ptr, unsigned int max_len, uint8_t recurs_depth);

bool ZbZclAttrIsFloat(enum ZclDataTypeT dataType);
bool ZbZclAttrIsAnalog(enum ZclDataTypeT dataType);
bool ZbZclAttrIsInteger(enum ZclDataTypeT dataType);

int ZbZclAppendInteger(unsigned long long value, enum ZclDataTypeT dataType, uint8_t *data, unsigned int len);
long long ZbZclParseInteger(enum ZclDataTypeT dataType, const uint8_t *data, enum ZclStatusCodeT *statusPtr);

int ZbZclAppendFloat(double value, enum ZclDataTypeT dataType, uint8_t *data, unsigned int len);
double ZbZclParseFloat(enum ZclDataTypeT dataType, const uint8_t *data);

/*---------------------------------------------------------------
 * Local Reading and Writing of Attributes
 *---------------------------------------------------------------
 */

/*FUNCTION
 *  ZbZclAttrRead
 *PARAMETERS
 *  clusterPtr      ; IN: Cluster Structure Pointer
 *  attrId          ; IN: Cluster's Attribute ID
 *  attrType        ; OUT (optional): Attribute Data Type
 *  outputBuf       ; OUT: Attribute data in ZCL format (i.e. same as what is sent over-the-air)
 *  max_len         ; IN: Maximum length that can be written to outputBuf.
 *  isReporting     ; IN: This read is from ZCL reporting. If the attribute does not support reporting, the read request will fail.
 */
enum ZclStatusCodeT ZbZclAttrRead(struct ZbZclClusterT *clusterPtr, uint16_t attrId, enum ZclDataTypeT *attrType,
    void *outputBuf, unsigned int max_len, bool isReporting);

/*FUNCTION
 *  ZbZclAttrWrite
 *PARAMETERS
 *  clusterPtr      ; IN: Cluster Structure Pointer
 *  src             ; IN: Source address of this command. May be NULL if generated locally.
 *  attr_id         ; IN: Cluster's Attribute ID
 *  attr_data       ; IN: Attribute data in ZCL format (i.e. same as what is sent over-the-air)
 *  max_len         ; IN: Length of attribute data
 *  mode            ; IN: Write mode
 */
enum ZclStatusCodeT ZbZclAttrWrite(struct ZbZclClusterT *clusterPtr, const struct ZbApsAddrT *src, uint16_t attr_id,
    const uint8_t *attr_data, unsigned int max_len, ZclWriteModeT mode);

/* Integer Attribute Helpers */
long long ZbZclAttrIntegerRead(struct ZbZclClusterT *clusterPtr, uint16_t attributeId,
    enum ZclDataTypeT *typePtr, enum ZclStatusCodeT *statusPtr);
enum ZclStatusCodeT ZbZclAttrIntegerWrite(struct ZbZclClusterT *clusterPtr, uint16_t attributeId, long long value);
enum ZclStatusCodeT ZbZclAttrIntegerIncrement(struct ZbZclClusterT *clusterPtr, uint16_t attributeId, long long value);

uint64_t ZbZclAttrEuiRead(struct ZbZclClusterT *clusterPtr, uint16_t attributeId, enum ZclStatusCodeT *statusPtr);
enum ZclStatusCodeT ZbZclAttrEuiWrite(struct ZbZclClusterT *clusterPtr, uint16_t attributeId, uint64_t eui);

/* String Attribute Helpers */
/* zcl_str is in the ZCL string format (length field [1 or 2 bytes] + ascii string) */
/* ZCL_DATATYPE_STRING_OCTET or ZCL_DATATYPE_STRING_CHARACTER */
enum ZclStatusCodeT ZbZclAttrStringWriteShort(struct ZbZclClusterT *clusterPtr, uint16_t attributeId,
    const uint8_t *zcl_str);
/* ZCL_DATATYPE_STRING_LONG_OCTET or ZCL_DATATYPE_STRING_LONG_CHARACTER */
enum ZclStatusCodeT ZbZclAttrStringWriteLong(struct ZbZclClusterT *clusterPtr, uint16_t attributeId,
    const uint8_t *zcl_str);

/* Zero-length ZCL strings */
extern const uint8_t zcl_attr_str_short_zero[1]; /* i.e. {0x00} */
extern const uint8_t zcl_attr_str_long_zero[2]; /* i.e. {0x00, 0x00} */

/*---------------------------------------------------------------
 * Configure Reporting
 *---------------------------------------------------------------
 */
struct ZbZclAttrReportConfigT {
    struct ZbApsAddrT dst;
    uint16_t min;
    uint16_t max;
    uint16_t attr_id;
    uint8_t attr_type;
    uint64_t change;
};

enum ZclStatusCodeT ZbZclAttrReportConfigReq(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrReportConfigT *report,
    void (*callback)(struct ZbZclCommandRspT *cmd_rsp, void *arg), void *arg);

struct ZbZclAttrReportReadT {
    struct ZbApsAddrT dst;
    uint16_t attr_id;
};

enum ZclStatusCodeT ZbZclAttrReportReadReq(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrReportReadT *report,
    void (*callback)(struct ZbZclCommandRspT *cmd_rsp, void *arg), void *arg);

/*---------------------------------------------------------------
 * Remote Reading and Writing of Attributes
 *---------------------------------------------------------------
 */
/* The maximum number of attributes we support reading/writing through the API at once.
 * This is not the maximum that we support from requests arriving from the network.
 * Those depend on maximum ASDU limits and other resources. */
#define ZCL_ATTRIBUTE_LIST_MAX_SZ        8U

typedef struct ZbZclReadReqT {
    struct ZbApsAddrT dst;
    unsigned int count;
    uint16_t attr[ZCL_ATTRIBUTE_LIST_MAX_SZ];
} ZbZclReadReqT;

typedef struct ZbZclReadRspDataT {
    enum ZclStatusCodeT status;
    uint16_t attrId;
    enum ZclDataTypeT type;
    const uint8_t *value; /* Value as-is over-the-air (strings include the length octet(s)) */
    unsigned int length;
} ZbZclReadRspDataT;

typedef struct ZbZclReadRspT {
    enum ZclStatusCodeT status;
    struct ZbApsAddrT src;
    uint16_t profileId;
    uint8_t linkQuality;
    unsigned int count;
    struct ZbZclReadRspDataT attr[ZCL_ATTRIBUTE_LIST_MAX_SZ];
} ZbZclReadRspT;

enum ZclStatusCodeT ZbZclReadReq(struct ZbZclClusterT *clusterPtr, struct ZbZclReadReqT *req,
    void (*callback)(const struct ZbZclReadRspT *readRsp, void *cb_arg), void *arg);

typedef struct ZbZclWriteReqDataT {
    /* EXEGIN: implement a selector for structured version. */
    uint16_t attrId;
    enum ZclDataTypeT type;
    const uint8_t *value; /* in zigbee format (integers are little endian) */
    unsigned int length;
} ZbZclWriteReqDataT;

typedef struct ZbZclWriteRspDataT {
    enum ZclStatusCodeT status;
    uint16_t attrId;
} ZbZclWriteRspDataT;

typedef struct ZbZclWriteReqT {
    struct ZbApsAddrT dst;
    unsigned int count;
    struct ZbZclWriteReqDataT attr[ZCL_ATTRIBUTE_LIST_MAX_SZ];
} ZbZclWriteReqT;

typedef struct ZbZclWriteRspT {
    enum ZclStatusCodeT status;
    struct ZbApsAddrT src;
    uint16_t profileId;
    uint8_t linkQuality;
    unsigned int count;
    struct ZbZclWriteRspDataT attr[ZCL_ATTRIBUTE_LIST_MAX_SZ];
} ZbZclWriteRspT;

enum ZclStatusCodeT ZbZclWriteReq(struct ZbZclClusterT *clusterPtr, struct ZbZclWriteReqT *req,
    void (*callback)(const struct ZbZclWriteRspT *writeResp, void *cb_arg), void *arg);

/*---------------------------------------------------------------
 * Attribute Discovery
 *---------------------------------------------------------------
 */
typedef struct ZbZclDiscoverAttrReqT {
    struct ZbApsAddrT dst;
    uint16_t startAttr;
    uint8_t max;
} ZbZclDiscoverAttrReqT;

typedef struct ZbZclDiscoverAttrRspDataT {
    uint16_t attrId;
    enum ZclDataTypeT type;
} ZbZclDiscoverAttrRspDataT;

typedef struct ZbZclDiscoverAttrRspT {
    enum ZclStatusCodeT status;
    struct ZbApsAddrT src;
    uint8_t discComplete;
    struct ZbZclDiscoverAttrRspDataT attrInfo[ZCL_ATTRIBUTE_LIST_MAX_SZ];
    uint8_t numAttr;
} ZbZclDiscoverAttrRspT;

enum ZclStatusCodeT ZbZclDiscoverAttrReq(struct ZbZclClusterT *clusterPtr, struct ZbZclDiscoverAttrReqT *req,
    void (*callback)(const struct ZbZclDiscoverAttrRspT *discRsp, void *cb_arg), void *arg);

/*---------------------------------------------------------------
 * Attribute Reporting
 *---------------------------------------------------------------
 */
enum ZbZclReportDirectionT {
    /* For Configure Reporting: receiver configures attribute to send
     * reports via binding table.
     *
     * For Read Reporting: specifies whether values of the attribute are reported */
    ZCL_REPORT_DIRECTION_NORMAL = 0x00,

    /* For Configure Reporting: sender is sending reports to the receiver,
     * based on the state of the sender's bindings. Tells receiver how / when
     * it should expect reports.
     *
     * For Read Reporting: specifies whether reports of the attribute are received. */
    ZCL_REPORT_DIRECTION_REVERSE = 0x01
};

/* ZCL Reporting Configuration Record Structure. */
struct ZbZclReportConfigT {
    enum ZbZclReportDirectionT direction;
    uint16_t attributeId;
    enum ZclDataTypeT dataType;
    uint16_t minInterval;
    uint16_t maxInterval;
    union {
        long long integer;
        double floating;
    } minChange;
    uint16_t reportTimeout;
};

/* Appends a Reporting configuration record to the end of the provided buffer.
 * Returns Length of data written, or negative value (-1) on error. */
int ZbZclAppendReportConfig(struct ZbZclReportConfigT *configPtr, uint8_t *payload, unsigned int max_len);

/*---------------------------------------------------------------
 * Attribute Persistence
 *---------------------------------------------------------------
 */
/* If an attribute is modified outside of the normal ZCL Write
 * mechanism, the application must call this function to inform
 * the stack to persist the new attribute data. */
bool ZbZclAttrPersist(struct ZbZclClusterT *clusterPtr, uint16_t attr_id);

/*---------------------------------------------------------------
 * Cluster Request helpers
 *---------------------------------------------------------------
 */
struct ZbZclClusterCommandReqT {
    /* Destination for command */
    struct ZbApsAddrT dst;
    /* Command ID */
    uint8_t cmdId;
    /* If noDefaultResp is TRUE, disables a Default Response on status = SUCCESS. */
    enum ZbZclNoDefaultResponseT noDefaultResp;
    /* ZCL Request Command Payload */
    uint8_t *payload;
    unsigned int length;
};

/* ZbZclClusterCommandReq and ZbZclClusterCommandWait are wrappers to ZbZclCommandReq. */
/* clusterPtr = The cluster that is originating this message. Information from the cluster is used when
 * sending the command (source addressing, APS TX Options).
 * "callback" may be NULL */
enum ZclStatusCodeT ZbZclClusterCommandReq(struct ZbZclClusterT *clusterPtr, struct ZbZclClusterCommandReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/*---------------------------------------------------------------
 * Cluster Response helpers
 *---------------------------------------------------------------
 */
/* ZbZclClusterCommandRsp sends a cluster-specific response to the destination provided.
 * There is no checking if the originating request was sent to broadcast, etc. That
 * checking must be done by the caller before this function is called.
 *
 * Returns ZCL_STATUS_SUCCESS if the message was queued to the stack, or an error status
 * otherwise. */
enum ZclStatusCodeT ZbZclClusterCommandRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo,
    uint8_t cmdId, struct ZbApsBufT *payloads, uint8_t numPayloads);

enum ZclStatusCodeT ZbZclClusterCommandRspWithCb(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo,
    uint8_t cmdId, struct ZbApsBufT *payloads, uint8_t numPayloads, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg);

/* ZbZclSendClusterStatusResponse is a wrapper to ZbZclClusterCommandRsp. The parameters
 * zclPayload and zclPaylen must be populated and the first byte must represent a status
 * code of some sort with success == 0x00. Most ZCL respones follow this format. If the
 * request was broadcast (address or endpoint) and status != 0x00 (ZCL_STATUS_SUCCESS),
 * then the response will not be sent. This simplifies the cluster application from having
 * to make these checks. */
enum ZclStatusCodeT ZbZclSendClusterStatusResponse(struct ZbZclClusterT *clusterPtr,
    struct ZbApsdeDataIndT *dataIndPtr, struct ZbZclHeaderT *zclHdrPtr, uint8_t cmdId,
    uint8_t *zclPayload, uint8_t zclPaylen);

/* Send a Default Response. */
void ZbZclSendDefaultResponse(struct ZbZclClusterT *clusterPtr, struct ZbApsdeDataIndT *dataIndPtr,
    struct ZbZclHeaderT *zclHdrPtr, enum ZclStatusCodeT status);
/* Different version of ZbZclSendDefaultResponse, but takes in ZbZclAddrInfoT rather than
 * struct ZbApsdeDataIndT and ZbZclHeaderT */
void ZbZclSendDefaultResponse2(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo,
    uint8_t cmdId, enum ZclStatusCodeT status);

/*---------------------------------------------------------------
 * Bind a cluster so it can receive messages directly from the APS layer
 *---------------------------------------------------------------
 */
/* Binds the cluster structure to a particular endpoint and optional remote
 * address, and sets up a packet filter to handle the command reception. */
enum ZclStatusCodeT ZbZclClusterBind(struct ZbZclClusterT *clusterPtr, uint8_t endpoint,
    uint16_t profileId, enum ZbZclDirectionT direction);
void ZbZclClusterUnbind(struct ZbZclClusterT *clusterPtr);

/*---------------------------------------------------------------
 * Device Log (whitelist)
 *---------------------------------------------------------------
 */
/* Since the Trust Center address is not known ahead of time, joiners using CBKE will automatically
 * add the Trust Center to the Device Log, so they can perform CBKE if Device Log is enabled.
 * The TC is removed if CBKE fails. */
void ZbZclDeviceLogEnable(struct ZigBeeT *zb, bool enable);
bool ZbZclDeviceLogAdd(struct ZigBeeT *zb, uint64_t ext_addr);
bool ZbZclDeviceLogRemove(struct ZigBeeT *zb, uint64_t ext_addr);
void ZbZclDeviceLogClear(struct ZigBeeT *zb);

/*---------------------------------------------------------------
 * ZCL Internal Use Only
 *---------------------------------------------------------------
 */
enum ZclStateReqReturn {
    ZCL_STATE_CONTINUE = 0, /**< Return zero to continue waiting for responses. */
    ZCL_STATE_CLEANUP /**< Return one to cleanup the state. */
};

typedef enum ZclStateReqReturn (*ZbZclHandlerFuncT)(struct ZigBeeT *zb, struct ZbZclCommandRspT *cmdRsp, void *callback, void *arg);

enum ZclStatusCodeT ZbZclStateBegin(struct ZigBeeT *zb, struct ZbApsdeDataReqT *apsReq, struct ZbZclHeaderT *zclHdr,
    unsigned int timeout, ZbZclHandlerFuncT handler, void *callback, void *arg);

/*---------------------------------------------------------------
 * Helper Functions
 *---------------------------------------------------------------
 */
/* Get the next ZCL sequence number to use in a request/notify message. */
uint8_t ZbZclGetNextSeqnum(void);

/* Helper functions to SET cluster parameters */
void ZbZclClusterSetCallbackArg(struct ZbZclClusterT *clusterPtr, void *app_cb_arg);
void ZbZclClusterSetMfrCode(struct ZbZclClusterT *clusterPtr, uint16_t mfrCode);
void ZbZclClusterSetProfileId(struct ZbZclClusterT *clusterPtr, uint16_t profileId);
/* Sets the minimum security level for incoming frames, and also adjusts
 * the tx options to match (assuming symmetrical security).
 * minSecurity can be one of: ZB_APS_STATUS_UNSECURED, ZB_APS_STATUS_SECURED_NWK_KEY or ZB_APS_STATUS_SECURED_LINK_KEY  */
bool ZbZclClusterSetMinSecurity(struct ZbZclClusterT *clusterPtr, enum ZbStatusCodeT minSecurity);
/* e.g. ZB_APSDE_DATAREQ_TXOPTIONS_ACK, ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY */
void ZbZclClusterSetTxOptions(struct ZbZclClusterT *clusterPtr, uint16_t txOptions);
void ZbZclClusterSetDiscoverRoute(struct ZbZclClusterT *clusterPtr, bool discoverRoute);
void ZbZclClusterSetRadius(struct ZbZclClusterT *clusterPtr, uint8_t radius);
/* Set the maximum ASDU length for the cluster. The ASDU length includes
 * fragmentation, if enabled in the cluster's tx options. */
bool ZbZclClusterSetMaxAsduLength(struct ZbZclClusterT *clusterPtr, uint16_t maxAsduLength);

/* Helper functions to initialize requests based on cluster parameters */
void ZbZclClusterInitCommandReq(struct ZbZclClusterT *clusterPtr, struct ZbZclCommandReqT *cmdReq);
void ZbZclClusterInitApsdeReq(struct ZbZclClusterT *clusterPtr, struct ZbApsdeDataReqT *apsReq, struct ZbApsdeDataIndT *dataIndPtr);
/* Helper to generate proper APS TX Options (e.g. ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY)
 * for a response, based on the given (incoming message) security status code. */
uint16_t ZbZclTxOptsFromSecurityStatus(enum ZbStatusCodeT security_status);

/* Helper functions to GET cluster parameters */
uint8_t ZbZclClusterGetEndpoint(struct ZbZclClusterT *clusterPtr);
enum ZbZclClusterIdT ZbZclClusterGetClusterId(struct ZbZclClusterT *clusterPtr);
uint16_t ZbZclClusterGetProfileId(struct ZbZclClusterT *clusterPtr);
uint16_t ZbZclClusterGetTxOptions(struct ZbZclClusterT *clusterPtr);
enum ZbZclDirectionT ZbZclClusterGetDirection(struct ZbZclClusterT *clusterPtr);
const char * ZbZclClusterGetDirectionStr(struct ZbZclClusterT *clusterPtr);
uint8_t ZbZclClusterGetRadius(struct ZbZclClusterT *clusterPtr);
unsigned int ZbZclClusterGetMaxAsduLength(struct ZbZclClusterT *clusterPtr);

void ZbZclClusterReportCallbackAttach(struct ZbZclClusterT *clusterPtr,
    void (*callback)(struct ZbZclClusterT *clusterPtr, struct ZbApsdeDataIndT *dataIndPtr,
        uint16_t attr_id, enum ZclDataTypeT data_type, const uint8_t *in_payload, uint16_t in_len));

/*---------------------------------------------------------------
 * Exegin Internal ZCL Loopback Commands
 *---------------------------------------------------------------
 */
/* Our internal Manufacturer Code (ZCL_FRAMECTRL_MANUFACTURER).
 * This code is never sent over the air. */
enum {
    ZCL_MANUF_CODE_INTERNAL = 0xfffe
};

/* Offsets into ZCL_CMD_MANUF_INTERNAL_SET_SCENE_EXTDATA message */
#define SET_SCENE_EXTDATA_OFFSET_TRANSITION             0U /* 4 octets (tenths of a second) */
#define SET_SCENE_EXTDATA_OFFSET_EXT_LEN                4U /* 1 octet */
#define SET_SCENE_EXTDATA_OFFSET_EXT_FIELD              5U /* variable length */
#define SET_SCENE_EXTDATA_HEADER_LEN                    (4U + 1U)

enum {
    ZCL_PERSIST_SVR_CMD_PUSH
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ZCL_H */
