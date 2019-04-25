/**
  ******************************************************************************
  * @file    conv.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Functions used to convert data representation from/to
  *                         Big Numbers and byte (octect) arrays
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
#ifndef __CRL_CONV_H__
#define __CRL_CONV_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/
#include "bn.h"

  /** @ingroup Conv
    * @{
    */

  /* Exported functions ------------------------------------------------------- */
  /* Converts a vector of 32 bits words ("in") to a vector of 8 bits words ("out"). */
  void W32_to_W8         (const uint32_t *P_pArrayIn, \
                          uint8_t *P_pArrayOut,      \
                          int32_t P_ReqW8);

  /* Converts a vector of 8 bits words ("in") to a BigNum_stt * ("out"). ********/
  int32_t W8_to_Big      (const uint8_t *P_pArrayIn, \
                          uint32_t P_ArrayInSize,   \
                          BigNum_stt * P_outBig);

  /* Converts a BigNum_stt * ("in") to a vector of 8 bits words ("out").*********/
  void Big_to_W8         (BigNum_stt * P_inBig,     \
                          uint8_t *P_pArrayOut,     \
                          int32_t *P_pArrayOutSize);

  /* Converts a BigNum_stt * ("in") to a vector of 8 bits words of fixed size("out") */
  int32_t Big_to_fixedW8 (BigNum_stt * P_pInBig,   \
                          uint8_t *P_pArrayOut,    \
                          int32_t P_pArrayOutSize);
  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif  /*__CRL_CONV_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
