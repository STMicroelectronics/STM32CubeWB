/**
******************************************************************************
* @file    serial_ut.c
* @author  BLE Mesh Team
* @brief   Upper Tester file 
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "hal_common.h"
#include "serial_ut.h"
#if (ENABLE_UT)
#include "serial_ctrl.h"
#endif
#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP
#include "appli_sensor.h"
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
#include "appli_light_lc.h"
#endif

/** @addtogroup BLE_Mesh
*  @{
*/

/** @addtogroup Application_Callbacks_BLENRG2
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CMD_INDEX_SEND_01                 1
#define CMD_INDEX_SEND_02                 2
#define CMD_INDEX_SET_01                  3
#define CMD_INDEX_SET_02                  4
#define CMD_INDEX_SET_03                  5
#define CMD_INDEX_SET_04                  6
#define CMD_INDEX_SET_05                  7
#define CMD_INDEX_SET_06                  8
#define CMD_INDEX_SET_07                  9
#define CMD_INDEX_SET_08                  10
#define CMD_INDEX_SET_09                  11
#define CMD_INDEX_SET_10                  12
#define CMD_INDEX_SET_11                  13
#define CMD_INDEX_SET_12                  14
#define CMD_INDEX_SET_13                  15
#define CMD_INDEX_SET_14                  16
#define CMD_INDEX_SET_15                  17
#define CMD_INDEX_SET_16                  18        //TR0
#define CMD_INDEX_SET_17                  19        //OCCUPANCY
#define CMD_INDEX_SET_18                  20        //setv
#define CMD_INDEX_SET_19                  21       //PUBLISH
#define CMD_INDEX_PRINT_01                22

#define CMD_SET_COUNT                     19 
#define CMD_SEND_COUNT                    2
#define CMD_PRINT_COUNT                   1

#define CMD_SET_OFFSET                    7
#define CMD_SEND_OFFSET                   8
#define CMD_PRINT_OFFSET                  9
#define CMD_CTRL_OFFSET                   5
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SerialUt_doubleHexToHex(MOBLEUINT8* hexArray,MOBLEUINT8* outputArray, MOBLEUINT8 length);
static MOBLEUINT16 SerialUt_GetFunctionIndex(char *text);
/* Private functions ---------------------------------------------------------*/

/**
* @brief  SerialUt_Process: This function extracts the command and variables from
the received string and passes it to BLEMesh library.
* @param  rcvdStringBuff: Pointer to the received ascii character array from the user
* @param  rcvdStringSize: Size of the received array
* @retval void
*/
void SerialUt_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  MOBLEUINT16 commandIndex = SerialUt_GetFunctionIndex(rcvdStringBuff+5);
  MOBLEUINT8 testFunctionParm[6]= {'\0'} ;
  MOBLEUINT8 asciiFunctionParameter[7] = {'\0'} ;
#ifdef ENABLE_SENSOR_MODEL_SERVER
  MOBLEUINT16 value = 0;           
  MOBLEUINT8 sensorOffset = 0;    
#endif
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEBOOL unprovisioned = MOBLE_FALSE;
  
  switch (commandIndex)
  {
  case CMD_INDEX_SEND_01:
    sscanf(rcvdStringBuff + CMD_CTRL_OFFSET + CMD_SEND_OFFSET, "%2s %4s", asciiFunctionParameter,asciiFunctionParameter+2);
    /*SerialUt_doubleHexToHex
    Function will convert the asci string into orinal hex format.
    eg- send-01 12 3456
    return 0x12,0x34,0x56       
    */
    SerialUt_doubleHexToHex(asciiFunctionParameter,testFunctionParm,6);
    /*BLEMesh_UpperTesterDataProcess
    this function will take action and execute some other functions
    */       
    result = BLEMesh_UpperTesterDataProcess( commandIndex,  testFunctionParm);
    break;
          
  case CMD_INDEX_SEND_02: 
    sscanf(rcvdStringBuff + CMD_CTRL_OFFSET + CMD_SEND_OFFSET, "%2s %4s", asciiFunctionParameter,asciiFunctionParameter+2);
    SerialUt_doubleHexToHex(asciiFunctionParameter,testFunctionParm,6);
    result = BLEMesh_UpperTesterDataProcess( commandIndex,  testFunctionParm);       
    break;
    /* Resets the Board */
  case CMD_INDEX_SET_01: 
    /* test signal in IV Update */
  case CMD_INDEX_SET_04:     
    /* Breaks the existing friendship */
  case CMD_INDEX_SET_06:      
    /* Clears the reply protection list */
  case CMD_INDEX_SET_07:     
    /* Orders IUT to allow only one NetKeyIndex */
  case CMD_INDEX_SET_08:     
    /* Clears the heartbeat subscription count */
  case CMD_INDEX_SET_09:   
    /* Order IUT to clear the Net & App Keys Buffer */
  case CMD_INDEX_SET_13:  
     /* Order IUT to update the NetKeyIndex */
  case CMD_INDEX_SET_14:
    /* Order IUT to clear Publication & Subscription List*/
  case CMD_INDEX_SET_15:
    result = BLEMesh_UpperTesterDataProcess(commandIndex, testFunctionParm);
    break;
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC    
    /* Change Light LC property states transitions that are immediate*/
  case CMD_INDEX_SET_16:
    result = Light_LC_SetTransitionTimeZero(1);
    break;
    /* Change IUT's Light LC Occupancy state to 1*/
  case CMD_INDEX_SET_17: 
    /* For Light LC on element index 1 */
    Appli_Light_LC_SensorPropertyUpdate(1, PRESENCE_DETECTED_PID, 1);
    result = MOBLE_RESULT_SUCCESS;
    break;
#endif    
    /* Updates the 96 hour IV Update limit */
  case CMD_INDEX_SET_03: 
    /* Subscribe/Un-Subscribe to group address */
  case CMD_INDEX_SET_05:     
    /* Sets all node identity for all Networks */
  case CMD_INDEX_SET_11:     
    /* Updates the features supported by the library */
  case CMD_INDEX_SET_12:  
    sscanf(rcvdStringBuff + CMD_CTRL_OFFSET + CMD_SET_OFFSET, "%1c", testFunctionParm);
    result = BLEMesh_UpperTesterDataProcess( commandIndex,  testFunctionParm);
    break;
    /* Sets system faults for Health Model*/
  case CMD_INDEX_SET_10: 
    sscanf(rcvdStringBuff + CMD_CTRL_OFFSET + CMD_SET_OFFSET, "%2s %1c", asciiFunctionParameter, testFunctionParm+1);
    SerialUt_doubleHexToHex(asciiFunctionParameter,testFunctionParm,2);
    result = BLEMesh_UpperTesterDataProcess( commandIndex,  testFunctionParm);
    break;
#ifdef ENABLE_SENSOR_MODEL_SERVER
  /* MMDL/SR/SNRS/BV-09-C, during test */
  case CMD_INDEX_SET_18:
    sscanf(rcvdStringBuff + CMD_CTRL_OFFSET + CMD_SET_OFFSET, "%4hx", &value);
    result = Appli_Sensor_Update(sensorOffset, value);
    break;
   
  /* MMDL/SR/SNRS/BV-08-C */
  case CMD_INDEX_SET_19:
    scanf(rcvdStringBuff + CMD_CTRL_OFFSET + CMD_SET_OFFSET, "%1hx %1hx", &value);
    sensorOffset = value;
    result = Sensor_UpdatePublishState(sensorOffset, value);
    break;
#endif
  /* Unprovisions the Node */
  case CMD_INDEX_SET_02:
    {
      BLEMesh_StopAdvScan();
      unprovisioned = BLEMesh_IsUnprovisioned();
      result = BLEMesh_UpperTesterDataProcess(commandIndex, testFunctionParm);
    }
    break;
    
    /* Print Security Credentials */
  case CMD_INDEX_PRINT_01:  
    result = BLEMesh_UpperTesterDataProcess( commandIndex,  testFunctionParm);
    break;
      
  default:
    printf("Invalid Command\r\n");
  }
  if (result == MOBLE_RESULT_SUCCESS)
  {
    BLEMesh_PrintStringCb("Test command executed successfully\r\n");
    if(commandIndex == CMD_INDEX_SET_02)
    {
      if(!unprovisioned)
      {
        Appli_Unprovision();
      }
      else
      {
        BLEMesh_PrintStringCb("Device is already unprovisioned !\r\n");
      }
    }
  }
  else if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
    BLEMesh_PrintStringCb("Test command Failed. Out of memory\r\n");  
  }
  else if(result == MOBLE_RESULT_INVALIDARG)
  {
    BLEMesh_PrintStringCb("Test command Failed. Invalid Argument\r\n");  
  }
  else
  {
    BLEMesh_PrintStringCb("Test command Failed.\r\n");   
  }
}
          
/**
* @brief  SerialUt_GetFunctionIndex: This function returns the calculated index 
of the command received by the user
* @param  rcvdStringBuff: Pointer to the received ascii character array from the user
* @param  rcvdStringSize: Size of the received array
* @retval MOBLEUINT16
*/
static MOBLEUINT16 SerialUt_GetFunctionIndex(char *text)
{
  MOBLEINT16 index = 0;

  if (!strncmp(text, "SEND-",5))
{
    /* sscanf is to extract the integeer value from the string */        
    sscanf(text, "SEND-%hd", &index);
    index = (index<=CMD_SEND_COUNT) ? index : 0;

}
  else if (!strncmp(text, "SET-",4))
{
    sscanf(text, "SET-%hd", &index);
    index = (index<=CMD_SET_COUNT)? index + CMD_SEND_COUNT : 0;

    }
  else if (!strncmp(text, "PRINT-",6))
    {
    sscanf(text, "PRINT-%hd", &index);
    index = (index<=CMD_PRINT_COUNT)?
      index + CMD_SET_COUNT + CMD_SEND_COUNT : 0;
    }
        return index;
    }

/**
* @brief  SerialUt_doubleHexToHex: This function converts two 4-bit hex integers 
to one 8-bit hex integer 
* @param  hexArray: Pointer to input hex array 
* @param  outputArray: Pointer to output hex array 
* @param  length: length of the input hex array
* @retval void
*/
static void SerialUt_doubleHexToHex(MOBLEUINT8* hexArray,MOBLEUINT8* outputArray, MOBLEUINT8 length)
{
  MOBLEUINT8 counter =0,position = 0;
  MOBLEUINT8 msb,lsb;;
  while (counter <length)
    {
    msb = Serial_CharToHexConvert(hexArray[counter]);
    lsb = Serial_CharToHexConvert(hexArray[counter + 1 ]);
    outputArray[position] = msb<<4;
    outputArray[position] |= lsb;
    printf("data scanned is  %02hx \n\r",outputArray[position]);
    counter+=2;
    position++;
    }
}

/**
* @brief  BLEMesh_UpperTesterDataProcess:THis funcrion is for testing ,If implemented in library, 
*         linker would replace weak linking from here.
* @param  testFunctionIndex: Index of the command to be executed by the user
* @param  testFunctionParm: Pointer to received parameters from the user
* @retval MOBLE_RESULT
*/
__weak MOBLE_RESULT BLEMesh_UpperTesterDataProcess(MOBLEUINT8 testFunctionIndex, MOBLEUINT8* testFunctionParm)
{ 
  printf("RECEIVED FUNCTION INDEX = %d\r\n",testFunctionIndex); 
  printf("RECEIVED FUNCTION PARAMETER = %s\r\n",testFunctionParm); 
  printf("\r\n"); 

  return MOBLE_RESULT_SUCCESS;
}


