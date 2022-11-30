/**
 * @file zcl.onoff.swconfig.h
 * @heading On/Off Switch Configuration
 * @brief ZCL OnOff Switch Configuration cluster header
 * ZCL 7 section 3.9
 * ZCL 8 section 3.9
 * @copyright Copyright [2019 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_ONOFF_SWCONFIG_H
#define ZCL_ONOFF_SWCONFIG_H

/* NOTE: The following PICS statement is for internal Exegin use only, and is not a product of the Zigbee Alliance. */
/* @PICS.ZCL.SwConfig
 * OOSC.S | Server | True
 * OOSC.C | Client | True
 *
 * Server Attributes
 * OOSC.S.A0000 | SwitchType | True
 * OOSC.S.A0010 | SwitchActions | True
 * OOSC.S.Afffd | ClusterRevision | True
 * OOSC.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * OOSC.S.Afffd | ClusterRevision | True
 * OOSC.S.Afffe | AttributeReportingStatus | True
 */

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL OnOff Switch Configuration cluster.
 *
 *      This cluster is dependent on the OnOff client cluster, as the switch
 *      actions attribute specifies the commands of the OnOff cluster to be
 *      generated when the switch moves between it's two states.
 *--------------------------------------------------------------------------
 */

/** Onoff Switch Configuration cluster attribute IDs */
enum ZbZclOnOffSwConfigSvrAttrId {
    ZCL_ONOFF_SWCONFIG_ATTR_TYPE = 0x0000, /**< SwitchType */
    ZCL_ONOFF_SWCONFIG_ATTR_ACTIONS = 0x0010 /**< SwitchActions */
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

/**
 * Create a new instance of the OnOff Switch Configuration Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param switch_type Type of switch associated with this cluster server
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclOnOffSwConfigServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint8_t switch_type);

/*---------------------------------------------------------------
 * OnOff Switch Configuration Client Cluster
 *---------------------------------------------------------------
 */

/**
 * Create a new instance of the OnOff Switch Configuration Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclOnOffSwConfigClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_ONOFF_SWCONFIG_H */
