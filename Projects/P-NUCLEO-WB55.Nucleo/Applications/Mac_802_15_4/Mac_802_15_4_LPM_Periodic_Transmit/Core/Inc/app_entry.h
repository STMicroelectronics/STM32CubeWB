/**
  ******************************************************************************
  * @file    app_entry.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of functions used for the
  *          overall system infrastructure (scheduler, syncro with the M0, etc..)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_ENTRY_H
#define APP_ENTRY_H

#include "stm32wbxx_hal.h"
#include "stm32_seq.h"
#include "tl.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
  APPE_LIMITED,
  APPE_FULL,
} APP_ENTRY_InitMode_t;

void APP_ENTRY_Init( APP_ENTRY_InitMode_t InitMode );
void APP_ENTRY_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer);

void APP_ENTRY_ProcessMsgFromRFCoreTask(void);

void APP_ENTRY_Init_RFD(void);
void APP_ENTRY_TL_MAC_802_15_4_Init(void);
void SEQ_EvtIdle( uint32_t evt_waited_bm );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_ENTRY_H */
