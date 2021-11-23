/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hw_flash.c
  * @author  MCD Application Team
  * @brief   This file contains the FLASH driver needed by EE module
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

#include "common.h"
#include "stm32_seq.h"
//#include "dbg_gpio.h"
#include "hw_flash.h"
#include "stm32wbxx_hal.h"

static void HW_FLASH_Program_DoubleWord(uint32_t Address, uint64_t Data);
static void HW_FLASH_PageErase(uint32_t Page);
//static void HW_FLASH_WaitEndOfOperation(void);

/*****************************************************************************/

MOBLE_RESULT HW_FLASH_Write(uint32_t address, uint64_t data)
{
  /* Enable EOP interrupt */
 // __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);

 // __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);

  HW_FLASH_Program_DoubleWord(address, data);

 // HW_FLASH_WaitEndOfOperation();

  /* Disable EOP interrupt */
//  __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);

  /* Clear the PG bit once data has been written */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);

  return (MOBLE_RESULT_SUCCESS);
}

/*****************************************************************************/

MOBLE_RESULT HW_FLASH_Erase(uint32_t page, uint16_t n, int interrupt)
{
  UNUSED(interrupt);

  uint32_t loop;

  /* Enable EOP interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);

  for( loop = 0;  loop < n ; loop++)
  {
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);

    HW_FLASH_PageErase(page+loop);

//    HW_FLASH_WaitEndOfOperation();
  }

  /* Disable EOP interrupt */
  __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);

  /* Clear the page erase bit */
  CLEAR_BIT(FLASH->CR, (FLASH_CR_PER | FLASH_CR_PNB));

  return (MOBLE_RESULT_SUCCESS);
}

#if 0
/*****************************************************************************/

uint32_t HW_FLASH_OB_GetIPCCBufferAddr(void)
{
  return READ_BIT(FLASH->IPCCBR, FLASH_IPCCBR_IPCCDBA);
}

/*****************************************************************************/

uint32_t HW_FLASH_OB_GetSFSA(void)
{
  return (READ_BIT(FLASH->SFR, FLASH_SFR_SFSA) >> FLASH_SFR_SFSA_Pos);
}

/*****************************************************************************/

uint32_t HW_FLASH_OB_GetSBRSA(void)
{
  return (READ_BIT(FLASH->SRRVR, FLASH_SRRVR_SBRSA) >> FLASH_SRRVR_SBRSA_Pos);
}

/*****************************************************************************/

uint32_t HW_FLASH_OB_GetSNBRSA(void)
{
  return (READ_BIT(FLASH->SRRVR, FLASH_SRRVR_SNBRSA) >> FLASH_SRRVR_SNBRSA_Pos);
}
#endif

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * This is a copy of FLASH_Program_DoubleWord() from the HAL
 */
static void HW_FLASH_Program_DoubleWord(uint32_t Address, uint64_t Data)
{
//  DBG_GPIO_Gr2Set(DBG_GPIO_GR2_FLASH_WRITE);

  /* Set PG bit */
  SET_BIT(FLASH->CR, FLASH_CR_PG);

  /* Program first word */
  *(uint32_t *)Address = (uint32_t)Data;

  /* Barrier to ensure programming is performed in 2 steps, in right order
    (independently of compiler optimization behavior) */
  __ISB();

  /* Program second word */
  *(uint32_t *)(Address + 4U) = (uint32_t)(Data >> 32U);

//  DBG_GPIO_Gr2Reset(DBG_GPIO_GR2_FLASH_WRITE);
}

/**
 * This is a copy of LASH_PageErase() from the HAL
 */
static void HW_FLASH_PageErase(uint32_t Page)
{
//  DBG_GPIO_Gr2Set(DBG_GPIO_GR2_FLASH_ERASE);

  /* Proceed to erase the page */
  MODIFY_REG(FLASH->CR, FLASH_CR_PNB, ((Page << FLASH_CR_PNB_Pos) | FLASH_CR_PER | FLASH_CR_STRT));

//  DBG_GPIO_Gr2Set(DBG_GPIO_GR2_FLASH_ERASE);
}
