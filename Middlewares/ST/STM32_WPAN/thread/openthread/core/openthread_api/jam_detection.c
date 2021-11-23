/**
  ******************************************************************************
  * @file    jam_detection.c
  * @author  MCD Application Team
  * @brief   This file contains the Jam Detection interface shared between M0 and
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

#include "jam_detection.h"

extern otJamDetectionCallback otJamDetectionCallbackCb;

#if OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE
otError otJamDetectionSetRssiThreshold(otInstance *aInstance, int8_t aRssiThreshold)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_SET_RSSI_THRESHOLD;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aRssiThreshold;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

int8_t otJamDetectionGetRssiThreshold(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_GET_RSSI_THRESHOLD;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (int8_t)p_ot_req->Data[0];
}

otError otJamDetectionSetWindow(otInstance *aInstance, uint8_t aWindow)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_SET_WINDOW;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aWindow;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

uint8_t otJamDetectionGetWindow(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_GET_WINDOW;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (int8_t)p_ot_req->Data[0];
}

otError otJamDetectionSetBusyPeriod(otInstance *aInstance, uint8_t aBusyPeriod)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_SET_BUSY_PERIOD;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aBusyPeriod;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

uint8_t otJamDetectionGetBusyPeriod(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_GET_BUSY_PERIOD;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (int8_t)p_ot_req->Data[0];
}

otError otJamDetectionStart(otInstance *aInstance, otJamDetectionCallback aCallback, void *aContext)
{
    Pre_OtCmdProcessing();
    /* Save Callback to global variable */
    otJamDetectionCallbackCb = aCallback;

    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_START;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aContext;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

otError otJamDetectionStop(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_STOP;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError)p_ot_req->Data[0];
}

bool otJamDetectionIsEnabled(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_IS_ENABLED;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (bool)p_ot_req->Data[0];
}

bool otJamDetectionGetState(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_GET_STATE;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (bool)p_ot_req->Data[0];
}

uint64_t otJamDetectionGetHistoryBitmap(otInstance *aInstance)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_JAM_DETECTION_GET_HISTORY_BITMAP;

    p_ot_req->Size=0;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();

    /* 64bit word is returned with two 32bits words (MSB are at offset 1 and LSB are offset 0) */
    return (uint64_t)((p_ot_req->Data[1] << 32) | p_ot_req->Data[0]);
}
#endif /* OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE */
