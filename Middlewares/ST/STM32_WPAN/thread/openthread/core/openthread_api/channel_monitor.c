/**
 ******************************************************************************
 * @file    channel_monitor.c
 * @author  MCD Application Team
 * @brief   This file contains the Channel Monitoring interface shared between
 *          M0 and M4.
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

#include "channel_monitor.h"


#if OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE

otError otChannelMonitorSetEnabled(otInstance *aInstance, bool aEnabled)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MONITOR_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otChannelMonitorIsEnabled(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MONITOR_IS_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otChannelMonitorGetSampleInterval(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MONITOR_GET_SAMPLE_INTERVAL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

int8_t otChannelMonitorGetRssiThreshold(otInstance *aInstance)
{
  int8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MONITOR_GET_RSSI_THRESHOLD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (int8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otChannelMonitorGetSampleWindow(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MONITOR_GET_SAMPLE_WINDOW;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otChannelMonitorGetSampleCount(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MONITOR_GET_SAMPLE_COUNT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otChannelMonitorGetChannelOccupancy(otInstance *aInstance, uint8_t aChannel)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_CHANNEL_MONITOR_GET_CHANNEL_OCCUPANCY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aChannel;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif /* OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE */

