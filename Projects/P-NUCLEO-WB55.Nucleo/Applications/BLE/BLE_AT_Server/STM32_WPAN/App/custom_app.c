/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/custon_app.c
  * Description        : Custom Service Application
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

#include "ble.h"
#include "hrs_app.h"
#include "stm32_seq.h"
#include "app_ble.h"
#include "stm32wb_at_ble.h"
#include "custom_app.h"
#include "ble_at_server.h"

extern Custom_Context_t Custom_Context;
extern uint8_t global_notif_serv_index;
extern uint8_t global_notif_char_index;
extern uint8_t global_notif_val_tab[64];
extern uint8_t global_notif_val_tab_len;
extern uint8_t global_indic_serv_index;
extern uint8_t global_indic_char_index;
extern uint8_t global_indic_val_tab[64];
extern uint8_t global_indic_val_tab_len;
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/

/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

//HRSAPP_Context_t HRSAPP_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private functions prototypes-----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void CUSTOMS_Notification( CUSTOMS_App_Notification_evt_t *pNotification )
{
  stm32wb_at_BLE_EVT_WRITE_t param_BLE_EVT_WRITE;
  uint8_t i;
  
  switch (pNotification->Evt_Opcode)
  {
    case CUSTOM_NOTIFICATION_ENABLED:
      printf("custom_app notification enabled \r\n");
      break;

    case CUSTOM_NOTIFICATION_DISABLED:
      printf("custom_app notification disabled \r\n");
      break;
      
    case CUSTOM_INDICATION_ENABLED:
      printf("custom_app indication enabled \r\n");
      break;
      
    case CUSTOM_INDICATION_DISABLED:
      printf("custom_app indication or notification disabled \r\n");
      break;      
      
    case CUSTOM_WRITE_EVT:
      param_BLE_EVT_WRITE.svc_index = pNotification->service_index;
      param_BLE_EVT_WRITE.char_index = pNotification->charac_index;
      param_BLE_EVT_WRITE.val_tab_len = pNotification->length;
      for (i = 0; i < param_BLE_EVT_WRITE.val_tab_len; i++)
      {
        param_BLE_EVT_WRITE.val_tab[i] = pNotification->charac_value[i];
      }
      ble_at_server_Send_evt(BLE_EVT_WRITE, &param_BLE_EVT_WRITE);
      break;

    default:
      break;
  }

  return;
}

void Custom_Send_Notification_ATcommand(void)
{
  aci_gatt_update_char_value(Custom_Context.SvcHdle[global_notif_serv_index - CUSTOM_APP_SVC_OFFSET],
                             Custom_Context.CharHdle[global_notif_serv_index - CUSTOM_APP_SVC_OFFSET][global_notif_char_index - 1],
                             0, /* charValOffset */
                             global_notif_val_tab_len, /* charValueLen */
                             &global_notif_val_tab[0]);
  
  return;
}

void Custom_Send_Indication_ATcommand(void)
{
  aci_gatt_update_char_value(Custom_Context.SvcHdle[global_notif_serv_index - CUSTOM_APP_SVC_OFFSET],
                             Custom_Context.CharHdle[global_indic_serv_index - CUSTOM_APP_SVC_OFFSET][global_indic_char_index - 1],
                             0, /* charValOffset */
                             global_indic_val_tab_len, /* charValueLen */
                             &global_indic_val_tab[0]);
  
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

