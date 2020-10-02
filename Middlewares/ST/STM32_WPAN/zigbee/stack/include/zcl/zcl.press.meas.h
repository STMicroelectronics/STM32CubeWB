/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_PRESS_MEAS_H
#define ZCL_PRESS_MEAS_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Pressure Measurement cluster.
 *--------------------------------------------------------------------------
 */

/* Attribute Identifiers */
enum ZbZclPressMeasSvrAttrT {
    ZCL_PRESS_MEAS_ATTR_MEAS_VAL = 0x0000,
    ZCL_PRESS_MEAS_ATTR_MIN_MEAS_VAL = 0x0001,
    ZCL_PRESS_MEAS_ATTR_MAX_MEAS_VAL = 0x0002,
    ZCL_PRESS_MEAS_ATTR_TOLERANCE = 0x0003,
    ZCL_PRESS_MEAS_ATTR_SCALED_VAL = 0x0010,
    ZCL_PRESS_MEAS_ATTR_MIN_SCALED_VAL = 0x0011,
    ZCL_PRESS_MEAS_ATTR_MAX_SCALED_VAL = 0x0012,
    ZCL_PRESS_MEAS_ATTR_SCALED_TOL = 0x0013,
    ZCL_PRESS_MEAS_ATTR_SCALE = 0x0014
};

/* Pressure Measurement Defines */
#define ZCL_PRESS_MEAS_UNKNOWN                       (int16_t)0x8000
#define ZCL_PRESS_MEAS_MEAS_VAL_DEFAULT              (int16_t)0xffff
#define ZCL_PRESS_MEAS_MIN_VAL_MIN                   (int16_t)0x8001
#define ZCL_PRESS_MEAS_MIN_VAL_MAX                   (int16_t)0x7ffe
#define ZCL_PRESS_MEAS_MAX_VAL_MIN                   (int16_t)0x8002
#define ZCL_PRESS_MEAS_MAX_VAL_MAX                   (int16_t)0x7fff
#define ZCL_PRESS_MEAS_SCALE_MIN                        (int8_t)0x81
#define ZCL_PRESS_MEAS_SCALE_MAX                        (int8_t)0x7f

/*---------------------------------------------------------------
 * Pressure Measurement Cluster Definitions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclPressMeasClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclPressMeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, int16_t min, int16_t max);

#endif /* __ZCL_PRESS_MEAS_H */
