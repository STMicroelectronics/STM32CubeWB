/* Copyright [2019 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_METER_ID_H
#define ZCL_METER_ID_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Meter Identification cluster.
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Electrical.Measurement
 * MTRID.S | True
 * MTRID.C | True
 *
 * Server Attributes
 * MTRID.S.A0000 | True
 * MTRID.S.A0001 | True
 * MTRID.S.A0004 | True
 * MTRID.S.A0005 | False
 * MTRID.S.A0006 | False
 * MTRID.S.A0007 | False
 * MTRID.S.A0008 | False
 * MTRID.S.A000a | False
 * MTRID.S.A000b | False
 * MTRID.S.A000c | True
 * MTRID.S.A000d | True
 * MTRID.S.A000e | True
 * MTRID.S.Afffd | True
 * MTRID.S.Afffe | False
 */

/* Attribute Identifiers */
enum ZbZclMeterIdSvrAttrT {
    ZCL_METER_ID_ATTR_COMPANY_NAME = 0x0000,
    ZCL_METER_ID_ATTR_METER_TYPE_ID = 0x0001,
    ZCL_METER_ID_ATTR_DATA_QUAL_ID = 0x0004,
    ZCL_METER_ID_ATTR_CUSTOMER_NAME = 0x0005,
    ZCL_METER_ID_ATTR_MODEL = 0x0006,
    ZCL_METER_ID_ATTR_PART_NUMBER = 0x0007,
    ZCL_METER_ID_ATTR_PRODUCT_REV = 0x0008,
    ZCL_METER_ID_ATTR_SOFTWARE_REV = 0x000A,
    ZCL_METER_ID_ATTR_UTILITY_NAME = 0x000B,
    ZCL_METER_ID_ATTR_POD = 0x000C,
    ZCL_METER_ID_ATTR_AVAILABLE_POWER = 0x000D,
    ZCL_METER_ID_ATTR_POWER_THRESH = 0x000E
};

#define ZCL_METER_ID_COMPANY_NAME_LEN_MAX   16U
#define ZCL_METER_ID_POD_LEN_MAX            16U

/*---------------------------------------------------------------
 * Meter Identification Cluster Definitions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclMeterIdClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclMeterIdServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_METER_ID_H */
