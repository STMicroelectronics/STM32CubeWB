/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/p2p_client_app.h
  * Description        : Header for p2p_server_app.c module
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
#ifndef P2P_APPLICATION_H
#define P2P_APPLICATION_H

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
  PEER_CONN_HANDLE_EVT,
  PEER_DISCON_HANDLE_EVT,
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
/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /*P2P_APPLICATION_H */
