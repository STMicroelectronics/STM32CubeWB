/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

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

/* Attribute Identifiers */
enum {
    ZCL_GROUPS_ATTR_NAME_SUPPORT = 0x0000,
};

/* Commands */
enum {
    ZCL_GROUPS_COMMAND_ADD = 0x00,
    ZCL_GROUPS_COMMAND_VIEW = 0x01,
    ZCL_GROUPS_COMMAND_GET_MEMBERSHIP = 0x02,
    ZCL_GROUPS_COMMAND_REMOVE = 0x03,
    ZCL_GROUPS_COMMAND_REMOVE_ALL = 0x04,
    ZCL_GROUPS_COMMAND_ADD_IDENTIFYING = 0x05,
};

typedef void (*ZbZclGroupsServerCallbackT)(void *arg, uint8_t state);
struct ZbZclClusterT * ZbZclGroupsServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclGroupClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Client APIs */

struct ZbZclGroupsClientAddReqT {
    struct ZbApsAddrT dst;
    uint16_t group_id;
};
enum ZclStatusCodeT ZbZclGroupsClientAddReq(struct ZbZclClusterT *clusterPtr, struct ZbZclGroupsClientAddReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclGroupsClientViewReqT {
    struct ZbApsAddrT dst;
    uint16_t group_id;
};
enum ZclStatusCodeT ZbZclGroupsClientViewReq(struct ZbZclClusterT *clusterPtr, struct ZbZclGroupsClientViewReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclGroupsClientGetMembershipReqT {
    struct ZbApsAddrT dst;
    uint8_t num_groups;
    uint16_t group_list[ZCL_GROUPS_GET_MEMBERS_MAX];
};
enum ZclStatusCodeT ZbZclGroupsClientGetMembershipReq(struct ZbZclClusterT *clusterPtr, struct ZbZclGroupsClientGetMembershipReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclGroupsClientRemoveReqT {
    struct ZbApsAddrT dst;
    uint16_t group_id;
};
enum ZclStatusCodeT ZbZclGroupsClientRemoveReq(struct ZbZclClusterT *clusterPtr, struct ZbZclGroupsClientRemoveReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclGroupsClientRemoveAllReqT {
    struct ZbApsAddrT dst;
    uint8_t num_groups;
    uint16_t group_list[ZCL_GROUPS_GET_MEMBERS_MAX];
};
enum ZclStatusCodeT ZbZclGroupsClientRemoveAllReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclGroupsClientRemoveAllReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

struct ZbZclGroupsClientAddIdentifyingReqT {
    struct ZbApsAddrT dst;
    uint16_t group_id;
};
enum ZclStatusCodeT ZbZclGroupsClientAddIdentifyingReq(struct ZbZclClusterT *clusterPtr,
    struct ZbZclGroupsClientAddIdentifyingReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_GROUPS_H */
