/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_ILLUM_MEAS_H
# define ZCL_ILLUM_MEAS_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Illuminance Measurement cluster.
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Illuminance_Measurement
 * IM.S | True
 * IM.C | True
 *
 * Server Attributes
 * IM.S.A0000 | True
 * IM.S.A0000.Report.Tx | True
 * IM.S.A0001 | True
 * IM.S.A0002 | True
 * IM.S.A0003 | False
 * IM.S.A0004 | False
 * IM.S.Afffd | True
 * IM.S.Afffe | False
 *
 *
 * Client Attributes
 * IM.C.A0000.Report.Rsp | False
 * IM.C.Afffd | True
 * IM.C.Afffe | False
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Illuminance Measurement Cluster Definitions
 *---------------------------------------------------------------
 */
/* Illuminance Measurement Server Attribute IDs */
enum ZbZclIllumMeasSvrAttrT {
    ZCL_ILLUM_MEAS_ATTR_MEAS_VAL = 0x0000,
    ZCL_ILLUM_MEAS_ATTR_MIN_MEAS_VAL = 0x0001,
    ZCL_ILLUM_MEAS_ATTR_MAX_MEAS_VAL = 0x0002,
    ZCL_ILLUM_MEAS_ATTR_TOLERANCE = 0x0003,
    ZCL_ILLUM_MEAS_ATTR_LIGHT_SENSOR_TYPE = 0x0004,
};

/* Illuminance Measurement Defines */
#define ZCL_ILLUM_MEAS_MEASURED_DEFAULT             0x0000
#define ZCL_ILLUM_MEAS_UNKNOWN                      0xffff
#define ZCL_ILLUM_MEAS_MIN_MEAS_VAL_MIN             0x0001
#define ZCL_ILLUM_MEAS_MIN_MEAS_VAL_MAX             0xfffd
#define ZCL_ILLUM_MEAS_MIN_MEAS_VAL_UNKNOWN         0xffff
#define ZCL_ILLUM_MEAS_MAX_MEAS_VAL_MIN             0x0002
#define ZCL_ILLUM_MEAS_MAX_MEAS_VAL_MAX             0xfffe
#define ZCL_ILLUM_MEAS_MAX_MEAS_VAL_UNKNOWN         0xffff
#define ZCL_ILLUM_MEAS_TOLERANCE_MIN                0x0000
#define ZCL_ILLUM_MEAS_TOLERANCE_MAX                0x0800
#define ZCL_ILLUM_MEAS_LIGHT_SENS_TYPE_PHOTODIODE   0x0000
#define ZCL_ILLUM_MEAS_LIGHT_SENS_TYPE_CMOS         0x0001
#define ZCL_ILLUM_MEAS_LIGHT_SENS_TYPE_UNKNOWN      0x00ff

/* Illuminance Measurement Client */
struct ZbZclClusterT * ZbZclIllumMeasClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Illuminance Measurement Server */
struct ZbZclClusterT * ZbZclIllumMeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t min, uint16_t max);

#endif
