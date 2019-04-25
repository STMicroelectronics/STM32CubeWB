/**
  ******************************************************************************
  * @file    crypto.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    05-June-2015
  * @brief   This file contains all the functions prototypes for the config firmware
  *          library.
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

#include "config.h"

/* List of automatically enabled defines and checks */
#if  defined ( __GNUC__ )
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
#endif

/* INCLUDE_DRBG_AES128 requires AES128 with Encryption capabilities */
#ifdef INCLUDE_DRBG_AES128
#define INCLUDE_AES128  ((uint16_t)0x0004)
#define INCLUDE_ENCRYPTION  ((uint8_t)0x01)
#endif

#if defined(INCLUDE_AES128) || defined(INCLUDE_AES192) ||  defined(INCLUDE_AES256)
#define INCLUDE_AES /*!< Automatically defined if AES has been selected */
#endif

#if defined(INCLUDE_DES) || defined(INCLUDE_TDES)
#define INCLUDE_DES_CORE /*!< TripleDES and DES has the same core function, this flag enables it */
#endif

#if defined(INCLUDE_RSA) || defined (INCLUDE_ECC)
#define INCLUDE_MATH /*!< The Multiprecision Math engine is included */
#endif

/** Hash functions are included  */
#ifdef INCLUDE_HKDF
#define INCLUDE_HMAC  ((uint16_t)0x0040)           
#define INCLUDE_SHA512 ((uint16_t)0x0020)
#endif

#if defined(INCLUDE_SHA1) || defined(INCLUDE_SHA224) || defined(INCLUDE_SHA256) || defined(INCLUDE_MD5)|| defined(INCLUDE_SHA512)    
#define INCLUDE_HASH /*!< Hash functions are included  */
#endif




/*************************************************************************************************
***-*-*-*-*--**- SOME CHECKS, DON'T MODIFY -**--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**
*************************************************************************************************/
#if (defined(INCLUDE_ENCRYPTION) || defined(INCLUDE_DECRYPTION)) && !(defined(INCLUDE_AES) || defined(INCLUDE_DES_CORE) || defined(INCLUDE_ARC4))
#error "Either encryption or decryption has been specified but no AES or DES are present"
#elif (!defined(INCLUDE_ENCRYPTION) && !defined(INCLUDE_DECRYPTION)) && (defined(INCLUDE_AES) || defined(INCLUDE_DES_CORE))
#error "AES or DES are present but without encryption or decryption functionalities!"
#endif

#ifdef CRL_CPU_SUPPORT_MISALIGNED
  #ifndef CRL_ENDIANNESS
    #error "With CRL_CPU_SUPPORT_MISALIGNED=1 a definition for CRL_ENDIANNESS is required!"
  #endif
#endif

#if defined(INCLUDE_GCM) && !defined(INCLUDE_AES)
#error "GCM is currently supported only by AES, but AES has not be included, please include AES or remove GCM"
#endif

#if defined(INCLUDE_CMAC) && !defined(INCLUDE_AES)
#error "CMAC is currently supported only by AES, but AES has not be included, please include AES or remove GCM"
#endif

#if defined(INCLUDE_KEY_WRAP) && !defined(INCLUDE_AES)
#error "KWRAP is currently supported only by AES, but AES has not be included, please include AES or remove KWRAP"
#endif

#if defined(INCLUDE_CTR) && !defined(INCLUDE_AES)
#error "CTR is currently supported only by AES, but AES has not be included, please include AES or remove CTR"
#endif

#if defined(INCLUDE_CCM) && !defined(INCLUDE_AES)
#error "CCM is currently supported only by AES, but AES has not be included, please include AES or remove CCM"
#endif

#if defined(INCLUDE_DRBG_AES128) && !defined(INCLUDE_AES128)
#error "DRBG_AES128 requires INCLUDE_AES_128"
#endif

#if defined(INCLUDE_POLY1305) && !defined(INCLUDE_AES128)   
#error "POLY1305-AES requires INCLUDE_AES_128"
#endif


#if defined(INCLUDE_ED25519) && !defined(INCLUDE_SHA512)   
#error "ED25519 requires INCLUDE_SHA512"
#endif

#if defined(INCLUDE_ECC_SIGN) & !(defined(INCLUDE_DRBG_AES128))
#error "To include ECC_SIGN functionalities you need a random generator. \n"
#endif

#if defined(INCLUDE_RSA) && !defined(RSA_WINDOW_SIZE)
#define RSA_WINDOW_SIZE 1
#endif

/*************************************************************************************************
***-*-*-*-*--**- STD PERIPHERAL DRIVERS INCLUSION -**--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-***
*************************************************************************************************/
#define CL_ON_STM32

/*************************************************************************************************
***-*-*-*-*--**- FAMILIES WITH HW CRYPTO -**--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-***
*************************************************************************************************/

#if defined(INCLUDE_DRBG_AES128)
#define INCLUDE_RNG  /*!< A Random Engine is present in the library  */
#endif

/*************************************************************************************************
***-*-*-*-*--**- CRYPTO LIBRARY INCLUSIONS -**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-***
*************************************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include "Common/types.h"
#include "Common/macros.h"
#include "Common/err_codes.h"
#include "Common/sk.h"

#ifdef INCLUDE_AES
#include "AES/aes.h"
#endif

#ifdef INCLUDE_DES
#include "DES/des.h"
#endif

#ifdef INCLUDE_TDES
#include "TDES/tdes.h"
#endif

#ifdef INCLUDE_ARC4
#include "ARC4/arc4.h"
#endif

#ifdef INCLUDE_HASH
#include "HASH/hash.h"
#endif

#ifdef INCLUDE_MATH
#include "Common_ecc_rsa/MATH/math.h"
#endif

#ifdef INCLUDE_RSA
#include "RSA/rsa.h"
#endif

#ifdef INCLUDE_RNG
#include "RNG/rng.h"
#endif

#ifdef INCLUDE_ECC
#include "ECC/ecc.h"
#endif
#ifdef INCLUDE_POLY1305            
#include "POLY1305/poly1305.h"     
#endif

#ifdef INCLUDE_ED25519             
#include "ED25519/ed25519.h"
#endif

#ifdef INCLUDE_CURVE25519         
#include "C25519/c25519.h"
#endif
                                  
#ifdef INCLUDE_CHACHA
#include "CHACHA/chacha.h"
#endif

#ifdef INCLUDE_CHACHA20POLY1305               
#include "CHACHA20-POLY1305/chacha20-poly1305.h"
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
