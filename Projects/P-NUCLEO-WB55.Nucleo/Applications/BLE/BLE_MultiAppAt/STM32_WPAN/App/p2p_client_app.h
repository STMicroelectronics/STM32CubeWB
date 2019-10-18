/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    p2p_server_app.h
 * @author  MCD Application Team
 * @brief   Header for p2p_server_app.c module
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __P2P_APPLICATION_H
#define __P2P_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  PEERC_CONN_HANDLE_EVT,
  PEERC_DISCON_HANDLE_EVT,
  PEERC_DISCON_SWITCH_HANDLE_EVT,
} P2PC_APP_Opcode_Notification_evt_t;

typedef struct
{
  P2PC_APP_Opcode_Notification_evt_t          P2P_Evt_Opcode;
  uint16_t                                    ConnectionHandle;

}P2PC_APP_ConnHandle_Not_evt_t;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
void P2PC_APP_Init( void );
void P2PC_APP_Notification( P2PC_APP_ConnHandle_Not_evt_t *pNotification );
uint8_t P2P_Client_APP_Get_State( void );
/* USER CODE BEGIN EFP */
void P2PC_APP_SW1_Button_Action(void);
void Enable_Notification( void );
void Disable_Notification( void );
/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /*__P2P_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
