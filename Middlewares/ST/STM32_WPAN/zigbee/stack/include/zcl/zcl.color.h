/* Copyright [2016 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_COLOR_H
#define ZCL_COLOR_H

#include "zcl/zcl.h"

/* PICS.ZCL.Color Control(0x0300)
*
* CC.S | TRUE
* CC.C | TRUE
*
* Server Attributes
* CC.S.A0000 | TRUE
* CC.S.A0000.Report.Tx | TRUE
* CC.S.A0001 | TRUE
* CC.S.A0001.Scene | TRUE
* CC.S.A0001.Report.Tx | TRUE
* CC.S.A0002 | FALSE
* CC.S.A0003  | TRUE
* CC.S.A0003.Scene  | TRUE
* CC.S.A0003.Report.Tx  | TRUE
* CC.S.A0004  | TRUE
* CC.S.A0004.Scene | TRUE
* CC.S.A0004.Report.Tx  | TRUE
* CC.S.A0005 | FALSE
* CC.S.A0006 | FALSE
* CC.S.A0007 | TRUE
* CC.S.A0007.Scene | TRUE
* CC.S.A0007.Report.Tx | TRUE
* CC.S.A0008 | TRUE
* CC.S.A000f | TRUE
* CC.S.A0010 | TRUE
* CC.S.A0011 | FALSE
* CC.S.A0012 | FALSE
* CC.S.A0013 | FALSE
* CC.S.A0015 | FALSE
* CC.S.A0016 | FALSE
* CC.S.A0017 | FALSE
* CC.S.A0019 | FALSE
* CC.S.A001a | FALSE
* CC.S.A001b | FALSE
* CC.S.A0020 | FALSE
* CC.S.A0021 | FALSE
* CC.S.A0022 | FALSE
* CC.S.A0024 | FALSE
* CC.S.A0025 | FALSE
* CC.S.A0026 | FALSE
* CC.S.A0028  | FALSE
* CC.S.A0029 | FALSE
* CC.S.A002a | FALSE
* CC.S.A0030 | FALSE
* CC.S.A0031 | FALSE
* CC.S.A0032 | FALSE
* CC.S.A0033 | FALSE
* CC.S.A0034 | FALSE
* CC.S.A0036 | FALSE
* CC.S.A0037 | FALSE
* CC.S.A0038 | FALSE
* CC.S.A003a | FALSE
* CC.S.A003b | FALSE
* CC.S.A003c | FALSE
* CC.S.A4000 | TRUE
* CC.S.A4000.Scene | TRUE
* CC.S.A4001 | TRUE
* CC.S.A4002 | TRUE
* CC.S.A4002.Scene | TRUE
* CC.S.A4003 | TRUE
* CC.S.A4003.Scene | TRUE
* CC.S.A4004 | TRUE
* CC.S.A4004.Scene | TRUE
* CC.S.A4005 | TRUE
* CC.S.A4006 | TRUE
* CC.S.A400a | TRUE
* CC.S.A400b | TRUE
* CC.S.A400c | TRUE
* CC.S.A400d | TRUE
* CC.S.A4010 | TRUE
* CC.S.Afffd | TRUE
* CC.S.Afffe | NOT IN ZCL6
*
* Commands Received
* CC.S.C00.Rsp | TRUE
* CC.S.C01.Rsp | TRUE
* CC.S.C02.Rsp | TRUE
* CC.S.C03.Rsp | TRUE
* CC.S.C04.Rsp | TRUE
* CC.S.C05.Rsp | TRUE
* CC.S.C06.Rsp | TRUE
* CC.S.C07.Rsp | TRUE
* CC.S.C08.Rsp | TRUE
* CC.S.C09.Rsp | TRUE
* CC.S.C0a.Rsp | TRUE
* CC.S.C40.Rsp | TRUE
* CC.S.C41.Rsp | TRUE
* CC.S.C42.Rsp | TRUE
* CC.S.C43.Rsp | TRUE
* CC.S.C44.Rsp | TRUE
* CC.S.C47.Rsp | TRUE
* CC.S.C4b.Rsp | TRUE
* CC.S.C4c.Rsp | TRUE
*
*
* Client Attributes
* CC.C.A0000.Report.Rsp | FALSE
* CC.C.A0001.Report.Rsp | FALSE
* CC.C.A0003.Report.Rsp | FALSE
* CC.C.A0004.Report.Rsp | FALSE
* CC.C.A0007.Report.Rsp | FALSE
* CC.C.Afffd | TRUE
* CC.C.Afffe | NOT IN ZCL6
*
* Commands Generated
* CC.C.C00.Tx | TRUE
* CC.C.C01.Tx | TRUE
* CC.C.C02.Tx | TRUE
* CC.C.C03.Tx | TRUE
* CC.C.C04.Tx | TRUE
* CC.C.C05.Tx | TRUE
* CC.C.C06.Tx  | TRUE
* CC.C.C07.Tx | TRUE
* CC.C.C08.Tx | TRUE
* CC.C.C09.Tx | TRUE
* CC.C.C0a.Tx | TRUE
* CC.C.C40.Tx | TRUE
* CC.C.C41.Tx | TRUE
* CC.C.C42.Tx | TRUE
* CC.C.C43.Tx | TRUE
* CC.C.C44.Tx | TRUE
* CC.C.C47.Tx | TRUE
* CC.C.C4b.Tx | TRUE
* CC.C.C4c.Tx | TRUE
*/

/* Color Control Attributes */
enum ZbZclColorSvrAttrT {
    ZCL_COLOR_ATTR_CURRENT_HUE = 0x0000,
    ZCL_COLOR_ATTR_CURRENT_SAT = 0x0001,
    ZCL_COLOR_ATTR_REMAINING_TIME = 0x0002,
    ZCL_COLOR_ATTR_CURRENT_X = 0x0003,
    ZCL_COLOR_ATTR_CURRENT_Y = 0x0004,
    ZCL_COLOR_ATTR_DRIFT_COMP = 0x0005,
    ZCL_COLOR_ATTR_COMPENSATION_TEXT = 0x0006,
    ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS = 0x0007,
    ZCL_COLOR_ATTR_COLOR_MODE = 0x0008,
    ZCL_COLOR_ATTR_OPTIONS = 0x000f,
    ZCL_COLOR_ATTR_NUM_PRIMARIES = 0x0010,
    ZCL_COLOR_ATTR_PRIMARY_1X = 0x0011,
    ZCL_COLOR_ATTR_PRIMARY_1Y = 0x0012,
    ZCL_COLOR_ATTR_PRIMARY_1_INTENS = 0x0013,
    ZCL_COLOR_ATTR_PRIMARY_2X = 0x0015,
    ZCL_COLOR_ATTR_PRIMARY_2Y = 0x0016,
    ZCL_COLOR_ATTR_PRIMARY_2_INTENS = 0x0017,
    ZCL_COLOR_ATTR_PRIMARY_3X = 0x0019,
    ZCL_COLOR_ATTR_PRIMARY_3Y = 0x001a,
    ZCL_COLOR_ATTR_PRIMARY_3_INTENS = 0x001b,
    ZCL_COLOR_ATTR_PRIMARY_4X = 0x0020,
    ZCL_COLOR_ATTR_PRIMARY_4Y = 0x0021,
    ZCL_COLOR_ATTR_PRIMARY_4_INTENS = 0x0022,
    ZCL_COLOR_ATTR_PRIMARY_5X = 0x0024,
    ZCL_COLOR_ATTR_PRIMARY_5Y = 0x0025,
    ZCL_COLOR_ATTR_PRIMARY_5_INTENS = 0x0026,
    ZCL_COLOR_ATTR_PRIMARY_6X = 0x0028,
    ZCL_COLOR_ATTR_PRIMARY_6Y = 0x0029,
    ZCL_COLOR_ATTR_PRIMARY_6_INTENS = 0x002a,
    ZCL_COLOR_ATTR_WHITE_POINT_X = 0x0030,
    ZCL_COLOR_ATTR_WHITE_POINT_Y = 0x0031,
    ZCL_COLOR_ATTR_COLOR_POINT_RX = 0x0032,
    ZCL_COLOR_ATTR_COLOR_POINT_RY = 0x0033,
    ZCL_COLOR_ATTR_COLOR_POINT_R_INTENS = 0x0034,
    ZCL_COLOR_ATTR_COLOR_POINT_GX = 0x0036,
    ZCL_COLOR_ATTR_COLOR_POINT_GY = 0x0037,
    ZCL_COLOR_ATTR_COLOR_POINT_G_INTENS = 0x0038,
    ZCL_COLOR_ATTR_COLOR_POINT_BX = 0x003a,
    ZCL_COLOR_ATTR_COLOR_POINT_BY = 0x003b,
    ZCL_COLOR_ATTR_COLOR_POINT_B_INTENS = 0x003c,
    ZCL_COLOR_ATTR_ENH_CURR_HUE = 0x4000,
    ZCL_COLOR_ATTR_ENH_COLOR_MODE = 0x4001,
    ZCL_COLOR_ATTR_COLOR_LOOP_ACTIVE = 0x4002,
    ZCL_COLOR_ATTR_COLOR_LOOP_DIR = 0x4003,
    ZCL_COLOR_ATTR_COLOR_LOOP_TIME = 0x4004,
    ZCL_COLOR_ATTR_COLOR_LOOP_START_HUE = 0x4005,
    ZCL_COLOR_ATTR_COLOR_LOOP_STORE_HUE = 0x4006,
    ZCL_COLOR_ATTR_COLOR_CAPABILITIES = 0x400A,
    ZCL_COLOR_ATTR_COLOR_TEMP_MIN = 0x400B,
    ZCL_COLOR_ATTR_COLOR_TEMP_MAX = 0x400C,
    ZCL_COLOR_ATTR_COUPLE_COLOR_TL_MIN = 0x400d,
    ZCL_COLOR_ATTR_STARTUP_COLOR_TEMP = 0x4010
};

/* Color Control Command Identifiers  */
enum ZbZclColorCommandT {
    ZCL_COLOR_COMMAND_MOVE_TO_HUE = 0x00,
    ZCL_COLOR_COMMAND_MOVE_HUE = 0x01,
    ZCL_COLOR_COMMAND_STEP_HUE = 0x02,
    ZCL_COLOR_COMMAND_MOVE_TO_SAT = 0x03,
    ZCL_COLOR_COMMAND_MOVE_SAT = 0x04,
    ZCL_COLOR_COMMAND_STEP_SAT = 0x05,
    ZCL_COLOR_COMMAND_MOVE_TO_HS = 0x06,
    ZCL_COLOR_COMMAND_MOVE_TO_COLOR = 0x07,
    ZCL_COLOR_COMMAND_MOVE_COLOR = 0x08,
    ZCL_COLOR_COMMAND_STEP_COLOR = 0x09,
    ZCL_COLOR_COMMAND_MOVE_TO_COLOR_TEMP = 0x0a,
    ZCL_COLOR_COMMAND_ENH_MOVE_TO_HUE = 0x40,
    ZCL_COLOR_COMMAND_ENH_MOVE_HUE = 0x41,
    ZCL_COLOR_COMMAND_ENH_STEP_HUE = 0x42,
    ZCL_COLOR_COMMAND_ENH_MOVE_TO_HS = 0x43,
    ZCL_COLOR_COMMAND_COLOR_LOOP_SET = 0x44,
    ZCL_COLOR_COMMAND_STOP_MOVE_STEP = 0x47,
    ZCL_COLOR_COMMAND_MOVE_COLOR_TEMP = 0x4b,
    ZCL_COLOR_COMMAND_STEP_COLOR_TEMP = 0x4c,
};

/* Drift Compensation */
enum ZbZclColorDriftComp {
    ZCL_COLOR_DRIFT_NONE = 0x00,
    ZCL_COLOR_DRIFT_OTHER_UKN = 0x01,
    ZCL_COLOR_DRIFT_TEMP_MON = 0x02,
    ZCL_COLOR_DRIFT_OPTIC_LUM = 0x03,
    ZCL_COLOR_DRIFT_OPTIC_COLOR = 0x04,
};

/* Color Mode */
enum ZbZclColorMode {
    ZCL_COLOR_MODE_HS = 0x00,
    ZCL_COLOR_MODE_XY = 0x01,
    ZCL_COLOR_MODE_TEMP = 0x02,
};

/* Enhanced Color Mode */
enum ZbZclColorModeEnh {
    ZCL_COLOR_ENH_MODE_CURR_HS = 0x00,
    ZCL_COLOR_ENH_MODE_CURR_XY = 0x01,
    ZCL_COLOR_ENH_MODE_COLOR_TEMP = 0x02,
    ZCL_COLOR_ENH_MODE_ENH_HUE_SAT = 0x03,
};

/* Color Capabilities - bit field */
#define ZCL_COLOR_CAP_HS                    0x01U /* Mandatory if ZCL_COLOR_CAP_ENH_HUE */
#define ZCL_COLOR_CAP_ENH_HUE               0x02U
#define ZCL_COLOR_CAP_COLOR_LOOP            0x04U
#define ZCL_COLOR_CAP_XY                    0x08U /* Mandatory */
#define ZCL_COLOR_CAP_COLOR_TEMP            0x10U

/* Move direction field */
enum ZbZclColorMoveDir {
    ZCL_COLOR_MOVE_TO_DIR_SHORTEST = 0x00,
    ZCL_COLOR_MOVE_TO_DIR_LONGEST = 0x01,
    ZCL_COLOR_MOVE_TO_DIR_UP = 0x02,
    ZCL_COLOR_MOVE_TO_DIR_DOWN = 0x03,
};

/* Move mode */
enum ZbZclColorMoveMode {
    ZCL_COLOR_MOVE_MODE_STOP = 0x00,
    ZCL_COLOR_MOVE_MODE_UP = 0x01,
    ZCL_COLOR_MOVE_MODE_DOWN = 0x03,
};

/* Step mode */
enum ZbZclColorStepMode {
    ZCL_COLOR_STEP_MODE_UP = 0x01,
    ZCL_COLOR_STEP_MODE_DOWN = 0X03,
};

/* Color Loop Set action */
enum ZbZclColorLoopAction {
    ZCL_COLOR_LOOP_ACTION_DEACTIVATE = 0x00,
    ZCL_COLOR_LOOP_ACTION_ACTIVATE_FIELD = 0x01,
    ZCL_COLOR_LOOP_ACTION_ACTIVATE_ATTR = 0x02,
};

/* Color Loop Set direction */
enum ZbZclColorLoopDir {
    ZCL_COLOR_LOOP_DIR_DECREMENT_HUE = 0x00,
    ZCL_COLOR_LOOP_DIR_INCREMENT_HUE = 0x01,
};

/* Color Loop Set update flags */
enum ZbZclColorLoopFlags {
    ZCL_COLOR_LOOP_FLAG_UPDATE_ACTION = 0x01,
    ZCL_COLOR_LOOP_FLAG_UPDATE_DIRECTION = 0x02,
    ZCL_COLOR_LOOP_FLAG_UPDATE_TIME = 0x04,
    ZCL_COLOR_LOOP_FLAG_UPDATE_START_HUE = 0x08,
};

/* Options Attribute - bit field */
#define ZCL_COLOR_OPTIONS_EXECUTE_IF_OFF            0x01

#define ZCL_COLOR_MODE_DEFAULT                      0x01
#define ZCL_COLOR_OPTIONS_DEFAULT                   0x00
#define ZCL_COLOR_ILLEGAL_HUE                       0xFFFF
#define ZCL_COLOR_MAX_HUE_SAT                       0xFE
#define ZCL_COLOR_X_DEFAULT                         0x616B
#define ZCL_COLOR_X_MAX                             0xFEFF
#define ZCL_COLOR_Y_DEFAULT                         0x607D
#define ZCL_COLOR_Y_MAX                             0xFEFF
#define ZCL_COLOR_TEMP_DEFAULT                      0x00FA
#define ZCL_COLOR_TEMP_MAX                          0xFEFF
#define ZCL_COLOR_LOOP_TIME_DEFAULT                 0x0019
#define ZCL_COLOR_LOOP_START_HUE_DEFAULT            0x2300
#define ZCL_COLOR_STARTUP_COLOR_TEMP_MAX            0xFFFF

/*-----------------------------------------------------------------------------
 * Structures
 *-----------------------------------------------------------------------------
 */
struct ZbZclColorClientMoveToHueReqT {
    uint8_t hue;
    uint8_t direction;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveHueReqT {
    uint8_t move_mode;
    uint8_t rate;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientStepHueReqT {
    uint8_t step_mode;
    uint8_t step_size;
    uint8_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveToSatReqT {
    uint8_t sat;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveSatReqT {
    uint8_t move_mode;
    uint8_t rate;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientStepSatReqT {
    uint8_t step_mode;
    uint8_t step_size;
    uint8_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveToHueSatReqT {
    uint8_t hue;
    uint8_t sat;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveToColorXYReqT {
    uint16_t color_x;
    uint16_t color_y;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveColorXYReqT {
    uint16_t rate_x;
    uint16_t rate_y;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientStepColorXYReqT {
    uint16_t step_x;
    uint16_t step_y;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveToColorTempReqT {
    uint16_t color_temp;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveToHueEnhReqT {
    uint16_t enh_hue;
    uint8_t direction;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveHueEnhReqT {
    uint8_t move_mode;
    uint16_t rate;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientStepHueEnhReqT {
    uint8_t step_mode;
    uint16_t step_size;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveToHueSatEnhReqT {
    uint16_t enh_hue;
    uint8_t sat;
    uint16_t transition_time;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientColorLoopSetReqT {
    uint8_t update_flags;
    uint8_t action;
    uint8_t direction;
    uint16_t transition_time;
    uint16_t start_hue; /* Enhanced Hue */
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientStopMoveStepReqT {
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

struct ZbZclColorClientMoveColorTempReqT {
    uint8_t move_mode;
    uint16_t rate;
    uint16_t color_temp_min;
    uint16_t color_temp_max;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

/* The color_temp_min and color_temp_max fields need to be checked against in the application layer.
 * The current value (ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS) needs to be known and the step may need
 * to be clipped so that the final value of ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS satisfies:
 * ZCL_COLOR_ATTR_COLOR_TEMP_MIN <= Color Temperature Minimum Mireds field <= ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS
 * ZCL_COLOR_ATTR_COLOR_TEMP_MIREDS  <= Color Temperature Maximum Mireds field <= ZCL_COLOR_ATTR_COLOR_TEMP_MAX */
struct ZbZclColorClientStepColorTempReqT {
    uint8_t step_mode;
    uint16_t step_size;
    uint16_t transition_time;
    uint16_t color_temp_min;
    uint16_t color_temp_max;
    uint8_t mask; /* OptionsMask */
    uint8_t override; /* OptionsOverride */
};

/*-----------------------------------------------------------------------------
 * Server
 *-----------------------------------------------------------------------------
 */
struct ZbZclColorServerCallbacksT {
    /* Returns a ZCL Status code to send in the Default Response, or ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE
     * if no Default Response to be generated. */

    enum ZclStatusCodeT (*move_to_hue)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*move_hue)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveHueReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*step_hue)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepHueReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*move_to_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*move_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*step_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*move_to_hue_sat)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueSatReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*move_to_color_xy)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToColorXYReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*move_color_xy)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveColorXYReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*step_color_xy)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepColorXYReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*move_to_color_temp)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToColorTempReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*move_to_hue_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*move_hue_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveHueEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*step_hue_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepHueEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*move_to_hue_sat_enh)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveToHueSatEnhReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*color_loop_set)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientColorLoopSetReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*stop_move_step)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStopMoveStepReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);

    enum ZclStatusCodeT (*move_color_temp)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientMoveColorTempReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
    enum ZclStatusCodeT (*step_color_temp)(struct ZbZclClusterT *cluster,
        struct ZbZclColorClientStepColorTempReqT *req,
        struct ZbZclAddrInfoT *srcInfo, void *arg);
};

struct ZbColorClusterConfig {
    struct ZbZclColorServerCallbacksT callbacks;
    uint8_t capabilities; /* e.g. ZCL_COLOR_CAP_HS */
};

struct ZbZclClusterT * ZbZclColorServerAlloc(struct ZigBeeT *zb, uint8_t endpoint, struct ZbZclClusterT *onoff_server,
    const struct ZbZclAttrT *attribute_list, unsigned int num_attrs, struct ZbColorClusterConfig *config, void *arg);

/*-----------------------------------------------------------------------------
 * Client
 *-----------------------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclColorClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

enum ZclStatusCodeT ZbZclColorClientMoveToHueReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveHueReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveHueReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientStepHueReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepHueReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveToSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientStepSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveToHueSatReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueSatReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveToColorXYReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToColorXYReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveColorXYReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveColorXYReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientStepColorXYReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepColorXYReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveToColorTempReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToColorTempReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveToHueEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveHueEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveHueEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientStepHueEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepHueEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveToHueSatEnhReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveToHueSatEnhReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientColorLoopSetReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientColorLoopSetReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientStopMoveStepReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStopMoveStepReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientMoveColorTempReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientMoveColorTempReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

enum ZclStatusCodeT ZbZclColorClientStepColorTempReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclColorClientStepColorTempReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

#endif /* ZCL_COLOR_H */
