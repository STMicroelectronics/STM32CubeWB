/**
 * @file zcl.groups.h
 * @heading Groups
 * @brief ZCL Groups cluster header
 * ZCL 7 section 3.6
 * ZCL 8 section 3.6
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_GROUPS_H
# define ZCL_GROUPS_H

/* @PICS.ZCL.Groups
 *
 * G.S | Server | True
 * G.C | Client | True
 *
 * Server Attributes
 * G.S.A0000 | NameSupport | True
 * G.S.Afffd | ClusterRevision | True
 *
 * Commands Received
 * G.S.C00.Rsp | Add group | True
 * G.S.C01.Rsp | View group | True
 * G.S.C02.Rsp | Get group membership | True
 * G.S.C03.Rsp | Remove group | True
 * G.S.C04.Rsp | Remove all groups | True
 * G.S.C05.Rsp | Add group if identifying | True
 *
 * Commands Generated
 * G.S.C00.Tx | Add group response | True
 * G.S.C01.Tx | View group response | True
 * G.S.C02.Tx | Get group membership response | True
 * G.S.C03.Tx | Remove group response | True
 *
 * Client Attributes
 * G.C.Afffd | ClusterRevision | True
 *
 * Commands Received
 * G.C.C00.Rsp | Add group response | True
 * G.C.C01.Rsp | View group response | True
 * G.C.C06.Rsp | Get group membership response | True
 * G.C.C02.Rsp | Remove group response | True
 *
 * Commands Generated
 * G.C.C00.Tx | Add group | True
 * G.C.C01.Tx | View group | True
 * G.C.C02.Tx | Get group membership | True
 * G.C.C03.Tx | Remove group | True
 * G.C.C04.Tx | Remove all groups | True
 * G.C.C05.Tx | Add group if identifying | True
 */

#include "zcl/zcl.h"

/* This number is arbitrary and is only used in the ZCL Groups Get Group Membership request
 * command. The safe max for this define 26, so that the request would not be fragmented.*/
#define ZCL_GROUPS_GET_MEMBERS_MAX          16U

/** Groups Server Attribute IDs */
enum ZbZclGroupsSvrAttrT {
    ZCL_GROUPS_ATTR_NAME_SUPPORT = 0x0000, /**< NameSupport */
};

#define ZCL_GROUPS_NAME_SUPPORT_MASK        0x80U

/* Groups Command IDs */
enum {
    ZCL_GROUPS_COMMAND_ADD = 0x00,
    ZCL_GROUPS_COMMAND_VIEW = 0x01,
    ZCL_GROUPS_COMMAND_GET_MEMBERSHIP = 0x02,
    ZCL_GROUPS_COMMAND_REMOVE = 0x03,
    ZCL_GROUPS_COMMAND_REMOVE_ALL = 0x04,
    ZCL_GROUPS_COMMAND_ADD_IDENTIFYING = 0x05,
};

/**
 * Create a new instance of the Groups Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclGroupsServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Create a new instance of the Groups Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclGroupsClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Client APIs */

/** Add Group command structure */
struct ZbZclGroupsClientAddReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};

/**
 * Send an Add Group command
 * @param cluster Cluster instance from which to send this command
 * @param req Add Group command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientAddReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientAddReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** View Group command structure */
struct ZbZclGroupsClientViewReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};
/**
 * Send a View Group command
 * @param cluster Cluster instance from which to send this command
 * @param req View Group command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientViewReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientViewReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get Group Membership command structure */
struct ZbZclGroupsClientGetMembershipReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint8_t num_groups; /**< Group count */
    uint16_t group_list[ZCL_GROUPS_GET_MEMBERS_MAX]; /**< Group list */
};
/**
 * Send a Get Group Membership command
 * @param cluster Cluster instance from which to send this command
 * @param req Get Group Membership command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientGetMembershipReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientGetMembershipReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Remove Group command structure */
struct ZbZclGroupsClientRemoveReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};
/**
 * Send a Remove Group command
 * @param cluster Cluster instance from which to send this command
 * @param req Remove Group command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientRemoveReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientRemoveReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Remove All Groups command
 * @param cluster Cluster instance from which to send this command
 * @param req Remove All Group command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientRemoveAllReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Add Group If Identifying command structure */
struct ZbZclGroupsClientAddIdentifyingReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};
/**
 * Send an Add Group If Identifying command
 * @param cluster Cluster instance from which to send this command
 * @param req Add Group If Identifying command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientAddIdentifyingReq(struct ZbZclClusterT *cluster,
    struct ZbZclGroupsClientAddIdentifyingReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_GROUPS_H */
