/**
 * @file zcl.device.temp.h
 * @brief ZCL Device Temperature Configuration cluster header
 * ZCL 7 section 3.4
 * ZCL 8 section 3.4
 * @copyright Copyright [2017 - 2020] Exegin Technologies Limited. All rights reserved.
 */

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

/** Device Temp Cluster Attribute Ids */
enum ZbZclDeviceTempSvrAttrT {
    ZCL_DEV_TEMP_CURRENT = 0x0000, /**< CurrentTemperature */
    ZCL_DEV_TEMP_MIN_TEMP = 0x0001, /**< MinTempExperienced */
    ZCL_DEV_TEMP_MAX_TEMP = 0x0002, /**< MaxTempExperienced */
    ZCL_DEV_TEMP_OVER_TEMP_DWELL = 0x0003, /**< OverTempTotalDwell */
    ZCL_DEV_TEMP_ALARM_MASK = 0x0010, /**< DeviceTempAlarmMask */
    ZCL_DEV_TEMP_LOW_THRESHOLD = 0x0011, /**< LowTempThreshold */
    ZCL_DEV_TEMP_HIGH_THRESHOLD = 0x0012, /**< HighTempThreshold */
    ZCL_DEV_TEMP_LOW_DWELL_TRIP = 0x0013, /**< LowTempDwellTripPoint */
    ZCL_DEV_TEMP_HIGH_DWELL_TRIP = 0x0014, /**< HighTempDwellTripPoint */
};

#define ZCL_DEVICE_TEMP_MIN                         (-200)
#define ZCL_DEVICE_TEMP_MAX                         200

/* Indicates an invalid temperature, or if set to a threshold value,
 * the alarm for that threshold is disabled. */
#define ZCL_DEVICE_TEMP_INVALID                     ((int16_t)0x8000)

/* Alarm Mask */
enum ZbZclDeviceTempAlarmMask {
    ZCL_DEV_TEMP_ALARM_MASK_CLEAR = 0x00,
    ZCL_DEV_TEMP_ALARM_MASK_LOW = 0x01,
    ZCL_DEV_TEMP_ALARM_MASK_HIGH = 0x02,
};

/* Alarms */
enum ZbZclDeviceTempAlarmCode {
    ZCL_DEV_TEMP_ALARM_CODE_LOW = 0x00,
    ZCL_DEV_TEMP_ALARM_CODE_HIGH = 0x01,
};

/**
 * Instantiate a new instance of the Device Temp server cluster.
 * @param zb Zigbee stack instance.
 * @param endpoint APS endpoint to create cluster on.
 * @return Cluster pointer, or NULL if there as an error.
 */
struct ZbZclClusterT * ZbZclDevTempClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Instantiate a new instance of the Device Temp client cluster.
 * @param zb Zigbee stack instance.
 * @param endpoint APS endpoint to create cluster on.
 * @return Cluster pointer, or NULL if there as an error.
 */
struct ZbZclClusterT * ZbZclDevTempServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_DEVICE_TEMP_H */
