/**
  ******************************************************************************
  * @file    hash.h
  * @author  MCD Application Team
  * @brief   Container for the HASH functionalities
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Image license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        http://www.st.com/SLA0044
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
