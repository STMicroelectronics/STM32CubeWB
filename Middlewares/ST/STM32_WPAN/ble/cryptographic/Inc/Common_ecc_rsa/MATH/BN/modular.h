/**
  ******************************************************************************
  * @file    modular.h
  * @author  MCD Application Team
  * @brief   Provides Base Modular Arithmetic Operations
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
#ifndef __CRL_MODULAR_H__
#define __CRL_MODULAR_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup Modular
    * @{
    */

  /* Exported types ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  /* Modular addition ***********************************************************/
  int32_t ModularAddBig    (const BigNum_stt * P_pAddend1BN, \
                            const BigNum_stt * P_pAddend2BN, \
                            BigNum_stt       * P_pModSumBN, \
                            const BigNum_stt * P_pModBN,    \
                            membuf_stt       *P_pMemBuf);
  \

  /* Modular Subtraction ********************************************************/
  int32_t ModularSubBig   (const BigNum_stt * P_pMinuendBN,   \
                           const BigNum_stt * P_pSubtrahendBN, \
                           BigNum_stt       * P_pModDiffBN,   \
                           const BigNum_stt * P_pModBN,       \
                           membuf_stt       *P_pMemBuf);
  \

  /* Fast Modular multiplication by 2 *******************************************/
  int32_t ModularFastMul2 (BigNum_stt       * P_pBN,    \
                           const BigNum_stt * P_pModBN, \
                           membuf_stt       *P_pMemBuf);
  \

  /* Modular inversion **********************************************************/
  int32_t ModularInvBig   (const BigNum_stt *P_pInputBN,  \
                           BigNum_stt       *P_pInverseBN, \
                           const BigNum_stt *P_pModBN,    \
                           membuf_stt       *P_pMemBuf );
  \

  /* Modular exponentiation *****************************************************/
  int32_t ModularExpSlidWin(BigNum_stt       * P_pBaseBN,   \
                            const BigNum_stt * P_pExpBN,    \
                            const BigNum_stt * P_pModulusBN, \
                            BigNum_stt        *P_pOutputBN, \
                            membuf_stt        *P_pMemBuf);
  \

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif  /*__CRL_MODULAR_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
