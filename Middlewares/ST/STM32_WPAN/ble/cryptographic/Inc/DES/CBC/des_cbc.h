/**
  ******************************************************************************
  * @file    des_cbc.h
  * @author  MCD Application Team
  * @brief   DES in CBC Mode
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
#ifndef __CRL_DES_CBC_H__
#define __CRL_DES_CBC_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @ingroup DESCBC
    * @{
    */
  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
#ifdef INCLUDE_ENCRYPTION
  /* load the key and ivec, eventually performs key schedule, etc. *****/
  int32_t DES_CBC_Encrypt_Init   (DESCBCctx_stt *P_pDESCBCctx, \
                                  const uint8_t *P_pKey,      \
                                  const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times **********************/
  int32_t DES_CBC_Encrypt_Append (DESCBCctx_stt *P_pDESCBCctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output ******************************************************/
  int32_t DES_CBC_Encrypt_Finish (DESCBCctx_stt *P_pDESCBCctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);
#endif
#ifdef INCLUDE_DECRYPTION
  /* load the key and ivec, eventually performs key schedule, etc. *****/
  int32_t DES_CBC_Decrypt_Init  (DESCBCctx_stt *P_pDESCBCctx, \
                                 const uint8_t *P_pKey,      \
                                 const uint8_t *P_pIv);

  /* launch crypto operation , can be called several times **********************/
  int32_t DES_CBC_Decrypt_Append (DESCBCctx_stt *P_pDESCBCctx,   \
                                  const uint8_t *P_pInputBuffer, \
                                  int32_t        P_inputSize,    \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);

  /* Possible final output ******************************************************/
  int32_t DES_CBC_Decrypt_Finish (DESCBCctx_stt *P_pDESCBCctx,   \
                                  uint8_t       *P_pOutputBuffer, \
                                  int32_t       *P_pOutputSize);
#endif /* decryption */
  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif /* __CRL_DES_CBC_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
