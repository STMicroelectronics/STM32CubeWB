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

#include "joiner.h"
#include "netdata.h"


otError otNetDataGet(otInstance *aInstance, bool aStable, uint8_t *aData,
    uint8_t *aDataLength)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aStable;
  p_ot_req->Data[1] = (uint32_t) aData;
  p_ot_req->Data[2] = (uint32_t) aDataLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint8_t otNetDataGetLength(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_LENGTH;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint8_t otNetDataGetMaxLength(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_MAX_LENGTH;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otNetDataResetMaxLength(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_RESET_MAX_LENGTH;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

otError otNetDataGetNextOnMeshPrefix(otInstance *aInstance, otNetworkDataIterator *aIterator,
    otBorderRouterConfig *aConfig)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_NEXT_ON_MESH_PREFIX;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE
bool otNetDataContainsOmrPrefix(otInstance *aInstance, const otIp6Prefix *aPrefix)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_CONTAINS_OMR_PREFIX;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPrefix;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif

otError otNetDataGetNextRoute(otInstance *aInstance, otNetworkDataIterator *aIterator,
    otExternalRouteConfig *aConfig)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_NEXT_ROUTE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otNetDataGetNextService(otInstance *aInstance, otNetworkDataIterator *aIterator, otServiceConfig *aConfig)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_NEXT_SERVICE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otNetDataGetNextLowpanContextInfo(otInstance            *aInstance,
                                          otNetworkDataIterator *aIterator,
                                          otLowpanContextInfo   *aContextInfo)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_NEXT_LOWPAN_CONTEXT_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aContextInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otNetDataGetCommissioningDataset(otInstance *aInstance, otCommissioningDataset *aDataset)
{ 
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_COMMISSIONING_DATA_SET;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDataset;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

uint8_t otNetDataGetVersion(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_VERSION;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint8_t otNetDataGetStableVersion(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_STABLE_VERSION;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otNetDataSteeringDataCheckJoiner(otInstance *aInstance, const otExtAddress *aEui64)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_STEERING_DATA_CHECK_JOINER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEui64;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otNetDataSteeringDataCheckJoinerWithDiscerner(otInstance *aInstance, const otJoinerDiscerner *aDiscerner)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_STEERING_DATA_CHECK_JOINER_WITH_DISCERNER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDiscerner;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

