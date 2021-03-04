/**
 ******************************************************************************
 * @file    instance.c
 * @author  MCD Application Team
 * @brief   This file contains the Instance interface shared between M0 and
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

#include "instance.h"

extern otStateChangedCallback otStateChangedCb;


#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
otInstance *otInstanceInit(void *aInstanceBuffer, size_t *aInstanceBufferSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_INIT;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aInstanceBuffer;
  p_ot_req->Data[1] = (uint32_t) aInstanceBufferSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otInstance *)p_ot_req->Data[0];
}
#else
otInstance *otInstanceInitSingle(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_INIT_SINGLE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otInstance *)p_ot_req->Data[0];
}
#endif /* #if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE */

bool otInstanceIsInitialized(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_IS_INITIALIZED;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (bool)p_ot_req->Data[0];
}

void otInstanceFinalize(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_FINALIZE;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#if OPENTHREAD_MTD || OPENTHREAD_FTD
otError otSetStateChangedCallback(otInstance *aInstance, otStateChangedCallback aCallback,
    void *aContext)
{
  Pre_OtCmdProcessing();
  /* Store the callback function */
  otStateChangedCb = aCallback;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_SET_STATE_CHANGED_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

void otRemoveStateChangeCallback(otInstance *aInstance, otStateChangedCallback aCallback,
    void *aCallbackContext)
{
  Pre_OtCmdProcessing();
  /* RESET the callback function */
  otStateChangedCb = NULL;
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_REMOVE_STATE_CHANGED_CALLBACK;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)aCallbackContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otInstanceReset(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_RESET;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

void otInstanceFactoryReset(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_FACTORY_RESET;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

otError otInstanceErasePersistentInfo(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_INSTANCE_ERASE_PERSISTENT_INFO;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}
#endif // OPENTHREAD_MTD || OPENTHREAD_FTD

const char *otGetVersionString(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_OPENTHREAD_GET_VERSION_STRING;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char *)p_ot_req->Data[0];
}

const char *otGetRadioVersionString(otInstance *aInstance)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_OPENTHREAD_GET_RADIO_VERSION_STRING;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char *)p_ot_req->Data[0];
}
