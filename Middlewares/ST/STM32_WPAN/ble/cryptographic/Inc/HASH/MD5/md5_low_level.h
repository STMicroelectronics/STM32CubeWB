/**
  ******************************************************************************
  * @file    md5_low_level.h
  * @author  MCD Application Team
  * @brief   MD5 core functions
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
#ifndef __MD5_LOW_LEVEL_H__
#define __MD5_LOW_LEVEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  /* MD5 - Initialize new context */
  void crl_u_MD5_Init   (MD5ctx_stt *P_pMD5ctx);
  /* MD5 Update function */
  void crl_u_MD5_Update (MD5ctx_stt *P_pMD5ctx,  \
                         const uint8_t *P_pInput, \
                         uint32_t P_inputSize);
  /* MD5 finalization function */
  void crl_u_MD5_Finish (MD5ctx_stt *P_pMD5ctx, \
                         uint8_t *P_pDigest);


#ifdef __cplusplus
}
#endif

#endif  /*__MD5_LOW_LEVEL_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
