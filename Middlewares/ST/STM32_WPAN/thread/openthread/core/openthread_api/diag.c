/**
 ******************************************************************************
 * @file    diag.c
 * @author  MCD Application Team
 * @brief   This file contains the diagnostic interface shared between M0 and
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

#include "diag.h"

#if OPENTHREAD_CONFIG_DIAG_ENABLE

void otDiagProcessCmd(otInstance *aInstance, int aArgCount, char *aArgVector[], char *aOutput, size_t aOutputMaxLen);
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DIAG_PROCESS_CMD;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aArgCount;
  p_ot_req->Data[1] = (uint32_t) aArgVector;
  p_ot_req->Data[2] = (uint32_t) aOutput;
  p_ot_req->Data[3] = (uint32_t) aOutputMaxLen;

  Ot_Cmd_Transfer();
}

void otDiagProcessCmdLine(otInstance *aInstance, const char *aString, char *aOutput, size_t aOutputMaxLen)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DIAG_PROCESS_CMD_LINE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aString;
  p_ot_req->Data[1] = (uint32_t) aOutput;
  p_ot_req->Data[2] = (uint32_t) aOutputMaxLen;

  Ot_Cmd_Transfer();
}

bool otDiagIsEnabled(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DIAG_IS_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

#endif // OPENTHREAD_CONFIG_DIAG_ENABLE
