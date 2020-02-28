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

/** @addtogroup ST_BLE_Mesh
*  @{
*/

/** @addtogroup Application_Mesh_Models
*  @{
*/
/* Private define ------------------------------------------------------------*/

extern const MOBLEUINT8* _bdaddr[];
extern const MOBLEUINT8* _prvsnr_data[];

#ifdef SAVE_MODEL_STATE_FOR_ALL_MESSAGES
extern MOBLEUINT8 PowerOnOff_flag;
#endif
MOBLEUINT8 PrvnFlag = 0;

#if defined SAVE_EMBD_PROVISION_DATA  
MOBLEUINT8 DataCopy[2048];
#endif
#ifdef ENABLE_PROVISIONER_FEATURE
extern MOBLEUINT16 nodeAddressOffset;
#endif

extern const void* appNvmBase;
extern const void* prvsnr_data;


/*
*  Page of size 4k, STM32WB, reserved for application is divided into 16 subpages of each 256 bytes
*  First subpage is reserved (for e.g. External MAC is present in this area)
*  Rest 15 subpages are used on rolling basis for application states. 
*    First byte of each subpage indicates if page is valid or not
*/

#define APP_NVM_RESERVED_SIZE             256U
#define APP_NVM_SUBPAGE_SIZE              256U
#define APP_NVM_MAX_SUBPAGE               15U
#define APP_NVM_SUBPAGE_OFFSET(i)         (unsigned int)(APP_NVM_SUBPAGE_SIZE + APP_NVM_SUBPAGE_SIZE*(i))

/* offsets defined wrt start of subpage */
#define APP_NVM_VALID_FLAG_OFFSET         0U
#define APP_NVM_VALID_FLAG_SIZE           4U
#define APP_NVM_RESET_COUNT_OFFSET        (unsigned int)APP_NVM_VALID_FLAG_SIZE
#define APP_NVM_RESET_COUNT_SIZE          12U /* 12 bytes reserved for write cycle optimization */
#define APP_NVM_GENERIC_MODEL_OFFSET      (unsigned int)(APP_NVM_VALID_FLAG_SIZE+APP_NVM_RESET_COUNT_SIZE)
#define APP_NVM_GENERIC_MODEL_SIZE        16U
#define APP_NVM_LIGHT_MODEL_OFFSET        (unsigned int)(APP_NVM_VALID_FLAG_SIZE+APP_NVM_RESET_COUNT_SIZE+APP_NVM_GENERIC_MODEL_SIZE)
#define APP_NVM_LIGHT_MODEL_SIZE          16U

/* offset defined for the embedded provisioner node */
#define PRVN_NVM_SUBPAGE_SIZE             16U
#define PRVN_NVM_MAX_SUBPAGE              (unsigned int)(PRVN_NVM_PAGE_SIZE/PRVN_NVM_SUBPAGE_SIZE)
#define PRVN_NVM_SUBPAGE_OFFSET(i)        (unsigned int)(PRVN_NVM_SUBPAGE_SIZE*(i))

#define FIRST_PRVND_NODE_ADDRSS           2U
#define LAST_PRVND_NODE_ADDRSS            (unsigned int)(PRVN_NVM_MAX_SUBPAGE+FIRST_PRVND_NODE_ADDRSS)

/* Private variables ---------------------------------------------------------*/
typedef struct
{
  MOBLEUINT8 modelData[APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE];
  MOBLEBOOL erasePageReq;
  MOBLEBOOL writeReq;
} APPLI_NVM_REQS;

/* ALIGN(4) */
__attribute__((aligned(4))) APPLI_NVM_REQS AppliNvm_Reqs;

typedef struct
{
  MOBLEUINT8  prvnData[PRVN_NVM_SUBPAGE_SIZE];
  MOBLEBOOL erasePageReq;
  MOBLEBOOL writeReq;
} PRVN_NVM_REQS;

/* ALIGN(4) */
__attribute__((aligned(4))) PRVN_NVM_REQS  PrvnNvm_Reqs;

MOBLE_RESULT AppliNvm_EraseRestoreResvNvm(void);
MOBLE_RESULT AppliNvm_FindFirstEmptyPage(MOBLEINT8* subPageIndex,MOBLEUINT32 SubPageSize,
                                          MOBLEUINT32 totalSubPage, MOBLEUINT32 nvmBaseOffset);
//MOBLE_RESULT AppliNvm_FindFirstValidSubPage(MOBLEINT8* subPageIndex);
MOBLE_RESULT AppliNvm_MarkSubpageInvalid(void);
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size);
//MOBLE_RESULT AppliNvm_LoadGenericState(uint8_t state[], uint8_t* size);
//MOBLE_RESULT AppliNvm_LoadLightState(uint8_t state[], uint8_t* size);
//MOBLE_RESULT AppliNvm_PrvnFindFirstValidSubPage(MOBLEINT8* subPageIndex);
//MOBLE_RESULT AppliNvm_PrvnMarkSubpageInvalid(void);
//MOBLE_RESULT AppliNvm_SavePrvsnerData(uint8_t* data, uint8_t size);
MOBLEUINT32* AppliPrvnNvm_GetSubPageData(MOBLE_ADDRESS addrss);
MOBLE_RESULT AppliPrvnNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size);

#if 0
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
  
  if (PageNumber < N_PAGES)
  {
    BLEMesh_StopAdvScan();
    ATOMIC_SECTION_BEGIN();
    result =  MoblePalNvmErase(APP_NVM_BASE, 0);
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
MOBLE_RESULT AppliNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size)
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
    result = MoblePalNvmWrite(APP_NVM_BASE, offset, buf, size);
  }
  
  return result;
}

/** 
*  @brief  Program word (32-bit) at a PRVN_NVM_BASE_OFFSET
*  @param  TypeProgram  Indicate the way to program at a specified address
*  @param  Address: address to write
*  @param  Data: word to write
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliPrvnNvm_FlashProgram(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  if (offset > PRVN_NVM_PAGE_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > (PRVN_NVM_BASE_OFFSET + PRVN_NVM_PAGE_SIZE))
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
    result = MoblePalNvmWrite(PRVN_NVM_BASE_OFFSET, offset, buf, size);
  }
  
  return result;
}

#if 0
/**
*  @brief  Find first valid subpage available. 
*          If no valid subpage found, erase page appli nvm to reset it
*  @param  pointer to variable to update first valid subpage index
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FindFirstValidSubPage(MOBLEINT8* subPageIndex)
{
  MOBLEUINT32 valid[2];
  MOBLEINT8 subPageIdx = -1;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* find valid subpage */
  for (MOBLEUINT8 count=0; count<APP_NVM_MAX_SUBPAGE; count++)
  {
    unsigned int offset = APP_NVM_SUBPAGE_OFFSET(count);
    
    memcpy((void*)&valid, (void*)(APP_NVM_BASE + offset), 8);
    if ((valid[0] == 0xFFFFFFFF) && ((valid[1] == 0xFFFFFFFF)))
    {
      subPageIdx = count;
      break;
    }
  }
  
  /* if no valid subpage found, erase all subpages and reset subPageIdx */
  if (subPageIdx < 0)
  {
    AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
    UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    result = MOBLE_RESULT_SUCCESS;
  }

  *subPageIndex = subPageIdx;
  
  return result;
}
#endif

/**
*  @brief  Mark first valid subpage as invalid. 
*  @param  void
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_MarkSubpageInvalid(void)
{
  MOBLEUINT32 valid = 0;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEINT8 currSubPageIdx;
  MOBLEUINT8 subPageTemp[APP_NVM_SUBPAGE_SIZE];
  
  /* find valid subpage */
  result = AppliNvm_FindFirstEmptyPage(&currSubPageIdx,APP_NVM_SUBPAGE_SIZE,APP_NVM_MAX_SUBPAGE,
                                        (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));//AppliNvm_FindFirstValidSubPage(&currSubPageIdx);
                                        
  
  if (result != MOBLE_RESULT_SUCCESS)
  {
    /* do nothing */
  }
  else
  {
    /* mark valid subpage as invalid */
    memcpy((void*)subPageTemp,
           (void*)(APP_NVM_SUBPAGE_OFFSET(currSubPageIdx)),
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
  MOBLEINT8 currSubPageIdx = 0;
  MOBLE_RESULT result;
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
             APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE);
      
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
      MoblePalNvmErase(NVM_BASE, 0);      
      MoblePalNvmErase(NVM_BASE, 0x1000);
      
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

  if (size > APP_NVM_MODEL_SIZE)
  {
    /* incorrect size */
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    memcpy(AppliNvm_Reqs.modelData, state, size);
    
    AppliNvm_Reqs.writeReq = MOBLE_TRUE;
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
  MOBLEINT8 subPageIdx;
  MOBLEUINT8 clearBuff[APP_NVM_GENERIC_MODEL_SIZE+APP_NVM_LIGHT_MODEL_SIZE] = {0};
  MOBLEUINT32 valid = 0;
  
  result = AppliNvm_FindFirstEmptyPage(&subPageIdx,APP_NVM_SUBPAGE_SIZE,APP_NVM_MAX_SUBPAGE,
                                        (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));
    
  if (MOBLE_FAILED(result))
  {
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    if(subPageIdx > 1)
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

#if 0
/**
* @brief  Load generic states from nvm
* @param  generic state buff
* @param  generic state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_LoadGenericState(uint8_t state[], uint8_t* size)
{  
#if (SAVE_MODEL_STATE_NVM == 1)  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 subPageTemp[APP_NVM_SUBPAGE_SIZE];
  MOBLEINT8 subPageIdx;
  
  result = AppliNvm_FindFirstValidSubPage(&subPageIdx);
  if (result == MOBLE_RESULT_SUCCESS)
  {
    if(subPageIdx > 0)
    { 
      /* Not the first reset */
      /* read the previous subpage */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx-1)),
             APP_NVM_SUBPAGE_SIZE);
    
      memcpy((void*)&(state[0]),
             (void*)&(subPageTemp[APP_NVM_GENERIC_MODEL_OFFSET]),
             APP_NVM_GENERIC_MODEL_SIZE);
      *size = APP_NVM_GENERIC_MODEL_SIZE;
    }
    else
    {
      /* read the current subpage */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx)),
             APP_NVM_SUBPAGE_SIZE);
    
      memcpy((void*)&(state[0]),
             (void*)&(subPageTemp[APP_NVM_GENERIC_MODEL_OFFSET]),
             APP_NVM_GENERIC_MODEL_SIZE);
      *size = APP_NVM_GENERIC_MODEL_SIZE;
    }
  }
  return result;  
#else /* SAVE_MODEL_STATE_NVM */  
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
//  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
//  
//  memcpy((void*)state, (void*)(AppliNvm_Reqs.modelData+APP_NVM_GENERIC_MODEL_SIZE), APP_NVM_LIGHT_MODEL_SIZE);
//  *size = APP_NVM_GENERIC_MODEL_SIZE;
//  
//  return result;  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 subPageTemp[APP_NVM_SUBPAGE_SIZE];
  MOBLEINT8 subPageIdx;
  
  result = AppliNvm_FindFirstValidSubPage(&subPageIdx);
  if (result == MOBLE_RESULT_SUCCESS)
  {
    if(subPageIdx > 1)
    { 
      /* Not the first reset */
      /* read the previous subpage */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx-1)),
             APP_NVM_SUBPAGE_SIZE);
    
      memcpy((void*)&(state[0]),
             (void*)&(subPageTemp[APP_NVM_LIGHT_MODEL_OFFSET]),
             APP_NVM_LIGHT_MODEL_SIZE);
      *size = APP_NVM_LIGHT_MODEL_SIZE;
    }
    else
    {
      /* read the current subpage */
      memcpy((void*)&(subPageTemp[0]),
             (void*)(APP_NVM_BASE + APP_NVM_SUBPAGE_OFFSET(subPageIdx)),
             APP_NVM_SUBPAGE_SIZE);
    
      memcpy((void*)&(state[0]),
             (void*)&(subPageTemp[APP_NVM_LIGHT_MODEL_OFFSET]),
             APP_NVM_LIGHT_MODEL_SIZE);
      *size = APP_NVM_LIGHT_MODEL_SIZE;
    }
  }
  return result;  
#else /* SAVE_MODEL_STATE_NVM */  
    *size = 0;
  return MOBLE_RESULT_SUCCESS;
#endif /* SAVE_MODEL_STATE_NVM */
  }
#endif

/**
* @brief  Load Model states from nvm
* @param  model state buff
* @param  model state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_LoadModelState(uint8_t state[], uint8_t* size)
{  
#if (SAVE_MODEL_STATE_NVM == 1)  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  memcpy((void*)state, (void*)(AppliNvm_Reqs.modelData), APP_NVM_MODEL_SIZE);
  *size = APP_NVM_MODEL_SIZE;
  
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
  MOBLEINT8 subPageIdx;
  uint8_t reserveAreaCopy[APP_NVM_RESERVED_SIZE];
  
  /* Erase if required */
  if (AppliNvm_Reqs.erasePageReq == MOBLE_TRUE)
  {
    /* save reserve flash area */
    memcpy((void*)reserveAreaCopy, (void*)APP_NVM_BASE, APP_NVM_RESERVED_SIZE);
  
    result = MoblePalNvmErase(APP_NVM_BASE, 0);

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
      if (result == MOBLE_RESULT_SUCCESS)
      {
        AppliNvm_Reqs.writeReq = MOBLE_FALSE;
      }
    }
  }
      
  if (AppliNvm_Reqs.erasePageReq == MOBLE_FALSE
      && AppliNvm_Reqs.writeReq == MOBLE_TRUE)
  {
    result = AppliNvm_FindFirstEmptyPage(&subPageIdx,APP_NVM_SUBPAGE_SIZE,APP_NVM_MAX_SUBPAGE,
                                        (APP_NVM_BASE+APP_NVM_RESERVED_SIZE));//AppliNvm_FindFirstValidSubPage(&subPageIdx);
    if(result == MOBLE_RESULT_OUTOFMEMORY)
    {
      AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
      UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
#endif
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
* @brief  Fuction used to set the flag which is responsible for storing the 
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


/********************Embedded Provisioner *******************************/

/**
*  @brief  Find first valid subpage available. 
*          If no valid subpage found, erase page appli nvm to reset it
*  @param  pointer to variable to update first valid subpage index
*  @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliNvm_FindFirstEmptyPage(MOBLEINT8* subPageIndex,MOBLEUINT32 SubPageSize,
                                          MOBLEUINT32 totalSubPage, MOBLEUINT32 nvmBaseOffset)
{
  MOBLEUINT32 valid[2];
  MOBLEINT8 subPageIdx = -1;
  MOBLEUINT32 *subPageOffset;
  MOBLEUINT32 addrssValidPage;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  for (MOBLEUINT8 count=0; count<totalSubPage; count++)
  {
    addrssValidPage = nvmBaseOffset + SubPageSize*count;
    subPageOffset = &addrssValidPage;
    memcpy((void*)&valid, (void*)(*subPageOffset), 8);

    if ((valid[0] == 0xFFFFFFFF) && ((valid[1] == 0xFFFFFFFF)))
    {
      subPageIdx = count;
      break;
    }
  }
  
  if (subPageIdx < 0)
  {
    TRACE_M(TF_PROVISION," Warning: No Valid Page Found,Erase operation \r\n");
    result = MOBLE_RESULT_OUTOFMEMORY;
  }
  else
  {
    result = MOBLE_RESULT_SUCCESS;
  }

  *subPageIndex = subPageIdx;
  
  return result;
}

/**
* @brief  Load Provisioner data from nvm
* @param  model state buff
* @param  model state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliPrvnNvm_LoadData(uint8_t state[], uint8_t* size)
{  
#if (SAVE_EMBD_PROVISION_DATA == 1)  
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  memcpy((void*)state, (void*)(PrvnNvm_Reqs.prvnData), PRVN_NVM_SUBPAGE_SIZE);
  *size = PRVN_NVM_SUBPAGE_SIZE;
  
  return result;  
#else /* SAVE_EMBD_PROVISION_DATA */  
  *size = 0;
  return MOBLE_RESULT_SUCCESS;
#endif /* SAVE_EMBD_PROVISION_DATA */
}

/**
* @brief  Save Embedded provisioner data in nvm
* @param  data: data buffer to be saved
* @param  size: data buffer size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliPrvnNvm_SaveData(uint8_t* data, uint8_t size)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS; /* if save model state not defined, return MOBLE_RESULT_FAIL */
  
  if (size > PRVN_NVM_SUBPAGE_SIZE)
  {
    /* incorrect size */
    result = MOBLE_RESULT_FAIL;
  }
  else
  {
    memcpy(PrvnNvm_Reqs.prvnData, data, size);
    PrvnNvm_Reqs.writeReq = MOBLE_TRUE;
#if (LOW_POWER_FEATURE == 1)
    UTIL_SEQ_SetTask( 1<<CFG_TASK_APPLI_REQ_ID, CFG_SCH_PRIO_0);
 #endif
      
    if (MOBLE_FAILED(result))
    {
      result = MOBLE_RESULT_FAIL;
    }
    else
    {
      result = MOBLE_RESULT_SUCCESS;
    }
  }

  return result;
}

/**
* @brief  Function to get the particular node device key with node address friom the 
*         provisioner flash.
* @param  MOBLE_ADDRESS: node address
* @retval subPageAddrss: pointer to the sub page address
*/
MOBLEUINT32* AppliPrvnNvm_GetNodeDevKey(MOBLE_ADDRESS addrss)
{
  MOBLEUINT16 subPageIndx;
  MOBLEUINT32 *subPageAddrss;
  
  subPageIndx = addrss - FIRST_PRVND_NODE_ADDRSS;
    
  if(subPageIndx >= PRVN_NVM_MAX_SUBPAGE)
  {
    TRACE_M(TF_PROVISION,"Invalid Address \r\n");
    return 0;   
  }
  else
  {
    subPageAddrss = (MOBLEUINT32 *)(PRVN_NVM_BASE_OFFSET + PRVN_NVM_SUBPAGE_OFFSET(subPageIndx));
    TRACE_M(TF_PROVISION,"Address Of SubPage =  %p \r\n", (void *)subPageAddrss);
  }
  return subPageAddrss;
}

/**
* @brief  backup by Copy all the data from the page in RAM.
* @param  model state buff
* @param  model state buff size
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliPrvnNvm_CopyData(uint8_t state[], uint8_t* size)
{
  
#if SAVE_EMBD_PROVISION_DATA    
  MOBLEUINT8 deviceKeysSaved[2048]; 
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
  /* find valid subpage */
    for (MOBLEUINT8 count=0; count<8; count++)
    {    
      memcpy((void*)deviceKeysSaved, (void*)(PRVN_NVM_BASE_OFFSET + PRVN_NVM_SUBPAGE_OFFSET(count)), PRVN_NVM_SUBPAGE_SIZE);
    }
    
  *size = PRVN_NVM_SUBPAGE_SIZE;
  
  return result;  
#else /* SAVE_EMBD_PROVISION_DATA */  
  *size = 0;
  return MOBLE_RESULT_SUCCESS;
#endif /* SAVE_EMBD_PROVISION_DATA */
    
}

/**
* @brief  Process Provisioner NVM erase and write requests
* @param  void
* @retval void
*/
void AppliPrvnNvm_Process(void)
{
  MOBLE_RESULT result;
  MOBLEINT8 subPageIdx;
  
  if ( (PrvnNvm_Reqs.erasePageReq == MOBLE_FALSE) && 
       (PrvnNvm_Reqs.writeReq == MOBLE_TRUE) )
  {
    result = AppliNvm_FindFirstEmptyPage(&subPageIdx,PRVN_NVM_SUBPAGE_SIZE,
                                          PRVN_NVM_MAX_SUBPAGE,PRVN_NVM_BASE_OFFSET);
    
    if (result == MOBLE_RESULT_SUCCESS)
    {
      TRACE_M(TF_PROVISION,"Saving in SubPage[%.8x] = \r\n", PRVN_NVM_SUBPAGE_OFFSET(subPageIdx));
      result = AppliPrvnNvm_FlashProgram(PRVN_NVM_SUBPAGE_OFFSET(subPageIdx), 
                                     PrvnNvm_Reqs.prvnData,
                                     PRVN_NVM_SUBPAGE_SIZE);
    
      if (result == MOBLE_RESULT_SUCCESS)
      {
        PrvnNvm_Reqs.writeReq = MOBLE_FALSE;
      }
    }
    else
    {
      //AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
    }
  }
}

/**
* @brief  This function is to save the provisioner device key in the flash.          
* @param  data:Pointer to the data passed.
* @param  size:size of the data
* @param  prvnFlag:Pointer to the flag passed.
* @retval void
*/
void AppliNvm_saveProvisionerDevKey(MOBLEUINT8 *data , MOBLEUINT8 size , MOBLEUINT8 *prvnFlag)
{
  if(*prvnFlag == 1)
  {
    AppliPrvnNvm_SaveData(&data[0] ,size);
    prvnFlag = 0;
  }
}

/**
* @brief  This function load the Provisioner nvm data to the buffer after reset.          
* @param  void
* @retval MOBLE_RESULT_SUCCESS on success
*/
MOBLE_RESULT AppliPrvnNvm_FactorySettingReset(MOBLEUINT8 *flag)
{
  MOBLEINT8 currSubPageIdx;
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    
  result = AppliNvm_FindFirstEmptyPage(&currSubPageIdx,PRVN_NVM_SUBPAGE_SIZE,
                                       PRVN_NVM_MAX_SUBPAGE,PRVN_NVM_BASE_OFFSET);
  
  if(result == MOBLE_RESULT_OUTOFMEMORY)
  {
    result = MOBLE_RESULT_FAIL;
    //AppliNvm_Reqs.erasePageReq = MOBLE_TRUE;
  }
  else
  {    
      /* After reset of provisioner , retreiving the next node address to be provisioned */
#ifdef ENABLE_PROVISIONER_FEATURE
    
      if(currSubPageIdx > 0)
      {
        nodeAddressOffset = currSubPageIdx + 1;         
      }
      
#endif           
          
      
      if(currSubPageIdx > 0)
      {
        currSubPageIdx = currSubPageIdx-1;
      }
      
      /* Load model data copy from nvm */
      memcpy((void*)PrvnNvm_Reqs.prvnData, 
             (void*)(PRVN_NVM_BASE_OFFSET + PRVN_NVM_SUBPAGE_OFFSET(currSubPageIdx)),PRVN_NVM_SUBPAGE_SIZE);          
          
      
      TRACE_I(TF_PROVISION,"NEXT NVM ADDRESS %.8x \r\n",PRVN_NVM_SUBPAGE_OFFSET(currSubPageIdx+PRVN_NVM_SUBPAGE_SIZE));
      
   }
     
  return result;
}

