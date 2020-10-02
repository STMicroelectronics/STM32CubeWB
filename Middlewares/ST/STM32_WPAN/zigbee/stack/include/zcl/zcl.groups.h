/**
 * @file zcl.groups.h
 * @brief ZCL Groups cluster header
 * ZCL 7 section 3.6
 * ZCL 8 section 3.6
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_GROUPS_H
# define ZCL_GROUPS_H

/* PICS.ZCL.Groups
 *
 * G.S | True
 * G.C | True
 *
 * Server Attributes
 * G.S.A0000 | True
 * G.S.Afffd | True
 *
 * Commands Received
 * G.S.C00.Rsp | True
 * G.S.C01.Rsp | True
 * G.S.C02.Rsp | True
 * G.S.C03.Rsp | True
 * G.S.C04.Rsp | True
 * G.S.C05.Rsp | True
 *
 * Commands Generated
 * G.S.C00.Tx | True
 * G.S.C01.Tx | True
 * G.S.C02.Tx | True
 * G.S.C03.Tx | True
 *
 *
 * Client Attributes
 * G.C.Afffd | True
 *
 * Commands Received
 * G.C.C00.Rsp | True
 * G.C.C01.Rsp | True
 * G.C.C06.Rsp | True
 * G.C.C02.Rsp | True
 *
 * Commands Generated
 * G.C.C00.Tx | True
 * G.C.C01.Tx | True
 * G.C.C02.Tx | True
 * G.C.C03.Tx | True
 * G.C.C04.Tx | True
 * G.C.C05.Tx | True
 *
 */

#include "zcl/zcl.h"

#define ZCL_GROUPS_GET_MEMBERS_MAX          16U /* arbitrary */

/* Groups Server Attribute IDs */
enum ZbZclGroupsSvrAttrT {
    ZCL_GROUPS_ATTR_NAME_SUPPORT = 0x0000,
};

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
 * Instantiate a new instance of the Groups server cluster.
 * @param zb Zigbee stack instance.
 * @param endpoint APS endpoint to create cluster on.
 * @return Cluster pointer, or NULL if there as an error.
 */
struct ZbZclClusterT * ZbZclGroupsServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Instantiate a new instance of the Groups client cluster.
 * @param zb Zigbee stack instance.
 * @param endpoint APS endpoint to create cluster on.
 * @return Cluster pointer, or NULL if there as an error.
 */
struct ZbZclClusterT * ZbZclGroupsClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Client APIs */

/** Add Group Command */
struct ZbZclGroupsClientAddReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};
/**
 * Add group membership in a particluar group for one or more endpoints on the receiving device
 * @param clusterPtr Cluster instance to send this command from
 * @param req Add group request structure
 * @param callback Callback function to call with command response
 * @param arg Application argument that will be included with the callback
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientAddReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientAddReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** View Group Command */
struct ZbZclGroupsClientViewReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};
/**
 * Request receiving entity or entities respond with view group response containing application name string for group
 * @param clusterPtr Cluster instance to send this command from
 * @param req View group request structure
 * @param callback Callback function to call with command response
 * @param arg Application argument that will be included with the callback
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientViewReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientViewReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Get Group Membership Command */
struct ZbZclGroupsClientGetMembershipReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint8_t num_groups; /**< Group count */
    uint16_t group_list[ZCL_GROUPS_GET_MEMBERS_MAX]; /**< Group list */
};

/**
 * Inquire about the group membership of the receiving device and endpoint in a number of ways
 * @param clusterPtr Cluster instance to send this command from
 * @param req Get group membership request structure
 * @param callback Callback function to call with command response
 * @param arg Application argument that will be included with the callback
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientGetMembershipReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientGetMembershipReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Remove Group Command */
struct ZbZclGroupsClientRemoveReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};
/**
 * Request receiving entity or entities remove their membership, if any, in a particular group
 * @param clusterPtr Cluster instance to send this command from
 * @param req Remove group request structure
 * @param callback Callback function to call with command response
 * @param arg Application argument that will be included with the callback
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientRemoveReq(struct ZbZclClusterT *cluster, struct ZbZclGroupsClientRemoveReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Remove All Groups Command */
/**
 * Direct receiving entity or entities to remove all group associations
 * @param clusterPtr Cluster instance to send this command from
 * @param req Remove all groups request structure
 * @param callback Callback function to call with command response
 * @param arg Application argument that will be included with the callback
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientRemoveAllReq(struct ZbZclClusterT *cluster, struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/** Add Group If Identifying Command */
struct ZbZclGroupsClientAddIdentifyingReqT {
    struct ZbApsAddrT dst; /**< Destination Address */
    uint16_t group_id; /**< Group ID */
};
/**
 * Add group membership in a particluar group for one or more endpoints on the receiving device,
 * on the condition that it is identifying itself.
 * @param clusterPtr Cluster instance to send this command from
 * @param req Add group if identifying request structure
 * @param callback Callback function to call with command response
 * @param arg Application argument that will be included with the callback
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclGroupsClientAddIdentifyingReq(struct ZbZclClusterT *cluster,
    struct ZbZclGroupsClientAddIdentifyingReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_GROUPS_H */
