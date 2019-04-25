/*
  ******************************************************************************
  * @file    rng_bn.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Random Generation of BigNumers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Image SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
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
