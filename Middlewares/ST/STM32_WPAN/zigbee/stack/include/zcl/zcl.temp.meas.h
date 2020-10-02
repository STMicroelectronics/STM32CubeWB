/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_MEASURE_H
# define ZCL_MEASURE_H

/* PICS.ZCL.TemperatureMeasurement
 * TM.S | True
 * TM.C | True
 *
 * Server Attributes
 * TM.S.A0000 | True
 * TM.S.A0000.Report.Tx | True
 * TM.S.A0001 | True
 * TM.S.A0002 | True
 * TM.S.A0003 | True
 * TM.S.Afffd | True
 * TM.S.Afffe | False
 *
 * Client Attributes
 * TM.C.A0000.Report.Rsp | False
 * TM.C.Afffd | True
 * TM.C.Afffe | False
 */

#include "zcl/zcl.h"

/** Temperature Measurement Server Attribute IDs */
enum ZbZclTempMeasSvrAttrT {
    ZCL_TEMP_MEAS_ATTR_MEAS_VAL = 0x0000,
    ZCL_TEMP_MEAS_ATTR_MIN_MEAS_VAL = 0x0001,
    ZCL_TEMP_MEAS_ATTR_MAX_MEAS_VAL = 0x0002,
    ZCL_TEMP_MEAS_ATTR_TOLERANCE = 0x0003
};

/* Temperature Measurement Defines */
#define ZCL_TEMP_MEAS_MEASURED_DEFAULT                       0xffff
#define ZCL_TEMP_MEAS_UNKNOWN                                0x8000
#define ZCL_TEMP_MEAS_MIN_MEAS_VAL_MIN              (int16_t)0x954d
#define ZCL_TEMP_MEAS_MIN_MEAS_VAL_MAX              (int16_t)0x7ffe
#define ZCL_TEMP_MEAS_MAX_MEAS_VAL_MIN              (int16_t)0x954e
#define ZCL_TEMP_MEAS_MAX_MEAS_VAL_MAX              (int16_t)0x7fff
#define ZCL_TEMP_MEAS_TOLERANCE_MIN                          0x0000
#define ZCL_TEMP_MEAS_TOLERANCE_MAX                          0x0800

/* Temperature Measurement Client */
struct ZbZclClusterT * ZbZclTempMeasClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Temperature Measurement Server */
struct ZbZclClusterT * ZbZclTempMeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, int16_t min, int16_t max, uint16_t tolerance);

#endif
