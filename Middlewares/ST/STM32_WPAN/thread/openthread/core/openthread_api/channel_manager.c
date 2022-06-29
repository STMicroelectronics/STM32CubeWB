/**
 ******************************************************************************
 * @file    channel_manager.c
 * @author  MCD Application Team
 * @brief   This file contains the Channel Manager interface shared between M0 and
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

#include "channel_manager.h"


#if OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE && OPENTHREAD_FTD

void otChannelManagerRequestChannelChange(otInstance *aInstance, uint8_t aChannel);
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_REQUEST_CHANNEL_CHANGE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aChannel;

  Ot_Cmd_Transfer();
}

uint8_t otChannelManagerGetRequestedChannel(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_GET_REQUESTED_CHANNEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

uint16_t otChannelManagerGetDelay(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_GET_DELAY;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

otError otChannelManagerSetDelay(otInstance *aInstance, uint16_t aDelay)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_SET_DELAY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDelay;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otChannelManagerRequestChannelSelect(otInstance *aInstance, bool aSkipQualityCheck)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_REQUEST_CHANNEL_SELECT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSkipQualityCheck;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otChannelManagerSetAutoChannelSelectionEnabled(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_SET_AUTO_CHANNEL_SELECTION_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();
}

bool otChannelManagerGetAutoChannelSelectionEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_GET_AUTO_CHANNEL_SELECTION_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otChannelManagerSetAutoChannelSelectionInterval(otInstance *aInstance, uint32_t aInterval)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_SET_AUTO_CHANNEL_SELECTION_INTERVAL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aInterval;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint32_t otChannelManagerGetAutoChannelSelectionInterval(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_GET_AUTO_CHANNEL_SELECTION_INTERVAL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

uint32_t otChannelManagerGetSupportedChannels(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_GET_SUPPORTED_CHANNELS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

void otChannelManagerSetSupportedChannels(otInstance *aInstance, uint32_t aChannelMask)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_SET_SUPPORTED_CHANNELS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = aChannelMask;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint32_t otChannelManagerGetFavoredChannels(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_GET_FAVORED_CHANNELS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

void otChannelManagerSetFavoredChannels(otInstance *aInstance, uint32_t aChannelMask)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_SET_FAVORED_CHANNELS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = aChannelMask;

  Ot_Cmd_Transfer();
}

uint16_t otChannelManagerGetCcaFailureRateThreshold(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_GET_CCA_FAILURE_RATE_THRESHOLD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();
  return (uint32_t)p_ot_req->Data[0];
}

void otChannelManagerSetCcaFailureRateThreshold(otInstance *aInstance, uint16_t aThreshold)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MANAGER_SET_CCA_FAILURE_RATE_THRESHOLD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = aThreshold;

  Ot_Cmd_Transfer();
}

#endif  /* OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE && OPENTHREAD_FTD */

