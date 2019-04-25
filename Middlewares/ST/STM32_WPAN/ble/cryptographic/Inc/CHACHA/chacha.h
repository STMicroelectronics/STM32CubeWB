/**
******************************************************************************
* @file    chacha.h
* @author  MCD Application Team
* @version V3.0.0
* @date    05-June-2015
* @brief   ChaCha20 header file
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Image SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
* http://www.st.com/software_license_agreement_liberty_v2
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
#ifndef __CHACHA_H__
#define __CHACHA_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @addtogroup CHACHA
  * @{
  */
  /* Includes ------------------------------------------------------------------*/
#include <stdint.h>
  /* Exported types ------------------------------------------------------------*/

  typedef struct
  {
    uint32_t   mContextId; /*!< Unique ID of this context. \b Not \b used in current implementation. */
    SKflags_et mFlags; /*!< 32 bit mFlags, used to perform keyschedule and future use */
    const uint8_t *pmKey; /*!< Pointer to original Key buffer */
    const uint8_t *pmIv; /*!< Pointer to original Initialization Vector buffer */
    int32_t   mIvSize; /*!< Size of the Initialization Vector in bytes */
    int32_t   mKeySize; /*!< Key length in bytes */
    uint64_t  mCounter; /*!< Value for counter, will allow to process not contiguous data. Set flag E_SK_SET_COUNTER to use it, otherwise it's ignored */
    uint32_t  amState[16]; /*!< ChaCha Internal State */
  }
  CHACHActx_stt; /*!< Chacha context structure */

  /* Exported constants --------------------------------------------------------*/
#define CRL_CHACHA128_KEY   16 /*!< Number of bytes (uint8_t) necessary to store a ChaCha key of 128 bits. */
#define CRL_CHACHA256_KEY   32 /*!< Number of bytes (uint8_t) necessary to store a Chacha key of 256 bits. */
#define CRL_CHACHA_NONCE     8 /*!< Number of bytes (uint8_t) necessary to store a Chacha Nonce of 64 bits. */
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  int32_t CHACHA_Encrypt_Init(CHACHActx_stt *P_pCHACHActx, const uint8_t *P_pKey, const uint8_t *P_pIv);

  int32_t CHACHA_Encrypt_Append(CHACHActx_stt *P_pCHACHActx,
                                const uint8_t *P_pInputBuffer,
                                int32_t        P_inputSize,
                                uint8_t       *P_pOutputBuffer,
                                int32_t       *P_pOutputSize);

  int32_t CHACHA_Encrypt_Finish(CHACHActx_stt *P_pCHACHActx,
                                uint8_t       *P_pOutputBuffer,
                                int32_t       *P_pOutputSize);

  int32_t CHACHA_Decrypt_Init(CHACHActx_stt *P_pCHACHActx, const uint8_t *P_pKey, const uint8_t *P_pIv);

  int32_t CHACHA_Decrypt_Append(CHACHActx_stt *P_pCHACHActx,
                                const uint8_t *P_pInputBuffer,
                                int32_t        P_inputSize,
                                uint8_t       *P_pOutputBuffer,
                                int32_t       *P_pOutputSize);

  int32_t CHACHA_Decrypt_Finish(CHACHActx_stt *P_pCHACHActx,
                                uint8_t       *P_pOutputBuffer,
                                int32_t       *P_pOutputSize);

  /**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __CHACHA_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
