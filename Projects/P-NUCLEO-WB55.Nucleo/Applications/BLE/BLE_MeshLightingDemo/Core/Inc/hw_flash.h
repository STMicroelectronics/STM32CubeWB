/**
******************************************************************************
* @file    hw_flash.h
* @brief   Header for hw_flash.c module
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the 
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_FLASH_H
#define __HW_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/**
 * @brief Write 64 bits double word in FLASH
 *
 * @param  FLASH destination address 
 * @param  64 bits double word data 
 * @retval result
 */
MOBLE_RESULT HW_FLASH_Write(uint32_t address, uint64_t data);

/**
 * @brief Erase FLASH page
 *
 * @param  address of FLASH page 
 * @param  number of page 
 * @param  interrupt not used 
 * @retval result
 */
MOBLE_RESULT HW_FLASH_Erase(uint32_t page, uint16_t n, int interrupt);

#if 0
/**
 * @brief Get Option Byte IPCC buffer address
 *
 * @param  None 
 * @retval Option Byte IPCC buffer address
 */
uint32_t HW_FLASH_OB_GetIPCCBufferAddr(void);

/**
 * @brief Get Option Byte SFSA
 *
 * @param  None 
 * @retval Option Byte SFSA
 */
uint32_t HW_FLASH_OB_GetSFSA(void);

/**
 * @brief Get Option Byte SBSRA
 *
 * @param  None 
 * @retval Option Byte SBSRA
 */
uint32_t HW_FLASH_OB_GetSBRSA(void);

/**
 * @brief Get Option Byte SNBRSA
 *
 * @param  None 
 * @retval Option Byte SNBRSA
 */
uint32_t HW_FLASH_OB_GetSNBRSA(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__HW_FLASH_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
