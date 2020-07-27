/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_LEVEL_H
#define ZCL_LEVEL_H

/* PICS.ZCL.Level
 *
 * LC.S | TRUE
 * LC.C | TRUE
 *
 * Server Attributes
 * LC.S.A0000 | TRUE
 * LC.S.A0000.Scene | TRUE
 * LC.S.A0000.Report.Tx | TRUE
 * LC.S.A0001 | FALSE
 * LC.S.A000f | TRUE
 * LC.S.A0010 | FALSE
 * LC.S.A0011 | FALSE
 * LC.S.A0012 | FALSE
 * LC.S.A0013 | FALSE
 * LC.S.A0014 | FALSE
 * LC.S.A4000 | FALSE
 * LC.S.Afffd | TRUE
 *
 * Commands Received
 * LC.S.C00.Rsp | TRUE
 * LC.S.C01.Rsp | TRUE
 * LC.S.C02.Rsp | TRUE
 * LC.S.C03.Rsp | TRUE
 * LC.S.C04.Rsp | TRUE
 * LC.S.C05.Rsp | TRUE
 * LC.S.C06.Rsp | TRUE
 * LC.S.C07.Rsp | TRUE
 *
 *
 * Client Attributes
 * LC.C.A0000.Report.Rsp | FALSE
 * LC.C.Afffd | TRUE
 *
 * Commands Generated
 * LC.S.C00.Tx | TRUE
 * LC.S.C01.Tx | TRUE
 * LC.S.C02.Tx | TRUE
 * LC.S.C03.Tx | TRUE
 * LC.S.C04.Tx | TRUE
 * LC.S.C05.Tx | TRUE
 * LC.S.C06.Tx | TRUE
 * LC.S.C07.Tx | TRUE
 *
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Level Control Cluster
 *---------------------------------------------------------------
 */

/* Level Server Attribute IDs */
enum ZbZclLevelSvrAttrT {
    ZCL_LEVEL_ATTR_CURRLEVEL = 0x0000,
    ZCL_LEVEL_ATTR_REMAINTIME = 0x0001,
    ZCL_LEVEL_ATTR_MINLEVEL = 0x0002,
    ZCL_LEVEL_ATTR_MAXLEVEL = 0x0003,
    ZCL_LEVEL_ATTR_CURRFREQ = 0x0004,
    ZCL_LEVEL_ATTR_MINFREQ = 0x0005,
    ZCL_LEVEL_ATTR_MAXFREQ = 0x0006,
    ZCL_LEVEL_ATTR_OPTIONS = 0x000f,
    ZCL_LEVEL_ATTR_ONOFF_TRANS_TIME = 0x0010,
    ZCL_LEVEL_ATTR_ONLEVEL = 0x0011,
    ZCL_LEVEL_ATTR_ON_TRANS_TIME = 0x0012,
    ZCL_LEVEL_ATTR_OFF_TRANS_TIME = 0x0013,
    ZCL_LEVEL_ATTR_DEFAULT_MOVE_RATE = 0x0014,
    ZCL_LEVEL_ATTR_STARTUP_CURRLEVEL = 0x4000
};

/* Level Command IDs  */
enum {
    ZCL_LEVEL_COMMAND_MOVELEVEL = 0x00,
    ZCL_LEVEL_COMMAND_MOVE = 0x01,
    ZCL_LEVEL_COMMAND_STEP = 0x02,
    ZCL_LEVEL_COMMAND_STOP = 0x03,
    ZCL_LEVEL_COMMAND_MOVELEVEL_ONOFF = 0x04,
    ZCL_LEVEL_COMMAND_MOVE_ONOFF = 0x05,
    ZCL_LEVEL_COMMAND_STEP_ONOFF = 0x06,
    ZCL_LEVEL_COMMAND_STOP_ONOFF = 0x07
};

/* Move mode field */
enum {
    ZCL_LEVEL_MODE_UP = 0x00,
    ZCL_LEVEL_MODE_DOWN = 0x01
};

#define ZCL_LEVEL_OPTIONS_EXECUTE_IF_OFF            0x01
#define ZCL_LEVEL_OPTIONS_COUPLE_TO_COLOR           0x02

#define ZCL_LEVEL_MINIMUM_LEVEL                     0x00
#define ZCL_LEVEL_MAXIMUM_LEVEL                     0xFE
#define ZCL_LEVEL_ILLEGAL_LEVEL                     0xFF

#define ZCL_LEVEL_MINIMUM_RATE                      0x00
#define ZCL_LEVEL_MAXIMUM_RATE                      0xFE
#define ZCL_LEVEL_ILLEGAL_RATE                      0xFF

#define ZCL_LEVEL_MINIMUM_TRANS_TIME                0x0000
#define ZCL_LEVEL_MAXIMUM_TRANS_TIME                0xFFFE
#define ZCL_LEVEL_ILLEGAL_TRANS_TIME                0xFFFF

/* The CoupleColorTempToLevel bit (bit 1) of the Options attribute indicates whether color
 * temperature in the color cluster is to be linked with current level in this cluster. If the
 * CoupleColorTempToLevel bit is 1 and ColorMode or EnhancedColorMode is 0x02, then changes in
 * Current Level shall effect the ColorTemperatureMireds attribute. This relationship is
 * defined by the manufacturer, with the qualification that the maximum value of CurrentLevel
 * shall correspond to a ColorTemperatureMired value equal to the CoupleColorTempToLevelMinMireds
 * attribute. A low value of the CurrentLevel attribute shall be associated with a high value of
 * the ColorTemperatureMireds attribute.
 */

/*-----------------------------------------------------------------------------
 * Structures
 *-----------------------------------------------------------------------------
 */
struct ZbZclLevelClientMoveToLevelReqT {
    bool with_onoff; /* If true then cmd is ZCL_LEVEL_COMMAND_MOVELEVEL_ONOFF, else ZCL_LEVEL_COMMAND_MOVELEVEL */
    uint8_t level;
    uint16_t transition_time;
    /* Note: OptionsMask and OptionsOverride are not included in the "With OnOff"
     * version of this command */
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclLevelClientMoveReqT {
    bool with_onoff; /* If true then cmd is ZCL_LEVEL_COMMAND_MOVE_ONOFF, else ZCL_LEVEL_COMMAND_MOVE */
    uint8_t mode;
    uint8_t rate;
    /* Note: OptionsMask and OptionsOverride are not included in the "With OnOff"
     * version of this command */
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclLevelClientStepReqT {
    bool with_onoff; /* If true then cmd is ZCL_LEVEL_COMMAND_STEP_ONOFF, else ZCL_LEVEL_COMMAND_STEP */
    uint8_t mode;
    uint8_t size;
    uint16_t transition_time;
    /* Note: OptionsMask and OptionsOverride are not included in the "With OnOff"
     * version of this command */
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclLevelClientStopReqT {
    bool with_onoff; /* If true then cmd is ZCL_LEVEL_COMMAND_STOP_ONOFF, else ZCL_LEVEL_COMMAND_STOP */
    /* Note: OptionsMask and OptionsOverride are not included in the "With OnOff"
     * version of this command */
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

/*-----------------------------------------------------------------------------
 * Server
 *-----------------------------------------------------------------------------
 */

struct ZbZclLevelServerCallbacksT {
    /* Returns a ZCL Status code to send in the Default Response, or ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE
     * if no Default Response to be generated. */

    enum ZclStatusCodeT (*move_to_level)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientMoveToLevelReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*move)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientMoveReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*step)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientStepReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*stop)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientStopReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
};

/**
 * Allocate the Level Server cluster.
 * @param zb
 * @param endpoint
 * @param onoff_server OnOff Server cluster pointer for processing commands with the Options
 * fields. May be NULL.
 * @param callback
 * @param arg
 * @return ZCL Status Code
 */
struct ZbZclClusterT * ZbZclLevelServerAlloc(struct ZigBeeT *zb,
    uint8_t endpoint, struct ZbZclClusterT *onoff_server,
    struct ZbZclLevelServerCallbacksT *callbacks, void *arg);

/*-----------------------------------------------------------------------------
 * Client
 *-----------------------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclLevelClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

enum ZclStatusCodeT ZbZclLevelClientMoveToLevelReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientMoveToLevelReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclLevelClientMoveReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientMoveReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclLevelClientStepReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientStepReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclLevelClientStopReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientStopReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_LEVEL_H */
