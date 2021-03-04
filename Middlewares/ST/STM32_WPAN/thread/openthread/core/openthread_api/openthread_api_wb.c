/*
 ******************************************************************************
 * @file    openthread_api_wb.c
 * @author  MCD Application Team
 * @brief   Contains STM32WB specificities requested to control the OpenThread
 *          interface.
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
#include "tl_thread_hci.h"

/* Include definition of compilation flags requested for OpenThread configuration */
#include OPENTHREAD_CONFIG_FILE

#include "tl.h"
#include "openthread_api_wb.h"
#include "dbg_trace.h"
#include "shci.h"


/* INSTANCE */
otStateChangedCallback otStateChangedCb = NULL;

/* IP6 */
#if OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
otIp6SlaacPrefixFilter otIp6SlaacPrefixFilterCb = NULL;
#endif // OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
otIp6ReceiveCallback otIp6ReceiveCb = NULL;
otIp6AddressCallback otIp6AddressCb = NULL;

/* LINK */
otHandleActiveScanResult otHandleActiveScanResultCb = NULL;
otHandleEnergyScanResult otHandleEnergyScanResultCb = NULL;
otLinkPcapCallback otLinkPcapCb = NULL;

/* THREAD */
otReceiveDiagnosticGetCallback otReceiveDiagnosticGetCb = NULL;
otThreadParentResponseCallback otThreadParentResponseCb = NULL;

#if OPENTHREAD_FTD
/* THREAD_FTD */
otNeighborTableCallback otNeighborTableCb = NULL;
#endif

/* COMMISSIONER */
otCommissionerStateCallback otCommissionerStateCb = NULL;
otCommissionerJoinerCallback otCommissionerJoinerCb = NULL;
otCommissionerEnergyReportCallback otCommissionerEnergyReportCb = NULL;
otCommissionerPanIdConflictCallback otCommissionerPanIdConflictCb = NULL;

/* DNS */
otDnsResponseHandler otDnsResponseHandlerCb = NULL;

/* ICMP6 */
otIcmp6ReceiveCallback otIcmp6ReceiveCb = NULL;

/* JOINER */
otJoinerCallback otJoinerCb = NULL;

/* LINK_RAW */
otLinkRawReceiveDone otLinkRawReceiveDoneCb = NULL;
otLinkRawTransmitDone otLinkRawTransmitDoneCb = NULL;
otLinkRawEnergyScanDone otLinkRawEnergyScanDoneCb = NULL;

/* UDP */
otUdpReceive otUdpReceiveCb = NULL;
#if OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE
otUdpForwardSetForwarder otUdpForwardSetForwarderCb = NULL;
#endif /* OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE */
/* COAP */
otCoapRequestHandler coapRequestHandlerCb = NULL;
otCoapRequestHandler defaultCoapRequestHandlerCb = NULL;
otCoapResponseHandler coapResponseHandlerCb = NULL;

STCoapRequestContextType* mySTCoapRequestContext;
STCoapResponseContextType* mySTCoapResponseContext;

/* COAP SECURE */
otCoapRequestHandler defaultCoapSecureRequestHandlerCb = NULL;
otHandleCoapSecureClientConnect coapSecureClientConnectCb = NULL;

STCoapSecureSpecificContextType* mySTCoapSecureContext;
otHandleCoapSecureClientConnect l_coapSecureClientConnectCb;

/* NETWORK TIME */
otNetworkTimeSyncCallbackFn otNetworkTimeSyncCb = NULL;

/* SNTP */
otSntpResponseHandler otSntpResponseHandlerCb = NULL;

#if OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE
/* JAM_DETECTION */
otJamDetectionCallback otJamDetectionCallbackCb = NULL;
#endif


/**
 * @brief  This function is used to manage all the callbacks used by the
 *         OpenThread interface. These callbacks are used for example to
 *         notify the application as soon as the state of a device has been
 *         modified.
 *
 *         Important Note: This function must be called each time a message
 *         is sent from the M0 to the M4.
 *
 * @param  None
 * @retval None
 */

HAL_StatusTypeDef OpenThread_CallBack_Processing(void)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Get pointer on received event buffer from M0 */
  Thread_OT_Cmd_Request_t* p_notification = THREAD_Get_NotificationPayloadBuffer();

  switch(p_notification->ID)
  {
  case MSG_M0TOM4_NOTIFY_STATE_CHANGE:
    if (otStateChangedCb != NULL)
    {
      otStateChangedCb((uint32_t) p_notification->Data[0],
          (void*) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_THREAD_PARENT_RESPONSE_HANDLER:
    if (otThreadParentResponseCb != NULL)
    {
      otThreadParentResponseCb((otThreadParentResponseInfo *) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
  case MSG_M0TOM4_COAP_REQUEST_HANDLER:
    mySTCoapRequestContext = (STCoapRequestContextType*) p_notification->Data[0];

    coapRequestHandlerCb = mySTCoapRequestContext->mHandler;

    if (coapRequestHandlerCb != NULL)
    {
      coapRequestHandlerCb(mySTCoapRequestContext->mContext,
          (otMessage *) p_notification->Data[1],
          (otMessageInfo *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_DEFAULT_COAP_REQUEST_HANDLER:
    if (defaultCoapRequestHandlerCb != NULL)
    {
      defaultCoapRequestHandlerCb((void *) p_notification->Data[0],
          (otMessage *) p_notification->Data[1],
          (otMessageInfo *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_COAP_RESPONSE_HANDLER:
    mySTCoapResponseContext = (STCoapResponseContextType*) p_notification->Data[0];
    coapResponseHandlerCb = mySTCoapResponseContext->mHandler;
    if (coapResponseHandlerCb != NULL)
    {
      coapResponseHandlerCb(mySTCoapResponseContext->mContext,
          (otMessage *) p_notification->Data[1],
          (otMessageInfo *) p_notification->Data[2],
          (otError) p_notification->Data[3]);
    }
    break;
  case MSG_M0TOM4_COAP_SECURE_CLIENT_CONNECT:
    mySTCoapSecureContext = (STCoapSecureSpecificContextType*) p_notification->Data[1];

    l_coapSecureClientConnectCb = mySTCoapSecureContext->mHandler;

    if (l_coapSecureClientConnectCb != NULL)
    {
      l_coapSecureClientConnectCb((bool) p_notification->Data[0],
          mySTCoapSecureContext->mContext);
    }
    break;
  case MSG_M0TOM4_COAP_SECURE_SET_CLIENT_CONNECT:
    if (coapSecureClientConnectCb != NULL)
    {
      coapSecureClientConnectCb((bool) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_COAP_SECURE_DEFAULT_REQUEST_HANDLER:
    if (defaultCoapSecureRequestHandlerCb != NULL)
    {
      defaultCoapSecureRequestHandlerCb((void *) p_notification->Data[0],
          (otMessage *) p_notification->Data[1],
          (otMessageInfo *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_NOTIFY_STACK_RESET:
    /* Store Thread NVM data in Flash*/
    SHCI_C2_FLASH_StoreData(THREAD_IP);
    /* Perform an NVIC Reset in order to reinitalize the device */
    HAL_NVIC_SystemReset();
    break;
  case MSG_M0TOM4_IP6_RECEIVE:
    if (otIp6ReceiveCb != NULL)
    {
      otIp6ReceiveCb((otMessage*) p_notification->Data[0],
          (void*) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_IP6_ADDRESS:
    if (otIp6AddressCb != NULL)
    {
      otIp6AddressCb((const otIp6Address *) p_notification->Data[0],
          (uint8_t) p_notification->Data[1],
          (bool) p_notification->Data[2],
          (void *) p_notification->Data[3]);
    }
    break;
#if OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
  case MSG_M0TOM4_IP6_SLAAC_PREFIX_FILTER:
    if (otIp6SlaacPrefixFilterCb != NULL)
    {
      /* Not passing otInstance as first parameter, because created on M0, passing NULL instead */
      otIp6SlaacPrefixFilterCb(NULL,
          (const otIp6Prefix *) p_notification->Data[0]);
    }
    break;
#endif // OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE
  case MSG_M0TOM4_HANDLE_ACTIVE_SCAN_RESULT:
    if (otHandleActiveScanResultCb != NULL)
    {
      otHandleActiveScanResultCb((otActiveScanResult*) p_notification->Data[0],
          (void*) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_HANDLE_ENERGY_SCAN_RESULT:
    if (otHandleEnergyScanResultCb != NULL)
    {
      otHandleEnergyScanResultCb((otEnergyScanResult*) p_notification->Data[0],
          (void*) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_HANDLE_LINK_PCAP:
    if (otLinkPcapCb != NULL)
    {
      otLinkPcapCb((otRadioFrame*) p_notification->Data[0],
          p_notification->Data[1],
          (void*) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_RECEIVE_DIAGNOSTIC_GET_CALLBACK:
    if (otReceiveDiagnosticGetCb != NULL)
    {
      otReceiveDiagnosticGetCb((otMessage*) p_notification->Data[0],
          (otMessageInfo*) p_notification->Data[1],
          (void*) p_notification->Data[2]);
    }
    break;
#if OPENTHREAD_FTD
  case MSG_M0TOM4_THREAD_FTD_NEIGHBOR_TABLE_CALLBACK:
    if (otNeighborTableCb != NULL)
    {
      otNeighborTableCb((otNeighborTableEvent) p_notification->Data[0],
          (const otNeighborTableEntryInfo *)p_notification->Data[1]);
    }
    break;
#endif
  case MSG_M0TOM4_COMMISSIONER_ENERGY_REPORT_CALLBACK:
    if (otCommissionerEnergyReportCb != NULL)
    {
      otCommissionerEnergyReportCb((uint32_t) p_notification->Data[0],
          (uint8_t*) p_notification->Data[1],
          (uint8_t) p_notification->Data[2],
          (void*) p_notification->Data[3]);
    }
    break;
  case MSG_M0TOM4_COMMISSIONER_STATE_CALLBACK:
    if (otCommissionerStateCb != NULL)
    {
      otCommissionerStateCb((otCommissionerState) p_notification->Data[0],
          (void*) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_COMMISSIONER_JOINER_CALLBACK:
    if (otCommissionerJoinerCb != NULL)
    {
      otCommissionerJoinerCb((otCommissionerJoinerEvent) p_notification->Data[0],
          (otExtAddress*) p_notification->Data[1],
          (void*) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_DNS_RESPONSE_HANDLER:
    if (otDnsResponseHandlerCb != NULL)
    {
      otDnsResponseHandlerCb((void*) p_notification->Data[0],
          (char*) p_notification->Data[1],
          (otIp6Address*) p_notification->Data[2],
          (uint32_t) p_notification->Data[3],
          (otError) p_notification->Data[4]);
    }
    break;
  case MSG_M0TOM4_ICMP6_RECEIVE_CALLBACK:
    if (otIcmp6ReceiveCb != NULL)
    {
      otIcmp6ReceiveCb((void*) p_notification->Data[0],
          (otMessage*) p_notification->Data[1],
          (otMessageInfo*) p_notification->Data[2],
          (otIcmp6Header*) p_notification->Data[3]);
    }
    break;
  case MSG_M0TOM4_JOINER_CALLBACK:
    if (otJoinerCb != NULL)
    {
      otJoinerCb((otError) p_notification->Data[0],
          (void*) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_LINK_RAW_RECEIVE_DONE:
    if (otLinkRawReceiveDoneCb != NULL)
    {
      otLinkRawReceiveDoneCb((otInstance*) p_notification->Data[0],
          (otRadioFrame*) p_notification->Data[1],
          (otError) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_LINK_RAW_TRANSMIT_DONE:
    if (otLinkRawTransmitDoneCb != NULL)
    {
      otLinkRawTransmitDoneCb((otInstance*) p_notification->Data[0],
          (otRadioFrame*) p_notification->Data[1],
          (otRadioFrame*) p_notification->Data[2],
          (otError) p_notification->Data[3]);
    }
    break;
  case MSG_M0TOM4_LINK_RAW_ENERGY_SCAN_DONE:
    if (otLinkRawEnergyScanDoneCb != NULL)
    {
      otLinkRawEnergyScanDoneCb((otInstance*) p_notification->Data[0],
          (int8_t) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_UDP_RECEIVE:
    if (otUdpReceiveCb != NULL)
    {
      otUdpReceiveCb((void*) p_notification->Data[0],
          (otMessage*) p_notification->Data[1],
          (otMessageInfo*) p_notification->Data[2]);
    }
    break;
#if OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE
  case MSG_M0TOM4_UDP_FORWARDER:
    if (otUdpForwardSetForwarderCb != NULL)
    {
      otUdpForwardSetForwarderCb((otMessage *) p_notification->Data[0],
          (uint16_t) p_notification->Data[1],
          (otIp6Address *) p_notification->Data[2],
          (uint16_t) p_notification->Data[3],
          (void *) p_notification->Data[4]);
    }
    break;
#endif /* OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE */
  case MSG_M0TOM4_NETWORK_TIME_SYNC_CALLBACK_FN:
    if (otNetworkTimeSyncCb != NULL)
    {
      otNetworkTimeSyncCb((void *)p_notification->Data[0]);
    }
    break;
  case MSG_M0TOM4_SNTP_RESPONSE_HANDLER:
    if (otSntpResponseHandlerCb != NULL)
    {
      otSntpResponseHandlerCb((void *)p_notification->Data[0],
          (uint64_t)p_notification->Data[1],
          (otError)p_notification->Data[2]);
    }
    break;
#if OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE
  case MSG_M0TOM4_JAM_DETECTION_CALLBACK:
    if (otJamDetectionCallbackCb != NULL)
    {
      otJamDetectionCallbackCb((bool) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
#endif
  default:
    status = HAL_ERROR;
    break;
  }

  TL_THREAD_SendAck();
  return status;

}
