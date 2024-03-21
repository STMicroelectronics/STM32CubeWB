/**
 ******************************************************************************
 * @file    mesh_diag.c
 * @author  GPM WBL Team
 * @brief   This file contains OpenThread mesh diagnostic API shared between
 *          M0 and M4.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2023 STMicroelectronics.
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

#include "mesh_diag.h"

#if OPENTHREAD_CONFIG_MESH_DIAG_ENABLE && OPENTHREAD_FTD

extern otMeshDiagDiscoverCallback otMeshDiagDiscoverCb;

otError otMeshDiagDiscoverTopology(otInstance                     *aInstance,
                                   const otMeshDiagDiscoverConfig *aConfig,
                                   otMeshDiagDiscoverCallback      aCallback,
                                   void                           *aContext)
{
  Pre_OtCmdProcessing();
  
  otMeshDiagDiscoverCb = aCallback;
    
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_MESH_DIAG_DISCOVER_TOPOLOGY;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aConfig;
  p_ot_req->Data[1] = (uint32_t) aContext;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0]; 
}

otError otMeshDiagGetNextIp6Address(otMeshDiagIp6AddrIterator *aIterator, otIp6Address *aIp6Address)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_MESH_DIAG_GET_NEXT_IP6_ADDRESS;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aIp6Address;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0];
}

otError otMeshDiagGetNextChildInfo(otMeshDiagChildIterator *aIterator, otMeshDiagChildInfo *aChildInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_MESH_DIAG_GET_NEXT_CHILD_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aIterator;
  p_ot_req->Data[1] = (uint32_t) aChildInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (otError)p_ot_req->Data[0]; 
}
#endif /* OPENTHREAD_CONFIG_MESH_DIAG_ENABLE && OPENTHREAD_FTD */
