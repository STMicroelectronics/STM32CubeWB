/**
  ******************************************************************************
  * @file    dataset.c
  * @author  MCD Application Team
  * @brief   This file contains the Dataset interface shared between M0 and M4.
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

#include "dataset.h"


bool otDatasetIsCommissioned(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_IS_COMMISSIONED;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (bool)p_ot_req->Data[0];
}

otError otDatasetGetActive(otInstance *aInstance, otOperationalDataset *aDataset)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_GET_ACTIVE;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aDataset;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otDatasetSetActive(otInstance *aInstance, const otOperationalDataset *aDataset)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SET_ACTIVE;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aDataset;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otDatasetGetPending(otInstance *aInstance, otOperationalDataset *aDataset)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_GET_PENDING;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aDataset;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otDatasetSetPending(otInstance *aInstance, const otOperationalDataset *aDataset)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SET_PENDING;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aDataset;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otDatasetSendMgmtActiveGet(otInstance *aInstance,
                                                const otOperationalDatasetComponents *aDatasetComponents,
                                                const uint8_t *aTlvTypes,
                                                uint8_t aLength,
                                                const otIp6Address *aAddress)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SEND_MGMT_ACTIVE_GET;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aDatasetComponents;
    p_ot_req->Data[1] = (uint32_t) aTlvTypes;
    p_ot_req->Data[2] = (uint32_t) aLength;
    p_ot_req->Data[3] = (uint32_t) aAddress;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otDatasetSendMgmtActiveSet(otInstance *aInstance, const otOperationalDataset *aDataset,
                                                const uint8_t *aTlvs, uint8_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SEND_MGMT_ACTIVE_SET;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t) aDataset;
    p_ot_req->Data[1] = (uint32_t) aTlvs;
    p_ot_req->Data[2] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otDatasetSendMgmtPendingGet(otInstance *aInstance,
                                                 const otOperationalDatasetComponents *aDatasetComponents,
                                                 const uint8_t *aTlvTypes,
                                                 uint8_t aLength,
                                                 const otIp6Address *aAddress)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SEND_MGMT_PENDING_GET;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aDatasetComponents;
    p_ot_req->Data[1] = (uint32_t) aTlvTypes;
    p_ot_req->Data[2] = (uint32_t) aLength;
    p_ot_req->Data[3] = (uint32_t) aAddress;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otDatasetSendMgmtPendingSet(otInstance *aInstance, const otOperationalDataset *aDataset,
                                                 const uint8_t *aTlvs, uint8_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SEND_MGMT_PENDING_SET;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t) aDataset;
    p_ot_req->Data[1] = (uint32_t) aTlvs;
    p_ot_req->Data[2] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

