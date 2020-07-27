/* Copyright [2009 - 2020] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      The public header file for the Home Automation
 *      clusters of the ZCL.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_IAS_ZONE_H
#define ZCL_IAS_ZONE_H

/* PICS.ZCL.IASZ
 *
 * IASZ.S | True
 * IASZ.C | True
 * IASZ.TTP | True
 * IASZ.ARSP | True
 * IASZ.AREQ | True
 * IASZ.PIXIT01 | False (seems like it would require a more complete sensor application)
 * IASZ.PIXIT02 | False (seems like it would require a more complete sensor application)
 *
 * IASZ.S.A0000 | True
 * IASZ.S.A0001 | True
 * IASZ.S.A0002 | True
 * IASZ.S.A0010 | True
 * IASZ.S.A0011 | True
 * IASZ.S.A0012 | False
 * IASZ.S.A0013 | False
 * IASZ.S.Afffd | True
 * IASZ.S.Afffe | False
 * IASZ.S.C00.Rsp | True
 * IASZ.S.C01.Rsp | True
 * IASZ.S.C02.Rsp | True
 * IASZ.S.C00.Tx | True
 * IASZ.S.C01.Tx | True
 *
 * IASZ.C.Afffd | True
 * IASZ.C.Afffe | False
 * IASZ.C.C00.Rsp | True
 * IASZ.C.C01.Rsp | True
 * IASZ.C.C00.Tx | True
 * IASZ.C.C01.Tx | True
 * IASZ.C.C02.Tx | True
 */

#include "zcl/zcl.h"

/* Zone Attributes */
enum ZbZclIasZoneServerAttrT {
    /* Zone Information Set */
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_STATE = 0x0000,
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_TYPE = 0x0001,
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_STATUS = 0x0002,
    /* Zone Settings Set */
    ZCL_IAS_ZONE_SVR_ATTR_CIE_ADDR = 0x0010, /* IAS_CIE_ADDR */
    ZCL_IAS_ZONE_SVR_ATTR_ZONE_ID = 0x0011,
    ZCL_IAS_ZONE_SVR_ATTR_NUM_ZONE_SENSITIVITY_SUPPORTED = 0x0012,
    ZCL_IAS_ZONE_SVR_ATTR_CURRENT_ZONE_SENSITIVITY_LEVEL = 0x0013,

    /* Exegin add-on (internal) to preserve endpoint of CIE */
    ZCL_IAS_ZONE_SVR_ATTR_CIE_ENDPOINT = 0x7fff,
};

/* Server Generated Commands */
enum ZbZclIasZoneServerCommandsT {
    ZCL_IAS_ZONE_SVR_CMD_ZONE_STATUS_CHANGE_NOTIFY = 0x00,
    ZCL_IAS_ZONE_SVR_CMD_ZONE_ENROLL_REQUEST = 0x01,
};

/* Client Generated Commands */
enum ZbZclIasZoneClientCommandsT {
    ZCL_IAS_ZONE_CLI_CMD_ZONE_ENROLL_RESPONSE = 0x00,
    ZCL_IAS_ZONE_CLI_CMD_INITIATE_NORMAL_MODE = 0x01,
    ZCL_IAS_ZONE_CLI_CMD_INITIATE_TEST_MODE = 0x02,
};

/* ZCL_IAS_ZONE_SVR_ATTR_ZONE_STATE */
enum ZbZclIasZoneServerZoneStateT {
    ZCL_IAS_ZONE_SVR_STATE_NOT_ENROLLED = 0x00,
    ZCL_IAS_ZONE_SVR_STATE_ENROLLED = 0x01,
};

/* ZCL_IAS_ZONE_SVR_ATTR_ZONE_TYPE */
enum ZbZclIasZoneServerZoneTypeT {
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_STANDARD_CIE = 0x0000,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_MOTION_SENSOR = 0x000d,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_CONTACT_SWITCH = 0x0015,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_FIRE_SENSOR = 0x0028,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_WATER_SENSOR = 0x002a,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_CO_SENSOR = 0x002b,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_PERSONAL_EMERG_DEVICE = 0x002c,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_MOVEMENT_SENSOR = 0x002d,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_REMOTE_CONTROL = 0x010f,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_KEY_FOB = 0x0115,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_KEYPAD = 0x021d,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_STANDARD_WARNING_DEVICE = 0x0225,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_GLASS_SENSOR = 0x0226,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_SECURITY_REPEATER = 0x0229,
    ZCL_IAS_ZONE_SVR_ZONE_TYPE_INVALID = 0xffff,
};

/* ZCL_IAS_ZONE_SVR_ATTR_ZONE_STATUS */
enum ZbZclIasZoneServerZoneStatusT {
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_ALARM1 = 1 << 0,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_ALARM2 = 1 << 1,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_TAMPER = 1 << 2,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_BATTERY = 1 << 3,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_SUPERVISION_REPORTS = 1 << 4,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_RESTORE_REPORTS = 1 << 5,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_TROUBLE = 1 << 6,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_AC_MAINS = 1 << 7,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_TEST = 1 << 8,
    ZCL_IAS_ZONE_SVR_ZONE_STATUS_BATTERY_DEFECT = 1 << 9,
};

/* Sensor mode: I.e. Zone Status "Test" bit cleared or set */
enum ZbZclIasZoneServerModeT {
    ZCL_IAS_ZONE_SVR_MODE_NORMAL = 0,
    ZCL_IAS_ZONE_SVR_MODE_TEST,
};

/* ZCL_IAS_ZONE_CLI_CMD_ZONE_ENROLL_RESPONSE */
enum ZbZclIasZoneClientResponseCodeT {
    ZCL_IAS_ZONE_CLI_RESP_SUCCESS = 0x00,
    ZCL_IAS_ZONE_CLI_RESP_NOT_SUPPORTED = 0x01,
    ZCL_IAS_ZONE_CLI_RESP_NO_ENROLL_PERMIT = 0x02,
    ZCL_IAS_ZONE_CLI_RESP_TOO_MANY_ZONES = 0x03,
};

/*---------------------------------------------------------------
 * API Structures
 *---------------------------------------------------------------
 */
struct ZbZclIasZoneServerStatusChangeNotifyT {
    enum ZbZclIasZoneServerZoneStatusT zone_status;
    uint8_t ext_status; /* reserved, set to 0x00 */
    uint8_t zone_id;
    uint16_t delay;
};

struct ZbZclIasZoneServerEnrollRequestT {
    uint16_t zone_type;
    uint16_t manuf_code;
};

struct ZbZclIasZoneClientEnrollResponseT {
    uint8_t zcl_status;
    enum ZbZclIasZoneClientResponseCodeT enroll_status;
    uint8_t zone_id;
};

struct ZbZclIasZoneClientTestModeReqT {
    uint8_t test_duration; /* seconds */
    uint8_t current_zone_sensitivity; /* 0x00 means to use the device's default */
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */
struct ZbZclIasZoneServerCallbacksT {
    /* Callback so the application is made aware of mode change
     * (i.e. Normal vs Test).
     * 'req' is only valid if mode == ZCL_IAS_ZONE_SVR_MODE_TEST.
     * Return value is a ZCL Status. If status indicates an error,
     * it sent in a Default Response back to the originator. */
    uint8_t (*mode_change)(struct ZbZclClusterT *clusterPtr,
        void *arg, /* ZbZclClusterSetCallbackArg */
        enum ZbZclIasZoneServerModeT mode,
        struct ZbZclIasZoneClientTestModeReqT *req);
};

/* If 'use_trip_pair' is true, application must call ZbZclIasZoneServerEnrollRequest
 * to perform the 'trip-to-pair' process, unless the IAS CIE has sent us an
 * unsolicited Auto-Enroll-Response. */
struct ZbZclClusterT * ZbZclIasZoneServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    uint16_t zone_type, uint16_t manuf_code, bool use_trip_pair,
    struct ZbZclIasZoneServerCallbacksT *callbacks, void *arg);

/* Used with 'trip-to-pair'. Before sending a Zone Enroll Request, the IAS CIE
 * must write to the IAS_CIE_Address attribute with its IEEE address. */
uint8_t ZbZclIasZoneServerEnrollRequest(struct ZbZclClusterT *clusterPtr,
    void (*callback)(struct ZbZclIasZoneClientEnrollResponseT *enrl_rsp, void *arg), void *arg);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */
struct ZbZclIasZoneClientCallbacksT {
    /* ZCL_IAS_ZONE_SVR_CMD_ZONE_STATUS_CHANGE_NOTIFY callback */
    void (*zone_status_change)(
        struct ZbZclClusterT *clusterPtr,
        void *arg, /* ZbZclClusterSetCallbackArg */
        struct ZbZclIasZoneServerStatusChangeNotifyT *notify,
        const struct ZbApsAddrT *src);

    /* ZCL_IAS_ZONE_SVR_CMD_ZONE_ENROLL_REQUEST callback.
     * Returns a ZCL Status Code. If not SUCCESS, a Default Response is
     * sent with the status code. If ZCL_STATUS_SUCCESS, the
     * Zone Enroll Response command is sent.*/
    uint8_t (*zone_enroll_req)(
        struct ZbZclClusterT *clusterPtr,
        void *arg, /* ZbZclClusterSetCallbackArg */
        struct ZbZclIasZoneServerEnrollRequestT *req,
        uint64_t ext_src_addr,
        /* OUT: Enroll response code */
        enum ZbZclIasZoneClientResponseCodeT *rsp_code,
        /* OUT: Zone ID */
        uint8_t *zone_id);
};

struct ZbZclClusterT * ZbZclIasZoneClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclIasZoneClientCallbacksT *callbacks, void *arg);

uint8_t ZbZclIasZoneClientInitiateAutoEnroll(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(const struct ZbZclWriteRspT *, void *), void *arg);

uint8_t ZbZclIasZoneClientSendAutoEnrollResponse(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, uint8_t zone_id,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/* Sends a ZCL_IAS_ZONE_CLI_CMD_INITIATE_NORMAL_MODE to destination */
uint8_t ZbZclIasZoneClientInitiateNormalMode(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

/* Sends a ZCL_IAS_ZONE_CLI_CMD_INITIATE_TEST_MODE to destination.
 * test_duration is in seconds.
 * current_zone_sensitivity of 0x00 means use device's default. */
uint8_t ZbZclIasZoneClientInitiateTestMode(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclIasZoneClientTestModeReqT *req,
    void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif /* __ZCL_IAS_ZONE_H */
