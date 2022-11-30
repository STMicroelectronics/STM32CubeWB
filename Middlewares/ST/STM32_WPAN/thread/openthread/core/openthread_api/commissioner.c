/**
 ******************************************************************************
 * @file    commissioner.c
 * @author  MCD Application Team
 * @brief   This file contains the commissioner interface shared between M0 and
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

#include "thread.h"
#include "commissioner.h"

#if OPENTHREAD_FTD && OPENTHREAD_CONFIG_COMMISSIONER_ENABLE

extern otCommissionerStateCallback otCommissionerStateCb;
extern otCommissionerJoinerCallback otCommissionerJoinerCb;
extern otCommissionerEnergyReportCallback otCommissionerEnergyReportCb;
extern otCommissionerPanIdConflictCallback otCommissionerPanIdConflictCb;

otError otCommissionerStart(otInstance *                 aInstance,
                            otCommissionerStateCallback  aStateCallback,
                            otCommissionerJoinerCallback aJoinerCallback,
                            void *                       aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otCommissionerStateCb = aStateCallback;
  otCommissionerJoinerCb = aJoinerCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_START;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aCallbackContext;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

const char *otCommissionerGetId(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_ID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char *)p_ot_req->Data[0];
}

otError otCommissionerSetId(otInstance *aInstance, const char *aId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_SET_ID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerStop(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerAddJoiner(otInstance *        aInstance,
    const otExtAddress *aEui64,
    const char *        aPskd,
    uint32_t            aTimeout)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_ADD_JOINER;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aEui64;
  p_ot_req->Data[1] = (uint32_t) aPskd;
  p_ot_req->Data[2] = (uint32_t) aTimeout;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerAddJoinerWithDiscerner(otInstance *             aInstance,
                                             const otJoinerDiscerner *aDiscerner,
                                             const char *             aPskd,
                                             uint32_t                 aTimeout)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_ADD_JOINER_WITH_DISCERNER;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aDiscerner;
  p_ot_req->Data[1] = (uint32_t) aPskd;
  p_ot_req->Data[2] = (uint32_t) aTimeout;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerGetNextJoinerInfo(otInstance *aInstance, uint16_t *aIterator, otJoinerInfo *aJoiner)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_NEXT_JOINER_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aJoiner;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerRemoveJoiner(otInstance *aInstance, const otExtAddress *aEui64)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_REMOVE_JOINER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEui64;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerRemoveJoinerWithDiscerner(otInstance *aInstance, const otJoinerDiscerner *aDiscerner)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_REMOVE_JOINER_WITH_DISCERNER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aDiscerner;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

const char *otCommissionerGetProvisioningUrl(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_PROVISIONING_URL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

otError otCommissionerSetProvisioningUrl(otInstance *aInstance, const char *aProvisioningUrl)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_SET_PROVISIONING_URL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aProvisioningUrl;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerAnnounceBegin(otInstance *        aInstance,
                                    uint32_t            aChannelMask,
                                    uint8_t             aCount,
                                    uint16_t            aPeriod,
                                    const otIp6Address *aAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_ANNOUNCE_BEGIN;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aChannelMask;
  p_ot_req->Data[1] = (uint32_t) aCount;
  p_ot_req->Data[2] = (uint32_t) aPeriod;
  p_ot_req->Data[3] = (uint32_t) aAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerEnergyScan(otInstance *                       aInstance,
                                 uint32_t                           aChannelMask,
                                 uint8_t                            aCount,
                                 uint16_t                           aPeriod,
                                 uint16_t                           aScanDuration,
                                 const otIp6Address *               aAddress,
                                 otCommissionerEnergyReportCallback aCallback,
                                 void *                             aContext)
{
  Pre_OtCmdProcessing();
  otCommissionerEnergyReportCb = aCallback;

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_ENERGY_SCAN;

  p_ot_req->Size=6;
  p_ot_req->Data[0] = (uint32_t) aChannelMask;
  p_ot_req->Data[1] = (uint32_t) aCount;
  p_ot_req->Data[2] = (uint32_t) aPeriod;
  p_ot_req->Data[3] = (uint32_t) aScanDuration;
  p_ot_req->Data[4] = (uint32_t) aAddress;
  p_ot_req->Data[5] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerPanIdQuery(otInstance *                        aInstance,
                                 uint16_t                            aPanId,
                                 uint32_t                            aChannelMask,
                                 const otIp6Address *                aAddress,
                                 otCommissionerPanIdConflictCallback aCallback,
                                 void *                              aContext)
{
  Pre_OtCmdProcessing();
  otCommissionerPanIdConflictCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_PANID_QUERY;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aPanId;
  p_ot_req->Data[1] = (uint32_t) aChannelMask;
  p_ot_req->Data[2] = (uint32_t) aAddress;
  p_ot_req->Data[3] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerSendMgmtGet(otInstance *aInstance, const uint8_t *aTlvs, uint8_t aLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_SEND_MGMT_GET;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aTlvs;
  p_ot_req->Data[1] = (uint32_t) aLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCommissionerSendMgmtSet(otInstance *                  aInstance,
                                  const otCommissioningDataset *aDataset,
                                  const uint8_t *               aTlvs,
                                  uint8_t                       aLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_SEND_MGMT_SET;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aDataset;
  p_ot_req->Data[1] = (uint32_t) aTlvs;
  p_ot_req->Data[2] = (uint32_t) aLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint16_t otCommissionerGetSessionId(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_SESSION_ID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

otCommissionerState otCommissionerGetState(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otCommissionerState) p_ot_req->Data[0];
}

#endif /* OPENTHREAD_FTD && OPENTHREAD_CONFIG_COMMISSIONER_ENABLE */
