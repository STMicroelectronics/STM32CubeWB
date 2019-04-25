/**
******************************************************************************
* @file    common.c
* @author  BLE Mesh Team
* @version V1.09.000
* @date    15-Oct-2018
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
#include "ble_mesh.h"
#include "models_if.h"
#include "appli_generic.h"
#include "mesh_cfg.h"
#include "common.h"
#include <string.h>


/** @addtogroup MODEL_GENERIC
*  @{
*/

/** @addtogroup Generic_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

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
* @brief  Chk_ParamMinMaxValidity: This function is to check validity of optional Parameters
* @param  param: Parameter 
* @param  max_param_value: Max Parameter Value 
* @param  min_param_value: Min Parameter Value 
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
  MOBLEUINT32 stepResolution;
  
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

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/

