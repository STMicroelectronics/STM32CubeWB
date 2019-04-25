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
otIp6SlaacIidCreate aIidCreateCb = NULL;
otIp6ReceiveCallback otIp6ReceiveCb = NULL;

/* LINK */
otHandleActiveScanResult otHandleActiveScanResultCb = NULL;
otHandleEnergyScanResult otHandleEnergyScanResultCb = NULL;
otLinkPcapCallback otLinkPcapCb = NULL;

/* THREAD */
otReceiveDiagnosticGetCallback otReceiveDiagnosticGetCb = NULL;

#if OPENTHREAD_FTD
/* THREAD_FTD */
otThreadChildTableCallback otThreadChildTableCallbackCb = NULL;
#endif

/* COMMISSIONER */
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

/* COAP */
typedef void (*CoapRequestHandlerCallback)(otCoapHeader *aHeader, otMessage *aMessage,
                                     const otMessageInfo *aMessageInfo);
typedef void (*CoapResponseHandlerCallback)(otCoapHeader *aHeader, otMessage *aMessage,
                                      const otMessageInfo *aMessageInfo, otError aResult);

CoapRequestHandlerCallback coapRequestHandlerCb = NULL;
CoapResponseHandlerCallback coapResponseHandlerCb = NULL;

#if OPENTHREAD_ENABLE_JAM_DETECTION
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
    case MSG_M0TOM4_COAP_REQUEST_HANDLER:
        coapRequestHandlerCb = (CoapRequestHandlerCallback) p_notification->Data[0];

        if (coapRequestHandlerCb != NULL)
        {
            coapRequestHandlerCb( (otCoapHeader *) p_notification->Data[1],
                    (otMessage *) p_notification->Data[2],
                    (otMessageInfo *) p_notification->Data[3]);
        }
        break;
    case MSG_M0TOM4_COAP_RESPONSE_HANDLER:
        coapResponseHandlerCb = (CoapResponseHandlerCallback) p_notification->Data[0];
        if (coapResponseHandlerCb != NULL)
        {
            coapResponseHandlerCb( (otCoapHeader *) p_notification->Data[1],
                    (otMessage *) p_notification->Data[2],
                    (otMessageInfo *) p_notification->Data[3],
                    (otError) p_notification->Data[4]);
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
    case MSG_M0TOM4_IP6_SLAAC_IID_CREATE:
        if (aIidCreateCb != NULL)
        {
            /* Not passing otInstance as first parameter, because created on M0, passing NULL instead */
            aIidCreateCb(NULL, (otNetifAddress*) p_notification->Data[0],
                    (void*) p_notification->Data[1]);
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
                    (void*) p_notification->Data[1]);
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
    case MSG_M0TOM4_THREAD_FTD_CHILD_TABLE_CALLBACK:
        if (otThreadChildTableCallbackCb != NULL)
        {
            otThreadChildTableCallbackCb((otThreadChildTableEvent) p_notification->Data[0],
                    (const otChildInfo *) p_notification->Data[1]);
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
#if OPENTHREAD_ENABLE_JAM_DETECTION
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
