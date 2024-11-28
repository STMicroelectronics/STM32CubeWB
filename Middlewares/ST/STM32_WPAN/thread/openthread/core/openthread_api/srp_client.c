/**
 ******************************************************************************
 * @file    srp_client.c
 * @author  MCD Application Team
 * @brief   This file contains the srp client interface shared between M0 and
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

#if OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE

#include "srp_client.h"

extern otSrpClientCallback otSrpClientCb;
extern otSrpClientAutoStartCallback otSrpClientAutoStartCb;

otError otSrpClientStart(otInstance *aInstance, const otSockAddr *aServerSockAddr)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_START;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aServerSockAddr;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpClientStop(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

bool otSrpClientIsRunning(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_IS_RUNNING;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otSockAddr *otSrpClientGetServerAddress(otInstance *aInstance)
{
  const otSockAddr * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_GET_SERVER_ADDRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSockAddr *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpClientSetCallback(otInstance *aInstance, otSrpClientCallback aCallback, void *aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otSrpClientCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

#if OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE
void otSrpClientEnableAutoStartMode(otInstance *aInstance, otSrpClientAutoStartCallback aCallback, void *aContext)
{
  Pre_OtCmdProcessing();

  otSrpClientAutoStartCb = aCallback;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_ENABLE_AUTO_START_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_TransferWithNotif();

  Post_OtCmdProcessing();
}

void otSrpClientDisableAutoStartMode(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_DISABLE_AUTO_START_MODE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

bool otSrpClientIsAutoStartModeEnabled(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_IS_AUTO_START_MODE_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif // OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE

uint32_t otSrpClientGetTtl(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_GET_TTL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpClientSetTtl(otInstance *aInstance, uint32_t aTtl)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_TTL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aTtl;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

uint32_t otSrpClientGetLeaseInterval(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_GET_LEASE_INTERVAL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpClientSetLeaseInterval(otInstance *aInstance, uint32_t aInterval)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_LEASE_INTERVAL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aInterval;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

uint32_t otSrpClientGetKeyLeaseInterval(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_GET_KEY_LEASE_INTERVAL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpClientSetKeyLeaseInterval(otInstance *aInstance, uint32_t aInterval)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_KEY_LEASE_INTERVAL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aInterval;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

const otSrpClientHostInfo *otSrpClientGetHostInfo(otInstance *aInstance)
{
  const otSrpClientHostInfo * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_GET_HOST_INFO;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSrpClientHostInfo *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientSetHostName(otInstance *aInstance, const char *aName)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_HOST_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientEnableAutoHostAddress(otInstance *aInstance)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_ENABLE_AUTO_HOST_ADDRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientSetHostAddresses(otInstance *aInstance, const otIp6Address *aIp6Addresses, uint8_t aNumAddresses)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_HOST_ADDRESSES;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIp6Addresses;
  p_ot_req->Data[1] = (uint32_t) aNumAddresses;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientAddService(otInstance *aInstance, otSrpClientService *aService)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_ADD_SERVICE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientRemoveService(otInstance *aInstance, otSrpClientService *aService)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_REMOVE_SERVICE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientClearService(otInstance *aInstance, otSrpClientService *aService)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_CLEAR_SERVICE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otSrpClientService *otSrpClientGetServices(otInstance *aInstance)
{
  const otSrpClientService * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_GET_SERVICES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSrpClientService *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientRemoveHostAndServices(otInstance *aInstance, bool aRemoveKeyLease, bool aSendUnregToServer)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_REMOVE_HOST_AND_SERVICES;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aRemoveKeyLease;
  p_ot_req->Data[1] = (uint32_t) aSendUnregToServer;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpClientClearHostAndServices(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_CLEAR_HOST_AND_SERVICES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

#if OPENTHREAD_CONFIG_SRP_CLIENT_DOMAIN_NAME_API_ENABLE
const char *otSrpClientGetDomainName(otInstance *aInstance)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_GET_DOMAIN_NAME;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpClientSetDomainName(otInstance *aInstance, const char *aDomainName)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_DOMAIN_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDomainName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif // OPENTHREAD_CONFIG_SRP_CLIENT_DOMAIN_NAME_API_ENABLE

const char *otSrpClientItemStateToString(otSrpClientItemState aItemState)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_ITEM_STATE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aItemState;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
void otSrpClientSetServiceKeyRecordEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_SET_SERVICE_KEY_RECORD_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

bool otSrpClientIsServiceKeyRecordEnabled(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_CLIENT_IS_SERVICE_KEY_RECORD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif

#endif // OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE
