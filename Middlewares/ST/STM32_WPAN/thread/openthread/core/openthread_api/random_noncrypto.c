/**
 ******************************************************************************
 * @file    random_noncrypto.c
 * @author  MCD Application Team
 * @brief   This file contains OpenThread software random number generator API
 *          shared between M0 and M4.
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

#include "random_noncrypto.h"
#include <mbedtls/ctr_drbg.h>

uint32_t otRandomNonCryptoGetUint32(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_GET_UINT_32;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return p_ot_req->Data[0];
}

uint8_t otRandomNonCryptoGetUint8(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_GET_UINT8;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

uint16_t otRandomNonCryptoGetUint16(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_GET_UINT16;

  p_ot_req->Size=0;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

uint8_t otRandomNonCryptoGetUint8InRange(uint8_t aMin, uint8_t aMax)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_GET_UINT8_IN_RANGE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMin;
  p_ot_req->Data[1] = (uint32_t) aMax;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

uint16_t otRandomNonCryptoGetUint16InRange(uint16_t aMin, uint16_t aMax)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_GET_UINT8_IN_RANGE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMin;
  p_ot_req->Data[1] = (uint32_t) aMax;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint16_t)p_ot_req->Data[0];
}

uint32_t otRandomNonCryptoGetUint32InRange(uint32_t aMin, uint32_t aMax)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_GET_UINT32_IN_RANGE;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aMin;
  p_ot_req->Data[1] = (uint32_t) aMax;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}

void otRandomNonCryptoFillBuffer(uint8_t *aBuffer, uint16_t aSize)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_FILL_BUFFER;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aBuffer;
  p_ot_req->Data[1] = (uint32_t) aSize;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}

uint32_t otRandomNonCryptoAddJitter(uint32_t aValue, uint16_t aJitter)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

  p_ot_req->ID = MSG_M4TOM0_OT_RANDOM_NON_CRYPTO_ADD_JITTER;

  p_ot_req->Size=2;
  p_ot_req->Data[0] = (uint32_t) aValue;
  p_ot_req->Data[1] = (uint32_t) aJitter;

  Ot_Cmd_Transfer();

  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}
