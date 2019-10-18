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
#include "zigbee_config.h"
#include "zigbee_types.h"

#include "zigbee_interface.h" /* zigbee.h, etc */
#include "zcl/zcl.h"
#include "local.zigbee.h" /* ZbHeap */
#include "zcl/local_zcl.h"
#include "zcl/key/zcl.key.h" /* ZbZclKeWithDevice */

#include "zigbee_core.h"
#include "tl_zigbee_hci.h"

#include "dbg_trace.h"
#include "stm_logging.h"
#include "stm32wbxx_core_interface_def.h"

#ifndef ZB_HEAP_MAX_ALLOC
#define ZB_HEAP_MAX_ALLOC                   2000U
#endif

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
    void (*callback)(ZbApsdeDataIndT *data_ind, void *cb_arg);
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
};

/* Single static callback for persistent data notifications */
static void (*zb_persist_cb)(struct ZigBeeT *zb, void *cbarg) = NULL;
static void *zb_persist_arg = NULL;

/* Callback for ZbZdoMatchDescMulti. It is static and never freed. It can be overwritten
 * by another call to ZbZdoMatchDescMulti. */
static void (*zdo_match_multi_cb)(ZbZdoMatchDescRspT *ind, void *cb_arg) = NULL;

static void ZbZclAttrFreeAttr(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *attrPtr);

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
} /* zb_ipc_m4_stack_logging_config */

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
} /* zb_ipc_m4_cb_info_alloc */

static void
zb_ipc_m4_cb_info_free(struct zb_ipc_m4_cb_info *info)
{
    free(info);
} /* zb_ipc_m4_cb_info_free */

static uint32_t
zb_ipc_m4_get_retval(void)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    uint32_t retval;

    ipcc_req = ZIGBEE_Get_OTCmdRspPayloadBuffer();
    assert(ipcc_req->Size == 1);
    (void)ZbMemCpy(&retval, &ipcc_req->Data[0], 4);
    return retval;
} /* zb_ipc_m4_get_retval */

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
} /* wpan_get_uint32 */

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
} /* wpan_get_uint64 */

unsigned int
ZbHeapMaxAlloc(void)
{
    return ZB_HEAP_MAX_ALLOC;
} /* ZbHeapMaxAlloc */

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
    (void)ZbMemCpy(&ipcc_req->Data[0], &extAddr, 8);
    ipcc_req->Data[2] = (uint32_t)tblSizes;
    ipcc_req->Data[3] = (uint32_t)setLogging;
    ZIGBEE_CmdTransfer();
    zb_ipc_globals.zb = (struct ZigBeeT *)zb_ipc_m4_get_retval();
    if (setLogging != NULL) {
        /* Save the log mask */
        zb_ipc_globals.log_mask = setLogging->mask;
    }
    return zb_ipc_globals.zb;
} /* ZbInit */

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
} /* ZbDestroy */

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
} /* ZbSetLogging */

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
} /* ZbGetLogging */

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
    (void)ZbMemCpy(&ext_addr, &ipcc_req->Data, 8);
    return ext_addr;
} /* ZbExtendedAddress */

uint16_t
ZbShortAddress(struct ZigBeeT *zb)
{
    uint16_t nwkAddr;

    (void)ZbNwkGet(zb, ZB_NWK_NIB_ID_NetworkAddress, &nwkAddr, sizeof(nwkAddr));
    return nwkAddr;
} /* ZbShortAddress */

void
ZbChangeExtAddr(struct ZigBeeT *zb, uint64_t extAddr)
{

    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_EXTADDR_CHANGE;
    ipcc_req->Size = 2;
    (void)ZbMemCpy(&ipcc_req->Data[0], &extAddr, 8);
    ZIGBEE_CmdTransfer();
} /* ZbChangeExtAddr */

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
    } /* for */
    return num_channels;
} /* WpanGetNumChannelsFromMask */

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
} /* ZbStartupConfigGetProDefaults */

void
ZbStartupConfigGetProSeDefaults(struct ZbStartupT *configPtr)
{
    ZbStartupConfigGetProDefaults(configPtr);
    /* Remove the default preconfigured link key by clearing it. */
    (void)ZbMemSet(configPtr->security.preconfiguredLinkKey, 0, ZB_SEC_KEYSIZE);
    /* Remove the distributed link key by clearing it. */
    (void)ZbMemSet(configPtr->security.distributedGlobalKey, 0, ZB_SEC_KEYSIZE);
} /* ZbStartupConfigGetProSeDefaults */

enum ZbStatusCodeT
ZbStartupCbkeConfig(struct ZigBeeT *zb, struct ZbStartupCbkeT *cbke_config)
{
    /* TODO - may be required for SE certification */
    return ZB_APS_STATUS_NOT_SUPPORTED;
} /* ZbStartupCbkeConfig */

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
} /* ZbStartup */

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
} /* ZbStartupRejoin */

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
} /* ZbStartupPersist */

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
} /* ZbStartupFindBindStart */

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
} /* ZbStartupTouchlinkTargetStop */

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
} /* ZbStartupTcsoStart */

void
ZbStartupTcsoAbort(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_STARTUP_TCSO_ABORT;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
} /* ZbStartupTcsoAbort */

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
} /* ZbTrustCenterRejoin */

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
} /* ZbPersistNotifyRegister */

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
} /* ZbPersistGet */

enum ZbStatusCodeT
ZbLeaveReq(struct ZigBeeT *zb, void (*callback)(ZbNlmeLeaveConfT *conf, void *arg), void *cbarg)
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
} /* ZbLeaveReq */

void
ZbReset(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_RESET_REQ;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
} /* ZbReset */

/******************************************************************************
 * ECDSA Signature
 ******************************************************************************
 */
enum ZbStatusCodeT
ZbSecEcdsaValidate(struct ZigBeeT *zb, enum ZbSecEcdsaSigType sig_type,
    const uint8_t *ca_pub_key_array, unsigned int ca_pub_key_len,
    const uint8_t *certificate, const uint8_t *signature, uint8_t *image_digest, uint8_t *cert_digest)
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
} /* ZbSecEcdsaValidate */

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
} /* ZbBdbGetIndex */

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
} /* ZbBdbSetIndex */

/* ZbBdbSetEndpointStatus is required for the Identify Server Cluster */
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
} /* ZbBdbSetEndpointStatus */

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
} /* ZbApsdeDataReqCallback */

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
} /* ZbApsmeAddEndpoint */

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
} /* ZbApsmeRemoveEndpoint */

struct ZbApsFilterT *
ZbApsFilterEndpointAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t profileId,
    void (*callback)(ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb *aps_filter_cb;
    struct ZbApsFilterT *filter;

    /* FIXME - track these allocations so they can be freed later? */
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
} /* ZbApsFilterEndpointAdd */

bool
ZbApsmeEndpointConfigNoMatchCallback(struct ZigBeeT *zb, uint8_t endpoint, void (*callback)(ZbApsdeDataIndT *ind, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb *aps_filter_cb;
    bool retval;

    /* FIXME - track these allocations so they can be freed later? */
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
} /* ZbApsmeEndpointConfigNoMatchCallback */

struct ZbApsFilterT *
ZbApsFilterClusterAdd(struct ZigBeeT *zb, uint8_t endpoint, uint16_t clusterId, uint16_t profileId,
    void (*callback)(ZbApsdeDataIndT *dataInd, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct aps_filter_cb *aps_filter_cb;
    struct ZbApsFilterT *filter;

    /* FIXME - track these allocations so they can be freed later. */
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
} /* ZbApsFilterClusterAdd */

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
} /* ZbApsmeEndpointClusterListAppend */

void
ZbApsFilterRemove(struct ZigBeeT *zb, struct ZbApsFilterT *filter)
{
    ZbEnterCritical(zb);
    LINK_LIST_UNLINK(&filter->link);
    ZbExitCritical(zb);
} /* ZbApsFilterRemove */

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
} /* ZbApsEndpointExists */

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
} /* ZbApsEndpointProfile */

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
} /* ZbApsAddrIsBcast */

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
} /* ZbApsAddrIsLocal */

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
} /* ZbApsGetIndex */

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
} /* ZbApsSetIndex */

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
} /* ZbApsmeAddGroupReq */

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
} /* ZbApsmeRemoveGroupReq */

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
} /* ZbApsmeRemoveAllGroupsReq */

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
    } /* for */
    ZbExitCritical(zb);
    return false;
} /* ZbApsGroupIsMember */

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
    } /* for */
    ZbExitCritical(zb);
    return (uint8_t)(i - num_found);
} /* ZbApsGroupsGetCapacity */

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
    } /* for */
    ZbExitCritical(zb);
    return len;
} /* ZbApsGroupsGetMembership */

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
} /* ZbApsmeBindReq */

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
} /* ZbApsmeUnbindReq */

void
ZbApsUnbindAllReq(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_UNBIND_ALL;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
} /* ZbApsUnbindAllReq */

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
} /* ZbApsmeTransportKeyReq */

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
} /* ZbApsmeRemoveDeviceReq */

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
} /* ZbApsmeRequestKeyReq */

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
} /* ZbApsmeSwitchKeyReq */

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
} /* ZbApsmeAddKeyReq */

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
} /* ZbApsmeGetKeyReq */

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
} /* ZbApsmeRemoveKeyReq */

/******************************************************************************
 * Zigbee Uptime
 ******************************************************************************
 */
ZbUptimeT
ZbUptime(void)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_UPTIME;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
    return (ZbUptimeT)zb_ipc_m4_get_retval();
} /* ZbUptime */

/* If a value is greater than the high mark, and a second value is lower
 * than the low mark, then the second value is deemed to have rolled-over
 * when comparing the two values. */
#define TIMER_ROLL_OVER_HIGH        ((ZB_UPTIME_MAX / 2UL) + (ZB_UPTIME_MAX / 4UL))
#define TIMER_ROLL_OVER_LOW         ((ZB_UPTIME_MAX / 2UL) - (ZB_UPTIME_MAX / 4UL))

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
} /* ZbTimeoutRemaining */

/******************************************************************************
* Zigbee Message Filters
******************************************************************************
*/
struct zb_msg_filter_cb_info {
    struct ZbMsgFilterT *filter;
    int (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg);
    void *arg;
};

#define ZB_IPC_MSG_FILTER_CB_LIST_MAX               4
static struct zb_msg_filter_cb_info zb_msg_filter_cb_list[ZB_IPC_MSG_FILTER_CB_LIST_MAX];

struct ZbMsgFilterT *
ZbMsgFilterRegister(struct ZigBeeT *zb, uint32_t mask, uint8_t prio,
    int (*callback)(struct ZigBeeT *zb, uint32_t id, void *msg, void *cbarg), void *arg)
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
} /* ZbMsgFilterRegister */

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
} /* ZbMsgFilterRemove */

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
} /* ZbTimerAlloc */

void
ZbTimerChangeCallback(struct ZbTimerT *timer, void (*callback)(struct ZigBeeT *zb, void *cb_arg), void *arg)
{
    ZbTimerStop(timer);
    timer->callback = callback;
    timer->arg = arg;
} /* ZbTimerChangeCallback */

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
} /* ZbTimerFree */

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
} /* ZbTimerRunning */

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
} /* ZbTimerRemaining */

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
} /* ZbTimerStop */

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
} /* ZbTimerReset */

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
} /* ZbNwkGetIndex */

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
} /* ZbNwkSetIndex */

enum ZbStatusCodeT
ZbNwkGet(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz)
{
    return ZbNwkGetIndex(zb, attrId, attrPtr, attrSz, 0);
} /* ZbNwkGet */

enum ZbStatusCodeT
ZbNwkSet(struct ZigBeeT *zb, enum ZbNwkNibAttrIdT attrId, void *attrPtr, unsigned int attrSz)
{
    return ZbNwkSetIndex(zb, attrId, attrPtr, attrSz, 0);
} /* ZbNwkSet */

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
    (void)ZbMemCpy(&ext_addr, &ipcc_req->Data[0], 8);
    return ext_addr;
} /* ZbNwkGetParentExtAddr */

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
} /* ZbNwkGetParentShortAddr */

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
    (void)ZbMemCpy(&ext_addr, &ipcc_req->Data[0], 8);
    return ext_addr;
} /* ZbNwkAddrLookupExt */

uint16_t
ZbNwkAddrLookupNwk(struct ZigBeeT *zb, uint64_t extAddr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_ADDR_LOOKUP_NWK;
    /* Extended address is split across two args */
    ipcc_req->Size = 2;
    memcpy(ipcc_req->Data, &extAddr, 8);
    ZIGBEE_CmdTransfer();
    return (uint16_t)zb_ipc_m4_get_retval();
} /* ZbNwkAddrLookupNwk */

bool
ZbNwkAddrIsChildExt(struct ZigBeeT *zb, uint64_t extAddr, uint16_t *nwkAddrPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_NWK_ADDR_IS_CHILD_EXT;
    /* Extended address is split across two args */
    ipcc_req->Size = 3;
    memcpy(ipcc_req->Data, &extAddr, 8);
    ipcc_req->Data[2] = (uint32_t)nwkAddrPtr;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
} /* ZbNwkAddrIsChildExt */

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
} /* ZbNwkAddrIsChildNwk */

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
} /* ZbNwkGetSecMaterial */

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
} /* ZbNlmeNetDiscReq */

enum ZbStatusCodeT
ZbNlmeLeaveReq(struct ZigBeeT *zb, ZbNlmeLeaveReqT *req,
    void (*callback)(ZbNlmeLeaveConfT *leaveConfPtr, void *arg), void *cbarg)
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
} /* ZbNlmeLeaveReq */

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
} /* ZbNlmePermitJoinReq */

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
} /* ZbNlmeResetReq */

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
} /* ZbNlmeSyncReq */

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
} /* ZbNlmeRouteDiscReq */

enum WpanJoinPolicyT
ZbNlmeJoiningPolicyGet(struct ZigBeeT *zb)
{
    /* This is only for zbcli. We don't need this for 2.4 GHz operation. */
    return WPAN_JOIN_POLICY_ALL;
} /* ZbNlmeJoiningPolicyGet */

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
} /* ZbNwkNeighborClearAll */

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
} /* ZbNlmeDirectJoinReq */

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
} /* ZbNlmeSetInterface */

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
} /* ZbNlmeGetInterface */

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
} /* ZbNwkIfSetTxPower */

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
} /* ZbNwkIfGetTxPower */

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
    (void)ZbMemCpy(&ipcc_req->Data[1], &srcAddr, 8);
    ipcc_req->Data[3] = (uint32_t)newFrameCount;
    ZIGBEE_CmdTransfer();
    return zb_ipc_m4_get_retval() != 0U ? true : false;
} /* ZbNwkSetFrameCounter */

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
} /* ZbNwkFastPollRequest */

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
} /* ZbNwkFastPollRelease */

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
} /* ZbNwkFastPollResourceCount */

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
} /* ZbNwkAddrSetNextChildAddr */

/* Required for zigbee certification only. May not be required for a DUT though. */
bool
nwk_status_send(struct ZigBeeT *zb, uint16_t dstAddr, enum ZbNwkAddrModeT dstAddrMode,
    uint16_t statusAddr, enum ZbNwkNetworkStatusCodeT statusCode)
{
    /* TODO? */
    return false;
} /* nwk_status_send */

/* Required for zigbee certification only. May not be required for a DUT though. */
bool
nwk_rreq_by_route_is_active(struct ZigBeeT *zb, ZbNwkRouteEntryT *route)
{
    /* TODO? */
    return false;
} /* nwk_rreq_by_route_is_active */

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
} /* ZbZdoDeviceAnnceFilterRegister */

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
} /* ZbZdoDeviceAnnceFilterRemove */

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
} /* ZbZdoNwkAddrReq */

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
} /* ZbZdoIeeeAddrReq */

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
} /* ZbZdoNodeDescReq */

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
} /* ZbZdoPowerDescReq */

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
} /* ZbZdoSimpleDescReq */

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
} /* ZbZdoActiveEpReq */

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
} /* ZbZdoMatchDescReq */

enum ZbStatusCodeT
ZbZdoMatchDescMulti(struct ZigBeeT *zb, ZbZdoMatchDescReqT *req,
    void (*callback)(ZbZdoMatchDescRspT *ind, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    /* The callback must be static so we can receive multiple responses. We don't check here if
     * we're overwriting an existing callback, so application cannot effecticely perform more
     * than one of these at a time. There should be no practical need for this though.
     * There may even be no practical need for this API. */
    zdo_match_multi_cb = callback;
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZDO_MATCH_DESC_MULTI;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)req;
    ZIGBEE_CmdTransfer();
    return (enum ZbStatusCodeT)zb_ipc_m4_get_retval();
    /* Followed up in MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB handler */
} /* ZbZdoMatchDescMulti */

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
} /* ZbZdoDeviceAnnce */

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
} /* ZbZdoDeviceAnnceAlias */

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
} /* ZbZdoBindReq */

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
} /* ZbZdoUnbindReq */

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
} /* ZbZdoLqiReq */

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
} /* ZbZdoMgmtBindReq */

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
} /* ZbZdoLeaveReq */

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
} /* ZbZdoPermitJoinReq */

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
} /* ZbZdoNwkUpdateReq */

/******************************************************************************
 * ZCL
 ******************************************************************************
 */
static const struct ZbZclAttrT zcl_attr_cluster_mandatory_revision = {
    ZCL_GLOBAL_ATTR_CLUSTER_REV, ZCL_DATATYPE_UNSIGNED_16BIT,
    ZCL_ATTR_FLAG_NONE, 0, NULL, {0, 0}, {0, 0}
};

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
} /* ZbZclBasicPostAlarm */

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
} /* ZbZclBasicServerResetCmdConfig */

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
} /* ZbZclBasicWriteDirect */

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

void *
ZbZclClusterAlloc(struct ZigBeeT *zb, unsigned int alloc_sz, enum ZbZclClusterIdT cluster_id,
    uint8_t endpoint, enum ZbZclDirectionT direction)
{
    struct ZbZclClusterT *clusterPtr;

    if (alloc_sz < sizeof(struct ZbZclClusterT)) {
        return NULL;
    }
    clusterPtr = (struct ZbZclClusterT *)ZbHeapAlloc(zb, alloc_sz);
    if (clusterPtr == NULL) {
        return NULL;
    }
    (void)ZbMemSet(clusterPtr, 0, alloc_sz);
    LINK_LIST_INIT(&clusterPtr->link);

    clusterPtr->zb = zb;
    clusterPtr->clusterId = cluster_id;
    clusterPtr->endpoint = endpoint;
    clusterPtr->mfrCode = 0x0000U;
    clusterPtr->profileId = ZCL_PROFILE_HOME_AUTOMATION;
    clusterPtr->txOptions = (uint16_t)ZCL_CLUSTER_TXOPTIONS_DEFAULT;
    clusterPtr->discoverRoute = true;
    clusterPtr->radius = 0U;
    clusterPtr->maxAsduLength = (uint16_t)ZB_APS_CONST_SAFE_APSSEC_PAYLOAD_SIZE;
    clusterPtr->direction = direction;
    LINK_LIST_INIT(&clusterPtr->attributeList);
    LINK_LIST_INIT(&clusterPtr->reports);
#ifdef CONFIG_ZB_ZCL_PERSIST
    clusterPtr->persist_timer = ZbTimerAlloc(zb, zcl_persist_cluster_timer, clusterPtr);
# if 0 /* FIXME - don't free cluster here, return false instead. */
    if (clusterPtr->persist_timer == NULL) {
        ZbHeapFree(zb, clusterPtr);
        return NULL;
    }
# endif
#endif

    (void)ZbZclClusterSetMinSecurity(clusterPtr, ZB_APS_STATUS_SECURED_NWK_KEY);

    /* Allocate the mandatory attributes */
    if (ZbZclAttrAppendList(clusterPtr, &zcl_attr_cluster_mandatory_revision, 1U) != ZCL_STATUS_SUCCESS) {
        ZbHeapFree(zb, clusterPtr);
        return NULL;
    }
    (void)ZbZclAttrIntegerWrite(clusterPtr, (uint16_t)ZCL_GLOBAL_ATTR_CLUSTER_REV, ZCL_CLUSTER_REVISION_LEGACY);

    return clusterPtr;
} /* ZbZclClusterAlloc */

void
ZbZclClusterAttach(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    /* Call to the M0 to initialize the cluster and attach any filters with the stack. */
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_ATTACH;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    /* IPC has no return value */
} /* ZbZclClusterAttach */

void
ZbZclClusterFree(struct ZbZclClusterT *clusterPtr)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    if (clusterPtr->zb == NULL) {
        return;
    }

    /* Call into the M0 to do any further cleanup (filters, etc) */
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_DETACH;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    /* IPC has no return value */

    if (clusterPtr->cleanup != NULL) {
        clusterPtr->cleanup(clusterPtr);
    }
    ZbHeapFree(clusterPtr->zb, clusterPtr);
} /* ZbZclClusterFree */

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
} /* ZbZclAddEndpoint */

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
} /* ZbZclRemoveEndpoint */

void
ZbZclReadReq(struct ZbZclClusterT *clusterPtr, ZbZclReadReqT *req,
    void (*callback)(const ZbZclReadRspT *readRsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        ZbZclReadRspT rsp;

        memset(&rsp, 0, sizeof(rsp));
        rsp.status = ZB_STATUS_ALLOC_FAIL;
        callback(&rsp, arg);
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_READ_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    /* Followed up in MSG_M0TOM4_ZCL_READ_CB handler */
} /* ZbZclReadReq */

void
ZbZclWriteReq(struct ZbZclClusterT *clusterPtr, ZbZclWriteReqT *req,
    void (*callback)(const ZbZclWriteRspT *writeResp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        ZbZclWriteRspT rsp;

        memset(&rsp, 0, sizeof(rsp));
        rsp.status = ZB_STATUS_ALLOC_FAIL;
        callback(&rsp, arg);
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_WRITE_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    /* Followed up in MSG_M0TOM4_ZCL_WRITE_CB handler */
} /* ZbZclWriteReq */

void
ZbZclDiscoverAttrReq(struct ZbZclClusterT *clusterPtr, ZbZclDiscoverAttrReqT *req,
    void (*callback)(const ZbZclDiscoverAttrRspT *discRsp, void *cb_arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        ZbZclDiscoverAttrRspT rsp;

        memset(&rsp, 0, sizeof(rsp));
        rsp.status = ZB_STATUS_ALLOC_FAIL;
        callback(&rsp, arg);
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_DISCOVER_ATTR_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    /* Followed up in MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB handler */
} /* ZbZclDiscoverAttrReq */

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
} /* ZbZclGetNextSeqnum */

void
ZbZclCommandReq(struct ZigBeeT *zb, ZbZclCommandReqT *zclReq,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        struct ZbZclCommandRspT rsp;

        memset(&rsp, 0, sizeof(rsp));
        rsp.status = ZB_STATUS_ALLOC_FAIL;
        callback(&rsp, arg);
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_COMMAND_REQ;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)zclReq;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    /* Followed up in MSG_M0TOM4_ZCL_COMMAND_REQ_CB handler */
} /* ZbZclCommandReq */

void
ZbZclCommandNoResp(struct ZigBeeT *zb, ZbZclCommandReqT *req,
    void (*callback)(ZbApsdeDataConfT *confPtr, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        ZbApsdeDataConfT conf;

        memset(&conf, 0, sizeof(conf));
        conf.status = ZB_STATUS_ALLOC_FAIL;
        callback(&conf, arg);
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_COMMAND_NO_RSP;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)req;
    ipcc_req->Data[1] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    /* Followed up in MSG_M0TOM4_ZCL_COMMAND_NO_RSP_CB handler */
} /* ZbZclCommandNoResp */

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
} /* ZbZclSendDefaultResponse */

void
ZbZclClusterCommandReq(struct ZbZclClusterT *clusterPtr, struct ZbZclClusterCommandReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    struct zb_ipc_m4_cb_info *info;

    info = zb_ipc_m4_cb_info_alloc((void *)callback, arg);
    if (info == NULL) {
        struct ZbZclCommandRspT rsp;

        memset(&rsp, 0, sizeof(rsp));
        rsp.status = ZB_STATUS_ALLOC_FAIL;
        callback(&rsp, arg);
        return;
    }
    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_CMD_REQ;
    ipcc_req->Size = 3;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)req;
    ipcc_req->Data[2] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    /* Followed up in MSG_M0TOM4_ZCL_CLUSTER_CMD_REQ_CB handler */
} /* ZbZclClusterCommandReq */

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
} /* ZbZclClusterCommandRsp */

enum ZclStatusCodeT
ZbZclClusterCommandRspWithCb(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *dstInfo, uint8_t cmdId,
    ZbApsBufT *payloads, uint8_t numPayloads, void (*callback)(ZbApsdeDataConfT *conf, void *arg), void *arg)
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
} /* ZbZclClusterCommandRspWithCb */

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
} /* ZbZclSendClusterStatusResponse */

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
} /* ZbZclClusterEndpointRegister */

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
} /* ZbZclClusterEndpointRemove */

enum ZclStatusCodeT
zcl_cluster_bind(struct ZbZclClusterT *clusterPtr, struct ZbApsFilterT *filter,
    uint8_t endpoint, uint16_t profileId, enum ZbZclDirectionT direction)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_BIND;
    ipcc_req->Size = 5;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ipcc_req->Data[1] = (uint32_t)filter;
    ipcc_req->Data[2] = (uint32_t)endpoint;
    ipcc_req->Data[3] = (uint32_t)profileId;
    ipcc_req->Data[4] = (uint32_t)direction;
    ZIGBEE_CmdTransfer();
    return (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
} /* zcl_cluster_bind */

enum ZclStatusCodeT
ZbZclClusterBind(struct ZbZclClusterT *clusterPtr, uint8_t endpoint, uint16_t profileId, enum ZbZclDirectionT direction)
{
    return zcl_cluster_bind(clusterPtr, &clusterPtr->filter, endpoint, profileId, direction);
} /* ZbZclClusterBind */

void
ZbZclClusterUnbind(struct ZbZclClusterT *clusterPtr)
{
    if (clusterPtr->filter.callback != NULL) {
        ZCL_LOG_PRINTF(clusterPtr->zb, __func__, "Removing filter for cluster 0x%04x on endpoint %d.", clusterPtr->clusterId, clusterPtr->endpoint);
        ZbApsFilterRemove(clusterPtr->zb, &clusterPtr->filter);
        clusterPtr->filter.callback = NULL;
    }
} /* ZbZclClusterUnbind */

enum ZclStatusCodeT
ZbZclClusterRegisterAlarmResetHandler(struct ZbZclClusterT *clusterPtr, ZbZclAlarmResetFuncT callback)
{
    Zigbee_Cmd_Request_t *ipcc_req;
    enum ZclStatusCodeT status;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZCL_CLUSTER_ALARM_ADD_FILTER;
    ipcc_req->Size = 1;
    ipcc_req->Data[0] = (uint32_t)clusterPtr;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    if (status == ZCL_STATUS_SUCCESS) {
        clusterPtr->alarm_reset_callback = callback;
    }
    else {
        clusterPtr->alarm_reset_callback = NULL;
    }
    return status;
    /* Callbacks followed up in MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB handler. */
} /* ZbZclClusterRegisterAlarmResetHandler */

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
} /* ZbZclClusterSendAlarm */

enum ZclStatusCodeT
zcl_reporting_config_default(struct ZbZclClusterT *clusterPtr)
{
    /* FIXME 0 - TODO - local shared code? */
    return ZCL_STATUS_FAILURE;
} /* zcl_reporting_config_default */

bool
zcl_reporting_remove(struct ZbZclClusterT *clusterPtr, uint16_t attributeId, enum ZbZclReportDirectionT direction)
{
    /* FIXME 0 - TODO - local shared code? */
    return false;
} /* zcl_reporting_remove */

enum ZclStatusCodeT
ZbZclAttrAppendList(struct ZbZclClusterT *clusterPtr, const struct ZbZclAttrT *attrList, unsigned int num_attrs)
{
    unsigned int i;
    struct ZbZclAttrListEntryT *attrPtr;
    unsigned int val_buf_sz;
    Zigbee_Cmd_Request_t *ipcc_req;

    ZclAssert(clusterPtr != NULL);
    /* The caller shouldn't be calling with a NULL attribute list, but
     * allow it anyway. */
    if ((attrList == NULL) || (num_attrs == 0U)) {
        return ZCL_STATUS_SUCCESS;
    }

    for (i = 0; i < num_attrs; i++) {
        attrPtr = ZbZclAttrFind(clusterPtr, attrList[i].attributeId);
        if (attrPtr != NULL) {
            /* Replace any duplicate attributes we find. The cluster might
             * have some default attribute definitions that the application
             * wants to override with its own. */
            ZbZclAttrFreeAttr(clusterPtr, attrPtr);
        }

        /* FIXME - use a single ZbHeapAlloc? Need to make sure structs are starting on aligned memory. */
        attrPtr = ZbHeapAlloc(clusterPtr->zb, sizeof(struct ZbZclAttrListEntryT));
        if (attrPtr == NULL) {
            ZCL_LOG_PRINTF(clusterPtr->zb, __func__, "Error, memory exhausted (len = %d)", sizeof(struct ZbZclAttrListEntryT) + val_buf_sz);
            return ZCL_STATUS_INSUFFICIENT_SPACE;
        }
        (void)ZbMemSet(attrPtr, 0, sizeof(struct ZbZclAttrListEntryT));
        LINK_LIST_INIT(&attrPtr->link);
        attrPtr->info = &attrList[i];

        if ((attrPtr->info->flags & ZCL_ATTR_FLAG_REPORTABLE) != 0U) {
            attrPtr->reporting.interval_secs_max = attrPtr->info->reporting.interval_max;
            attrPtr->reporting.interval_secs_min = attrPtr->info->reporting.interval_min;
        }

        if (attrPtr->info->customValSz > 0U) {
            attrPtr->valSz = attrPtr->info->customValSz;

            /* Allow for leading string length header, to keep these details from user. */
            if ((attrPtr->info->dataType == ZCL_DATATYPE_STRING_OCTET)
                || (attrPtr->info->dataType == ZCL_DATATYPE_STRING_CHARACTER)) {
                attrPtr->valSz += 1U;
            }
            else if ((attrPtr->info->dataType == ZCL_DATATYPE_STRING_LONG_OCTET)
                     || (attrPtr->info->dataType == ZCL_DATATYPE_STRING_LONG_CHARACTER)) {
                attrPtr->valSz += 2U;
            }
            else {
                /* no change to valSz */
            }

            if (((attrPtr->info->flags & ZCL_ATTR_FLAG_CB_READ) != 0U)
                && ((attrPtr->info->flags & ZCL_ATTR_FLAG_CB_WRITE) != 0U)) {
                /* If defining a custom value size and also both custom
                 * read and write functions, then don't allocate the
                 * attribute data buffer. The cluster will maintain
                 * this information separately.
                 *
                 * attrPtr->valSz in this case represents the maximum
                 * potential size of the attribute data, for persistence. */
                val_buf_sz = 0U;
            }
            else {
                val_buf_sz = attrPtr->valSz;
            }
        }
        else {
            attrPtr->valSz = ZbZclAttrTypeLength(attrPtr->info->dataType);
            if (attrPtr->valSz == 0U) {
                ZCL_LOG_PRINTF(clusterPtr->zb, __func__, "Error, attr = 0x%04x, type = %d, len = 0", attrPtr->info->attributeId, attrPtr->info->dataType);
                ZbHeapFree(clusterPtr->zb, attrPtr);
                return ZCL_STATUS_INVALID_DATA_TYPE;
            }
            val_buf_sz = attrPtr->valSz;
        }

        if (val_buf_sz > 0U) {
            /* Set the valBuf memory pointer */
            attrPtr->valBuf = ZbHeapAlloc(clusterPtr->zb, val_buf_sz);
            if (attrPtr->valBuf == NULL) {
                ZbHeapFree(clusterPtr->zb, attrPtr);
                ZCL_LOG_PRINTF(clusterPtr->zb, __func__, "Error, memory exhausted (len = %d)", sizeof(struct ZbZclAttrListEntryT) + val_buf_sz);
                return ZCL_STATUS_INSUFFICIENT_SPACE;
            }
            (void)ZbMemSet(attrPtr->valBuf, 0, val_buf_sz);
        }

#if 0 /* enable for debugging */
        ZCL_LOG_PRINTF(clusterPtr->zb, __func__, "Allocating attribute (cl = 0x%04x, attr = 0x%04x, size = %d)",
            clusterPtr->clusterId, attrPtr->info->attributeId, sizeof(struct ZbZclAttrListEntryT) + val_buf_sz);
#endif

        /* Append it to the list */
        ZbZclAttrAddSorted(clusterPtr, attrPtr);

        /* Give the attribute a default value */
        if (((attrPtr->info->flags & ZCL_ATTR_FLAG_CB_DEFAULT) != 0U)) {
            struct ZbZclAttrCbInfoT cb;

            (void)ZbMemSet(&cb, 0, sizeof(struct ZbZclAttrCbInfoT));
            cb.info = attrPtr->info;
            cb.type = ZCL_ATTR_CB_TYPE_DEFAULT;
            cb.app_cb_arg = clusterPtr->app_cb_arg;
            (void)attrPtr->info->callback(clusterPtr, &cb);
        }
        else if (attrPtr->valBuf != NULL) {
            (void)ZbZclAttrDefaultValue(attrPtr->info->dataType, attrPtr->valBuf, attrPtr->valSz);
        }
        else {
            /* managed by app */
        }

        /* Let the application know about this attribute (for callbacks and attribute discovery) */
        Pre_ZigbeeCmdProcessing();
        ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
        ipcc_req->ID = MSG_M4TOM0_ZCL_ATTR_REGISTER;
        ipcc_req->Size = 2;
        ipcc_req->Data[0] = (uint32_t)clusterPtr;
        ipcc_req->Data[1] = (uint32_t)attrPtr;
        ZIGBEE_CmdTransfer();
        /* Note, attribute callbacks from M0 go to MSG_M0TOM4_ZCL_ATTR_CB */
    } /* for */

    (void)zcl_reporting_config_default(clusterPtr);
    return ZCL_STATUS_SUCCESS;
} /* ZbZclAttrAppendList */

enum ZclStatusCodeT
zcl_attr_callback_exec(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *attrPtr, struct ZbZclAttrCbInfoT *cbInfo)
{
#if 0 /* running on the application side, so don't use the stack's internal_callback */
    if (attrPtr->internal_callback != NULL) {
        return attrPtr->internal_callback(clusterPtr, cbInfo);
    }
#endif
    if (attrPtr->info->callback != NULL) {
        return attrPtr->info->callback(clusterPtr, cbInfo);
    }
    return ZCL_STATUS_FAILURE;
} /* zcl_attr_callback_exec */

static void
ZbZclAttrFreeAttr(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *attrPtr)
{
    ZclAssert(clusterPtr != NULL);

    if ((attrPtr->info->flags & ZCL_ATTR_FLAG_REPORTABLE) != 0U) {
        (void)zcl_reporting_remove(clusterPtr, attrPtr->info->attributeId, ZCL_REPORT_DIRECTION_NORMAL);
        (void)zcl_reporting_remove(clusterPtr, attrPtr->info->attributeId, ZCL_REPORT_DIRECTION_REVERSE);
    }

    LINK_LIST_UNLINK(&attrPtr->link);

    if (attrPtr->valBuf != NULL) {
        ZbHeapFree(clusterPtr->zb, attrPtr->valBuf);
    }
    ZbHeapFree(clusterPtr->zb, attrPtr);
} /* ZbZclAttrFreeAttr */

void
ZbZclAttrFreeList(struct ZbZclClusterT *clusterPtr)
{
    struct LinkListT *p;
    struct ZbZclAttrListEntryT *attrPtr;

    ZclAssert(clusterPtr != NULL);
    while (true) {
        p = LINK_LIST_HEAD(&clusterPtr->attributeList);
        if (p == NULL) {
            break;
        }
        attrPtr = LINK_LIST_ITEM(p, struct ZbZclAttrListEntryT, link);
        ZbZclAttrFreeAttr(clusterPtr, attrPtr);
    } /* while */
} /* ZbZclAttrFreeList */

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
    memcpy(&ipcc_req->Data[0], &partnerAddr, 8);
    ipcc_req->Data[2] = (uint32_t)aps_req_key;
    ipcc_req->Data[3] = (uint32_t)info;
    ZIGBEE_CmdTransfer();
    status = (enum ZclStatusCodeT)zb_ipc_m4_get_retval();
    if (status != ZCL_STATUS_SUCCESS) {
        zb_ipc_m4_cb_info_free(info);
    }
    return status;
    /* Followed up in MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB handler */
} /* ZbZclKeWithDevice */

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
} /* ZbTestCaseClear */

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
} /* ZbTestCaseCurrent */

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
} /* ZbTestCaseDisable */

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
} /* ZbTestCaseEnable */

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
} /* ZbApsFragDropTxAdd */

void
ZbApsFragDropTxClear(struct ZigBeeT *zb)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_APS_FRAG_DROP_CLEAR;
    ipcc_req->Size = 0;
    ZIGBEE_CmdTransfer();
} /* ZbApsFragDropTxClear */

/******************************************************************************
 * AES & Hashing
 ******************************************************************************
 */
void
ZbAesMmoHash(uint8_t const *data, const unsigned int length, uint8_t *hash)
{
    ZbHashT newHash;

    ZbHashInit(&newHash);
    ZbHashAdd(&newHash, data, length);
    ZbHashDigest(&newHash, hash);
} /* ZbAesMmoHash */

void
ZbHashInit(ZbHashT *h)
{
    (void)ZbMemSet(h->m, 0, sizeof(h->m));
    (void)ZbMemSet(h->hash, 0, sizeof(h->hash));
    h->length = 0;
} /* ZbHashInit */

void
ZbHashAdd(ZbHashT *h, const void *data, uint32_t len)
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
} /* ZbHashAdd */

void
ZbHashByte(ZbHashT *h, uint8_t byte)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_HASH_BYTE;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)h;
    ipcc_req->Data[1] = (uint32_t)byte;
    ZIGBEE_CmdTransfer();
} /* ZbHashByte */

void
ZbHashDigest(ZbHashT *h, void *digest)
{
    Zigbee_Cmd_Request_t *ipcc_req;

    Pre_ZigbeeCmdProcessing();
    ipcc_req = ZIGBEE_Get_OTCmdPayloadBuffer();
    ipcc_req->ID = MSG_M4TOM0_ZB_HASH_DIGEST;
    ipcc_req->Size = 2;
    ipcc_req->Data[0] = (uint32_t)h;
    ipcc_req->Data[1] = (uint32_t)digest;
    ZIGBEE_CmdTransfer();
} /* ZbHashDigest */

/******************************************************************************
 * Memory Helpers
 ******************************************************************************
 */
void *
zb_heap_alloc(struct ZigBeeT *zb, size_t sz, const char *filename, unsigned int line)
{
    /* The M4 has access to malloc */
    return malloc(sz);
} /* zb_heap_alloc */

void
zb_heap_free(struct ZigBeeT *zb, void *ptr, const char *filename, unsigned int line)
{
    free(ptr);
} /* ZbHeapFree */

void *
ZbMemSet(void *s, int c, size_t n)
{
    return memset(s, c, n);
} /* ZbMemSet */

void *
ZbMemCpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
} /* ZbMemCpy */

void *
ZbMemMove(void *dest, const void *src, size_t n)
{
    return memmove(dest, src, n);
} /* ZbMemMove */

char *
ZbStrCpy(char *dest, const char *src)
{
    return strcpy(dest, src);
} /* ZbStrCpy */

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
    } /* while */
    return crc;
} /* WpanCrc */

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
        case MSG_M0TOM4_ZB_MALLOC:
        {
            void *ptr;

             assert(p_notification->Size == 1);
             ptr = malloc((uint32_t)p_notification->Data[0]);

             /* Return ptr in second argument */
             p_notification->Data[1] = (uint32_t) ptr;

            break;
        }

        case MSG_M0TOM4_ZB_FREE:
            assert(p_notification->Size == 1);
            free((void *)p_notification->Data[0]);
            break;

        case MSG_M0TOM4_FILTER_MSG_CB:
        {
            struct zb_msg_filter_cb_info *cb_info;
            int retval;
#ifdef PATCH_MEM1
#else
            int *retptr;
#endif /* PATCH_MEM1 */

            assert(p_notification->Size == 4);
            cb_info = (struct zb_msg_filter_cb_info *)p_notification->Data[2];
#ifdef PATCH_MEM1
#else
            retptr = (int *)p_notification->Data[3];
#endif /* PATCH_MEM1 */
            retval = cb_info->callback(zb_ipc_globals.zb, (uint32_t)p_notification->Data[0],
                    (void *)p_notification->Data[1], cb_info->arg);

#ifdef PATCH_MEM1
            /******************************************************************************************/
            /*  M4                                         M0                                         */
            /*  |                                           |     Fill data[0],data[x], etc..         */
            /*  |                                           |                                         */
            /*  |                                           |      Call to zigbee_m0_send_notify      */
            /*  |<-------------Notif (data[x])------------- |                                         */
            /*  |                                           |                                         */
            /*  |Fill the data[0]                           |                                         */
            /*  |(contain the status of the notification    |                                         */
            /*  | handler)                                  |   zigbee_m0_retrieve_notif_data0()      */
            /*  |                                           |                                         */
            /******************************************************************************************/

             p_notification->Data[0] = retval; /* This parameter is filled by the M4 but is read back
                                                  by the M0 once the M0 has send the notification to
                                                  the M4*/
#else
            if (retptr != NULL) {
                *retptr = retval;
            }
#endif /* PATCH_MEM1 */
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
            if (info->callback != NULL) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_REJOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbNlmeJoinConfT *conf, void *arg);

                callback = (void (*)(ZbNlmeJoinConfT *conf, void *arg))info->callback;
                callback((ZbNlmeJoinConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_FINDBIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_TCSO_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(enum ZbTcsoStatusT status, void *arg);

                callback = (void (*)(enum ZbTcsoStatusT status, void *arg))info->callback;
                callback((enum ZbTcsoStatusT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_STARTUP_TC_REJOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
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
            if (info->callback != NULL) {
                void (*callback)(enum ZbStatusCodeT status, void *arg);

                callback = (void (*)(enum ZbStatusCodeT status, void *arg))info->callback;
                callback((enum ZbStatusCodeT)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_APSDE_DATA_REQ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbApsdeDataConfT *conf, void *arg);

                callback = (void (*)(ZbApsdeDataConfT *conf, void *arg))info->callback;
                callback((ZbApsdeDataConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_APS_FILTER_ENDPOINT_CB:
        {
            ZbApsdeDataIndT *data_ind;
            struct aps_filter_cb *aps_filter_cb;

            assert(p_notification->Size == 2);
            data_ind = (ZbApsdeDataIndT *)p_notification->Data[0];
            aps_filter_cb = (struct aps_filter_cb *)p_notification->Data[1];
            if (aps_filter_cb->callback != NULL) {
                aps_filter_cb->callback(data_ind, aps_filter_cb->cb_arg);
            }
            break;
        }

        case MSG_M0TOM4_NLME_NET_DISC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbNlmeNetDiscConfT *conf, void *arg);

                callback = (void (*)(ZbNlmeNetDiscConfT *conf, void *arg))info->callback;
                callback((ZbNlmeNetDiscConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_LEAVE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbNlmeLeaveConfT *conf, void *arg);

                callback = (void (*)(ZbNlmeLeaveConfT *conf, void *arg))info->callback;
                callback((ZbNlmeLeaveConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_SYNC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbNlmeSyncConfT *conf, void *arg);

                callback = (void (*)(ZbNlmeSyncConfT *discConf, void *arg))info->callback;
                callback((ZbNlmeSyncConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_NLME_ROUTE_DISC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
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
            if (info->callback != NULL) {
                void (*callback)(ZbZdoNwkAddrRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoNwkAddrRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoNwkAddrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_IEEE_ADDR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoIeeeAddrRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoIeeeAddrRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoIeeeAddrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_NODE_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoNodeDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoNodeDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoNodeDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_POWER_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoPowerDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoPowerDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoPowerDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_SIMPLE_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoSimpleDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoSimpleDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoSimpleDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_ACTIVE_EP_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoActiveEpRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoActiveEpRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoActiveEpRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MATCH_DESC_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoMatchDescRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoMatchDescRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoMatchDescRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB:
            if (zdo_match_multi_cb != NULL) {
                assert(p_notification->Size == 2);
                zdo_match_multi_cb((ZbZdoMatchDescRspT *)p_notification->Data[0], (void *)p_notification->Data[1]);
                /* Don't release zdo_match_multi_cb */
            }
            break;

        case MSG_M0TOM4_ZDO_BIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoBindRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoBindRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_UNBIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoBindRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoBindRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_LQI_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoLqiRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoLqiRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoLqiRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_BIND_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoMgmtBindRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoMgmtBindRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoMgmtBindRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_LEAVE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoLeaveRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoLeaveRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoLeaveRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoPermitJoinRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoPermitJoinRspT *rsp, void *cbarg))info->callback;
                callback((ZbZdoPermitJoinRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZdoNwkUpdateNotifyT *rsp, void *cbarg);

                callback = (void (*)(ZbZdoNwkUpdateNotifyT *rsp, void *cbarg))info->callback;
                callback((ZbZdoNwkUpdateNotifyT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB:
        {
            struct ZbZclClusterT *clusterPtr;

            assert(p_notification->Size == 5);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
            if (clusterPtr->alarm_reset_callback != NULL) {
                clusterPtr->alarm_reset_callback(clusterPtr, (uint8_t)p_notification->Data[1],
                    (uint16_t)p_notification->Data[2], (ZbApsdeDataIndT *)p_notification->Data[3],
                    (struct ZbZclHeaderT *)p_notification->Data[4]);
            }
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_COMMAND_CB:
        {
            struct ZbZclClusterT *clusterPtr;
            enum ZclStatusCodeT status = ZCL_STATUS_SUCCESS;
#ifdef PATCH_MEM1
#else
            enum ZclStatusCodeT *retptr;
#endif /* PATCH_MEM1 */

            assert(p_notification->Size == 4);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
#ifdef PATCH_MEM1
#else
            retptr = (enum ZclStatusCodeT *)p_notification->Data[3];
#endif /* PATCH_MEM1 */
            if (clusterPtr->command != NULL) {
                status = clusterPtr->command(clusterPtr, (struct ZbZclHeaderT *)p_notification->Data[1],
                    (ZbApsdeDataIndT *)p_notification->Data[2]);
            }
#ifdef PATCH_MEM1
            /******************************************************************************************/
            /*  M4                                         M0                                         */
            /*  |                                           |     Fill data[0],data[x], etc..         */
            /*  |                                           |                                         */
            /*  |                                           |      Call to zigbee_m0_send_notify      */
            /*  |<-------------Notif (data[x])------------- |                                         */
            /*  |                                           |                                         */
            /*  |Fill the data[0]                           |                                         */
            /*  |(contain the status of the notification    |                                         */
            /*  | handler)                                  |   zigbee_m0_retrieve_notif_data0()      */
            /*  |                                           |                                         */
            /******************************************************************************************/

             p_notification->Data[0] = status; /* This parameter is filled by the M4 but is read back
                                                  by the M0 once the M0 has send the notification to
                                                  the M4*/
#else
            if (retptr != NULL) {
                *retptr = status;
            }
#endif /* PATCH_MEM1 */		
			
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_CONFIG_CB:
        {
            struct ZbZclClusterT *clusterPtr;

            assert(p_notification->Size == 5);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
            if (clusterPtr->config != NULL) {
                clusterPtr->config(clusterPtr, (ZbApsdeDataIndT *)p_notification->Data[1],
                    (uint16_t)p_notification->Data[2], (enum ZclStatusCodeT)p_notification->Data[3],
                    (uint8_t)p_notification->Data[4]);
            }
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_REPORT_CB:
        {
            struct ZbZclClusterT *clusterPtr;

            assert(p_notification->Size == 6);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
            if (clusterPtr->report != NULL) {
                clusterPtr->report(clusterPtr, (ZbApsdeDataIndT *)p_notification->Data[1],
                    (uint16_t)p_notification->Data[2], (enum ZclDataTypeT)p_notification->Data[3],
                    (const uint8_t *)p_notification->Data[4], (uint16_t)p_notification->Data[5]);
            }
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_GET_SCENE_CB:
        {
            struct ZbZclClusterT *clusterPtr;
#ifdef PATCH_MEM1
            uint8_t retval = 0U;
#else
            uint8_t *retptr, retval = 0U;
#endif /*PATCH_MEM1 */

            assert(p_notification->Size == 4);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
#ifdef PATCH_MEM1
#else
            retptr = (uint8_t *)p_notification->Data[3];
#endif /*PATCH_MEM1 */
            if (clusterPtr->get_scene_data != NULL) {
                retval = clusterPtr->get_scene_data(clusterPtr, (uint8_t *)p_notification->Data[1],
                    (uint8_t)p_notification->Data[2]);
            }
#ifdef PATCH_MEM1
            /******************************************************************************************/
            /*  M4                                         M0                                         */
            /*  |                                           |     Fill data[0],data[x], etc..         */
            /*  |                                           |                                         */
            /*  |                                           |      Call to zigbee_m0_send_notify      */
            /*  |<-------------Notif (data[x])------------- |                                         */
            /*  |                                           |                                         */
            /*  |Fill the data[0]                           |                                         */
            /*  |(contain the status of the notification    |                                         */
            /*  | handler)                                  |   zigbee_m0_retrieve_notif_data0()      */
            /*  |                                           |                                         */
            /******************************************************************************************/

             p_notification->Data[0] = retval; /* This parameter is filled by the M4 but is read back
                                                  by the M0 once the M0 has send the notification to
                                                  the M4*/
#else
            if (retptr != NULL) {
                *retptr = retval;
            }
#endif /* PATCH_MEM1 */
            break;

        }

        case MSG_M0TOM4_ZCL_CLUSTER_SET_SCENE_CB:
        {
            struct ZbZclClusterT *clusterPtr;
#ifdef PATCH_MEM1
#else
            enum ZclStatusCodeT *retptr;
#endif /* PATCH_MEM1 */
            enum ZclStatusCodeT retval = ZCL_STATUS_SUCCESS;

            assert(p_notification->Size == 5);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
#ifdef PATCH_MEM1
#else
            retptr = (enum ZclStatusCodeT *)p_notification->Data[4];
#endif /* PATCH_MEM1 */
            if (clusterPtr->set_scene_data != NULL) {
                retval = clusterPtr->set_scene_data(clusterPtr, (uint8_t *)p_notification->Data[1],
                    (uint8_t)p_notification->Data[2], (uint16_t)p_notification->Data[3]);
            }
#ifdef PATCH_MEM1
            /******************************************************************************************/
            /*  M4                                         M0                                         */
            /*  |                                           |     Fill data[0],data[x], etc..         */
            /*  |                                           |                                         */
            /*  |                                           |      Call to zigbee_m0_send_notify      */
            /*  |<-------------Notif (data[x])------------- |                                         */
            /*  |                                           |                                         */
            /*  |Fill the data[0]                           |                                         */
            /*  |(contain the status of the notification    |                                         */
            /*  | handler)                                  |   zigbee_m0_retrieve_notif_data0()      */
            /*  |                                           |                                         */
            /******************************************************************************************/

             p_notification->Data[0] = retval; /* This parameter is filled by the M4 but is read back
                                                  by the M0 once the M0 has send the notification to
                                                  the M4*/
#else
            if (retptr != NULL) {
                *retptr = retval;
            }
            break;
#endif /* PATCH_MEM1 */
        }

        case MSG_M0TOM4_ZCL_CLUSTER_CLEANUP_CB:
        {
            struct ZbZclClusterT *clusterPtr;

            assert(p_notification->Size == 1);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
            if (clusterPtr->cleanup != NULL) {
                clusterPtr->cleanup(clusterPtr);
            }
            break;
        }

        case MSG_M0TOM4_ZCL_CLUSTER_CMD_REQ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(struct ZbZclCommandRspT *rsp, void *cbarg);

                callback = (void (*)(struct ZbZclCommandRspT *rsp, void *cbarg))info->callback;
                callback((struct ZbZclCommandRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbApsdeDataConfT *conf, void *arg);

                callback = (void (*)(ZbApsdeDataConfT *conf, void *arg))info->callback;
                callback((ZbApsdeDataConfT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_READ_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZclReadRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZclReadRspT *rsp, void *cbarg))info->callback;
                callback((ZbZclReadRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_WRITE_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZclWriteRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZclWriteRspT *rsp, void *cbarg))info->callback;
                callback((ZbZclWriteRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB:
            assert(p_notification->Size == 2);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[1];
            if (info->callback != NULL) {
                void (*callback)(ZbZclDiscoverAttrRspT *rsp, void *cbarg);

                callback = (void (*)(ZbZclDiscoverAttrRspT *rsp, void *cbarg))info->callback;
                callback((ZbZclDiscoverAttrRspT *)p_notification->Data[0], info->arg);
            }
            break;

        case MSG_M0TOM4_ZCL_ATTR_CB:
        {
            struct ZbZclClusterT *clusterPtr;
            struct ZbZclAttrCbInfoT *cb;
            struct ZbZclAttrListEntryT *attr;
            enum ZclStatusCodeT zcl_status;
#ifdef PATCH_MEM1
#else
            enum ZclStatusCodeT *retptr;
#endif /*PATCH_MEM1 */

            assert(p_notification->Size == 3);
            clusterPtr = (struct ZbZclClusterT *)p_notification->Data[0];
            cb = (struct ZbZclAttrCbInfoT *)p_notification->Data[1];
#ifdef PATCH_MEM1
#else
            retptr = (enum ZclStatusCodeT *)p_notification->Data[2];
#endif /*PATCH_MEM1 */
            attr = ZbZclAttrFind(clusterPtr, cb->info->attributeId);
            if (attr == NULL) {
#ifdef PATCH_MEM1
                /******************************************************************************************/
                /*  M4                                         M0                                         */
                /*  |                                           |     Fill data[0],data[x], etc..         */
                /*  |                                           |                                         */
                /*  |                                           |      Call to zigbee_m0_send_notify      */
                /*  |<-------------Notif (data[x])------------- |                                         */
                /*  |                                           |                                         */
                /*  |Fill the data[0]                           |                                         */
                /*  |(contain the status of the notification    |                                         */
                /*  | handler)                                  |   zigbee_m0_retrieve_notif_data0()      */
                /*  |                                           |                                         */
                /******************************************************************************************/

                p_notification->Data[0] = ZCL_STATUS_SUCCESS; /* This parameter is filled by the M4 but is read back
                                                                 by the M0 once the M0 has send the notification to
                                                                 the M4*/
#else
                if (retptr != NULL) {
                    *retptr = ZCL_STATUS_SUCCESS;
                }
#endif /* PATCH_MEM1 */
                break;
            }

            zcl_status = ZCL_STATUS_SUCCESS;
            switch (cb->type) {
                case ZCL_ATTR_CB_TYPE_READ:
                    if ((attr->info->flags & ZCL_ATTR_FLAG_CB_READ) != 0U) {
                        zcl_status = zcl_attr_callback_exec(clusterPtr, attr, cb);
                    }
                    else {
                        zcl_status = ZbZclAttrDefaultRead(clusterPtr, attr, cb->zcl_data, cb->zcl_len);
                    }
                    break;

                case ZCL_ATTR_CB_TYPE_WRITE:
                    if ((attr->info->flags & ZCL_ATTR_FLAG_CB_WRITE) != 0U) {
                        zcl_status = zcl_attr_callback_exec(clusterPtr, attr, cb);
                    }
                    else {
                        zcl_status = ZbZclAttrDefaultWrite(clusterPtr, attr, cb->zcl_data, cb->write_mode);
                        if ((zcl_status == ZCL_STATUS_SUCCESS) && ((attr->info->flags & ZCL_ATTR_FLAG_CB_NOTIFY) != 0U)) {
                            struct ZbZclAttrCbInfoT notify;

                            /* Notify the application that this attribute has been modified
                             * internally by the stack. */
                            (void)ZbMemSet(&notify, 0, sizeof(struct ZbZclAttrCbInfoT));
                            notify.info = attr->info;
                            notify.type = ZCL_ATTR_CB_TYPE_NOTIFY;
                            notify.app_cb_arg = clusterPtr->app_cb_arg;
                            (void)zcl_attr_callback_exec(clusterPtr, attr, &notify);
                        }
                    }
                    break;

                case ZCL_ATTR_CB_TYPE_DEFAULT:
                    if ((attr->info->flags & ZCL_ATTR_FLAG_CB_DEFAULT) != 0U) {
                        zcl_status = zcl_attr_callback_exec(clusterPtr, attr, cb);
                    }
                    else {
                        if (attr->valBuf != NULL) {
                            int len;

                            len = ZbZclAttrDefaultValue(attr->info->dataType, attr->valBuf, attr->valSz);
                            if ((len > 0) && ((attr->info->flags & ZCL_ATTR_FLAG_CB_NOTIFY) != 0U)) {
                                struct ZbZclAttrCbInfoT notify;

                                /* Notify the application that this attribute has been modified
                                 * internally by the stack. */
                                (void)ZbMemSet(&notify, 0, sizeof(struct ZbZclAttrCbInfoT));
                                notify.info = attr->info;
                                notify.type = ZCL_ATTR_CB_TYPE_NOTIFY;
                                notify.app_cb_arg = clusterPtr->app_cb_arg;
                                (void)zcl_attr_callback_exec(clusterPtr, attr, &notify);
                                zcl_status = ZCL_STATUS_SUCCESS;
                            }
                        }
                    }
                    break;

                case ZCL_ATTR_CB_TYPE_NOTIFY:
                    if ((attr->info->flags & ZCL_ATTR_FLAG_CB_NOTIFY) != 0U) {
                        (void)zcl_attr_callback_exec(clusterPtr, attr, cb);
                        zcl_status = ZCL_STATUS_SUCCESS;
                    }
                    break;

                default:
                    zcl_status = ZCL_STATUS_SUCCESS;
                    break;
            }
#ifdef PATCH_MEM1
            /******************************************************************************************/
             /*  M4                                         M0                                         */
             /*  |                                           |     Fill data[0],data[x], etc..         */
             /*  |                                           |                                         */
             /*  |                                           |      Call to zigbee_m0_send_notify      */
             /*  |<-------------Notif (data[x])------------- |                                         */
             /*  |                                           |                                         */
             /*  |Fill the data[0]                           |                                         */
             /*  |(contain the status of the notification    |                                         */
             /*  | handler)                                  |   zigbee_m0_retrieve_notif_data0()      */
             /*  |                                           |                                         */
             /******************************************************************************************/

             p_notification->Data[0] = zcl_status; /* This parameter is filled by the M4 but is read back
                                                      by the M0 once the M0 has send the notification to
                                                      the M4*/
#else
            if (retptr != NULL) {
                *retptr = zcl_status;
            }
#endif /* PATCH_MEM1 */
            break;
        }

        case MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB:
            assert(p_notification->Size == 5);
            info = (struct zb_ipc_m4_cb_info *)p_notification->Data[4];
            if (info->callback != NULL) {
                void (*callback)(uint64_t partnerAddr, uint16_t keSuite, enum ZbZclKeyStatusT key_status, void *arg);
                uint64_t partnerAddr;

                memcpy(&partnerAddr, &p_notification->Data[0], 8);
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

    TL_ZIGBEE_SendAckAfterAppliNotifFromM0();
    return status;
} /* Zigbee_CallBackProcessing */

HAL_StatusTypeDef
Zigbee_LoggingProcessing(void)
{
    const char *log_str;
    Zigbee_Cmd_Request_t *p_logging = ZIGBEE_Get_LoggingPayloadBuffer();

    assert(p_logging->Size == 1);
    log_str = (const char *)p_logging->Data[0];
    cli_port_print_msg(NULL, log_str);

    TL_ZIGBEE_SendAckAfterAppliLoggingFromM0();
    return HAL_OK;
} /* Zigbee_LoggingProcessing */
