/**
  ******************************************************************************
  * @file    sha1_low_level.h
  * @author  MCD Application Team
  * @brief   SHA-1 core functions
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
#ifndef __SHA1_LOW_LEVEL_H__
#define __SHA1_LOW_LEVEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Exported functions ------------------------------------------------------- */
  /* SHA1Init - Initialize new context */
  void SHA1Init  (SHA1ctx_stt *P_pHASHctx);

  /* SHA1 Update function */
  void SHA1Update(SHA1ctx_stt* P_pHASHctx, \
                  const uint8_t* P_pInput, \
                  uint32_t P_inputSize);

  /* SHA1: finalization function */
  void SHA1Final (SHA1ctx_stt* P_pHASHctx, \
                  uint8_t *P_pDigest);

#ifdef __cplusplus
}
#endif

#endif  /*__SHA1_LOW_LEVEL_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
