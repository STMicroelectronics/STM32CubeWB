/**
 ******************************************************************************
 * @file    network_time.c
 * @author  MCD Application Team
 * @brief   This file contains the Network Time interface shared between M0 and
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

#include "instance.h"
#include "network_time.h"


#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE

extern otNetworkTimeSyncCallbackFn otNetworkTimeSyncCb;

otNetworkTimeStatus otNetworkTimeGet(otInstance *aInstance, uint64_t &aNetworkTime)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_GET;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aNetworkTime;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otNetworkTimeStatus)p_ot_req->Data[0];
}

otError otNetworkTimeSetSyncPeriod(otInstance *aInstance, uint16_t aTimeSyncPeriod)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_SET_SYNC_PERIOD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aTimeSyncPeriod;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint16_t otNetworkTimeGetSyncPeriod(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_GET_SYNC_PERIOD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

otError otNetworkTimeSetXtalThreshold(otInstance *aInstance, uint16_t aXTALThreshold)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_SET_XTAL_THRESHOLD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aXTALThreshold;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint16_t otNetworkTimeGetXtalThreshold(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_TIME_GET_XTAL_THRESHOLD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

void otNetworkTimeSyncSetCallback(otInstance *                aInstance,
                                  otNetworkTimeSyncCallbackFn aCallbackFn,
                                  void *                      aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otNetworkTimeSyncCb = aCallbackFn;

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_SYNC_SET_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

#endif //OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
