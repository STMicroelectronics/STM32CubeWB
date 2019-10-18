/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hrs_app.h
  * @author  MCD Application Team
  * @brief   Header for hrs_application.c module
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HRS_APP_H
#define __HRS_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
  HR_CONN_HANDLE_EVT,
  HR_DISCON_HANDLE_EVT,
  HR_DISCON_SWITCH_HANDLE_EVT,
} HRS_APP__Opcode_Notification_evt_t;

typedef struct
{
  HRS_APP__Opcode_Notification_evt_t   HR_Evt_Opcode;
  uint16_t                              ConnectionHandle;
}HRS_APP_ConnHandle_Not_evt_t;
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
void HRS_APP_Notification( HRS_APP_ConnHandle_Not_evt_t *pNotification );
void HRSAPP_Init( void );
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*__HRS_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
