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
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_ADD_RECEIVER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aUdpReceiver;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otUdpRemoveReceiver(otInstance *aInstance, otUdpReceiver *aUdpReceiver)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_REMOVE_RECEIVER;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aUdpReceiver;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otUdpSendDatagram(otInstance *aInstance, otMessage *aMessage, otMessageInfo *aMessageInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_SEND_DATAGRAM;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aMessage;
  p_ot_req->Data[1] = (uint32_t)aMessageInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otMessage *otUdpNewMessage(otInstance *aInstance, const otMessageSettings *aSettings)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_NEW_MESSAGE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aSettings;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otMessage*)p_ot_req->Data[0];
}

otError otUdpOpen(otInstance *aInstance, otUdpSocket *aSocket, otUdpReceive aCallback, void *aContext)
{
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
  return (otError)p_ot_req->Data[0];
}

bool otUdpIsOpen(otInstance *aInstance, const otUdpSocket *aSocket)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_IS_OPEN;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aSocket;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

bool otUdpIsPortInUse(otInstance *aInstance, uint16_t port)
{
  Pre_OtCmdProcessing();

  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_IS_PORT_IN_USE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)port;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

otError otUdpClose(otInstance *aInstance, otUdpSocket *aSocket)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_CLOSE;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aSocket;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otUdpBind(otInstance *aInstance, otUdpSocket *aSocket, const otSockAddr *aSockName, otNetifIdentifier aNetif)
{
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
  return (otError)p_ot_req->Data[0];
}

otError otUdpConnect(otInstance *aInstance, otUdpSocket *aSocket, const otSockAddr *aSockName)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_UDP_CONNECT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t)aSocket;
  p_ot_req->Data[1] = (uint32_t)aSockName;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otUdpSend(otInstance *aInstance, otUdpSocket *aSocket, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
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
  return (otError)p_ot_req->Data[0];
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

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
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

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}
#endif /* OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE */

#if OPENTHREAD_CONFIG_PLATFORM_UDP_ENABLE
otUdpSocket *otUdpGetSockets(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M0TOM4_UDP_GET_SOCKETS;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otUdpSocket *)p_ot_req->Data[0];
}
#endif /* OPENTHREAD_CONFIG_PLATFORM_UDP_ENABLE */
