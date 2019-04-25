/**
  ******************************************************************************
  * @file    rsa.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides RSA operations with support for PKCS#1v1.5
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
  ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_RSA_H__
#define __CRL_RSA_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup RSA
    * @{
    */

  /**
    * @brief  Structure type for RSA public key
    */
  typedef struct
  {
    uint8_t  *pmModulus;    /*!< RSA Modulus */
    int32_t  mModulusSize;  /*!< Size of RSA Modulus */
    uint8_t  *pmExponent;   /*!< RSA Public Exponent */
    int32_t  mExponentSize; /*!< Size of RSA Public Exponent */
  }
  RSApubKey_stt;

  /**
    * @brief  Structure type for RSA private key
    */
  typedef struct
  {
    uint8_t  *pmModulus; /*!< RSA Modulus */
    int32_t  mModulusSize; /*!< Size of RSA Modulus */
    uint8_t  *pmExponent; /*!< RSA Private Exponent */
    int32_t  mExponentSize; /*!< Size of RSA Private Exponent */
  }
  RSAprivKey_stt;

  /**
    * @brief Structure type for input/output of PKCS#1 encryption/decryption operation
    */
  typedef struct
  {
    const uint8_t *pmInput;       /*!< Pointer to input buffer */
    int32_t mInputSize; /*!< Size of input buffer */
    uint8_t *pmOutput;      /*!< Pointer to output buffer */
  }
  RSAinOut_stt;

  int32_t RSA_PKCS1v15_Sign(const RSAprivKey_stt *P_pPrivKey, const uint8_t *P_pDigest, hashType_et P_hashType, uint8_t *P_pSignature, membuf_stt *P_pMemBuf);

  int32_t RSA_PKCS1v15_Verify(const RSApubKey_stt *P_pPubKey, const uint8_t *P_pDigest, hashType_et P_hashType, const uint8_t *P_pSignature, membuf_stt *P_pMemBuf);

  int32_t RSA_PKCS1v15_Encrypt (const RSApubKey_stt *P_pPubKey,
                                RSAinOut_stt *P_pInOut_st,
                                RNGstate_stt *P_pRandomState,
                                membuf_stt *P_pMemBuf);

  int32_t RSA_PKCS1v15_Decrypt (const RSAprivKey_stt *P_pPrivKey,
                                RSAinOut_stt *P_pInOut_st,
                                int32_t *P_pOutputSize,
                                membuf_stt *P_pMemBuf);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_RSA_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
