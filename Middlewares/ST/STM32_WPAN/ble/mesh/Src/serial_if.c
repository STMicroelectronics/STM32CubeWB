/**
******************************************************************************
* @file    serial_if.c
* @author  BLE Mesh Team
* @version V1.09.000
* @date    15-Oct-2018
* @brief   Serial Interface file 
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
/** @addtogroup BLE_Mesh
*  @{
*/

/** @addtogroup Middlewares_Serial_Interface
*  @{
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

typedef enum 
{
    STATE_IDLE,
    STATE_INPUT_ENTERED,
}tSerialState;
tSerialState SerialCurrentState = STATE_IDLE;

/* Private variables ---------------------------------------------------------*/
static char Rcvd_String[48];

/* Private function prototypes -----------------------------------------------*/
static int Serial_GetString(MOBLEUINT8*, MOBLEUINT8);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Callback function to print the data on serial terminal
* @param  message: Pointer to the string to be printed
* @retval tUpperTesterTestSubState: subState of running test case
*/
void Serial_PrintStringCb(const char *message)
{
    printf("%s\n\r", (char*)message);
}


/**
* @brief  Processes Upper Tester control commands (If implemented in application, 
*         linker would replace weak linking in library )
* @param  rcvdStringBuff: Pointer to received string   
* @param  rcvdStringSize: Size of the received string 
* @retval void
*/
__weak void Upper_Tester_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
}


/**
* @brief  Processes data coming on serial interface
* @param  void   
* @retval void
*/
void Serial_InterfaceProcess(void)
{
    int stringSize = 0;
    
    stringSize = Serial_GetString((MOBLEUINT8*)Rcvd_String, sizeof(Rcvd_String) - 1);
    /* Check if no input has come from user */
    if (!stringSize)
    {
        return;
    }
    else
    {
        Rcvd_String[stringSize] = 0; /* Make last char NULL for string comp */

        /* Check if correct string has been entered or not */
        if (!strncmp(Rcvd_String, "ctrl", 4))
        {            
            SerialCtrl_Process(Rcvd_String, stringSize);
        }
        else if(!strncmp(Rcvd_String, "test", 4))
        {
            Upper_Tester_Process(Rcvd_String, stringSize);  
        }
        else
        {
            printf("Not Entered valid test parameters");  
            SerialCurrentState = STATE_IDLE;
        }      
    }
}


/**
* @brief  Gets the input from user from Serial port
* @param  text: Pointer of String to take input 
* @param  size: Expected size of input string
* @retval int: Size of receive string
*/
static int Serial_GetString(MOBLEUINT8* text, MOBLEUINT8 size)
{
    static int index = 0;
    int stringSize = 0;
    
#ifndef __IAR_SYSTEMS_ICC__
        clearerr(stdin);
#endif
    int ch = getchar();
    /* Note: Please use Full Library configuration in project options to use the full 
         configuration of the C/C++ runtime library for printf and scanf functionality */
    /* Check for error in get function */
    if (ch == EOF)
    {                       
        clearerr(stdin);
    }
    /* check for backspace press */
    else if (ch == 0x7F)
    {
        if (index) --index;
        printf("\b");
    }
    /* Check for the enter key*/
    else if (ch == 0x0D)
    {
        /* check for first time enter to display help message */
        if (!index)
        {
            stringSize = 1;
        }
        else
        {
            stringSize = index;
        }
        index = 0;
        printf("\n\r");
        return stringSize;    
    }
    else
    {
      printf("%c", ch);
        if (index < size)
        {
            text[index++] = (char)ch;
        }
    }
    return stringSize;
}

/**
* @}
*/

/**
* @}
*/
/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
