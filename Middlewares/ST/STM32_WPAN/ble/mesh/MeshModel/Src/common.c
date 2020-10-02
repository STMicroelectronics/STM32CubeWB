/**
******************************************************************************
* @file    common.c
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
extern const APPLI_SAVE_MODEL_TEST_STATE_CB SaveModelTestState_cb;
extern const APPLI_RETRIEVE_MODEL_TEST_STATE_CB RetrieveModelTestState_cb;
#endif
extern Light_ModelFlag_t Light_ModelFlag[APPLICATION_NUMBER_OF_ELEMENTS];
extern MOBLEUINT8 NumberOfElements;
MOBLEUINT8 PowerOnOff_flag = FLAG_RESET;
MOBLEUINT8 RestoreFlag[APPLICATION_NUMBER_OF_ELEMENTS];

Model_Tid_t Model_Tid;
/*Variables used for the publishing of binded data */
Model_Binding_Var_t Model_Binding_Var;

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
MOBLE_RESULT Chk_MultiParamValidity(MOBLEUINT16 min_param_range1, 
                                     MOBLEUINT16 max_param_range1,                                        
                                     MOBLEUINT16 min_param_range2, 
                                     MOBLEUINT16 max_param_range2,
                                     MOBLEINT16 min_param_range3, 
                                     MOBLEUINT16 max_param_range3,
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
MOBLE_RESULT Chk_MultiParamValidityAllUnsigned(MOBLEUINT16 min_param_range1, 
                                                MOBLEUINT16 max_param_range1,
                                                MOBLEUINT16 min_param_range2, 
                                                MOBLEUINT16 max_param_range2,
                                                MOBLEINT16 min_param_range3, 
                                                MOBLEUINT16 max_param_range3,
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
MOBLE_RESULT Chk_TwoParamValidity(MOBLEUINT16 min_param_range1, 
                                  MOBLEUINT16 max_param_range1,
                                  MOBLEUINT16 min_param_range2, 
                                  MOBLEUINT16 max_param_range2,
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
MOBLE_RESULT Chk_RangeValidity(MOBLEUINT16 min_param_value, 
                                const MOBLEUINT8* param,
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
MOBLE_RESULT Chk_HslRangeValidity(const MOBLEUINT8* param,
                                  MOBLEUINT16 min_param_value_1,
                                  MOBLEUINT16 max_param_value_1,
                                  MOBLEUINT16 min_param_value_2,
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
MOBLE_RESULT Chk_OptionalParamValidity(MOBLEUINT8 param_length,
                                       MOBLEUINT8 mandatory_length,
                                       MOBLEUINT8 param, 
                                       MOBLEUINT8 max_param_value  )
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
MOBLE_RESULT Chk_ParamMinMaxIntValidity(MOBLEINT16 min_param_value,
                                        const MOBLEUINT8* param,
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
MOBLE_RESULT Chk_ParamMinMaxValidity(MOBLEUINT16 min_param_value,
                                     const MOBLEUINT8* param,
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


/**
  * @brief  Get remaining time in format as defined for Generic Default Transition Time
  *         If remaining time > max possible value (620 min), DTTF is 0xFE
  * @param  Remaining time in milliseconds
  * @param  Reference to time in Default Transition Time Format
  * @retval Success if remaining time < max possible value else Fail
  */
MOBLE_RESULT TimeDttFGet(MOBLEUINT32 timeMs, MOBLEUINT8* timeDttF)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 stepRes = 0;
  MOBLEUINT8 noOfSteps = 0;
  
  if (timeMs <= 6200) /* Max possible with resolution of 100ms */
  {
    stepRes = 0x00;
    noOfSteps = (timeMs+50)/100;
  }
  else if (timeMs <= 62000) /* Max possible with resolution of 1s */
  {
    stepRes = 0x01;
    noOfSteps = (timeMs+500)/1000;
  }
  else if (timeMs <= 620000) /* Max possible with resolution of 10s */
  {
    stepRes = 0x02;
    noOfSteps = (timeMs+5000)/10000;
  }
  else if (timeMs <= 37200000)/* Max possible with resolution of 10m */
  {
    stepRes = 0x03;
    noOfSteps = (timeMs+300000)/600000;
  }
  else
  {
    /* Remaining time > max possible
       This may happen in case delay is added to max possible transition time */
    stepRes = 0x03;
    noOfSteps = 0x3E; /* set maximum possible value */
    result = MOBLE_RESULT_FALSE;
  }
  
  *timeDttF = noOfSteps | (stepRes << 6);
  
  /* TRACE_M(TF_COMMON, "Raw time %dms, Default transition format time %x\r\n", timeMs, *timeDttF); */
  
  return result;    
}


/**
* @brief  Get actual value in milliseconds from default transition step resolution 
* @param  time_param: intger value received for transition time
* @retval stepResolution 
 */                                         
MOBLEUINT32 Get_StepResolutionValue(MOBLEUINT8 time_param)
{
  MOBLEUINT32 stepResolution = 0;
  
  if(time_param == STEP_HEX_VALUE_0)
  {
    stepResolution = STEP_RESOLUTION_100MS;   
  }
  else if(time_param == STEP_HEX_VALUE_1)
  {
    stepResolution = STEP_RESOLUTION_1S;   
  }
  else if(time_param == STEP_HEX_VALUE_2)
  {
    stepResolution = STEP_RESOLUTION_10S;   
  }
  else if(time_param == STEP_HEX_VALUE_3)
  {
    stepResolution = STEP_RESOLUTION_10M;
  }
  else
  {
    stepResolution = 0;   
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


/**
  * @brief  
  * @param  
  * @retval 
*/
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
MOBLE_RESULT SaveModelsStateNvm(MOBLEUINT8* flag)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  MOBLEUINT16 saveBuffSize = APP_NVM_MODEL_SIZE;
  MOBLEUINT8 Model_GetBuff[APP_NVM_MODEL_SIZE];
//  uint16_t my_Address;
//  MOBLEUINT16 dstPeer;
  MOBLEUINT16 elementParamOffset;
//  MOBLEUINT16 PresentLightness;
//  MOBLEUINT16 TargetLightness;
//  MODEL_MessageHeader_t msgParam;
  
//  my_Address = BLEMesh_GetAddress(); /* Get the Primary address */
  memset(Model_GetBuff, 0x00, saveBuffSize);  /* Initialize the buffer to 0 */
  
  for(uint8_t elementIndex = 0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++)
  {
//    dstPeer = my_Address+elementIndex;
    elementParamOffset =  elementIndex * APP_NVM_MODELDATA_PER_ELEMENT_SIZE; 
	
    Model_GetBuff[elementParamOffset+GENERIC_VALID_FLAG_OFFSET] = flag[elementIndex];
    
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF   
    (Appli_GenericState_cb.GetOnOffStatus_cb)(&Model_GetBuff[elementParamOffset+GENERIC_ON_OFF_NVM_OFFSET],
//                                              dstPeer,
                                              BLEMesh_GetAddress() + elementIndex,
                                              elementIndex);
#endif  
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL	
    (Appli_GenericState_cb.GetLevelStatus_cb)(&Model_GetBuff[elementParamOffset+GENERIC_LEVEL_NVM_OFFSET],
//                                              dstPeer,
                                              BLEMesh_GetAddress() + elementIndex,
                                              elementIndex);
#endif 
  
#ifdef ENABLE_GENERIC_MODEL_SERVER_POWER_ONOFF   
    (Appli_GenericState_cb.GetPowerOnOffStatus_cb)(&Model_GetBuff[elementParamOffset+GENERIC_POWER_ON_OFF_NVM_OFFSET],
//                                                 dstPeer,
                                                   BLEMesh_GetAddress() + elementIndex,
                                                   elementIndex);
#endif  
  

#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS	
        
    if(Light_ModelFlag[elementIndex].LightTransitionFlag)
    {
      MODEL_MessageHeader_t msgParam;
      MOBLEUINT32 dataLength; 
//      MOBLEUINT16 PresentLightness;
//      MOBLEUINT16 TargetLightness;
      uint16_t my_Address = BLEMesh_GetAddress(); /* Get the Primary address */;
      
      /* Initialize the messageParam*/
      msgParam.dst_peer = my_Address+elementIndex;
      msgParam.peer_addr = my_Address;
      msgParam.elementIndex = elementIndex;
      msgParam.rcvdAppKeyOffset = 0;
      msgParam.rcvdNetKeyOffset = 0;
      msgParam.rssi = 0;
      msgParam.ttl = 0;
  
      Light_Lightness_Status(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET],
                             &dataLength, &msgParam); 
//      PresentLightness = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET]));
//      TargetLightness = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_TARGET_NVM_OFFSET]));
      TRACE_M(TF_COMMON,"PresentLightness = 0x%.2x, TrgtLightness = 0x%.2x \r\n", 
              CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET])), 
              CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_TARGET_NVM_OFFSET])));
    }
    
    else
    {
//      MOBLEUINT16 PresentLightness;

      (Appli_Light_GetStatus_cb.GetLightLightness_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET],
//                                                    dstPeer,
                                                      BLEMesh_GetAddress() + elementIndex,
                                                      elementIndex);
//       PresentLightness = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET]));
       TRACE_M(TF_COMMON,"PresentLightness = 0x%.2x \r\n", 
               CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET])));
    }
    
    
    
    (Appli_Light_GetStatus_cb.GetLightLightnessDefault_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET],
//                                                         dstPeer,
                                                           BLEMesh_GetAddress() + elementIndex,
                                                           elementIndex);
    
    (Appli_Light_GetStatus_cb.GetLightLightnessLast_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET],
//                                                      dstPeer,
                                                        BLEMesh_GetAddress() + elementIndex,
                                                        elementIndex);
     
     /*3.1.4 : If a transition was in progress when powered down, the element restores the 
     target state when powered up. Otherwise the element restores the state it was in when 
     powered down. Saving the Transition Flag Status*/
     Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_TRANSITION_STATUS] = Light_ModelFlag[elementIndex].LightTransitionFlag;
     TRACE_M(TF_COMMON,"Transition Flag is = %.2x \r\n", Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_TRANSITION_STATUS]);
 
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL    
         
    /* Check if the transition is in process - Save the target value to Restore from the Target value */
    if(Light_ModelFlag[elementIndex].LightTransitionFlag)
    {
      MOBLEUINT16 dstPeer = BLEMesh_GetAddress() + elementIndex;

      /* Getting Target paramters to save in NVM for the PowerUp state handing */
      (Appli_Light_GetStatus_cb.GetLightCtlTrgt_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_CTL_NVM_OFFSET],
                                                    dstPeer, 
                                                    elementIndex);
       (Appli_Light_GetStatus_cb.GetLightCtlDefault_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_CTL_DEFAULT_NVM_OFFSET],
                                                        dstPeer, 
                                                        elementIndex);
    }
    else
    {
      MOBLEUINT16 dstPeer = BLEMesh_GetAddress() + elementIndex;

      /* Getting set paramters to save in NVM for the PowerUp state handing */
      (Appli_Light_GetStatus_cb.GetLightCtl_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_CTL_NVM_OFFSET],
                                                dstPeer, 
                                                elementIndex);
       (Appli_Light_GetStatus_cb.GetLightCtlDefault_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_CTL_DEFAULT_NVM_OFFSET],
                                                        dstPeer, 
                                                        elementIndex);   
    }
    
#endif
  
#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL  
         
   /* Check if the transition is in process - Save the target value to Restore from the Target value */
   if(Light_ModelFlag[elementIndex].LightTransitionFlag)
   {
      MOBLEUINT16 dstPeer = BLEMesh_GetAddress() + elementIndex;

     /* Getting Target paramters to save in NVM for the PowerUp state handing */
     (Appli_Light_GetStatus_cb.GetLightHslTrgt_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_HSL_NVM_OFFSET],
                                                   dstPeer, 
                                                   elementIndex);
      
      (Appli_Light_GetStatus_cb.GetLightHslDefault_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_HSL_DEFAULT_NVM_OFFSET],
                                                       dstPeer, 
                                                       elementIndex);
   }
   else
   {
      MOBLEUINT16 dstPeer = BLEMesh_GetAddress() + elementIndex;

     /* Getting set paramters to save in NVM for the PowerUp state handing */
     (Appli_Light_GetStatus_cb.GetLightHsl_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_HSL_NVM_OFFSET],
                                               dstPeer, 
                                               elementIndex);
      
      (Appli_Light_GetStatus_cb.GetLightHslDefault_cb)(&Model_GetBuff[elementParamOffset+GENERIC_DATA_LIMIT+LIGHT_HSL_DEFAULT_NVM_OFFSET],
                                                       dstPeer, 
                                                       elementIndex);   
   }
  
#endif  
         
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
         
    MOBLEUINT8 lightLcNvmParamsBuff[APP_NVM_LIGHT_LC_MODEL_SIZE];
    MOBLEUINT8 lcNvmParamsSize;
    
    /* only for element index for which LC is supported */
//    if((ENABLE_LIGHT_MODEL_SERVER_LC & (1 << elementIndex)) == (1 << elementIndex))
    {
    Light_LC_NvmParams_Get(elementIndex, lightLcNvmParamsBuff, &lcNvmParamsSize);
    
    if (lcNvmParamsSize <= APP_NVM_LIGHT_LC_MODEL_SIZE)
    {
      memcpy(&Model_GetBuff[elementParamOffset+LIGHT_LC_MODE_NVM_OFFSET],
             lightLcNvmParamsBuff,
             lcNvmParamsSize);
    }
    else
    {
      /* length mismatch */
  }
    }
    
    break; /* Only for element index 0 */
#endif /* ENABLE_LIGHT_MODEL_SERVER_LC */
  } /* for */
  
  if (SaveModelState_cb != NULL)
  {
    result = SaveModelState_cb(Model_GetBuff, saveBuffSize);
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

#if 0
void Device_FlashTesting(void)
{
  MOBLEUINT8 SaveBuffer[TESTING_BYTE];	
  MOBLEUINT8 RetrieveBuffer[TESTING_BYTE];
  MOBLEUINT16 buffSize = TESTING_BYTE;
  
  MOBLEUINT8 ret;
	
  SaveModelTestState_cb(SaveBuffer,buffSize);
	
  RetrieveModelTestState_cb(RetrieveBuffer,buffSize);
	
  ret = memcmp(SaveBuffer, RetrieveBuffer, APP_NVM_MODEL_SIZE);
  
  if(ret == MOBLE_RESULT_SUCCESS)
  {
    TRACE_M(TF_MISC, "Successfully retrieved the saved data from flash \r\n");
  }
  else
  {
    TRACE_M(TF_MISC, "Failed to retrieved the saved data from flash \r\n");
  }
}
#endif

/**
* @brief function to call light middle layer function for restoration of 
*        saved states.
* @param void.
* return void.
*/
void Model_RestoreStates(MOBLEUINT8 const *pModelState_Load, MOBLEUINT8 size)
{ 
  MOBLEUINT8 pData[2];
  MOBLE_ADDRESS publishAddress;
  MOBLEUINT16 model_ID = 0;
  MOBLEUINT16 opcode = 0;
  MOBLEUINT32 length = 0;
  MOBLE_ADDRESS my_Address;
  MOBLEUINT16 dstPeer;
  MOBLEUINT16 elementParamOffset;
  MOBLEUINT16 powerOnOffOffset;
  MODEL_MessageHeader_t msgParam;
  
  my_Address = BLEMesh_GetAddress(); /* Get the Primary address */
  
  if (size > 0)
  {
    for(uint8_t elementIndex = 0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++)
    {
      dstPeer = my_Address+elementIndex;
    
      /* Initialize the messageParam */
      msgParam.dst_peer = dstPeer;
      msgParam.peer_addr = dstPeer;
      msgParam.elementIndex = elementIndex;
      msgParam.rcvdAppKeyOffset = 0;
      msgParam.rcvdNetKeyOffset = 0;
      msgParam.rssi = 0;
      msgParam.ttl = 0;
      
      elementParamOffset =  elementIndex * APP_NVM_MODELDATA_PER_ELEMENT_SIZE; 
      
      powerOnOffOffset = (elementIndex * APP_NVM_MODELDATA_PER_ELEMENT_SIZE)+ GENERIC_POWER_ON_OFF_NVM_OFFSET;
			    
      switch(pModelState_Load[elementParamOffset + GENERIC_VALID_FLAG_OFFSET])
      { 
        
#ifdef ENABLE_GENERIC_MODEL_SERVER_ONOFF      
        case GENERIC_ON_OFF_NVM_FLAG:
        {
          /* checking the Power on off retrieved value according to the given
             in standered and taking decision for Generic on off.
          */
             
          if(pModelState_Load[powerOnOffOffset] == GENERIC_POWER_OFF_STATE)
          {
            pData[0] = APPLI_LED_OFF;
            Generic_OnOff_Set(pData,1,&msgParam);
          }
          else if(pModelState_Load[powerOnOffOffset] == GENERIC_POWER_ON_STATE)
          {
            pData[0] = APPLI_LED_ON;
            Generic_OnOff_Set(pData,1,&msgParam);
              
          }
          else if(pModelState_Load[powerOnOffOffset] == GENERIC_POWER_RESTORE_STATE)
          {
            Generic_OnOff_Set(pModelState_Load+GENERIC_ON_OFF_NVM_OFFSET+elementParamOffset, 1,&msgParam);
          }
          else
          {
            TRACE_M(TF_GENERIC_M, "Power On Off value invalid %d \r\n", pModelState_Load[0]);
          }         
                                          
          opcode = GENERIC_ON_OFF_SET_UNACK;
          model_ID = GENERIC_ONOFF_SERVER_MODEL_ID;
          break;
        }
#endif

#ifdef ENABLE_GENERIC_MODEL_SERVER_LEVEL        
        case GENERIC_LEVEL_NVM_FLAG:
        {
          Generic_Level_Set(pModelState_Load+GENERIC_LEVEL_NVM_OFFSET,2, &msgParam);
          break;
        }
#endif
        
#ifdef ENABLE_LIGHT_MODEL_SERVER_LIGHTNESS        
        case LIGHT_LIGHTNESS_NVM_FLAG:
        { 
          MOBLEUINT16 light_LightnessValue;
          light_LightnessValue = Light_lightnessPowerOnValue(pModelState_Load, &msgParam);
          *pData = light_LightnessValue;
          *(pData+1) = light_LightnessValue >> 8;
          Light_Lightness_Set(pData, 2, &msgParam);

          opcode = LIGHT_LIGHTNESS_SET_UNACK;
          model_ID = LIGHT_LIGHTNESS_SERVER_MODEL_ID;
          break;
        } 
#endif 
        
#ifdef ENABLE_LIGHT_MODEL_SERVER_CTL         
        case LIGHT_CTL_NVM_FLAG:
        {  
          Light_CtlPowerOnValue(pModelState_Load, &msgParam);
          opcode = LIGHT_CTL_TEMPERATURE_SET_UNACK;
          model_ID = LIGHT_CTL_TEMPERATURE_SERVER_MODEL_ID;
          break;
        }
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_HSL        
        case LIGHT_HSL_NVM_FLAG:
        {  
          /* restore the light HSL with respect to Power on off value */
          Light_HslPowerOnValue(pModelState_Load, &msgParam);
          opcode = LIGHT_HSL_SET_UNACK;
          model_ID = LIGHT_HSL_SERVER_MODEL_ID;
          break;
        }
#endif    
        
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC        
        case LIGHT_LC_NVM_FLAG:
//        if((ENABLE_LIGHT_MODEL_SERVER_LC & (1 << elementIndex)) == (1 << elementIndex))
        {         
          MOBLEUINT8 genericOnPowerUp;
          MOBLEUINT16 lightDefault;
          MOBLEUINT16 lightLast;
          MOBLEUINT16 lightActualLKV;
          MOBLEUINT8 transitionStatus;
          MOBLEUINT16 temp;
          
          genericOnPowerUp = pModelState_Load[GENERIC_POWER_ON_OFF_NVM_OFFSET];
          
          lightDefault = pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET+1]<<8;
          lightDefault |= pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET];
          
          lightLast = pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET+1]<<8; 
          lightLast |= pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET]; 
          
          transitionStatus = pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_TRANSITION_STATUS];
          if(transitionStatus)
          {
            temp = pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_TARGET_NVM_OFFSET+1]<<8;  
            temp |= pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_TARGET_NVM_OFFSET];   
            lightActualLKV = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&temp));
          }
          else
          {
            temp = pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET+1]<<8;  
            temp |= pModelState_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET];
            lightActualLKV = CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&temp));
          }                   
         
          TRACE_M(TF_COMMON, "Transition Flag %.2x\r\n", transitionStatus);
          
          Light_LC_OnPowerUp(elementIndex,
                             pModelState_Load+elementParamOffset+LIGHT_LC_MODE_NVM_OFFSET, 
                             genericOnPowerUp, 
                             lightDefault, 
                             lightLast, 
                             lightActualLKV);
        }
          
        break;
#endif        
        
        case No_NVM_FLAG:
        {
          TRACE_M(TF_GENERIC_M, "Power OnOff value stored = %d \r\n",pModelState_Load[4]);
          break;
        }
        
        default: 
        {
          TRACE_M(TF_LIGHT_M, "No Saved Data Found \r\n");
          break;
        }
        
      } /* switch(pModelState_Load[GENERIC_VALID_FLAG_OFFSET]) */    
    
      pData[0] = pModelState_Load[powerOnOffOffset];
      Generic_PowerOnOff_Set(pData,1,&msgParam);
     
      my_Address = BLEMesh_GetAddress();
      publishAddress = BLEMesh_GetPublishAddress(elementIndex, model_ID);
      
      if(publishAddress != 0x0000 )
      {
        msgParam.peer_addr = publishAddress;
        msgParam.dst_peer = dstPeer;
      
        Model_SendResponse(&msgParam, opcode, pData,length);
        TRACE_I(TF_COMMON,"Publishing the Power on state to address %.2X \r\n",publishAddress);
      } /* if(publishAddress != 0x0000) */
     
      break; /* Run only once for element index 0 */
    } /* for(uint8_t elementIndex = 0; elementIndex < APPLICATION_NUMBER_OF_ELEMENTS; elementIndex++) */
  } /* if (size > 0) */
}
  

/**
* @brief  Function used to restore the light lighness with respect to Power on off
*         value .
* @param  pModelValue_Load:array of saved data
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLEUINT16
*/
MOBLEUINT16 Light_lightnessPowerOnValue(MOBLEUINT8 const *pModelValue_Load, MODEL_MessageHeader_t *pmsgParam)
{
  MOBLEUINT16 light_DefaultValue;
  MOBLEUINT16 light_LastValue;
  MOBLEUINT8 powerOn_Value;
  MOBLEUINT16 light_Actual = 0;
  MOBLEUINT16 last_Known_Value;
  MOBLEUINT8 transitionStatus;
  
  transitionStatus = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_TRANSITION_STATUS];
  
  light_DefaultValue = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET+1]<<8;
  light_DefaultValue |= pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET];
  
  light_LastValue = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET+1]<<8; 
  light_LastValue |= pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET]; 
  
  if(transitionStatus)
  {
    last_Known_Value = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_TARGET_NVM_OFFSET+1]<<8;  
    last_Known_Value |= pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_TARGET_NVM_OFFSET];   
    TRACE_M(TF_COMMON,"Transition Flag is = %.2x, Last_Known Value = 0x%.2x \r\n",
            transitionStatus, 
            CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&last_Known_Value)));
  }
  else
  {
    last_Known_Value = pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET+1]<<8;  
    last_Known_Value |= pModelValue_Load[GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_PRESENT_NVM_OFFSET];
    TRACE_M(TF_COMMON,"Transition Flag is = %.2x, Last_Known Value = 0x%.2x \r\n",
            transitionStatus, 
            CopyU8LittleEndienArrayToU16word((MOBLEUINT8*)(&last_Known_Value)));
  }
  
  powerOn_Value = *(pModelValue_Load+GENERIC_POWER_ON_OFF_NVM_OFFSET);
  
  Light_LightnessLast_Set((pModelValue_Load+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_LAST_NVM_OFFSET),2, pmsgParam);
  Light_LightnessDefault_Set((pModelValue_Load+GENERIC_DATA_LIMIT+LIGHT_LIGHTNESS_DEFAULT_NVM_OFFSET),2, pmsgParam);
   
  if(powerOn_Value == GENERIC_POWER_OFF_STATE)
  { 
    /* Light Lightness Actual = 0 for value of the Generic OnPowerUp state equal 
       to 0x00 */
    light_Actual = 0x00;
  }
  else if((powerOn_Value == GENERIC_POWER_ON_STATE) && (light_DefaultValue != 0x00))
  {
    /* Light Lightness Actual = Light Lightness Default for value of the Generic 
        OnPowerUp state equal to 0x01 and Light Lightness Default not equal to zero, */
    light_Actual = light_DefaultValue;
  }
  else if((powerOn_Value == GENERIC_POWER_ON_STATE) && (light_DefaultValue == 0x00))
  {
    /* Light Lightness Actual = Light Lightness Last (see Section 6.1.2.3)
       for value of the Generic OnPowerUp state equal to 0x01 and 
       Light Lightness Default equal to zero */
    light_Actual = light_LastValue;
  }
  else if(powerOn_Value == GENERIC_POWER_RESTORE_STATE)
  {
    light_Actual = last_Known_Value;
  }
  else
  {
     
  }

  TRACE_I(TF_COMMON, "LIGHT LIGHTNESS VALUE ON POWER UP %.2x \r\n",light_Actual);
  return light_Actual;
}  


/**
* @brief  Function used to restore the light CTL with respect to Power on off
*         value .
* @param  pModelValue_Load:array of saved data
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLEUINT16
*/
void Light_CtlPowerOnValue(MOBLEUINT8 const *pModelValue_Load, MODEL_MessageHeader_t *pmsgParam)
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
    Light_CtlDefault_Set((pData+LIGHT_DEFAULT_OFFSET), 6,0); 
    Light_CtlTemperature_Set((pData+8), 4, pmsgParam);
  }
  else if(powerOn_Value == GENERIC_POWER_RESTORE_STATE)
  {
    Light_CtlTemperature_Set((pData+2), 4, pmsgParam);
  }
  else
  {
    
  }
}
  

/**
* @brief  Function used to restore the light HSL with respect to Power on off
*         value .
* @param  pModelValue_Load:array of saved data
* @param  *pmsgParam: Pointer to structure of message header for parameters:
*          elementIndex, src, dst addresses, TTL, RSSI, NetKey & ApkmpKey Offset
* @retval MOBLEUINT16
*/
void Light_HslPowerOnValue(MOBLEUINT8 const *pModelValue_Load, MODEL_MessageHeader_t *pmsgParam)
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
    Light_HslDefault_Set((pData+LIGHT_DEFAULT_OFFSET), 6, 0);
    if(powerOn_Value == GENERIC_POWER_OFF_STATE)
    {
        memset((void*)(pData+LIGHT_DEFAULT_OFFSET), 0x00, 2);
  }
    Light_Hsl_Set((pData+LIGHT_DEFAULT_OFFSET), 6, 0);
  }
  /* Check to restore the state during Power Up */
  else if(powerOn_Value == GENERIC_POWER_RESTORE_STATE)
  {
    Light_Hsl_Set(pData, 6, 0);
  }
  else
  {
    TRACE_M(TF_GENERIC_M, "Power On Off value invalid %d \r\n", powerOn_Value);    
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
#endif   /* #ifdef ENABLE_SAVE_MODEL_STATE_NVM */


/**
  * @brief  
  * @param  
  * @retval 
  */
MOBLE_RESULT Chk_TidValidity(MOBLE_ADDRESS peerAddr, MOBLE_ADDRESS dstAddr, MOBLEUINT8 tidValue)
{
  static MOBLEUINT32 Check_time;
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  
  TRACE_M(TF_COMMON, "dst_peer %.2X peer_add %.2X, tid %d\r\n", dstAddr, peerAddr, tidValue);
  
  if(((Clock_Time()- Check_time) <= 6000))
  {
    if((Model_Tid.Peer_Addrs == peerAddr)&&(Model_Tid.Dst_Addrs == dstAddr)&&(Model_Tid.Tid_Value == tidValue))
    {
      TRACE_M(TF_COMMON,"Duplicate Message Parameter within six second \r\n");
      status =  MOBLE_RESULT_INVALIDARG;       
    }
    else
    {
      Check_time = Clock_Time();
      Model_Tid.Peer_Addrs = peerAddr;
      Model_Tid.Dst_Addrs = dstAddr;
      Model_Tid.Tid_Value = tidValue; 
      TRACE_M(TF_COMMON,"New Message Parameter within six second \r\n");
    }
  }
  else
  {
    Check_time = Clock_Time();
    Model_Tid.Peer_Addrs = peerAddr;
    Model_Tid.Dst_Addrs = dstAddr;
    Model_Tid.Tid_Value = tidValue;
    TRACE_M(TF_COMMON,"New Message Parameter after six second\r\n");
  }
  
  return status;
}


/**
  * @brief  Last TID parameters update with current TID parameters if
  *         either 6 seconds has passed or TID mismatch or src & dst mismatch 
  * @param  current TID parameters and reference to last TID parameters
  * @retval If last TID parameters updated return 1 else 0
  */
MOBLEUINT8 Tid_CheckAndUpdate(MOBLEUINT8 currentMsgTid,
                              MOBLE_ADDRESS currentMsgSrc,
                              MOBLE_ADDRESS currentMsgDst,
                              tid_param_t* pLastMsgTidParams)
{
  MOBLEUINT8 updateLastTidParams = 0;
  MOBLEUINT32 lastMsgPlus6Tick = pLastMsgTidParams->tidTick + 6000;
  MOBLEUINT8 lastTidExpired = 0;
    
  if(currentMsgTid != pLastMsgTidParams->tid)
  {
    updateLastTidParams = 1;
  }
  else if(currentMsgSrc != pLastMsgTidParams->src ||
          currentMsgDst != pLastMsgTidParams->dst)
  {
    updateLastTidParams = 1;
  }
  else
  {
    if(pLastMsgTidParams->tidTick <= lastMsgPlus6Tick)
    {
      lastTidExpired = Clock_Time() >= lastMsgPlus6Tick ||
                       Clock_Time() < pLastMsgTidParams->tidTick;
    }
    else
    {
      lastTidExpired = Clock_Time() >= lastMsgPlus6Tick &&
                       Clock_Time() < pLastMsgTidParams->tidTick;
    }
    
    if(lastTidExpired == 1)
    {
      updateLastTidParams = 1;
    }
  }
  
  if(updateLastTidParams == 1)
  {
    pLastMsgTidParams->tid = currentMsgTid;
    pLastMsgTidParams->src = currentMsgSrc;
    pLastMsgTidParams->dst = currentMsgDst;
    pLastMsgTidParams->tidTick = Clock_Time();
  }
  
  return updateLastTidParams;
}


/**
  * @brief  
  * @param  
  * @retval 
*/
void CopyU8LittleEndienArray_fromU16word (MOBLEUINT8* pArray, MOBLEUINT16 inputWord)
{
  *(pArray) = (MOBLEUINT8)(inputWord & 0x00ff);  /* Copy the LSB first */
  *(pArray+1) = (MOBLEUINT8)((inputWord & 0xff00) >> 0x08); /* Copy the MSB later */
}


/**
  * @brief  
  * @param  
  * @retval 
*/
MOBLEUINT16 CopyU8LittleEndienArrayToU16word (MOBLEUINT8* pArray) 
{
  MOBLEUINT16 u16Word=0;
  MOBLEUINT8 lsb_byte=0;
  MOBLEUINT8 msb_byte=0;
  
  lsb_byte = *pArray;
  pArray++;
  msb_byte = *pArray;
  u16Word = (msb_byte<<8);
  u16Word &= 0xFF00;
  u16Word |= lsb_byte;

  return u16Word;
}


/**
  * @brief  
  * @param  
  * @retval 
  */
MOBLEUINT32 CopyU8LittleEndienArrayToU32word (MOBLEUINT8* pArray) 
{
  MOBLEUINT32 u32Word=0;

  u32Word = *(pArray+3); 
  u32Word <<= 8;     
  u32Word |= *(pArray+2); 
  u32Word <<= 8;     
  u32Word |= *(pArray+1); 
  u32Word <<= 8;     
  u32Word |= *pArray;
  return u32Word;
}


/**
  * @brief  
  * @param  
  * @retval 
*/
void CopyU8LittleEndienArray_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord)
{
  *pArray = (MOBLEUINT8)(inputWord & 0x000000ff);  /* Copy the LSB first */
  *(pArray+1) = (MOBLEUINT8)((inputWord & 0x0000ff00) >> 8); /* Copy the MSB later */
  *(pArray+2) = (MOBLEUINT8)((inputWord & 0x00ff0000) >> 16); /* Copy the MSB later */
  *(pArray+3) = (MOBLEUINT8)((inputWord & 0xff000000) >> 24); /* Copy the MSB later */
}


/**
  * @brief  
  * @param  
  * @retval 
*/
void CopyU8LittleEndienArray_2B_fromU32word (MOBLEUINT8* pArray, MOBLEUINT32 inputWord)
{
  *pArray = (MOBLEUINT8)(inputWord & 0x000000ff);  /* Copy the LSB first */
  *(pArray+1) = (MOBLEUINT8)((inputWord & 0x0000ff00) >> 8); /* Copy the MSB later */
}


/**
  * @brief  Fill array from uint32 value in little endian format
  * @param  Array to be filled
  * @param  uint32 value (may have uint16 or uint8 value)
  * @param  No of bytes to be extracted and filled
  * @retval None
  */
void PutLittleEndian(MOBLEUINT8* stream, MOBLEUINT32 value, MOBLEUINT8 octets)
{
  for (MOBLEUINT8 i = 0; i < octets; ++i)
  {
    stream[i] = (MOBLEUINT8)value;
    value >>= 8;
  }
}


/**
  * @brief  Get value from array formatted as little endian
  * @param  Initial array
  * @param  Extracted value
  * @retval None
  */
void GetLittleEndian(MOBLEUINT8 const *stream, MOBLEUINT8 octets, MOBLEUINT32* result)
{
  MOBLEUINT32 data;
  memcpy(&data, stream, sizeof(MOBLEUINT32));
  *result = data & ~((unsigned int)-1 << (octets << 3));
}


/**
* @brief  Update parameters for ModelSaveProcess to save model states
* @param  NVM flag for model
* @retval None
*/
void NvmStatePowerFlag_Set(MOBLEUINT8 nvmModelFlag, MOBLEUINT8 elementIndex)
{
#ifdef SAVE_MODEL_STATE_FOR_ALL_MESSAGES
  PowerOnOff_flag = FLAG_SET;
  RestoreFlag[elementIndex] = nvmModelFlag;
#endif
}


/**
  * @brief  Stops ongoing transition
  * @param  Reference to transition status
  * @retval TRANSITION_EVENT_ABORT if ongoing transition stopped
  *         else TRANSITION_EVENT_NO
  **/
transition_event_e Transition_Stop(transition_status_e* trStatus)
{
  transition_event_e transitionEvent = TRANSITION_EVENT_NO;
  
  if (*trStatus != TRANSITION_STATUS_STOP)
  {
    *trStatus = TRANSITION_STATUS_STOP;
    transitionEvent = TRANSITION_EVENT_ABORT;
    
    /* TRACE_M(TF_COMMON, "Transition aborted\r\n"); */
  }
  
  return transitionEvent;
}


/**
  * @brief  Transition state machine
  *         If transition stopped, transition starts with delay or without delay
  *         If transition in delay state, next action is to move to run state
  *         If in run state, next action can be to generate intermediate triggers
  *         according to step resolution or next action can be to stop ongoing
  *         transition if transition time is elapsed
  *         If transition time > 2 seconds, publish event is generated after
  *         1 second w.r.t. transition run
  *         TRANSITION_EVENT_DELAY can be override by TRANSITION_EVENT_TIMER_START
  *         TRANSITION_EVENT_DELAY can be override by TRANSITION_EVENT_TIMER_STOP
  *         TRANSITION_EVENT_TIMER_START can be override by TRANSITION_EVENT_TIMER_STOP
  *         TRANSITION_EVENT_TIMER_TRIG can be override by TRANSITION_EVENT_TIMER_STOP
  * @param  Reference to transition parameters
  * @param  delay required in milliseconds
  * @retval TRANSITION_EVENT_DELAY if transition started with delay
  *         TRANSITION_EVENT_TIMER_START if transition started without delay or delay elapsed
  *         TRANSITION_EVENT_TIMER_TRIG at every step resolution time elapse
  *         TRANSITION_EVENT_PUBLISH if 1 second elapsed after run started
  *         TRANSITION_EVENT_TIMER_STOP if transition stopped
  *         else TRANSITION_EVENT_NO
  **/
transition_event_e Transition_Sm(transition_params_t* pTrParams,
                                MOBLEUINT32 delayMs)
{
  transition_event_e transitionEvent = TRANSITION_EVENT_NO;
  MOBLEUINT8 triggerStatus = 0;
  MOBLEUINT8 timerOffEvent = 0;
  
  if (pTrParams->trStatus == TRANSITION_STATUS_STOP)
  {
    if(pTrParams->trTimeMs > 2000 &&
       /* optimization to not generate publish event if step resoltuion is comparable to transition time */
       pTrParams->stepResolutionMs < pTrParams->trTimeMs)
  {
      /* Intermediate publish event to be generated after 1 second of start */
      pTrParams->publishEventTrig = 1;
    }
    else
    {
      pTrParams->publishEventTrig = 0;
    }
    
    /* Set begin and end time */
    pTrParams->trBeginTick = Clock_Time();
    pTrParams->trEndTick = pTrParams->trBeginTick + 
                                          delayMs + 
                                          pTrParams->trTimeMs;
    
    /* If not in transition
          Either transition is with delay -> SWITCH TO DELAY
          Or transition is without delay -> SWITCH TO RUNNING */
    if (delayMs != 0)
    {
      /* Switch to delay state */
      pTrParams->trStatus = TRANSITION_STATUS_DELAY;
      transitionEvent = TRANSITION_EVENT_DELAY;
      pTrParams->trNextActionTick = pTrParams->trBeginTick + delayMs;
    }
    else
    {
      /* delay is 0 -> transition starts but no change in value in first iteration */
      pTrParams->trStatus = TRANSITION_STATUS_RUNNING;
      transitionEvent = TRANSITION_EVENT_TIMER_START;
      pTrParams->trNextActionTick = pTrParams->trBeginTick + 
                                                   pTrParams->stepResolutionMs;
    }
  }
  
  if(pTrParams->trBeginTick <= pTrParams->trNextActionTick)
  {
    triggerStatus = Clock_Time() >= pTrParams->trNextActionTick ||
                    Clock_Time() < pTrParams->trBeginTick;
  }
  else /* overflow */
  {
    triggerStatus = Clock_Time() >= pTrParams->trNextActionTick &&
                    Clock_Time() < pTrParams->trBeginTick;
  }
  
  if(triggerStatus == 1) /* next action triggered */
  {
    if(pTrParams->trStatus == TRANSITION_STATUS_DELAY)
    {
      /* Switcht to run state and timer started */
      pTrParams->trStatus = TRANSITION_STATUS_RUNNING;
      transitionEvent = TRANSITION_EVENT_TIMER_START;
    }
    else /* running */
    {
      transitionEvent = TRANSITION_EVENT_TIMER_TRIG;
    }
    
    pTrParams->trNextActionTick = pTrParams->trNextActionTick + 
                                                  pTrParams->stepResolutionMs;
  }
  
  /* check for timer off event
     next action tick is already updated for next iteration
       below check is w.r.t. next iteration itself */
  if (pTrParams->trBeginTick <= pTrParams->trEndTick)
  {
    timerOffEvent = Clock_Time() >= pTrParams->trEndTick ||
                    Clock_Time() < pTrParams->trBeginTick;
  }
  else /* overflow */
  {
    timerOffEvent = Clock_Time() >= pTrParams->trEndTick &&
                    Clock_Time() < pTrParams->trBeginTick;
  }
  
  if(timerOffEvent == 1)
  {
    transitionEvent = TRANSITION_EVENT_TIMER_STOP;
    pTrParams->trStatus = TRANSITION_STATUS_STOP;
  }
  
  if(pTrParams->publishEventTrig == 1 &&
     /* pTrParams->trTimeMs > 2000 && */
     transitionEvent == TRANSITION_EVENT_NO &&
     pTrParams->trStatus == TRANSITION_STATUS_RUNNING)
  {
    /* TRANSITION_EVENT_PUBLISH can't override any other transition event */
    
    if (pTrParams->trBeginTick <= pTrParams->trEndTick)
    {
      triggerStatus = Clock_Time() >= (pTrParams->trEndTick - (pTrParams->trTimeMs -1000)) ||
                      Clock_Time() < pTrParams->trBeginTick;
    }
    else
    {
      if(pTrParams->trEndTick >= (pTrParams->trTimeMs - 1000))
      {
        triggerStatus = Clock_Time() >= (pTrParams->trEndTick-(pTrParams->trTimeMs-1000)) &&
                      Clock_Time() < pTrParams->trBeginTick;
      }
      else
      {
        triggerStatus = Clock_Time() >= ((0xFFFFFFFF-(pTrParams->trTimeMs-1000))+pTrParams->trEndTick) ||
                      Clock_Time() < pTrParams->trBeginTick;
      }
    }
    
    if(triggerStatus == 1) /* status publication */
    {
      pTrParams->publishEventTrig = 0;
      transitionEvent = TRANSITION_EVENT_PUBLISH;
    }
  }
  
  if(transitionEvent == TRANSITION_EVENT_NO)
  {
    /* TRACE_I(TF_COMMON, "Tr sm TRANSITION_EVENT_NO\r\n"); */
  }
  else if(transitionEvent == TRANSITION_EVENT_ABORT)
  {
    /* TRACE_I(TF_COMMON, "Tr sm TRANSITION_EVENT_ABORT\r\n"); */
  }
  else if(transitionEvent == TRANSITION_EVENT_DELAY)
  {
    /* TRACE_I(TF_COMMON, "Tr sm TRANSITION_EVENT_DELAY\r\n"); */
  }
  else if(transitionEvent == TRANSITION_EVENT_TIMER_START)
  {
    /* TRACE_I(TF_COMMON, "Tr sm TRANSITION_EVENT_TIMER_START\r\n"); */
  }
  else if(transitionEvent == TRANSITION_EVENT_TIMER_TRIG)
  {
    /* TRACE_I(TF_COMMON, "Tr sm TRANSITION_EVENT_TIMER_TRIG\r\n"); */
  }
  else if(transitionEvent == TRANSITION_EVENT_PUBLISH)
  {
    /* TRACE_I(TF_COMMON, "Tr sm TRANSITION_EVENT_PUBLISH\r\n"); */
  }
  else if(transitionEvent == TRANSITION_EVENT_TIMER_STOP)
  {
    /* TRACE_I(TF_COMMON, "Tr sm TRANSITION_EVENT_TIMER_STOP\r\n"); */
  }
  
  return transitionEvent;
}


/**
  * @brief  Returns time remaining before transition stops
  * @param  Reference to transition parameters
  * @retval Remaining time in milliseconds
  **/
MOBLEUINT32 Transition_RemainingTimeGet(transition_params_t* pTrParams)
{
  MOBLEUINT32 remainingTimeMs = 0;
  
  if (pTrParams->trStatus == TRANSITION_STATUS_STOP)
  {
    /* Timer is stopped */
  }
  else /* delay or running */
  {
    if(pTrParams->trBeginTick <= pTrParams->trEndTick)
    {
      if(Clock_Time() < pTrParams->trEndTick &&
         Clock_Time() >= pTrParams->trBeginTick)
      {
        remainingTimeMs = pTrParams->trEndTick - Clock_Time();
      }
      else
      {
        remainingTimeMs = 0;
      }
    }
    else
    {
      if(Clock_Time() >= pTrParams->trEndTick &&
         Clock_Time() < pTrParams->trBeginTick)
      {
        remainingTimeMs = 0;
      }
      else
      {
        if(Clock_Time() < pTrParams->trEndTick)
        {
          remainingTimeMs = pTrParams->trEndTick - Clock_Time();
        }
        else
        {
          remainingTimeMs = pTrParams->trEndTick + (0xFFFFFFFF - Clock_Time());
        }
      }
    }
  }
  
  /* TRACE_I(TF_COMMON, "Tr remaining time %dms\r\n", remainingTimeMs); */
  return remainingTimeMs;
}


/**
  * @brief  Returns time before transition stops w.r.t. transition time
  * @param  Reference to transition parameters
  * @retval timer in milliseconds
  **/
MOBLEUINT32 Transition_TimerGet(transition_params_t* pTrParams)
{
  MOBLEUINT32 timerMs = 0;
  
  if(pTrParams->trStatus == TRANSITION_STATUS_STOP)
  {
    /* Timer is stopped */
  }
  else if(pTrParams->trStatus == TRANSITION_STATUS_DELAY)
  {
    timerMs = pTrParams->trTimeMs;
  }
  else /* running */
  {
    if(pTrParams->trBeginTick <= pTrParams->trEndTick)
    {
      if(Clock_Time() < pTrParams->trEndTick &&
         Clock_Time() >= pTrParams->trBeginTick)
      {
        timerMs = pTrParams->trEndTick - Clock_Time();
      }
      else
      {
        timerMs = 0;
      }
    }
    else
    {
      if(Clock_Time() >= pTrParams->trEndTick &&
         Clock_Time() < pTrParams->trBeginTick)
      {
        timerMs = 0;
      }
      else
      {
        if(Clock_Time() < pTrParams->trEndTick)
        {
          timerMs = pTrParams->trEndTick - Clock_Time();
        }
        else
        {
          timerMs = pTrParams->trEndTick + (0xFFFFFFFF - Clock_Time());
        }
      }
    }
  }
  
  return timerMs;
}


/**
  * @brief  Returns time to next scheduled tick
  *         May be used for low power remaining time calculation
  *         TRANSITION_EVENT_PUBLISH is not in consideration
  * @param  Reference to transition parameters
  * @retval time in milliseconds
  **/
MOBLEUINT32 Transition_TimeToNextActionGet(transition_params_t* pTrParams)
{
  MOBLEUINT32 timerMs = 0xFFFFFFFF;
  
  if (pTrParams->trStatus == TRANSITION_STATUS_STOP)
  {
    /* Timer is stopped */
  }
  else /* delay or running */
  {
    if(pTrParams->trBeginTick <= pTrParams->trNextActionTick)
    {
      if(Clock_Time() < pTrParams->trNextActionTick &&
         Clock_Time() >= pTrParams->trBeginTick)
      {
        timerMs = pTrParams->trNextActionTick - Clock_Time();
      }
      else
      {
        timerMs = 0;
      }
    }
    else
    {
      if(Clock_Time() >= pTrParams->trNextActionTick &&
         Clock_Time() < pTrParams->trBeginTick)
      {
        timerMs = 0;
      }
      else
      {
        if(Clock_Time() < pTrParams->trNextActionTick)
        {
          timerMs = pTrParams->trNextActionTick - Clock_Time();
        }
        else
        {
          timerMs = pTrParams->trNextActionTick + (0xFFFFFFFF - Clock_Time());
        }
      }
    }
  }
  
  return timerMs;
}


/**
  * @brief  Returns intermediate state during transition
  *         Intermediate state is based on initial & final state and timer
  * @param  Final state
  * @param  Initial state
  * @param  Timer (starts with transition time and decrements to 0)
  * @retval Intermediate calculated state
  **/
MOBLEUINT32 Transition_StateValueGet(MOBLEUINT32 finalState, 
                                    MOBLEUINT32 initialState, 
                                    MOBLEUINT32 timer, 
                                    MOBLEUINT32 transitionTime)
{
  MOBLEUINT32 transitionStateVal = 0;
  
  if(transitionTime != 0)
  {
    transitionStateVal = (finalState*(transitionTime - timer) + initialState*timer)/\
                          transitionTime;
  }
  else
  {
    if(finalState == initialState)
    {
      transitionStateVal = finalState;
    }
    else
    {
      /* Undefined */
    }
  }
  
  return transitionStateVal;
}


/**
  * @brief  ExtractPropertyId
  *         Extract 16 bit Property ID from buffer
  * @param  reference to data to be used for extracting Property ID
  * @param  reference to be updated with Property ID
  * @retval Success if Property ID is not prohibited 
  *         else Invalid
  */
MOBLE_RESULT ExtractPropertyId(const MOBLEUINT8* data,
                               MOBLEUINT16* pPropertyId)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 propertyId = (*data | (*(data+1)<<8));
  
  if (propertyId == PROPERTY_ID_PROHIBITED)
  {
    TRACE_M(TF_COMMON, "Prohibited Property Id received\r\n");
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    *pPropertyId = propertyId;
  }
  
  return result;
}


/**
  * @brief  
  * @param  
  * @retval 
  */
MOBLE_RESULT Binding_GenericOnOff_LightLcLightOnOff(MOBLEUINT8 genericElementIndex, 
                                                    MOBLEUINT8 genericOnOff,
                                                    MOBLEUINT8 optionalParams,
                                                    MOBLEUINT32 delayMs,
                                                      MOBLEUINT8 transitionParam)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* optional params to be inserted */
  Light_LC_LcOnOffUpdate(genericElementIndex, genericOnOff, 0, 0, 0, 0);
  
  return result;
}


/**
  * @brief  
  * @param  
  * @retval 
  */
MOBLE_RESULT Binding_LightLcLightOnOff_GenericOnOff(MOBLEUINT8 lcElementIndex, MOBLEUINT8 lcOnOff)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  Generic_OnOffUpdate(lcElementIndex, lcOnOff);
  
  return result;
}


/**
  * @brief  
  * @param  
  * @retval 
  */
MOBLE_RESULT Binding_LcLinearOut_LightLightnessLinear(MOBLEUINT8 lcElementIndex, MOBLEUINT16 lcLinearOut)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* Extract element index of target light lightness linear
     should not be same element index as of lcElementIndex */
  MOBLEUINT8 targetElementIndex = 0;
  
  if(lcElementIndex == targetElementIndex)
  {
    TRACE_M(TF_COMMON, "Element index conflict\r\n");
  }
  
  Light_LightnessLinearUpdate(targetElementIndex, lcLinearOut);
  
  return result;
}


/**
  * @brief  Disable binding between LC Linear Out and Light Lightness Server
  *         upon an unsolicited change in binded Light Lightness Linear
  *         Extract target LC Server element index
  * @param  element index of Light Lightness Server
  * @retval None
  */
void BindingDisable_LcLinearOut_LightLightnessLinear(MOBLEUINT8 lightnessLinearElementIndex)
{
  MOBLEUINT8 targetElementIndex = 1; /* Element index of targeted LC server */
  Light_LC_LigtnessLinearUnsolicitedChange(targetElementIndex);
}


WEAK_FUNCTION ( MOBLE_RESULT ApplicationGetConfigServerDeviceKey(MOBLE_ADDRESS src, 
                                                 const MOBLEUINT8 **ppkeyTbUse))
{
  return MOBLE_RESULT_SUCCESS;
}


/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/

