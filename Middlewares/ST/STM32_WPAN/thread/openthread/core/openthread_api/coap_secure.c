/**
 ******************************************************************************
 * @file    coap.c
 * @author  MCD Application Team
 * @brief   This file contains the Coap interface shared between M0 and
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

#include "thread.h"
#include "coap_secure.h"
#include "mbedtls_config.h"

#if OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE

extern otCoapRequestHandler defaultCoapSecureRequestHandlerCb;
extern otHandleCoapSecureClientConnect coapSecureClientConnectCb;
extern otCoapSecureAutoStopCallback coapSecureAutoStopCb;

otError otCoapSecureStart(otInstance *aInstance, uint16_t aPort)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_START;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapSecureStartWithMaxConnAttempts(otInstance                  *aInstance,
                                             uint16_t                     aPort,
                                             uint16_t                     aMaxAttempts,
                                             otCoapSecureAutoStopCallback aCallback,
                                             void                        *aContext)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  coapSecureAutoStopCb = aCallback;

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_START_WITH_MAX_CONN_ATTEMPS;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aPort;
  p_ot_req->Data[1] = (uint32_t) aMaxAttempts;
  p_ot_req->Data[2] = (uint32_t) aCallback;
  p_ot_req->Data[3] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#ifdef MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
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

  Post_OtCmdProcessing();
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

  Post_OtCmdProcessing();
}
#endif // MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED

#ifdef MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
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

  Post_OtCmdProcessing();
}
#endif // MBEDTLS_KEY_EXCHANGE_PSK_ENABLED

#if defined(MBEDTLS_BASE64_C) && defined(MBEDTLS_SSL_KEEP_PEER_CERTIFICATE)
otError otCoapSecureGetPeerCertificateBase64(otInstance *   aInstance,
                                             unsigned char *aPeerCert,
                                             size_t *       aCertLength,
                                             size_t         aCertBufferSize)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif // defined(MBEDTLS_BASE64_C) && defined(MBEDTLS_SSL_KEEP_PEER_CERTIFICATE)

void otCoapSecureSetSslAuthMode(otInstance *aInstance, bool aVerifyPeerCertificate)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SET_SSL_AUTH_MODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aVerifyPeerCertificate;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

otError otCoapSecureConnect(otInstance *                    aInstance,
                            const otSockAddr *              aSockAddr,
                            otHandleCoapSecureClientConnect aHandler,
                            void *                          aContext)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  coapSecureClientConnectCb = aHandler;

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_CONNECT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aSockAddr;
  p_ot_req->Data[1] = (uint32_t) aHandler;
  p_ot_req->Data[2] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otCoapSecureDisconnect(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_DISCONNECT;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

bool otCoapSecureIsConnected(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_IS_CONNECTED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otCoapSecureIsConnectionActive(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_IS_CONNECTION_ALIVE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otCoapSecureIsClosed(otInstance *aInstance)
{
  bool rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_IS_CLOSED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otCoapSecureStop(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

#if OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE
otError otCoapSecureSendRequestBlockWise(otInstance *                aInstance,
                                         otMessage *                 aMessage,
                                         otCoapResponseHandler       aHandler,
                                         void *                      aContext,
                                         otCoapBlockwiseTransmitHook aTransmitHook,
                                         otCoapBlockwiseReceiveHook  aReceiveHook)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SEND_REQUEST_BLOCK_WISE;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aHandler;
  p_ot_req->Data[2] = (uint32_t) aContext;
  p_ot_req->Data[3] = (uint32_t) aTransmitHook;
  p_ot_req->Data[4] = (uint32_t) aReceiveHook;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif

otError otCoapSecureSendRequest(otInstance *          aInstance,
                                otMessage *           aMessage,
                                otCoapResponseHandler aHandler,
                                void *                aContext)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE
void otCoapSecureAddBlockWiseResource(otInstance *aInstance, otCoapBlockwiseResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_ADD_BLOCK_WISE_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otCoapSecureRemoveBlockWiseResource(otInstance *aInstance, otCoapBlockwiseResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_REMOVE_BLOCK_WISE_RESOURCE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}
#endif

void otCoapSecureAddResource(otInstance *aInstance, otCoapResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_ADD_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
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

  Post_OtCmdProcessing();
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

  Post_OtCmdProcessing();
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

  Post_OtCmdProcessing();
}

#if OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE
otError otCoapSecureSendResponseBlockWise(otInstance *                aInstance,
                                          otMessage *                 aMessage,
                                          const otMessageInfo *       aMessageInfo,
                                          void *                      aContext,
                                          otCoapBlockwiseTransmitHook aTransmitHook)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SEND_RESPONSE_BLOCK_WISE;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;
  p_ot_req->Data[2] = (uint32_t) aContext;
  p_ot_req->Data[3] = (uint32_t) aTransmitHook;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif

otError otCoapSecureSendResponse(otInstance *aInstance, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SECURE_SEND_RESPONSE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif /* OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE */
