/**
  ******************************************************************************
  * @file    aes_xts.h
  * @author  MCD Application Team
  * @brief   AES in XTS Mode
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

#ifndef __CRL_AES_XTS_H__
#define __CRL_AES_XTS_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup AESXTS
    * @{
    */

  typedef struct
  {
    uint32_t   mContextId;   /*!< Unique ID of this context. \b Not \b used in current implementation. */
    SKflags_et mFlags;       /*!< 32 bit mFlags, used to perform keyschedule */
    const uint8_t *pmKey;    /*!< Pointer to original XTS Key buffer */
    const uint8_t *pmTweak;  /*!< Pointer to original Tweak buffer */
    int32_t   mTweakSize;    /*!< Size of the Tweak in bytes */
    uint32_t  amTweak[4]; /*!< Temporary result/Tweak */
    int32_t   mKeySize; /*!< Size of half of the XTS Key in bytes */
    uint32_t  amExpKey[CRL_AES_MAX_EXPKEY_SIZE]; /*!< Expanded AES Key 1 */
    uint32_t  amExpKey2[CRL_AES_MAX_EXPKEY_SIZE]; /*!< Expanded AES Key 2 */
  }
  AESXTSctx_stt; /*!< AES context structure for CBC mode */

  /* Exported functions --------------------------------------------------------*/
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_XTS_Encrypt_Init (AESXTSctx_stt *P_pAESXTSctx, \
                                const uint8_t *P_pKey,      \
                                const uint8_t *P_pTweak);

  /* launch crypto operation , can be called several times */
  int32_t AES_XTS_Encrypt_Append (AESXTSctx_stt *P_pAESXTSctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_XTS_Encrypt_Finish (AESXTSctx_stt *P_pAESXTSctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

#ifdef INCLUDE_DECRYPTION
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_XTS_Decrypt_Init (AESXTSctx_stt *P_pAESXTSctx, const uint8_t *P_pKey, const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times */
  int32_t AES_XTS_Decrypt_Append (AESXTSctx_stt *P_pAESXTSctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_XTS_Decrypt_Finish (AESXTSctx_stt *P_pAESXTSctx,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);
#endif
  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_AES_XTS_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
