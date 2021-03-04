/**
 * @file zcl.identify.h
 * @heading Identify
 * @brief ZCL Identify cluster header
 * ZCL 7 section 3.5
 * ZCL 8 section 3.5
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_CORE_IDENTIFY_H
# define ZCL_CORE_IDENTIFY_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Identify cluster.
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Identify Cluster (0x0003)
 *
 * I.S | Server | True
 * I.C | Client | True
 *
 * Server Attributes
 * I.S.A0000 | IdentifyTime | True
 * I.S.Afffd | ClusterRevision |True
 *
 * Commands Received
 * I.S.C00.Rsp | Identify | True
 * I.S.C01.Rsp | Identify Query | True
 * I.S.C40.Rsp | Trigger effect | False
 *
 * Commands Generated
 * I.S.C00.Tx | Identify Query Response | True
 *
 * Client Attributes
 * I.C.Afffd | ClusterRevision | True
 *
 * Commands Received
 * I.C.C00.Rsp | Identify Query Response | True
 *
 * Commands Generated
 * I.C.C00.Tx | Identify | True
 * I.C.C01.Tx | Identify Query | True
 * I.C.C40.Tx | Trigger effect | False
 */

#include "zcl/zcl.h"

/** Identify Server Attribute IDs */
enum ZbZclIdentifySvrAttrT {
    ZCL_IDENTIFY_ATTR_TIME = 0x0000 /**< IdentifyTime */
};

/* Identify Command IDs  */
enum ZbZclIdentifyClientCommandT {
    ZCL_IDENTIFY_COMMAND_IDENTIFY = 0x00,
    ZCL_IDENTIFY_COMMAND_QUERY = 0x01
};

enum ZbZclIdentifyServerCommandT {
    ZCL_IDENTIFY_COMMAND_QUERY_RESP = 0x00
};

/*---------------------------------------------------------------
 * Identify Server Cluster
 *---------------------------------------------------------------
 */

enum ZbZclIdentifyServerStateT {
    ZCL_IDENTIFY_START = 0,
    ZCL_IDENTIFY_STOP
};

/**
 * Callback to receive start and stop events
 * @param cluster Cluster instance from which this callback will be invoked
 * @param state Identification state: start or stop
 * @param arg Pointer to application data provided in initiating API call
 * @return Void
 */
typedef void (*ZbZclIdentifyCallbackT)(struct ZbZclClusterT *cluster,
    enum ZbZclIdentifyServerStateT state, void *arg);

/**
 * Create a new instance of the Identify Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIdentifyServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, void *arg);

/**
 * Set the callback in the cluster private structure.
 * @param cluster Cluster instance which will later invoke the callback
 * @param callback Callback function that will be invoked later when the identification state changes
 * @return Void
 */
void ZbZclIdentifyServerSetCallback(struct ZbZclClusterT *cluster,
    ZbZclIdentifyCallbackT callback);

/**
 * Set the local Identify Server time
 *
 * If BDB_COMMISSION_MODE_FIND_BIND is enabled and seconds > 0, seconds
 * is adjusted to be >= ZB_BDBC_MinCommissioningTime
 * @param cluster Cluster instance with timer to set
 * @param seconds Seconds for updating the identify time counter
 * @return Void
 */
void ZbZclIdentifyServerSetTime(struct ZbZclClusterT *cluster, uint16_t seconds);

/**
 * Get the local Identify Server time
 * @param cluster Cluster instance with timer to get
 * @return uint16_t Time remaining in zigbee timer
 */
uint16_t ZbZclIdentifyServerGetTime(struct ZbZclClusterT *cluster);

/*---------------------------------------------------------------
 * Identify Client Cluster
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Identify Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIdentifyClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send an Identify command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param identify_time Time which will be used to set the IdentifyTime attribute
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_identify_identify_request(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    uint16_t identify_time, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send an Identify Query command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT zcl_identify_query_request(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_IDENTIFY_H */
