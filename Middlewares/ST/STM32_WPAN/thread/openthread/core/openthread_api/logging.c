/**
 ******************************************************************************
 * @file    logging.c
 * @author  MCD Application Team
 * @brief   This file contains the OT wrapper for logging
 *          shared between M0 and M4.
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

#include "logging.h"

otLogLevel otLoggingGetLevel(void)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LOGGING_GET_LEVEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_CONFIG_LOG_LEVEL_DYNAMIC_ENABLE
otError otLoggingSetLevel(otLogLevel aLogLevel)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LOGGING_SET_LEVEL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aLogLevel;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

void otDumpCritPlat(const char *aText, const void *aData, uint16_t aDataLength)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DUMP_CRIT_PLAT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aText;
  p_ot_req->Data[1] = (uint32_t)aData;
  p_ot_req->Data[2] = (uint32_t)aDataLength;

  Ot_Cmd_Transfer();
}

void otDumpWarnPlat(const char *aText, const void *aData, uint16_t aDataLength)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DUMP_WARN_PLAT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aText;
  p_ot_req->Data[1] = (uint32_t)aData;
  p_ot_req->Data[2] = (uint32_t)aDataLength;

  Ot_Cmd_Transfer();
}

void otDumpNotePlat(const char *aText, const void *aData, uint16_t aDataLength)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DUMP_NOTE_PLAT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aText;
  p_ot_req->Data[1] = (uint32_t)aData;
  p_ot_req->Data[2] = (uint32_t)aDataLength;

  Ot_Cmd_Transfer();
}

void otDumpInfoPlat(const char *aText, const void *aData, uint16_t aDataLength)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DUMP_INFO_PLAT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aText;
  p_ot_req->Data[1] = (uint32_t)aData;
  p_ot_req->Data[2] = (uint32_t)aDataLength;

  Ot_Cmd_Transfer();
}

void otDumpDebgPlat(const char *aText, const void *aData, uint16_t aDataLength)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DUMP_DEBG_PLAT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aText;
  p_ot_req->Data[1] = (uint32_t)aData;
  p_ot_req->Data[2] = (uint32_t)aDataLength;

  Ot_Cmd_Transfer();
}
