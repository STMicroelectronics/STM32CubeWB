/**
  ******************************************************************************
  * @file    bas.h
  * @author  MCD Application Team
  * @brief   Header for bas.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#ifndef __BAS_H
#define __BAS_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  BAS_LEVEL_NOT_ENABLED_EVT,
  BAS_LEVEL_NOT_DISABLED_EVT,
  BAS_LEVEL_READ_EVT
} BAS_Opcode_Notification_evt_t;

typedef struct
{
  BAS_Opcode_Notification_evt_t  BAS_Evt_Opcode;
  uint8_t ServiceInstance;
}BAS_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define BAS_LEVEL_NOTIFICATION_OPTION                                          1


/* Exported functions ------------------------------------------------------- */
void BAS_Init(void);
void BAS_Update_Char(uint16_t UUID, uint8_t service_instance, uint8_t *pPayload);
void BAS_Notification(BAS_Notification_evt_t * pNotification);

#ifdef __cplusplus
}
#endif

#endif /*__BAS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
