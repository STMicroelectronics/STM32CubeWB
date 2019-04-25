/**
  ******************************************************************************
  * @file    sha1.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides SHA1 functions
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
#ifndef __CRL_SHA1_H__
#define __CRL_SHA1_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup SHA1
    * @{
    */
  /* Exported constants --------------------------------------------------------*/
#define CRL_SHA1_SIZE 20   /*!<  Number of bytes (uint8_t) to store a SHA-1 digest. */

  /* Exported types ------------------------------------------------------------*/
  /**
    * @brief  Structure for the SHA-1 context
    */
  typedef HASHctx_stt SHA1ctx_stt;
  /* Exported functions --------------------------------------------------------*/
  /* Initialization of Context */
  int32_t SHA1_Init(SHA1ctx_stt *P_pSHA1ctx);
  /* Data processing function */
  int32_t SHA1_Append(SHA1ctx_stt *P_pSHA1ctx,      \
                      const uint8_t *P_pInputBuffer, \
                      int32_t P_inputSize);
  /* Returns digest */
  int32_t SHA1_Finish(SHA1ctx_stt *P_pSHA1ctx, \
                      uint8_t *P_pOutputBuffer, \
                      int32_t *P_pOutputSize);

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif   /*__CRL_SHA1_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
