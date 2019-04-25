/**
  ******************************************************************************
  * @file    hash_common.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Provides common HASH/HMAC definitions
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
#ifndef __CRL_HASH_COMMON_H__
#define __CRL_HASH_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Exported types ------------------------------------------------------------*/
  /** @addtogroup HASH
    * @{
    */

  /**
    * @brief  Enumeration of possible hash functions
    */
  typedef enum HashType_e
  {
    E_MD5,          /*!< MD5     */
    E_SHA1,         /*!< SHA-1   */
    E_SHA224,       /*!< SHA-224 */
    E_SHA256,       /*!< SHA-256 */
    E_SHA384,       /*!< SHA-384 */
    E_SHA512,       /*!< SHA-512 */
  } hashType_et;


  /**
    * @brief  Type definitation for Hash/Hmac Flags
    */

  typedef enum HashFlags_e
  {
    E_HASH_DEFAULT = (uint32_t) (0x00000000),   /*!< User Flag: No flag specified.
                                                   This is the default value that should be set to this flag  */
    E_HASH_DONT_PERFORM_KEY_SCHEDULE = (uint32_t) (0x00000001), /*!< User Flag: Used to force the init to not reperform key processing in HMAC mode. */
    E_HASH_OPERATION_COMPLETED = (uint32_t) (0x00000002),  /*!< Internal Flag: used to check that the Finish function has been already called */
    E_HASH_NO_MORE_APPEND_ALLOWED = (uint32_t) (0x00000004),  /*!< Internal Flag: it is set when the last append has been called.
                                                                Used where the append is called with an InputSize not multiple of the block size,
                                                                which means that is the last input.*/
  } HashFlags_et;

  /**
    * @brief  Structure for HASH contextlags
    */

  typedef struct
  {
    uint32_t   mContextId;   /*!<  Unique ID of this context. \b Not \b used in current implementation. */
    HashFlags_et mFlags;     /*!<  32 bit mFlags, used to perform keyschedule */
    int32_t  mTagSize;       /*!<  Size of the required Digest */
    uint8_t amBuffer[64];    /*!<  Internal: It's a buffer with the data to be hashed */
    uint32_t amCount[2];     /*!<  Internal: Keeps the count of processed bits */
    uint32_t amState[8];     /*!<  Internal: Keeps the internal state */
  }
  HASHctx_stt;
  /**
    * @brief  Structure for Large (SHA-512 based) HASH contexts
    */

  typedef struct
  {
    uint32_t   mContextId;   /*!<  Unique ID of this context. \b Not \b used in current implementation. */
    HashFlags_et mFlags;     /*!<  32 bit mFlags, used to perform keyschedule */
    int32_t  mTagSize;       /*!<  Size of the required Digest */
    uint8_t amBuffer[128];    /*!<  Internal: It's a buffer with the data to be hashed */
    uint32_t amCount[2];     /*!<  Internal: Keeps the count of processed bits */
    uint64_t amState[8];     /*!<  Internal: Keeps the internal state */
  }
  HASHLctx_stt;


  /**
    * @brief  Structure for HMAC context
    */
  typedef struct
  {
    uint32_t   mContextId;   /*!< Unique ID of this context. \b Not \b used in current implementation. */
    HashFlags_et mFlags;     /*!< 32 bit mFlags, used to perform keyschedule */
    int32_t  mTagSize;       /*!< Size of the required Digest */
    const uint8_t  *pmKey;   /*!< Pointer for the HMAC key */
    int32_t  mKeySize;       /*!< Size, in uint8_t (bytes) of the HMAC key */
    uint8_t amKey[64];       /*!< Internal: The HMAC key */
    HASHctx_stt mHASHctx_st; /*!< Internal: Hash Context */
  }
  HMACctx_stt;
  /**
    * @brief  Structure for Large (SHA-512 based) HMAC context
    */
  typedef struct
  {
    uint32_t   mContextId;   /*!< Unique ID of this context. \b Not \b used in current implementation. */
    HashFlags_et mFlags;     /*!< 32 bit mFlags, used to perform keyschedule */
    int32_t  mTagSize;       /*!< Size of the required Digest */
    const uint8_t  *pmKey;   /*!< Pointer for the HMAC key */
    int32_t  mKeySize;       /*!< Size, in uint8_t (bytes) of the HMAC key */
    uint8_t amKey[128];       /*!< Internal: The HMAC key */
    HASHLctx_stt mHASHctx_st; /*!< Internal: Hash Context */
  }
  HMACLctx_stt;

  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif   /*__CRL_HASH_COMMON_H__*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
