/**
 * @file zcl.illum.level.h
 * @heading Illuminance Level Sensing
 * @brief Illuminance Level Sensing cluster header
 * ZCL 7 section 4.3
 * ZCL 8 section 4.3
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ILLUM_LEVEL_H
#define ZCL_ILLUM_LEVEL_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Illuminance Level Sensing
 *      cluster. This cluster's "level status" attribute is expected
 *      to change depending on the current value of the "target level"
 *      attribute and the illuminance measurement cluster's "measured
 *      value" attribute. This behaviour should be implemented in the
 *      application.
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Illuminance_Level_Sensing
 * ILLVL.S | Server | True
 * ILLVL.C | Client | True
 *
 * Server Attributes
 * ILLVL.S.A0000 | LevelStatus | True
 * ILLVL.S.A0001 | LightSensorType | False | Optional
 * ILLVL.S.A0010 | IlluminanceTargetLevel | True
 * ILLVL.S.Afffd | ClusterRevision | True
 * ILLVL.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * ILLVL.C.Afffd | ClusterRevision | True
 * ILLVL.C.Afffe | AttributeReportingStatus | True
 */

/** Attribute Identifiers */
enum ZbZclIllumLevelSvrAttrT {
    ZCL_ILLUM_LEVEL_ATTR_LEVEL_STATUS = 0x0000, /**< LevelStatus */
    ZCL_ILLUM_LEVEL_ATTR_LIGHT_SENSOR_TYPE = 0x0001, /**< LightSensorType */
    ZCL_ILLUM_LEVEL_ATTR_ILLUM_TARGET_LEVEL = 0x0010 /**< IlluminanceTargetLevel */
};

/* Level Status values */
#define ZCL_ILLUM_LEVEL_STATUS_ON_TARGET             0x00U
#define ZCL_ILLUM_LEVEL_STATUS_BELOW_TARGET          0x01U
#define ZCL_ILLUM_LEVEL_STATUS_ABOVE_TARGET          0x02U

/* Light Sensor Type values */
#define ZCL_ILLUM_LEVEL_SENSOR_TYPE_PHOTODIODE       0x00U
#define ZCL_ILLUM_LEVEL_SENSOR_TYPE_CMOS             0x01U
#define ZCL_ILLUM_LEVEL_SENSOR_TYPE_UNKNOWN          0xffU

/* Illuminance Target Level values */
#define ZCL_ILLUM_LEVEL_TARGET_INVALID               0xffff

/* Illum Level Sensing Defines */
#define ZCL_ILLUM_LEVEL_STATUS_MIN                   0x00
#define ZCL_ILLUM_LEVEL_STATUS_MAX                   0xfe
#define ZCL_ILLUM_LEVEL_SEN_TYPE_MIN                 0x00
#define ZCL_ILLUM_LEVEL_SEN_TYPE_MAX                 0xff
#define ZCL_ILLUM_LEVEL_TARGET_MIN                   0x0000
#define ZCL_ILLUM_LEVEL_TARGET_MAX                   0xffff

/*---------------------------------------------------------------
 * Illumination Level Sensing Cluster Definitions
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the Illuminance Level Sensing Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIllumLevelClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Create a new instance of the Illuminance Level Sensing Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIllumLevelServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_ILLUM_LEVEL_H */
