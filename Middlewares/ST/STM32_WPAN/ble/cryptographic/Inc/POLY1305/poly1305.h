/**
******************************************************************************
* @file    poly1305.h
* @author  MCD Application Team
* @version V3.0.0
* @date    05-June-2015
* @brief   Container for poly1305-AES functionalities
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
#ifndef __POLY1305_H__
#define __POLY1305_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @addtogroup POLY1305
  * @{
  */
  /* Includes ------------------------------------------------------------------*/
#include <stdint.h>
  /* Exported types ------------------------------------------------------------*/

  /**
  * @brief  Type definitation for Poly1305-AES Flags
  */
  typedef enum
  {
    E_POLY1305_DEFAULT = (uint32_t)(0x00000000),   /*!< User Flag: No flag specified.
                                                       This is the default value that should be set to this flag  */
    E_POLY1305_OPERATION_COMPLETED = (uint32_t)(0x00000002),  /*!< Internal Flag: used to check that the Finish function has been already called */
    E_POLY1305_NO_MORE_APPEND_ALLOWED = (uint32_t)(0x00000004),  /*!< Internal Flag: it is set when the last append has been called.
                                                              Used where the append is called with an InputSize not multiple of the block size,
                                                              which means that is the last input.*/
  } PolyFlags_et;

  /**
  * @brief  Structure for Poly1305-AES context
  */
  typedef struct
  {
    uint32_t   mContextId;   /*!< Unique ID of this context. \b Not \b used in current implementation. */
    PolyFlags_et mFlags;     /*!< 32 bit mFlags, used to perform keyschedule */
    const uint8_t *pmKey;    /*!< Pointer to original 32 bytes Key buffer */
    const uint8_t *pmNonce;  /*!< Pointer to original 16 bytes Nonce buffer */
    const uint8_t *pmTag;    /*!< Pointer to Authentication TAG. This value must be set in decryption, and this TAG will be verified */
    int32_t  mTagSize;       /*!< Size of the required Authentication TAG */
    uint32_t r[5];           /*!< Internal: value of r */
    uint32_t h[5];           /*!< Internal: value of h */
    uint32_t pad[4];         /*!< Internal: value of encrypted nonce */
  }
  Poly1305ctx_stt;


  /* Exported constants --------------------------------------------------------*/
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

  int32_t Poly1305_Auth_Init(Poly1305ctx_stt *P_pPoly1305ctx, const uint8_t *P_pKey, const uint8_t *P_pNonce);

  int32_t Poly1305_Auth_Append(Poly1305ctx_stt *P_pPoly1305ctx, const uint8_t *P_pInputBuffer, int32_t P_inputSize);

  int32_t Poly1305_Auth_Finish(Poly1305ctx_stt *P_pPoly1305ctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);

  int32_t Poly1305_Verify_Init(Poly1305ctx_stt *P_pPoly1305ctx, const uint8_t *P_pKey, const uint8_t *P_pNonce);

  int32_t Poly1305_Verify_Append(Poly1305ctx_stt *P_pPoly1305ctx, const uint8_t *P_pInputBuffer, int32_t P_inputSize);

  int32_t Poly1305_Verify_Finish(Poly1305ctx_stt *P_pPoly1305ctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);
  /**
  * @}
  */
  /* Lower level functionality. Useful for testing, might be used in real world if there is small NVM */


#ifdef __cplusplus
}
#endif

#endif /* __POLY1305_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
