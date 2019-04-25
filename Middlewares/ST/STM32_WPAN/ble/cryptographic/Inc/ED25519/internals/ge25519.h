/**
******************************************************************************
* @file    ge25519.h
* @author  MCD Application Team
* @version V1.0
* @date    09-December-2014
* @brief   ge25519 header file
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
* http://www.st.com/software_license_agreement_liberty_v2
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
