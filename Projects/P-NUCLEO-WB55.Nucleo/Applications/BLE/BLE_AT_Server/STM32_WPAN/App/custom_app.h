/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/custom_app.h
  * Description        : Header for custom_app.c module
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
#ifndef CUSTOM_APP_H
#define CUSTOM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#define CUSTOM_APP_SVC_OFFSET   3
#define CUSTOM_APP_SVC_COUNT    5
#define CUSTOM_APP_CHAR_COUNT   5

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/

/* USER CODE BEGIN ET */
typedef struct
{
  uint16_t SvcHdle[CUSTOM_APP_SVC_COUNT];
  uint16_t CharHdle[CUSTOM_APP_SVC_COUNT][CUSTOM_APP_CHAR_COUNT];
} Custom_Context_t;

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

/* USER CODE BEGIN EF */
  void Custom_Send_Notification_ATcommand(void);
  void Custom_Send_Indication_ATcommand(void);

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /* CUSTOM_APP_H */

