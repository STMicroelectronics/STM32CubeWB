/**
 ******************************************************************************
 * @file    link_raw.c
 * @author  MCD Application Team
 * @brief   This file contains the link_raw interface shared between M0 and
 *          M4.
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

#include "link_raw.h"

extern otLinkRawReceiveDone otLinkRawReceiveDoneCb;
extern otLinkRawTransmitDone otLinkRawTransmitDoneCb;
extern otLinkRawEnergyScanDone otLinkRawEnergyScanDoneCb;


#if OPENTHREAD_RADIO || OPENTHREAD_CONFIG_LINK_RAW_ENABLE

otError otLinkRawSetEnable(otInstance *aInstance, bool aEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SET_ENABLE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnabled;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otLinkRawIsEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_IS_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

bool otLinkRawGetPromiscuous(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_GET_PROMISCUOUS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otLinkRawSetPromiscuous(otInstance *aInstance, bool aEnable)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SET_PROMISCUOUS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnable;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSetShortAddress(otInstance *aInstance, uint16_t aShortAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SET_SHORT_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aShortAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSleep(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SLEEP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawReceive(otInstance *aInstance, otLinkRawReceiveDone aCallback)
{
  Pre_OtCmdProcessing();
  otLinkRawReceiveDoneCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_RECEIVE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otRadioFrame *otLinkRawGetTransmitBuffer(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_GET_TRANSMIT_BUFFER;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otRadioFrame*)p_ot_req->Data[0];
}

otError otLinkRawTransmit(otInstance *aInstance, otRadioFrame *aFrame, otLinkRawTransmitDone aCallback)
{
  Pre_OtCmdProcessing();
  otLinkRawTransmitDoneCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_TRANSMIT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aFrame;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

int8_t otLinkRawGetRssi(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_GET_RSSI;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (int8_t)p_ot_req->Data[0];
}

otRadioCaps otLinkRawGetCaps(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_GET_CAPS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otRadioCaps)p_ot_req->Data[0];
}

otError otLinkRawEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration,
    otLinkRawEnergyScanDone aCallback)
{
  Pre_OtCmdProcessing();
  otLinkRawEnergyScanDoneCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_ENERGY_SCAN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aScanChannel;
  p_ot_req->Data[1] = (uint32_t) aScanDuration;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSrcMatchEnable(otInstance *aInstance, bool aEnable)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SRC_MATCH_ENABLE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEnable;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSrcMatchAddShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SRC_MATCH_ADD_SHORT_ENTRY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aShortAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSrcMatchAddExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SRC_MATCH_ADD_EXT_ENTRY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSrcMatchClearShortEntry(otInstance *aInstance, const uint16_t aShortAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SRC_MATCH_CLEAR_SHORT_ENTRY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aShortAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSrcMatchClearExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SRC_MATCH_CLEAR_EXT_ENTRY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSrcMatchClearShortEntries(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SRC_MATCH_CLEAR_SHORT_ENTRIES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkRawSrcMatchClearExtEntries(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RAW_SRC_MATCH_CLEAR_EXT_ENTRIES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#endif /* OPENTHREAD_RADIO || OPENTHREAD_CONFIG_LINK_RAW_ENABLE */
