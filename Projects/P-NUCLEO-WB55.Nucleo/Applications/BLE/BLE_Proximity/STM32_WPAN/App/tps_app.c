/**
  ******************************************************************************
  * @file    tps_app.c
  * @author  MCD Application Team
  * @brief   TX Power Service Application
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
