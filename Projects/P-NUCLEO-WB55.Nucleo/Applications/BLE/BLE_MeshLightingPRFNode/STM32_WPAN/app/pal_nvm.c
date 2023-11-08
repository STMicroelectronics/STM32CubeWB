/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    pal_nvm.c
  * @author  MCD Application Team
  * @brief   Flash management for the Controller
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
#include "types.h"
#include "pal_nvm.h"
#include "pal_if.h"  
#include <string.h>

#include "ble.h"
#include "shci.h"
#include "mesh_cfg_usr.h"

#include "flash_driver.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

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
MOBLEBOOL PalNvmIsWriteProtected(void)
{
    /* All flash is writable */
    return MOBLE_FALSE;
}

/**
* @brief  Read NVM
* @param  address: read start address of nvm
* @param  buf: copy of read content
* @param  size: size of memory to be read
* @param  backup: If read from backup memory
* @retval Result of read operation
*/
MOBLE_RESULT PalNvmRead(MOBLEUINT32 address,
                        void *buf, 
                        MOBLEUINT32 size, 
                        MOBLEBOOL backup)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
//  printf("MoblePalNvmRead >>>\r\n");  

  if (address > (NVM_BASE + NVM_SIZE))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if ((address + size) > (NVM_BASE + NVM_SIZE))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    memcpy(buf, (void *)(address), size);
  }
  
//  printf("MoblePalNvmRead <<<\r\n");  
  return result;
}

/**
* @brief  Compare with NVM
* @param  offset: start address of nvm to compare
* @param  buf: copy of content
* @param  size: size of memory to be compared
* @param  comparison: outcome of comparison
* @retval Result
*/
MOBLE_RESULT PalNvmCompare(MOBLEUINT32 address,
                           void const *buf, 
                           MOBLEUINT32 size, 
                           MOBLE_NVM_COMPARE* comparison)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT32 i;

//  printf("MoblePalNvmCompare >>>\r\n");
  
  if ((comparison == NULL) || (buf == NULL))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (address > (NVM_BASE + NVM_SIZE))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if ((address + size) > (NVM_BASE + NVM_SIZE))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (address & 3)
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
    size >>= 2;

    MOBLEUINT32 * src = (MOBLEUINT32*)buf;
    MOBLEUINT32 * dst = (MOBLEUINT32*)(address);
    
    i = 0;
    do
    {
      if ((src[i] != dst[i]) && (*comparison == MOBLE_NVM_COMPARE_EQUAL))
      {
        *comparison = MOBLE_NVM_COMPARE_NOT_EQUAL;
      }
      i++;
    } while((*comparison != MOBLE_NVM_COMPARE_NOT_EQUAL) && (i < size));
  }
  
//  printf("MoblePalNvmCompare <<<\r\n");
  return result;
}

/**
* @brief  Erase NVM
* @param  None
* @retval Result
*/
MOBLE_RESULT PalNvmErase(MOBLEUINT32 address,
                         MOBLEUINT8 nb_pages)
{
  if(FD_EraseSectors(GetPage(address), nb_pages) != nb_pages)
  {
    return MOBLE_RESULT_FAIL;
  }

  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Write to NVM
* @param  offset: wrt start address of nvm
* @param  buf: copy of write content
* @param  size: size of memory to be written
* @retval Result
*/
MOBLE_RESULT PalNvmWrite(MOBLEUINT32 address,
                          void const *buf, 
                          MOBLEUINT32 size)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;

  if (address > (NVM_BASE + NVM_SIZE))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if ((address + size) > (NVM_BASE + NVM_SIZE))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (address & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    MOBLEUINT32 remain = 0;
    MOBLEUINT32 nb_dword = 0;
  
    nb_dword = (size >> 3);
    remain = size - ((nb_dword) << 3);
    if(remain > 0)
      nb_dword += 1;
    
  if(FD_WriteData(address, (uint64_t*)buf, nb_dword) != 0)
    result = MOBLE_RESULT_FAIL;
  }
  
  return result;
}

#if 0
/**
* @brief  Backup process
* @param  None
* @retval Result
*/
static MOBLE_RESULT PalNvmBackupProcess(void)
{
  MOBLEUINT32 buff[4*N_BYTES_WORD];
  static MOBLEUINT8 backup_pages_to_be_erased = 0;    
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    
  if (backup_pages_to_be_erased == 0)
  {
    backup_pages_to_be_erased = 1;
  }
      
  if(backup_pages_to_be_erased != 0)
  {
#if 0
    BLEMesh_StopAdvScan();
    ATOMIC_SECTION_BEGIN();
    if(BluenrgMesh_IsFlashReadyToErase())
    {
      FLASH_ErasePage((uint16_t)((BNRGM_NVM_BACKUP_BASE - 
                                  RESET_MANAGER_FLASH_BASE_ADDRESS) / PAGE_SIZE +
                       BNRGM_NVM_BACKUP_SIZE/PAGE_SIZE - 
                       backup_pages_to_be_erased));
         
      if (FLASH_GetFlagStatus(Flash_CMDERR) == SET)
      {
        result = MOBLE_RESULT_FAIL;
      }
      else
      {
        backup_pages_to_be_erased--;
      }
    }
    else
    {
      /* do nothing */
    }
    ATOMIC_SECTION_END();
#else
    result = PalNvmErase(NVM_BASE, FLASH_SECTOR_SIZE);
    if(result == MOBLE_RESULT_SUCCESS)
      backup_pages_to_be_erased = 0;
#endif
  }
    
  if (result == MOBLE_RESULT_SUCCESS && backup_pages_to_be_erased == 0)
  {
#if 0
    BLEMesh_StopAdvScan();
    ATOMIC_SECTION_BEGIN();
    if(BluenrgMesh_IsFlashReadyToErase())
    {
      for (size_t i = 0; i < BNRGM_NVM_BACKUP_SIZE && FLASH_GetFlagStatus(Flash_CMDERR) == RESET; )
      {
        memcpy((MOBLEUINT8*)buff, (void *)(BNRGM_NVM_BASE + i), 4*N_BYTES_WORD);
        FLASH_ProgramWordBurst(BNRGM_NVM_BACKUP_BASE + i, (uint32_t*)buff);
        i += 4*N_BYTES_WORD;
      }
          
      if (FLASH_GetFlagStatus(Flash_CMDERR) == SET)
      {
        result = MOBLE_RESULT_FAIL;
      }
      else
      {
        PalNvmReqs.backup_req = MOBLE_FALSE;
      }
    }
    else
    {
      /* do nothing */
    }
    ATOMIC_SECTION_END();
#else
#endif
  }
 return result;
}
#endif

/**
* @brief  NVM process
* @param  None
* @retval Result
*/
MOBLE_RESULT PalNvmProcess(void)
{
  /* do nothing */
  return MOBLE_RESULT_SUCCESS;
}

