/* USER CODE BEGIN Header */
/**
  ******************************************************************************
 * @file    dt_client_app.h
 * @author  MCD Application Team
 * @brief   Header for dt_client_app.c module
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DTC_APPLICATION_H
#define DT_CLIENT_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#define BUTTON_PRESSED                                            GPIO_PIN_RESET
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions --------------------------------------------------------*/
void DTC_App_Init(void);
void DTC_App_LinkReadyNotification(uint16_t ConnectionHandle);
void DTCAPP_Update_Service(void);
void DTC_App_KeyButton1Action(void);
void DTC_App_KeyButton2Action( void );
void DTC_App_KeyButton3Action( void );
void Resume_Write(void);
void DTC_Button1TriggerReceived( void );
void DTC_Button2TriggerReceived( void );
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /* DTC_APPLICATION_H */
