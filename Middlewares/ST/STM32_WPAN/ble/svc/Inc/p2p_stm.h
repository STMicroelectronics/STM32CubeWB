
/**
  ******************************************************************************
  *  @file    p2p_stm.h
  * @author  MCD Application Team
  * @brief   Header for p2p_stm.c module
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
#ifndef __P2PS_STM_H
#define __P2PS_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

 
  
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  P2PS_STM__NOTIFY_ENABLED_EVT,
  P2PS_STM_NOTIFY_DISABLED_EVT,
  P2PS_STM_READ_EVT,
  P2PS_STM_WRITE_EVT,
  P2PS_STM_BOOT_REQUEST_EVT,
} P2PS_STM_Opcode_evt_t;

typedef struct
{
  uint8_t * pPayload;
  uint8_t     Length;
}P2PS_STM_Data_t;  

typedef struct
{
  P2PS_STM_Opcode_evt_t     P2P_Evt_Opcode;
  P2PS_STM_Data_t           DataTransfered;
  uint16_t                  ConnectionHandle;
  uint8_t                   ServiceInstance;
}P2PS_STM_App_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void P2PS_STM_Init( void );
void P2PS_STM_App_Notification(P2PS_STM_App_Notification_evt_t *pNotification);
tBleStatus P2PS_STM_App_Update_Char(uint16_t UUID,  uint8_t *pPayload);


#ifdef __cplusplus
}
#endif

#endif /*__P2PS_STM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
