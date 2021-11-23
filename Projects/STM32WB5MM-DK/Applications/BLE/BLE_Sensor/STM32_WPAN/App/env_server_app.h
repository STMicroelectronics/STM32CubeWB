/**
  ******************************************************************************
  * File Name          : env_server_app.h
  * Description        : Handle HW/Environmental Service/Char
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
#ifndef ENV_SERVER_APP_H
#define ENV_SERVER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ENV_Context_Init(void);
void ENV_Set_Notification_Status(uint8_t status);
void ENV_Send_Notification_Task(void);
void ENV_Update(void);

#ifdef __cplusplus
}
#endif

#endif /* ENV_SERVER_APP_H */

