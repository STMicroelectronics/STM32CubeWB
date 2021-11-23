/**
  ******************************************************************************
  * File Name          : motion_server_app.h
  * Description        : Handle Motion (Acc/Gyro/Mag) Service
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
#ifndef MOTION_SERVER_APP_H
#define MOTION_SERVER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MOTION_Context_Init(void);
void MOTION_Set_Notification_Status(uint8_t status);
void MOTION_Send_Notification_Task(void);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_SERVER_APP_H */

