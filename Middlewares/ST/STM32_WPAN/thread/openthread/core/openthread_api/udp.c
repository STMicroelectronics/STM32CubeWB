/**
 ******************************************************************************
 * @file    udp.c
 * @author  MCD Application Team
 * @brief   This file contains the UDP interface shared between M0 and
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

#include "udp.h"

extern otUdpReceive otUdpReceiveCb;


otError otUdpAddReceiver(otInstance *aInstance, otUdpReceiver *aUdpReceiver)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_ADD_RECEIVER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aUdpReceiver;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otUdpRemoveReceiver(otInstance *aInstance, otUdpReceiver *aUdpReceiver)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_REMOVE_RECEIVER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aUdpReceiver;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otUdpSendDatagram(otInstance *aInstance, otMessage *aMessage, otMessageInfo *aMessageInfo)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_SEND_DATAGRAM;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aMessage;
  p_ot_req->Data[1] = (uint32_t)aMessageInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otMessage *otUdpNewMessage(otInstance *aInstance, const otMessageSettings *aSettings)
{
  otMessage * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_NEW_MESSAGE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aSettings;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otMessage *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otUdpOpen(otInstance *aInstance, otUdpSocket *aSocket, otUdpReceive aCallback, void *aContext)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  otUdpReceiveCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_OPEN;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aSocket;
  p_ot_req->Data[1] = (uint32_t)aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otUdpIsOpen(otInstance *aInstance, const otUdpSocket *aSocket)
{
  bool rspData;
  
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_IS_OPEN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aSocket;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

bool otUdpIsPortInUse(otInstance *aInstance, uint16_t port)
{
  bool rspData;
  
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_IS_PORT_IN_USE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)port;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (bool)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otUdpClose(otInstance *aInstance, otUdpSocket *aSocket)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_CLOSE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aSocket;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otUdpBind(otInstance *aInstance, otUdpSocket *aSocket, const otSockAddr *aSockName, otNetifIdentifier aNetif)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_BIND;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aSocket;
  p_ot_req->Data[1] = (uint32_t)aSockName;
  p_ot_req->Data[2] = (uint32_t)aNetif;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otUdpConnect(otInstance *aInstance, otUdpSocket *aSocket, const otSockAddr *aSockName)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_CONNECT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aSocket;
  p_ot_req->Data[1] = (uint32_t)aSockName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

otError otUdpSend(otInstance *aInstance, otUdpSocket *aSocket, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
  otError rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_SEND;

  p_ot_req->Size=3;
  p_ot_req->Data[0] = (uint32_t)aSocket;
  p_ot_req->Data[1] = (uint32_t)aMessage;
  p_ot_req->Data[2] = (uint32_t)aMessageInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otError)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}

#if OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE
void otUdpForwardSetForwarder(otInstance *aInstance, otUdpForwarder aForwarder, void *aContext)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  otUdpForwardSetForwarderCb = aForwarder;

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_FORWARD_SET_FORWARDER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aContext;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}

void otUdpForwardReceive(otInstance *        aInstance,
                         otMessage *         aMessage,
                         uint16_t            aPeerPort,
                         const otIp6Address *aPeerAddr,
                         uint16_t            aSockPort)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M0TOM4_UDP_FORWARD_RECEIVE;

  p_ot_req->Size=4;
  p_ot_req->Data[0] = (uint32_t)aMessage;
  p_ot_req->Data[1] = (uint32_t)aPeerPort;
  p_ot_req->Data[2] = (uint32_t)aPeerAddr;
  p_ot_req->Data[3] = (uint32_t)aSockPort;

  Ot_Cmd_Transfer();

  Post_OtCmdProcessing();
}
#endif /* OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE */

otUdpSocket *otUdpGetSockets(otInstance *aInstance)
{
  otUdpSocket * rspData;
  
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M0TOM4_UDP_GET_SOCKETS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  rspData = (otUdpSocket *)p_ot_req->Data[0];
  
  Post_OtCmdProcessing();
  
  return rspData;
}
