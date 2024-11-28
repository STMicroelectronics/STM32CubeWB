/**
 ******************************************************************************
 * @file    link.c
 * @author  MCD Application Team
 * @brief   This file contains the  OpenThread IEEE 802.15.4 Link Layer interface
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

#include "thread.h"
#include <string.h>

#ifdef ENABLE_OPENTHREAD_CLI
#include "radio_link.h"
#endif

extern otHandleActiveScanResult otHandleActiveScanResultCb;
extern otHandleEnergyScanResult otHandleEnergyScanResultCb;
extern otLinkPcapCallback otLinkPcapCb;


otError otLinkActiveScan(otInstance *             aInstance,
    uint32_t                 aScanChannels,
    uint16_t                 aScanDuration,
    otHandleActiveScanResult aCallback,
    void *                   aCallbackContext)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otLinkIsActiveScanInProgress(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_ACTIVE_SCAN_IN_PROGRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkEnergyScan(otInstance *             aInstance,
    uint32_t                 aScanChannels,
    uint16_t                 aScanDuration,
    otHandleEnergyScanResult aCallback,
    void *                   aCallbackContext)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otLinkIsEnergyScanInProgress(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_ENERGY_SCAN_IN_PROGRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSendDataRequest(otInstance *aInstance)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SEND_DATA_REQUEST;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otLinkIsInTransmitState(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_IN_TRANSMIT_STATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint8_t otLinkGetChannel(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_CHANNEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetChannel(otInstance *aInstance, uint8_t aChannel)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_CHANNEL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aChannel;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otLinkGetSupportedChannelMask(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_SUPPORTED_CHANNEL_MASK;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetSupportedChannelMask(otInstance *aInstance, uint32_t aChannelMask)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_SUPPORTED_CHANNEL_MASK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aChannelMask;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otExtAddress *otLinkGetExtendedAddress(otInstance *aInstance)
{
  const otExtAddress * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_EXTENDED_ADDRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otExtAddress *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_EXTENDED_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
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

  Post_OtCmdProcessing();
}

otPanId otLinkGetPanId(otInstance *aInstance)
{
  otPanId rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_PANID;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otPanId)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetPanId(otInstance *aInstance, otPanId aPanId)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_PANID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPanId;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otLinkGetPollPeriod(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_POLL_PERIOD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetPollPeriod(otInstance *aInstance, uint32_t aPollPeriod)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_POLL_PERIOD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPollPeriod;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otShortAddress otLinkGetShortAddress(otInstance *aInstance)
{
  otShortAddress rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_SHORT_ADDRESS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otShortAddress)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint8_t otLinkGetMaxFrameRetriesDirect(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_MAX_FRAME_RETRIES_DIRECT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
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

  Post_OtCmdProcessing();
}

uint8_t otLinkGetMaxFrameRetriesIndirect(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_MAX_FRAME_RETRIES_INDIRECT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
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

  Post_OtCmdProcessing();
}

#if OPENTHREAD_CONFIG_MAC_FILTER_ENABLE
otMacFilterAddressMode otLinkFilterGetAddressMode(otInstance *aInstance)
{
  otMacFilterAddressMode rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_GET_ADRESS_MODE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otMacFilterAddressMode)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otLinkFilterSetAddressMode(otInstance *aInstance, otMacFilterAddressMode aMode)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_SET_ADDRESS_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aMode;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

otError otLinkFilterAddAddress(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_ADD_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otLinkFilterRemoveAddress(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_REMOVE_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otLinkFilterClearAddresses(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_CLEAR_ADDRESSES;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

otError otLinkFilterGetNextAddress(otInstance *aInstance, otMacFilterIterator *aIterator, otMacFilterEntry *aEntry)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_GET_NEXT_ADDRESS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntry;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkFilterAddRssIn(otInstance *aInstance, const otExtAddress *aExtAddress, int8_t aRss)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_ADD_RSS_IN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;
  p_ot_req->Data[1] = (uint32_t) aRss;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otLinkFilterRemoveRssIn(otInstance *aInstance, const otExtAddress *aExtAddress)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_REMOVE_RSS_IN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otLinkFilterSetDefaultRssIn(otInstance *aInstance, int8_t aRss)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_SET_DEFAULT_RSS_IN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aRss;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otLinkFilterClearDefaultRssIn(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_CLEAR_DEFAULT_RSS_IN;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otLinkFilterClearAllRssIn(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_FILTER_CLEAR_ALL_RSS_IN;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

otError otLinkFilterGetNextRssIn(otInstance *aInstance,otMacFilterIterator *aIterator, otMacFilterEntry *aEntry)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_NEXT_RSS_IN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntry;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_RADIO_LINK_IEEE_802_15_4_ENABLE
void otLinkSetRadioFilterEnabled(otInstance *aInstance, bool aFilterEnabled)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_RADIO_FILTER_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aFilterEnabled;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

bool otLinkIsRadioFilterEnabled(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_RADIO_FILTER_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif

#endif // OPENTHREAD_CONFIG_MAC_FILTER_ENABLE

uint8_t otLinkConvertRssToLinkQuality(otInstance *aInstance, int8_t aRss)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CONVERT_RSS_TO_LINK_QUALITY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aRss;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

int8_t otLinkConvertLinkQualityToRss(otInstance *aInstance, uint8_t aLinkQuality)
{
  int8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CONVERT_LINK_QUALITY_TO_RSS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aLinkQuality;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (int8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_MAC_RETRY_SUCCESS_HISTOGRAM_ENABLE
const uint32_t *otLinkGetTxDirectRetrySuccessHistogram(otInstance *aInstance, uint8_t *aNumberOfEntries)
{
  const uint32_t * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_TX_DIRECT_RETRY_SUCCESS_HISTOGRAM;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aNumberOfEntries;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const uint32_t *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const uint32_t *otLinkGetTxIndirectRetrySuccessHistogram(otInstance *aInstance, uint8_t *aNumberOfEntries)
{
  const uint32_t * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_TX_INDIRECT_RETRY_SUCCESS_HISTOGRAM;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aNumberOfEntries;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const uint32_t *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otLinkResetTxRetrySuccessHistogram(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RESET_TX_RETRY_SUCCESS_HISTOGRAM;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}
#endif // OPENTHREAD_CONFIG_MAC_RETRY_SUCCESS_HISTOGRAM_ENABLE

const otMacCounters *otLinkGetCounters(otInstance *aInstance)
{
  const otMacCounters * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otMacCounters *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otLinkResetCounters(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_RESET_COUNTERS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
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

  Post_OtCmdProcessing();
}

bool otLinkIsPromiscuous(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_PROMISCUOUS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetPromiscuous(otInstance *aInstance, bool aPromiscuous)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_PROMISCUOUS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aPromiscuous;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otLinkGetCcaFailureRate(otInstance *aInstance)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_GET_CCA_FAILURE_RATE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE
uint8_t otLinkGetCslChannel(otInstance *aInstance)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CSL_GET_CHANNEL;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetCslChannel(otInstance *aInstance, uint8_t aChannel)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CSL_SET_CHANNEL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aChannel;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otLinkGetCslPeriod(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CSL_GET_PERIOD;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetCslPeriod(otInstance *aInstance, uint32_t aPeriod)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CSL_SET_PERIOD;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = aPeriod;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint32_t otLinkGetCslTimeout(otInstance *aInstance)
{
  uint32_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CSL_GET_TIMEOUT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint32_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkSetCslTimeout(otInstance *aInstance, uint32_t aTimeout)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_CSL_SET_TIMEOUT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aTimeout;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif // OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE

#if OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE
otError otLinkSendEmptyData(otInstance *aInstance)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SEND_EMPTY_DATA;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif

otError otLinkSetEnabled(otInstance *aInstance, bool aEnable)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_SET_ENABLED;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aEnable;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otLinkIsEnabled(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_IS_ENABLED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otLinkGetRegion(otInstance *aInstance, uint16_t *aRegionCode)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}

otError otLinkSetRegion(otInstance *aInstance, uint16_t aRegionCode)
{
  return OT_ERROR_NOT_IMPLEMENTED;
}
