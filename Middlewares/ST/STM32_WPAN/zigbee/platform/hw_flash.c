/*****************************************************************************
 * @file    hw_flash.c
 * @author  MCD Application Team
 * @brief   This file contains the Flash driver needed by EE module
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#include "hw_flash.h"
#include "stm32wbxx_hal.h"

static void HW_FLASH_Program_DoubleWord(uint32_t Address, uint64_t Data);
static void HW_FLASH_PageErase(uint32_t Page);
//static void HW_FLASH_WaitEndOfOperation(void);

/*****************************************************************************/

int HW_FLASH_Write(uint32_t address, uint64_t data)
{
  /* Enable EOP interrupt */
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR |FLASH_FLAG_PGSERR | FLASH_FLAG_OPTVERR); 

  /*Enable EOP interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
  
  HW_FLASH_Program_DoubleWord(address, data);

  /* Disable EOP interrupt */
  __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);
  
  /* Clear the PG bit once data has been written */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
  HAL_FLASH_Lock();
  return (HW_OK);
}

/*****************************************************************************/

int HW_FLASH_Erase(uint32_t page, uint16_t n, int interrupt)
{
  UNUSED(interrupt);
 
  uint32_t loop;
 
   HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP |FLASH_FLAG_ALL_ERRORS); 
  /* Enable EOP interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);

  for( loop = 0;  loop < n ; loop++)
  {
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);

    HW_FLASH_PageErase(page+loop);
  }

  /* Disable EOP interrupt */
  __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);

  /* Clear the page erase bit */
  CLEAR_BIT(FLASH->CR, (FLASH_CR_PER | FLASH_CR_PNB));
  HAL_FLASH_Lock();
  return (HW_OK);
}



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
  
  /* Check the parameters */
  assert_param(IS_FLASH_PAGE(Page));

  /* Proceed to erase the page */
  MODIFY_REG(FLASH->CR, FLASH_CR_PNB, ((Page << FLASH_CR_PNB_Pos) | FLASH_CR_PER | FLASH_CR_STRT));
 
//  DBG_GPIO_Gr2Set(DBG_GPIO_GR2_FLASH_ERASE);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE***/
