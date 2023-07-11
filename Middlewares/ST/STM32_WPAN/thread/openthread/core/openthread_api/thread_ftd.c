/**
 ******************************************************************************
 * @file    thread_ftd.c
 * @author  MCD Application Team
 * @brief   This file contains the OpenThread thread_ftd interface shared between
 *          M0 and M4.
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


/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"

/* Include definition of compilation flags requested for OpenThread configuration */
#include OPENTHREAD_CONFIG_FILE

#include "thread_ftd.h"

#if OPENTHREAD_FTD

extern otNeighborTableCallback otNeighborTableCb;
extern otThreadDiscoveryRequestCallback otThreadDiscoveryRequestCb;

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

uint8_t otThreadGetMaxChildIpAddresses(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_MAX_CHILD_IP_ADDRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
otError otThreadSetMaxChildIpAddresses(otInstance *aInstance, uint8_t aMaxIpAddresses)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_MAX_CHILD_IP_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aMaxIpAddresses;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

bool otThreadIsRouterEligible(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_IS_ROUTER_ELIGIBLE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otThreadSetRouterEligible(otInstance *aInstance, bool aEligible)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_ROUTER_ELIGIBLE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = aEligible;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
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

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
uint32_t otThreadGetPreferredLeaderPartitionId(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_PREFERRED_LEADER_PARTITION_ID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

void otThreadSetPreferredLeaderPartitionId(otInstance *aInstance, uint32_t aPartitionId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_PREFERRED_LEADER_PARTITION_ID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPartitionId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}
#endif

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
  p_ot_req->Data[2] = (uint32_t)aAddress;

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

otError otThreadGetNextCacheEntry(otInstance *aInstance, otCacheEntryInfo *aEntryInfo, otCacheEntryIterator *aIterator)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_NEXT_CACHE_ENTRY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aEntryInfo;
  p_ot_req->Data[1] = (uint32_t)aIterator;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otThreadGetPskc(otInstance *aInstance, otPskc *aPskc)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_PSKC;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPskc;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#if OPENTHREAD_CONFIG_PLATFORM_KEY_REFERENCES_ENABLE
otPskcRef otThreadGetPskcRef(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_GET_PSKC_REF;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otPskcRef)p_ot_req->Data[0];
}
#endif

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

#if OPENTHREAD_CONFIG_PLATFORM_KEY_REFERENCES_ENABLE
otError otThreadSetPskcRef(otInstance *aInstance, otPskcRef aKeyRef)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_FTD_SET_PSKC_REF;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aKeyRef;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

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

void otThreadSetDiscoveryRequestCallback(otInstance *                     aInstance,
                                         otThreadDiscoveryRequestCallback aCallback,
                                         void *                           aContext)
{
  Pre_OtCmdProcessing();

  otThreadDiscoveryRequestCb = aCallback;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_DISCOVERY_REQUEST_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
void otThreadSendAddressNotification(otInstance *              aInstance,
                                     otIp6Address *            aDestination,
                                     otIp6Address *            aTarget,
                                     otIp6InterfaceIdentifier *aMlIid)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SEND_ADDRESS_NOTIFICATION;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aDestination;
  p_ot_req->Data[1] = (uint32_t)aTarget;
  p_ot_req->Data[2] = (uint32_t)aMlIid;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#if OPENTHREAD_CONFIG_BACKBONE_ROUTER_DUA_NDPROXYING_ENABLE
otError otThreadSendProactiveBackboneNotification(otInstance *              aInstance,
                                                  otIp6Address *            aTarget,
                                                  otIp6InterfaceIdentifier *aMlIid,
                                                  uint32_t                  aTimeSinceLastTransaction)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SEND_PROACTIVE_BACKBONE_NOTIFICATION;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aTarget;
  p_ot_req->Data[1] = (uint32_t)aMlIid;
  p_ot_req->Data[2] = (uint32_t)aTimeSinceLastTransaction;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif // OPENTHREAD_CONFIG_BACKBONE_ROUTER_DUA_NDPROXYING_ENABLE

void otThreadSetCcmEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_CCM_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otThreadSetThreadVersionCheckEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_THREAD_VERSION_CHECK_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otThreadGetRouterIdRange(otInstance *aInstance, uint8_t *aMinRouterId, uint8_t *aMaxRouterId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_ROUTER_ID_RANGE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aMinRouterId;
  p_ot_req->Data[1] = (uint32_t)aMaxRouterId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otThreadSetRouterIdRange(otInstance *aInstance, uint8_t aMinRouterId, uint8_t aMaxRouterId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_ROUTER_ID_RANGE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aMinRouterId;
  p_ot_req->Data[1] = (uint32_t)aMaxRouterId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif // OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE

uint8_t otThreadGetChildRouterLinks(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_CHILD_ROUTER_LINKS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

otError otThreadSetChildRouterLinks(otInstance *aInstance, uint8_t aChildRouterLinks)
{
   Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_CHILD_ROUTER_LINKS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aChildRouterLinks;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otThreadGetNextHopAndPathCost(otInstance *aInstance,
                                   uint16_t    aDestRloc16,
                                   uint16_t   *aNextHopRloc16,
                                   uint8_t    *aPathCost)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_NEXT_HOP_AND_PAST_COST;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aDestRloc16;
  p_ot_req->Data[1] = (uint32_t)aNextHopRloc16;
  p_ot_req->Data[2] = (uint32_t)aPathCost;

  Ot_Cmd_Transfer();
}


#endif /* OPENTHREAD_FTD */
