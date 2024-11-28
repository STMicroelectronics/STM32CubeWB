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
#include "coap.h"

#if OPENTHREAD_CONFIG_COAP_API_ENABLE

extern otCoapRequestHandler defaultCoapRequestHandlerCb;

otMessage *otCoapNewMessage(otInstance *aInstance, const otMessageSettings *aSettings)
{
  otMessage *rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_NEW_MESSAGE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSettings;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otMessage *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;

}

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

  Post_OtCmdProcessing();
}

otError otCoapMessageInitResponse(otMessage *aResponse, const otMessage *aRequest, otCoapType aType, otCoapCode aCode)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageSetToken(otMessage *aMessage, const uint8_t *aToken, uint8_t aTokenLength)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
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

  Post_OtCmdProcessing();
}

otError otCoapMessageAppendContentFormatOption(otMessage *aMessage, otCoapOptionContentFormat aContentFormat)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_CONTENT_FORMAT_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aContentFormat;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendOption(otMessage *aMessage, uint16_t aNumber, uint16_t aLength, const void *aValue)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendUintOption(otMessage *aMessage, uint16_t aNumber, uint32_t aValue)
{
  otError rspData;
  
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
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendObserveOption(otMessage *aMessage, uint32_t aObserve)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_OBSERVE_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aObserve;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendUriPathOptions(otMessage *aMessage, const char *aUriPath)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_URI_PATH_OPTIONS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aUriPath;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otCoapBlockSizeFromExponent(otCoapBlockSzx aSize)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_BLOCK_SIZE_FROM_EXPONENT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendBlock2Option(otMessage *aMessage, uint32_t aNum, bool aMore, otCoapBlockSzx aSize)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_BLOCK2_OPTION;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aNum;
  p_ot_req->Data[2] = (uint32_t) aMore;
  p_ot_req->Data[3] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendBlock1Option(otMessage *aMessage, uint32_t aNum, bool aMore, otCoapBlockSzx aSize)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_BLOCK1_OPTION;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aNum;
  p_ot_req->Data[2] = (uint32_t) aMore;
  p_ot_req->Data[3] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendProxyUriOption(otMessage *aMessage, const char *aUriPath)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_PROXY_URI_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aUriPath;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendMaxAgeOption(otMessage *aMessage, uint32_t aMaxAge)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_MAX_AGE_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMaxAge;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageAppendUriQueryOption(otMessage *aMessage, const char *aUriQuery)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_APPEND_URI_QUERY_OPTION;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aUriQuery;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapMessageSetPayloadMarker(otMessage *aMessage)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_SET_PAYLOAD_MARKER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otCoapType otCoapMessageGetType(const otMessage *aMessage)
{
  otCoapType rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_TYPE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otCoapType)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otCoapCode otCoapMessageGetCode(const otMessage *aMessage)
{
  otCoapCode rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_CODE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otCoapCode)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void otCoapMessageSetCode(otMessage *aMessage, otCoapCode aCode)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_SET_CODE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aCode;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

const char *otCoapMessageCodeToString(const otMessage *aMessage)
{
  const char *rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_CODE_TO_STRING;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const char *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint16_t otCoapMessageGetMessageId(const otMessage *aMessage)
{
  uint16_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_MESSAGE_ID;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint16_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

uint8_t otCoapMessageGetTokenLength(const otMessage *aMessage)
{
  uint8_t rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_TOKEN_LENGTH;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (uint8_t)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const uint8_t *otCoapMessageGetToken(const otMessage *aMessage)
{
  const uint8_t *rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_MESSAGE_GET_TOKEN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const uint8_t *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapOptionIteratorInit(otCoapOptionIterator *aIterator, const otMessage *aMessage)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_OPTION_ITERATOR_INIT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aMessage;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otCoapOption *otCoapOptionIteratorGetFirstOptionMatching(otCoapOptionIterator *aIterator, uint16_t aOption)
{
  const otCoapOption *rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_OPTION_ITERATOR_GET_FIRST_OPTION_MATCHING;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aOption;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otCoapOption *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otCoapOption *otCoapOptionIteratorGetFirstOption(otCoapOptionIterator *aIterator)
{
  const otCoapOption *rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_OPTION_ITERATOR_GET_FIRST_OPTION;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIterator;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otCoapOption *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otCoapOption *otCoapOptionIteratorGetNextOptionMatching(otCoapOptionIterator *aIterator, uint16_t aOption)
{
  const otCoapOption *rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_OPTION_ITERATOR_GET_NEXT_OPTION_MATCHING;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aOption;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otCoapOption *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otCoapOption *otCoapOptionIteratorGetNextOption(otCoapOptionIterator *aIterator)
{
  const otCoapOption *rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_OPTION_ITERATOR_GET_NEXT_OPTION;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIterator;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otCoapOption *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapOptionIteratorGetOptionUintValue(otCoapOptionIterator *aIterator, uint64_t *aValue)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_OPTION_ITERATOR_GET_OPTION_UINT_VALUE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aValue;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapOptionIteratorGetOptionValue(otCoapOptionIterator *aIterator, void *aValue)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_OPTION_ITERATOR_GET_OPTION_VALUE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aValue;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE
otError otCoapSendRequestBlockWiseWithParameters(otInstance *                aInstance,
                                                 otMessage *                 aMessage,
                                                 const otMessageInfo *       aMessageInfo,
                                                 otCoapResponseHandler       aHandler,
                                                 void *                      aContext,
                                                 const otCoapTxParameters *  aTxParameters,
                                                 otCoapBlockwiseTransmitHook aTransmitHook,
                                                 otCoapBlockwiseReceiveHook  aReceiveHook)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SEND_REQUEST_BLOCK_WISE_WITH_PARAMETERS;

  p_ot_req->Size=7;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;
  p_ot_req->Data[2] = (uint32_t) aHandler;
  p_ot_req->Data[3] = (uint32_t) aContext;
  p_ot_req->Data[4] = (uint32_t) aTxParameters;
  p_ot_req->Data[5] = (uint32_t) aTransmitHook;
  p_ot_req->Data[6] = (uint32_t) aReceiveHook;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif // OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE

otError otCoapSendRequestWithParameters(otInstance *              aInstance,
                                        otMessage *               aMessage,
                                        const otMessageInfo *     aMessageInfo,
                                        otCoapResponseHandler     aHandler,
                                        void *                    aContext,
                                        const otCoapTxParameters *aTxParameters)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SEND_REQUEST_WITH_PARAMETERS;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;
  p_ot_req->Data[2] = (uint32_t) aHandler;
  p_ot_req->Data[3] = (uint32_t) aContext;
  p_ot_req->Data[4] = (uint32_t) aTxParameters;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapStart(otInstance *aInstance, uint16_t aPort)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_START;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint16_t) aPort;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otCoapStop(otInstance *aInstance)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_STOP;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE
void otCoapAddBlockWiseResource(otInstance *aInstance, otCoapBlockwiseResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_ADD_BLOCK_WISE_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint16_t) aResource;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otCoapRemoveBlockWiseResource(otInstance *aInstance, otCoapBlockwiseResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_REMOVE_BLOCK_WISE_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint16_t) aResource;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}
#endif

void otCoapAddResource(otInstance *aInstance, otCoapResource *aResource)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_ADD_RESOURCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aResource;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
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

  Post_OtCmdProcessing();
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

  Post_OtCmdProcessing();
}

#if OPENTHREAD_CONFIG_COAP_BLOCKWISE_TRANSFER_ENABLE
otError otCoapSendResponseBlockWiseWithParameters(otInstance *                aInstance,
                                                  otMessage *                 aMessage,
                                                  const otMessageInfo *       aMessageInfo,
                                                  const otCoapTxParameters *  aTxParameters,
                                                  void *                      aContext,
                                                  otCoapBlockwiseTransmitHook aTransmitHook)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SEND_RESPONSE_BLOCK_WISE_WITH_PARAMETERS;

  p_ot_req->Size=5;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;
  p_ot_req->Data[2] = (uint32_t) aTxParameters;
  p_ot_req->Data[3] = (uint32_t) aContext;
  p_ot_req->Data[4] = (uint32_t) aTransmitHook;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
#endif

otError otCoapSendResponseWithParameters(otInstance *              aInstance,
                                         otMessage *               aMessage,
                                         const otMessageInfo *     aMessageInfo,
                                         const otCoapTxParameters *aTxParameters)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COAP_SEND_RESPONSE_WITH_PARAMETERS;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aMessage;
  p_ot_req->Data[1] = (uint32_t) aMessageInfo;
  p_ot_req->Data[2] = (uint32_t) aTxParameters;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif /* OPENTHREAD_CONFIG_COAP_API_ENABLE */
