/*
 ******************************************************************************
 * @file    openthread_api_wb.c
 * @author  MCD Application Team
 * @brief   Contains STM32WB specificities requested to control the OpenThread
 *          interface.
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

otIp6RegisterMulticastListenersCallback otIp6RegisterMulticastListenersCb = NULL;

/* LINK */
otHandleActiveScanResult otHandleActiveScanResultCb = NULL;
otHandleEnergyScanResult otHandleEnergyScanResultCb = NULL;
otLinkPcapCallback otLinkPcapCb = NULL;

/* LINK METRICS */
otLinkMetricsReportCallback otLinkMetricsReportCb = NULL;
otLinkMetricsMgmtResponseCallback otLinkMetricsMgmtResponseCb = NULL;
otLinkMetricsMgmtResponseCallback otLinkMetricsMgmtResponseCb1 = NULL;
otLinkMetricsEnhAckProbingIeReportCallback otLinkMetricsEnhAckProbingIeReportCb = NULL;

/* THREAD */
otThreadParentResponseCallback otThreadParentResponseCb = NULL;
otDetachGracefullyCallback otDetachGracefullyCb = NULL;

#if OPENTHREAD_CONFIG_TMF_ANYCAST_LOCATOR_ENABLE
otThreadAnycastLocatorCallback otThreadAnycastLocatorCb = NULL;
#endif // OPENTHREAD_CONFIG_TMF_ANYCAST_LOCATOR_ENABLE

#if OPENTHREAD_FTD
otThreadDiscoveryRequestCallback otThreadDiscoveryRequestCb = NULL;
#endif // OPENTHREAD_FTD

#if OPENTHREAD_FTD
/* THREAD_FTD */
otNeighborTableCallback otNeighborTableCb = NULL;
#endif

/* COMMISSIONER */
otCommissionerStateCallback otCommissionerStateCb = NULL;
otCommissionerJoinerCallback otCommissionerJoinerCb = NULL;
otCommissionerEnergyReportCallback otCommissionerEnergyReportCb = NULL;
otCommissionerPanIdConflictCallback otCommissionerPanIdConflictCb = NULL;

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
otUdpHandler otUdpHandlerCb = NULL;
STUdpHandlerContextType* mySTUdpHandlerContext = NULL;
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

/* BACKBONE ROUTER */
otBackboneRouterDomainPrefixCallback otBackboneRouterDomainPrefixCb = NULL;
otBackboneRouterNdProxyCallback otBackboneRouterNdProxyCb = NULL;
otBackboneRouterMulticastListenerCallback otBackboneRouterMulticastListenerCb = NULL;

/* DATASET */
otDatasetMgmtSetCallback otDatasetMgmtSetActiveCb = NULL;
otDatasetMgmtSetCallback otDatasetMgmtSetPendingCb = NULL;

/* DATASET UPDATER */
otDatasetUpdaterCallback otDatasetUpdaterCb = NULL;

/* DNS */
otDnsAddressCallback otDnsAddressCb = NULL;
otDnsBrowseCallback otDnsBrowseCb = NULL;
otDnsServiceCallback otDnsServiceCb = NULL;

#if OPENTHREAD_CONFIG_DNSSD_SERVER_ENABLE
otDnssdQuerySubscribeCallback otDnssdQuerySubscribeCb = NULL;
otDnssdQueryUnsubscribeCallback otDnssdQueryUnsubscribeCb = NULL;
#endif

#if OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE
/* JAM_DETECTION */
otJamDetectionCallback otJamDetectionCallbackCb = NULL;
#endif

#if OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE
/* NET_DATA PUBLISHER*/
otNetDataDnsSrpServicePublisherCallback otNetDataDnsSrpServicePublisherCb = NULL;
#endif /* OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE */

#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
otNetDataPrefixPublisherCallback otNetDataPrefixPublisherCb = NULL;
#endif

#if OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE
otReceiveDiagnosticGetCallback otReceiveDiagnosticGetCb = NULL;
#endif

#if OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE
otSrpClientCallback otSrpClientCb = NULL;
#endif

#if OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE
#if OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE
otSrpClientAutoStartCallback otSrpClientAutoStartCb = NULL;
#endif // OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE
#endif // OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE

#if OPENTHREAD_CONFIG_SRP_SERVER_ENABLE
otSrpServerServiceUpdateHandler otSrpServerServiceUpdateHandlerCb = NULL;
#endif

/* PING SENDER */
#if OPENTHREAD_CONFIG_PING_SENDER_ENABLE
otPingSenderReplyCallback otPingSenderReplyCb = NULL;
otPingSenderStatisticsCallback otPingSenderStatisticsCb = NULL;
#endif // OPENTHREAD_CONFIG_PING_SENDER_ENABLE

/* TCP */
#if OPENTHREAD_CONFIG_TCP_ENABLE
STTcpEndpointHandlerContextType* mySTTcpEndpointHandlerContext = NULL;
STTcpListenerHandlerContextType* mySTTcpListenerHandlerContext = NULL;
//otTcpBytesAcked otTcpBytesAckedCb = NULL;
otTcpDisconnected otTcpDisconnectedCb = NULL;
otTcpEstablished otTcpEstablishedCb = NULL;
otTcpReceiveAvailable otTcpReceiveAvailableCb = NULL;
otTcpSendDone otTcpSendDoneCb = NULL;
//otTcpSendReady otTcpSendReadyCb = NULL;
otTcpAcceptReady otTcpAcceptReadyCb = NULL;
otTcpAcceptDone otTcpAcceptDoneCb = NULL;
otTcpForwardProgress  mForwardProgressCallback = NULL;
#endif // OPENTHREAD_CONFIG_TCP_ENABLE


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
  case MSG_M0TOM4_BACKBONE_ROUTER_DOMAIN_PREFIX_CB:
    if (otBackboneRouterDomainPrefixCb != NULL)
    {
      otBackboneRouterDomainPrefixCb((void*) p_notification->Data[0],
          (otBackboneRouterDomainPrefixEvent) p_notification->Data[1],
          (const otIp6Prefix *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_BACKBONE_ROUTER_MULTICAST_LISTENER_CB:
    if (otBackboneRouterMulticastListenerCb != NULL)
    {
      otBackboneRouterMulticastListenerCb((void*) p_notification->Data[0],
          (otBackboneRouterMulticastListenerEvent) p_notification->Data[1],
          (const otIp6Address *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_BACKBONE_ROUTER_ND_PROXY_CB:
    if (otBackboneRouterNdProxyCb != NULL)
    {
      otBackboneRouterNdProxyCb((void*) p_notification->Data[0],
          (otBackboneRouterNdProxyEvent) p_notification->Data[1],
          (const otIp6Address *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_LINK_METRICS_MGMT_RESPONSE_ENHACK_PROBING_CB:
    if (otLinkMetricsMgmtResponseCb1 != NULL)
    {
      otLinkMetricsMgmtResponseCb1((const otIp6Address *) p_notification->Data[0],
          (uint8_t) p_notification->Data[1],
          (void *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_LINK_METRICS_MGMT_RESPONSE_CB:
    if (otLinkMetricsMgmtResponseCb != NULL)
    {
      otLinkMetricsMgmtResponseCb((const otIp6Address *) p_notification->Data[0],
          (uint8_t) p_notification->Data[1],
          (void *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_LINK_METRICS_ENHACK_PROBING_IE_REPORT_CB:
    if (otLinkMetricsEnhAckProbingIeReportCb != NULL)
    {
      otLinkMetricsEnhAckProbingIeReportCb((otShortAddress) p_notification->Data[0],
          (const otExtAddress *) p_notification->Data[1],
          (const otLinkMetricsValues *) p_notification->Data[2],
          (void *) p_notification->Data[3]);
    }
    break;
  case MSG_M0TOM4_LINK_METRICS_REPORT_CB:
    if (otLinkMetricsReportCb != NULL)
    {
      otLinkMetricsReportCb((const otIp6Address *) p_notification->Data[0],
          (const otLinkMetricsValues *) p_notification->Data[1],
          (uint8_t) p_notification->Data[2],
          (void *) p_notification->Data[3]);
    }
    break;
  case MSG_M0TOM4_DATASET_MGMT_SET_CALLBACK_ACTIVE:
    if (otDatasetMgmtSetActiveCb != NULL)
    {
      otDatasetMgmtSetActiveCb((otError) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_DATASET_MGMT_SET_CALLBACK_PENDING:
    if (otDatasetMgmtSetPendingCb != NULL)
    {
      otDatasetMgmtSetPendingCb((otError) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_DATASET_UPDATER_CB:
    if (otDatasetUpdaterCb != NULL)
    {
      otDatasetUpdaterCb((otError) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_DNS_BROWSE_CB:
    if (otDnsBrowseCb != NULL)
    {
      otDnsBrowseCb((otError) p_notification->Data[0],
          (const otDnsBrowseResponse *) p_notification->Data[1],
          (void *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_DNS_ADDRESS_CB:
    if (otDnsAddressCb != NULL)
    {
      otDnsAddressCb((otError) p_notification->Data[0],
          (const otDnsAddressResponse *) p_notification->Data[1],
          (void *) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_DNS_SERVICE_CB:
    if (otDnsAddressCb != NULL)
    {
      otDnsServiceCb((otError) p_notification->Data[0],
          (const otDnsServiceResponse *) p_notification->Data[1],
          (void *) p_notification->Data[2]);
    }
    break;
#if OPENTHREAD_CONFIG_DNSSD_SERVER_ENABLE
  case MSG_M0TOM4_DNSSD_QUERY_SUBSCRIBE_CB:
    if (otDnssdQuerySubscribeCb != NULL)
    {
      otDnssdQuerySubscribeCb((void *) p_notification->Data[0],
          (const char *) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_DNSSD_QUERY_UNSUBSCRIBE_CB:
    if (otDnssdQueryUnsubscribeCb != NULL)
    {
      otDnssdQueryUnsubscribeCb((void *) p_notification->Data[0],
          (const char *) p_notification->Data[1]);
    }
    break;
  //case MSG_M0TOM4_DNSSD_QUERY_UNSUBSCRIBE_CB:
  //  if (otDnssdQueryUnsubscribeCb != NULL)
  //  {
  //    otDnssdQueryUnsubscribeCb((void *) p_notification->Data[0],
  //        (const char *) p_notification->Data[1]);
  //  }
  //  break;
#endif // OPENTHREAD_CONFIG_DNSSD_SERVER_ENABLE
#if OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE
  case MSG_M0TOM4_NETDATA_DNS_SRP_SERVICE_PUBLISHER_CB:
    if (otNetDataDnsSrpServicePublisherCb != NULL)
    {
      otNetDataDnsSrpServicePublisherCb((otNetDataPublisherEvent) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
#endif // OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE
#if OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE
#if OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
  case MSG_M0TOM4_NETDATA_PREFIX_PUBLISHER_CB:
    if (otNetDataPrefixPublisherCb != NULL)
    {
      otNetDataPrefixPublisherCb((otNetDataPublisherEvent) p_notification->Data[0],
          (const otIp6Prefix *) p_notification->Data[1],
          (void *) p_notification->Data[2]);
    }
    break;
#endif // OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE
#endif // OPENTHREAD_CONFIG_NETDATA_PUBLISHER_ENABLE
#if OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE
#if OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE
  case MSG_M0TOM4_SRP_CLIENT_AUTO_START_CB:
    if (otSrpClientAutoStartCb != NULL)
    {
      otSrpClientAutoStartCb((const otSockAddr *) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
#endif // OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE
#endif // OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE
#if OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE
  case MSG_M0TOM4_SRP_CLIENT_CB:
    if (otSrpClientCb != NULL)
    {
      otSrpClientCb((otError) p_notification->Data[0],
          (const otSrpClientHostInfo *) p_notification->Data[1],
          (const otSrpClientService *) p_notification->Data[2],
          (const otSrpClientService *) p_notification->Data[3],
          (void *) p_notification->Data[4]);
    }
    break;
#endif // OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE
#if OPENTHREAD_CONFIG_SRP_SERVER_ENABLE
  case MSG_M0TOM4_SRP_SERVER_SERVICE_UPDATE_HANDLER_CB:
    if (otSrpServerServiceUpdateHandlerCb != NULL)
    {
      otSrpServerServiceUpdateHandlerCb((otSrpServerServiceUpdateId) p_notification->Data[0],
          (const otSrpServerHost *) p_notification->Data[1],
          (uint32_t) p_notification->Data[2],
          (void *) p_notification->Data[3]);
    }
    break;
#endif // OPENTHREAD_CONFIG_SRP_SERVER_ENABLE
#if OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE
  case MSG_M0TOM4_RECEIVE_DIAGNOSTIC_GET_CB:
    if (otReceiveDiagnosticGetCb != NULL)
    {
      otReceiveDiagnosticGetCb((otError) p_notification->Data[0],
          (otMessage *) p_notification->Data[1],
          (const otMessageInfo *) p_notification->Data[2],
          (void *) p_notification->Data[3]);
    }
    break;
#endif // OPENTHREAD_FTD || OPENTHREAD_CONFIG_TMF_NETWORK_DIAG_MTD_ENABLE
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
    break;
  case MSG_M0TOM4_DETACH_GRACEFULLY_CALLBACK:
    if (otDetachGracefullyCb != NULL)
    {
      otDetachGracefullyCb((void *) p_notification->Data[0]);
    }
    break;
#if OPENTHREAD_CONFIG_TMF_ANYCAST_LOCATOR_ENABLE
  case MSG_M0TOM4_THREAD_ANYCAST_LOCATOR_CB:
    if (otThreadAnycastLocatorCb != NULL)
    {
      otThreadAnycastLocatorCb((void *) p_notification->Data[0],
          (otError) p_notification->Data[1],
          (otIp6Address *) p_notification->Data[2],
          (uint16_t) p_notification->Data[3]);
    }
    break;
#endif // OPENTHREAD_CONFIG_TMF_ANYCAST_LOCATOR_ENABLE
#if OPENTHREAD_FTD
  case MSG_M0TOM4_THREAD_DISCOVERY_REQUEST_CB:
    if (otThreadDiscoveryRequestCb != NULL)
    {
      otThreadDiscoveryRequestCb((const otThreadDiscoveryRequestInfo *) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
#endif // OPENTHREAD_FTD
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
    if (coapSecureClientConnectCb != NULL)
    {
      coapSecureClientConnectCb((bool) p_notification->Data[0],
          (void *) p_notification->Data[1]);
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
    /* Perform an NVIC Reset in order to reinitialize the device */
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
      otIp6AddressCb((const otIp6AddressInfo *) p_notification->Data[0],
          (bool) p_notification->Data[1],
          (void *) p_notification->Data[2]);
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
  case MSG_M0TOM4_IP6_REGISTER_MULTICAST_LISTENERS_CB:
    if (otIp6RegisterMulticastListenersCb != NULL)
    {
      otIp6RegisterMulticastListenersCb((void *) p_notification->Data[0],
          (otError) p_notification->Data[1],
          (uint8_t) p_notification->Data[2],
          (const otIp6Address *) p_notification->Data[3],
          (uint8_t) p_notification->Data[4]);
    }
    break;
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
  case MSG_M0TOM4_COMMISSIONER_PANID_CONFLICT_CALLBACK:
    if (otCommissionerPanIdConflictCb != NULL)
    {
      otCommissionerPanIdConflictCb((uint16_t) p_notification->Data[0],
          (uint32_t) p_notification->Data[1],
          (void*) p_notification->Data[2]);
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
          (otJoinerInfo*) p_notification->Data[1],
          (otExtAddress*) p_notification->Data[2],
          (void*) p_notification->Data[3]);
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
  case MSG_M0TOM4_UDP_HANDLER:
      mySTUdpHandlerContext = (STUdpHandlerContextType*) p_notification->Data[0];

      otUdpHandlerCb = mySTUdpHandlerContext->mHandler;

      if (otUdpHandlerCb != NULL)
      {
        p_notification->Data[0] = otUdpHandlerCb(mySTUdpHandlerContext->mContext,
            (otMessage *) p_notification->Data[1],
            (otMessageInfo *) p_notification->Data[2]);
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
#if OPENTHREAD_CONFIG_PING_SENDER_ENABLE
  case MSG_M0TOM4_PING_SENDER_REPLY_CALLBACK:
    if (otPingSenderReplyCb != NULL)
    {
      otPingSenderReplyCb((const otPingSenderReply *) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_PING_SENDER_STATISTICS_CALLBACK:
    if (otPingSenderStatisticsCb != NULL)
    {
      otPingSenderStatisticsCb((const otPingSenderStatistics *) p_notification->Data[0],
          (void *) p_notification->Data[1]);
    }
    break;
#endif /* OPENTHREAD_CONFIG_PING_SENDER_ENABLE */
#if OPENTHREAD_CONFIG_TCP_ENABLE
//  case MSG_M0TOM4_TCP_BYTES_ACKED_CALLBACK:
//    mySTTcpEndpointHandlerContext = (STTcpEndpointHandlerContextType*) ((otTcpEndpoint *)p_notification->Data[0])->mContext;
//
//    otTcpBytesAckedCb = mySTTcpEndpointHandlerContext->mBytesAckedCallback;
//
//    if (otTcpBytesAckedCb != NULL)
//    {
//      otTcpBytesAckedCb((otTcpEndpoint *) p_notification->Data[0],
//          (size_t) p_notification->Data[1]);
//    }
//    break;
  case MSG_M0TOM4_TCP_DISCONNECTED_CALLBACK:
    mySTTcpEndpointHandlerContext = (STTcpEndpointHandlerContextType*) ((otTcpEndpoint *)p_notification->Data[0])->mContext;

    otTcpDisconnectedCb = mySTTcpEndpointHandlerContext->mDisconnectedCallback;

    if (otTcpDisconnectedCb != NULL)
    {
      otTcpDisconnectedCb((otTcpEndpoint *) p_notification->Data[0],
          (otTcpDisconnectedReason) p_notification->Data[1]);
    }
    break;
  case MSG_M0TOM4_TCP_ESTABLISHED_CALLBACK:
    mySTTcpEndpointHandlerContext = (STTcpEndpointHandlerContextType*) ((otTcpEndpoint *)p_notification->Data[0])->mContext;

    otTcpEstablishedCb = mySTTcpEndpointHandlerContext->mEstablishedCallback;

    if (otTcpEstablishedCb != NULL)
    {
      otTcpEstablishedCb((otTcpEndpoint *) p_notification->Data[0]);
    }
    break;
  case MSG_M0TOM4_TCP_RECEIVE_AVAILABLE_CALLBACK:
    mySTTcpEndpointHandlerContext = (STTcpEndpointHandlerContextType*) ((otTcpEndpoint *)p_notification->Data[0])->mContext;

    otTcpReceiveAvailableCb = mySTTcpEndpointHandlerContext->mReceiveAvailableCallback;

    if (otTcpReceiveAvailableCb != NULL)
    {
      otTcpReceiveAvailableCb((otTcpEndpoint *) p_notification->Data[0],
          (size_t) p_notification->Data[1],
          (bool) p_notification->Data[2],
          (size_t) p_notification->Data[3]);
    }
    break;
  case MSG_M0TOM4_TCP_SEND_DONE_CALLBACK:
    mySTTcpEndpointHandlerContext = (STTcpEndpointHandlerContextType*) ((otTcpEndpoint *)p_notification->Data[0])->mContext;

    otTcpSendDoneCb = mySTTcpEndpointHandlerContext->mSendDoneCallback;

    if (otTcpSendDoneCb != NULL)
    {
      otTcpSendDoneCb((otTcpEndpoint *) p_notification->Data[0],
          (otLinkedBuffer *) p_notification->Data[1]);
    }
    break;
//  case MSG_M0TOM4_TCP_SEND_READY_CALLBACK:
//    mySTTcpEndpointHandlerContext = (STTcpEndpointHandlerContextType*) ((otTcpEndpoint *)p_notification->Data[0])->mContext;
//
//    otTcpSendReadyCb = mySTTcpEndpointHandlerContext->mSendReadyCallback;
//
//    if (otTcpSendReadyCb != NULL)
//    {
//      otTcpSendReadyCb((otTcpEndpoint *) p_notification->Data[0]);
//    }
//    break;
  case MSG_M0TOM4_TCP_FORWARD_PROGRESS_CALLBACK:
    mySTTcpEndpointHandlerContext = (STTcpEndpointHandlerContextType*) ((otTcpEndpoint *)p_notification->Data[0])->mContext;

    mForwardProgressCallback = mySTTcpEndpointHandlerContext->mForwardProgressCallback;

    if (mForwardProgressCallback != NULL)
    {
      mForwardProgressCallback((otTcpEndpoint *)p_notification->Data[0],
          (size_t) p_notification->Data[1],
          (size_t) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_TCP_ACCEPT_READY_CALLBACK:
    mySTTcpListenerHandlerContext = (STTcpListenerHandlerContextType*) ((otTcpListener *)p_notification->Data[0])->mContext;

    otTcpAcceptReadyCb = mySTTcpListenerHandlerContext->mAcceptReadyCallback;

    if (otTcpAcceptReadyCb != NULL)
    {
      p_notification->Data[0] = otTcpAcceptReadyCb((otTcpListener *) p_notification->Data[0],
          (const otSockAddr *) p_notification->Data[1],
          (otTcpEndpoint **) p_notification->Data[2]);
    }
    break;
  case MSG_M0TOM4_TCP_ACCEPT_DONE_CALLBACK:
    mySTTcpListenerHandlerContext = (STTcpListenerHandlerContextType*) ((otTcpListener *)p_notification->Data[0])->mContext;

    otTcpAcceptDoneCb = mySTTcpListenerHandlerContext->mAcceptDoneCallback;

    if (otTcpAcceptDoneCb != NULL)
    {
      otTcpAcceptDoneCb((otTcpListener *) p_notification->Data[0],
          (otTcpEndpoint *) p_notification->Data[1],
          (const otSockAddr *) p_notification->Data[2]);
    }
    break;
#endif /* OPENTHREAD_CONFIG_TCP_ENABLE */
  default:
    status = HAL_ERROR;
    break;
  }

  TL_THREAD_SendAck();
  return status;

}
