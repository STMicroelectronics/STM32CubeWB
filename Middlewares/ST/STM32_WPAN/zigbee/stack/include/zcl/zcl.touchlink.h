/**
 * @file zcl.touchlink.h
 * @heading Touchlink
 * @brief  Touchlink API
 * @copyright Copyright [2009 - 2021] Exegin Technologies Limited. All rights reserved.
 */

#ifndef ZCL_TOUCHLINK_H
#define ZCL_TOUCHLINK_H

/* @PICS.ZCL.Touchlink
 * TC.S | Server | True
 * TC.C | Client | True
 *
 * Server Attributes
 * TC.S.Afffd | ClusterRevision | True
 *
 * Commands Received
 * TC.S.C00.Rsp | Scan request | True
 * TC.S.C02.Rsp | Device information request | True
 * TC.S.C06.Rsp | Identify request | True
 * TC.S.C07.Rsp | Reset to factory new request | True
 * TC.S.C10.Rsp | Network start request | True
 * TC.S.C12.Rsp | Network join router request | True
 * TC.S.C14.Rsp | Network join end device request | True
 * TC.S.C16.Rsp | Network update request | True
 * TC.S.C41.Rsp | Get group identifiers request | True
 * TC.S.C42.Rsp | Get endpoint list request | True
 *
 * Commands Generated
 * TC.S.C01.Tx | Scan response | True
 * TC.S.C03.Tx | Device information response | True
 * TC.S.C11.Tx | Network start response | True
 * TC.S.C13.Tx | Network join router response | True
 * TC.S.C15.Tx | Network join end device response | True
 * TC.S.C40.Tx | Endpoint information | True
 * TC.S.C41.Tx | Get group identifiers response | True
 * TC.S.C42.Tx | Get endpoint list response | True
 *
 *
 * Client Attributes
 * TC.C.Afffd | ClusterRevision | True
 *
 * Commands Received
 * TC.C.C01.Rsp | Scan response | True
 * TC.C.C03.Rsp | Device information response | True
 * TC.C.C11.Rsp | Network start response | True
 * TC.C.C13.Rsp | Network join router response | True
 * TC.C.C15.Rsp | Network join end device response | True
 * TC.C.C40.Rsp | Endpoint information | True
 * TC.C.C41.Rsp | Get group identifiers response | True
 * TC.C.C42.Rsp | Get endpoint list response | True
 *
 * Commands Generated
 * TC.C.C00.Tx | Scan request | True
 * TC.C.C02.Tx | Device information request | True
 * TC.C.C06.Tx | Identify request | True
 * TC.C.C07.Tx | Reset to factory new request | True
 * TC.C.C10.Tx | Network start request | True
 * TC.C.C12.Tx | Network join router request | True
 * TC.C.C14.Tx | Network join end device request | True
 * TC.C.C16.Tx | Network update request | True
 * TC.C.C41.Tx | Get group identifiers request | True
 * TC.C.C42.Tx | Get endpoint list request | True
 */

#include "zcl/zcl.enum.h"

/* Zigbee Information Field
 * See Figure 13-10. Format of the ZigBee Information Field */
/* Logical Type */
#define ZCL_TL_ZBINFO_TYPE_MASK                     0x03U
#define ZCL_TL_ZBINFO_TYPE_COORD                    0x00U
#define ZCL_TL_ZBINFO_TYPE_ROUTER                   0x01U
#define ZCL_TL_ZBINFO_TYPE_END_DEVICE               0x02U
/* Rx on when idle */
#define ZCL_TL_ZBINFO_RX_ON_IDLE                    0x04U
/* Reserved */
#define ZCL_TL_ZBINFO_RESERVED                      0xf8U

/* Internal use flags (don't get sent over-the-air)
 * Developer Note: These used to be mixed with the ZCL_TL_ZBINFO_ flags, so for backward
 * compatibility make sure they don't conflict. */
#define ZCL_TL_FLAGS_IS_TARGET                      0x10U
#define ZCL_TL_FLAGS_USE_PERSIST                    0x20U
#define ZCL_TL_FLAGS_FACTORY_RESET                  0x40U

/*-----------------------------------------------------------------------------
 * Touchlink Utility Commands
 *-----------------------------------------------------------------------------
 */

/** Get Group Identifiers Request Command */
struct ZbTlGetGroupIdsReqCmd {
    uint8_t startIdx; /**< Start Index */
};

/** Get Endpoint List Request Command */
struct ZbTlGetEpListReqCmd {
    uint8_t startIdx; /**< Start Index */
};

/** Endpoint Information Command */
struct ZbTlEpInfoCmd {
    uint64_t ieeeAddr; /**< IEEE address */
    uint16_t nwkAddr; /**< Network address */
    uint8_t endpoint; /**< Endpoint identifier */
    uint16_t profileId; /**< Profile identifier */
    uint16_t deviceId; /**< Device identifier */
    uint8_t version; /**< Version */
};
#define ZCL_TL_MSG_EP_INFO_MIN_LEN          16U

/** Group Information Record List */
struct ZbTlGroupRecordList {
    uint16_t groupId; /**< Group list */
    uint8_t groupType; /**< Group type -  Must be 0 */
};

/* ZCL_PAYLOAD_UNFRAG_SAFE_SIZE = 54. Therefore: (54 - 3) / 3 = 17. */
#define ZCL_TL_GRP_IDS_LIST_MAX_LEN         17U

/** Get Group Identifiers Response Command */
struct ZbTlGetGroupIdsRspCmd {
    enum ZclStatusCodeT status; /**< ZCL status code */
    uint8_t total; /**< Total */
    uint8_t startIdx; /**< Start index */
    uint8_t count; /**< Count */
    struct ZbTlGroupRecordList record_list[ZCL_TL_GRP_IDS_LIST_MAX_LEN];
    /**< Group Information Record List */
};

/** Endpoint Information Record Entry */
struct ZbTlEndpointList {
    uint16_t nwkAddr; /**< Network address */
    uint8_t endpoint; /**< Endpoint identifier */
    uint16_t profileId; /**< Profile identifier */
    uint16_t deviceId; /**< Device identifier */
    uint8_t version; /**< Version */
};

/* ZCL_PAYLOAD_UNFRAG_SAFE_SIZE = 54. Therefore: (54 - 3) / 8 = 6.4 */
#define ZCL_TL_ENDPOINT_LIST_MAX_LEN        6U

/** Get Endpoint List Response Command */
struct ZbTlGetEpListRspCmd {
    enum ZclStatusCodeT status; /**< ZCL Status code */
    uint8_t total; /**< Total */
    uint8_t startIdx; /**< Start index */
    uint8_t count; /**< Count */
    struct ZbTlEndpointList endpoint_list[ZCL_TL_ENDPOINT_LIST_MAX_LEN]; /**< Endpoint Information Record Entry */
};

/** Zigbee Touchlink callback functions for Touchlink Controller Device Utility commands.
 * These are configured by "struct ZbStartupT" with ZbStartup().
 */
struct ZbTouchlinkCallbacks {
    enum ZclStatusCodeT (*ep_info_cb)(struct ZigBeeT *zb, struct ZbTlEpInfoCmd *cmd,
        struct ZbZclAddrInfoT *srcInfo, void *arg); /**< Endpoint Info callback */
};

/**
 * Sends a Touchlink Utility Get Group Identifiers Request Command
 * @param zb Zigbee stack instance
 * @param dst Destination address
 * @param cmd Get Group Identifiers Request structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTouchlinkInitiatorGetGrpIdReq(struct ZigBeeT *zb, struct ZbTlGetGroupIdsReqCmd *cmd,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbTlGetGroupIdsRspCmd *rsp, void *arg), void *arg);

/**
 * Sends a Touchlink Utility Get Endpoint List Request Command
 * @param zb Zigbee stack instance
 * @param dst Destination address
 * @param cmd Get Endpoint List Request Command structure
 * @param callback Callback function that will be invoked later when the response is received
 * @param arg Pointer to application data that will later be provided back to the callback functions when invoked
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTouchlinkInitiatorGetEpListReq(struct ZigBeeT *zb, struct ZbTlGetEpListReqCmd *cmd,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbTlGetEpListRspCmd *rsp, void *arg), void *arg);

/**
 * Sends a Touchlink Utility Endpoint Information Command
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint identifier. Must match the endpoint already registered with Touchlink
 * through the ZbStartup configuration, otherwise an error is returned.
 * @param dst Destination address
 * @param callback Callback function that will be invoked later when the response is received
 * @return ZCL_STATUS_SUCCESS if successful, or other ZclStatusCodeT value on error
 */
enum ZclStatusCodeT ZbZclTouchlinkTargetSendEpInfoCmd(struct ZigBeeT *zb, uint8_t endpoint,
    const struct ZbApsAddrT *dst, void (*callback)(struct ZbZclCommandRspT *zcl_rsp, void *arg), void *arg);

#endif
