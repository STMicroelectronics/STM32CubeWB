/**
  ******************************************************************************
  * @file    eeprom.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the eeprom supported by the
  *          B-WL5M-SUBG board
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "b_wb1m_wpan1_bus.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus; 
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE             (countof(TxBuffer))
#define EEPROM_WRITE_ADDRESS1   0x0
#define EEPROM_READ_ADDRESS1    0x0
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM Demo
  * @param  None
  * @retval None
  */
void EEPROM_demo (void)
{ 
  uint8_t TxBuffer[] = "**** B-WB1M-SUBG EEPROM ****";
  uint8_t RxBuffer[64] = {0};
  
  /* Select EEPROM Component */
  /* Initialize the EEPROM driver --------------------------------------------*/
  if (BSP_EEPROM_Init() != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* First write in the memory followed by a read of the written data ----*/
  /* Write on EEPROM to EEPROM_WRITE_ADDRESS1 */
  if (BSP_EEPROM_WriteBuffer(TxBuffer, EEPROM_WRITE_ADDRESS1, BUFFER_SIZE) != BSP_ERROR_NONE)
  {
     Error_Handler();
  }
      
  /* Set the Number of data to be read */
  uint16_t NumDataRead = (uint16_t)BUFFER_SIZE;
      
  /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
  if (BSP_EEPROM_ReadBuffer(RxBuffer, EEPROM_READ_ADDRESS1, (uint16_t *)(&NumDataRead)) != BSP_ERROR_NONE)
  {
     Error_Handler();
  }
      
  /* Check if the data written to the memory is read correctly */
  TestStatus TransferStatus = Buffercmp(TxBuffer, RxBuffer, BUFFER_SIZE);
  if(TransferStatus == FAILED)
  {
    printf("\tWrite/Read failed !!\n");
    Error_Handler();
  }
  else
  {
    printf("\tWrite/Read passed !!\n");
    HAL_Delay( 5000 );
  }
  
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval 0.
  */
void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}

/**
  * @}
  */ 
/**
  * @}
  */ 
