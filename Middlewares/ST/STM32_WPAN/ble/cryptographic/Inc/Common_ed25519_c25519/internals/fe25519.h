/**
******************************************************************************
* @file    fe25519.h
* @author  MCD Application Team
* @version V1.0
* @date    09-December-2014
* @brief   fe25519 header file
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
#ifndef __FE25519_H__
#define __FE25519_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup FE25519 fe25519
* @brief Arithmetic in GF(2^255-19)
* @{
  */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef struct
{
  uint32_t v[8]; /*!< uint32_t array representing the number in low endian format */
} fe25519; /*!< Representation of a number in GF(2^255-19) */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void fe25519_freeze(fe25519 *r);

void fe25519_unpack(fe25519 *r, const uint8_t x[32]);

void fe25519_pack(uint8_t r[32], const fe25519 *x);

int32_t fe25519_iszero(const fe25519 *x);

int32_t fe25519_iseq_vartime(const fe25519 *x, const fe25519 *y);

void fe25519_cmov(fe25519 *r, const fe25519 *x, uint8_t b);

void fe25519_copy(fe25519 *r, const fe25519 *x);

void fe25519_setone(fe25519 *r);

void fe25519_setzero(fe25519 *r);

void fe25519_neg(fe25519 *r, const fe25519 *x);

uint8_t fe25519_getparity(const fe25519 *x);

void fe25519_add(fe25519 *r, const fe25519 *x, const fe25519 *y);

void fe25519_sub(fe25519 *r, const fe25519 *x, const fe25519 *y);

void fe25519_mul(fe25519 *r, const fe25519 *x, const fe25519 *y);

void fe25519_square(fe25519 *r, const fe25519 *x);

void fe25519_invert(fe25519 *r, const fe25519 *x);

void fe25519_pow2523(fe25519 *r, const fe25519 *x);

void fe25519_mult121665(fe25519 *r, const fe25519 *x);

/**
* @} fe25519
*/

#ifdef __cplusplus
}
#endif

#endif /* __FE25519_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
