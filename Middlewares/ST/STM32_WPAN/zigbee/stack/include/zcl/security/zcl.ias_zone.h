/**
 * @file zcl.ias_zone.h
 * @heading IAS Zone
 * @brief ZCL Security IAS Zone cluster header
 * ZCL 7 section 8.2
 * ZCL 8 section 8.2
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_IAS_ZONE_H
#define ZCL_IAS_ZONE_H

/* @PICS.ZCL.IASZ
 *
 * IASZ.S | Server |True
 * IASZ.C | Clinet | True
 * IASZ.TTP | Trip-to-Pair Enrollment | True
 * IASZ.ARSP | Auto-Enroll-Response Enrollment |True
 * IASZ.AREQ | Auto-Enroll-Request Enrollment | True
 * IASZ.PIXIT01 | See Test Spec | False
 * IASZ.PIXIT02 | See Text Spec | False
 *
 * Server Attributes
 * IASZ.S.A0000 | ZoneState | True
 * IASZ.S.A0001 | ZoneType | True
 * IASZ.S.A0002 | ZoneStatus | True
 * IASZ.S.A0010 | IAS_CIE_Address |True
 * IASZ.S.A0011 | ZoneID | True
 * IASZ.S.A0012 | NumberOfZoneSensitivityLevelsSupported | False | Optional
 * IASZ.S.A0013 | CurrentZoneSensitivityLevel | False |  Optional
 * IASZ.S.Afffd | ClusterRevision | True
 * IASZ.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * IASZ.S.C00.Rsp | Zone Enroll Response | True
 * IASZ.S.C01.Rsp | Initiate Normal Operation Mode | True
 * IASZ.S.C02.Rsp | Initiate Test Mode | True
 *
 * Commands Generated
 * IASZ.S.C00.Tx | Zone Status Change Notification | True
 * IASZ.S.C01.Tx | Zone Enroll Request | True
 *
 * Client Attributes
 * IASZ.C.Afffd | ClusterRevision | True
 * IASZ.C.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * IASZ.C.C00.Rsp | Zone Status Change Notification  | True
 * IASZ.C.C01.Rsp | Zone Enroll Request | True
 *
 * Commands Generated
 * IASZ.C.C00.Tx | Zone Enroll Response | True
 * IASZ.C.C01.Tx | Initiate Normal Operation Mode | True
 * IASZ.C.C02.Tx | Initiate Test Mode  | True
 */

#include "zcl/zcl.h"

/** IAS Zone Server Attribute IDs */
enum ZbZclIasZoneServerAttrT {
    /* Zone Information Set */
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_STATE = 0x0000, /**< ZoneState */
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_TYPE = 0x0001, /**< ZoneType */
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_STATUS = 0x0002, /**< ZoneStatus */
    /* Zone Settings Set */
    ZCL_IAS_ZONE_SVR_ATTR_CIE_ADDR = 0x0010, /**< IAS_CIE_Address */
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_ID = 0x0011, /**< ZoneID */
    ZCL_IAS_ZONE_SVR_ATTR_NUM_ZONE_SENSITIVITY_SUPPORTED = 0x0012, /**< NumberOfZoneSensitivityLevelsSupported (Optional) */
    ZCL_IAS_ZONE_SVR_ATTR_CURRENT_ZONE_SENSITIVITY_LEVEL = 0x0013, /**< CurrentZoneSensitivityLevel (Optional) */

    /* Exegin add-on (internal) to preserve endpoint of CIE */
    ZCL_IAS_ZONE_SVR_ATTR_CIE_ENDPOINT = 0x7fff, /**< Exegin internal (Optional) */
};

/* IAS Zone Server commands/responses */
enum ZbZclIasZoneServerCommandsT {
    ZCL_IAS_ZONE_SVR_CMD_ZONE_STATUS_CHANGE_NOTIFY = 0x00, /* Zone Status Change Notification */
    ZCL_IAS_ZONE_SVR_CMD_ZONE_ENROLL_REQUEST = 0x01, /* Zone Enroll Request */
};

/* IAS Zone Client commands/responses */
enum ZbZclIasZoneClientCommandsT {
    ZCL_IAS_ZONE_CLI_CMD_ZONE_ENROLL_RESPONSE = 0x00, /* Zone Enroll Response */
    ZCL_IAS_ZONE_CLI_CMD_INITIATE_NORMAL_MODE = 0x01, /* Initiate Normal Operation Mode (Optional) */
    ZCL_IAS_ZONE_CLI_CMD_INITIATE_TEST_MODE = 0x02, /* Initiate Test Mode (Optional) */
};

/** IAS Zone ZoneState Attribute */
enum ZbZclIasZoneServerZoneStateT {
    ZCL_IAS_ZONE_SVR_STATE_NOT_ENROLLED = 0x00, /**< Not enrolled */
    ZCL_IAS_ZONE_SVR_STATE_ENROLLED = 0x01,
    /**< Enrolled (the client will react to Zone State Change Notification commands from the server) */
};

/** IAS Zone ZoneType Attribute */
enum ZbZclIasZoneServerZoneTypeT {
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_STANDARD_CIE = 0x0000, /**< Standard CIE */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_MOTION_SENSOR = 0x000d, /**< Motion sensor */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_CONTACT_SWITCH = 0x0015, /**< Contact switch */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_DOOR_WINDOW = 0x0016, /**< Door/Window handle */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_FIRE_SENSOR = 0x0028, /**< Fire sensor */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_WATER_SENSOR = 0x002a, /**< Water sensor */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_CO_SENSOR = 0x002b, /**< Carbon Monoxide (CO) sensor */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_PERSONAL_EMERG_DEVICE = 0x002c, /**< Personal emergency device */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_MOVEMENT_SENSOR = 0x002d, /**< Vibration/Movement sensor */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_REMOTE_CONTROL = 0x010f, /**< Remote Control */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_KEY_FOB = 0x0115, /**< Key fob */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_KEYPAD = 0x021d, /**< Keypad */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_STANDARD_WARNING_DEVICE = 0x0225, /**< Standard Warning Device (see [N1] part 4) */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_GLASS_SENSOR = 0x0226, /**< Glass break sensor */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_SECURITY_REPEATER = 0x0229, /**< Security repeater */
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_INVALID = 0xffff /**< Invalid Zone Type */
};

/** IAS Zone ZoneStatus Attribute */
enum ZbZclIasZoneServerZoneStatusT {
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_NONE = 0,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_ALARM1 = 1 << 0, /**< Alarm1 */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_ALARM2 = 1 << 1, /**< Alarm2 */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_TAMPER = 1 << 2, /**< Tamper */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_BATTERY = 1 << 3, /**< Battery */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_SUPERVISION_REPORTS = 1 << 4, /**< Supervision Notify */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_RESTORE_REPORTS = 1 << 5, /**< Restore Notify */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_TROUBLE = 1 << 6, /**< Trouble */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_AC_MAINS = 1 << 7, /**< AC (mains) */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_TEST = 1 << 8, /**< Test */
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_BATTERY_DEFECT = 1 << 9, /**< Battery Defect */
};
#define ZCL_IAS_ZONE_SVR_ZONE_STATUS_MASK           0x03ffU

/** IAS Zone ZoneStatus Attribute Bit Test Value */
enum ZbZclIasZoneServerModeT {
    ZCL_IAS_ZONE_SVR_MODE_NORMAL = 0, /**< Normal */
    ZCL_IAS_ZONE_SVR_MODE_TEST, /**< Set */
};

/** IAS Zone Enroll Response Code */
enum ZbZclIasZoneClientResponseCodeT {
    ZCL_IAS_ZONE_CLI_RESP_SUCCESS = 0x00, /**< Success */
    ZCL_IAS_ZONE_CLI_RESP_NOT_SUPPORTED = 0x01, /**< Not supported */
    ZCL_IAS_ZONE_CLI_RESP_NO_ENROLL_PERMIT = 0x02, /**< No enroll permit */
    ZCL_IAS_ZONE_CLI_RESP_TOO_MANY_ZONES = 0x03, /**< Too many zones */
};

/** Zone State Change Notification request structure */
struct ZbZclIasZoneServerStatusChangeNotifyT {
    enum ZbZclIasZoneServerZoneStatusT zone_status; /**< Zone Status */
    uint8_t ext_status; /**< Extended Status */
    uint8_t zone_id; /**< Zone ID */
    uint16_t delay; /**< Delay */
};

/** Zone Enroll request structure */
struct ZbZclIasZoneServerEnrollRequestT {
    enum ZbZclIasZoneServerZoneTypeT zone_type; /**< Zone Type */
    uint16_t manuf_code; /**< Manufacturer Code */
};

/** Zone Enroll response structure */
struct ZbZclIasZoneClientEnrollResponseT {
    enum ZclStatusCodeT zcl_status; /**< Response status */
    enum ZbZclIasZoneClientResponseCodeT enroll_status; /**< Enroll response code */
    uint8_t zone_id; /**< Zone ID */
};

/** Initiate Test Mode request structure */
struct ZbZclIasZoneClientTestModeReqT {
    uint8_t test_duration; /**< Test Mode Duration */
    uint8_t current_zone_sensitivity; /**< Current Zone Sensitivity Level */
};

/** IAS Zone Server callbacks configuration */
struct ZbZclIasZoneServerCallbacksT {
    /* Callback so the application is made aware of mode change
     * (i.e. Normal vs Test).
     * 'req' is only valid if mode == ZCL_IAS_ZONE_SVR_MODE_TEST.
     * Return value is a ZCL Status. If status indicates an error,
     * it sent in a Default Response back to the originator. */
    enum ZclStatusCodeT (*mode_change)(struct ZbZclClusterT *cluster,
        void *arg, /* ZbZclClusterSetCallbackArg */
        enum ZbZclIasZoneServerModeT mode,
        struct ZbZclIasZoneClientTestModeReqT *req);
    /**< Callback to application, invoked on receipt of Initiate Normal Operation Mode
     *  or Initiate Test Mode command
     */
};

/**
 * Create a new instance of the IAS Zone Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param zone_type Zone Type
 * @param manuf_code Manufacturer Code
 * @param use_trip_pair If true, use 'trip-to-pair' application must call ZbZclIasZoneServerEnrollRequest to perform
 * the 'trip-to-pair' process, unless the IAS CIE has sent us an unsolicited Auto-Enroll-Response
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIasZoneServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t zone_type, uint16_t manuf_code, bool use_trip_pair,
    struct ZbZclIasZoneServerCallbacksT *callbacks, void *arg);

/**
 * Send a Zone Enroll request command
 * Used with 'trip-to-pair'. Before sending a Zone Enroll Request, the IAS CIE
 * must write to the IAS_CIE_Address attribute with its IEEE address.
 * @param cluster Cluster instance from which to send this command
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclIasZoneServerEnrollRequest(struct ZbZclClusterT *cluster,
    void (*callback)(struct ZbZclIasZoneClientEnrollResponseT *enrl_rsp, void *arg), void *arg);

/** IAS Zone Client callbacks configuration */
struct ZbZclIasZoneClientCallbacksT {
    /* ZCL_IAS_ZONE_SVR_CMD_ZONE_STATUS_CHANGE_NOTIFY callback */
    void (*zone_status_change)(
        struct ZbZclClusterT *cluster,
        void *arg, /* ZbZclClusterSetCallbackArg */
        struct ZbZclIasZoneServerStatusChangeNotifyT *notify,
        const struct ZbApsAddrT *src);
    /**< Callback to application, invoked on receipt of Zone Status Change Notification command */

    /* ZCL_IAS_ZONE_SVR_CMD_ZONE_ENROLL_REQUEST callback.
     * Returns a ZCL Status Code. If not SUCCESS, a Default Response is
     * sent with the status code. If ZCL_STATUS_SUCCESS, the
     * Zone Enroll Response command is sent.*/
    enum ZclStatusCodeT (*zone_enroll_req)(
        struct ZbZclClusterT *cluster,
        void *arg, /* ZbZclClusterSetCallbackArg */
        struct ZbZclIasZoneServerEnrollRequestT *req,
        uint64_t ext_src_addr,
        /* OUT: Enroll response code */
        enum ZbZclIasZoneClientResponseCodeT *rsp_code,
        /* OUT: Zone ID */
        uint8_t *zone_id);
    /**< Callback to application, invoked on receipt of Zone Enroll command */
};

/**
 * Create a new instance of the IAS Zone Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIasZoneClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclIasZoneClientCallbacksT *callbacks, void *arg);

/**
 * Send a Zone Auto-Enroll request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasZoneClientInitiateAutoEnroll(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    void (*callback)(const struct ZbZclWriteRspT *, void *), void *arg);

/**
 * Send a Zone Auto-Enroll response command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param zone_id Zone ID
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasZoneClientSendAutoEnrollResponse(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, uint8_t zone_id,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Initiate Normal Operation Mode request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasZoneClientInitiateNormalMode(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/**
 * Send a Initiate Test Mode request command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param req Initiate Test Mode request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasZoneClientInitiateTestMode(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclIasZoneClientTestModeReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/* For internal tesitng only ----------------------------------------------- */
enum ZclStatusCodeT zcl_ias_zone_server_initiate_test_mode(struct ZbZclClusterT *clusterPtr,
    struct ZbZclIasZoneClientTestModeReqT *req);
enum ZclStatusCodeT zcl_ias_zone_server_initiate_normal_mode(struct ZbZclClusterT *clusterPtr);

#endif
