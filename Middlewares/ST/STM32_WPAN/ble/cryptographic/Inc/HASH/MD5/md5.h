/**
  ******************************************************************************
  * @file    md5.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides MD5 functions
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
#ifndef __CRL_MD5_H__
#define __CRL_MD5_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup MD5
    * @{
    */

  /* Exported constants --------------------------------------------------------*/
#define CRL_MD5_SIZE  16   /*!< Number of bytes (uint8_t) to store a MD5 digest. */

  /**
    * @brief  Structure for the MD5 context
    */
  /* Exported types ------------------------------------------------------------*/

  typedef HASHctx_stt MD5ctx_stt;

  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  /* Initialization of Context */
  int32_t MD5_Init   (MD5ctx_stt *P_pMD5ctx);
  /* Data processing function */
  int32_t MD5_Append (MD5ctx_stt *P_pMD5ctx,        \
                      const uint8_t *P_pInputBuffer, \
                      int32_t P_inputSize);
  \
  /* Returns digest */
  int32_t MD5_Finish (MD5ctx_stt *P_pMD5ctx,   \
                      uint8_t *P_pOutputBuffer, \
                      int32_t *P_pOutputSize);
  \
  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif   /*__CRL_MD5_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
