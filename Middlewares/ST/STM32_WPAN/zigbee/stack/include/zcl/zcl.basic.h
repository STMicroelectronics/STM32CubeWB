/**
 * There is no public API to create a new Basic cluster server instance, i.e. ZbZclBasicServerAlloc does not exist. The stack automatically
 * allocates a new instance of the server on every endpoint
 * @file zcl.basic.h
 * @brief ZCL Basic cluster header
 * ZCL 7 section 3.2
 * ZCL 8 section 3.2
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_BASIC_H
# define ZCL_BASIC_H

/* PICS.ZCL.Basic
 * B.S | True
 * B.C | True
 *
 * Server Attributes
 * B.S.A0000 | True
 * B.S.A0001 | True
 * B.S.A0002 | True
 * B.S.A0003 | True
 * B.S.A0004 | True
 * B.S.A0005 | True
 * B.S.A0006 | True
 * B.S.A0007 | True
 * B.S.A0008 | False
 * B.S.A0009 | False
 * B.S.A000a | False
 * B.S.A000b | False
 * B.S.A0010 | True
 * B.S.A0011 | True
 * B.S.A0012 | False (ZSDK-2519)
 * B.S.A0013 | True
 * B.S.A0014 | True
 * B.S.A4000 | True
 * B.S.Afffd | True
 * B.S.Afffe | False
 *
 * Commands Received
 * B.S.C00.Rsp | True
 *
 *
 * Client Attributes
 * B.C.Afffd | True
 * B.C.Afffe | False
 *
 * Commands Generated
 * B.C.C00.Tx | True
 */

#include "zcl/zcl.h"

/** Basic Server Attribute IDs */
enum ZbZclBasicSvrAttrT {
    ZCL_BASIC_ATTR_ZCL_VERSION = 0x0000, /**< ZCLVersion */
    ZCL_BASIC_ATTR_APP_VERSION, /**< ApplicationVersion (Optional) */
    ZCL_BASIC_ATTR_STACK_VERSION, /**< StackVersion (Optional) */
    ZCL_BASIC_ATTR_HARDWARE_VERSION, /**< HWVersion (Optional) */
    ZCL_BASIC_ATTR_MFR_NAME, /**< ManufacturerName (Optional) */
    ZCL_BASIC_ATTR_MODEL_NAME, /**< ModelIdentifier (Optional) */
    ZCL_BASIC_ATTR_DATE_CODE, /**< DateCode (Optional) */
    ZCL_BASIC_ATTR_POWER_SOURCE, /**< PowerSource */

    ZCL_BASIC_ATTR_LOCATION = 0x0010, /**< LocationDescription (Optional) */
    ZCL_BASIC_ATTR_ENVIRONMENT, /**< PhysicalEnvironment (Optional) */
    ZCL_BASIC_ATTR_ENABLED, /**< DeviceEnabled (Optional) */
    ZCL_BASIC_ATTR_ALARM_MASK, /**< AlarmMask (Optional) */
    ZCL_BASIC_ATTR_DISABLE_LOCAL_CONFIG, /**< DisableLocalConfig (Optional) */

    ZCL_BASIC_ATTR_SW_BUILD_ID = 0x4000 /**< SWBuildID (Optional) */
};

/* Power Source Enumerations */
#define ZCL_BASIC_POWER_UNKNOWN                  0x00
#define ZCL_BASIC_POWER_SINGLE_PHASE             0x01
#define ZCL_BASIC_POWER_THREE_PHASE              0x02
#define ZCL_BASIC_POWER_BATTERY                  0x03
#define ZCL_BASIC_POWER_DC                       0x04
#define ZCL_BASIC_POWER_EMERGENCY_CONSTANT       0x05
#define ZCL_BASIC_POWER_EMERGENCY_TRANSFER       0x06
#define ZCL_BASIC_POWER_BATTERY_BACKUP_BIT       0x80

/* Physical Environment Enumerations */
#define ZCL_BASIC_ENVIRONMENT_UNSPECIFIED        0x00
#define ZCL_BASIC_ENVIRONMENT_MIRROR_SUPPORT     0x01
#define ZCL_BASIC_ENVIRONMENT_UNKNOWN            0xff

/* Alarm Codes */
#define ZCL_BASIC_ALARM_CODE_HARDWARE            0x00
#define ZCL_BASIC_ALARM_CODE_SOFTWARE            0x01

/* Alarm Mask */
#define ZCL_BASIC_ALARM_MASK_ALL                 0x03U
#define ZCL_BASIC_ALARM_MASK_HARDWARE            0x01U
#define ZCL_BASIC_ALARM_MASK_SOFTWARE            0x02U

/* Only for the ZCL Basic 'ZCLVersion' attribute. This version attribute is unrelated to the ZCL Global Revision Attribute. */
enum {
    ZCL_BASIC_ZCL_VERSION_6 = 0x02U,
    ZCL_BASIC_ZCL_VERSION_7 = 0x03U,
    ZCL_BASIC_ZCL_VERSION_8 = 0x08U /* Starting from ZCL8, this number matches the revision of ZCL. */
};

/* Local Config */
#define ZCL_BASIC_LOCAL_CONFIG_RESET_DISABLE     0x01U
#define ZCL_BASIC_LOCAL_CONFIG_DEVICE_DISABLE    0x02U

/* Commands */
enum {
    ZCL_BASIC_RESET_FACTORY = 0x00
};

/* The Basic Server is a special cluster that has global attributes and
 * behaviour. The API is found in zigbee.h. */

/**
 * Create a new instance of the Basic Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclBasicClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send a Reset to Factory Defaults command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclBasicClientResetReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst);

#endif /* __ZCL_BASIC_H */
