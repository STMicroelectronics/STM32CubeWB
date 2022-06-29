/**
******************************************************************************
* @file    pal_nvm.h
* @author  BLE Mesh Team
* @brief   Header file for pal_nvm.c 
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
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
#if defined(STM32WB55xx)
#define NVM_SIZE                                                           8192U
#elif defined(STM32WB15xx)
#define NVM_SIZE                                                           4096U
#endif

typedef enum
{
    MOBLE_NVM_COMPARE_EQUAL,
    MOBLE_NVM_COMPARE_NOT_EQUAL,
    MOBLE_NVM_COMPARE_NOT_EQUAL_ERASE
} MOBLE_NVM_COMPARE;

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT PalNvmRead(MOBLEUINT32 address,
                        void *buf, 
                        MOBLEUINT32 size, 
                        MOBLEBOOL backup);
MOBLE_RESULT PalNvmBackupRead(MOBLEUINT32 address, 
                              void *buf, 
                              MOBLEUINT32 size);
MOBLE_RESULT PalNvmWrite(MOBLEUINT32 address,
                         void const *buf, 
                         MOBLEUINT32 size);
MOBLEBOOL    PalNvmIsWriteProtected(void);
MOBLE_RESULT PalNvmCompare(MOBLEUINT32 address,
                           void const *buf, 
                           MOBLEUINT32 size, 
                           MOBLE_NVM_COMPARE* result);
MOBLE_RESULT PalNvmErase(MOBLEUINT32 address,
                         MOBLEUINT8 nb_pages);
MOBLE_RESULT PalNvmProcess(void);

#endif /* __PAL_NVM_H */
