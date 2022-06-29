/**
  ******************************************************************************
  * @file    b_wb1m_wpan1_eeprom.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage
  *          an I2C M24256DFMC6TG EEPROM memory.
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
#include "b_wb1m_wpan1_bus.h"
#include "b_wb1m_wpan1_eeprom.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup B_WB1M_WPAN1
  * @{
  */

/** @defgroup B_WB1M_WPAN1_EEPROM  B-WB1M-WPAN1 EEPROM
  * @brief This file includes the I2C EEPROM driver of B-WB1M-WPAN1 board.
  * @{
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Private_Types EEPROM Private Types
  * @{
  */
/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Private_Defines EEPROM Private Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Private_Macros EEPROM Private Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Private_Variables EEPROM Private Variables
  * @{
  */
__IO uint8_t EEPROMAddressWrite = 0xA0;
__IO uint8_t EEPROMAddressRead = 0xA1;

/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Private_Functions_Prototypes EEPROM Private Functions Prototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup B_WB1M_WPAN1_EEPROM_Private_Functions EEPROM Private Functions
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @retval BSP_ERROR_NONE (0) if operation is correctly performed, else return value 
  *         different from BSP_ERROR_NONE (0)
  */
int32_t BSP_EEPROM_Init(void)
{ 
  /* I2C Initialization */
  B_WB1M_WPAN1_I2C_Init();

  /* Select the EEPROM address and check if OK */
  if(BSP_I2C1_IsReady(EEPROMAddressRead, EEPROM_MAX_TRIALS) != BSP_ERROR_NONE) 
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  DeInitializes the EEPROM.
  * @retval EEPROM state
  */
int32_t BSP_EEPROM_DeInit(void)
{ 
  return B_WB1M_WPAN1_I2C_DeInit();
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer: pointer to the buffer that receives the data read from
  *         the EEPROM.
  * @param  ReadAddr: EEPROM's internal address to start reading from.
  * @param  NumByteToRead: pointer to the variable holding number of bytes to
  *         be read from the EEPROM.
  *
  * @retval BSP_ERROR_NONE (0) if operation is correctly performed, else return value
  *         different from BSP_ERROR_NONE (0) or the timeout user callback.
  */
int32_t BSP_EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{
  uint32_t buffersize = *NumByteToRead;

  if(BSP_I2C1_ReadReg16(EEPROMAddressRead, ReadAddr, pBuffer, buffersize) != HAL_OK)
  {
    BSP_EEPROM_TIMEOUT_UserCallback();
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* If all operations OK, return BSP_ERROR_NONE (0) */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  *
  * @note   The number of bytes (combined to write start address) must not 
  *         cross the EEPROM page boundary. This function can only write into
  *         the boundaries of an EEPROM page.
  *         This function doesn't check on boundaries condition (in this driver 
  *         the function BSP_EEPROM_WriteBuffer() which calls BSP_EEPROM_WritePage() is 
  *         responsible of checking on Page boundaries).
  *
  * @param  pBuffer: pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: pointer to the variable holding number of bytes to 
  *         be written into the EEPROM. 
  * 
  * @retval BSP_ERROR_NONE (0) if operation is correctly performed, else return value 
  *         different from BSP_ERROR_NONE (0) or the timeout user callback.
  */
int32_t BSP_EEPROM_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{
  uint32_t buffersize = *NumByteToWrite;
  int32_t status = BSP_ERROR_NONE;

  if(BSP_I2C1_WriteReg16(EEPROMAddressWrite, WriteAddr, pBuffer, buffersize) != HAL_OK)
  {
    BSP_EEPROM_TIMEOUT_UserCallback();
    status = BSP_ERROR_PERIPH_FAILURE;
  }

  if(BSP_EEPROM_WaitEepromStandbyState() != BSP_ERROR_NONE) 
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  /* If all operations OK, return BSP_ERROR_NONE (0) */
  return status;
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer: pointer to the buffer containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval BSP_ERROR_NONE (0) if operation is correctly performed, else return value 
  *         different from BSP_ERROR_NONE (0) or the timeout user callback.
  */
int32_t BSP_EEPROM_WriteBuffer(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint16_t numofpage = 0;
  uint16_t numofsingle = 0;
  uint16_t count = 0;
  uint16_t addr = 0;
  uint8_t  dataindex = 0;
  int32_t status = BSP_ERROR_NONE;

  addr = WriteAddr % EEPROM_PAGESIZE;
  count = EEPROM_PAGESIZE - addr;
  numofpage =  NumByteToWrite / EEPROM_PAGESIZE;
  numofsingle = NumByteToWrite % EEPROM_PAGESIZE;

  /* If WriteAddr is EEPROM_PAGESIZE aligned */
  if(addr == 0) 
  {
    /* If NumByteToWrite < EEPROM_PAGESIZE */
    if(numofpage == 0) 
    {
      /* Store the number of data to be written */
      dataindex = numofsingle;
      /* Start writing data */
      status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
      if(status != BSP_ERROR_NONE)
      {
        return status;
      }
    }
    /* If NumByteToWrite > EEPROM_PAGESIZE */
    else
    {
      while(numofpage--)
      {
        /* Store the number of data to be written */
        dataindex = EEPROM_PAGESIZE;
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }

        WriteAddr +=  EEPROM_PAGESIZE;
        pBuffer += EEPROM_PAGESIZE;
      }
      
      if(numofsingle!=0)
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }
      }
    }
  }
  /* If WriteAddr is not EEPROM_PAGESIZE aligned */
  else
  {
    /* If NumByteToWrite < EEPROM_PAGESIZE */
    if(numofpage== 0) 
    {
      /* If the number of data to be written is more than the remaining space
      in the current page: */
      if(NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        dataindex = count;
        /* Write the data contained in same page */
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }

        /* Store the number of data to be written */
        dataindex = (NumByteToWrite - count);
        /* Write the remaining data in the following page */
        status = BSP_EEPROM_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }
      }
      else
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }
      }
    }
    /* If NumByteToWrite > EEPROM_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      numofpage =  NumByteToWrite / EEPROM_PAGESIZE;
      numofsingle = NumByteToWrite % EEPROM_PAGESIZE;

      if(count != 0)
      {  
        /* Store the number of data to be written */
        dataindex = count;
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }
        WriteAddr += count;
        pBuffer += count;
      }

      while(numofpage--)
      {
        /* Store the number of data to be written */
        dataindex = EEPROM_PAGESIZE;
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }
        WriteAddr +=  EEPROM_PAGESIZE;
        pBuffer += EEPROM_PAGESIZE;
      }
      if(numofsingle != 0)
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != BSP_ERROR_NONE)
        {
          return status;
        }
      }
    }
  }

  /* If all operations OK, return BSP_ERROR_NONE (0) */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Wait for EEPROM Standby state.
  *
  * @note  This function allows to wait and check that EEPROM has finished the 
  *        last operation. It is mostly used after Write operation: after receiving
  *        the buffer to be written, the EEPROM may need additional time to actually
  *        perform the write operation. During this time, it doesn't answer to
  *        I2C packets addressed to it. Once the write operation is complete
  *        the EEPROM responds to its address.
  *
  * @retval BSP_ERROR_NONE (0) if operation is correctly performed, else return value
  *         different from BSP_ERROR_NONE (0) or the timeout user callback.
  */
int32_t BSP_EEPROM_WaitEepromStandbyState(void)
{
  /* Check if the maximum allowed number of trials has bee reached */
  if(BSP_I2C1_IsReady(EEPROMAddressWrite, EEPROM_MAX_TRIALS) != HAL_OK)
  {
    /* If the maximum number of trials has been reached, exit the function */
    BSP_EEPROM_TIMEOUT_UserCallback();
    return BSP_ERROR_PERIPH_FAILURE;
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Basic management of the timeout situation.
  * @retval None
  */
__weak void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
