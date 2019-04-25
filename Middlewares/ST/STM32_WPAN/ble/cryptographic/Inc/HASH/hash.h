/**
  ******************************************************************************
  * @file    hash.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   Container for the HASH functionalities
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Image SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.hom/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Common/hash_common.h"

/** @ingroup HASH
  * @{
  */

#ifdef INCLUDE_MD5
#include "MD5/md5.h"
#ifdef INCLUDE_HMAC
#include "MD5/hmac_md5.h"
#endif
#endif

#ifdef INCLUDE_SHA1
#include "SHA1/sha1.h"
#ifdef INCLUDE_HMAC
#include "SHA1/hmac_sha1.h"
#endif
#endif

#ifdef INCLUDE_SHA224
#include "SHA224/sha224.h"
#include "Common/sha256_sha224_transform.h"
#ifdef INCLUDE_HMAC
#include "SHA224/hmac_sha224.h"
#endif
#endif

#ifdef INCLUDE_SHA256
#include "SHA256/sha256.h"
#include "Common/sha256_sha224_transform.h"
#ifdef INCLUDE_HMAC
#include "SHA256/hmac_sha256.h"
#endif
#endif


#ifdef INCLUDE_SHA384
#include "SHA384/sha384.h"
#include "Common/sha512_sha384_transform.h"
#ifdef INCLUDE_HMAC
#include "SHA384/hmac_sha384.h"
#endif
#endif


#ifdef INCLUDE_SHA512
#include "SHA512/sha512.h"
#include "Common/sha512_sha384_transform.h"
#include "SHA512/hmac_sha512.h"
#include "SHA512/hkdf512.h"
#endif

/**
 * @} HASH
 */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
