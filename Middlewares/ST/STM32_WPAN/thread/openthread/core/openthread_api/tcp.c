/**
 ******************************************************************************
 * @file    tcp.c
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

#include "tcp.h"

otError otTcpEndpointInitialize(otInstance *                       aInstance,
                                otTcpEndpoint *                    aEndpoint,
                                const otTcpEndpointInitializeArgs *aArgs)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_ENDPOINT_INITIALIZE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aArgs;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otInstance *otTcpEndpointGetInstance(otTcpEndpoint *aEndpoint)
{
  otInstance * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_ENDPOINT_GET_INSTANCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otInstance *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void *otTcpEndpointGetContext(otTcpEndpoint *aEndpoint)
{
  void * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_ENDPOINT_GET_CONTEXT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (void *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otSockAddr *otTcpGetLocalAddress(const otTcpEndpoint *aEndpoint)
{
  const otSockAddr * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_GET_LOCAL_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSockAddr *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

const otSockAddr *otTcpGetPeerAddress(const otTcpEndpoint *aEndpoint)
{
  const otSockAddr * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_GET_PEER_ADDRESS;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (const otSockAddr *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpBind(otTcpEndpoint *aEndpoint, const otSockAddr *aSockName)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_BIND;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aSockName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpConnect(otTcpEndpoint *aEndpoint, const otSockAddr *aSockName, uint32_t aFlags)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_CONNECT;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aSockName;
  p_ot_req->Data[2] = (uint32_t) aFlags;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpSendByReference(otTcpEndpoint *aEndpoint, otLinkedBuffer *aBuffer, uint32_t aFlags)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_SEND_BY_REFERENCE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aBuffer;
  p_ot_req->Data[2] = (uint32_t) aFlags;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpSendByExtension(otTcpEndpoint *aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_SEND_BY_EXTENSION;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aNumBytes;
  p_ot_req->Data[2] = (uint32_t) aFlags;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpReceiveByReference(otTcpEndpoint *aEndpoint, const otLinkedBuffer **aBuffer)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_RECEIVE_BY_REFERENCE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aBuffer;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpReceiveContiguify(otTcpEndpoint *aEndpoint)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_RECEIVE_CONTIGUITY;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpCommitReceive(otTcpEndpoint *aEndpoint, size_t aNumBytes, uint32_t aFlags)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_COMMIT_RECEIVE;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;
  p_ot_req->Data[1] = (uint32_t) aNumBytes;
  p_ot_req->Data[2] = (uint32_t) aFlags;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpSendEndOfStream(otTcpEndpoint *aEndpoint)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_SEND_END_OF_STREAM;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpAbort(otTcpEndpoint *aEndpoint)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_ABORT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpEndpointDeinitialize(otTcpEndpoint *aEndpoint)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_ENDPOINT_DEINITIALIZE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aEndpoint;

  Ot_Cmd_TransferWithNotif();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpListenerInitialize(otInstance *                       aInstance,
                                otTcpListener *                    aListener,
                                const otTcpListenerInitializeArgs *aArgs)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_LISTENER_INITIALIZE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aListener;
  p_ot_req->Data[1] = (uint32_t) aArgs;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otInstance *otTcpListenerGetInstance(otTcpListener *aListener)
{
  otInstance * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_LISTENER_GET_INSTANCE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aListener;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otInstance *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

void *otTcpListenerGetContext(otTcpListener *aListener)
{
  void * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_LISTENER_GET_CONTEXT;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aListener;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (void *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpListen(otTcpListener *aListener, const otSockAddr *aSockName)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_LISTEN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aListener;
  p_ot_req->Data[1] = (uint32_t) aSockName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpStopListening(otTcpListener *aListener)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_STOP_LISTENING;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aListener;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otTcpListenerDeinitialize(otTcpListener *aListener)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_TCP_LISTENER_DEINITIALIZE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aListener;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#endif // OPENTHREAD_CONFIG_TCP_ENABLE
