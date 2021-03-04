/**
  ******************************************************************************
  * @file    icmp6.c
  * @author  MCD Application Team
  * @brief   This file contains the icmp6 interface shared between M0 and
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
