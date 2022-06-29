/**
 ******************************************************************************
 * @file    diag.c
 * @author  MCD Application Team
 * @brief   This file contains the diagnostic interface shared between M0 and
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

#if OPENTHREAD_CONFIG_DIAG_ENABLE

#include "diag.h"

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

otError otDiagProcessCmd(otInstance *aInstance, uint8_t aArgsLength, char *aArgs[], char *aOutput, size_t aOutputMaxLen)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DIAG_PROCESS_CMD;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aArgsLength;
  p_ot_req->Data[1] = (uint32_t) aArgs;
  p_ot_req->Data[2] = (uint32_t) aOutput;
  p_ot_req->Data[3] = (uint32_t) aOutputMaxLen;

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
