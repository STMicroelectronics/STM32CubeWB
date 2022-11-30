/**
 * @file zcl.pump.h
 * @heading Pump Configuration and Control
 * @brief ZCL Pump Configuration and Control cluster header
 * ZCL 7 section 6.2
 * ZCL 8 section 6.2
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_PUMP_H
#define ZCL_PUMP_H

/* NOTE: The following PICS statement is for internal Exegin use only, and is not a product of the Zigbee Alliance. */
/* @PICS.ZCL.Pump
 * PCC.S | Server | True
 * PCC.C | Client | True
 *
 * Server Attributes
 * PCC.S.A0000 | MaxPressure | True
 * PCC.S.A0001 | MaxSpeed | True
 * PCC.S.A0002 | MaxFlow | True
 * PCC.S.A0003 | MinConstPressure | False | Optional
 * PCC.S.A0004 | MaxConstPressure | False | Optional
 * PCC.S.A0005 | MinCompPressure | False | Optional
 * PCC.S.A0006 | MaxCompPressure | False | Optional
 * PCC.S.A0007 | MinConstSpeed | False | Optional
 * PCC.S.A0008 | MaxConstSpeed | False | Optional
 * PCC.S.A0009 | MinConstFlow | False | Optional
 * PCC.S.A000a | MaxConstFlow | False | Optional
 * PCC.S.A000b | MinConstTemp | False | Optional
 * PCC.S.A000c | MaxConstTemp | False | Optional
 * PCC.S.A0010 | PumpStatus | False | Optional
 * PCC.S.A0010.Report.Tx | PumpStatus | False
 * PCC.S.A0011 | EffectiveOperationMode | True
 * PCC.S.A0012 | EffectiveControlMode | True
 * PCC.S.A0013 | Capacity | True
 * PCC.S.A0013.Report.Tx | Capacity | True
 * PCC.S.A0014 | Speed | False | Optional
 * PCC.S.A0015 | LifetimeRunningHours | False | Optional
 * PCC.S.A0016 | Power | False | Optional
 * PCC.S.A0017 | LifetimeEnergyConsumed | False | Optional
 * PCC.S.A0020 | OperationMode | True
 * PCC.S.A0021 | ControlMode | False | Optional
 * PCC.S.A0022 | AlarmMask | False | Optional
 * PCC.S.Afffd | ClusterRevision | True
 * PCC.S.Afffe | AttributeReportingStatus | True

 * Client Attributes
 * PCC.C.A0010.Report.Rsp | PumpStatus | False
 * PCC.S.A0013.Report.Rsp | Capacity | False
 * PCC.C.Afffd | ClusterRevision | True
 * PCC.C.Afffe | AttributeReportingStatus | True
 */

#include "zcl/zcl.h"

/* Pump Configuration and Control cluster interface definition */

/** Pump Configuration and Control Attribute Ids */
enum ZbZclPumpServerAttrT {
    ZCL_PUMP_SVR_ATTR_MAX_PRESSURE = 0x0000, /**< MaxPressure */
    ZCL_PUMP_SVR_ATTR_MAX_SPEED = 0x0001, /**< MaxSpeed */
    ZCL_PUMP_SVR_ATTR_MAX_FLOW = 0x0002, /**< MaxFlow */
    ZCL_PUMP_SVR_ATTR_MIN_CONST_PRESSURE = 0x0003, /**< MinConstPressure */
    ZCL_PUMP_SVR_ATTR_MAX_CONST_PRESSURE = 0x0004, /**< MaxConstPressure */
    ZCL_PUMP_SVR_ATTR_MIN_COMP_PRESSURE = 0x0005, /**< MinCompPressure */
    ZCL_PUMP_SVR_ATTR_MAX_COMP_PRESSURE = 0x0006, /**< MaxCompPressure */
    ZCL_PUMP_SVR_ATTR_MIN_CONST_SPEED = 0x0007, /**< MinConstSpeed */
    ZCL_PUMP_SVR_ATTR_MAX_CONST_SPEED = 0x0008, /**< MaxConstSpeed */
    ZCL_PUMP_SVR_ATTR_MIN_CONST_FLOW = 0x0009, /**< MinConstFlow */
    ZCL_PUMP_SVR_ATTR_MAX_CONST_FLOW = 0x000a, /**< MaxConstFlow */
    ZCL_PUMP_SVR_ATTR_MIN_CONST_TEMP = 0x000b, /**< MinConstTemp */
    ZCL_PUMP_SVR_ATTR_MAX_CONST_TEMP = 0x000c, /**< MaxConstTemp */
    ZCL_PUMP_SVR_ATTR_PUMP_STATUS = 0x0010, /**< PumpStatus */
    ZCL_PUMP_SVR_ATTR_EFF_OP_MODE = 0x0011, /**< EffectiveOperationMode */
    ZCL_PUMP_SVR_ATTR_EFF_CTRL_MODE = 0x0012, /**< EffectiveControlMode */
    ZCL_PUMP_SVR_ATTR_CAPACITY = 0x0013, /**< Capacity */
    ZCL_PUMP_SVR_ATTR_SPEED = 0x0014, /**< Speed */
    ZCL_PUMP_SVR_ATTR_RUNNING_HOURS = 0x0015, /**< LifetimeRunningHours */
    ZCL_PUMP_SVR_ATTR_POWER = 0x0016, /**< Power */
    ZCL_PUMP_SVR_ATTR_ENERGY_CONSUMED = 0x0017, /**< LifetimeEnergyConsumed */
    ZCL_PUMP_SVR_ATTR_OP_MODE = 0x0020, /**< OperationMode */
    ZCL_PUMP_SVR_ATTR_CTRL_MODE = 0x0021, /**< ControlMode */
    ZCL_PUMP_SVR_ATTR_ALARM_MASK = 0x0022 /**< AlarmMask */
};

/** Pump Configuration and Control cluster defines */
#define ZCL_PUMP_OP_MODE_DEFAULT                0x00
#define ZCL_PUMP_PRESSURE_MIN                   (int16_t)0x8001
#define ZCL_PUMP_PRESSURE_MAX                   (int16_t)0x7fff
#define ZCL_PUMP_TEMP_MIN                       (int16_t)0x954d
#define ZCL_PUMP_TEMP_MAX                       (int16_t)0x7fff
#define ZCL_PUMP_CAPACITY_MIN                   (int16_t)0x0000
#define ZCL_PUMP_CAPACITY_MAX                   (int16_t)0x7fff
#define ZCL_PUMP_CAPACITY_UNKNOWN               (int16_t)0x8000

/* Pump Configuration and Control cluster function prototypes */
/**
 * Instantiate a new instance of the Pump Configuration and Control client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPumpClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Instantiate a new instance of the Pump Configuration and Control server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclPumpServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif
