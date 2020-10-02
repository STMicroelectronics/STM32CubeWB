/* Copyright [2017 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_FAN_H
#define ZCL_FAN_H

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

/** Fan Control Server Attribute IDs */
enum ZbZclFanSvrAttrT {
    ZCL_FAN_ATTR_MODE = 0x0000, /**< FanMode */
    ZCL_FAN_ATTR_SEQUENCE = 0x00001, /**< FanModeSequence */
};

/** Fan Mode Attribute Values */
enum ZbZclFanModeT {
    ZCL_FAN_MODE_OFF = 0x00, /**< Off */
    ZCL_FAN_MODE_LOW = 0x01, /**< Low */
    ZCL_FAN_MODE_MED = 0x02, /**< Medium */
    ZCL_FAN_MODE_HI = 0x03, /**< High */
    ZCL_FAN_MODE_ON = 0x04, /**< On */
    ZCL_FAN_MODE_AUTO = 0x05, /**< Auto (the fan speed is self-regulated) */
    ZCL_FAN_MODE_SMART = 0x06 /**< Smart (when the heated/cooled space is occupied, the fan is always on) */
};

/** Fan Sequence Operation Attribute Values */
enum ZbZclFanSeqT {
    ZCL_FAN_SEQ_LMH = 0x00, /**< Low/Med/High */
    ZCL_FAN_SEQ_LH = 0x01, /**< Low/High */
    ZCL_FAN_SEQ_LMHA = 0x02, /**< Low/Med/High/Auto */
    ZCL_FAN_SEQ_LHA = 0x03, /**< Low/High/Auto */
    ZCL_FAN_SEQ_OA = 0x04 /**< On/Auto */
};

struct ZbZclClusterT * ZbZclFanClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclFanServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif
