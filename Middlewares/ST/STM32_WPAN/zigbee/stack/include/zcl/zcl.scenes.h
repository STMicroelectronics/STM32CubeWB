/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_SCENES_H
# define ZCL_SCENES_H

#include "zcl/zcl.h"

/* PICS.ZCL.Scenes
 *
 * S.S | True
 * S.C | True
 *
 * Server Attributes
 * S.S.A0000 | True
 * S.S.A0001 | True
 * S.S.A0002 | True
 * S.S.A0003 | True
 * S.S.A0004 | True
 * S.S.A0005 | True
 * S.S.Afffd | True
 *
 * Commands Received
 * S.S.C00.Rsp | True
 * S.S.C01.Rsp | True
 * S.S.C02.Rsp | True
 * S.S.C03.Rsp | True
 * S.S.C04.Rsp | True
 * S.S.C05.Rsp | True
 * S.S.C06.Rsp | True
 * S.S.C40.Rsp | True
 * S.S.C41.Rsp | True
 * S.S.C42.Rsp | True
 *
 * Commands Generated
 * S.S.C00.Tx | True
 * S.S.C01.Tx | True
 * S.S.C02.Tx | True
 * S.S.C03.Tx | True
 * S.S.C04.Tx | True
 * S.S.C06.Tx | True
 * S.S.C40.Tx | True
 * S.S.C41.Tx | True
 * S.S.C42.Tx | True
 *
 *
 * Client Attributes
 * S.C.Afffd | True
 *
 * Commands Received
 * S.C.C00.Rsp | True
 * S.C.C01.Rsp | True
 * S.C.C02.Rsp | True
 * S.C.C03.Rsp | True
 * S.C.C04.Rsp | True
 * S.C.C06.Rsp | True
 * S.C.C40.Rsp | True
 * S.C.C41.Rsp | True
 * S.C.C42.Rsp | True
 *
 * Commands Generated
 * S.C.C00.Tx | True
 * S.C.C01.Tx | True
 * S.C.C02.Tx | True
 * S.C.C03.Tx | True
 * S.C.C04.Tx | True
 * S.C.C05.Tx | True
 * S.C.C06.Tx | True
 * S.C.C40.Tx | True
 * S.C.C41.Tx | True
 * S.C.C42.Tx | True
 */

/** Scenes Attribute IDs */
enum ZbZclScenesAttrT {
    ZCL_SCENES_ATTR_SCENE_COUNT = 0x0000,
    ZCL_SCENES_ATTR_CURRENT_SCENE = 0x0001,
    ZCL_SCENES_ATTR_CURRENT_GROUP = 0x0002,
    ZCL_SCENES_ATTR_SCENE_VALID = 0x0003,
    ZCL_SCENES_ATTR_NAME_SUPPORT = 0x0004,
    ZCL_SCENES_ATTR_LAST_CONFIGURED_BY = 0x0005,
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
#define ZCL_SCENES_RECALL_TRANSITION_INVALID        0xffff

/*---------------------------------------------------------------------------
 * Scenes Server
 *---------------------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclScenesServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint8_t maxScenes);

/*---------------------------------------------------------------------------
 * Scenes Client
 *---------------------------------------------------------------------------
 */

struct ZbZclClusterT * ZbZclScenesClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/** Add Scene command structure */
struct zcl_scenes_add_request_t {
    bool isEnhanced;
    struct ZbApsAddrT dst;
    uint16_t groupId;
    uint8_t sceneId;
    uint16_t transition;
    const char *sceneName;
    const char *extStrPtr;
};

/** Add Scene Response command structure */
struct zcl_scenes_add_response_t {
    uint8_t status;
    uint16_t groupId;
    uint8_t sceneId;
};

enum ZclStatusCodeT zcl_scenes_client_add_req(struct ZbZclClusterT *cluster, struct zcl_scenes_add_request_t *add_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_add_rsp_parse(struct zcl_scenes_add_response_t *add_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** View Scene command structure */
struct zcl_scenes_view_request_t {
    bool isEnhanced;
    struct ZbApsAddrT dst;
    uint16_t groupId;
    uint8_t sceneId;
};

#define ZCL_SCENES_VIEW_NAME_MAX_LEN                128
#define ZCL_SCENES_VIEW_EXT_FIELD_MAX_LEN           128
#define ZCL_SCENES_VIEW_EXT_LIST_MAX_SZ             8

/** View Scene Response command structure */
struct zcl_scenes_view_response_t {
    uint8_t status;
    uint16_t groupId;
    uint8_t sceneId;
    uint16_t transition;
    char nameStr[ZCL_SCENES_NAME_MAX_LENGTH + 1U];
    uint8_t extNum;
    struct {
        uint16_t clusterId;
        uint8_t length;
        uint8_t field[ZCL_SCENES_VIEW_EXT_FIELD_MAX_LEN];
    } extList[ZCL_SCENES_VIEW_EXT_LIST_MAX_SZ];
};

enum ZclStatusCodeT zcl_scenes_client_view_req(struct ZbZclClusterT *cluster, struct zcl_scenes_view_request_t *view_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_view_rsp_parse(struct zcl_scenes_view_response_t *view_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Remove Scene command structure */
struct zcl_scenes_remove_request_t {
    struct ZbApsAddrT dst;
    uint16_t groupId;
    uint8_t sceneId;
};

/** Remove Scene Response command structure */
struct zcl_scenes_remove_response_t {
    uint8_t status;
    uint16_t groupId;
    uint8_t sceneId;
};

enum ZclStatusCodeT zcl_scenes_client_remove_req(struct ZbZclClusterT *cluster, struct zcl_scenes_remove_request_t *remove_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_remove_rsp_parse(struct zcl_scenes_remove_response_t *remove_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Remove All Scenes command structure */
struct zcl_scenes_remove_all_request_t {
    struct ZbApsAddrT dst;
    uint16_t groupId;
};

/** Remove All Scenes Response command structure */
struct zcl_scenes_remove_all_response_t {
    uint8_t status;
    uint16_t groupId;
};

enum ZclStatusCodeT zcl_scenes_client_remove_all_req(struct ZbZclClusterT *cluster, struct zcl_scenes_remove_all_request_t *remove_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_remove_all_rsp_parse(struct zcl_scenes_remove_all_response_t *remove_rsp,
    struct ZbZclCommandRspT *zcl_rsp);

/** Store Scene command structure */
struct zcl_scenes_store_request_t {
    struct ZbApsAddrT dst;
    uint16_t groupId;
    uint8_t sceneId;
};

/** Store Scene Response command structure */
struct zcl_scenes_store_response_t {
    uint8_t status;
    uint16_t groupId;
    uint8_t sceneId;
};

enum ZclStatusCodeT zcl_scenes_client_store_req(struct ZbZclClusterT *cluster, struct zcl_scenes_store_request_t *store_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_store_rsp_parse(struct zcl_scenes_store_response_t *store_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Recall Scene command structure */
struct zcl_scenes_recall_request_t {
    struct ZbApsAddrT dst;
    uint16_t groupId;
    uint8_t sceneId;
    /* [optional] Transition time in tenths. ZCL_SCENES_RECALL_TRANSITION_INVALID
     * (0xffff) means invalid, and won't be included. */
    uint16_t transition;
};

/** Recall Scene Response command structure */
struct zcl_scenes_recall_response_t {
    uint8_t status;
};

enum ZclStatusCodeT zcl_scenes_client_recall_req(struct ZbZclClusterT *cluster, struct zcl_scenes_recall_request_t *recall_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_recall_rsp_parse(struct zcl_scenes_recall_response_t *recall_rsp, struct ZbZclCommandRspT *zcl_rsp);

/** Get Scene Membership command structure */
struct zcl_scenes_membership_request_t {
    struct ZbApsAddrT dst;
    uint16_t groupId;
};

/* EXEGIN - arbitrary */
#define ZCL_SCENES_GET_MEMBERSHIP_MAX_SCENES        128

/** Get Scene Membership Response command structure */
struct zcl_scenes_membership_response_t {
    uint8_t status;
    uint8_t capacity;
    uint16_t groupId;
    uint8_t sceneCount;
    uint8_t sceneList[ZCL_SCENES_GET_MEMBERSHIP_MAX_SCENES];
};

enum ZclStatusCodeT zcl_scenes_client_get_membership_req(struct ZbZclClusterT *cluster, struct zcl_scenes_membership_request_t *get_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_get_membership_rsp_parse(struct zcl_scenes_membership_response_t *get_rsp,
    struct ZbZclCommandRspT *zcl_rsp);

/** Copy Scene command structure */
struct zcl_scenes_copy_request_t {
    struct ZbApsAddrT dst;
    bool allScenes;
    uint16_t groupFrom;
    uint8_t sceneFrom;
    uint16_t groupTo;
    uint8_t sceneTo;
};

/** Copy Scene Response command structure */
struct zcl_scenes_copy_response_t {
    uint8_t status;
    uint16_t groupFrom;
    uint8_t sceneFrom;
};

enum ZclStatusCodeT zcl_scenes_client_copy_req(struct ZbZclClusterT *cluster, struct zcl_scenes_copy_request_t *copy_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

enum ZclStatusCodeT zcl_scenes_client_copy_rsp_parse(struct zcl_scenes_copy_response_t *copy_rsp, struct ZbZclCommandRspT *zcl_rsp);

#endif
