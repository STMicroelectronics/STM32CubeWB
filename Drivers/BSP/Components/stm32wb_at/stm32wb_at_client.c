/**
  ******************************************************************************
  * @file    stm32wb_at_client.c
  * @author  MCD Application Team
  * @brief   AT client.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32wb_at_client.h"

uint8_t str_to_byte_tab(char *str_in, uint8_t *tab, uint8_t *tab_len);
extern float tab_conv_tx_power[32];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SRV_CUSTOM_APP_SVC_OFFSET     3
#define SRV_CUSTOM_APP_SVC_COUNT      5

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static char client_buff_tx[64];
static stm32wb_at_BLE_CMD_t client_current_cmd;

/* Private function prototypes -----------------------------------------------*/
static uint8_t stm32wb_at_client_Wait_ready(void);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Init the at client
 * @param None
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t stm32wb_at_client_Init(void)
{
  (void)memset(&client_buff_tx[0], 0, sizeof(client_buff_tx));
  client_current_cmd =  BLE_NONE;

  return 0;
}

/**
 * @brief Query the server on a specified command
 * @param cmd code to query
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t stm32wb_at_client_Query(stm32wb_at_BLE_CMD_t cmd)
{
  uint8_t status;

  (void)stm32wb_at_client_Wait_ready();

  if(cmd == BLE_TEST)
  {
    (void)strcpy(client_buff_tx, AT_CMD_TEST);
    (void)strcat(client_buff_tx, AT_EOL);

    client_current_cmd = cmd;
    status = stm32wb_at_ll_Transmit((uint8_t *)&client_buff_tx[0], (uint16_t)strlen(client_buff_tx));
    (void)memset(&client_buff_tx[0], 0, sizeof(client_buff_tx));
  }
  else if(cmd < BLE_NONE)
  {
    (void)strcpy(client_buff_tx, AT_PRE_CMD);
    (void)strcat(client_buff_tx, AT_BLE_CMD_STRING[cmd]);
    (void)strcat(client_buff_tx, AT_GET);
    (void)strcat(client_buff_tx, AT_EOL);

    client_current_cmd = cmd;
    status = stm32wb_at_ll_Transmit((uint8_t *)&client_buff_tx[0], (uint16_t)strlen(client_buff_tx));
    (void)memset(&client_buff_tx[0], 0, sizeof(client_buff_tx));
  }
  else
  {
    status = 1;
  }

  return status;
}

/**
 * @brief Process an At string and perform required actions
 * @param str pointer to the string to process
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t stm32wb_at_client_Process_rx_frame(char * str)
{
  uint8_t status;
  stm32wb_at_BLE_EVT_t current_evt;

  status = 1;

  if( strcmp(str, AT_CMD_STATUS_OK) == 0)
  {
    client_current_cmd = BLE_NONE;
    status = 0;
  }
  else if( strcmp(str, AT_CMD_STATUS_ERROR) == 0)
  {
    client_current_cmd = BLE_NONE;
    status = 0;
  }
  else if( strncmp(str, AT_REPLY, strlen(AT_REPLY)) == 0)
  {
    str++;
    /* check if it's a response of current command */
    if( strncmp(str, AT_BLE_CMD_STRING[client_current_cmd], strlen(AT_BLE_CMD_STRING[client_current_cmd])) == 0 )
    {
      str += strlen(AT_BLE_CMD_STRING[client_current_cmd]);

      if( strncmp(str, AT_READ, strlen(AT_READ)) == 0)
      {
        str++;
        /* extract params regarding current command */
        switch (client_current_cmd) {
          case BLE_SVC:
          {
            stm32wb_at_BLE_SVC_t param;
            char * token;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.index = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            if((param.index >= SRV_CUSTOM_APP_SVC_OFFSET) && 
               (param.index < (SRV_CUSTOM_APP_SVC_OFFSET + SRV_CUSTOM_APP_SVC_COUNT)) )
            {
              token = strtok(NULL, AT_SEPARATOR);
              if(token != 0)
              {
                param.svc_uuid_type = (uint8_t)strtol(token, NULL, 0);
              }
              else
              {
                status |= 1;
              }
              
              token = strtok(NULL, AT_SEPARATOR);
              if(token != 0)
              {
                param.svc_uuid = (uint16_t)strtol(token, NULL, 0);
              }
              else
              {
                status |= 1;
              }
              
              token = strtok(NULL, AT_SEPARATOR);
              if(token != 0)
              {
                param.max_attr_record = (uint8_t)strtol(token, NULL, 0);
              }
              else
              {
                status |= 1;
              }              
            }
            
            if( param.index >= (SRV_CUSTOM_APP_SVC_OFFSET + SRV_CUSTOM_APP_SVC_COUNT) )
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_SVC_cb(&param);
            }            
            break;            
          }
          case BLE_INIT:
          {
            stm32wb_at_BLE_INIT_t param;
            char * token;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.init_set = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_INIT_cb(&param);
            }            
            break;
          }
          case BLE_ADV:
          {
            stm32wb_at_BLE_ADV_t param;
            char * token;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.enable = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_ADV_cb(&param);
            }
            break;
          }
          case BLE_ADV_PARAM:
          {
            stm32wb_at_BLE_ADV_PARAM_t param;
            char * token;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.adv_type = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              param.adv_int_min = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              param.adv_int_max = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }            

//            token = strtok(NULL, AT_SEPARATOR);
//            if(token != 0)
//            {
//              param.own_addr_type = (uint8_t)strtol(token, NULL, 0);
//            }
//            else
//            {
//              status |= 1;
//            }

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_ADV_PARAM_cb(&param);
            }
            break;
          }
          case BLE_CONN_INT:
          {
            stm32wb_at_BLE_CONN_INT_t param;
            char * token;
            float value;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
               value = strtof(token, NULL);
               if ((value >= 7.5f) && (value <= 4000.0f))
               {
                 param.min = value;
               }
               else
               {
                 status |= 1;
               }
            }
            else
            {
              status |= 1;
            }

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              value = strtof(token, NULL);
              if ((value >= 7.5f) && (value <= 4000.0f) && (value >= param.min))
              {
                param.max = value;
              }
              else
              {
                status |= 1;
              }
            }
            else
            {
              status |= 1;
            }

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_CONN_INT_cb(&param);
            }
            break;
          }
          case BLE_CHAR_ADD:
          {
            stm32wb_at_BLE_CHAR_ADD_t param;
            char * token;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.svc_id = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.char_id = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.char_uuid_type = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.char_uuid = (uint16_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.char_value_len = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.char_properties = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.sec_permission = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.gatt_evt_mask = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }            

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_CHAR_ADD_cb(&param);
            }
            break;
          }
          case BLE_PERIPH_SEC_REQ:
          {
            stm32wb_at_BLE_PERIPH_SEC_REQ_t param;
            char * token;
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.enable = (uint32_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            if(status == 0)
            {
              status = stm32wb_at_BLE_PERIPH_SEC_REQ_cb(&param);
            }
            break;
          }
          case BLE_PAIRING_CONFIRM:
          {
            stm32wb_at_BLE_PAIRING_CONFIRM_t param;
            char * token;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.confirm_yes_no = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }        

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_PAIRING_CONFIRM_cb(&param);
            }
            break;
          }
          case BLE_PASSKEY_RESP:
          {
            stm32wb_at_BLE_PASSKEY_RESP_t param;
            char * token;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.pass_key_resp = (uint32_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            if(status == 0)
            {
              status = stm32wb_at_BLE_PASSKEY_RESP_cb(&param);
            }
            break;
          }
          case BLE_IO_CAPA:
          {
            stm32wb_at_BLE_IO_CAPA_t  param;
            char * token;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.io_capability = (uint8_t)strtol(token, NULL, 0); 
            }
            else
            {
              status |= 1;
            }                                       
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            if(status == 0)
            {
              status = stm32wb_at_BLE_IO_CAPA_cb(&param);
            } 
            break;
          }
          case BLE_SET_AUTHEN_REQ:
          {
            stm32wb_at_BLE_SET_AUTHEN_REQ_t  param;
            char * token;
                    
            /* all the fields are mandatory using this method */                    
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.bonding_mode = (uint8_t)strtol(str, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              param.mitm = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              param.sc_support = (uint16_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              param.use_fixed_pin = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              param.fixed_pin = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              param.identity_address_type = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            if(status == 0)
            {
              status = stm32wb_at_BLE_SET_AUTHEN_REQ_cb(&param);
            }
          }
          break;
          case BLE_CLEAR_DB:
          {
            stm32wb_at_BLE_CLEAR_DB_t param;
            char * token;
            status = 0;
                    
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.dummy = (uint32_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }
            if(status == 0)
            {
              status = stm32wb_at_BLE_CLEAR_DB_cb(&param);
            }
            break;
          }
          case BLE_RST:
          {
            stm32wb_at_BLE_RST_t param;
            char * token;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.reset = (uint8_t)strtol(token, NULL, 0); 
            }
            else
            {
              status |= 1;
            }                                       
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            if(status == 0)
            {
              status = stm32wb_at_BLE_RST_cb(&param);
            } 
            break;
          }
          case BLE_PUB_ADDR:
          {
            stm32wb_at_BLE_PUB_ADDR_t param;
            char * token;
            uint8_t tab_len;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              tab_len = sizeof(param.BD_address);
              status |= str_to_byte_tab(token, &param.BD_address[0], &tab_len);
              if(tab_len != 6)
              {
                status |= 1;
              }
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            if(status == 0)
            {
              status = stm32wb_at_BLE_PUB_ADDR_cb(&param);
            }
            break;
          }
          case BLE_RAND_ADDR:
          {
            stm32wb_at_BLE_RAND_ADDR_t param;
            char * token;
            uint8_t tab_len;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              tab_len = sizeof(param.BD_address);
              status |= str_to_byte_tab(token, &param.BD_address[0], &tab_len);
              if(tab_len != 6)
              {
                status |= 1;
              }                    
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            if(status == 0)
            {
              status = stm32wb_at_BLE_RAND_ADDR_cb(&param);
            }
            break;
          }
          case BLE_RF_POWER:
          {
            stm32wb_at_BLE_RF_POWER_t param;
            char * token;
            float value;
            uint8_t value_flag = 0;
            status = 0;
            uint8_t i;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              value = strtof(token, NULL);
              for (i = 0 ; i < sizeof(tab_conv_tx_power) ; i++)
              {
                if (value == tab_conv_tx_power[i])
                {
                  value_flag = 1;
                  param.tx_power = value;
                  param.tx_power_code = i;
                  break;
                }
              }
              if (value_flag != 1)
              {
                status |= 1;
              }
            }
            else
            {
              status |= 1;
            }
            
            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }
            
            if(status == 0)
            {
              status = stm32wb_at_BLE_RF_POWER_cb(&param);
            }
            break;
          }
          case BLE_NAME:
          {
            stm32wb_at_BLE_NAME_t param;
            status = 0;
            
            strcpy(&param.name[0], str);
            
            /* do not check separator as it can be in the name itself */
            if(status == 0)
            {
              status = stm32wb_at_BLE_NAME_cb(&param);
            }
            break;
          }
          case BLE_NOTIF_VAL:
          {
            stm32wb_at_BLE_NOTIF_VAL_t param;
            
            break;
          }
          case BLE_INDIC_VAL:
          {
              stm32wb_at_BLE_INDIC_VAL_t param;
              
              break;
          }
          default:
            break;
        }
      }
    }
  }
  else if( strncmp(str, AT_EVENT, strlen(AT_EVENT)) == 0)
  {
    str++;

    for( current_evt = (stm32wb_at_BLE_EVT_t)0 ; current_evt < BLE_EVT_NONE ; current_evt++ )
    {
      if( strncmp(str, AT_BLE_EVT_STRING[current_evt], strlen(AT_BLE_EVT_STRING[current_evt])) == 0 )
      {
        str += strlen(AT_BLE_EVT_STRING[current_evt]);
        break;
      }
    }
    if( current_evt == BLE_EVT_NONE)
    {
      status = 1;
    }
    else
    {
      if (strncmp(str, AT_SET, strlen(AT_SET)) == 0)
      {
        str++;
        /* extract params regarding current command */
        switch (current_evt) {
          case BLE_EVT_WRITE:
          {
            stm32wb_at_BLE_EVT_WRITE_t param;
            char * token;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.svc_index = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.char_index = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.val_tab_len = sizeof(param.val_tab);
              status |= str_to_byte_tab(token, &param.val_tab[0], &param.val_tab_len);
            }
            else
            {
              status |= 1;
            }

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_EVT_WRITE_cb(&param);
            }
            break;
          }
          case BLE_EVT_CONN:
          {
            stm32wb_at_BLE_EVT_CONN_t param;
            char * token;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.status = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_EVT_CONN_cb(&param);
            }
            break;
          }
          case BLE_EVT_UPD_CONN:
          {
            stm32wb_at_BLE_EVT_UPD_CONN_t param;
            char * token;
            status = 0;

            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.status = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_EVT_UPD_CONN_cb(&param);
            }
            break;
          }
          case BLE_EVT_VALUE_CONFIRM:
          {
            stm32wb_at_BLE_EVT_VALUE_CONFIRM_t param;
            char * token;
            status = 0;
            
            token = strtok(str, AT_SEPARATOR);
            if(token != 0)
            {
              param.numeric_value = (uint8_t)strtol(token, NULL, 0);
            }
            else
            {
              status |= 1;
            }

            token = strtok(NULL, AT_SEPARATOR);
            if(token != 0)
            {
              status |= 1;
            }

            if(status == 0)
            {
              status = stm32wb_at_BLE_EVT_VALUE_CONFIRM_cb(&param);
            }
            break;
          }
          default:
            break;
        }
      }
      else
      {
        status = 1;
      }
    }
  }

  return status;
}

/**
 * @brief Send an AT set with specified command and parameters
 * @param cmd command code
 * @param param pointer to parameters of command
 * @retval 0 in case of success, an error code otherwise
 */
uint8_t stm32wb_at_client_Set(stm32wb_at_BLE_CMD_t cmd, void *param)
{
  uint8_t status;

  (void)stm32wb_at_client_Wait_ready();

  if(cmd < BLE_NONE)
  {
    client_current_cmd = cmd;

    (void)strcpy(client_buff_tx, AT_PRE_CMD);
    (void)strcat(client_buff_tx, AT_BLE_CMD_STRING[cmd]);
    (void)strcat(client_buff_tx, AT_SET);

    switch (cmd) {
      case BLE_SVC:
      {
        stm32wb_at_BLE_SVC_t *p = param;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%d", p->index);
        (void)strcat(client_buff_tx, str_tmp);
        if(p->index >= SRV_CUSTOM_APP_SVC_OFFSET)
        {
          (void)strcat(client_buff_tx, AT_SEPARATOR);
          
          (void)sprintf(str_tmp, "%d", p->svc_uuid_type);
          (void)strcat(client_buff_tx, str_tmp);
          (void)strcat(client_buff_tx, AT_SEPARATOR);
          
          (void)sprintf(str_tmp, "0x%04x", p->svc_uuid);
          (void)strcat(client_buff_tx, str_tmp);
          (void)strcat(client_buff_tx, AT_SEPARATOR);
          
          (void)sprintf(str_tmp, "%d", p->max_attr_record);
          (void)strcat(client_buff_tx, str_tmp);
        }
        break;
      }
      case BLE_INIT:
      {
        stm32wb_at_BLE_INIT_t *p = param;
        char str_tmp[8];
        
        (void)sprintf(str_tmp, "%d", p->init_set);
        (void)strcat(client_buff_tx, str_tmp);

        break;
      }
      case BLE_RF_POWER:
      {
        stm32wb_at_BLE_RF_POWER_t *p = param;
        char str_tmp[8];
        
        (void)sprintf(str_tmp, "%.2f", p->tx_power);
        (void)strcat(client_buff_tx, str_tmp);

        break;
      }
      case BLE_IO_CAPA:
      {
          stm32wb_at_BLE_IO_CAPA_t  param;
          
          break;
      }
      case BLE_SET_AUTHEN_REQ:
      {
          stm32wb_at_BLE_SET_AUTHEN_REQ_t  param;
          
          break;
      }
      case BLE_PERIPH_SEC_REQ:
      {
          stm32wb_at_BLE_PERIPH_SEC_REQ_t param;
          
          break;
      }
      case BLE_PAIRING_CONFIRM:
      {
          stm32wb_at_BLE_PAIRING_CONFIRM_t param;
          
          break;
      }
      case BLE_PASSKEY_RESP:
      {
        stm32wb_at_BLE_PASSKEY_RESP_t *p = param;
        char str_tmp[8];
        
        (void)sprintf(str_tmp, "%d", p->pass_key_resp);
        (void)strcat(client_buff_tx, str_tmp);

        break;
      }
      case BLE_CLEAR_DB:
      {
          stm32wb_at_BLE_CLEAR_DB_t param;
          
          break;
      }
      case BLE_NOTIF_VAL:
      {
        stm32wb_at_BLE_NOTIF_VAL_t *p = param;
        uint8_t i;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%d", p->svc_index);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        (void)sprintf(str_tmp, "%d", p->char_index);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);
        
        
        if(p->val_tab_len <= 1)
        {
          sprintf(str_tmp, "%d", (uint16_t)p->val_tab[0]);
          strcat(client_buff_tx, str_tmp);
        }
        else
        {
          (void)strcat(client_buff_tx, "0x");
          for(i = 0 ; i < p->val_tab_len ; i++)
          {
            (void)sprintf( str_tmp, "%02x", p->val_tab[i] );
            (void)strcat(client_buff_tx, str_tmp);
          }
        }

        break;
      }
      case BLE_INDIC_VAL:
      {
        stm32wb_at_BLE_INDIC_VAL_t *p = param;
        uint8_t i;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%d", p->svc_index);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        (void)sprintf(str_tmp, "%d", p->char_index);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        if(p->val_tab_len <= 1)
        {
          sprintf(str_tmp, "%d", (uint16_t)p->val_tab[0]);
          strcat(client_buff_tx, str_tmp);
        }
        else
        {
          (void)strcat(client_buff_tx, "0x");
          for(i = 0 ; i < p->val_tab_len ; i++)
          {
            (void)sprintf( str_tmp, "%02x", p->val_tab[i] );
            (void)strcat(client_buff_tx, str_tmp);
          }
        }
          
        break;
      }
      case BLE_ADV:
      {
        stm32wb_at_BLE_ADV_t *p = param;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%d", p->enable);
        (void)strcat(client_buff_tx, str_tmp);

        break;
      }
      case BLE_ADV_PARAM:
      {
        stm32wb_at_BLE_ADV_PARAM_t *p = param;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%d", p->entry_number);
        (void)strcat(client_buff_tx, str_tmp);
        if(p->entry_number == 0)
        {
          (void)strcat(client_buff_tx, AT_SEPARATOR);

          (void)sprintf(str_tmp, "%d", p->adv_type);
          (void)strcat(client_buff_tx, str_tmp);
          (void)strcat(client_buff_tx, AT_SEPARATOR);
          
          (void)sprintf(str_tmp, "%.2f", p->adv_int_min);
          (void)strcat(client_buff_tx, str_tmp);
          (void)strcat(client_buff_tx, AT_SEPARATOR);

          (void)sprintf(str_tmp, "%.2f", p->adv_int_max);
          (void)strcat(client_buff_tx, str_tmp);
          (void)strcat(client_buff_tx, AT_SEPARATOR);

//          (void)sprintf(str_tmp, "%d", p->own_addr_type);
//          (void)strcat(client_buff_tx, str_tmp);
//          (void)strcat(client_buff_tx, AT_SEPARATOR);        
        }
        break;
      }
      case BLE_CONN_INT:
      {
        stm32wb_at_BLE_CONN_INT_t *p = param;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%.1f", p->min);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        (void)sprintf(str_tmp, "%.1f", p->max);
        (void)strcat(client_buff_tx, str_tmp);

        break;
      }
      case BLE_CHAR_ADD:
      {
        stm32wb_at_BLE_CHAR_ADD_t *p = param;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%d", p->svc_id);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);
        
        (void)sprintf(str_tmp, "%d", p->char_id);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        (void)sprintf(str_tmp, "%d", p->char_uuid_type);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);
        
        (void)sprintf(str_tmp, "0x%04x", p->char_uuid);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        (void)sprintf(str_tmp, "%d", p->char_value_len);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        (void)sprintf(str_tmp, "0x%02x", p->char_properties);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);
        
        (void)sprintf(str_tmp, "0x%02x", p->sec_permission);
        (void)strcat(client_buff_tx, str_tmp);
        (void)strcat(client_buff_tx, AT_SEPARATOR);

        (void)sprintf(str_tmp, "0x%02x", p->gatt_evt_mask);
        (void)strcat(client_buff_tx, str_tmp);

        break;
      }
      case BLE_RST:
      {
        stm32wb_at_BLE_RST_t *p = param;
        char str_tmp[8];

        (void)sprintf(str_tmp, "%d", p->reset);
        (void)strcat(client_buff_tx, str_tmp);

        break;
      }
      case BLE_PUB_ADDR:
      {
        stm32wb_at_BLE_PUB_ADDR_t *p = param;
        char str_tmp[20];
        uint8_t i;
        
        (void)strcat(client_buff_tx, "0x");
        for(i = 0 ; i < sizeof(p->BD_address) ; i++)
        {
          (void)sprintf(str_tmp, "%02x", p->BD_address[i]);
          (void)strcat(client_buff_tx, str_tmp);
        }

        break;
      }
      case BLE_RAND_ADDR:
      {
        stm32wb_at_BLE_RAND_ADDR_t *p = param;
        char str_tmp[20];
        uint8_t i;
        
        (void)strcat(client_buff_tx, "0x");
        for(i = 0 ; i < sizeof(p->BD_address) ; i++)
        {
          (void)sprintf(str_tmp, "%02x", p->BD_address[i]);
          (void)strcat(client_buff_tx, str_tmp);
        }

        break;
      }
      case BLE_NAME:
      {
        stm32wb_at_BLE_NAME_t *p = param;

        (void)strcat(client_buff_tx, p->name);

        break;
      }
      default:
        break;
    }

    (void)strcat(client_buff_tx, AT_EOL);

    client_current_cmd = cmd;
    status = stm32wb_at_ll_Transmit((uint8_t *)&client_buff_tx[0], (uint16_t)strlen(client_buff_tx));
    (void)memset(&client_buff_tx[0], 0, sizeof(client_buff_tx));
  }
  else
  {
    status = 1;
  }

  return status;
}

/**
 * @brief Wait for the last AT command completed
 * @param None
 * @retval 0 in case of success, an error code otherwise
 */
static uint8_t stm32wb_at_client_Wait_ready(void)
{
  while (client_current_cmd !=  BLE_NONE)
  {
  }

  return 0;
}
