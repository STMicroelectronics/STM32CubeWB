/**
  ******************************************************************************
  * @file    aes_ctr.h
  * @author  MCD Application Team
  * @brief   AES in CTR Mode
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Image license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
  *
  ****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_AES_CTR_H__
#define __CRL_AES_CTR_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup AESCTR
    * @{
    */

  /* Exported types ------------------------------------------------------------*/
  /** CTR context = CBC context. AESCTRctx_stt is equal to AESCBCctx_stt */
  typedef AESCBCctx_stt AESCTRctx_stt;
  /* Exported functions --------------------------------------------------------*/
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_CTR_Encrypt_Init   (AESCTRctx_stt *P_pAESCTRctx, \
                                  const uint8_t *P_pKey,      \
                                  const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times */
  int32_t AES_CTR_Encrypt_Append (AESCTRctx_stt *P_pAESCTRctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_CTR_Encrypt_Finish (AESCTRctx_stt *P_pAESCTRctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_CTR_Decrypt_Init   (AESCTRctx_stt *P_pAESCTRctx, \
                                  const uint8_t *P_pKey,      \
                                  const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times */
  int32_t AES_CTR_Decrypt_Append (AESCTRctx_stt *P_pAESCTRctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_CTR_Decrypt_Finish (AESCTRctx_stt *P_pAESCTRctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /** @} */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_AES_CTR_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
