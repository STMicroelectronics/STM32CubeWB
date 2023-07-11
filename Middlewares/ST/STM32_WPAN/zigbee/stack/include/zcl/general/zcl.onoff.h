/**
 * This cluster may be required by other clusters. See ZCL specification
 *
 * This cluster supports scenes functionality. When a scene is recalled that includes this cluster, the attributes listed in the Scenes
 * Table Extensions for this cluster will be set to the value in the scene
 * @file zcl.onoff.h
 * @heading On/Off
 * @brief ZCL OnOff cluster header
 * ZCL 7 section 3.8
 * ZCL 8 section 3.8
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ONOFF_H
# define ZCL_ONOFF_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL OnOff cluster.
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.OnOff
 *
 * OO.S | Server | True
 * OO.C | Client | True
 *
 * Server Attributes
 * OO.S.A0000 | OnOff | True
 * OO.S.A0000.Scene | OnOff | True
 * OO.S.A0000.Report.Tx | OnOff | True
 * OO.S.A4000 | GlobalSceneControl | False | Optional
 * OO.S.A4001 | OnTime | False | Optional
 * OO.S.A4002 | OffWaitTime | False | Optional
 * OO.S.A4003 | StartUpOnOff | False | Optional
 * OO.S.Afffd | ClusterRevision | True
 *
 * Commands Received
 * OO.S.C00.Rsp | Off | True
 * OO.S.C01.Rsp | On | True
 * OO.S.C02.Rsp | Toggle | True
 * OO.S.C40.Rsp | Off with effect | False
 * OO.S.C41.Rsp | On with recall global scene | False
 * OO.S.C42.Rsp | On with timed off | False
 *
 * Client Attributes
 * OO.C.A0000.Report.Rsp | OnOff | False
 * OO.C.Afffd | ClusterRevision | True
 *
 * Commands Generated
 * OO.C.C00.Tx | Off | True
 * OO.C.C01.Tx | On | True
 * OO.C.C02.Tx | Toggle | True
 * OO.C.C40.Tx | Off with effect | False
 * OO.C.C41.Tx | On with recall global scene | False
 * OO.C.C42.Tx | On with timed off | False
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "zcl/zcl.h"

/** OnOff Server Attribute IDs */
enum ZbZclOnOffSvrAttrT {
    ZCL_ONOFF_ATTR_ONOFF = 0x0000, /**< OnOff */
    ZCL_ONOFF_ATTR_GLOBAL_SCENE_CONTROL = 0x4000, /**< GlobalSceneControl (Optional) */
    ZCL_ONOFF_ATTR_ON_TIME, /**< OnTime (Optional) */
    ZCL_ONOFF_ATTR_OFF_WAIT_TIME, /**< OffWaitTime (Optional) */
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

/**
 * Callback to notify Level cluster when an OnOff command is received.
 * @param level_cluster Level cluster instance to be notified
 * @param on_off_command OnOff command ID that was received
 * @return Void
 */
typedef void (*ZbZclLevelControlCallbackT)(struct ZbZclClusterT *level_cluster, uint8_t on_off_command);

/**
 * Set the Level control callback
 * @param on_off_cluster OnOff cluster instance
 * @param level_cluster Level cluster instance
 * @param levelControlCallback Level control callback function
 * @return Void
 */
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
}
#endif

#endif /* ZCL_ONOFF_H */
