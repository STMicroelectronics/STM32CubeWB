
/**
  ******************************************************************************
  * @file    motenv_stm.h
  * @author  SRA/AST
  * @brief   Header for motenv_stm.c module
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
#ifndef MOTENV_STM_H
#define MOTENV_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  MOTENV Event Opcode definition
 */
typedef enum
{
  /* HW Service Chars related events */
  HW_MOTION_NOTIFY_ENABLED_EVT,
  HW_MOTION_NOTIFY_DISABLED_EVT,
  HW_ENV_NOTIFY_ENABLED_EVT,
  HW_ENV_NOTIFY_DISABLED_EVT,
  HW_ENV_READ_EVT,
} MOTENV_STM_Opcode_evt_t;

/**
 * @brief  MOTENV Event data structure definition
 */
typedef struct
{
  uint8_t *pPayload;
  uint8_t  Length;
} MOTENV_STM_Data_t;  

/**
 * @brief  MOTENV Notification structure definition
 */
typedef struct
{
  MOTENV_STM_Opcode_evt_t Motenv_Evt_Opcode;
  MOTENV_STM_Data_t       DataTransfered;
  uint16_t                ConnectionHandle;
  uint8_t                 ServiceInstance;
} MOTENV_STM_App_Notification_evt_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/**
 * @brief  Motion (Acc-Gyro-Magneto) Char shortened UUID
 */
#define MOTION_CHAR_UUID                (0xE000)
/**
 * @brief  Environmental (Temp-Humidity-Pressure) Char shortened UUID
 */
#define ENV_CHAR_UUID                   (0x1D00)

/* Exported functions ------------------------------------------------------- */
void MOTENV_STM_Init(void);
void MOTENV_STM_App_Notification(MOTENV_STM_App_Notification_evt_t *pNotification);
tBleStatus MOTENV_STM_App_Update_Char(uint16_t UUID, uint8_t payloadLen, uint8_t *pPayload);


#ifdef __cplusplus
}
#endif

#endif /* MOTENV_STM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
