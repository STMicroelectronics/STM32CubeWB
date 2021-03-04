/**
 * @file zcl.alarm.h
 * @heading Alarms
 * @brief ZCL Alarms cluster header
 * ZCL 7 section 3.11
 * ZCL 8 section 3.11
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ALARM_H
# define ZCL_ALARM_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Alarms cluster
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Alarm
 * ALM.S | Server | True
 * ALM.C | Client | True
 *
 * Server Attributes
 * ALM.S.A0000 | AlarmCount | True | Optional
 * ALM.S.Afffd | ClusterRevision | True
 * ALM.S.Afffe | AttributeReportingStatus | False
 *
 * Commands Received
 * ALM.S.C00.Rsp | Reset Alarm | True
 * ALM.S.C01.Rsp | Reset all alarms | True
 * ALM.S.C02.Rsp | Get Alarm | True
 * ALM.S.C03.Rsp | Reset alarm log | True
 *
 * Commands Generated
 * ALM.S.C00.Tx | Alarm | True
 * ALM.S.C01.Tx | Get alarm response | True
 *
 * Client Attributes
 * ALM.C.Afffd | ClusterRevision | True
 * ALM.C.Afffe | AttributeReportingStatus | False
 *
 * Commands Received
 * ALM.C.C00.Rsp | Alarm | True
 * ALM.C.C01.Rsp | Get alarm response | True
 *
 * Commands Generated
 * ALM.C.C00.Tx | Reset Alarm | True
 * ALM.C.C01.Tx | Reset all alarms | True
 * ALM.C.C02.Tx | Get Alarm | True
 * ALM.C.C03.Tx | Reset alarm log | True
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */

/** Alarms Attribute IDs */
enum ZbZclAlarmsAttrT {
    ZCL_ALARM_ATTR_COUNT = 0x0000 /**< AlarmCount (Optional) */
};

/* Alarms cluster client commands */
enum {
    ZCL_ALARM_COMMAND_RESET = 0x00,
    ZCL_ALARM_COMMAND_RESET_ALL,
    ZCL_ALARM_COMMAND_GET,
    ZCL_ALARM_COMMAND_RESET_LOG
};

/* Alarms cluster server commands */
enum {
    ZCL_ALARM_COMMAND_ALARM = 0x00,
    ZCL_ALARM_COMMAND_GET_RESPONSE
};

/*---------------------------------------------------------------
 * Structures
 *---------------------------------------------------------------
 */

/**
 * Callback when an alarm is received
 * @param arg Pointer to application data as provided previously at the time the pointer to this callback function was provided in
 *  initiating API call
 * @param nwk_addr Short address where the alarm originates
 * @param endpoint Endpoint where the alarm originates
 * @param alarm_code Code of the detected alarm condition
 * @param cluster_id ID of cluster where alarm condition occurred
 * @return Returns void
 */
typedef void (*ZbZclAlarmClientCallbackT)(void *arg, uint16_t nwk_addr, uint8_t endpoint,
    uint8_t alarm_code, uint16_t cluster_id);

/*---------------------------------------------------------------
 * Function Declarations
 *---------------------------------------------------------------
 */

/*
 * Server Commands
 */

/**
 * Create a new instance of the Alarms Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param logSize Alarm log size
 * @param time_server Time Server cluster instance
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclAlarmServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t logSize, struct ZbZclClusterT *time_server);

/*
 * Client Commands
 */

/**
 * Create a new instance of the Alarms Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callback Callback function that will be invoked later when an alarm occurs
 * @param arg Pointer to application data that will later be provided back to the callback function when it is invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclAlarmClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, ZbZclAlarmClientCallbackT callback, void *arg);

/*uint8_t ZbZclAlarmClientBind(struct ZbZclClusterT *clusterPtr, uint16_t nwkAddr, uint8_t endpoint);
uint8_t ZbZclAlarmClientBindDiscover(struct ZbZclClusterT *clusterPtr);
*/

/**
 * Send a Reset Alarm command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param alarm_code Code of the detected alarm condition
 * @param cluster_id ID of cluster where alarm condition occurred
 * @param callback Callback function that will be invoked later when response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when it is invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclAlarmClientResetAlarmReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    uint8_t alarm_code, uint16_t cluster_id, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Reset All Alarms command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when it is invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclAlarmClientResetAllAlarmsReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Get Alarm command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when it is invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclAlarmClientGetAlarmReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Reset Alarm Log command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when it is invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclAlarmClientResetAlarmLogReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_ALARM_H */
