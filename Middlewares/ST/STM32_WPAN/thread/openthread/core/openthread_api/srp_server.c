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
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_DOMAIN;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

otError otSrpServerSetDomain(otInstance *aInstance, const char *aDomain)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_DOMAIN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDomain;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otSrpServerState otSrpServerGetState(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otSrpServerState)p_ot_req->Data[0];
}

uint16_t otSrpServerGetPort(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_PORT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

otSrpServerAddressMode otSrpServerGetAddressMode(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_ADDRESS_MODE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otSrpServerAddressMode)p_ot_req->Data[0];
}

otError otSrpServerSetAddressMode(otInstance *aInstance, otSrpServerAddressMode aMode)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_ADDRESS_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMode;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint8_t otSrpServerGetAnycastModeSequenceNumber(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_ANYCAST_MODE_SEQUENCE_NUMBER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

otError otSrpServerSetAnycastModeSequenceNumber(otInstance *aInstance, uint8_t aSequenceNumber)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_ANYCAST_MODE_SEQUENCE_NUMBER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSequenceNumber;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
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

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
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
}

otError otSrpServerSetTtlConfig(otInstance *aInstance, const otSrpServerTtlConfig *aTtlConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_TTL_CONGIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aTtlConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
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

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otSrpServerSetLeaseConfig(otInstance *aInstance, const otSrpServerLeaseConfig *aLeaseConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SET_LEASE_CONFIG;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aLeaseConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
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

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
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

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

const otSrpServerHost *otSrpServerGetNextHost(otInstance *aInstance, const otSrpServerHost *aHost)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_NEXT_HOST;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aHost;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otSrpServerHost*)p_ot_req->Data[0];
}

const otSrpServerResponseCounters *otSrpServerGetResponseCounters(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_GET_RESPONSE_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otSrpServerResponseCounters*)p_ot_req->Data[0];
}

bool otSrpServerHostIsDeleted(const otSrpServerHost *aHost)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_IS_DELETED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aHost;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

const char *otSrpServerHostGetFullName(const otSrpServerHost *aHost)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_GET_FULL_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aHost;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

const otIp6Address *otSrpServerHostGetAddresses(const otSrpServerHost *aHost, uint8_t *aAddressesNum)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_GET_ADDRESSES;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHost;
  p_ot_req->Data[1] = (uint32_t) aAddressesNum;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otIp6Address*)p_ot_req->Data[0];
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
}

const otSrpServerService *otSrpServerHostGetNextService(const otSrpServerHost *   aHost,
                                                        const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_GET_NEXT_SERVICE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHost;
  p_ot_req->Data[1] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otSrpServerService*)p_ot_req->Data[0];
}

const otSrpServerService *otSrpServerHostFindNextService(const otSrpServerHost *   aHost,
                                                         const otSrpServerService *aPrevService,
                                                         otSrpServerServiceFlags   aFlags,
                                                         const char *              aServiceName,
                                                         const char *              aInstanceName)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_HOST_FIND_NEXT_SERVICE;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t) aHost;
  p_ot_req->Data[1] = (uint32_t) aPrevService;
  p_ot_req->Data[2] = (uint32_t) aFlags;
  p_ot_req->Data[3] = (uint32_t) aServiceName;
  p_ot_req->Data[4] = (uint32_t) aInstanceName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otSrpServerService*)p_ot_req->Data[0];
}

bool otSrpServerServiceIsDeleted(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_IS_DELETED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

bool otSrpServerServiceIsSubType(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_IS_SUB_TYPE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

const char *otSrpServerServiceGetFullName(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_FULL_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

const char *otSrpServerServiceGetInstanceName(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_INSTANCE_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

const char *otSrpServerServiceGetServiceName(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_SERVICE_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

otError otSrpServerServiceGetServiceSubTypeLabel(const otSrpServerService *aService, char *aLabel, uint8_t aMaxSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_SERVICE_SUB_TYPE_LABEL;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aService;
  p_ot_req->Data[1] = (uint32_t) aLabel;
  p_ot_req->Data[2] = (uint32_t) aMaxSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint16_t otSrpServerServiceGetPort(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_PORT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

uint16_t otSrpServerServiceGetWeight(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_WEIGHT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

uint16_t otSrpServerServiceGetPriority(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_PRIORITY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

uint32_t otSrpServerServiceGetTtl(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_TTL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

const uint8_t *otSrpServerServiceGetTxtData(const otSrpServerService *aService, uint16_t *aDataLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_TX_DATA;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aService;
  p_ot_req->Data[1] = (uint32_t) aDataLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t*)p_ot_req->Data[0];
}

const otSrpServerHost *otSrpServerServiceGetHost(const otSrpServerService *aService)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_SERVICE_GET_HOST;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aService;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otSrpServerHost*)p_ot_req->Data[0];
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
}

bool otSrpServerIsAutoEnableMode(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SRP_SERVER_IS_AUTO_ENABLE_MODE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

#endif // OPENTHREAD_CONFIG_SRP_SERVER_ENABLE
