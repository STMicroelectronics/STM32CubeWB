/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

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

/*---------------------------------------------------------------
 * Water Content Measurement Cluster Definitions
 *---------------------------------------------------------------
 */
/* Water Content Measurement Information Attribute Set */
enum {
    ZCL_WC_MEAS_ATTR_MEAS_VAL = 0x0000,
    ZCL_WC_MEAS_ATTR_MIN_MEAS_VAL = 0x0001,
    ZCL_WC_MEAS_ATTR_MAX_MEAS_VAL = 0x0002,
    ZCL_WC_MEAS_ATTR_TOLERANCE = 0x0003,
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

/*---------------------------------------------------------------
 * Water Content Measurement Cluster Definitions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclWaterContentMeasServerAlloc(struct ZigBeeT *zb,
    uint8_t endpoint, enum ZbZclClusterIdT clusterID, uint16_t min, uint16_t max);

struct ZbZclClusterT * ZbZclWaterContentMeasClientAlloc(struct ZigBeeT *zb,
    uint8_t endpoint, enum ZbZclClusterIdT clusterID);

#endif /* __ZCL_WATER_CONTENT_MEAS_H */
