/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/app_thread.h
  * Description        : Header for Thread Application.
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
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_THREAD_H
#define APP_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "tl.h"
#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"
#include "tx_api.h"

/* OpenThread Library */
#include OPENTHREAD_CONFIG_FILE

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/

/* Thread application generic defines */
/*------------------------------------*/
typedef enum
{
  APP_THREAD_LIMITED,
  APP_THREAD_FULL,
} APP_THREAD_InitMode_t;

/* ipv6-addressing defines        */
/*------------------------------------*/
/* Key Point: A major difference between FTDs and MTDs are that FTDs subscribe to the ff03::2 multicast address.
 * MTDs do not. */

#define MULICAST_FTD_MED            "ff03::1"
#define MULICAST_FTD_BORDER_ROUTER  "ff03::2"

/* Application errors                 */
/*------------------------------------*/

/*
 *  List of all errors tracked by the Thread application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum
{
  ERR_REC_MULTI_MSG_FROM_M0,
  ERR_THREAD_SET_STATE_CB,
  ERR_THREAD_SET_CHANNEL,
  ERR_THREAD_SET_PANID,
  ERR_THREAD_IPV6_ENABLE,
  ERR_THREAD_START,
  ERR_THREAD_ERASE_PERSISTENT_INFO,
  ERR_THREAD_SET_NETWORK_KEY,
/* USER CODE BEGIN ERROR_APPLI_ENUM */
  ERR_THREAD_COAP_START,
  ERR_THREAD_COAP_ADD_RESSOURCE,
  ERR_THREAD_MESSAGE_READ,
  ERR_THREAD_COAP_SEND_RESPONSE,
  ERR_THREAD_COAP_NEW_MSG,
  ERR_THREAD_COAP_APPEND,
  ERR_THREAD_COAP_SEND_REQUEST,
  ERR_THREAD_COAP_DATA_RESPONSE,
  ERR_THREAD_SETUP,
  ERR_THREAD_LINK_MODE,
  ERR_TIMER_INIT,
  ERR_TIMER_START,
  ERR_THREAD_MSG_COMPARE_FAILED,
  ERR_THREAD_COAP_ADDRESS_NOT_DEFINED,
  ERR_THREAD_THREAD_X_FAILED,
/* USER CODE END ERROR_APPLI_ENUM */
  ERR_THREAD_CHECK_WIRELESS
  } ErrAppliIdEnum_t;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

// -- Previous defines --
//#define SHCI_USER_EVT_PROCESS_PRIORITY  16u
//#define MESSAGE_M0_TO_M4_PRIORITY       16u
//#define COAP_CONFIRMABLE_PRIORITY       16u
//#define COAP_NON_CONFIRMABLE_PRIORITY   16u
//#define SEND_CLI_TO_M0_PRIORITY         16u


// -- New priorities to do not have a loop in 'Wait_Getting_Ack_From_M0' --
#define SHCI_USER_EVT_PROCESS_PRIORITY  6u
#define MESSAGE_M0_TO_M4_PRIORITY       7u
#define COAP_CONFIRMABLE_PRIORITY       5u
#define COAP_NON_CONFIRMABLE_PRIORITY   5u
#define SEND_CLI_TO_M0_PRIORITY         5u


// -- Define the Level of LowPower --
#define LOWPOWER_NONE               	0x00u
#define LOWPOWER_SLEEPMODE          	0x01u
#define LOWPOWER_STOPMODE           	0x02u
#define LOWPOWER_OFFMODE            	0x03u

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void APP_THREAD_Init(TX_BYTE_POOL* p_byte_pool);
void APP_THREAD_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_THREAD_TraceError(const char * pMess, uint32_t ErrCode);
void APP_THREAD_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer);
void APP_THREAD_ProcessMsgM0ToM4(ULONG argument);
void APP_THREAD_Init_UART_CLI(TX_BYTE_POOL* p_byte_pool);
void APP_THREAD_TL_THREAD_INIT(void);
void APP_THREAD_SW1_Task(void);
void APP_THREAD_SW2_Task(void);

#ifdef TX_LOW_POWER
void APP_THREAD_ThreadX_LowPowerEnable                  ( uint8_t cEnable );
void APP_THREAD_ThreadX_EnterLowPower                   ( void );
void APP_THREAD_ThreadX_ExitLowPower                    ( void );
void APP_THREAD_ThreadX_Low_Power_Setup                 ( ULONG tx_low_power_next_expiration );
unsigned long APP_THREAD_ThreadX_Low_Power_Adjust_Ticks ( void );
#endif // TX_LOW_POWER

/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_THREAD_H */
