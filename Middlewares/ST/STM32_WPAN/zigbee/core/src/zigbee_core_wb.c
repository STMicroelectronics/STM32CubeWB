/**
 ******************************************************************************
 * @file    zigbee_core_wb.c
 * @author  MCD Application Team
 * @brief   Contains STM32WB specificities requested to control the zigbee stack
 *          from the M4 core.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <limits.h> /* ULONG_MAX */
#include <assert.h>

#include "zigbee_types.h"
#include "zigbee_interface.h" /* zigbee.h, etc */
#include "zcl/zcl.h"
#include "zcl/key/zcl.key.h" /* ZbZclKeWithDevice */
#include "zcl/general/zcl.diagnostics.h"
#include "zcl/zcl.touchlink.h"
#include "ieee802154_crc.h"

#ifdef ZIGBEE_DIRECT_ACTIVATED
/* SHA-256 */
#include "sha.h"
#endif /* ZIGBEE_DIRECT_ACTIVATED */

#include "zigbee_core.h"
#include "tl_zigbee_hci.h"

#include "dbg_trace.h"
#include "stm_logging.h"

/* Lock isn't required on this platform */
#define ZbEnterCritical(_zb_)
#define ZbExitCritical(_zb_)

#ifndef ZB_HEAP_MAX_ALLOC
#define ZB_HEAP_MAX_ALLOC                   2000U
#endif

/* Protyptes (move to header file? */
void zb_ipc_m4_stack_logging_config(bool enable);
unsigned int ZbHeapMaxAlloc(void);
bool zb_ipc_get_secured_mem_info(uint32_t *unsec_sram2a_sz, uint32_t *unsec_sram2b_sz);
unsigned int zb_malloc_current_sz(void);
bool ZbZclDeviceLogCheckAllow(struct ZigBeeT *zb, struct ZbApsdeDataIndT *dataIndPtr, struct ZbZclHeaderT *zclHdrPtr);

#ifdef ZIGBEE_DIRECT_ACTIVATED
extern void ZbCcmTransform(const void *key, const void *nonce, uint32_t nonce_len, void *m, uint32_t m_len, void *mic);
extern void ZbCcmAuthenticate(const void *key, const void *nonce, uint32_t nonce_len, const void *a, uint32_t a_len,
    const void *m, uint32_t m_len, void *mic, uint32_t mic_len);
int ec_mul_x25519(const void *private, const void *base, void *result);
int ec_mul_p256(const void *priv, const void *pub_x, const void *pub_y, void *result_x, void *result_y);
enum sha_status_t SHA256Reset(SHA256Context *context);
enum sha_status_t SHA256Input(SHA256Context *context, const uint8_t *bytes, unsigned int bytecount);
enum sha_status_t SHA256Result(SHA256Context *context, uint8_t Message_Digest[SHA256HashSize]);
int hmacReset(HMACContext *context, enum SHAversion whichSha, const unsigned char *key, int key_len);
int hmacInput(HMACContext *context, const unsigned char *text, int text_len);
int hmacResult(HMACContext *context, uint8_t digest[USHAMaxHashSize]);
#endif /* ZIGBEE_DIRECT_ACTIVATED */
/* Touchlink callbacks */
static struct ZbTouchlinkCallbacks zigbee_m4_tl_callbacks;

void * zb_heap_alloc(struct ZigBeeT *zb, size_t sz, const char *funcname, unsigned int linenum);
void zb_heap_free(struct ZigBeeT *zb, void *ptr, const char *funcname, unsigned int linenum);
#if defined(CONFIG_ZB_MEMORY_DEBUG) || defined(CONFIG_ZB_MEMPOOL_HISTORY)
# define ZbHeapAlloc(_zb_, _sz_)             zb_heap_alloc(_zb_, _sz_, __func__, __LINE__)
# define ZbHeapFree(_zb_, _ptr_)             zb_heap_free(_zb_, _ptr_, __func__, __LINE__)
#else
# define ZbHeapAlloc(_zb_, _sz_)             zb_heap_alloc(_zb_, _sz_, "", 0)
# define ZbHeapFree(_zb_, _ptr_)             zb_heap_free(_zb_, _ptr_, "", 0)
#endif

/* Memory debug --------------------------------------------------------------*/
/* to do - make configurable from Makefile */
/* #define CONFIG_ZB_M4_MALLOC_DEBUG_SZ            512U */

#ifdef CONFIG_ZB_M4_MALLOC_DEBUG_SZ
PACKED_STRUCT zb_malloc_tracking_t {
    void *ptr;
    unsigned int sz;
};
#endif

static void * zb_malloc_track(void *ptr, unsigned int sz);
static void * zb_malloc_untrack(void *ptr);

/* API Wrapper Helpers -------------------------------------------------------*/
#define IPC_REQ_FUNC(name, cmd_id, req_type) \
    void name(struct ZigBeeT *zb, req_type *r) \
    { \
        Zigbee_Cmd_Request_t *ipcc_req; \
    \
        Pre_ZigbeeCmdProcessing(); \
        ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer(); \
        ipcc_req->ID = cmd_id; \
        ipcc_req->Size = 1; \
        ipcc_req->Data[0] = (uint32_t)r; \
        ZIGBEE_CmdTransfer(); \
        Post_ZigbeeCmdProcessing(); \
    }

#define IPC_REQ_CONF_FUNC(name, cmd_id, req_type, conf_type) \
    void name(struct ZigBeeT *zb, req_type *r, conf_type *c) \
    { \
        Zigbee_Cmd_Request_t *ipcc_req; \
    \
        Pre_ZigbeeCmdProcessing(); \
        ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer(); \
        ipcc_req->ID = cmd_id; \
        ipcc_req->Size = 2; \
        ipcc_req->Data[0] = (uint32_t)r; \
        ipcc_req->Data[1] = (uint32_t)c; \
        ZIGBEE_CmdTransfer(); \
        Post_ZigbeeCmdProcessing(); \
    }

#define IPC_REQ_CALLBACK_FUNC(name, cmd_id, req_type, conf_type) \
    enum ZbStatusCodeT \
    name(struct ZigBeeT *zb, req_type *r, void (*callback)(conf_type *conf, void *cbarg), void *cbarg) \
    { \
        Zigbee_Cmd_Request_t *ipcc_req; \
        struct zb_ipc_m4_cb_info_t *info; \
        enum ZbStatusCodeT status; \
    \
        info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg); \
        if (info == NULL) { \
            return ZB_STATUS_ALLOC_FAIL; \
        } \
    \
        Pre_ZigbeeCmdProcessing(); \
        ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer(); \
        ipcc_req->ID = cmd_id; \
        ipcc_req->Size = 2; \
        ipcc_req->Data[0] = (uint32_t)r; \
        ipcc_req->Data[1] = (uint32_t)info; \
        ZIGBEE_CmdTransfer(); \
        status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval(); \
        Post_ZigbeeCmdProcessing(); \
        if (status != ZB_STATUS_SUCCESS) { \
            zb_ipc_m4_cb_info_free(info); \
        } \
        return status; \
    }

#define IPC_CLUSTER_REQ_CALLBACK_FUNC(name, cmd_id, req_type, rsp_type) \
    enum ZclStatusCodeT \
    name(struct ZbZclClusterT *clusterPtr, req_type *r, \
    void (*callback)(const rsp_type *rsp, void *cbarg), void *cbarg) \
    { \
        Zigbee_Cmd_Request_t *ipcc_req; \
        struct zb_ipc_m4_cb_info_t *info; \
        enum ZclStatusCodeT status; \
    \
        info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg); \
        if (info == NULL) { \
            return ZCL_STATUS_INSUFFICIENT_SPACE; \
        } \
    \
        Pre_ZigbeeCmdProcessing(); \
        ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer(); \
        ipcc_req->ID = cmd_id; \
        ipcc_req->Size = 3; \
        ipcc_req->Data[0] = (uint32_t)clusterPtr; \
        ipcc_req->Data[1] = (uint32_t)r; \
        ipcc_req->Data[2] = (uint32_t)info; \
        ZIGBEE_CmdTransfer(); \
        status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval(); \
        Post_ZigbeeCmdProcessing(); \
        if (status != ZCL_STATUS_SUCCESS) { \
            zb_ipc_m4_cb_info_free(info); \
        } \
        return status; \
    }

/* ZbUptimeT is an "unsigned long" */
#define ZB_UPTIME_MAX                       (ULONG_MAX)

/* If a value is greater than the high mark, and a second value is lower
 * than the low mark, then the second value is deemed to have rolled-over
 * when comparing the two values. */
#define TIMER_ROLL_OVER_HIGH                ((ZB_UPTIME_MAX / 2UL) + (ZB_UPTIME_MAX / 4UL))
#define TIMER_ROLL_OVER_LOW                 ((ZB_UPTIME_MAX / 2UL) - (ZB_UPTIME_MAX / 4UL))

/* Null (all zeroes)
 * 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00 */
const uint8_t sec_key_null[ZB_SEC_KEYSIZE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* "ZigBeeAlliance09"
 * 5a:69:67:42:65:65:41:6c:6c:69:61:6e:63:65:30:39 */
const uint8_t sec_key_ha[ZB_SEC_KEYSIZE] = {
    0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c, 0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x30, 0x39
};

/* Uncertified Device's Distributed Link Key
 * d0:d1:d2:d3:d4:d5:d6:d7:d8:d9:da:db:dc:dd:de:df */
const uint8_t sec_key_distrib_uncert[ZB_SEC_KEYSIZE] = {
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf
};

/* TOUCHLINK_KEY_INDEX_CERTIFICATION key
 * c0:c1:c2:c3:c4:c5:c6:c7:c8:c9:ca:cb:cc:cd:ce:cf */
const uint8_t sec_key_touchlink_cert[ZB_SEC_KEYSIZE] = {
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf
};

struct aps_filter_cb_t {
    struct ZbApsFilterT *filter;
    int (*callback)(struct ZbApsdeDataIndT *data_ind, void *cb_arg);
    void *cb_arg;
};

/* Helper struct to send to APS Binding */
static const struct ZbApsAddrT sendToBinding = {
    ZB_APSDE_ADDRMODE_NOTPRESENT,
    0xff,
    0xffff,
    0xffff,
    0,
};
const struct ZbApsAddrT *ZbApsAddrBinding = &sendToBinding;

struct zb_ipc_m4_cb_info_t {
    void *callback;
    void *arg;
    bool zcl_recv_multi_rsp;
};

static const va_list va_null;

/* Single static callback for persistent data notifications */
static void (*zb_persist_cb)(struct ZigBeeT *zb, void *cbarg) = NULL;
static void *zb_persist_arg = NULL;

/* Callback for ZbZdoMatchDescMulti. It is static and never freed. It can be overwritten
 * by another call to ZbZdoMatchDescMulti. */
static void (*zdo_match_multi_cb)(struct ZbZdoMatchDescRspT *ind, void *cb_arg) = NULL;
static void *zdo_match_multi_arg;

/* Some globals */
PACKED_STRUCT zb_ipc_globals_t {
    struct ZigBeeT *zb;
    /* Stack logging callback */
    void (*log_cb)(struct ZigBeeT *zb, uint32_t mask, const char *hdr,
        const char *fmt, va_list argptr);
    uint32_t log_mask;
    bool log_enable;
#ifdef CONFIG_ZB_M4_MALLOC_DEBUG_SZ
    struct zb_malloc_tracking_t zb_malloc_info[CONFIG_ZB_M4_MALLOC_DEBUG_SZ];
#else
    unsigned int zb_alloc_sz;
#endif
};
static struct zb_ipc_globals_t zb_ipc_globals;

int zcl_cluster_data_ind(struct ZbApsdeDataIndT *dataIndPtr, void *arg);
int zcl_cluster_alarm_data_ind(struct ZbApsdeDataIndT *data_ind, void *arg);

/* ST: Don't use built-in memcpy. "Unfortunately when full size optimization is enabled on
 * M4 side, IAR maps memcpy to aeaby_memcpy4 instead of aeabi_memcpy which allows
 * unaligned memcpy." */
static void
zb_ipc_m4_memcpy2(void *dst, void *src, unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++) {
        ((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
    }
}

#if 0 /* not used */
struct ZigBeeT *
GetZigbeePtr(void)
{
    return zb_ipc_globals.zb;
}

#endif

void
zb_ipc_m4_stack_logging_config(bool enable)
{
    uint32_t mask = 0U;
    void (*func)(struct ZigBeeT *zb, uint32_t mask, const char *hdr, const char *fmt, va_list argptr) = NULL;

    zb_ipc_globals.log_enable = enable;

    /* Now reconfigure the logging on the M0. If log_func == NULL, then we disable the M0
     * sending log messages over the IPCC. Otherwise, it's enabled. We first need to get
     * the current log mask in-use, before re-asserting it back to the M0. */
    ZbGetLogging(zb_ipc_globals.zb, &mask, &func);
    ZbSetLogging(zb_ipc_globals.zb, mask, func);
}

static struct zb_ipc_m4_cb_info_t *
zb_ipc_m4_cb_info_alloc(void *callback, void *arg)
{
    struct zb_ipc_m4_cb_info_t *info;

    info = malloc(sizeof(struct zb_ipc_m4_cb_info_t));
    if (info != NULL) {
        memset(info, 0, sizeof(struct zb_ipc_m4_cb_info_t));
        info->callback = callback;
        info->arg = arg;
    }
    return info;
}

static void
zb_ipc_m4_cb_info_free(struct zb_ipc_m4_cb_info_t *info)
{
    free(info);
}

static uint32_t
zb_ipc_m4_get_retval(void)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;

    ipcc_req = ZIGBEE_Get_OTCmdRspPayloadBuffer();
    assert(ipcc_req->Size == 1);
    zb_ipc_m4_memcpy2(&retval, (void *)&ipcc_req->Data[0], 4);
    return retval;
}

unsigned int
ZbHeapMaxAlloc(void)
{
    return ZB_HEAP_MAX_ALLOC;
}

struct ZigBeeT *
ZbInit(uint64_t extAddr, struct ZbInitTblSizesT *tblSizes, struct ZbInitSetLoggingT *setLogging)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    if (zb_ipc_globals.zb != NULL) {
        return NULL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_INIT;
    ipcc_req->Size = 4;
    zb_ipc_m4_memcpy2((void *)&ipcc_req->Data[0], &extAddr, 8);
    ipcc_req->Data[2] = (uint32_t)tblSizes;
    ipcc_req->Data[3] = (uint32_t)setLogging;
    ZIGBEE_CmdTransfer();
    zb_ipc_globals.zb = (struct ZigBeeT *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (setLogging != NULL) {
        /* Save the log mask */
        zb_ipc_globals.log_cb = setLogging->func;
        zb_ipc_globals.log_mask = setLogging->mask;
    }
    return zb_ipc_globals.zb;
}

void
ZbDestroy(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    if (zb_ipc_globals.zb == NULL) {
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_DESTROY;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    zb_ipc_globals.zb = NULL;
}

enum ZbStatusCodeT
ZbDestroyWithCb(struct ZigBeeT *zb, void (*callback)(void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;

    if (zb_ipc_globals.zb == NULL) {
        return ZB_NWK_STATUS_INVALID_REQUEST;
    }
    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_DESTROY_WITH_CB;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    return ZB_STATUS_SUCCESS;
    /* Followed up in MSG_M0TOM4_ZB_DESTROY_CB */
}

void
ZbSetLogging(struct ZigBeeT *zb, uint32_t mask,
    void (*func)(struct ZigBeeT *zb, uint32_t mask, const char *hdr, const char *fmt, va_list argptr))
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_LOG_CONFIG;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = mask;
    /* Ignore the 'func' argument. Only use zb_ipc_globals.log_enable to determine if we want
     * log messages from the M4. If zb_ipc_globals.log_func != NULL, M0 sends log messages
     * over IPCC. */
    ipcc_req->Data[1] = (zb_ipc_globals.log_enable || (func != NULL)) ? 1U : 0U;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    /* Save the log mask */
    zb_ipc_globals.log_cb = func;
    zb_ipc_globals.log_mask = mask;
}

void
ZbGetLogging(struct ZigBeeT *zb, uint32_t *mask,
    void(**func)(struct ZigBeeT *zb, uint32_t mask, const char *hdr, const char *fmt, va_list argptr))
{
    if (mask != NULL) {
        *mask = zb_ipc_globals.log_mask;
    }
    if (func != NULL) {
        *func = zb_ipc_globals.log_cb;
    }
}

uint64_t
ZbExtendedAddress(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint64_t ext_addr = 0U;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_EXTADDR_GET;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    ipcc_req = ZIGBEE_Get_OTCmdRspPayloadBuffer();
    assert(ipcc_req->Size == 2);
    zb_ipc_m4_memcpy2(&ext_addr, &ipcc_req->Data, 8);
    Post_ZigbeeCmdProcessing();
    return ext_addr;
}

uint16_t
ZbShortAddress(struct ZigBeeT *zb)
{
    uint16_t nwkAddr = 0U;

    (void)ZbNwkGet(zb, ZB_NWK_NIB_ID_NetworkAddress, &nwkAddr, sizeof(nwkAddr));
    return nwkAddr;
}

void
ZbChangeExtAddr(struct ZigBeeT *zb, uint64_t extAddr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_EXTADDR_CHANGE;
    ipcc_req->Size = 2;
    zb_ipc_m4_memcpy2((void *)&ipcc_req->Data[0], &extAddr, 8);
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbStartupConfigGetProDefaults(struct ZbStartupT *configPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_GET_CFG;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)configPtr;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbStartupConfigGetProSeDefaults(struct ZbStartupT *configPtr)
{
    ZbStartupConfigGetProDefaults(configPtr);
    /* Remove the default preconfigured link key by clearing it. */
    (void)memset(configPtr->security.preconfiguredLinkKey, 0, ZB_SEC_KEYSIZE);
    /* Remove the distributed link key by clearing it. */
    (void)memset(configPtr->security.distributedGlobalKey, 0, ZB_SEC_KEYSIZE);
}

enum ZbStatusCodeT
ZbStartup(struct ZigBeeT *zb, struct ZbStartupT *configPtr,
    void (*callback)(enum ZbStatusCodeT status, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZbStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }

    /* Save the touchlink callbacks */
    /* I.e. MSG_M0TOM4_ZCL_TL_EP_INFO_CB */
    memcpy(&zigbee_m4_tl_callbacks, &configPtr->touchlink.callbacks, sizeof(struct ZbTouchlinkCallbacks));

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)configPtr;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* If success, followed up in MSG_M0TOM4_STARTUP_CB handler */
}

enum ZbStatusCodeT
ZbStartupRejoin(struct ZigBeeT *zb, void (*callback)(struct ZbNlmeJoinConfT *conf, void *arg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZbStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_REJOIN;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* If ZB_STATUS_SUCECSS, followed up in MSG_M0TOM4_STARTUP_REJOIN_CB handler */
}

enum ZbStatusCodeT
ZbStartupPersist(struct ZigBeeT *zb, const void *pdata, unsigned int plen,
    struct ZbStartupCbkeT *cbke_config,
    void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZbStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_PERSIST;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)pdata;
    ipcc_req->Data[1] = (uint32_t)plen;
    ipcc_req->Data[2] = (uint32_t)cbke_config;
    ipcc_req->Data[3] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* If ZB_STATUS_SUCECSS, followed up in MSG_M0TOM4_STARTUP_PERSIST_CB handler */
}

enum ZbStatusCodeT
ZbStartupFindBindStart(struct ZigBeeT *zb, void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZbStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_FINDBIND;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_STARTUP_FINDBIND_CB handler */
}

enum ZbStatusCodeT
ZbStartupTouchlinkTargetStop(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_TOUCHLINK_TARGET_STOP;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();

    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;
}

bool
ZbStartupTcsoStart(struct ZigBeeT *zb, void (*callback)(enum ZbTcsoStatusT status, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    bool rc;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return false;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_TCSO_START;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    if (!rc) {
        zb_ipc_m4_cb_info_free(info);
    }
    return rc;
    /* Followed up in MSG_M0TOM4_STARTUP_TCSO_CB */
}

bool
ZbStartupTcsoAbort(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_TCSO_ABORT;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

enum ZbStatusCodeT
ZbTrustCenterRejoin(struct ZigBeeT *zb, void (*callback)(enum ZbStatusCodeT status, void *arg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZbStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_TC_REJOIN;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_STARTUP_TC_REJOIN_CB handler */
}

bool
ZbPersistNotifyRegister(struct ZigBeeT *zb, void (*callback)(struct ZigBeeT *zb, void *cbarg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_PERSIST_ENABLE;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    if (rc) {
        zb_persist_arg = cbarg;
        zb_persist_cb = callback;
    }
    else {
        zb_persist_arg = NULL;
        zb_persist_cb = NULL;
    }
    return rc;
    /* Followed up by M0 calls to MSG_M0TOM4_PERSIST_CB */
}

unsigned int
ZbPersistGet(struct ZigBeeT *zb, uint8_t *buf, unsigned int maxlen)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    unsigned int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_PERSIST_GET;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)buf;
    ipcc_req->Data[1] = (uint32_t)maxlen;
    ZIGBEE_CmdTransfer();
    rc = (unsigned int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

unsigned int
ZbStateGet(struct ZigBeeT *zb, uint8_t *buf, unsigned int maxlen)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    unsigned int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STATE_GET;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)buf;
    ipcc_req->Data[1] = (uint32_t)maxlen;
    ZIGBEE_CmdTransfer();
    rc = (unsigned int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

enum ZbStatusCodeT
ZbLeaveReq(struct ZigBeeT *zb, void (*callback)(struct ZbNlmeLeaveConfT *conf, void *arg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZbStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_LEAVE_REQ;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* If success, followed up in MSG_M0TOM4_ZB_LEAVE_CB handler */
}

void
ZbReset(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_RESET_REQ;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

/******************************************************************************
 * Shutdown, Pause, Resume
 ******************************************************************************
 */

void
ZbShutdown(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_SHUTDOWN;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

enum ZbStatusCodeT
ZbStatePause(struct ZigBeeT *zb, void (*callback)(void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZbStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_STATE_PAUSE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* If success, followed up in MSG_M0TOM4_ZB_STATE_PAUSE_CB handler */
}

enum ZbStatusCodeT
ZbStateResume(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_STATE_RESUME;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;
}

/******************************************************************************
 * ECDSA Signature
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbSecEcdsaValidate(struct ZigBeeT *zb, enum ZbSecEcdsaSigType sig_type,
    const uint8_t *ca_pub_key_array, unsigned int ca_pub_key_len,
    const uint8_t *certificate, const uint8_t *signature,
    const uint8_t *image_digest, const uint8_t *cert_digest)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_SEC_ECDSA_VALIDATE;
    ipcc_req->Size = 7;
    ipcc_req->Data[0] = (uint32_t)sig_type;
    ipcc_req->Data[1] = (uint32_t)ca_pub_key_array;
    ipcc_req->Data[2] = (uint32_t)ca_pub_key_len;
    ipcc_req->Data[3] = (uint32_t)certificate;
    ipcc_req->Data[4] = (uint32_t)signature;
    ipcc_req->Data[5] = (uint32_t)image_digest;
    ipcc_req->Data[6] = (uint32_t)cert_digest;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;
}

/******************************************************************************
 * Touchlink Utility Commands
 ******************************************************************************
 */
enum ZclStatusCodeT
ZbZclTouchlinkInitiatorGetGrpIdReq(struct ZigBeeT *zb, struct ZbTlGetGroupIdsReqCmd *cmd,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbTlGetGroupIdsRspCmd *rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZclStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZCL_STATUS_FAILURE;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_TL_GET_GRP_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)cmd;
    ipcc_req->Data[1] = (uint32_t)dst;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_TL_GET_GRP_CB handler */
}

enum ZclStatusCodeT
ZbZclTouchlinkInitiatorGetEpListReq(struct ZigBeeT *zb, struct ZbTlGetEpListReqCmd *cmd,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbTlGetEpListRspCmd *rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZclStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZCL_STATUS_FAILURE;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_TL_GET_EPLIST_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)cmd;
    ipcc_req->Data[1] = (uint32_t)dst;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_TL_GET_EPLIST_CB handler */
}

enum ZclStatusCodeT
ZbZclTouchlinkTargetSendEpInfoCmd(struct ZigBeeT *zb, uint8_t endpoint,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZclStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZCL_STATUS_FAILURE;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_TL_SEND_EPINFO_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)dst;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_TL_SEND_EPINFO_CB handler */
}

/******************************************************************************
 * BDB
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbBdbGetIndex(struct ZigBeeT *zb, enum ZbBdbAttrIdT attrId, void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbBdbGetReqT bdbGetReq;
    struct ZbBdbGetConfT bdbGetConf;
    enum ZbStatusCodeT status;

    /* Do the BDB-GET.request */
    bdbGetReq.attrId = attrId;
    bdbGetReq.attr = attrPtr;
    bdbGetReq.attrLength = attrSz;
    bdbGetReq.attrIndex = attrIndex;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_BDB_GET_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)&bdbGetReq;
    ipcc_req->Data[1] = (uint32_t)&bdbGetConf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    status = (enum ZbStatusCodeT)bdbGetConf.status;
    return status;
}

enum ZbStatusCodeT
ZbBdbSetIndex(struct ZigBeeT *zb, enum ZbBdbAttrIdT attrId, const void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbBdbSetReqT bdbSetReq;
    struct ZbBdbSetConfT bdbSetConf;
    enum ZbStatusCodeT status;

    /* Do the BDB-SET.request */
    bdbSetReq.attrId = attrId;
    bdbSetReq.attr = attrPtr;
    bdbSetReq.attrLength = attrSz;
    bdbSetReq.attrIndex = attrIndex;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_BDB_SET_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)&bdbSetReq;
    ipcc_req->Data[1] = (uint32_t)&bdbSetConf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    status = (enum ZbStatusCodeT)bdbSetConf.status;
    return status;
}

enum ZbBdbCommissioningStatusT
ZbBdbGetEndpointStatus(struct ZigBeeT *zb, uint8_t endpoint)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbBdbCommissioningStatusT rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_BDB_GET_EP_STATUS;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ZIGBEE_CmdTransfer();
    /* Get the status code */
    rc = (enum ZbBdbCommissioningStatusT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

/* Required for the Identify Server Cluster */
void
ZbBdbSetEndpointStatus(struct ZigBeeT *zb, enum ZbBdbCommissioningStatusT status, uint8_t endpoint)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_BDB_SET_EP_STATUS;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)status;
    ipcc_req->Data[1] = (uint32_t)endpoint;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

/******************************************************************************
 * APS
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbApsdeDataReqCallback(struct ZigBeeT *zb, struct ZbApsdeDataReqT *req,
    void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbStatusCodeT status;
    struct zb_ipc_m4_cb_info_t *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSDE_DATA_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    /* Get the status code */
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* If success, followed up in MSG_M0TOM4_APSDE_DATA_REQ_CB handler */
}

void
ZbApsmeAddEndpoint(struct ZigBeeT *zb, struct ZbApsmeAddEndpointReqT *r, struct ZbApsmeAddEndpointConfT *c)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_ENDPOINT_ADD;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)r;
    ipcc_req->Data[1] = (uint32_t)c;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

IPC_REQ_CONF_FUNC(ZbApsmeRemoveEndpoint, MSG_M4TOM0_APS_ENDPOINT_DEL,
    struct ZbApsmeRemoveEndpointReqT, struct ZbApsmeRemoveEndpointConfT)

struct ZbApsFilterT *
ZbApsFilterEndpointAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId,
    int (*callback)(struct ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb_t *aps_filter_cb;

    /* This will be freed by ZbApsFilterEndpointFree */
    aps_filter_cb = malloc(sizeof(struct aps_filter_cb_t));
    if (aps_filter_cb == NULL) {
        return NULL;
    }
    aps_filter_cb->callback = callback;
    aps_filter_cb->cb_arg = arg;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_FILTER_ENDPOINT_ADD;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)profileId;
    ipcc_req->Data[2] = (uint32_t)aps_filter_cb;
    ZIGBEE_CmdTransfer();
    aps_filter_cb->filter = (struct ZbApsFilterT *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (aps_filter_cb->filter == NULL) {
        free(aps_filter_cb);
        return NULL;
    }
    /* Callbacks go to MSG_M0TOM4_APS_FILTER_ENDPOINT_CB handler */

    /* Since "struct ZbApsFilterT *" is opaque to the application, we can return
     * a pointer to aps_filter_cb, which we can use to free later in
     * ZbApsFilterEndpointFree. */
    return (struct ZbApsFilterT *)aps_filter_cb;
}

struct ZbApsFilterT *
ZbApsFilterClusterAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t clusterId, uint16_t profileId,
    int (*callback)(struct ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb_t *aps_filter_cb;

    /* This will be freed by ZbApsFilterEndpointFree */
    aps_filter_cb = malloc(sizeof(struct aps_filter_cb_t));
    if (aps_filter_cb == NULL) {
        return NULL;
    }
    aps_filter_cb->callback = callback;
    aps_filter_cb->cb_arg = arg;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_FILTER_CLUSTER_ADD;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)clusterId;
    ipcc_req->Data[2] = (uint32_t)profileId;
    ipcc_req->Data[3] = (uint32_t)aps_filter_cb;
    ZIGBEE_CmdTransfer();
    aps_filter_cb->filter = (struct ZbApsFilterT *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (aps_filter_cb->filter == NULL) {
        free(aps_filter_cb);
        return NULL;
    }
    /* Callbacks go to MSG_M0TOM4_APS_FILTER_CLUSTER_CB handler */

    /* Since "struct ZbApsFilterT *" is opaque to the application, we can return
     * a pointer to aps_filter_cb, which we can use to free later in
     * ZbApsFilterEndpointFree. */
    return (struct ZbApsFilterT *)aps_filter_cb;
}

void
ZbApsFilterEndpointFree(struct ZigBeeT *zb, struct ZbApsFilterT *filter)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb_t *aps_filter_cb;

    /* filter is actually a pointer to a "struct aps_filter_cb_t" */
    aps_filter_cb = (struct aps_filter_cb_t *)filter;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_FILTER_REMOVE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)aps_filter_cb->filter;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    /* memset(aps_filter_cb, 0, sizeof(struct aps_filter_cb_t)); */
    free(aps_filter_cb);
}

bool
ZbApsmeEndpointClusterListAppend(struct ZigBeeT *zb, uint8_t endpoint, uint16_t cluster_id, bool is_input)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_EP_CLUSTER_ID_ADD;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)cluster_id;
    ipcc_req->Data[2] = (uint32_t)is_input;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbApsEndpointExists(struct ZigBeeT *zb, uint8_t endpoint)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_EP_EXISTS;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

uint16_t
ZbApsEndpointProfile(struct ZigBeeT *zb, uint8_t endpoint)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint16_t val_ret;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_EP_GET_PROFILE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ZIGBEE_CmdTransfer();
    val_ret = (uint16_t)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return val_ret;
}

bool
ZbApsAddrIsBcast(const struct ZbApsAddrT *addr)
{
    /* Check the destination of the original request */
    if (addr->mode == ZB_APSDE_ADDRMODE_GROUP) {
        return true;
    }
    if ((addr->mode == ZB_APSDE_ADDRMODE_SHORT) && (ZbNwkAddrIsBcast(addr->nwkAddr))) {
        return true;
    }
    return false;
}

bool
ZbApsAddrIsLocal(struct ZigBeeT *zb, const struct ZbApsAddrT *addr)
{
    if (addr->mode == ZB_APSDE_ADDRMODE_EXT) {
        if (addr->extAddr == ZbExtendedAddress(zb)) {
            return true;
        }
    }
    if (addr->mode == ZB_APSDE_ADDRMODE_SHORT) {
        if (addr->nwkAddr == ZbShortAddress(zb)) {
            /* Note, if our address is ZB_NWK_ADDR_UNDEFINED, then this message could only
             * have been generated locally. */
            return true;
        }
    }
    return false;
}

/** APSME-GET.request */
struct ZbApsmeGetReqT {
    enum ZbApsmeIbAttrIdT attrId; /**< AIBAttribute */
    void *attr; /**< Pointer to attribute */
    unsigned int attrLength; /**< AIBAttributeLength  */
    unsigned int attrIndex; /** Index of the attribute */
};

/** APSME-GET.confirm */
struct ZbApsmeGetConfT {
    enum ZbStatusCodeT status; /**< Status */
    enum ZbApsmeIbAttrIdT attrId; /**< AIBAttribute */
};

enum ZbStatusCodeT
ZbApsGetIndex(struct ZigBeeT *zb, enum ZbApsmeIbAttrIdT attrId, void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbApsmeGetReqT apsmeGetReq;
    struct ZbApsmeGetConfT apsmeGetConf;

    /* Do the APSME-GET.request */
    apsmeGetReq.attrId = attrId;
    apsmeGetReq.attr = attrPtr;
    apsmeGetReq.attrLength = attrSz;
    apsmeGetReq.attrIndex = attrIndex;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_GET_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)&apsmeGetReq;
    ipcc_req->Data[1] = (uint32_t)&apsmeGetConf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    return apsmeGetConf.status;
}

/** APSME-SET.request */
struct ZbApsmeSetReqT {
    enum ZbApsmeIbAttrIdT attrId; /**< AIBAttribute */
    const void *attr; /**< Pointer to attribute */
    unsigned int attrLength; /**< AIBAttributeLength */
    unsigned int attrIndex; /**< AIBAttributeValue */
};

/** APSME-SET.confirm */
struct ZbApsmeSetConfT {
    enum ZbStatusCodeT status; /**< Status */
    enum ZbApsmeIbAttrIdT attrId; /**< AIBAttribute */
};

enum ZbStatusCodeT
ZbApsSetIndex(struct ZigBeeT *zb, enum ZbApsmeIbAttrIdT attrId, const void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbApsmeSetReqT apsmeSetReq;
    struct ZbApsmeSetConfT apsmeSetConf;

    /* Do the APSME-SET.request */
    apsmeSetReq.attrId = attrId;
    apsmeSetReq.attr = attrPtr;
    apsmeSetReq.attrLength = attrSz;
    apsmeSetReq.attrIndex = attrIndex;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_SET_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)&apsmeSetReq;
    ipcc_req->Data[1] = (uint32_t)&apsmeSetConf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    return apsmeSetConf.status;
}

/* This API is a helper, which just accesses ZbApsmeGetReq to look through the
 * ZB_APS_IB_ID_DEVICE_KEY_PAIR_SET to find the matching device-key pair. */
struct ZbApsmeKeyPairT *
ZbApsLookupKey(struct ZigBeeT *zb, struct ZbApsmeKeyPairT *key, uint64_t addr, unsigned int *idx)
{
    unsigned int i;

    if (addr == 0U) {
        return NULL;
    }
    for (i = 0;; i++) {
        if (ZbApsGetIndex(zb, ZB_APS_IB_ID_DEVICE_KEY_PAIR_SET, key,
                sizeof(struct ZbApsmeKeyPairT), i) != ZB_STATUS_SUCCESS) {
            (void)memset(key, 0, sizeof(struct ZbApsmeKeyPairT));
            return NULL;
        }
        if (key->deviceAddress == addr) {
            break;
        }
    }
    if (idx != NULL) {
        *idx = i;
    }
    return key;
}

IPC_REQ_CONF_FUNC(ZbApsmeAddGroupReq, MSG_M4TOM0_APSME_ADD_GROUP,
    struct ZbApsmeAddGroupReqT, struct ZbApsmeAddGroupConfT)
IPC_REQ_CONF_FUNC(ZbApsmeRemoveGroupReq, MSG_M4TOM0_APSME_REMOVE_GROUP,
    struct ZbApsmeRemoveGroupReqT, struct ZbApsmeRemoveGroupConfT)
IPC_REQ_CONF_FUNC(ZbApsmeRemoveAllGroupsReq, MSG_M4TOM0_APSME_REMOVE_ALL_GROUPS,
    struct ZbApsmeRemoveAllGroupsReqT, struct ZbApsmeRemoveAllGroupsConfT)

bool
ZbApsGroupIsMember(struct ZigBeeT *zb, uint16_t groupAddr, uint8_t endpoint)
{
    struct ZbApsmeGroupT group;
    unsigned int i;
    enum ZbStatusCodeT status;

    ZbEnterCritical(zb);
    for (i = 0;; i++) {
        status = ZbApsGetIndex(zb, ZB_APS_IB_ID_GROUP_TABLE, &group, sizeof(group), i);
        if (status != ZB_APS_STATUS_SUCCESS) {
            break;
        }
        /* Check for valid entries. */
        if (group.endpoint == ZB_ENDPOINT_BCAST) {
            continue;
        }
        /* Check if we're a member of this group. */
        if (group.groupAddr != groupAddr) {
            continue;
        }
        if ((endpoint != ZB_ENDPOINT_BCAST) && (group.endpoint != endpoint)) {
            continue;
        }
        /* We are a member. */
        ZbExitCritical(zb);
        return true;
    }
    ZbExitCritical(zb);
    return false;
}

/* Helper to iterate through the Groups table using the APSME-GET interface */
uint8_t
ZbApsGroupsGetCapacity(struct ZigBeeT *zb)
{
    struct ZbApsmeGroupT group;
    unsigned int i, num_found = 0;
    enum ZbStatusCodeT status;

    ZbEnterCritical(zb);
    for (i = 0;; i++) {
        status = ZbApsGetIndex(zb, ZB_APS_IB_ID_GROUP_TABLE, &group, sizeof(group), i);
        if (status != ZB_APS_STATUS_SUCCESS) {
            break;
        }
        if (group.endpoint == ZB_ENDPOINT_BCAST) {
            continue;
        }
        num_found++;
    }
    ZbExitCritical(zb);
    return (uint8_t)(i - num_found);
}

/* Helper to iterate through the Groups table using the APSME-GET interface */
uint8_t
ZbApsGroupsGetMembership(struct ZigBeeT *zb, uint8_t endpoint, uint16_t *group_list, uint8_t max_len)
{
    struct ZbApsmeGroupT group;
    unsigned int i, len = 0;
    enum ZbStatusCodeT status;

    ZbEnterCritical(zb);
    for (i = 0;; i++) {
        status = ZbApsGetIndex(zb, ZB_APS_IB_ID_GROUP_TABLE, &group, sizeof(group), i);
        if (status != ZB_APS_STATUS_SUCCESS) {
            break;
        }
        if (group.endpoint == ZB_ENDPOINT_BCAST) {
            continue;
        }
        if (group.endpoint != endpoint) {
            continue;
        }
        if (len >= max_len) {
            /* ZCL Groups: "If the total number of groups will cause
             * the maximum payload length of a ZigBee frame to be
             * exceeded, then the Group list field shall contain only
             * as many groups as will fit." */
            break;
        }
        group_list[len++] = group.groupAddr;
    }
    ZbExitCritical(zb);
    return len;
}

IPC_REQ_CONF_FUNC(ZbApsmeBindReq, MSG_M4TOM0_APSME_BIND, struct ZbApsmeBindReqT, struct ZbApsmeBindConfT)
IPC_REQ_CONF_FUNC(ZbApsmeUnbindReq, MSG_M4TOM0_APSME_UNBIND, struct ZbApsmeUnbindReqT, struct ZbApsmeUnbindConfT)

void
ZbApsUnbindAllReq(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_UNBIND_ALL;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

bool
ZbApsBindSrcExists(struct ZigBeeT *zb, uint8_t endpoint, uint16_t clusterId)
{
    struct ZbApsmeBindT entry;
    enum ZbStatusCodeT status;
    unsigned int i;
    uint64_t local_ext = ZbExtendedAddress(zb);

    ZbEnterCritical(zb);
    for (i = 0;; i++) {
        status = ZbApsGetIndex(zb, ZB_APS_IB_ID_BINDING_TABLE, &entry, sizeof(entry), i);
        if (status != ZB_APS_STATUS_SUCCESS) {
            break;
        }
        if (entry.srcExtAddr == 0ULL) {
            continue;
        }
        if (entry.srcExtAddr != local_ext) {
            continue;
        }
        if (entry.srcEndpt != endpoint) {
            continue;
        }
        if (entry.clusterId != clusterId) {
            continue;
        }
        ZbExitCritical(zb);
        return true;
    }
    ZbExitCritical(zb);
    return false;
}

IPC_REQ_FUNC(ZbApsmeTransportKeyReq, MSG_M4TOM0_APSME_TRANSPORT_KEY, struct ZbApsmeTransportKeyReqT);
IPC_REQ_FUNC(ZbApsmeRemoveDeviceReq, MSG_M4TOM0_APSME_REMOVE_DEVICE, struct ZbApsmeRemoveDeviceReqT);

enum ZbStatusCodeT
ZbApsmeRequestKeyReq(struct ZigBeeT *zb, struct ZbApsmeRequestKeyReqT *req)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_REQUEST_KEY;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;
}

IPC_REQ_FUNC(ZbApsmeSwitchKeyReq, MSG_M4TOM0_APSME_SWITCH_KEY, struct ZbApsmeSwitchKeyReqT);
IPC_REQ_CONF_FUNC(ZbApsmeAddKeyReq, MSG_M4TOM0_APSME_ADD_KEY, struct ZbApsmeAddKeyReqT, struct ZbApsmeAddKeyConfT);
IPC_REQ_CONF_FUNC(ZbApsmeGetKeyReq, MSG_M4TOM0_APSME_GET_KEY, struct ZbApsmeGetKeyReqT, struct ZbApsmeGetKeyConfT);
IPC_REQ_CONF_FUNC(ZbApsmeRemoveKeyReq, MSG_M4TOM0_APSME_REMOVE_KEY, struct ZbApsmeRemoveKeyReqT, struct ZbApsmeRemoveKeyConfT);

/******************************************************************************
 * Zigbee Message Filters
 ******************************************************************************
 */
PACKED_STRUCT zb_msg_filter_cb_info_t {
    struct ZbMsgFilterT *filter;
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg);
    void *arg;
};

#define ZB_IPC_MSG_FILTER_CB_LIST_MAX               32U
static struct zb_msg_filter_cb_info_t zb_msg_filter_cb_list[ZB_IPC_MSG_FILTER_CB_LIST_MAX];

struct ZbMsgFilterT *
ZbMsgFilterRegister(struct ZigBeeT *zb, uint32_t mask, uint8_t prio,
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbMsgFilterT *filter;
    struct zb_msg_filter_cb_info_t *cb_info;
    unsigned int i;

    for (i = 0; i < ZB_IPC_MSG_FILTER_CB_LIST_MAX; i++) {
        cb_info = &zb_msg_filter_cb_list[i];
        if (cb_info->filter == NULL) {
            break;
        }
    }
    if (i == ZB_IPC_MSG_FILTER_CB_LIST_MAX) {
        return NULL;
    }

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_FILTER_ADD;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)mask;
    ipcc_req->Data[1] = (uint32_t)prio;
    ipcc_req->Data[2] = (uint32_t)cb_info;
    ZIGBEE_CmdTransfer();
    filter = (struct ZbMsgFilterT *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (filter != NULL) {
        cb_info->filter = filter;
        cb_info->callback = callback;
        cb_info->arg = arg;
    }
    return filter;
    /* Followed up by MSG_M0TOM4_FILTER_MSG_CB */
}

void
ZbMsgFilterRemove(struct ZigBeeT *zb, struct ZbMsgFilterT *filter)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_msg_filter_cb_info_t *cb_info;
    unsigned int i;

    if (filter == NULL) {
        return;
    }
    for (i = 0; i < ZB_IPC_MSG_FILTER_CB_LIST_MAX; i++) {
        cb_info = &zb_msg_filter_cb_list[i];
        if (cb_info->filter == filter) {
            break;
        }
    }
    if (i == ZB_IPC_MSG_FILTER_CB_LIST_MAX) {
        return;
    }

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_FILTER_DEL;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)filter;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    cb_info->filter = NULL;
}

/******************************************************************************
 * Zigbee Timer API
 ******************************************************************************
 */

/* M4 version of 'struct ZbTimerT'. It allows us to wrap up callback and arg into
 * a single arg pointer when communicating with the M0. */
struct ZbTimerT {
    void (*callback)(struct ZigBeeT *zb, void *data);
    void *arg;
    void *m0_timer; /* Handle */
};

struct ZbTimerT *
ZbTimerAlloc(struct ZigBeeT *zb, void (*callback)(struct ZigBeeT *zb, void *cn_arg), void *arg)
{
    struct ZbTimerT *timer;

    timer = ZbHeapAlloc(NULL, sizeof(struct ZbTimerT));
    if (timer != NULL) {
        Zigbee_Cmd_Request_t *ipcc_req;

        /* Configure the callback struct */
        timer->callback = callback;
        timer->arg = arg;

        /* Pass this to the M0 (stack) */
        Pre_ZigbeeCmdProcessing();
        ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
        ipcc_req->ID = MSG_M4TOM0_TIMER_ALLOC;
        ipcc_req->Size = 1;
        ipcc_req->Data[0] = (uint32_t)timer;
        ZIGBEE_CmdTransfer();
        timer->m0_timer = (void *)zb_ipc_m4_get_retval();
        Post_ZigbeeCmdProcessing();
        if (timer->m0_timer == NULL) {
            ZbHeapFree(NULL, timer);
            timer = NULL;
        }
    }
    return timer;
}

void
ZbTimerChangeCallback(struct ZbTimerT *timer, void (*callback)(struct ZigBeeT *zb, void *cb_arg), void *arg)
{
    ZbTimerStop(timer);
    timer->callback = callback;
    timer->arg = arg;
}

void
ZbTimerFree(struct ZbTimerT *timer)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    /* Stop and free the timer on the M0 */
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TIMER_FREE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)timer->m0_timer;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();

    /* Free the timer struct on the M4 */
    ZbHeapFree(NULL, timer);
}

bool
ZbTimerRunning(struct ZbTimerT *timer)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TIMER_RUNNING;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)timer->m0_timer;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

unsigned int
ZbTimerRemaining(struct ZbTimerT *timer)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    unsigned int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TIMER_REMAINING;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)timer->m0_timer;
    ZIGBEE_CmdTransfer();
    rc = (unsigned int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

void
ZbTimerStop(struct ZbTimerT *timer)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TIMER_STOP;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)timer->m0_timer;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbTimerReset(struct ZbTimerT *timer, unsigned int timeout)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TIMER_RESET;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)timer->m0_timer;
    ipcc_req->Data[1] = (uint32_t)timeout;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

unsigned int
ZbTimeoutRemaining(ZbUptimeT now, ZbUptimeT expire_time)
{
    ZbUptimeT u_delta;

    /* Check for 'timeout' rollover condition */
    if ((now >= TIMER_ROLL_OVER_HIGH) && (expire_time <= TIMER_ROLL_OVER_LOW)) {
        /* Timeout has rolled over, we haven't expired.
         * Compute timeout remaining */
        u_delta = (ZB_UPTIME_MAX - now) + expire_time + 1U;

        return (unsigned int)u_delta;
    }

    /* Check for 'now' rollover condition */
    if ((expire_time >= TIMER_ROLL_OVER_HIGH) && (now <= TIMER_ROLL_OVER_LOW)) {
        /* 'now' has rolled over, so now is > timeout, meaning we expired. */
        return 0;
    }

    /* No rollover, check if timer has expired */
    if (now >= expire_time) {
        /* Timer has expired */
        return 0;
    }

    /* Compute time remaining */
    u_delta = expire_time - now;

    return (unsigned int)u_delta;
}

/******************************************************************************
 * NWK
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbNwkGetIndex(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbNlmeGetReqT nlmeGetReq;
    struct ZbNlmeGetConfT nlmeGetConf;
    enum ZbStatusCodeT status;

    /* Form the NLME-GET.request */
    nlmeGetReq.attrId = attrId;
    nlmeGetReq.attr = attrPtr;
    nlmeGetReq.attrLength = attrSz;
    nlmeGetReq.attrIndex = attrIndex;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_GET_INDEX;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)&nlmeGetReq;
    ipcc_req->Data[1] = (uint32_t)&nlmeGetConf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    status = nlmeGetConf.status;
    return status;
}

enum ZbStatusCodeT
ZbNwkSetIndex(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbNlmeSetReqT nlmeSetReq;
    struct ZbNlmeSetConfT nlmeSetConf;
    enum ZbStatusCodeT status;

    /* Form the NLME-SET.request */
    nlmeSetReq.attrId = attrId;
    nlmeSetReq.attr = attrPtr;
    nlmeSetReq.attrLength = attrSz;
    nlmeSetReq.attrIndex = attrIndex;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_SET_INDEX;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)&nlmeSetReq;
    ipcc_req->Data[1] = (uint32_t)&nlmeSetConf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
    status = nlmeSetConf.status;
    return status;
}

enum ZbStatusCodeT
ZbNwkGet(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz)
{
    return ZbNwkGetIndex(zb, attrId, attrPtr, attrSz, 0);
}

enum ZbStatusCodeT
ZbNwkSet(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz)
{
    return ZbNwkSetIndex(zb, attrId, attrPtr, attrSz, 0);
}

uint64_t
ZbNwkAddrLookupExt(struct ZigBeeT *zb, uint16_t nwkAddr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint64_t ext_addr;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_ADDR_LOOKUP_EXT;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)nwkAddr;
    ZIGBEE_CmdTransfer();
    /* Parse return value */
    ipcc_req = ZIGBEE_Get_OTCmdRspPayloadBuffer();
    assert(ipcc_req->Size == 2);
    zb_ipc_m4_memcpy2(&ext_addr, (void *)&ipcc_req->Data[0], 8);
    Post_ZigbeeCmdProcessing();
    return ext_addr;
}

uint16_t
ZbNwkAddrLookupNwk(struct ZigBeeT *zb, uint64_t extAddr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint16_t rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_ADDR_LOOKUP_NWK;
    /* Extended address is split across two args */
    ipcc_req->Size = 2;
    zb_ipc_m4_memcpy2((void *)ipcc_req->Data, &extAddr, 8);
    ZIGBEE_CmdTransfer();
    rc = (uint16_t)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbNwkGetSecMaterial(struct ZigBeeT *zb, uint8_t keySeqno, struct ZbNwkSecMaterialT *material)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_GET_SEC_MATERIAL;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)keySeqno;
    ipcc_req->Data[1] = (uint32_t)material;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

/* Helper function to get the currently active NWK Key. */
bool
ZbNwkGetActiveKey(struct ZigBeeT *zb, struct ZbNwkSecMaterialT *active_key)
{
    uint8_t seqno;
    struct ZbNwkSecMaterialT material;

    if (ZbNwkGet(zb, ZB_NWK_NIB_ID_ActiveKeySeqNumber, &seqno, sizeof(seqno)) != ZB_STATUS_SUCCESS) {
        return false;
    }
    if (!ZbNwkGetSecMaterial(zb, seqno, &material)) {
        return false;
    }
    if (active_key != NULL) {
        *active_key = material;
    }
    return true;
}

IPC_REQ_CALLBACK_FUNC(ZbNlmeNetDiscReq, MSG_M4TOM0_NLME_NET_DISC, struct ZbNlmeNetDiscReqT, struct ZbNlmeNetDiscConfT);
/* Followed up in MSG_M0TOM4_NLME_NET_DISC_CB handler */

#ifndef CONFIG_ZB_ENDNODE
IPC_REQ_CALLBACK_FUNC(ZbNlmeEdScanReq, MSG_M4TOM0_NLME_ED_SCAN, struct ZbNlmeEdScanReqT, struct ZbNlmeEdScanConfT);
/* Followed up in MSG_M0TOM4_NLME_ED_SCAN_CB handler */
#endif

IPC_REQ_CALLBACK_FUNC(ZbNlmeLeaveReq, MSG_M4TOM0_NLME_LEAVE, struct ZbNlmeLeaveReqT, struct ZbNlmeLeaveConfT);
/* Followed up in MSG_M0TOM4_NLME_LEAVE_CB handler */

IPC_REQ_CONF_FUNC(ZbNlmePermitJoinReq, MSG_M4TOM0_NLME_PERMIT_JOIN, struct ZbNlmePermitJoinReqT, struct ZbNlmePermitJoinConfT);
IPC_REQ_CONF_FUNC(ZbNlmeResetReq, MSG_M4TOM0_NLME_RESET, struct ZbNlmeResetReqT, struct ZbNlmeResetConfT);

IPC_REQ_CALLBACK_FUNC(ZbNlmeSyncReq, MSG_M4TOM0_NLME_SYNC, struct ZbNlmeSyncReqT, struct ZbNlmeSyncConfT);
/* Followed up in MSG_M0TOM4_NLME_SYNC_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbNlmeRouteDiscReq, MSG_M4TOM0_NLME_ROUTE_DISC, struct ZbNlmeRouteDiscReqT, struct ZbNlmeRouteDiscConfT);
/* Followed up in MSG_M0TOM4_NLME_ROUTE_DISC_CB handler */

enum WpanJoinPolicyT
ZbNlmeJoiningPolicyGet(struct ZigBeeT *zb)
{
    /* This is only for zbcli. We don't need this for 2.4 GHz operation. */
    return WPAN_JOIN_POLICY_ALL;
}

void
ZbNwkNeighborClearAll(struct ZigBeeT *zb, bool keep_parent, bool keep_children)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_NNT_CLEAR_ALL;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)keep_parent;
    ipcc_req->Data[1] = (uint32_t)keep_children;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

IPC_REQ_CONF_FUNC(ZbNlmeDirectJoinReq, MSG_M4TOM0_NLME_DIRECT_JOIN, struct ZbNlmeDirectJoinReqT, struct ZbNlmeDirectJoinConfT);
IPC_REQ_CONF_FUNC(ZbNlmeSetInterface, MSG_M4TOM0_NLME_SET_INTERFACE, struct ZbNlmeSetInterfaceReqT, struct ZbNlmeSetInterfaceConfT);
IPC_REQ_CONF_FUNC(ZbNlmeGetInterface, MSG_M4TOM0_NLME_GET_INTERFACE, struct ZbNlmeGetInterfaceReqT, struct ZbNlmeGetInterfaceConfT);

bool
ZbNwkIfSetTxPower(struct ZigBeeT *zb, const char *name, int8_t tx_power)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_IFC_SET_TX_POWER;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)name;
    ipcc_req->Data[1] = (uint32_t)tx_power;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbNwkIfGetTxPower(struct ZigBeeT *zb, const char *name, int8_t *tx_power)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_IFC_GET_TX_POWER;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)name;
    ipcc_req->Data[1] = (uint32_t)tx_power;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbNwkSetFrameCounter(struct ZigBeeT *zb, uint8_t keySeqno, uint64_t srcAddr, uint32_t newFrameCount)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_SET_FRAME_COUNTER;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)keySeqno;
    zb_ipc_m4_memcpy2((void *)&ipcc_req->Data[1], &srcAddr, 8);
    ipcc_req->Data[3] = (uint32_t)newFrameCount;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

struct nwk_fastpoll_entry_t *
ZbNwkFastPollRequest(struct ZigBeeT *zb, unsigned int delay, unsigned int timeout)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct nwk_fastpoll_entry_t *rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_FAST_POLL_REQUEST;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)delay;
    ipcc_req->Data[1] = (uint32_t)timeout;
    ZIGBEE_CmdTransfer();
    rc = (struct nwk_fastpoll_entry_t *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbNwkFastPollRelease(struct ZigBeeT *zb, struct nwk_fastpoll_entry_t *handle)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_FAST_POLL_RELEASE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)handle;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

unsigned int
ZbNwkFastPollResourceCount(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    unsigned int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_FAST_POLL_RESOURCE;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    rc = (unsigned int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbNwkSendEdkaReq(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_EDKA_SEND;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

/* NOTE: Required for ZCP testing, but also needed by the DUT. */
void
ZbNwkAddrSetNextChildAddr(struct ZigBeeT *zb, uint16_t nextChildAddr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_SET_NEXT_CHILD_ADDR;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)nextChildAddr;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

bool
ZbNwkCommissioningConfig(struct ZigBeeT *zb, struct ZbNwkCommissioningInfo *commission_info)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_SET_COMM_INFO;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)commission_info;
    ZIGBEE_CmdTransfer();
    rc = (bool)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

/******************************************************************************
 * ZDO Utility
 ******************************************************************************
 */
struct zdo_filter_cb_info_t {
    struct ZbZdoFilterT *filter;
    /* void (*callback)(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *annce, uint8_t seqno, void *arg); */
    void *callback;
    void *arg;
};

#define ZB_IPC_ZDO_FILTER_CB_LIST_MAX         8U
static struct zdo_filter_cb_info_t zdo_filter_cb_list[ZB_IPC_ZDO_FILTER_CB_LIST_MAX];

static struct zdo_filter_cb_info_t *
zdo_filter_cbinfo_get(void)
{
    struct zdo_filter_cb_info_t *cb_info;
    unsigned int i;

    for (i = 0; i < ZB_IPC_ZDO_FILTER_CB_LIST_MAX; i++) {
        cb_info = &zdo_filter_cb_list[i];
        if (cb_info->filter == NULL) {
            return cb_info;
        }
    }
    return NULL;
}

static bool
zdo_filter_cbinfo_del(struct ZbZdoFilterT *filter)
{
    struct zdo_filter_cb_info_t *cb_info;
    unsigned int i;

    if (filter == NULL) {
        return false;
    }
    for (i = 0; i < ZB_IPC_ZDO_FILTER_CB_LIST_MAX; i++) {
        cb_info = &zdo_filter_cb_list[i];
        if (cb_info->filter == filter) {
            cb_info->filter = NULL;
            return true;
        }
    }
    return false;
}

struct ZbZdoFilterT *
ZbZdoDeviceAnnceFilterRegister(struct ZigBeeT *zb,
    int (*callback)(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *annce, uint8_t seqno, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zdo_filter_cb_info_t *cb_info;
    struct ZbZdoFilterT *filter;

    if (callback == NULL) {
        return NULL;
    }
    cb_info = zdo_filter_cbinfo_get();
    if (cb_info == NULL) {
        return NULL;
    }

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_REG;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)cb_info;
    ZIGBEE_CmdTransfer();
    filter = (struct ZbZdoFilterT *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (filter != NULL) {
        cb_info->filter = filter;
        cb_info->callback = (void *)callback;
        cb_info->arg = arg;
    }
    return filter;
    /* Followed up in MSG_M0TOM4_ZDO_DEVICE_ANNCE_FILTER_CB handler */
}

/******************************************************************************
 * ZDO
 ******************************************************************************
 */
IPC_REQ_CALLBACK_FUNC(ZbZdoNwkAddrReq, MSG_M4TOM0_ZDO_NWK_ADDR, struct ZbZdoNwkAddrReqT, struct ZbZdoNwkAddrRspT);
/* Followed up in MSG_M0TOM4_ZDO_NWK_ADDR_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoIeeeAddrReq, MSG_M4TOM0_ZDO_IEEE_ADDR, struct ZbZdoIeeeAddrReqT, struct ZbZdoIeeeAddrRspT);
/* Followed up in MSG_M0TOM4_ZDO_IEEE_ADDR_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoNodeDescReq, MSG_M4TOM0_ZDO_NODE_DESC, struct ZbZdoNodeDescReqT, struct ZbZdoNodeDescRspT);
/* Followed up in MSG_M0TOM4_ZDO_NODE_DESC_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoPowerDescReq, MSG_M4TOM0_ZDO_POWER_DESC, struct ZbZdoPowerDescReqT, struct ZbZdoPowerDescRspT);
/* Followed up in MSG_M0TOM4_ZDO_POWER_DESC_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoSimpleDescReq, MSG_M4TOM0_ZDO_SIMPLE_DESC, struct ZbZdoSimpleDescReqT, struct ZbZdoSimpleDescRspT);
/* Followed up in MSG_M0TOM4_ZDO_SIMPLE_DESC_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoActiveEpReq, MSG_M4TOM0_ZDO_ACTIVE_EP, struct ZbZdoActiveEpReqT, struct ZbZdoActiveEpRspT);
/* Followed up in MSG_M0TOM4_ZDO_ACTIVE_EP_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoMatchDescReq, MSG_M4TOM0_ZDO_MATCH_DESC, struct ZbZdoMatchDescReqT, struct ZbZdoMatchDescRspT);
/* Followed up in MSG_M0TOM4_ZDO_MATCH_DESC_CB handler */

enum ZbStatusCodeT
ZbZdoMatchDescMulti(struct ZigBeeT *zb, struct ZbZdoMatchDescReqT *req,
    void (*callback)(struct ZbZdoMatchDescRspT *ind, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbStatusCodeT status;

    /* The callback must be static so we can receive multiple responses. We don't check here if
     * we're overwriting an existing callback, so application cannot effectively perform more
     * than one of these at a time. */

    /* Check if a request is already active */
    if (zdo_match_multi_cb != NULL) {
        return ZB_ZDP_STATUS_TABLE_FULL;
    }
    zdo_match_multi_cb = callback;
    zdo_match_multi_arg = arg;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MATCH_DESC_MULTI;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZB_STATUS_SUCCESS) {
        zdo_match_multi_cb = NULL;
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB handler */
}

IPC_REQ_FUNC(ZbZdoDeviceAnnce, MSG_M4TOM0_ZDO_DEVICE_ANNCE, struct ZbZdoDeviceAnnceT);
IPC_REQ_FUNC(ZbZdoDeviceAnnceAlias, MSG_M4TOM0_ZDO_DEVICE_ANNCE_ALIAS, struct ZbZdoDeviceAnnceT);

IPC_REQ_CALLBACK_FUNC(ZbZdoBindReq, MSG_M4TOM0_ZDO_BIND, struct ZbZdoBindReqT, struct ZbZdoBindRspT);
/* Followed up in MSG_M0TOM4_ZDO_BIND_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoUnbindReq, MSG_M4TOM0_ZDO_UNBIND, struct ZbZdoBindReqT, struct ZbZdoBindRspT);
/* Followed up in MSG_M0TOM4_ZDO_UNBIND_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoLqiReq, MSG_M4TOM0_ZDO_MGMT_LQI, struct ZbZdoLqiReqT, struct ZbZdoLqiRspT);
/* Followed up in MSG_M0TOM4_ZDO_MGMT_LQI_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoRtgReq, MSG_M4TOM0_ZDO_MGMT_RTG, struct ZbZdoRtgReqT, struct ZbZdoRtgRspT);
/* Followed up in MSG_M0TOM4_ZDO_MGMT_RTG_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoMgmtBindReq, MSG_M4TOM0_ZDO_MGMT_BIND, struct ZbZdoMgmtBindReqT, struct ZbZdoMgmtBindRspT);
/* Followed up in MSG_M0TOM4_ZDO_MGMT_BIND_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoLeaveReq, MSG_M4TOM0_ZDO_MGMT_LEAVE, struct ZbZdoLeaveReqT, struct ZbZdoLeaveRspT);
/* Followed up in MSG_M0TOM4_ZDO_MGMT_LEAVE_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoPermitJoinReq, MSG_M4TOM0_ZDO_MGMT_PERMIT_JOIN, struct ZbZdoPermitJoinReqT, struct ZbZdoPermitJoinRspT);
/* Followed up in MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB handler */

IPC_REQ_CALLBACK_FUNC(ZbZdoNwkUpdateReq, MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE, struct ZbZdoNwkUpdateReqT, struct ZbZdoNwkUpdateNotifyT);
/* Followed up in MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB handler */

struct ZbZdoFilterT *
ZbZdoNwkUpdateNotifyFilterRegister(struct ZigBeeT *zb,
    int (*callback)(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *msg, uint8_t seqno, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zdo_filter_cb_info_t *cb_info;
    struct ZbZdoFilterT *filter;

    if (callback == NULL) {
        return NULL;
    }
    cb_info = zdo_filter_cbinfo_get();
    if (cb_info == NULL) {
        return NULL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE_FILTER_REGISTER;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)cb_info;
    ZIGBEE_CmdTransfer();
    filter = (struct ZbZdoFilterT *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (filter != NULL) {
        cb_info->filter = filter;
        cb_info->callback = (void *)callback;
        cb_info->arg = arg;
    }
    return filter;
    /* Callbacks go through MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_FILTER_CB */
}

void
ZbZdoFilterRemove(struct ZigBeeT *zb, struct ZbZdoFilterT *filter)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    if (!zdo_filter_cbinfo_del(filter)) {
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_FILTER_DEL;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)filter;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

/******************************************************************************
 * ZCL
 ******************************************************************************
 */

ZbUptimeT
ZbZclUptime(struct ZigBeeT *zb)
{
    uint32_t uptime;

    ZbBdbGet(zb, ZB_BDB_Uptime, &uptime, sizeof(uptime));
    return (ZbUptimeT)uptime;
}

bool
ZbZclDeviceLogCheckAllow(struct ZigBeeT *zb, struct ZbApsdeDataIndT *dataIndPtr, struct ZbZclHeaderT *zclHdrPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_DEVICE_LOG_CHECK;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)dataIndPtr;
    ipcc_req->Data[1] = (uint32_t)zclHdrPtr;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbZclBasicPostAlarm(struct ZigBeeT *zb, uint8_t endpoint, uint8_t alarm_code)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_BASIC_SERVER_ALARM_POST;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)alarm_code;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

void
ZbZclBasicServerResetCmdConfig(struct ZigBeeT *zb, bool allow_reset)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_BASIC_SERVER_CONFIG_RESET;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)allow_reset;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

enum ZclStatusCodeT
ZbZclBasicWriteDirect(struct ZigBeeT *zb, uint8_t endpoint, uint16_t attributeId, const uint8_t *ptr, unsigned int len)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZclStatusCodeT rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_BASIC_SERVER_LOCAL_WRITE;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)attributeId;
    ipcc_req->Data[2] = (uint32_t)ptr;
    ipcc_req->Data[3] = (uint32_t)len;
    ZIGBEE_CmdTransfer();
    rc = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

void
ZbZclBasicServerConfigDefaults(struct ZigBeeT *zb, const struct ZbZclBasicServerDefaults *defaults)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_BASIC_SERVER_CONFIG_DEFAULTS;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)defaults;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

bool
ZbZclDiagServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId, enum ZbStatusCodeT minSecurity)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_DIAG_SERVER_ALLOC;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)profileId;
    ipcc_req->Data[2] = (uint32_t)minSecurity;
    ZIGBEE_CmdTransfer();
    rc = (bool)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

void
ZbZclAddEndpoint(struct ZigBeeT *zb, struct ZbApsmeAddEndpointReqT *req, struct ZbApsmeAddEndpointConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_ENDPOINT_ADD;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbZclRemoveEndpoint(struct ZigBeeT *zb, struct ZbApsmeRemoveEndpointReqT *req, struct ZbApsmeRemoveEndpointConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_ENDPOINT_DEL;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

IPC_CLUSTER_REQ_CALLBACK_FUNC(ZbZclReadReq, MSG_M4TOM0_ZCL_READ_REQ, ZbZclReadReqT, ZbZclReadRspT);
/* Followed up in MSG_M0TOM4_ZCL_READ_CB handler */

enum ZclStatusCodeT
ZbZclWriteReq(struct ZbZclClusterT *clusterPtr, ZbZclWriteReqT *req,
    void (*callback)(const struct ZbZclWriteRspT *rsp, void *cbarg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info;
    enum ZclStatusCodeT status;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg);
    if (info == NULL) {
        return ZCL_STATUS_INSUFFICIENT_SPACE;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_WRITE_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_WRITE_CB handler */
}

IPC_CLUSTER_REQ_CALLBACK_FUNC(ZbZclDiscoverAttrReq, MSG_M4TOM0_ZCL_DISCOVER_ATTR_REQ,
    ZbZclDiscoverAttrReqT, ZbZclDiscoverAttrRspT);
/* Followed up in MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB handler */

uint8_t
ZbZclGetNextSeqnum(void)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint8_t rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_GET_SEQNUM;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    rc = (uint8_t)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

enum ZclStatusCodeT
ZbZclCommandReq(struct ZigBeeT *zb, struct ZbZclCommandReqT *zclReq,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info = NULL;
    enum ZclStatusCodeT status;

    if (callback != NULL) {
        info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
        if (info == NULL) {
            return ZCL_STATUS_INSUFFICIENT_SPACE;
        }
        if (ZbApsAddrIsBcast(&zclReq->dst)) {
            info->zcl_recv_multi_rsp = true; /* callback only freed on ZCL_STATUS_TIMEOUT */
        }
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_COMMAND_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)zclReq;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZCL_STATUS_SUCCESS) {
        if (info != NULL) {
            zb_ipc_m4_cb_info_free(info);
        }
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_COMMAND_REQ_CB handler if callback != NULL */
}

void
ZbZclSendDefaultResponse(struct ZbZclClusterT *clusterPtr, struct ZbApsdeDataIndT *dataIndPtr,
    struct ZbZclHeaderT *zclHdrPtr, enum ZclStatusCodeT status)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_SEND_DEFAULT_RSP;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)dataIndPtr;
    ipcc_req->Data[2] = (uint32_t)zclHdrPtr;
    ipcc_req->Data[3] = (uint32_t)status;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

enum ZclStatusCodeT
ZbZclClusterCommandReq(struct ZbZclClusterT *clusterPtr, struct ZbZclClusterCommandReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg)
{
    struct ZbZclCommandReqT zcl_req;

    /* Configure the request */
    ZbZclClusterInitCommandReq(clusterPtr, &zcl_req);
    zcl_req.dst = req->dst;
    /* ZCL Header */
    zcl_req.hdr.cmdId = req->cmdId;
    zcl_req.hdr.frameCtrl.frameType = ZCL_FRAMETYPE_CLUSTER;
    zcl_req.hdr.frameCtrl.manufacturer = (clusterPtr->mfrCode != 0U) ? 1U : 0U;
    zcl_req.hdr.frameCtrl.direction = (clusterPtr->direction == ZCL_DIRECTION_TO_SERVER) ? \
        ZCL_DIRECTION_TO_CLIENT : ZCL_DIRECTION_TO_SERVER;
    zcl_req.hdr.frameCtrl.noDefaultResp = req->noDefaultResp;
    zcl_req.hdr.manufacturerCode = clusterPtr->mfrCode;
    zcl_req.hdr.seqNum = ZbZclGetNextSeqnum();

    /* Payload */
    zcl_req.payload = req->payload;
    zcl_req.length = req->length;

    return ZbZclCommandReq(clusterPtr->zb, &zcl_req, callback, arg);
}

struct cluster_command_delay_t {
    struct ZbZclClusterT *cluster;
    struct ZbTimerT *timer;
    struct ZbZclClusterCommandReqT req;
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg);
    void *arg;
};

static void
cluster_command_timer(struct ZigBeeT *zb, void *arg)
{
    struct cluster_command_delay_t *info = arg;
    enum ZclStatusCodeT status;

    status = ZbZclClusterCommandReq(info->cluster, &info->req, info->callback, info->arg);
    if (status != ZCL_STATUS_SUCCESS) {
        if (info->callback != NULL) {
            struct ZbZclCommandRspT rsp;

            (void)memset(&rsp, 0, sizeof(rsp));
            rsp.status = status;
            rsp.profileId = info->cluster->profileId;
            rsp.clusterId = info->cluster->clusterId;
            info->callback(&rsp, info->arg);
        }
    }

    ZbTimerFree(info->timer);
    ZbHeapFree(info->cluster->zb, info);
}

enum ZclStatusCodeT
ZbZclClusterCommandReqDelayed(struct ZbZclClusterT *cluster, struct ZbZclClusterCommandReqT *req,
    unsigned int delay, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg)
{
    struct cluster_command_delay_t *info;

    info = ZbHeapAlloc(cluster->zb, sizeof(struct cluster_command_delay_t) + req->length);
    if (info == NULL) {
        return ZCL_STATUS_FAILURE;
    }
    (void)memset(info, 0, sizeof(struct cluster_command_delay_t));
    info->timer = ZbTimerAlloc(cluster->zb, cluster_command_timer, info);
    if (info->timer == NULL) {
        ZbHeapFree(cluster->zb, info);
        return ZCL_STATUS_FAILURE;
    }
    info->cluster = cluster;
    info->callback = callback;
    info->arg = arg;
    (void)memcpy(&info->req, req, sizeof(struct ZbZclClusterCommandReqT));
    info->req.payload = (uint8_t *)(info + 1);
    (void)memcpy(info->req.payload, req->payload, req->length);

    ZbTimerReset(info->timer, delay);
    return ZCL_STATUS_SUCCESS;
}

enum ZclStatusCodeT
ZbZclClusterCommandRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo,
    uint8_t cmdId, struct ZbApsBufT *payloads, uint8_t numPayloads)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZclStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP;
    ipcc_req->Size = 5;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)dstInfo;
    ipcc_req->Data[2] = (uint32_t)cmdId;
    ipcc_req->Data[3] = (uint32_t)payloads;
    ipcc_req->Data[4] = (uint32_t)numPayloads;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;
}

enum ZclStatusCodeT
ZbZclClusterCommandRspWithCb(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo, uint8_t cmdId,
    struct ZbApsBufT *payloads, uint8_t numPayloads, void (*callback)(struct ZbApsdeDataConfT *conf, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info = NULL;
    enum ZclStatusCodeT status;

    if (callback == NULL) {
        return ZbZclClusterCommandRsp(clusterPtr, dstInfo, cmdId, payloads, numPayloads);
    }
    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZCL_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_CB;
    ipcc_req->Size = 6;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)dstInfo;
    ipcc_req->Data[2] = (uint32_t)cmdId;
    ipcc_req->Data[3] = (uint32_t)payloads;
    ipcc_req->Data[4] = (uint32_t)numPayloads;
    ipcc_req->Data[5] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB handler */
}

/* NOTE: This API changed for ZCL8. Both the M0 and M4 binaries must be updated if
 * upgrading from ZCL7 to ZCL8. */
enum ZclStatusCodeT
ZbZclSendClusterStatusResponse(struct ZbZclClusterT *clusterPtr, struct ZbApsdeDataIndT *dataIndPtr,
    struct ZbZclHeaderT *zclHdrPtr, uint8_t cmdId, uint8_t *zclPayload, uint8_t zclPaylen, bool allow_bcast)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZclStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_STATUS;
    ipcc_req->Size = 7;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)dataIndPtr;
    ipcc_req->Data[2] = (uint32_t)zclHdrPtr;
    ipcc_req->Data[3] = (uint32_t)cmdId;
    ipcc_req->Data[4] = (uint32_t)zclPayload;
    ipcc_req->Data[5] = (uint32_t)zclPaylen;
    ipcc_req->Data[6] = (uint32_t)allow_bcast;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;
}

bool
ZbZclClusterEndpointRegister(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_EP_REGISTER;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

bool
ZbZclClusterEndpointRemove(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_EP_REMOVE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

enum ZclStatusCodeT
ZbZclClusterBind(struct ZbZclClusterT *clusterPtr, uint8_t endpoint, uint16_t profileId, enum ZbZclDirectionT direction)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZclStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_BIND;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)endpoint;
    ipcc_req->Data[2] = (uint32_t)profileId;
    ipcc_req->Data[3] = (uint32_t)direction;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;

    /* Data indication callbacks go to MSG_M0TOM4_ZCL_CLUSTER_DATA_IND */
}

void
ZbZclClusterUnbind(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_UNBIND;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

struct ZbApsFilterT *
ZbZclClusterReverseBind(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbApsFilterT *aps_filter_ptr;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_REVERSE_BIND;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    aps_filter_ptr = (struct ZbApsFilterT *)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return aps_filter_ptr;
    /* Data indication callbacks go to MSG_M0TOM4_ZCL_CLUSTER_DATA_IND */
}

void
ZbZclClusterReverseUnbind(struct ZbZclClusterT *clusterPtr, struct ZbApsFilterT *filter)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_REVERSE_UNBIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)filter;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

enum ZclStatusCodeT
ZbZclClusterRegisterAlarmResetHandler(struct ZbZclClusterT *clusterPtr,
    enum ZclStatusCodeT (*callback)(struct ZbZclClusterT *clusterPtr, uint8_t alarm_code,
        uint16_t cluster_id, struct ZbApsdeDataIndT *data_ind, struct ZbZclHeaderT *hdr))
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZclStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_ALARM_ADD_FILTER;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)callback;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return status;
    /* Callbacks followed up in MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB handler. */
}

void
ZbZclClusterRemoveAlarmResetHandler(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_ALARM_REMOVE_FILTER;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbZclClusterSendAlarm(struct ZbZclClusterT *clusterPtr, uint8_t src_endpoint, uint8_t alarm_code)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_ALARM_SEND;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)src_endpoint;
    ipcc_req->Data[2] = (uint32_t)alarm_code;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

/******************************************************************************
 * SE Key Exchange
 ******************************************************************************
 */
enum ZclStatusCodeT
ZbZclKeWithDevice(struct ZigBeeT *zb, uint64_t partnerAddr, bool aps_req_key,
    void (*callback)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT key_status, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info_t *info = NULL;
    enum ZclStatusCodeT status;

    if (callback != NULL) {
        info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
        if (info == NULL) {
            return ZCL_STATUS_ALLOC_FAIL;
        }
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_KE_WITH_DEVICE;
    ipcc_req->Size = 4;
    zb_ipc_m4_memcpy2((void *)&ipcc_req->Data[0], &partnerAddr, 8);
    ipcc_req->Data[2] = (uint32_t)aps_req_key;
    ipcc_req->Data[3] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    if (status != ZCL_STATUS_SUCCESS) {
        if (info != NULL) {
            zb_ipc_m4_cb_info_free(info);
        }
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB handler */
}

/******************************************************************************
 * AES & Hashing
 ******************************************************************************
 */
bool
ZbAesMmoHash(uint8_t const *data, const unsigned int length, uint8_t *digest)
{
    struct ZbHash hash;

    memset(&hash, 0, sizeof(struct ZbHash));
    ZbHashInit(&hash);
    ZbHashAdd(&hash, data, length);
    ZbHashDigest(&hash, digest);
    return true;
}

void
ZbHashInit(struct ZbHash *h)
{
    (void)memset(h->m, 0, sizeof(h->m));
    (void)memset(h->hash, 0, sizeof(h->hash));
    /* Don't touch h->key */
    h->length = 0;
}

void
ZbHashAdd(struct ZbHash *h, const void *data, uint32_t len)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_HASH_ADD;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)h;
    ipcc_req->Data[1] = (uint32_t)data;
    ipcc_req->Data[2] = (uint32_t)len;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

#ifdef ZIGBEE_DIRECT_ACTIVATED
void
ZbHashByte(struct ZbHash *h, uint8_t data)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_HASH_BYTE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)h;
    ipcc_req->Data[1] = (uint32_t)data;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

#endif /*ZIGBEE_DIRECT_ACTIVATED */

void
ZbHashDigest(struct ZbHash *h, void *digest)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_HASH_DIGEST;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)h;
    ipcc_req->Data[1] = (uint32_t)digest;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbHmacInit(struct ZbHash *h, const void *key, uint32_t len)
{
    const unsigned char ipad = 0x36;
    const unsigned char opad = 0x5c;
    unsigned int i;

    /* Initialize the hash data. */
    ZbHashInit(h);
    /* If the key is longer than the digest size, hash it into something smaller. */
    if (len > AES_BLOCK_SIZE) {
        ZbHashAdd(h, key, len);
        ZbHashDigest(h, h->key);
        ZbHashInit(h);
    }
    else {
        (void)memcpy(h->key, key, AES_BLOCK_SIZE);
    }

    /* Apply the inner padding and input the key to the hash function. */
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        h->key[i] ^= ipad;
    }
    ZbHashAdd(h, h->key, AES_BLOCK_SIZE);

    /* Undo the inner padding, and save the outer-padded key for later. */
    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        h->key[i] ^= (ipad ^ opad);
    }
}

void
ZbHmacDigest(struct ZbHash *h, void *digest)
{
    /* Compute the inner digest of the inner-padded key and the message. */
    ZbHashDigest(h, digest);

    /* Reset the hash, and compute the digest of the outer-padded key and the inner digest. */
    ZbHashInit(h);
    ZbHashAdd(h, h->key, AES_BLOCK_SIZE);
    ZbHashAdd(h, digest, AES_BLOCK_SIZE);
    ZbHashDigest(h, digest);
}

/******************************************************************************
 * Security Helpers
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbSecAddDeviceLinkKeyByKey(struct ZigBeeT *zb, uint64_t extAddr, uint8_t *key)
{
    struct ZbApsmeAddKeyReqT addKeyReq;
    struct ZbApsmeAddKeyConfT addKeyConf;

    (void)memset(&addKeyReq, 0, sizeof(addKeyReq));
    addKeyReq.keyType = ZB_SEC_KEYTYPE_TC_LINK;
    (void)memcpy(addKeyReq.key, key, sizeof(addKeyReq.key));
    addKeyReq.keySeqNumber = 0;
    addKeyReq.partnerAddr = extAddr;
    addKeyReq.keyAttribute = ZB_APSME_KEY_ATTR_VERIFIED; /* 0x00 */
    ZbApsmeAddKeyReq(zb, &addKeyReq, &addKeyConf);
    return addKeyConf.status;
}

enum ZbStatusCodeT
ZbSecAddDeviceLinkKeyByInstallCode(struct ZigBeeT *zb, uint64_t extAddr, uint8_t *ic, unsigned int len)
{
    uint8_t key[ZB_SEC_KEYSIZE];

    if (!ZbAesMmoHash(ic, len, key)) {
        return ZB_APS_STATUS_INVALID_PARAMETER;
    }
    return ZbSecAddDeviceLinkKeyByKey(zb, extAddr, key);
}

void
ZbSecKeyTransform(uint8_t *key, uint8_t input, uint8_t *keyOut)
{
    struct ZbHash hash;

    ZbHmacInit(&hash, key, ZB_SEC_KEYSIZE);
    ZbHmacAdd(&hash, &input, sizeof(input));
    ZbHmacDigest(&hash, keyOut);
}

#ifdef ZIGBEE_DIRECT_ACTIVATED
void
ZbCcmTransform(const void *key, const void *nonce, uint32_t nonce_len, void *m, uint32_t m_len, void *mic)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_AES_CCM_TRANSFORM;
    ipcc_req->Size = 6;
    ipcc_req->Data[0] = (uint32_t)key;
    ipcc_req->Data[1] = (uint32_t)nonce;
    ipcc_req->Data[2] = (uint32_t)nonce_len;
    ipcc_req->Data[3] = (uint32_t)m;
    ipcc_req->Data[4] = (uint32_t)m_len;
    ipcc_req->Data[5] = (uint32_t)mic;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbCcmAuthenticate(const void *key, const void *nonce, uint32_t nonce_len, const void *a, uint32_t a_len,
    const void *m, uint32_t m_len, void *mic, uint32_t mic_len)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_AES_CCM_AUTHENTICATE;
    ipcc_req->Size = 9;
    ipcc_req->Data[0] = (uint32_t)key;
    ipcc_req->Data[1] = (uint32_t)nonce;
    ipcc_req->Data[2] = (uint32_t)nonce_len;
    ipcc_req->Data[3] = (uint32_t)a;
    ipcc_req->Data[4] = (uint32_t)a_len;
    ipcc_req->Data[5] = (uint32_t)m;
    ipcc_req->Data[6] = (uint32_t)m_len;
    ipcc_req->Data[7] = (uint32_t)mic;
    ipcc_req->Data[8] = (uint32_t)mic_len;
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

int
ec_mul_x25519(const void *private, const void *base, void *result)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_EC_MUL_X25519;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t) private;
    ipcc_req->Data[1] = (uint32_t)base;
    ipcc_req->Data[2] = (uint32_t)result;
    ZIGBEE_CmdTransfer();
    rc = (int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

int
ec_mul_p256(const void *priv, const void *pub_x, const void *pub_y, void *result_x, void *result_y)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_EC_MUL_P256;
    ipcc_req->Size = 5;
    ipcc_req->Data[0] = (uint32_t)priv;
    ipcc_req->Data[1] = (uint32_t)pub_x;
    ipcc_req->Data[2] = (uint32_t)pub_y;
    ipcc_req->Data[3] = (uint32_t)result_x;
    ipcc_req->Data[4] = (uint32_t)result_y;
    ZIGBEE_CmdTransfer();
    rc = (int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

enum sha_status_t
SHA256Reset(SHA256Context *context)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum sha_status_t rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_SHA256_RESET;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)context;
    ZIGBEE_CmdTransfer();
    rc = (enum sha_status_t)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

enum sha_status_t
SHA256Input(SHA256Context *context, const uint8_t *bytes, unsigned int bytecount)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum sha_status_t rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_SHA256_INPUT;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)context;
    ipcc_req->Data[1] = (uint32_t)bytes;
    ipcc_req->Data[2] = (uint32_t)bytecount;
    ZIGBEE_CmdTransfer();
    rc = (enum sha_status_t)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

enum sha_status_t
SHA256Result(SHA256Context *context, uint8_t Message_Digest[SHA256HashSize])
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum sha_status_t rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_SHA256_RESULT;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)context;
    ipcc_req->Data[1] = (uint32_t)Message_Digest;
    ZIGBEE_CmdTransfer();
    rc = (enum sha_status_t)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

int
hmacReset(HMACContext *context, enum SHAversion whichSha,
    const unsigned char *key, int key_len)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_SHA256_HMAC_RESET;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)context;
    ipcc_req->Data[1] = (uint32_t)whichSha;
    ipcc_req->Data[2] = (uint32_t)key;
    ipcc_req->Data[3] = (uint32_t)key_len;
    ZIGBEE_CmdTransfer();
    rc = (int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

int
hmacInput(HMACContext *context, const unsigned char *text, int text_len)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_SHA256_HMAC_INPUT;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)context;
    ipcc_req->Data[1] = (uint32_t)text;
    ipcc_req->Data[2] = (uint32_t)text_len;
    ZIGBEE_CmdTransfer();
    rc = (int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

int
hmacResult(HMACContext *context, uint8_t digest[USHAMaxHashSize])
{
    Zigbee_Cmd_Request_t *ipcc_req;
    int rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_SHA256_HMAC_RESULT;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)context;
    ipcc_req->Data[1] = (uint32_t)digest;
    ZIGBEE_CmdTransfer();
    rc = (int)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

#endif /* ZIGBEE_DIRECT_ACTIVATED */

/******************************************************************************
 * Misc Debug
 ******************************************************************************
 */
void
ZbDebugMemory(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_DEBUG_MEMORY;
    ipcc_req->Size = 0;
    /* ipcc_req->Data[0] = (uint32_t)xxx; */
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

void
ZbDebugInfo(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_DEBUG_INFO;
    ipcc_req->Size = 0;
    /* ipcc_req->Data[0] = (uint32_t)xxx; */
    ZIGBEE_CmdTransfer();
    Post_ZigbeeCmdProcessing();
}

/******************************************************************************
 * MAC (direct access)
 ******************************************************************************
 */
/* Configures g_MAC_PROP_STRICT_DATA_POLL_REQ_c */

uint8_t
/* MAC_Status_t */
MacSetPropStrictDataPollReq(uint8_t val)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint8_t rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_MAC_SET_PROP_STRICT_DATA_POLL_REQ;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)val;
    ZIGBEE_CmdTransfer();
    rc = (uint8_t)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

/******************************************************************************
 * Memory Helpers
 ******************************************************************************
 */
void *
zb_heap_alloc(struct ZigBeeT *zb, size_t sz, const char *filename, unsigned int line)
{
    /* The M4 has access to malloc */
    return malloc(sz);
}

void
zb_heap_free(struct ZigBeeT *zb, void *ptr, const char *filename, unsigned int line)
{
    free(ptr);
}

bool
zb_ipc_get_secured_mem_info(uint32_t *unsec_sram2a_sz, uint32_t *unsec_sram2b_sz)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_GET_SECURED_MEM_INFO;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)unsec_sram2a_sz;
    ipcc_req->Data[1] = (uint32_t)unsec_sram2b_sz;
    ZIGBEE_CmdTransfer();
    rc = zb_ipc_m4_get_retval() != 0U ? true : false;
    Post_ZigbeeCmdProcessing();
    return rc;
}

unsigned long
ZbHeapAvailable(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    unsigned long rc;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_GET_ZB_HEAP_AVAILABLE;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    rc = (unsigned long)zb_ipc_m4_get_retval();
    Post_ZigbeeCmdProcessing();
    return rc;
}

/******************************************************************************
 * CRC (required for ZCL reporting (hash)
 ******************************************************************************
 */
/*
 * Precomputed CRC table for IEEE 802.15.4 FCS fields. Intended to be
 * used in the reflected CRC algorithm. Generated using the Rocksoft
 * CRC table generator with the following parameters:
 *
 * Width = 2 bytes
 * Polynomial = x^16 + x^12 + x^5 + 1 (0x1021)
 * Reflected = true
 */
const uint16_t wpanCrcTable[256] = {
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
    0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
    0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E,
    0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
    0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD,
    0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
    0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C,
    0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
    0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
    0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
    0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A,
    0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
    0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9,
    0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
    0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738,
    0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
    0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7,
    0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
    0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036,
    0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
    0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5,
    0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
    0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134,
    0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
    0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3,
    0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
    0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
    0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
    0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1,
    0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
    0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330,
    0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
};

uint16_t
WpanCrc(uint16_t crc, const void *dataPtr, unsigned int dataLen)
{
    const uint8_t *p = dataPtr;
    /*
     * Reflected CRC for dummies:
     *
     * Step 1: Right-Shift the crc register by one byte, save the byte that
     *          pops off as an index to the CRC table.
     * Step 2: XOR the index with the next byte of data.
     * Step 3: XOR the crc register with the table entry given by the index.
     * Step 4: Repeat until out of data.
     *
     * Non-Reflected CRCs use the same algorithm, except that the crc register
     * is shifted left, and the table needs to be regenerated.
     */
    while (dataLen--) {
        crc = (crc >> 8) ^ wpanCrcTable[(crc & 0xff) ^ *p++];
    }
    return crc;
}

/**
 * @brief  This function is used to manage all the callbacks used by the
 *         OpenThread interface. These callbacks are used for example to
 *         notify the application as soon as the state of a device has been
 *         modified.
 *
 *         Important Note: This function must be called each time a message
 *         is sent from the M0 to the M4.
 *
 * @param  None
 * @retval None
 */
HAL_StatusTypeDef
Zigbee_CallBackProcessing(void)
{
    HAL_StatusTypeDef status = HAL_OK;
    struct zb_ipc_m4_cb_info_t *info = NULL;
    Zigbee_Cmd_Request_t *p_notification;
    uint32_t retval = 0;

    /* Get pointer on received event buffer from M0 */
    p_notification = ZIGBEE_Get_NotificationPayloadBuffer();

    switch (p_notification->ID) {
        case MSG_M0TOM4_ZB_DESTROY_CB:
            zb_ipc_globals.zb = NULL;
            assert(p_notification->Size == 1);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[0];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(void *arg);

                callback = (void (*)(void *arg))info->callback;
                callback(info->arg);
            }
            break;

        case MSG_M0TOM4_FILTER_MSG_CB:
        {
            struct zb_msg_filter_cb_info_t *cb_info;
            enum zb_msg_filter_rc filter_rc;

            assert(p_notification->Size == 3);
            cb_info = (struct zb_msg_filter_cb_info_t *)p_notification->Data[2];
            filter_rc = cb_info->callback(zb_ipc_globals.zb, (uint32_t)p_notification->Data[0],
                    (void *)p_notification->Data[1], cb_info->arg);
            retval = (uint32_t)filter_rc;
            break;
        }

        case MSG_M0TOM4_TIMER_CB:
        {
            struct ZbTimerT *timer;

            assert(p_notification->Size == 1);
            timer = (struct ZbTimerT *)p_notification->Data[0];
            if (timer->callback != NULL) {
                timer->callback(NULL, timer->arg);
            }
            break;
        }

        case MSG_M0TOM4_STARTUP_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_REJOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeJoinConfT *conf, void *arg);

                callback = (void (*)(struct ZbNlmeJoinConfT *conf, void *arg))info->callback;
                callback((struct ZbNlmeJoinConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_PERSIST_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_FINDBIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_TCSO_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbTcsoStatusT status, void *arg);

                callback = (void (*)(enum ZbTcsoStatusT status, void *arg))info->callback;
                callback((enum ZbTcsoStatusT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_TC_REJOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_PERSIST_CB:
            if (zb_persist_cb != NULL) {
                zb_persist_cb(zb_ipc_globals.zb, zb_persist_arg);
            }
            break;

        case MSG_M0TOM4_ZB_LEAVE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeLeaveConfT *conf, void *arg);

                callback = (void (*)(struct ZbNlmeLeaveConfT *conf, void *arg))info->callback;
                callback((struct ZbNlmeLeaveConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZB_STATE_PAUSE_CB:
            assert(p_notification->Size == 1);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[0];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(void *arg);

                callback = (void (*)(void *arg))info->callback;
                callback(info->arg);
            }
            break;

        /* void (*callback)(struct ZbTlGetGroupIdsRspCmd *rsp, void *arg) */
        case MSG_M0TOM4_ZCL_TL_GET_GRP_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbTlGetGroupIdsRspCmd *rsp, void *arg);

                callback = (void (*)(struct ZbTlGetGroupIdsRspCmd *rsp, void *arg))info->callback;
                callback((struct ZbTlGetGroupIdsRspCmd *)p_notification->Data[0], info->arg);
            }
            break;

        /* void (*callback)(struct ZbTlGetEpListRspCmd *rsp, void *arg) */
        case MSG_M0TOM4_ZCL_TL_GET_EPLIST_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbTlGetEpListRspCmd *rsp, void *arg);

                callback = (void (*)(struct ZbTlGetEpListRspCmd *rsp, void *arg))info->callback;
                callback((struct ZbTlGetEpListRspCmd *)p_notification->Data[0], info->arg);
            }
            break;

        /* void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg) */
        case MSG_M0TOM4_ZCL_TL_SEND_EPINFO_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZclCommandRspT *rsp, void *arg);

                callback = (void (*)(struct ZbZclCommandRspT *rsp, void *arg))info->callback;
                callback((struct ZbZclCommandRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_APSDE_DATA_REQ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbApsdeDataConfT *conf, void *arg);

                callback = (void (*)(struct ZbApsdeDataConfT *conf, void *arg))info->callback;
                callback((struct ZbApsdeDataConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_APS_FILTER_ENDPOINT_CB:
        {
            struct ZbApsdeDataIndT *data_ind;
            struct aps_filter_cb_t *aps_filter_cb;
            int err = ZB_APS_FILTER_CONTINUE;

            assert(p_notification->Size == 2);
            data_ind = (struct ZbApsdeDataIndT *)p_notification->Data[0];
            aps_filter_cb = (struct aps_filter_cb_t *)p_notification->Data[1];
            if (aps_filter_cb->callback != NULL) {
                err = aps_filter_cb->callback(data_ind, aps_filter_cb->cb_arg);
            }
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_APS_FILTER_CLUSTER_CB:
        {
            struct ZbApsdeDataIndT *data_ind;
            struct aps_filter_cb_t *aps_filter_cb;
            int err = ZB_APS_FILTER_CONTINUE;

            assert(p_notification->Size == 2);
            data_ind = (struct ZbApsdeDataIndT *)p_notification->Data[0];
            aps_filter_cb = (struct aps_filter_cb_t *)p_notification->Data[1];
            if (aps_filter_cb->callback != NULL) {
                err = aps_filter_cb->callback(data_ind, aps_filter_cb->cb_arg);
            }
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_NLME_NET_DISC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeNetDiscConfT *conf, void *arg);

                callback = (void (*)(struct ZbNlmeNetDiscConfT *conf, void *arg))info->callback;
                callback((struct ZbNlmeNetDiscConfT *)p_notification->Data[0], info->arg);
            }
            break;

#ifndef CONFIG_ZB_ENDNODE
        case MSG_M0TOM4_NLME_ED_SCAN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeEdScanConfT *conf, void *arg);

                callback = (void (*)(struct ZbNlmeEdScanConfT *conf, void *arg))info->callback;
                callback((struct ZbNlmeEdScanConfT *)p_notification->Data[0], info->arg);
            }
            break;
#endif

        case MSG_M0TOM4_NLME_LEAVE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeLeaveConfT *conf, void *arg);

                callback = (void (*)(struct ZbNlmeLeaveConfT *conf, void *arg))info->callback;
                callback((struct ZbNlmeLeaveConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_SYNC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeSyncConfT *conf, void *arg);

                callback = (void (*)(struct ZbNlmeSyncConfT *discConf, void *arg))info->callback;
                callback((struct ZbNlmeSyncConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_ROUTE_DISC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeRouteDiscConfT *discConf, void *cbarg);

                callback = (void (*)(struct ZbNlmeRouteDiscConfT *discConf, void *cbarg))info->callback;
                callback((struct ZbNlmeRouteDiscConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_DEVICE_ANNCE_FILTER_CB:
        {
            struct zdo_filter_cb_info_t *cb_info;

            assert(p_notification->Size == 3);
            cb_info = (void *)p_notification->Data[2];
            if ((cb_info != NULL) && (cb_info->callback != NULL)) {
                struct ZbZdoDeviceAnnceT *msg;
                uint8_t seqno;
                unsigned int i;
                int (*callback)(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *annce, uint8_t seqno, void *arg);

                for (i = 0; i < ZB_IPC_ZDO_FILTER_CB_LIST_MAX; i++) {
                    /* Find the matching filter callback */
                    if (cb_info != &zdo_filter_cb_list[i]) {
                        continue;
                    }
                    if (cb_info->filter == NULL) {
                        /* Shouldn't get here */
                        break;
                    }
                    /* Call the Device Annce callback */
                    msg = (struct ZbZdoDeviceAnnceT *)p_notification->Data[0];
                    seqno = (uint8_t)p_notification->Data[1];
                    callback = (int (*)(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceT *annce, uint8_t seqno, void *arg))cb_info->callback;
                    callback(zb_ipc_globals.zb, msg, seqno, cb_info->arg);
                    break;
                }
            }
            break;
        }

        case MSG_M0TOM4_ZDO_NWK_ADDR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoNwkAddrRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoNwkAddrRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoNwkAddrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_IEEE_ADDR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoIeeeAddrRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoIeeeAddrRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoIeeeAddrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_NODE_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoNodeDescRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoNodeDescRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoNodeDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_POWER_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoPowerDescRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoPowerDescRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoPowerDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_SIMPLE_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoSimpleDescRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoSimpleDescRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoSimpleDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_ACTIVE_EP_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoActiveEpRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoActiveEpRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoActiveEpRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MATCH_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoMatchDescRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoMatchDescRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoMatchDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB:
            /* Note, we're not using zb_ipc_m4_cb_info for this API, so we don't need
             * the callback argument. */
            assert(p_notification->Size == 1);
            if (zdo_match_multi_cb != NULL) {
                struct ZbZdoMatchDescRspT *rsp;

                rsp = (struct ZbZdoMatchDescRspT *)p_notification->Data[0];
                zdo_match_multi_cb(rsp, zdo_match_multi_arg);
                if (rsp->status == ZB_ZDP_STATUS_TIMEOUT) {
                    /* Release the callback */
                    zdo_match_multi_cb = NULL;
                }
            }
            break;

        case MSG_M0TOM4_ZDO_BIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoBindRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoBindRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_UNBIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoBindRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoBindRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_LQI_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoLqiRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoLqiRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoLqiRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_RTG_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoRtgRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoRtgRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoRtgRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_BIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoMgmtBindRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoMgmtBindRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoMgmtBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_LEAVE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoLeaveRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoLeaveRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoLeaveRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoPermitJoinRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoPermitJoinRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoPermitJoinRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZdoNwkUpdateNotifyT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZdoNwkUpdateNotifyT *rsp, void *cbarg))info->callback;
                callback((struct ZbZdoNwkUpdateNotifyT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_FILTER_CB:
        {
            struct zdo_filter_cb_info_t *cb_info;

            assert(p_notification->Size == 3);
            cb_info = (void *)p_notification->Data[2];
            if ((cb_info != NULL) && (cb_info->callback != NULL)) {
                struct ZbZdoNwkUpdateNotifyT *msg;
                uint8_t seqno;
                unsigned int i;
                int (*callback)(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *msg, uint8_t seqno, void *arg);

                for (i = 0; i < ZB_IPC_ZDO_FILTER_CB_LIST_MAX; i++) {
                    /* Find the matching filter callback */
                    if (cb_info != &zdo_filter_cb_list[i]) {
                        continue;
                    }
                    if (cb_info->filter == NULL) {
                        /* Shouldn't get here */
                        break;
                    }
                    msg = (struct ZbZdoNwkUpdateNotifyT *)p_notification->Data[0];
                    seqno = (uint8_t)p_notification->Data[1];
                    callback = (int (*)(struct ZigBeeT *zb, struct ZbZdoNwkUpdateNotifyT *msg, uint8_t seqno, void *arg))cb_info->callback;
                    callback(zb_ipc_globals.zb, msg, seqno, cb_info->arg);
                    break;
                }
            }
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_DATA_IND:
        {
            struct ZbApsdeDataIndT *dataIndPtr;
            void *cb_arg;
            int err;

            assert(p_notification->Size == 2);
            dataIndPtr = (struct ZbApsdeDataIndT *)p_notification->Data[0];
            cb_arg = (void *)p_notification->Data[1];
            err = zcl_cluster_data_ind(dataIndPtr, cb_arg);
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB:
        {
            struct ZbApsdeDataIndT *dataIndPtr;
            void *cb_arg;
            int err;

            assert(p_notification->Size == 2);
            dataIndPtr = (struct ZbApsdeDataIndT *)p_notification->Data[0];
            cb_arg = (void *)p_notification->Data[1];
            err = zcl_cluster_alarm_data_ind(dataIndPtr, cb_arg);
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbApsdeDataConfT *conf, void *arg);

                callback = (void (*)(struct ZbApsdeDataConfT *conf, void *arg))info->callback;
                callback((struct ZbApsdeDataConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_COMMAND_REQ_CB:
        {
            int err = ZB_APS_FILTER_CONTINUE;

            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            /* Note: shouldn't get here if callback was NULL in request, so info should
             * always be non-NULL. */
            if (info != NULL) {
                struct ZbZclCommandRspT *zcl_rsp = (struct ZbZclCommandRspT *)p_notification->Data[0];

                if (info->callback != NULL) {
                    int (*callback)(struct ZbZclCommandRspT *conf, void *arg);

                    callback = (int (*)(struct ZbZclCommandRspT *rsp, void *arg))info->callback;
                    err = callback(zcl_rsp, info->arg);
                }
                if (info->zcl_recv_multi_rsp && (zcl_rsp->status != ZCL_STATUS_TIMEOUT)) {
                    /* Don't free the callback yet */
                    info = NULL;
                }
            }
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_ZCL_READ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZclReadRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZclReadRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZclReadRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_WRITE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZclWriteRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZclWriteRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZclWriteRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbZclDiscoverAttrRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZclDiscoverAttrRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZclDiscoverAttrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB:
            assert(p_notification->Size == 5);
            info = (struct zb_ipc_m4_cb_info_t *)p_notification->Data[4];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT key_status, void *arg);
                uint64_t partnerAddr;

                zb_ipc_m4_memcpy2(&partnerAddr, (void *)&p_notification->Data[0], 8);
                callback = (void (*)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT key_status, void *arg))info->callback;
                callback(partnerAddr, (uint16_t)p_notification->Data[2], (enum ZbZclKeyStatusT)p_notification->Data[3], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_TL_EP_INFO_CB:
        {
            struct ZbTlEpInfoCmd *cmd;
            struct ZbZclAddrInfoT *srcInfo;

            assert(p_notification->Size == 3);
            if (zigbee_m4_tl_callbacks.ep_info_cb == NULL) {
                retval = (uint32_t)ZCL_STATUS_UNSUPP_COMMAND;
                break;
            }
            cmd = (struct ZbTlEpInfoCmd *)p_notification->Data[0];
            srcInfo = (struct ZbZclAddrInfoT *)p_notification->Data[1];
            retval = (uint32_t)zigbee_m4_tl_callbacks.ep_info_cb(zb_ipc_globals.zb, cmd,
                    srcInfo, (void *)p_notification->Data[2]);
            break;
        }

        default:
            status = HAL_ERROR;
            break;
    }

    if (info != NULL) {
        zb_ipc_m4_cb_info_free(info);
    }

    /* Return the retval, if any. */
    p_notification->Data[0] = retval;

    TL_ZIGBEE_SendM4AckToM0Notify();
    return status;
}

HAL_StatusTypeDef
Zigbee_M0RequestProcessing(void)
{
    HAL_StatusTypeDef status = HAL_OK;
    Zigbee_Cmd_Request_t *p_logging = ZIGBEE_Get_M0RequestPayloadBuffer();

    switch (p_logging->ID) {
        case MSG_M0TOM4_ZB_LOGGING:
        {
            const char *log_str;

            assert(p_logging->Size == 1);
            log_str = (const char *)p_logging->Data[0];
            if (zb_ipc_globals.log_cb != NULL) {
                /* We just need to print the raw string. The formatting has already been done. */
                zb_ipc_globals.log_cb(zb_ipc_globals.zb, 0 /* mask is unknown */, NULL,
                    log_str /* fmt */, va_null);
            }
            break;
        }

        /* ZbMalloc */
        case MSG_M0TOM4_ZB_MALLOC:
        {
            void *ptr;
            uint32_t alloc_sz;

            assert(p_logging->Size == 1);
            alloc_sz = (uint32_t)p_logging->Data[0];
#ifndef CONFIG_ZB_M4_MALLOC_DEBUG_SZ
            /* Make room for tracking size at start of memory block */
            alloc_sz += 4U;
#endif
            ptr = malloc(alloc_sz);
            if (ptr != NULL) {
                ptr = zb_malloc_track(ptr, alloc_sz);
            }
            /* Return ptr in second argument */
            p_logging->Data[1] = (uint32_t)ptr;
            break;
        }

        /* ZbFree */
        case MSG_M0TOM4_ZB_FREE:
        {
            void *ptr;

            assert(p_logging->Size == 1);
            ptr = (void *)p_logging->Data[0];
            assert(ptr != NULL);
            ptr = zb_malloc_untrack(ptr);
            free(ptr);
            break;
        }

        default:
            status = HAL_ERROR;
            break;
    }

    TL_ZIGBEE_SendM4AckToM0Request();
    return status;
}

/* ZbMalloc (MSG_M0TOM4_ZB_MALLOC) Debugging */
static void *
zb_malloc_track(void *ptr, unsigned int sz)
{
#ifdef CONFIG_ZB_M4_MALLOC_DEBUG_SZ
    unsigned int i;

    for (i = 0; i < CONFIG_ZB_M4_MALLOC_DEBUG_SZ; i++) {
        if (zb_ipc_globals.zb_malloc_info[i].ptr == NULL) {
            zb_ipc_globals.zb_malloc_info[i].ptr = ptr;
            zb_ipc_globals.zb_malloc_info[i].sz = sz;
            return;
        }
    }
    ZbLogPrintf(zb_ipc_globals.zb, ZB_LOG_MASK_HEAP, __func__, "Warning, can't track allocation (p=%p, sz=%d)", ptr, sz);
    return ptr;

#else
    void *ret;

    *(uint32_t *)ptr = sz;
    ret = ((uint8_t *)ptr) + 4U;
    zb_ipc_globals.zb_alloc_sz += sz;
    return ret;
#endif
}

static void *
zb_malloc_untrack(void *ptr)
{
#ifdef CONFIG_ZB_M4_MALLOC_DEBUG_SZ
    unsigned int i;

    for (i = 0; i < CONFIG_ZB_M4_MALLOC_DEBUG_SZ; i++) {
        if (zb_ipc_globals.zb_malloc_info[i].ptr == ptr) {
            zb_ipc_globals.zb_malloc_info[i].ptr = NULL;
            return;
        }
    }
    ZbLogPrintf(zb_ipc_globals.zb, ZB_LOG_MASK_HEAP, __func__, "Warning, can't find allocation (p=%p)", ptr);
    return ptr;

#else
    uint32_t sz;
    void *ret;

    ret = ((uint8_t *)ptr) - 4U;
    sz = *(uint32_t *)ret;
    zb_ipc_globals.zb_alloc_sz -= sz;
    return ret;
#endif
}

unsigned int
zb_malloc_current_sz(void)
{
#ifdef CONFIG_ZB_M4_MALLOC_DEBUG_SZ
    unsigned int i, alloc_sz = 0U;

    for (i = 0; i < CONFIG_ZB_M4_MALLOC_DEBUG_SZ; i++) {
        if (zb_ipc_globals.zb_malloc_info[i].ptr != NULL) {
            alloc_sz += zb_ipc_globals.zb_malloc_info[i].sz;
        }
    }
    return alloc_sz;
#else
    return zb_ipc_globals.zb_alloc_sz;
#endif
}

/* This is only for ZB_LOG_MASK_ZCL log messages from M4 */
void
ZbLogPrintf(struct ZigBeeT *zb, uint32_t mask, const char *hdr, const char *fmt, ...)
{
    /* ZB_LOG_MASK_ZCL */
    if ((zb_ipc_globals.log_cb != NULL) && ((mask & zb_ipc_globals.log_mask) != 0U)) {
        va_list argptr;

        va_start(argptr, fmt);
        zb_ipc_globals.log_cb(zb, mask, hdr, fmt, argptr);
        va_end(argptr);
    }
}
