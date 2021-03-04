/**
 ******************************************************************************
 * @file    thread_ftd.c
 * @author  MCD Application Team
 * @brief   This file contains the OpenThread thread_ftd interface shared between
 *          M0 and M4.
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


/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"

/* Include definition of compilation flags requested for OpenThread configuration */
#include OPENTHREAD_CONFIG_FILE

#include "thread_ftd.h"

extern otNeighborTableCallback otNeighborTableCb;

#if OPENTHREAD_FTD

uint16_t otThreadGetMaxAllowedChildren(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_MAX_ALLOWED_CHILDREN;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

otError otThreadSetMaxAllowedChildren(otInstance *aInstance, uint16_t aMaxChildren)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_MAX_ALLOWED_CHILDREN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aMaxChildren;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otThreadIsRouterRoleEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_IS_ROUTER_ROLE_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otThreadSetRouterRoleEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_ROUTER_ROLE_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otThreadSetPreferredRouterId(otInstance *aInstance, uint8_t aRouterId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_PREFERRED_ROUTER_ID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aRouterId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint8_t otThreadGetLocalLeaderWeight(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_LOCAL_LEADER_WEIGHT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

void otThreadSetLocalLeaderWeight(otInstance *aInstance, uint8_t aWeight)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_LOCAL_LEADER_WEIGHT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aWeight;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint32_t otThreadGetLocalLeaderPartitionId(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_LOCAL_LEADER_PARTITION_ID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

void otThreadSetLocalLeaderPartitionId(otInstance *aInstance, uint32_t aPartitionId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_LOCAL_LEADER_PARTITION_ID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPartitionId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint16_t otThreadGetJoinerUdpPort(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_JOINER_UDP_PORT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

otError otThreadSetJoinerUdpPort(otInstance *aInstance, uint16_t aJoinerUdpPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_JOINER_UDP_PORT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aJoinerUdpPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otThreadSetSteeringData(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_STEERING_DATA;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint32_t otThreadGetContextIdReuseDelay(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_CONTEXT_ID_REUSE_DELAY;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

void otThreadSetContextIdReuseDelay(otInstance *aInstance, uint32_t aDelay)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_CONTEXT_ID_REUSE_DELAY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aDelay;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint8_t otThreadGetNetworkIdTimeout(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_NETWORK_ID_TIMEOUT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

void otThreadSetNetworkIdTimeout(otInstance *aInstance, uint8_t aTimeout)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_NETWORK_ID_TIMEOUT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aTimeout;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint8_t otThreadGetRouterUpgradeThreshold(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_ROUTER_UPGRADE_THRESHOLD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

void otThreadSetRouterUpgradeThreshold(otInstance *aInstance, uint8_t aThreshold)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_ROUTER_UPGRADE_THRESHOLD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aThreshold;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otThreadReleaseRouterId(otInstance *aInstance, uint8_t aRouterId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_RELEASE_ROUTER_ID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aRouterId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otThreadBecomeRouter(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_BECOME_ROUTER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otThreadBecomeLeader(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_BECOME_LEADER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint8_t otThreadGetRouterDowngradeThreshold(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_ROUTER_DOWNGRADE_THRESHOLD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

void otThreadSetRouterDowngradeThreshold(otInstance *aInstance, uint8_t aThreshold)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_ROUTER_DOWNGRADE_THRESHOLD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aThreshold;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint8_t otThreadGetRouterSelectionJitter(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_ROUTER_SELECTION_JITTER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

void otThreadSetRouterSelectionJitter(otInstance *aInstance, uint8_t aRouterJitter)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_ROUTER_SELECTION_JITTER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aRouterJitter;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otThreadGetChildInfoById(otInstance *aInstance, uint16_t aChildId, otChildInfo *aChildInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_CHILD_INFO_BY_ID;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aChildId;
  p_ot_req->Data[1] = (uint32_t)aChildInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otThreadGetChildInfoByIndex(otInstance *aInstance, uint16_t aChildIndex, otChildInfo *aChildInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_CHILD_INFO_BY_INDEX;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aChildIndex;
  p_ot_req->Data[1] = (uint32_t)aChildInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otThreadGetChildNextIp6Address(otInstance *               aInstance,
                                       uint16_t                   aChildIndex,
                                       otChildIp6AddressIterator *aIterator,
                                       otIp6Address *             aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_CHILD_NEXT_IP6_ADDRESS;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aChildIndex;
  p_ot_req->Data[1] = (uint32_t)aIterator;
  p_ot_req->Data[1] = (uint32_t)aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint8_t otThreadGetRouterIdSequence(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_ROUTER_ID_SEQUENCE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

uint8_t otThreadGetMaxRouterId(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_MAX_ROUTER_ID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

otError otThreadGetRouterInfo(otInstance *aInstance, uint16_t aRouterId, otRouterInfo *aRouterInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_ROUTER_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aRouterId;
  p_ot_req->Data[1] = (uint32_t)aRouterInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otThreadGetEidCacheEntry(otInstance *aInstance, uint8_t aIndex, otEidCacheEntry *aEntry)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_EID_CACHE_ENTRY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aIndex;
  p_ot_req->Data[1] = (uint32_t)aEntry;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

const otPskc *otThreadGetPskc(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_PSKC;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otPskc * )p_ot_req->Data[0];
}

otError otThreadSetPskc(otInstance *aInstance, const otPskc *aPskc)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_PSKC;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPskc;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

int8_t otThreadGetParentPriority(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_PARENT_PRIORITY;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (int8_t)p_ot_req->Data[0];
}

otError otThreadSetParentPriority(otInstance *aInstance, int8_t aParentPriority)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_PARENT_PRIORITY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aParentPriority;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otThreadRegisterNeighborTableCallback(otInstance *aInstance, otNeighborTableCallback aCallback)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otNeighborTableCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_REGISTER_NEIGHBOR_TABLE_CALLBACK;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#endif /* OPENTHREAD_FTD */
