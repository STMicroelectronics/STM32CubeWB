/**
 * @file zcl.occupancy.h
 * @heading Occupancy Sensing
 * @brief ZCL Occupancy Sensing cluster header
 * ZCL 7 section 4.8
 * ZCL 8 section 4.8
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_OCCUP_H
# define ZCL_OCCUP_H

/* @PICS.ZCL.Occupancy
 * OS.S | Server | True
 * OS.C | Client | True
 *
 * Occupancy Sensor Type
 * OS.S.OST00 | PIR | False
 * OS.S.OST01 | Ultrasonic | False
 * OS.S.OST02 | PIR and ultrasonic | False
 *
 * Server Attributes
 * OS.S.A0000 | Occupancy | True
 * OS.S.A0000.Report.Tx | Occupancy | True
 * OS.S.A0001 | OccupancySensorType | True
 * OS.S.A0002 | OccupancySensorTypeBitmap | True
 * OS.S.A0010 | PIROccupiedToUnoccupiedDelay | True | Optional
 * OS.S.A0011 | PIRUnoccupiedToOccupiedDelay | True | Optional
 * OS.S.A0012 | PIRUnoccupiedToOccupiedThreshold | True | Optional
 * OS.S.A0020 | UltrasonicOccupiedToUnoccupiedDelay | True | Optional
 * OS.S.A0021 | UltrasonicUnoccupiedToOccupiedDelay | True | Optional
 * OS.S.A0022 | UltrasonicUnoccupiedToOccupiedThreshold | True | Optional
 * OS.S.A0030 | PhysicalContactOccupiedToUnoccupiedDelay | False | Optional
 * OS.S.A0031 | PhysicalContactUnoccupiedToOccupiedDelay | False | Optional
 * OS.S.A0032 | PhysicalContactUnoccupiedToOccupiedThreshold | False | Optional
 * OS.S.Afffd | ClusterRevision | True
 * OS.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * OS.C.A0000.Report.Rsp | Occupancy | False
 * OS.C.Afffd | ClusterRevision | True
 * OS.C.Afffe | AttributeReportingStatus | True
 */

#include "zcl/zcl.h"

/** Occupancy Sensing Server Attribute IDs */
enum ZbZclOccupancySvrAttrT {
    ZCL_OCC_ATTR_OCCUPANCY = 0x0000, /**< Occupancy */
    ZCL_OCC_ATTR_SENSORTYPE = 0x0001, /**< OccupancySensorType */
    ZCL_OCC_ATTR_SENSORTYPE_BITMAP = 0x0002, /**< OccupancySensorTypeBitmap */
    ZCL_OCC_ATTR_PIR_OU_DELAY = 0x0010, /**< PIROccupiedToUnoccupiedDelay (Optional) */
    ZCL_OCC_ATTR_PIR_UO_DELAY = 0x0011, /**< PIRUnoccupiedToOccupiedDelay (Optional) */
    ZCL_OCC_ATTR_PIR_UO_THRESHOLD = 0x0012, /**< PIRUnoccupiedToOccupiedThreshold (Optional) */
    ZCL_OCC_ATTR_US_OU_DELAY = 0x0020, /**< UltrasonicOccupiedToUnoccupiedDelay (Optional) */
    ZCL_OCC_ATTR_US_UO_DELAY = 0x0021, /**< UltrasonicUnoccupiedToOccupiedDelay (Optional) */
    ZCL_OCC_ATTR_US_UO_THRESHOLD = 0x0022, /**< UltrasonicUnoccupiedToOccupiedThreshold (Optional) */
    ZCL_OCC_ATTR_PHY_OU_DELAY = 0x0030, /**< PhysicalContactOccupiedToUnoccupiedDelay (Optional) */
    ZCL_OCC_ATTR_PHY_UO_DELAY = 0x0031, /**< PhysicalContactUnoccupiedToOccupiedDelay (Optional) */
    ZCL_OCC_ATTR_PHY_UO_THRESHOLD = 0x0032, /**< PhysicalContactUnoccupiedToOccupiedThreshold (Optional) */
};

#define OCC_SENSOR_PIR                      0x00
#define OCC_SENSOR_ULTRASONIC               0x01
#define OCC_SENSOR_PIR_ULTRASONIC           0x02

/**
 * Create a new instance of the Occupancy Sensing Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclOccupancyClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Create a new instance of the Occupancy Sensing Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclOccupancyServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* ZCL_OCCUP_H */
