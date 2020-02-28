/* Copyright [2017 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_DEVICE_TEMP_H
#define ZCL_DEVICE_TEMP_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      ZCL Device Temperature Configuration cluster definitions
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Device_Temo
 * DTMP.S | True
 * DTMP.C | True
 *
 * Server Attributes
 * DTMP.S.A0000 | True
 * DTMP.S.A0001 | True
 * DTMP.S.A0002 | True
 * DTMP.S.A0003 | False
 * DTMP.S.A0010 | True
 * DTMP.S.A0011 | True
 * DTMP.S.A0012 | True
 * DTMP.S.A0013 | True
 * DTMP.S.A0014 | True
 * DTMP.S.Afffd | True
 * DTMP.S.Afffe | False
 *
 *
 * Client Attributes
 * DTMP.C.Afffd | True
 * DTMP.C.Afffe | False
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */
/* Attribute Identifiers */
enum {
    ZCL_DEV_TEMP_CURRENT = 0x0000,
    ZCL_DEV_TEMP_MIN_TEMP = 0x0001,
    ZCL_DEV_TEMP_MAX_TEMP = 0x0002,
    ZCL_DEV_TEMP_OVER_TEMP_DWELL = 0x0003, /* not currently supported */
    ZCL_DEV_TEMP_ALARM_MASK = 0x0010,
    ZCL_DEV_TEMP_LOW_THRESHOLD = 0x0011,
    ZCL_DEV_TEMP_HIGH_THRESHOLD = 0x0012,
    ZCL_DEV_TEMP_LOW_DWELL_TRIP = 0x0013,
    ZCL_DEV_TEMP_HIGH_DWELL_TRIP = 0x0014,
};

#define ZCL_DEVICE_TEMP_MIN                         (-200)
#define ZCL_DEVICE_TEMP_MAX                         200

/* Indicates an invalid temperature, or if set to a threshold value,
 * the alarm for that threshold is disabled. */
#define ZCL_DEVICE_TEMP_INVALID                     ((int16_t)0x8000)

/* Alarm Mask */
enum {
    ZCL_DEV_TEMP_ALARM_MASK_CLEAR = 0x00,
    ZCL_DEV_TEMP_ALARM_MASK_LOW = 0x01,
    ZCL_DEV_TEMP_ALARM_MASK_HIGH = 0x02,
};

/* Alarms */
enum {
    ZCL_DEV_TEMP_ALARM_CODE_LOW = 0x00,
    ZCL_DEV_TEMP_ALARM_CODE_HIGH = 0x01,
};

struct ZbZclClusterT * ZbZclDevTempClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclDevTempServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_DEVICE_TEMP_H */
