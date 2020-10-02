/**
******************************************************************************
* @file    pal_nvm.h
* @author  BLE Mesh Team
* @brief   Header file for pal_nvm.c 
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under Ultimate Liberty license
* SLA0044, the "License"; You may not use this file except in compliance with
* the License. You may obtain a copy of the License at:
*                             www.st.com/SLA0044
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
MOBLE_RESULT PalNvmRead(MOBLEUINT32 address,
                        MOBLEUINT32 offset,
                        void *buf, 
                        MOBLEUINT32 size, 
                        MOBLEBOOL backup);
MOBLE_RESULT PalNvmBackupRead(MOBLEUINT32 address, 
                              void *buf, 
                              MOBLEUINT32 size);
MOBLE_RESULT PalNvmWrite(MOBLEUINT32 address,
                         MOBLEUINT32 offset,
                         void const *buf, 
                         MOBLEUINT32 size);
MOBLEBOOL    PalNvmIsWriteProtected(void);
MOBLE_RESULT PalNvmCompare(MOBLEUINT32 address,
                           MOBLEUINT32 offset,
                           void const *buf, 
                           MOBLEUINT32 size, 
                           MOBLE_NVM_COMPARE* result);
MOBLE_RESULT PalNvmErase(MOBLEUINT32 address,
                         MOBLEUINT32 offset);
MOBLE_RESULT PalNvmProcess(void);

#endif /* __PAL_NVM_H */
