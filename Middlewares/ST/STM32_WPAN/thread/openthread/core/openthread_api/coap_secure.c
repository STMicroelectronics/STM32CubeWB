/**
 ******************************************************************************
 * @file    coap.c
 * @author  MCD Application Team
 * @brief   This file contains the Coap interface shared between M0 and
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

#include "thread.h"
#include "coap_secure.h"

extern otCoapRequestHandler defaultCoapSecureRequestHandlerCb;
extern otHandleCoapSecureClientConnect coapSecureClientConnectCb;

#if OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE

otError otCoapSecureStart(otInstance *aInstance, uint16_t aPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_START;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

void otCoapSecureStop(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otCoapSecureSetPsk(otInstance *   aInstance,
                        const uint8_t *aPsk,
                        uint16_t       aPskLength,
                        const uint8_t *aPskIdentity,
                        uint16_t       aPskIdLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SET_PSK;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aPsk;
  p_ot_req->Data[1] = (uint32_t) aPskLength;
  p_ot_req->Data[2] = (uint32_t) aPskIdentity;
  p_ot_req->Data[3] = (uint32_t) aPskIdLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otCoapSecureGetPeerCertificateBase64(otInstance *   aInstance,
                                             unsigned char *aPeerCert,
                                             size_t *       aCertLength,
                                             size_t         aCertBufferSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_GET_PEER_CERTIFICATE_BASE_64;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aPeerCert;
  p_ot_req->Data[1] = (uint32_t) aCertLength;
  p_ot_req->Data[2] = (uint32_t) aCertBufferSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

void otCoapSecureSetSslAuthMode(otInstance *aInstance, bool aVerifyPeerCertificate)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SET_SSL_AUTH_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aVerifyPeerCertificate;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otCoapSecureSetCertificate(otInstance *   aInstance,
                                const uint8_t *aX509Cert,
                                uint32_t       aX509Length,
                                const uint8_t *aPrivateKey,
                                uint32_t       aPrivateKeyLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SET_CERTIFICATE;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aX509Cert;
  p_ot_req->Data[1] = (uint32_t) aX509Length;
  p_ot_req->Data[2] = (uint32_t) aPrivateKey;
  p_ot_req->Data[3] = (uint32_t) aPrivateKeyLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otCoapSecureSetCaCertificateChain(otInstance *   aInstance,
                                       const uint8_t *aX509CaCertificateChain,
                                       uint32_t       aX509CaCertChainLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SET_CA_CERTIFICATE_CHAIN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aX509CaCertificateChain;
  p_ot_req->Data[1] = (uint32_t) aX509CaCertChainLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otCoapSecureConnect(otInstance *                    aInstance,
                            const otSockAddr *              aSockAddr,
                            otHandleCoapSecureClientConnect aHandler,
                            void *                          aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_CONNECT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aSockAddr;
  p_ot_req->Data[1] = (uint32_t) aHandler;
  p_ot_req->Data[2] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

void otCoapSecureDisconnect(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_DISCONNECT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

bool otCoapSecureIsConnected(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_IS_CONNECTED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool) p_ot_req->Data[0];
}

bool otCoapSecureIsConnectionActive(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_IS_CONNECTION_ALIVE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool) p_ot_req->Data[0];
}

otError otCoapSecureSendRequest(otInstance *          aInstance,
                                otMessage *           aMessage,
                                otCoapResponseHandler aHandler,
                                void *                aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SEND_REQUEST;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aHandler;
  p_ot_req->Data[2] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapSecureAddResource(otInstance *aInstance, otCoapResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_ADD_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

void otCoapSecureRemoveResource(otInstance *aInstance, otCoapResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_REMOVE_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otCoapSecureSetDefaultHandler(otInstance *aInstance, otCoapRequestHandler aHandler, void *aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  defaultCoapSecureRequestHandlerCb = aHandler;

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SET_DEFAULT_HANDLER;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHandler;
  p_ot_req->Data[1] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otCoapSecureSetClientConnectedCallback(otInstance *                    aInstance,
                                            otHandleCoapSecureClientConnect aHandler,
                                            void *                          aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  coapSecureClientConnectCb = aHandler;

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SET_CLIENT_CONNECTED_CALLBACK;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHandler;
  p_ot_req->Data[1] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otCoapSecureSendResponse(otInstance *aInstance, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SEND_RESPONSE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

#endif /* OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE */
