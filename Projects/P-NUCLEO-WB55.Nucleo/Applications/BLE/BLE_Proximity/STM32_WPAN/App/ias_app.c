/* USER CODE BEGIN Header */
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "dbg_trace.h"
#include "ias_app.h"
#include "ble.h"
#include "stm32_seq.h"
#include "app_ble.h"

/* Private includes ----------------------------------------------------------*/
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
/**
 * START of Section BLE_APP_CONTEXT
 */

/**
 * END of Section BLE_APP_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Functions Definition ------------------------------------------------------*/
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
        APP_DBG_MSG("LED RED OFF\n");
        BSP_LED_Off(LED_BLUE);
        APP_DBG_MSG("LED_BLUE OFF\n");
        BSP_LED_On(LED_GREEN);
        APP_DBG_MSG("LED GREEN ON\n");
      }
      break;

    case IAS_MID_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("MILD ALERT\n");
        BSP_LED_Off(LED_GREEN);
        APP_DBG_MSG("LED GREEN OFF\n");
        BSP_LED_Off(LED_RED);
        APP_DBG_MSG("LED RED OFF\n");
        BSP_LED_On(LED_BLUE);
        APP_DBG_MSG("LED BLUE ON\n");
      }
      break;

    case IAS_HIGH_ALERT_EVT:
      {
        BLE_DBG_APP_MSG("HIGH ALERT\n");
        BSP_LED_Off(LED_GREEN);
        APP_DBG_MSG("LED GREEN OFF\n");
        BSP_LED_Off(LED_BLUE);
        APP_DBG_MSG("LED_BLUE OFF\n");
        BSP_LED_On(LED_RED);
        APP_DBG_MSG("LED RED ON\n");
      }
      break;

   default:
      break;
  }

  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */
