/**
  ******************************************************************************
  * @file    dataset_ftd.c
  * @author  MCD Application Team
  * @brief   This file contains the Operational Dataset (FTD only) interface
  *          shared between M0 and M4.
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

#if OPENTHREAD_FTD

#include "dataset_ftd.h"

otError otDatasetCreateNewNetwork(otInstance *aInstance, otOperationalDataset *aDataset)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_CREATE_NEW_NETWORK;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aDataset;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

uint32_t otDatasetGetDelayTimerMinimal(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_GET_DELAY_TIMER_MINIMAL;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint32_t)p_ot_req->Data[0];
}

otError otDatasetSetDelayTimerMinimal(otInstance *aInstance, uint32_t aDelayTimerMinimal)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SET_DELAY_TIMER_MINIMAL;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aDelayTimerMinimal;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

#endif //OPENTHREAD_FTD
