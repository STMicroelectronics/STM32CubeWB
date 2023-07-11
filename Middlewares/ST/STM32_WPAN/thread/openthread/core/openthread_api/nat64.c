/**
 ******************************************************************************
 * @file    nat64.c
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

#include "nat64.h"

extern otNat64ReceiveIp4Callback otNat64ReceiveIp4Cb;

void otNat64GetCounters(otInstance *aInstance, otNat64ProtocolCounters *aCounters)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_GET_COUNTERS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aCounters;

  Ot_Cmd_Transfer();
}

void otNat64GetErrorCounters(otInstance *aInstance, otNat64ErrorCounters *aCounters)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_GET_ERROR_COUNTERS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aCounters;

  Ot_Cmd_Transfer();
}

void otNat64InitAddressMappingIterator(otInstance *aInstance, otNat64AddressMappingIterator *aIterator)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_INIT_MAPPING_ITERATORS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIterator;

  Ot_Cmd_Transfer();
}

otError otNat64GetNextAddressMapping(otInstance                    *aInstance,
                                     otNat64AddressMappingIterator *aIterator,
                                     otNat64AddressMapping         *aMapping)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_GET_NEXT_ADDRESS_MAPPING;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aMapping;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otNat64State otNat64GetTranslatorState(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_GET_TRANSLATOR_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otNat64State)p_ot_req->Data[0];

}

otNat64State otNat64GetPrefixManagerState(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_GET_PREFIX_MANAGER_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otNat64State)p_ot_req->Data[0];
}

void otNat64SetEnabled(otInstance *aInstance, bool aEnable)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnable;

  Ot_Cmd_Transfer();
}

otMessage *otIp4NewMessage(otInstance *aInstance, const otMessageSettings *aSettings)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP4_NEW_MESSAGE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSettings;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otMessage *)p_ot_req->Data[0];
}

otError otNat64SetIp4Cidr(otInstance *aInstance, const otIp4Cidr *aCidr)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_SET_IP4_CIDR;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aCidr;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otNat64Send(otInstance *aInstance, otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_SEND;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otNat64SetReceiveIp4Callback(otInstance *aInstance, otNat64ReceiveIp4Callback aCallback, void *aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otNat64ReceiveIp4Cb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_SET_RECEIVED_IP4_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();
}

otError otNat64GetCidr(otInstance *aInstance, otIp4Cidr *aCidr)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_GET_IP4_CIDR;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aCidr;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otIp4IsAddressEqual(const otIp4Address *aFirst, const otIp4Address *aSecond)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP4_IS_ADDRESS_EQUAL;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aFirst;
  p_ot_req->Data[1] = (uint32_t) aSecond;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otIp4ExtractFromIp6Address(uint8_t aPrefixLength, const otIp6Address *aIp6Address, otIp4Address *aIp4Address)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP4_EXTRACT_FROM_IP_ADDRESS;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aPrefixLength;
  p_ot_req->Data[1] = (uint32_t) aIp6Address;
  p_ot_req->Data[2] = (uint32_t) aIp4Address;

  Ot_Cmd_Transfer();
}

void otIp4AddressToString(const otIp4Address *aAddress, char *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP4_ADDRESS_TO_STRING;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aAddress;
  p_ot_req->Data[1] = (uint32_t) aBuffer;
  p_ot_req->Data[2] = (uint32_t) aSize;

  Ot_Cmd_Transfer();
}

void otIp4CidrToString(const otIp4Cidr *aCidr, char *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP4_CIDR_TO_STRING;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aCidr;
  p_ot_req->Data[1] = (uint32_t) aBuffer;
  p_ot_req->Data[2] = (uint32_t) aSize;

  Ot_Cmd_Transfer();
}

otError otIp4AddressFromString(const char *aString, otIp4Address *aAddress)
{  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_IP4_ADDRESS_FROM_STRING;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aString;
  p_ot_req->Data[1] = (uint32_t) aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otNat64SynthesizeIp6Address(otInstance *aInstance, const otIp4Address *aIp4Address, otIp6Address *aIp6Address)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NAT64_SYNTHESIZE_IP6_ADRESS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIp4Address;
  p_ot_req->Data[1] = (uint32_t) aIp6Address;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}