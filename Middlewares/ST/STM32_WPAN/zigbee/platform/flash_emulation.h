/**
 ******************************************************************************
 * @file    flash_emulation.h
 * @author  MCD Application Team
 * @brief   This file contains the flash emulation.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_EMULATION_H
#define __FLASH_EMULATION_H

#ifdef STM32WB35xx
#error  TMP_STORAGE_BUF_ADDR will be determined during Zigbee porting on Little Dory
#define TMP_STORAGE_BUF_ADDR          xxx /* Should be __ICFEDIT_region_RAM_end__ inside the scatter file*/
#define TMP_STORAGE_BUF_SIZE         (xxx - TMP_STORAGE_BUF_ADDR)
#else
#define TMP_STORAGE_BUF_ADDR          0x2002F000 /* Should be __ICFEDIT_region_RAM_end__ inside the scatter file*/
#define TMP_STORAGE_BUF_SIZE         (0x2002FFFF - TMP_STORAGE_BUF_ADDR)
#endif

uint32_t utilsFlashGetSize(void);
uint32_t utilsFlashWrite(uint32_t aAddress, uint8_t *aData, uint32_t aSize);
uint32_t utilsFlashRead(uint32_t aAddress, uint8_t *aData, uint32_t aSize);

#endif //__FLASH_EMULATION_H
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
