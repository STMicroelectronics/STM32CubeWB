/**
  ******************************************************************************
  * @file    message.c
  * @author  MCD Application Team
  * @brief   This file contains the message buffers and queues interface shared
  *          between M0 and M4.
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
#include "message.h"


void otMessageFree(otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_FREE;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint16_t otMessageGetLength(const otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_GET_LENGTH;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint16_t)p_ot_req->Data[0];
}

otError otMessageSetLength(otMessage *aMessage, uint16_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_SET_LENGTH;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aMessage;
    p_ot_req->Data[1] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

uint16_t otMessageGetOffset(const otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_GET_OFFSET;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint16_t)p_ot_req->Data[0];
}

otError otMessageSetOffset(otMessage *aMessage, uint16_t aOffset)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_SET_OFFSET;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aMessage;
    p_ot_req->Data[1] = (uint32_t) aOffset;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

bool otMessageIsLinkSecurityEnabled(const otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_IS_LINK_SECURITY_ENABLED;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (bool)p_ot_req->Data[0];
}

void otMessageSetDirectTransmission(otMessage *aMessage, bool aEnabled)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_SET_DIRECT_TRANSMISSION;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aMessage;
    p_ot_req->Data[1] = (uint32_t) aEnabled;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

int8_t otMessageGetRss(const otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_GET_RSS;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (int8_t)p_ot_req->Data[0];
}

otError otMessageAppend(otMessage *aMessage, const void *aBuf, uint16_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_APPEND;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t) aMessage;
    p_ot_req->Data[1] = (uint32_t) aBuf;
    p_ot_req->Data[2] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

uint16_t otMessageRead(const otMessage *aMessage, uint16_t aOffset, void *aBuf, uint16_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_READ;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aMessage;
    p_ot_req->Data[1] = (uint32_t) aOffset;
    p_ot_req->Data[2] = (uint32_t) aBuf;
    p_ot_req->Data[3] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint16_t)p_ot_req->Data[0];
}

int otMessageWrite(otMessage *aMessage, uint16_t aOffset, const void *aBuf, uint16_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_WRITE;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aMessage;
    p_ot_req->Data[1] = (uint32_t) aOffset;
    p_ot_req->Data[2] = (uint32_t) aBuf;
    p_ot_req->Data[3] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (int)p_ot_req->Data[0];
}

void otMessageQueueInit(otMessageQueue *aQueue)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_QUEUE_INIT;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aQueue;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otMessageQueueEnqueue(otMessageQueue *aQueue, otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_QUEUE_ENQUEUE;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aQueue;
    p_ot_req->Data[1] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otMessageQueueEnqueueAtHead(otMessageQueue *aQueue, otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_QUEUE_ATHEAD;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aQueue;
    p_ot_req->Data[1] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];

}

otError otMessageQueueDequeue(otMessageQueue *aQueue, otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_QUEUE_DEQUEUE;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aQueue;
    p_ot_req->Data[1] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otMessage *otMessageQueueGetHead(otMessageQueue *aQueue)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_QUEUE_GET_HEAD;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aQueue;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otMessage *)p_ot_req->Data[0];
}

otMessage *otMessageQueueGetNext(otMessageQueue *aQueue, const otMessage *aMessage)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_QUEUE_GET_NEXT;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aQueue;
    p_ot_req->Data[1] = (uint32_t) aMessage;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otMessage *)p_ot_req->Data[0];
}

void otMessageGetBufferInfo(otInstance *aInstance, otBufferInfo *aBufferInfo)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_MESSAGE_BUFFER_INFO;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aBufferInfo;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}
