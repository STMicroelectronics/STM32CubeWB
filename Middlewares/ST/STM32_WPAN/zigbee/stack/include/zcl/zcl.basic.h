/**
 * @file zcl.basic.h
 * @brief ZCL Basic cluster header
 * ZCL 7 section 3.2
 * ZCL 8 section 3.2
 * @copyright Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved.
 */

/*
 * Client and Server support for the ZCL Basic Cluster
 *
 * The Basic cluster is unique amoung the ZCL clusters in ZSDK. There is no
 * public API to create a new instance, i.e. ZbZclBasicServerAlloc does not
 * exist. The stack automatically allocates a new instance of the Basic
 * server cluster on every endpoint ...MORE HERE
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

/** Basic Cluster Attribute Ids */
enum ZbZclBasicSvrAttrT {
    ZCL_BASIC_ATTR_ZCL_VERSION = 0x0000, /**< ZCLVersion */
    ZCL_BASIC_ATTR_APP_VERSION, /**< ApplicationVersion */
    ZCL_BASIC_ATTR_STACK_VERSION, /**< StackVersion */
    ZCL_BASIC_ATTR_HARDWARE_VERSION, /**< HWVersion */
    ZCL_BASIC_ATTR_MFR_NAME, /**< ManufacturerName */
    ZCL_BASIC_ATTR_MODEL_NAME, /**< ModelIdentifier */
    ZCL_BASIC_ATTR_DATE_CODE, /**< DateCode */
    ZCL_BASIC_ATTR_POWER_SOURCE, /**< PowerSource */

    ZCL_BASIC_ATTR_LOCATION = 0x0010, /**< LocationDescription */
    ZCL_BASIC_ATTR_ENVIRONMENT, /**< PhysicalEnvironment */
    ZCL_BASIC_ATTR_ENABLED, /**< DeviceEnabled */
    ZCL_BASIC_ATTR_ALARM_MASK, /**< AlarmMask */
    ZCL_BASIC_ATTR_DISABLE_LOCAL_CONFIG, /**< DisableLocalConfig */

    ZCL_BASIC_ATTR_SW_BUILD_ID = 0x4000 /**< SWBuildID */
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

/* ZCL Version Identifier. */
#define ZCL_BASIC_ZCL_VERSION                    0x02

/* Local Config */
#define ZCL_BASIC_LOCAL_CONFIG_RESET_DISABLE     0x01U
#define ZCL_BASIC_LOCAL_CONFIG_DEVICE_DISABLE    0x02U

/* Commands */
enum {
    ZCL_BASIC_RESET_FACTORY = 0x00
};

/*---------------------------------------------------------------
 * Basic Server
 *---------------------------------------------------------------
 */
/* The Basic Server is a special cluster that has global attributes and
 * behaviour. The API is found in zigbee.h. */

/**
 * Creates a new instance of the Basic client cluster
 *
 * @param zb the ZSDK stack pointer
 * @param endpoint the endpoint on which the new cluster instance will reside
 * @return a new Basic client cluster or NULL on error
 */
struct ZbZclClusterT * ZbZclBasicClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send Basic Cluster Reset to Factory Defaults command to the server
 *
 * The reset to factory defaults commands instructs the server to resets all attributes
 * to their factory default values. Other functionality is not affected.
 *
 * @param clusterPtr the client cluster from which to send the command
 * @param dst the destination address to the server to which the command is sent
 * @return ZCL_STATUS_SUCEESS if successful or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclBasicClientResetReq(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst);

#endif /* __ZCL_BASIC_H */
