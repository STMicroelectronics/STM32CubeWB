/**
  ******************************************************************************
  * @file    tps_app.c
  * @author  MCD Application Team
  * @brief   TX Power Service Application
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



/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "tps_app.h"
#include "ble.h"
#include "stm32_seq.h"
#include "app_ble.h"



/* Private typedef -----------------------------------------------------------*/
static int8_t TxPower;


/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static int8_t TxPower;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void TPSAPP_Init(void)
{
  /**
   * Set TX Power
   */
  TxPower = 0;
  TPS_Update_Char(TX_POWER_LEVEL_CHARACTERISTIC_UUID, (uint8_t *)&TxPower);

  return;
}

