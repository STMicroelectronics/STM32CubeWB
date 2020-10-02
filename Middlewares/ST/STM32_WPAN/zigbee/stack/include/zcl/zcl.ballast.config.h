/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_BALLAST_CONFIG_H
# define ZCL_BALLAST_CONFIG_H

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Ballast Configuration cluster.
 *--------------------------------------------------------------------------
 */
/* PICS.ZCL.Ballast_Config
 * BC.S | True
 * BC.C | True
 *
 * Server Attributes
 * BC.S.A0000 | True
 * BC.S.A0001 | True
 * BC.S.A0002 | False
 * BC.S.A0010 | True
 * BC.S.A0011 | True
 * BC.S.A0014 | False
 * BC.S.A0015 | False
 * BC.S.A0020 | False
 * BC.S.A0030 | False
 * BC.S.A0031 | False
 * BC.S.A0032 | False
 * BC.S.A0033 | False
 * BC.S.A0034 | False
 * BC.S.A0035 | False
 * BC.S.Afffd | True
 * BC.S.Afffe | False
 *
 *
 * Client Attributes
 * BC.C.Afffd | True
 * BC.C.Afffe | False
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Ballast Configuration Cluster Definitions
 *---------------------------------------------------------------
 */
enum ZbZclBallastConfigSvrAttrT {
    /* Ballast Information Attribute Set */
    ZCL_BALLAST_CONFIG_ATTR_PHY_MIN_LEVEL = 0x0000,
    ZCL_BALLAST_CONFIG_ATTR_PHY_MAX_LEVEL = 0x0001,
    ZCL_BALLAST_CONFIG_ATTR_BALLAST_STATUS = 0x0002,

    /* Ballast Settings Attribute Set */
    ZCL_BALLAST_CONFIG_ATTR_MIN_LEVEL = 0x0010,
    ZCL_BALLAST_CONFIG_ATTR_MAX_LEVEL = 0x0011,
    /* deprecated - PowerOnLevel */
    ZCL_BALLAST_CONFIG_ATTR_POWER_ON_LEVEL = 0x0012,
    /* deprecated - PowerOnFadeTime */
    ZCL_BALLAST_CONFIG_ATTR_POWER_ON_FADE_TIME = 0x0013,
    ZCL_BALLAST_CONFIG_ATTR_INTRINSIC_BALLAST_FACTOR = 0x0014,
    ZCL_BALLAST_CONFIG_ATTR_BALLAST_FACTOR_ADJUSTMENT = 0x0015,

    /* Lamp Information Attribute Set */
    ZCL_BALLAST_CONFIG_ATTR_LAMP_QUANTITY = 0x0020,

    /* Lamp Settings Attribute Set */
    ZCL_BALLAST_CONFIG_ATTR_LAMP_TYPE = 0x0030,
    ZCL_BALLAST_CONFIG_ATTR_LAMP_MANUFACTURER = 0x0031,
    ZCL_BALLAST_CONFIG_ATTR_LAMP_RATED_HOURS = 0x0032,
    ZCL_BALLAST_CONFIG_ATTR_LAMP_BURN_HOURS = 0x0033,
    ZCL_BALLAST_CONFIG_ATTR_LAMP_ALARM_MODE = 0x0034,
    ZCL_BALLAST_CONFIG_ATTR_LAMP_BURN_HOURS_TRIP_POINT = 0x0035,
};

/* Ballast Status (bit mask) defines */
#define ZCL_BALLAST_CONFIG_STATUS_NON_OP                         0x01 /* Ballast Non-operational */
#define ZCL_BALLAST_CONFIG_STATUS_LAMP_FAIL                      0x02 /* Lamp Failure */

/* Ballast Information attribute set defines */
#define ZCL_BALLAST_CONFIG_PHY_MIN_LEVEL_DEFAULT                 0x01
#define ZCL_BALLAST_CONFIG_PHY_MIN_LEVEL_MIN                     0x01
#define ZCL_BALLAST_CONFIG_PHY_MIN_LEVEL_MAX                     0xfe

#define ZCL_BALLAST_CONFIG_PHY_MAX_LEVEL_DEFAULT                 0xfe
#define ZCL_BALLAST_CONFIG_PHY_MAX_LEVEL_MIN                     0x01
#define ZCL_BALLAST_CONFIG_PHY_MAX_LEVEL_MAX                     0xfe

#define ZCL_BALLAST_CONFIG_BALLAST_STATUS_DEFAULT                0x00
#define ZCL_BALLAST_CONFIG_BALLAST_STATUS_MAX                    0x03

#define ZCL_BALLAST_CONFIG_BALLAST_STATUS_NON_OPERATIONAL_BIT    0x00
#define ZCL_BALLAST_CONFIG_BALLAST_STATUS_LAMP_FAILURE_BIT       0x01
#define ZCL_BALLAST_CONFIG_BALLAST_STATUS_BIT_MASK  \
    ((1 << ZCL_BALLAST_CONFIG_BALLAST_STATUS_NON_OPERATIONAL_BIT) | \
     (1 << ZCL_BALLAST_CONFIG_BALLAST_STATUS_LAMP_FAILURE_BIT))

/* Ballast Configuration Settings attribute set defines */
#define ZCL_BALLAST_CONFIG_MIN_LEVEL_MIN                         0x01
#define ZCL_BALLAST_CONFIG_MIN_LEVEL_MAX                         0xfe

#define ZCL_BALLAST_CONFIG_MAX_LEVEL_MIN                         0x01
#define ZCL_BALLAST_CONFIG_MAX_LEVEL_MAX                         0xfe

#define ZCL_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_DEFAULT      0xff
#define ZCL_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_MIN          0x00
#define ZCL_BALLAST_CONFIG_INTRINSIC_BALLAST_FACTOR_MAX          0xfe

#define ZCL_BALLAST_CONFIG_BALLAST_FACTOR_ADJ_DEFAULT            0xff
#define ZCL_BALLAST_CONFIG_BALLAST_FACTOR_ADJ_MIN                0x64
#define ZCL_BALLAST_CONFIG_BALLAST_FACTOR_ADJ_MAX                0xfe

/* Ballast Settings attribute set defines */
#define ZCL_BALLAST_CONFIG_LAMP_QUANTITY_DEFAULT                 0x00
#define ZCL_BALLAST_CONFIG_LAMP_QUANTITY_MIN                     0x00
#define ZCL_BALLAST_CONFIG_LAMP_QUANTITY_MAX                     0xfe

/* Ballast Settings attribute set defines */
#define ZCL_BALLAST_CONFIG_LAMP_TYPE_NAME_LENGTH                 0x10
#define ZCL_BALLAST_CONFIG_LAMP_MANUFACTURER_NAME_LENGTH         0x10

#define ZCL_BALLAST_CONFIG_LAMP_RATED_HOURS_DEFAULT              0xffffff
#define ZCL_BALLAST_CONFIG_LAMP_RATED_HOURS_MIN                  0x000000
#define ZCL_BALLAST_CONFIG_LAMP_RATED_HOURS_MAX                  0xfffffe

#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_DEFAULT               0x000000
#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_UNKNOWN               0xffffff
#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_MIN                   0x000000
#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_MAX                   0xfffffe

#define ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_DEFAULT               0x00
#define ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_BIT_MASK              0x01
#define ZCL_BALLAST_CONFIG_LAMP_ALARM_MODE_BIT_POS               0x00
#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOUR_ALARM_CODE             0x01

#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_TRIP_POINT_DEFAULT    0xffffff
#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_TRIP_POINT_MIN        0x000000
#define ZCL_BALLAST_CONFIG_LAMP_BURN_HOURS_MAX                   0xfffffe

/*---------------------------------------------------------------
 * Ballast Configuration Cluster Definitions
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclBallastConfigClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

struct ZbZclClusterT * ZbZclBallastConfigServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, uint8_t phyMin, uint8_t phyMax);

#endif
