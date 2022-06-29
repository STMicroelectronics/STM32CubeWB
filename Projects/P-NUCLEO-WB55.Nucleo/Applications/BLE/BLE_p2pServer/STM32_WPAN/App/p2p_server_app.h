/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    p2p_server_app.h
  * @author  MCD Application Team
  * @brief   PHeader for p2p_server_app.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#ifndef P2P_SERVER_APP_H
#define P2P_SERVER_APP_H

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
} P2PS_APP__Opcode_Notification_evt_t;

typedef struct
{
  P2PS_APP__Opcode_Notification_evt_t   P2P_Evt_Opcode;
  uint16_t                              ConnectionHandle;
}P2PS_APP_ConnHandle_Not_evt_t;
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
  void P2PS_APP_Init( void );
  void P2PS_APP_Notification( P2PS_APP_ConnHandle_Not_evt_t *pNotification );
/* USER CODE BEGIN EF */
  void P2PS_APP_SW1_Button_Action( void );

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*P2P_SERVER_APP_H */
