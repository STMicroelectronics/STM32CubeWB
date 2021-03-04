/**
 ******************************************************************************
 * @file    link.c
 * @author  MCD Application Team
 * @brief   This file contains the  OpenThread IEEE 802.15.4 Link Layer interface
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

#include "thread.h"
#include <string.h>

extern otHandleActiveScanResult otHandleActiveScanResultCb;
extern otHandleEnergyScanResult otHandleEnergyScanResultCb;
extern otLinkPcapCallback otLinkPcapCb;


otError otLinkActiveScan(otInstance *             aInstance,
                         uint32_t                 aScanChannels,
                         uint16_t                 aScanDuration,
                         otHandleActiveScanResult aCallback,
                         void *                   aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* Save Callback to global variable */
  otHandleActiveScanResultCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();


  p_ot_req->ID = MSG_M4TOM0_OT_LINK_ACTIVE_SCAN;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aScanChannels;
  p_ot_req->Data[1] = (uint32_t)aScanDuration;
  p_ot_req->Data[2] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otLinkIsActiveScanInProgress(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_ACTIVE_SCAN_IN_PROGRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otLinkEnergyScan(otInstance *             aInstance,
                         uint32_t                 aScanChannels,
                         uint16_t                 aScanDuration,
                         otHandleEnergyScanResult aCallback,
                         void *                   aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* Save Callback to global variable */
  otHandleEnergyScanResultCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_ENERGY_SCAN;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aScanChannels;
  p_ot_req->Data[1] = (uint32_t)aScanDuration;
  p_ot_req->Data[2] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otLinkIsEnergyScanInProgress(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_ENERGY_SCAN_IN_PROGRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otLinkSendDataRequest(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SEND_DATA_REQUEST;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otLinkIsInTransmitState(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_IN_TRANSMIT_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otLinkOutOfBandTransmitRequest(otInstance *aInstance, otRadioFrame *aOobFrame)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_OUT_OF_BAND_TRANSMIT_REQUEST;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aOobFrame;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint8_t otLinkGetChannel(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_CHANNEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

otError otLinkSetChannel(otInstance *aInstance, uint8_t aChannel)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_CHANNEL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aChannel;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint32_t otLinkGetSupportedChannelMask(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_SUPPORTED_CHANNEL_MASK;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

otError otLinkSetSupportedChannelMask(otInstance *aInstance, uint32_t aChannelMask)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_SUPPORTED_CHANNEL_MASK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aChannelMask;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

const otExtAddress *otLinkGetExtendedAddress(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_EXTENDED_ADDRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otExtAddress *) p_ot_req->Data[0];
}

otError otLinkSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_EXTENDED_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otLinkGetFactoryAssignedIeeeEui64(otInstance *aInstance, otExtAddress *aEui64)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_FACTORY_ASSIGNED_EUI64;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEui64;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otPanId otLinkGetPanId(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_PANID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otPanId)p_ot_req->Data[0];
}

otError otLinkSetPanId(otInstance *aInstance, otPanId aPanId)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_PANID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPanId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint32_t otLinkGetPollPeriod(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_POLL_PERIOD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

otError otLinkSetPollPeriod(otInstance *aInstance, uint32_t aPollPeriod)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_POLL_PERIOD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPollPeriod;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otShortAddress otLinkGetShortAddress(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_SHORT_ADDRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otShortAddress)p_ot_req->Data[0];
}

uint8_t otLinkGetMaxFrameRetriesDirect(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_MAX_FRAME_RETRIES_DIRECT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otShortAddress)p_ot_req->Data[0];
}

void otLinkSetMaxFrameRetriesDirect(otInstance *aInstance, uint8_t aMaxFrameRetriesDirect)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_MAX_FRAME_RETRIES_DIRECT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aMaxFrameRetriesDirect;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint8_t otLinkGetMaxFrameRetriesIndirect(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_MAX_FRAME_RETRIES_INDIRECT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otShortAddress)p_ot_req->Data[0];
}

void otLinkSetMaxFrameRetriesIndirect(otInstance *aInstance, uint8_t aMaxFrameRetriesIndirect)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_MAX_FRAME_RETRIES_INDIRECT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aMaxFrameRetriesIndirect;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otMacFilterAddressMode otLinkFilterGetAddressMode(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_GET_ADRESS_MODE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otMacFilterAddressMode)p_ot_req->Data[0];
}

otError otLinkFilterSetAddressMode(otInstance *aInstance, otMacFilterAddressMode aMode)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_SET_ADDRESS_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aMode;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkFilterAddAddress(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_ADD_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkFilterRemoveAddress(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_REMOVE_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otLinkFilterClearAddresses(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_CLEAR_ADDRESSES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otLinkFilterGetNextAddress(otInstance *aInstance, otMacFilterIterator *aIterator, otMacFilterEntry *aEntry)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_GET_NEXT_ADDRESS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntry;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkFilterAddRssIn(otInstance *aInstance, const otExtAddress *aExtAddress, int8_t aRss)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_ADD_RSS_IN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;
  p_ot_req->Data[1] = (uint32_t) aRss;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otLinkFilterRemoveRssIn(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_REMOVE_RSS_IN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

void otLinkFilterClearRssIn(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_CLEAR_RSS_IN;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otLinkFilterGetNextRssIn(otInstance *aInstance,otMacFilterIterator *aIterator, otMacFilterEntry *aEntry)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_NEXT_RSS_IN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntry;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

uint8_t otLinkConvertRssToLinkQuality(otInstance *aInstance, int8_t aRss)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CONVERT_RSS_TO_LINK_QUALITY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aRss;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t) p_ot_req->Data[0];
}

int8_t otLinkConvertLinkQualityToRss(otInstance *aInstance, uint8_t aLinkQuality)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CONVERT_LINK_QUALITY_TO_RSS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aLinkQuality;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t) p_ot_req->Data[0];
}

const otMacCounters *otLinkGetCounters(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otMacCounters *)p_ot_req->Data[0];
}

void otLinkResetCounters(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RESET_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otLinkSetPcapCallback(otInstance *aInstance, otLinkPcapCallback aPcapCallback, void *aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* Save Callback to global variable */
  otLinkPcapCb = aPcapCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_PCAP_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

bool otLinkIsPromiscuous(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_PROMISCUOUS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otLinkSetPromiscuous(otInstance *aInstance, bool aPromiscuous)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_PROMISCUOUS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPromiscuous;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

uint16_t otLinkGetCcaFailureRate(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_CCA_FAILURE_RATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

otError otLinkSetEnabled(otInstance *aInstance, bool aEnable)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnable;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

bool otLinkIsEnabled(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}
