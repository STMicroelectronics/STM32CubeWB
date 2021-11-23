/**
  ******************************************************************************
  * @file    joiner.c
  * @author  MCD Application Team
  * @brief   This file contains the joiner interface shared between M0 and
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

#include "joiner.h"

extern otJoinerCallback otJoinerCb;

#if OPENTHREAD_CONFIG_JOINER_ENABLE
otError otJoinerStart(otInstance *     aInstance,
                      const char *     aPskd,
                      const char *     aProvisioningUrl,
                      const char *     aVendorName,
                      const char *     aVendorModel,
                      const char *     aVendorSwVersion,
                      const char *     aVendorData,
                      otJoinerCallback aCallback,
                      void *           aContext)
{
    Pre_OtCmdProcessing();
    otJoinerCb = aCallback;
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JOINER_START;

    p_ot_req->Size=7;
    p_ot_req->Data[0] = (uint32_t) aPskd;
    p_ot_req->Data[1] = (uint32_t) aProvisioningUrl;
    p_ot_req->Data[2] = (uint32_t) aVendorName;
    p_ot_req->Data[3] = (uint32_t) aVendorModel;
    p_ot_req->Data[4] = (uint32_t) aVendorSwVersion;
    p_ot_req->Data[5] = (uint32_t) aVendorData;
    p_ot_req->Data[6] = (uint32_t) aContext;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

void otJoinerStop(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JOINER_STOP;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otJoinerState otJoinerGetState(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JOINER_GET_STATE;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otJoinerState)p_ot_req->Data[0];
}

void otJoinerGetId(otInstance *aInstance, otExtAddress *aJoinerId)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JOINER_GET_ID;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aJoinerId;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}
#endif // OPENTHREAD_CONFIG_JOINER_ENABLE
