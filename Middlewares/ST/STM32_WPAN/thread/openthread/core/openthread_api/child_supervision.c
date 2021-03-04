/**
  ******************************************************************************
  * @file    child_supervision.c
  * @author  MCD Application Team
  * @brief   This file contains the Child Supervision interface shared between
  *          M0 and M4.
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

#include "child_supervision.h"


#if OPENTHREAD_CONFIG_CHILD_SUPERVISION_ENABLE

uint16_t otChildSupervisionGetInterval(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_CHILD_SUPERVISION_GET_INTERVAL;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint16_t)p_ot_req->Data[0];
}

void otChildSupervisionSetInterval(otInstance *aInstance, uint16_t aInterval)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_CHILD_SUPERVISION_SET_INTERVAL;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aInterval;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint16_t otChildSupervisionGetCheckTimeout(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_CHILD_SUPERVISION_GET_CHECK_TIMEOUT;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint16_t)p_ot_req->Data[0];
}

void otChildSupervisionSetCheckTimeout(otInstance *aInstance, uint16_t aTimeout)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_CHILD_SUPERVISION_SET_CHECK_TIMEOUT;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aTimeout;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#endif /* OPENTHREAD_CONFIG_CHILD_SUPERVISION_ENABLE */
