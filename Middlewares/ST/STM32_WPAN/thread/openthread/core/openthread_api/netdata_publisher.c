/**
 ******************************************************************************
 * @file    netdata.c
 * @author  MCD Application Team
 * @brief   This file contains the netdata interface shared between M0 and
 *          M4.
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

#include "netdata_publisher.h"

#if OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE

extern otNetDataDnsSrpServicePublisherCallback otNetDataDnsSrpServicePublisherCb;

#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE

void otNetDataPublishDnsSrpServiceAnycast(otInstance *aInstance, uint8_t aSequenceNumber)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_PUBLISH_DNS_SRP_SERVICE_ANYCAST;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSequenceNumber;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otNetDataPublishDnsSrpServiceUnicast(otInstance *aInstance, const otIp6Address *aAddress, uint16_t aPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_PUBLISH_DNS_SRP_SERVICE_UNICAST;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aAddress;
  p_ot_req->Data[1] = (uint32_t) aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otNetDataPublishDnsSrpServiceUnicastMeshLocalEid(otInstance *aInstance, uint16_t aPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_PUBLISH_DNS_UNICAST_MESH_LOCAL_EID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

bool otNetDataIsDnsSrpServiceAdded(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_IS_DNS_SRP_SERVICE_ADDED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otNetDataSetDnsSrpServicePublisherCallback(otInstance *                            aInstance,
                                                otNetDataDnsSrpServicePublisherCallback aCallback,
                                                void *                                  aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otNetDataDnsSrpServicePublisherCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_SET_DNS_SRP_SERVICE_PUBLISHER_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otNetDataUnpublishDnsSrpService(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_UNPUBLISH_DNS_SRP_SERVICE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#endif // OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE

extern otNetDataPrefixPublisherCallback otNetDataPrefixPublisherCb;

otError otNetDataPublishOnMeshPrefix(otInstance *aInstance, const otBorderRouterConfig *aConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_PUBLISH_ON_MESH_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otNetDataPublishExternalRoute(otInstance *aInstance, const otExternalRouteConfig *aConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_PUBLISH_EXTERNAL_ROUTE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otNetDataIsPrefixAdded(otInstance *aInstance, const otIp6Prefix *aPrefix)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_IS_PREFIX_ADDED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPrefix;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otNetDataSetPrefixPublisherCallback(otInstance *                     aInstance,
                                         otNetDataPrefixPublisherCallback aCallback,
                                         void *                           aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otNetDataPrefixPublisherCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_SET_PREFIX_PUBLISHER_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otNetDataUnpublishPrefix(otInstance *aInstance, const otIp6Prefix *aPrefix)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_UNPUBLISH_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPrefix;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#endif // OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE

#endif // OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE
