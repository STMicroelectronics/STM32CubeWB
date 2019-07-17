/**
  ******************************************************************************
  * @file    app_entry.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of functions used for the
  *          overall system infrastructure (scheduler, syncro with the M0, etc..)
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
