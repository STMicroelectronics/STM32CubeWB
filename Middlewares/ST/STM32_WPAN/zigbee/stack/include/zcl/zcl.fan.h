/* Copyright [2017 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_FAN_H
#define ZCL_FAN_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      ZCL FAN cluster definitions
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Fan
 *
 * FAN.S | True
 * FAN.C | True
 *
 * Server Attributes
 * FAN.S.A0000 | True
 * FAN.S.A0001 | True
 * FAN.S.Afffd | True
 * FAN.S.Afffe | False
 *
 *
 * Client Attributes
 * FAN.C.Afffd | True
 * FAN.C.Afffe | False
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------
 */
/* Attribute Identifiers */
enum {
    ZCL_FAN_ATTR_MODE = 0x0000,
    ZCL_FAN_ATTR_SEQUENCE = 0x00001,
};

/* FAN_MODES */
#define FAN_MODE_OFF  0x00
#define FAN_MODE_LOW  0x01
#define FAN_MODE_MED  0x02
#define FAN_MODE_HI   0x03
#define FAN_MODE_ON   0x04
#define FAN_MODE_AUTO 0x05
#define FAN_MODE_SMART  0x06

/* FAN_SEQUENCES */
#define FAN_SEQ_LMH     0x00
#define FAN_SEQ_LH      0x01
#define FAN_SEQ_LMHA    0x02
#define FAN_SEQ_LHA     0x03
#define FAN_SEQ_OA      0x04

struct ZbZclClusterT * ZbZclFanClientAlloc(struct ZigBeeT *zb, uint8_t ep);
struct ZbZclClusterT * ZbZclFanServerAlloc(struct ZigBeeT *zb, uint8_t ep);

#endif /* __ZCL_DEVICE_TEMP_H */
