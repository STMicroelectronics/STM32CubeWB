/**
  ******************************************************************************
  * @file    ge25519.h
  * @author  MCD Application Team
  * @brief   ge25519 header file
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
#ifndef __GE25519_H__
#define __GE25519_H__

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include "../../Common_ed25519_c25519/internals/fe25519.h"
#include "sc25519.h"

/** @addtogroup GE25519
* @{
*/

/* Exported types ------------------------------------------------------------*/

typedef struct
{
  fe25519 x; /*!< X Parameter of the representation */
  fe25519 y; /*!< Y Parameter of the representation */
  fe25519 z; /*!< Z Parameter of the representation */
  fe25519 t; /*!< T Parameter of the representation */
} ge25519; /*!< Point on Twisted Edwards Curve ed25519 represented in extended coordinates */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

int32_t ge25519_unpackneg_vartime(ge25519 *r, const uint8_t p[32]);

void ge25519_pack(uint8_t r[32], const ge25519 *p);

void ge25519_double_scalarmult_vartime(ge25519 *r, const ge25519 *p1, const sc25519 *s1, const ge25519 *p2, const sc25519 *s2);

void ge25519_scalarmult_base(ge25519 *r, const sc25519 *s);

/**
* @} ge25519
*/

#ifdef __cplusplus
}
#endif

#endif /* __GE25519_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
