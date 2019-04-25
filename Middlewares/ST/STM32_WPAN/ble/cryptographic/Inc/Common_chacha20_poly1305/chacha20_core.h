/**
******************************************************************************
* @file    chacha20_core.h
* @author  MCD Application Team
* @version V3.0.0
* @date    05-June-2015
* @brief   Implements the core chacha functionality
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
/*
*  The implementation of chacha20 is just an adapatation (for consistency with
*  the library) of code coming from: chacha-ref.c version 20080118 D. J. Bernstein
*  Which was released in Public domain.
*/
#ifndef __CHACHA20_CORE_H__
#define __CHACHA20_CORE_H__

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/** @addtogroup SymKeylowlevel
* @{
*/

/** @defgroup CHACHAlowlevel ChaCha
* @brief Internal ChaCha Functions and Macros
* @{
*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void salsa20_wordtobyte(uint8_t output[64],\
                        const uint32_t input[16]);

/**
* @} SymKeylowlevel
*/

/**
* @} CHACHAlowlevel
*/
#endif /* __CHACHA20_CORE_H__ */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
