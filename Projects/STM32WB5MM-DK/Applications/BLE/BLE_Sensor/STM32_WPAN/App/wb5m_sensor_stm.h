
/**
  ******************************************************************************
  * @file    motenv_stm.h
  * @author  SRA/AST
  * @brief   Header for motenv_stm.c module
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
