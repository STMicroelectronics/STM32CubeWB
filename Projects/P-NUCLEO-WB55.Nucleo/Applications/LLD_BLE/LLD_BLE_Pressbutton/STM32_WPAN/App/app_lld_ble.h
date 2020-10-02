/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_lld_ble.h
  * Description        : Header for LLD BLE application.
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
#ifndef APP_LLD_BLE_H
#define APP_LLD_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Application errors                 */
/*------------------------------------*/

/*
 *  List of all errors tracked by the Thread application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum
{
  ERR_LLD_BLE_SET_STATE_CB,
  ERR_LLD_BLE_ERASE_PERSISTENT_INFO,
  ERR_LLD_BLE_CHECK_WIRELESS
  } ErrAppLldBleIdEnum_t;

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void APP_LLD_BLE_Init( void );
void APP_LLD_BLE_Error(uint32_t ErrId, uint32_t ErrCode);
void APP_LLD_BLE_ProcessMsgM0ToM4(void);
void APP_LLD_BLE_Init_UART_CLI(void);
void APP_LLD_BLE_DeInit_UART_CLI(void);
void APP_LLD_BLE_Init_TL(void);
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

/* only for M4 */
uint8_t APP_LLD_BLE_SendCmdM0(uint8_t currentCmd , uint32_t* currentPt);

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
void Appli_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Appli_TIM_IC_CaptureCallback(void);
void Appli_TIM_PeriodElapsedCallback(void);
/* USER CODE END FD_WRAP_FUNCTIONS */

#endif /* APP_LLD_BLE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/