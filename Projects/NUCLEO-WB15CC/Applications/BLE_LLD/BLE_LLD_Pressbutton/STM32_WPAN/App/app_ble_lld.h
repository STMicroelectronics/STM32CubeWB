/**
 ******************************************************************************
  * File Name          : app_ble_lld.h
  * Description        : Header for BLE LLD application.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_BLE_LLD_H
#define APP_BLE_LLD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_lld.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Layer of indirection for command to M0 */
typedef PACKED_STRUCT
{
  void *command;
  uint32_t length;
} bleCmdIndirect_t;

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void APP_BLE_LLD_Init( void );
void APP_BLE_LLD_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_BLE_LLD_Init_UART_CLI(void);
void APP_BLE_LLD_DeInit_UART_CLI(void);

void CheckWirelessFirmwareInfo(void);
void logUart(const char *format, ...);
void logUartRaw(const char *str);
void APP_BLE_LLD_SendCmdM0(void *command);

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



/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
void Appli_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Appli_TIM_IC_CaptureCallback(void);
/* USER CODE END FD_WRAP_FUNCTIONS */

#endif /* APP_BLE_LLD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
