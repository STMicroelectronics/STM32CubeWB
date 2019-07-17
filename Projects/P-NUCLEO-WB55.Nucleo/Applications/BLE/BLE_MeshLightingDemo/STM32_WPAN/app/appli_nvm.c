/**
	******************************************************************************
	* @file    appli_nvm.c
	* @author  BLE Mesh Team
	* @brief   User Application file 
	******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "hal_common.h"
#include "appli_nvm.h"
#include "mesh_cfg.h"
#include "appli_mesh.h"

extern const MOBLEUINT8* _bdaddr[];
extern const void* bnrgmNvmBase;

/* Reserved for Bluenrg-Mesh library */
#define BLUENRGMESH_NVM_BASE               ((unsigned int)bnrgmNvmBase)
#define BLUENRGMESH_NVM_BACKUP_BASE        (BLUENRGMESH_NVM_BASE + PAGE_SIZE)

/*
*  Page of size 2k, BlueNRG-1 and BlueNRG-2, reserved for application is divided into 8 subpages of each 256 bytes
*  First subpage is reserved (for e.g. External MAC is present in this area)
*  Rest 7 subpages are used on rolling basis for application states. 
*    First byte of each subpage indicates if page is valid or not
*/
#define APP_NVM_BASE                      ((unsigned int)_bdaddr)
#define APP_NVM_RESERVED_SIZE             256U
#define APP_NVM_SUBPAGE_SIZE              256U
#define APP_NVM_MAX_SUBPAGE               7U
#define APP_NVM_SUBPAGE_OFFSET(i)         (unsigned int)(APP_NVM_BASE+APP_NVM_RESERVED_SIZE+256*i)

/* offsets defined wrt start of subpage */
#define APP_NVM_VALID_FLAG_OFFSET         0U
#define APP_NVM_VALID_FLAG_SIZE           4U
#define APP_NVM_RESET_COUNT_OFFSET        (unsigned int)APP_NVM_VALID_FLAG_SIZE
#define APP_NVM_RESET_COUNT_SIZE          12U /* 12 bytes reserved for write cycle optimization */
#define APP_NVM_GENERIC_MODEL_OFFSET      (unsigned int)(APP_NVM_VALID_FLAG_SIZE+APP_NVM_RESET_COUNT_SIZE)
#define APP_NVM_GENERIC_MODEL_SIZE        16U
#define APP_NVM_LIGHT_MODEL_OFFSET        (unsigned int)(APP_NVM_VALID_FLAG_SIZE+APP_NVM_RESET_COUNT_SIZE+APP_NVM_GENERIC_MODEL_SIZE)
#define APP_NVM_LIGHT_MODEL_SIZE          16U

MOBLE_RESULT AppliNvm_EraseRestoreResvNvm(void);

/**
* @brief  This function erases a specified flash page
* @param  void
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FlashErase(void)
{
  if(BluenrgMesh_IsFlashReadyToErase())
  {
    BLEMesh_StopAdvScan();
    FLASH_ErasePage((uint16_t)((APP_NVM_BASE - RESET_MANAGER_FLASH_BASE_ADDRESS) / PAGE_SIZE));
    
    if (FLASH_GetFlagStatus(Flash_CMDERR) == SET)
    {
      return MOBLE_RESULT_FAIL;
    }
    else
    {
      return MOBLE_RESULT_SUCCESS;
    }
  }
  else
  {
    return MOBLE_RESULT_FALSE;
  }
}

/** 
*  @brief  Program word (32-bit) at a APP_NVM_BASE + APP_NVM_RESERVED_SIZE
*  @param  TypeProgram  Indicate the way to program at a specified address
*  @param  Address: address to write
*  @param  Data: word to write
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FlashProgram(uint32_t TypeProgram, uint32_t Address, uint32_t Data[])
{
   if (Address < (APP_NVM_BASE) 
       || Address > (APP_NVM_BASE + PAGE_SIZE) 
         || (Address & 0x03 != 0))
   {   
     return MOBLE_RESULT_INVALIDARG;
   }
   else
   {
     if(TypeProgram == FLASH_WRITE)
     {
       /* Program 4 bytes (32-bit) at a specified address.*/
       FLASH_ProgramWord(Address, (uint32_t) Data[0]);
     }
     else if(TypeProgram == FLASH_BURSTWRITE)
     {
       /*Program 4 words (128-bit) at a specified address.*/
       FLASH_ProgramWordBurst(Address, (uint32_t*) Data);
     }  
   }
   
   if (FLASH_GetFlagStatus(Flash_CMDERR) == SET)
   {
     return MOBLE_RESULT_FAIL;
   }
   
   return MOBLE_RESULT_SUCCESS; 
}

/**
*  @brief  Find first valid subpage available. 
*          If no valid subpage found, erase page appli nvm to reset it
*  @param  pointer to variable to update first valid subpage index
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FindFirstValidSubPage(MOBLEINT8* subPageIndex)
{
  MOBLEUINT32 valid;
  MOBLEINT8 subPageIdx = -1;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* find valid subpage */
  for (MOBLEUINT8 count=0; count<APP_NVM_MAX_SUBPAGE; count++)
  {
    memcpy((void*)&valid, (void*)(APP_NVM_SUBPAGE_OFFSET(count)), 4);
    if (valid == 0xFFFFFFFF)
    {
      subPageIdx = count;
      break;
    }
  }
  
  /* if no valid subpage found, erase all subpages and reset subPageIdx */
  if (subPageIdx < 0)
  {
    result = AppliNvm_EraseRestoreResvNvm();
    subPageIdx = 0;
  }
  else
  {
    result = MOBLE_RESULT_SUCCESS;
  }

  *subPageIndex = subPageIdx;
  
  return result;
}

/**
*  @brief  Mark first valid subpage as invalid. 
*          If no valid subpage found, erase page appli nvm to reset it
*          Also copy model data from current subpage into new subpage
*          It is assumed that all models are consecutive in memory
*  @param  model backup flag. If true, copy model states from current subpage to new subpage
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_MarkSubpageInvalid(MOBLEBOOL modelBackup)
{
  MOBLEUINT32 valid = 0;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEINT8 currSubPageIdx;
  MOBLEINT8 newSubPageIdx;
  MOBLEUINT8 modelBackupData[APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE];
  
  /* find valid subpage */
  result = AppliNvm_FindFirstValidSubPage(&currSubPageIdx);
  
  if (MOBLE_FAILED(result))
  {
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    if (modelBackup == MOBLE_TRUE)
    {
      /* copy model data from current subpage*/
      memcpy((void*)modelBackupData,
             (void*)(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET),
             APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE);
    }
    
    /* mark valid subpage as invalid */
    AppliNvm_FlashProgram(FLASH_WRITE, 
                          APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_VALID_FLAG_OFFSET,
                          (uint32_t*)&valid);
    
    /* find next valid subpage */
    result = AppliNvm_FindFirstValidSubPage(&newSubPageIdx);
    
    if (MOBLE_FAILED(result))
    {
      result = MOBLE_RESULT_FAIL;
    }
    else if (modelBackup == MOBLE_TRUE)
    {
      /* update model data in new subpage */
      for (MOBLEUINT32 count=0; count<APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE; )
      {
        if (FLASH_GetFlagStatus(Flash_CMDERR) == RESET)
        {
          /* If size to be written > 4 words, use FLASH_ProgramWordBurst*/
          if (APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE-count >= 4*N_BYTES_WORD)
          {
            AppliNvm_FlashProgram(FLASH_BURSTWRITE, 
                                  APP_NVM_SUBPAGE_OFFSET(newSubPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET+count, 
                                  (uint32_t*)&modelBackupData[count]);
            count += 4*N_BYTES_WORD;
          }
          /* If size to be written < 4 words, use FLASH_ProgramWord*/
          else
          {
            AppliNvm_FlashProgram(FLASH_WRITE, 
                                  APP_NVM_SUBPAGE_OFFSET(newSubPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET+count,
                                  (uint32_t*)&modelBackupData[count]);
            count += N_BYTES_WORD;
          }
        }
        else
        {
          /* Flash command error occurred */
          result = MOBLE_RESULT_FAIL;            
          break;
        }
      }
    }
  }
  
  return result;
}

/**
* @brief  This function writes Reset count value in flash at APP_NVM_BASE + APP_NVM_RESERVED_SIZE
*         if device has been turned ON 5 times consecutively with each ON duration < 2s, reset to factory settings
*         LED blinks 2 times to indicate device startup without reset
*         LED blinks 5 times to indicate reset at device startup
* @param  void
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FactorySettingReset(void)
{
  MOBLEINT8 currSubPageIdx;
  MOBLE_RESULT result;
  MOBLEUINT32 resetCount;
    
  result = AppliNvm_FindFirstValidSubPage(&currSubPageIdx);
  
  if (MOBLE_FAILED(result))
  {
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    /* read saved reset counter from subpage */
    memcpy((void*)&resetCount, (void*)(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_RESET_COUNT_OFFSET), 4);
    resetCount = resetCount << 1; /* Shifting for making Bits 0 from LSB */
    
    /* Check for 5 times, 5 LSB are not zero */
    if (resetCount > 0xFFFFFFE0)
    {
      /* update reset count */
      AppliNvm_FlashProgram(FLASH_WRITE, 
                            APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_RESET_COUNT_OFFSET,
                            (uint32_t*)&resetCount);
          
      if (!memcmp((void*)&resetCount, (void*)(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_RESET_COUNT_OFFSET), 4))
      {
        /* updated value of reset count in flash */
      }
      
      /* Blink twice to indicate device startup 
         1 second to blink once and 2 seconds to blink twice */
      for (MOBLEUINT8 i=0; i<2; i++)
      {
        Appli_LedBlink();
      }
      
      result = AppliNvm_MarkSubpageInvalid(MOBLE_TRUE);
      
      if (MOBLE_FAILED(result))
      {
        result = MOBLE_RESULT_FAIL;
      }
    }
    else /* Device is forced to factory reset, 5 LSBs are zero */
    {
      /* Unprovision node */
      BluenrgMesh_Unprovision();
      
      /* Clear lib data, primary and backup nvm used by BlueNRG-Mesh lib */
      FLASH_ErasePage((uint16_t)((BLUENRGMESH_NVM_BASE - RESET_MANAGER_FLASH_BASE_ADDRESS) / PAGE_SIZE));
      FLASH_ErasePage((uint16_t)((BLUENRGMESH_NVM_BACKUP_BASE - RESET_MANAGER_FLASH_BASE_ADDRESS) / PAGE_SIZE));
      
      AppliNvm_ClearModelState();
      
      result = AppliNvm_MarkSubpageInvalid(MOBLE_FALSE);
      
      if (MOBLE_FAILED(result))
      {
        result = MOBLE_RESULT_FAIL;
      }
      
      BluenrgMesh_SetUnprovisionedDevBeaconInterval(100);
      
      /* Blink 5 times to indicate factory setting reset */
      for (MOBLEUINT8 i=0; i<5; i++)
      {
        Appli_LedBlink();
      }
      
      TRACE_M(TF_MISC, "Reset to factory settings \r\n");
    }
    
    result = MOBLE_RESULT_SUCCESS;
  }
  
  return result;
}

/**
* @brief  This function erases application Flash page data only keeping reserved Nvm data 
*         located at APP_NVM_BASE + APP_NVM_RESERVED_SIZE
* @param  void
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_EraseRestoreResvNvm(void)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 reserveAreaCopy[APP_NVM_RESERVED_SIZE];
  
  /* save reserve flash area */
  memcpy((void*)reserveAreaCopy, (void*)APP_NVM_BASE, APP_NVM_RESERVED_SIZE);
    
  /* erase page */
  FLASH_ErasePage((uint16_t)((APP_NVM_BASE - RESET_MANAGER_FLASH_BASE_ADDRESS) / PAGE_SIZE));

  if (FLASH_GetFlagStatus(Flash_CMDERR) == SET)
  {
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    /* restore reserve area */
    for (MOBLEUINT32 count=0; count<APP_NVM_RESERVED_SIZE; )
    {
      if (FLASH_GetFlagStatus(Flash_CMDERR) == RESET)
      {
        /* If size to be written > 4 words, use FLASH_ProgramWordBurst*/
        if (APP_NVM_RESERVED_SIZE-count >= 4*N_BYTES_WORD)
        {
          AppliNvm_FlashProgram(FLASH_BURSTWRITE,APP_NVM_BASE + count, (uint32_t*)&reserveAreaCopy[count]);
          count += 4*N_BYTES_WORD;
        }
        /* If size to be written < 4 words, use FLASH_ProgramWord*/
        else
        {
          AppliNvm_FlashProgram(FLASH_WRITE,APP_NVM_BASE + count, (uint32_t*)&reserveAreaCopy[count]);
          count += N_BYTES_WORD;
        }
      }
      else
      {
        /* Flash command error occurred */
        result = MOBLE_RESULT_FAIL;            
        break;
      }
    }
  }

  return result;
}

/**
* @brief  Save model states in nvm
* @param  model state buff
* @param  model state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_SaveModelState(uint8_t* state, uint8_t size)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL; /* if save model state not defined, return MOBLE_RESULT_FAIL */
  
#if (SAVE_MODEL_STATE_NVM == 1)

  MOBLEINT8 subPageIdx;
  
  if (size > APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE)
  {
    /* incorrect size */
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    AppliNvm_MarkSubpageInvalid(MOBLE_FALSE);
    result = AppliNvm_FindFirstValidSubPage(&subPageIdx);
    
    if (MOBLE_FAILED(result))
    {
      result = MOBLE_RESULT_FAIL;
    }
    else
    {
      /* update models states */
      for (MOBLEUINT32 count=0; count<size; )
      {
        if (FLASH_GetFlagStatus(Flash_CMDERR) == RESET)
        {
          /* If size to be written > 4 words, use FLASH_ProgramWordBurst*/
          if (APP_NVM_RESERVED_SIZE-count >= 4*N_BYTES_WORD)
          {
            result = AppliNvm_FlashProgram(FLASH_BURSTWRITE, 
                                           APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET+count, 
                                           (uint32_t*)&state[count]);
            count += 4*N_BYTES_WORD;
          }
          /* If size to be written < 4 words, use FLASH_ProgramWord*/
          else
          {
            result = AppliNvm_FlashProgram(FLASH_WRITE, 
                                           APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET+count, 
                                           (uint32_t*)&state[count]);
            count += N_BYTES_WORD;
          }
        }
        else
        {
          /* Flash command error occurred */
          result = MOBLE_RESULT_FAIL;            
          break;
        }
      }
      
      for (MOBLEUINT8 count=0; count<size; count++)
      {
        TRACE_M(TF_MISC, "%.2x ", *((MOBLEUINT8*)((APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET+count))));
      }
      
      TRACE_M(TF_MISC,"\r\n"); 

    }
  }
#endif /* SAVE_MODEL_STATE_NVM */  
  return result;
}

/**
* @brief  Clear model states in nvm
* @param  none
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_ClearModelState(void)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL; /* if save model state not defined, return MOBLE_RESULT_FAIL */
  
#if (SAVE_MODEL_STATE_NVM == 1)
  MOBLEINT8 subPageIdx;
  MOBLEUINT8 clearBuff[16] = {0};
  
  result = AppliNvm_FindFirstValidSubPage(&subPageIdx);
    
  if (MOBLE_FAILED(result))
  {
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    /* clear models states */
    for (MOBLEUINT32 count=0; count<APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE; )
    {
      if (FLASH_GetFlagStatus(Flash_CMDERR) == RESET)
      {
        /* If size to be written > 4 words, use FLASH_ProgramWordBurst*/
        if (APP_NVM_RESERVED_SIZE-count >= 4*N_BYTES_WORD)
        {
          result = AppliNvm_FlashProgram(FLASH_BURSTWRITE, 
                                         APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET+count, 
                                         (uint32_t*)&clearBuff[count]);
          count += 4*N_BYTES_WORD;
        }
        /* If size to be written < 4 words, use FLASH_ProgramWord*/
        else
        {
          result = AppliNvm_FlashProgram(FLASH_WRITE, 
                                         APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET+count, 
                                         (uint32_t*)&clearBuff[count]);
          count += N_BYTES_WORD;
        }
      }
      else
      {
        /* Flash command error occurred */
        result = MOBLE_RESULT_FAIL;            
        break;
      }
    }
  }
#endif /* SAVE_MODEL_STATE_NVM */  
  return result;
}

/**
* @brief  Load generic states from nvm
* @param  generic state buff
* @param  generic state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_LoadGenericState(uint8_t state[], uint8_t* size)
{  
#if (SAVE_MODEL_STATE_NVM == 1)  
  MOBLEINT8 subPageIdx;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  result = AppliNvm_FindFirstValidSubPage(&subPageIdx);
  
  if (MOBLE_FAILED(result))
  {
    *size = 0;
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    /* read saved reset counter from subpage */
    memcpy((void*)state, (void*)(APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET), APP_NVM_GENERIC_MODEL_SIZE);
    *size = APP_NVM_GENERIC_MODEL_SIZE;
  }

  for (MOBLEUINT8 count=0; count<(*size); count++)
  {
     TRACE_M(TF_MISC, "%.2x ", *(state+count));
  }
      
  TRACE_M(TF_MISC,"\r\n"); 
  
  return result;  
#else
  *size = 0;
  return MOBLE_RESULT_SUCCESS;
#endif /* SAVE_MODEL_STATE_NVM */
}

/**
* @brief  Load light states from nvm
* @param  light state buff
* @param  light state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_LoadLightState(uint8_t state[], uint8_t* size)
{
#if (SAVE_MODEL_STATE_NVM == 1)  
  MOBLEINT8 subPageIdx;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  result = AppliNvm_FindFirstValidSubPage(&subPageIdx);
  
  if (MOBLE_FAILED(result))
  {
    *size = 0;
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    /* read saved reset counter from subpage */
    memcpy((void*)state, (void*)(APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_LIGHT_MODEL_OFFSET), APP_NVM_LIGHT_MODEL_SIZE);
    *size = APP_NVM_LIGHT_MODEL_SIZE;
  }

  for (MOBLEUINT8 count=0; count<(*size); count++)
  {
     TRACE_M(TF_MISC, "%.2x ", *(state+count));
  }
      
  TRACE_M(TF_MISC,"\r\n"); 
  
  return result;  
#else
  *size = 0;
  return MOBLE_RESULT_SUCCESS;
#endif /* SAVE_MODEL_STATE_NVM */
}

