/**
  ******************************************************************************
  * @file    sha512.h
  * @author  MCD Application Team
  * @brief   Provides SHA512 functions
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

/*! \page Tutorial_SHA512 SHA512 Tutorial
  *
  * Here follows an example of SHA512 functionality.
  * Please remember that before starting to call the Init function the context \b requires user
  * initialization. The mFlags and mTagSize member must be initialized prior to calling the
  *  init function. Look at the each function documentation to see what is needed prior of calling.
  *
  * The API functions to be used are:
  *  - \ref SHA512_Init initialize an \ref SHA512ctx_stt context with required digest size and flags.
  *  - \ref SHA512_Append process input buffers.
  *    It can be called multiple times.
  *  - \ref SHA512_Finish can be called only one time for the digest generation process.
  *
  * The following code performs a SHA512 hash of a zero-size buffer in a single Append call
  * \code
  * #include <stdio.h>
  * #include "crypto.h"
  * int32_t main()
  * {
  *   uint8_t message[1024]={...};
  *   uint8_t digest[CRL_SHA512_SIZE]; // 64 byte buffer
  *   // outSize is for digest output size, retval is for return value
  *   int32_t outSize, retval;
  *   SHA512ctx_stt context_st; // The SHA512 context
  *
  *   // Initialize Context Flag with default value
  *   context_st.mFlags = E_HASH_DEFAULT;
  *   // Set the required digest size to be of 32 byte
  *   context_st.mTagSize = 32;
  *
  *   // call init function
  *   retval = SHA512_Init(&context_st);
  *   if (retval != HASH_SUCCESS)
  *   {  ... }
  *
  *   retval = SHA512_Append(&context_st, message, 0);
  *   if (retval != HASH_SUCCESS)
  *   {  ... }
  *
  *   //Generate the message digest
  *   retval = SHA512_Finish(&context_st, digest, &outSize );
  *   if (retval != HASH_SUCCESS)
  *   {  ... }
  * }
  * \endcode
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRL_SHA512_H__
#define __CRL_SHA512_H__

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

  /** @ingroup SHA512
    * @{
    */
#define CRL_SHA512_SIZE 64 /*!<  Number of bytes (uint8_t) to store a SHA-512 digest. */

  /** Structure for the SHA-512 context */
  typedef HASHLctx_stt SHA512ctx_stt;

  /* Initialization of Context */
  int32_t SHA512_Init(SHA512ctx_stt *P_pSHA512ctx);
  /* Data processing function */
  int32_t SHA512_Append(SHA512ctx_stt *P_pSHA512ctx, const uint8_t *P_pInputBuffer, int32_t P_inputSize);
  /* Returns digest */
  int32_t SHA512_Finish(SHA512ctx_stt *P_pSHA512ctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);
  /**
    * @}
    */

#ifdef __cplusplus
}
#endif

#endif   /*__CRL_SHA512_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
