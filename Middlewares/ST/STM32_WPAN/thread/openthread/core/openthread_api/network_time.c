/**
 ******************************************************************************
 * @file    network_time.c
 * @author  MCD Application Team
 * @brief   This file contains the Network Time interface shared between M0 and
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

#include "instance.h"
#include "network_time.h"


#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE

extern otNetworkTimeSyncCallbackFn otNetworkTimeSyncCb;

otNetworkTimeStatus otNetworkTimeGet(otInstance *aInstance, uint64_t &aNetworkTime)
{
  otNetworkTimeStatus rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_GET;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aNetworkTime;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otNetworkTimeStatus)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otNetworkTimeSetSyncPeriod(otInstance *aInstance, uint16_t aTimeSyncPeriod)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_SET_SYNC_PERIOD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aTimeSyncPeriod;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otNetworkTimeGetSyncPeriod(otInstance *aInstance)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_GET_SYNC_PERIOD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otNetworkTimeSetXtalThreshold(otInstance *aInstance, uint16_t aXTALThreshold)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_SET_XTAL_THRESHOLD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aXTALThreshold;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otNetworkTimeGetXtalThreshold(otInstance *aInstance)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_GET_XTAL_THRESHOLD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otNetworkTimeSyncSetCallback(otInstance *                aInstance,
                                  otNetworkTimeSyncCallbackFn aCallbackFn,
                                  void *                      aCallbackContext)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otNetworkTimeSyncCb = aCallbackFn;

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_SYNC_SET_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif //OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
