/**
******************************************************************************
* @file    serial_ctrl.c
* @author  BLE Mesh Team
* @version V1.10.000
* @date    15-Jan-2019
* @brief   Serial Control file 
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
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
#include "serial_if.h"
#include "serial_ctrl.h"
#include "light.h"

/** @addtogroup BlueNRG_Mesh
*  @{
*/

/** @addtogroup Middlewares_Serial_Interface
*  @{
*/

/* Private define ------------------------------------------------------------*/
#define SERIAL_MODEL_DATA_OFFSET      15

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const MODEL_OpcodeTableParam_t *lightOptcodeTable;
const MODEL_OpcodeTableParam_t *genericOptcodeTable;
MOBLEUINT16 lightOptcodeTableLength;
MOBLEUINT16 genericOptcodeTableLength;

/* Private function prototypes -----------------------------------------------*/
MOBLEUINT8 SerialCtrl_GetMinParamLength(MOBLEUINT32 opcode, const MODEL_OpcodeTableParam_t list[], MOBLEUINT16 length);

/* Private functions ---------------------------------------------------------*/ 
/**
* @brief  This funcrion is used to parse the string given by the user
* @param  rcvdStringBuff: buffer to store input string 
* @param  rcvdStringSize: length of the input string 
* @retval void
*/ 
void SerialCtrl_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  
  MOBLE_ADDRESS peer;                 /*node adderess of the destination node*/
  MOBLEUINT16 command;                /*opcode command to be executed by the destination node*/
  MOBLEUINT8 minPropertyLength = 0;   /*minimum number of properties required by a specific command*/
  MOBLEUINT8 elementIndex = 0;          /*default element index*/
  MOBLEUINT8 counter,i,byteCounter = 0; /*variables to convert big-endian input to little endian output*/
  MOBLEUINT8 msb,lsb;                 /*variables to convert ascii characters to 8-bit hex*/
  MOBLEUINT8  data [5] = {0};        /*buffer to output property variables */
  MOBLEUINT8  byteBuff [10] = {0};    /*buffer to store input property variables temperarily*/
  
  sscanf(rcvdStringBuff+5, "%hx %hx ", &peer,&command); 
  
  /* Callback to store a pointer to opcode table starting sddress and length of the table*/
  LightModelServer_GetOpcodeTableCb(&lightOptcodeTable,&lightOptcodeTableLength);     
  GenericModelServer_GetOpcodeTableCb(&genericOptcodeTable,&genericOptcodeTableLength);
  
  /* Minimum parameter length required for a valid opcade in Generic opcode table */
  minPropertyLength = SerialCtrl_GetMinParamLength(command,genericOptcodeTable,genericOptcodeTableLength);
  
  /* Opcode not found in Generic opcode table */
  if (minPropertyLength == 0xff)
  {
    minPropertyLength = SerialCtrl_GetMinParamLength(command,lightOptcodeTable,lightOptcodeTableLength);/*Start finding for opcode in Light Table*/
  }
  /* Opcode not found in Light opcode table */
  if (minPropertyLength == 0xff)
  {
    TRACE_M(TF_SERIAL_CTRL,"incorrect optcode\r\n");  
  }
  /* Found Valid opcode */
  else 
  {    
    i=0;                
    byteCounter=0;     /*variable to count number of bytes in each space separeted word*/  
    counter = 0;
    for(i=SERIAL_MODEL_DATA_OFFSET;i<=(rcvdStringSize);i++)
    {
      /* check if space or NULL found */
      if(rcvdStringBuff[i] == ' '||rcvdStringBuff[i] == '\0' )
      {
        /*if number of bytes is one*/
        if (byteCounter == 1 )
        {
          data[counter] = byteBuff[0];
        }
        /*if number of bytes is two*/
        if (byteCounter == 2 )
        {
          /*assigning value from byteBuff to data in swapped way to convert 
          big-endian input from the user to little-endian input required by the BLEMesh Library */
          data[counter] = byteBuff[1];    
          data[counter+1] = byteBuff[0];
          counter++;
        }
        counter++;       /*for next word*/
        byteCounter=0;          /*for next word, init index to 0*/
      }
      else
      {   /* take two consecutive ascii characters from the rcvdStringBuff and convert to hex values */  
        msb = Serial_CharToHexConvert(rcvdStringBuff[i]);        
        lsb = Serial_CharToHexConvert(rcvdStringBuff[i + 1 ]);
        /*join two hex values to make one hex value*/
        byteBuff[byteCounter]  = msb << 4;
        byteBuff[byteCounter] |= lsb;
        
        i++;      /*increament for loop counter as two values are used */
        byteCounter++;   /*increament byteCounter counter*/
      }
    }
    /*call BLEMesh Library function to send the processed variables */ 
    BLEMesh_SetRemoteData(peer,elementIndex,command, 
                              data, minPropertyLength,
                              MOBLE_FALSE, MOBLE_FALSE); 
  }
}



/**
* @brief  Returns the minimum number of parameters required by a particular optcode
* @param  opcode: Opcode of the model whose minimum number of parameters are required
* @param  list: 
* @param  length:  
* @retval MOBLEUINT16
*/ 
MOBLEUINT8 SerialCtrl_GetMinParamLength(MOBLEUINT32 opcode, const MODEL_OpcodeTableParam_t list[], MOBLEUINT16 length)
{
  for (int i = 0; i < length; i++)
  {
    if (list[i].opcode == opcode)
    {
      return list[i].min_payload_size;
    }
  }
  return 0xff;
}


/**
* @}
*/

/**
* @}
*/
/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
