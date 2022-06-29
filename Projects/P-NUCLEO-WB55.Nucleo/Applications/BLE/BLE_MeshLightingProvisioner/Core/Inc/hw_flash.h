/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file    hw_flash.h
  * @author  MCD Application Team
* @brief   Header for hw_flash.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HW_FLASH_H
#define HW_FLASH_H

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

#endif /* HW_FLASH_H */
