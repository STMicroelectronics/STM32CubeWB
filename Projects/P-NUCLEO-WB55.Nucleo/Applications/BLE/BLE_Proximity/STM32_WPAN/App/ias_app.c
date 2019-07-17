/**
  ******************************************************************************
  * @file    ias_app.c
  * @author  MCD Application Team
  * @brief   Immediate Alert Service Application
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
