/**
 ******************************************************************************
 * @file    backbone_router_ftd.c
 * @author  MCD Application Team
 * @brief   This file contains the BackBone Router FTD interface shared between M0 and
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

#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE
#include "backbone_router_ftd.h"

extern otBackboneRouterDomainPrefixCallback otBackboneRouterDomainPrefixCb;

void otBackboneRouterSetEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();
}

otBackboneRouterState otBackboneRouterGetState(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_GET_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otBackboneRouterState)p_ot_req->Data[0];
}

void otBackboneRouterGetConfig(otInstance *aInstance, otBackboneRouterConfig *aConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_GET_CONFIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();
}

otError otBackboneRouterSetConfig(otInstance *aInstance, const otBackboneRouterConfig *aConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_SET_CONFIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otBackboneRouterRegister(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_REGISTER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint8_t otBackboneRouterGetRegistrationJitter(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_GET_REGISTRATION_JITTER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

void otBackboneRouterSetRegistrationJitter(otInstance *aInstance, uint8_t aJitter)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_SET_REGISTRATION_JITTER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aJitter;

  Ot_Cmd_Transfer();
}

otError otBackboneRouterGetDomainPrefix(otInstance *aInstance, otBorderRouterConfig *aConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_GET_DOMAIN_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otBackboneRouterSetDomainPrefixCallback(otInstance *                         aInstance,
                                             otBackboneRouterDomainPrefixCallback aCallback,
                                             void *                               aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otBackboneRouterDomainPrefixCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_SET_DOMAIN_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();
}

#if OPENTHREAD_CONFIG_BACKBONE_ROUTER_DUA_NDPROXYING_ENABLE
extern otBackboneRouterNdProxyCallback otBackboneRouterNdProxyCb;

void otBackboneRouterSetNdProxyCallback(otInstance *                    aInstance,
                                        otBackboneRouterNdProxyCallback aCallback,
                                        void *                          aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otBackboneRouterNdProxyCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_SET_ND_PROXY_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();
}

otError otBackboneRouterGetNdProxyInfo(otInstance *                 aInstance,
                                       const otIp6Address *         aDua,
                                       otBackboneRouterNdProxyInfo *aNdProxyInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_GET_ND_PROXY_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aDua;
  p_ot_req->Data[1] = (uint32_t) aNdProxyInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif // OPENTHREAD_CONFIG_BACKBONE_ROUTER_DUA_NDPROXYING_ENABLE

#if OPENTHREAD_CONFIG_BACKBONE_ROUTER_MULTICAST_ROUTING_ENABLE

extern otBackboneRouterMulticastListenerCallback otBackboneRouterMulticastListenerCb;

void otBackboneRouterSetMulticastListenerCallback(otInstance *                              aInstance,
                                                  otBackboneRouterMulticastListenerCallback aCallback,
                                                  void *                                    aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otBackboneRouterMulticastListenerCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_SET_MULTICAST_LISTENER_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_TransferWithNotif();
}

otError otBackboneRouterMulticastListenerGetNext(otInstance *                                   aInstance,
                                                 otBackboneRouterMulticastListenerIterator      *aIterator,
                                                 otBackboneRouterMulticastListenerInfo          *aListenerInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_MULTICAST_LISTENER_GET_NEXT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aListenerInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
#if OPENTHREAD_CONFIG_BACKBONE_ROUTER_DUA_NDPROXYING_ENABLE
void otBackboneRouterConfigNextDuaRegistrationResponse(otInstance *                    aInstance,
                                                       const otIp6InterfaceIdentifier *aMlIid,
                                                       uint8_t                         aStatus)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_CONFIG_NEXT_DUA_REGISTTRATION_RESPONSE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMlIid;
  p_ot_req->Data[1] = (uint32_t) aStatus;

  Ot_Cmd_Transfer();
}
#endif

#if OPENTHREAD_CONFIG_BACKBONE_ROUTER_MULTICAST_ROUTING_ENABLE
void otBackboneRouterConfigNextMulticastListenerRegistrationResponse(otInstance *aInstance, uint8_t aStatus)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_CONFIG_NEXT_MULTICAST_LISTENER_REGISTRATION;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aStatus;

  Ot_Cmd_Transfer();
}

void otBackboneRouterMulticastListenerClear(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_MULTICAST_LISTENER_CLEAR;

  p_ot_req->Size=0;

  Ot_Cmd_TransferWithNotif();
}

otError otBackboneRouterMulticastListenerAdd(otInstance *aInstance, const otIp6Address *aAddress, uint32_t aTimeout)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BACKBONE_ROUTER_MULTICAST_LISTENER_ADD;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aAddress;
  p_ot_req->Data[1] = (uint32_t) aTimeout;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif // OPENTHREAD_CONFIG_BACKBONE_ROUTER_MULTICAST_ROUTING_ENABLE
#endif // OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE

#endif // OPENTHREAD_FTD && OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE
