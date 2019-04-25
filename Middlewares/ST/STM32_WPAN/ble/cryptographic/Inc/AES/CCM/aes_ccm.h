/**
  ******************************************************************************
  * @file    aes_ccm.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   AES in CCM (Counter with CBC-MAC) Mode
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Image SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_AES_CCM_H__
#define __CRL_AES_CCM_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup AESCCM
    * @{
    */

  /* Exported types ------------------------------------------------------------*/
  /** Structure used only for AES-GCM. Used to store the expanded key and, eventually,
  *   precomputed tables, according the the defined value of CRL_GFMUL in config.h */
  typedef struct
  {
    /** Unique ID of this AES-GCM Context. \b Not \b used in current implementation. */
    uint32_t   mContextId;
    /** 32 bit mFlags, used to perform keyschedule */
    SKflags_et mFlags;
    /** Pointer to original Key buffer */
    const uint8_t *pmKey;
    /** Pointer to original Nonce buffer */
    const uint8_t *pmNonce;
    /** Size of the Nonce in bytes. This must be set by the caller prior to calling Init.
         Possible values are {7,8,9,10,11,12,13}  */
    int32_t   mNonceSize;
    /** This is the current IV value for encryption.*/
    uint32_t   amIvCTR[4];
    /** This is the current IV value for authentication.*/
    uint32_t   amIvCBC[4];
    /** AES Key length in bytes. This must be set by the caller prior to calling Init */
    int32_t   mKeySize;
    /** Pointer to Authentication TAG. This value must be set in decryption, and this TAG will be verified */
    const uint8_t *pmTag;
    /** Size of the Tag to return. This must be set by the caller prior to calling Init.
        Possible values are values are {4,6,8,10,12,14,16} */
    int32_t mTagSize;
    /** Size of the associated data to be processed yet. This must be set by the caller prior to calling Init.*/
    int32_t mAssDataSize;
    /** Size of the payload data to be processed yet size. This must be set by the caller prior to calling Init. */
    int32_t mPayloadSize;
    /** AES Expanded key. For internal use.  */
    uint32_t amExpKey[CRL_AES_MAX_EXPKEY_SIZE];
    /** Temp Buffer */
    uint32_t amTmpBuf[CRL_AES_BLOCK/sizeof(uint32_t)];
    /**  Number of bytes actually in use */
    int32_t mTmpBufUse;
  }
  AESCCMctx_stt;

  /* Exported functions --------------------------------------------------------*/
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_CCM_Encrypt_Init   (AESCCMctx_stt *P_pAESCCMctx, \
                                  const uint8_t *P_pKey,      \
                                  const uint8_t *P_pNonce);

  /* Header Processing Function */
  int32_t AES_CCM_Header_Append  (AESCCMctx_stt *P_pAESCCMctx,  \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize);

  /* launch crypto operation , can be called several times */
  int32_t AES_CCM_Encrypt_Append (AESCCMctx_stt *P_pAESCCMctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_CCM_Encrypt_Finish (AESCCMctx_stt *P_pAESCCMctx,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_CCM_Decrypt_Init   (AESCCMctx_stt *P_pAESCCMctx,   \
                                  const uint8_t *P_pKey,         \
                                  const uint8_t *P_pNonce);

  /* launch crypto operation , can be called several times */
  int32_t AES_CCM_Decrypt_Append (AESCCMctx_stt *P_pAESCCMctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_CCM_Decrypt_Finish (AESCCMctx_stt *P_pAESCCMctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /**
   * @} 
   */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_AES_CCM_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
