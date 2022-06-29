/**
  ******************************************************************************
  * @file    ble_at_server.c
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


/* Includes ------------------------------------------------------------------*/
#include "stm32wb_at_ble.h"
#include "stm32wb_at_ll.h"
#include "ble_at_server.h"
#include "custom_app.h"
    
uint8_t str_to_byte_tab(char *str_in, uint8_t *tab, uint8_t *tab_len);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char AT_server_buffer_tx[64];
char AT_server_buffer_reply[64];
char AT_server_buffer_event[64];
stm32wb_at_BLE_CMD_t AT_server_current_cmd;
uint8_t at_buffer[64];

extern float tab_conv_tx_power[32];

/* Private function prototypes -----------------------------------------------*/
static uint8_t ble_at_server_Reply_status(stm32wb_at_BLE_CMD_t cmd, uint8_t status);
static uint8_t ble_at_server_Reply_get(stm32wb_at_BLE_CMD_t cmd, char *data_str);

FOREACH_AT_BLE_CMD(GENERATE_AT_BLE_SERVER_CB_FUNC)

uint8_t ble_at_server_Init(void)
{
  memset(&AT_server_buffer_tx[0], '\0', sizeof(AT_server_buffer_tx));
  memset(&AT_server_buffer_reply[0], '\0', sizeof(AT_server_buffer_reply));
  memset(&AT_server_buffer_event[0], '\0', sizeof(AT_server_buffer_event));
  AT_server_current_cmd =  BLE_NONE;

  return 0;
}

uint8_t ble_at_server_Process_rx_frame(char * str)
{
  uint8_t status, op, get_status;
  stm32wb_at_BLE_CMD_t i;
  uint8_t j;

  status = 1;
  get_status = 1;

  if( strncmp(str, AT_CMD_TEST, strlen(AT_CMD_TEST)) == 0 )
  {
    if(strlen(str) == strlen(AT_CMD_TEST))
    {
      AT_server_current_cmd = BLE_TEST;
      status = 0;
    }
    else
    {
      if( strncmp(str, AT_PRE_CMD, strlen(AT_PRE_CMD)) == 0 )
      {
        str += strlen(AT_PRE_CMD);

        if(AT_server_current_cmd != BLE_NONE)
        {
          status = 1;
        }
        else
        {
          for( i = (stm32wb_at_BLE_CMD_t)0 ; i < BLE_NONE ; i++ )
          {
            if( strncmp(str, AT_BLE_CMD_STRING[i], strlen(AT_BLE_CMD_STRING[i])) == 0 )
            {
              AT_server_current_cmd = i;
              str += strlen(AT_BLE_CMD_STRING[AT_server_current_cmd]);
              break;
            }
          }

          if(AT_server_current_cmd == BLE_NONE)
          {
            status = 1;
          }
          else
          {
            memset(&AT_server_buffer_reply[0], '\0', sizeof(AT_server_buffer_reply));

            if (strncmp(str, AT_GET, strlen(AT_GET)) == 0)
            {
              memcpy(&op, AT_GET, 1);
              str++;
            }
            else if( strncmp(str, AT_SET, strlen(AT_SET)) == 0 )
            {
              memcpy(&op, AT_SET, 1);
              str++;
            }
            else
            {
              op = 0;
              status = 1;
            }

            if(op != 0)
            {
              switch (AT_server_current_cmd) {
                case BLE_SVC:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_SVC_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_SVC_t param;
                    char * token;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.index = (uint8_t)strtol(str, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    if((param.index >= CUSTOM_APP_SVC_OFFSET) && 
                       (param.index < (CUSTOM_APP_SVC_OFFSET + CUSTOM_APP_SVC_COUNT)) )
                    {
                      token = strtok(NULL, AT_SEPARATOR);
                      if(token != 0)
                      {
                        errno = 0;
                        param.svc_uuid_type = (uint8_t)strtol(token, NULL, 0);
                      }
                      else
                      {
                        status |= 1;
                      }
                      
                      token = strtok(NULL, AT_SEPARATOR);
                      if(token != 0)
                      {
                        errno = 0;
                        param.svc_uuid = (uint16_t)strtol(token, NULL, 0);
                      }
                      else
                      {
                        status |= 1;
                      }
                      
                      token = strtok(NULL, AT_SEPARATOR);
                      if(token != 0)
                      {
                        errno = 0;
                        param.max_attr_record = (uint8_t)strtol(token, NULL, 0);
                      }
                      else
                      {
                        status |= 1;
                      }
                    }
                    
                    if( param.index >= (CUSTOM_APP_SVC_OFFSET + CUSTOM_APP_SVC_COUNT) )
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
                  }
                  break;
                }
                case BLE_RST:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = 1;
                    status = 1;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_RST_t param;
                    char * token;
                    status = 0;
                    
                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
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
                  }
                  break;
                }
                case BLE_IO_CAPA:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_IO_CAPA_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_IO_CAPA_t param;
                    char * token;
                    status = 0;
                    
                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
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
                  }
                  break;
                }
                case BLE_SET_AUTHEN_REQ:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_SET_AUTHEN_REQ_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_SET_AUTHEN_REQ_t param;
                    char * token;
                    
                    /* all the fields are mandatory using this method */                    
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.bonding_mode = (uint8_t)strtol(str, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.mitm = (uint8_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.sc_support = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.use_fixed_pin = (uint8_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.fixed_pin = (uint32_t)strtol(token, NULL, 0);
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
                }
                case BLE_PERIPH_SEC_REQ:
                {
                if(op == AT_GET[0])
                  {
                    get_status = 1;
                    status = 1;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_PERIPH_SEC_REQ_t param;
                    char * token;
                    status = 0;
                    
                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
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
                  }
                  break;
                }
                case BLE_PAIRING_CONFIRM:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = 1;
                    status = 1;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_PAIRING_CONFIRM_t param;
                    char * token;
                    status = 0;
                    
                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.confirm_yes_no = (uint32_t)strtol(token, NULL, 0);
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
                  }
                  break;
                }
                case BLE_PASSKEY_RESP:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_PASSKEY_RESP_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_PASSKEY_RESP_t param;
                    char * token;
                    status = 0;
                    
                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
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
                  }
                  break;
                }
                case BLE_CLEAR_DB:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = 1;
                    status = 1;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_CLEAR_DB_t param;
                    char * token;
                    status = 0;
                    
                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
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
                  }
                  break;
                }
                case BLE_PUB_ADDR:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_PUB_ADDR_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
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
                  }
                  break;
                }
                case BLE_RAND_ADDR:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_RAND_ADDR_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
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
                  }
                  break;
                }
                case BLE_CFG_RAND_ADDR:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_CFG_RAND_ADDR_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_CFG_RAND_ADDR_t param;
                    char * token;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.cfg_rand_addr = (uint8_t)strtol(token, NULL, 0);
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
                      status = stm32wb_at_BLE_CFG_RAND_ADDR_cb(&param);
                    }
                  }
                  break;
                }
                case BLE_NAME:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_NAME_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_NAME_t param;
                    status = 0;
                    
                    if( (strlen(str) + 1) <= sizeof(param.name) )
                    {                      
                      strcpy(&param.name[0], str);
                    }
                    else
                    {
                      status = 1;
                    }
                    /* do not check separator as it can be in the name itself */
                    if(status == 0)
                    {
                      status = stm32wb_at_BLE_NAME_cb(&param);
                    }
                  }
                  break;
                }
                case BLE_NOTIF_VAL:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = 1;
                    status = 1;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_NOTIF_VAL_t param;
                    char * token;
                    uint16_t data;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.svc_index = (uint8_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }

                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.char_index = (uint8_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      if( strncmp(token, "0x", 2) == 0 )
                      {
                        param.val_tab_len = sizeof(param.val_tab);
                        status |= str_to_byte_tab(token, &param.val_tab[0], &param.val_tab_len);                        
                      }
                      else
                      {
                        errno = 0;
                        data = (uint16_t)strtol(token, NULL, 0);
                        param.val_tab_len = sizeof(uint16_t);
                        memcpy(&param.val_tab[0], &data, sizeof(uint16_t));
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
                      status = stm32wb_at_BLE_NOTIF_VAL_cb(&param);
                    }
                  }
                  break;
                }
                case BLE_INDIC_VAL:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = 1;
                    status = 1;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_INDIC_VAL_t param;
                    char * token;
                    uint16_t data;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.svc_index = (uint8_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }

                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.char_index = (uint8_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      if( strncmp(token, "0x", 2) == 0 )
                      {
                        param.val_tab_len = sizeof(param.val_tab);
                        status |= str_to_byte_tab(token, &param.val_tab[0], &param.val_tab_len);                        
                      }
                      else
                      {
                        errno = 0;
                        data = (uint16_t)strtol(token, NULL, 0);
                        param.val_tab_len = sizeof(uint16_t);
                        memcpy(&param.val_tab[0], &data, sizeof(uint16_t));
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
                      status = stm32wb_at_BLE_INDIC_VAL_cb(&param);
                    }
                  }
                  break;
                }
                case BLE_CONN_INT:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_CONN_INT_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
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

                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      value = strtof(token, NULL);
                      if ((value >= 7.5f) && (value <= 4000.f) && (value >= param.min))
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
                  }
                  break;
                }
                case BLE_ADV_PARAM:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_ADV_PARAM_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_ADV_PARAM_t param;
                    char * token;
                    float value;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.entry_number = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }

                    if(param.entry_number == 0)
                    {
                      token = strtok(NULL, AT_SEPARATOR);
                      if(token != 0)
                      {
                        errno = 0;
                        param.adv_type = (uint16_t)strtol(token, NULL, 0);
                      }
                      else
                      {
                        status |= 1;
                      }
                      
                      token = strtok(NULL, AT_SEPARATOR);
                      if(token != 0)
                      {
                        value = strtof(token, NULL);
                        if ((value >= 20) && (value <= 10240))
                        {
                          param.adv_int_min = value;
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
                        value = strtof(token, NULL);
                        if ((value >= 20) && (value <= 10240))
                        {
                          param.adv_int_max = value;
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
                    }
                    else if(param.entry_number == 1)
                    {
                      param.adv_type = 0;
                      param.adv_int_min = CFG_MINI_CONN_ADV_INTERVAL_MIN; /* 20ms */
                      param.adv_int_max = CFG_MINI_CONN_ADV_INTERVAL_MAX; /* 30ms */
                    }
                    else if(param.entry_number == 2)
                    {
                      param.adv_type = 0;
                      param.adv_int_min = CFG_INTER_CONN_ADV_INTERVAL_MIN; /* 180 ms */
                      param.adv_int_max = CFG_INTER_CONN_ADV_INTERVAL_MAX; /* 200 ms */
                    }
                    else if(param.entry_number == 3)
                    {
                      param.adv_type = 0;
                      param.adv_int_min = CFG_LP_CONN_ADV_INTERVAL_MIN; /* 1000 ms */
                      param.adv_int_max = CFG_LP_CONN_ADV_INTERVAL_MAX; /* 2500 ms */
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
                      status = stm32wb_at_BLE_ADV_PARAM_cb(&param);
                    }
                  }
                  break;
                }
                case BLE_ADV:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_ADV_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_ADV_t param;
                    
                    char * token;
                    status = 0;
                    
                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
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
                  }
                  break;
                }
                case BLE_CHAR_ADD:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = 1;
                    status = 1;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_CHAR_ADD_t param;
                    char * token;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.svc_id = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }

                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.char_id = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.char_uuid_type = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.char_uuid = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.char_value_len = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.char_properties = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.sec_permission = (uint16_t)strtol(token, NULL, 0);
                    }
                    else
                    {
                      status |= 1;
                    }
                    
                    token = strtok(NULL, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
                      param.gatt_evt_mask = (uint16_t)strtol(token, NULL, 0);
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
                  }
                  break;
                }
                case BLE_RF_POWER:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_RF_POWER_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_RF_POWER_t param;
                    char * token;
                    float value;
                    uint8_t value_flag = 0;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      value = strtof(token, NULL);
                      for (j = 0 ; j < sizeof(tab_conv_tx_power) ; j++)
                      {
                        if (value == tab_conv_tx_power[j])
                        {
                          value_flag = 1;
                          param.tx_power = value;
                          param.tx_power_code = j;
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
                  }
                  break;
                }
                case BLE_INIT:
                {
                  if(op == AT_GET[0])
                  {
                    get_status = stm32wb_at_BLE_INIT_server_cb(&AT_server_buffer_reply[0]);
                    status = 0;
                  }
                  else if(op == AT_SET[0])
                  {
                    stm32wb_at_BLE_INIT_t param;
                    char * token;
                    status = 0;

                    token = strtok(str, AT_SEPARATOR);
                    if(token != 0)
                    {
                      errno = 0;
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
                  }
                  break;
                }
                default:
                  
                  break;
              }
            }
            if((op == AT_GET[0]) && (get_status == 0))
            {
              ble_at_server_Reply_get(AT_server_current_cmd, &AT_server_buffer_reply[0]);
              memset(&AT_server_buffer_reply[0], '\0', sizeof(AT_server_buffer_reply));
            }
          }
        }
      }
    }
  }
  
  ble_at_server_Reply_status(AT_server_current_cmd, status);
  AT_server_current_cmd = BLE_NONE;

  return status;
}

uint8_t ble_at_server_Send_evt(stm32wb_at_BLE_EVT_t evt, void *param)
{
  uint8_t status;

  status = 1;
  while (AT_server_current_cmd !=  BLE_NONE)
  {
    /* a command is not yet completed */
  }

  if(evt < BLE_EVT_NONE)
  {

    strcpy(AT_server_buffer_event, AT_EVENT);
    strcat(AT_server_buffer_event, AT_BLE_EVT_STRING[evt]);
    strcat(AT_server_buffer_event, AT_SET);

    switch (evt) {
      case BLE_EVT_WRITE:
      {
        stm32wb_at_BLE_EVT_WRITE_t *p = param;
        char str_tmp[8];
        uint8_t i;

        sprintf(str_tmp, "%d", p->svc_index);
        strcat(AT_server_buffer_event, str_tmp);
        strcat(AT_server_buffer_event, AT_SEPARATOR);

        sprintf(str_tmp, "%d", p->char_index);
        strcat(AT_server_buffer_event, str_tmp);
        strcat(AT_server_buffer_event, AT_SEPARATOR);

        if(p->val_tab_len <= 1)
        {
          sprintf(str_tmp, "%d", (uint16_t)p->val_tab[0]);
          strcat(AT_server_buffer_event, str_tmp);
        }
        else
        {
          sprintf(str_tmp, "0x");
          strcat(AT_server_buffer_event, str_tmp);
          for (i = 0 ; i < p->val_tab_len ; i++)
          {
            sprintf(str_tmp, "%02x", p->val_tab[i]);
            strcat(AT_server_buffer_event, str_tmp);
          }
        }
        break;
      }
      case BLE_EVT_CONN:
      {
        stm32wb_at_BLE_EVT_CONN_t *p = param;
        char str_tmp[8];

        sprintf(str_tmp, "%d", p->status);
        strcat(AT_server_buffer_event, str_tmp);

        break;
      }
      case BLE_EVT_UPD_CONN:
      {
        stm32wb_at_BLE_EVT_UPD_CONN_t *p = param;
        char str_tmp[8];

        sprintf(str_tmp, "%d", p->status);
        strcat(AT_server_buffer_event, str_tmp);

        break;
      }
      case BLE_EVT_VALUE_CONFIRM:
      {
        stm32wb_at_BLE_EVT_VALUE_CONFIRM_t *p = param;
        char str_tmp[8];

        sprintf(str_tmp, "%ld", p->numeric_value);
        strcat(AT_server_buffer_event, str_tmp);

        break;
      }
      case BLE_EVT_PASSKEY_REQ:
      {
        stm32wb_at_BLE_EVT_PASSKEY_REQ_t *p = param;
        char str_tmp[8];

        sprintf(str_tmp, "%d", p->dummy);
        strcat(AT_server_buffer_event, str_tmp);

        break;
      }
      case BLE_EVT_PAIRING:
      {
        stm32wb_at_BLE_EVT_PAIRING_t *p = param;
        char str_tmp[8];
        
        sprintf(str_tmp, "%d", p->status);
        strcat(AT_server_buffer_event, str_tmp);
        strcat(AT_server_buffer_event, AT_SEPARATOR);

        sprintf(str_tmp, "%d", p->reason);
        strcat(AT_server_buffer_event, str_tmp);

        break;
      }
      default:
        break;
    }
    strcat(AT_server_buffer_event, AT_EOL);

    status = stm32wb_at_ll_Transmit((uint8_t *)&AT_server_buffer_event[0], strlen(AT_server_buffer_event));
    memset(&AT_server_buffer_event[0], '\0', sizeof(AT_server_buffer_event));
  }

  return status;
}

/* Private function definition -----------------------------------------------*/

static uint8_t ble_at_server_Reply_status(stm32wb_at_BLE_CMD_t cmd, uint8_t stat)
{
  uint8_t status = 0;

  if (cmd ==  BLE_TEST)
  {
    strcat(AT_server_buffer_tx, AT_CMD_STATUS_OK);
    strcat(AT_server_buffer_tx, AT_EOL);
  }
  else if(cmd < BLE_NONE)
  {
    if(stat == 0)
    {
      strcat(AT_server_buffer_tx, AT_CMD_STATUS_OK);
    }
    else
    {
      strcat(AT_server_buffer_tx, AT_CMD_STATUS_ERROR);
    }
    strcat(AT_server_buffer_tx, AT_EOL);
  }
  else
  {
    strcat(AT_server_buffer_tx, AT_CMD_STATUS_ERROR);
    strcat(AT_server_buffer_tx, AT_EOL);
  }

  if(status == 0)
  {
    status = stm32wb_at_ll_Transmit((uint8_t *)&AT_server_buffer_tx[0], strlen(AT_server_buffer_tx));
    memset(&AT_server_buffer_tx[0], '\0', sizeof(AT_server_buffer_tx));
  }

  return status;
}


static uint8_t ble_at_server_Reply_get(stm32wb_at_BLE_CMD_t cmd, char *data_str)
{
  uint8_t status = 0;

  if (cmd ==  BLE_TEST)
  {
    status = 1;
  }
  else if(cmd < BLE_NONE)
  {
    status = 0;
    strcat(AT_server_buffer_tx, AT_REPLY);
    strcat(AT_server_buffer_tx, AT_BLE_CMD_STRING[cmd]);
    strcat(AT_server_buffer_tx, AT_READ);
    strcat(AT_server_buffer_tx, data_str);
    strcat(AT_server_buffer_tx, AT_EOL);
  }
  else
  {
    status = 1;
  }

  if(status == 0)
  {
    status = stm32wb_at_ll_Transmit((uint8_t *)&AT_server_buffer_tx[0], strlen(AT_server_buffer_tx));
    memset(&AT_server_buffer_tx[0], '\0', sizeof(AT_server_buffer_tx));
  }

  return status;
}
