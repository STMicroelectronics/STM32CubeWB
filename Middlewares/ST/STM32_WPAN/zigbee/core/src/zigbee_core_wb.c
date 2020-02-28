/**
 ******************************************************************************
 * @file    zigbee_core_wb.c
 * @author  MCD Application Team
 * @brief   Contains STM32WB specificities requested to control the Zigbee stack
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>

#include "zigbee_errors.h"
#include "zigbee_types.h"

#include "zigbee_interface.h" /* zigbee.h, etc */
#include "zcl/zcl.h"
#include "zcl/key/zcl.key.h" /* ZbZclKeWithDevice */

#include "zigbee_core.h"
#include "tl_zigbee_hci.h"

#include "dbg_trace.h"
#include "stm_logging.h"
#include "stm32wbxx_core_interface_def.h"

/* Lock isn't required on this platform */
#define ZbEnterCritical(_zb_)
#define ZbExitCritical(_zb_)

#ifndef ZB_HEAP_MAX_ALLOC
#define ZB_HEAP_MAX_ALLOC                   2000U
#endif

/* ZbTimerAlloc() needs ZbHeapAlloc */
/* EXEGIN - remove this dependancy? */
void * zb_heap_alloc(struct ZigBeeT *zb, size_t sz, const char *funcname, unsigned int linenum);
void zb_heap_free(struct ZigBeeT *zb, void *ptr, const char *funcname, unsigned int linenum);
#if defined(CONFIG_ZB_MEMORY_DEBUG) || defined(CONFIG_ZB_MEMPOOL_HISTORY)
# define ZbHeapAlloc(_zb_, _sz_)             zb_heap_alloc(_zb_, _sz_, __func__, __LINE__)
# define ZbHeapFree(_zb_, _ptr_)             zb_heap_free(_zb_, _ptr_, __func__, __LINE__)
#else
# define ZbHeapAlloc(_zb_, _sz_)             zb_heap_alloc(_zb_, _sz_, "", 0)
# define ZbHeapFree(_zb_, _ptr_)             zb_heap_free(_zb_, _ptr_, "", 0)
#endif

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

struct aps_filter_cb {
    int (*callback)(ZbApsdeDataIndT *data_ind, void *cb_arg);
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

struct zb_ipc_m4_cb_info {
    void *callback;
    void *arg;
    bool zcl_recv_multi_rsp;
};

/* Single static callback for persistent data notifications */
static void (*zb_persist_cb)(struct ZigBeeT *zb, void *cbarg) = NULL;
static void *zb_persist_arg = NULL;

/* Callback for ZbZdoMatchDescMulti. It is static and never freed. It can be overwritten
 * by another call to ZbZdoMatchDescMulti. */
static void (*zdo_match_multi_cb)(ZbZdoMatchDescRspT *ind, void *cb_arg) = NULL;

/* Some globals */
struct zb_ipc_globals {
    struct ZigBeeT *zb;
    /* Stack logging callback */
    uint32_t log_mask;
    bool log_enable;
};
static struct zb_ipc_globals zb_ipc_globals;

struct cli_app;
extern void cli_port_print_msg(struct cli_app *cli_p, const char *msg);

int zcl_cluster_data_ind(ZbApsdeDataIndT *dataIndPtr, void *arg);
int zcl_cluster_alarm_data_ind(ZbApsdeDataIndT *data_ind, void *arg);

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

/* EXEGIN - need to get rid of calling this from the application */
ZbUptimeT
ZbUptime(void)
{
    return HAL_GetTick();
}

void
zb_ipc_m4_stack_logging_config(bool enable)
{
    uint32_t mask = 0U;

    zb_ipc_globals.log_enable = enable;

    /* Now reconfigure the logging on the M0. If log_func == NULL, then we disable the M0
     * sending log messages over the IPCC. Otherwise, it's enabled. We first need to get
     * the current log mask in-use, before re-asserting it back to the M0. */
    ZbGetLogging(zb_ipc_globals.zb, &mask, NULL);
    ZbSetLogging(zb_ipc_globals.zb, mask, NULL);
}

static struct zb_ipc_m4_cb_info *
zb_ipc_m4_cb_info_alloc(void *callback, void *arg)
{
    struct zb_ipc_m4_cb_info *info;

    info = malloc(sizeof(struct zb_ipc_m4_cb_info));
    if (info != NULL) {
        memset(info, 0, sizeof(struct zb_ipc_m4_cb_info));
        info->callback = callback;
        info->arg = arg;
    }
    return info;
}

static void
zb_ipc_m4_cb_info_free(struct zb_ipc_m4_cb_info *info)
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
    zb_ipc_m4_memcpy2(&retval, &ipcc_req->Data[0], 4);
    return retval;
}

bool
wpan_set_uint32(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint32_t value, uint16_t index)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    (void)publicPtr;
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_WPAN_SET_UINT32;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)attrId;
    ipcc_req->Data[1] = (uint32_t)value;
    ipcc_req->Data[2] = (uint32_t)index;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
wpan_set_uint16(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint16_t value, uint16_t index)
{
    return wpan_set_uint32(publicPtr, attrId, (uint32_t)value, index);
}

bool
wpan_set_uint8(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint8_t value, uint16_t index)
{
    return wpan_set_uint32(publicPtr, attrId, (uint32_t)value, index);
}

bool
wpan_get_uint32(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint32_t *value, uint16_t index)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    (void)publicPtr;
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_WPAN_GET_UINT32;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)attrId;
    ipcc_req->Data[1] = (uint32_t)value;
    ipcc_req->Data[2] = (uint32_t)index;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
wpan_get_uint64(struct WpanPublicT *publicPtr, enum mcp_attr_id attrId, uint64_t *value, uint16_t index)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    (void)publicPtr;
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_WPAN_GET_UINT64;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)attrId;
    ipcc_req->Data[1] = (uint32_t)value;
    ipcc_req->Data[2] = (uint32_t)index;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

unsigned int
ZbHeapMaxAlloc(void)
{
    return ZB_HEAP_MAX_ALLOC;
}

struct ZigBeeT *
ZbInit(uint64_t extAddr, ZbInitTblSizesT *tblSizes, ZbInitSetLoggingT *setLogging)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    if (zb_ipc_globals.zb != NULL) {
        return NULL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_INIT;
    ipcc_req->Size = 4;
    zb_ipc_m4_memcpy2(&ipcc_req->Data[0], &extAddr, 8);
    ipcc_req->Data[2] = (uint32_t)tblSizes;
    ipcc_req->Data[3] = (uint32_t)setLogging;
    ZIGBEE_CmdTransfer();
    zb_ipc_globals.zb = (struct ZigBeeT *)zb_ipc_m4_get_retval();
    if (setLogging != NULL) {
        /* Save the log mask */
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
    zb_ipc_globals.zb = NULL;
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
    ipcc_req->Data[1] = zb_ipc_globals.log_enable ? 1U : 0U;
    ZIGBEE_CmdTransfer();
    /* Save the log mask */
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
        *func = NULL;
    }
}

uint64_t
ZbExtendedAddress(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint64_t ext_addr;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_EXTADDR_GET;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    ipcc_req = ZIGBEE_Get_OTCmdRspPayloadBuffer();
    assert(ipcc_req->Size == 2);
    zb_ipc_m4_memcpy2(&ext_addr, &ipcc_req->Data, 8);
    return ext_addr;
}

uint16_t
ZbShortAddress(struct ZigBeeT *zb)
{
    uint16_t nwkAddr;

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
    zb_ipc_m4_memcpy2(&ipcc_req->Data[0], &extAddr, 8);
    ZIGBEE_CmdTransfer();
}

uint8_t
WpanGetNumChannelsFromMask(uint32_t mask, uint8_t *first_channel)
{
    uint8_t i, num_channels = 0;

    if (first_channel) {
        *first_channel = 0xff;
    }
    for (i = 0; i < WPAN_PAGE_CHANNELS_MAX; i++) {
        if (((1 << i) & mask)) {
            num_channels++;
            if (first_channel && (*first_channel == 0xff)) {
                *first_channel = i;
            }
        }
    }
    return num_channels;
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
ZbStartupCbkeConfig(struct ZigBeeT *zb, struct ZbStartupCbkeT *cbke_config)
{
    /* TODO - may be required for SE certification */
    return ZB_APS_STATUS_NOT_SUPPORTED;
}

enum ZbStatusCodeT
ZbStartup(struct ZigBeeT *zb, struct ZbStartupT *configPtr, void (*callback)(enum ZbStatusCodeT status, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)configPtr;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_STARTUP_CB handler */
}

enum ZbStatusCodeT
ZbStartupRejoin(struct ZigBeeT *zb, void (*callback)(ZbNlmeJoinConfT *conf, void *arg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

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
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_STARTUP_REJOIN_CB handler */
}

enum ZbStatusCodeT
ZbStartupPersist(struct ZigBeeT *zb, const void *pdata, unsigned int plen, struct ZbStartupCbkeT *cbke_config)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_PERSIST;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)pdata;
    ipcc_req->Data[1] = (uint32_t)plen;
    ipcc_req->Data[2] = (uint32_t)cbke_config;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
}

enum ZbStatusCodeT
ZbStartupFindBindStart(struct ZigBeeT *zb, void (*callback)(enum ZbStatusCodeT status, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;
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

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_TOUCHLINK_TARGET_STOP;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
}

bool
ZbStartupTcsoStart(struct ZigBeeT *zb, void (*callback)(enum ZbTcsoStatusT status, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;
    bool retval;

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
    retval = zb_ipc_m4_get_retval() != 0U ? true : false;
    if (!retval) {
        zb_ipc_m4_cb_info_free(info);
    }
    return retval;
    /* Followed up in MSG_M0TOM4_STARTUP_TCSO_CB */
}

void
ZbStartupTcsoAbort(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_TCSO_ABORT;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
}

enum ZbStatusCodeT
ZbTrustCenterRejoin(struct ZigBeeT *zb, void (*callback)(enum ZbStatusCodeT status, void *arg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

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
    return zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_STARTUP_TC_REJOIN_CB handler */
}

bool
ZbPersistNotifyRegister(struct ZigBeeT *zb, void (*callback)(struct ZigBeeT *zb, void *cbarg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    bool result;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_PERSIST_ENABLE;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    result = zb_ipc_m4_get_retval() != 0U ? true : false;
    if (result) {
        zb_persist_arg = cbarg;
        zb_persist_cb = callback;
    }
    else {
        zb_persist_arg = NULL;
        zb_persist_cb = NULL;
    }
    return result;
    /* Followed up by M0 calls to MSG_M0TOM4_PERSIST_CB */
}

unsigned int
ZbPersistGet(struct ZigBeeT *zb, uint8_t *buf, unsigned int maxlen)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_PERSIST_GET;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)buf;
    ipcc_req->Data[1] = (uint32_t)maxlen;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval();
}

enum ZbStatusCodeT
ZbLeaveReq(struct ZigBeeT *zb, void (*callback)(struct ZbNlmeLeaveConfT *conf, void *arg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

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
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZB_LEAVE_CB handler */
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
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
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
    ZbBdbGetReqT bdbGetReq;
    ZbBdbGetConfT bdbGetConf;

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
    return bdbGetConf.status;
}

enum ZbStatusCodeT
ZbBdbSetIndex(struct ZigBeeT *zb, enum ZbBdbAttrIdT attrId, const void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    ZbBdbSetReqT bdbSetReq;
    ZbBdbSetConfT bdbSetConf;

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
    return bdbSetConf.status;
}

enum ZbBdbCommissioningStatusT
ZbBdbGetEndpointStatus(struct ZigBeeT *zb, uint8_t endpoint)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_BDB_GET_EP_STATUS;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ZIGBEE_CmdTransfer();
    /* Get the status code */
    retval = zb_ipc_m4_get_retval();
    return (enum ZbBdbCommissioningStatusT)retval;
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
}

/******************************************************************************
 * APS
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbApsdeDataReqCallback(struct ZigBeeT *zb, ZbApsdeDataReqT *req, void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;
    struct zb_ipc_m4_cb_info *info;

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
    retval = zb_ipc_m4_get_retval();
    if (retval != 0x00) {
        zb_ipc_m4_cb_info_free(info);
    }
    return (enum ZbStatusCodeT)retval;
    /* If success, followed up in MSG_M0TOM4_APSDE_DATA_REQ_CB handler */
}

void
ZbApsmeAddEndpoint(struct ZigBeeT *zb, ZbApsmeAddEndpointReqT *r, ZbApsmeAddEndpointConfT *c)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_ENDPOINT_ADD;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)r;
    ipcc_req->Data[1] = (uint32_t)c;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeRemoveEndpoint(struct ZigBeeT *zb, ZbApsmeRemoveEndpointReqT *r, ZbApsmeRemoveEndpointConfT *c)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_ENDPOINT_DEL;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)r;
    ipcc_req->Data[1] = (uint32_t)c;
    ZIGBEE_CmdTransfer();
}

struct ZbApsFilterT *
ZbApsFilterEndpointAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId,
    int (*callback)(ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb *aps_filter_cb;
    struct ZbApsFilterT *filter;

    /* EXEGIN - track these allocations so they can be freed later? */
    aps_filter_cb = malloc(sizeof(struct aps_filter_cb));
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
    filter = (struct ZbApsFilterT *)zb_ipc_m4_get_retval();
    if (filter == NULL) {
        free(aps_filter_cb);
    }
    /* Callbacks go to MSG_M0TOM4_APS_FILTER_ENDPOINT_CB handler */
    return filter;
}

bool
ZbApsmeEndpointConfigNoMatchCallback(struct ZigBeeT *zb, uint8_t endpoint,
    int (*callback)(ZbApsdeDataIndT *ind, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb *aps_filter_cb;
    bool retval;

    /* EXEGIN - track these allocations so they can be freed later? */
    aps_filter_cb = malloc(sizeof(struct aps_filter_cb));
    if (aps_filter_cb == NULL) {
        return false;
    }
    aps_filter_cb->callback = callback;
    aps_filter_cb->cb_arg = arg;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_FILTER_ENDPOINT_NOMATCH_ADD;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)aps_filter_cb;
    ZIGBEE_CmdTransfer();
    retval = zb_ipc_m4_get_retval() != 0U ? true : false;
    if (!retval) {
        free(aps_filter_cb);
    }
    /* Callbacks go to MSG_M0TOM4_APS_FILTER_ENDPOINT_CB handler */
    return retval;
}

struct ZbApsFilterT *
ZbApsFilterClusterAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t clusterId, uint16_t profileId,
    int (*callback)(ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb *aps_filter_cb;
    struct ZbApsFilterT *filter;

    /* EXEGIN - track these allocations so they can be freed later. */
    aps_filter_cb = malloc(sizeof(struct aps_filter_cb));
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
    filter = (struct ZbApsFilterT *)zb_ipc_m4_get_retval();
    if (filter == NULL) {
        free(aps_filter_cb);
    }
    /* Callbacks go to MSG_M0TOM4_APS_FILTER_CLUSTER_CB handler */
    return filter;
}

bool
ZbApsmeEndpointClusterListAppend(struct ZigBeeT *zb, uint8_t endpoint, uint16_t cluster_id, bool is_input)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_EP_CLUSTER_ID_ADD;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)cluster_id;
    ipcc_req->Data[2] = (uint32_t)is_input;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbApsEndpointExists(struct ZigBeeT *zb, uint8_t endpoint)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_EP_EXISTS;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

uint16_t
ZbApsEndpointProfile(struct ZigBeeT *zb, uint8_t endpoint)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_EP_GET_PROFILE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ZIGBEE_CmdTransfer();
    return (uint16_t)zb_ipc_m4_get_retval();
}

bool
ZbApsAddrIsBcast(struct ZbApsAddrT *addr)
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
ZbApsAddrIsLocal(struct ZigBeeT *zb, struct ZbApsAddrT *addr)
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

enum ZbStatusCodeT
ZbApsGetIndex(struct ZigBeeT *zb, enum ZbApsmeIbAttrIdT attrId, void *attrPtr, unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    ZbApsmeGetReqT apsmeGetReq;
    ZbApsmeGetConfT apsmeGetConf;

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
    return apsmeGetConf.status;
}

enum ZbStatusCodeT
ZbApsSetIndex(struct ZigBeeT *zb, enum ZbApsmeIbAttrIdT attrId, const void *attrPtr, unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    ZbApsmeSetReqT apsmeSetReq;
    ZbApsmeSetConfT apsmeSetConf;

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
    return apsmeSetConf.status;
}

void
ZbApsmeAddGroupReq(struct ZigBeeT *zb, ZbApsmeAddGroupReqT *r, ZbApsmeAddGroupConfT *c)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_ADD_GROUP;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)r;
    ipcc_req->Data[1] = (uint32_t)c;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeRemoveGroupReq(struct ZigBeeT *zb, ZbApsmeRemoveGroupReqT *r, ZbApsmeRemoveGroupConfT *c)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_REMOVE_GROUP;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)r;
    ipcc_req->Data[1] = (uint32_t)c;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeRemoveAllGroupsReq(struct ZigBeeT *zb, ZbApsmeRemoveAllGroupsReqT *r, ZbApsmeRemoveAllGroupsConfT *c)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_REMOVE_ALL_GROUPS;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)r;
    ipcc_req->Data[1] = (uint32_t)c;
    ZIGBEE_CmdTransfer();
}

bool
ZbApsGroupIsMember(struct ZigBeeT *zb, uint16_t groupAddr, uint8_t endpoint)
{
    ZbApsmeGroupT group;
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
    ZbApsmeGroupT group;
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
    ZbApsmeGroupT group;
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

void
ZbApsmeBindReq(struct ZigBeeT *zb, ZbApsmeBindReqT *bindReqPtr, ZbApsmeBindConfT *bindConfPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_BIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)bindReqPtr;
    ipcc_req->Data[1] = (uint32_t)bindConfPtr;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeUnbindReq(struct ZigBeeT *zb, ZbApsmeUnbindReqT *unbindReqPtr, ZbApsmeUnbindConfT *unbindConfPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_UNBIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)unbindReqPtr;
    ipcc_req->Data[1] = (uint32_t)unbindConfPtr;
    ZIGBEE_CmdTransfer();
}

void
ZbApsUnbindAllReq(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_UNBIND_ALL;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeTransportKeyReq(struct ZigBeeT *zb, ZbApsmeTransportKeyReqT *req)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_TRANSPORT_KEY;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeRemoveDeviceReq(struct ZigBeeT *zb, ZbApsmeRemoveDeviceReqT *req)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_REMOVE_DEVICE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
}

enum ZbStatusCodeT
ZbApsmeRequestKeyReq(struct ZigBeeT *zb, ZbApsmeRequestKeyReqT *req)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_REQUEST_KEY;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
}

void
ZbApsmeSwitchKeyReq(struct ZigBeeT *zb, ZbApsmeSwitchKeyReqT *req)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_SWITCH_KEY;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeAddKeyReq(struct ZigBeeT *zb, ZbApsmeAddKeyReqT *req, ZbApsmeAddKeyConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_ADD_KEY;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeGetKeyReq(struct ZigBeeT *zb, ZbApsmeGetKeyReqT *req, ZbApsmeGetKeyConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_GET_KEY;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

void
ZbApsmeRemoveKeyReq(struct ZigBeeT *zb, ZbApsmeRemoveKeyReqT *req, ZbApsmeRemoveKeyConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APSME_REMOVE_KEY;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

/******************************************************************************
* Zigbee Message Filters
******************************************************************************
*/
struct zb_msg_filter_cb_info {
    struct ZbMsgFilterT *filter;
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg);
    void *arg;
};

#define ZB_IPC_MSG_FILTER_CB_LIST_MAX               4
static struct zb_msg_filter_cb_info zb_msg_filter_cb_list[ZB_IPC_MSG_FILTER_CB_LIST_MAX];

struct ZbMsgFilterT *
ZbMsgFilterRegister(struct ZigBeeT *zb, uint32_t mask, uint8_t prio,
    enum zb_msg_filter_rc (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbMsgFilterT *filter;
    struct zb_msg_filter_cb_info *cb_info;
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
    struct zb_msg_filter_cb_info *cb_info;
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

    /* Free the timer struct on the M4 */
    ZbHeapFree(NULL, timer);
}

bool
ZbTimerRunning(struct ZbTimerT *timer)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TIMER_RUNNING;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)timer->m0_timer;
    ZIGBEE_CmdTransfer();
    retval = zb_ipc_m4_get_retval();
    return retval != 0 ? true : false;
}

unsigned int
ZbTimerRemaining(struct ZbTimerT *timer)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TIMER_REMAINING;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)timer->m0_timer;
    ZIGBEE_CmdTransfer();
    return (unsigned int)zb_ipc_m4_get_retval();
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
    ZbNlmeGetReqT nlmeGetReq;
    ZbNlmeGetConfT nlmeGetConf;

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
    return nlmeGetConf.status;
}

enum ZbStatusCodeT
ZbNwkSetIndex(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr,
    unsigned int attrSz, unsigned int attrIndex)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    ZbNlmeSetReqT nlmeSetReq;
    ZbNlmeSetConfT nlmeSetConf;

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
    return nlmeSetConf.status;
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
ZbNwkGetParentExtAddr(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint64_t ext_addr;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_GET_PARENT_EXT;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    /* Parse return value */
    ipcc_req = ZIGBEE_Get_OTCmdRspPayloadBuffer();
    assert(ipcc_req->Size == 2);
    zb_ipc_m4_memcpy2(&ext_addr, &ipcc_req->Data[0], 8);
    return ext_addr;
}

uint16_t
ZbNwkGetParentShortAddr(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_GET_PARENT_NWK;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return (uint16_t)zb_ipc_m4_get_retval();
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
    zb_ipc_m4_memcpy2(&ext_addr, &ipcc_req->Data[0], 8);
    return ext_addr;
}

uint16_t
ZbNwkAddrLookupNwk(struct ZigBeeT *zb, uint64_t extAddr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_ADDR_LOOKUP_NWK;
    /* Extended address is split across two args */
    ipcc_req->Size = 2;
    zb_ipc_m4_memcpy2(ipcc_req->Data, &extAddr, 8);
    ZIGBEE_CmdTransfer();
    return (uint16_t)zb_ipc_m4_get_retval();
}

bool
ZbNwkAddrIsChildExt(struct ZigBeeT *zb, uint64_t extAddr, uint16_t *nwkAddrPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_ADDR_IS_CHILD_EXT;
    /* Extended address is split across two args */
    ipcc_req->Size = 3;
    zb_ipc_m4_memcpy2(ipcc_req->Data, &extAddr, 8);
    ipcc_req->Data[2] = (uint32_t)nwkAddrPtr;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbNwkAddrIsChildNwk(struct ZigBeeT *zb, uint16_t nwkAddr, uint64_t *extAddrPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_ADDR_IS_CHILD_NWK;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)nwkAddr;
    ipcc_req->Data[1] = (uint32_t)extAddrPtr;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbNwkGetSecMaterial(struct ZigBeeT *zb, uint8_t keySeqno, ZbNwkSecMaterialT *material)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_GET_SEC_MATERIAL;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)keySeqno;
    ipcc_req->Data[1] = (uint32_t)material;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

enum ZbStatusCodeT
ZbNlmeNetDiscReq(struct ZigBeeT *zb, ZbNlmeNetDiscReqT *req,
    void (*callback)(ZbNlmeNetDiscConfT *conf, void *cbarg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_NET_DISC;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_NLME_NET_DISC_CB handler */
}

enum ZbStatusCodeT
ZbNlmeLeaveReq(struct ZigBeeT *zb, struct ZbNlmeLeaveReqT *req,
    void (*callback)(struct ZbNlmeLeaveConfT *leaveConfPtr, void *arg), void *cbarg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, cbarg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_LEAVE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_NLME_LEAVE_CB handler */
}

void
ZbNlmePermitJoinReq(struct ZigBeeT *zb, ZbNlmePermitJoinReqT *req, ZbNlmePermitJoinConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_PERMIT_JOIN;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

void
ZbNlmeResetReq(struct ZigBeeT *zb, ZbNlmeResetReqT *req, ZbNlmeResetConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_RESET;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

enum ZbStatusCodeT
ZbNlmeSyncReq(struct ZigBeeT *zb, ZbNlmeSyncReqT *req,
    void (*callback)(ZbNlmeSyncConfT *syncConfPtr, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_SYNC;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_NLME_SYNC_CB handler */
}

enum ZbStatusCodeT
ZbNlmeRouteDiscReq(struct ZigBeeT *zb, ZbNlmeRouteDiscReqT *req,
    void (*callback)(ZbNlmeRouteDiscConfT *discConf, void *cbarg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_ROUTE_DISC;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_NLME_ROUTE_DISC_CB handler */
}

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
}

void
ZbNlmeDirectJoinReq(struct ZigBeeT *zb, ZbNlmeDirectJoinReqT *req, ZbNlmeDirectJoinConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_DIRECT_JOIN;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

void
ZbNlmeSetInterface(struct ZigBeeT *zb, ZbNlmeSetInterfaceReqT *req, ZbNlmeSetInterfaceConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_SET_INTERFACE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

void
ZbNlmeGetInterface(struct ZigBeeT *zb, ZbNlmeGetInterfaceReqT *req, ZbNlmeGetInterfaceConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NLME_GET_INTERFACE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

bool
ZbNwkIfSetTxPower(struct ZigBeeT *zb, const char *name, int8_t tx_power)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_IFC_SET_TX_POWER;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)name;
    ipcc_req->Data[1] = (uint32_t)tx_power;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbNwkIfGetTxPower(struct ZigBeeT *zb, const char *name, int8_t *tx_power)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_IFC_GET_TX_POWER;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)name;
    ipcc_req->Data[1] = (uint32_t)tx_power;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbNwkIfSetDsn(struct ZigBeeT *zb, const char *name, uint8_t macDsn)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_IFC_SET_DSN;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)name;
    ipcc_req->Data[1] = (uint32_t)macDsn;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbNwkSetFrameCounter(struct ZigBeeT *zb, uint8_t keySeqno, uint64_t srcAddr, uint32_t newFrameCount)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_SET_FRAME_COUNTER;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)keySeqno;
    zb_ipc_m4_memcpy2(&ipcc_req->Data[1], &srcAddr, 8);
    ipcc_req->Data[3] = (uint32_t)newFrameCount;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbNwkFastPollRequest(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_FAST_POLL_REQUEST;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbNwkFastPollRelease(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_FAST_POLL_RELEASE;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

unsigned int
ZbNwkFastPollResourceCount(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_FAST_POLL_RESOURCE;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval();
}

/* Only required by zbcli. And only helps with automated verification of test cases.
 * Do not use for certification. */
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
}

/* Required for zigbee certification only. May not be required for a DUT though. */
bool
nwk_status_send(struct ZigBeeT *zb, uint16_t dstAddr, enum ZbNwkAddrModeT dstAddrMode,
    uint16_t statusAddr, enum ZbNwkNetworkStatusCodeT statusCode)
{
    /* TODO? */
    return false;
}

/* Required for zigbee certification only. May not be required for a DUT though. */
bool
nwk_rreq_by_route_is_active(struct ZigBeeT *zb, ZbNwkRouteEntryT *route)
{
    /* TODO? */
    return false;
}

bool
ZbNwkSendEdkaReq(struct ZigBeeT *zb)
{
    /* TODO - may be required for a true sleepy device, where application wants
     * full control of when device polls for data. */
    return false;
}

/******************************************************************************
 * ZDO Utility
 ******************************************************************************
 */
struct zdo_device_annce_cb_info {
    struct ZbZdoDeviceAnnceFilterT *handle;
    void (*callback)(struct ZigBeeT *zb, ZbZdoDeviceAnnceT *annce, uint8_t seqno);
};

#define ZB_IPC_ZDO_DEVICE_ANNCE_CB_LIST_MAX         4
static struct zdo_device_annce_cb_info zdo_device_annce_cb_list[ZB_IPC_ZDO_DEVICE_ANNCE_CB_LIST_MAX];

struct ZbZdoDeviceAnnceFilterT *
ZbZdoDeviceAnnceFilterRegister(struct ZigBeeT *zb,
    void (*callback)(struct ZigBeeT *zb, ZbZdoDeviceAnnceT *annce, uint8_t seqno))
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct ZbZdoDeviceAnnceFilterT *handle;
    struct zdo_device_annce_cb_info *cb_info;
    unsigned int i;

    if (callback == NULL) {
        return NULL;
    }
    for (i = 0; i < ZB_IPC_ZDO_DEVICE_ANNCE_CB_LIST_MAX; i++) {
        cb_info = &zdo_device_annce_cb_list[i];
        if (cb_info->handle == NULL) {
            break;
        }
    }
    if (i == ZB_IPC_ZDO_DEVICE_ANNCE_CB_LIST_MAX) {
        return NULL;
    }

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_REG;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    handle = (struct ZbZdoDeviceAnnceFilterT *)zb_ipc_m4_get_retval();
    if (handle != NULL) {
        cb_info->handle = handle;
        cb_info->callback = callback;
    }
    return handle;
    /* Followed up in MSG_M0TOM4_ZDO_DEVICE_ANNCE_FILTER_CB handler */
}

void
ZbZdoDeviceAnnceFilterRemove(struct ZigBeeT *zb, struct ZbZdoDeviceAnnceFilterT *handle)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zdo_device_annce_cb_info *cb_info;
    unsigned int i;

    if (handle == NULL) {
        return;
    }
    for (i = 0; i < ZB_IPC_ZDO_DEVICE_ANNCE_CB_LIST_MAX; i++) {
        cb_info = &zdo_device_annce_cb_list[i];
        if (cb_info->handle == handle) {
            break;
        }
    }
    if (i == ZB_IPC_ZDO_DEVICE_ANNCE_CB_LIST_MAX) {
        return;
    }

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_DEL;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)handle;
    ZIGBEE_CmdTransfer();
    cb_info->handle = NULL;
}

/******************************************************************************
 * ZDO
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbZdoNwkAddrReq(struct ZigBeeT *zb, ZbZdoNwkAddrReqT *req,
    void (*callback)(ZbZdoNwkAddrRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_NWK_ADDR;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_NWK_ADDR_CB handler */
}

enum ZbStatusCodeT
ZbZdoIeeeAddrReq(struct ZigBeeT *zb, ZbZdoIeeeAddrReqT *req,
    void (*callback)(ZbZdoIeeeAddrRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_IEEE_ADDR;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_IEEE_ADDR_CB handler */
}

enum ZbStatusCodeT
ZbZdoNodeDescReq(struct ZigBeeT *zb, ZbZdoNodeDescReqT *req,
    void (*callback)(ZbZdoNodeDescRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_NODE_DESC;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_NODE_DESC_CB handler */
}

enum ZbStatusCodeT
ZbZdoPowerDescReq(struct ZigBeeT *zb, ZbZdoPowerDescReqT *req,
    void (*callback)(ZbZdoPowerDescRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_POWER_DESC;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_POWER_DESC_CB handler */
}

enum ZbStatusCodeT
ZbZdoSimpleDescReq(struct ZigBeeT *zb, ZbZdoSimpleDescReqT *req,
    void (*callback)(ZbZdoSimpleDescRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_SIMPLE_DESC;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_SIMPLE_DESC_CB handler */
}

enum ZbStatusCodeT
ZbZdoActiveEpReq(struct ZigBeeT *zb, ZbZdoActiveEpReqT *req,
    void (*callback)(ZbZdoActiveEpRspT *rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_ACTIVE_EP;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_ACTIVE_EP_CB handler */
}

enum ZbStatusCodeT
ZbZdoMatchDescReq(struct ZigBeeT *zb, ZbZdoMatchDescReqT *req,
    void (*callback)(ZbZdoMatchDescRspT *ind, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MATCH_DESC;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_MATCH_DESC_CB handler */
}

enum ZbStatusCodeT
ZbZdoMatchDescMulti(struct ZigBeeT *zb, ZbZdoMatchDescReqT *req,
    void (*callback)(ZbZdoMatchDescRspT *ind, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZbStatusCodeT status;

    /* The callback must be static so we can receive multiple responses. We don't check here if
     * we're overwriting an existing callback, so application cannot effecticely perform more
     * than one of these at a time. */

    /* Check if a request is already active */
    if (zdo_match_multi_cb != NULL) {
        return ZB_ZDP_STATUS_TABLE_FULL;
    }
    zdo_match_multi_cb = callback;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MATCH_DESC_MULTI;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
    status = (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    if (status != ZB_STATUS_SUCCESS) {
        zdo_match_multi_cb = NULL;
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB handler */
}

void
ZbZdoDeviceAnnce(struct ZigBeeT *zb, ZbZdoDeviceAnnceT *deviceAnncePtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_DEVICE_ANNCE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)deviceAnncePtr;
    ZIGBEE_CmdTransfer();
}

void
ZbZdoDeviceAnnceAlias(struct ZigBeeT *zb, ZbZdoDeviceAnnceT *deviceAnncePtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_DEVICE_ANNCE_ALIAS;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)deviceAnncePtr;
    ZIGBEE_CmdTransfer();
}

enum ZbStatusCodeT
ZbZdoBindReq(struct ZigBeeT *zb, ZbZdoBindReqT *req,
    void (*callback)(ZbZdoBindRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_BIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_BIND_CB handler */
}

enum ZbStatusCodeT
ZbZdoUnbindReq(struct ZigBeeT *zb, ZbZdoBindReqT *req,
    void (*callback)(ZbZdoBindRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_UNBIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_UNBIND_CB handler */
}

enum ZbStatusCodeT
ZbZdoLqiReq(struct ZigBeeT *zb, ZbZdoLqiReqT *req, void (*callback)(ZbZdoLqiRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MGMT_LQI;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_MGMT_LQI_CB handler */
}

enum ZbStatusCodeT
ZbZdoMgmtBindReq(struct ZigBeeT *zb, ZbZdoMgmtBindReqT *req,
    void (*callback)(ZbZdoMgmtBindRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MGMT_BIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_MGMT_BIND_CB handler */
}

enum ZbStatusCodeT
ZbZdoLeaveReq(struct ZigBeeT *zb, ZbZdoLeaveReqT *req,
    void (*callback)(ZbZdoLeaveRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MGMT_LEAVE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_MGMT_LEAVE_CB handler */
}

enum ZbStatusCodeT
ZbZdoPermitJoinReq(struct ZigBeeT *zb, ZbZdoPermitJoinReqT *req,
    void (*callback)(ZbZdoPermitJoinRspT *rsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MGMT_PERMIT_JOIN;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB handler */
}

enum ZbStatusCodeT
ZbZdoNwkUpdateReq(struct ZigBeeT *zb, ZbZdoNwkUpdateReqT *req,
    void (*callback)(ZbZdoNwkUpdateNotifyT *ind, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB handler */
}

/******************************************************************************
 * ZCL
 ******************************************************************************
 */

bool
ZbZclDeviceLogCheckAllow(struct ZigBeeT *zb, ZbApsdeDataIndT *dataIndPtr, struct ZbZclHeaderT *zclHdrPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_DEVICE_LOG_CHECK;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)dataIndPtr;
    ipcc_req->Data[1] = (uint32_t)zclHdrPtr;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
}

bool
ZbZclBasicPostAlarm(struct ZigBeeT *zb, uint8_t endpoint, uint8_t alarm_code)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_BASIC_SERVER_ALARM_POST;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)alarm_code;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
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
}

enum ZclStatusCodeT
ZbZclBasicWriteDirect(struct ZigBeeT *zb, uint8_t endpoint, uint16_t attributeId, const void *ptr, unsigned int len)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_BASIC_SERVER_LOCAL_WRITE;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)attributeId;
    ipcc_req->Data[2] = (uint32_t)ptr;
    ipcc_req->Data[3] = (uint32_t)len;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
}

bool
ZbZclDiagnosticsServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId, enum ZbStatusCodeT minSecurity)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_DIAG_SERVER_ALLOC;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)endpoint;
    ipcc_req->Data[1] = (uint32_t)profileId;
    ipcc_req->Data[2] = (uint32_t)minSecurity;
    ZIGBEE_CmdTransfer();
    return (bool)zb_ipc_m4_get_retval();
}

void
ZbZclAddEndpoint(struct ZigBeeT *zb, ZbApsmeAddEndpointReqT *addReqPtr, ZbApsmeAddEndpointConfT *addConfPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_ENDPOINT_ADD;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)addReqPtr;
    ipcc_req->Data[1] = (uint32_t)addConfPtr;
    ZIGBEE_CmdTransfer();
}

void
ZbZclRemoveEndpoint(struct ZigBeeT *zb, ZbApsmeRemoveEndpointReqT *req, ZbApsmeRemoveEndpointConfT *conf)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_ENDPOINT_DEL;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)conf;
    ZIGBEE_CmdTransfer();
}

enum ZclStatusCodeT
ZbZclReadReq(struct ZbZclClusterT *clusterPtr, ZbZclReadReqT *req,
    void (*callback)(const ZbZclReadRspT *readRsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZCL_STATUS_INSUFFICIENT_SPACE;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_READ_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZCL_READ_CB handler */
}

enum ZclStatusCodeT
ZbZclWriteReq(struct ZbZclClusterT *clusterPtr, ZbZclWriteReqT *req,
    void (*callback)(const ZbZclWriteRspT *writeResp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
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
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZCL_WRITE_CB handler */
}

enum ZclStatusCodeT
ZbZclDiscoverAttrReq(struct ZbZclClusterT *clusterPtr, ZbZclDiscoverAttrReqT *req,
    void (*callback)(const ZbZclDiscoverAttrRspT *discRsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZCL_STATUS_INSUFFICIENT_SPACE;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_DISCOVER_ATTR_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB handler */
}

uint8_t
ZbZclGetNextSeqnum(void)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_GET_SEQNUM;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return (uint8_t)zb_ipc_m4_get_retval();
}

enum ZclStatusCodeT
ZbZclCommandReq(struct ZigBeeT *zb, ZbZclCommandReqT *zclReq,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info = NULL;

    if (callback != NULL) {
        info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
        if (info == NULL) {
            return ZB_STATUS_ALLOC_FAIL;
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
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZCL_COMMAND_REQ_CB handler if callback != NULL */
}

enum ZclStatusCodeT
ZbZclCommandNoResp(struct ZigBeeT *zb, ZbZclCommandReqT *req,
    void (*callback)(ZbApsdeDataConfT *confPtr, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        return ZB_STATUS_ALLOC_FAIL;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_COMMAND_NO_RSP;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZCL_COMMAND_NO_RSP_CB handler */
}

void
ZbZclSendDefaultResponse(struct ZbZclClusterT *clusterPtr, ZbApsdeDataIndT *dataIndPtr,
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
}

enum ZclStatusCodeT
ZbZclClusterCommandReq(struct ZbZclClusterT *clusterPtr, struct ZbZclClusterCommandReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info = NULL;

    if (callback != NULL) {
        info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
        if (info == NULL) {
            return ZB_STATUS_ALLOC_FAIL;
        }
        if (ZbApsAddrIsBcast(&req->dst)) {
            info->zcl_recv_multi_rsp = true; /* callback only freed on ZCL_STATUS_TIMEOUT */
        }
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_CMD_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZCL_CLUSTER_CMD_REQ_CB handler if callback != NULL */
}

enum ZclStatusCodeT
ZbZclClusterCommandRsp(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo,
    uint8_t cmdId, ZbApsBufT *payloads, uint8_t numPayloads)
{
    Zigbee_Cmd_Request_t *ipcc_req;

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
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
}

enum ZclStatusCodeT
ZbZclClusterCommandRspWithCb(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo, uint8_t cmdId,
    ZbApsBufT *payloads, uint8_t numPayloads, void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info = NULL;
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
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB handler */
}

enum ZclStatusCodeT
ZbZclSendClusterStatusResponse(struct ZbZclClusterT *clusterPtr, ZbApsdeDataIndT *dataIndPtr,
    struct ZbZclHeaderT *zclHdrPtr, uint8_t cmdId, uint8_t *zclPayload, uint8_t zclPaylen)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_STATUS;
    ipcc_req->Size = 6;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)dataIndPtr;
    ipcc_req->Data[2] = (uint32_t)zclHdrPtr;
    ipcc_req->Data[3] = (uint32_t)cmdId;
    ipcc_req->Data[4] = (uint32_t)zclPayload;
    ipcc_req->Data[5] = (uint32_t)zclPaylen;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
}

bool
ZbZclClusterEndpointRegister(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_EP_REGISTER;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    retval = zb_ipc_m4_get_retval();
    return retval != 0 ? true : false;
}

bool
ZbZclClusterEndpointRemove(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_EP_REMOVE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    retval = zb_ipc_m4_get_retval();
    return retval != 0 ? true : false;
}

enum ZclStatusCodeT
ZbZclClusterBind(struct ZbZclClusterT *clusterPtr, uint8_t endpoint, uint16_t profileId, enum ZbZclDirectionT direction)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_BIND;
    ipcc_req->Size = 4;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)endpoint;
    ipcc_req->Data[2] = (uint32_t)profileId;
    ipcc_req->Data[3] = (uint32_t)direction;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
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
}

enum ZclStatusCodeT
ZbZclClusterLoopbackBind(struct ZbZclClusterT *clusterPtr, struct ZbApsFilterT *filter)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_LOOPBACK_BIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)filter;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    /* Data indication callbacks go to MSG_M0TOM4_ZCL_CLUSTER_DATA_IND */
}

void
ZbZclClusterLoopbackUnbind(struct ZbZclClusterT *clusterPtr, struct ZbApsFilterT *filter)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_LOOPBACK_UNBIND;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)filter;
    ZIGBEE_CmdTransfer();
}

enum ZclStatusCodeT
ZbZclClusterRegisterAlarmResetHandler(struct ZbZclClusterT *clusterPtr, ZbZclAlarmResetFuncT callback)
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
    struct zb_ipc_m4_cb_info *info = NULL;
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
    zb_ipc_m4_memcpy2(&ipcc_req->Data[0], &partnerAddr, 8);
    ipcc_req->Data[2] = (uint32_t)aps_req_key;
    ipcc_req->Data[3] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB handler */
}

/******************************************************************************
 * Certification Test Hooks
 ******************************************************************************
 */
void
ZbTestCaseClear(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TEST_CASE_CLEAR;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
}

uint32_t
ZbTestCaseCurrent(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TEST_CASE_CURRENT;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval();
}

void
ZbTestCaseDisable(struct ZigBeeT *zb, enum ZbTestcaseT testcase)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TEST_CASE_DISABLE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)testcase;
    ZIGBEE_CmdTransfer();
}

void
ZbTestCaseEnable(struct ZigBeeT *zb, enum ZbTestcaseT testcase)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_TEST_CASE_ENABLE;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)testcase;
    ZIGBEE_CmdTransfer();
}

bool
ZbApsFragDropTxAdd(struct ZigBeeT *zb, uint8_t blockNum)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_FRAG_DROP_ADD;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)blockNum;
    ZIGBEE_CmdTransfer();
    retval = zb_ipc_m4_get_retval();
    return retval != 0 ? true : false;
}

void
ZbApsFragDropTxClear(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_FRAG_DROP_CLEAR;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
}

/******************************************************************************
 * AES & Hashing
 ******************************************************************************
 */
void
ZbAesMmoHash(uint8_t const *data, const unsigned int length, uint8_t *hash)
{
    struct ZbHash newHash;

    ZbHashInit(&newHash);
    ZbHashAdd(&newHash, data, length);
    ZbHashDigest(&newHash, hash);
}

void
ZbHashInit(struct ZbHash *h)
{
    (void)memset(h->m, 0, sizeof(h->m));
    (void)memset(h->hash, 0, sizeof(h->hash));
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
}

void
ZbHashByte(struct ZbHash *h, uint8_t byte)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_HASH_BYTE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)h;
    ipcc_req->Data[1] = (uint32_t)byte;
    ZIGBEE_CmdTransfer();
}

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
    struct zb_ipc_m4_cb_info *info = NULL;
    Zigbee_Cmd_Request_t *p_notification;

    /* Get pointer on received event buffer from M0 */
    p_notification = ZIGBEE_Get_NotificationPayloadBuffer();

    switch (p_notification->ID) {
        case MSG_M0TOM4_FILTER_MSG_CB:
        {
            struct zb_msg_filter_cb_info *cb_info;
            int retval;

            assert(p_notification->Size == 3);
            cb_info = (struct zb_msg_filter_cb_info *)p_notification->Data[2];
            retval = cb_info->callback(zb_ipc_globals.zb, (uint32_t)p_notification->Data[0],
                    (void *)p_notification->Data[1], cb_info->arg);
            /* Return the retval in the notification ACK */
            p_notification->Data[0] = retval;
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
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_REJOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbNlmeJoinConfT *conf, void *arg);

                callback = (void (*)(ZbNlmeJoinConfT *conf, void *arg))info->callback;
                callback((ZbNlmeJoinConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_FINDBIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_TCSO_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbTcsoStatusT status, void *arg);

                callback = (void (*)(enum ZbTcsoStatusT status, void *arg))info->callback;
                callback((enum ZbTcsoStatusT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_TC_REJOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
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
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_APSDE_DATA_REQ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbApsdeDataConfT *conf, void *arg);

                callback = (void (*)(ZbApsdeDataConfT *conf, void *arg))info->callback;
                callback((ZbApsdeDataConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_APS_FILTER_ENDPOINT_CB:
        {
            ZbApsdeDataIndT *data_ind;
            struct aps_filter_cb *aps_filter_cb;
            int err = ZB_APS_FILTER_CONTINUE;

            assert(p_notification->Size == 2);
            data_ind = (ZbApsdeDataIndT *)p_notification->Data[0];
            aps_filter_cb = (struct aps_filter_cb *)p_notification->Data[1];
            if (aps_filter_cb->callback != NULL) {
                err = aps_filter_cb->callback(data_ind, aps_filter_cb->cb_arg);
            }
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_APS_FILTER_CLUSTER_CB:
        {
            ZbApsdeDataIndT *data_ind;
            struct aps_filter_cb *aps_filter_cb;
            int err = ZB_APS_FILTER_CONTINUE;

            assert(p_notification->Size == 2);
            data_ind = (ZbApsdeDataIndT *)p_notification->Data[0];
            aps_filter_cb = (struct aps_filter_cb *)p_notification->Data[1];
            if (aps_filter_cb->callback != NULL) {
                err = aps_filter_cb->callback(data_ind, aps_filter_cb->cb_arg);
            }
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_NLME_NET_DISC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbNlmeNetDiscConfT *conf, void *arg);

                callback = (void (*)(ZbNlmeNetDiscConfT *conf, void *arg))info->callback;
                callback((ZbNlmeNetDiscConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_LEAVE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(struct ZbNlmeLeaveConfT *conf, void *arg);

                callback = (void (*)(struct ZbNlmeLeaveConfT *conf, void *arg))info->callback;
                callback((struct ZbNlmeLeaveConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_SYNC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbNlmeSyncConfT *conf, void *arg);

                callback = (void (*)(ZbNlmeSyncConfT *discConf, void *arg))info->callback;
                callback((ZbNlmeSyncConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_ROUTE_DISC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbNlmeRouteDiscConfT *discConf, void *cbarg);

                callback = (void (*)(ZbNlmeRouteDiscConfT *discConf, void *cbarg))info->callback;
                callback((ZbNlmeRouteDiscConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_DEVICE_ANNCE_FILTER_CB:
        {
            struct zdo_device_annce_cb_info *cb_info;
            unsigned int i;

            assert(p_notification->Size == 2);
            for (i = 0; i < ZB_IPC_ZDO_DEVICE_ANNCE_CB_LIST_MAX; i++) {
                cb_info = &zdo_device_annce_cb_list[i];
                if (cb_info->handle != NULL) {
                    cb_info->callback(zb_ipc_globals.zb, (ZbZdoDeviceAnnceT *)p_notification->Data[0], (uint8_t)p_notification->Data[1]);
                    break;
                }
            }
            break;
        }

        case MSG_M0TOM4_ZDO_NWK_ADDR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoNwkAddrRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoNwkAddrRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoNwkAddrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_IEEE_ADDR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoIeeeAddrRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoIeeeAddrRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoIeeeAddrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_NODE_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoNodeDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoNodeDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoNodeDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_POWER_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoPowerDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoPowerDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoPowerDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_SIMPLE_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoSimpleDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoSimpleDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoSimpleDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_ACTIVE_EP_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoActiveEpRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoActiveEpRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoActiveEpRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MATCH_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoMatchDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoMatchDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoMatchDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB:
            if (zdo_match_multi_cb != NULL) {
                ZbZdoMatchDescRspT *rsp;

                /* Note, we're not using zb_ipc_m4_cb_info for this API, so we don't need
                 * the callback argument. */
                assert(p_notification->Size == 1);
                rsp = (ZbZdoMatchDescRspT *)p_notification->Data[0];
                zdo_match_multi_cb(rsp, NULL);
                if (rsp->status == ZB_ZDP_STATUS_TIMEOUT) {
                    /* Release the callback */
                    zdo_match_multi_cb = NULL;
                }
            }
            break;

        case MSG_M0TOM4_ZDO_BIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoBindRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoBindRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_UNBIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoBindRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoBindRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_LQI_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoLqiRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoLqiRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoLqiRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_BIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoMgmtBindRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoMgmtBindRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoMgmtBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_LEAVE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoLeaveRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoLeaveRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoLeaveRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoPermitJoinRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoPermitJoinRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoPermitJoinRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZdoNwkUpdateNotifyT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoNwkUpdateNotifyT *rsp, void *cbarg))info->callback;
                callback((ZbZdoNwkUpdateNotifyT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_CLUSTER_DATA_IND:
        {
            ZbApsdeDataIndT *dataIndPtr;
            void *cb_arg;
            int err;

            assert(p_notification->Size == 2);
            dataIndPtr = (ZbApsdeDataIndT *)p_notification->Data[0];
            cb_arg = (void *)p_notification->Data[1];
            err = zcl_cluster_data_ind(dataIndPtr, cb_arg);
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB:
        {
            ZbApsdeDataIndT *dataIndPtr;
            void *cb_arg;
            int err;

            assert(p_notification->Size == 2);
            dataIndPtr = (ZbApsdeDataIndT *)p_notification->Data[0];
            cb_arg = (void *)p_notification->Data[1];
            err = zcl_cluster_alarm_data_ind(dataIndPtr, cb_arg);
            /* Return err in second argument */
            p_notification->Data[1] = (uint32_t)err;
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_CMD_REQ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            /* Note: shouldn't get here if callback was NULL in request, so info should
             * always be non-NULL. */
            if (info != NULL) {
                struct ZbZclCommandRspT *zcl_rsp = (struct ZbZclCommandRspT *)p_notification->Data[0];

                if (info->callback != NULL) {
                    void (*callback)(struct ZbZclCommandRspT *rsp, void *cbarg);

                    callback = (void (*)(struct ZbZclCommandRspT *rsp, void *cbarg))info->callback;
                    callback(zcl_rsp, info->arg);
                }
                if (info->zcl_recv_multi_rsp && (zcl_rsp->status != ZCL_STATUS_TIMEOUT)) {
                    /* Don't free the callback yet */
                    info = NULL;
                }
            }
            break;

        case MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbApsdeDataConfT *conf, void *arg);

                callback = (void (*)(ZbApsdeDataConfT *conf, void *arg))info->callback;
                callback((ZbApsdeDataConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_COMMAND_REQ_CB:
        {
            int err = ZB_APS_FILTER_CONTINUE;

            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
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

        case MSG_M0TOM4_ZCL_COMMAND_NO_RSP_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbApsdeDataConfT *conf, void *arg);

                callback = (void (*)(ZbApsdeDataConfT *conf, void *arg))info->callback;
                callback((ZbApsdeDataConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_READ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZclReadRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZclReadRspT *rsp, void *cbarg))info->callback;
                callback((ZbZclReadRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_WRITE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZclWriteRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZclWriteRspT *rsp, void *cbarg))info->callback;
                callback((ZbZclWriteRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(ZbZclDiscoverAttrRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZclDiscoverAttrRspT *rsp, void *cbarg))info->callback;
                callback((ZbZclDiscoverAttrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB:
            assert(p_notification->Size == 5);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[4];
            if ((info != NULL) && (info->callback != NULL)) {
                void (*callback)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT key_status, void *arg);
                uint64_t partnerAddr;

                zb_ipc_m4_memcpy2(&partnerAddr, &p_notification->Data[0], 8);
                callback = (void (*)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT key_status, void *arg))info->callback;
                callback(partnerAddr, (uint16_t)p_notification->Data[2], (enum ZbZclKeyStatusT)p_notification->Data[3], info->arg);
            }
            break;

        default:
            status = HAL_ERROR;
            break;
    }

    if (info != NULL) {
        zb_ipc_m4_cb_info_free(info);
    }

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
            cli_port_print_msg(NULL, log_str);
            break;
        }

        case MSG_M0TOM4_ZB_MALLOC:
        {
            void *ptr;

            assert(p_logging->Size == 1);
            ptr = malloc((uint32_t)p_logging->Data[0]);
            /* Return ptr in second argument */
            p_logging->Data[1] = (uint32_t)ptr;
            break;
        }

        case MSG_M0TOM4_ZB_FREE:
            assert(p_logging->Size == 1);
            free((void *)p_logging->Data[0]);
            break;

        default:
            status = HAL_ERROR;
            break;
    }

    TL_ZIGBEE_SendM4AckToM0Request();
    return status;
}


/* Function which needs to be overloaded for test purpose
 * In particular when using the ZbCli interface
 */
__weak void cli_port_print_msg(struct cli_app *cli_p, const char *msg)
{
    UNUSED(cli_p);
    UNUSED(msg);

}
