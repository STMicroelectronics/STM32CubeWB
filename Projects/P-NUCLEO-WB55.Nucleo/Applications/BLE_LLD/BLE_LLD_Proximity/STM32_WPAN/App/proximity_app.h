/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : proximity_app.h
  * Description        : Header for BLE LLD application.
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
#ifndef PROXIMITY_APP_H
#define PROXIMITY_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void PROXIMITY_APP_Init(void);
void Appli_TIM_PeriodElapsedCallback(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

