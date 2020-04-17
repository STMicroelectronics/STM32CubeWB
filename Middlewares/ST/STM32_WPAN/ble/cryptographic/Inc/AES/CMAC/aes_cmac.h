/**
  ******************************************************************************
  * @file    aes_cmac.h
  * @author  MCD Application Team
  * @brief   AES in CMAC Mode
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
  *****************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_AES_CMAC_H__
#define __CRL_AES_CMAC_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup AESCMAC
    * @{
    */

  /* Exported types ------------------------------------------------------------*/
  typedef struct
  {
    uint32_t   mContextId;   /*!< Unique ID of this context. \b Not \b used in current implementation. */
    SKflags_et mFlags; /*!< 32 bit mFlags, used to perform keyschedule and future use */
    const uint8_t *pmKey; /*!< Pointer to original Key buffer */
    const uint8_t *pmIv; /*!< Pointer to original Initialization Vector buffer */
    int32_t   mIvSize; /*!< Size of the Initialization Vector in bytes */
    uint32_t   amIv[4]; /*!< Temporary result/IV */
    int32_t   mKeySize;   /*!< Key length in bytes */
    uint32_t   amExpKey[CRL_AES_MAX_EXPKEY_SIZE];   /*!< Expanded AES key */
    const uint8_t *pmTag;   /*!< Pointer to Authentication TAG. This value must be set in decryption, and this TAG will be verified */
    int32_t mTagSize; /*!< Size of the Tag to return. This must be set by the caller prior to calling Init */
  }
  AESCMACctx_stt; /*<! AES context structure for CMAC mode */

  /* Exported functions --------------------------------------------------------*/
  int32_t AES_CMAC_Encrypt_Init  (AESCMACctx_stt *P_pAESCMACctx);

  int32_t AES_CMAC_Encrypt_Append(AESCMACctx_stt *P_pAESCMACctx, \
                                  const uint8_t  *P_pInputBuffer, \
                                  int32_t         P_inputSize);
  \

  int32_t AES_CMAC_Encrypt_Finish(AESCMACctx_stt *P_pAESCMACctx,  \
                                  uint8_t        *P_pOutputBuffer, \
                                  int32_t        *P_pOutputSize);
  \

  int32_t AES_CMAC_Decrypt_Init  (AESCMACctx_stt *P_pAESCMACctx);

  int32_t AES_CMAC_Decrypt_Append(AESCMACctx_stt *P_pAESCMACctx, \
                                  const uint8_t  *P_pInputBuffer, \
                                  int32_t         P_inputSize);
  \

  int32_t AES_CMAC_Decrypt_Finish(AESCMACctx_stt *P_pAESCMACctx,  \
                                  uint8_t        *P_pOutputBuffer, \
                                  int32_t        *P_pOutputSize);
  \


  /**
   * @}
   */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_AES_CMAC_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
