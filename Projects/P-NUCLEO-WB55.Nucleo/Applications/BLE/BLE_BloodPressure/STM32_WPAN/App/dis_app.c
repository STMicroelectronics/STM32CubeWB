/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/dis_app.c
  * @author  MCD Application Team
  * @brief   Device Information Service Application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "ble.h"
#include "dis_app.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if ((BLE_CFG_DIS_SYSTEM_ID != 0) || (CFG_MENU_DEVICE_INFORMATION != 0))
static const uint8_t system_id[BLE_CFG_DIS_SYSTEM_ID_LEN_MAX] =
{
  (uint8_t)((DISAPP_MANUFACTURER_ID & 0xFF0000) >> 16),
  (uint8_t)((DISAPP_MANUFACTURER_ID & 0x00FF00) >> 8),
  (uint8_t)(DISAPP_MANUFACTURER_ID & 0x0000FF),
  0xFE,
  0xFF,
  (uint8_t)((DISAPP_OUI & 0xFF0000) >> 16),
  (uint8_t)((DISAPP_OUI & 0x00FF00) >> 8),
  (uint8_t)(DISAPP_OUI & 0x0000FF)
};
#endif

#if ((BLE_CFG_DIS_IEEE_CERTIFICATION != 0) || (CFG_MENU_DEVICE_INFORMATION != 0))
static const uint8_t ieee_id[BLE_CFG_DIS_IEEE_CERTIFICATION_LEN_MAX] =
{
  0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA,
  0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA,
  0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA,
  0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA, 0xFE, 0xCA,
};
#endif
#if ((BLE_CFG_DIS_PNP_ID != 0) || (CFG_MENU_DEVICE_INFORMATION != 0))
static const uint8_t pnp_id[BLE_CFG_DIS_PNP_ID_LEN_MAX] =
{
  0x1,
  0xAD, 0xDE,
  0xDE, 0xDA,
  0x01, 0x00
};
#endif
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void DISAPP_Init(void)
{
/* USER CODE BEGIN DISAPP_Init */

/* USER CODE END DISAPP_Init */
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */
