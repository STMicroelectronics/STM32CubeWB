/**
 ******************************************************************************
 * @file    dataset.c
 * @author  MCD Application Team
 * @brief   This file contains the Dataset interface shared between M0 and M4.
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

#include "dataset.h"

extern otDatasetMgmtSetCallback otDatasetMgmtSetActiveCb;
extern otDatasetMgmtSetCallback otDatasetMgmtSetPendingCb;

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

otError otDatasetGetActiveTlvs(otInstance *aInstance, otOperationalDatasetTlvs *aDataset)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_GET_ACTIVE_TLVS;

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

otError otDatasetSetActiveTlvs(otInstance *aInstance, const otOperationalDatasetTlvs *aDataset)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SET_ACTIVE_TLVS;

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

otError otDatasetGetPendingTlvs(otInstance *aInstance, otOperationalDatasetTlvs *aDataset)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_GET_PENDING_TLVS;

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

otError otDatasetSetPendingTlvs(otInstance *aInstance, const otOperationalDatasetTlvs *aDataset)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SET_PENDING_TLVS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDataset;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDatasetSendMgmtActiveGet(otInstance *                          aInstance,
                                   const otOperationalDatasetComponents *aDatasetComponents,
                                   const uint8_t *                       aTlvTypes,
                                   uint8_t                               aLength,
                                   const otIp6Address *                  aAddress)
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

otError otDatasetSendMgmtActiveSet(otInstance *                aInstance,
                                   const otOperationalDataset *aDataset,
                                   const uint8_t *             aTlvs,
                                   uint8_t                     aLength,
                                   otDatasetMgmtSetCallback    aCallback,
                                   void *                      aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otDatasetMgmtSetActiveCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SEND_MGMT_ACTIVE_SET;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aDataset;
  p_ot_req->Data[1] = (uint32_t) aTlvs;
  p_ot_req->Data[2] = (uint32_t) aLength;
  p_ot_req->Data[3] = (uint32_t) aContext;

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

otError otDatasetSendMgmtPendingSet(otInstance *                aInstance,
                                    const otOperationalDataset *aDataset,
                                    const uint8_t *             aTlvs,
                                    uint8_t                     aLength,
                                    otDatasetMgmtSetCallback    aCallback,
                                    void *                      aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otDatasetMgmtSetPendingCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_SEND_MGMT_PENDING_SET;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aDataset;
  p_ot_req->Data[1] = (uint32_t) aTlvs;
  p_ot_req->Data[2] = (uint32_t) aLength;
  p_ot_req->Data[3] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_FTD
otError otDatasetGeneratePskc(const char *           aPassPhrase,
                              const otNetworkName *  aNetworkName,
                              const otExtendedPanId *aExtPanId,
                              otPskc *               aPskc)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_GENERATE_PSKC;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aPassPhrase;
  p_ot_req->Data[1] = (uint32_t) aNetworkName;
  p_ot_req->Data[2] = (uint32_t) aExtPanId;
  p_ot_req->Data[3] = (uint32_t) aPskc;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];}
#endif // OPENTHREAD_FTD

otError otNetworkNameFromString(otNetworkName *aNetworkName, const char *aNameString)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_NETWORK_NAME_FROM_STRING;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aNetworkName;
  p_ot_req->Data[1] = (uint32_t) aNameString;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otDatasetParseTlvs(const otOperationalDatasetTlvs *aDatasetTlvs, otOperationalDataset *aDataset)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_DATASET_PARSE_TLVS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aDatasetTlvs;
  p_ot_req->Data[1] = (uint32_t) aDataset;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

