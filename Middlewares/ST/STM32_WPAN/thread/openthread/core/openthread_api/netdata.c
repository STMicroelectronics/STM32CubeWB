/**
  ******************************************************************************
  * @file    netdata.c
  * @author  MCD Application Team
  * @brief   This file contains the netdata interface shared between M0 and
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

#include "netdata.h"


otError otNetDataGet(otInstance *aInstance, bool aStable, uint8_t *aData,
                                  uint8_t *aDataLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t) aStable;
    p_ot_req->Data[1] = (uint32_t) aData;
    p_ot_req->Data[2] = (uint32_t) aDataLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otNetDataGetNextOnMeshPrefix(otInstance *aInstance, otNetworkDataIterator *aIterator,
                                                  otBorderRouterConfig *aConfig)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_NEXT_ON_MESH_PREFIX;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aIterator;
    p_ot_req->Data[1] = (uint32_t) aConfig;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otNetDataGetNextRoute(otInstance *aInstance, otNetworkDataIterator *aIterator,
                              otExternalRouteConfig *aConfig)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_NEXT_ROUTE;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aIterator;
    p_ot_req->Data[1] = (uint32_t) aConfig;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

uint8_t otNetDataGetVersion(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_VERSION;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint8_t)p_ot_req->Data[0];
}

uint8_t otNetDataGetStableVersion(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_NET_DATA_GET_STABLE_VERSION;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint8_t)p_ot_req->Data[0];
}
