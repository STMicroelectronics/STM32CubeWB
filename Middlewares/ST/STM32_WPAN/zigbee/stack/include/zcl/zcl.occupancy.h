/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_OCCUP_H
# define ZCL_OCCUP_H

/* PICS.ZCL.Occupancy
 * OS.S | True
 * OS.C | True
 *
 * Occupancy Sensor Type
 * OS.S.OST00 | False
 * OS.S.OST01 | False
 * OS.S.OST02 | False
 *
 * Server Attributes
 * OS.S.A0000 | True
 * OS.S.A0000.Report.Tx | True
 * OS.S.A0001 | True
 * OS.S.A0002 | True
 * OS.S.A0010 | True
 * OS.S.A0011 | True
 * OS.S.A0012 | True
 * OS.S.A0020 | True
 * OS.S.A0021 | True
 * OS.S.A0022 | True
 * OS.S.A0030 | False
 * OS.S.A0031 | False
 * OS.S.A0032 | False
 * OS.S.Afffd | True
 * OS.S.Afffe | False
 *
 * Client Attributes
 * OS.C.A0000.Report.Rsp | False
 * OS.C.Afffd | True
 * OS.C.Afffe | False
 */

#include "zcl/zcl.h"

/* Power Configuration Cluster Attributes */
enum {
    ZCL_OCC_ATTR_OCCUPANCY = 0x0000, /* R- mandatory! */
    ZCL_OCC_ATTR_SENSORTYPE = 0x0001, /* R- mandatory! */
    ZCL_OCC_ATTR_SENSORTYPE_BITMAP = 0x0002, /* R- mandatory! */
    ZCL_OCC_ATTR_PIR_OU_DELAY = 0x0010, /* RW */
    ZCL_OCC_ATTR_PIR_UO_DELAY = 0x0011, /* RW */
    ZCL_OCC_ATTR_PIR_UO_THRESHOLD = 0x0012, /* RW */
    ZCL_OCC_ATTR_US_OU_DELAY = 0x0020, /* RW */
    ZCL_OCC_ATTR_US_UO_DELAY = 0x0021, /* RW */
    ZCL_OCC_ATTR_US_UO_THRESHOLD = 0x0022, /* RW */
    ZCL_OCC_ATTR_PHY_OU_DELAY = 0x0030, /* RW */
    ZCL_OCC_ATTR_PHY_UO_DELAY = 0x0031, /* RW */
    ZCL_OCC_ATTR_PHY_UO_THRESHOLD = 0x0032, /* RW */
};

#define OCC_SENSOR_PIR                      0x00
#define OCC_SENSOR_ULTRASONIC               0x01
#define OCC_SENSOR_PIR_ULTRASONIC           0x02

struct ZbZclClusterT * ZbZclOccupancyClientAlloc(struct ZigBeeT *zb, uint8_t ept);
struct ZbZclClusterT * ZbZclOccupancyServerAlloc(struct ZigBeeT *zb, uint8_t ept);

#endif /* ZCL_OCCUP_H */
