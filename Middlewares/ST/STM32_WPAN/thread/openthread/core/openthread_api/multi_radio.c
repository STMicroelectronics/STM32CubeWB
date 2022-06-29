/**
 ******************************************************************************
 * @file    logging.c
 * @author  MCD Application Team
 * @brief   This file contains the OT wrapper for Multi Radio API
 *          shared between M0 and M4.
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

#if OPENTHREAD_CONFIG_MULTI_RADIO

#include "multi_radio.h"

otError otMultiRadioGetNeighborInfo(otInstance *              aInstance,
                                    const otExtAddress *      aExtAddress,
                                    otMultiRadioNeighborInfo *aNeighborInfo)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_MULTI_RADIO_GET_NEIGHBOR_INFO;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aExtAddress;
  p_ot_req->Data[1] = (uint32_t) aNeighborInfo;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

#endif // OPENTHREAD_CONFIG_MULTI_RADIO
