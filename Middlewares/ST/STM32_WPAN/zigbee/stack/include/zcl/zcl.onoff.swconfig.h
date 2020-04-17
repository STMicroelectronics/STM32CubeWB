/* Copyright [2019 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_ONOFF_SWCONFIG_H
#define ZCL_ONOFF_SWCONFIG_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL OnOff cluster.
 *
 *      This cluster is dependent on the OnOff client cluster, as the switch
 *      actions attribute specifies the commands of the OnOff cluster to be
 *      generated when the switch moves between it's two states.
 *--------------------------------------------------------------------------
 */

/* Attribute Identifiers */
enum {
    ZCL_ONOFF_SWCONFIG_ATTR_TYPE = 0x0000, /* R- mandatory! */
    ZCL_ONOFF_SWCONFIG_ATTR_ACTIONS = 0x0010 /* RW mandatory! */
};

/* Switch Info values */
enum {
    ZCL_ONOFF_SWCONFIG_TOGGLE = 0x00,
    ZCL_ONOFF_SWCONFIG_MOMENTARY = 0x01,
    ZCL_ONOFF_SWCONFIG_MULTIFUNCTION = 0x02
};

/* Switch Settings values */
enum {
    ZCL_ONOFF_SWCONFIG_ON_OFF = 0x00,
    ZCL_ONOFF_SWCONFIG_OFF_ON = 0x01,
    ZCL_ONOFF_SWCONFIG_TOGGLE_TOGGLE = 0x02
};

/*---------------------------------------------------------------
 * OnOff Switch Configuration Server Cluster
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclOnOffSwConfigServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint8_t switch_type);

/*---------------------------------------------------------------
 * OnOff Switch Configuration Client Cluster
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclOnOffSwConfigClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_ONOFF_SWCONFIG_H */
