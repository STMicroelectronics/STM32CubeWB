/**
  ******************************************************************************
  * @file    config.h
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_CONFIG_H__
#define __CRL_CONFIG_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  Structure that contains STM32 Cryptographic Library settings  
  */  
typedef struct
{
  uint8_t X;                   /*!< Used to get the X parametre of the current STM32 crypto library verion */
  uint8_t Y;                   /*!< Used to get the Y parametre of the current STM32 crypto library verion */
  uint8_t Z;                   /*!< Used to get the Z parametre of the current STM32 crypto library verion */
  uint8_t Type;                /*!< Used to get the Type of the verion. 
                                    This parameter can be a value of @ref TypeConf */
  uint8_t CortexConf ;         /*!< Used to get the Cortex.
                                    This parameter can be a value of @ref CortexConf */
  uint8_t IdeConf ;            /*!< Used to get the IDE used to compile the library.
                                    This parameter can be a value of @ref IdeConf */  
  uint8_t IdeOptimization ;    /*!< Used to get the compiler optimization.
                                    This parameter can be any combination of values of @ref IdeOptimization */       
  uint8_t EndiannessConf ;     /*!< Used to get the option value used to specify the memory representation of the platform.                                  
                                    This parameter can be a value of @ref EndiannessConf */
  uint8_t MisalignedConf ;     /*!< Used to get if the CRL_CPU_SUPPORT_MISALIGNED is defined or not. 
                                    This parameter can be any combination of values of @ref MisalignedConf  */                                                                              
  uint8_t EncDecConf;          /*!< Used to get which functionalities of encryption and decryptio functionalities are included.
                                    This parameter can be any combination of values of @ref EncDecConf  */                  
  uint16_t SymKeyConf  ;       /*!< Used to get the Symmetric algorithms included in the library
                                    This parameter can be any combination of values of @ref SymKeyConf  */     
  uint16_t SymKeyModesConf ;   /*!< Used to get the Modes of Operations for Symmetric Key Algorithms included in the library                                                                                             
                                    This parameter can be any combination of values of @ref SymKeyModesConf */
  uint16_t AsymKeyConf ;       /*!< Used to get the Asymmetric algorithms included in the library                                                          
                                    This parameter can be any combination of values of @ref AsymKeyConf  */
  uint16_t HashConf ;          /*!< Used to get the Hash and Hmac algorithms included in the library                                                                     
                                    This parameter can be any combination of values of @ref HashConf  */
  uint16_t MACConf;             /*!< Used to get the MAC algorithms included in the library  
                                    This parameter can be any combination of values of @ref MACConf  */
  uint8_t DrbgConf ;           /*!< Used to get if the deterministic random bit generator is inculded. 
                                    This parameter can be any combination of values of @ref DRBGConf  */              
  uint8_t AesConf ;            /*!< Used to get the AES algorithm version used.           
                                    This parameter can be a value of @ref AESConf */                                                    
  uint8_t RsaConf ;            /*!< Used to get the Window size selected.                                   
                                    This parameter can be a value of @ref RSAConf*/                                                     
  uint8_t GcmConf ;            /*!< Used to get the algorithm to be used for polynomial multiplication in AES-GCM.                                    
                                    This parameter can be a value of @ref GCMConf */                                                               
}STM32CryptoLibVer_TypeDef;  

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t STM32_GetCryptoLibrarySettings(STM32CryptoLibVer_TypeDef * LibVersion);

/*************************************************************************************************
***-*-*-*-*--**- CONFIGURATION OPTIONS -**--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-***
*************************************************************************************************/
  
/** @defgroup CRYPTO_LIBRARY_VERSION
  *  This groupe is just used by the function STM32_GetCryptoLibrarySettings to get the current library settings
  *  This groupe has no impact on the algorithms implementation 
  * @{
  */   
#define VERSION_X 3 /*!< The X parametre of the current STM32 crypto library verion */
#define VERSION_Y 0 /*!< The Y parametre of the current STM32 crypto library verion */
#define VERSION_Z 0 /*!< The Z parametre of the current STM32 crypto library verion */
/**
  * @}   End of CRYPTO_LIBRARY_VERSION
  */ 
  
/** 
  * @defgroup TypeConf 
  *  This groupe is just used by the function STM32_GetCryptoLibrarySettings to get the current library settings
  *  This groupe has no impact on the algorithms implementation 
  *  Only one option should be enabled      
  * @{
  */
#define SW      ((uint8_t)0x01)         /*!< This impelmentation is Full SW */
  /* #define HW                 ((uint8_t)0x02)   */    /*!< This impelmentation support a HW accelerations*/
                                         
/**                          
  * @}   End of TypeConf                    
  */ 
  
/** 
  * @defgroup CortexConf Compiler setting
  *  This groupe is just used by the function STM32_GetCryptoLibrarySettings to get the current library settings
  *  This groupe has no impact on the algorithms implementation  
  *  Only one option should be enabled   
  * @{
  */  
  /* #define CORTEX_M0             ((uint16_t)0x0001) */   /*!< Library is compiled for Cortex M0  */
  /* #define CORTEX_M0+            ((uint16_t)0x0002) */   /*!< Library is compiled for Cortex M0+ */
  /* #define CORTEX_M3             ((uint16_t)0x0003) */   /*!< Library is compiled for Cortex M3  */
#define CORTEX_M4             ((uint16_t)0x0004)      /*!< Library is compiled for Cortex M4  */
  /* #define CORTEX_M7             ((uint16_t)0x0005) */   /*!< Library is compiled for Cortex M7  */
/**
  * @}   End of CortexConf
  */ 

/** 
  * @defgroup IdeConf Compiler setting
  *  This groupe is just used by the function STM32_GetCryptoLibrarySettings to get the current library settings
  *  This groupe has no impact on the algorithms implementation    
  *  Only one option should be enabled   
  * @{
  */
#define IAR      ((uint8_t)0x01)         /*!< Library is compiled with Iar  */
  /* #define KEIL                  ((uint8_t)0x02)   */    /*!< Library is compiled with Keil */
  /* #define GCC                   ((uint8_t)0x03)   */    /*!< Library is compiled with GCC  */
                                         
/**                          
  * @}   End of IdeConf                    
  */ 


/** 
  * @defgroup IdeOptimization Compiler setting
  *  This groupe is just used by the function STM32_GetCryptoLibrarySettings to get the current library settings
  *  This groupe has no impact on the algorithms implementation    
  * @{
  */
#ifdef IAR  
  /* #define IAR_Optimization      ((uint16_t)0x0001) */   /*!< The compiler optimization is None optimization                                */
  /* #define IAR_Optimization      ((uint16_t)0x0002) */   /*!< The compiler optimization is Low optimization                                 */
  /* #define IAR_Optimization      ((uint16_t)0x0003) */   /*!< The compiler optimization is Meduim optimization                              */
  /* #define IAR_Optimization      ((uint16_t)0x0004) */   /*!< The compiler optimization is high balanced optimization                       */
  /* #define IAR_Optimization      ((uint16_t)0x0005) */   /*!< The compiler optimization is high Size optimization                           */
#define IAR_Optimization      ((uint16_t)0x0006)    /*!< The compiler optimization is high speed optimization                          */
  /* #define IAR_Optimization      ((uint16_t)0x0007) */   /*!< The compiler optimization is high speed optimization with No Size constraints */
#endif                        
                  
#ifdef KEIL    
  /* #define KEIL_Optimization     ((uint16_t)0x0001) */   /*!< The compiler optimization is O0 */
  /* #define KEIL_Optimization     ((uint16_t)0x0002) */   /*!< The compiler optimization is O1 */
  /* #define KEIL_Optimization     ((uint16_t)0x0003) */   /*!< The compiler optimization is O2 */
  /* #define KEIL_Optimization     ((uint16_t)0x0004) */   /*!< The compiler optimization is O3 */
                                                        
  /* #define OFT                   ((uint16_t)0x0010) */   /*!< Library is compiled with option Optimize For Time             */
  /* #define SLSM                  ((uint16_t)0x0020) */   /*!< Library is compiled with option Split Load And Store Multiple */
  /* #define O1ELFSPF              ((uint16_t)0x0040) */   /*!< Library is compiled with option One ELF Section per Function  */  
#endif

#ifdef GCC 
  /* #define GCC_Optimization      ((uint16_t)0x0001) */   /*!< The compiler optimization is O0    */
  /* #define GCC_Optimization      ((uint16_t)0x0002) */   /*!< The compiler optimization is OG    */
  /* #define GCC_Optimization      ((uint16_t)0x0003) */   /*!< The compiler optimization is O1    */
  /* #define GCC_Optimization      ((uint16_t)0x0004) */   /*!< The compiler optimization is O2    */
  /* #define GCC_Optimization      ((uint16_t)0x0005) */   /*!< The compiler optimization is O3    */
  /* #define GCC_Optimization      ((uint16_t)0x0006) */   /*!< The compiler optimization is OS    */
  /* #define GCC_Optimization      ((uint16_t)0x0007) */   /*!< The compiler optimization is Ofast */
 
  /* #define PDCR                  ((uint16_t)0x0010) */   /*!< Library is compiled with option Prepare Dead Code Removal */
  /* #define PDDR                  ((uint16_t)0x0020) */   /*!< Library is compiled with option Prepare Dead Data Removal */
  /* #define NSA                   ((uint16_t)0x0040) */   /*!< Library is compiled with option No Strict Aliasing        */
#endif

/**
  * @}   End of IdeOptimization
  */ 
  



/** @defgroup EndiannessConf Endianness
  * Used to specify the memory representation of the platform.
  *   - CRL_ENDIANNESS=1 for LITTLE ENDIAN
  *   - CRL_ENDIANNESS=2 for BIG ENDIAN \n
  *   This setting is \b VERY \b IMPORTANT 
  *   Only one option should be enabled    
  * @{
  */
  
#define CRL_ENDIANNESS  1      /*!< LITTLE ENDIAN is selected */
    /* #define CRL_ENDIANNESS  2 */   /*!< BIG ENDIAN is selected */
    
/**
  * @}  End of EndiannessConf
  */ 

/**  
  * @defgroup  MisalignedConf Misaligned words read/write operations.
  * Used to specify if the processor supports misaligned integers read/write operations. \n
  * To be portable this flag is not defined but if the processor supports read/write operations of 4
  * bytes to address not multiple by 4 then setting this flag to 1 will improve the performance
  * of AES when used through high level functions (ex AES_CBC or crl_AES).
  * @{
  */
#define CRL_CPU_SUPPORT_MISALIGNED   ((uint8_t)0x01)   /*!< Improve the performance of AES when used through high level functions 
                                                            Enabled for Cortex M3, M4, M7 and Disabled for Cortex M0, M0+   */
                                                              

/**
  * @}  End of MisalignedConf
  */ 

/** 
  * @defgroup EncDecConf Encryption and/or Decryption functionalities
  * @{
  */
#define INCLUDE_ENCRYPTION  ((uint8_t)0x01) /*!< Includes the Encryption functionalities in the library.*/
#define INCLUDE_DECRYPTION  ((uint8_t)0x02) /*!< Includes the Decryption functionalities in the library.*/

/**
  * @}   End of EncDecConf   
  */ 

/** 
  * @defgroup SymKeyConf Symmetric Key Algorithms
  * @{
  */

#define INCLUDE_DES              ((uint16_t)0x0001) /*!< DES functions are included in the library.                          */
#define INCLUDE_TDES             ((uint16_t)0x0002) /*!< TripleDES (TDES) functions are included in the library.             */
#define INCLUDE_AES128           ((uint16_t)0x0004) /*!< AES functions with key size of 128 bit are included in the library. */ 
#define INCLUDE_AES192           ((uint16_t)0x0008) /*!< AES functions with key size of 192 bit are included in the library. */
#define INCLUDE_AES256           ((uint16_t)0x0010) /*!< AES functions with key size of 256 bit are included in the library. */
#define INCLUDE_ARC4             ((uint16_t)0x0020) /*!< ARC4 functions are included in the library.                         */
#define INCLUDE_CHACHA           ((uint16_t)0x0040) /*!< ChaCha functions are included in the library.                       */ 
#define INCLUDE_CHACHA20POLY1305 ((uint16_t)0x0080) /*!< oly1305-AES functions are included in the library                   */

/**
  * @}   End of SymKeyConf  
  */ 

/**
  * @defgroup SymKeyModesConf Modes of Operations for Symmetric Key Algorithms
  * @{
  */

#define INCLUDE_ECB      ((uint16_t)0x0001) /*!< AES high level functions for ECB mode are included in the library */
#define INCLUDE_CBC      ((uint16_t)0x0002) /*!< AES high level functions for CBC mode are included in the library */
#define INCLUDE_CTR      ((uint16_t)0x0004) /*!< AES high level functions for CTR mode are included in the library */
#define INCLUDE_GCM      ((uint16_t)0x0008) /*!< AES high level functions for GCM mode are included in the library */
#define INCLUDE_KEY_WRAP ((uint16_t)0x0010) /*!< AES-KWRAP is included in the library                              */
#define INCLUDE_CCM      ((uint16_t)0x0020) /*!< AES-CCM is included in the library                                */
#define INCLUDE_CMAC     ((uint16_t)0x0040) /*!< AES-CMAC is included in the library                               */
#define INCLUDE_XTS      ((uint16_t)0x0080) /*!< AES-XTS  is included in the library                               */
#define INCLUDE_OFB      ((uint16_t)0x0100) /*!< AES-OFB  is included in the library                               */
#define INCLUDE_CFB      ((uint16_t)0x0200) /*!< AES-CFB  is included in the library                               */

/**
  * @}    End of SymKeyModesConf 
  */ 

/** 
  * @defgroup AsymKeyConf Asymmetric Key Algorithms
  * @{
  */

#define INCLUDE_RSA        (uint16_t)0x0001) /*!< RSA functions for signature verification are included in the library */
#define INCLUDE_ECC        (uint16_t)0x0002) /*!< ECC functions are included in the library                            */
#define INCLUDE_ED25519    (uint16_t)0x0004) /*!< ED25519 functions are included in the library                        */
#define INCLUDE_CURVE25519 (uint16_t)0x0008) /*!< Curve25519 functions are included in the library                     */     
/**                         
  * @}    End of AsymKeyConf                   
  */                        

/** 
  * @defgroup HashConf Hash and HMAC Algorithms
  * @{
  */

#define INCLUDE_MD5      ((uint16_t)0x0001) /*!< MD5 functions are included in the library                                           */
#define INCLUDE_SHA1     ((uint16_t)0x0002) /*!< SHA-1 functions are included in the library                                         */
#define INCLUDE_SHA224   ((uint16_t)0x0004) /*!< SHA-224 functions are included in the library                                       */
#define INCLUDE_SHA256   ((uint16_t)0x0008) /*!< SHA-256 functions are included in the library                                       */
#define INCLUDE_SHA384   ((uint16_t)0x0010) /*!< SHA-384 functions are included in the library                                       */
#define INCLUDE_SHA512   ((uint16_t)0x0020) /*!< SHA-512 functions are included in the library                                       */
#define INCLUDE_HMAC     ((uint16_t)0x0040) /*!< Select if, for the selected hash algorithms, HMAC should be included in the library */
#define INCLUDE_HKDF     ((uint16_t)0x0080) /*!< Select if including the HKDF key derivation function, it requires HMAC              */


 /**
  * @}
  */ 

/**
* @defgroup MACConf Poly1305-AES Algorithm
* @{
*/

#define INCLUDE_POLY1305  ((uint16_t)0x0001) /*!< This defines if Poly1305-AES functions are included in the library */

/**
* @}
*/
  


/**
  * @defgroup DRBGConf Deterministic Random Bit Generator Based on AES-128
  * @{
  */
#define INCLUDE_DRBG_AES128       ((uint16_t)0x0001)  /*!< Includes the deterministic random bit generator library                                     */
#define CRL_RANDOM_REQUIRE_RESEED ((uint16_t)0x0002)  /*!< Implements the request for reseed when using the DRBG too many times for security standards */
/**
  * @}   End of DRBGConf 
  */ 






/** 
  * @defgroup AESConf AES version
  * Used to select the AES algorithm version to use
  * - CRL_AES_ALGORITHM = 1 : version with 522 bytes of look-up tables, slower than version 2.
  * - CRL_AES_ALGORITHM = 2 : version with 2048 bytes of look-up tables, faster than version 1.
  * Only one option should be enabled    
  * @{
  
  */
  /* #define CRL_AES_ALGORITHM 1 */  /*!< version with 522 bytes of look-up tables is selected */ 
   #define CRL_AES_ALGORITHM 2     /*!< version with 2048 bytes of look-up tables is selected */   
/**
  * @}   End of AESConf 
  */ 

/**
  * @defgroup RSAConf RSA Window Size
  * This parameter is used to speed up RSA operation with private key at expense of RAM memory.
  * It can't be set less than one.
  * Suggested values are 3 or 4. Entering a value of 7 or more will be probably worst than using 6.
  * Only one option should be enabled    
  * @{
  */    
  /* #define RSA_WINDOW_SIZE 1 */   /*!< Window size selected is 1 */
  /* #define RSA_WINDOW_SIZE 2 */   /*!< Window size selected is 2 */
  /* #define RSA_WINDOW_SIZE 3 */   /*!< Window size selected is 3 */
#define RSA_WINDOW_SIZE 4   /*!< Window size selected is 4 */
/**
  * @}  End of RSAConf 
  */ 

/** 
  * @defgroup GCMConf AES GCM GF(2^128) Precomputations Tables
  * Used to specify the algorithm to be used for polynomial multiplication in AES-GCM.\n
  *  The algorithm's choice also defines the size of the precomputed table made to speed
  *  up the multiplication. \n There are two types of table, one is based on the value of
  *  the key and so needs to be generated at runting (through AES_GCM_keyschedule), the other
  *  is constant and is defined (if included here) in privkey.h.
  *  There are 3 possible choices:
  *  - 0 = Without any tables. No space required. Slower version.
  *  - 1 = Key-dependent table for *Poly(y) 0000<y<1111 and constant table for *x^4 \n
  *       (256 key-dependent bytes - 32 constant bytes)
  *  - 2 = 4 key-dependent tables for *Poly(y^(2^(32*i))) and 4 key-dependent tables for *Poly((y*x^4)^(2^(32*i)))
  *        with 0000<y<1111 and 0<i<4 and constant tables for *x^8 and for *x^4\n
  *       (2048 key-dependent bytes - 544 constant bytes) 
  *  Only one option should be enabled  
  * @{
  */   
  /* #define CRL_GFMUL 0 */   /*!< Any tables is selected */
  /* #define CRL_GFMUL 1 */   /*!< Key-dependent table  is selected   */
     #define CRL_GFMUL 2    /*!< 4 key-dependent tables is selected */
/**
  * @}  End of GCMConf
  */                                             


#endif  /*__CRL_CONFIG_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
