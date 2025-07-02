/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ble_at_server_cb.c
  * @author  BLE Application Team
  * @brief
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32wb_at.h"
#include "stm32wb_at_ble.h"
#include "stm32_seq.h"
#include "ble_defs.h"
#include "custom_app.h"
#include "ble_at_server_cb.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern Custom_Context_t Custom_Context;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
uint8_t stm32wb_at_BLE_ADV_cb(stm32wb_at_BLE_ADV_t *param);
uint8_t stm32wb_at_BLE_ADV_PARAM_server_cb(char *buff);
uint8_t stm32wb_at_BLE_SVC_server_cb(char *buff);
uint8_t stm32wb_at_BLE_INIT_server_cb(char *buff);
uint8_t stm32wb_at_BLE_PUB_ADDR_server_cb(char *buff);
uint8_t stm32wb_at_BLE_RAND_ADDR_server_cb(char *buff);
uint8_t stm32wb_at_BLE_CFG_RAND_ADDR_server_cb(char *buff);

uint8_t stm32wb_at_BLE_IO_CAPA_server_cb(char *buff);
uint8_t stm32wb_at_BLE_SET_AUTHEN_REQ_server_cb(char *buff);
uint8_t stm32wb_at_BLE_NAME_server_cb(char *buff);
uint8_t stm32wb_at_BLE_RF_POWER_server_cb(char *buff);
uint8_t stm32wb_at_BLE_CONN_INT_server_cb(char *buff);
uint8_t stm32wb_at_BLE_ADV_server_cb(char *buff);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

uint8_t global_svc_index = 0;
uint8_t global_svc_uuid_type;
uint16_t global_svc_uuid;
uint8_t global_max_attr;
uint8_t global_svc_id;
uint8_t global_char_id;
uint8_t global_char_uuid_type;
uint16_t global_char_uuid;
uint16_t global_char_value_len;
uint8_t global_char_properties;
uint8_t global_char_sec_permission;
uint8_t global_gatt_event_mask;
uint8_t global_pairing_confirm;
uint32_t global_pass_key_resp;
uint8_t global_io_capability;
uint8_t global_bonding;
uint8_t global_mitm;
uint8_t global_scsupport;
uint8_t global_usefixedpin;
uint32_t global_fixedpin;
uint8_t global_addrtype;
char global_name[20];
uint8_t global_bdaddress[6];
uint8_t global_rand_bdaddress[6];
uint8_t global_ble_addr_type = 0;
uint8_t global_ble_cfg_addr_type = 0;
float global_tx_power;
uint8_t global_tx_power_code;
float global_conn_int_min = 0;
float global_conn_int_max = 0;
uint8_t global_notif_serv_index = 0;
uint8_t global_notif_char_index = 0;
uint8_t global_notif_val_tab[64];
uint8_t global_notif_val_tab_len = 0;
uint8_t global_indic_serv_index = 0;
uint8_t global_indic_char_index = 0;
uint8_t global_indic_val_tab[64];
uint8_t global_indic_val_tab_len = 0;
uint8_t global_entry_number;
uint8_t global_adv_type;
float global_adv_int_min;
float global_adv_int_max;
uint8_t global_adv_enable;
uint8_t global_init_set;

/******************************************************************************/

uint8_t stm32wb_at_BLE_SVC_cb(stm32wb_at_BLE_SVC_t *param)
{
 global_svc_index = param->index;
 
  if (global_svc_index >= CUSTOM_APP_SVC_OFFSET)
  {
    global_svc_uuid_type = param->svc_uuid_type;
    global_svc_uuid = param->svc_uuid;
    global_max_attr = param->max_attr_record;

    UTIL_SEQ_SetTask( 1<<CFG_TASK_MANAGE_CUSTOM_SERVICE_INIT_ID, CFG_SCH_PRIO_0);    
  }
  else if (global_svc_index != 0)
  {  
    UTIL_SEQ_SetTask( 1<<CFG_TASK_MANAGE_SERVICE_INIT_ID, CFG_SCH_PRIO_0);
  }
  
  return 0;
}

uint8_t stm32wb_at_BLE_INIT_cb(stm32wb_at_BLE_INIT_t *param)
{
  global_init_set = param->init_set;
  
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MANAGE_INIT_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_CHAR_ADD_cb(stm32wb_at_BLE_CHAR_ADD_t *param)
{
  global_svc_id = param->svc_id;
  global_char_id = param->char_id;
  global_char_uuid_type = param->char_uuid_type;
  global_char_uuid = param->char_uuid;
  global_char_value_len = param->char_value_len;
  global_char_properties = param->char_properties;
  global_char_sec_permission = param->sec_permission;
  global_gatt_event_mask = param->gatt_evt_mask;
  
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MANAGE_CHARAC_INIT_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_PERIPH_SEC_REQ_cb(stm32wb_at_BLE_PERIPH_SEC_REQ_t *param)
{
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MANAGE_PERIPHERAL_SECUTITY_REQ_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_PAIRING_CONFIRM_cb(stm32wb_at_BLE_PAIRING_CONFIRM_t *param)
{
  global_pairing_confirm = param->confirm_yes_no;
  
  UTIL_SEQ_SetTask( 1<<CFG_TASK_PAIRING_CONFIRM_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_PASSKEY_RESP_cb(stm32wb_at_BLE_PASSKEY_RESP_t *param)
{
  global_pass_key_resp = param->pass_key_resp;
  
  UTIL_SEQ_SetTask( 1<<CFG_TASK_PASS_KEY_RESP_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_IO_CAPA_cb(stm32wb_at_BLE_IO_CAPA_t *param)
{
  global_io_capability = param->io_capability;
  
  UTIL_SEQ_SetTask( 1<<CFG_TASK_IO_CAPA_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_SET_AUTHEN_REQ_cb(stm32wb_at_BLE_SET_AUTHEN_REQ_t *param)
{
  global_bonding = param->bonding_mode;
  global_mitm = param->mitm;
  global_scsupport = param->sc_support;
  global_usefixedpin = param->use_fixed_pin;
  global_fixedpin = param->fixed_pin;
  global_addrtype = global_ble_addr_type | global_ble_cfg_addr_type;
  
  UTIL_SEQ_SetTask( 1<<CFG_TASK_SET_AUTHEN_REQ_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_CLEAR_DB_cb(stm32wb_at_BLE_CLEAR_DB_t *param)
{
  UTIL_SEQ_SetTask( 1<<CFG_TASK_CLEAR_DB_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_RST_cb(stm32wb_at_BLE_RST_t *param)
{
  UTIL_SEQ_SetTask( 1<<CFG_TASK_HCIRESET_ID, CFG_SCH_PRIO_0);
  
  return(0);
}

uint8_t stm32wb_at_BLE_PUB_ADDR_cb(stm32wb_at_BLE_PUB_ADDR_t *param)
{
  uint8_t i;
  
  global_ble_addr_type = GAP_PUBLIC_ADDR;
  for (i = 0 ; i < sizeof(param->BD_address) ; i++)
  {
    global_bdaddress[sizeof(param->BD_address) - 1 - i] = param->BD_address[i];
  }
  
  return 0;
}

uint8_t stm32wb_at_BLE_RAND_ADDR_cb(stm32wb_at_BLE_RAND_ADDR_t *param)
{
  uint8_t i;
  
  global_ble_addr_type = GAP_STATIC_RANDOM_ADDR;
  for (i = 0 ; i < sizeof(param->BD_address) ; i++)
  {
    global_rand_bdaddress[sizeof(param->BD_address) - 1 - i] = param->BD_address[i];
  }
  
  return 0;
}

uint8_t stm32wb_at_BLE_CFG_RAND_ADDR_cb(stm32wb_at_BLE_CFG_RAND_ADDR_t *param)
{
  global_ble_cfg_addr_type = param->cfg_rand_addr;
  
  return 0;
}

uint8_t stm32wb_at_BLE_NAME_cb(stm32wb_at_BLE_NAME_t *param)
{
  strcpy(global_name, param->name);
  
  return 0;
}

uint8_t stm32wb_at_BLE_RF_POWER_cb(stm32wb_at_BLE_RF_POWER_t *param)
{
  global_tx_power = param->tx_power;
  global_tx_power_code = param->tx_power_code;
  
  return 0;
}

uint8_t stm32wb_at_BLE_NOTIF_VAL_cb(stm32wb_at_BLE_NOTIF_VAL_t *param)
{
  global_notif_serv_index = param->svc_index;
  global_notif_char_index = param->char_index;
  memcpy(&global_notif_val_tab[0], &param->val_tab[0], sizeof(global_notif_val_tab) );
  global_notif_val_tab_len = param->val_tab_len;

  UTIL_SEQ_SetTask( 1<<CFG_TASK_UPDATE_CHAR_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_INDIC_VAL_cb(stm32wb_at_BLE_INDIC_VAL_t *param)
{

  global_indic_serv_index = param->svc_index;
  global_indic_char_index = param->char_index;
  memcpy(&global_indic_val_tab[0], &param->val_tab[0], sizeof(global_indic_val_tab) );
  global_indic_val_tab_len = param->val_tab_len;

  UTIL_SEQ_SetTask( 1<<CFG_TASK_INDICATION_UPDATE_CHAR_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_CONN_INT_cb(stm32wb_at_BLE_CONN_INT_t *param)
{
  global_conn_int_min = param->min;
  global_conn_int_max = param->max;

  if ((global_conn_int_min != 0) && (global_conn_int_max != 0))
  {
    UTIL_SEQ_SetTask( 1<<CFG_TASK_MANAGE_CONN_PARAM_ID, CFG_SCH_PRIO_0);
  }

  return 0;
}

uint8_t stm32wb_at_BLE_ADV_PARAM_cb(stm32wb_at_BLE_ADV_PARAM_t *param)
{
  global_entry_number = param->entry_number;
  global_adv_type = param->adv_type;
  if (global_entry_number == 0)
  {
    global_adv_int_min = (param->adv_int_min) / 0.625f;
    global_adv_int_max = (param->adv_int_max) / 0.625f;
  }
  else
  {
    global_adv_int_min = param->adv_int_min;
    global_adv_int_max = param->adv_int_max;    
  }  
  
  return 0;
}

uint8_t stm32wb_at_BLE_ADV_cb(stm32wb_at_BLE_ADV_t *param)
{
  global_adv_enable = param->enable;
  
  UTIL_SEQ_SetTask( 1<<CFG_TASK_MANAGE_START_STOP_ADV_ID, CFG_SCH_PRIO_0);
  
  return 0;
}

uint8_t stm32wb_at_BLE_ADV_PARAM_server_cb(char *buff)
{
  sprintf(buff, "%d,%d,%.1f,%.1f,%d", 
                  global_entry_number,
                  global_adv_type,
                  (double)(global_adv_int_min * 0.625f),
                  (double)(global_adv_int_max * 0.625f),
                  global_ble_addr_type | global_ble_cfg_addr_type);

  return 0;
}

uint8_t stm32wb_at_BLE_SVC_server_cb(char *buff)
{
  uint8_t i, j = 0;
  char str_tmp[20];
  
  if (global_svc_index < CUSTOM_APP_SVC_OFFSET)
  {
    sprintf(buff, "%d", global_svc_index);
  }
  else
  {
    for(i = 0 ; i < CUSTOM_APP_SVC_COUNT ; i++)
    {
      if(Custom_Context.SvcHdle[i] != 0)
      {
        if(j != 0)
        {
          (void)strcat(buff, AT_SEPARATOR);
        }
        j = 1;
        (void)sprintf(str_tmp, "%d", i + CUSTOM_APP_SVC_OFFSET);
        (void)strcat(buff, str_tmp);
        
      }
    }
  }

  return 0;
}

uint8_t stm32wb_at_BLE_INIT_server_cb(char *buff)
{
  int8_t i;
  char str_tmp[20];

  if ((global_ble_addr_type == GAP_PUBLIC_ADDR) && (global_ble_cfg_addr_type == GAP_PUBLIC_ADDR))
  {
    (void)strcat(buff, "0x");
    for(i = sizeof(global_bdaddress) - 1 ; i >= 0 ; i--)
    {
      (void)sprintf(str_tmp, "%02x", global_bdaddress[i]);
      (void)strcat(buff, str_tmp);
    }
    (void)strcat(buff, AT_SEPARATOR);
  }
  else
  {
    (void)strcat(buff, "0x");
    for(i = sizeof(global_rand_bdaddress) - 1 ; i >= 0 ; i--)
    {
      (void)sprintf(str_tmp, "%02x", global_rand_bdaddress[i]);
      (void)strcat(buff, str_tmp);
    }
    (void)strcat(buff, AT_SEPARATOR);
  }
  
  (void)strcat(buff, global_name);
  (void)strcat(buff, AT_SEPARATOR);
  
  (void)sprintf(str_tmp, "%.2f", (double)global_tx_power);
  (void)strcat(buff, str_tmp);

  return 0;
}  

uint8_t stm32wb_at_BLE_PUB_ADDR_server_cb(char *buff)
{
  int8_t i;
  char str_tmp[20];
  
  (void)strcat(buff, "0x");
  for(i = sizeof(global_bdaddress) - 1 ; i >= 0 ; i--)
  {
    (void)sprintf(str_tmp, "%02x", global_bdaddress[i]);
    (void)strcat(buff, str_tmp);
  }

  return 0;
}
          
uint8_t stm32wb_at_BLE_RAND_ADDR_server_cb(char *buff)
{
  int8_t i;
  char str_tmp[20];
  
  (void)strcat(buff, "0x");
  for(i = sizeof(global_rand_bdaddress) - 1 ; i >= 0 ; i--)
  {
    (void)sprintf(str_tmp, "%02x", global_rand_bdaddress[i]);
    (void)strcat(buff, str_tmp);
  }

  return 0;
}

uint8_t stm32wb_at_BLE_CFG_RAND_ADDR_server_cb(char *buff)
{
  sprintf(buff, "%d", global_ble_cfg_addr_type);
  return 0;
}

uint8_t stm32wb_at_BLE_IO_CAPA_server_cb(char *buff)
{
  sprintf(buff, "%d", global_io_capability);
  return 0;
}

uint8_t stm32wb_at_BLE_SET_AUTHEN_REQ_server_cb(char *buff)
{
  sprintf(buff, "%d,%d,%d,%d,0x%lx,%d", global_bonding, global_mitm, global_scsupport, global_usefixedpin, global_fixedpin,
          global_ble_cfg_addr_type | global_ble_addr_type);
  return 0;
}

uint8_t stm32wb_at_BLE_NAME_server_cb(char *buff)
{
  sprintf(buff, "%s", &global_name[0]);

  return 0;
}

uint8_t stm32wb_at_BLE_RF_POWER_server_cb(char *buff)
{
  sprintf(buff, "%.2f", (double)global_tx_power);

  return 0;
}

uint8_t stm32wb_at_BLE_CONN_INT_server_cb(char *buff)
{
  sprintf(buff, "%.2f,%.2f", (double)global_conn_int_min, (double)global_conn_int_max);

  return 0;
}

uint8_t stm32wb_at_BLE_ADV_server_cb(char *buff)
{
  sprintf(buff, "%d", global_adv_enable);

  return 0;
}

/* USER CODE END 4 */
