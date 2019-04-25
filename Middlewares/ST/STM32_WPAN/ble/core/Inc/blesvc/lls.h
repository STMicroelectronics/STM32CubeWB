/**
  ******************************************************************************
  * @file    lls.h
  * @author  MCD Application Team
  * @brief   Header for lls.c module
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
#ifndef __LLS_H
#define __LLS_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  LLS_NO_ALERT_EVT,
  LLS_MID_ALERT_EVT,
  LLS_HIGH_ALERT_EVT,
  LLS_DISCONNECT_EVT,
  LLS_CONNECT_EVT
} LLS_App_Opcode_Notification_evt_t;

typedef struct
{
  LLS_App_Opcode_Notification_evt_t  LLS_Evt_Opcode;
}LLS_App_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LLS_Init(void);
tBleStatus LLS_Update_Char(uint16_t UUID, uint8_t *pPayload);
void LLS_App_Notification(LLS_App_Notification_evt_t *pNotification);


#ifdef __cplusplus
}
#endif

#endif /*__LLS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
