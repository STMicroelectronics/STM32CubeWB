/**
 ******************************************************************************
 * @file    ping_sender.c
 * @author  MCD Application Team
 * @brief   This file contains the ping sender interface shared between M0 and
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

#include "ping_sender.h"

#if OPENTHREAD_CONFIG_PING_SENDER_ENABLE

extern otPingSenderReplyCallback otPingSenderReplyCb;
extern otPingSenderStatisticsCallback otPingSenderStatisticsCb;

otError otPingSenderPing(otInstance *aInstance, const otPingSenderConfig *aConfig)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otPingSenderReplyCb = aConfig->mReplyCallback;
  otPingSenderStatisticsCb = aConfig->mStatisticsCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_PING_SENDER_PING;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aConfig;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otPingSenderStop(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_PING_SENDER_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#endif // OPENTHREAD_CONFIG_PING_SENDER_ENABLE
