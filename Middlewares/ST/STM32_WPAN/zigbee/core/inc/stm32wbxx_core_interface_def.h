/**
  ******************************************************************************
  * @file    stm32wbxx_core_interface_def.h
  * @author  MCD Application Team
  * @brief   This file contains all the defines and structures used for the
  *          communication between the two core M0 and M4.
  *          This file is shared between the code running on M4 and the code
  *          running on M0.
  *
  ******************************************************************************
  * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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

/* Structure of the messages exchanged between M0 and M4 */
#define OT_CMD_BUFFER_SIZE 20U

typedef struct {
    uint32_t ID;
    uint32_t Size;
    uint32_t Data[OT_CMD_BUFFER_SIZE];
} Zigbee_Cmd_Request_t;

/* List of messages between M4 and M0 */
typedef enum {
    MSG_M4TOM0_UNKNOWN = 0x0000,
    MSG_M0TOM4_UNKNOWN = 0x0000,

    /* WPAN Interface */
    MSG_M4TOM0_WPAN_SET_UINT32, /* wpan_set_uint32 */
    MSG_M4TOM0_WPAN_GET_UINT32, /* wpan_get_uint32 */
    MSG_M4TOM0_WPAN_GET_UINT64, /* wpan_get_uint64 */

    /* M0 Request (Separate IPCC Channel) */
    MSG_M0TOM4_ZB_LOGGING, /* Zigbee stack logging (configured by ZbSetLogging) */
    MSG_M0TOM4_ZB_MALLOC, /* ZbMalloc */
    MSG_M0TOM4_ZB_FREE, /* ZbFree */

    /* Stack Init / Destroy */
    MSG_M4TOM0_ZB_INIT, /* ZbInit */
    MSG_M4TOM0_ZB_DESTROY, /* ZbDestroy */

    /* Stack Misc */
    MSG_M4TOM0_EXTADDR_GET, /* ZbExtendedAddress */
    MSG_M4TOM0_EXTADDR_CHANGE, /* ZbChangeExtAddr */
    MSG_M4TOM0_FILTER_ADD, /* ZbMsgFilterRegister - Create filter in stack */
    MSG_M4TOM0_FILTER_DEL, /* ZbMsgFilterRemove - Delete filter in stack */
    MSG_M0TOM4_FILTER_MSG_CB, /* Callback for filter messages from stack */
    MSG_M4TOM0_LOG_CONFIG, /* ZbSetLogging */

    /* ZbTimer API */
    MSG_M4TOM0_TIMER_ALLOC, /* ZbTimerAlloc */
    MSG_M4TOM0_TIMER_FREE, /* ZbTimerFree */
    MSG_M4TOM0_TIMER_STOP, /* ZbTimerStop */
    MSG_M4TOM0_TIMER_RESET, /* ZbTimerReset */
    MSG_M4TOM0_TIMER_RUNNING, /* ZbTimerRunning */
    MSG_M4TOM0_TIMER_REMAINING, /* ZbTimerRemaining */
    MSG_M0TOM4_TIMER_CB = MSG_M4TOM0_TIMER_REMAINING, /* callback from ZbTimerAlloc() */
    /* Reserved to 0x001f for Future Use */

    /* Stack Config and Startup */
    MSG_M4TOM0_STARTUP_GET_CFG = 0x0020, /* ZbStartupConfigGetProDefaults */
    MSG_M4TOM0_STARTUP_REQ, /* ZbStartup - Form or Join zigbee network */
    MSG_M0TOM4_STARTUP_CB = MSG_M4TOM0_STARTUP_REQ, /* Callback for ZbStartup */
    MSG_M4TOM0_STARTUP_REJOIN, /* ZbStartupRejoin */
    MSG_M0TOM4_STARTUP_REJOIN_CB = MSG_M4TOM0_STARTUP_REJOIN, /* Callback for ZbStartupRejoin */

    MSG_M4TOM0_STARTUP_PERSIST, /* ZbStartupPersist */
    MSG_M4TOM0_STARTUP_FINDBIND, /* ZbStartupFindBindStart */
    MSG_M0TOM4_STARTUP_FINDBIND_CB = MSG_M4TOM0_STARTUP_FINDBIND,
    MSG_M4TOM0_STARTUP_TOUCHLINK_TARGET_STOP, /* ZbStartupTouchlinkTargetStop */
    MSG_M4TOM0_STARTUP_TCSO_START, /* ZbStartupTcsoStart */
    MSG_M0TOM4_STARTUP_TCSO_CB = MSG_M4TOM0_STARTUP_TCSO_START,
    MSG_M4TOM0_STARTUP_TCSO_ABORT, /* ZbStartupTcsoAbort */
    MSG_M4TOM0_STARTUP_TC_REJOIN, /* ZbTrustCenterRejoin */
    MSG_M0TOM4_STARTUP_TC_REJOIN_CB = MSG_M4TOM0_STARTUP_TC_REJOIN,
    MSG_M4TOM0_PERSIST_ENABLE, /* ZbPersistNotifyRegister */
    MSG_M0TOM4_PERSIST_CB, /* Persistent data update notification callback */
    MSG_M4TOM0_PERSIST_GET, /* ZbPersistGet */

    MSG_M4TOM0_ZB_LEAVE_REQ, /* ZbLeaveReq */
    MSG_M0TOM4_ZB_LEAVE_CB = MSG_M4TOM0_ZB_LEAVE_REQ, /* Callback for ZbLeaveReq */
    MSG_M4TOM0_ZB_RESET_REQ, /* ZbReset */

    /* AES Hashing Functions */
    MSG_M4TOM0_ZB_HASH_ADD, /* ZbHashAdd */
    MSG_M4TOM0_ZB_HASH_BYTE, /* ZbHashByte */
    MSG_M4TOM0_ZB_HASH_DIGEST, /* ZbHashDigest */

    /* ECDSA Signature Validation */
    MSG_M4TOM0_ZB_SEC_ECDSA_VALIDATE, /* ZbSecEcdsaValidate */
    /* Reserved to 0x003f for Future Use */

    /* BDB */
    MSG_M4TOM0_BDB_GET_REQ = 0x0040, /* ZbBdbGetReq */
    MSG_M4TOM0_BDB_SET_REQ, /* ZbBdbSetReq */
    MSG_M4TOM0_BDB_SET_EP_STATUS, /* ZbBdbSetEndpointStatus */
    MSG_M4TOM0_BDB_GET_EP_STATUS, /* ZbBdbGetEndpointStatus */
    /* Reserved to 0x004f for Future Use */

    /* ZDO Commands */
    MSG_M4TOM0_ZDO_NWK_ADDR = 0x0050, /* ZbZdoNwkAddrReq */
    MSG_M0TOM4_ZDO_NWK_ADDR_CB = MSG_M4TOM0_ZDO_NWK_ADDR,
    MSG_M4TOM0_ZDO_IEEE_ADDR = 0x0051, /* ZbZdoIeeeAddrReq */
    MSG_M0TOM4_ZDO_IEEE_ADDR_CB = MSG_M4TOM0_ZDO_IEEE_ADDR,
    MSG_M4TOM0_ZDO_NODE_DESC = 0x0052, /* ZbZdoNodeDescReq */
    MSG_M0TOM4_ZDO_NODE_DESC_CB = MSG_M4TOM0_ZDO_NODE_DESC,
    MSG_M4TOM0_ZDO_POWER_DESC = 0x0053, /* ZbZdoPowerDescReq */
    MSG_M0TOM4_ZDO_POWER_DESC_CB = MSG_M4TOM0_ZDO_POWER_DESC,
    MSG_M4TOM0_ZDO_SIMPLE_DESC = 0x0054, /* ZbZdoSimpleDescReq */
    MSG_M0TOM4_ZDO_SIMPLE_DESC_CB = MSG_M4TOM0_ZDO_SIMPLE_DESC,
    MSG_M4TOM0_ZDO_ACTIVE_EP = 0x0055, /* ZbZdoActiveEpReq */
    MSG_M0TOM4_ZDO_ACTIVE_EP_CB = MSG_M4TOM0_ZDO_ACTIVE_EP,
    MSG_M4TOM0_ZDO_MATCH_DESC = 0x0056, /* ZbZdoMatchDescReq */
    MSG_M0TOM4_ZDO_MATCH_DESC_CB = MSG_M4TOM0_ZDO_MATCH_DESC,
    MSG_M4TOM0_ZDO_MATCH_DESC_MULTI = 0x0057, /* ZbZdoMatchDescMulti (reusing a reserved ZDO cluster ID) */
    MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB = MSG_M4TOM0_ZDO_MATCH_DESC_MULTI,
    /* Reserved */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE = 0x0063, /* ZbZdoDeviceAnnce */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_ALIAS = 0x0064, /* ZbZdoDeviceAnnceAlias (reusing a reserved ZDO cluster ID) */
    /* Reserved */
    MSG_M4TOM0_ZDO_BIND = 0x0071, /* ZbZdoBindReq */
    MSG_M0TOM4_ZDO_BIND_CB = MSG_M4TOM0_ZDO_BIND,
    MSG_M4TOM0_ZDO_UNBIND = 0x0072, /* ZbZdoUnbindReq */
    MSG_M0TOM4_ZDO_UNBIND_CB = MSG_M4TOM0_ZDO_UNBIND,
    /* Reserved */
    MSG_M4TOM0_ZDO_MGMT_LQI = 0x0081, /* ZbZdoLqiReq */
    MSG_M0TOM4_ZDO_MGMT_LQI_CB = MSG_M4TOM0_ZDO_MGMT_LQI,
    /* Reserved */
    MSG_M4TOM0_ZDO_MGMT_BIND = 0x0083, /* ZbZdoMgmtBindReq */
    MSG_M0TOM4_ZDO_MGMT_BIND_CB = MSG_M4TOM0_ZDO_MGMT_BIND,
    MSG_M4TOM0_ZDO_MGMT_LEAVE = 0x0084, /* ZbZdoLeaveReq */
    MSG_M0TOM4_ZDO_MGMT_LEAVE_CB = MSG_M4TOM0_ZDO_MGMT_LEAVE,
    /* Reserved */
    MSG_M4TOM0_ZDO_MGMT_PERMIT_JOIN = 0x0086, /* ZbZdoPermitJoinReq */
    MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB = MSG_M4TOM0_ZDO_MGMT_PERMIT_JOIN,
    /* Reserved */
    MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE = 0x0088, /* ZbZdoNwkUpdateReq */
    MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB = MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE,
#if 0 /* Not required for 2.4 GHz DUT */
    MSG_M4TOM0_ZDO_MGMT_NWK_ENH_UPDATE_REQ = 0x0089, /* ZbZdoNwkEnhUpdateReq */
    MSG_M4TOM0_ZDO_MGMT_NWK_IEEE_JOIN_REQ = 0x008a, /* ZbZdoNwkIeeeJoinListReq */
    MSG_M4TOM0_ZDO_MGMT_NWK_ENH_UPDATE_NOTIFY = 0x008b, /* ZbZdoUnsolicitedEnhUpdateNotify */
    /* Reserved (0x008c to 0x00df) Future Use */
    MSG_M4TOM0_ZDO_MGMT_NWK_IEEE_JOIN_BCAST = 0x00e0, /* ZbZdoNwkIeeeJoinListBcastAll */
#endif
    /* Reserved (0x00e1 to 0x00ff) Future Use */

    /* ZDO Utility */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_REG = 0x0100, /* ZbZdoDeviceAnnceFilterRegister */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_DEL, /* ZbZdoDeviceAnnceFilterRemove */
    MSG_M0TOM4_ZDO_DEVICE_ANNCE_FILTER_CB,
    /* Reserved to 0x01ff for Future Use */

    /* APSDE */
    MSG_M4TOM0_APSDE_DATA_REQ = 0x0200, /* ZbApsdeDataReqCallback */
    MSG_M0TOM4_APSDE_DATA_REQ_CB = MSG_M4TOM0_APSDE_DATA_REQ,

    /* APS (Misc) */
    MSG_M4TOM0_APS_FILTER_ENDPOINT_ADD, /* ZbApsFilterEndpointAdd */
    MSG_M0TOM4_APS_FILTER_ENDPOINT_CB,
    MSG_M4TOM0_APS_FILTER_ENDPOINT_NOMATCH_ADD, /* ZbApsmeEndpointConfigNoMatchCallback */
    /* Can re-use MSG_M0TOM4_APS_FILTER_ENDPOINT_CB, since parameters are the same. */
    MSG_M4TOM0_APS_FILTER_CLUSTER_ADD, /* ZbApsFilterClusterAdd */
    MSG_M0TOM4_APS_FILTER_CLUSTER_CB,
    MSG_M4TOM0_APS_EP_CLUSTER_ID_ADD, /* ZbApsmeEndpointClusterListAppend */
    MSG_M4TOM0_APS_EP_EXISTS, /* ZbApsEndpointExists */
    MSG_M4TOM0_APS_EP_GET_PROFILE, /* ZbApsEndpointProfile */

    /* APSME */
    MSG_M4TOM0_APS_ENDPOINT_ADD, /* ZbApsmeAddEndpoint */
    MSG_M4TOM0_APS_ENDPOINT_DEL, /* ZbApsmeRemoveEndpoint */
    MSG_M4TOM0_APS_GET_REQ, /* ZbApsmeGetReq */
    MSG_M4TOM0_APS_SET_REQ, /* ZbApsmeSetReq */

    MSG_M4TOM0_APSME_ADD_GROUP, /* ZbApsmeAddGroupReq */
    MSG_M4TOM0_APSME_REMOVE_GROUP, /* ZbApsmeRemoveGroupReq */
    MSG_M4TOM0_APSME_REMOVE_ALL_GROUPS, /* ZbApsmeRemoveAllGroupsReq */

    MSG_M4TOM0_APSME_BIND, /* ZbApsmeBindReq */
    MSG_M4TOM0_APSME_UNBIND, /* ZbApsmeUnbindReq */
    MSG_M4TOM0_APS_UNBIND_ALL, /* ZbApsUnbindAllReq */

    MSG_M4TOM0_APSME_TRANSPORT_KEY, /* ZbApsmeTransportKeyReq */
    MSG_M4TOM0_APSME_REMOVE_DEVICE, /* ZbApsmeRemoveDeviceReq */
    MSG_M4TOM0_APSME_REQUEST_KEY, /* ZbApsmeRequestKeyReq */
    MSG_M4TOM0_APSME_SWITCH_KEY, /* ZbApsmeSwitchKeyReq */
    MSG_M4TOM0_APSME_ADD_KEY, /* ZbApsmeAddKeyReq */
    MSG_M4TOM0_APSME_GET_KEY, /* ZbApsmeGetKeyReq */
    MSG_M4TOM0_APSME_REMOVE_KEY, /* ZbApsmeRemoveKeyReq */

    /* Reserved to 0x02ff for Future Use */

    /* NLME */
    MSG_M4TOM0_NWK_GET_INDEX = 0x0300, /* ZbNwkGetIndex */
    MSG_M4TOM0_NWK_SET_INDEX, /* ZbNwkSetIndex */
    MSG_M4TOM0_NLME_DIRECT_JOIN, /* ZbNlmeDirectJoinReq */
    MSG_M4TOM0_NLME_SET_INTERFACE, /* ZbNlmeSetInterface */
    MSG_M4TOM0_NLME_GET_INTERFACE, /* ZbNlmeGetInterface */
    MSG_M4TOM0_NLME_NET_DISC, /* ZbNlmeNetDiscReq */
    MSG_M0TOM4_NLME_NET_DISC_CB = MSG_M4TOM0_NLME_NET_DISC,
    MSG_M4TOM0_NLME_LEAVE, /* ZbNlmeLeaveReq */
    MSG_M0TOM4_NLME_LEAVE_CB = MSG_M4TOM0_NLME_LEAVE,
    MSG_M4TOM0_NLME_PERMIT_JOIN, /* ZbNlmePermitJoinReq */
    MSG_M4TOM0_NLME_RESET, /* ZbNlmeResetReq */
    MSG_M4TOM0_NLME_SYNC, /* ZbNlmeSyncReq */
    MSG_M0TOM4_NLME_SYNC_CB = MSG_M4TOM0_NLME_SYNC,
    MSG_M4TOM0_NLME_ROUTE_DISC, /* ZbNlmeRouteDiscReq */
    MSG_M0TOM4_NLME_ROUTE_DISC_CB = MSG_M4TOM0_NLME_ROUTE_DISC,
    /* NWK Misc and Helper API */
    MSG_M4TOM0_NWK_GET_PARENT_EXT, /* ZbNwkGetParentExtAddr */
    MSG_M4TOM0_NWK_GET_PARENT_NWK, /* ZbNwkGetParentShortAddr */
    MSG_M4TOM0_NWK_GET_SEC_MATERIAL, /* ZbNwkGetSecMaterial */
    MSG_M4TOM0_NWK_NNT_CLEAR_ALL, /* ZbNwkNeighborClearAll */
    MSG_M4TOM0_NWK_SET_FRAME_COUNTER, /* ZbNwkSetFrameCounter */
    MSG_M4TOM0_NWK_ADDR_LOOKUP_EXT, /* ZbNwkAddrLookupExt */
    MSG_M4TOM0_NWK_ADDR_LOOKUP_NWK, /* ZbNwkAddrLookupNwk */
    MSG_M4TOM0_NWK_ADDR_IS_CHILD_EXT, /* ZbNwkAddrIsChildExt */
    MSG_M4TOM0_NWK_ADDR_IS_CHILD_NWK, /* ZbNwkAddrIsChildNwk */
    MSG_M4TOM0_NWK_FAST_POLL_REQUEST, /* ZbNwkFastPollRequest */
    MSG_M4TOM0_NWK_FAST_POLL_RELEASE, /* ZbNwkFastPollRelease */
    MSG_M4TOM0_NWK_FAST_POLL_RESOURCE, /* ZbNwkFastPollResourceCount */
    MSG_M4TOM0_NWK_IFC_SET_TX_POWER, /* ZbNwkIfSetTxPower */
    MSG_M4TOM0_NWK_IFC_GET_TX_POWER, /* ZbNwkIfGetTxPower */
    MSG_M4TOM0_NWK_IFC_SET_DSN, /* ZbNwkIfSetDsn */
    MSG_M4TOM0_NWK_SET_NEXT_CHILD_ADDR, /* ZbNwkAddrSetNextChildAddr */
    /* Reserved to 0x03ff for Future Use */

    /* ZCL Endpoint */
    MSG_M4TOM0_ZCL_ENDPOINT_ADD = 0x0400, /* ZbZclAddEndpoint */
    MSG_M4TOM0_ZCL_ENDPOINT_DEL, /* ZbZclRemoveEndpoint */

    /* ZCL Cluster ID register for discovery */
    MSG_M4TOM0_ZCL_CLUSTER_EP_REGISTER, /* ZbZclClusterEndpointRegister */
    MSG_M4TOM0_ZCL_CLUSTER_EP_REMOVE, /* ZbZclClusterEndpointRemove */

    /* ZCL Cluster binding and data indications */
    MSG_M4TOM0_ZCL_CLUSTER_BIND, /* ZbZclClusterBind */
    MSG_M0TOM4_ZCL_CLUSTER_DATA_IND = MSG_M4TOM0_ZCL_CLUSTER_BIND,
    MSG_M4TOM0_ZCL_CLUSTER_UNBIND, /* ZbZclClusterUnbind */

    MSG_M4TOM0_ZCL_CLUSTER_LOOPBACK_BIND, /* ZbZclClusterLoopbackBind */
    /* Data Indications are sent using MSG_M0TOM4_ZCL_CLUSTER_DATA_IND */
    MSG_M4TOM0_ZCL_CLUSTER_LOOPBACK_UNBIND, /* ZbZclClusterLoopbackUnbind */

    /* ZCL Cluster Alarms */
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_ADD_FILTER, /* ZbZclClusterRegisterAlarmResetHandler */
    MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB = MSG_M4TOM0_ZCL_CLUSTER_ALARM_ADD_FILTER,
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_REMOVE_FILTER, /* ZbZclClusterRemoveAlarmResetHandler */
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_SEND, /* ZbZclClusterSendAlarm */

    /* ZCL Cluster Command and Response */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_REQ, /* ZbZclClusterCommandReq */
    MSG_M0TOM4_ZCL_CLUSTER_CMD_REQ_CB = MSG_M4TOM0_ZCL_CLUSTER_CMD_REQ,
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP, /* ZbZclClusterCommandRsp */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_CB, /* ZbZclClusterCommandRspWithCb */
    MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB = MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_CB,
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_STATUS, /* ZbZclSendClusterStatusResponse */

    /* ZCL Profile-Wide Commands */
    MSG_M4TOM0_ZCL_GET_SEQNUM, /* ZbZclGetNextSeqnum */
    MSG_M4TOM0_ZCL_COMMAND_REQ, /* ZbZclCommandReq */
    MSG_M0TOM4_ZCL_COMMAND_REQ_CB = MSG_M4TOM0_ZCL_COMMAND_REQ,
    MSG_M4TOM0_ZCL_COMMAND_NO_RSP, /* ZbZclCommandNoResp */
    MSG_M0TOM4_ZCL_COMMAND_NO_RSP_CB = MSG_M4TOM0_ZCL_COMMAND_NO_RSP,
    MSG_M4TOM0_ZCL_SEND_DEFAULT_RSP, /* ZbZclSendDefaultResponse */
    MSG_M4TOM0_ZCL_READ_REQ, /* ZbZclReadReq */
    MSG_M0TOM4_ZCL_READ_CB = MSG_M4TOM0_ZCL_READ_REQ,
    MSG_M4TOM0_ZCL_WRITE_REQ, /* ZbZclWriteReq */
    MSG_M0TOM4_ZCL_WRITE_CB = MSG_M4TOM0_ZCL_WRITE_REQ,
    MSG_M4TOM0_ZCL_DISCOVER_ATTR_REQ, /* ZbZclDiscoverAttrReq */
    MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB = MSG_M4TOM0_ZCL_DISCOVER_ATTR_REQ,

    /* Reserved to 0x043f for Future Use */

    /* ZCL Cluster Specific APIs */

    /* ZCL Key Exchange (i.e. CBKE) */
    MSG_M4TOM0_ZCL_KE_WITH_DEVICE = 0x0440, /* ZbZclKeWithDevice */
    MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB = MSG_M4TOM0_ZCL_KE_WITH_DEVICE,

    /* ZCL Device Log */
#if 0 /* needed? */
    MSG_M4TOM0_ZCL_DEVICE_LOG_ADD, /* ZbZclDeviceLogAdd */
    MSG_M4TOM0_ZCL_DEVICE_LOG_CLEAR, /* ZbZclDeviceLogClear */
    MSG_M4TOM0_ZCL_DEVICE_LOG_ENABLE, /* ZbZclDeviceLogEnable */
    MSG_M4TOM0_ZCL_DEVICE_LOG_REMOVE, /* ZbZclDeviceLogRemove */
#endif
    MSG_M4TOM0_ZCL_DEVICE_LOG_CHECK, /* ZbZclDeviceLogCheckAllow */

    /* ZCL Basic Server */
    MSG_M4TOM0_ZCL_BASIC_SERVER_ALARM_POST, /* ZbZclBasicPostAlarm */
    MSG_M4TOM0_ZCL_BASIC_SERVER_CONFIG_RESET, /* ZbZclBasicServerResetCmdConfig */
    MSG_M4TOM0_ZCL_BASIC_SERVER_LOCAL_WRITE, /* ZbZclBasicWriteDirect */

    /* ZCL Diagnostics Server */
    MSG_M4TOM0_ZCL_DIAG_SERVER_ALLOC, /* ZbZclDiagnosticsServerAlloc */

    /* Reserved to 0x0fff for Future Use */

    /* Test Hooks */
    MSG_M4TOM0_TEST_CASE_CLEAR = 0x1000, /* ZbTestCaseClear */
    MSG_M4TOM0_TEST_CASE_CURRENT, /* ZbTestCaseCurrent */
    MSG_M4TOM0_TEST_CASE_DISABLE, /* ZbTestCaseDisable */
    MSG_M4TOM0_TEST_CASE_ENABLE, /* ZbTestCaseEnable */
    MSG_M4TOM0_APS_FRAG_DROP_ADD, /* ZbApsFragDropTxAdd */
    MSG_M4TOM0_APS_FRAG_DROP_CLEAR, /* ZbApsFragDropTxClear */

    /* Raw MCP Messages */
    MSG_M4TOM0_WPAN_MCP_MSG,
    MSG_M0TOM4_WPAN_MCP_MSG
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
