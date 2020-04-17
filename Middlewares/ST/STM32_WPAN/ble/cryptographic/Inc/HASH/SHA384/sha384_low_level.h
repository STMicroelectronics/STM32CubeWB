/**
  ******************************************************************************
  * @file    sha384_low_level.h
  * @author  MCD Application Team
  * @brief   SHA-384 core functions
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
#ifndef __SHA384_LOW_LEVEL_H__
#define __SHA384_LOW_LEVEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Exported functions ------------------------------------------------------- */
  /* SHA-384 Initialize new context */
  void SHA384Init(SHA384ctx_stt* P_pSHA384ctx);
  /* SHA-384 finalization function */
  void SHA384Final(SHA384ctx_stt* P_pSHA384ctx, \
                   uint8_t *P_pDigest);

#ifdef __cplusplus
}
#endif

#endif  /*__SHA384_LOW_LEVEL_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
