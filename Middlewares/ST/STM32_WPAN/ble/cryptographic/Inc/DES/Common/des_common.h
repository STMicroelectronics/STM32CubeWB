/**
  ******************************************************************************
  * @file    des_common.h
  * @author  MCD Application Team
  * @brief   DES common functions and definitions
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
#ifndef __DES_COMMON_H__
#define __DES_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif


  /** @addtogroup DES
    * @{
    */
  /* Exported constants --------------------------------------------------------*/
#define CRL_DES_BLOCK   8   /*!< Number of bytes (uint8_t) necessary to store a DES block. */
#define CRL_DES_KEY     8   /*!< Number of bytes (uint8_t) necessary to store a DES key. */
  /* Exported types ------------------------------------------------------------*/
  typedef struct
  {
    uint32_t   mContextId;   /*!< Unique ID of this context. \b Not \b used in current implementation. */
    SKflags_et mFlags;       /*!< 32 bit mFlags, used to perform keyschedule */
    const uint8_t *pmKey;    /*!< Pointer to original Key buffer */
    const uint8_t *pmIv;     /*!< Pointer to original Initialization Vector buffer */
    int32_t   mIvSize;       /*!< Size of the Initialization Vector in bytes */
    uint32_t   amIv[2];      /*!< Temporary result/IV */
    uint32_t   amExpKey[32]; /*!< Expanded DES key */
  }
  DESCBCctx_stt; /*!< Structure for the context of a DES operation */
  /* Exported macro ------------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  /**
   * @}
   */

#ifdef __cplusplus
}
#endif

#endif  /*__DES_COMMON_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
