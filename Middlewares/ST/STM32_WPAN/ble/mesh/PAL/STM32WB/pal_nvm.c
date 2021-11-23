/**
******************************************************************************
* @file    pal_nvm.c
* @author  MCD Application Team
* @brief   Flash management for the Controller
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
#include "types.h"
#include "pal_nvm.h"
#include "pal_if.h"  
#include <string.h>

#include "ble.h"

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static MOBLEUINT32 GetPage(MOBLEUINT32 Addr)
{
  MOBLEUINT32 page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
* @brief  returns NVM write protect status
* @param  None
* @retval TRUE if flash is write protected
*/
MOBLEBOOL MoblePalNvmIsWriteProtected(void)
{
  FLASH_OBProgramInitTypeDef ob;
  
  ob.WRPArea = 0;
  HAL_FLASHEx_OBGetConfig(&ob);
  
  const int sector = (NVM_BASE - FLASH_BASE) / FLASH_SECTOR_SIZE;
  const int sectorMask = 1 << (sector & 0x1f);
  
  return ob.WRPStartOffset & sectorMask ? MOBLE_TRUE : MOBLE_FALSE;
}

/**
* @brief  Read NVM
* @param  address: read start address of nvm
* @param  offset: offset read start address of nvm
* @param  buf: copy of read content
* @param  size: size of memory to be read
* @param  backup: If read from backup memory
* @retval Result of read operation
*/
MOBLE_RESULT MoblePalNvmRead(MOBLEUINT32 address,
                             MOBLEUINT32 offset, 
                             void *buf, 
                             MOBLEUINT32 size, 
                             MOBLEBOOL backup)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
//  printf("MoblePalNvmRead >>>\r\n");  

  if (offset > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    memmove(buf, (void *)(address + offset), size);
  }
  
//  printf("MoblePalNvmRead <<<\r\n");  
  return result;
}

/**
* @brief  Compare with NVM
* @param  offset: start address of nvm to compare
* @param  offset: offset start address of nvm
* @param  buf: copy of content
* @param  size: size of memory to be compared
* @param  comparison: outcome of comparison
* @retval Result
*/
MOBLE_RESULT MoblePalNvmCompare(MOBLEUINT32 address,
                                MOBLEUINT32 offset, 
                                void const *buf, 
                                MOBLEUINT32 size, 
                                MOBLE_NVM_COMPARE* comparison)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
//  printf("MoblePalNvmCompare >>>\r\n");
  
  if ((comparison == NULL) || (buf == NULL))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (offset > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (offset & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    *comparison = MOBLE_NVM_COMPARE_EQUAL;
    size >>= 3;
    
    uint64_t* src = (uint64_t*)buf;
    uint64_t* dst = (uint64_t*)(address + offset);
    
    for (MOBLEUINT32 i=0; i<size; ++i)
    {
      if ((src[i<<3] != dst[i<<3]) && (*comparison == MOBLE_NVM_COMPARE_EQUAL))
      {
        *comparison = MOBLE_NVM_COMPARE_NOT_EQUAL;
      }
        if ((src[i<<3] & dst[i<<3]) != dst[i<<3])
        {
          *comparison = MOBLE_NVM_COMPARE_NOT_EQUAL_ERASE;
          break;
        }
    }
  }
  
//  printf("MoblePalNvmCompare <<<\r\n");
  
  return result;
}

/**
* @brief  Erase NVM
* @param  None
* @retval Result
*/
MOBLE_RESULT MoblePalNvmErase(MOBLEUINT32 address,
                              MOBLEUINT32 offset)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t pageError = 0;

//  printf("MoblePalNvmErase >>>\r\n");
  
  FLASH_EraseInitTypeDef erase;
  erase.TypeErase = FLASH_TYPEERASE_PAGES;
  erase.Page = GetPage(address + offset); /* 126 or 127 */;
  erase.NbPages = FLASH_SECTOR_SIZE >> 12;
  
  while( LL_HSEM_1StepLock( HSEM, CFG_HW_FLASH_SEMID ) );
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);
  
  SHCI_C2_FLASH_EraseActivity( ERASE_ACTIVITY_ON);

  while(LL_FLASH_IsActiveFlag_OperationSuspended());
  status = HAL_FLASHEx_Erase(&erase, &pageError);
  while(LL_FLASH_IsActiveFlag_OperationSuspended());
  
  SHCI_C2_FLASH_EraseActivity( ERASE_ACTIVITY_OFF);

  HAL_FLASH_Lock();
  LL_HSEM_ReleaseLock( HSEM, CFG_HW_FLASH_SEMID, 0 );
  
//  printf("MoblePalNvmErase <<<\r\n");
  
  return status == HAL_OK ? MOBLE_RESULT_SUCCESS : MOBLE_RESULT_FAIL;
}

/**
* @brief  Write to NVM
* @param  offset: wrt start address of nvm
* @param  offset: offset wrt start address of nvm
* @param  buf: copy of write content
* @param  size: size of memory to be written
* @retval Result
*/
MOBLE_RESULT MoblePalNvmWrite(MOBLEUINT32 address,
                              MOBLEUINT32 offset, 
                              void const *buf, 
                              MOBLEUINT32 size)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
//  printf("MoblePalNvmWrite >>>\r\n");
  
  if (offset > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (offset & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    size >>= 3;
    
    uint64_t* src = (uint64_t*)buf;
//    uint64_t* dst = (uint64_t*)(address + offset);
    
    HAL_StatusTypeDef status = HAL_OK;
    
    while( LL_HSEM_1StepLock( HSEM, CFG_HW_FLASH_SEMID ) );
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);
    for (size_t i = 0; (i < size) && (status == HAL_OK); i++)
      {
//      if (src[i<<3] != dst[i<<3])
//      {
        while(LL_FLASH_IsActiveFlag_OperationSuspended());
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + offset + (i <<3), src[i]);
        if (status != HAL_OK)
        {
          break;
        }
//      }
    }
    HAL_FLASH_Lock();
    LL_HSEM_ReleaseLock( HSEM, CFG_HW_FLASH_SEMID, 0 );
    
    if (HAL_OK != status)
    {
      result = MOBLE_RESULT_FAIL;
    }
  }
//  printf("MoblePalNvmWrite <<<\r\n");
  
  return result;
}

/**
* @brief  NVM process
* @param  None
* @retval Result
*/
MOBLE_RESULT BnrgmPalNvmProcess(void)
{
    /* do nothing */
    return MOBLE_RESULT_SUCCESS;
}


