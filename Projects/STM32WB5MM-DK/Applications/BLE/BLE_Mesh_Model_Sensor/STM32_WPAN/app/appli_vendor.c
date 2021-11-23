/**
  ******************************************************************************
  * @file    appli_vendor.c
  * @author  BLE Mesh Team
  * @brief   Application interface for Vendor Mesh Models 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "app_conf.h"
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

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
typedef struct
{
  MOBLE_ADDRESS dst;
  MOBLEUINT8 data[VENDOR_DATA_BUFFER_SIZE]; 
  MOBLEUINT32 length;
  MOBLEUINT8 elementIndex;
} APPLI_SEND_BIG_DATA_PACKET;

/* Private variables ---------------------------------------------------------*/
aPwmLedGsData_TypeDef aPwmLedGsData_vendor;

MOBLEUINT8 ResponseBuffer[VENDOR_DATA_BUFFER_SIZE];
MOBLEUINT16 BuffLength;
APPLI_SEND_BIG_DATA_PACKET Appli_VendorBigData;

/*Variable to enable OTA for received vendor command*/
extern MOBLEUINT8 Appli_LedState;
extern uint16_t DUTY;
extern MOBLEUINT8 NumberOfElements;
MOBLEUINT32 TestHitCounter = 0;
extern Appli_LightPwmValue_t Appli_LightPwmValue[APPLICATION_NUMBER_OF_ELEMENTS];

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
        TRACE_M(TF_VENDOR,"#02-%02hx! \n\r",data[0]);
  
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
        if(tempBuffer[inc] != 0x2E)
        {
          tempBuffer[inc] = BLEMesh_ModelsASCII_To_Char(tempBuffer[inc]);
          TRACE_M(TF_VENDOR,"Lib version is %x\n\r" ,(unsigned char)tempBuffer[inc]);
        }
        else
        {
          TRACE_M(TF_VENDOR,"Lib version is %c\n\r" ,(unsigned char)tempBuffer[inc]);
        }             
        libVersion++;  
             
        inc++;
      } 
      ResponseBuffer[1]= tempBuffer[0];
      ResponseBuffer[2]= tempBuffer[1];
      ResponseBuffer[3]= tempBuffer[3];
      ResponseBuffer[4]= tempBuffer[4];
      ResponseBuffer[5]= tempBuffer[6];
      ResponseBuffer[6]= tempBuffer[7];
      ResponseBuffer[7]= tempBuffer[8];
      BuffLength = 8;      
      break;
    }
  case LIB_SUB_VER:
    {
      subLibVersion = BLEMesh_GetLibrarySubVersion();
      while(*subLibVersion != '\0')
      {
        tempBuffer[inc] = * subLibVersion;
        if((tempBuffer[inc] != 0x2E) && (tempBuffer[inc] != 0x52))
        {               
          tempBuffer[inc] = BLEMesh_ModelsASCII_To_Char(tempBuffer[inc]);
          TRACE_M(TF_VENDOR,"Sub Lib version is %x\n\r" ,(unsigned char)tempBuffer[inc]);
        }
        else
        {
          TRACE_M(TF_VENDOR,"Sub Lib version is %c\n\r" ,(unsigned char)tempBuffer[inc]);
        } 
        subLibVersion++;  
        inc++;
      } 
      ResponseBuffer[1]= tempBuffer[0];
      ResponseBuffer[2]= tempBuffer[1];
      ResponseBuffer[3]= tempBuffer[3];
      ResponseBuffer[4]= tempBuffer[5];
      ResponseBuffer[5]= tempBuffer[7];
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
       TRACE_M(TF_VENDOR,"#01-%02hx! \n\r",data[0]);
  switch(subCmd)
  {             
  case APPLI_TEST_ECHO: 
    {
      if(length > sizeof(ResponseBuffer))
      {
        length = sizeof(ResponseBuffer);
        TRACE_M(TF_VENDOR,"Length received greater than size of response buffer \r\n");
      }
      memcpy (&(ResponseBuffer[1]),&(data[1]),(length-1));
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
        TRACE_M(TF_VENDOR,"Test Counter is running \r\n");
        ResponseBuffer[0] = subCmd;
        ResponseBuffer[1] = Appli_LedState ;
        BuffLength = 2; 
      /*Insert Test related Commands here*/
      break;
    }
  case APPLI_TEST_INC_COUNTER: 
    {           
      TestHitCounter++;              
      TRACE_M(TF_VENDOR,"Command received Count %.2lx \r\n",TestHitCounter);
      ResponseBuffer[0] = subCmd;
      ResponseBuffer[1] = Appli_LedState ;
      BuffLength = 2;
      /*Insert Test related Commands here*/
      break;
    }
  case APPLI_MODEL_PUBLISH_SELECT:
    {
       for (MOBLEUINT8 idx=0; idx<length; idx++)
       {
         TRACE_I(TF_VENDOR,"data[%d]= %d",idx,data[idx]);  
         TRACE_I(TF_VENDOR,"\n\r");
       } 
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
* @param  elementIndex : selected element where '0' is first element       
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_Vendor_LEDControl( MOBLEUINT8 const *data, MOBLEUINT32 length,
                                       MOBLEUINT8 elementIndex , MOBLE_ADDRESS dst_peer)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 subCommand; 
  subCommand = data[0];
  MOBLEUINT16 duty;
  MOBLEUINT16 intensityValue = 0;
      
  TRACE_M(TF_VENDOR,"#03-%02hx!\n\r",data[0]);
  switch(subCommand)
  {
    /* 
    Message Received     B0     B1    B2      B3    B4    B5    B6     B7 
    B0 - Sub-Cmd LED
    B1-B7 - Data Bytes       
    */
  case APPLI_CMD_LED_BULB:
    {
      /*User Need to write the commands as per the element selected*/
          
      TRACE_M(TF_VENDOR,"Appli_LED_Control callback received for elementIndex %d \r\n", elementIndex);
      Appli_LedState = *(data+1); /* Toggle the state of the Blue LED */
      if( Appli_LedState == 1)
      {
        aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_99_9 ;
        aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;

        LED_On(aPwmLedGsData_vendor);
      }
      else
      {
        LED_Off();
      }  
      break;
    }
    
    /* Toggle Command */  
  case APPLI_CMD_TOGGLE:
    {
      /*User Need to write the commands as per the element selected*/
          
      TRACE_M(TF_VENDOR,"Appli_LED_Toggle callback received for elementIndex %d \r\n", elementIndex);
      if(Appli_LedState == 1)
      {
        LED_Off();
        Appli_LedState = 0;
      }
      else
      {
        aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_99_9 ;
        aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
        aPwmLedGsData_vendor[PWM_LED_RED] = PWM_LED_GSDATA_OFF;
        LED_On(aPwmLedGsData_vendor);

        Appli_LedState = 1;
      }
              
      break;
    }
    /* On Command */  
  case APPLI_CMD_ON:
    {
      /*User Need to write the commands as per the element selected*/
          
      TRACE_M(TF_VENDOR,"Appli_LED_ON callback received for elementIndex %d \r\n", elementIndex);     
      aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_99_9 ;
      aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
      aPwmLedGsData_vendor[PWM_LED_RED] = PWM_LED_GSDATA_OFF;
      LED_On(aPwmLedGsData_vendor);

      Appli_LedState = 1;
          
      break;
    }
    /* Off Command */  
  case APPLI_CMD_OFF:
    {
      /*User Need to write the commands as per the element selected*/
          
      TRACE_M(TF_VENDOR,"Appli_LED_OFF callback received for elementIndex %d \r\n", elementIndex);                   
      LED_Off();
      
      Appli_LedState = 0;         
      break;
    }
        /* intensity command */
    case APPLI_CMD_LED_INTENSITY:
      {
        /*User Need to write the commands as per the element selected*/
          
        TRACE_M(TF_VENDOR,"Appli_LED_Intensity callback received for elementIndex %d \r\n", elementIndex);    
        intensityValue = data[2] << 8;
        intensityValue |= data[1];
                    
        duty = PwmValueMapping(intensityValue , 0x7FFF ,0);                         
        if(duty > 16000)
        {
          aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_99_9 ;
          aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
          aPwmLedGsData_vendor[PWM_LED_RED] = PWM_LED_GSDATA_OFF;

          LED_On(aPwmLedGsData_vendor);
        }
        else
        {
          LED_Off();
        }
        break;
      }
      /* Thermometer command */
#ifdef APPLI_CMD_LED_THERMOMETER
      case APPLI_CMD_LED_THERMOMETER:
      {
        /*User Need to write the commands as per the element selected*/
      
        TRACE_M(TF_VENDOR,"Appli_LED_THERMOMETER callback received for elementIndex %d \r\n", elementIndex);
        intensityValue = data[1];
#ifdef INDICATOR_NODE
        TRACE_M(TF_VENDOR,"Appli_LED_THERMOMETER : Intensity value %d \r\n", intensityValue);
        if((intensityValue>170)&&(intensityValue<=255)){
          aPwmLedGsData_vendor[PWM_LED_RED]   = PWM_LED_GSDATA_7_0;
          aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
          aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_OFF;
        }
        else if((intensityValue>85)&&(intensityValue<=170)){
          aPwmLedGsData_vendor[PWM_LED_RED]   = PWM_LED_GSDATA_OFF;
          aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_7_0;
          aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_OFF;
        }
        else if((intensityValue>0)&&(intensityValue<=85)){
          aPwmLedGsData_vendor[PWM_LED_RED]   = PWM_LED_GSDATA_OFF;
          aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
          aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_7_0;
        }
        else if(intensityValue==0){
          aPwmLedGsData_vendor[PWM_LED_RED]   = PWM_LED_GSDATA_OFF;
          aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;
          aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_7_0; 
        }
        else{
          //Error invalid value
          TRACE_M(TF_VENDOR,"Error Invalid Value \r\n");
        }
        TRACE_M(TF_VENDOR,"Appli_LED_THERMOMETER : RED %d , GREEN %d, BLUE %d \r\n", aPwmLedGsData_vendor[PWM_LED_RED], aPwmLedGsData_vendor[PWM_LED_GREEN], aPwmLedGsData_vendor[PWM_LED_BLUE]);
        LED_On(aPwmLedGsData_vendor);
#endif /* INDICATOR_NODE */
        break;
      }
#endif  
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

/**
* @brief  Process the Vendor Data write Command 
* @param  data: Pointer to the data received from peer_addr
* @param  length: Length of the data
* @retval MOBLE_RESULT status of result
*/          
MOBLE_RESULT Appli_Vendor_Data_write(MOBLEUINT8 const *data, MOBLEUINT32 length)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;  
  MOBLEUINT8 subCmd = data[0];
  /*First Byte is sending the Sub Command*/      
  ResponseBuffer[0]=subCmd;
       
  TRACE_M(TF_VENDOR, "#0E-%02hx %02lx! \n\r",data[0], length);
  for(MOBLEUINT16 i=0; i<length; i++)
  {
    TRACE_I(TF_VENDOR,"%02hx ",data[i]);
  }
  TRACE_I(TF_VENDOR,"\n\r");
  switch(subCmd)
  {     
    case APPLI_STRING_WRITE:
    {            
      memcpy(&ResponseBuffer,data,length);
      BuffLength = length;
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
* @brief  Appli_GetTestValue: This function is callback for Application
*          when Vensor application test command received then status message is to be provided
* @param  responseValue: Pointer to the status message
* @retval void
*/ 
void Appli_GetTestValue (MOBLEUINT8 *responseValue)
{
  *responseValue = TestHitCounter;
  *(responseValue+1)  = TestHitCounter >> 8;
  *(responseValue+2)  = TestHitCounter >> 16;
  *(responseValue+3)  = TestHitCounter >> 24;
  TestHitCounter = 0;
}
/**
* @brief  Process the Vendor LED Control Command
* @param  data: Pointer to the data received from peer_addr
* @param  length: Length of the data
* @param  elementIndex : selected element where '0' is first element       
* @param  dst_peer : Destination address
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_Vendor_SetBigDataPacket(MOBLEUINT8 *data, MOBLEUINT32 length,
                                       MOBLEUINT8 elementIndex , MOBLE_ADDRESS dst_peer)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  
  if (length > VENDOR_DATA_BUFFER_SIZE)
  {
    status = MOBLE_RESULT_INVALIDARG;
  }
  memmove(Appli_VendorBigData.data, data, length);
  Appli_VendorBigData.dst = dst_peer;
  Appli_VendorBigData.length = length;
  Appli_VendorBigData.elementIndex = elementIndex;
  return status;
}
/**
* @brief  Send Vendor big data packet
* @param  void      
* @retval MOBLE_RESULT status of result
*/ 
MOBLE_RESULT Appli_Vendor_SendBigDataPacket(void)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  
  if(BLEMesh_TrsptIsBusyState())
  {
    aPwmLedGsData_vendor[PWM_LED_BLUE]  = PWM_LED_GSDATA_99_9 ;
    aPwmLedGsData_vendor[PWM_LED_GREEN] = PWM_LED_GSDATA_OFF;

    LED_On(aPwmLedGsData_vendor);
    status = MOBLE_RESULT_FALSE;
  }
  else
  {
    LED_Off();
    status = BLEMesh_SetRemoteData(Appli_VendorBigData.dst,
                                       Appli_VendorBigData.elementIndex,
                                       0x000E,                                   
                                       Appli_VendorBigData.data, 
                                       Appli_VendorBigData.length,
                                       MOBLE_FALSE, 
                                       MOBLE_TRUE);
  }
  return status;
}

/**
* @brief  Publish Command for Vendor Model
* @param  srcAddress: Source Address of Node 
* @retval void
*/          
void Appli_Vendor_Publish(MOBLE_ADDRESS srcAddress)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 AppliBuff[1];
  /* changes the LED status on other nodes in the network */
    
  AppliBuff[0] = APPLI_CMD_TOGGLE;
    
  result = BLEMesh_SetRemotePublication(VENDORMODEL_STMICRO_ID1, srcAddress,
                                            APPLI_LED_CONTROL_STATUS_CMD, 
                                            AppliBuff, sizeof(AppliBuff),
                                            MOBLE_FALSE, MOBLE_TRUE);
  
  if(result)
  {
    TRACE_I(TF_VENDOR_M, "Publication Error \r\n");
  }
}


/**
* @brief  Publish Temperature Command for Vendor Model
* @param  srcAddress: Source Address of Node 
* @param  temperature: Temperature Measured 
* @retval void
*/          
void Appli_Vendor_Publish_Temperature(MOBLE_ADDRESS srcAddress, MOBLEUINT8 temperature)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 AppliBuff[2];
  /* changes the LED status on other nodes in the network */
    
  AppliBuff[0] = APPLI_CMD_LED_THERMOMETER;
  AppliBuff[1] = temperature;
  TRACE_I(TF_VENDOR_M, "Publish Temperature \r\n");
  result = BLEMesh_SetRemotePublication(VENDORMODEL_STMICRO_ID1, srcAddress,
                                            APPLI_LED_CONTROL_STATUS_CMD, 
                                            AppliBuff, sizeof(AppliBuff),
                                            MOBLE_FALSE, MOBLE_TRUE);
  
  if(result)
  {
    TRACE_I(TF_VENDOR_M, "Publication Error \r\n");
  }
}
/**
* @}
*/

/**
* @}
*/


