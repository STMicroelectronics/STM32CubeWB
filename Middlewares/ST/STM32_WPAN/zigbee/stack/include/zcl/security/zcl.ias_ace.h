/**
 * @file zcl.ias_ace.h
 * @heading IAS ACE
 * @brief ZCL Intruder Alarm Systems Ancillary Control Equipment cluster header
 * ZCL 7 section 8.3
 * ZCL 8 section 8.3
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      The public header file for the Home Automation
 *      clusters of the ZCL.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_IAS_ACE_H
#define ZCL_IAS_ACE_H

/* @PICS.ZCL.IASACE
 *
 * IASACE.S | Server | True
 * IASACE.C | Client | True
 *
 * Server Attributes
 * IASACE.S.Afffd | ClusterRevision | True
 * IASACE.S.Afffe | AttributeReportingStatus | True
 *
 * Commands Received
 * IASACE.S.C00.Rsp | Arm | True
 * IASACE.S.C01.Rsp | Bypass | True
 * IASACE.S.C02.Rsp | Emergency | True
 * IASACE.S.C03.Rsp | Fire | True
 * IASACE.S.C04.Rsp | Panic | True
 * IASACE.S.C05.Rsp | Get Zone ID Map | True
 * IASACE.S.C06.Rsp | Get Zone Information | True
 * IASACE.S.C07.Rsp | Get Panel Status | True
 * IASACE.S.C08.Rsp | Get Bypassed Zone List | True
 * IASACE.S.C09.Rsp | Get Zone Status | True
 *
 * Commands Generated
 * IASACE.S.C00.Tx | Arm Response | True
 * IASACE.S.C01.Tx | Get Zone ID Map Response | True
 * IASACE.S.C02.Tx | Get Zone Information Response | True
 * IASACE.S.C03.Tx | Zone Status Changed | True
 * IASACE.S.C04.Tx | Panel Status Changed | True
 * IASACE.S.C05.Tx | Get Panel Status Response | True
 * IASACE.S.C06.Tx | Set Bypassed Zone List | True
 * IASACE.S.C07.Tx | Bypass Response | True
 * IASACE.S.C08.Tx | Get Zone Status Response | True
 *
 * Client Attributes
 * IASACE.C.Afffd | ClusterRevision | True
 * IASACE.C.Afffe | AttributeReportingStatus | True
 *
 * Coomands Generated
 * IASACE.C.C00.Tx | Arm | True
 * IASACE.C.C01.Tx | Bypass | True
 * IASACE.C.C02.Tx | Emergency | True
 * IASACE.C.C03.Tx | Fire | True
 * IASACE.C.C04.Tx | Panic | True
 * IASACE.C.C05.Tx | Get Zone ID Map | True
 * IASACE.C.C06.Tx | Get Zone Information | True
 * IASACE.C.C07.Tx | Get Panel Status | True
 * IASACE.C.C08.Tx | Get Bypassed Zone List | True
 * IASACE.C.C09.Tx | Get Zone Status | True
 *
 * Commands Received
 * IASACE.C.C00.Rsp | Arm Response | True
 * IASACE.C.C01.Rsp | Get Zone ID Map Response | True
 * IASACE.C.C02.Rsp | Get Zone Information Response | True
 * IASACE.C.C03.Rsp | Zone Status Changed | True
 * IASACE.C.C04.Rsp | Panel Status Changed | True
 * IASACE.C.C05.Rsp | Get Panel Status Response | True
 * IASACE.C.C06.Rsp | Set Bypassed Zone List | True
 * IASACE.C.C07.Rsp | Bypass Response | True
 * IASACE.C.C08.Rsp | Get Zone Status Response | True
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

/** Arm command structure */
struct ZbZclIasAceClientCommandArmT {
    enum ZbZclIasAceArmModeT arm_mode; /**< Arm Mode */
    char arm_code[ZCL_IAS_ACE_ARM_CODE_STRING_MAX_LEN + 1U]; /**< Arm/Disarm Code */
    uint8_t zone_id; /**< Zone ID */
};

/* Arbitrary max. May want to convert to a list with a max of 255. */
#define ZCL_IAS_ACE_BYPASS_MAX_ZONES                 32

/** Bypass command structure */
struct ZbZclIasAceClientCommandBypassT {
    uint8_t num_zones; /**< Number of Zones */
    uint8_t zone_id_list[ZCL_IAS_ACE_BYPASS_MAX_ZONES]; /**< Zone ID List */
    char arm_code[ZCL_IAS_ACE_ARM_CODE_STRING_MAX_LEN + 1U]; /**< Arm/Disarm Code */
};

/** Get Zone Info command structure */
struct ZbZclIasAceClientCommandGetZoneInfoT {
    uint8_t zone_id; /**< Zone ID */
};

/** Get Zone Status command structure */
struct ZbZclIasAceClientCommandGetZoneStatusT {
    uint8_t starting_zone_id; /**< Starting Zone ID */
    uint8_t max_zone_ids; /**< Max Number of Zone IDs */
    uint8_t zone_status_mask_flag; /**< Zone Status Mask Flag */
    uint16_t zone_status_mask; /**< Zone Status Mask */
};

/*---------------------------------------------------------------
 * Server Command Structures
 *---------------------------------------------------------------
 */

/** Arm response structure */
struct ZbZclIasAceServerCommandArmRspT {
    enum ZbZclIasAceArmNotifyT arm_notify; /**< Arm Notification */
};

#define ZCL_IAS_ACE_ZONE_ID_MAP_NUM_SECTIONS         16

/** Get Zone ID Map response structure */
struct ZbZclIasAceServerCommandGetZoneIdMapRspT {
    uint16_t zond_id_map_list[ZCL_IAS_ACE_ZONE_ID_MAP_NUM_SECTIONS]; /**< Zone ID Map List */
};

/* Arbitrary max. "There is no minimum or maximum length to the Zone Label
 * field; however, the Zone Label SHOULD be between 16 to 24 alphanumeric
 * characters in length." */
#define ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN        32

/** Get Zone Info response structure */
struct ZbZclIasAceServerCommandGetZoneInfoRspT {
    uint8_t zone_id; /**< Zone ID */
    enum ZbZclIasZoneServerZoneTypeT zone_type; /**< Zone Type */
    uint64_t zone_addr; /**< IEEE Address */
    char zone_label[ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN + 1U]; /**< Zone Label */
};

/** Zone Status Changed command structure */
struct ZbZclIasAceServerCommandZoneStatusChangedT {
    uint8_t zone_id; /**< Zone ID */
    enum ZbZclIasZoneServerZoneStatusT zone_status; /**< Zone Status */
    enum ZbZclIasAceAudibleNotifyT audible_notify; /**< Audible Notification */
    char zone_label[ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN + 1U]; /**< Zone Label */
};

/** Get Panel Status response structure */
struct ZbZclIasAceServerCommandGetPanelStatusRspT {
    enum ZbZclIasAcePanelStatusT panel_status; /**< Panel Status */
    uint8_t seconds_remain; /**< Seconds Remaining */
    enum ZbZclIasAceAudibleNotifyT audible_notify; /**< Audible Notification */
    enum ZbZclIasAceAlarmStatusT alarm_status; /**< Alarm Status */
};

/** Set Bypassed Zone List command structure */
struct ZbZclIasAceServerCommandSetBypassedZoneListT {
    uint8_t num_zones; /**< Number of Zones */
    uint8_t zone_id_list[ZCL_IAS_ACE_BYPASS_MAX_ZONES]; /**< Zone ID List */
};

/** Bypass Response response structure */
struct ZbZclIasAceServerCommandBypassRspT {
    uint8_t num_zones; /**< Number of Zones */
    enum ZbZclIasAceBypassResultT bypass_result_list[ZCL_IAS_ACE_BYPASS_MAX_ZONES]; /**< Bypass Result for Zone ID List */
};

#define ZCL_IAS_ACE_ZONE_STATUS_MAX_ZONES            10

/** Get Zone Status response structure */
/* ZCL_IAS_ACE_SVR_CMD_GET_ZONE_STATUS_RSP */
struct ZbZclIasAceServerCommandGetZoneStatusRspT {
    uint8_t zone_status_complete; /**< Zone Status Complete */
    uint8_t num_zones; /**< Number of Zones */
    struct {
        uint8_t zone_id; /**< Zone ID */
        enum ZbZclIasZoneServerZoneStatusT zone_status; /**< Zone Status */
    } zone_list[ZCL_IAS_ACE_ZONE_STATUS_MAX_ZONES]; /**< Zone List */
};

/*---------------------------------------------------------------
 * Server API
 *---------------------------------------------------------------
 */

/** IAS ACE Server callbacks configuration */
struct ZbZclIasAceServerCallbacksT {
    /* The callback for Arming should handle arming the system and
     * calling ZbZclIasAceServerPanelStatusConfig when the panel status changes */
    bool (*arm_req)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclIasAceClientCommandArmT *arm_req,
        struct ZbZclIasAceServerCommandArmRspT *arm_rsp);
    /**< Callback to application, invoked on receipt of Arm command */

    void (*bypass_req)(struct ZbZclClusterT *clusterPtr, void *arg,
        struct ZbZclIasAceClientCommandBypassT *bypass_req,
        struct ZbZclIasAceServerCommandBypassRspT *bypass_rsp);
    /**< Callback to application, invoked on receipt of Bypass command */

    /* Emergency, Fire and Panic callbacks return a ZCL status to
     * return in the Default Response (e.g. ZCL_STATUS_SUCCESS) */
    enum ZclStatusCodeT (*emerg_req)(struct ZbZclClusterT *clusterPtr, void *arg, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Emergency command */
    enum ZclStatusCodeT (*fire_req)(struct ZbZclClusterT *clusterPtr, void *arg, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Fire command */
    enum ZclStatusCodeT (*panic_req)(struct ZbZclClusterT *clusterPtr, void *arg, struct ZbZclAddrInfoT *srcInfo);
    /**< Callback to application, invoked on receipt of Panic command */
};

/**
 * Create a new instance of the IAS ACE Server cluster
 *
 * If 'use_trip_pair' is true, application must call ZbZclIasAceServerEnrollRequest
 * to perform the 'trip-to-pair' process, unless the IAS CIE has sent us an
 * unsolicited Auto-Enroll-Response
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param callbacks Structure containing any callback function pointers for this cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIasAceServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    struct ZbZclIasAceServerCallbacksT *callbacks, void *arg);

/**
 * Change the Panel Arm/Disarm Code
 * @param cluster Cluster instance from which to send this command
 * @param arm_code Arm code
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerPanelCodeConfig(struct ZbZclClusterT *cluster,
    const char *arm_code);

/**
 * Update the Panel Status
 * @param cluster Cluster instance from which to send this command
 * @param panel_status Panel Status enumeration
 * @param seconds_remain Seconds remaining
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerPanelStatusConfig(struct ZbZclClusterT *cluster,
    enum ZbZclIasAcePanelStatusT panel_status, uint8_t seconds_remain,
    enum ZbZclIasAceAudibleNotifyT audible_notify);

/**
 * Returns the first free Zone ID not already in the Zone Table
 * @param cluster Cluster instance from which to send this command
 * @param zone_id_ptr Zone ID value
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerGetFreeZoneId(struct ZbZclClusterT *cluster, uint8_t *zone_id_ptr);

/** Zone Table Add request structure */
struct ZbZclIasAceServerZoneTableAddT {
    enum ZbZclIasZoneServerZoneTypeT zone_type; /**< Zone Type */
    uint64_t zone_addr; /**< Zone Address */
    const char *zone_label; /**< Zone Label - May be NULL, Max length = ZCL_IAS_ACE_ZONE_LABEL_STRING_MAX_LEN */
    uint8_t zone_id; /**< Zone ID - can use ZbZclIasAceServerGetFreeZoneId */
};

/**
 * Add new zone entry
 * @param cluster Cluster instance from which to send this command
 * @param req Zone Table Add request structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerZoneTableAdd(struct ZbZclClusterT *cluster,
    struct ZbZclIasAceServerZoneTableAddT *req);

/**
 * Delete a zone by zone ID
 * @param cluster Cluster instance from which to send this command
 * @param zone_id ID of zone to be deleted
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerZoneTableDeleteById(struct ZbZclClusterT *cluster, uint8_t zone_id);

/**
 * Delete a zone by address
 * @param cluster Cluster instance from which to send this command
 * @param addr Address of zone to be deleted
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerZoneTableDeleteByAddr(struct ZbZclClusterT *cluster, uint64_t addr);

/**
 * Returns address of paired zone, or 0 if not found
 * @param cluster Cluster instance from which to send this command
 * @param zone_id ID of zone to be returned
 * @return Address of zone if successful, 0 on error
 */
uint64_t ZbZclIasAceServerZoneTableAddrLookup(struct ZbZclClusterT *cluster, uint8_t zone_id);

/**
 * Attempts to find a zone based on extended address, and returns the zone Id if found
 * @param cluster Cluster instance from which to send this command
 * @param zone_addr Address of Zone being looked up
 * @param zone_id_ptr If successful, points to zone ID that was looked up
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerZoneTableIdLookup(struct ZbZclClusterT *cluster,
    uint64_t zone_addr, uint8_t *zone_id_ptr);

/**
 * Configure Zone Status
 * @param cluster Cluster instance from which to send this command
 * @param zone_id Zone ID of zone in question
 * @param zone_status Desired zone status
 * @param audible_notify Determines if zone notification is audible or not
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerZoneStatusConfig(struct ZbZclClusterT *cluster,
    uint8_t zone_id, enum ZbZclIasZoneServerZoneStatusT zone_status,
    enum ZbZclIasAceAudibleNotifyT audible_notify);

/**
 * Configure Bypass Permissions
 * @param cluster Cluster instance from which to send this command
 * @param zone_id Zone ID of zone in question
 * @param bypass_perms Desired bypass permissions
 * @return True on success, false otherwise
 */
bool ZbZclIasAceServerZoneBypassPerms(struct ZbZclClusterT *cluster,
    uint8_t zone_id, enum ZbZclIasAceBypassPermsT bypass_perms);

/**
 * Bypass zone if allowed
 * @param cluster Cluster instance from which to send this command
 * @param zone_id Zone ID of zone in question
 * @param bypass True if zone will be bypassed, false if not
 * @return Result of bypass config command
 */
enum ZbZclIasAceBypassResultT ZbZclIasAceServerZoneBypassConfig(struct ZbZclClusterT *cluster,
    uint8_t zone_id, bool bypass);

/*---------------------------------------------------------------
 * Client API
 *---------------------------------------------------------------
 */

/**
 * Allocate the IAS ACE Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclIasAceClientAlloc(struct ZigBeeT *zb, uint8_t endpoint, void *arg);

/* ARM */

/**
 * Send an Arm command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Arm command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandArmReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandArmT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Parse an Arm Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Arm response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseArmRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandArmRspT *rsp);

/* BYPASS */

/**
 * Send a Bypass command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Bypass command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandBypassReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandBypassT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Parse a Bypass Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Bypass Response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseBypassRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandBypassRspT *rsp);

/* Emergency / Fire / Panic */

/**
 * Send an Emergency command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandEmergencyReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Fire command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandFireReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Send a Panic command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandPanicReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/* Get Zone Id Map */

/**
 * Send a Get Zone ID Map command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandGetZoneIdMapReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Parse a Get Zone ID Map Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Get Zone ID Map Response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseGetZoneIdMapRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetZoneIdMapRspT *rsp);

/* Get Zone Info */

/**
 * Send a Get Zone Info command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Zone Info command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandGetZoneInfoReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandGetZoneInfoT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Parse a Get Zone Info Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Get Zone Info Response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseGetZoneInfoRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetZoneInfoRspT *rsp);

/* Zone / Panel Status Changed */

/**
 * Parse a Zone Status Changed Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Zone Status Changed Response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseZoneStatusChanged(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandZoneStatusChangedT *rsp);

/* Panel Status Changed command has same payload as Get Panel Status Response.
 * Use ZbZclIasAceClientParseGetPanelStatusRsp. */

/* Get Panel Status */

/**
 * Send a Get Panel Status changed request
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandGetPanelStatusReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Parse a Get Panel Status Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Zone Status Changed Response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseGetPanelStatusRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetPanelStatusRspT *rsp);

/* Get Bypassed Zone List */

/**
 * Send a Get Bypassed Zone List command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandGetBypassedZoneListReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Parse a Set Bypassed Zone List Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Set Bypassed Zone List Response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseSetBypassedZoneList(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandSetBypassedZoneListT *rsp);

/* Get Zone Status */

/**
 * Send a Get Zone Status command
 * @param cluster Cluster instance from which to send this command
 * @param dst Destination address for request
 * @param cmd_req Get Zone Status command request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
uint8_t ZbZclIasAceClientCommandGetZoneStatusReq(struct ZbZclClusterT *cluster,
    const struct ZbApsAddrT *dst, struct ZbZclIasAceClientCommandGetZoneStatusT *cmd_req,
    void (*callback)(struct ZbZclCommandRspT *rsp, void *arg), void *arg);

/**
 * Parse a Get Zone Status Response command payload into a data structure
 * @param buf Response buffer
 * @param len Length of response buffer
 * @param rsp Get Zone Status Response structure
 * @return True on success, false otherwise
 */
bool ZbZclIasAceClientParseGetZoneStatusRsp(const uint8_t *buf, unsigned int len,
    struct ZbZclIasAceServerCommandGetZoneStatusRspT *rsp);

#endif
