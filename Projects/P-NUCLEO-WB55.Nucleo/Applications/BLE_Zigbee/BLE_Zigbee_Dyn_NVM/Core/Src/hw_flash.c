/*****************************************************************************
 * @file    hw_flash.c
 * @author  MCD Application Team
 * @brief   This file contains the FLASH driver needed by EE module
 *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 *****************************************************************************
 */

//#include "common.h"
#include "stm32_seq.h"
//#include "dbg_gpio.h"
#include "hw_flash.h"
#include "stm32wbxx_hal.h"
#include "flash_driver.h"

#include "app_common.h"
#include "shci.h"
#include "utilities_conf.h"

/*****************************************************************************/

int HW_FLASH_Write(uint32_t address, uint64_t data)
{
  /**
   *  Take the semaphore to take ownership of the Flash IP
   */
  while(LL_HSEM_1StepLock(HSEM, CFG_HW_FLASH_SEMID));

  HAL_FLASH_Unlock();
  
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR |FLASH_FLAG_PGSERR | FLASH_FLAG_OPTVERR); 

  /*Enable EOP interupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
  
  FD_WriteSingleData(address, data);

  /* Disable EOP interrupt */
  __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);
  
  /* Clear the PG bit once data has been written */
  CLEAR_BIT(FLASH->CR, FLASH_CR_PG);

  HAL_FLASH_Lock();

  /**
   *  Release the ownership of the Flash IP
   */
  LL_HSEM_ReleaseLock(HSEM, CFG_HW_FLASH_SEMID, 0);
  
  return (HW_OK);
}

/*****************************************************************************/

int HW_FLASH_Erase(uint32_t page, uint16_t n, int interrupt)
{
  UNUSED(interrupt);
 
  uint32_t loop;
  
  /**
   *  Take the semaphore to take ownership of the Flash IP
   */
  while(LL_HSEM_1StepLock(HSEM, CFG_HW_FLASH_SEMID));

  HAL_FLASH_Unlock();
  
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP |FLASH_FLAG_ALL_ERRORS); 
  /* Enable EOP interrupt */
  __HAL_FLASH_ENABLE_IT(FLASH_IT_EOP);

  /**
   *  Notify the CPU2 that some flash erase activity may be executed
   *  On reception of this command, the CPU2 enables the BLE timing protection versus flash erase processing
   *  The Erase flash activity will be executed only when the BLE RF is idle for at least 25ms
   *  The CPU2 will prevent all flash activity (write or erase) in all cases when the BL RF Idle is shorter than 25ms.
   */
  SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_ON);

  for( loop = 0;  loop < n ; loop++)
  {
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);

    FD_EraseSingleSector(page+loop);
  }

  /* Disable EOP interrupt */
  __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);

  /* Clear the page erase bit */
  CLEAR_BIT(FLASH->CR, (FLASH_CR_PER | FLASH_CR_PNB));
  
  /**
   *  Notify the CPU2 there will be no request anymore to erase the flash
   *  On reception of this command, the CPU2 disables the BLE timing protection versus flash erase processing
   */
  SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_OFF);

  HAL_FLASH_Lock();

  /**
   *  Release the ownership of the Flash IP
   */
  LL_HSEM_ReleaseLock(HSEM, CFG_HW_FLASH_SEMID, 0);
    
  return (HW_OK);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE***/
