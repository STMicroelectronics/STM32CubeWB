/**
  ******************************************************************************
  * @file    aes_keywrap.h
  * @author  MCD Application Team
  * @brief   AES in KeyWrap Mode
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
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_AES_KEYWRAP_H__
#define __CRL_AES_KEYWRAP_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup AESKEYWRAP
    * @{
    */

  /* Exported types ------------------------------------------------------------*/
  /** AES context structure for Key Wrap mode */
  typedef AESCBCctx_stt AESKWctx_stt;
  /* Exported constants --------------------------------------------------------*/
  /** Number of uint32_t words in a Key Wrap Block */
#define  CRL_KWRAP_BLOCK_SIZE 2
  /** Number of uint8_t bytes in a Key Wrap Block */
#define  CRL_AES_KWRAP_BLOCK 8

  /* Exported functions --------------------------------------------------------*/
#ifdef INCLUDE_ENCRYPTION
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_KeyWrap_Encrypt_Init (AESKWctx_stt *P_pAESKWctx, const uint8_t *P_pKey, const uint8_t *P_pIv);

  /* launch crypto operation , can be called just one times */
  int32_t AES_KeyWrap_Encrypt_Append (AESKWctx_stt *P_pAESKWctx,
                                      const uint8_t *P_pInputBuffer,
                                      int32_t        P_inputSize,
                                      uint8_t       *P_pOutputBuffer,
                                      int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_KeyWrap_Encrypt_Finish (AESKWctx_stt *P_pAESKWctx,
                                      uint8_t       *P_pOutputBuffer,
                                      int32_t       *P_pOutputSize);
#endif /* ENC */
#ifdef INCLUDE_DECRYPTION
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_KeyWrap_Decrypt_Init (AESKWctx_stt *P_pAESKWctx, const uint8_t *P_pKey, const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times */
  int32_t AES_KeyWrap_Decrypt_Append (AESKWctx_stt *P_pAESKWctx,
                                      const uint8_t *P_pInputBuffer,
                                      int32_t        P_inputSize,
                                      uint8_t       *P_pOutputBuffer,
                                      int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_KeyWrap_Decrypt_Finish (AESKWctx_stt *P_pAESKWctx,
                                      uint8_t       *P_pOutputBuffer,
                                      int32_t       *P_pOutputSize);
#endif

  /** @} */

#ifdef __cplusplus
}
#endif

#endif /*__CRL_AES_KEYWRAP_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
