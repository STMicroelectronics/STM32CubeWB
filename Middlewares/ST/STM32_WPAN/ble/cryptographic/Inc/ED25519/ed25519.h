/**
  ******************************************************************************
  * @file    ed25519.h
  * @author  MCD Application Team
  * @brief   Container for ed25519 functionalities
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
#ifndef __ED25519_H__
#define __ED25519_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup ED25519
* @{
*/
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t ED25519keyGen(uint8_t *P_pPrivateKey, uint8_t *P_pPublicKey);
int32_t ED25519sign(uint8_t *P_pSignature, const uint8_t *P_pInput, int32_t P_InputSize, const uint8_t *P_pPrivateKey);
int32_t ED25519verify(const uint8_t *P_pInput, int32_t P_InputSize, const uint8_t *P_pSignature, const uint8_t *P_pPublicKey);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __ED25519_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
