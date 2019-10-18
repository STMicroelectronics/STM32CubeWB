/**
******************************************************************************
* @file    appli_nvm.h
* @author  BLE Mesh Team
* @version V1.10.000
* @date    15-Jan-2019
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
* Initial BlueNRG-Mesh is built over Motorola’s Mesh over Bluetooth Low Energy 
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

/* Exported Functions Prototypes ---------------------------------------------*/
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size);
MOBLE_RESULT AppliNvm_FactorySettingReset(void);
//MOBLE_RESULT AppliNvm_FlashErase(uint16_t PageNumber);
MOBLE_RESULT AppliNvm_SaveModelState(uint8_t* state, uint8_t size);
MOBLE_RESULT AppliNvm_ClearModelState(void);
//MOBLE_RESULT AppliNvm_LoadGenericState(uint8_t state[], uint8_t* size);
//MOBLE_RESULT AppliNvm_LoadLightState(uint8_t state[], uint8_t* size);
MOBLE_RESULT AppliNvm_LoadModelState(uint8_t state[], uint8_t* size);
void AppliNvm_Process(void);
void AppliNvm_SaveMessageParam (void);

#endif /* __APPLI_NVM_H */

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

