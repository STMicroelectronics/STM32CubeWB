/**
  ******************************************************************************
  * @file    ias.h
  * @author  MCD Application Team
  * @brief   Header for ias.c module
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
#ifndef __IAS_H
#define __IAS_H

#ifdef __cplusplus
extern "C" 
{
#endif


/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  IAS_NO_ALERT_EVT,
  IAS_MID_ALERT_EVT,
  IAS_HIGH_ALERT_EVT
} IAS_App_Opcode_Notification_evt_t;

typedef struct
{
  IAS_App_Opcode_Notification_evt_t  IAS_Evt_Opcode;
}IAS_App_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void IAS_Init(void);
tBleStatus IAS_Update_Char(uint16_t UUID, uint8_t *pPayload);
void IAS_App_Notification(IAS_App_Notification_evt_t *pNotification);


#ifdef __cplusplus
}
#endif

#endif /*__IAS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
