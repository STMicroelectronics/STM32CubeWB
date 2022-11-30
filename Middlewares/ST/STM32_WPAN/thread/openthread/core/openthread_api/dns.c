/**
 ******************************************************************************
 * @file    dns.c
 * @author  MCD Application Team
 * @brief   This file contains the DNS interface shared between M0 and
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

#include "dns.h"
#include "dns_client.h"

void otDnsInitTxtEntryIterator(otDnsTxtEntryIterator *aIterator, const uint8_t *aTxtData, uint16_t aTxtDataLength)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_INIT_TX_ENTRY_ITERATOR;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aTxtData;
  p_ot_req->Data[2] = (uint32_t) aTxtDataLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otDnsGetNextTxtEntry(otDnsTxtEntryIterator *aIterator, otDnsTxtEntry *aEntry)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_GET_NEXT_TX_ENTRY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntry;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
void otDnsSetNameCompressionEnabled(bool aEnabled)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_SET_NAME_COMPRESSION_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

bool otDnsIsNameCompressionEnabled(void)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_IS_NAME_COMPRESSION_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}
#endif

#if OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE

extern otDnsAddressCallback otDnsAddressCb;
extern otDnsBrowseCallback otDnsBrowseCb;

const otDnsQueryConfig *otDnsClientGetDefaultConfig(otInstance *aInstance)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_CLIENT_GET_DEFAULT_CONFIG;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otDnsQueryConfig*)p_ot_req->Data[0];
}

void otDnsClientSetDefaultConfig(otInstance *aInstance, const otDnsQueryConfig *aConfig)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_CLIENT_SET_DEFAULT_CONFIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otDnsClientResolveAddress(otInstance *            aInstance,
                                  const char *            aHostName,
                                  otDnsAddressCallback    aCallback,
                                  void *                  aContext,
                                  const otDnsQueryConfig *aConfig)
{
  Pre_OtCmdProcessing();

  otDnsAddressCb = aCallback;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_CLIENT_RESOLVE_ADDRESS;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aHostName;
  p_ot_req->Data[1] = (uint32_t) aContext;
  p_ot_req->Data[2] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_CONFIG_DNS_CLIENT_NAT64_ENABLE
otError otDnsClientResolveIp4Address(otInstance *            aInstance,
                                     const char *            aHostName,
                                     otDnsAddressCallback    aCallback,
                                     void *                  aContext,
                                     const otDnsQueryConfig *aConfig)
{
  Pre_OtCmdProcessing();

  otDnsAddressCb = aCallback;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_CLIENT_NAT64_ENABLE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aHostName;
  p_ot_req->Data[1] = (uint32_t) aContext;
  p_ot_req->Data[2] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

otError otDnsAddressResponseGetHostName(const otDnsAddressResponse *aResponse,
                                        char *                      aNameBuffer,
                                        uint16_t                    aNameBufferSize)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_ADDRESS_RESPONSE_GET_HOST_NAME;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aNameBuffer;
  p_ot_req->Data[2] = (uint32_t) aNameBufferSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsAddressResponseGetAddress(const otDnsAddressResponse *aResponse,
                                       uint16_t                    aIndex,
                                       otIp6Address *              aAddress,
                                       uint32_t *                  aTtl)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_ADDRESS_RESPONSE_GET_ADDRESS;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aIndex;
  p_ot_req->Data[2] = (uint32_t) aAddress;
  p_ot_req->Data[3] = (uint32_t) aTtl;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_CONFIG_DNS_CLIENT_SERVICE_DISCOVERY_ENABLE

otError otDnsClientBrowse(otInstance *            aInstance,
                          const char *            aServiceName,
                          otDnsBrowseCallback     aCallback,
                          void *                  aContext,
                          const otDnsQueryConfig *aConfig)
{
  Pre_OtCmdProcessing();

  otDnsBrowseCb = aCallback;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_CLIENT_BROWSE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aServiceName;
  p_ot_req->Data[1] = (uint32_t) aContext;
  p_ot_req->Data[2] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsBrowseResponseGetServiceName(const otDnsBrowseResponse *aResponse,
                                          char *                     aNameBuffer,
                                          uint16_t                   aNameBufferSize)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BROWSE_RESPONSE_GET_SERVICE_NAME;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aNameBuffer;
  p_ot_req->Data[2] = (uint32_t) aNameBufferSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsBrowseResponseGetServiceInstance(const otDnsBrowseResponse *aResponse,
                                              uint16_t                   aIndex,
                                              char *                     aLabelBuffer,
                                              uint8_t                    aLabelBufferSize)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BROWSE_RESPONSE_GET_SERVICE_INSTANCE;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aIndex;
  p_ot_req->Data[2] = (uint32_t) aLabelBuffer;
  p_ot_req->Data[3] = (uint32_t) aLabelBufferSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsBrowseResponseGetServiceInfo(const otDnsBrowseResponse *aResponse,
                                          const char *               aInstanceLabel,
                                          otDnsServiceInfo *         aServiceInfo)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BROWSE_RESPONSE_GET_SERVICE_INFO;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aInstanceLabel;
  p_ot_req->Data[2] = (uint32_t) aServiceInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsBrowseResponseGetHostAddress(const otDnsBrowseResponse *aResponse,
                                          const char *               aHostName,
                                          uint16_t                   aIndex,
                                          otIp6Address *             aAddress,
                                          uint32_t *                 aTtl)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BROWSE_RESPONSE_GET_HOST_ADDRESS;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aHostName;
  p_ot_req->Data[2] = (uint32_t) aIndex;
  p_ot_req->Data[3] = (uint32_t) aAddress;
  p_ot_req->Data[4] = (uint32_t) aTtl;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

extern otDnsServiceCallback otDnsServiceCb;

otError otDnsClientResolveService(otInstance *            aInstance,
                                  const char *            aInstanceLabel,
                                  const char *            aServiceName,
                                  otDnsServiceCallback    aCallback,
                                  void *                  aContext,
                                  const otDnsQueryConfig *aConfig)
{
  Pre_OtCmdProcessing();

  otDnsServiceCb = aCallback;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_CLIENT_RESOLVE_SERVICE;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aInstanceLabel;
  p_ot_req->Data[1] = (uint32_t) aServiceName;
  p_ot_req->Data[2] = (uint32_t) aContext;
  p_ot_req->Data[3] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsServiceResponseGetServiceName(const otDnsServiceResponse *aResponse,
                                           char *                      aLabelBuffer,
                                           uint8_t                     aLabelBufferSize,
                                           char *                      aNameBuffer,
                                           uint16_t                    aNameBufferSize)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_SERVICE_RESPONSE_GET_SERVICE_NAME;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aLabelBuffer;
  p_ot_req->Data[2] = (uint32_t) aLabelBufferSize;
  p_ot_req->Data[3] = (uint32_t) aNameBuffer;
  p_ot_req->Data[4] = (uint32_t) aNameBufferSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsServiceResponseGetServiceInfo(const otDnsServiceResponse *aResponse, otDnsServiceInfo *aServiceInfo)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_SERVICE_RESPONSE_GET_SERVICE_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aServiceInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDnsServiceResponseGetHostAddress(const otDnsServiceResponse *aResponse,
                                           const char *                aHostName,
                                           uint16_t                    aIndex,
                                           otIp6Address *              aAddress,
                                           uint32_t *                  aTtl)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DNS_SERVICE_RESPONSE_GET_HOST_ADDRESS;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aHostName;
  p_ot_req->Data[2] = (uint32_t) aIndex;
  p_ot_req->Data[3] = (uint32_t) aAddress;
  p_ot_req->Data[4] = (uint32_t) aTtl;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#endif // OPENTHREAD_CONFIG_DNS_CLIENT_SERVICE_DISCOVERY_ENABLE

#endif // OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE
