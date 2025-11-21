/**
 ******************************************************************************
 * @file    border_agent.c
 * @author  MCD Application Team
 * @brief   This file contains the Border Agent interface shared between M0 and
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

#include "border_agent.h"

#if OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE

otBorderAgentState otBorderAgentIsActive(otInstance *aInstance);
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_AGENT_GET_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

uint16_t otBorderAgentGetUdpPort(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_AGENT_GET_UDP_PORT;
  p_ot_req->Size=0;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

void otBorderAgentInitSessionIterator(otInstance *aInstance, otBorderAgentSessionIterator *aIterator)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_AGENT_INIT_SESSION;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIterator;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

void otBorderAgentGetNextSessionInfo(otBorderAgentSessionIterator *aIterator, otBorderAgentSessionInfo *aSessionInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_AGENT_NEXT_SESSION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aSessionInfo;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

void otBorderAgentCounters *otBorderAgentGetCounters(otInstance *aInstance);
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_BORDER_AGENT_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();
  
  Post_OtCmdProcessing();
}

#endif // OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE
