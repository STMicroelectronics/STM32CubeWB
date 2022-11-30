/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_zigbee.h
  * Description        : Header for Zigbee Application.
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
#ifndef APP_ZIGBEE_H
#define APP_ZIGBEE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#include "tl.h"
#include "tl_zigbee_hci.h"
#include "tx_api.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/

/* Application errors                 */
/*------------------------------------*/

/*
 *  List of all errors tracked by the Zigbee application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum
{
  ERR_REC_MULTI_MSG_FROM_M0,
  ERR_ZIGBE_CMD_TO_M0,
/* USER CODE BEGIN ERROR_APPLI_ENUM */

  ERR_ZIGBEE_THREAD_X_FAILED,
  
/* USER CODE END ERROR_APPLI_ENUM */
  ERR_ZIGBEE_CHECK_WIRELESS
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
#define NOTIFY_M0_TO_M4_PRIORITY        7u
#define REQUEST_M0_TO_M4_PRIORITY       7u
#define NWL_FORM_PRIORITY               5u
#define PUSH_BUTTON_PRIORITY            5u
//#define SEND_CLI_TO_M0_PRIORITY         5u

// -- Define the Level of LowPower --
#define LOWPOWER_NONE               0x00u        
#define LOWPOWER_SLEEPMODE          0x01u
#define LOWPOWER_STOPMODE           0x02u
#define LOWPOWER_OFFMODE            0x03u


/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void APP_ZIGBEE_Init(TX_BYTE_POOL* p_byte_pool);
void APP_ZIGBEE_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_ZIGBEE_RegisterCmdBuffer(TL_CmdPacket_t *p_buffer);
void APP_ZIGBEE_TL_INIT(void);
void Pre_ZigbeeCmdProcessing(void);

#ifdef TX_LOW_POWER
void APP_ZIGBEE_ThreadX_LowPowerEnable                  ( uint8_t cEnable );
void APP_ZIGBEE_ThreadX_EnterLowPower                   ( void );
void APP_ZIGBEE_ThreadX_ExitLowPower                    ( void );
void APP_ZIGBEE_ThreadX_Low_Power_Setup                 ( ULONG tx_low_power_next_expiration );
unsigned long APP_ZIGBEE_ThreadX_Low_Power_Adjust_Ticks ( void );
#endif // TX_LOW_POWER
/* USER CODE BEGIN EF */

/* USER CODE END EF */
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_ZIGBEE_H */

