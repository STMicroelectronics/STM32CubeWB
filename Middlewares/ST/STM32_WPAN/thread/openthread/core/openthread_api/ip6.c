/**
 ******************************************************************************
 * @file    ip6.c
 * @author  MCD Application Team
 * @brief   This file contains the IPv6 interface shared between M0 and
 *          M4.
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

#include "ip6.h"

#if OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
extern otIp6SlaacPrefixFilter otIp6SlaacPrefixFilterCb;
#endif // OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
extern otIp6ReceiveCallback otIp6ReceiveCb;
extern otIp6AddressCallback otIp6AddressCb;


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

  p_ot_req->ID = MSG_M4TOM0_OT_IP6_SET_RECEIVE_CALLBACK;

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
