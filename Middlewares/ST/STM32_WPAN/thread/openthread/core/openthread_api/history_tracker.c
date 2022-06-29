/**
  ******************************************************************************
  * @file    history_tracker.c
  * @author  MCD Application Team
  * @brief   This file contains the OpenThread History Tracker API shared
  * between M0 and M4.
  ******************************************************************************
  * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

#if OPENTHREAD_CONFIG_HISTORY_TRACKER_ENABLE

#include "history_tracker.h"

void otHistoryTrackerInitIterator(otHistoryTrackerIterator *aIterator)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_INIT_ITERATOR;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIterator;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

const otHistoryTrackerNetworkInfo *otHistoryTrackerIterateNetInfoHistory(otInstance *              aInstance,
                                                                         otHistoryTrackerIterator *aIterator,
                                                                         uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_NET_INFO_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerNetworkInfo *)p_ot_req->Data[0];
}

const otHistoryTrackerUnicastAddressInfo *otHistoryTrackerIterateUnicastAddressHistory(
    otInstance *              aInstance,
    otHistoryTrackerIterator *aIterator,
    uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_UNICAST_ADDRESS_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerUnicastAddressInfo *)p_ot_req->Data[0];
}

const otHistoryTrackerMulticastAddressInfo *otHistoryTrackerIterateMulticastAddressHistory(
    otInstance *              aInstance,
    otHistoryTrackerIterator *aIterator,
    uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_MULTICAST_ADDRESS_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerMulticastAddressInfo *)p_ot_req->Data[0];
}

const otHistoryTrackerMessageInfo *otHistoryTrackerIterateRxHistory(otInstance *              aInstance,
                                                                    otHistoryTrackerIterator *aIterator,
                                                                    uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_RX_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerMessageInfo *)p_ot_req->Data[0];
}

const otHistoryTrackerMessageInfo *otHistoryTrackerIterateTxHistory(otInstance *              aInstance,
                                                                    otHistoryTrackerIterator *aIterator,
                                                                    uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_TX_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerMessageInfo *)p_ot_req->Data[0];
}

const otHistoryTrackerNeighborInfo *otHistoryTrackerIterateNeighborHistory(otInstance *              aInstance,
                                                                           otHistoryTrackerIterator *aIterator,
                                                                           uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_NEIGHBOR_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerNeighborInfo *)p_ot_req->Data[0];
}

const otHistoryTrackerOnMeshPrefixInfo *otHistoryTrackerIterateOnMeshPrefixHistory(otInstance *              aInstance,
                                                                                   otHistoryTrackerIterator *aIterator,
                                                                                   uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_ON_MESH_PREFIX_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerOnMeshPrefixInfo *)p_ot_req->Data[0];
}

const otHistoryTrackerExternalRouteInfo *otHistoryTrackerIterateExternalRouteHistory(
    otInstance *              aInstance,
    otHistoryTrackerIterator *aIterator,
    uint32_t *                aEntryAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ITERATE_EXTERNAL_ROUTE_HISTORY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aEntryAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otHistoryTrackerExternalRouteInfo *)p_ot_req->Data[0];
}

void otHistoryTrackerEntryAgeToString(uint32_t aEntryAge, char *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_HISTORY_TRACKER_ENTRY_AGE_TO_STRING;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEntryAge;
  p_ot_req->Data[1] = (uint32_t) aBuffer;
  p_ot_req->Data[2] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#endif // OPENTHREAD_CONFIG_HISTORY_TRACKER_ENABLE

