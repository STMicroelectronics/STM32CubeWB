/**
  ******************************************************************************
  * @file    child_supervision.c
  * @author  MCD Application Team
  * @brief   This file contains the Child Supervision interface shared between
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
