/**
******************************************************************************
* @file    poly1305_core.h
* @author  MCD Application Team
* @version V3.0.0
* @date    05-June-2015
* @brief   Implements the core poly1305 functionality
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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
******************************************************************************/

#ifndef __POLY1305_CORE_H__
#define __POLY1305_CORE_H__

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @addtogroup SymKeylowlevel
* @{
*/

/** @defgroup POLY1305lowlevel Poly1305-AES
* @brief Internal  Poly1305-AES Function
* @{
*/

void Poly1305_Core_Update (uint32_t *P_h,          \
                           const uint32_t *P_r,    \
                           const uint8_t *P_pInput,\
                           uint32_t P_LastBlock);

void Poly1305_Core_Finish(uint32_t *P_h,        \
                          const uint32_t *P_pad,\
                          uint32_t *P_pTag);                           

/**
* @}
*/

/**
* @}
*/

#endif /* __POLY1305_CORE_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
