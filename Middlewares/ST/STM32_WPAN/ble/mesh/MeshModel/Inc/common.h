/**
******************************************************************************
* @file    common.h
* @author  BLE Mesh Team
* @brief   Model middleware file
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
#define LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET  3
#define LIGHT_LIGHTNESS_LAST_NVM_OFFSET     5
#define LIGHT_CTL_NVM_OFFSET                7
#define LIGHT_CTL_DEFAULT_NVM_OFFSET        13
#define LIGHT_HSL_NVM_OFFSET                19
#define LIGHT_HSL_DEFAULT_NVM_OFFSET        25

/*  Light model nvm offset used for local array*/
#define LIGHT_DEFAULT_OFFSET       6
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

typedef MOBLE_RESULT (*APPLI_SAVE_MODEL_STATE_CB)(MOBLEUINT8* stateBuff, MOBLEUINT8 size);
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
MOBLE_RESULT  Chk_HslRangeValidity(const MOBLEUINT8* param,MOBLEUINT16 min_param_value_1, 
                                   MOBLEUINT16 max_param_value_1,MOBLEUINT16 min_param_value_2,
                                     MOBLEUINT16 max_param_value_2);
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

MOBLE_RESULT Chk_ParamMinMaxIntValidity(MOBLEINT16 min_param_value, const MOBLEUINT8* param, 
                                                     MOBLEINT16 max_param_value );

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
MOBLEUINT16 Light_lightnessPowerOnValue(MOBLEUINT8 const *pModelValue_Load);
void Light_CtlPowerOnValue(MOBLEUINT8 const *pModelValue_Load);
void Light_HslPowerOnValue(MOBLEUINT8 const *pModelValue_Load);

void MemoryDumpHex(const MOBLEUINT8* memory_addr, int size);
#endif
