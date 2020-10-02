/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_ALARM_H
# define ZCL_ALARM_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Alarms cluster.
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Alarm
 * ALM.S | True
 * ALM.C | True
 *
 * Server Attributes
 * ALM.S.A0000 | True
 *
 * Commands Received
 * ALM.S.C00.Rsp | True
 * ALM.S.C01.Rsp | True
 * ALM.S.C02.Rsp | True
 * ALM.S.C03.Rsp | True
 *
 * Commands Generated
 * ALM.S.C00.Tx | True
 * ALM.S.C01.Tx | True
 *
 *
 * Client Side
 *
 * Commands Received
 * ALM.C.C00.Rsp | True
 * ALM.C.C01.Rsp | True
 *
 * Commands Generated
 * ALM.C.C00.Tx | True
 * ALM.C.C01.Tx | True
 * ALM.C.C02.Tx | True
 * ALM.C.C03.Tx | True
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */
/* Alarms cluster attribute identifiers. */
enum ZbZclAlarmsAttrT {
    ZCL_ALARM_ATTR_COUNT = 0x0000
};

/* Alarms cluster client commands. */
enum {
    ZCL_ALARM_COMMAND_RESET = 0x00,
    ZCL_ALARM_COMMAND_RESET_ALL,
    ZCL_ALARM_COMMAND_GET,
    ZCL_ALARM_COMMAND_RESET_LOG
};

/* Alarms cluster server commands. */
enum {
    ZCL_ALARM_COMMAND_ALARM = 0x00,
    ZCL_ALARM_COMMAND_GET_RESPONSE
};

/*---------------------------------------------------------------
 * Structures
 *---------------------------------------------------------------
 */

/*
 * Callback on Alarm Client Cluster when an alarm is received
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
struct ZbZclClusterT * ZbZclAlarmServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t logSize, struct ZbZclClusterT *time_server);

/*
 * Client Commands
 */
struct ZbZclClusterT * ZbZclAlarmClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, ZbZclAlarmClientCallbackT callback, void *arg);
/*uint8_t ZbZclAlarmClientBind(struct ZbZclClusterT *clusterPtr, uint16_t nwkAddr, uint8_t endpoint);
uint8_t ZbZclAlarmClientBindDiscover(struct ZbZclClusterT *clusterPtr);
*/
enum ZclStatusCodeT ZbZclAlarmClientResetAlarmReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    uint8_t alarm_code, uint16_t cluster_id, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclAlarmClientResetAllAlarmsReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclAlarmClientGetAlarmReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclAlarmClientResetAlarmLogReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* __ZCL_ALARM_H */
