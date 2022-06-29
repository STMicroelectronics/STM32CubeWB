/**
  ******************************************************************************
  * @file    server.c
  * @author  MCD Application Team
  * @brief   This file contains the Server interface shared between M0 and
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

#include "server.h"


#if OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE

otError otServerGetNetDataLocal(otInstance *aInstance, bool aStable, uint8_t *aData, uint8_t *aDataLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_SERVER_GET_NET_DATA_LOCAL;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t)aStable;
    p_ot_req->Data[1] = (uint32_t)aData;
    p_ot_req->Data[2] = (uint32_t)aDataLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otServerAddService(otInstance *aInstance, const otServiceConfig *aConfig)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_SERVER_ADD_SERVICE;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t)aConfig;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otServerRemoveService(otInstance *   aInstance,
                              uint32_t       aEnterpriseNumber,
                              const uint8_t *aServiceData,
                              uint8_t        aServiceDataLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_SERVER_REMOVE_SERVICE;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t)aEnterpriseNumber;
    p_ot_req->Data[1] = (uint32_t)aServiceData;
    p_ot_req->Data[2] = (uint32_t)aServiceDataLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otServerGetNextService(otInstance *aInstance, otNetworkDataIterator *aIterator, otServiceConfig *aConfig)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_SERVER_GET_NEXT_SERVICE;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t)aIterator;
    p_ot_req->Data[1] = (uint32_t)aConfig;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otServerRegister(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_SERVER_REGISTER;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

#endif /* OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE */
