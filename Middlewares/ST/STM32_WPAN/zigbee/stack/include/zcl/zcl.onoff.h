/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_ONOFF_H
# define ZCL_ONOFF_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL OnOff cluster.
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.OnOff
 *
 * OO.S | True
 * OO.C | True
 *
 * OO.S.A0000 | True
 * OO.S.A0000.Scene | True
 * OO.S.A0000.Report.Tx | True
 *
 * OO.S.A4000 | False
 * OO.S.A4001 | False
 * OO.S.A4002 | False
 * OO.S.A4003 | False
 *
 * OO.S.Afffd | True
 *
 * OO.S.C00.Rsp | True
 * OO.S.C01.Rsp | True
 * OO.S.C02.Rsp | True
 *
 * OO.S.C40.Rsp | False
 * OO.S.C41.Rsp | False
 * OO.S.C42.Rsp | False
 *
 * OO.C.A0000.Report.Rsp | False
 *
 * OO.C.Afffd | True
 *
 * OO.C.C00.Tx | True
 * OO.C.C01.Tx | True
 * OO.C.C02.Tx | True
 *
 * OO.C.C40.Tx | False
 * OO.C.C41.Tx | False
 * OO.C.C42.Tx | False
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "zcl/zcl.h"

/* Attribute Identifiers */
enum {
    ZCL_ONOFF_ATTR_ONOFF = 0x0000,
    ZCL_ONOFF_ATTR_GLOBAL_SCENE_CONTROL = 0x4000,
    ZCL_ONOFF_ATTR_ON_TIME, /* 0x4001 */
    ZCL_ONOFF_OFF_WAIT_TIME, /* 0x4002 */
};

/* Commands */
enum {
    ZCL_ONOFF_COMMAND_OFF = 0x00,
    ZCL_ONOFF_COMMAND_ON = 0x01,
    ZCL_ONOFF_COMMAND_TOGGLE = 0x02,
};

/*---------------------------------------------------------------
 * OnOff Server Cluster
 *---------------------------------------------------------------
 */
struct ZbZclOnOffServerCallbacksT {
    /* The application is expected to update ZCL_ONOFF_ATTR_ONOFF if these callbacks are
     * successfully executed. */
    enum ZclStatusCodeT (*off)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*on)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*toggle)(struct ZbZclClusterT *clusterPtr, struct ZbZclAddrInfoT *srcInfo, void *arg);
};

struct ZbZclClusterT * ZbZclOnOffServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclOnOffServerCallbacksT *callbacks, void *arg);

/* Allow the Level Control Cluster to be notified of OnOff commands */
typedef void (*ZbZclLevelControlCallbackT)(struct ZbZclClusterT *level_cluster, uint8_t on_off_command);
void ZbZclOnOffServerSetLevelControlCallback(struct ZbZclClusterT *on_off_cluster,
    struct ZbZclClusterT *level_cluster, ZbZclLevelControlCallbackT levelControlCallback);

/*---------------------------------------------------------------
 * OnOff Client Cluster
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclOnOffClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Client Cluster Commands */
enum ZclStatusCodeT ZbZclOnOffClientOnReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);
/* Blocking version. Not supported on all platforms. */
enum ZclStatusCodeT ZbZclOnOffClientOnWait(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst);

enum ZclStatusCodeT ZbZclOnOffClientOffReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);
/* Blocking version. Not supported on all platforms. */
enum ZclStatusCodeT ZbZclOnOffClientOffWait(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst);

enum ZclStatusCodeT ZbZclOnOffClientToggleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);
/* Blocking version. Not supported on all platforms. */
enum ZclStatusCodeT ZbZclOnOffClientToggleWait(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ZCL_ONOFF_H */
