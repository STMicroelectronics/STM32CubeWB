/**
******************************************************************************
* @file    appli_nvm.h
* @author  BLE Mesh Team
* @version V1.11.000
* @date    25-07-2019
* @brief   Header file for the user application file 
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
#ifndef __APPLI_NVM_H
#define __APPLI_NVM_H

/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables  ------------------------------------------------------- */
#define APP_NVM_BASE                                  ((unsigned int)appNvmBase)
#define APP_NVM_SIZE                                                       4096U
#define PRVN_NVM_BASE_OFFSET                         ((unsigned int)prvsnr_data)
#define PRVN_NVM_PAGE_SIZE                                                 4096U

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size);
MOBLE_RESULT AppliNvm_FactorySettingReset(void);
//MOBLE_RESULT AppliNvm_FlashErase(uint16_t PageNumber);
MOBLE_RESULT AppliNvm_SaveModelState(uint8_t* state, uint8_t size);
MOBLE_RESULT AppliNvm_ClearModelState(void);
MOBLE_RESULT AppliNvm_LoadModelState(uint8_t state[], uint8_t* size);
void AppliNvm_Process(void);
void AppliNvm_SaveMessageParam (void);
MOBLE_RESULT AppliNvm_FindFirstEmptyPage(MOBLEINT8* subPageIndex,MOBLEUINT32 SubPageSize,
                                          MOBLEUINT32 totalSubPage, MOBLEUINT32 nvmBaseOffset);
MOBLE_RESULT AppliPrvnNvm_LoadData(uint8_t state[], uint8_t* size);
MOBLE_RESULT AppliPrvnNvm_SaveData(uint8_t* data, uint8_t size);
MOBLEUINT32* AppliPrvnNvm_GetNodeDevKey(MOBLE_ADDRESS addrss);
void AppliPrvnNvm_Process(void);

MOBLE_RESULT AppliPrvnNvm_FactorySettingReset(MOBLEUINT8*flag);

MOBLE_RESULT AppliPrvnNvm_CopyData(uint8_t state[], uint8_t* size);
void AppliNvm_saveProvisionerDevKey(MOBLEUINT8 *data , MOBLEUINT8 size 
                                                     , MOBLEUINT8 *prvnFlag);
#endif /* __APPLI_NVM_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

