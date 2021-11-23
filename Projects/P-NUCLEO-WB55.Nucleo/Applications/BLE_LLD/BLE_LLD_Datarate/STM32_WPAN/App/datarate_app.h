/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : datarate_app.h
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
#ifndef DATARATE_APP_H
#define DATARATE_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  dataRateOff,
  dataRateReceive,
  dataRateTransmit
}dataRate_t;
/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void DATARATE_APP_Init(void);
void Appli_TS_Callback(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

