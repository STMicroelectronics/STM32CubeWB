/**
  ******************************************************************************
  * @file    radio.c
  * @author  MCD Application Team
  * @brief   This file contains the radio interface shared between M0 and
  *          M4.
  *
  *          Usually, inside the OpenThread stack, the functions located under
  *          openthread/platform don't need to be exported on M4 side since
  *          they are used internally by the stack. It is why the functions
  *          otPlatRadioXXX provided by OpenThread are not supposed to be
  *          expported to the Application layer.
  *          The only exception to this rule concerns the functions used to
  *          configure the Tx power.
  *
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

/**
 * Get the radio's transmit power in dBm.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 * @param[out] aPower    The transmit power in dBm.
 *
 * @retval OT_ERROR_NONE             Successfully retrieved the transmit power.
 * @retval OT_ERROR_INVALID_ARGS     @p aPower was NULL.
 * @retval OT_ERROR_NOT_IMPLEMENTED  Transmit power configuration via dBm is not implemented.
 *
 */
otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower)
{
   Pre_OtCmdProcessing();
   /* prepare buffer */
   Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

   p_ot_req->ID = MSG_M4TOM0_OT_RADIO_GET_TRANSMIT_POWER;

   p_ot_req->Size=1;
   p_ot_req->Data[0] = (uint32_t) aPower;

   Ot_Cmd_Transfer();

   p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
   return (otError) p_ot_req->Data[0];
}

/**
 * Set the radio's transmit power in dBm.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 * @param[in] aPower     The transmit power in dBm.
 *
 * @retval OT_ERROR_NONE             Successfully set the transmit power.
 * @retval OT_ERROR_NOT_IMPLEMENTED  Transmit power configuration via dBm is not implemented.
 *
 */
otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_RADIO_SET_TRANSMIT_POWER;

    p_ot_req->Size=1;
    p_ot_req->Data[0] = (uint32_t) aPower;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (otError) p_ot_req->Data[0];
}

