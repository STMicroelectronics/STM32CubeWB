/* Copyright [2009 - 2019] Exegin Technologies Limited. All rights reserved. */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      The public header file for the Home Automation
 *      clusters of the ZCL.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_IAS_ACE_H
#define ZCL_IAS_ACE_H

/* PICS.ZCL.IASACE
 *
 * IASACE.S | True
 * IASACE.C | True
 *
 * IASACE.S.Afffd | True
 * IASACE.S.Afffe | False
 * IASACE.S.C00.Rsp | True
 * IASACE.S.C01.Rsp | True
 * IASACE.S.C02.Rsp | True
 * IASACE.S.C03.Rsp | True
 * IASACE.S.C04.Rsp | True
 * IASACE.S.C05.Rsp | True
 * IASACE.S.C06.Rsp | True
 * IASACE.S.C07.Rsp | True
 * IASACE.S.C08.Rsp | True
 * IASACE.S.C09.Rsp | True
 * IASACE.S.C00.Tx | True
 * IASACE.S.C01.Tx | True
 * IASACE.S.C02.Tx | True
 * IASACE.S.C03.Tx | True
 * IASACE.S.C04.Tx | True
 * IASACE.S.C05.Tx | True
 * IASACE.S.C06.Tx | True
 * IASACE.S.C07.Tx | True
 * IASACE.S.C08.Tx | True
 *
 * IASACE.C.Afffd | True
 * IASACE.C.Afffe | False
 * IASACE.C.C00.Tx | True
 * IASACE.C.C01.Tx | True
 * IASACE.C.C02.Tx | True
 * IASACE.C.C03.Tx | True
 * IASACE.C.C04.Tx | True
 * IASACE.C.C05.Tx | True
 * IASACE.C.C06.Tx | True
 * IASACE.C.C07.Tx | True
 * IASACE.C.C08.Tx | True
 * IASACE.C.C09.Tx | True
 * IASACE.C.C00.Rsp | True
 * IASACE.C.C01.Rsp | True
 * IASACE.C.C02.Rsp | True
 * IASACE.C.C03.Rsp | True
 * IASACE.C.C04.Rsp | True
 * IASACE.C.C05.Rsp | True
 * IASACE.C.C06.Rsp | True
 * IASACE.C.C07.Rsp | True
 * IASACE.C.C08.Rsp | True
 */

#include "zcl/zcl.h"
#include "zcl/security/zcl.ias_zone.h"

#define ZCL_IAS_ACE_SVR_MAX_ZONES             256

/* IAS ACE has no server or client attributes */

/* Server Generated Commands */
enum ZbZclIasAceServerCommandsT {
    ZCL_IAS_ACE_SVR_CMD_ARM_RSP = 0x00,
    ZCL_IAS_ACE_SVR_CMD_GET_ZONE_ID_MAP_RSP = 0x01,
    ZCL_IAS_ACE_SVR_CMD_GET_ZONE_INFO_RSP = 0x02,
    ZCL_IAS_ACE_SVR_CMD_ZONE_STATUS_CHANGED = 0x03,
    ZCL_IAS_ACE_SVR_CMD_PANEL_STATUS_CHANGED = 0x04,
    ZCL_IAS_ACE_SVR_CMD_GET_PANEL_STATUS_RSP = 0x05,
    ZCL_IAS_ACE_SVR_CMD_SET_BYPASSED_ZONE_LIST = 0x06,
    ZCL_IAS_ACE_SVR_CMD_BYPASS_RSP = 0x07,
    ZCL_IAS_ACE_SVR_CMD_GET_ZONE_STATUS_RSP = 0x08,
};

/* Client Generated Commands */
enum ZbZclIasAceClientCommandsT {
    ZCL_IAS_ACE_CLI_CMD_ARM = 0x00,
    ZCL_IAS_ACE_CLI_CMD_BYPASS = 0x01,
    ZCL_IAS_ACE_CLI_CMD_EMERGENCY = 0x02,
    ZCL_IAS_ACE_CLI_CMD_FIRE = 0x03,
    ZCL_IAS_ACE_CLI_CMD_PANIC = 0x04,
    ZCL_IAS_ACE_CLI_CMD_GET_ZONE_ID_MAP = 0x05,
    ZCL_IAS_ACE_CLI_CMD_GET_ZONE_INFO = 0x06,
    ZCL_IAS_ACE_CLI_CMD_GET_PANEL_STATUS = 0x07,
    ZCL_IAS_ACE_CLI_CMD_GET_BYPASSED_ZONE_LIST = 0x08,
    ZCL_IAS_ACE_CLI_CMD_GET_ZONE_STATUS = 0x09,
};

enum ZbZclIasAceArmModeT {
    ZCL_IAS_ACE_ARM_MODE_DISARM = 0x00,
    ZCL_IAS_ACE_ARM_MODE_ARM_DAY_ZONES = 0x01,
    ZCL_IAS_ACE_ARM_MODE_ARM_NIGHT_ZONES = 0x02,
    ZCL_IAS_ACE_ARM_MODE_ARM_ALL_ZONES = 0x03,
};

enum ZbZclIasAceArmNotifyT {
    ZCL_IAS_ACE_ARM_NOTIFY_ALL_ZONES_DISARMED = 0x00,
    ZCL_IAS_ACE_ARM_NOTIFY_ONLY_DAY_ZONES_ARMED = 0x01,
    ZCL_IAS_ACE_ARM_NOTIFY_ONLY_NIGHT_ZONES_ARMED = 0x02,
    ZCL_IAS_ACE_ARM_NOTIFY_ALL_ZONES_ARMED = 0x03,
    ZCL_IAS_ACE_ARM_NOTIFY_INVALID_ARM_CODE = 0x04,
    ZCL_IAS_ACE_ARM_NOTIFY_NOT_READY_TO_ARM = 0x05,
    ZCL_IAS_ACE_ARM_NOTIFY_ALREADY_DISARMED = 0x06,
};

enum ZbZclIasAcePanelStatusT {
    ZCL_IAS_ACE_PANEL_STATUS_PANEL_DISARMED = 0x00,
    ZCL_IAS_ACE_PANEL_STATUS_ARMED_STAY = 0x01,
    ZCL_IAS_ACE_PANEL_STATUS_ARMED_NIGHT = 0x02,
    ZCL_IAS_ACE_PANEL_STATUS_ARMED_AWAY = 0x03,
    ZCL_IAS_ACE_PANEL_STATUS_EXIT_DELAY = 0x04,
    ZCL_IAS_ACE_PANEL_STATUS_ENTRY_DELAY = 0x05,
    ZCL_IAS_ACE_PANEL_STATUS_NOT_READY_TO_ARM = 0x06,
    ZCL_IAS_ACE_PANEL_STATUS_IN_ALARM = 0x07,
    ZCL_IAS_ACE_PANEL_STATUS_ARMING_STAY = 0x08,
    ZCL_IAS_ACE_PANEL_STATUS_ARMING_NIGHT = 0x09,
    ZCL_IAS_ACE_PANEL_STATUS_ARMING_AWAY = 0x0a,
};

enum ZbZclIasAceAudibleNotifyT {
    ZCL_IAS_ACE_AUDIBLE_NOTIFY_MUTE = 0x00,
    ZCL_IAS_ACE_AUDIBLE_NOTIFY_DEFAULT_SOUND = 0x01,
};

enum ZbZclIasAceAlarmStatusT {
    ZCL_IAS_ACE_ALARM_STATUS_NO_ALARM = 0x00,
    ZCL_IAS_ACE_ALARM_STATUS_BURGLAR = 0x01,
    ZCL_IAS_ACE_ALARM_STATUS_FIRE = 0x02,
    ZCL_IAS_ACE_ALARM_STATUS_EMERGENCY = 0x03,
    ZCL_IAS_ACE_ALARM_STATUS_POLICE_PANIC = 0x04,
    ZCL_IAS_ACE_ALARM_STATUS_FIRE_PANIC = 0x05,
    ZCL_IAS_ACE_ALARM_STATUS_EMERGENCY_PANIC = 0x06,
};

enum ZbZclIasAceBypassResultT {
    ZCL_IAS_ACE_BYPASS_RESULT_ZONE_BYPASSED = 0x00,
    ZCL_IAS_ACE_BYPASS_RESULT_ZONE_NOT_BYPASSED = 0x01,
    ZCL_IAS_ACE_BYPASS_RESULT_NOT_ALLOWED = 0x02,
    ZCL_IAS_ACE_BYPASS_RESULT_INVALID_ZONE_ID = 0x03,
    ZCL_IAS_ACE_BYPASS_RESULT_UNKNOWN_ZONE_ID = 0x04,
    ZCL_IAS_ACE_BYPASS_RESULT_INVALID_ARM_CODE = 0x05,
};

enum ZbZclIasAceBypassPermsT {
    ZCL_IAS_ACE_BYPASS_PERMS_ALLOWED = 0x00,
    ZCL_IAS_ACE_BYPASS_PERMS_NOT_ALLOWED = 0x01,
};

/*---------------------------------------------------------------
 * Client Command Structures
 *---------------------------------------------------------------
 */
/* Arbitrary max. "There is no minimum or maximum length to the
 * Arm/Disarm Code; however, the Arm/Disarm Code SHOULD be between
 * four and eight alphanumeric characters in length." */
#define ZCL_IAS_ACE_ARM_CODE_STRING_MAX_LEN          32

struct ZbZclIasAceClientCommandArmT {
    enum ZbZclIasAceArmModeT arm_mode;
    char arm_code[ZCL_IAS_ACE_ARM_CODE_STRING_MAX_LEN + 1U];
    /* EXEGIN - what is the Zone ID used for here?
     * We're arming/disarming the entire system or set of zones.
     * There's nothing to indicate an individual zone
     * is useful here. */
    uint8_t zone_id;
};

/* Arbitrary max. May want to convert to a list with a max of 255. */
#define ZCL_IAS_ACE_BYPASS_MAX_ZONES                 32

struct ZbZclIasAceClientCommandBypassT {
    uint8_t num_zones;
    uint8_t zone_id_list[ZCL_IAS_ACE_BYPASS_MAX_ZONES];
    char arm_code[ZCL_IAS_ACE_ARM_CODE_STRING_MAX_LEN + 1U];
};

struct ZbZclIasAceClientCommandGetZoneInfoT {
    uint8_t zone_id;
};

struct ZbZclIasAceClientCommandGetZoneStatusT {
    uint8_t starting_zone_id;
    uint8_t max_zone_ids;
    uint8_t zone_status_mask_flag;
    uint16_t zone_status_mask;
};

/*---------------------------------------------------------------
 * Server Command Structures
 *---------------------------------------------------------------
 */

struct ZbZclIasAceServerCommandArmRspT {
    enum ZbZclIasAceArmNotifyT arm_notify;
};

#define ZCL_IAS_ACE_ZONE_ID_MAP_NUM_SECTIONS         16

struct ZbZclIasAceServerCommandGetZoneIdMapRspT {
    uint16_t zond_id_map_list[ZCL_IAS_ACE_ZONE_ID_MAP_NUM_SECTIONS];
};

/* Arbitrary max. "There is no minimum or maximum length to the Zone Label
 * field; however, the Zone Label SHOULD be between 16 to 24 alphanumeric
 * characters in length." */
#define ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN        32

struct ZbZclIasAceServerCommandGetZoneInfoRspT {
    uint8_t zone_id;
    enum ZbZclIasZoneServerZoneTypeT zone_type;
    uint64_t zone_addr;
    char zone_label[ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN + 1U];
};

struct ZbZclIasAceServerCommandZoneStatusChangedT {
    uint8_t zone_id;
    enum ZbZclIasZoneServerZoneStatusT zone_status;
    enum ZbZclIasAceAudibleNotifyT audible_notify;
    char zone_label[ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN + 1U];
};

struct ZbZclIasAceServerCommandGetPanelStatusRspT {
    enum ZbZclIasAcePanelStatusT panel_status;
    uint8_t seconds_remain;
    enum ZbZclIasAceAudibleNotifyT audible_notify;
    enum ZbZclIasAceAlarmStatusT alarm_status;
};

struct ZbZclIasAceServerCommandSetBypassedZoneListT {
    uint8_t num_zones;
    uint8_t zone_id_list[ZCL_IAS_ACE_BYPASS_MAX_ZONES];
};

struct ZbZclIasAceServerCommandBypassRspT {
    uint8_t num_zones;
    enum ZbZclIasAceBypassResultT bypass_result_list[ZCL_IAS_ACE_BYPASS_MAX_ZONES];
};

#define ZCL_IAS_ACE_ZONE_STATUS_MAX_ZONES            10

struct ZbZclIasAceServerCommandGetZoneStatusRspT {
    uint8_t zone_status_complete;
    uint8_t num_zones;
    struct {
        uint8_t zone_id;
        enum ZbZclIasZoneServerZoneStatusT zone_status;
    } zone_list[ZCL_IAS_ACE_ZONE_STATUS_MAX_ZONES];
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */

struct ZbZclIasAceServerCallbacksT {
    /* The callback for Arming should handle arming the system and
     * calling ZbZclIasAceServerPanelStatusConfig when the panel status changes. */
    bool (*arm_req)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclIasAceClientCommandArmT *arm_req,
        struct ZbZclIasAceServerCommandArmRspT *arm_rsp);

    void (*bypass_req)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclIasAceClientCommandBypassT *bypass_req,
        struct ZbZclIasAceServerCommandBypassRspT *bypass_rsp);

    /* Emergency, Fire and Panic callbacks return a ZCL status to
     * return in the Default Response (e.g. ZCL_STATUS_SUCCESS). */
    uint8_t (*emerg_req)(struct ZbZclClusterT *clusterPtr, void *arg, struct ZbZclAddrInfoT *srcInfo);
    uint8_t (*fire_req)(struct ZbZclClusterT *clusterPtr, void *arg, struct ZbZclAddrInfoT *srcInfo);
    uint8_t (*panic_req)(struct ZbZclClusterT *clusterPtr, void *arg, struct ZbZclAddrInfoT *srcInfo);
};

/* Allocate the IAS ACE Server cluster.
 *
 * If 'use_trip_pair' is true, application must call ZbZclIasAceServerEnrollRequest
 * to perform the 'trip-to-pair' process, unless the IAS CIE has sent us an
 * unsolicited Auto-Enroll-Response. */
struct ZbZclClusterT * ZbZclIasAceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclIasAceServerCallbacksT *callbacks, void *arg);

void ZbZclIasAceServerConfigCallbacks(struct ZbZclClusterT *clusterPtr,
    struct ZbZclIasAceServerCallbacksT *callbacks);

/* Change the Panel Arm/Disarm Code */
bool ZbZclIasAceServerPanelCodeConfig(struct ZbZclClusterT *clusterPtr,
    const char *arm_code);

/* Update the Panel Status */
bool ZbZclIasAceServerPanelStatusConfig(struct ZbZclClusterT *clusterPtr,
    enum ZbZclIasAcePanelStatusT panel_status, uint8_t seconds_remain,
    enum ZbZclIasAceAudibleNotifyT audible_notify);

/* Returns the first free Zone ID not already in the Zone Table. */
bool ZbZclIasAceServerGetFreeZoneId(struct ZbZclClusterT *clusterPtr, uint8_t *zone_id_ptr);

/* Adds a new zone. Returns true if successful, and the assigned Zone ID
 * is returned via zone_id. */
struct ZbZclIasAceServerZoneTableAddT {
    enum ZbZclIasZoneServerZoneTypeT zone_type;
    uint64_t zone_addr;
    /* zone_label - optional, may be NULL.
     * Max length = ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN. */
    const char *zone_label;
    /* zone_id - can use ZbZclIasAceServerGetFreeZoneId */
    uint8_t zone_id;
};

bool ZbZclIasAceServerZoneTableAdd(struct ZbZclClusterT *clusterPtr,
    struct ZbZclIasAceServerZoneTableAddT *req);

/* Delete a zone */
bool ZbZclIasAceServerZoneTableDeleteById(struct ZbZclClusterT *clusterPtr, uint8_t zone_id);
bool ZbZclIasAceServerZoneTableDeleteByAddr(struct ZbZclClusterT *clusterPtr, uint64_t addr);

/* Returns address of paired zone, or 0 if not found. */
uint64_t ZbZclIasAceServerZoneTableAddrLookup(struct ZbZclClusterT *clusterPtr, uint8_t zone_id);

bool ZbZclIasAceServerZoneTableIdLookup(struct ZbZclClusterT *clusterPtr,
    uint64_t zone_addr, uint8_t *zone_id_ptr);

bool ZbZclIasAceServerZoneStatusConfig(struct ZbZclClusterT *clusterPtr,
    uint8_t zone_id, enum ZbZclIasZoneServerZoneStatusT zone_status,
    enum ZbZclIasAceAudibleNotifyT audible_notify);

bool ZbZclIasAceServerZoneBypassPerms(struct ZbZclClusterT *clusterPtr,
    uint8_t zone_id, enum ZbZclIasAceBypassPermsT bypass_perms);

enum ZbZclIasAceBypassResultT ZbZclIasAceServerZoneBypassConfig(struct ZbZclClusterT *clusterPtr,
    uint8_t zone_id, bool bypass);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */
struct ZbZclClusterT * ZbZclIasAceClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, void *arg);

/* ARM */
uint8_t ZbZclIasAceClientCommandArmReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandArmT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

bool ZbZclIasAceClientParseArmRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandArmRspT *rsp);

/* BYPASS */
uint8_t ZbZclIasAceClientCommandBypassReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandBypassT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

bool ZbZclIasAceClientParseBypassRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandBypassRspT *rsp);

/* Emergency / Fire / Panic */
uint8_t ZbZclIasAceClientCommandEmergencyReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

uint8_t ZbZclIasAceClientCommandFireReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

uint8_t ZbZclIasAceClientCommandPanicReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Get Zone Id Map */
uint8_t ZbZclIasAceClientCommandGetZoneIdMapReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

bool ZbZclIasAceClientParseGetZoneIdMapRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetZoneIdMapRspT *rsp);

/* Get Zone Info */
uint8_t ZbZclIasAceClientCommandGetZoneInfoReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandGetZoneInfoT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

bool ZbZclIasAceClientParseGetZoneInfoRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetZoneInfoRspT *rsp);

/* Zone / Panel Status Changed */
bool ZbZclIasAceClientParseZoneStatusChanged(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandZoneStatusChangedT *rsp);

/* Panel Status Changed command has same payload as Get Panel Status Response.
 * Use ZbZclIasAceClientParseGetPanelStatusRsp. */

/* Get Panel Status */
uint8_t ZbZclIasAceClientCommandGetPanelStatusReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

bool ZbZclIasAceClientParseGetPanelStatusRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetPanelStatusRspT *rsp);

/* Get Bypassed Zone List */
uint8_t ZbZclIasAceClientCommandGetBypassedZoneListReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

bool ZbZclIasAceClientParseSetBypassedZoneList(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandSetBypassedZoneListT *rsp);

/* Get Zone Status */
uint8_t ZbZclIasAceClientCommandGetZoneStatusReq(struct ZbZclClusterT *clusterPtr,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandGetZoneStatusT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

bool ZbZclIasAceClientParseGetZoneStatusRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetZoneStatusRspT *rsp);

#endif /* __ZCL_IAS_ACE_H */
