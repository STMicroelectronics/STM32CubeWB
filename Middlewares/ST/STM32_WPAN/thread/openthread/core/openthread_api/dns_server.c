/**
 ******************************************************************************
 * @file    dns_server.c
 * @author  MCD Application Team
 * @brief   This file contains the DNS Server interface shared between M0 and
 *          M4.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

#include "dnssd_server.h"

#if OPENTHREAD_CONFIG_DNSSD_SERVER_ENABLE

extern otDnssdQuerySubscribeCallback otDnssdQuerySubscribeCb;
extern otDnssdQueryUnsubscribeCallback otDnssdQueryUnsubscribeCb;

void otDnssdQuerySetCallbacks(otInstance *                    aInstance,
                              otDnssdQuerySubscribeCallback   aSubscribe,
                              otDnssdQueryUnsubscribeCallback aUnsubscribe,
                              void *                          aContext)
{
  Pre_OtCmdProcessing();

  otDnssdQuerySubscribeCb = aSubscribe;
  otDnssdQueryUnsubscribeCb = aUnsubscribe;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNSSD_QUERY_SET_CALLBACKS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otDnssdQueryHandleDiscoveredServiceInstance(otInstance *                aInstance,
                                                 const char *                aServiceFullName,
                                                 otDnssdServiceInstanceInfo *aInstanceInfo)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNSSD_QUERY_HANDLE_DISCOVERED_SERVICE_INSTANCE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aServiceFullName;
  p_ot_req->Data[1] = (uint32_t) aInstanceInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otDnssdQueryHandleDiscoveredHost(otInstance *aInstance, const char *aHostFullName, otDnssdHostInfo *aHostInfo)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNSSD_QUERY_DISCOVERED_HOST;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHostFullName;
  p_ot_req->Data[1] = (uint32_t) aHostInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

const otDnssdQuery *otDnssdGetNextQuery(otInstance *aInstance, const otDnssdQuery *aQuery)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNSSD_GET_NEXT_QUERY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aQuery;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otDnssdQuery*)p_ot_req->Data[0];
}

otDnssdQueryType otDnssdGetQueryTypeAndName(const otDnssdQuery *aQuery, char (*aNameOutput)[OT_DNS_MAX_NAME_SIZE])
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNSSD_GET_QUERY_TYPE_AND_NAME;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aQuery;
  p_ot_req->Data[1] = (uint32_t) aNameOutput;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otDnssdQueryType)p_ot_req->Data[0];
}

const otDnssdCounters *otDnssdGetCounters(otInstance *aInstance)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNSSD_GET_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otDnssdCounters*)p_ot_req->Data[0];
}

#endif // OPENTHREAD_CONFIG_DNSSD_SERVER_ENABLE
