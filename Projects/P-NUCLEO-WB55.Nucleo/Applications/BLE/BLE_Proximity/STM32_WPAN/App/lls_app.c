/**
  ******************************************************************************
  * @file    lls_app.c
  * @author  MCD Application Team
  * @brief   Link Loss Service Application
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
#include "lls_app.h"
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

PLACE_IN_SECTION("BLE_APP_CONTEXT") static uint8_t AlertLevel;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void LLSAPP_Init(void)
{
  AlertLevel = LLS_NO_ALERT_EVT;
}


void LLS_App_Notification(LLS_App_Notification_evt_t *pNotification)
{
  switch(pNotification->LLS_Evt_Opcode)
  {
    case LLS_NO_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("Set Link Loss Alert Level to NO ALERT\n");
        AlertLevel = LLS_NO_ALERT_EVT;
      }
      break;

    case LLS_MID_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("Set Link Loss Alert Level to MID ALERT\n");
        AlertLevel = LLS_MID_ALERT_EVT;
      }
      break;

    case LLS_HIGH_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("Set Link Loss Alert Level to HIGH ALERT\n");
        AlertLevel = LLS_HIGH_ALERT_EVT;
      }
      break;

    case LLS_CONNECT_EVT:
      {
        BLE_DBG_APP_MSG("Connected with NO ALERT\n");
        BSP_LED_Off(LED_RED);
        BSP_LED_Off(LED_BLUE);
        BSP_LED_On(LED_GREEN);
      }
      break;

    case LLS_DISCONNECT_EVT:
      {
        if(AlertLevel == LLS_NO_ALERT_EVT)
        {
          BLE_DBG_APP_MSG("Disconnected with NO ALERT\n");
          BSP_LED_Off(LED_RED);
          BSP_LED_Off(LED_BLUE);
          BSP_LED_Off(LED_GREEN);
        }
        else if(AlertLevel == LLS_MID_ALERT_EVT)
        {
          BLE_DBG_APP_MSG("Disconnected with MID ALERT\n");
          BSP_LED_Off(LED_RED);
          BSP_LED_On(LED_BLUE);
          BSP_LED_Off(LED_GREEN);
        }
        else
        {
          BLE_DBG_APP_MSG("Disconnected with HIGH ALERT\n");
          BSP_LED_On(LED_RED);
          BSP_LED_Off(LED_BLUE);
          BSP_LED_Off(LED_GREEN);
        }
      }
      break;

   default:
      break;
  }

  return;
}

