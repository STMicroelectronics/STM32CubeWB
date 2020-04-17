/**
  ******************************************************************************
  * @file    sha224.h
  * @author  MCD Application Team
  * @brief   Provides SHA224 functions
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
#ifndef __CRL_SHA224_H__
#define __CRL_SHA224_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup SHA224
    * @{
    */
  /* Exported constants --------------------------------------------------------*/
#define CRL_SHA224_SIZE 28 /*!<  Number of bytes (uint8_t) to store a SHA-224 digest. */

  /* Exported functions --------------------------------------------------------*/
  /**  Structure for the SHA-224 context */
  typedef HASHctx_stt SHA224ctx_stt;
  /* Initialization of Context */
  int32_t SHA224_Init  (SHA224ctx_stt *P_pSHA224ctx);
  /* Data processing function */
  int32_t SHA224_Append (SHA224ctx_stt *P_pSHA224ctx,  \
                         const uint8_t *P_pInputBuffer, \
                         int32_t P_inputSize);
  /* Returns digest */
  int32_t SHA224_Finish (SHA224ctx_stt *P_pSHA224ctx, \
                         uint8_t *P_pOutputBuffer,   \
                         int32_t *P_pOutputSize);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif   /*__CRL_SHA224_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
