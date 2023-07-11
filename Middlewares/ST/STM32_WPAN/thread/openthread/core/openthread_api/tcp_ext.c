/**
 ******************************************************************************
 * @file    tcp-ext.c
 * @author  MCD Application Team
 * @brief   This file contains the TCP interface shared between M0 and
 *          M4.
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

#if OPENTHREAD_CONFIG_TCP_ENABLE

#include "tcp_ext.h"

void otTcpCircularSendBufferInitialize(otTcpCircularSendBuffer *aSendBuffer, void *aDataBuffer, size_t aCapacity)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_CIRCULAR_SEND_BUFFER_INITIALIZE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aSendBuffer;
  p_ot_req->Data[1] = (uint32_t) aDataBuffer;
  p_ot_req->Data[2] = (uint32_t) aCapacity;

  Ot_Cmd_Transfer();
}

otError otTcpCircularSendBufferWrite(otTcpEndpoint           *aEndpoint,
                                     otTcpCircularSendBuffer *aSendBuffer,
                                     const void              *aData,
                                     size_t                   aLength,
                                     size_t                  *aWritten,
                                     uint32_t                 aFlags)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_CIRCULAR_SEND_BUFFER_WRITE;

  p_ot_req->Size=6;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aSendBuffer;
  p_ot_req->Data[2] = (uint32_t) aData;
  p_ot_req->Data[3] = (uint32_t) aLength;
  p_ot_req->Data[4] = (uint32_t) aWritten;
  p_ot_req->Data[5] = (uint32_t) aFlags;

  Ot_Cmd_Transfer();
  
  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otTcpCircularSendBufferHandleForwardProgress(otTcpCircularSendBuffer *aSendBuffer, size_t aInSendBuffer)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_CIRCULAR_SEND_BUFFER_HANDLE_FORWARD_PROGRESS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aSendBuffer;
  p_ot_req->Data[1] = (uint32_t) aInSendBuffer;

  Ot_Cmd_Transfer();
}

size_t otTcpCircularSendBufferGetFreeSpace(const otTcpCircularSendBuffer *aSendBuffer)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_CIRCULAR_SEND_BUFFER_GET_FREE_SPACE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSendBuffer;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (size_t)p_ot_req->Data[0];
}

void otTcpCircularSendBufferForceDiscardAll(otTcpCircularSendBuffer *aSendBuffer)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_CIRCULAR_SEND_BUFFER_FORCE_DISCARD_ALL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSendBuffer;

  Ot_Cmd_Transfer();
}

otError otTcpCircularSendBufferDeinitialize(otTcpCircularSendBuffer *aSendBuffer)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_CIRCULAR_SEND_BUFFER_DEINITIALIZE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aSendBuffer;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

int otTcpMbedTlsSslSendCallback(void *aCtx, const unsigned char *aBuf, size_t aLen)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_MBED_TLS_SSL_SEND_CALLBACK;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aCtx;
  p_ot_req->Data[0] = (uint32_t) aBuf;
  p_ot_req->Data[0] = (uint32_t) aLen;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (int)p_ot_req->Data[0];
}

int otTcpMbedTlsSslRecvCallback(void *aCtx, unsigned char *aBuf, size_t aLen)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_MBED_TLS_SSL_RECV_CALLBACK;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aCtx;
  p_ot_req->Data[0] = (uint32_t) aBuf;
  p_ot_req->Data[0] = (uint32_t) aLen;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (int)p_ot_req->Data[0];
}

#endif // OPENTHREAD_CONFIG_TCP_ENABLE
