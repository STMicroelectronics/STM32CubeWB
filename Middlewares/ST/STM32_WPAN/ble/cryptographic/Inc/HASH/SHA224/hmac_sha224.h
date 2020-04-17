/**
  ******************************************************************************
  * @file    hmac_sha224.h
  * @author  MCD Application Team
  * @brief   Provides HMAC-SHA224 functions
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
#ifndef __CRL_HMAC_SHA224_H__
#define __CRL_HMAC_SHA224_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup HMAC_SHA224
    * @{
    */

  /**
    * @brief  HMAC-SHA-224 Context Structure
    */
  /* Exported constants --------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  typedef HMACctx_stt HMAC_SHA224ctx_stt;


  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

  int32_t HMAC_SHA224_Init   (HMAC_SHA224ctx_stt *P_pHMAC_SHA224ctx);

  int32_t HMAC_SHA224_Append (HMAC_SHA224ctx_stt *P_pHMAC_SHA224ctx, \
                              const uint8_t *P_pInputBuffer,        \
                              int32_t P_inputSize);

  int32_t HMAC_SHA224_Finish (HMAC_SHA224ctx_stt *P_pHMAC_SHA224ctx, \
                              uint8_t *P_pOutputBuffer,             \
                              int32_t *P_pOutputSize);

  /**
    * @}
    */


#ifdef __cplusplus
}
#endif

#endif   /* __CRL_HMAC_SHA224_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
