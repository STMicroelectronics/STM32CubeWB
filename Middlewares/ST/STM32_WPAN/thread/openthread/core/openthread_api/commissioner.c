/**
  ******************************************************************************
  * @file    commissioner.c
  * @author  MCD Application Team
  * @brief   This file contains the commissioner interface shared between M0 and
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
#include "commissioner.h"

extern otCommissionerEnergyReportCallback otCommissionerEnergyReportCb;
extern otCommissionerPanIdConflictCallback otCommissionerPanIdConflictCb;


#if OPENTHREAD_FTD && OPENTHREAD_ENABLE_COMMISSIONER

OTAPI otError OTCALL otCommissionerStart(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_START;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerStop(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_STOP;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerAddJoiner(otInstance *aInstance, const otExtAddress *aEuid64,
                                             const char *aPSKd, uint32_t aTimeout)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_ADD_JOINER;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t) aEuid64;
    p_ot_req->Data[1] = (uint32_t) aPSKd;
    p_ot_req->Data[2] = (uint32_t) aTimeout;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerRemoveJoiner(otInstance *aInstance, const otExtAddress *aEuid64)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_REMOVE_JOINER;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aEuid64;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

const char *otCommissionerGetProvisioningUrl(otInstance *aInstance, uint16_t *aLength)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_PROVISIONING_URL;

  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t) aLength;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (char*)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerSetProvisioningUrl(otInstance *aInstance, const char *aProvisioningUrl)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_SET_PROVISIONING_URL;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aProvisioningUrl;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerAnnounceBegin(otInstance *aInstance, uint32_t aChannelMask, uint8_t aCount,
                                                 uint16_t aPeriod,
                                                 const otIp6Address *aAddress)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_ANNOUNCE_BEGIN;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aChannelMask;
    p_ot_req->Data[1] = (uint32_t) aCount;
    p_ot_req->Data[2] = (uint32_t) aPeriod;
    p_ot_req->Data[3] = (uint32_t) aAddress;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerEnergyScan(otInstance *aInstance, uint32_t aChannelMask, uint8_t aCount,
                                              uint16_t aPeriod, uint16_t aScanDuration, const otIp6Address *aAddress,
                                              otCommissionerEnergyReportCallback aCallback, void *aContext)
{
    Pre_OtCmdProcessing();
    otCommissionerEnergyReportCb = aCallback;
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_ENERGY_SCAN;

    p_ot_req->Size=6;
    p_ot_req->Data[0] = (uint32_t) aChannelMask;
    p_ot_req->Data[1] = (uint32_t) aCount;
    p_ot_req->Data[2] = (uint32_t) aPeriod;
    p_ot_req->Data[3] = (uint32_t) aScanDuration;
    p_ot_req->Data[4] = (uint32_t) aAddress;
    p_ot_req->Data[5] = (uint32_t) aContext;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerPanIdQuery(otInstance *aInstance, uint16_t aPanId, uint32_t aChannelMask,
                                              const otIp6Address *aAddress,
                                              otCommissionerPanIdConflictCallback aCallback, void *aContext)
{
    Pre_OtCmdProcessing();
    otCommissionerPanIdConflictCb = aCallback;
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_PANID_QUERY;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aPanId;
    p_ot_req->Data[1] = (uint32_t) aChannelMask;
    p_ot_req->Data[2] = (uint32_t) aAddress;
    p_ot_req->Data[3] = (uint32_t) aContext;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerSendMgmtGet(otInstance *aInstance, const uint8_t *aTlvs, uint8_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_SEND_MGMT_GET;

    p_ot_req->Size=2;
    p_ot_req->Data[0] = (uint32_t) aTlvs;
    p_ot_req->Data[1] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerSendMgmtSet(otInstance *aInstance, const otCommissioningDataset *aDataset,
                                               const uint8_t *aTlvs, uint8_t aLength)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_SEND_MGMT_SET;

    p_ot_req->Size=3;
    p_ot_req->Data[0] = (uint32_t) aDataset;
    p_ot_req->Data[1] = (uint32_t) aTlvs;
    p_ot_req->Data[2] = (uint32_t) aLength;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

OTAPI uint16_t OTCALL otCommissionerGetSessionId(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_SESSION_ID;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint32_t)p_ot_req->Data[0];
}

OTAPI otCommissionerState OTCALL otCommissionerGetState(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GET_STATE;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otCommissionerState) p_ot_req->Data[0];
}

OTAPI otError OTCALL otCommissionerGeneratePSKc(otInstance *aInstance, const char *aPassPhrase,
                                                const char *aNetworkName, const otExtendedPanId *aExtPanId,
                                                uint8_t *aPSKc)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_COMMISSIONER_GENERATE_PSKC;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aPassPhrase;
    p_ot_req->Data[1] = (uint32_t) aNetworkName;
    p_ot_req->Data[2] = (uint32_t) aExtPanId;
    p_ot_req->Data[3] = (uint32_t) aPSKc;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

#endif /* OPENTHREAD_FTD && OPENTHREAD_ENABLE_COMMISSIONER */
