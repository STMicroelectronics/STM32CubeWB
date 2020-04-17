/**
  ******************************************************************************
  * @file    rng_bn.h
  * @author  MCD Application Team

  * @brief   Random Generation of BigNumers
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
#ifndef __CRL_RNG_BN_H__
#define __CRL_RNG_BN_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/
#include "bn.h"
#include "../../../RNG/rng.h"

  /** @ingroup RNGBN
    * @{
    */
  /* Exported types --------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

  /* Generate random number of P_nBits bits */
  int32_t GenRndBN     (int32_t P_nBits,              \
                        BigNum_stt *P_pRandBN,        \
                        RNGstate_stt *P_pRandomState);
  \


  /* Generate random big number smaller than P_pModBN ********************************/
  int32_t GenRndBNmodN (BigNum_stt *P_pRandBN,        \
                        const BigNum_stt *P_pModBN,   \
                        RNGstate_stt *P_pRandomState);
  \

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif  /*__CRL_RNG_BN_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
