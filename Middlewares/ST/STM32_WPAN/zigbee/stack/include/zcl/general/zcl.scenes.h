/**
 * @file zcl.scenes.h
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 * @heading Scenes
 * @brief ZCL Scenes cluster header
 *
 * ZCL 8 section 3.7
 *
 * A scene is a set of values for attributes from multiple clusters capable of being applied at the
 * same time. The few clusters that support scenes are identified by a section with the title
 * "Scene Table Extensions" in the ZCL 8 Specification (ZCL8) section for those cluster. There is
 * only one scene table (list of attributes) for a cluster that supports scenes, and when a scene
 * is invoked all scene table attributes are set to the values given in the scene table.
 *
 * To use the scene table for a cluster, the cluster must reside on an endpoint  which also hosts an
 * instance of the Scenes cluster. There may be multiple scene table supporting clusters on a given
 * endpoint. A scene is defined in the scenes cluster and contains scene tables for one or more
 * clusters. Through the use of group addressing a scene may be applied to multiple endpoints on a node.
 *
 * A scene may be created by the scene cluster using the Add Scene command, where the application
 * manually defines the scene table for each cluster included in that scene. All attributes must
 * have values in the scene table, but inclusion of individual clusters is optional. A scene may
 * also be created using the Store Scene command where the current value of all the attributes in
 * the cluster at the time the Store Scene command is issued are recorded in the scene table for
 * later use.
 *
 * The Scenes cluster Recall Scene command takes the scene table for each cluster in that scene
 * and sets the values of every scene table attribute.
 *
 * For example, a node could contain three endpoints:
 *
 * * `0x01` with the OnOff and Window Covering clusters
 * * `0x02` with the OnOff and Door Lock clusters
 * * `0x03` with the OnOff and Level.
 *
 * A scene is defined with a scene tables for the:
 *
 * * OnOff cluster: `OnOff = On`
 * * Level cluster: `CurrentLevel = 50%`
 * * DoorLock cluster: `LockState = Locked`
 *
 * Additionally:
 *
 * * Endpoints `0x01` and `0x02` are in group `0x0001`
 * * Endpoint `0x03` is not in group `0x0001`
 *
 * If the scenes cluster Recall Scenes command is issued with group address `0x0001` and the scene
 * defined above, then on endpoint `0x01` and `0x02` the OnOff cluster OnOff attribute will be set
 * on and the DoorLock on endpoint `0x02` will be locked.
 *
 * The Window Covering cluster on endpoint `0x01` will not be affected because this scene does not
 * include a scene table for this cluster and all of endpoint `0x03` will be unaffected because it
 * is not in group `0x0001`.
 *
 * For more information about the Scenes cluster, see Section 3.7 in ZCL8.
 */

/* @PICS.ZCL.Scenes
 *
 * S.S | Server | True
 * S.C | Client | True
 *
 * Server Attributes
 * S.S.A0000 | SceneCount | True
 * S.S.A0001 | CurrentScene | True
 * S.S.A0002 | CurrentGroup | True
 * S.S.A0003 | SceneValid | True
 * S.S.A0004 | NameSupport | True
 * S.S.A0005 | LastConfiguredBy | True | Optional
 * S.S.Afffd | ClusterRevision | True
 * S.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * S.S.C00.Rsp | Add Scene | True
 * S.S.C01.Rsp | View Scene | True
 * S.S.C02.Rsp | Remove Scene | True
 * S.S.C03.Rsp | Remove All Scenes | True
 * S.S.C04.Rsp | Store Scene | True
 * S.S.C05.Rsp | Recall Scene | True
 * S.S.C06.Rsp | Get Scene Membership | True
 * S.S.C40.Rsp | Enhanced Add Scene | True
 * S.S.C41.Rsp | Enhanced View Scene | True
 * S.S.C42.Rsp | Copy Scene | True
 *
 * Commands Generated
 * S.S.C00.Tx | Add Scene Response | True
 * S.S.C01.Tx | View Scene Response | True
 * S.S.C02.Tx | Remove Scene Response | True
 * S.S.C03.Tx | Remove All Scenes Response | True
 * S.S.C04.Tx | Store Scene Response | True
 * S.S.C06.Tx | Get Scene Membership Response | True
 * S.S.C40.Tx | Enhanced Add Scene Response | True
 * S.S.C41.Tx | Enhanced View Scene Response | True
 * S.S.C42.Tx | Copy Scene Response | True
 *
 * Client Attributes
 * S.C.Afffd | ClusterRevision | True
 * S.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * S.C.C00.Rsp | Add Scene Response | True
 * S.C.C01.Rsp | View Scene Response | True
 * S.C.C02.Rsp | Remove Scene Response | True
 * S.C.C03.Rsp | Remove All Scenes Response | True
 * S.C.C04.Rsp | Store Scene Response | True
 * S.C.C06.Rsp | Get Scene Membership Response | True
 * S.C.C40.Rsp | Enhanced Add Scene Response | True
 * S.C.C41.Rsp | Enhanced View Scene Response | True
 * S.C.C42.Rsp | Copy Scene Response | True
 *
 * Commands Generated
 * S.C.C00.Tx | Add Scene | True
 * S.C.C01.TX | View Scene | True
 * S.C.C02.Tx | Remove Scene | True
 * S.C.C03.Tx | Remove All Scenes | True
 * S.C.C04.Tx | Store Scene | True
 * S.C.C05.Tx | Recall Scene | True
 * S.C.C06.Tx | Get Scene Membership | True
 * S.C.C40.Tx | Enhanced Add Scene | True
 * S.C.C41.Tx | Enhanced View Scene | True
 * S.C.C42.Tx | Copy Scene | True
 */

#ifndef ZCL_SCENES_H
# define ZCL_SCENES_H

#include "zcl/zcl.h"

/** Scenes Attribute IDs */
enum ZbZclScenesAttrT {
    ZCL_SCENES_ATTR_SCENE_COUNT = 0x0000, /**< SceneCount */
    ZCL_SCENES_ATTR_CURRENT_SCENE = 0x0001, /**< CurrentScene */
    ZCL_SCENES_ATTR_CURRENT_GROUP = 0x0002, /**< CurrentGroup */
    ZCL_SCENES_ATTR_SCENE_VALID = 0x0003, /**< SceneValid */
    ZCL_SCENES_ATTR_NAME_SUPPORT = 0x0004, /**< NameSupport */
    ZCL_SCENES_ATTR_LAST_CONFIGURED_BY = 0x0005, /**< LastConfiguredBy (Optional) */
};

/* Scenes Commands (Request IDs are the same as the Response IDs) */
enum {
    ZCL_SCENES_COMMAND_ADD_SCENE = 0x00,
    ZCL_SCENES_COMMAND_VIEW_SCENE = 0x01,
    ZCL_SCENES_COMMAND_REMOVE_SCENE = 0x02,
    ZCL_SCENES_COMMAND_REMOVE_ALL_SCENES = 0x03,
    ZCL_SCENES_COMMAND_STORE_SCENE = 0x04,
    ZCL_SCENES_COMMAND_RECALL_SCENE = 0x05,
    ZCL_SCENES_COMMAND_GET_SCENE_MBRSHIP = 0x06,
    ZCL_SCENES_COMMAND_ENH_ADD_SCENE = 0x40,
    ZCL_SCENES_COMMAND_ENH_VIEW_SCENE = 0x41,
    ZCL_SCENES_COMMAND_COPY_SCENE = 0x42,
};

/** Scenes Constants */
#define ZCL_SCENES_NAME_MAX_LENGTH                  16
#define ZCL_SCENES_RECALL_TRANSITION_INVALID        0xffffU
#define ZCL_SCENES_NAME_SUPPORT_MASK                0x80U

/* Scenes Server */

/**
 * Create a new instance of the Scenes Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param maxScenes Maximum number of scenes supported by this cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclScenesServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint8_t maxScenes);

/* Scenes Client */

/**
 * Create a new instance of the Scenes Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclScenesClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Scenes - Helper Functions */

/** Add Scene command structure */
struct zcl_scenes_add_request_t {
    bool isEnhanced; /**< If true, send an Enhanced Add Scene request */
    struct ZbApsAddrT dst; /**< Destination address for request */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
    uint16_t transition; /**< Transition time */
    const char *sceneName; /**< Scene Name (string length must be <= ZCL_SCENES_NAME_MAX_LENGTH) */
    const char *extStrPtr;
    /**< Extension field sets as an ASCII hex string in the format as sent
     * in the command payload. As per the ZCL Spec, each set has the format:
     * {clusterId, length, {extension field set}}
     * E.g. For the OnOff cluster: "06000101"
     */
};

/** Add Scene Response command structure */
struct zcl_scenes_add_response_t {
    uint8_t status; /**< Status */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
};

/**
 * Send an Add Scene or Enhanced Add Scene command, depending on isEnhanced flag in Add Scene command structure
 * @param cluster Cluster instance from which to send this command
 * @param add_req Add Scene command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_add_req(struct ZbZclClusterT *cluster, struct zcl_scenes_add_request_t *add_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse an Add Scene Response command payload into a structure
 * @param add_rsp Add Scene Response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_add_rsp_parse(struct zcl_scenes_add_response_t *add_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** View Scene command structure */
struct zcl_scenes_view_request_t {
    bool isEnhanced; /**< If true, send an Enhanced View Scene request */
    struct ZbApsAddrT dst; /**< Destination address for request */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
};

/* EXEGIN - arbitrary */
#define ZCL_SCENES_VIEW_NAME_MAX_LEN                128
#define ZCL_SCENES_VIEW_EXT_FIELD_MAX_LEN           128
#define ZCL_SCENES_VIEW_EXT_LIST_MAX_SZ             8

/** View Scene Response command structure */
struct zcl_scenes_view_response_t {
    uint8_t status; /**< Status */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
    uint16_t transition; /**< Transition time */
    char nameStr[ZCL_SCENES_NAME_MAX_LENGTH + 1U]; /**< Scene Name */
    uint8_t extNum; /**< Number of Extension field sets */
    struct {
        uint16_t clusterId; /**< Cluster ID */
        uint8_t length; /**< Length */
        uint8_t field[ZCL_SCENES_VIEW_EXT_FIELD_MAX_LEN]; /**< Field - ZCL_SCENES_VIEW_EXT_FIELD_MAX_LEN */
    } extList[ZCL_SCENES_VIEW_EXT_LIST_MAX_SZ]; /**< Extension field sets, one per cluster */
};

/**
 * Send an View Scene or Enhanced View Scene command, depending on isEnhanced flag in View Scene command structure
 * @param cluster Cluster instance from which to send this command
 * @param view_req View Scene command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_view_req(struct ZbZclClusterT *cluster, struct zcl_scenes_view_request_t *view_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse a View Scene Response command payload into a structure
 * @param view_rsp View Scene Response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_view_rsp_parse(struct zcl_scenes_view_response_t *view_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Remove Scene command structure */
struct zcl_scenes_remove_request_t {
    struct ZbApsAddrT dst; /**< Destination address for request */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
};

/** Remove Scene Response command structure */
struct zcl_scenes_remove_response_t {
    uint8_t status; /**< Status */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
};

/**
 * Send a Remove Scene command
 * @param cluster Cluster instance from which to send this command
 * @param remove_req Remove Scene command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_remove_req(struct ZbZclClusterT *cluster, struct zcl_scenes_remove_request_t *remove_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse a Remove Scene Response command payload into a structure
 * @param remove_rsp Remove Scene Response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_remove_rsp_parse(struct zcl_scenes_remove_response_t *remove_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Remove All Scenes command structure */
struct zcl_scenes_remove_all_request_t {
    struct ZbApsAddrT dst; /**< Destination address for request */
    uint16_t groupId; /**< Group ID */
};

/** Remove All Scenes Response command structure */
struct zcl_scenes_remove_all_response_t {
    uint8_t status; /**< Status */
    uint16_t groupId; /**< Group ID */
};

/**
 * Send a Remove All Scenes command
 * @param cluster Cluster instance from which to send this command
 * @param remove_req Remove All Scenes command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_remove_all_req(struct ZbZclClusterT *cluster, struct zcl_scenes_remove_all_request_t *remove_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse a Remove All Scenes Response command payload into a structure
 * @param remove_rsp Remove All Scenes Response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_remove_all_rsp_parse(struct zcl_scenes_remove_all_response_t *remove_rsp,
    struct ZbZclCommandRspT *zcl_rsp);

/** Store Scene command structure */
struct zcl_scenes_store_request_t {
    struct ZbApsAddrT dst; /**< Destination address for request */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
};

/** Store Scene Response command structure */
struct zcl_scenes_store_response_t {
    uint8_t status; /**< Status */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
};

/**
 * Send a Store Scene command
 * @param cluster Cluster instance from which to send this command
 * @param store_req Store Scene command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_store_req(struct ZbZclClusterT *cluster, struct zcl_scenes_store_request_t *store_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse a Store Scene Response command payload into a structure
 * @param store_rsp Store Scene Response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_store_rsp_parse(struct zcl_scenes_store_response_t *store_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Recall Scene command structure */
struct zcl_scenes_recall_request_t {
    struct ZbApsAddrT dst; /**< Destination address for request */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneId; /**< Scene ID */
    uint16_t transition; /**< Transition time - time in 1/10ths of second.
        ZCL_SCENES_RECALL_TRANSITION_INVALID (0xffff) means invalid, and won't be included. (Optional) */
};

/** Recall Scene Response command structure */
struct zcl_scenes_recall_response_t {
    uint8_t status; /**< Status */
};

/**
 * Send a Recall Scene command
 * @param cluster Cluster instance from which to send this command
 * @param recall_req Recall Scene command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_recall_req(struct ZbZclClusterT *cluster, struct zcl_scenes_recall_request_t *recall_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse a Recall Scene Response command payload into a structure
 * @param recall_rsp Recall Scene response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_recall_rsp_parse(struct zcl_scenes_recall_response_t *recall_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Get Scene Membership command structure */
struct zcl_scenes_membership_request_t {
    struct ZbApsAddrT dst; /**< Destination address for request */
    uint16_t groupId; /**< Group ID */
};

/* EXEGIN - arbitrary */
#define ZCL_SCENES_GET_MEMBERSHIP_MAX_SCENES        128

/** Get Scene Membership Response command structure */
struct zcl_scenes_membership_response_t {
    uint8_t status; /**< Status */
    uint8_t capacity; /**< Capacity */
    uint16_t groupId; /**< Group ID */
    uint8_t sceneCount; /**< Scent count */
    uint8_t sceneList[ZCL_SCENES_GET_MEMBERSHIP_MAX_SCENES]; /**< Scene list */
};

/**
 * Send a Get Scene Membership command
 * @param cluster Cluster instance from which to send this command
 * @param get_req Get Scene Membership command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_get_membership_req(struct ZbZclClusterT *cluster, struct zcl_scenes_membership_request_t *get_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse a Get Scene Membership Response command payload into a structure
 * @param get_rsp Get Scene Membership Response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_get_membership_rsp_parse(struct zcl_scenes_membership_response_t *get_rsp,
    struct ZbZclCommandRspT *zcl_rsp);

/** Copy Scene command structure */
struct zcl_scenes_copy_request_t {
    struct ZbApsAddrT dst; /**< Destination address for request */
    bool allScenes; /**< Copy All Scenes - sets bit within Mode parameter to enable Copy All Scenes */
    uint16_t groupFrom; /**< Group identifier from  */
    uint8_t sceneFrom; /**< Scene identifier from - only used if allScenes is FALSE */
    uint16_t groupTo; /**< Group identifier to  */
    uint8_t sceneTo; /**< Scene identifier to - only used if allScenes is FALSE */
};

/** Copy Scene Response command structure */
struct zcl_scenes_copy_response_t {
    uint8_t status; /**< Status */
    uint16_t groupFrom; /**< Group identifier from */
    uint8_t sceneFrom; /**< Scene identifier from */
};

/**
 * Send a Copy Scene command
 * @param cluster Cluster instance from which to send this command
 * @param copy_req Copy Scene command structure
 * @param callback Callback function that will be called when the response for this request is received
 * @param arg Pointer to application data provided in initiating API call
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_copy_req(struct ZbZclClusterT *cluster, struct zcl_scenes_copy_request_t *copy_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Parse a Copy Scene Response command payload into a structure
 * @param copy_rsp Copy Scene response command structure
 * @param zcl_rsp Cluster response structure
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_scenes_client_copy_rsp_parse(struct zcl_scenes_copy_response_t *copy_rsp, struct ZbZclCommandRspT *zcl_rsp);

#endif
