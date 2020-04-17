/**
  ******************************************************************************
  * @file    aes_gcm.h
  * @author  MCD Application Team
  * @brief   AES in GCM Mode
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
#ifndef __CRL_AES_GCM_H__
#define __CRL_AES_GCM_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup AESGCMlowlevel
    * @{
    */
  /* Exported types ------------------------------------------------------------*/
  typedef uint32_t poly_t[4]; /*!< Definition of the way a polynomial of max degree 127 is represented */

#if CRL_GFMUL==1
  typedef poly_t table16_t[16]; /*!< Definition of the type used for the precomputed table */
#elif CRL_GFMUL==2
  typedef poly_t table8x16_t[8][16]; /*!< Definition of the type used for the precomputed table */
#elif CRL_GFMUL==0
#else
#error "A value for CRL_GFMUL MUST be provided when INCLUDE_GCM is defined"
#endif

  /** @} */

  /** @ingroup AESGCM
    * @{
    */

  typedef struct
  {
    uint32_t   mContextId; /*!< Unique ID of this AES-GCM Context. \b Not \b used in current implementation. */
    SKflags_et mFlags; /*!< 32 bit mFlags, used to perform keyschedule */
    const uint8_t *pmKey; /*!< Pointer to original Key buffer */
    const uint8_t *pmIv; /*!< Pointer to original Initialization Vector buffer */
    int32_t   mIvSize; /*!< Size of the Initialization Vector in bytes. This must be set by the caller prior to calling Init */
    uint32_t   amIv[4]; /*!< This is the current IV value.*/
    int32_t   mKeySize;   /*!< AES Key length in bytes. This must be set by the caller prior to calling Init */
    const uint8_t *pmTag; /*!< Pointer to Authentication TAG. This value must be set in decryption, and this TAG will be verified */
    int32_t mTagSize; /*!< Size of the Tag to return. This must be set by the caller prior to calling Init */
    int32_t mAADsize;   /*!< Additional authenticated data size. For internal use. */
    int32_t mPayloadSize;   /*!< Payload size. For internal use. */
    poly_t mPartialAuth; /*!< Partial authentication value. For internal use. */
    uint32_t amExpKey[CRL_AES_MAX_EXPKEY_SIZE];   /*!< AES Expanded key. For internal use.  */

#if CRL_GFMUL==0
    poly_t  mPrecomputedValues; /*!< (CRL_GFMUL==0) No precomputed tables, just the polynomial. For internal use. */
#elif CRL_GFMUL==1
    table16_t mPrecomputedValues; /*!< (CRL_GFMUL==1) Precomputation of polynomial according to Shoup's 4-bit table\n (Requires 256 bytes of key-dependent data and 32 bytes of constant data). For internal use. */
#elif CRL_GFMUL==2
    table8x16_t mPrecomputedValues; /*!< (CRL_GFMUL==2) Precomputation of polynomial according to Shoup's 8-bit table\n (Requires 4096 bytes of key-dependent data and 512 bytes of constant data). For internal use. */
#endif
  }
  AESGCMctx_stt; /*!< Structure used only for AES-GCM. Used to store the expanded key and, eventually,
                        precomputed tables, according the the defined value of CRL_GFMUL in config.h */

  /* Exported functions --------------------------------------------------------*/
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_GCM_Encrypt_Init(AESGCMctx_stt *P_pAESGCMctx, \
                               const uint8_t *P_pKey,       \
                               const uint8_t *P_pIv);

  /* Header Processing Function */
  int32_t AES_GCM_Header_Append(AESGCMctx_stt *P_pAESGCMctx,    \
                                const uint8_t *P_pInputBuffer, \
                                int32_t        P_inputSize);

  /* launch crypto operation , can be called several times */
  int32_t AES_GCM_Encrypt_Append (AESGCMctx_stt *P_pAESGCMctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_GCM_Encrypt_Finish (AESGCMctx_stt *P_pAESGCMctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);
  \

  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_GCM_Decrypt_Init (AESGCMctx_stt *P_pAESGCMctx, \
                                const uint8_t *P_pKey,      \
                                const uint8_t *P_pIv);
  \

  /* launch crypto operation , can be called several times */
  int32_t AES_GCM_Decrypt_Append (AESGCMctx_stt *P_pAESGCMctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);
  \

  /* Possible final output */
  int32_t AES_GCM_Decrypt_Finish (AESGCMctx_stt *P_pAESGCMctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);


  /** @} */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_AES_GCM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
