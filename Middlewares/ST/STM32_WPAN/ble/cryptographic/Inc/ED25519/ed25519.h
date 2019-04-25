/**
******************************************************************************
* @file    ed25519.h
* @author  MCD Application Team
* @version V1.0
* @date    04-December-2014
* @brief   Container for ed25519 functionalities
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
