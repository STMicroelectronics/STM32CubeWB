/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    user_if.c
  * @author  MCD Application Team
  * @brief   User interface file 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "ble_clock.h" 
#include "hci.h"
#include "user_if.h" 
#include "hal.h"  
#include "hal_common.h"
#include "mesh_cfg.h"

#include <stdio.h>
#ifdef IAR
#include <yfuns.h>
#else
#define _LLIO_STDIN  0
#define _LLIO_STDOUT 1
#define _LLIO_STDERR 2
#define _LLIO_ERROR ((size_t)-1) /* For __read and __write. */
#endif



typedef struct _IO_ReceiveCallBack
{
  /**
  * callback to notify the application with the received I/O data
  */ 
  IO_RECEIVE_DATA_USER_CALLBACK_TYPE IO_user_application;
} IO_ReceiveCallBack;
/**
* @}
*/

#define READ_DATA_SIZE      1024
#define READ_BUFFER_IS_EMPTY() (Read_ptr_in == Read_ptr_out)

/* IO User Callback used to handle received data from I/O channel*/
static uint8_t  Read_Buffer [READ_DATA_SIZE]; 
static uint32_t Read_ptr_in = 0;
static uint32_t Read_ptr_out = 0;
/*static volatile uint8_t s_SdkEvalVersion = ID_UNKNOWN;*/
static IO_ReceiveCallBack IO_Receive_Data;
static void SdkEval_IO_processInputData(uint8_t * rx_data, uint16_t data_size);


/**
* @brief  SetLed Sets the state of led
* @param  int state State for the led GPIO
* @retval void
*/ 
void SetLed(int state)
{
  if (state)
  {
    BSP_LED_On(LED_BLUE);
  }
  else
  {
    BSP_LED_Off(LED_BLUE);
  }
}

#if 0
/**
* @brief  GetButtonState 
* @param  void
* @retval BUTTON_STATE returns the status of the userbutton GPIO
*/ 
BUTTON_STATE GetButtonState(void)
{
  return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
}

/**
* @brief  InitButton initializes the GPIO for the user button 
* @param  void
* @retval void
*/
void InitButton(void)
{
  GPIO_InitTypeDef init;
  init.Pin = GPIO_PIN_13;
  init.Mode = GPIO_MODE_INPUT | GPIO_MODE_EVT_FALLING;
  init.Pull = GPIO_NOPULL;
  init.Speed = GPIO_SPEED_VERY_LOW;
  init.Alternate = 0;
  __HAL_RCC_GPIOC_CLK_ENABLE();
  HAL_GPIO_Init(GPIOC, &init);
  EXTI->FTSR |= EXTI_RTSR_TR13; /* enable wake-up event on falling edge */
  EXTI->IMR &= ~EXTI_IMR_MR13;  /* disable interrupt */
  EXTI->EMR  |= EXTI_EMR_MR13;  /* enable event */
}
#endif

/**
* @brief  processInputData  
* @param  uint8_t * rx_data pointer to receive data
* @param  uint16_t data_size size of receive data
* @retval void
*/
void processInputData(uint8_t * rx_data, uint16_t data_size)
{
  SdkEval_IO_processInputData(rx_data, data_size);
}

/**
* @brief  Configure the selected I/O communication channel
* @param  None.
* @retval None.
*/
void SdkEval_IO_Config(IO_RECEIVE_DATA_USER_CALLBACK_TYPE io_receive_function)
{
  IO_Receive_Data.IO_user_application = io_receive_function;
#if defined (ENABLE_USB)
  /* Init Device Library */
  USBD_Init(&USBD_Device, &VCP_Desc, 0);
  
  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
  
  /* Add CDC Interface Class */
  USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
  
  /* Start Device Process */
  USBD_Start(&USBD_Device);
  
#elif defined (ENABLE_USART)
  /* Init Serial Uart */
  MX_USART2_UART_Init();
#endif 
}

/**
* @brief  InitDevice intitializes the Device  
* @param  void
* @retval void
*/
void InitDevice(void)
{
  Init_Device(); /* Initializes HAL and System Clock */
  
#ifndef NEWLIB_SYSCALLS
  SdkEval_IO_Config(processInputData);
#endif
  /* Delay for debug purpose, in order to see printed data at startup. */
#if defined(ENABLE_USART)
  Clock_Wait(2000);
#endif
  
#if defined(ACCELEROMETER_TYPE) || defined(THERMOMETER_TYPE)
  I2C_Reset();
  Temperature_Init();
  Accel_Init();
#endif
  InitButton();
}


/**
* @brief  ShouldSleepFunc sleep mode function   
* @param  void
* @retval void
*/
void ShouldSleepFunc(void)
{
  __disable_irq();
  __WFE();
  __NOP();
  __enable_irq();
}

/**
* @brief  Receive data on selected I/O port
* @param  rx_data pointer to buffer of received
* @param  data_size number of received data 
* @retval None.
*/
void SdkEval_IO_Receive_Data(uint8_t * rx_data, uint16_t data_size)
{
  if(IO_Receive_Data.IO_user_application)
    IO_Receive_Data.IO_user_application(rx_data,data_size);
  
}
/**
* @brief  Read buffer push
* @param  byte 
* @retval None.
*/
void Read_Buffer_Push(uint8_t byte)
{
  Read_Buffer[Read_ptr_in] = byte;
  
  Read_ptr_in++;
  
  /* To avoid buffer overflow */
  if(Read_ptr_in == READ_DATA_SIZE)
  {
    Read_ptr_in = 0;
  }        
}
/**
* @brief  Read buffer pop
* @param  byte 
* @retval None.
*/
int Read_Buffer_Pop(uint8_t *byte)
{
  if(READ_BUFFER_IS_EMPTY())
    return 0;
  
  *byte = Read_Buffer[Read_ptr_out];
  
  Read_ptr_out++;
  
  if(Read_ptr_out == READ_DATA_SIZE)
    Read_ptr_out = 0;  
  
  return 1;
}
/**
* @brief  Process input data
* @param  byte 
* @retval None.
*/
static void SdkEval_IO_processInputData(uint8_t * rx_data, uint16_t data_size)
{
  for(int i = 0; i < data_size; i++){
    Read_Buffer_Push(rx_data[i]);
  }
}
extern UART_HandleTypeDef huart2;
#undef putchar
/**
* @brief  Put character
* @param  byte 
* @retval None.
*/
int putchar(int c)
{
  /* wait for the transmission end */
  while(!(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE)));
  /* send the data */
  HAL_UART_Transmit(&huart2, (uint8_t*)&c, 1, UART_TX_TIMEOUT);
  return c;
}

void __io_putchar(char c)
{
  putchar(c);
}

/* Required for Keil */
#ifndef __IAR_SYSTEMS_ICC__
void _ttywrch(int ch)
{
  putchar(ch);
}

int fputc(int ch, FILE *f)
{
  _ttywrch(ch);
  return(ch);
}
int fgetc(FILE *f)
{
  return -1;
}
#endif

/**
* @brief  Write handle
* @param  buffer 
* @param  size 
* @retval None.
*/
size_t _write(int handle, const unsigned char * buffer, size_t size)
{
  size_t nChars = 0;
  
  if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR) {
    return _LLIO_ERROR;
  }
  
  if (buffer == 0) {
    /* This means that we should flush internal buffers.
    spin until TX complete (TX is idle)*/
#ifdef ENABLE_USB
    while (!USB_OUT_BUFFER_IS_EMPTY()) {}   
#endif /* ENABLE_USB */
    return 0;
  }
  
  while(size--) {
    __io_putchar(*buffer++);
    ++nChars;
  }
  
  return nChars;
}

/**
* @brief  Read handle
* @param  buffer 
* @param  size 
* @retval None.
*/
size_t __read(int handle, unsigned char * buffer, size_t size)
{
  size_t bytes = 0;
  
  while(bytes < size && Read_Buffer_Pop(buffer+bytes)){
    bytes++;
  }
  return bytes;
}

uint8_t __io_getcharNonBlocking(uint8_t *data)
{
  if (__read(_LLIO_STDIN,data,1))
    return 1;
  else
    return 0;
}/* end serialReadByte() */

