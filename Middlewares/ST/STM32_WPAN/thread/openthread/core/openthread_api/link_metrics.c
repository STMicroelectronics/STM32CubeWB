/**
 ******************************************************************************
 * @file    link_metrics.c
 * @author  MCD Application Team
 * @brief   This file contains the  OpenThread IEEE 802.15.4 Link Metrics Layer
 *          interface shared between M0 and M4.
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


#include <string.h>

#if OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE || OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE

#include "link_metrics.h"

//#include "common/instance.hpp"
//#include "net/ip6_address.hpp"

extern otLinkMetricsReportCallback otLinkMetricsReportCb;
extern otLinkMetricsMgmtResponseCallback otLinkMetricsMgmtResponseCb;

extern otLinkMetricsMgmtResponseCallback otLinkMetricsMgmtResponseCb1;
extern otLinkMetricsEnhAckProbingIeReportCallback otLinkMetricsEnhAckProbingIeReportCb;

otError otLinkMetricsQuery(otInstance *                aInstance,
                           const otIp6Address *        aDestination,
                           uint8_t                     aSeriesId,
                           const otLinkMetrics *       aLinkMetricsFlags,
                           otLinkMetricsReportCallback aCallback,
                           void *                      aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otLinkMetricsReportCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_METRICS_QUERY;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t)aDestination;
  p_ot_req->Data[1] = (uint32_t)aSeriesId;
  p_ot_req->Data[2] = (uint32_t)aLinkMetricsFlags;
  p_ot_req->Data[3] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkMetricsConfigForwardTrackingSeries(otInstance *                      aInstance,
                                                 const otIp6Address *              aDestination,
                                                 uint8_t                           aSeriesId,
                                                 const otLinkMetricsSeriesFlags    aSeriesFlags,
                                                 const otLinkMetrics *             aLinkMetricsFlags,
                                                 otLinkMetricsMgmtResponseCallback aCallback,
                                                 void *                            aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otLinkMetricsMgmtResponseCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_METRICS_CONFIG_FORWARD_TRACKING_SERIES;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t)aDestination;
  p_ot_req->Data[1] = (uint32_t)aSeriesId;
  p_ot_req->Data[2] = (uint32_t)(aSeriesFlags.mLinkProbe + (aSeriesFlags.mMacData >> 8) + (aSeriesFlags.mMacDataRequest >> 16) + (aSeriesFlags.mMacAck >> 24));
  p_ot_req->Data[3] = (uint32_t)aLinkMetricsFlags;
  p_ot_req->Data[4] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#if OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE
otError otLinkMetricsConfigEnhAckProbing(otInstance *                               aInstance,
                                         const otIp6Address *                       aDestination,
                                         otLinkMetricsEnhAckFlags                   aEnhAckFlags,
                                         const otLinkMetrics *                      aLinkMetricsFlags,
                                         otLinkMetricsMgmtResponseCallback          aCallback,
                                         void *                                     aCallbackContext,
                                         otLinkMetricsEnhAckProbingIeReportCallback aEnhAckCallback,
                                         void *                                     aEnhAckCallbackContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otLinkMetricsMgmtResponseCb1 = aCallback;
  otLinkMetricsEnhAckProbingIeReportCb = aEnhAckCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_METRICS_CONFIG_ENH_ACK_PROBING;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t)aDestination;
  p_ot_req->Data[1] = (uint32_t)aEnhAckFlags;
  p_ot_req->Data[2] = (uint32_t)aLinkMetricsFlags;
  p_ot_req->Data[3] = (uint32_t)aCallbackContext;
  p_ot_req->Data[4] = (uint32_t)aEnhAckCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otLinkMetricsSendLinkProbe(otInstance *        aInstance,
                                   const otIp6Address *aDestination,
                                   uint8_t             aSeriesId,
                                   uint8_t             aLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_LINK_METRICS_SEND_LINK_PROBE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aDestination;
  p_ot_req->Data[1] = (uint32_t)aSeriesId;
  p_ot_req->Data[2] = (uint32_t)aLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif

#endif // OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE || OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE
