/**
  ******************************************************************************
  * @file    sha1.h
  * @author  MCD Application Team
  * @brief   Provides SHA1 functions
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
#ifndef __CRL_SHA1_H__
#define __CRL_SHA1_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup SHA1
    * @{
    */
  /* Exported constants --------------------------------------------------------*/
#define CRL_SHA1_SIZE 20   /*!<  Number of bytes (uint8_t) to store a SHA-1 digest. */

  /* Exported types ------------------------------------------------------------*/
  /**
    * @brief  Structure for the SHA-1 context
    */
  typedef HASHctx_stt SHA1ctx_stt;
  /* Exported functions --------------------------------------------------------*/
  /* Initialization of Context */
  int32_t SHA1_Init(SHA1ctx_stt *P_pSHA1ctx);
  /* Data processing function */
  int32_t SHA1_Append(SHA1ctx_stt *P_pSHA1ctx,      \
                      const uint8_t *P_pInputBuffer, \
                      int32_t P_inputSize);
  /* Returns digest */
  int32_t SHA1_Finish(SHA1ctx_stt *P_pSHA1ctx, \
                      uint8_t *P_pOutputBuffer, \
                      int32_t *P_pOutputSize);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif   /*__CRL_SHA1_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
