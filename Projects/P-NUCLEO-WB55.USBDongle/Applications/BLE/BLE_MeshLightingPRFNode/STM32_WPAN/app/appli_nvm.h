/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_nvm.h
  * @author  MCD Application Team
  * @brief   Header file for the NVM application file 
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
#ifndef APPLI_NVM_H
#define APPLI_NVM_H

/* Includes ------------------------------------------------------------------*/

#include "mesh_cfg.h"

/* Exported macro ------------------------------------------------------------*/

extern const void *prvsnr_data;

/* offset defined for the embedded provisioner node */
#if defined(STM32WB55xx)
#define PRVN_NVM_PAGE_SIZE                                                 4096U
#elif defined(STM32WB15xx)
#define PRVN_NVM_PAGE_SIZE                                                 2048U
#endif
#define PRVN_NVM_BASE_OFFSET                         ((unsigned int)prvsnr_data)
#define PRVN_NVM_CHUNK_SIZE         (sizeof(NvmPerNode_t)) /* Number of bytes saved per Element */
#define PRVN_NVM_SUBPAGE_SIZE       (sizeof(NvmPerNode_t)) /* Number of bytes saved per Element */
#define PRVN_NVM_MAX_SUBPAGE        (unsigned int)(PRVN_NVM_PAGE_SIZE/PRVN_NVM_CHUNK_SIZE)
#define PRVN_NVM_SUBPAGE_OFFSET(i)  (unsigned int)(PRVN_NVM_SUBPAGE_SIZE*(i))

#define APP_NVM_BASE                                  ((unsigned int)appNvmBase)
#if defined(STM32WB55xx)
#define APP_NVM_SIZE                                                       4096U
#elif defined(STM32WB15xx)
#define APP_NVM_SIZE                                                       2048U
#endif

/* Exported variables  ------------------------------------------------------- */
typedef struct
{
  MOBLEUINT8 devKey[4+DEVICE_KEY_SIZE];

#if ENABLE_SAVE_UUID_PER_NODE  
  MOBLEUINT8 node_UUID[UUID_SIZE];
#endif
  MOBLEUINT8 dummy[4];                    /*  4 bytes more for 3 or 5 words of 64 bits */
} NvmPerNode_t;

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size);
MOBLE_RESULT AppliNvm_FactorySettingReset(void);
#if 0 //#ifdef ENABLE_NVM_TEST
MOBLE_RESULT AppliNvm_FlashErase(uint16_t PageNumber);
#endif
MOBLE_RESULT AppliNvm_SaveModelState(uint8_t* state, uint16_t size);
MOBLE_RESULT AppliNvm_ClearModelState(void);
MOBLE_RESULT AppliNvm_LoadModelState(uint8_t state[], uint16_t* size);
void AppliNvm_Process(void);
void AppliNvm_SaveMessageParam (void);
MOBLE_RESULT AppliNvm_FindFirstEmptyPage(MOBLEINT16* subPageIndex,
                                         MOBLEUINT32 SubPageSize,
                                         MOBLEUINT32 totalSubPage, 
                                         MOBLEUINT32 nvmBaseOffset);
MOBLE_RESULT AppliPrvnNvm_LoadData(uint8_t state[], uint8_t* size);

MOBLE_RESULT AppliPrvnNvm_SaveData(uint8_t* data, 
                                   MOBLE_ADDRESS address,
                                   uint8_t* pUUID, 
                                   uint8_t numElements);

MOBLEUINT32* AppliPrvnNvm_GetNodeDevKey(MOBLE_ADDRESS address);
void AppliPrvnNvm_Process(void);

MOBLE_RESULT AppliPrvnNvm_FactorySettingReset(MOBLEUINT8*flag);

MOBLE_RESULT AppliPrvnNvm_CopyData(uint8_t state[], uint8_t* size);

void AppliPrvnNvm_SaveProvisionerDevKey(MOBLEUINT8 *data , 
                                        MOBLE_ADDRESS address,
                                        MOBLEUINT8 *prvnFlag, 
                                        MOBLEUINT8 numOfElements);

void AppliPrvnNvm_SaveKeys (MOBLEUINT8 status);

MOBLE_RESULT AppliNVM_Save_FlashTesting(MOBLEUINT8 *buffer,
                                        MOBLEUINT16 buffer_size);
MOBLE_RESULT AppliNVM_Retrieve_FlashTesting(MOBLEUINT8 *buffer,
                                            MOBLEUINT16 buffer_size);


#endif /* APPLI_NVM_H */


