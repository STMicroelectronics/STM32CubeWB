/**
  ******************************************************************************
  * @file    icmp6.c
  * @author  MCD Application Team
  * @brief   This file contains the icmp6 interface shared between M0 and
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

#include "icmp6.h"

extern otIcmp6ReceiveCallback otIcmp6ReceiveCb;


otIcmp6EchoMode otIcmp6GetEchoMode(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_ICMP6_GET_ECHO_MODE;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otIcmp6EchoMode)p_ot_req->Data[0];
}

void otIcmp6SetEchoMode(otInstance *aInstance, otIcmp6EchoMode aMode)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_ICMP6_SET_ECHO_MODE;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aMode;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otIcmp6RegisterHandler(otInstance *aInstance, otIcmp6Handler *aHandler)
{
    Pre_OtCmdProcessing();
    otIcmp6ReceiveCb = aHandler->mReceiveCallback;
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_ICMP6_REGISTER_HANDLER;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aHandler;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otIcmp6SendEchoRequest(otInstance *         aInstance,
                               otMessage *          aMessage,
                               const otMessageInfo *aMessageInfo,
                               uint16_t             aIdentifier)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_ICMP6_SEND_ECHO_REQUEST;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t) aMessage;
    p_ot_req->Data[1] = (uint32_t) aMessageInfo;
    p_ot_req->Data[2] = (uint32_t) aIdentifier;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}
