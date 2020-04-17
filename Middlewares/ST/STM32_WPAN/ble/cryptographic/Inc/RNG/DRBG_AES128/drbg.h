/**
  ******************************************************************************
  * @file    drbg.h
  * @author  MCD Application Team
  * @brief   Implements a pseudo random engine based on AES128-DRBG.
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
#ifndef __CRL_DRGB_H__
#define __CRL_DRGB_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup DRBG-AES128
    * @{
    */

  /* Exported types ------------------------------------------------------------*/
  /**
    * @brief  Structure to store the state/context of the DRBG_AES128
    */
  typedef struct
  {
    uint32_t v[4];           /*!<  V value, a 128 bit value */
    uint32_t key[4];         /*!<  K value, a 128 bit value */
    uint32_t reseed_counter; /*!< Reseed counter 32-bit of data */
  }
  drbg_aes128_state_stt;

  /* Exported constants --------------------------------------------------------*/


#define CRL_DRBG_AES_MAX_BYTES_PER_REQUEST 65536u      /*!< Maximum number of bytes (uint8_t) a generate request can output */
#define CRL_DRBG_AES128_ENTROPY_MIN_LEN    16u         /*!< Minimum number of bytes for the entropy input */
#define CRL_DRBG_AES_ENTROPY_MAX_LEN       32u         /*!<  Maximum number of bytes for the entropy input */
#define CRL_DRBG_AES_MAX_PERS_STR_LEN      32u         /*!< Maximum number of bytes for the personalization string */
#define CRL_DRBG_AES_MAX_ADD_INPUT_LEN     CRL_DRBG_AES_MAX_PERS_STR_LEN         /*!< Maximum number of bytes for the additional input */
#define CRL_DRBG_AES_MAX_NONCE_LEN         16u         /*!< Maximum number of bytes for the nonce */
#define CRL_DRBG_AES_REQS_BTW_RESEEDS      0xFFFFFFFF  /*!< Maximum number of request before a reseed is needed. Not supported. */
#define CRL_DRBG_AES128_STATE_SIZE         36u         /*!< Required size in bytes for a DRBG-AES128 state */

  /* Exported functions ------------------------------------------------------- */
  /* DRBG functions *************************************************************/
  /* Instantiate function for DRBG_AES128 ***************************************/
  int32_t crl_DRBG_AES128_Instantiate (uint8_t *P_pState,             \
                                       const uint8_t *P_pEntropyInput, \
                                       int32_t P_entropySize,         \
                                       const uint8_t *P_pNonce,       \
                                       int32_t P_nonceSize,           \
                                       const uint8_t *P_pPersStr,     \
                                       int32_t P_persStrSize);
  \

  /* Generate function for DRBG_AES128 ******************************************/
  int32_t crl_DRBG_AES128_Generate    (uint8_t *P_pState,         \
                                       uint8_t *P_pOutput,        \
                                       const uint8_t *P_pAddInput, \
                                       int32_t P_addInputSize,    \
                                       int32_t P_nBytes);
  \
  /* Reseed *********************************************************************/
  int32_t crl_DRBG_AES128_Reseed      (uint8_t *P_pState,             \
                                       const uint8_t *P_pEntropyInput, \
                                       int32_t P_entropySize,         \
                                       const uint8_t *P_pAddInput,    \
                                       int32_t P_addInputSize);
  \
  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif  /*__DRGB_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
