/**
 * @file zcl.therm.ui.h
 * @heading Thermostat User Interface
 * @brief ZCL Thermostat User Interface cluster header
 * ZCL 7 section 6.6
 * ZCL 8 section 6.6
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_THERM_UI_H
#define ZCL_THERM_UI_H

#include "zcl/zcl.h"

/* @PICS.ZCL.Thermostat_UI_Configuration
 * TSUIC.S | Server | True
 * TSUIC.C | Client | True
 *
 * Server Attributes
 * TSUIC.S.A0000 | TemperatureDisplayMode | True
 * TSUIC.S.A0001 | KeypadLockout | True
 * TSUIC.S.A0002 | ScheduleProgrammingVisibility | False | Optional
 * TSUIC.S.Afffd | ClusterRevision | True
 * TSUIC.S.Afffe | AttributeReportingStatus | True
 *
 * Client Attributes
 * TSUIC.C.Afffd | ClusterRevision | True
 * TSUIC.C.Afffe | AttributeReportingStatus | True
 */

/** Thermostat User Interface Attribute IDs */
enum ZbZclThermUiServerAttrT {
    ZCL_THERM_UI_SVR_ATTR_DISPLAY_MODE = 0x0000, /**< TemperatureDisplayMode */
    ZCL_THERM_UI_SVR_ATTR_KEYPAD_LOCKOUT = 0x0001, /**< KeypadLockout */
    ZCL_THERM_UI_SVR_ATTR_SCHEDULE_PROG_VIS = 0x0010, /**< ScheduleProgrammingVisibility (Optional) */
};

/* Thermostat User Interface Client */
/**
 * Create a new instance of the Thermostat User Interface Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclThermUiClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/* Thermostat User Interface Server */
/**
 * Create a new instance of the Thermostat User Interface Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclThermUiServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif
