/**
 ******************************************************************************
 * @file    srp_server.c
 * @author  MCD Application Team
 * @brief   This file contains the srp server interface shared between M0 and
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

#if OPENTHREAD_CONFIG_SRP_SERVER_ENABLE

#include "srp_server.h"

extern otSrpServerServiceUpdateHandler otSrpServerServiceUpdateHandlerCb;

const char *otSrpServerGetDomain(otInstance *aInstance)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_DOMAIN;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpServerSetDomain(otInstance *aInstance, const char *aDomain)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_DOMAIN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDomain;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otSrpServerState otSrpServerGetState(otInstance *aInstance)
{
  otSrpServerState rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otSrpServerState)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otSrpServerGetPort(otInstance *aInstance)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_PORT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otSrpServerAddressMode otSrpServerGetAddressMode(otInstance *aInstance)
{
  otSrpServerAddressMode rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_ADDRESS_MODE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otSrpServerAddressMode)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpServerSetAddressMode(otInstance *aInstance, otSrpServerAddressMode aMode)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_ADDRESS_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMode;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint8_t otSrpServerGetAnycastModeSequenceNumber(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_ANYCAST_MODE_SEQUENCE_NUMBER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otSrpServerSetAnycastModeSequenceNumber(otInstance *aInstance, uint8_t aSequenceNumber)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_ANYCAST_MODE_SEQUENCE_NUMBER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSequenceNumber;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpServerSetEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otSrpServerGetTtlConfig(otInstance *aInstance, otSrpServerTtlConfig *aTtlConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_TTL_CONGIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aTtlConfig;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

otError otSrpServerSetTtlConfig(otInstance *aInstance, const otSrpServerTtlConfig *aTtlConfig)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_TTL_CONGIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aTtlConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpServerGetLeaseConfig(otInstance *aInstance, otSrpServerLeaseConfig *aLeaseConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_LEASE_CONFIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aLeaseConfig;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

otError otSrpServerSetLeaseConfig(otInstance *aInstance, const otSrpServerLeaseConfig *aLeaseConfig)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_LEASE_CONFIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aLeaseConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpServerSetServiceUpdateHandler(otInstance *                    aInstance,
                                        otSrpServerServiceUpdateHandler aServiceHandler,
                                        void *                          aContext)
{
  Pre_OtCmdProcessing();

  otSrpServerServiceUpdateHandlerCb = aServiceHandler;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_SERVICE_UPDATE_HANDLER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otSrpServerHandleServiceUpdateResult(otInstance *aInstance, otSrpServerServiceUpdateId aId, otError aError)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HANDLE_SERVICE_UPDATE_RESULT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aId;
  p_ot_req->Data[1] = (uint32_t) aError;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

const otSrpServerHost *otSrpServerGetNextHost(otInstance *aInstance, const otSrpServerHost *aHost)
{
  const otSrpServerHost * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_NEXT_HOST;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aHost;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSrpServerHost *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otSrpServerResponseCounters *otSrpServerGetResponseCounters(otInstance *aInstance)
{
  const otSrpServerResponseCounters * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_RESPONSE_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSrpServerResponseCounters *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otSrpServerHostIsDeleted(const otSrpServerHost *aHost)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_IS_DELETED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aHost;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const char *otSrpServerHostGetFullName(const otSrpServerHost *aHost)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_GET_FULL_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aHost;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otIp6Address *otSrpServerHostGetAddresses(const otSrpServerHost *aHost, uint8_t *aAddressesNum)
{
  const otIp6Address * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_GET_ADDRESSES;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHost;
  p_ot_req->Data[1] = (uint32_t) aAddressesNum;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otIp6Address *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpServerHostGetLeaseInfo(const otSrpServerHost *aHost, otSrpServerLeaseInfo *aLeaseInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_GET_LEASE_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHost;
  p_ot_req->Data[1] = (uint32_t) aLeaseInfo;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

const otSrpServerService *otSrpServerHostGetNextService(const otSrpServerHost *   aHost,
                                                        const otSrpServerService *aService)
{
  const otSrpServerService * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_GET_NEXT_SERVICE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHost;
  p_ot_req->Data[1] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();

  rspData = (const otSrpServerService *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otSrpServerServiceIsDeleted(const otSrpServerService *aService)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_IS_DELETED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const char *otSrpServerServiceGetInstanceName(const otSrpServerService *aService)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_INSTANCE_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const char *otSrpServerServiceGetServiceName(const otSrpServerService *aService)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_SERVICE_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otSrpServerServiceGetPort(const otSrpServerService *aService)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_PORT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otSrpServerServiceGetWeight(const otSrpServerService *aService)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_WEIGHT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otSrpServerServiceGetPriority(const otSrpServerService *aService)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_PRIORITY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otSrpServerServiceGetTtl(const otSrpServerService *aService)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_TTL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const uint8_t *otSrpServerServiceGetTxtData(const otSrpServerService *aService, uint16_t *aDataLength)
{
  const uint8_t * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_TX_DATA;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aService;
  p_ot_req->Data[1] = (uint32_t) aDataLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const uint8_t *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otSrpServerHost *otSrpServerServiceGetHost(const otSrpServerService *aService)
{
  const otSrpServerHost * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_HOST;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSrpServerHost *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otSrpServerServiceGetLeaseInfo(const otSrpServerService *aService, otSrpServerLeaseInfo *aLeaseInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_LEASE_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aService;
  p_ot_req->Data[1] = (uint32_t) aLeaseInfo;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

void otSrpServerSetAutoEnableMode(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_AUTO_ENABLE_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

bool otSrpServerIsAutoEnableMode(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_IS_AUTO_ENABLE_MODE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif // OPENTHREAD_CONFIG_SRP_SERVER_ENABLE
