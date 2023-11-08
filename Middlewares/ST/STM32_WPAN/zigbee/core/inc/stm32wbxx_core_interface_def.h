/**
 ******************************************************************************
 * @file    stm32wbxx_core_interface_def.h
 * @author  MCD Application Team
 * @brief   This file contains all the defines and structures used for the
 *          communication between the two core M0 and M4 when using zigbee.
 *          This file is shared between the code running on M4 and the code
 *          running on M0.
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WBxx_CORE_INTERFACE_DEF_H
#define STM32WBxx_CORE_INTERFACE_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "zigbee_types.h"
#include "stm32wbxx_hal.h"
#include "stm32_wpan_common.h"

/* Structure of the messages exchanged between M0 and M4 */
#define OT_CMD_BUFFER_SIZE 20U

typedef PACKED_STRUCT {
    uint32_t ID;
    uint32_t Size;
    uint32_t Data[OT_CMD_BUFFER_SIZE];
} Zigbee_Cmd_Request_t;

/* List of messages between M4 and M0 */
/* /!\ New enum has to be insert a t the end of the list */
/* Not in the middle of it in order to insure inter-version compatibility /!\ */
typedef enum {
    MSG_M4TOM0_UNKNOWN = 0x0000,
    MSG_M0TOM4_UNKNOWN = 0x0001,
    /* Query M0 to get secured memory information */
    MSG_M4TOM0_GET_SECURED_MEM_INFO = 0x0002,
    MSG_M4TOM0_GET_ZB_HEAP_AVAILABLE = 0x0003, /* ZbHeapAvailable */
    /* WPAN Interface (Virtual Testing Only) */
    MSG_M4TOM0_WPAN_INIT = 0x0004,
    MSG_M4TOM0_WPAN_DESTROY = 0x0005,
    MSG_M4TOM0_WPAN_GET_UINT64 = 0x0006, /* wpan_get_uint64 */
    /* M0 Request (Separate IPCC Channel) */
    MSG_M0TOM4_ZB_LOGGING = 0x0007, /* Zigbee stack logging (configured by ZbSetLogging) */
    MSG_M0TOM4_ZB_MALLOC = 0x0008, /* ZbMalloc */
    MSG_M0TOM4_ZB_FREE = 0x0009, /* ZbFree */
    /* Stack Init / Destroy */
    MSG_M4TOM0_ZB_INIT = 0x000a, /* ZbInit */
    MSG_M4TOM0_ZB_DESTROY = 0x000b, /* ZbDestroy */
    MSG_M4TOM0_ZB_DESTROY_WITH_CB = 0x000c, /* ZbDestroyWithCb */
    MSG_M0TOM4_ZB_DESTROY_CB = 0x000d,
    /* Stack Misc */
    MSG_M4TOM0_EXTADDR_GET = 0x000e, /* ZbExtendedAddress */
    MSG_M4TOM0_EXTADDR_CHANGE = 0x000f, /* ZbChangeExtAddr */
    MSG_M4TOM0_FILTER_ADD = 0x0010, /* ZbMsgFilterRegister - Create filter in stack */
    MSG_M4TOM0_FILTER_DEL = 0x0011, /* ZbMsgFilterRemove - Delete filter in stack */
    MSG_M0TOM4_FILTER_MSG_CB = 0x0012, /* Callback for filter messages from stack */
    MSG_M4TOM0_LOG_CONFIG = 0x0013, /* ZbSetLogging */
    /* ZbTimer API */
    MSG_M4TOM0_TIMER_ALLOC = 0x0014, /* ZbTimerAlloc */
    MSG_M4TOM0_TIMER_FREE = 0x0015, /* ZbTimerFree */
    MSG_M4TOM0_TIMER_STOP = 0x0016, /* ZbTimerStop */
    MSG_M4TOM0_TIMER_RESET = 0x0017, /* ZbTimerReset */
    MSG_M4TOM0_TIMER_RUNNING = 0x0018, /* ZbTimerRunning */
    MSG_M4TOM0_TIMER_REMAINING = 0x0019, /* ZbTimerRemaining */
    MSG_M0TOM4_TIMER_CB = 0x001a, /* callback from ZbTimerAlloc() */
    /* Stack Config and Startup */
    MSG_M4TOM0_STARTUP_GET_CFG = 0x001b, /* ZbStartupConfigGetProDefaults */
    MSG_M4TOM0_STARTUP_REQ = 0x001c, /* ZbStartup - Form or Join zigbee network */
    MSG_M0TOM4_STARTUP_CB = 0x001d, /* Callback for ZbStartup */
    MSG_M4TOM0_STARTUP_REJOIN = 0x001e, /* ZbStartupRejoin */
    MSG_M0TOM4_STARTUP_REJOIN_CB = 0x001f, /* Callback for ZbStartupRejoin */
    MSG_M4TOM0_STARTUP_PERSIST = 0x0020, /* ZbStartupPersist */
    MSG_M0TOM4_STARTUP_PERSIST_CB = 0x0021,
    MSG_M4TOM0_STARTUP_FINDBIND = 0x0022, /* ZbStartupFindBindStart */
    MSG_M0TOM4_STARTUP_FINDBIND_CB = 0x0023,
    MSG_M4TOM0_STARTUP_TOUCHLINK_TARGET_STOP = 0x0024, /* ZbStartupTouchlinkTargetStop */
    MSG_M4TOM0_STARTUP_TCSO_START = 0x0025, /* ZbStartupTcsoStart */
    MSG_M0TOM4_STARTUP_TCSO_CB = 0x0026,
    MSG_M4TOM0_STARTUP_TCSO_ABORT = 0x0027, /* ZbStartupTcsoAbort */
    MSG_M4TOM0_STARTUP_TC_REJOIN = 0x0028, /* ZbTrustCenterRejoin */
    MSG_M0TOM4_STARTUP_TC_REJOIN_CB = 0x0029,
    MSG_M4TOM0_PERSIST_ENABLE = 0x002a, /* ZbPersistNotifyRegister */
    MSG_M0TOM4_PERSIST_CB = 0x002b, /* Persistent data update notification callback */
    MSG_M4TOM0_PERSIST_GET = 0x002c, /* ZbPersistGet */
    MSG_M4TOM0_STATE_GET = 0x002d, /* ZbStateGet */
    /* ZbLeave / ZbReset */
    MSG_M4TOM0_ZB_LEAVE_REQ = 0x002e, /* ZbLeaveReq */
    MSG_M0TOM4_ZB_LEAVE_CB = 0x002f, /* Callback for ZbLeaveReq */
    MSG_M4TOM0_ZB_RESET_REQ = 0x0030, /* ZbReset */
    /* Shutdown, Pause, Resume */
    MSG_M4TOM0_ZB_SHUTDOWN = 0x0031, /* ZbShutdown */
    MSG_M4TOM0_ZB_STATE_PAUSE = 0x0032, /* ZbStatePause */
    MSG_M0TOM4_ZB_STATE_PAUSE_CB = 0x0033,
    MSG_M4TOM0_ZB_STATE_RESUME = 0x0034, /* ZbStateResume */
    /* AES Hashing Functions */
    MSG_M4TOM0_ZB_HASH_ADD = 0x0035, /* ZbHashAdd */
    MSG_M4TOM0_ZB_HASH_DIGEST = 0x0036, /* ZbHashDigest */
    /* ECDSA Signature Validation */
    MSG_M4TOM0_ZB_SEC_ECDSA_VALIDATE = 0x0037, /* ZbSecEcdsaValidate */
    /* Touchlink Utility Commands */
    MSG_M4TOM0_ZCL_TL_GET_GRP_REQ = 0x0038, /* ZbZclTouchlinkInitiatorGetGrpIdReq */
    MSG_M0TOM4_ZCL_TL_GET_GRP_CB = 0x0039,
    MSG_M4TOM0_ZCL_TL_GET_EPLIST_REQ = 0x003a, /* ZbZclTouchlinkInitiatorGetEpListReq */
    MSG_M0TOM4_ZCL_TL_GET_EPLIST_CB = 0x003b,
    MSG_M4TOM0_ZCL_TL_SEND_EPINFO_REQ = 0x003c, /* ZbZclTouchlinkTargetSendEpInfoCmd */
    MSG_M0TOM4_ZCL_TL_SEND_EPINFO_CB = 0x003d,
    MSG_M0TOM4_ZCL_TL_EP_INFO_CB = 0x003e, /* ZbTouchlinkCallbacks.ep_info_cb() */

    /* AES CCM* mode functions for private use */
    MSG_M4TOM0_ZB_HASH_BYTE = 0x003f, /* ZbHashByte */
    MSG_M4TOM0_AES_CCM_TRANSFORM = 0x0040,
    MSG_M4TOM0_AES_CCM_AUTHENTICATE = 0x0041,
    /* Elliptic Curve (semi-private use) */
    MSG_M4TOM0_EC_MUL_X25519 = 0x0042, /* ec_mul_x25519 */
    MSG_M4TOM0_EC_MUL_P256 = 0x0043, /* ec_mul_p256 */
    /* SHA-256 functions (semi-private use) */
    MSG_M4TOM0_SHA256_RESET = 0x0044, /* SHA256Reset */
    MSG_M4TOM0_SHA256_INPUT = 0x0045, /* SHA256Input */
    MSG_M4TOM0_SHA256_RESULT = 0x0046, /* SHA256Result */
    /* HMAC SHA-256 functions (semi-private use) */
    MSG_M4TOM0_SHA256_HMAC_RESET = 0x0047, /* hmacReset */
    MSG_M4TOM0_SHA256_HMAC_INPUT = 0x0048, /* hmacInput */
    MSG_M4TOM0_SHA256_HMAC_RESULT = 0x0049, /* hmacResult */
    /* Reserved to 0x00ff for Future Use */

    /* ZDO Commands */
    MSG_M4TOM0_ZDO_NWK_ADDR = 0x0100, /* ZbZdoNwkAddrReq */
    MSG_M0TOM4_ZDO_NWK_ADDR_CB = 0x0101,
    MSG_M4TOM0_ZDO_IEEE_ADDR = 0x0102, /* ZbZdoIeeeAddrReq */
    MSG_M0TOM4_ZDO_IEEE_ADDR_CB = 0x0103,
    MSG_M4TOM0_ZDO_NODE_DESC = 0x0104, /* ZbZdoNodeDescReq */
    MSG_M0TOM4_ZDO_NODE_DESC_CB = 0x0105,
    MSG_M4TOM0_ZDO_POWER_DESC = 0x0106, /* ZbZdoPowerDescReq */
    MSG_M0TOM4_ZDO_POWER_DESC_CB = 0x0107,
    MSG_M4TOM0_ZDO_SIMPLE_DESC = 0x0108, /* ZbZdoSimpleDescReq */
    MSG_M0TOM4_ZDO_SIMPLE_DESC_CB = 0x0109,
    MSG_M4TOM0_ZDO_ACTIVE_EP = 0x010a, /* ZbZdoActiveEpReq */
    MSG_M0TOM4_ZDO_ACTIVE_EP_CB = 0x010b,
    MSG_M4TOM0_ZDO_MATCH_DESC = 0x010c, /* ZbZdoMatchDescReq */
    MSG_M0TOM4_ZDO_MATCH_DESC_CB = 0x010d,
    MSG_M4TOM0_ZDO_MATCH_DESC_MULTI = 0x010e, /* ZbZdoMatchDescMulti (reusing a reserved ZDO cluster ID) */
    MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB = 0x010f,
    MSG_M4TOM0_ZDO_DEVICE_ANNCE = 0x0110, /* ZbZdoDeviceAnnce */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_ALIAS = 0x0111, /* ZbZdoDeviceAnnceAlias (reusing a reserved ZDO cluster ID) */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_REG = 0x0112, /* ZbZdoDeviceAnnceFilterRegister */
    MSG_M0TOM4_ZDO_DEVICE_ANNCE_FILTER_CB = 0x0113,
    MSG_M4TOM0_ZDO_BIND = 0x0114, /* ZbZdoBindReq */
    MSG_M0TOM4_ZDO_BIND_CB = 0x0115,
    MSG_M4TOM0_ZDO_UNBIND = 0x0116, /* ZbZdoUnbindReq */
    MSG_M0TOM4_ZDO_UNBIND_CB = 0x0117,
    MSG_M4TOM0_ZDO_MGMT_LQI = 0x0118, /* ZbZdoLqiReq */
    MSG_M0TOM4_ZDO_MGMT_LQI_CB = 0x0119,
    MSG_M4TOM0_ZDO_MGMT_RTG = 0x011a, /* ZbZdoRtgReq */
    MSG_M0TOM4_ZDO_MGMT_RTG_CB = 0x011b,
    MSG_M4TOM0_ZDO_MGMT_BIND = 0x011c, /* ZbZdoMgmtBindReq */
    MSG_M0TOM4_ZDO_MGMT_BIND_CB = 0x011d,
    MSG_M4TOM0_ZDO_MGMT_LEAVE = 0x011e, /* ZbZdoLeaveReq */
    MSG_M0TOM4_ZDO_MGMT_LEAVE_CB = 0x011f,
    MSG_M4TOM0_ZDO_MGMT_PERMIT_JOIN = 0x0120, /* ZbZdoPermitJoinReq */
    MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB = 0x0121,
    MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE = 0x0122, /* ZbZdoNwkUpdateReq */
    MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB = 0x0123,
    MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE_FILTER_REGISTER = 0x0124, /* ZbZdoNwkUpdateNotifyFilterRegister */
    MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_FILTER_CB = 0x0125,
    MSG_M4TOM0_ZDO_FILTER_DEL = 0x0126, /* ZbZdoFilterRemove */
#if 0 /* Not required for 2.4 GHz DUT */
    MSG_M4TOM0_ZDO_MGMT_NWK_ENH_UPDATE_REQ =, /* ZbZdoNwkEnhUpdateReq */
    MSG_M4TOM0_ZDO_MGMT_NWK_IEEE_JOIN_REQ =, /* ZbZdoNwkIeeeJoinListReq */
    MSG_M4TOM0_ZDO_MGMT_NWK_ENH_UPDATE_NOTIFY =, /* ZbZdoUnsolicitedEnhUpdateNotify */
    MSG_M4TOM0_ZDO_MGMT_NWK_IEEE_JOIN_BCAST =, /* ZbZdoNwkIeeeJoinListBcastAll */
#endif
    /* Reserved to 0x01ff Future Use */

    /* BDB */
    MSG_M4TOM0_BDB_GET_REQ = 0x0200, /* ZbBdbGetReq */
    MSG_M4TOM0_BDB_SET_REQ = 0x0201, /* ZbBdbSetReq */
    MSG_M4TOM0_BDB_SET_EP_STATUS = 0x0202, /* ZbBdbSetEndpointStatus */
    MSG_M4TOM0_BDB_GET_EP_STATUS = 0x0203, /* ZbBdbGetEndpointStatus */
    /* APSDE */
    MSG_M4TOM0_APSDE_DATA_REQ = 0x0204, /* ZbApsdeDataReqCallback */
    MSG_M0TOM4_APSDE_DATA_REQ_CB = 0x0205,
    /* APS Endpoint */
    MSG_M4TOM0_APS_FILTER_ENDPOINT_ADD = 0x0206, /* ZbApsFilterEndpointAdd */
    MSG_M0TOM4_APS_FILTER_ENDPOINT_CB = 0x0207,
    /* NoMatch filter callbacks go through MSG_M0TOM4_APS_FILTER_ENDPOINT_CB, since parameters are the same. */
    MSG_M4TOM0_APS_FILTER_CLUSTER_ADD = 0x0208, /* ZbApsFilterClusterAdd */
    MSG_M0TOM4_APS_FILTER_CLUSTER_CB = 0x0209,
    MSG_M4TOM0_APS_FILTER_REMOVE = 0x020a, /* ZbApsFilterEndpointFree */
    MSG_M4TOM0_APS_EP_CLUSTER_ID_ADD = 0x020b, /* ZbApsmeEndpointClusterListAppend */
    MSG_M4TOM0_APS_EP_EXISTS = 0x020c, /* ZbApsEndpointExists */
    MSG_M4TOM0_APS_EP_GET_PROFILE = 0x020d, /* ZbApsEndpointProfile */
    /* APSME */
    MSG_M4TOM0_APS_ENDPOINT_ADD = 0x020e, /* ZbApsmeAddEndpoint */
    MSG_M4TOM0_APS_ENDPOINT_DEL = 0x020f, /* ZbApsmeRemoveEndpoint */
    MSG_M4TOM0_APS_GET_REQ = 0x0210, /* ZbApsmeGetReq */
    MSG_M4TOM0_APS_SET_REQ = 0x0211, /* ZbApsmeSetReq */
    MSG_M4TOM0_APSME_ADD_GROUP = 0x0212, /* ZbApsmeAddGroupReq */
    MSG_M4TOM0_APSME_REMOVE_GROUP = 0x0213, /* ZbApsmeRemoveGroupReq */
    MSG_M4TOM0_APSME_REMOVE_ALL_GROUPS = 0x0214, /* ZbApsmeRemoveAllGroupsReq */
    MSG_M4TOM0_APSME_BIND = 0x0215, /* ZbApsmeBindReq */
    MSG_M4TOM0_APSME_UNBIND = 0x0216, /* ZbApsmeUnbindReq */
    MSG_M4TOM0_APS_UNBIND_ALL = 0x0217, /* ZbApsUnbindAllReq */
    MSG_M4TOM0_APSME_TRANSPORT_KEY = 0x0218, /* ZbApsmeTransportKeyReq */
    MSG_M4TOM0_APSME_REMOVE_DEVICE = 0x0219, /* ZbApsmeRemoveDeviceReq */
    MSG_M4TOM0_APSME_REQUEST_KEY = 0x021a, /* ZbApsmeRequestKeyReq */
    MSG_M4TOM0_APSME_SWITCH_KEY = 0x021b, /* ZbApsmeSwitchKeyReq */
    MSG_M4TOM0_APSME_ADD_KEY = 0x021c, /* ZbApsmeAddKeyReq */
    MSG_M4TOM0_APSME_GET_KEY = 0x021d, /* ZbApsmeGetKeyReq */
    MSG_M4TOM0_APSME_REMOVE_KEY = 0x021e, /* ZbApsmeRemoveKeyReq */
    MSG_M4TOM0_APS_EP_CLUSTER_ID_DEL = 0x021f, /* ZbApsmeEndpointClusterListRemove */
    /* Reserved to 0x02ff for Future Use */

    /* NLME */
    MSG_M4TOM0_NWK_GET_INDEX = 0x0300, /* ZbNwkGetIndex */
    MSG_M4TOM0_NWK_SET_INDEX = 0x0301, /* ZbNwkSetIndex */
    MSG_M4TOM0_NLME_DIRECT_JOIN = 0x0302, /* ZbNlmeDirectJoinReq */
    MSG_M4TOM0_NLME_SET_INTERFACE = 0x0303, /* ZbNlmeSetInterface */
    MSG_M4TOM0_NLME_GET_INTERFACE = 0x0304, /* ZbNlmeGetInterface */
    MSG_M4TOM0_NLME_NET_DISC = 0x0305, /* ZbNlmeNetDiscReq */
    MSG_M0TOM4_NLME_NET_DISC_CB = 0x0306,
    MSG_M4TOM0_NLME_LEAVE = 0x0307, /* ZbNlmeLeaveReq */
    MSG_M0TOM4_NLME_LEAVE_CB = 0x0308,
    MSG_M4TOM0_NLME_PERMIT_JOIN = 0x0309, /* ZbNlmePermitJoinReq */
    MSG_M4TOM0_NLME_RESET = 0x030a, /* ZbNlmeResetReq */
    MSG_M4TOM0_NLME_SYNC = 0x030b, /* ZbNlmeSyncReq */
    MSG_M0TOM4_NLME_SYNC_CB = 0x030c,
    MSG_M4TOM0_NLME_ROUTE_DISC = 0x030d, /* ZbNlmeRouteDiscReq */
    MSG_M0TOM4_NLME_ROUTE_DISC_CB = 0x030e,
    /* NWK Misc and Helper API */
    MSG_M4TOM0_NWK_GET_SEC_MATERIAL = 0x030f, /* ZbNwkGetSecMaterial */
    MSG_M4TOM0_NWK_NNT_CLEAR_ALL = 0x0310, /* ZbNwkNeighborClearAll */
    MSG_M4TOM0_NWK_SET_FRAME_COUNTER = 0x0311, /* ZbNwkSetFrameCounter */
    MSG_M4TOM0_NWK_ADDR_LOOKUP_EXT = 0x0312, /* ZbNwkAddrLookupExt */
    MSG_M4TOM0_NWK_ADDR_LOOKUP_NWK = 0x0313, /* ZbNwkAddrLookupNwk */
    MSG_M4TOM0_NWK_FAST_POLL_REQUEST = 0x0314, /* ZbNwkFastPollRequest */
    MSG_M4TOM0_NWK_FAST_POLL_RELEASE = 0x0315, /* ZbNwkFastPollRelease */
    MSG_M4TOM0_NWK_FAST_POLL_RESOURCE = 0x0316, /* ZbNwkFastPollResourceCount */
    MSG_M4TOM0_NWK_EDKA_SEND = 0x0317, /* ZbNwkSendEdkaReq */
    MSG_M4TOM0_NWK_IFC_SET_TX_POWER = 0x0318, /* ZbNwkIfSetTxPower */
    MSG_M4TOM0_NWK_IFC_GET_TX_POWER = 0x0319, /* ZbNwkIfGetTxPower */
    MSG_M4TOM0_NWK_SET_NEXT_CHILD_ADDR = 0x031a, /* ZbNwkAddrSetNextChildAddr */
    MSG_M4TOM0_NWK_SET_COMM_INFO = 0x031b, /* ZbNwkCommissioningConfig */
    /* NLME-cont'd */
    MSG_M4TOM0_NLME_ED_SCAN = 0x031c, /* ZbNlmeEdScanReq */
    MSG_M0TOM4_NLME_ED_SCAN_CB = 0x031d,
    /* MAC Filtering */
    MSG_M4TOM0_NWK_MAC_FILTER_ADD = 0x031e, /* nwk_mac_filter_add */
    MSG_M4TOM0_NWK_MAC_FILTER_DEL = 0x031f, /* nwk_mac_filter_del */
    MSG_M4TOM0_NWK_MAC_FILTER_DEBUG = 0x0320, /* nwk_mac_filter_debug */
    /* Reserved to 0x03ff for Future Use */

    /* ZCL Endpoint */
    MSG_M4TOM0_ZCL_ENDPOINT_ADD = 0x0400, /* ZbZclAddEndpoint */
    MSG_M4TOM0_ZCL_ENDPOINT_DEL = 0x0401, /* ZbZclRemoveEndpoint */
    /* ZCL Cluster ID register for discovery */
    MSG_M4TOM0_ZCL_CLUSTER_EP_REGISTER = 0x0402, /* ZbZclClusterEndpointRegister */
    MSG_M4TOM0_ZCL_CLUSTER_EP_REMOVE = 0x0403, /* ZbZclClusterEndpointRemove */
    /* ZCL Cluster binding and data indications */
    MSG_M4TOM0_ZCL_CLUSTER_BIND = 0x0404, /* ZbZclClusterBind */
    MSG_M0TOM4_ZCL_CLUSTER_DATA_IND = 0x0405,
    MSG_M4TOM0_ZCL_CLUSTER_UNBIND = 0x0406, /* ZbZclClusterUnbind */
    /* ZCL Cluster Alarms */
    MSG_M4TOM0_ZCL_CLUSTER_REVERSE_BIND = 0x0407, /* ZbZclClusterReverseBind */
    MSG_M4TOM0_ZCL_CLUSTER_REVERSE_UNBIND = 0x0408, /* ZbZclClusterReverseUnbind */
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_ADD_FILTER = 0x0409, /* ZbZclClusterRegisterAlarmResetHandler */
    MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB = 0x040a,
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_REMOVE_FILTER = 0x040b, /* ZbZclClusterRemoveAlarmResetHandler */
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_SEND = 0x040c, /* ZbZclClusterSendAlarm */
    /* ZCL Cluster Response Senders */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP = 0x040d, /* ZbZclClusterCommandRsp */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_CB = 0x040e, /* ZbZclClusterCommandRspWithCb */
    MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB = 0x040f, /* APSDE.confirm callback for ZbZclClusterCommandRspWithCb */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_STATUS = 0x0410, /* ZbZclSendClusterStatusResponse */
    /* ZCL Profile-Wide Commands */
    MSG_M4TOM0_ZCL_GET_SEQNUM = 0x0411, /* ZbZclGetNextSeqnum */
    MSG_M4TOM0_ZCL_COMMAND_REQ = 0x0412, /* ZbZclCommandReq */
    MSG_M0TOM4_ZCL_COMMAND_REQ_CB = 0x0413,
    MSG_M4TOM0_ZCL_SEND_DEFAULT_RSP = 0x0414, /* ZbZclSendDefaultResponse */
    MSG_M4TOM0_ZCL_READ_REQ = 0x0415, /* ZbZclReadReq */
    MSG_M0TOM4_ZCL_READ_CB = 0x0416,
    MSG_M4TOM0_ZCL_WRITE_REQ = 0x0417, /* ZbZclWriteReq */
    MSG_M0TOM4_ZCL_WRITE_CB = 0x0418,
    MSG_M4TOM0_ZCL_DISCOVER_ATTR_REQ = 0x0419, /* ZbZclDiscoverAttrReq */
    MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB = 0x041a,
    /* ZCL Key Exchange (i.e. CBKE) */
    MSG_M4TOM0_ZCL_KE_WITH_DEVICE = 0x041b, /* ZbZclKeWithDevice */
    MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB = 0x041c,
    /* ZCL Device Log */
    MSG_M4TOM0_ZCL_DEVICE_LOG_CHECK = 0x041d, /* ZbZclDeviceLogCheckAllow */
    /* ZCL Basic Server */
    MSG_M4TOM0_ZCL_BASIC_SERVER_ALARM_POST = 0x041e, /* ZbZclBasicPostAlarm */
    MSG_M4TOM0_ZCL_BASIC_SERVER_CONFIG_RESET = 0x041f, /* ZbZclBasicServerResetCmdConfig */
    MSG_M4TOM0_ZCL_BASIC_SERVER_LOCAL_WRITE = 0x0420, /* ZbZclBasicWriteDirect */
    MSG_M4TOM0_ZCL_BASIC_SERVER_CONFIG_DEFAULTS = 0x0421, /* ZbZclBasicServerConfigDefaults */
    /* ZCL Diagnostics Server */
    MSG_M4TOM0_ZCL_DIAG_SERVER_ALLOC = 0x0422, /* ZbZclDiagServerAlloc */
#if 0 /* needed? */
      /* ZCL Device Log - Cont'd */
    MSG_M4TOM0_ZCL_DEVICE_LOG_ADD =, /* ZbZclDeviceLogAdd */
    MSG_M4TOM0_ZCL_DEVICE_LOG_CLEAR =, /* ZbZclDeviceLogClear */
    MSG_M4TOM0_ZCL_DEVICE_LOG_ENABLE =, /* ZbZclDeviceLogEnable */
    MSG_M4TOM0_ZCL_DEVICE_LOG_REMOVE =, /* ZbZclDeviceLogRemove */
#endif
    /* Reserved to 0x04ff for Future Use */

    /* Reserved to 0x0fff for Future Use */

    /* Test Hooks */
    MSG_M4TOM0_TEST_CASE_CLEAR = 0x1000, /* ZbTestCaseClear */
    MSG_M4TOM0_TEST_CASE_CURRENT = 0x1001, /* ZbTestCaseCurrent */
    MSG_M4TOM0_TEST_CASE_DISABLE = 0x1002, /* ZbTestCaseDisable */
    MSG_M4TOM0_TEST_CASE_ENABLE = 0x1003, /* ZbTestCaseEnable */
    MSG_M4TOM0_APS_FRAG_DROP_ADD = 0x1004, /* ZbApsFragDropTxAdd */
    MSG_M4TOM0_APS_FRAG_DROP_CLEAR = 0x1005, /* ZbApsFragDropTxClear */
    /* Reserved to 0x10ff for Future Use */

    /* Raw MCP Messages */
    MSG_M4TOM0_WPAN_MCP_MSG = 0x1100,
    MSG_M0TOM4_WPAN_MCP_MSG = 0x1101,

    /* Misc Debug */
    MSG_M4TOM0_DEBUG_MEMORY = 0x2000,
    MSG_M4TOM0_DEBUG_INFO = 0x2001,

    /* MAC */
    MSG_M4TOM0_MAC_SET_PROP_STRICT_DATA_POLL_REQ = 0x3000, /* MacSetPropStrictDataPollReq / g_MAC_PROP_STRICT_DATA_POLL_REQ_c */
    
    /* PHY */
    MSG_M4TOM0_PHY_TEST_MODE = 0x4000,
    MSG_M4TOM0_PHY_SET_CHANNEL = 0x4001,
    MSG_M4TOM0_PHY_CONTINUOUS_WAVE_START = 0x4002,
    MSG_M4TOM0_PHY_CONTINUOUS_WAVE_STOP = 0x4003,
    MSG_M4TOM0_PHY_TX_START = 0x4004,
    MSG_M4TOM0_PHY_RX_START = 0x4005,
    MSG_M4TOM0_PHY_RX_STOP = 0x4006,
} MsgId_Zigbee_Enum_t;

/* List of errors returned by the interface  */
typedef enum {
    ERR_INTERFACE_IPCC_INIT_FAIL = 100U,
    ERR_INTERFACE_IPCC_REMOTE_FAIL = 101U,
    ERR_INTERFACE_IPCC_SEND_ACK_TO_M0 = 102U,
} Error_Interface_Id_Enum_t;

HAL_StatusTypeDef Zigbee_CallBackProcessing(void);
HAL_StatusTypeDef Zigbee_M0RequestProcessing(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* STM32WBxx_CORE_INTERFACE_DEF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
