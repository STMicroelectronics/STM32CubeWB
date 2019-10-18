/**
  ******************************************************************************
  * @file    appli_vendor.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Vendor Mesh Models 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "ble.h"
#include "hal_common.h"
#include "types.h"
#include "ble_mesh.h"
#include "appli_mesh.h"
#include "vendor.h"
#include "appli_vendor.h"
#include "common.h"
#include "appli_light.h"
#include "models_if.h"
#include "mesh_cfg.h"
#include <string.h>


/** @addtogroup MODEL_VENDOR
*  @{
*/

/** @addtogroup Vendor_Model_Callbacks
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

MOBLEUINT8 ResponseBuffer[8];
MOBLEUINT8 BuffLength;
extern MOBLEUINT8 Appli_LedState;
extern uint16_t DUTY;
MOBLEUINT32 TestHitCounter = 0;
extern Appli_LightPwmValue_t Appli_LightPwmValue;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  Process the Vendor Device Info Command
* @param  data: Pointer to the data received from peer_addr
* @param  length: Length of the data
* @retval MOBLE_RESULT status of result
*/          
MOBLE_RESULT Appli_Vendor_DeviceInfo(MOBLEUINT8 const *data, MOBLEUINT32 length)
{
  MOBLEUINT8 tempBuffer[10];
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;  
  
  MOBLEUINT8 subCmd = data[0];
  char *libVersion;
  char *subLibVersion;
  MOBLEUINT8 inc = 0;
        /*First Byte is sending the Sub Command*/      
  ResponseBuffer[0] = subCmd;
  
  switch(subCmd)
  {
  case IC_TYPE:
    {
#ifdef BLUENRG1_DEVICE           
      ResponseBuffer[1] = BLUENRG1_BRD;              
#endif
      
#ifdef BLUENRG2_DEVICE              
      ResponseBuffer[1] = BLUENRG2_BRD;              
#endif
      
#ifdef BLUENRG_MS    
      ResponseBuffer[1] = BLUENRG_MS_BRD;               
#endif
      
#ifdef STM32WB55xx
      ResponseBuffer[1] = STM32WB55XX_BRD;               
#endif      

      BuffLength = 2;
      break;
    }
  case LIB_VER:
    {
      libVersion = BLEMesh_GetLibraryVersion();
      while(*libVersion != '\0')
      {
        tempBuffer[inc] = *libVersion;
        libVersion++;  
              TRACE_M(TF_VENDOR,"Lib version is %x\n\r" ,(unsigned char)tempBuffer[inc]); 
        inc++;
      } 
      ResponseBuffer[1]= BLEMesh_ModelsASCII_To_Char(tempBuffer[0]);
      ResponseBuffer[2]= BLEMesh_ModelsASCII_To_Char(tempBuffer[1]);
      ResponseBuffer[3]= BLEMesh_ModelsASCII_To_Char(tempBuffer[3]);
      ResponseBuffer[4]= BLEMesh_ModelsASCII_To_Char(tempBuffer[4]);
      ResponseBuffer[5]= BLEMesh_ModelsASCII_To_Char(tempBuffer[6]);
      ResponseBuffer[6]= BLEMesh_ModelsASCII_To_Char(tempBuffer[7]);
      ResponseBuffer[7]= BLEMesh_ModelsASCII_To_Char(tempBuffer[8]);
      BuffLength = 8;      
      break;
    }
  case LIB_SUB_VER:
    {
      subLibVersion = BLEMesh_GetLibrarySubVersion();
      while(*subLibVersion != '\0')
      {
        tempBuffer[inc] = * subLibVersion;
        subLibVersion++;  
              TRACE_M(TF_VENDOR," Lib Sub Version is %x\n\r" ,(unsigned char)tempBuffer[inc]);           
        inc++;
      } 
      ResponseBuffer[1]= BLEMesh_ModelsASCII_To_Char(tempBuffer[0]);
      ResponseBuffer[2]= BLEMesh_ModelsASCII_To_Char(tempBuffer[1]);
      ResponseBuffer[3]= BLEMesh_ModelsASCII_To_Char(tempBuffer[3]);
      ResponseBuffer[4]= BLEMesh_ModelsASCII_To_Char(tempBuffer[5]);
      ResponseBuffer[5]= BLEMesh_ModelsASCII_To_Char(tempBuffer[7]);
      ResponseBuffer[6]= tempBuffer[9];
      
      BuffLength = 7;
      
      break;
    }
  case APPLICATION_VER:
    {
      /*Insert Command to check Application Version*/
      break;
    }
    
  default:
    {
      status = MOBLE_RESULT_FALSE;
      break;
    }
    
  }
  
  return status;
}


/**
* @brief  Process the Vendor Test Command 
* @param  data: Pointer to the data received from peer_addr
* @param  length: Length of the data
* @retval MOBLE_RESULT status of result
*/          
MOBLE_RESULT Appli_Vendor_Test(MOBLEUINT8 const *data, MOBLEUINT32 length)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;  
  MOBLEUINT8 subCmd = data[0];
       /*First Byte is sending the Sub Command*/      
       ResponseBuffer[0]=subCmd;

  switch(subCmd)
  {             
  case APPLI_TEST_ECHO: 
    {
      memcpy ((ResponseBuffer+1),(data+1),length);
      BuffLength = length;
      break;
    }
  case APPLI_TEST_RANDOMIZATION_RANGE:  
    {
      /*Insert Test related Commands here*/
      break;
    }
  case APPLI_TEST_COUNTER:
    {
      if((DUTY <= PWM_TIME_PERIOD) && (DUTY > 1))
      {
        Appli_LightPwmValue.IntensityValue = LED_OFF_VALUE;
        Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
      }
      else
      {
        Appli_LightPwmValue.IntensityValue = PWM_TIME_PERIOD;
       Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
      }
        TRACE_M(TF_VENDOR,"Test Counter is running \r\n");
        ResponseBuffer[0] = subCmd;
        ResponseBuffer[1] = Appli_LedState ;
        BuffLength = 2; 
      /*Insert Test related Commands here*/
      break;
    }
  case APPLI_TEST_INC_COUNTER: 
    {
                
      if((DUTY <= PWM_TIME_PERIOD) && (DUTY > 1))
      {
        Appli_LightPwmValue.IntensityValue = LED_OFF_VALUE;
        Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
      }
      else
      {
        Appli_LightPwmValue.IntensityValue = PWM_TIME_PERIOD;
        Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
      }                                                    
              
      TestHitCounter++;              
      TRACE_M(TF_VENDOR,"Command received Count %.2x \r\n",TestHitCounter);
      ResponseBuffer[0] = subCmd;
      ResponseBuffer[1] = Appli_LedState ;
      BuffLength = 2;
      /*Insert Test related Commands here*/
      break;
    }
  case APPLI_MODEL_PUBLISH_SELECT:
    {
                
      break;
    }
  default:
    {
      status = MOBLE_RESULT_FALSE;
      break;
    }
  }
       
  return status;
}


/**
* @brief  Process the Vendor LED Control Command
* @param  data: Pointer to the data received from peer_addr
* @param  length: Length of the data
* @param  elementNumber : element selected for a node        
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_Vendor_LEDControl( MOBLEUINT8 const *data, MOBLEUINT32 length,
                                       MOBLEUINT8 elementNumber , MOBLE_ADDRESS dst_peer)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 subCommand; 
  subCommand = data[0];
  MOBLEUINT16 duty;
  MOBLEUINT16 intensityValue = 0;
/*  tClockTime delay_t = Clock_Time(); */
  
  switch(subCommand)
  {
    /* 
    Message Received     B0     B1    B2      B3    B4    B5    B6     B7 
    B0 - Sub-Cmd LED
    B1-B7 - Data Bytes       
    */
  case APPLI_CMD_LED_BULB:
    {
      if(elementNumber == FIRST_ELEMENT)
      {
        Appli_LedState = *(data+1); /* Toggle the state of the Blue LED */
      }
      else if(elementNumber == SECOND_ELEMENT)
      {
        /* user application code */
      }
      else if(elementNumber == THIRD_ELEMENT)
      {
        /* user application code */
      }
      break;
    }
    
    /* Toggle Command */  
  case APPLI_CMD_TOGGLE:
    {
      if(elementNumber == FIRST_ELEMENT)
      {
        if((Appli_LightPwmValue.IntensityValue <= PWM_TIME_PERIOD) && (Appli_LightPwmValue.IntensityValue > 1))
        {
          Appli_LightPwmValue.IntensityValue = LED_OFF_VALUE;
          Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
          BSP_LED_Off(LED_BLUE);
        }
        else
        {
          Appli_LightPwmValue.IntensityValue = PWM_TIME_PERIOD;
          Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);
          BSP_LED_On(LED_BLUE);
        }
              
      }
      else if(elementNumber == SECOND_ELEMENT)
      {
        /* user application code */
      }
      else if(elementNumber == THIRD_ELEMENT)
      {
        /* user application code */
      }
      break;
    }
    /* On Command */  
  case APPLI_CMD_ON:
    {
      if(elementNumber == FIRST_ELEMENT)
      {
        Appli_LightPwmValue.IntensityValue = PWM_TIME_PERIOD;
        Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);   /* PWM_ID = PWM4, mapped on PWM4_PIN (GPIO_14 in mapping) */
        BSP_LED_On(LED_BLUE);
        Appli_LedState = 1;
      }
      else if(elementNumber == SECOND_ELEMENT)
      {
        Appli_LightPwmValue.IntensityValue = PWM_TIME_PERIOD;
        Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue); /* PWM_ID = PWM3, mapped on PWM3_PIN (GPIO_2 in mapping) */
        BSP_LED_On(LED_BLUE);
        Appli_LedState = 1;
      }
      else if(elementNumber == THIRD_ELEMENT)
      {
        /* user application code */
      }
      break;
    }
    /* Off Command */  
  case APPLI_CMD_OFF:
    {
      if(elementNumber == FIRST_ELEMENT)
      {
        Appli_LightPwmValue.IntensityValue = LED_OFF_VALUE;
        Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);   /* PWM_ID = PWM4, mapped on PWM4_PIN (GPIO_14 in mapping) */
        Appli_LedState = 0;
        BSP_LED_Off(LED_BLUE);
      }
      else if(elementNumber == SECOND_ELEMENT)
      {        
        Appli_LightPwmValue.IntensityValue = LED_OFF_VALUE;
        Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue); /* PWM_ID = PWM3, mapped on PWM3_PIN (GPIO_2 in mapping) */
        Appli_LedState = 0;
        BSP_LED_Off(LED_BLUE);
      }
      else if(elementNumber == THIRD_ELEMENT)
      {
        /* user application code */
      }
          
      break;
    }
        /* intensity command */
    case APPLI_CMD_LED_INTENSITY:
      {
        if(elementNumber == FIRST_ELEMENT)
        {
          intensityValue = data[2] << 8;
          intensityValue |= data[1];
                    
          duty = PwmValueMapping(intensityValue , 0x7FFF ,0);                         
          Appli_LightPwmValue.IntensityValue = duty;
          Light_UpdateLedValue(LOAD_STATE , Appli_LightPwmValue);             
          if(duty > 16000)
          {
            BSP_LED_On(LED_BLUE);
          }
          else
          {
            BSP_LED_Off(LED_BLUE);
          }
        }
        else if(elementNumber == SECOND_ELEMENT)
        {
           /* user application code */
        }
        else if(elementNumber == THIRD_ELEMENT)
        {
          /* user application code */
        }
      break;
    }
    /* Default case - Not valid command */
  default:
    {
      status = MOBLE_RESULT_FALSE;
      break;
    }
  }
  /*Buffer will be sent for Reliable Response*/
  /*First Byte is Sub Command and 2nd Byte is LED Status*/
  ResponseBuffer[0] = subCommand;
  if(subCommand == APPLI_CMD_LED_INTENSITY)
  {
    ResponseBuffer[1] = intensityValue >> 8 ;
    ResponseBuffer[2] = intensityValue ;
    BuffLength = 3;
  }
  else
  {
    ResponseBuffer[1] = Appli_LedState ;
    BuffLength = 2; 
  }
      
  return status;
}

void Appli_GetTestValue (MOBLEUINT8 *responseValue)
{
  *responseValue = TestHitCounter;
  *(responseValue+1)  = TestHitCounter >> 8;
  *(responseValue+2)  = TestHitCounter >> 16;
  *(responseValue+3)  = TestHitCounter >> 24;
  TestHitCounter = 0;
}
/**
* @}
*/

/**
* @}
*/

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/

