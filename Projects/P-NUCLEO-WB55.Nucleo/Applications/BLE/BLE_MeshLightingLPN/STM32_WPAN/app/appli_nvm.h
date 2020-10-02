/**
  ******************************************************************************
  * @file    appli_nvm.h
  * @author  BLE Mesh Team
  * @brief   Header file for the NVM application file 
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
#ifndef __APPLI_NVM_H
#define __APPLI_NVM_H

/* Includes ------------------------------------------------------------------*/

#include "mesh_cfg.h"

/* Exported macro ------------------------------------------------------------*/

extern const void *prvsnr_data;

/* offset defined for the embedded provisioner node */
#define PRVN_NVM_PAGE_SIZE                                                 4096U
#define PRVN_NVM_BASE_OFFSET                         ((unsigned int)prvsnr_data)
#define PRVN_NVM_CHUNK_SIZE                               (sizeof(NvmPerNode_t)) /* Number of bytes saved per Element */
#define PRVN_NVM_SUBPAGE_SIZE                              (sizeof(NvmPerNode_t)) /* Number of bytes saved per Element */
#define PRVN_NVM_MAX_SUBPAGE        (unsigned int)(PRVN_NVM_PAGE_SIZE/PRVN_NVM_CHUNK_SIZE)
#define PRVN_NVM_SUBPAGE_OFFSET(i)  (unsigned int)(PRVN_NVM_SUBPAGE_SIZE*(i))

#define APP_NVM_BASE                                  ((unsigned int)appNvmBase)
#define APP_NVM_SIZE                                                       4096U

/* Exported variables  ------------------------------------------------------- */
typedef struct
{
  MOBLEUINT8 devKey[4+DEVICE_KEY_SIZE];

#if ENABLE_SAVE_UUID_PER_NODE  
  MOBLEUINT8 node_UUID[UUID_SIZE];
#endif
 
} NvmPerNode_t;

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size);
MOBLE_RESULT AppliNvm_FactorySettingReset(void);
//MOBLE_RESULT AppliNvm_FlashErase(uint16_t PageNumber);
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

MOBLEUINT32* AppliPrvnNvm_GetNodeDevKey(MOBLE_ADDRESS addrss);
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


#endif /* __APPLI_NVM_H */

/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

