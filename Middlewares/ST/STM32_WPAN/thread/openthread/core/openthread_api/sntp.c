/**
 ******************************************************************************
 * @file    sntp.c
 * @author  MCD Application Team
 * @brief   This file contains OpenThread SNTP API
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

#if OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE

#include "sntp.h"

extern otSntpResponseHandler otSntpResponseHandlerCb;

otError otSntpClientQuery(otInstance *          aInstance,
                          const otSntpQuery *   aQuery,
                          otSntpResponseHandler aHandler,
                          void *                aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otSntpResponseHandlerCb = aHandler;

  p_ot_req->ID = MSG_M4TOM0_OT_SNTP_CLIENT_QUERY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aQuery;
  p_ot_req->Data[1] = (uint32_t)aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otSntpClientSetUnixEra(otInstance *aInstance, uint32_t aUnixEra)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SNTP_CLIENT_SET_UNIX_ERA;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aUnixEra;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return p_ot_req->Data[0];
}

#endif /* OPENTHREAD_CONFIG_SNTP_CLIENT_ENABLE */
