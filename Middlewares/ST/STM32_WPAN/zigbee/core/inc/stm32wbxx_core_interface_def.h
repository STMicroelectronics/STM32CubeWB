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
#include "stm32_wpan_common.h"

/* Structure of the messages exchanged between M0 and M4 */
#define OT_CMD_BUFFER_SIZE 20U

typedef PACKED_STRUCT {
    uint32_t ID;
    uint32_t Size;
    uint32_t Data[OT_CMD_BUFFER_SIZE];
} Zigbee_Cmd_Request_t;

/* List of messages between M4 and M0 */
typedef enum {
    MSG_M4TOM0_UNKNOWN = 0,
    MSG_M0TOM4_UNKNOWN,
    /* Query M0 to get secured memory information */
    MSG_M4TOM0_GET_SECURED_MEM_INFO,
    /* WPAN Interface (Virtual Testing Only) */
    MSG_M4TOM0_WPAN_INIT,
    MSG_M4TOM0_WPAN_DESTROY,
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
#ifndef CONFIG_ZB_M4_ZBTIMER_OVERRIDE
    /* ZbTimer API */
    MSG_M4TOM0_TIMER_ALLOC, /* ZbTimerAlloc */
    MSG_M4TOM0_TIMER_FREE, /* ZbTimerFree */
    MSG_M4TOM0_TIMER_STOP, /* ZbTimerStop */
    MSG_M4TOM0_TIMER_RESET, /* ZbTimerReset */
    MSG_M4TOM0_TIMER_RUNNING, /* ZbTimerRunning */
    MSG_M4TOM0_TIMER_REMAINING, /* ZbTimerRemaining */
    MSG_M0TOM4_TIMER_CB, /* callback from ZbTimerAlloc() */
#endif
    /* Stack Config and Startup */
    MSG_M4TOM0_STARTUP_GET_CFG, /* ZbStartupConfigGetProDefaults */
    MSG_M4TOM0_STARTUP_REQ, /* ZbStartup - Form or Join zigbee network */
    MSG_M0TOM4_STARTUP_CB, /* Callback for ZbStartup */
    MSG_M4TOM0_STARTUP_REJOIN, /* ZbStartupRejoin */
    MSG_M0TOM4_STARTUP_REJOIN_CB, /* Callback for ZbStartupRejoin */
    MSG_M4TOM0_STARTUP_PERSIST, /* ZbStartupPersist */
    MSG_M0TOM4_STARTUP_PERSIST_CB,
    MSG_M4TOM0_STARTUP_FINDBIND, /* ZbStartupFindBindStart */
    MSG_M0TOM4_STARTUP_FINDBIND_CB,
    MSG_M4TOM0_STARTUP_TOUCHLINK_TARGET_STOP, /* ZbStartupTouchlinkTargetStop */
    MSG_M4TOM0_STARTUP_TCSO_START, /* ZbStartupTcsoStart */
    MSG_M0TOM4_STARTUP_TCSO_CB,
    MSG_M4TOM0_STARTUP_TCSO_ABORT, /* ZbStartupTcsoAbort */
    MSG_M4TOM0_STARTUP_TC_REJOIN, /* ZbTrustCenterRejoin */
    MSG_M0TOM4_STARTUP_TC_REJOIN_CB,
    MSG_M4TOM0_PERSIST_ENABLE, /* ZbPersistNotifyRegister */
    MSG_M0TOM4_PERSIST_CB, /* Persistent data update notification callback */
    MSG_M4TOM0_PERSIST_GET, /* ZbPersistGet */
    /* ZbLeave / ZbReset */
    MSG_M4TOM0_ZB_LEAVE_REQ, /* ZbLeaveReq */
    MSG_M0TOM4_ZB_LEAVE_CB, /* Callback for ZbLeaveReq */
    MSG_M4TOM0_ZB_RESET_REQ, /* ZbReset */
    /* AES Hashing Functions */
    MSG_M4TOM0_ZB_HASH_ADD, /* ZbHashAdd */
    MSG_M4TOM0_ZB_HASH_DIGEST, /* ZbHashDigest */
    /* ECDSA Signature Validation */
    MSG_M4TOM0_ZB_SEC_ECDSA_VALIDATE, /* ZbSecEcdsaValidate */
    /* Touchlink Utility Commands */
    MSG_M4TOM0_ZCL_TL_GET_GRP_REQ, /* ZbZclTouchlinkInitiatorGetGrpIdReq */
    MSG_M0TOM4_ZCL_TL_GET_GRP_CB,
    MSG_M4TOM0_ZCL_TL_GET_EPLIST_REQ, /* ZbZclTouchlinkInitiatorGetEpListReq */
    MSG_M0TOM4_ZCL_TL_GET_EPLIST_CB,
    MSG_M4TOM0_ZCL_TL_SEND_EPINFO_REQ, /* ZbZclTouchlinkTargetSendEpInfoCmd */
    MSG_M0TOM4_ZCL_TL_SEND_EPINFO_CB,
    MSG_M0TOM4_ZCL_TL_EP_INFO_CB, /* ZbTouchlinkCallbacks.ep_info_cb() */
    /* Reserved to 0x00ff for Future Use */

    /* ZDO Commands */
    MSG_M4TOM0_ZDO_NWK_ADDR = 0x0100, /* ZbZdoNwkAddrReq */
    MSG_M0TOM4_ZDO_NWK_ADDR_CB,
    MSG_M4TOM0_ZDO_IEEE_ADDR, /* ZbZdoIeeeAddrReq */
    MSG_M0TOM4_ZDO_IEEE_ADDR_CB,
    MSG_M4TOM0_ZDO_NODE_DESC, /* ZbZdoNodeDescReq */
    MSG_M0TOM4_ZDO_NODE_DESC_CB,
    MSG_M4TOM0_ZDO_POWER_DESC, /* ZbZdoPowerDescReq */
    MSG_M0TOM4_ZDO_POWER_DESC_CB,
    MSG_M4TOM0_ZDO_SIMPLE_DESC, /* ZbZdoSimpleDescReq */
    MSG_M0TOM4_ZDO_SIMPLE_DESC_CB,
    MSG_M4TOM0_ZDO_ACTIVE_EP, /* ZbZdoActiveEpReq */
    MSG_M0TOM4_ZDO_ACTIVE_EP_CB,
    MSG_M4TOM0_ZDO_MATCH_DESC, /* ZbZdoMatchDescReq */
    MSG_M0TOM4_ZDO_MATCH_DESC_CB,
    MSG_M4TOM0_ZDO_MATCH_DESC_MULTI, /* ZbZdoMatchDescMulti (reusing a reserved ZDO cluster ID) */
    MSG_M0TOM4_ZDO_MATCH_DESC_MULTI_CB,
    MSG_M4TOM0_ZDO_DEVICE_ANNCE, /* ZbZdoDeviceAnnce */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_ALIAS, /* ZbZdoDeviceAnnceAlias (reusing a reserved ZDO cluster ID) */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_REG, /* ZbZdoDeviceAnnceFilterRegister */
    MSG_M4TOM0_ZDO_DEVICE_ANNCE_FILTER_DEL, /* ZbZdoDeviceAnnceFilterRemove */
    MSG_M0TOM4_ZDO_DEVICE_ANNCE_FILTER_CB,
    MSG_M4TOM0_ZDO_BIND, /* ZbZdoBindReq */
    MSG_M0TOM4_ZDO_BIND_CB,
    MSG_M4TOM0_ZDO_UNBIND, /* ZbZdoUnbindReq */
    MSG_M0TOM4_ZDO_UNBIND_CB,
    MSG_M4TOM0_ZDO_MGMT_LQI, /* ZbZdoLqiReq */
    MSG_M0TOM4_ZDO_MGMT_LQI_CB,
    MSG_M4TOM0_ZDO_MGMT_RTG, /* ZbZdoRtgReq */
    MSG_M0TOM4_ZDO_MGMT_RTG_CB,
    MSG_M4TOM0_ZDO_MGMT_BIND, /* ZbZdoMgmtBindReq */
    MSG_M0TOM4_ZDO_MGMT_BIND_CB,
    MSG_M4TOM0_ZDO_MGMT_LEAVE, /* ZbZdoLeaveReq */
    MSG_M0TOM4_ZDO_MGMT_LEAVE_CB,
    MSG_M4TOM0_ZDO_MGMT_PERMIT_JOIN, /* ZbZdoPermitJoinReq */
    MSG_M0TOM4_ZDO_MGMT_PERMIT_JOIN_CB,
    MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE, /* ZbZdoNwkUpdateReq */
    MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_CB,
#if 0 /* Not required for 2.4 GHz DUT */
    MSG_M4TOM0_ZDO_MGMT_NWK_ENH_UPDATE_REQ, /* ZbZdoNwkEnhUpdateReq */
    MSG_M4TOM0_ZDO_MGMT_NWK_IEEE_JOIN_REQ, /* ZbZdoNwkIeeeJoinListReq */
    MSG_M4TOM0_ZDO_MGMT_NWK_ENH_UPDATE_NOTIFY, /* ZbZdoUnsolicitedEnhUpdateNotify */
    MSG_M4TOM0_ZDO_MGMT_NWK_IEEE_JOIN_BCAST, /* ZbZdoNwkIeeeJoinListBcastAll */
#endif
    MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE_FILTER_REGISTER, /* ZbZdoNwkUpdateNotifyFilterRegister */
    MSG_M4TOM0_ZDO_MGMT_NWK_UPDATE_FILTER_REMOVE, /* ZbZdoNwkUpdateNotifyFilterRemove */
    MSG_M0TOM4_ZDO_MGMT_NWK_UPDATE_FILTER_CB,
    /* Reserved to 0x01ff Future Use */

    /* BDB */
    MSG_M4TOM0_BDB_GET_REQ = 0x0200, /* ZbBdbGetReq */
    MSG_M4TOM0_BDB_SET_REQ, /* ZbBdbSetReq */
    MSG_M4TOM0_BDB_SET_EP_STATUS, /* ZbBdbSetEndpointStatus */
    MSG_M4TOM0_BDB_GET_EP_STATUS, /* ZbBdbGetEndpointStatus */
    /* APSDE */
    MSG_M4TOM0_APSDE_DATA_REQ, /* ZbApsdeDataReqCallback */
    MSG_M0TOM4_APSDE_DATA_REQ_CB,
    /* APS Endpoint */
    MSG_M4TOM0_APS_FILTER_ENDPOINT_ADD, /* ZbApsFilterEndpointAdd */
    MSG_M0TOM4_APS_FILTER_ENDPOINT_CB,
    /* NoMatch filter callbacks go through MSG_M0TOM4_APS_FILTER_ENDPOINT_CB, since parameters are the same. */
    MSG_M4TOM0_APS_FILTER_CLUSTER_ADD, /* ZbApsFilterClusterAdd */
    MSG_M0TOM4_APS_FILTER_CLUSTER_CB,
    MSG_M4TOM0_APS_FILTER_REMOVE, /* ZbApsFilterEndpointFree */
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
    MSG_M0TOM4_NLME_NET_DISC_CB,
    MSG_M4TOM0_NLME_LEAVE, /* ZbNlmeLeaveReq */
    MSG_M0TOM4_NLME_LEAVE_CB,
    MSG_M4TOM0_NLME_PERMIT_JOIN, /* ZbNlmePermitJoinReq */
    MSG_M4TOM0_NLME_RESET, /* ZbNlmeResetReq */
    MSG_M4TOM0_NLME_SYNC, /* ZbNlmeSyncReq */
    MSG_M0TOM4_NLME_SYNC_CB,
    MSG_M4TOM0_NLME_ROUTE_DISC, /* ZbNlmeRouteDiscReq */
    MSG_M0TOM4_NLME_ROUTE_DISC_CB,
    /* NWK Misc and Helper API */
    MSG_M4TOM0_NWK_GET_SEC_MATERIAL, /* ZbNwkGetSecMaterial */
    MSG_M4TOM0_NWK_NNT_CLEAR_ALL, /* ZbNwkNeighborClearAll */
    MSG_M4TOM0_NWK_SET_FRAME_COUNTER, /* ZbNwkSetFrameCounter */
    MSG_M4TOM0_NWK_ADDR_LOOKUP_EXT, /* ZbNwkAddrLookupExt */
    MSG_M4TOM0_NWK_ADDR_LOOKUP_NWK, /* ZbNwkAddrLookupNwk */
    MSG_M4TOM0_NWK_FAST_POLL_REQUEST, /* ZbNwkFastPollRequest */
    MSG_M4TOM0_NWK_FAST_POLL_RELEASE, /* ZbNwkFastPollRelease */
    MSG_M4TOM0_NWK_FAST_POLL_RESOURCE, /* ZbNwkFastPollResourceCount */
    MSG_M4TOM0_NWK_EDKA_SEND, /* ZbNwkSendEdkaReq */
    MSG_M4TOM0_NWK_IFC_SET_TX_POWER, /* ZbNwkIfSetTxPower */
    MSG_M4TOM0_NWK_IFC_GET_TX_POWER, /* ZbNwkIfGetTxPower */
    MSG_M4TOM0_NWK_SET_NEXT_CHILD_ADDR, /* ZbNwkAddrSetNextChildAddr */
    MSG_M4TOM0_NWK_SET_COMM_INFO, /* ZbNwkCommissioningConfig */
    /* Reserved to 0x03ff for Future Use */

    /* ZCL Endpoint */
    MSG_M4TOM0_ZCL_ENDPOINT_ADD = 0x0400, /* ZbZclAddEndpoint */
    MSG_M4TOM0_ZCL_ENDPOINT_DEL, /* ZbZclRemoveEndpoint */
    /* ZCL Cluster ID register for discovery */
    MSG_M4TOM0_ZCL_CLUSTER_EP_REGISTER, /* ZbZclClusterEndpointRegister */
    MSG_M4TOM0_ZCL_CLUSTER_EP_REMOVE, /* ZbZclClusterEndpointRemove */
    /* ZCL Cluster binding and data indications */
    MSG_M4TOM0_ZCL_CLUSTER_BIND, /* ZbZclClusterBind */
    MSG_M0TOM4_ZCL_CLUSTER_DATA_IND,
    MSG_M4TOM0_ZCL_CLUSTER_UNBIND, /* ZbZclClusterUnbind */
    /* ZCL Cluster Alarms */
    MSG_M4TOM0_ZCL_CLUSTER_REVERSE_BIND, /* ZbZclClusterReverseBind */
    MSG_M4TOM0_ZCL_CLUSTER_REVERSE_UNBIND, /* ZbZclClusterReverseUnbind */
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_ADD_FILTER, /* ZbZclClusterRegisterAlarmResetHandler */
    MSG_M0TOM4_ZCL_CLUSTER_ALARM_CB,
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_REMOVE_FILTER, /* ZbZclClusterRemoveAlarmResetHandler */
    MSG_M4TOM0_ZCL_CLUSTER_ALARM_SEND, /* ZbZclClusterSendAlarm */
    /* ZCL Cluster Response Senders */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP, /* ZbZclClusterCommandRsp */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_CB, /* ZbZclClusterCommandRspWithCb */
    MSG_M0TOM4_ZCL_CLUSTER_CMD_RSP_CONF_CB, /* APSDE.confirm callback for ZbZclClusterCommandRspWithCb */
    MSG_M4TOM0_ZCL_CLUSTER_CMD_RSP_WITH_STATUS, /* ZbZclSendClusterStatusResponse */
    /* ZCL Profile-Wide Commands */
    MSG_M4TOM0_ZCL_GET_SEQNUM, /* ZbZclGetNextSeqnum */
    MSG_M4TOM0_ZCL_COMMAND_REQ, /* ZbZclCommandReq */
    MSG_M0TOM4_ZCL_COMMAND_REQ_CB,
    MSG_M4TOM0_ZCL_SEND_DEFAULT_RSP, /* ZbZclSendDefaultResponse */
    MSG_M4TOM0_ZCL_READ_REQ, /* ZbZclReadReq */
    MSG_M0TOM4_ZCL_READ_CB,
    MSG_M4TOM0_ZCL_WRITE_REQ, /* ZbZclWriteReq */
    MSG_M0TOM4_ZCL_WRITE_CB,
    MSG_M4TOM0_ZCL_DISCOVER_ATTR_REQ, /* ZbZclDiscoverAttrReq */
    MSG_M0TOM4_ZCL_DISCOVER_ATTR_CB,
    /* ZCL Key Exchange (i.e. CBKE) */
    MSG_M4TOM0_ZCL_KE_WITH_DEVICE, /* ZbZclKeWithDevice */
    MSG_M0TOM4_ZCL_KE_WITH_DEVICE_CB,
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
    MSG_M4TOM0_ZCL_BASIC_SERVER_CONFIG_DEFAULTS, /* ZbZclBasicServerConfigDefaults */
    /* ZCL Diagnostics Server */
    MSG_M4TOM0_ZCL_DIAG_SERVER_ALLOC, /* ZbZclDiagServerAlloc */
    /* Reserved to 0x04ff for Future Use */

    /* Reserved to 0x0fff for Future Use */

    /* Test Hooks */
    MSG_M4TOM0_TEST_CASE_CLEAR = 0x1000, /* ZbTestCaseClear */
    MSG_M4TOM0_TEST_CASE_CURRENT, /* ZbTestCaseCurrent */
    MSG_M4TOM0_TEST_CASE_DISABLE, /* ZbTestCaseDisable */
    MSG_M4TOM0_TEST_CASE_ENABLE, /* ZbTestCaseEnable */
    MSG_M4TOM0_APS_FRAG_DROP_ADD, /* ZbApsFragDropTxAdd */
    MSG_M4TOM0_APS_FRAG_DROP_CLEAR, /* ZbApsFragDropTxClear */
    /* Reserved to 0x10ff for Future Use */

    /* Raw MCP Messages */
    MSG_M4TOM0_WPAN_MCP_MSG = 0x1100,
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
