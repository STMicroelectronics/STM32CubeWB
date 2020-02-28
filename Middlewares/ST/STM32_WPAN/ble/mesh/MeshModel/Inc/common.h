/**
******************************************************************************
* @file    common.h
* @author  BLE Mesh Team
* @version V1.12.000
* @date    06-12-2019
* @brief   Model middleware file
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
#ifndef __COMMON_H
#define __COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"

#define GENERIC_VALID_FLAG                  0xAA

/* 16 bytes are reserved for generic */
#define GENERIC_VALID_FLAG_OFFSET           0
#define GENERIC_ON_OFF_NVM_OFFSET           1
#define GENERIC_LEVEL_NVM_OFFSET            2
#define GENERIC_POWER_ON_OFF_NVM_OFFSET     4
/* 16 bytes are reserved for light
*  Light model nvm offset is 16 bytes ahead of generic model */
#define LIGHT_VALID_FLAG_OFFSET             0
#define LIGHT_LIGHTNESS_NVM_OFFSET          1
#define LIGHT_CTL_NVM_OFFSET                3
#define LIGHT_HSL_NVM_OFFSET                7
#define LIGHT_HSL_DEFAULT_NVM_OFFSET        13

/* Buffer index limit for the generic data */
#define GENERIC_DATA_LIMIT                  15

#define FLAG_SET      1
#define FLAG_RESET    0

#define VALUE_UPDATE_SET               0X01
#define VALUE_UPDATE_RESET             0X00

#define     CLK_FLAG_ENABLE           1 
#define     CLK_FLAG_DISABLE          0 

#define PWM_ZERO_VALUE                1
#define INTENSITY_LEVEL_ZERO     0X00
#define INTENSITY_LEVEL_FULL     31990U

#define MAX_TID_VALUE          0XFF
//#if STM32 
//typedef MOBLE_RESULT (*APPLI_SAVE_MODEL_STATE_CB)(MOBLEUINT8* stateBuff, MOBLEUINT16 size);
//#elif BLUENRG2_DEVICE
typedef MOBLE_RESULT (*APPLI_SAVE_MODEL_STATE_CB)(MOBLEUINT8* stateBuff, MOBLEUINT8 size);
//#endif
/** @addtogroup MODEL_GENERIC
*  @{
*/

/** @addtogroup Generic_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#pragma pack(1)
typedef struct displayFloatToInt_s 
{
  MOBLEINT8 sign; /* 0 means positive, 1 means negative*/
  MOBLEUINT32  out_int;
  MOBLEUINT32  out_dec;
} displayFloatToInt_t;
#pragma pack(4)

MOBLE_RESULT Chk_OptionalParamValidity(MOBLEUINT8 param_length, MOBLEUINT8
                                        mandatory_length, MOBLEUINT8 param,
                                                  MOBLEUINT8 max_param_value  );
MOBLE_RESULT Chk_ParamMinMaxValidity(MOBLEUINT16 min_param_value, 
                                                 const MOBLEUINT8* param,
                                                 MOBLEUINT16 max_param_value );

MOBLE_RESULT Chk_ParamValidity(MOBLEUINT8 param, MOBLEUINT8 max_param_val );
 
MOBLE_RESULT Chk_RangeValidity(MOBLEUINT16 min_param_value, const MOBLEUINT8* param, 
                                                     MOBLEUINT16 max_param_value );
MOBLE_RESULT  Chk_TwoParamValidity(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                        const MOBLEUINT8* param);
MOBLE_RESULT  Chk_MultiParamValidity(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                      MOBLEINT16 min_param_range3, MOBLEUINT16 max_param_range3,
                                        const MOBLEUINT8* param);
MOBLE_RESULT  Chk_MultiParamValidityAllUnsigned(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                      MOBLEINT16 min_param_range3, MOBLEUINT16 max_param_range3,
                                        const MOBLEUINT8* param);

MOBLE_RESULT Chk_TidValidity(MOBLE_ADDRESS peer_Addrs,MOBLE_ADDRESS dst_Addrs,MOBLEUINT8 tidValue);
MOBLEUINT32 Get_StepResolutionValue(MOBLEUINT8 time_param);

MOBLEUINT16 PwmValueMapping(MOBLEUINT16 setValue , MOBLEUINT16 maxRange , MOBLEINT16 minRange);

float Ratio_CalculateValue(MOBLEUINT16 setValue , MOBLEUINT16 maxRange , MOBLEINT16 minRange);
MOBLEUINT16 PWM_CoolValue(float colourValue ,float brightValue);
MOBLEUINT16 PWM_WarmValue(float colourValue ,float brightValue);
void floatToInt(float in, displayFloatToInt_t *out_value, MOBLEINT32 dec_prec);
void TraceHeader(const char* func_name, int mode);
#ifdef ENABLE_SAVE_MODEL_STATE_NVM
MOBLE_RESULT SaveModelsStateNvm(MOBLEUINT8 flag);
#endif
MOBLEUINT8 BLE_GetElementNumber(void);

void Test_Process(void);
void ModelSave_Process(void);
void BLEMesh_PacketResponseTime(MOBLEUINT8 *testFunctionParm);
MOBLEUINT8 BLE_waitPeriod(MOBLEUINT32 waitPeriod);
MOBLEUINT8 Time_Conversion(MOBLEUINT32 lc_Time);

void Model_RestoreStates(MOBLEUINT8 const *pModelState_Load, MOBLEUINT8 size);
#endif
