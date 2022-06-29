/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    appli_nvm.c
  * @author  MCD Application Team
  * @brief   User Application file 
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
#include <string.h>
#include "hal_common.h"
#include "ble_mesh.h"
#include "mesh_cfg_usr.h"
#include "appli_nvm.h"
#include "mesh_cfg.h"
#include "pal_nvm.h"
#include "appli_mesh.h"
#ifdef SAVE_MODEL_STATE_FOR_ALL_MESSAGES
#include "common.h"
#endif
#if (LOW_POWER_FEATURE == 1)
#include "app_conf.h"
#include "stm32_seq.h"
#endif
#include "config_client.h"

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/
/* Private define ------------------------------------------------------------*/

extern const MOBLEUINT8* _bdaddr[];

#ifdef SAVE_MODEL_STATE_FOR_ALL_MESSAGES
extern MOBLEUINT8 PowerOnOff_flag;
#endif

extern const void* appNvmBase;


/*
*  Page of size 4k, STM32WB, reserved for application is divided into 16 subpages of each 256 bytes
*  First subpage is reserved (for e.g. External MAC is present in this area)
*  Rest 15 subpages are used on rolling basis for application states. 
*    First byte of each subpage indicates if page is valid or not
*/

#define APP_NVM_RESERVED_SIZE             256U
#define APP_NVM_SUBPAGE_SIZE              256U
#define APP_NVM_MAX_SUBPAGE               15U
#define APP_NVM_SUBPAGE_OFFSET(i)         (unsigned int)(APP_NVM_RESERVED_SIZE + APP_NVM_SUBPAGE_SIZE*(i))

/* offsets defined wrt start of subpage */
#define APP_NVM_VALID_FLAG_OFFSET         0U
#define APP_NVM_VALID_FLAG_SIZE           4U
#define APP_NVM_RESET_COUNT_OFFSET        (unsigned int)APP_NVM_VALID_FLAG_SIZE
#define APP_NVM_RESET_COUNT_SIZE          12U /* 12 bytes reserved for write cycle optimization */
#define APP_NVM_GENERIC_MODEL_OFFSET      (unsigned int)(APP_NVM_VALID_FLAG_SIZE+APP_NVM_RESET_COUNT_SIZE)
//#define APP_NVM_GENERIC_MODEL_SIZE        16U
#define APP_NVM_LIGHT_MODEL_OFFSET        (unsigned int)(APP_NVM_VALID_FLAG_SIZE+APP_NVM_RESET_COUNT_SIZE+APP_NVM_GENERIC_MODEL_SIZE)
//#define APP_NVM_LIGHT_MODEL_SIZE          16U

#define FLASH_EMPTY_SIGNATURE       0xFFFFFFFF 

#define RADIO_OFF_DELAY             1000

/* Private variables ---------------------------------------------------------*/

typedef struct
{
  MOBLEUINT8 modelData[APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE];
  MOBLEBOOL erasePageReq;
  MOBLEBOOL writeReq;
} APPLI_NVM_REQS;

/* ALIGN(4) */
__attribute__((aligned(4))) APPLI_NVM_REQS AppliNvm_Reqs;
MOBLEUINT16 StopRadioInProgress;

MOBLE_RESULT AppliNvm_EraseRestoreResvNvm(void);
MOBLE_RESULT AppliNvm_FindFirstEmptyPage(MOBLEINT16* subPageIndex,
                                         MOBLEUINT32 SubPageSize,
                                         MOBLEUINT32 totalSubPage, 
                                         MOBLEUINT32 nvmBaseOffset);
MOBLE_RESULT AppliNvm_MarkSubpageInvalid(void);
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, 
                                   void const *buf, MOBLEUINT32 size);

#if 0 //#ifdef ENABLE_NVM_TEST
/**
* @brief  This function erases a specified flash page
* @param  Page Number
* @retval MOBLE_RESULT_INVALIDARG if PageNumber overflow
*         MOBLE_RESULT_FALSE if flash operation is very close to next radio event
*         MOBLE_RESULT_FAIL if flash command error is set
*         MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FlashErase(uint16_t PageNumber)
{
  MOBLE_RESULT result;
  
  if (PageNumber < FLASH_PAGE_NB)
  {
    BLEMesh_StopAdvScan();
    ATOMIC_SECTION_BEGIN();
    result =  PalNvmErase(APP_NVM_BASE, 1);
    ATOMIC_SECTION_END();
  }
  else /* Invalid page no */
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  
  return result;
}
#endif

/** 
*  @brief  Program word (32-bit) at a APP_NVM_BASE + APP_NVM_RESERVED_SIZE
*  @param  TypeProgram  Indicate the way to program at a specified address
*  @param  Address: address to write
*  @param  Data: word to write
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, 
                                   void const *buf, 
                                   MOBLEUINT32 size)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  if (offset > PAGE_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > (APP_NVM_BASE + PAGE_SIZE))
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
    result = PalNvmWrite(APP_NVM_BASE + offset, 
                         buf, 
                         size);
  }
  
  return result;
}


/**
*  @brief  Mark first valid subpage as invalid. 
*  @param  void
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_MarkSubpageInvalid(void)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
#if 0
  MOBLEUINT32 valid = 0;
  MOBLEINT16 currSubPageIdx;
  MOBLEUINT8 subPageTemp[APP_NVM_SUBPAGE_SIZE];
  
  /* find valid subpage */
  result = AppliNvm_FindFirstEmptyPage(&currSubPageIdx,
                                       APP_NVM_SUBPAGE_SIZE,
                                       APP_NVM_MAX_SUBPAGE,
                                       (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));
                                        
  
  if (result == MOBLE_RESULT_OUTOFMEMORY)
  {
    AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
  }
  else
  {
    /* mark valid subpage as invalid */
    memcpy((void*)subPageTemp,
           (void*)(PRVN_NVM_BASE_OFFSET + APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)),
           APP_NVM_SUBPAGE_SIZE);
    subPageTemp[APP_NVM_VALID_FLAG_OFFSET] = valid;
    result = AppliNvm_FlashProgram(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx),
                                   (uint32_t*)&subPageTemp, 
                                   APP_NVM_SUBPAGE_SIZE);
    if (MOBLE_FAILED(result))
    {
      result = MOBLE_RESULT_FAIL;
    }
    else if (result == MOBLE_RESULT_FALSE)
    {
      /* radio busy, try again later */
    }
    else
    {
      /* do nothing */
    }
  }
#endif  
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
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
#if 0
  MOBLEINT16 currSubPageIdx = 0;
  MOBLEUINT32 resetCount;
  MOBLEUINT8 subPageTemp[APP_NVM_SUBPAGE_SIZE];
    
  result = AppliNvm_FindFirstEmptyPage(&currSubPageIdx,APP_NVM_SUBPAGE_SIZE,APP_NVM_MAX_SUBPAGE,
                                        (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));
  
  if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
    AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
    UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    if(currSubPageIdx > 0)
    { 
      /* Not the first reset */
      /* read the previous subpage */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(currSubPageIdx-1)),
             APP_NVM_SUBPAGE_SIZE);
    }
    else
    {
      /* read the current subpage */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)),
             APP_NVM_SUBPAGE_SIZE);
    }
    
    /* read reset count from the subpage */
    memcpy((void*)&resetCount,
           (void*)&(subPageTemp[APP_NVM_RESET_COUNT_OFFSET]),
           4);
    resetCount = resetCount << 1; /* Shifting for making Bits 0 from LSB */
    memcpy((void*)&(subPageTemp[APP_NVM_RESET_COUNT_OFFSET]), (void*)&resetCount, 4);
    
    /* Check for "POWER_ON_OFF_CYCLES_FOR_UNPROVISIONING" times, that many bits shall be zero */
    if (resetCount > MASK_BITS_FOR_POWER_ON_OFF_CYCLES)
    {
      /* MASK_BITS_FOR_POWER_ON_OFF_CYCLES = 0xFFFFFFE0 */
      
      /* update reset count */
      result = AppliNvm_FlashProgram(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx),
                                     (uint32_t*)&subPageTemp, 
                                     APP_NVM_SUBPAGE_SIZE);
          
      if (!memcmp((void*)&subPageTemp[APP_NVM_RESET_COUNT_OFFSET],
                  (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_RESET_COUNT_OFFSET), 
                  4))
      {
        /* updated value of reset count in flash */
      }
      
      /* Blink twice to indicate device startup 
         1 second to blink once and 2 seconds to blink twice */
      for (MOBLEUINT8 i=0; i< ON_TIME_IN_SECONDS_FOR_POWER_CYCLING; i++)
      {
        Appli_LedBlink();
      }
      
      /* Load model data copy from nvm */
      memcpy((void*)AppliNvm_Reqs.modelData, 
             (void*)(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET), 
             APP_NVM_MODELDATA_PER_ELEMENT_SIZE);
      
      result = AppliNvm_MarkSubpageInvalid();
      
      if (MOBLE_FAILED(result))
      {
        result = MOBLE_RESULT_FAIL;
      }
    }
    else /* Device is forced to factory reset, 5 LSBs are zero */
    {
      /* Unprovision node */
      BLEMesh_Unprovision();
      
      /* Clear lib data, primary and backup nvm used by BLE-Mesh lib */
      PalNvmErase(NVM_BASE, 1);      
      PalNvmErase(NVM_BASE + PAGE_SIZE, 1);
      
      AppliNvm_ClearModelState();
      
      BLEMesh_SetUnprovisionedDevBeaconInterval(100);
      
      /* Blink 5 times to indicate factory setting reset */
      for (MOBLEUINT8 i=0; i<BLINK_TIMES_FOR_FACTORY_RESET; i++)
      {
        Appli_LedBlink();
      }
      
      TRACE_M(TF_MISC, "Reset to factory settings \r\n");
    }
    
    result = MOBLE_RESULT_SUCCESS;
  }
#endif
  return result;
}


/**
* @brief  Save model states in nvm
* @param  model state buff
* @param  model state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_SaveModelState(uint8_t* state, 
                                     uint16_t size)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; /* if save model state not defined, return MOBLE_RESULT_FAIL */
  
#if (SAVE_MODEL_STATE_NVM == 1)

  if (size > APP_NVM_MODEL_SIZE)
  {
    /* incorrect size */
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    memcpy(AppliNvm_Reqs.modelData, 
           state, 
           size);
    
    AppliNvm_Reqs.writeReq = MOBLE_TRUE;
    AppliNvm_Reqs.erasePageReq = MOBLE_FALSE;
#if (LOW_POWER_FEATURE == 1)
    UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
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
  MOBLEUINT8 subPageTemp[APP_NVM_SUBPAGE_SIZE];
  MOBLEINT16 subPageIdx;
  MOBLEUINT8 clearBuff[APP_NVM_MODELDATA_PER_ELEMENT_SIZE] = {0};
  MOBLEUINT32 valid = 0;
  
  result = AppliNvm_FindFirstEmptyPage(&subPageIdx,
                                       APP_NVM_SUBPAGE_SIZE,
                                       APP_NVM_MAX_SUBPAGE,
                                       (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));
    
  if (MOBLE_FAILED(result))
  {
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    if(subPageIdx > 0)
    { /* Not the first subpage */
      /* clear models states */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx-1)),
             APP_NVM_SUBPAGE_SIZE);
      memcpy((void*)&subPageTemp[APP_NVM_GENERIC_MODEL_OFFSET],
             (void*)(clearBuff),
             APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE);
      subPageTemp[APP_NVM_VALID_FLAG_OFFSET] = valid;
      
      result = AppliNvm_FlashProgram(APP_NVM_SUBPAGE_OFFSET(subPageIdx),
                                     (uint32_t*)&subPageTemp, 
                                     APP_NVM_SUBPAGE_SIZE);
    }
    else
    {  /* First subpage */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx)),
             APP_NVM_SUBPAGE_SIZE);
      memcpy((void*)&subPageTemp[APP_NVM_GENERIC_MODEL_OFFSET],
             (void*)(clearBuff),
             APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE);
      subPageTemp[APP_NVM_VALID_FLAG_OFFSET] = valid;
      
      result = AppliNvm_FlashProgram(APP_NVM_SUBPAGE_OFFSET(subPageIdx),
                                     (uint32_t*)&subPageTemp, 
                                     APP_NVM_SUBPAGE_SIZE);
    }
  }
#endif /* SAVE_MODEL_STATE_NVM */  
  return result;
}


/**
* @brief  Load Model states from nvm
* @param  model state buff
* @param  model state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_LoadModelState(uint8_t state[], 
                                     uint16_t* size)
{  
#if (SAVE_MODEL_STATE_NVM == 1)  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEINT16 currSubPageIdx = 0;
  
  *size = 0;
  result = AppliNvm_FindFirstEmptyPage(&currSubPageIdx,
                                       APP_NVM_SUBPAGE_SIZE,
                                       APP_NVM_MAX_SUBPAGE,
                                       APP_NVM_BASE + APP_NVM_RESERVED_SIZE);
  
  if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
    AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
    UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    if(currSubPageIdx > 0)
    { 
      /* Not the first reset */
      /* Load model data copy from nvm */
      memcpy((void*)AppliNvm_Reqs.modelData, 
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(currSubPageIdx-1) + APP_NVM_GENERIC_MODEL_OFFSET), 
             APP_NVM_GENERIC_MODEL_SIZE + APP_NVM_LIGHT_MODEL_SIZE);
      memcpy((void*)state, 
             (void*)(AppliNvm_Reqs.modelData), 
             APP_NVM_MODEL_SIZE);
      *size = APP_NVM_MODEL_SIZE;
    }
  }
  return result;  
#else /* SAVE_MODEL_STATE_NVM */  
  *size = 0;
  return MOBLE_RESULT_SUCCESS;
#endif /* SAVE_MODEL_STATE_NVM */
}


/**
* @brief  Process NVM erase and write requests
* @param  void
* @retval void
*/
void AppliNvm_Process(void)
{
  MOBLE_RESULT result;
  MOBLEINT16 subPageIdx;
  uint8_t reserveAreaCopy[APP_NVM_RESERVED_SIZE];
  
  /* Erase if required */
  if ((AppliNvm_Reqs.erasePageReq == MOBLE_TRUE) &&
      (!LL_FLASH_IsActiveFlag_OperationSuspended()))
  {
    if(StopRadioInProgress == 0)
    {
      /* save reserve flash area */
      memcpy((void*)reserveAreaCopy, 
             (void*)APP_NVM_BASE, 
             APP_NVM_RESERVED_SIZE);
    
      TRACE_M(TF_PROVISION,"Erase flash page\r\n");
      result = PalNvmErase(APP_NVM_BASE, 1);

      if(result == MOBLE_RESULT_OUTOFMEMORY)
      {
        AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
        UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
      }  
      else
      {
        AppliNvm_Reqs.erasePageReq = MOBLE_FALSE;
        AppliNvm_Reqs.writeReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
        UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
      /* restore reserve area */
/*      FLASH_ProgramWordBurst(APP_NVM_BASE, (uint32_t*)reserveAreaCopy); */
        result = AppliNvm_FlashProgram(0,
                                       (uint32_t*)&reserveAreaCopy, 
                                       APP_NVM_RESERVED_SIZE);
        BLEMesh_ResumeAdvScan();
      }
    }
    else
      StopRadioInProgress--;
  }
      
  if (AppliNvm_Reqs.erasePageReq == MOBLE_FALSE && 
      AppliNvm_Reqs.writeReq == MOBLE_TRUE)
  {
    result = AppliNvm_FindFirstEmptyPage(&subPageIdx,
                                         APP_NVM_SUBPAGE_SIZE,
                                         APP_NVM_MAX_SUBPAGE,
                                        (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));
    
    if(result == MOBLE_RESULT_OUTOFMEMORY)
    {
      BLEMesh_SuspendAdvScan();
      StopRadioInProgress = RADIO_OFF_DELAY;

      AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
      UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
      result = MOBLE_RESULT_SUCCESS;
    }  
    else
    {
      MOBLEUINT8 subPageTemp[APP_NVM_SUBPAGE_SIZE];
      MOBLEUINT32 valid = 0;
      
      if(subPageIdx > 0)
      { 
        /* Not the first reset */
        /* read the previous subpage */
        memcpy((void*)&(subPageTemp[0]),
               (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx-1)),
               APP_NVM_SUBPAGE_SIZE);
      }
      else
      {
        /* read the current subpage */
        memcpy((void*)&(subPageTemp[0]),
               (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx)),
               APP_NVM_SUBPAGE_SIZE);
      }
      
      memcpy((void*)&(subPageTemp[APP_NVM_GENERIC_MODEL_OFFSET]),
             (void*)&(AppliNvm_Reqs.modelData),
             APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE);
            
      subPageTemp[APP_NVM_VALID_FLAG_OFFSET] = valid;

      result = AppliNvm_FlashProgram(APP_NVM_SUBPAGE_OFFSET(subPageIdx), 
                                     (uint8_t*)subPageTemp,
                                     APP_NVM_SUBPAGE_SIZE);
    
      if (result == MOBLE_RESULT_SUCCESS)
      {
        AppliNvm_Reqs.writeReq = MOBLE_FALSE;
      }
    }
  }
}


/**
* @brief  Function used to set the flag which is responsible for storing the 
  states in flash.
* @param  void
* @retval void
*/
void AppliNvm_SaveMessageParam (void)
{
#ifdef SAVE_MODEL_STATE_FOR_ALL_MESSAGES
  PowerOnOff_flag = FLAG_SET;
#endif 
} 


/**
*  @brief  Find first valid subpage available. 
*          If no valid subpage found, erase page appli nvm to reset it
*  @param  pointer to variable to update first valid subpage index
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FindFirstEmptyPage(MOBLEINT16* subPageIndex,
                                         MOBLEUINT32 SubPageSize,
                                         MOBLEUINT32 totalSubPage, 
                                         MOBLEUINT32 nvmBaseOffset)
{
  MOBLEUINT32 valid[2];
  MOBLEINT8 subPageIdx = -1;
  MOBLEUINT32 *subPageOffset;
  MOBLEUINT32 addressValidPage;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  for (MOBLEUINT16 count=0; count<totalSubPage; count++)
  {
    addressValidPage = nvmBaseOffset + SubPageSize*count;
    subPageOffset = &addressValidPage;

    /* Read 8 bytes and check if it's Empty */
    memcpy((void*)&valid, (void*)(*subPageOffset), 8);

    if ((valid[0] == FLASH_EMPTY_SIGNATURE) /*&& (valid[1] == FLASH_EMPTY_SIGNATURE)*/)
    {
      subPageIdx = count;
      break;
    }
  }
  
  if (subPageIdx < 0)
  {
    TRACE_M(TF_PROVISION,"Out of Memory, Flash page is full \r\n");
    result = MOBLE_RESULT_OUTOFMEMORY;
  }
  else
  {
    result = MOBLE_RESULT_SUCCESS;
  }

  *subPageIndex = subPageIdx;
  
  return result;
}

#if 0 //#ifdef ENABLE_NVM_TEST
MOBLE_RESULT AppliNVM_Save_FlashTesting(MOBLEUINT8 *buffer, 
                                        MOBLEUINT16 buffer_size)
{
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;
  uint8_t reserveAreaCopy[APP_NVM_RESERVED_SIZE];
  MOBLEINT16 subPageIdx;
  
  for(MOBLEUINT8 count = 0;count <= buffer_size;count++)
  {
    buffer[count] = count;
  }
      
      /* save reserve flash area */
  memcpy((void*)reserveAreaCopy, 
         (void*)APP_NVM_BASE, 
         APP_NVM_RESERVED_SIZE);
  
  result = AppliNvm_FlashErase((uint16_t)((APP_NVM_BASE - RESET_MANAGER_FLASH_BASE_ADDRESS) / PAGE_SIZE));
  
  if (result == MOBLE_RESULT_SUCCESS)
  {
    /* restore reserve area */
//    FLASH_ProgramWordBurst(APP_NVM_BASE, (uint32_t*)reserveAreaCopy);
      result = AppliNvm_FlashProgram(0,
                                     (uint32_t*)&reserveAreaCopy, 
                                     APP_NVM_RESERVED_SIZE);
  }
              
  result = AppliNvm_FindFirstEmptyPage(&subPageIdx,
                                       APP_NVM_SUBPAGE_SIZE,
                                       APP_NVM_MAX_SUBPAGE,
                                       (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));

  if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
    TRACE_M(TF_PROVISION, "Flash sub page is full, preparing new subpage\r\n");
  }
  else
  {
    result = AppliNvm_FlashProgram(APP_NVM_SUBPAGE_OFFSET(subPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET, 
                                   buffer,
                                   buffer_size); 
  }	
   return result;	
}

MOBLE_RESULT AppliNVM_Retrieve_FlashTesting(MOBLEUINT8 *buffer,
                                            MOBLEUINT16 buffer_size)
{
  MOBLEINT16 currSubPageIdx;
  MOBLE_RESULT result = MOBLE_RESULT_FAIL;

  result = AppliNvm_FindFirstEmptyPage(&currSubPageIdx,
                                       APP_NVM_SUBPAGE_SIZE,
                                       APP_NVM_MAX_SUBPAGE,
                                       (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));

  if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
    TRACE_M(TF_PROVISION," Can not find the empty page for testing flash \r\n");
  }
  else
  {		
    memcpy((void*)buffer, 
           (void*)(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)+APP_NVM_GENERIC_MODEL_OFFSET), 
           buffer_size);
                                               
    result = AppliNvm_MarkSubpageInvalid();
                  
    if (MOBLE_FAILED(result))
    {
      result = MOBLE_RESULT_FAIL;
    }	
  }		

  return result;					 
}
#endif

/**
* @}
*/

/**
* @}
*/
