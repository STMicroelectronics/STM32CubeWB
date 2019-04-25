
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __P2P_SERVER_APP_H
#define __P2P_SERVER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Includes ------------------------------------------------------------------*/
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


  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  /* Exported macros -----------------------------------------------------------*/
  /* Exported functions ------------------------------------------------------- */

  void P2PS_APP_Init( void );
  void P2PS_APP_Notification( P2PS_APP_ConnHandle_Not_evt_t *pNotification );
  void P2PS_APP_SW1_Button_Action( void );



#ifdef __cplusplus
}
#endif

#endif /*__P2P_SERVER_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
