/**
  ******************************************************************************
  * @file    qspi.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the qspi supported by the 
  *          STM32WB55MM-DK board
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
#include "main.h"

/** @addtogroup STM32WBxx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x0200)
#define WRITE_READ_ADDR     ((uint32_t)0x0050)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t ButtonState;
uint8_t qspi_aTxBuffer[BUFFER_SIZE];
uint8_t qspi_aRxBuffer[BUFFER_SIZE];
uint8_t qspi_eraseBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t Buffercmp (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  demo of qspi.
  */
void QSPI_demo (void)
{
  /* QSPI info structure */
  BSP_QSPI_Info_t pQSPI_Info;
  uint8_t status;
  S25FL128S_Interface_t Tab[3]={S25FL128S_SPI_MODE, S25FL128S_SPI_1I2O_MODE, S25FL128S_SPI_4IO_MODE};
  char* Tab1[3]={"SPI MODE", "SPI 1I2O MODE", "SPI 4IO MODE"};
  
  /*##-1- Configure the QSPI device ##########################################*/
  /* QSPI device configuration */
  BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
  BSP_LCD_Refresh(0);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI Tests", CENTER_MODE);
  UTIL_LCD_DrawRect(0, 0, 127 , 11, SSD1315_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(0, 15, (uint8_t*)"This example shows", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 27, (uint8_t*)"how to write and", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 39, (uint8_t*)"read data on QSPI", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 51, (uint8_t*)"memory", CENTER_MODE);

  BSP_LCD_Refresh(0);
  HAL_Delay(5000);
  BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
  BSP_LCD_Refresh(0);

  BSP_QSPI_Init_t init ;
  init.TransferRate= S25FL128S_STR_TRANSFER ;
  init.DualFlashMode= S25FL128S_DUALFLASH_DISABLE;

  for(int i=0; i<3; i++)
  {
    if(CheckForUserInput() > 0)
    {
      ButtonState = 0;
      return;
    }
    init.InterfaceMode = Tab[i];
    status = BSP_QSPI_Init(0,&init);

    UTIL_LCD_DisplayStringAt(0, 30, (uint8_t*)Tab1[i], CENTER_MODE);
    UTIL_LCD_DrawRect(0, 30, 127 , 11, SSD1315_COLOR_WHITE);
    BSP_LCD_Refresh(0);
    HAL_Delay(2000);
    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
    BSP_LCD_Refresh(0);

    if (status != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI Init:FAILED", LEFT_MODE);
      BSP_LCD_Refresh(0);
    }

    else
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI Init:OK", LEFT_MODE);
      BSP_LCD_Refresh(0);
      HAL_Delay(500);

      /*##-2- Read & check the QSPI info #######################################*/
      /* Initialize the structure */
      pQSPI_Info.FlashSize          = (uint32_t)0x00;
      pQSPI_Info.EraseSectorSize    = (uint32_t)0x00;
      pQSPI_Info.EraseSectorsNumber = (uint32_t)0x00;
      pQSPI_Info.ProgPageSize       = (uint32_t)0x00;
      pQSPI_Info.ProgPagesNumber    = (uint32_t)0x00;

      /* Read the QSPI memory info */
      BSP_QSPI_GetInfo(0,&pQSPI_Info);

      /* Test the correctness */
      if((pQSPI_Info.FlashSize != 0x1000000) || (pQSPI_Info.EraseSectorSize != 0x1000)  ||
         (pQSPI_Info.ProgPageSize != 0x100)  || (pQSPI_Info.EraseSectorsNumber != 0x1000) ||
           (pQSPI_Info.ProgPagesNumber != 0x10000))
      {

        UTIL_LCD_DisplayStringAt(0, 16, (uint8_t*)"QSPI GET INFO:FAILED", LEFT_MODE);
        BSP_LCD_Refresh(0);
      }
      else
      {
        UTIL_LCD_DisplayStringAt(0, 16, (uint8_t*)"QSPI GET INFO:OK", LEFT_MODE);
        BSP_LCD_Refresh(0);
        HAL_Delay(500);
        /*##-3- Erase QSPI memory ################################################*/
        if(BSP_QSPI_EraseBlock(0,WRITE_READ_ADDR,S25FL128S_ERASE_64K) != BSP_ERROR_NONE)
        {
          UTIL_LCD_DisplayStringAt(0, 32, (uint8_t*)"QSPI ERASE:FAILED", LEFT_MODE);
          BSP_LCD_Refresh(0);
        }
        else
        {
          UTIL_LCD_DisplayStringAt(0, 32, (uint8_t*)"QSPI ERASE:OK", LEFT_MODE);
          BSP_LCD_Refresh(0);
          HAL_Delay(500);
          /*##-4- QSPI memory read/write access  #################################*/
          /* Fill the buffer to write */
          Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, 0xD20F);

          /* Write data to the QSPI memory */
          if(BSP_QSPI_Write(0,qspi_aTxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != BSP_ERROR_NONE)
          {
            UTIL_LCD_DisplayStringAt(0, 48, (uint8_t*)"QSPI WRITE:FAILED", LEFT_MODE);
            BSP_LCD_Refresh(0);
          }
          else
          {
            UTIL_LCD_DisplayStringAt(0, 48, (uint8_t*)"QSPI WRITE:OK", LEFT_MODE);
            BSP_LCD_Refresh(0);
            HAL_Delay(500);
            BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
            BSP_LCD_Refresh(0);

            /* Read back data from the QSPI memory */
            if(BSP_QSPI_Read(0,qspi_aRxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != BSP_ERROR_NONE)
            {
              UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI READ:FAILED", LEFT_MODE);
              BSP_LCD_Refresh(0);
            }
            else
            {
              UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI READ:OK", LEFT_MODE);
              BSP_LCD_Refresh(0);
              HAL_Delay(500);

              /*##-5- Checking data integrity ############################################*/
              if(Buffercmp(qspi_aRxBuffer, qspi_aTxBuffer, BUFFER_SIZE) > 0)
              {
                UTIL_LCD_DisplayStringAt(0, 16, (uint8_t*)"QSPI COMPARE:FAILED", LEFT_MODE);
                BSP_LCD_Refresh(0);
              }
              else
              {
                UTIL_LCD_DisplayStringAt(0, 16, (uint8_t*)"QSPI COMPARE:OK", LEFT_MODE);
                BSP_LCD_Refresh(0);
                HAL_Delay(500);
                /*##-6- Erase QSPI memory ################################################*/
                if(BSP_QSPI_EraseBlock(0,WRITE_READ_ADDR,S25FL128S_ERASE_64K) != BSP_ERROR_NONE)
                {
                  UTIL_LCD_DisplayStringAt(0, 32, (uint8_t*)"QSPI ERASE:FAILED", LEFT_MODE);
                  BSP_LCD_Refresh(0);
                }
                else
                {
                  UTIL_LCD_DisplayStringAt(0, 32, (uint8_t*)"QSPI ERASE:OK", LEFT_MODE);
                  BSP_LCD_Refresh(0);
                  HAL_Delay(500);
                  
                  /* Read back data from the QSPI memory */
                  if(BSP_QSPI_Read(0,qspi_aRxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != BSP_ERROR_NONE)
                  {
                    UTIL_LCD_DisplayStringAt(0, 48, (uint8_t*)"QSPI READ:FAILED", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                  }
                  else
                  {
                    UTIL_LCD_DisplayStringAt(0, 48, (uint8_t*)"QSPI READ:OK", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                    HAL_Delay(500);
                    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
                    BSP_LCD_Refresh(0);
                  }
                  /*##-7- Fill erase Buffer ############################################*/
                  for (int i=0; i < BUFFER_SIZE; i++)
                  {
                    qspi_eraseBuffer[i]= 0xff;
                  }
                  /*##-8- Checking data integrity ############################################*/
                  if(Buffercmp(qspi_aRxBuffer, qspi_eraseBuffer, BUFFER_SIZE) > 0)
                  {
                    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI COMPARE:FAILED", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                  }
                  else
                  {
                    UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI COMPARE:OK", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                    HAL_Delay(500);
                  }
                  /*##-9-Enable Memory Mapped Mode ###############################################*/
                  if(BSP_QSPI_EnableMemoryMappedMode(0)!=BSP_ERROR_NONE)
                  {
                    UTIL_LCD_DisplayStringAt(0, 16, (uint8_t*)"QSPI MMP:FAILED", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                  }
                  else
                  {
                    UTIL_LCD_DisplayStringAt(0, 16, (uint8_t*)"QSPI MMP:OK", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                    HAL_Delay(500);
                  }
                  /*##-10- Disable Memory Mapped Mode ###############################################*/
                  if(BSP_QSPI_DisableMemoryMappedMode(0)!=BSP_ERROR_NONE)
                  {
                    UTIL_LCD_DisplayStringAt(0, 32, (uint8_t*)"QSPI DMMP:FAILED", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                  }
                  else
                  {
                    UTIL_LCD_DisplayStringAt(0, 32, (uint8_t*)"QSPI DMMP:OK", LEFT_MODE);
                    BSP_LCD_Refresh(0);
                    HAL_Delay(500);
                    BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);
                    BSP_LCD_Refresh(0);
                    BSP_QSPI_DeInit(0);
                  }
                }
              }
            }
          }
        }
      }
    }

    /* De-Initialization */
    status = BSP_QSPI_DeInit(0);
    if (status != BSP_ERROR_NONE)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI DeInit:FAILED", LEFT_MODE);
      BSP_LCD_Refresh(0);
    }
  }
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @}
  */ 

/**
  * @}
  */
 
