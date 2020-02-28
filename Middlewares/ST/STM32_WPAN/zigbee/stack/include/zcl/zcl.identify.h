/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_CORE_IDENTIFY_H
# define ZCL_CORE_IDENTIFY_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Identify cluster.
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Identify Cluster (0x0003)
 *
 * Server PICS
 * I.S | True
 * I.C | True
 *
 * Server Attributes
 * I.S.A0000 | True
 * I.S.Afffd | True
 *
 * Commands Received
 * I.S.C00.Rsp | True
 * I.S.C01.Rsp | True
 * I.S.C40.Rsp | False
 *
 * Commands Generated
 * I.S.C00.Tx | True
 *
 * Client Attributes
 * I.C.Afffd | True
 *
 * Commands Received
 * I.C.C00.Rsp | True
 *
 * Commands Generated
 * I.C.C00.Tx | True
 * I.C.C01.Tx | True
 * I.C.C40.Tx | False
 */

#include "zcl/zcl.h"

/* Identify Cluster Attribute Identifiers */
enum ZbZclIdentifyServerAttrT {
    ZCL_IDENTIFY_ATTR_TIME = 0x0000 /* ZCL_DATATYPE_UNSIGNED_16BIT */
};

/* Identify Cluster Command Identifiers  */
enum ZbZclIdentifyClientCommandT {
    ZCL_IDENTIFY_COMMAND_IDENTIFY = 0x00,
    ZCL_IDENTIFY_COMMAND_QUERY = 0x01
};

enum ZbZclIdentifyServerCommandT {
    ZCL_IDENTIFY_COMMAND_QUERY_RESP = 0x00
};

/* Parameters to the identify callback. */
enum ZbZclIdentifyServerStateT {
    ZCL_IDENTIFY_START = 0,
    ZCL_IDENTIFY_STOP
};

/* Create the Identify Server Cluster.
 * 'arg' gets assigned to ZbZclClusterSetCallbackArg. */
struct ZbZclClusterT * ZbZclIdentifyServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, void *arg);
struct ZbZclClusterT * ZbZclIdentifyClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, void *arg);

/* Configure a callback to receive events to "start" or "stop" identifying. */
typedef void (*ZbZclIdentifyCallbackT)(struct ZbZclClusterT *clusterPtr,
    enum ZbZclIdentifyServerStateT state, void *arg);

void ZbZclIdentifyServerSetCallback(struct ZbZclClusterT *clusterPtr,
    ZbZclIdentifyCallbackT callback);

/* Set the local identify server time. Same as writing to the
 * ZCL_IDENTIFY_ATTR_TIME attribute.
 *
 * If BDB_COMMISSION_MODE_FIND_BIND is enabled and seconds > 0, seconds
 * is adjusted to be >= ZB_BDBC_MinCommissioningTime. */
void ZbZclIdentifyServerSetTime(struct ZbZclClusterT *clusterPtr, uint16_t seconds);

/* Get the local identify server time. */
uint16_t ZbZclIdentifyServerGetTime(struct ZbZclClusterT *clusterPtr);

/* Client commands */
enum ZclStatusCodeT zcl_identify_identify_request(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    uint16_t identify_time, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);
enum ZclStatusCodeT zcl_identify_query_request(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_IDENTIFY_H */
