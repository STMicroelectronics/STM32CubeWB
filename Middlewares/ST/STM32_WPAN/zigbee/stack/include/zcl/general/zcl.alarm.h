/**
 * @file zcl.alarm.h
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 * @heading Alarms
 * @brief ZCL Alarms cluster header
 *
 * ZCL 8 section 3.11
 *
 * Zigbee defines an __alarm__ as the occurrence of a specific condition. Individual clusters
 * (such as Basic, Power Configuration, Door Lock, Ballast Configuration, etc.) define these
 * conditions and a corresponding alarm code.
 *
 * For the definition of the alarm condition its corresponding code for a specific cluster,
 * see the Zigbee Cluster Library Specification 8 (ZCL8).
 *
 * Alarm conditions are typically defined in terms of a cluster attribute. For example, the Power
 * Configuration cluster defines alarm code 0x00 for the alarm generated when the MainsVoltage
 * attribute drops below the value specified in the MainsVoltageMinThreshold attribute for a time
 * period greater than the MainsVoltageDwellTripPoint attribute in seconds.
 *
 * Clusters typically have an additional AlarmMask attribute which is a bitmask that allows the
 * client to enable or disable the generation of individual alarms when the corresponding alarm
 * condition is met.
 *
 * It is the responsibility of the cluster application implementation to detect the alarm condition,
 * check the alarm mask, and when needed initiate generation of the alarm by calling
 * `ZbZclClusterSendAlarm()` (defined in zcl.h), resulting in the sending of an Alarm command.
 * It is important to note that this Alarm command is not sent from the originating cluster.
 * Instead, it is sent from an instance of the Alarm cluster that must reside on the same
 * endpoint as the originating cluster.
 *
 * The alarm cluster sends the alarm command to all clients with bindings to the alarm cluster on
 * this endpoint. It also adds alarm details to an internal log. The alarm cluster provides
 * commands that allow clients to query this alarm log. The alarm log is shared by all clusters
 * on the same endpoint. In order to receive alarms, clients must bind to the alarms cluster on
 * the same endpoint as the alarm generating cluster. For clusters that support an alarm mask,
 * any client can enable/disable generation of alarm commands by setting/clearing the mask bit
 * in the originating cluster. The mask controls sending of alarms to all bound clients.
 *
 * Some alarm conditions do not automatically reset and must be manually reset by the client.
 * The Alarm cluster provides the Reset Alarm and Reset all Alarms commands for this reason
 * the application should register a callback for each endpoint with cluster(s) that require
 * resetting using `ZbZclClusterRegisterAlarmResetHandler()`. The callback handles the specific
 * cluster(s) against which it was registered.
 *
 * When the alarm cluster receives a Reset Alarm or Reset all Alarms command, the cluster
 * application callback will be invoked and can then handle the whatever is necessary to
 * internally reset to detect new occurrences of the alarm condition. The same callback is
 * invoked for both commands. When the Reset all Alarms command is received the callback is
 * invoked with an Alarm Code of `0xFF` and Cluster ID of `0xFFFF`. When a callback is
 * provided the function return code is provided as the status in the Default Response.
 * When no callbacks are provided the alarm cluster will send a Default Response with a
 * status of `SUCCESS`.
 *
 * As a summary:
 *
 * * When an endpoint contains a cluster that can generate alarms, it is the application's
 * responsibility to also instantiate the alarms cluster on that endpoint.
 *
 * * It is the responsibility of the cluster implementation to
 * ** Detect alarm conditions
 * ** Check the alarm mask (where supported)
 * ** Generate an alarm by calling `ZbZclClusterSendAlarm()`
 *
 * * If the alarm conditions for any cluster(s) on an endpoint need to be manually reset,
 * then the application should register a callback for each endpoint with cluster(s) that
 * require resetting using ZbZclClusterRegisterAlarmResetHandler().
 *
 * For more information about the Alarms cluster, see Section 3.11 in ZCL8.
 */

/* @PICS.ZCL.Alarm
 * ALM.S | Server | True
 * ALM.C | Client | True
 *
 * Server Attributes
 * ALM.S.A0000 | AlarmCount | True | Optional
 * ALM.S.Afffd | ClusterRevision | True
 * ALM.S.Afffe | AttributeReportingStatus | True
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
 * ALM.C.Afffe | AttributeReportingStatus | True
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

#ifndef ZCL_ALARM_H
# define ZCL_ALARM_H

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
