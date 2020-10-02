/**
******************************************************************************
* @file    serial_if.c
* @date    06-12-2019
* @brief   Serial Interface file 
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
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "serial_if.h"
#include "ble_common.h"
#include "hal_common.h"
#include "mesh_cfg.h"
#if ENABLE_SERIAL_CONTROL
#include "serial_ctrl.h"
#endif
#if ENABLE_UT
#include "serial_ut.h"
#endif
#if ENABLE_APPLI_TEST
#include "appli_test.h"
#endif
#include "stm_queue.h"
#include "stm32_seq.h"
#if ENABLE_SERIAL_PRVN
#include "serial_prvn.h"
#endif
#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP
#include "appli_sensor.h"
#endif

#ifdef ENABLE_LIGHT_MODEL_SERVER_LC_SETUP
#include "appli_light_lc.h"
#endif

/** @addtogroup BlueNRG_Mesh
*  @{
*/

/** @addtogroup Application
*  @{
*/

/* Private define ------------------------------------------------------------*/
#define RECEIVE_STRING_SIZE                                                   48
#define RX_BUFFER_SIZE                                                        48/*64*/
#define ESC                                                                 0x1b

/* Private macro -------------------------------------------------------------*/
typedef enum 
{
  STATE_IDLE,
  STATE_INPUT_ENTERED,
}tSerialState;

/* Private variables ---------------------------------------------------------*/
static char Rcvd_String[RECEIVE_STRING_SIZE];
tSerialState SerialCurrentState = STATE_IDLE;

static uint8_t InputCharFromUart; 
static queue_t RxQueue;
static uint8_t RxQueueBuffer[RX_BUFFER_SIZE];  
static int stringSize;
static int ch;
static int index_str;

/* Private function prototypes -----------------------------------------------*/
static int Serial_GetString(MOBLEUINT8* text, MOBLEUINT8 size);
#if (!ENABLE_UT)
__weak void SerialUt_Process(char *rcvdStringBuff, uint16_t rcvdStringSize);
#endif 
#if (!ENABLE_SERIAL_CONTROL)
__weak void SerialCtrl_Process(char *rcvdStringBuff, uint16_t rcvdStringSize);
#endif
#if (!ENABLE_APPLI_TEST)
__weak void SerialResponse_Process(char *rcvdStringBuff, uint16_t rcvdStringSize);
#endif
#if (!ENABLE_SERIAL_PRVN)
__weak void SerialPrvn_Process(char *rcvdStringBuff, uint16_t rcvdStringSize);
#endif

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  DBG_TRACE USART Rx Transfer completed callback
 * @retval None
 */
static void Serial_RxCpltCallback( void )
{
  CircularQueue_Add(&RxQueue, &InputCharFromUart,1,1);

  Serial_InterfaceProcess();
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_UART_RX_REQ_ID, CFG_SCH_PRIO_0);

  return;
 }
 
/**
 * @brief  DBG_TRACE USART Rx Transfer completed callback
 * @retval None
 */
static void Serial_Uart_Rx_Task( void )
{
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, &InputCharFromUart, 1, Serial_RxCpltCallback);
}
 
/**
 * *****************************************************************************
 * @fn    char UartReadChar(uint8_t blocking)
 * @author  -
 * @brief   Wait until the receipt of a character from an Uart or JTAG,
 *          then convert it into ASCII and return the character
 * @param   blocking : blocking mode
 * @return  Return the character received from the serial link or the JTAG.
 * *****************************************************************************
 */
static char UartReadChar(uint8_t blocking)
{
  uint16_t size;
  uint8_t *c;
  static uint8_t  ESC_Seq = 0;
  uint8_t leave = 0;
  char retc = 0;
  static uint16_t ESC_Timeout = 0;
    
  while (!leave)
  {
    c = CircularQueue_Remove(&RxQueue, &size);
    if (!c) 
    { 
     if (ESC_Seq == 1)
     {
       if (--ESC_Timeout == 0) 
       {
         retc = ESC;
         ESC_Seq = 0;
         break; /* Assume ESC have been hit */ 
       }
     }
     if (!blocking) leave=1;
    }
    else if (c) /* I got a char */
    {
      retc = *c;
      if ((*c == ESC) && (ESC_Seq == 0))  /* ESC sequence */
      {
        ESC_Timeout = 5000;  /* Maybe better to arm a timer */
        ESC_Seq = 1;          /* If ESC AND Esc_seq ==> error case, here we just restart ESC sequence */
        retc = 0;
      }
      else if (ESC_Seq == 1)  /* check second char in ESC sequence */
      {
        if (*c == '[')  /* True ESC sequence */
        {
          /* wait for next char in ESC seq */
          ESC_Seq++;   
          retc = 0;
          ESC_Timeout = 0;
        }
        else          /* Unexpected char follwing ESC, so ESC hit [no ESC sequence ] */
        {
          leave = 1;
          ESC_Seq = 0;
          retc = ESC;
          ESC_Timeout = 0;
        }
      }
      else if (ESC_Seq == 2)
      {
        leave = 1;
        ESC_Seq = 0;
        retc = *c | 0x80;  
      }
      else
      {
        leave = 1;
      }
    }  
  }
  
  return retc;       
}

/**
* @brief  Gets the input from user from Serial port
* @param  text: String to take input 
* @param  size: Size of string
* @retval int: Running Status of the test case
*/
static int Serial_GetString(MOBLEUINT8* text, MOBLEUINT8 size)
{
//  static int index_str = 0;
  stringSize = 0;

#ifndef __IAR_SYSTEMS_ICC__
  clearerr(stdin);
#endif
  ch = (int)(UartReadChar(FALSE));
  /* Note: Please use Full Library configuration in project options to use the full 
       configuration of the C/C++ runtime library for printf and scanf functionality */
  /* Check for error in get function */
  if (ch == EOF)
  {                       
#ifndef __IAR_SYSTEMS_ICC__
    clearerr(stdin);
#endif
  }
  /* check for backspace press */
  else if (ch == 0x08)
  {
    if (index_str > 0)
    {
      --index_str;
    }
    TRACE_I(TF_SERIAL_CTRL,"\b");
  }
  /* Check for the enter key*/
  else if ((ch == 0x0D) || (ch == 0xFFFFFF0D))
  {
    /* check for first time enter to display help message */
    if (index_str == 0)
    {
      stringSize = 1;
    }
    else
    {
      stringSize = index_str;
    }
    index_str = 0;
    TRACE_I(TF_SERIAL_CTRL,"\n\r");
 //   return stringSize;    
  }
  else
  {
    TRACE_I(TF_SERIAL_CTRL,"%c", ch);
    if (index_str < size)
    {
      /* check if lowercase and convert it to upper case */
      if ((ch >= 'a') && (ch <= 'z'))
      {
        ch = ch + 'A' - 'a';
      }
      text[index_str++] = (char)ch;
    }
  }
  return stringSize;
}

/**
* @brief  This funcrion is used to parse the string given by the user(If 
*         implemented in application, 
*         linker would replace weak linking in library )
* @param  rcvdStringBuff: buffer to store input string 
* @param  rcvdStringSize: length of the input string 
* @retval void
*/ 
#if (!ENABLE_SERIAL_CONTROL)
__weak void SerialCtrl_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{}
__weak void SerialCtrlVendorRead_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{}
__weak void SerialCtrlVendorWrite_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{}
#endif
/**
* @brief  Upper Tester control commands (If implemented in application, 
*         linker would replace weak linking in library )
* @param  rcvdStringBuff: Pointer to received string   
* @retval void
*/
#if (!ENABLE_UT)
__weak void SerialUt_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
}
#endif 

#if (!ENABLE_APPLI_TEST)
__weak void SerialResponse_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
}
#endif

#if (!ENABLE_SERIAL_PRVN)
__weak void SerialPrvn_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
}
#endif
/**
* @brief  Processes data coming from serial port   
* @param  void  
* @retval void
*/
void Serial_InterfaceProcess(void)
{
  Serial_GetString((MOBLEUINT8*)Rcvd_String, sizeof(Rcvd_String) - 1);
  /* Check if no input has come from user */
  if (!stringSize)
  {
//    TRACE_I(TF_SERIAL_CTRL,"No input come from user\r\n");  
    return;
  }
  else
  {
    Rcvd_String[stringSize] = 0; /* Make last char NULL for string comp */

    /* Check if correct string has been entered or not */
        
    if ( 1 != 1 )
    {
      /* This will ALWAYS FAIL
         This statement is put here so, next all statements can enter 
     "else if" 
      */
    }
#ifdef ENABLE_SERIAL_CONTROL
    if (!strncmp(Rcvd_String, "ATCL", 4))
    {            
      SerialCtrl_Process(Rcvd_String, stringSize);
    }
    else if (!strncmp(Rcvd_String, "ATVR", 4))
    {            
      SerialCtrlVendorRead_Process(Rcvd_String, stringSize);
    }
    else if (!strncmp(Rcvd_String, "ATVW", 4))
    {            
      SerialCtrlVendorWrite_Process(Rcvd_String, stringSize);
    }
#endif
#if ENABLE_UT
    else if(!strncmp(Rcvd_String, "ATUT", 4))
    {
      SerialUt_Process(Rcvd_String, stringSize);  
    }
#endif
#if ENABLE_APPLI_TEST
    else if(!strncmp(Rcvd_String, "ATAP", 4))
    {
      SerialResponse_Process(Rcvd_String, stringSize);  
    }
#endif
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB        
    else if(!strncmp(Rcvd_String, "ATIN", 4))
    {
      Appli_BleSerialInputOOBValue(Rcvd_String, stringSize);  
    }
#endif
#if ENABLE_SERIAL_PRVN        
    else if(!strncmp(Rcvd_String, "ATEP", 4))
    {
       SerialPrvn_Process(Rcvd_String, stringSize);
    }
#endif        
#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP
    else if(!strncmp(Rcvd_String, "ATSNR", 5))
    {
      Appli_Sensor_SerialCmd(Rcvd_String, stringSize);
    }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
    else if(!strncmp(Rcvd_String, "ATLLC", 5))
    {
      Appli_Light_LC_SerialCmd(Rcvd_String, stringSize);
    }
#endif

    else
    {
      TRACE_I(TF_SERIAL_PRINTS,"Not Entered valid test parameters\r\n");  
      SerialCurrentState = STATE_IDLE;
    }      
    while(stringSize)
    {
      Rcvd_String[--stringSize] = 0;
    }
  }
}

/**
* @brief  Convert ASCII character to hexadecimal number
* @param  addr: input ASCI character
* @retval MOBLEUINT8
*/ 

MOBLEUINT8 Serial_CharToHexConvert(char addr)
{
  MOBLEUINT8 retVal=0;
  if (addr >= '0' && addr <= '9')
        retVal = addr - '0';
  else if (addr >= 'a' && addr <= 'f')
        retVal = addr+10-'a';
  else if (addr >= 'A' && addr <= 'F')
        retVal = addr+10-'A';
  else if (addr == ' ')
        retVal = addr+10-' ';
  else
       return 0xFF;
    
  return retVal;
}

/**
  * @brief  This function initialize the Rx from UART
  * @param  None
  * @retval None
  */
void Serial_Init(void)
{
  CircularQueue_Init(&RxQueue, RxQueueBuffer, RX_BUFFER_SIZE, 1, CIRCULAR_QUEUE_NO_WRAP_FLAG);
  
//  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, &InputCharFromUart, 1, Serial_RxCpltCallback);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_UART_RX_REQ_ID, UTIL_SEQ_RFU, Serial_Uart_Rx_Task );
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_UART_RX_REQ_ID, CFG_SCH_PRIO_0);

  return;
}

/**
* @brief  Callback function to print data serially  
* @param  *message: Pointer of data string  
* @retval void
*/
void BLEMesh_PrintStringCb(const char *message)
{
    TRACE_I(TF_SERIAL_PRINTS,"%s\n\r", (char*)message);
}
/**
* @brief  Callback function to print data array on screen LSB first 
* @param  *data : Pointer to the data to be printed 
* @param  size : Length of data to be printed
* @retval void
*/
void BLEMesh_PrintDataCb(MOBLEUINT8* data, MOBLEUINT16 size)
{
    for (int count=0; count<size; ++count)
    {
        TRACE_I(TF_SERIAL_PRINTS,"%02X", data[count]);
    }
    
    TRACE_I(TF_SERIAL_PRINTS,"\n\r");
}
/**
* @}
*/

/**
* @}
*/
/******************* (C) COPYRIGHT 2020 STMicroelectronics *****END OF FILE****/
