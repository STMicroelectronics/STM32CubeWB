/**
  ******************************************************************************
  * @file    tsl.c
  * @author  MCD Application Team
  * @brief   This file contains the driver main functions.
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
#include "tsl.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/**
  * @brief  Initializes the TS interface.
  * @param  bank  Array holding all the banks
  * @retval Status
  */
TSL_Status_enum_T TSL_Init(CONST TSL_Bank_T *bank)
{
  TSL_Status_enum_T retval;

  // Get banks array
  TSL_Globals.Bank_Array = bank;

  // Initialize the delay that will be used to discharge the capacitors
  TSL_Globals.DelayDischarge = (uint32_t)((TSLPRM_DELAY_DISCHARGE_ALL * (uint32_t)(SystemCoreClock/1000000)) / 72);
  
  // Note: The timing configuration (Systick) must be done in the user code.

  // Initialization of the acquisition module
#ifdef __TSL_ACQ_TSC_H
  // Note: The TSC peripheral initialization must be done in the user code.
  retval = TSL_STATUS_OK;
#else
  retval = TSL_acq_Init();
#endif

  return retval;
}
