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

#include "netdiag.h"

#if OPENTHREAD_CONFIG_TMF_NETDIAG_CLIENT_ENABLE

extern otReceiveDiagnosticGetCallback otReceiveDiagnosticGetCb;

otError otThreadGetNextDiagnosticTlv(const otMessage *      aMessage,
                                     otNetworkDiagIterator *aIterator,
                                     otNetworkDiagTlv *     aNetworkDiagTlv)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otThreadSendDiagnosticGet(otInstance *                   aInstance,
                                  const otIp6Address *           aDestination,
                                  const uint8_t                  aTlvTypes[],
                                  uint8_t                        aCount,
                                  otReceiveDiagnosticGetCallback aCallback,
                                  void *                         aCallbackContext)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otThreadSendDiagnosticReset(otInstance *        aInstance,
                                    const otIp6Address *aDestination,
                                    const uint8_t       aTlvTypes[],
                                    uint8_t             aCount)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif // OPENTHREAD_CONFIG_TMF_NETDIAG_CLIENT_ENABLE

#ifndef OPENTHREAD_RCP
const char *otThreadGetVendorName(otInstance *aInstance)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_VENDOR_NAME;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const char *otThreadGetVendorModel(otInstance *aInstance)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_VENDOR_MODEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const char *otThreadGetVendorSwVersion(otInstance *aInstance)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_VENDOR_SW_VERSION;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const char *otThreadGetVendorAppUrl(otInstance *aInstance)
{
  const char * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_GET_VENDOR_APP_URL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif
#ifdef OPENTHREAD_CONFIG_NET_DIAG_VENDOR_INFO_SET_API_ENABLE

otError otThreadSetVendorName(otInstance *aInstance, const char *aVendorName)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_VENDOR_NAME;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aVendorName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otThreadSetVendorModel(otInstance *aInstance, const char *aVendorModel)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_VENDOR_MODEL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aVendorModel;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otThreadSetVendorSwVersion(otInstance *aInstance, const char *aVendorSwVersion)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_VENDOR_SW_VERSION;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aVendorSwVersion;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}


otError otThreadSetVendorAppUrl(otInstance *aInstance, const char *aVendorAppUrl)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_THREAD_SET_VENDOR_APP_URL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aVendorAppUrl;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif

otChannelMask otThreadGetNonPreferredChannels(otInstance *aInstance)
{
  
  otChannelMask rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4ToM0_OT_THREAD_GET_NON_PREFERED_CHANNEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otChannelMask)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otThreadSetNonPreferredChannels(otInstance *aInstance, otChannelMask aChannelMask)
{
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4ToM0_OT_THREAD_GET_NON_PREFERED_CHANNEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  
  Post_OtCmdProcessing();
  
}