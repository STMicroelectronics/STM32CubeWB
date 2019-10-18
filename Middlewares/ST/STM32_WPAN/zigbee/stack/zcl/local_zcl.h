/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/*-------------------------------------------------
 *  DESCRIPTION
 *      Internal header file for the ZigBee Cluster
 *      Library.
 *-------------------------------------------------
 */
#ifndef LOCAL_ZCL_H
#define LOCAL_ZCL_H

#include "zigbee.h"
#include "zcl/zcl.h"

struct ZbTimerT;

#ifndef ZclAssert
# include <assert.h>
# define ZclAssert assert
#endif

/*---------------------------------------------------------------
 * Debug Logging
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

/* Default cluster TX Options (NWK layer security, APS ACK, fragmentation allowed) */
#define ZCL_CLUSTER_TXOPTIONS_DEFAULT \
    (ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY | \
     ZB_APSDE_DATAREQ_TXOPTIONS_NWKKEY | \
     ZB_APSDE_DATAREQ_TXOPTIONS_ACK | \
     ZB_APSDE_DATAREQ_TXOPTIONS_FRAG)

/* Default SE cluster TX Options (same as default above, but APS security is enabled) */
#define ZCL_CLUSTER_TXOPTIONS_SE \
    (ZB_APSDE_DATAREQ_TXOPTIONS_SECURITY | \
     ZB_APSDE_DATAREQ_TXOPTIONS_ACK | \
     ZB_APSDE_DATAREQ_TXOPTIONS_FRAG)

uint16_t zcl_rsp_txoptions(enum ZbStatusCodeT security_status);

#define ZCL_ATTRIBUTE_BUFFER_SIZE        256U

/* Our internal Manufacturer Code (ZCL_FRAMECTRL_MANUFACTURER).
 * This code is never sent over the air. */
enum {
    ZCL_MANUF_CODE_INTERNAL = 0xfffe /* FIXME - arbitrary */
};

/* Exegin's Manufacturer Specific Global Commands
 * ZCL_FRAMECTRL_MANUFACTURER && ZCL_FRAMETYPE_PROFILE && (Manufacturer Code == ZCL_MANUF_CODE_INTERNAL) */
enum {
    ZCL_CMD_MANUF_INTERNAL_ATTR_RESET_DEFAULT, /* attr_reset() */

    /* For Scene Store Command (ZCL_SCENES_COMMAND_STORE_SCENE)
     * Payload for Request (to Server): None
     * Payload for Response: [CLUSTER(2) | EXT_LEN(1) | EXT_ATTR_DATA(N)] */
    ZCL_CMD_MANUF_INTERNAL_GET_SCENE_EXTDATA, /* get_scene_data() */

    /* For Scene Recall Command (ZCL_SCENES_COMMAND_RECALL_SCENE)
     * Payload for Request (to Server): [EXT_LEN(1) | EXT_ATTR_DATA(N)]
     * Payload for Response: [STATUS(1)] */
    ZCL_CMD_MANUF_INTERNAL_SET_SCENE_EXTDATA, /* set_scene_data() */

    /* ZCL Cluster Persistence: Profile-Wide Manufacturer-Specific command
     * sent to a cluster to restore persistence. */
    ZCL_CMD_MANUF_INTERNAL_ATTR_PERSIST_SET
};

/* Offsets into ZCL_CMD_MANUF_INTERNAL_SET_SCENE_EXTDATA message */
#define SET_SCENE_EXTDATA_OFFSET_EXT_LEN                4U
#define SET_SCENE_EXTDATA_OFFSET_EXT_FIELD              5U
#define SET_SCENE_EXTDATA_HEADER_LEN                    (4U + 1U)

/*---------------------------------------------------------------------------
 * ZCL Attributes
 *---------------------------------------------------------------------------
 */

/* The internal allocated attribute struct */
struct ZbZclAttrListEntryT {
    struct LinkListT link;
    const struct ZbZclAttrT *info; /* Attribute info */
    /* Internal Callback - used by the stack only for IPC, if applicable.
     * Can't override info->callback because it's a const and may be stored in Flash. */
    enum ZclStatusCodeT (*internal_callback)(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrCbInfoT *info);
    uint8_t *valBuf; /* ZCL format (i.e. same as what is sent over-the-air) */
    unsigned int valSz; /* Allocation size of valBuf. */
#ifndef CONFIG_ZB_ZCL_MINIMAL
    struct {
        uint16_t interval_secs_min; /* seconds */
        uint16_t interval_secs_max; /* seconds */
    } reporting;
#endif
};

enum ZclStatusCodeT zcl_attr_callback_exec(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *attrPtr, struct ZbZclAttrCbInfoT *cb);

void ZbZclAttrFreeList(struct ZbZclClusterT *clusterPtr);
enum ZclStatusCodeT ZbZclAttrDefaultRead(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *attrPtr, uint8_t *data, unsigned int data_len);
enum ZclStatusCodeT ZbZclAttrDefaultWrite(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *attrPtr, const uint8_t *data, ZclWriteModeT mode);
void ZbZclAttrPostWrite(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *attrPtr);
bool ZbZclAttrIntegerRangeCheck(struct ZbZclAttrListEntryT *attrPtr, const uint8_t *attr_data, unsigned int attr_len);

/* Handlers for response commands. */
void ZbZclCallbackTick(struct ZbZclClusterT *clusterPtr);
void ZbZclCallbackCleanup(struct ZbZclClusterT *clusterPtr);

/* Handlers for the global commands. */
void ZbZclHandleReadAttr(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);
void ZbZclHandleWriteAttr(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);
void ZbZclAttrHandleDiscover(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);

/* Functions for attribute reporting. */
void ZbZclReportTick(struct ZbZclClusterT *clusterPtr);
void ZbZclReportCleanup(struct ZbZclClusterT *clusterPtr);
enum ZclStatusCodeT zcl_reporting_config_default(struct ZbZclClusterT *clusterPtr);
void ZbZclHandleConfigReport(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);
void ZbZclHandleConfigResponse(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *dataIndPtr);
void ZbZclHandleReadReport(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);
void ZbZclHandleReportAttr(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);

/* Functions for structured reading/writing. */
void ZbZclHandleReadStruct(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);
void ZbZclHandleWriteStruct(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *dataIndPtr);

/* ZCL command state stuff. */
#define ZCL_STATE_CONTINUE   0 /* Return zero to continue waiting for responses. */
#define ZCL_STATE_CLEANUP    1 /* Return one to cleanup the state. */
typedef int (*ZbZclHandlerFuncT)(struct ZbZclCommandRspT *cmdRsp, void (*callback)(void *msg, void *arg), void *arg);

void ZbZclStateBegin(struct ZigBeeT *zb, ZbApsdeDataReqT *apsReq, struct ZbZclHeaderT *zclHdr,
    unsigned int timeout, ZbZclHandlerFuncT handler,
    void (*callback)(void *msg, void *arg), void *arg);

/* A callback to use when waiting for a ZCL response.
 * "struct ZbZclCommandWakeInfoT" is allocated by caller and pointer
 * passed as callback "arg". Semaphore is posted upon response reception. */
struct ZbZclCommandWakeInfoT {
    struct ZbSemT *sem;
    struct ZbZclCommandRspT *rsp;
};
void zcl_command_rsp_wakeup(struct ZbZclCommandRspT *rsp, void *arg);

/* ZCL Attributes */
enum ZclDataTypeT ZbZclAttrType(struct ZbZclClusterT *clusterPtr, uint16_t attrId);
unsigned int ZbZclAttrTypeLength(enum ZclDataTypeT type);
void ZbZclAttrAddSorted(struct ZbZclClusterT *clusterPtr, struct ZbZclAttrListEntryT *new_entry);
struct ZbZclAttrListEntryT * ZbZclAttrFind(struct ZbZclClusterT *clusterPtr, uint16_t attrId);
unsigned int ZbZclAttrDiscoverGetList(struct ZbZclClusterT *clusterPtr, uint16_t start_attr, uint8_t *max_num_attr, uint8_t *buf, unsigned int max_len);

int ZbZclAttrDefaultValue(enum ZclDataTypeT type, uint8_t *buf, unsigned int max_len);

void ZbZclAttrResetAllClusters(struct ZigBeeT *zb);

/* ZCL Basic Server */
struct ZbZclClusterT * zcl_basic_server_alloc(struct ZigBeeT *zb, uint8_t endpoint, bool is_first);
uint8_t zcl_basic_server_alarm_mask(struct ZbZclClusterT *clusterPtr);

/* ZCL Scene Callbacks */
void ZbZclHandleGetSceneData(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);
void ZbZclHandleSetSceneData(struct ZbZclClusterT *clusterPtr, struct ZbZclHeaderT *zclHdrPtr, ZbApsdeDataIndT *ind);

/* ZCL Persistence */
struct ZbZclClusterT * zcl_persist_server_alloc(struct ZigBeeT *zb);
unsigned int zcl_persist_server_get(struct ZbZclClusterT *clusterPtr, uint8_t *buf, unsigned int max_len);
enum ZclStatusCodeT zcl_persist_restore(struct ZigBeeT *zb, const uint8_t *buf, uint16_t len);
enum ZclStatusCodeT zcl_persist_handle_set(struct ZbZclClusterT *clusterPtr, uint8_t *buf, uint16_t len);

void zcl_persist_cluster_timer(struct ZigBeeT *zb, void *arg);

enum ZclStatusCodeT zcl_cluster_bind(struct ZbZclClusterT *clusterPtr, struct ZbApsFilterT *filter, uint8_t endpoint, uint16_t profileId, enum ZbZclDirectionT direction);

bool zcl_reporting_remove(struct ZbZclClusterT *clusterPtr, uint16_t attributeId, enum ZbZclReportDirectionT direction);

/* Helper function to check incoming packet security against the
 * cluster's minimum security level allowed. */
bool ZbZclClusterCheckMinSecurity(struct ZbZclClusterT *clusterPtr, ZbApsdeDataIndT *dataIndPtr, struct ZbZclHeaderT *zclHdrPtr);

bool ZbZclDeviceLogCheckAllow(struct ZbZclClusterT *clusterPtr, ZbApsdeDataIndT *dataIndPtr, struct ZbZclHeaderT *zclHdrPtr);

#endif /* LOCAL_ZCL_H */
