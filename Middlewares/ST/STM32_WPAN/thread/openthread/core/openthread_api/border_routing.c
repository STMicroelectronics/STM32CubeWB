/**
 ******************************************************************************
 * @file    border_routing.c
 * @author  MCD Application Team
 * @brief   This file contains the Border Routing interface shared between M0 and
 *          M4.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2022 STMicroelectronics.
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

#include "border_routing.h"

#if OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE

otError otBorderRoutingInit(otInstance *aInstance, uint32_t aInfraIfIndex, bool aInfraIfIsRunning)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_INIT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aInfraIfIndex;
  p_ot_req->Data[1] = (uint32_t) aInfraIfIsRunning;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otBorderRoutingSetEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otRoutePreference otBorderRoutingGetRouteInfoOptionPreference(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_GET_ROUTE_INFO_OPTION_PREFERENCE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otRoutePreference)p_ot_req->Data[0];
}

void otBorderRoutingSetRouteInfoOptionPreference(otInstance *aInstance, otRoutePreference aPreference)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_SET_ROUTE_INFO_OPTION_PREFERENCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPreference;

  Ot_Cmd_Transfer();
}

otError otBorderRoutingGetOmrPrefix(otInstance *aInstance, otIp6Prefix *aPrefix)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_GET_OMR_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPrefix;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otBorderRoutingGetFavoredOmrPrefix(otInstance *aInstance, otIp6Prefix *aPrefix, otRoutePreference *aPreference)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_GET_FAVORED_OMR_PREFIX;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aPrefix;
  p_ot_req->Data[1] = (uint32_t) aPreference;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otBorderRoutingGetOnLinkPrefix(otInstance *aInstance, otIp6Prefix *aPrefix)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_GET_ONLINK_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPrefix;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_CONFIG_BORDER_ROUTING_NAT64_ENABLE
otError otBorderRoutingGetNat64Prefix(otInstance *aInstance, otIp6Prefix *aPrefix)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_GET_NAT64_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPrefix;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

void otBorderRoutingPrefixTableInitIterator(otInstance *aInstance, otBorderRoutingPrefixTableIterator *aIterator)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_PREFIX_TABLE_INIT_INITIATOR;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIterator;

  Ot_Cmd_Transfer();
}

otError otBorderRoutingGetNextPrefixTableEntry(otInstance *                        aInstance,
                                               otBorderRoutingPrefixTableIterator *aIterator,
                                               otBorderRoutingPrefixTableEntry *   aEntry)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_GET_NEXT_PREFIX_TABLE_ENTRY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntry;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}


void otBorderRoutingClearRouteInfoOptionPreference(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_CLEAR_ROUTE_INFO_OPTION_PREFERENCE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();
}


otError otBorderRoutingGetFavoredNat64Prefix(otInstance        *aInstance,
                                             otIp6Prefix       *aPrefix,
                                             otRoutePreference *aPreference)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_ROUTING_GET_FAVORED_NAT_64_PREFIX;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aPrefix;
  p_ot_req->Data[1] = (uint32_t) aPreference;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#endif // OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE
