/**
******************************************************************************
* @file    common.c
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
/* Includes ------------------------------------------------------------------*/
#include "hal_common.h"
#include "mesh_cfg.h"
#include "generic.h"
#include "light.h"
#include "common.h"
#include "vendor.h"
#include "light_lc.h"
#include <string.h>
#include "appli_nvm.h"
#include "compiler.h"

/** @addtogroup COMMON
*  @{
*/

/** @addtogroup Model_Common_Api
*  @{
*/

WEAK_FUNCTION(MOBLE_RESULT ApplicationGetConfigServerDeviceKey(MOBLE_ADDRESS src,
                                                               const MOBLEUINT8**ppkeyTbUse));
MOBLEUINT8 TimeDelay(MOBLEUINT16 waitPeriod);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#ifdef ENABLE_SAVE_MODEL_STATE_NVM
extern const APPLI_SAVE_MODEL_STATE_CB SaveModelState_cb;
#endif
extern MOBLEUINT8 NumberOfElements;
MOBLEUINT8 PowerOnOff_flag = FLAG_RESET;
MOBLEUINT8 RestoreFlag;
MOBLE_ADDRESS Peer_Addrs;
MOBLE_ADDRESS Dst_Addrs;
MOBLEUINT8 Tid_Value = 0;
MOBLEUINT8 TidSend = 0;
MOBLE_ADDRESS Dst_Peer;

/**
* @brief  Chk_ParamValidity: This function is to check validity of Parameters
* @param  param: Parameter 
* @param  max_param_value: Max Parameter Value 
* @retval MOBLE_RESULT
*/ 
 MOBLE_RESULT Chk_ParamValidity(MOBLEUINT8 param, MOBLEUINT8 max_param_val )
{
  if (param > max_param_val) 
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    return MOBLE_RESULT_SUCCESS;
  }
}


/**
* @brief  Chk_MultiParamValidity: This function is to check validity of range of Parameters
*         of multi parameters.
* @param  param: Parameter 
* @param  min_param_range1: Min Value of parameter 1
* @param  max_param_range1: Max Value of parameter 1
* @param  min_param_range2: Min Value of parameter 2
* @param  max_param_range2: Max Value of parameter 2
* @param  min_param_range3: Min Value of parameter 3
* @param  max_param_range3: Max Value of parameter 3
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT  Chk_MultiParamValidity(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                      MOBLEINT16 min_param_range3, MOBLEUINT16 max_param_range3,
                                        const MOBLEUINT8* param)
{
   MOBLEUINT16 param_value1;
   MOBLEUINT16 param_value2;
   MOBLEINT16 param_value3;
    
    param_value1 =  param[1] << 8;
    param_value1 |= param[0];
    
    param_value2 =  param[3] << 8;
    param_value2 |= param[2];
    
    param_value3 =  param[5] << 8;
    param_value3 |= param[4];
    
  if((param_value1 < min_param_range1) || (param_value1 > max_param_range1)) 
{
    return MOBLE_RESULT_INVALIDARG;
  }
  if((param_value2 < min_param_range2) || (param_value2 > max_param_range2))        
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  if((param_value3 < min_param_range3) || (param_value3 > max_param_range3))        
  {
    return MOBLE_RESULT_INVALIDARG;
  } 
  
    return MOBLE_RESULT_SUCCESS;
  }


/**
* @brief  Chk_MultiParamValidityAllUnsigned: This function is to check validity of range of Parameters
*         of multi parameters.
* @param  param: Parameter 
* @param  min_param_range1: Min Value of parameter 1
* @param  max_param_range1: Max Value of parameter 1
* @param  min_param_range2: Min Value of parameter 2
* @param  max_param_range2: Max Value of parameter 2
* @param  min_param_range3: Min Value of parameter 3
* @param  max_param_range3: Max Value of parameter 3
* @retval MOBLE_RESULT
*/ 
MOBLE_RESULT  Chk_MultiParamValidityAllUnsigned(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                      MOBLEINT16 min_param_range3, MOBLEUINT16 max_param_range3,
                                        const MOBLEUINT8* param)
{
   MOBLEUINT16 param_value1;
   MOBLEUINT16 param_value2;
   MOBLEUINT16 param_value3;
    
    param_value1 =  param[1] << 8;
    param_value1 |= param[0];
    
    param_value2 =  param[3] << 8;
    param_value2 |= param[2];
    
    param_value3 =  param[5] << 8;
    param_value3 |= param[4];
    
  if((param_value1 < min_param_range1) || (param_value1 > max_param_range1)) 
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  if((param_value2 < min_param_range2) || (param_value2 > max_param_range2))        
  {
    return MOBLE_RESULT_INVALIDARG;
  } 
  if((param_value3 < min_param_range3) || (param_value3 > max_param_range3))        
  {
    return MOBLE_RESULT_INVALIDARG;
  } 
  
  return MOBLE_RESULT_SUCCESS;  
}


/**
* @brief  Chk_MultiParamValidity: This function is to check validity of range of Parameters
*         of multi parameters.
* @param  param: Parameter 
* @param  min_param_range1: Min Value of parameter 1
* @param  max_param_range1: Max Value of parameter 1
* @param  min_param_range2: Min Value of parameter 2
* @param  max_param_range2: Max Value of parameter 2
* @retval MOBLE_RESULT
*/ 
 MOBLE_RESULT  Chk_TwoParamValidity(MOBLEUINT16 min_param_range1, MOBLEUINT16 max_param_range1,                                        
                                      MOBLEUINT16 min_param_range2, MOBLEUINT16 max_param_range2,
                                        const MOBLEUINT8* param)
{
   MOBLEUINT16 param_value1;
   MOBLEUINT16 param_value2;
    
    param_value1 =  param[1] << 8;
    param_value1 |= param[0];
    
    param_value2 =  param[3] << 8;
    param_value2 |= param[2];
    
  if((param_value1 < min_param_range1) || (param_value1 > max_param_range1)) 
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  if((param_value2 < min_param_range2) || (param_value2 > max_param_range2))        
  {
    return MOBLE_RESULT_INVALIDARG;
  } 
  
  return MOBLE_RESULT_SUCCESS;  
}


/**
* @brief  Chk_RangeValidity: This function is to check validity of range of Parameters
* @param  param: Parameter 
* @param  max_param_value: Max Parameter Value 
* @param  min_param_value: Min Parameter Value
* @retval MOBLE_RESULT
*/ 
 MOBLE_RESULT  Chk_RangeValidity(MOBLEUINT16 min_param_value, const MOBLEUINT8* param, 
                                                     MOBLEUINT16 max_param_value )
{
   MOBLEUINT16 minRange;
   MOBLEUINT16 maxRange;
    
    minRange =  param[1] << 8;
    minRange |= param[0];
    
    maxRange =  param[3] << 8;
    maxRange |= param[2];
    
  if(minRange < min_param_value)
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  if((maxRange > max_param_value) || (maxRange < minRange))        
  {
    return MOBLE_RESULT_INVALIDARG;
  } 
  
  return MOBLE_RESULT_SUCCESS;  
}

/**
* @brief  Chk_HslRangeValidity: This function is to check validity of range of Parameters
* @param  param: Parameter 
* @param  max_param_value: Max Parameter Value 
* @param  min_param_value: Min Parameter Value
* @retval MOBLE_RESULT
*/ 
 MOBLE_RESULT  Chk_HslRangeValidity(const MOBLEUINT8* param,MOBLEUINT16 min_param_value_1, 
                                   MOBLEUINT16 max_param_value_1,MOBLEUINT16 min_param_value_2,
                                     MOBLEUINT16 max_param_value_2)
{
  MOBLEUINT16 minRange_1;
  MOBLEUINT16 maxRange_1;
  MOBLEUINT16 minRange_2;
  MOBLEUINT16 maxRange_2;
   
  minRange_1 =  param[1] << 8;
  minRange_1 |= param[0];   
  maxRange_1 =  param[3] << 8;
  maxRange_1 |= param[2];
  minRange_2 =  param[5] << 8;
  minRange_2 |= param[4];   
  maxRange_2 =  param[7] << 8;
  maxRange_2 |= param[6];
    
  if(minRange_1 < min_param_value_1)
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  if((maxRange_1 > max_param_value_1) || (maxRange_1 < minRange_1))        
  {
    return MOBLE_RESULT_INVALIDARG;
  } 
  if(minRange_2 < min_param_value_2)
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  if((maxRange_2 > max_param_value_2) || (maxRange_2 < minRange_2))        
  {
    return MOBLE_RESULT_INVALIDARG;
  } 
  
  return MOBLE_RESULT_SUCCESS;  
}


/**
* @brief  Chk_OptionalParamValidity: This function is to check validity of optional Parameters
* @param  param_length:  Parameter length 
* @param  mandatory_length: Mandatory Length
* @param  param: Parameter 
* @param  max_param_value: Max Parameter Value 
* @retval MOBLE_RESULT
*/ 
 MOBLE_RESULT Chk_OptionalParamValidity(MOBLEUINT8 param_length, MOBLEUINT8 mandatory_length, 
                                            MOBLEUINT8 param, MOBLEUINT8 max_param_value  )
{
  if ((param_length > mandatory_length) && (param > max_param_value))
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    return MOBLE_RESULT_SUCCESS;
  }
}

/**
* @brief  Chk_ParamMinMaxIntValidity: This function is to check validity of optional Parameters
* @param  param: Parameter 
* @param  max_param_value: signed Max Parameter Value 
* @param  min_param_value: signed Min Parameter Value 
* @retval MOBLE_RESULT
*/ 
 MOBLE_RESULT Chk_ParamMinMaxIntValidity(MOBLEINT16 min_param_value, const MOBLEUINT8* param, 
                                                     MOBLEINT16 max_param_value )
{
  MOBLEINT16 param_State1;
    
  param_State1 =  param[1] << 8;
  param_State1 |= param[0];
    
  if((param_State1 < min_param_value) || (param_State1 > max_param_value))
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    return MOBLE_RESULT_SUCCESS;
  }
}

/**
* @brief  Chk_ParamMinMaxValidity: This function is to check validity of optional Parameters
* @param  param: Parameter 
* @param  max_param_value: unsigned Max Parameter Value 
* @param  min_param_value: unsigned Min Parameter Value 
* @retval MOBLE_RESULT
*/ 
 MOBLE_RESULT Chk_ParamMinMaxValidity(MOBLEUINT16 min_param_value, const MOBLEUINT8* param, 
                                                     MOBLEUINT16 max_param_value )
{
   MOBLEUINT16 param_State1;
    
    param_State1 =  param[1] << 8;
    param_State1 |= param[0];
    
  if((param_State1 < min_param_value) || (param_State1 > max_param_value))
  {
    return MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    return MOBLE_RESULT_SUCCESS;
  }
}


 /*
  @brief  Get_StepResolutionValue
          To get the default transition step resolution value 
* @param  time_param: intger value received for transition time
* @retval stepResolution 
 */                                         
MOBLEUINT32 Get_StepResolutionValue(MOBLEUINT8 time_param)
{
  MOBLEUINT32 stepResolution = 0;
  
  if(time_param == STEP_HEX_VALUE_0)
  {
    stepResolution = STEP_RESOLUTION_0;   
  }
  else if(time_param == STEP_HEX_VALUE_1)
  {
    stepResolution = STEP_RESOLUTION_1;   
  }
  else if(time_param == STEP_HEX_VALUE_2)
  {
    stepResolution = STEP_RESOLUTION_2;   
  }
  else if(time_param == STEP_HEX_VALUE_3)
  {
    stepResolution = STEP_RESOLUTION_3;
  }
  else
  {
   /* stepResolution = STEP_RESOLUTION_0; */   
  }  
  return stepResolution;  
}


/**
* @brief PwmValueMapping: This function takes the value as parameter from the 
*        model and return the value according to .
*@param  setValue: value to be set from model
*@param  maxRange: Max range of the parameter
*@param  minRange: Min range of the parameter
* @retval MOBLEUINT8
*/
MOBLEUINT16 PwmValueMapping(MOBLEUINT16 setValue , MOBLEUINT16 maxRange , MOBLEINT16 minRange)
{
  MOBLEUINT16 percentValue;
  MOBLEUINT16 duty;
  
#ifdef SMART_PLUG
   percentValue =  (setValue * 100)/ (maxRange - minRange);
        
   if(percentValue < 2)
   {
     duty = 0;
   }   
   else if((percentValue > 2) && (percentValue <= 10))
   {
     duty = 1;
   }
   else if((percentValue > 10) && (percentValue <= 20))
   {
     duty = 2;
   }
   else if((percentValue > 20) && (percentValue <= 30))
   {
     duty = 3;
   }
   else if((percentValue > 30) && (percentValue <= 40))
   {
     duty = 4;
   }
   else if((percentValue > 40) && (percentValue <= 50))
   {
     duty = 5;
   }
   else if((percentValue > 50) && (percentValue <= 60))
   {
     duty = 6;
   }
   else if((percentValue > 60) && (percentValue <= 70))
   {
     duty = 7;
   }
   else if((percentValue > 70) && (percentValue <= 80))
   {
     duty = 8;
   }
   else if((percentValue > 80) && (percentValue <= 90))
   {
     duty = 9;
   }
   else if(percentValue > 90)
   {
     duty = 10;
   }
#else
      if(minRange > 0x00)
      {
        percentValue =  (setValue - 800)/ (maxRange - minRange);
        percentValue = (percentValue*100);
      }
      else
      {
      percentValue =  (setValue * 100)/ (maxRange - minRange);
      }
      
      duty = percentValue*320 ;
       
      if(duty >= 32000)
   {
        duty = 31999;
   }   
      if(duty == 0)
      {
        duty = 1;
      }
#endif
     
   return duty;
}


/**
* @brief  Ratio_CalculateValue: This function is used to calculate the ratio of
          set value to the maximum value.
* @param  setValue: set value from the client.
* @param  maxRange: Maximum value for the given model.
* @param  minRange: Minimum value for the given model.
* @retval Ratio: ratio of the set value
*/ 
float Ratio_CalculateValue(MOBLEUINT16 setValue , MOBLEUINT16 maxRange , MOBLEINT16 minRange)
{
      float Ratio;
      if(minRange == 0)
   {
         Ratio = (float)(setValue)/(maxRange - minRange);
   }
      else
   {
         Ratio = (float)(setValue-minRange)/(maxRange - minRange);
   }
      
      return Ratio;
  
   }

/**
* @brief  PWM_CoolValue: This function is used to calculate the value for the 
          cool temperature for the light.
* @param  colourValue: Temperature value ratio
* @param  brightValue: Lightness value ratio.
* @retval duty: duty fot the PWM
*/ 
MOBLEUINT16 PWM_CoolValue(float colourValue ,float brightValue)
   {
    float colourDuty; 
    MOBLEUINT16 duty;
     
    colourDuty = colourValue * brightValue;    
    duty = (MOBLEUINT16)(colourDuty * PWM_TIME_PERIOD);
    
    return duty;
   } 


/**
* @brief  PWM_WarmValue: This function is used to calculate the value for the 
          warm temperature for the light.
* @param  colourValue: Temperature value ratio
* @param  brightValue: Lightness value ratio.
* @retval duty: duty fot the PWM
*/ 
MOBLEUINT16 PWM_WarmValue(float colourValue ,float brightValue)
   {
    float warmDuty;    
    MOBLEUINT16 duty;
   
    warmDuty = (1-colourValue)* brightValue;
    duty = (MOBLEUINT16)(warmDuty * PWM_TIME_PERIOD);
     
   return duty;
}

/**
* @brief  TraceHeader: This function is used print the timestamp , function name
*         depends on the flag.
* @param  func_name: function pointer.
* @param  mode: Flag
* @retval void
*/ 
void TraceHeader(const char* func_name, int mode)
{
    if (mode >= 0)
        printf("%ld %s - ", Clock_Time(), func_name);
    else
        printf("%ld %s - <<<ERROR>>>", Clock_Time(), func_name);
}    

void MemoryDumpHex(const MOBLEUINT8* memory_addr, int size) 
{
  int row_index;
  int column_index;
  int rows_to_dump;
    
  rows_to_dump = (size+15)/16;
  unsigned char *p = (unsigned char *)memory_addr;

  for (row_index = 0; row_index < rows_to_dump; row_index++) 
  {
    for (column_index =0; column_index < 16; column_index++) 
    {
      printf("0x%02x ", p[column_index]);
    }
    printf("\n\r");   /* Put a line change */
  }
}

#ifdef ENABLE_SAVE_MODEL_STATE_NVM
/**
* @brief  Prepare and save buffer of Generic and Light models state in NVM
* @param  void
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT SaveModelsStateNvm(MOBLEUINT8 flag)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  MOBLEUINT8 Model_GetBuff[APP_NVM_MODEL_SIZE];/* 16 bytes for generic model and 16 bytes for light model */
  
  memset(Model_GetBuff, 0x00, APP_NVM_MODEL_SIZE);
  
  Model_GetBuff[GENERIC_VALID_FLAG_OFFSET] = flag;
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF   
  (Appli_GenericState_cb.GetOnOffStatus_cb)(&Model_GetBuff[GENERIC_ON_OFF_NVM_OFFSET]);
#endif  
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL	
  (Appli_GenericState_cb.GetLevelStatus_cb)(&Model_GetBuff[GENERIC_LEVEL_NVM_OFFSET]);
#endif 
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF   
  (Appli_GenericState_cb.GetPowerOnOffStatus_cb)(&Model_GetBuff[GENERIC_POWER_ON_OFF_NVM_OFFSET]);
#endif  
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS	
  (Appli_Light_GetStatus_cb.GetLightLightness_cb)(&Model_GetBuff[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_NVM_OFFSET]);
  (Appli_Light_GetStatus_cb.GetLightLightnessDefault_cb)(&Model_GetBuff[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET]);
  (Appli_Light_GetStatus_cb.GetLightLightnessLast_cb)(&Model_GetBuff[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET]);
 
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL    
  (Appli_Light_GetStatus_cb.GetLightCtl_cb)(&Model_GetBuff[GENERIC_DATA_LIMIT+LIGHT_CTL_NVM_OFFSET]);
  (Appli_Light_GetStatus_cb.GetLightCtlDefault_cb)(&Model_GetBuff[GENERIC_DATA_LIMIT+LIGHT_CTL_DEFAULT_NVM_OFFSET]);
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL  
  (Appli_Light_GetStatus_cb.GetLightHsl_cb)(&Model_GetBuff[GENERIC_DATA_LIMIT+LIGHT_HSL_NVM_OFFSET]);
  (Appli_Light_GetStatus_cb.GetLightHslDefault_cb)(&Model_GetBuff[GENERIC_DATA_LIMIT+LIGHT_HSL_DEFAULT_NVM_OFFSET]);
  
#endif  
  if (SaveModelState_cb != NULL)
  {
    result = SaveModelState_cb(Model_GetBuff, APP_NVM_MODEL_SIZE);
  }
  
  if (MOBLE_FAILED(result))
  {
    TRACE_M(TF_MISC, "Failed to save model state \r\n");
  }
  else if (result == MOBLE_RESULT_FALSE)
  {
    TRACE_M(TF_MISC, "Not able to save model state. Try again \r\n");
  }
  
  return result;
}
#endif
/*
* @brief function to call light middle layer function for restoration of 
*        saved states.
* @param void.
* return void.
*/
void Model_RestoreStates(MOBLEUINT8 const *pModelState_Load, MOBLEUINT8 size)
{ 
  MOBLEUINT8 pData[2];
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT8 elementNumber;
  MOBLEUINT16 model_ID = 0;
  MOBLEUINT16 opcode = 0;
  MOBLEUINT32 length = 0;
  MOBLE_ADDRESS my_Address;
  if (size > 0)
  {
    switch(pModelState_Load[0])
    { 
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF      
      case GENERIC_ON_OFF_NVM_FLAG:
      {
        /* checking the Power on off retrieved value according to the given
           in standered and taking decision for Generic on off.
        */
           
        if(pModelState_Load[4] == GENERIC_POWER_OFF_STATE)
        {
          pData[0] = APPLI_LED_OFF;
          Generic_OnOff_Set(pData,1);
        }
        else if(pModelState_Load[4] == GENERIC_POWER_ON_STATE)
        {
          pData[0] = APPLI_LED_ON;
          Generic_OnOff_Set(pData,1);
        }
        else if(pModelState_Load[4] == GENERIC_POWER_RESTORE_STATE)
        {
          Generic_OnOff_Set(pModelState_Load+GENERIC_ON_OFF_NVM_OFFSET, 1); 
        }
        else
        {
          TRACE_M(TF_GENERIC, "Power On Off value invalid %d \r\n", pModelState_Load[0]);
        }         
              opcode = GENERIC_ON_OFF_SET_UNACK;
              model_ID = GENERIC_MODEL_SERVER_ONOFF_MODEL_ID;
        break;
      }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL        
      case GENERIC_LEVEL_NVM_FLAG:
      {
        Generic_Level_Set(pModelState_Load+GENERIC_LEVEL_NVM_OFFSET,2);
        break;
      }
#endif
        
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS        
      case LIGHT_LIGHTNESS_NVM_FLAG:
      { 
           MOBLEUINT16 light_LightnessValue;
           light_LightnessValue = Light_lightnessPowerOnValue(pModelState_Load);
           *pData = light_LightnessValue;
           *(pData+1) = light_LightnessValue >> 8;
           Light_Lightness_Set(pData, 2);
          
           opcode = LIGHT_LIGHTNESS_SET_UNACK;
           model_ID = LIGHT_MODEL_SERVER_LIGHTNESS_MODEL_ID;
        break;
      } 
#endif 
        
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL         
      case LIGHT_CTL_NVM_FLAG:
      {  
          
        Light_CtlPowerOnValue(pModelState_Load);
        opcode = LIGHT_CTL_TEMPERATURE_SET_UNACK;
        model_ID = LIGHT_MODEL_SERVER_CTL_TEMPERATURE_MODEL_ID;
        break;
      }
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL        
      case LIGHT_HSL_NVM_FLAG:
      {  
          if((pModelState_Load[4] == GENERIC_POWER_OFF_STATE) || (pModelState_Load[4] == GENERIC_POWER_ON_STATE))
          {         
            Light_Hsl_Set((pModelState_Load+GENERIC_DATA_LIMIT+LIGHT_HSL_DEFAULT_NVM_OFFSET), 6);
            Light_HslDefault_Set((pModelState_Load+GENERIC_DATA_LIMIT+LIGHT_HSL_DEFAULT_NVM_OFFSET), 6);
          }          
          else if(pModelState_Load[4] == GENERIC_POWER_RESTORE_STATE)
          {
        Light_Hsl_Set((pModelState_Load+GENERIC_DATA_LIMIT+LIGHT_HSL_NVM_OFFSET), 6);
          }
          else
          {
            TRACE_M(TF_GENERIC, "Power On Off value invalid %d \r\n", pModelState_Load[0]);
          }                   
           
           opcode = LIGHT_HSL_SET_UNACK;
           model_ID = LIGHT_MODEL_SERVER_HSL_MODEL_ID;
        break;
      }
#endif        
      case No_NVM_FLAG:
      {
        TRACE_M(TF_GENERIC,"Power OnOff value stored = %d \r\n",pModelState_Load[4]);
        break;
      }
      default: 
      {
          TRACE_M(TF_LIGHT, "No Saved Data Found \r\n");
        break;
      }
    }     pData[0] = pModelState_Load[4];
          Generic_PowerOnOff_Set(pData,1);
     
    my_Address = BLEMesh_GetAddress();
    elementNumber = BLE_GetElementNumber();
    publishAddress = BLEMesh_GetPublishAddress(elementNumber,model_ID);
    
    if(publishAddress != 0x0000 )
    {
      Model_SendResponse(publishAddress,my_Address,opcode,pData,length);         
      
      TRACE_I(TF_MISC,"Publishing the Power on state to address %.2X \r\n",publishAddress);
    }
     
    }
     
    }
  
/**
* @brief  Function used to restore the light lighness with respect to Power on off
*         value .
* @param  pModelValue_Load:array of saved data
* @retval MOBLEUINT16
*/
MOBLEUINT16 Light_lightnessPowerOnValue(MOBLEUINT8 const *pModelValue_Load)
{
  MOBLEUINT16 light_DefaultValue;
  MOBLEUINT16 light_LastValue;
  MOBLEUINT8 powerOn_Value;
  MOBLEUINT16 light_Actual = 0;
  MOBLEUINT16 last_Known_Value;
  
  light_DefaultValue = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET+1]<<8;
  light_DefaultValue |= pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET];
  
  light_LastValue = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET+1]<<8; 
  light_LastValue |= pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET]; 
  
  last_Known_Value = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_NVM_OFFSET+1]<<8;  
  last_Known_Value |= pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_NVM_OFFSET];
  
  powerOn_Value = *(pModelValue_Load+GENERIC_POWER_ON_OFF_NVM_OFFSET);
  
  Light_Lightness_Last_Set((pModelValue_Load+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET),2);
  Light_Lightness_Default_Set((pModelValue_Load+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET),2);
   
  if(powerOn_Value == GENERIC_POWER_OFF_STATE)
  {
    light_Actual = 0x00;
  }
  else if((powerOn_Value == GENERIC_POWER_ON_STATE) && (light_DefaultValue != 0x00))
  {
    light_Actual = light_DefaultValue;
  }
  else if((powerOn_Value == GENERIC_POWER_ON_STATE) && (light_DefaultValue == 0x00))
  {
    light_Actual = light_LastValue;
  }
  else if(powerOn_Value == GENERIC_POWER_RESTORE_STATE)
  {
    light_Actual = last_Known_Value;
  }
  else
  {
     
  }
  
  printf("LIGHT LIGHTNESS VALUE ON POWER UP %.2x \r\n",light_Actual);
  return light_Actual;
}  

/**
* @brief  Function used to restore the light CTL with respect to Power on off
*         value .
* @param  pModelValue_Load:array of saved data
* @retval MOBLEUINT16
*/
void Light_CtlPowerOnValue(MOBLEUINT8 const *pModelValue_Load)
{ 
  MOBLEUINT8 powerOn_Value;
  MOBLEUINT8 pData[12];
          
  memcpy((void*)pData, (void*)(pModelValue_Load+GENERIC_DATA_LIMIT+LIGHT_CTL_NVM_OFFSET), 12);         
  powerOn_Value = *(pModelValue_Load+GENERIC_POWER_ON_OFF_NVM_OFFSET);
  
  if((powerOn_Value == GENERIC_POWER_OFF_STATE) || (powerOn_Value == GENERIC_POWER_ON_STATE))
  {
    /* pData has first 6 byte for ctl set including 2 byte for lightness, 2 bytes for
       temperature , 2 bytes for delta uv.
       next 6 byte for Ctl default set
    */
    Light_CtlDefault_Set((pData+LIGHT_DEFAULT_OFFSET), 6); 
    Light_CtlTemperature_Set((pData+8), 4);
  }
  else if(powerOn_Value == GENERIC_POWER_RESTORE_STATE)
  {
    Light_CtlTemperature_Set((pData+2), 4);
  }
  else
  {
    
  }
}
  
/**
* @brief  Function used to restore the light HSL with respect to Power on off
*         value .
* @param  pModelValue_Load:array of saved data
* @retval MOBLEUINT16
*/
void Light_HslPowerOnValue(MOBLEUINT8 const *pModelValue_Load)
{ 
  MOBLEUINT8 powerOn_Value;
  MOBLEUINT8 pData[12];
          
  memcpy((void*)pData, (void*)(pModelValue_Load+GENERIC_DATA_LIMIT+LIGHT_HSL_NVM_OFFSET), 12);         
  powerOn_Value = *(pModelValue_Load+GENERIC_POWER_ON_OFF_NVM_OFFSET);
  
  if((powerOn_Value == GENERIC_POWER_OFF_STATE) || (powerOn_Value == GENERIC_POWER_ON_STATE))
  {
    /* pData has first 6 byte for ctl set including 2 byte for lightness, 2 bytes for
       Hue , 2 bytes for Saturation.
       next 6 byte for Ctl default set
    */
    Light_HslDefault_Set((pData+LIGHT_DEFAULT_OFFSET), 6); 
    Light_HslHue_Set((pData+8), 2);
    Light_HslSaturation_Set((pData+10), 2);
  }
  else if(powerOn_Value == GENERIC_POWER_RESTORE_STATE)
  {
    Light_HslHue_Set((pData+2), 2);
    Light_HslSaturation_Set((pData+4), 2);    
  }
  else
  {
    
  }
}
  
/**
* @brief  Function used to select the element number
* @param  void
* @retval element number
*/
MOBLEUINT8 BLE_GetElementNumber(void)
{

  MOBLEUINT8 elementNumber = 0;
  
  /*Select the Element Number for which publication address is required*/
  
  if (NumberOfElements == 1)
  {
    elementNumber = 0x01; 
  }
  
  else if(NumberOfElements == 2)
  { 
    elementNumber = 0x02; /*Element 2 is configured as switch*/
  }
  
  else if(NumberOfElements == 3)
  {
    elementNumber = 0x03; /*Element 3 is configured as switch*/
  }
  return elementNumber;
}  

#ifdef ENABLE_SAVE_MODEL_STATE_NVM
/**
* @brief  Function used to save the states of the node, when power down is detected.
* @param  void
* @retval void
*/
void ModelSave_Process(void)
{
  if(PowerOnOff_flag == FLAG_SET)
  {
    SaveModelsStateNvm(RestoreFlag);
    PowerOnOff_flag = FLAG_RESET;
  }
}

#endif
/**
* @brief  Function used to calculate the delay.
* @param  MOBLEUINT16
* @retval MOBLEUINT8
*/
MOBLEUINT8 BLE_waitPeriod(MOBLEUINT32 waitPeriod)
{
   static MOBLEUINT8 Clockflag = 0;
   static MOBLEUINT32 Check_time;
   
 
   if(Clockflag == CLK_FLAG_DISABLE)
   {
     Check_time = Clock_Time();
     Clockflag = CLK_FLAG_ENABLE;
   } 
/* The function will called untill the testcount will not become zero */     

     if(((Clock_Time()- Check_time) >= waitPeriod))
     {
        Clockflag = CLK_FLAG_DISABLE;
        return 0x01;
                
       }
   return 0x00;
}

/**
* @brief  Function used to calculate the delay.
* @param  MOBLEUINT16
* @retval MOBLEUINT8
*/
MOBLEUINT8 TimeDelay(MOBLEUINT16 waitPeriod)
{
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;
  
  
  if(Clockflag == CLK_FLAG_DISABLE)
  {
    Check_time = Clock_Time();
    Clockflag = CLK_FLAG_ENABLE;
  } 
  /* The function will called untill the testcount will not become zero */     
  
  if(((Clock_Time()- Check_time) <= waitPeriod))
  {
    Clockflag = CLK_FLAG_DISABLE;
    return 0x01;
    
  }
  return 0x00;
}

MOBLE_RESULT Chk_TidValidity(MOBLE_ADDRESS peer_Addrs,MOBLE_ADDRESS dst_Addrs,MOBLEUINT8 tidValue)
{
  static MOBLEUINT32 Check_time;
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  
  if(((Clock_Time()- Check_time) <= 6000))
  {
    if((Peer_Addrs == peer_Addrs)&&(Dst_Addrs == dst_Addrs)&&(Tid_Value == tidValue))
    {
      TRACE_M(TF_COMMON,"dst_peer = %.2X , peer_add = %.2X,tid = %.2X \r\n",dst_Addrs,peer_Addrs,tidValue);
      TRACE_M(TF_COMMON,"Duplicate Message Parameter within six second \r\n");
      status =  MOBLE_RESULT_INVALIDARG;       
    }
    else
    {
      Check_time = Clock_Time();
      Peer_Addrs = peer_Addrs;
      Dst_Addrs = dst_Addrs;
      Tid_Value = tidValue; 
      TRACE_M(TF_COMMON,"dst_peer = %.2X , peer_add = %.2X,tid = %.2X \r\n",dst_Addrs,peer_Addrs,tidValue);
      TRACE_M(TF_COMMON,"New Message Parameter within six second \r\n");
    }
  }
  else
  {
    Check_time = Clock_Time();
    Peer_Addrs = peer_Addrs;
    Dst_Addrs = dst_Addrs;
    Tid_Value = tidValue;
    TRACE_M(TF_COMMON,"dst_peer = %.2X , peer_add = %.2X,tid = %.2X \r\n",dst_Addrs,peer_Addrs,tidValue);
    TRACE_M(TF_COMMON,"New Message Parameter  \r\n");
  }
  
  return status;
}

WEAK_FUNCTION(MOBLE_RESULT ApplicationGetConfigServerDeviceKey(MOBLE_ADDRESS src, 
                                                               const MOBLEUINT8**ppkeyTbUse))
{
  return MOBLE_RESULT_SUCCESS;
}
/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

