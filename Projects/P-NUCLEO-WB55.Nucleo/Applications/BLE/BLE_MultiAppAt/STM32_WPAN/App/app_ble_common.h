/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_ble_sv.h
  * @author  MCD Application Team
  * @brief   Header for ble application
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
#ifndef APP_BLE_COMMON_H
#define APP_BLE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hci_tl.h"

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
  typedef enum
  {
    APP_BLE_IDLE,
    APP_BLE_FAST_ADV,
    APP_BLE_LP_ADV,
    APP_BLE_SCAN,
    APP_BLE_LP_CONNECTING,
    APP_BLE_CONNECTED_SERVER,
    APP_BLE_CONNECTED_CLIENT,
    
    APP_BLE_DISCOVER_SERVICES,
    APP_BLE_DISCOVER_CHARACS,
    APP_BLE_DISCOVER_WRITE_DESC,
    APP_BLE_DISCOVER_NOTIFICATION_CHAR_DESC,
    APP_BLE_ENABLE_NOTIFICATION_DESC,
    APP_BLE_DISABLE_NOTIFICATION_DESC
  } APP_BLE_ConnStatus_t;

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ---------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /*APP_BLE_COMMON_H */
