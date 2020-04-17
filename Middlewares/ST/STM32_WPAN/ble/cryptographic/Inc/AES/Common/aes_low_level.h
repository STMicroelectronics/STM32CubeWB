/**
  ******************************************************************************
  * @file    aes_low_level.h
  * @author  MCD Application Team
  * @brief   AES low level functions
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AES_LOW_LEVEL_H__
#define __AES_LOW_LEVEL_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /** @addtogroup AESlowlevel
    * @{
    */

  /*------------------------------------------------------------------------------
  AES - Advanced Encryption Standard
  ------------------------------------------------------------------------------*/
  /* key size:    16 bytes (aes128), 24 bytes (aes192) or 32 bytes (aes256)
  *  block size:  16 bytes */

#define AES_BLOCK_SIZE 4 /*!< Number of (uint32_t) 32 bit words to store an AES block. */

  /*---------------------------AES MACROS---------------------------------------*/

  /** @defgroup AESmacros AES Macros
    * @{
    */

  /** Multiply for 2 each byte of a uint32_t working in parallel mode on each one. */
#define Xtime(x)  ((((x) & 0x7F7F7F7Fu) << 1) ^ ((((x) & 0x80808080u) >> 7) * 0x0000001Bu))

  /** Right shift x of n bytes. */
#define upr(x,n) ((x) >> (8*(n))) | ((x) << (32 - (8*(n))))

  /** Develop of the matrix necessary for the MixColomn procedure. */
#define fwd_mcol(x)  (Xtime(x)^(upr(((x)^Xtime(x)),3)) ^ (upr((x),2)) ^ (upr((x),1)))

  /** Develop of the matrix necessary for the InvMixColomn procedure. */
#define inv_mcol(x)  (f2=Xtime(x),f4=Xtime(f2),f8=Xtime(f4),(x)^=f8, f2^=f4^f8^(upr((f2^(x)),3))^(upr((f4^(x)),2))^(upr((x),1)))


  /** Defines right rotation of a uint32_t by 3.*/
#define rot3(x) (((x) << 8 ) | ((x) >> 24))
  /** Defines right rotation of a uint32_t by 2.*/
#define rot2(x) (((x) << 16) | ((x) >> 16))
  /** Defines right rotation of a uint32_t by 1.*/
#define rot1(x) (((x) << 24) | ((x) >> 8 ))

  /**
   * @}
   */


  /**
   * @}
   */



  /*----- AES 128 ---------------------------------------------------------------- */
  /** @ingroup AES128
    * @{
    */

#define AES_KEY_SIZE         4 /*!< Number of (uint32_t) 32 bit words to store an AES128 key. */
#define AES_EXPKEY_SIZE     44 /*!< Number of (uint32_t) 32 bit words to store an AES128 expanded key. */

  /* According to key computes the expanded key exp for AES128 encryption. */
  void AES128_keyschedule_enc(const uint32_t* key, uint32_t* expkey) ;
  /* According to key computes the expanded key exp for AES128 decryption. */
  void AES128_keyschedule_dec(const uint32_t* key, uint32_t* expkey) ;
  /* AES128 encryption of an AES128 block. */
  void AES128_encrypt(const uint32_t* input_pointer, uint32_t* output_pointer, const uint32_t* expkey) ;
  /* AES128 decryption of an AES128 block. */
  void AES128_decrypt(const uint32_t* input_pointer, uint32_t* output_pointer, const uint32_t* expkey) ;

  /**
   * @}
   */

  /*----- AES 192 ----------------------------------------------------------------*/
  /** @ingroup AES192
    * @{
    */

#define AES192_BLOCK_SIZE    4 /*!< Number of (uint32_t) 32 bit words to store an AES192 block. */
#define AES192_KEY_SIZE      6 /*!< Number of (uint32_t) 32 bit words to store an AES192 key. */
#define AES192_EXPKEY_SIZE  52 /*!< Number of (uint32_t) 32 bit words to store an AES192 expanded key. */

  /* According to key computes the expanded key exp for AES192 encryption.*/
  void AES192_keyschedule_enc(const uint32_t* key, uint32_t* exp) ;
  /* According to key computes the expanded key exp for AES192 decryption. */
  void AES192_keyschedule_dec(const uint32_t* key, uint32_t* exp) ;
  /* AES192 encryption of an AES192 block. */
  void AES192_encrypt(const uint32_t* input_pointer, uint32_t* output_pointer, const uint32_t* expkey) ;
  /* AES192 decryption of an AES192 block. */
  void AES192_decrypt(const uint32_t* input_pointer, uint32_t* output_pointer, const uint32_t* expkey) ;

  /**
   * @}
   */

  /*----- AES 256 ----------------------------------------------------------------*/
  /** @ingroup AES256
    * @{
    */

#define AES256_BLOCK_SIZE    4 /*!< Number of (uint32_t) 32 bit words to store an AES256 block. */
#define AES256_KEY_SIZE      8 /*!< Number of (uint32_t) 32 bit words to store an AES256 key. */
#define AES256_EXPKEY_SIZE  60 /*!< Number of (uint32_t) 32 bit words to store an AES256 expanded key. */

  /* According to key computes the expanded key exp for AES256 encryption. */
  void AES256_keyschedule_enc(const uint32_t* key, uint32_t* exp) ;
  /* According to key computes the expanded key exp for AES256 decryption. */
  void AES256_keyschedule_dec(const uint32_t* key, uint32_t* exp) ;
  /* AES256 encryption of an AES256 block. */
  void AES256_encrypt(const uint32_t* input_pointer, uint32_t* output_pointer, const uint32_t* expkey) ;
  /* AES256 decryption of an AES256 block. */
  void AES256_decrypt(const uint32_t* input_pointer, uint32_t* output_pointer, const uint32_t* expkey) ;

  /**
   * @}
   */

#ifdef __cplusplus
}
#endif


#endif  /*__AES_LOW_LEVEL_H__*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
