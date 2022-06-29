/**
  ******************************************************************************
  * @file    heap.c
  * @author  MCD Application Team
  * @brief   This file contains the OpenThread Heap API shared between M0 and
  *          M4.
  ******************************************************************************
  * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32wbxx_hal.h"

#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"

/* Include definition of compilation flags requested for OpenThread configuration */
#include OPENTHREAD_CONFIG_FILE

#include "heap.h"

void *otHeapCAlloc(size_t aCount, size_t aSize)
{
  OT_UNUSED_VARIABLE(aCount);
  OT_UNUSED_VARIABLE(aSize);
  
  return (void*)NULL;
}

void otHeapFree(void *aPointer)
{
  OT_UNUSED_VARIABLE(aPointer);
}

