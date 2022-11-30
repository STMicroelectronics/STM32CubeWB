/**
 * @file zcl.meter.id.h
 * @heading Meter Identification
 * @brief ZCL Meter Identification cluster header
 * ZCL 7 section 10.3
 * ZCL 8 section 10.3
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_METER_ID_H
#define ZCL_METER_ID_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Meter Identification cluster.
 *--------------------------------------------------------------------------
 */

/* @PICS.ZCL.Meter.ID
 * MTRID.S | Server | True
 * MTRID.C | Client | True
 *
 * Server Attributes
 * MTRID.S.A0000 | CompanyName| True
 * MTRID.S.A0001 | MeterTypeID | True
 * MTRID.S.A0004 | DataQualityID | True
 * MTRID.S.A0005 | CustomerName | False | Optional
 * MTRID.S.A0006 | Model | False | Optional
 * MTRID.S.A0007 | PartNumber | False | Optional
 * MTRID.S.A0008 | ProductRevision | False | Optional
 * MTRID.S.A000a | SoftwareRevision | False | Optional
 * MTRID.S.A000b | UtilityName | False | Optional
 * MTRID.S.A000c | POD | True
 * MTRID.S.A000d | AvailablePower | True
 * MTRID.S.A000e | PowerThreshold | True
 * MTRID.S.Afffd | ClusterRevision | True
 * MTRID.S.Afffe | AttributeReportingStatus | True
 */

/** Attribute Identifiers */
enum ZbZclMeterIdSvrAttrT {
    ZCL_METER_ID_ATTR_COMPANY_NAME = 0x0000, /**< CompanyName */
    ZCL_METER_ID_ATTR_METER_TYPE_ID = 0x0001, /**< MeterTypeID */
    ZCL_METER_ID_ATTR_DATA_QUAL_ID = 0x0004, /**< DataQualityID */
    ZCL_METER_ID_ATTR_CUSTOMER_NAME = 0x0005, /**< CustomerName (Optional) */
    ZCL_METER_ID_ATTR_MODEL = 0x0006, /**< Model (Optional) */
    ZCL_METER_ID_ATTR_PART_NUMBER = 0x0007, /**< PartNumber (Optional) */
    ZCL_METER_ID_ATTR_PRODUCT_REV = 0x0008, /**< ProductRevision (Optional) */
    ZCL_METER_ID_ATTR_SOFTWARE_REV = 0x000A, /**< SoftwareRevision (Optional) */
    ZCL_METER_ID_ATTR_UTILITY_NAME = 0x000B, /**< UtilityName (Optional) */
    ZCL_METER_ID_ATTR_POD = 0x000C, /**< POD */
    ZCL_METER_ID_ATTR_AVAILABLE_POWER = 0x000D, /**< AvailablePower */
    ZCL_METER_ID_ATTR_POWER_THRESH = 0x000E /**< PowerThreshold */
};

/* Level Status values */
#define ZCL_METER_ID_COMPANY_NAME_LEN_MAX   16U
#define ZCL_METER_ID_POD_LEN_MAX            16U

/*---------------------------------------------------------------
 * Meter Identification Cluster Definitions
 *---------------------------------------------------------------
 */

/**
 * Instantiate a new instance of the Meter Identification Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclMeterIdClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Instantiate a new instance of the Meter Identification Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclMeterIdServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_METER_ID_H */
