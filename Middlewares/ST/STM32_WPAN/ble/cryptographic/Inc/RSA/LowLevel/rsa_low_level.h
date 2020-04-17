/**
  ******************************************************************************
  * @file    rsa_low_level.h
  * @author  MCD Application Team
  * @brief   Provides Low Level RSA operations
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
#ifndef __CRL_RSA_LOW_LEVEL_H__
#define __CRL_RSA_LOW_LEVEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup RSAlowlevel
    * @{
    */

#define RSAEP(P_pPubKey, P_pInput, P_pOutput, P_pMemBuf) RSAVP1((P_pPubKey),(P_pInput),(P_pOutput), (P_pMemBuf))   /*!< Wrapper for RSAVP1*/
#define RSADP(P_pPrivKey, P_pInput, P_pOutput, P_pMemBuf) RSASP1((P_pPrivKey),(P_pInput),(P_pOutput), (P_pMemBuf)) /*!< Wrapper for RSADP*/


  int32_t RSASP1(const RSAprivKey_stt *P_pPrivKey, const uint8_t * P_pEM, uint8_t *P_pOutput, membuf_stt *P_pMemBuf);
  int32_t RSAVP1(const RSApubKey_stt *P_pPubKey, const uint8_t * P_pSignature, uint8_t *P_pOutput, membuf_stt *P_pMemBuf);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_RSA_LOW_LEVEL_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
