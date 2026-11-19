/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Inc/system_config.h
  * @author  MCD Application Team
  * @brief   Header for system_config.c module
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
/* USER CODE BEGIN 1 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"

/* Exported types ------------------------------------------------------------*/

/** @defgroup  System Status Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_OK    = 0x00, /*!< System status OK             */
  SYSTEM_ERROR = 0x01  /*!< System status ERROR          */

} System_StatusTypeDef;

/** @defgroup  System Low Power Mode Scenario Enumeration Definition.
  * @{
  */
typedef enum
{
  SYSTEM_LOWPOWER_S0,         /*!< Run    - 64 MHz (PLL from MSI).                     */
  SYSTEM_LOWPOWER_S1,         /*!< Run    - 16 MHz (HSI16).           */
  SYSTEM_LOWPOWER_S2,         /*!< Sleep  - 2 MHz  (MSI, low power regulator).         */
  SYSTEM_LOWPOWER_S3,         /*!< Stop 0 - main regulator.                            */
  SYSTEM_LOWPOWER_S4,         /*!< Stop 1 - low power regulator.                       */
  SYSTEM_LOWPOWER_S5,         /*!< Standby - SRAM retention + RTC.                     */
  SYSTEM_LOWPOWER_S6,         /*!< Standby - no SRAM retention + RTC.                  */
  SYSTEM_LOWPOWER_S7,         /*!< Standby - no SRAM retention + no RTC.               */
  SYSTEM_LOWPOWER_S8,         /*!< Shutdown - w/ RTC.                                  */
  SYSTEM_LOWPOWER_S9,         /*!< Shutdown - wo/ RTC.                                 */
  SYSTEM_LOWPOWER_NB
} System_LowPowerModeScenarioTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void system_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_lowpower_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_rtc_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_clock_config(System_LowPowerModeScenarioTypeDef scenario);
System_StatusTypeDef system_ram_retention_config(System_LowPowerModeScenarioTypeDef scenario);

#endif /* SYSTEM_CONFIG_H */
/* USER CODE END 1 */