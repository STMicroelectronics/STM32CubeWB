/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

#ifndef ZCL_WINDOW_H
# define ZCL_WINDOW_H

/* IMPORTANT INFORMATION:
 *
 * The Window Covering Cluster uses scene data to save and restore configurations
 * and therefore needs 2 additional callback functions.
 * Instead of writing attributes directly from the callback functions:
 * windowServerSetSceneData, this function serves a as helper function instead to
 * check if the tilt and lift pecentage attributes exist and unpack the scene data.
 *
 * The additional callback functions must be implemented and used in the application layer
 * to read and write the attributes.
 */

/* PICS.ZCL.Window
 * WNCV.S | True
 * WNCV.S.Lift | True
 * WNCV.S.Tilt | True
 * WNCV.S.ClosedLoop | False
 * WNCV.C | True
 *
 * Server Attributes
 * WNCV.S.Afffd | True
 * WNCV.S.A0000 | True
 * WNCV.S.A0001 | False
 * WNCV.S.A0002 | False
 * WNCV.S.A0003 | False
 * WNCV.S.A0004 | False
 * WNCV.S.A0005 | False
 * WNCV.S.A0006 | False
 * WNCV.S.A0007 | True
 * WNCV.S.A0008 | False
 * WNCV.S.A0008.Scene | False
 * WNCV.S.A0008.Report.DefaultConfig | False
 * WNCV.S.A0008.Report.Tx | False
 * WNCV.S.A0009 | False
 * WNCV.S.A0009.Scene | False
 * WNCV.S.A0009.Report.DefaultConfig | False
 * WNCV.S.A0009.Report.Tx | False
 * WNCV.S.A0010 | False
 * WNCV.S.A0012 | False
 * WNCV.S.A0013 | False
 * WNCV.S.A0014 | False
 * WNCV.S.A0015 | False
 * WNCV.S.A0016 | False
 * WNCV.S.A0017 | True
 * WNCV.S.A0018 | False
 * WNCV.S.A0019 | False
 *
 * Commands Received
 * WNCV.S.C00.Rsp | True
 * WNCV.S.C01.Rsp | True
 * WNCV.S.C02.Rsp | True
 * WNCV.S.C04.Rsp | False
 * WNCV.S.C05.Rsp | False
 * WNCV.S.C07.Rsp | False
 * WNCV.S.C08.Rsp | False
 *
 *
 * Client Attributes
 * WNCV.C.Afffd | True
 * WNCV.C.A0008.Report.Rsp | False
 * WNCV.C.A0009.Report.Rsp | False
 *
 * Commands Generated
 * WNCV.C.C00.Tx | True
 * WNCV.C.C01.Tx | True
 * WNCV.C.C02.Tx | True
 * WNCV.C.C04.Tx | False
 * WNCV.C.C05.Tx | False
 * WNCV.C.C07.Tx | False
 * WNCV.C.C08.Tx | False
 */

#include "zcl/zcl.h"

/* Window Covering Cluster Attribute Set */
enum {
    /* M* - mandatory if closed loop and lift/tile action is supported */
    /* Information Attribute Set */
    ZCL_WINDOW_ATTR_COVERING_TYPE = 0x0000, /* R- mandatory */
    ZCL_WINDOW_PHY_CLOSE_LIMIT_LIFT = 0x0001, /* R */
    ZCL_WINDOW_PHY_CLOSE_LIMIT_TILT = 0x0002, /* R */
    ZCL_WINDOW_CURR_POSITION_LIFT = 0x0003, /* R */
    ZCL_WINDOW_CURR_POSITION_TILT = 0x0004, /* R */
    ZCL_WINDOW_ACTUATION_NUMBER_LIFT = 0x0005, /* R */
    ZCL_WINDOW_ACCUATION_NUMBER_TILT = 0x0006, /* R */
    ZCL_WINDOW_CONFIG_STATUS = 0x0007, /* R - mandatory */
    ZCL_WINDOW_CURR_POS_LIFT_PERCENT = 0x0008, /* RSP - M* */
    ZCL_WINDOW_CURR_POS_TILT_PERCENT = 0x0009, /* RSP - M* */
    /* Settings Attribute Set */
    ZCL_WINDOW_INSTALLED_OPENED_LIMIT_LIFT = 0x0010, /* R - M* */
    ZCL_WINDOW_INSTALLED_CLOSED_LIMIT_LIFT = 0x0011, /* R - M* */
    ZCL_WINDOW_INSTALLED_OPENED_LIMIT_TILT = 0x0012, /* R - M* */
    ZCL_WINDOW_INSTALLED_CLOSED_LIMIT_TILT = 0x0013, /* R - M* */
    ZCL_WINDOW_VELOCITY_LIFT = 0x0014, /* RW */
    ZCL_WINDOW_ACCELERATION_TIME_LIFT = 0x0015, /* RW */
    ZCL_WINDOW_DECELERATION_TIME_LIFT = 0x0016, /* RW */
    ZCL_WINDOW_MODE = 0x0017, /* RW - mandatory */
    ZCL_WINDOW_INTERMEDIATE_SETPOINTS_LIFT = 0x0018, /* RW */
    ZCL_WINDOW_INTERMEDIATE_SETPOINTS_TILT = 0x0019 /* RW */

};

/* Window Covering Type Attributes */
enum {
    WINDOW_ROLLERSHADE = 0x00,
    WINDOW_ROLLERSHADE_2_MOTOR,
    WINDOW_ROLLERSHADE_EXTERIOR,
    WINDOW_ROLLERSHADE_EXTERIOR_2_MOTOR,
    WINDOW_DRAPERY,
    WINDOW_AWNING,
    WINDOW_SHUTTER,
    WINDOW_TILT_BLIND_TILT,
    WINDOW_TILT_BLIND_LIFT_TILT,
    WINDOW_PROJECTOR_SCREEN
};

/* Commands */
#define ZCL_WINDOW_COMMAND_UP                    0x00 /* mandatory */
#define ZCL_WINDOW_COMMAND_DOWN                  0x01 /* mandatory */
#define ZCL_WINDOW_COMMAND_STOP                  0x02 /* mandatory */
#define ZCL_WINDOW_COMMAND_GOTO_LIFT_VALUE       0x04
#define ZCL_WINDOW_COMMAND_GOTO_LIFT_PERCENTAGE  0x05
#define ZCL_WINDOW_COMMAND_GOTO_TILT_VALUE       0x07
#define ZCL_WINDOW_COMMAND_GOTO_TILT_PERCENTAGE  0x08

/* Config/Status Bits */
#define ZCL_WINDOW_OPERATIONAL                   0x01
#define ZCL_WINDOW_ONLINE                        0x02
#define ZCL_WINDOW_UP_REVERSED                   0x04
#define ZCL_WINDOW_LIFT_CLOSED_LOOP              0x08
#define ZCL_WINDOW_TILT_CLOSED_LOOP              0x16
#define ZCL_WINDOW_LIFT_ENCODER                  0x32
#define ZCL_WINDOW_TILT_ENCODER                  0x64

/* Mode Bits */
#define ZCL_WINDOW_REVERSED                      0x01
#define ZCL_WINDOW_CALIBRATION                   0x02
#define ZCL_WINDOW_MAINTENANCE                   0x04
#define ZCL_WINDOW_LED_FEEDBACK                  0x08

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */
struct ZbZclWindowServerCallbacksT {
    /* Returns a ZCL Status code to send in the Default Response, or
     * ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE if no Default Response
     * to be generated.
     * All 3 commnads should update ZCL_WINDOW_CURR_POS_LIFT_PERCENT&
     * ZCL_WINDOW_CURR_POS_TILT_PERCENT accordingly*/
    uint8_t (*up_command)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr, void *arg);

    uint8_t (*down_command)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr, void *arg);

    uint8_t (*set_lift_and_tilt_command)(struct ZbZclClusterT *clusterPtr, void *arg, uint8_t liftPercentage, uint8_t tiltPercentage);

    uint8_t (*stop_command)(struct ZbZclClusterT *clusterPtr,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr, void *arg);
};

struct ZbZclClusterT * ZbZclWindowServerAlloc(struct ZigBeeT *zb, uint8_t ept,
    struct ZbZclWindowServerCallbacksT *callbacks, void *arg);

uint8_t ZbZclWindowClosureServerMode(struct ZbZclClusterT *clusterPtr, uint8_t mode);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclWindowClientAlloc(struct ZigBeeT *zb, uint8_t ept);
uint8_t ZbZclWindowClientCommandUp(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

uint8_t ZbZclWindowClientCommandDown(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

uint8_t ZbZclWindowClientCommandStop(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif /* __ZCL_WINDOW_H */
