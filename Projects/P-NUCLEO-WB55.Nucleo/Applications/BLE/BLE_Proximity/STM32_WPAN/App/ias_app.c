/**
  ******************************************************************************
  * @file    ias_app.c
  * @author  MCD Application Team
  * @brief   Immediate Alert Service Application
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

#include "dbg_trace.h"
#include "ias_app.h"
#include "ble.h"
#include "stm32_seq.h"
#include "app_ble.h"



/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

/**
 * END of Section BLE_APP_CONTEXT
 */

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void IASAPP_Init(void)
{
}


void IAS_App_Notification(IAS_App_Notification_evt_t *pNotification)
{
  switch(pNotification->IAS_Evt_Opcode)
  {
    case IAS_NO_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("NO ALERT\n");
        BSP_LED_Off(LED_RED);
        BSP_LED_Off(LED_BLUE);
        BSP_LED_On(LED_GREEN);
      }
      break;

    case IAS_MID_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("MID ALERT\n");
        BSP_LED_Off(LED_GREEN);
        BSP_LED_Off(LED_RED);
        BSP_LED_On(LED_BLUE);
      }
      break;

    case IAS_HIGH_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("HIGH ALERT\n");
        BSP_LED_Off(LED_GREEN);
        BSP_LED_Off(LED_BLUE);
        BSP_LED_On(LED_RED);
      }
      break;

   default:
      break;
  }

  return;
}

