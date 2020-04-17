/* Copyright [2019 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_PUMP_H
#define ZCL_PUMP_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Pump Configuration and
 *      Control cluster. There are many optional attributes for this
 *      cluster. If external pressure, flow or temperture measurements
 *      are expected to be processed, then these measurements can be
 *      obtained from clusters 0x0402, 0x0403 and 0x0404 which may or
 *      may not be on the same endpoint. If alarms are used, the Alarm
 *      Server cluster must be on the same endpoint. More information
 *      can be found in the Zigbee Cluster Library Specification.
 *--------------------------------------------------------------------------
 */

/* Attribute Identifiers */
enum {
    ZCL_PUMP_ATTR_MAX_PRESSURE = 0x0000,
    ZCL_PUMP_ATTR_MAX_SPEED = 0x0001,
    ZCL_PUMP_ATTR_MAX_FLOW = 0x0002,
    ZCL_PUMP_ATTR_MIN_CONST_PRESSURE = 0x0003,
    ZCL_PUMP_ATTR_MAX_CONST_PRESSURE = 0x0004,
    ZCL_PUMP_ATTR_MIN_COMP_PRESSURE = 0x0005,
    ZCL_PUMP_ATTR_MAX_COMP_PRESSURE = 0x0006,
    ZCL_PUMP_ATTR_MIN_CONST_SPEED = 0x0007,
    ZCL_PUMP_ATTR_MAX_CONST_SPEED = 0x0008,
    ZCL_PUMP_ATTR_MIN_CONST_FLOW = 0x0009,
    ZCL_PUMP_ATTR_MAX_CONST_FLOW = 0x000a,
    ZCL_PUMP_ATTR_MIN_CONST_TEMP = 0x000b,
    ZCL_PUMP_ATTR_MAX_CONST_TEMP = 0x000c,
    ZCL_PUMP_ATTR_PUMP_STATUS = 0x0010,
    ZCL_PUMP_ATTR_EFF_OP_MODE = 0x0011,
    ZCL_PUMP_ATTR_EFF_CTRL_MODE = 0x0012,
    ZCL_PUMP_ATTR_CAPACITY = 0x0013,
    ZCL_PUMP_ATTR_SPEED = 0x0014,
    ZCL_PUMP_ATTR_RUNNING_HOURS = 0x0015,
    ZCL_PUMP_ATTR_POWER = 0x0016,
    ZCL_PUMP_ATTR_ENERGY_CONSUMED = 0x0017,
    ZCL_PUMP_ATTR_OP_MODE = 0x0020,
    ZCL_PUMP_ATTR_CTRL_MODE = 0x0021,
    ZCL_PUMP_ATTR_ALARM_MASK = 0x0022
};

#define ZCL_PUMP_OP_MODE_DEFAULT                0x00
#define ZCL_PUMP_PRESSURE_MIN                   (int16_t)0x8001
#define ZCL_PUMP_PRESSURE_MAX                   (int16_t)0x7fff
#define ZCL_PUMP_TEMP_MIN                       (int16_t)0x954d
#define ZCL_PUMP_TEMP_MAX                       (int16_t)0x7fff
#define ZCL_PUMP_CAPACITY_MIN                   (int16_t)0x0000
#define ZCL_PUMP_CAPACITY_MAX                   (int16_t)0x7fff
#define ZCL_PUMP_CAPACITY_UNKNOWN               (int16_t)0x8000

/*---------------------------------------------------------------
 * Illumination Level Sensing Cluster Definitions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclPumpClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclPumpServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_PUMP_H */
