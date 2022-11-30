/**
 * @file zcl.illum.meas.h
 * @heading Illuminance Measurement
 * @brief ZCL Illuminance Measurement cluster header
 * ZCL 7 section 4.2
 * ZCL 8 section 4.2
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ILLUM_MEAS_H
# define ZCL_ILLUM_MEAS_H

/* @PICS.ZCL.Illuminance_Measurement
 * IM.S | Server | True
 * IM.C | Client | True
 *
 * Server Attributes
 * IM.S.A0000 | MeasuredValue | True
 * IM.S.A0000.Report.Tx | MeasuredValue | True
 * IM.S.A0001 | MinMeasuredValue | True
 * IM.S.A0002 | MaxMeasuredValue | True
 * IM.S.A0003 | Tolerance | False | Optional
 * IM.S.A0004 | LightSensorType | False | Optional
 * IM.S.Afffd | ClusterRevision | True
 * IM.S.Afffe | AttributeReportingStatus | True
 *
 *
 * Client Attributes
 * IM.C.A0000.Report.Rsp | MeasuredValue | False
 * IM.C.Afffd | ClusterRevision | True
 * IM.C.Afffe | AttributeReportingStatus | True
 */

#include "zcl/zcl.h"

/** Illuminance Measurement Server Attribute IDs */
enum ZbZclIllumMeasSvrAttrT {
    ZCL_ILLUM_MEAS_ATTR_MEAS_VAL = 0x0000, /**< MeasuredValue */
    ZCL_ILLUM_MEAS_ATTR_MIN_MEAS_VAL = 0x0001, /**< MinMeasuredValue */
    ZCL_ILLUM_MEAS_ATTR_MAX_MEAS_VAL = 0x0002, /**< MaxMeasuredValue */
    ZCL_ILLUM_MEAS_ATTR_TOLERANCE = 0x0003, /**< Tolerance (Optional) */
    ZCL_ILLUM_MEAS_ATTR_LIGHT_SENSOR_TYPE = 0x0004, /**< LightSensorType (Optional) */
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
/**
 * Create a new instance of the Illuminance Measurement Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIllumMeasClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Illuminance Measurement Server */
/**
 * Create a new instance of the Illuminance Measurement Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param min Minimum value capable of being measured (MinMeasuredValue)
 * @param max Maximum value capable of being measured (MaxMeasuredValue)
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIllumMeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint16_t min, uint16_t max);

#endif
