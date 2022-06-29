/**
 ******************************************************************************
 * @file    netdata.c
 * @author  MCD Application Team
 * @brief   This file contains the netdiag interface shared between M0 and
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

#if OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE

#include "netdiag.h"

extern otReceiveDiagnosticGetCallback otReceiveDiagnosticGetCb;

otError otThreadGetNextDiagnosticTlv(const otMessage *      aMessage,
                                     otNetworkDiagIterator *aIterator,
                                     otNetworkDiagTlv *     aNetworkDiagTlv)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_NEXT_DIAGNOSTIC_TLV;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aIterator;
  p_ot_req->Data[2] = (uint32_t) aNetworkDiagTlv;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otThreadSendDiagnosticGet(otInstance *                   aInstance,
                                  const otIp6Address *           aDestination,
                                  const uint8_t                  aTlvTypes[],
                                  uint8_t                        aCount,
                                  otReceiveDiagnosticGetCallback aCallback,
                                  void *                         aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otReceiveDiagnosticGetCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SEND_DIAGNOSTIC_GET;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aDestination;
  p_ot_req->Data[1] = (uint32_t) aTlvTypes;
  p_ot_req->Data[2] = (uint32_t) aCount;
  p_ot_req->Data[3] = (uint32_t) aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otThreadSendDiagnosticReset(otInstance *        aInstance,
                                    const otIp6Address *aDestination,
                                    const uint8_t       aTlvTypes[],
                                    uint8_t             aCount)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SEND_DIAGNOSTIC_RESET;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aDestination;
  p_ot_req->Data[1] = (uint32_t) aTlvTypes;
  p_ot_req->Data[2] = (uint32_t) aCount;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#endif // OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE
