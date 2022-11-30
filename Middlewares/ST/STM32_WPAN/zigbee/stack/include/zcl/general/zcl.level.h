/**
 * For many applications, this cluster has a close relationship with the OnOff cluster.
 * See ZCL specification
 *
 * This cluster supports scenes functionality. When a scene is recalled that includes
 * this cluster, the attributes listed in the Scenes Table Extensions for this cluster
 * will be set to the value in the scene
 * @file zcl.level.h
 * @heading Level
 * @brief ZCL Level cluster header
 * ZCL 7 section 3.10
 * ZCL 8 section 3.10
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_LEVEL_H
#define ZCL_LEVEL_H

/* @PICS.ZCL.Level
 *
 * LC.S | Server | True
 * LC.C | Client | True
 *
 * Server Attributes
 * LC.S.A0000 | CurrentLevel | True
 * LC.S.A0000.Scene | CurrentLevel | True
 * LC.S.A0000.Report.Tx | CurrentLevel | True
 * LC.S.A0001 | RemainingTime | False | Optional
 * LC.S.A000f | OnOffTransitionTime | True | Optional
 * LC.S.A0010 | OnLevel | False | Optional
 * LC.S.A0011 | OnTransitionTime | False | Optional
 * LC.S.A0012 | OffTransitionTime | False | Optional
 * LC.S.A0013 | DefaultMoveRate | False | Optional
 * LC.S.A0014 | Options | False | Optional
 * LC.S.A4000 | StartUpCurrentLevel | False | Optional
 * LC.S.Afffd | ClusterRevision | True
 *
 * Commands Received
 * LC.S.C00.Rsp | Move to Level | True
 * LC.S.C01.Rsp | Move | True
 * LC.S.C02.Rsp | Step | True
 * LC.S.C03.Rsp | Stop | True
 * LC.S.C04.Rsp | Move to Level (with On/Off) | True
 * LC.S.C05.Rsp | Move (with On/Off) | True
 * LC.S.C06.Rsp | Step (with On/Off) | True
 * LC.S.C07.Rsp | Stop | True
 *
 * Client Attributes
 * LC.C.A0000.Report.Rsp | CurrentLevel | False
 * LC.C.Afffd | ClusterRevision | True
 *
 * Commands Generated
 * LC.S.C00.Tx | Move to Level | True
 * LC.S.C01.Tx | Move | True
 * LC.S.C02.Tx | Step | True
 * LC.S.C03.Tx | Stop | True
 * LC.S.C04.Tx | Move to Level (with On/Off) | True
 * LC.S.C05.Tx | Move (with On/Off) | True
 * LC.S.C06.Tx | Step (with On/Off)  | True
 * LC.S.C07.Tx | Stop | True
 */

#include "zcl/zcl.h"

/*---------------------------------------------------------------
 * Level Control Cluster
 *---------------------------------------------------------------
 */

/** Level Server Attribute IDs */
enum ZbZclLevelSvrAttrT {
    ZCL_LEVEL_ATTR_CURRLEVEL = 0x0000, /**< CurrentLevel */
    ZCL_LEVEL_ATTR_REMAINTIME = 0x0001, /**< RemainingTime (Optional) */
    ZCL_LEVEL_ATTR_MINLEVEL = 0x0002, /**< MinLevel (Optional) */
    ZCL_LEVEL_ATTR_MAXLEVEL = 0x0003, /**< MaxLevel (Optional) */
    ZCL_LEVEL_ATTR_CURRFREQ = 0x0004, /**< CurrentFrequency (Optional) */
    ZCL_LEVEL_ATTR_MINFREQ = 0x0005, /**< MinFrequency (Optional) */
    ZCL_LEVEL_ATTR_MAXFREQ = 0x0006, /**< MaxFrequency (Optional) */
    ZCL_LEVEL_ATTR_OPTIONS = 0x000f, /**< OnOffTransitionTime (Optional) */
    ZCL_LEVEL_ATTR_ONOFF_TRANS_TIME = 0x0010, /**< OnLevel (Optional) */
    ZCL_LEVEL_ATTR_ONLEVEL = 0x0011, /**< OnTransitionTime (Optional) */
    ZCL_LEVEL_ATTR_ON_TRANS_TIME = 0x0012, /**< OffTransitionTime (Optional) */
    ZCL_LEVEL_ATTR_OFF_TRANS_TIME = 0x0013, /**< DefaultMoveRate (Optional) */
    ZCL_LEVEL_ATTR_DEFAULT_MOVE_RATE = 0x0014, /**< Options (Optional) */
    ZCL_LEVEL_ATTR_STARTUP_CURRLEVEL = 0x4000 /**< StartUpCurrentLevel (Optional) */
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

/** Move To Level command structure */
struct ZbZclLevelClientMoveToLevelReqT {
    bool with_onoff; /**< With Onoff - If true then cmd is ZCL_LEVEL_COMMAND_MOVELEVEL_ONOFF, else ZCL_LEVEL_COMMAND_MOVELEVEL*/
    uint8_t level; /**< Level */
    uint16_t transition_time; /**< Transition time */
    uint8_t mask; /**< OptionsMask - Not included if with_onoff is true */
    uint8_t override; /**< OptionsOverride - Not included if with_onoff is true */
};

/** Move command structure */
struct ZbZclLevelClientMoveReqT {
    bool with_onoff; /**< With Onoff - If true then cmd is ZCL_LEVEL_COMMAND_MOVE_ONOFF, else ZCL_LEVEL_COMMAND_MOVE */
    uint8_t mode; /**< Mode */
    uint8_t rate; /**< Rate */
    uint8_t mask; /**< OptionsMask - Not included if with_onoff is true */
    uint8_t override; /**< OptionsOverride - Not included if with_onoff is true */
};

/** Step command structure */
struct ZbZclLevelClientStepReqT {
    bool with_onoff; /**< With Onoff - If true then cmd is ZCL_LEVEL_COMMAND_STEP_ONOFF, else ZCL_LEVEL_COMMAND_STEP */
    uint8_t mode; /**< Mode */
    uint8_t size; /**< Size */
    uint16_t transition_time; /**< Transition Time */
    uint8_t mask; /**< OptionsMask - Not included if with_onoff is true */
    uint8_t override; /**< OptionsOverride - Not included if with_onoff is true */
};

/** Stop command structure */
struct ZbZclLevelClientStopReqT {
    bool with_onoff; /**< With Onoff - If true then cmd is ZCL_LEVEL_COMMAND_STOP_ONOFF, else ZCL_LEVEL_COMMAND_STOP */
    uint8_t mask; /**< OptionsMask - Not included if with_onoff is true */
    uint8_t override; /**< OptionsOverride - Not included if with_onoff is true */
};

/*-----------------------------------------------------------------------------
 * Server
 *-----------------------------------------------------------------------------
 */

/** Level Server callbacks configuration */
struct ZbZclLevelServerCallbacksT {
    /* Returns a ZCL Status code to send in the Default Response, or ZCL_STATUS_SUCCESS_NO_DEFAULT_RESPONSE
     * if no Default Response to be generated. */

    enum ZclStatusCodeT (*move_to_level)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientMoveToLevelReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move To Level command. Set with_onoff to true in the req struct when utilizing the
     * onoff cluster on the same endpoint. The application is expected to update ZCL_LEVEL_ATTR_CURRLEVEL */
    enum ZclStatusCodeT (*move)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientMoveReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Move command. Set with_onoff to true in the req struct when utilizing the onoff
     * cluster on the same endpoint. The application is expected to update ZCL_LEVEL_ATTR_CURRLEVEL */
    enum ZclStatusCodeT (*step)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientStepReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Step command. Set with_onoff to true in the req struct when utilizing the onoff
     * cluster on the same endpoint. The application is expected to update ZCL_LEVEL_ATTR_CURRLEVEL */
    enum ZclStatusCodeT (*stop)(struct ZbZclClusterT *cluster,
        struct ZbZclLevelClientStopReqT *req, struct ZbZclAddrInfoT *srcInfo, void *arg);
    /**< Callback to application, invoked on receipt of Stop command. Set with_onoff to true in the req struct when utilizing the onoff
     * cluster on the same endpoint. The application is expected to update ZCL_LEVEL_ATTR_CURRLEVEL */
};

/**
 * Create a new instance of the Level Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param onoff_server OnOff server cluster pointer for processing commands with the Options fields. May be NULL
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclLevelServerAlloc(struct ZigBeeT *zb,
    uint8_t endpoint, struct ZbZclClusterT *onoff_server,
    struct ZbZclLevelServerCallbacksT *callbacks, void *arg);

/*-----------------------------------------------------------------------------
 * Client
 *-----------------------------------------------------------------------------
 */

/**
 * Create a new instance of the Level Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclLevelClientAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Send a Move to Level command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move To Level command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclLevelClientMoveToLevelReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientMoveToLevelReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Move command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Move command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclLevelClientMoveReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientMoveReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Step command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Step command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclLevelClientStepReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientStepReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Stop command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Stop command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclLevelClientStopReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclLevelClientStopReqT *req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/*-----------------------------------------------------------------------------
 * Cluster Payload Helpers
 *-----------------------------------------------------------------------------
 */
int ZbZclLevelClientMoveToLevelBuild(uint8_t *payload, unsigned int length, struct ZbZclLevelClientMoveToLevelReqT *req);
int ZbZclLevelClientMoveBuild(uint8_t *payload, unsigned int length, struct ZbZclLevelClientMoveReqT *req);
int ZbZclLevelClientStepBuild(uint8_t *payload, unsigned int length, struct ZbZclLevelClientStepReqT *req);
int ZbZclLevelClientStopBuild(uint8_t *payload, unsigned int length, struct ZbZclLevelClientStopReqT *req);

#endif /* ZCL_LEVEL_H */
