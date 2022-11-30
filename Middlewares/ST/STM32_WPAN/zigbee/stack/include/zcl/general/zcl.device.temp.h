/**
 * @file zcl.device.temp.h
 * @heading Device Temperature Configuration
 * @brief ZCL Device Temperature Configuration cluster header
 * ZCL 7 section 3.4
 * ZCL 8 section 3.4
 * @copyright Copyright [2017 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_DEVICE_TEMP_H
#define ZCL_DEVICE_TEMP_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      ZCL Device Temperature Configuration cluster definitions
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Device_Temp
 * DTMP.S | Server | True
 * DTMP.C | Client | True
 *
 * Server Attributes
 * DTMP.S.A0000 | CurrentTemperature | True
 * DTMP.S.A0001 | MinTempExperienced | True | Optional
 * DTMP.S.A0002 | MaxTempExperienced | True | Optional
 * DTMP.S.A0003 | OverTempTotalDwell | False | Optional
 * DTMP.S.A0010 | DeviceTempAlarmMask | True | Optional
 * DTMP.S.A0011 | LowTempThreshold | True | Optional
 * DTMP.S.A0012 | HighTempThreshold | True | Optional
 * DTMP.S.A0013 | LowTempDwellTripPoint | True | Optional
 * DTMP.S.A0014 | HighTempDwellTripPoint | True | Optional
 * DTMP.S.Afffd | ClusterRevision | True
 * DTMP.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * DTMP.C.Afffd | ClusterRevision | True
 * DTMP.C.Afffe | AttributeReportingStatus | True
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */

/** Device Temperature Cluster Attribute Ids */
enum ZbZclDeviceTempSvrAttrT {
    ZCL_DEV_TEMP_CURRENT = 0x0000, /**< CurrentTemperature */
    ZCL_DEV_TEMP_MIN_TEMP = 0x0001, /**< MinTempExperienced (Optional) */
    ZCL_DEV_TEMP_MAX_TEMP = 0x0002, /**< MaxTempExperienced (Optional) */
    ZCL_DEV_TEMP_OVER_TEMP_DWELL = 0x0003, /**< OverTempTotalDwell (Optional) */
    ZCL_DEV_TEMP_ALARM_MASK = 0x0010, /**< DeviceTempAlarmMask (Optional) */
    ZCL_DEV_TEMP_LOW_THRESHOLD = 0x0011, /**< LowTempThreshold (Optional) */
    ZCL_DEV_TEMP_HIGH_THRESHOLD = 0x0012, /**< HighTempThreshold (Optional) */
    ZCL_DEV_TEMP_LOW_DWELL_TRIP = 0x0013, /**< LowTempDwellTripPoint (Optional) */
    ZCL_DEV_TEMP_HIGH_DWELL_TRIP = 0x0014, /**< HighTempDwellTripPoint (Optional) */
};

#define ZCL_DEVICE_TEMP_MIN                         (-200)
#define ZCL_DEVICE_TEMP_MAX                         200

/* Indicates an invalid temperature, or if set to a threshold value,
 * the alarm for that threshold is disabled. */
#define ZCL_DEVICE_TEMP_INVALID                     ((int16_t)0x8000)

#define ZCL_DEVICE_TEMP_ALARM_MASK_ALL              0x03U

/** Device Temperature Configuration Alarm Mask */
enum ZbZclDeviceTempAlarmMask {
    ZCL_DEV_TEMP_ALARM_MASK_CLEAR = 0x00, /**< Alarm mask clear */
    ZCL_DEV_TEMP_ALARM_MASK_LOW = 0x01, /**< Alarm mask low */
    ZCL_DEV_TEMP_ALARM_MASK_HIGH = 0x02, /**< Alarm mask high */
};

/** Device Temperature Configuration Alarm Code */
enum ZbZclDeviceTempAlarmCode {
    ZCL_DEV_TEMP_ALARM_CODE_LOW = 0x00, /**< Device Temperature too low */
    ZCL_DEV_TEMP_ALARM_CODE_HIGH = 0x01, /**< Device Temperature too high */
};

/**
 * Create a new instance of the Device Temperature Configuration Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDevTempClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Instantiate a new instance of the Device Temp client cluster.
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclDevTempServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif
