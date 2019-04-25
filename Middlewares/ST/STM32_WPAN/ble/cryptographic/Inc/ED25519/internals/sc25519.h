/**
******************************************************************************
* @file    sc25519.h
* @author  MCD Application Team
* @version V1.0
* @date    09-December-2014
* @brief   sc25519 header file
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
#ifndef __SC25519_H__
#define __SC25519_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup sc25519 
* @{
*/

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef struct
{
  uint32_t v[8]; /*!< uint32_t array representing the number in low endian format */
} sc25519; /*!< Representation of a number in GF(n = 2 ^ 252 + 27742317777372353535851937790883648493) */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void sc25519_from32bytes(sc25519 *r, const unsigned char x[32]);

void sc25519_from64bytes(sc25519 *r, const unsigned char x[64]);

void sc25519_to32bytes(unsigned char r[32], const sc25519 *x);

void sc25519_add(sc25519 *r, const sc25519 *x, const sc25519 *y);

void sc25519_mul(sc25519 *r, const sc25519 *x, const sc25519 *y);


#ifdef FAST_GE_COMPUTATION
void sc25519_window3(signed char r[85], const sc25519 *s);

void sc25519_2interleave2(unsigned char r[127], const sc25519 *s1, const sc25519 *s2);
#endif

/**
* @} sc25519
*/

#ifdef __cplusplus
}
#endif

#endif /* __SC25519_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
