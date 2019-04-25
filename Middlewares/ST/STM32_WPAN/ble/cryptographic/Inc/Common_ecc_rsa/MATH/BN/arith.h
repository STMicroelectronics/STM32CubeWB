/**
  ******************************************************************************
  * @file    arith.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Basic Arithmetic Functions for Big Numbers (Multi Precision Integers)
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
#ifndef __CRL_ARITH_H__
#define __CRL_ARITH_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/
#include "bn.h"

  /** @ingroup Arith
    * @{
    */

  /* Exported functions ------------------------------------------------------- */

  /* z=x+y.**********************************************************************/
  int32_t AddBig           (const BigNum_stt *P_pAddend1BN, \
                            const BigNum_stt *P_pAddend2BN, \
                            BigNum_stt *P_pSumBN,          \
                            membuf_stt *P_pMemBuf);

  /* z=x-y. *********************************************************************/
  int32_t SubBig           (const BigNum_stt *P_pMinuendBN,   \
                            const BigNum_stt *P_pSubtrahendBN, \
                            BigNum_stt * P_pDifferenceBN,     \
                            membuf_stt *P_pMemBuf);

  /* x=x<<1.
  * Left shift of 1 bit. */
  int32_t FastMul2         (BigNum_stt *P_pBN);

  void FastDiv2            (BigNum_stt *P_pBN);

  /* x=x<<(esp words).  Left shift of esp words. ********************************/
  int32_t FastMulInnerBase (BigNum_stt *P_pBN, \
                            uint16_t P_exp);

  /* x=x>>(esp words).  Right shift of esp words. *******************************/
  void FastDivInnerBase    (BigNum_stt * P_pBN, \
                            uint16_t P_exp);

  /* x=x<<shift. Left shift of 'shift' bits. Condition: 0<shift<32. *************/
  int32_t LeftShift        (BigNum_stt * P_pBN, \
                            uint32_t P_shift);
  \

  void RightShift          (BigNum_stt *P_pBN, \
                            uint32_t P_shift);

  /* x=x*y. *********************************************************************/
  int32_t MulBigbyUnInt    (BigNum_stt *P_pBN,  \
                            uint32_t P_factor2);
  \


  /* Core Subtraction function, the signs are not used. P_difference = |P_minuend| - |P_subtrahend| */
  int32_t Sub_Moduli_Big   (const BigNum_stt *P_pMinuendBN,   \
                            const BigNum_stt *P_pSubtrahendBN, \
                            BigNum_stt *P_pDifferenceBN);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif  /*__CRL_BIG_H__*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
