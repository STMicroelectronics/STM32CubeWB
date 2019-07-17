
/**
  ******************************************************************************
  * @file    app_entry.h
  * @author  MCD Application Team
  * @brief   Interface to the application
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
#ifndef __APP_ENTRY_H
#define __APP_ENTRY_H

#include "stm32wbxx_hal.h"
#include "stm32_seq.h"
#include "tl.h"

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */
  void APPE_Init( void );
  void APP_ENTRY_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer);
  void APP_ENTRY_ProcessMsgM0ToM4(void);
  void APP_ENTRY_Init_CFG_CLI_UART(void);
  void APP_ENTRY_TL_THREAD_INIT(void);
  void UTIL_SEQ_EvtIdle( UTIL_SEQ_bm_t task_id_bm, UTIL_SEQ_bm_t evt_waited_bm );

#ifdef __cplusplus
}
#endif

#endif /* __APP_ENTRY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
