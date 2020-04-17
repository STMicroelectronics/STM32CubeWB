/**
  ******************************************************************************
  * @file    hmac_md5.h
  * @author  MCD Application Team
  * @brief   Provides HMAC-MD5 functions
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
#ifndef __CRL_HMAC_MD5_H__
#define __CRL_HMAC_MD5_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup HMAC_MD5
    * @{
    */

  /**
    * @brief  HMAC-MD5 Context Structure
    */
  /* Exported types ------------------------------------------------------------*/
  typedef HMACctx_stt HMAC_MD5ctx_stt;
  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

  int32_t HMAC_MD5_Init   (HMAC_MD5ctx_stt *P_pHMAC_MD5ctx);

  int32_t HMAC_MD5_Append (HMAC_MD5ctx_stt *P_pHMAC_MD5ctx, \
                           const uint8_t *P_pInputBuffer,  \
                           int32_t P_inputSize);

  int32_t HMAC_MD5_Finish (HMAC_MD5ctx_stt *P_pHMAC_MD5ctx, \
                           uint8_t *P_pOutputBuffer,       \
                           int32_t *P_pOutputSize);

  /**
    * @}
    */


#ifdef __cplusplus
}
#endif

#endif   /* __CRL_HMAC_MD5_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
