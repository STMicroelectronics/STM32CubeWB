/**
  ******************************************************************************
  * @file    aes_ofb.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   AES in OFB Mode
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

#ifndef __CRL_AES_OFB_H__
#define __CRL_AES_OFB_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup AESOFB
    * @{
    */

  /** OFB context = CBC context. AESOFBctx_stt is equal to AESCBCctx_stt */
  typedef AESCBCctx_stt AESOFBctx_stt;

#ifdef INCLUDE_ENCRYPTION
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_OFB_Encrypt_Init (AESOFBctx_stt *P_pAESOFBctx, \
                                const uint8_t *P_pKey,      \
                                const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times */
  int32_t AES_OFB_Encrypt_Append (AESOFBctx_stt *P_pAESOFBctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_OFB_Encrypt_Finish (AESOFBctx_stt *P_pAESOFBctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);
#endif
#ifdef INCLUDE_DECRYPTION
  /* load the key and ivec, eventually performs key schedule, etc. */
  int32_t AES_OFB_Decrypt_Init (AESOFBctx_stt *P_pAESOFBctx, \
                                const uint8_t *P_pKey,      \
                                const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times */
  int32_t AES_OFB_Decrypt_Append (AESOFBctx_stt *P_pAESOFBctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output */
  int32_t AES_OFB_Decrypt_Finish (AESOFBctx_stt *P_pAESOFBctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);
#endif
  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_AES_OFB_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
