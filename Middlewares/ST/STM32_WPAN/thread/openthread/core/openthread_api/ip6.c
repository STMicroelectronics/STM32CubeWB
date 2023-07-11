/**
 ******************************************************************************
 * @file    ip6.c
 * @author  MCD Application Team
 * @brief   This file contains the IPv6 interface shared between M0 and
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

#include "ip6.h"

#if OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
extern otIp6SlaacPrefixFilter otIp6SlaacPrefixFilterCb;
#endif // OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
extern otIp6ReceiveCallback otIp6ReceiveCb;
extern otIp6AddressCallback otIp6AddressCb;

#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_TMF_PROXY_MLR_ENABLE && OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
extern otIp6RegisterMulticastListenersCallback otIp6RegisterMulticastListenersCb;
#endif // OPENTHREAD_FTD && OPENTHREAD_CONFIG_TMF_PROXY_MLR_ENABLE && OPENTHREAD_CONFIG_COMMISSIONER_ENABLE

otError otIp6SetEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otIp6IsEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_IS_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otIp6AddUnicastAddress(otInstance *aInstance, const otNetifAddress *aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_ADD_UNICAST_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otIp6RemoveUnicastAddress(otInstance *aInstance, const otIp6Address *aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_REMOVE_UNICAST_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

const otNetifAddress *otIp6GetUnicastAddresses(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_GET_UNICAST_ADDRESSES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otNetifAddress *)p_ot_req->Data[0];
}

otError otIp6SubscribeMulticastAddress(otInstance *aInstance, const otIp6Address *aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SUBSCRIBE_MULTICAST_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otIp6UnsubscribeMulticastAddress(otInstance *aInstance, const otIp6Address *aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_UNSUBSCRIBE_MULTICAST_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

const otNetifMulticastAddress *otIp6GetMulticastAddresses(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_GET_MULTICAST_ADDRESSES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otNetifMulticastAddress *)p_ot_req->Data[0];
}

bool otIp6IsMulticastPromiscuousEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_IS_MULTICAST_PROMISCUOUS_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otIp6SetMulticastPromiscuousEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_MULTICAST_PROMISCUOUS_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otMessage *otIp6NewMessage(otInstance *aInstance, const otMessageSettings *aSettings)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_NEW_MESSAGE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aSettings;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otMessage *)p_ot_req->Data[0];
}

otMessage *otIp6NewMessageFromBuffer(otInstance *             aInstance,
                                     const uint8_t *          aData,
                                     uint16_t                 aDataLength,
                                     const otMessageSettings *aSettings)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_NEW_MESSAGE_FROM_BUFFER;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aData;
  p_ot_req->Data[1] = (uint32_t)aDataLength;
  p_ot_req->Data[2] = (uint32_t)aSettings;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otMessage *)p_ot_req->Data[0];
}

void otIp6SetReceiveCallback(otInstance *aInstance, otIp6ReceiveCallback aCallback, void *aCallbackContext)
{
  Pre_OtCmdProcessing();
  otIp6ReceiveCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_RECEIVE_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otIp6SetAddressCallback(otInstance *aInstance, otIp6AddressCallback aCallback, void *aCallbackContext)
{
  Pre_OtCmdProcessing();
  otIp6AddressCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_ADDRESS_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

bool otIp6IsReceiveFilterEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_IS_RECEIVE_FILTER_ENABLED;
  p_ot_req->Size=0;
  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otIp6SetReceiveFilterEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_RECEIVE_FILTER_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otIp6Send(otInstance *aInstance, otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SEND;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otIp6AddUnsecurePort(otInstance *aInstance, uint16_t aPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_ADD_UNSECURE_PORT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otIp6RemoveUnsecurePort(otInstance *aInstance, uint16_t aPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_REMOVE_UNSECURE_PORT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otIp6RemoveAllUnsecurePorts(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_REMOVE_ALL_UNSECURE_PORTS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();
}

const uint16_t *otIp6GetUnsecurePorts(otInstance *aInstance, uint8_t *aNumEntries)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_GET_UNSECURE_PORTS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aNumEntries;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t *)p_ot_req->Data[0];
}

bool otIp6IsAddressEqual(const otIp6Address *aFirst, const otIp6Address *aSecond)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_IS_ADDRESS_EQUAL;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aFirst;
  p_ot_req->Data[1] = (uint32_t)aSecond;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

bool otIp6ArePrefixesEqual(const otIp6Prefix *aFirst, const otIp6Prefix *aSecond)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_ARE_PREFIXES_EQUAL;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aFirst;
  p_ot_req->Data[1] = (uint32_t)aSecond;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otIp6AddressFromString(const char *aString, otIp6Address *aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_ADDRESS_FROM_STRING;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aString;
  p_ot_req->Data[1] = (uint32_t) aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otIp6AddressToString(const otIp6Address *aAddress, char *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_ADDRESS_TO_STRING;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aAddress;
  p_ot_req->Data[1] = (uint32_t) aBuffer;
  p_ot_req->Data[2] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otIp6SockAddrToString(const otSockAddr *aSockAddr, char *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SOCK_ADDR_TO_STRING;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aSockAddr;
  p_ot_req->Data[1] = (uint32_t) aBuffer;
  p_ot_req->Data[2] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otIp6PrefixToString(const otIp6Prefix *aPrefix, char *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_PREFIX_TO_STRING;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aPrefix;
  p_ot_req->Data[1] = (uint32_t) aBuffer;
  p_ot_req->Data[2] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint8_t otIp6PrefixMatch(const otIp6Address *aFirst, const otIp6Address *aSecond)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_PREFIX_MATCH;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aFirst;
  p_ot_req->Data[1] = (uint32_t) aSecond;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

bool otIp6IsAddressUnspecified(const otIp6Address *aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_IS_ADDRESS_UNSPECIFIED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otIp6SelectSourceAddress(otInstance *aInstance, otMessageInfo *aMessageInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SELECT_SOURCE_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessageInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_TMF_PROXY_MLR_ENABLE && OPENTHREAD_CONFIG_COMMISSIONER_ENABLE
otError otIp6RegisterMulticastListeners(otInstance *                            aInstance,
                                        const otIp6Address *                    aAddresses,
                                        uint8_t                                 aAddressNum,
                                        const uint32_t *                        aTimeout,
                                        otIp6RegisterMulticastListenersCallback aCallback,
                                        void *                                  aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otIp6RegisterMulticastListenersCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_REGISTER_MULTICAST_LISTENERS;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aAddresses;
  p_ot_req->Data[1] = (uint32_t) aAddressNum;
  p_ot_req->Data[2] = (uint32_t) aTimeout;
  p_ot_req->Data[3] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

#if OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
bool otIp6IsSlaacEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_IS_SLAAC_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otIp6SetSlaacEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_SLAAC_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otIp6SetSlaacPrefixFilter(otInstance *aInstance, otIp6SlaacPrefixFilter aFilter)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otIp6SlaacPrefixFilterCb = aFilter;

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_SLAAC_PREFIX_FILTER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}
#endif // OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE

otError otIp6SetMeshLocalIid(otInstance *aInstance, const otIp6InterfaceIdentifier *aIid)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_MESH_LOCAL_IID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIid;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}


#endif

const char *otIp6ProtoToString(uint8_t aIpProto)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_PROTO_TO_STRING;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIpProto;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

void otIp6GetPrefix(const otIp6Address *aAddress, uint8_t aLength, otIp6Prefix *aPrefix)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_GET_PREFIX;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aAddress;
  p_ot_req->Data[1] = (uint32_t) aLength;
  p_ot_req->Data[2] = (uint32_t) aPrefix;

  Ot_Cmd_Transfer();
}

const otBorderRoutingCounters *otIp6GetBorderRoutingCounters(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_GET_BORDER_ROUTING_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otBorderRoutingCounters*)p_ot_req->Data[0];
}

void otIp6ResetBorderRoutingCounters(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_RESET_BORDER_ROUTING_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();
}
