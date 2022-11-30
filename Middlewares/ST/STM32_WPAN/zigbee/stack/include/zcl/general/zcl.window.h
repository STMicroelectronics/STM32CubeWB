/**
 * @file zcl.window.h
 * @heading Window Covering
 * @brief ZCL Window Covering cluster header
 * ZCL 7 section 7.4
 * ZCL 8 section 7.4
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_WINDOW_H
# define ZCL_WINDOW_H

/* IMPORTANT INFORMATION:
 *
 * The Window Covering Cluster uses scene data to save and restore configurations
 * and therefore needs 2 additional callback functions.
 * Instead of writing attributes directly from the callback functions:
 * windowServerSetSceneData, this function serves a as helper function instead to
 * check if the tilt and lift percentage attributes exist and unpack the scene data.
 *
 * The additional callback functions must be implemented and used in the application layer
 * to read and write the attributes.
 */

/* @PICS.ZCL.Window
 * WNCV.S | Server | True
 * WNCV.C | Client | True
 * WNCV.S.Lift | Lift Action Support | True
 * WNCV.S.Tilt | Tilt Action Support | True
 * WNCV.S.ClosedLoop | Closed Loop Control |False
 *
 *
 * Server Attributes
 * WNCV.S.A0000 | WindowCoveringType | True
 * WNCV.S.A0001 | PhysicalClosedLimitLift | False | Optional
 * WNCV.S.A0002 | PhysicalClosedLimitTilt | False | Optional
 * WNCV.S.A0003 | CurrentPositionLift | False | Optional
 * WNCV.S.A0004 | CurrentPositionTilt | False | Optional
 * WNCV.S.A0005 | NumberOfActuationsLift | False | Optional
 * WNCV.S.A0006 | NumberOfActuationsTilt | False | Optional
 * WNCV.S.A0007 | ConfigStatus | True
 * WNCV.S.A0008 | CurrentPositionLiftPercentage | True | Mandatory only if Closed Loop control is enabled
 * WNCV.S.A0008.Scene | CurrentPositionLiftPercentage | False
 * WNCV.S.A0008.Report.DefaultConfig | CurrentPositionLiftPercentage | False
 * WNCV.S.A0008.Report.Tx | CurrentPositionLiftPercentage | False
 * WNCV.S.A0009 | CurrentPositionTiltPercentage | True | Mandatory only if Closed Loop control is enabled
 * WNCV.S.A0009.Scene | CurrentPositionTiltPercentage | False
 * WNCV.S.A0009.Report.DefaultConfig | CurrentPositionTiltPercentage | False
 * WNCV.S.A0009.Report.Tx | CurrentPositionTiltPercentage | False
 * WNCV.S.A0010 | InstalledOpenLimitLift | False | Mandatory only if Closed Loop control is enabled
 * WNCV.S.A0011 | InstalledClosedLimitLift | False | Mandatory only if Closed Loop control is enabled
 * WNCV.S.A0012 | InstalledOpenLimitTilt | False | Mandatory only if Closed Loop control is enabled
 * WNCV.S.A0013 | InstalledClosedLimitTilt | False | Mandatory only if Closed Loop control is enabled
 * WNCV.S.A0014 | VelocityLift | False | Optional
 * WNCV.S.A0015 | AccelerationTimeLift | False | Optional
 * WNCV.S.A0016 | DecelerationTimeLift | False | Optional
 * WNCV.S.A0017 | Mode | True
 * WNCV.S.A0018 | Intermediate Setpoints - Lift | False | Optional
 * WNCV.S.A0019 | Intermediate Setpoints - Tilt | False | Optional
 * WNCV.S.Afffd | ClusterRevision | True
 * WNCV.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * WNCV.S.C00.Rsp | Up / Open | True
 * WNCV.S.C01.Rsp | Down / Close | True
 * WNCV.S.C02.Rsp | Stop | True
 * WNCV.S.C04.Rsp | Go To Lift Value | False
 * WNCV.S.C05.Rsp | Go to Lift Percentage | False
 * WNCV.S.C07.Rsp | Go to Tilt Value | False
 * WNCV.S.C08.Rsp | Go to Tilt Percentage | False
 *
 * Client Attributes
 * WNCV.C.A0008.Report.Rsp | CurrentPositionLiftPercentage | False
 * WNCV.C.A0009.Report.Rsp | CurrentPositionTiltPercentage | False
 * WNCV.C.Afffd | ClusterRevision | True
 * WNCV.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Generated
 * WNCV.C.C00.Tx | Up / Open | True
 * WNCV.C.C01.Tx | Down / Close | True
 * WNCV.C.C02.Tx | Stop | True
 * WNCV.C.C04.Tx | Go To Lift Value | False
 * WNCV.C.C05.Tx | Go to Lift Percentage | False
 * WNCV.C.C07.Tx | Go to Tilt Value | False
 * WNCV.C.C08.Tx | Go to Tilt Percentage | False
 */

#include "zcl/zcl.h"

/** Window Covering Server Attribute Ids */
enum ZbZclWncvServerAttrT {
    /* Information Attribute Set */
    ZCL_WNCV_SVR_ATTR_COVERING_TYPE = 0x0000, /**< WindowCoveringType */
    ZCL_WNCV_SVR_ATTR_PHY_CLOSE_LIMIT_LIFT = 0x0001, /**< PhysicalClosedLimitLift (Optional) */
    ZCL_WNCV_SVR_ATTR_PHY_CLOSE_LIMIT_TILT = 0x0002, /**<  PhysicalClosedLimitTilt (Optional) */
    ZCL_WNCV_SVR_ATTR_CURR_POSITION_LIFT = 0x0003, /**< CurrentPositionLift (Optional) */
    ZCL_WNCV_SVR_ATTR_CURR_POSITION_TILT = 0x0004, /**< CurrentPositionTilt (Optional) */
    ZCL_WNCV_SVR_ATTR_ACTUATION_NUMBER_LIFT = 0x0005, /**< NumberOfActuationsLift (Optional) */
    ZCL_WNCV_SVR_ATTR_ACCUATION_NUMBER_TILT = 0x0006, /**< NumberOfActuationsTilt (Optional) */
    ZCL_WNCV_SVR_ATTR_CONFIG_STATUS = 0x0007, /**< ConfigStatus */
    ZCL_WNCV_SVR_ATTR_CURR_POS_LIFT_PERCENT = 0x0008, /**< CurrentPositionLiftPercentage */
    ZCL_WNCV_SVR_ATTR_CURR_POS_TILT_PERCENT = 0x0009, /**< CurrentPositionTiltPercentage */
    /* Settings Attribute Set */
    ZCL_WNCV_SVR_ATTR_INSTALLED_OPENED_LIMIT_LIFT = 0x0010, /**< InstalledOpenLimitLift */
    ZCL_WNCV_SVR_ATTR_INSTALLED_CLOSED_LIMIT_LIFT = 0x0011, /**< InstalledClosedLimitLift */
    ZCL_WNCV_SVR_ATTR_INSTALLED_OPENED_LIMIT_TILT = 0x0012, /**< InstalledOpenLimitTilt */
    ZCL_WNCV_SVR_ATTR_INSTALLED_CLOSED_LIMIT_TILT = 0x0013, /**< InstalledClosedLimitTilt */
    ZCL_WNCV_SVR_ATTR_VELOCITY_LIFT = 0x0014, /**< VelocityLift (Optional) */
    ZCL_WNCV_SVR_ATTR_ACCELERATION_TIME_LIFT = 0x0015, /**< AccelerationTimeLift (Optional) */
    ZCL_WNCV_SVR_ATTR_DECELERATION_TIME_LIFT = 0x0016, /**< DecelerationTimeLift (Optional) */
    ZCL_WNCV_SVR_ATTR_MODE = 0x0017, /**< Mode */
    ZCL_WNCV_SVR_ATTR_INTERMEDIATE_SETPOINTS_LIFT = 0x0018, /**< Intermediate Setpoints - Lift (Optional) */
    ZCL_WNCV_SVR_ATTR_INTERMEDIATE_SETPOINTS_TILT = 0x0019 /**< Intermediate Setpoints - Tilt (Optional) */
};

/** Window Covering Type enumerations */
enum ZbZclWncvTypes {
    ZCL_WNCV_TYPE_ROLLERSHADE = 0x00, /**< Rollershade */
    ZCL_WNCV_TYPE_ROLLERSHADE_2_MOTOR, /**< Rollershade - 2 Motor */
    ZCL_WNCV_TYPE_ROLLERSHADE_EXTERIOR, /**< Rollershade – Exterior */
    ZCL_WNCV_TYPE_ROLLERSHADE_EXTERIOR_2_MOTOR, /**< Rollershade - Exterior - 2 Motor */
    ZCL_WNCV_TYPE_DRAPERY, /**< Drapery */
    ZCL_WNCV_TYPE_AWNING, /**< Awning */
    ZCL_WNCV_TYPE_SHUTTER, /**< Shutter */
    ZCL_WNCV_TYPE_TILT_BLIND_TILT_ONLY, /**< Tilt Blind - Tilt Only */
    ZCL_WNCV_TYPE_TILT_BLIND_LIFT_TILT, /**< Tilt Blind - Lift and Tilt */
    ZCL_WNCV_TYPE_PROJECTOR_SCREEN /**< Projector Screen */
};

/* Window Covering Commands */
#define ZCL_WNCV_COMMAND_UP                         0x00
#define ZCL_WNCV_COMMAND_DOWN                       0x01
#define ZCL_WNCV_COMMAND_STOP                       0x02
#define ZCL_WNCV_COMMAND_GOTO_LIFT_VALUE            0x04
#define ZCL_WNCV_COMMAND_GOTO_LIFT_PERCENTAGE       0x05
#define ZCL_WNCV_COMMAND_GOTO_TILT_VALUE            0x07
#define ZCL_WNCV_COMMAND_GOTO_TILT_PERCENTAGE       0x08

/* Window Covering Config/Status Bit Mask */
#define ZCL_WNCV_STATUS_OPERATIONAL                 0x01U
#define ZCL_WNCV_STATUS_ONLINE                      0x02U
#define ZCL_WNCV_STATUS_UP_REVERSED                 0x04U
#define ZCL_WNCV_STATUS_LIFT_CLOSED_LOOP            0x08U
#define ZCL_WNCV_STATUS_TILT_CLOSED_LOOP            0x10U
#define ZCL_WNCV_STATUS_LIFT_ENCODER                0x20U
#define ZCL_WNCV_STATUS_TILT_ENCODER                0x40U
#define ZCL_WNCV_STATUS_MASK                        0x7fU

/* Window Covering Mode Bit Mask */
#define ZCL_WNCV_MODE_REVERSED                      0x01U
#define ZCL_WNCV_MODE_CALIBRATION                   0x02U
#define ZCL_WNCV_MODE_MAINTENANCE                   0x04U
#define ZCL_WNCV_MODE_LED_FEEDBACK                  0x08U
#define ZCL_WNCV_MODE_MASK                          0x0fU

/* Window Covering Server */
/** Window Covering Server callbacks configuration */
struct ZbZclWindowServerCallbacksT {
    enum ZclStatusCodeT (*up_command)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr, void *arg);
    /**< Callback to application, invoked on receipt of Up/Open command.
     * Application should update ZCL_WNCV_SVR_ATTR_CURR_POS_LIFT_PERCENT and ZCL_WNCV_SVR_ATTR_CURR_POS_TILT_PERCENT.
     */

    enum ZclStatusCodeT (*down_command)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr, void *arg);
    /**< Callback to application, invoked on receipt of Down/Close command.
     * Application should update ZCL_WNCV_SVR_ATTR_CURR_POS_LIFT_PERCENT and ZCL_WNCV_SVR_ATTR_CURR_POS_TILT_PERCENT.
     */

    enum ZclStatusCodeT (*stop_command)(struct ZbZclClusterT *cluster,
        struct ZbZclHeaderT *zclHdrPtr, struct ZbApsdeDataIndT *dataIndPtr, void *arg);
    /**< Callback to application, invoked on receipt of Stop command.
     * Application should update ZCL_WNCV_SVR_ATTR_CURR_POS_LIFT_PERCENT and ZCL_WNCV_SVR_ATTR_CURR_POS_TILT_PERCENT.
     */

    enum ZclStatusCodeT (*set_lift_and_tilt_command)(struct ZbZclClusterT *cluster,
        void *arg, uint8_t liftPercentage, uint8_t tiltPercentage);
    /**< Callback to application, invoked to handle setting a Scene, which includes Lift and Tilt values.
     * Application should update ZCL_WNCV_SVR_ATTR_CURR_POS_LIFT_PERCENT and ZCL_WNCV_SVR_ATTR_CURR_POS_TILT_PERCENT.
     */
};

/**
 * Create a new instance of the Window Covering Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclWindowServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclWindowServerCallbacksT *callbacks, void *arg);

/**
 * Configure the Window Covering mode
 * @param cluster Cluster instance from which to send this command
 * @param mode Window Covering mode bit mask
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclWindowClosureServerMode(struct ZbZclClusterT *cluster, uint8_t mode);

/* Window Covering Client */
/**
 * Create a new instance of the Window Covering Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclWindowClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send an Up/Open command
 * @param cluster Cluster instance from which to send this command
 * @param dst The destination address information
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclWindowClientCommandUp(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Down/Close command
 * @param cluster Cluster instance from which to send this command
 * @param dst The destination address information
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclWindowClientCommandDown(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Stop command
 * @param cluster Cluster instance from which to send this command
 * @param dst The destination address information
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclWindowClientCommandStop(struct ZbZclClusterT *cluster, const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif
