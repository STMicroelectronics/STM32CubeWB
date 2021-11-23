/**
  ******************************************************************************
  * @file    ble_at_server.h
  * @author  BLE Application Team
  * @brief
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLE_AT_SERVER_H
#define BLE_AT_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "stm32wb_at.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
uint8_t ble_at_server_Init(void);
uint8_t ble_at_server_Process_rx_frame(char * str);
uint8_t ble_at_server_Send_evt(stm32wb_at_BLE_EVT_t cmd, void *param);
FOREACH_AT_BLE_CMD(GENERATE_AT_BLE_SERVER_CB_PROTO)

#ifdef __cplusplus
}
#endif

#endif /* AT_BLE_H */
