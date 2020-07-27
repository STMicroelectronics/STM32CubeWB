/**
 * @file zcl.onoff.h
 * @brief ZCL OnOff cluster header
 * ZCL 7 section 3.8
 * ZCL 8 section 3.8
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 */

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

/** OnOff Server Attribute IDs */
enum ZbZclOnOffSvrAttrT {
    ZCL_ONOFF_ATTR_ONOFF = 0x0000,
    ZCL_ONOFF_ATTR_GLOBAL_SCENE_CONTROL = 0x4000,
    ZCL_ONOFF_ATTR_ON_TIME,
    ZCL_ONOFF_ATTR_OFF_WAIT_TIME,
};

/* OnOff Command IDs (don't include in doxygen) */
enum ZbZclOnOffCmdT {
    ZCL_ONOFF_COMMAND_OFF = 0x00,
    ZCL_ONOFF_COMMAND_ON = 0x01,
    ZCL_ONOFF_COMMAND_TOGGLE = 0x02,
};

/*---------------------------------------------------------------
 * OnOff Server Cluster
 *---------------------------------------------------------------
 */

/** OnOff Server callbacks configuration */
struct ZbZclOnOffServerCallbacksT {
    enum ZclStatusCodeT (*off)(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of an Off command. The application is expected to
    * update ZCL_ONOFF_ATTR_ONOFF. */

    enum ZclStatusCodeT (*on)(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of an On command. The application is expected to
    * update ZCL_ONOFF_ATTR_ONOFF. */

    enum ZclStatusCodeT (*toggle)(struct ZbZclClusterT *cluster, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of a Toggle command. The application is expected to
    * update ZCL_ONOFF_ATTR_ONOFF. */
};

/**
 * Create a new instance of the OnOff Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
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

/**
 * Create a new instance of the OnOff Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclOnOffClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send an Off command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOnOffClientOffReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send an On command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOnOffClientOnReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Toggle command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclOnOffClientToggleReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ZCL_ONOFF_H */
