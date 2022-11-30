/**
 * @file zcl.enum.h
 * @brief Zigbee ZCL Enumerations header file.
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ENUM_H
# define ZCL_ENUM_H

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h> /* NULL */
#include "pletoh.h" /* Little endian conversion */
#include "llist.h" /* Link list */

struct ZigBeeT;
struct ZbApsAddrT;
struct ZbStartupT;
struct ZbZclClusterT;
struct ZbZclAddrInfoT;
struct ZbZclCommandReqT;
struct ZbZclCommandRspT;

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
    ZCL_GLOBAL_ATTR_CLUSTER_REV = 0xfffd,
    /**< ClusterRevision. Mandatory. This attribute is automatically allocated and initialized
     * by a cluster's allocation function when it calls ZbZclClusterAlloc. It does not need to
     * be included in the cluster's attribute list during initialization. The value may be
     * modified by the cluster or application after initialization (ZbZclAttrIntegerWrite).
     * The default value is 1. All clusters starting with ZCL Spec Revision 6, have their
     * revision start at 1. */

    ZCL_GLOBAL_ATTR_REPORTING_STATUS = 0xfffe,
    /**< AttributeReportingStatus. Optional. This is a dummy attribute that can go at the end
     * of each ZCL Report Command to indicate whether additional report commands are being
     * sent shortly after, or if this is the last report command in a series of reports.
     * This is currently not being used when sending reports, because it's not very useful. */

    ZCL_GLOBAL_ATTR_UNDEFINED = 0xffff /**< Not defined in Spec. Internal use only. */
};

/* AttributeReportingStatus (ZCL_GLOBAL_ATTR_REPORTING_STATUS) values. */
enum {
    ZCL_ATTR_REPORTING_STATUS_PENDING = 0x00, /**< Pending */
    ZCL_ATTR_REPORTING_STATUS_COMPLETE = 0x01 /**< Attribute Reporting Complete */
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
/* Enables ZCL_ATTR_CB_TYPE_READ for this attribute */
#define ZCL_ATTR_FLAG_CB_READ               (ZclAttrFlagT)0x0010U
/* Enables ZCL_ATTR_CB_TYPE_WRITE for this attribute */
#define ZCL_ATTR_FLAG_CB_WRITE              (ZclAttrFlagT)0x0020U
#define ZCL_ATTR_FLAG_CB_NOTIFY             (ZclAttrFlagT)0x0040U
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
#define ZCL_INVALID_BOOLEAN                     0xffU
#define ZCL_INVALID_UNSIGNED_8BIT               0xffU
#define ZCL_INVALID_UNSIGNED_16BIT              0xffffU
#define ZCL_INVALID_UNSIGNED_24BIT              0xffffffU
#define ZCL_INVALID_UNSIGNED_32BIT              0xffffffffU
#define ZCL_INVALID_UNSIGNED_40BIT              0xffffffffffULL
#define ZCL_INVALID_UNSIGNED_48BIT              0xffffffffffffULL
#define ZCL_INVALID_UNSIGNED_56BIT              0xffffffffffffffULL
#define ZCL_INVALID_UNSIGNED_64BIT              0xffffffffffffffffULL

#define ZCL_INVALID_SIGNED_8BIT                 (-128) /* 0x80 */
#define ZCL_INVALID_SIGNED_16BIT                (-32768) /* 0x8000 */
#define ZCL_INVALID_SIGNED_24BIT                (-8388608) /* 0x800000 */
#define ZCL_INVALID_SIGNED_32BIT                (-2147483648) /* 0x80000000 */
#define ZCL_INVALID_SIGNED_40BIT                (-549755813888LL) /* 0x8000000000 */
#define ZCL_INVALID_SIGNED_48BIT                (-140737488355328LL) /* 0x800000000000 */
#define ZCL_INVALID_SIGNED_56BIT                (-36028797018963968LL) /* 0x80000000000000 */
/*lint -e9048 "unsigned integer literal without a 'U' suffix [MISRA Rule 7.2 REQUIRED]"*/
#define ZCL_INVALID_SIGNED_64BIT                0x8000000000000000 /* integer constant is so large that it is unsigned  */
/*lint -restore */

#define ZCL_INVALID_ENUMERATION_8BIT            0xffU
#define ZCL_INVALID_ENUMERATION_16BIT           0xffffU

#define ZCL_INVALID_FLOATING                    (0.0 / 0.0)
/* Note, can't bit-or signed values (MISRA rule 10.1) */
#define ZCL_INVALID_FLOATING_SEMI               (ZCL_FLOAT_SEMI_MANTISSA + ZCL_FLOAT_SEMI_EXPONENT)
#define ZCL_INVALID_FLOATING_SINGLE             (ZCL_FLOAT_SINGLE_MANTISSA + ZCL_FLOAT_SINGLE_MANTISSA)
#define ZCL_INVALID_FLOATING_DOUBLE             (ZCL_FLOAT_DOUBLE_MANTISSA + ZCL_FLOAT_DOUBLE_MANTISSA)

#define ZCL_INVALID_STRING_OCTET                0xffU
#define ZCL_INVALID_STRING_CHARACTER            0xffU
#define ZCL_INVALID_STRING_LONG_OCTET           0xffffU
#define ZCL_INVALID_STRING_LONG_CHARACTER       0xffffU

#define ZCL_INVALID_ARRAY                       0xffffU
#define ZCL_INVALID_STRUCT                      0xffffU
#define ZCL_INVALID_SET                         0xffffU
#define ZCL_INVALID_BAG                         0xffffU
#define ZCL_INVALID_TIME_OF_DAY                 0xffffffffU
#define ZCL_INVALID_DATE                        0xffffffffU
#define ZCL_INVALID_TIME_UTC                    0xffffffffU
#define ZCL_INVALID_CLUSTER_ID                  0xffffU
#define ZCL_INVALID_ATTRIBUTE_ID                0xffffU
#define ZCL_INVALID_BACNET_OID                  0xffffffffU
#define ZCL_INVALID_EUI64                       0xffffffffffffffffULL

/* ZCL Valid Ranges */
#define ZCL_MIN_SIGNED_8BIT                     (-(ZCL_INVALID_SIGNED_8BIT - 1))
#define ZCL_MIN_SIGNED_16BIT                    (-(ZCL_INVALID_SIGNED_16BIT - 1))
#define ZCL_MIN_SIGNED_24BIT                    (-(ZCL_INVALID_SIGNED_24BIT - 1))
#define ZCL_MIN_SIGNED_32BIT                    (-(ZCL_INVALID_SIGNED_32BIT - 1))
#define ZCL_MIN_SIGNED_40BIT                    (-(ZCL_INVALID_SIGNED_40BIT - 1))
#define ZCL_MIN_SIGNED_48BIT                    (-(ZCL_INVALID_SIGNED_48BIT - 1))
#define ZCL_MIN_SIGNED_56BIT                    (-(ZCL_INVALID_SIGNED_56BIT - 1))
#define ZCL_MIN_SIGNED_64BIT                    (-(ZCL_INVALID_SIGNED_64BIT - 1))

#define ZCL_MAX_SIGNED_8BIT                     0x7f
#define ZCL_MAX_SIGNED_16BIT                    0x7fff
#define ZCL_MAX_SIGNED_24BIT                    0x7fffff
#define ZCL_MAX_SIGNED_32BIT                    0x7fffffff
#define ZCL_MAX_SIGNED_40BIT                    0x7fffffffffULL
#define ZCL_MAX_SIGNED_48BIT                    0x7fffffffffffULL
#define ZCL_MAX_SIGNED_56BIT                    0x7fffffffffffffULL
#define ZCL_MAX_SIGNED_64BIT                    0x7fffffffffffffffULL

#define ZCL_MAX_UNSIGNED_8BIT                   (ZCL_INVALID_UNSIGNED_8BIT - 1U)
#define ZCL_MAX_UNSIGNED_16BIT                  (ZCL_INVALID_UNSIGNED_16BIT - 1U)
#define ZCL_MAX_UNSIGNED_24BIT                  (ZCL_INVALID_UNSIGNED_24BIT - 1U)
#define ZCL_MAX_UNSIGNED_32BIT                  (ZCL_INVALID_UNSIGNED_32BIT - 1U)
#define ZCL_MAX_UNSIGNED_40BIT                  (ZCL_INVALID_UNSIGNED_40BIT - 1U)
#define ZCL_MAX_UNSIGNED_48BIT                  (ZCL_INVALID_UNSIGNED_48BIT - 1U)
#define ZCL_MAX_UNSIGNED_56BIT                  (ZCL_INVALID_UNSIGNED_56BIT - 1U)
#define ZCL_MAX_UNSIGNED_64BIT                  (ZCL_INVALID_UNSIGNED_64BIT - 1U)

#define ZCL_MIN_FLOAT_SEMI                      (-65504)
#define ZCL_MAX_FLOAT_SEMI                      (65504)

/* ZCL Status Codes */
enum ZclStatusCodeT {
    ZCL_STATUS_SUCCESS = 0x00,
    ZCL_STATUS_FAILURE = 0x01,
    ZCL_STATUS_REQUEST_DENIED = 0x70,
    ZCL_STATUS_NOT_AUTHORIZED = 0x7e,
    ZCL_STATUS_MALFORMED_COMMAND = 0x80,
    ZCL_STATUS_UNSUPP_COMMAND = 0x81,
    /* ZCL_STATUS_UNSUPP_CLUSTER_COMMAND deprecated. Use ZCL_STATUS_UNSUPP_COMMAND. */
    /* 0x82 reserved, ZCL_STATUS_UNSUPP_GENERAL_COMMAND deprecated. Use ZCL_STATUS_UNSUPP_COMMAND */
    /* 0x83 reserved, ZCL_STATUS_UNSUPP_MFR_CLUSTER_COMMAND deprecated. Use ZCL_STATUS_UNSUPP_COMMAND */
    /* 0x84 reserved, ZCL_STATUS_UNSUPP_MFR_GENERAL_COMMAND deprecated. Use ZCL_STATUS_UNSUPP_COMMAND */
    ZCL_STATUS_INVALID_FIELD = 0x85,
    ZCL_STATUS_UNSUPP_ATTRIBUTE = 0x86,
    ZCL_STATUS_INVALID_VALUE = 0x87,
    ZCL_STATUS_READ_ONLY = 0x88,
    ZCL_STATUS_INSUFFICIENT_SPACE = 0x89,
    /* 0x8a reserved, ZCL_STATUS_DUPLICATE_EXISTS deprecated. Use ZCL_STATUS_FAILURE */
    ZCL_STATUS_NOT_FOUND = 0x8b,
    ZCL_STATUS_UNREPORTABLE_ATTRIBUTE = 0x8c,
    ZCL_STATUS_INVALID_DATA_TYPE = 0x8d,
    ZCL_STATUS_INVALID_SELECTOR = 0x8e,
    /* 0x8f reserved, ZCL_STATUS_WRITE_ONLY deprecated. Use ZCL_STATUS_NOT_AUTHORIZED */
    /* 0x90 reserved, ZCL_STATUS_INCONSISTENT_STARTUP_STATE deprecated. Use ZCL_STATUS_FAILURE */
    /* 0x91 reserved, ZCL_STATUS_DEFINED_OUT_OF_BAND deprecated. Use ZCL_STATUS_FAILURE */
    /* 0x92 reserved CCB 2477 Status Code Cleanup: ZCL_STATUS_INCONSISTENT never used */
    /* 0x93 reserved, ZCL_STATUS_ACTION_DENIED deprecated. Use ZCL_STATUS_FAILURE */
    ZCL_STATUS_TIMEOUT = 0x94,
    ZCL_STATUS_ABORT = 0x95,
    ZCL_STATUS_INVALID_IMAGE = 0x96,
    ZCL_STATUS_WAIT_FOR_DATA = 0x97,
    ZCL_STATUS_NO_IMAGE_AVAILABLE = 0x98,
    ZCL_STATUS_REQUIRE_MORE_IMAGE = 0x99,
    ZCL_STATUS_NOTIFICATION_PENDING = 0x9A,
    /* 0xc0 reserved, ZCL_STATUS_HARDWARE_FAILURE deprecated. Use ZCL_STATUS_FAILURE */
    /* 0xc1 reserved, ZCL_STATUS_SOFTWARE_FAILURE deprecated. Use ZCL_STATUS_FAILURE */
    ZCL_STATUS_CALIBRATION_ERROR = 0xc2,
    ZCL_STATUS_UNSUPP_CLUSTER = 0xc3,
    /* 0xc4 reserved, ZCL_STATUS_LIMIT_REACHED deprecated. Use ZCL_STATUS_SUCCESS */

    /* Exegin add-ons */
    ZCL_STATUS_ALLOC_FAIL = 0xfe,
    /* Exegin add-on: Used with cluster command callback to indicate
     * command was handled successfully, but don't send a Default Response
     * for success. I.e. a response has already been sent. */
    ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE = 0xff
        /* zcl_attr_integer.c relies on these enum constants to be =< 0xff since they're cast as uint8_t */
};

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
    ZCL_DEVICE_ONOFF_BALLAST = 0x0108,
    ZCL_DEVICE_DIMMABLE_BALLAST = 0x0109,
    ZCL_DEVICE_ONOFF_PLUGIN_UNIT = 0x010a,
    ZCL_DEVICE_DIMMABLE_PLUGIN_UNIT = 0x010b,
    ZCL_DEVICE_COLOR_TEMPERATURE_LIGHT = 0x010c,
    ZCL_DEVICE_EXTENDED_COLOR_LIGHT = 0x010d,
    ZCL_DEVICE_LIGHT_LEVEL_SENSOR = 0x010e,

    /* Information (0x0120) */
    ZCL_DEVICE_ZIGBEE_ACCESS_POINT = 0x0120,
    ZCL_DEVICE_ZIGBEE_INFORMATION_NODE = 0x0121,
    ZCL_DEVICE_ZIGBEE_INFORMATION_TERMINAL = 0x0122,

    /* Closures (0x0200) */
    ZCL_DEVICE_SHADE = 0x0200,
    ZCL_DEVICE_SHADE_CONTROLLER = 0x0201,
    ZCL_DEVICE_WINDOW_COVERING_DEVICE = 0x0202,
    ZCL_DEVICE_WINDOW_COVERING_CONTROLLER = 0x0203,
    ZCL_DEVICE_BARRIER_DEVICE = 0x0204,
    ZCL_DEVICE_BARRIER_DEVICE_CONTROLLER = 0x0205,

    /* Payments (0x0220) */
    ZCL_DEVICE_POINT_OF_SALE = 0x0220,
    ZCL_DEVICE_TICKETING_MACHINE = 0x0221,
    ZCL_DEVICE_PAY_CONTROLLER = 0x0222,
    ZCL_DEVICE_BILLING_UNIT = 0x0223,
    ZCL_DEVICE_CHARGING_UNIT = 0x0224,

    /* HVAC (0x0300) */
    ZCL_DEVICE_HEATING_COOLING_UNIT = 0x0300,
    ZCL_DEVICE_THERMOSTAT = 0x0301,
    ZCL_DEVICE_TEMPERATURE_SENSOR = 0x0302,
    ZCL_DEVICE_PUMP = 0x0303,
    ZCL_DEVICE_PUMP_CONTROLLER = 0x0304,
    ZCL_DEVICE_PRESSURE_SENSOR = 0x0305,
    ZCL_DEVICE_FLOW_SENSOR = 0x0306,
    ZCL_DEVICE_HUMIDITY_SENSOR = 0x0307,

    /* Cards (0x0320) */
    ZCL_DEVICE_FLASH_CARD = 0x0320,
    ZCL_DEVICE_PC_SMART_CARD_READER = 0x0321,

    /* IAS (0x0400) */
    ZCL_DEVICE_IAS_CONTROL_AND_INDICATING_EQUIPMENT = 0x0400,
    ZCL_DEVICE_IAS_ANCILLARY_CONTROL_EQUIPMENT = 0x0401,
    ZCL_DEVICE_IAS_ZONE = 0x0402,
    ZCL_DEVICE_IAS_WARNING_DEVICE = 0x0403,

    /* Security (0x0445) */
    ZCL_DEVICE_GLASS_BREAK_SENSOR = 0x0445,

    /* Smart Energy device identifiers (0x0500) */
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
    ZCL_DEVICE_ELECTRIC_VEHICLE_STATION_EQUIPMENT = 0x050a,

    /* Lighting and Occupancy Continued (0x0800) */
    ZCL_DEVICE_COLOR_CONTROLLER = 0x0800,
    ZCL_DEVICE_COLOR_SCENE_CONTROLLER = 0x0810,
    ZCL_DEVICE_NON_COLOR_CONTROLLER = 0x0820,
    ZCL_DEVICE_NON_COLOR_SCENE_CONTROLLER = 0x0830,
    ZCL_DEVICE_CONTROL_BRIDGE = 0x0840,
    ZCL_DEVICE_ONOFF_SENSOR = 0x0850, /* aka Door Sensor? */

    /* Multifunction (0x0f00) */
    ZCL_DEVICE_GENERIC_MULTIFUNCTION_HEALTHCARE_DEVICE = 0x0f00,

    /* Health and Fitness (0x1004) */
    ZCL_DEVICE_PULSE_OXIMETER = 0x1004,
    ZCL_DEVICE_ECG = 0x1006,
    ZCL_DEVICE_BLOOD_PRESSURE_MONITOR = 0x1007,
    ZCL_DEVICE_THERMOMETER = 0x1008,
    ZCL_DEVICE_WEIGHT_SCALE = 0x100f,
    ZCL_DEVICE_GLUCOSE_METER = 0x1011,
    ZCL_DEVICE_INTERNATIONAL_NORMALIZED_RATIO = 0x1012,
    ZCL_DEVICE_INSULIN_PUMP = 0x1013,
    ZCL_DEVICE_PEAK_FLOW_MONITOR = 0x1015,
    ZCL_DEVICE_CARDIOVASCULAR_FITNESS_AND_ACTIVITY_MONITOR = 0x1029,
    ZCL_DEVICE_STRENGTH_FITNESS_EQUIPMENT = 0x102a,
    ZCL_DEVICE_PHYSICAL_ACTIVITY_MONITOR = 0x102b,
    ZCL_DEVICE_INDEPENDENT_LIVING_ACTIVITY_HUB = 0x1047,
    ZCL_DEVICE_ADHERENCE_MONITOR = 0x1048,
    ZCL_DEVICE_STEP_COUNTER = 0x1068,
    ZCL_DEVICE_FALL_SENSOR = 0x1075,
    ZCL_DEVICE_PERS_SENSOR = 0x1076,
    ZCL_DEVICE_SMOKE_SENSOR = 0x1077,
    ZCL_DEVICE_CO_SENSOR = 0x1078,
    ZCL_DEVICE_WATER_SENSOR = 0x1079,
    ZCL_DEVICE_GAS_SENSOR = 0x107a,
    ZCL_DEVICE_MOTION_SENSOR = 0x107b,
    ZCL_DEVICE_PROPERTY_EXIT_SENSOR = 0x107c,
    ZCL_DEVICE_ENURESIS_SENSOR = 0x107d,
    ZCL_DEVICE_CONTACT_CLOSURE_SENSOR = 0x107e,
    ZCL_DEVICE_USAGE_SENSOR = 0x107f,
    ZCL_DEVICE_SWITCH_USE_SENSOR = 0x1080,
    ZCL_DEVICE_DOSAGE_SENSOR = 0x1081,
    ZCL_DEVICE_BODY_TEMPERATURE_SENSOR = 0x1082

        /* ZCL_DEVICE_GREEN_POWER_DEVICES = 0x20xx */
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
    ZCL_CLUSTER_ZDD = 0x003d,

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

    ZCL_CLUSTER_METER_ID = 0x0b01,
    ZCL_CLUSTER_DIAGNOSTICS = 0x0b05,

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

#endif
