/**
 * @file zigbee.h
 * @heading Zigbee Utilities
 * @brief Zigbee header file.
 * @author Exegin Technologies
 * @copyright Copyright [2009 - 2023] Exegin Technologies Limited. All rights reserved.
 *
 * This file groups global/external definitions from all the layer specific header files
 * e.g, aps, nwk, zdo etc... into a single place, so that one can just include zigbee.h for
 * all the global definitions eliminating the header file inclusion clutter from source files.
 */

#ifndef ZIGBEE_H
# define ZIGBEE_H

#if defined(__GNUC__)
# define ZB_WARN_UNUSED __attribute__((warn_unused_result))
#elif defined(_MSC_VER) && (_MSC_VER >= 1800)
# define ZB_WARN_UNUSED
#else
# define ZB_WARN_UNUSED
#endif

#ifdef __CDT_PARSER__
/* Eclipse indexer only code */
#include "zb_make_config.h"
#endif

/* stdarg is required for the logging (ZbSetLogging) */
/*lint -save -e829 "'stdarg.h' usage should be deprecated [MISRA Rule 17.1 (REQUIRED)]" */
/*lint -save -e451 "header file included withoutstandard guard [Dir 4.10 (REQUIRED)]" */
#include <stdarg.h>
/*lint -restore */
/*lint -restore */

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h> /* NULL */
#include "pletoh.h" /* Little endian conversion */
#include "llist.h" /* Link list */
#include "ieee802154_enums.h"

/* Zigbee instance */
struct ZigBeeT;

/* MAC instance */
struct WpanPublicT;

/* ZigBee Protocol Versions as related to the spec version. */
#if 0 /* deprecated */
#define ZB_PROTOCOL_VERSION_2004        0x0001U
#endif
#define ZB_PROTOCOL_VERSION_2007        0x0002U

/* Channel Mask / Page Helpers */
#define ZB_CHANNELMASK_GETPAGE(x)       (uint8_t)(((x) >> WPAN_PAGE_CHANNELS_MAX) & 0x1FU)
#define ZB_CHANNELMASK(mask, page)      ((page != 0U) ? \
                                         ((mask) | ((((uint32_t)page) & 0x1FU) << WPAN_PAGE_CHANNELS_MAX)) : (mask))

/* A value of 0xffff means the device is not associated. */
#define ZB_PANID_MAX                    0xfffeU

#define ZB_EPID_MIN                     0x0000000000000001ULL
#define ZB_EPID_MAX                     0xfffffffffffffffeULL

/* The ZigBee distributed trust center address. */
#define ZB_DISTRIBUTED_TC_ADDR          0xffffffffffffffffULL

/* A subset of WPAN_CHANNELMASK_2400MHZ (HA and SE preferred channels) */
#define ZB_CHANNELMASK_2400MHZ_HA       0x0318C800U /* Channels 11, 14, 15, 19, 20, 24, 25 */

/* Define a constant maximum allocation size for both zigbee.mem.heap.c and
 * zigbee.mem.pool.c so both have the same behaviour. The memory pool has a max
 * allocation size of 2048 minus some overhead (should be >= 2016 bytes).
 * NOTE: The max allocation is not the total size of the heap, only the
 * largest chunk of data we can allocate from it at a time.
 * If we define both implementation limits to be 2000, we should be safe. */
#define ZB_HEAP_MAX_ALLOC                   2000U

/* ZigBee Status Codes */
enum ZbStatusCodeT {
    /* General Status Codes */
    ZB_STATUS_SUCCESS = 0x00,
    ZB_STATUS_ALLOC_FAIL = 0x70, /* Memory allocation failure. */

    /* ZDP Status Codes */
    ZB_ZDP_STATUS_SUCCESS = ZB_STATUS_SUCCESS,
    ZB_ZDP_STATUS_INV_REQTYPE = 0x80,
    ZB_ZDP_STATUS_DEVNOTFOUND = 0x81,
    ZB_ZDP_STATUS_INVALID_EP = 0x82,
    ZB_ZDP_STATUS_NOT_ACTIVE = 0x83,
    ZB_ZDP_STATUS_NOT_SUPPORTED = 0x84,
    ZB_ZDP_STATUS_TIMEOUT = 0x85,
    ZB_ZDP_STATUS_NO_MATCH = 0x86,
    ZB_ZDP_STATUS_NO_ENTRY = 0x88,
    ZB_ZDP_STATUS_NO_DESCRIPTOR = 0x89,
    ZB_ZDP_STATUS_INSUFFICIENT_SPACE = 0x8a,
    ZB_ZDP_STATUS_NOT_PERMITTED = 0x8b,
    ZB_ZDP_STATUS_TABLE_FULL = 0x8c,
    ZB_ZDP_STATUS_NOT_AUTHORIZED = 0x8d,
    ZB_ZDP_STATUS_DEVICE_BINDING_TABLE_FULL = 0x8e,
    ZB_ZDP_STATUS_INVALID_INDEX = 0x8f,

    /* APS Status Codes. */
    ZB_APS_STATUS_SUCCESS = ZB_STATUS_SUCCESS,
    ZB_APS_STATUS_ASDU_TOO_LONG = 0xa0,
    ZB_APS_STATUS_DEFRAG_DEFERRED = 0xa1,
    ZB_APS_STATUS_DEFRAG_UNSUPPORTED = 0xa2,
    ZB_APS_STATUS_ILLEGAL_REQUEST = 0xa3,
    ZB_APS_STATUS_INVALID_BINDING = 0xa4,
    ZB_APS_STATUS_INVALID_GROUP = 0xa5,
    ZB_APS_STATUS_INVALID_PARAMETER = 0xa6,
    ZB_APS_STATUS_NO_ACK = 0xa7,
    ZB_APS_STATUS_NO_BOUND_DEVICE = 0xa8,
    ZB_APS_STATUS_NO_SHORT_ADDRESS = 0xa9,
    ZB_APS_STATUS_NOT_SUPPORTED = 0xaa,
    ZB_APS_STATUS_SECURED_LINK_KEY = 0xab,
    ZB_APS_STATUS_SECURED_NWK_KEY = 0xac,
    ZB_APS_STATUS_SECURITY_FAIL = 0xad,
    ZB_APS_STATUS_TABLE_FULL = 0xae,
    ZB_APS_STATUS_UNSECURED = 0xaf,
    ZB_APS_STATUS_UNSUPPORTED_ATTRIBUTE = 0xb0,
    /* These values used in case of internal errors. */
    ZB_APS_STATUS_INVALID_INDEX = 0xbd,

    /* NWK Status Codes. */
    ZB_NWK_STATUS_SUCCESS = ZB_STATUS_SUCCESS,
    ZB_NWK_STATUS_INVALID_PARAMETER = 0xc1,
    ZB_NWK_STATUS_INVALID_REQUEST = 0xc2,
    ZB_NWK_STATUS_NOT_PERMITTED = 0xc3,
    ZB_NWK_STATUS_STARTUP_FAILURE = 0xc4,
    ZB_NWK_STATUS_ALREADY_PRESENT = 0xc5,
    ZB_NWK_STATUS_SYNC_FAILURE = 0xc6,
    ZB_NWK_STATUS_TABLE_FULL = 0xc7,
    ZB_NWK_STATUS_UNKNOWN_DEVICE = 0xc8,
    ZB_NWK_STATUS_UNSUPPORTED_ATTRIBUTE = 0xc9,
    ZB_NWK_STATUS_NO_NETWORKS = 0xca,
    ZB_NWK_STATUS_LEAVE_UNCONFIRMED = 0xcb,
    ZB_NWK_STATUS_MAX_FRM_CNTR = 0xcc,
    ZB_NWK_STATUS_NO_KEY = 0xcd,
    ZB_NWK_STATUS_BAD_CCM_OUTPUT = 0xce,
    ZB_NWK_STATUS_NO_ROUTING_CAPACITY = 0xcf,
    ZB_NWK_STATUS_ROUTE_DISCOVERY_FAILED = 0xd0,
    ZB_NWK_STATUS_ROUTE_ERROR = 0xd1,
    ZB_NWK_STATUS_BT_TABLE_FULL = 0xd2,
    ZB_NWK_STATUS_FRAME_NOT_BUFFERED = 0xd3,
    /* Exegin Custom Status Values. */
    ZB_NWK_STATUS_INVALID_INDEX = 0xd4,
    ZB_NWK_STATUS_INTERNAL_ERR = 0xd6,

    /* WPAN Status Codes (copied from mcp_enums.h to here for MISRA) */
    ZB_WPAN_STATUS_SUCCESS = ZB_STATUS_SUCCESS,
    ZB_WPAN_STATUS_COUNTER_ERROR = mcp_STATUS_COUNTER_ERROR,
    ZB_WPAN_STATUS_IMPROPER_KEY_TYPE = mcp_STATUS_IMPROPER_KEY_TYPE,
    ZB_WPAN_STATUS_IMPROPER_SECURITY_LEVEL = mcp_STATUS_IMPROPER_SECURITY_LEVEL,
    ZB_WPAN_STATUS_UNSUPPORTED_LEGACY = mcp_STATUS_UNSUPPORTED_LEGACY,
    ZB_WPAN_STATUS_UNSUPPORTED_SECURITY = mcp_STATUS_UNSUPPORTED_SECURITY,
    ZB_WPAN_STATUS_BEACON_LOSS = mcp_STATUS_BEACON_LOSS,
    ZB_WPAN_STATUS_CHANNEL_ACCESS_FAILURE = mcp_STATUS_CHANNEL_ACCESS_FAILURE,
    ZB_WPAN_STATUS_DENIED = mcp_STATUS_DENIED,
    ZB_WPAN_STATUS_DISABLE_TRX_FAILURE = mcp_STATUS_DISABLE_TRX_FAILURE,
    ZB_WPAN_STATUS_SECURITY_ERROR = mcp_STATUS_SECURITY_ERROR,
    ZB_WPAN_STATUS_FRAME_TOO_LONG = mcp_STATUS_FRAME_TOO_LONG,
    ZB_WPAN_STATUS_INVALID_GTS = mcp_STATUS_INVALID_GTS,
    ZB_WPAN_STATUS_INVALID_HANDLE = mcp_STATUS_INVALID_HANDLE,
    ZB_WPAN_STATUS_INVALID_PARAMETER = mcp_STATUS_INVALID_PARAMETER,
    ZB_WPAN_STATUS_NO_ACK = mcp_STATUS_NO_ACK,
    ZB_WPAN_STATUS_NO_BEACON = mcp_STATUS_NO_BEACON,
    ZB_WPAN_STATUS_NO_DATA = mcp_STATUS_NO_DATA,
    ZB_WPAN_STATUS_NO_SHORT_ADDRESS = mcp_STATUS_NO_SHORT_ADDRESS,
    ZB_WPAN_STATUS_OUT_OF_CAP = mcp_STATUS_OUT_OF_CAP,
    ZB_WPAN_STATUS_PAN_ID_CONFLICT = mcp_STATUS_PAN_ID_CONFLICT,
    ZB_WPAN_STATUS_REALIGNMENT = mcp_STATUS_REALIGNMENT,
    ZB_WPAN_STATUS_TRANSACTION_EXPIRED = mcp_STATUS_TRANSACTION_EXPIRED,
    ZB_WPAN_STATUS_TRANSACTION_OVERFLOW = mcp_STATUS_TRANSACTION_OVERFLOW,
    ZB_WPAN_STATUS_TX_ACTIVE = mcp_STATUS_TX_ACTIVE,
    ZB_WPAN_STATUS_UNAVAILABLE_KEY = mcp_STATUS_UNAVAILABLE_KEY,
    ZB_WPAN_STATUS_UNSUPPORTED_ATTRIBUTE = mcp_STATUS_UNSUPPORTED_ATTRIBUTE,
    ZB_WPAN_STATUS_INVALID_ADDRESS = mcp_STATUS_INVALID_ADDRESS,
    ZB_WPAN_STATUS_ON_TIME_TOO_LONG = mcp_STATUS_ON_TIME_TOO_LONG,
    ZB_WPAN_STATUS_PAST_TIME = mcp_STATUS_PAST_TIME,
    ZB_WPAN_STATUS_TRACKING_OFF = mcp_STATUS_TRACKING_OFF,
    ZB_WPAN_STATUS_INVALID_INDEX = mcp_STATUS_INVALID_INDEX,
    ZB_WPAN_STATUS_LIMIT_REACHED = mcp_STATUS_LIMIT_REACHED,
    ZB_WPAN_STATUS_READ_ONLY = mcp_STATUS_READ_ONLY,
    ZB_WPAN_STATUS_SCAN_IN_PROGRESS = mcp_STATUS_SCAN_IN_PROGRESS,
    ZB_WPAN_STATUS_SUPERFRAME_OVERLAP = mcp_STATUS_SUPERFRAME_OVERLAP,
    ZB_WPAN_STATUS_DRIVER_ERROR = mcp_STATUS_DRIVER_ERROR,
    ZB_WPAN_STATUS_DEVICE_ERROR = mcp_STATUS_DEVICE_ERROR
};

/* Trust Center Swap Out status codes */
enum ZbTcsoStatusT {
    ZB_TCSO_STATUS_SUCCESS = 0x00, /* Successfully performed TCSO with new TC */
    ZB_TCSO_STATUS_DISCOVERY_UNDERWAY, /* ZCL Keepalive has initiated TCSO, because of three consecutive failures. */
    ZB_TCSO_STATUS_REJOIN_PREV, /* Found previous TC and rejoined to it. */
    ZB_TCSO_STATUS_NOT_FOUND, /* Didn't find new or old TC, or process was aborted, resumed previous operation. */
    ZB_TCSO_STATUS_FATAL /* TCSO failed and unable to restore previous operation. */
};

/*---------------------------------------------------------------
 * Stack Logging
 *---------------------------------------------------------------
 */
/* Debugging log mask. */
#define ZB_LOG_MASK_FATAL               0x00000001U /* Unrecoverable errors. */
#define ZB_LOG_MASK_ERROR               0x00000002U /* Recoverable internal errors. */
#define ZB_LOG_MASK_WARNING             0x00000004U /* Misused API calls, network problems, etc... */
#define ZB_LOG_MASK_INFO                0x00000008U /* Basic debugging info. Less verbose than ZB_LOG_MASK_DEBUG. */
#define ZB_LOG_MASK_DEBUG               0x00000010U /* General debug info */
#define ZB_LOG_MASK_PERSIST             0x00000020U /* Persistence */
/* NWK */
#define ZB_LOG_MASK_NWK_ROUTING         0x00000040U /* Network routing details. */
#define ZB_LOG_MASK_NWK_LQI             0x00000080U /* Network link status and lqi updates. */
#define ZB_LOG_MASK_NWK_SECURITY        0x00000100U /* Network security. */
#define ZB_LOG_MASK_NWK_ADDR_MAP        0x00000200U /* Network address map changes */
/* APS */
#define ZB_LOG_MASK_APS_SEND            0x00000400U /* APS packet transmission */
#define ZB_LOG_MASK_APS_FRAG            0x00000800U /* APS fragmentation debugging */
/* ZDO */
#define ZB_LOG_MASK_ZDO_ANNCE           0x00001000U /* Print on reception of ZDO Device_Annce */
#define ZB_LOG_MASK_ZCL                 0x00002000U
#define ZB_LOG_MASK_GREENPOWER          0x00004000U
#define ZB_LOG_MASK_DIAG                0x00008000U
#define ZB_LOG_MASK_HEAP                0x00010000U
#define ZB_LOG_MASK_TIMER               0x00020000U
#define ZB_LOG_MASK_SLEEPY              0x01000000U /* Sleepy (e.g. Polling) */
#define ZB_LOG_MASK_MAC_RSSI            0x10000000U /* Print debug message per MCPS-DATA.indication showing RSSI */

/* Log mask helpers */
#define ZB_LOG_MASK_LEVEL_0             0x00000000U
#define ZB_LOG_MASK_LEVEL_1             (ZB_LOG_MASK_FATAL)
#define ZB_LOG_MASK_LEVEL_2             (ZB_LOG_MASK_LEVEL_1 | ZB_LOG_MASK_ERROR)
#define ZB_LOG_MASK_LEVEL_3             (ZB_LOG_MASK_LEVEL_2 | ZB_LOG_MASK_WARNING)
#define ZB_LOG_MASK_LEVEL_4             (ZB_LOG_MASK_LEVEL_3 | ZB_LOG_MASK_INFO)
#define ZB_LOG_MASK_LEVEL_5             (ZB_LOG_MASK_LEVEL_4 | ZB_LOG_MASK_DEBUG)
#define ZB_LOG_MASK_LEVEL_6             (ZB_LOG_MASK_LEVEL_5 | ZB_LOG_MASK_ZCL)
#define ZB_LOG_MASK_LEVEL_ALL           0xFFFFFFFFU

/* Specifies the level of logging to use, and a callback that outputs the log information. */
void ZbSetLogging(struct ZigBeeT *zb, uint32_t mask,
    void (*func)(struct ZigBeeT *zb, uint32_t mask, const char *hdr, const char *fmt, va_list argptr));

void ZbGetLogging(struct ZigBeeT *zb, uint32_t *mask,
    void(**func)(struct ZigBeeT *zb, uint32_t mask, const char *hdr, const char *fmt, va_list argptr));

/*---------------------------------------------------------------
 * Exegin Manufacturing ID
 *---------------------------------------------------------------
 */
/* The Exegin manufacturer ID. */
#define ZB_MFG_CODE_EXEGIN              0x10D7U
#define ZB_MFG_CODE_WILDCARD            0xFFFFU

/*---------------------------------------------------------------
 * Channel List Structure
 *---------------------------------------------------------------
 */
/* One 2.4 GHz mask + one North American mask + five EU masks + four GB-868 masks. */
#define MAX_CHANNEL_LIST_ENTRIES        11U

struct ZbChannelListT {
    /* Number of channel masks in 'list' */
    uint8_t count;
    struct {
        /* 802.15.4 Channel Page */
        uint8_t page;
        /* e.g. WPAN_CHANNELMASK_2400MHZ or ZB_CHANNELMASK_2400MHZ_HA for Page 0. */
        uint32_t channelMask;
    } list[MAX_CHANNEL_LIST_ENTRIES];
};

/*---------------------------------------------------------------
 * Stack Initialization
 *---------------------------------------------------------------
 */
/* Opaque stack structures. */
struct ZigBeeT;
struct ZbMsgFilterT;

typedef unsigned long ZbUptimeT;
unsigned int ZbTimeoutRemaining(ZbUptimeT now, ZbUptimeT expire_time);

/* A pointer to this struct type is passed to ZbInit to define the various
 * ZigBee tables used in the stack. If the pointer to ZbInit is NULL, the
 * default sizes are used. */
struct ZbInitTblSizesT {
    void *heapPtr;
    /**< Pointer to memory to use for stack's run-time heap. Size equals heapSz bytes.
     * If heapSz is zero, set this to NULL and the buffer will be allocated using ZbMalloc().
     * If the stack is built with CONFIG_ZB_ALLOC_STATIC, then heapPtr and heapSz must be non-zero
     * and provided by the application.
     */
    unsigned int heapSz;
    /**< Size of memory buffer provided by heapPtr.
     * If heapSz is zero, then a default value of 32 kB for FFD or 16 kB for RFD is chosen,
     * and memory will be allocated through ZbMalloc().
     * An FFD vs RFD type of stack is controlled by the CONFIG_ZB_ENDNODE when the stack is built. */

    /* NWK Table Sizes */
    unsigned int nwkNeighborTblSz;
    /**< Network Neighbor Table (NNT) number of entries.
     * Default is 64. */
    unsigned int nwkRouteTblSz;
    /**<
     * Default is 32. */
    unsigned int nwkAddrMapTblSz;
    /**<
     * Default is 32. */
    unsigned int nwkBttSz;
    /**<
     * Default is 32. */
    unsigned int nwkRReqSz;
    /**<
     * Default is 16. */

    /* APS Table Sizes */
    unsigned int apsPeerLinkKeyTblSz; /* Default: 32 */
};

/* Same parameters as ZbSetLogging takes. Allows debug log output
 * as stack is being initialized. */
struct ZbInitSetLoggingT {
    uint32_t mask; /**< e.g. ZB_LOG_MASK_LEVEL_2 */
    void (*func)(struct ZigBeeT *zb, uint32_t mask, const char *hdr,
        const char *fmt, va_list argptr);
    /**< Callback function to print log messages. */
};

/**
 * This is called to create a new Zigbee stack instance. One of the parameters provided
 * is an optional pointer to a struct that defines the sizes of some of the tables
 * within the stack (e.g. NNT, Routing Table, Security Keys Table). This lets the
 * application developer tailor the memory usage depending on the type of device
 * being developed.
 * @param extAddr EUI to be assigned to this Zigbee stack instance.
 * @param tblSizes Optional pointer to a 'struct ZbInitTblSizesT' data structure defining
 * some of the table sizes to be allocated.
 * @param setLogging Optional pointer to a 'struct ZbInitSetLoggingT' data structure
 * defining the log mask and callback to call to print log messages.
 * @return Pointer to Zigbee stack instance, or NULL if there was a problem
 * (e.g. not enough memory)
 */
struct ZigBeeT * ZbInit(uint64_t extAddr, struct ZbInitTblSizesT *tblSizes, struct ZbInitSetLoggingT *setLogging);

/* Deallocates a Zigbee stack instance. */
void ZbDestroy(struct ZigBeeT *zb);
enum ZbStatusCodeT ZbDestroyWithCb(struct ZigBeeT *zb, void (*callback)(void *arg), void *arg);

/* Help seed the stack's PRNG. If the data has real entropy, set the has_entropy flag to true. */
void ZbSeedRand(struct ZigBeeT *zb, uint8_t *randBuf, unsigned int len, bool has_entropy);

/* Called periodically to run the stack. */
void ZbTimerWork(struct ZigBeeT *zb);

/* Returns the length of time (in milliseconds) until the next scheduled timer will elapse,
 * or UINT_MAX if there are no scheduled timers. */
unsigned int ZbCheckTime(struct ZigBeeT *zb);

/* Configure a callback to wakeup the application if there's a new stack event to
 *      process. Not all stack ports require this. */
void ZbWakeupCallbackConfig(struct ZigBeeT *zb, void (*wakeup_cb)(void));

/* Called to get the file descriptor to be used to wake-up the stack thread that is
 * calling ZbTimerWork if something in the stack needs attention. This is only required
 * in multi-threaded environments. Without this event, it is possible for a user thread
 * to initiate a stack function which* doesn't tickle the MAC layer, which in turn would
 * wake up the stack thread. */
int ZbPortStackEventFd(struct ZigBeeT *zb);

void ZbChangeExtAddr(struct ZigBeeT *zb, uint64_t extAddr);

struct ZbNlmeLeaveConfT;
enum ZbStatusCodeT ZB_WARN_UNUSED ZbLeaveReq(struct ZigBeeT *zb,
    void (*callback)(struct ZbNlmeLeaveConfT *conf, void *arg), void *cbarg);

/* Helper function to perform an APS and NWK reset */
void ZbReset(struct ZigBeeT *zb);

/* Attaches an IEEE 802.15.4 device driver to the ZigBee stack. Uses the link pointers
 * within the device structure for linking. */
bool ZbIfAttach(struct ZigBeeT *zb, struct WpanPublicT *dev);

/* Detaches an IEEE 802.15.4 device driver from the ZigBee stack. */
void ZbIfDetach(struct ZigBeeT *zb, struct WpanPublicT *dev);

/*---------------------------------------------------------------
 * ZCL Basic Server API
 *---------------------------------------------------------------
 */
/* Basic Cluster maximum string lengths. */
#define ZCL_BASIC_MANUFACTURER_NAME_LENGTH       32U
#define ZCL_BASIC_MODEL_IDENTIFIER_LENGTH        32U
#define ZCL_BASIC_DATE_CODE_LENGTH               16U
#define ZCL_BASIC_LOCATION_DESC_LENGTH           16U
#define ZCL_BASIC_SW_BUILD_ID_LENGTH             16U

/**
 * This data structure is used to configure the default attribute values
 * for the Basic Server. All values are in ZCL data format (i.e. strings
 * are prefixed with the length byte)
 */
struct ZbZclBasicServerDefaults {
    uint8_t app_version; /**< ZCL_BASIC_ATTR_APP_VERSION */
    uint8_t stack_version; /**< ZCL_BASIC_ATTR_STACK_VERSION */
    uint8_t hw_version; /**< ZCL_BASIC_ATTR_HARDWARE_VERSION */
    uint8_t mfr_name[ZCL_BASIC_MANUFACTURER_NAME_LENGTH + 1U]; /**< ZCL_BASIC_ATTR_MFR_NAME (First byte length) */
    uint8_t model_name[ZCL_BASIC_MODEL_IDENTIFIER_LENGTH + 1U]; /**< ZCL_BASIC_ATTR_MODEL_NAME (First byte length) */
    uint8_t date_code[ZCL_BASIC_DATE_CODE_LENGTH + 1U]; /**< ZCL_BASIC_ATTR_DATE_CODE (First byte length) */
    uint8_t power_source; /**< ZCL_BASIC_ATTR_POWER_SOURCE (e.g. ZCL_BASIC_POWER_UNKNOWN) */
    uint8_t sw_build_id[ZCL_BASIC_SW_BUILD_ID_LENGTH + 1U]; /**< ZCL_BASIC_ATTR_SW_BUILD_ID (First byte length) */
};

/**
 * Configure the default ZCL Basic Server attribute values. The Basic Server
 * is integral to the stack in order for the attribute values to be made "global"
 * and shared between all Basic Server instances on all endpoints.
 * This should be called after calling ZbInit and before creating any ZCL endpoints.
 * @param zb Zigbee instance
 * @param defaults Pointer to the default configuration data structure
 * @return None
 */
void ZbZclBasicServerConfigDefaults(struct ZigBeeT *zb, const struct ZbZclBasicServerDefaults *defaults);

/* Controls whether the Basic Server is allowed to process the ZCL_BASIC_RESET_FACTORY command. */
void ZbZclBasicServerResetCmdConfig(struct ZigBeeT *zb, bool allow_reset);

/* Write to the local attributes (e.g. ZCL_BASIC_ATTR_MFR_NAME) */
/**
 * Write to a Basic Server attribute
 * @param zb Zigbee instance
 * @param endpoint Specify the endpoint of the Basic Server, or use ZB_ENDPOINT_BCAST to find and use
 * the first endpoint that contains a Basic Server. Since the attributes are shared between all
 * instances of the Basic Server, it doesn't matter which endpoint is chosen.
 * @param attributeId The attribute Id to modify
 * @param ptr Pointer to the attribute data, in the ZCL defined format.
 * @param len Maximum length of the attribute data. May exceed the length of the particular attribute.
 * @return ZCL Status Code
 */
enum ZclStatusCodeT ZbZclBasicWriteDirect(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t attributeId, const uint8_t *ptr, unsigned int len);

/* Post an alarm code to the Basic Cluster */
bool ZbZclBasicPostAlarm(struct ZigBeeT *zb, uint8_t endpoint, uint8_t alarm_code);

/*---------------------------------------------------------------
 * ZigBee Timer
 *---------------------------------------------------------------
 */
/* Opaque timer structure. */
struct ZbTimerT;

/* Creates a ZigBee timer structure. */
struct ZbTimerT * ZbTimerAlloc(struct ZigBeeT *zb, void (*callback)(struct ZigBeeT *zb, void *cb_arg), void *arg);
void ZbTimerChangeCallback(struct ZbTimerT *timer, void (*callback)(struct ZigBeeT *zb, void *cb_arg), void *arg);
void ZbTimerStop(struct ZbTimerT *timer);
void ZbTimerFree(struct ZbTimerT *timer);
/* Resets and schedules a ZigBee timer. */
void ZbTimerReset(struct ZbTimerT *timer, unsigned int timeout);
bool ZbTimerRunning(struct ZbTimerT *timer);
/* ZbTimerRemaining returns time remaining in mS for the given timer. */
unsigned int ZbTimerRemaining(struct ZbTimerT *timer);

/*---------------------------------------------------------------
 * Asynchronous messaging filter API
 *---------------------------------------------------------------
 */
/* Asynchronous message filter mask. */
/* NWK Indications */
#define ZB_MSG_FILTER_JOIN_IND                  0x00000001U /* NLME-JOIN.indication (struct ZbNlmeJoinIndT) */
#define ZB_MSG_FILTER_LEAVE_IND                 0x00000002U /* NLME-LEAVE.indication (struct ZbNlmeLeaveIndT) */
#define ZB_MSG_FILTER_STATUS_IND                0x00000004U /* NLME-NETWORK-STATUS.indication (struct ZbNlmeNetworkStatusIndT) */
/* APS Indications */
#define ZB_MSG_FILTER_TRANSPORT_KEY_IND         0x00000008U /* APSME-TRANSPORT-KEY.indication (struct ZbApsmeTransKeyIndT) */
#define ZB_MSG_FILTER_UPDATE_DEVICE_IND         0x00000010U /* APSME-UPDATE-DEVICE.indication (struct ZbApsmeUpdateDeviceIndT) */
#define ZB_MSG_FILTER_REMOVE_DEVICE_IND         0x00000020U /* APSME-REMOVE-DEVICE.indication (struct ZbApsmeRemoveDeviceIndT) */
#define ZB_MSG_FILTER_REQUEST_KEY_IND           0x00000040U /* APSME-REQUEST-KEY.indication (struct ZbApsmeRequestKeyIndT) */
#define ZB_MSG_FILTER_SWITCH_KEY_IND            0x00000080U /* APSME-SWITCH-KEY.indication (struct ZbApsmeSwitchKeyIndT) */
#define ZB_MSG_FILTER_VERIFY_KEY_IND            0x00000100U /* APSME-VERIFY-KEY.indication (struct ZbApsmeVerifyKeyIndT) */
#define ZB_MSG_FILTER_CONFIRM_KEY_IND           0x00000200U /* APSME-CONFIRM-KEY.indication (struct ZbApsmeConfirmKeyIndT) */
/* Data Indications */
#define ZB_MSG_FILTER_MCPS_DATA_IND             0x00000400U /* MCPS-DATA.indication (struct wpan_data_ind) */
#define ZB_MSG_FILTER_NLDE_DATA_IND             0x00000800U /* NLDE-DATA.indication (struct ZbNldeDataIndT) */
#define ZB_MSG_FILTER_APSDE_DATA_IND            0x00001000U /* APSDE-DATA.indication (struct ZbApsdeDataIndT) */
/* Startup Indications */
#define ZB_MSG_FILTER_STARTUP_IND               0x00002000U /* (struct ZbMsgStartupInd) */
/* Reset to Factory Defaults (e.g. Basic Server ZCL_BASIC_RESET_FACTORY command) */
#define ZB_MSG_FILTER_FACTORY_RESET             0x00004000U
#define ZB_MSG_FILTER_RESET_REPORTS             0x00008000U
/* Note, max filter bit we can specify here is  0x00080000U */

/* Groups of messages that are filterable. */
#define ZB_MSG_FILTER_NLME \
    (ZB_MSG_FILTER_JOIN_IND | ZB_MSG_FILTER_LEAVE_IND | ZB_MSG_FILTER_STATUS_IND)

#define ZB_MSG_FILTER_APSME \
    (ZB_MSG_FILTER_TRANSPORT_KEY_IND | ZB_MSG_FILTER_UPDATE_DEVICE_IND | ZB_MSG_FILTER_REMOVE_DEVICE_IND | \
     ZB_MSG_FILTER_REQUEST_KEY_IND | ZB_MSG_FILTER_SWITCH_KEY_IND | ZB_MSG_FILTER_VERIFY_KEY_IND | \
     ZB_MSG_FILTER_CONFIRM_KEY_IND)

/* Message filter priorities (255 = highest, 0 = lowest) */
#define ZB_MSG_INTERNAL_PRIO                    128 /* default stack priority */
#define ZB_MSG_DEFAULT_PRIO                     64 /* default application priority */

/* Message filter return values. */
enum zb_msg_filter_rc {
    ZB_MSG_CONTINUE = 0, /* Continue processing any further filter callbacks. */
    ZB_MSG_DISCARD /* Stop processing further filter callbacks. */
};

struct ZbApsFilterT;

struct ZbMsgFilterT * ZbMsgFilterRegister(struct ZigBeeT *zb, uint32_t mask, uint8_t prio,
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg), void *arg);

void ZbMsgFilterRemove(struct ZigBeeT *zb, struct ZbMsgFilterT *filter);

struct ZbMsgStartupInd {
    enum ZbStatusCodeT status;
};

/*---------------------------------------------------------
 * Persistence
 *---------------------------------------------------------
 */
/* Configure the persistence callback that tells the application when important
 * stack parameters have changed and should be saved. */
bool ZbPersistNotifyRegister(struct ZigBeeT *zb, void (*callback)(struct ZigBeeT *zb, void *cbarg), void *cbarg);

/* Get the stack persistence data and write it to the buffer provided by buf.
 * If buf is NULL and maxlen is zero, this function determines the buffer size
 * required to save the persistence data.
 * Use ZbStartupPersist to restore the persistence data to the stack and restart
 * the zigbee stack. */
unsigned int ZbPersistGet(struct ZigBeeT *zb, uint8_t *buf, unsigned int maxlen);

/* Same as ZbPersistGet, but saves additional more dynamic data such as routing tables.
 * Use the same ZbStartupPersist to restore the persistence data to the stack and
 * to restart the zigbee stack. */
unsigned int ZbStateGet(struct ZigBeeT *zb, uint8_t *buf, unsigned int maxlen);

/*---------------------------------------------------------
 * Shutdown
 *---------------------------------------------------------
 */
/* This API moves the stack to shutdown mode, used in case of a sleepy end device to conserve power. */
void ZbShutdown(struct ZigBeeT *zb);

/* The following APIs are to pause the stack and allow another service to use the
 * MAC layer. Upon resume, the MAC interface(s) are reconfigured to work with the
 * Zigbee stack again. */
enum ZbStatusCodeT ZbStatePause(struct ZigBeeT *zb, void (*callback)(void *arg), void *arg);
enum ZbStatusCodeT ZbStateResume(struct ZigBeeT *zb);

/*---------------------------------------------------------------
 * Test Case Hooks
 *---------------------------------------------------------------
 */
#ifdef CONFIG_TEST_HOOK
/* These represent bits in a 32-bit bitmask. */
enum ZbTestcaseT {
    ZB_TESTCASE_NONE = 0,
    ZB_TESTCASE_SE1X_15_47, /* Server sends a truncated INITIATE_KEY response */
    ZB_TESTCASE_SE1X_15_48, /* Client sends a truncated EPHEMERAL_DATA request */
    ZB_TESTCASE_CBKE_DELAY_EPH_DATA, /* e.g. SE 1.4 test case 15.25 */
    ZB_TESTCASE_CBKE_DELAY_RESPONSE, /* e.g. SE 1.4 test case 15.26 */
    ZB_TESTCASE_NWK_ENH_BEACON_REQ_06, /* Bogus EBR */
    /* Override tx power management in the MAC and always set a minimum
     * power level. Useful for co-ax testing to prevent cross-talk. */
    ZB_TESTCASE_NWK_ENH_BEACON_MIN_TX_POWER,
    ZB_TESTCASE_NWK_ENH_BEACON_REQ_DROP,
    ZB_TESTCASE_NWK_STD_BEACON_REQ_DROP,
    ZB_TESTCASE_NWK_LINKPOWER_DROP_NOTIFY,
    ZB_TESTCASE_NWK_LINKPOWER_DROP_REQUEST,
    ZB_TESTCASE_NWK_REJOIN_DROP_RSP,
    ZB_TESTCASE_NWK_REJOIN_RETURN_FULL,
    ZB_TESTCASE_NWK_EDKA_DROP_REQUEST,
    ZB_TESTCASE_NWK_ASSOC_RSP_FULL,
    ZB_TESTCASE_NWK_ROUTE_THRU_NNT_IGNORE_COST, /*  */
    ZB_TESTCASE_NWK_FC_MAX_ALLOW,
    ZB_TESTCASE_DLK_NOT_ALLOWED,
    ZB_TESTCASE_DLK_INVALID_KEY_NEGO_STATE,
    ZB_TESTCASE_DLK_INVALID_TLV,
    ZB_TESTCASE_DLK_MISSING_TLV,
    ZB_TESTCASE_DLK_NO_TLV,
    ZB_TESTCASE_DLK_OUT_OF_ORDER_TLVS,
    ZB_TESTCASE_APS_REQUEST_KEY_DROP,
    ZB_TESTCASE_APS_DISABLE_FC_SYNC,
    ZB_TESTCASE_ZDO_SEC_CHLNG_WITH_EXTRA_TLVS,
    ZB_TESTCASE_TOUCHLINK_DEBUG_KEY,
    ZB_TESTCASE_ZED_STACK_SHUTDOWN
};

/* External API */
void ZbTestCaseEnable(struct ZigBeeT *zb, enum ZbTestcaseT testcase);
void ZbTestCaseDisable(struct ZigBeeT *zb, enum ZbTestcaseT testcase);
void ZbTestCaseClear(struct ZigBeeT *zb);
uint32_t ZbTestCaseCurrent(struct ZigBeeT *zb);

/* Should only be required for the stack */
bool ZbTestCaseIsEnabled(struct ZigBeeT *zb, enum ZbTestcaseT testcase);
#endif // CONFIG_TEST_HOOK
/*---------------------------------------------------------------
 * Misc. Helper Functions
 *---------------------------------------------------------------
 */
uint64_t ZbExtendedAddress(struct ZigBeeT *zb);
uint16_t ZbShortAddress(struct ZigBeeT *zb);

/* Total memory allocated from the heap. */
unsigned int ZbMallocTotalSz(void);

/* Memory allocated from the internal ZigBee heap. */
unsigned long ZbHeapUsed(struct ZigBeeT *zb);
/* Returns the amount of memory available in the Zigbee heap. May be zero if unknown. */
unsigned long ZbHeapAvailable(struct ZigBeeT *zb);

unsigned long ZbHeapHighWaterMark(struct ZigBeeT *zb);

/* String conversion */
int zb_hex_str_to_bin(const char *string, void *out, unsigned int maxlen);
unsigned int zb_hex_bin_to_str(const uint8_t *in_data, unsigned int in_len, char *out_str, unsigned int max_len,
    const char delimiter, unsigned int interval);

/*---------------------------------------------------------------
 * Misc Debug (may not be available on all platforms)
 *---------------------------------------------------------------
 */
void ZbDebugMemory(struct ZigBeeT *zb);
void ZbDebugInfo(struct ZigBeeT *zb);

/*---------------------------------------------------------------
 * Additional Layer Includes
 *---------------------------------------------------------------
 */
#include "zigbee.security.h"
#include "zigbee.hash.h"
#include "zigbee.bdb.h"
#include "zigbee.aps.h"
#include "zigbee.nwk.h"
#include "zigbee.startup.h"
#include "zigbee.zdo.h"

#endif
