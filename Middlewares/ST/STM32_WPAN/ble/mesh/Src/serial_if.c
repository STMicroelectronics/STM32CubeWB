/**
******************************************************************************
* @file    serial_if.c
* @author  MCD Application Team
* @brief   Serial Interface file 
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
#define C_SIZE_CMD_STRING                                                   256U
#define RX_BUFFER_SIZE                                                        8U

#define ESC                                                                 0x1b

/* Private macro -------------------------------------------------------------*/
typedef enum 
{
  STATE_IDLE,
  STATE_INPUT_ENTERED,
}tSerialState;

/* Private variables ---------------------------------------------------------*/
uint8_t button_emulation; /* Button emulation state */
tSerialState SerialCurrentState = STATE_IDLE;

static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;

extern EXTI_HandleTypeDef exti_handle;

/* Private function prototypes -----------------------------------------------*/
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
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    if (aRxBuffer[0] == '\r')
    {
      CommandString[indexReceiveChar] = 0; /* Make last char NULL for string comp */
        
      TRACE_I(TF_SERIAL_PRINTS,"received %s\n", CommandString);
      
      UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_SERIAL_REQ_ID, CFG_SCH_PRIO_0);
    }
#if 1
    else if (aRxBuffer[0] == '\b')
    {
      if (indexReceiveChar > 1)
      {
        indexReceiveChar--;
      }
      UTIL_SEQ_SetTask( 1<< CFG_TASK_MESH_UART_RX_REQ_ID, CFG_SCH_PRIO_0);
    }
#endif
    else
    {
      if ((aRxBuffer[0] >= 'a') && (aRxBuffer[0] <= 'z'))
      {
        aRxBuffer[0] = aRxBuffer[0] + 'A' - 'a';
      }
      CommandString[indexReceiveChar++] = aRxBuffer[0];
      UTIL_SEQ_SetTask( 1<< CFG_TASK_MESH_UART_RX_REQ_ID, CFG_SCH_PRIO_0);
    }
  }

  return;
 }
 
/**
 * @brief  DBG_TRACE USART Rx Transfer completed callback
 * @retval None
 */
static void Serial_Uart_Rx_Task( void )
{
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, Serial_RxCpltCallback);
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
  /* Reset button emulation state */
  button_emulation = 0;
#ifdef ENABLE_SERIAL_CONTROL
  if (!strncmp((char const*)CommandString, "ATCL", 4))
  {            
    SerialCtrl_Process((char *)CommandString, indexReceiveChar);
  }
  else if (!strncmp((char const*)CommandString, "ATVR", 4))
  {            
    SerialCtrlVendorRead_Process((char *)CommandString, indexReceiveChar);
  }
  else if (!strncmp((char const*)CommandString, "ATVW", 4))
  {            
    SerialCtrlVendorWrite_Process((char *)CommandString, indexReceiveChar);
  }
#endif
#if ENABLE_UT
  else if(!strncmp((char const*)CommandString, "ATUT", 4))
  {
    SerialUt_Process((char *)CommandString, indexReceiveChar);  
  }
#endif
#if ENABLE_APPLI_TEST
  else if(!strncmp((char const*)CommandString, "ATAP", 4))
  {
    SerialResponse_Process((char *)CommandString, indexReceiveChar);  
  }
#endif
#ifdef ENABLE_AUTH_TYPE_INPUT_OOB        
  else if(!strncmp((char const*)CommandString, "ATIN", 4))
  {
    Appli_BleSerialInputOOBValue((char *)CommandString, indexReceiveChar);  
  }
#endif
#if ENABLE_SERIAL_PRVN        
  else if(!strncmp((char const*)CommandString, "ATEP", 4))
  {
     SerialPrvn_Process((char *)CommandString, indexReceiveChar);
  }
#endif        
#ifdef ENABLE_SENSOR_MODEL_SERVER_SETUP
  else if(!strncmp((char const*)CommandString, "ATSNR", 5))
  {
    Appli_Sensor_SerialCmd((char *)CommandString, indexReceiveChar);
  }
#endif
#ifdef ENABLE_LIGHT_MODEL_SERVER_LC
  else if(!strncmp((char const*)CommandString, "ATLLC", 5))
  {
    Appli_Light_LC_SerialCmd((char *)CommandString, indexReceiveChar);
  }
#endif
  else if(!strncmp((char const*)CommandString, "SW1", 3))
  {
    button_emulation = 1; /* Set the button emulation */
    TRACE_I(TF_SERIAL_PRINTS,"SW1 OK\r\n");
    exti_handle.Line = EXTI_LINE_4;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if(!strncmp((char const*)CommandString, "SW2", 3))
  {
    button_emulation = 1; /* Set the button emulation */
    TRACE_I(TF_SERIAL_PRINTS,"SW2 OK\r\n");
    exti_handle.Line = EXTI_LINE_0;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if(!strncmp((char const*)CommandString, "SW3", 3))
  {
    button_emulation = 1; /* Set the button emulation */
    TRACE_I(TF_SERIAL_PRINTS,"SW3 OK\r\n");
    exti_handle.Line = EXTI_LINE_1;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }

  else
  {
    TRACE_I(TF_SERIAL_PRINTS,"Not Entered valid test parameters\r\n");  
    SerialCurrentState = STATE_IDLE;
  }      
  while(indexReceiveChar)
  {
    CommandString[--indexReceiveChar] = 0;
  }
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_UART_RX_REQ_ID, CFG_SCH_PRIO_0);
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
  button_emulation = 0; /* Reset the button emulation state */

  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_SERIAL_REQ_ID, UTIL_SEQ_RFU, Serial_InterfaceProcess);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MESH_UART_RX_REQ_ID, UTIL_SEQ_RFU, Serial_Uart_Rx_Task );
  UTIL_SEQ_SetTask( 1<< CFG_TASK_MESH_UART_RX_REQ_ID, CFG_SCH_PRIO_0);

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

