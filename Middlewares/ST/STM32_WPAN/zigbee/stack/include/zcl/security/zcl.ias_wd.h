/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      The public header file for the Home Automation
 *      clusters of the ZCL.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_IAS_WD_H
#define ZCL_IAS_WD_H

/* PICS.ZCL.IASWD
 *
 * IASWD.S | True
 * IASWD.C | True
 *
 * IASWD.S.A0000 | True
 * IASWD.S.Afffd | True
 * IASWD.S.Afffe | False
 * IASWD.S.C00.Rsp | True
 * IASWD.S.C01.Rsp | True
 *
 * IASWD.C.Afffd | True
 * IASWD.C.Afffe | False
 * IASWD.C.C00.Tx | True
 * IASWD.C.C01.Tx | True
 */

#include "zcl/zcl.h"

enum ZbZclIasWdServerAttrT {
    ZCL_IAS_WD_SVR_ATTR_MAX_DURATION = 0x0000,
};

/* Client Generated Commands */
enum ZbZclIasWdClientCommandsT {
    ZCL_IAS_WD_CLI_CMD_START_WARNING = 0x00,
    ZCL_IAS_WD_CLI_CMD_SQUAWK = 0x01,
};

enum ZbZclIasWdWarningModeT {
    ZCL_IAS_WD_WARNING_MODE_STOP = 0,
    ZCL_IAS_WD_WARNING_MODE_BURGLAR = 1,
    ZCL_IAS_WD_WARNING_MODE_FIRE = 2,
    ZCL_IAS_WD_WARNING_MODE_EMERGENCY = 3,
    ZCL_IAS_WD_WARNING_MODE_POLICE_PANIC = 4,
    ZCL_IAS_WD_WARNING_MODE_FIRE_PANIC = 5,
    ZCL_IAS_WD_WARNING_MODE_EMERGENCY_PANIC = 6,
};

enum ZbZclIasWdStrobeT {
    ZCL_IAS_WD_STROBE_OFF = 0,
    ZCL_IAS_WD_STROBE_ON = 1,
};

enum ZbZclIasWdLevelT {
    ZCL_IAS_WD_LEVEL_LOW = 0,
    ZCL_IAS_WD_LEVEL_MEDIUM = 1,
    ZCL_IAS_WD_LEVEL_HIGH = 2,
    ZCL_IAS_WD_LEVEL_VERY_HIGH = 3,
};

enum ZbZclIasWdSquawkModeT {
    ZCL_IAS_WD_SQUAWK_MODE_ARMED = 0,
    ZCL_IAS_WD_SQUAWK_MODE_DISARMED = 1,
};

/*---------------------------------------------------------------
 * API Structures
 *---------------------------------------------------------------
 */
struct ZbZclIasWdClientStartWarningReqT {
    enum ZbZclIasWdWarningModeT warning_mode;
    enum ZbZclIasWdStrobeT strobe;
    enum ZbZclIasWdLevelT siren_level;
    uint16_t warning_duration;
    uint8_t strobe_dutycycle;
    enum ZbZclIasWdLevelT strobe_level;
};

struct ZbZclIasWdClientSquawkReqT {
    enum ZbZclIasWdSquawkModeT squawk_mode;
    enum ZbZclIasWdStrobeT strobe;
    enum ZbZclIasWdLevelT squawk_level;
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */
struct ZbZclIasWdServerCallbacksT {
    /* Returns a ZCL Status code to send in the Default Response, or
     * ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE if no Default Response
     * to be generated. */
    uint8_t (*start_warning)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclIasWdClientStartWarningReqT *warn_req);

    uint8_t (*squawk)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclIasWdClientSquawkReqT *squawk_req);
};

struct ZbZclClusterT * ZbZclIasWdServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclIasWdServerCallbacksT *callbacks, void *arg);

void ZbZclIasWdServerConfigCallbacks(struct ZbZclClusterT *clusterPtr,
    struct ZbZclIasWdServerCallbacksT *callbacks);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclIasWdClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, void *arg);

uint8_t ZbZclIasWdClientStartWarningReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    struct ZbZclIasWdClientStartWarningReqT *warning_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

uint8_t ZbZclIasWdClientSquawkReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    struct ZbZclIasWdClientSquawkReqT *squawk_req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif /* __ZCL_IAS_WD_H */
