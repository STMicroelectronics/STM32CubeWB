/**
  ******************************************************************************
  * @file    crypto.c
  * @author  MCD Application Team
  * @brief   This file contains the Crypto interface shared between M0 and
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

#include "crypto.h"

void otCryptoHmacSha256(const otCryptoKey *aKey, const uint8_t *aBuf, uint16_t aBufLength, otCryptoSha256Hash *aHash)
{

    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_CRYPTO_HMAC_SHA256;

    p_ot_req->Size=4;
    p_ot_req->Data[0] = (uint32_t) aKey;
    p_ot_req->Data[1] = (uint32_t) aBuf;
    p_ot_req->Data[2] = (uint32_t) aBufLength;
    p_ot_req->Data[3] = (uint32_t) aHash;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}


void otCryptoAesCcm(const otCryptoKey *aKey,
                    uint8_t            aTagLength,
                    const void *       aNonce,
                    uint8_t            aNonceLength,
                    const void *       aHeader,
                    uint32_t           aHeaderLength,
                    void *             aPlainText,
                    void *             aCipherText,
                    uint32_t           aLength,
                    bool               aEncrypt,
                    void *             aTag)
{
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();

    p_ot_req->ID = MSG_M4TOM0_OT_CRYPTO_AES_CCM;

    p_ot_req->Size=11;
    p_ot_req->Data[0]  = (uint32_t) aKey;
    p_ot_req->Data[1]  = (uint32_t) aTagLength;
    p_ot_req->Data[2]  = (uint32_t) aNonce;
    p_ot_req->Data[3]  = (uint32_t) aNonceLength;
    p_ot_req->Data[4]  = (uint32_t) aHeader;
    p_ot_req->Data[5]  = aHeaderLength;
    p_ot_req->Data[6]  = (uint32_t) aPlainText;
    p_ot_req->Data[7]  = (uint32_t) aCipherText;
    p_ot_req->Data[8]  = (uint32_t) aLength;
    p_ot_req->Data[9]  = (uint32_t) aEncrypt;
    p_ot_req->Data[10] = (uint32_t) aTag;

    Ot_Cmd_Transfer();

    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
}
