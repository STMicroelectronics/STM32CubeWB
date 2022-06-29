/**
  ******************************************************************************
  * @file    dataset_ftd.c
  * @author  MCD Application Team
  * @brief   This file contains the Operational Dataset (FTD only) interface
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

#if OPENTHREAD_CONFIG_DATASET_UPDATER_ENABLE && OPENTHREAD_FTD

#include "dataset_updater.h"

extern otDatasetUpdaterCallback otDatasetUpdaterCb;

otError otDatasetUpdaterRequestUpdate(otInstance *                aInstance,
                                      const otOperationalDataset *aDataset,
                                      otDatasetUpdaterCallback    aCallback,
                                      void *                      aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otDatasetUpdaterCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_UPDATER_REQUEST_UPDATE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aDataset;
  p_ot_req->Data[1] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otDatasetUpdaterCancelUpdate(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_UPDATER_CANCEL_UPDATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

bool otDatasetUpdaterIsUpdateOngoing(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_UPDATER_IS_UPDATE_ON_GOING;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}


#endif // OPENTHREAD_CONFIG_DATASET_UPDATER_ENABLE && OPENTHREAD_FTD

