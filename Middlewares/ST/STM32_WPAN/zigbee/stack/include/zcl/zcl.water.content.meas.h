/**
 * @file zcl.water.content.meas.h
 * @brief ZCL Water Content Measurement cluster header
 * ZCL 7 section 4.7
 * ZCL 8 section 4.7
 * @copyright Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved.
 */
#ifndef ZCL_WATER_CONTENT_MEAS_H
# define ZCL_WATER_CONTENT_MEAS_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Water Content Measurement cluster.
 *--------------------------------------------------------------------------
 */
/* PICS.ZCL.Water_Content_Measurement
 * WCM.S | True
 * WCM.C | True
 *
 * Server Attributes
 * WCM.S.A0000 | True
 * WCM.S.A0000.Report | True
 * WCM.S.A0001 | True
 * WCM.S.A0002 | True
 * WCM.S.A0003 | False
 * WCM.S.Afffd | True
 * WCM.S.Afffe | False
 *
 *
 * Client Attributes
 * WCM.C.A0000.Report.Rsp | False
 * WCM.C.Afffd | True
 * WCM.C.Afffe | False
 */

#include "zcl/zcl.h"

/** Water Content Measurement Attribute IDs */
enum {
    ZCL_WC_MEAS_ATTR_MEAS_VAL = 0x0000, /**< MeasuredValue */
    ZCL_WC_MEAS_ATTR_MIN_MEAS_VAL = 0x0001, /**< MinMeasuredValue */
    ZCL_WC_MEAS_ATTR_MAX_MEAS_VAL = 0x0002, /**< MaxMeasuredValue */
    ZCL_WC_MEAS_ATTR_TOLERANCE = 0x0003, /**< Tolerance (Optional) */
};

/* Water Content Measurement Defines */
#define ZCL_WC_MEAS_MEASURED_DEFAULT                0xffff
#define ZCL_WC_MEAS_UNKNOWN                         0xffff
#define ZCL_WC_MEAS_MIN_MEAS_VAL_MIN                0x0000
#define ZCL_WC_MEAS_MIN_MEAS_VAL_MAX                0x270f
#define ZCL_WC_MEAS_MIN_MEAS_VAL_UNKNOWN            0xffff
#define ZCL_WC_MEAS_MAX_MEAS_VAL_MIN                0x0001
#define ZCL_WC_MEAS_MAX_MEAS_VAL_MAX                0x2710
#define ZCL_WC_MEAS_MAX_MEAS_VAL_UNKNOWN            0xffff
#define ZCL_WC_MEAS_TOLERANCE_MIN                   0x0000
#define ZCL_WC_MEAS_TOLERANCE_MAX                   0x0800

/**
 * Create a new instance of the Water Content Measurement Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param clusterID Cluster identifier (Relative Humidity 0x0405, Leaf Wetness 0x0407 or Soil Moisture 0x0408)
 * @param min Default minimum value
 * @param max Default maximum value
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclWaterContentMeasServerAlloc(struct ZigBeeT *zb,
    uint8_t endpoint, enum ZbZclClusterIdT clusterID, uint16_t min, uint16_t max);

/**
 * Create a new instance of the Water Content Measurement Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param clusterID Cluster identifier (Relative Humidity 0x0405, Leaf Wetness 0x0407 or Soil Moisture 0x0408)
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclWaterContentMeasClientAlloc(struct ZigBeeT *zb,
    uint8_t endpoint, enum ZbZclClusterIdT clusterID);

#endif
