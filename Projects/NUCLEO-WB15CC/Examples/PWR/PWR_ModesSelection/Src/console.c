/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/console.c
  * @author  MCD Application Team
  * @brief   This file provides console output input
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE BEGIN 1 */
/* Includes ------------------------------------------------------------------*/
#include "console.h"
#include <stdio.h>
#include <string.h>
#if defined(__ICCARM__)
#include <LowLevelIOInterface.h>
#endif /* __ICCARM__ */

/** @addtogroup STM32WBxx_HAL_Driver
  * @{
  */

/** @addtogroup PWR_ModesSelection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declared in "main.c" file */
extern UART_HandleTypeDef huart1;

/* Receive status flag */
__IO uint8_t ReceiveStatus = 0;

/* Private function prototypes -----------------------------------------------*/
int _write(int fd, char *ptr, int len);
/* USER CODE BEGIN PFP */
#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE int iar_fputc(int ch)
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#endif /* __ICCARM__ */
/* USER CODE END PFP */
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Output a message on Hyper-terminal.
  * @param  msg : Pointer to the message printed on the Hyper-terminal.
  *
  * @retval None.
  */
void Console_Log(const char *msg)
{
  for (uint32_t i = 0; msg[i] != 0; i++)
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)&msg[i], 1, HAL_MAX_DELAY);
  }
}

/**
  * @brief  Receive a command via the Hyper-terminal.
  * @param  Cmd     : Pointer to return the command received from hyper-terminal.
  * @param  CmdSize : Specifies the size of the buffer received from hyper-terminal.
  * @retval Console Status.
  */
Console_StatusTypeDef Console_GetCommand(uint8_t *Cmd, uint16_t CmdSize)
{
  /* Start the UART Receive process to receive user inputs */

  if (HAL_UART_Receive_IT(&huart1, Cmd, CmdSize) != HAL_OK)
  {
    return CONSOLE_ERROR;
  }

  /* Wait for user inputs */
  while (ReceiveStatus == 0);

  /* Clear receive status flag */
  ReceiveStatus = 0;

  return CONSOLE_OK;
}

/**
  * @brief  UART receive transfer completed callback.
  * @param  husart : UART handle.
  * @retval None.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  ReceiveStatus = 1;
}

#if defined(__ICCARM__)
size_t __write(int file, unsigned char const *ptr, size_t len)
{
  size_t idx;
  unsigned char const *pdata = ptr;

  for (idx = 0; idx < len; idx++)
  {
    iar_fputc((int)*pdata);
    pdata++;
  }
  return len;
}
#endif /* __ICCARM__ */

/**
  * @brief  Re-targets the C library printf function to the UART.
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of putchar here */
  /* e.g. write a character to the UART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END 1 */