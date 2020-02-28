/* Copyright [2019 - 2019] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_THERM_UI_H
#define ZCL_THERM_UI_H

#include "zcl/zcl.h"

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Thermostat UI Configuration
 *      cluster.
 *--------------------------------------------------------------------------
 */

/* PICS.ZCL.Thermostat_UI_Configuration
 * TSUIC.S | True
 * TSUIC.C | True
 *
 * Server Attributes
 * TSUIC.S.A0000 | True
 * TSUIC.S.A0001 | True
 * TSUIC.S.A0002 | False
 * TSUIC.S.Afffd | True
 *
 * Client Attributes
 * TSUIC.C.Afffd | True
 */

/* Attribute Identifiers */
enum {
    ZCL_THERM_UI_ATTR_DISPLAY_MODE = 0x0000,
    ZCL_THERM_UI_ATTR_KEYPAD_LOCKOUT = 0x0001,
    ZCL_THERM_UI_ATTR_SCHEDULE_PROG_VIS = 0x0010,
};

/*---------------------------------------------------------------
 * Thermostat UI Config Cluster Definitions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclThermUiClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);
struct ZbZclClusterT * ZbZclThermUiServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

#endif /* __ZCL_THERM_UI_H */
