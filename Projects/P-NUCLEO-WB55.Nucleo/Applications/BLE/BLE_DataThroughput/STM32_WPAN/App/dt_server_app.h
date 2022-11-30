/* USER CODE BEGIN Header */
/**
  ******************************************************************************
 * @file    dt_server_app.h
 * @author  MCD Application Team
 * @brief   Header for dt_server_app.c module
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
#ifndef DTS_SERVER_APP_H
#define DTS_SERVER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#define BUTTON_PRESSED                                            GPIO_PIN_RESET

/* Exported types ------------------------------------------------------------*/
typedef GPIO_PinState BUTTON_STATE;
typedef void (* IO_RECEIVE_DATA_USER_CALLBACK_TYPE) (uint8_t * rx_data, uint16_t data_size);
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
void DTS_App_Init(void);
void DTS_App_KeyButton1Action(void);
void DTS_App_KeyButton2Action( void );
void DTS_App_KeyButton3Action( void );
void DTS_App_TxPoolAvailableNotification(void);

void DTS_Button1TriggerReceived( void );
void DTS_Button2TriggerReceived( void );
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /* DTS_SERVER_APP_H */
