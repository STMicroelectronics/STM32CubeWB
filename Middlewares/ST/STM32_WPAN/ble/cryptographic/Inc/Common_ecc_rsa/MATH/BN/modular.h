/**
  ******************************************************************************
  * @file    modular.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides Base Modular Arithmetic Operations
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
