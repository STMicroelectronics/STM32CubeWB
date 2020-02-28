/**
******************************************************************************
* @file    pal_nvm.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Header file for pal_nvm.c 
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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
* Initial BLE-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAL_NVM_H
#define __PAL_NVM_H

#define ATOMIC_SECTION_BEGIN() uint32_t uwPRIMASK_Bit = __get_PRIMASK(); \
                                __disable_irq()
/* Must be called in the same or in a lower scope of ATOMIC_SECTION_BEGIN */ 
#define ATOMIC_SECTION_END() __set_PRIMASK(uwPRIMASK_Bit)

/* Includes ------------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported variables  -------------------------------------------------------*/
extern const void* mobleNvmBase;

/* Private define ------------------------------------------------------------*/
#define NVM_BASE                                     ((unsigned int)mobleNvmBase)
#define NVM_SIZE                                                           8192U

typedef enum
{
    MOBLE_NVM_COMPARE_EQUAL,
    MOBLE_NVM_COMPARE_NOT_EQUAL,
    MOBLE_NVM_COMPARE_NOT_EQUAL_ERASE
} MOBLE_NVM_COMPARE;

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT MoblePalNvmRead(MOBLEUINT32 address,
                             MOBLEUINT32 offset,
                             void *buf, 
                             MOBLEUINT32 size, 
                             MOBLEBOOL backup);
MOBLE_RESULT MoblePalNvmBackupRead(MOBLEUINT32 address, void *buf, MOBLEUINT32 size);
MOBLE_RESULT MoblePalNvmWrite(MOBLEUINT32 address,
                              MOBLEUINT32 offset,
                              void const *buf, 
                              MOBLEUINT32 size);
MOBLEBOOL    MoblePalNvmIsWriteProtected(void);
MOBLE_RESULT MoblePalNvmCompare(MOBLEUINT32 address,
                                MOBLEUINT32 offset,
                                void const *buf, 
                                MOBLEUINT32 size, 
                                MOBLE_NVM_COMPARE* result);
MOBLE_RESULT MoblePalNvmErase(MOBLEUINT32 address,
                              MOBLEUINT32 offset);

#endif /* __PAL_NVM_H */
