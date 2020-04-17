/**
  ******************************************************************************
  * @file    rng.h
  * @author  MCD Application Team
  * @brief   Provides the random engine to the crypto library
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
