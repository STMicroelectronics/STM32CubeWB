/**
  ******************************************************************************
  * @file    hkdf512.h
  * @author  MCD Application Team
  * @brief   Provides HKDF-SHA512 functions
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
#ifndef __CRL_HKDF_SHA512_H__
#define __CRL_HKDF_SHA512_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/
#include "hmac_sha512.h"

  /** @ingroup HKDF_SHA512
    * @{
    */


  /**
  * @brief  Structure HKDF-SHA-512 input
  */
  typedef struct
  {
    const uint8_t *pmKey;   /*!< Pointer for the HKDF Key */
    int32_t  mKeySize;      /*!< Size of the HKDF Key */
    const uint8_t *pmSalt;  /*!< Pointer for the HKDF Salt */
    int32_t  mSaltSize;     /*!< Size of the HKDF Salt */
    const uint8_t *pmInfo;  /*!< Pointer for the HKDF Info */
    int32_t  mInfoSize;     /*!< Size of the HKDF Info */
  }
  HKDFinput_stt;



  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  int32_t HKDF_SHA512(const HKDFinput_stt *P_pInputSt, uint8_t *P_pOutputBuffer, int32_t P_OutputSize);
  /**
    * @}
    */


#ifdef __cplusplus
}
#endif

#endif   /* __CRL_HKDF_SHA512_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
