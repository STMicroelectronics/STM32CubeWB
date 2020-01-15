/**
******************************************************************************
* @file    serial_ut.c
* @author  BLE Mesh Team
* @version V1.07.000
* @date    15-June-2018
* @brief   Upper Tester file 
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
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "hal_common.h"
#include "serial_ut.h"
#if (ENABLE_UT)
#include "serial_ctrl.h"
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
#define CMD_INDEX_PRINT_01                18

#define CMD_SET_COUNT                     15 
#define CMD_SEND_COUNT                    2
#define CMD_PRINT_COUNT                   1

#define CMD_SET_OFFSET                    7
#define CMD_SEND_OFFSET                   8
#define CMD_SET_OFFSET                    7
#define CMD_PRINT_OFFSET                  9
#define CMD_CTRL_OFFSET                   5
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SerialUt_doubleHexToHex(MOBLEUINT8* hexArray,MOBLEUINT8* outputArray, MOBLEUINT8 length);
static MOBLEUINT16 SerialUt_GetFunctionIndex(char *text);
/* Private functions ---------------------------------------------------------*/

/**
* @brief  SerialUt_Process: This function extracts the command and variables from
the recieved string and passes it to BLEMesh library.
* @param  rcvdStringBuff: Pointer to the recieved ascii character array from the user
* @param  rcvdStringSize: Size of the recieved array
* @retval void
*/
void SerialUt_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  MOBLEUINT16 commandIndex = SerialUt_GetFunctionIndex(rcvdStringBuff+5);
  MOBLEUINT8 testFunctionParm[6]= {'\0'} ;
  MOBLEUINT8 asciiFunctionParameter[7] = {'\0'} ;
  MOBLE_RESULT result;
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
    /* Unprovisions the Node */
  case CMD_INDEX_SET_02:     
    /* test signal in IV Update */
  case CMD_INDEX_SET_04:     
    /* Breakes the exsisting friendship */
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
of the command recieved by the user
* @param  rcvdStringBuff: Pointer to the recieved ascii character array from the user
* @param  rcvdStringSize: Size of the recieved array
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
* @param  testFunctionParm: Pointer to recieved parameters from the user
* @retval MOBLE_RESULT
*/
__weak MOBLE_RESULT BLEMesh_UpperTesterDataProcess(MOBLEUINT8 testFunctionIndex, MOBLEUINT8* testFunctionParm)
{ 
  printf("RECIEVED FUNCTION INDEX = %d\r\n",testFunctionIndex); 
  printf("RECIEVED FUNCTION PARAMETER = %s\r\n",testFunctionParm); 
  printf("\r\n"); 

  return MOBLE_RESULT_SUCCESS;
}