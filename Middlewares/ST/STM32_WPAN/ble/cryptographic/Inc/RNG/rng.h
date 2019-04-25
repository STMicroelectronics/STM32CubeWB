/**
  ******************************************************************************
  * @file    rng.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides the random engine to the crypto library
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
#ifndef __CRL_RNG_H__
#define __CRL_RNG_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/
#include "DRBG_AES128/drbg.h"


  /** @addtogroup RNG
    * @{
    */
  /* Exported constants ------------------------------------------------------- */

  /* Exported types ----------------------------------------------------------- */
  /**
    * @brief  Structure that contains the RNG stat
    */
  typedef struct
  {
    uint8_t mRNGstate[CRL_DRBG_AES128_STATE_SIZE];  /*!< Underlying DRBG context. It is initialized by \ref RNGinit */

    int32_t mDRBGtype;     /*!< Used to check if the random state has been mFlag */

    uint32_t mFlag;        /*!< Used to check if the random state has been mFlag */
  }
  RNGstate_stt;


  /**
    * @brief  Structure used by RNGinit to initialize a DRBG
    */
  typedef struct
  {

    uint8_t* pmEntropyData;   /*!< The entropy data input */

    int32_t mEntropyDataSize; /*!< Size of the entropy data input */

    uint8_t* pmNonce;         /*!< The Nonce data */

    int32_t mNonceSize;       /*!< Size of the Nonce */

    uint8_t* pmPersData;      /*!< Personalization String */

    int32_t mPersDataSize;    /*!< Size of personalization string*/
  }
  RNGinitInput_stt;


  /**
    * @brief  Structure used by RNGreseed to reseed a DRBG
    */
  typedef struct
  {

    uint8_t* pmEntropyData;   /*!< The entropy data input */

    int32_t mEntropyDataSize; /*!< Size of the entropy data input */

    uint8_t* pmAddInput;      /*!< Additional input */

    int32_t mAddInputSize;    /*!< Size of additional input */
  }
  RNGreInput_stt;

  /**
    * @brief  Structure used by RNGgenBytes or RNGgenWords to provide the optional additional input
    */
  typedef struct
  {
    uint8_t* pmAddInput;      /*!< Additional input */

    int32_t mAddInputSize;    /*!< Size of additional input */
  }
  RNGaddInput_stt;


  /* Exported functions ------------------------------------------------------- */
  /* Reseed random **************************************************************/
  int32_t RNGreseed   (const RNGreInput_stt *P_pInputData, \
                       RNGstate_stt *P_pRandomState);

  /* Initialize random **********************************************************/
  int32_t RNGinit    (const RNGinitInput_stt *P_pInputData, \
                      RNGstate_stt *P_pRandomState);
  /* Free random ****************************************************************/
  int32_t RNGfree    (RNGstate_stt *P_pRandomState );

  /* Generate random octets to a buffer *****************************************/
  int32_t RNGgenBytes(RNGstate_stt *P_pRandomState,      \
                      const RNGaddInput_stt *P_pAddInput, \
                      uint8_t *P_pOutput,                \
                      int32_t P_OutLen);

  /* Return a random int32_t ****************************************************/
  int32_t RNGgenWords(RNGstate_stt *P_pRandomState,      \
                      const RNGaddInput_stt *P_pAddInput, \
                      uint32_t *P_pWordBuf,              \
                      int32_t P_BufSize);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */


#endif /* __CRL_RAND_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
