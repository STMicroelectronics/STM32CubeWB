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
#include "coap.h"

extern otCoapRequestHandler defaultCoapRequestHandlerCb;

#if OPENTHREAD_CONFIG_COAP_API_ENABLE

void otCoapMessageInit(otMessage *aMessage, otCoapType aType, otCoapCode aCode)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_INIT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aType;
  p_ot_req->Data[2] = (uint32_t) aCode;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otCoapMessageInitResponse(otMessage *aResponse, const otMessage *aRequest, otCoapType aType, otCoapCode aCode)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_INIT_RESPONSE;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aResponse;
  p_ot_req->Data[1] = (uint32_t) aRequest;
  p_ot_req->Data[2] = (uint32_t) aType;
  p_ot_req->Data[3] = (uint32_t) aCode;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageSetToken(otMessage *aMessage, const uint8_t *aToken, uint8_t aTokenLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_SET_TOKEN;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aToken;
  p_ot_req->Data[2] = (uint32_t) aTokenLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

void otCoapMessageGenerateToken(otMessage *aMessage, uint8_t aTokenLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GENERATE_TOKEN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aTokenLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otCoapMessageAppendContentFormatOption(otMessage *aMessage, otCoapOptionContentFormat aContentFormat)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_CONTENT_FORMAT_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aContentFormat;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageAppendOption(otMessage *aMessage, uint16_t aNumber, uint16_t aLength, const void *aValue)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_OPTION;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aNumber;
  p_ot_req->Data[2] = (uint32_t) aLength;
  p_ot_req->Data[3] = (uint32_t) aValue;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageAppendUintOption(otMessage *aMessage, uint16_t aNumber, uint32_t aValue)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_UINT_OPTION;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aNumber;
  p_ot_req->Data[2] = (uint32_t) aValue;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageAppendObserveOption(otMessage *aMessage, uint32_t aObserve)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_OBSERVE_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aObserve;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageAppendUriPathOptions(otMessage *aMessage, const char *aUriPath)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_URI_PATH_OPTIONS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aUriPath;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageAppendProxyUriOption(otMessage *aMessage, const char *aUriPath)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_PROXY_URI_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aUriPath;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageAppendMaxAgeOption(otMessage *aMessage, uint32_t aMaxAge)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_MAX_AGE_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMaxAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageAppendUriQueryOption(otMessage *aMessage, const char *aUriQuery)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_URI_QUERY_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aUriQuery;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otError otCoapMessageSetPayloadMarker(otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_SET_PAYLOAD_MARKER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError) p_ot_req->Data[0];
}

otCoapType otCoapMessageGetType(const otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_TYPE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otCoapType)p_ot_req->Data[0];
}

otCoapCode otCoapMessageGetCode(const otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_CODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otCoapCode)p_ot_req->Data[0];
}

const char *otCoapMessageCodeToString(const otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_CODE_TO_STRING;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char *)p_ot_req->Data[0];
}

uint16_t otCoapMessageGetMessageId(const otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_MESSAGE_ID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

uint8_t otCoapMessageGetTokenLength(const otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_TOKEN_LENGTH;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

const uint8_t *otCoapMessageGetToken(const otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_TOKEN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t *)p_ot_req->Data[0];
}

const otCoapOption *otCoapMessageGetFirstOption(otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_FIRST_OPTION;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otCoapOption *)p_ot_req->Data[0];
}

const otCoapOption *otCoapMessageGetNextOption(otMessage *aMessage)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_NEXT_OPTION;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otCoapOption *)p_ot_req->Data[0];
}

otError otCoapMessageGetOptionValue(otMessage *aMessage, void *aValue)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_OPTION_VALUE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aValue;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otMessage *otCoapNewMessage(otInstance *aInstance, const otMessageSettings *aSettings)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_NEW_MESSAGE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSettings;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otMessage *)p_ot_req->Data[0];

}

otError otCoapSendRequest(otInstance *aInstance,otMessage *aMessage, const otMessageInfo *aMessageInfo,
    otCoapResponseHandler aHandler, void *aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SEND_REQUEST;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;
  p_ot_req->Data[2] = (uint32_t) aHandler;
  p_ot_req->Data[3] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCoapStart(otInstance *aInstance, uint16_t aPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_START;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint16_t) aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCoapStop(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otCoapAddResource(otInstance *aInstance, otCoapResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_ADD_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otCoapRemoveResource(otInstance *aInstance, otCoapResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_REMOVE_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otCoapSetDefaultHandler(otInstance *aInstance, otCoapRequestHandler aHandler, void *aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  defaultCoapRequestHandlerCb = aHandler;

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SET_DEFAULT_HANDLER;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aHandler;
  p_ot_req->Data[1] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otCoapSendResponse(otInstance *aInstance, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SEND_RESPONSE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

#endif /* OPENTHREAD_CONFIG_COAP_API_ENABLE */
