/**
  ******************************************************************************
  * @file    app_thread.h
  * @author  MCD Application Team
  * @brief   Header for Thread application
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
#ifndef APP_THREAD_H
#define APP_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/
#include "tl.h"
#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"

/* OpenThread Library */
#include OPENTHREAD_CONFIG_FILE

/* ---------------------------------------------------------------------------*/
/* Exported types and defines ------------------------------------------------*/
/* ---------------------------------------------------------------------------*/

/* Thread application generic defines */
/*------------------------------------*/
typedef enum
{
  APP_THREAD_LIMITED,
  APP_THREAD_FULL,
} APP_THREAD_InitMode_t;

/* Application errors                 */
/*------------------------------------*/

 /*
  *  List of all errors tracked the Thread application
  *  running on M4. Some of these errors may be fatal
  *  or just warnings
  */
typedef enum
{
  ERR_REC_MULTI_MSG_FROM_M0 = 11U,
  ERR_REC_MULTI_TRACE_FROM_M0 = 12U,
  ERR_THREAD_SET_STATE_CB = 13U,
  ERR_THREAD_SET_CHANNEL = 14U,
  ERR_THREAD_SET_PANID = 15U,
  ERR_THREAD_IPV6_ENABLE = 16U,
  ERR_THREAD_START = 17U,
  ERR_THREAD_COAP_START = 18U,
  ERR_THREAD_COAP_ADD_RESSOURCE = 19U,
  ERR_THREAD_MESSAGE_READ = 20U,
  ERR_THREAD_COAP_SEND_RESPONSE = 21U,
  ERR_THREAD_COAP_APPEND = 22U,
  ERR_THREAD_COAP_SEND_REQUEST = 23U,
  ERR_THREAD_COAP_SEND_RESP = 24U,
  ERR_THREAD_SETUP = 25U,
  ERR_THREAD_LINK_MODE = 26U,
  ERR_ALLOC_MSG = 27U,
  ERR_FILE_RESP_HANDLER = 28U,
  ERR_MSG_COMPARE_FAILED = 29U,
  ERR_NEW_MSG_ALLOC = 30U,
  ERR_PROVISIONING_HANDLER = 31U,
  ERR_THREAD_DATA_RESPONSE = 32U,
  ERR_APPEND = 33U,
  ERR_PROVISIONING_RESP = 34U,
  ERR_HEADER_INIT = 36U,
  ERR_TOKEN = 37U,
  ERR_APEND_URI = 38U,
  ERR_READ = 39U,
  ERR_THREAD_ERASE_PERSISTENT_INFO = 40U,
  ERR_THREAD_CHECK_WIRELESS
} ErrAppliIdEnum_t;

 /* ---------------------------------------------------------------------------*/
 /* Exported functions ------------------------------------------------------- */
 /* ---------------------------------------------------------------------------*/
void APP_THREAD_Init( void );
void APP_THREAD_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_THREAD_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer);
void APP_THREAD_ProcessMsgM0ToM4(void);
void APP_THREAD_Init_UART_CLI(void);
void APP_THREAD_TL_THREAD_INIT(void);
  

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_THREAD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
