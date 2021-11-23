/**
 ******************************************************************************
  * File Name          : app_ble_lld.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_BLE_LLD_H
#define APP_BLE_LLD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_lld_transport.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void APP_BLE_LLD_Init(void);
void APP_BLE_LLD_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_BLE_LLD_Init_UART(void);
void APP_BLE_LLD_DeInit_UART(void);

void APP_BLE_LLD_uartRxStart(void(*cb)(char));

void CheckWirelessFirmwareInfo(void);
void uartWrite(const char *format, ...);
void uartWriteRaw(const char *str);
uint8_t APP_BLE_LLD_SendCmdM0(BLE_LLD_Code_t bleCmd);

/**
 * @brief Active polling for a given delay
 * @param microsec the delay in us unit
 **/
void us_delay_16m(uint32_t microsec);
void us_delay_32m(uint32_t microsec);
#ifdef USE_SYS_CLOCK_DIV_2
#define us_delay us_delay_16m
#else
#define us_delay us_delay_32m
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* APP_BLE_LLD_H */

