/* Copyright [2019 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_DEHUM_CTRL_H
#define ZCL_DEHUM_CTRL_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Dehumidification Control
 *      cluster.
 *--------------------------------------------------------------------------
 */

/* Attribute Identifiers */
enum {
    ZCL_DEHUM_CTRL_ATTR_REL_HUM = 0x0000,
    ZCL_DEHUM_CTRL_ATTR_DEHUM_COOLING = 0x0001,
    ZCL_DEHUM_CTRL_ATTR_RHDH_SETPT = 0x0010,
    ZCL_DEHUM_CTRL_ATTR_RH_MODE = 0x0011,
    ZCL_DEHUM_CTRL_ATTR_DH_LOCKOUT = 0x0012,
    ZCL_DEHUM_CTRL_ATTR_DH_HYS = 0x0013,
    ZCL_DEHUM_CTRL_ATTR_DH_MAX_COOL = 0x0014,
    ZCL_DEHUM_CTRL_ATTR_RH_DISPLAY = 0x0015
};

/* Dehumidification Control Defines */
#define ZCL_DEHUM_CTRL_UNKNOWN                      0xff
#define ZCL_DEHUM_CTRL_RHDH_SETPT_DEFAULT           0x32
#define ZCL_DEHUM_CTRL_DH_HYS_DEFAULT               0x02
#define ZCL_DEHUM_CTRL_DH_MAX_COOL_DEFAULT          0x14

/*---------------------------------------------------------------
 * Dehumidification Control Cluster Definitions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclDehumCtrlClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclDehumCtrlServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_DEHUM_CTRL_H */
