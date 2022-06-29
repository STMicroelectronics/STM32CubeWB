/**
  ******************************************************************************
  * @file    stm32wb_at_ble.h
  * @author  MCD Application Team
  * @brief   Header file of AT BLE commands.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32WB_AT_BLE_H
#define STM32WB_AT_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wb_at.h"

/* Private includes ----------------------------------------------------------*/
#define FOREACH_AT_BLE_CMD(AT_BLE_CMD)\
        AT_BLE_CMD(BLE_TEST)\
        AT_BLE_CMD(BLE_SVC)\
        AT_BLE_CMD(BLE_INIT)\
        AT_BLE_CMD(BLE_ADV_PARAM)\
        AT_BLE_CMD(BLE_ADV)\
        AT_BLE_CMD(BLE_CHAR_ADD)\
        AT_BLE_CMD(BLE_PERIPH_SEC_REQ)\
        AT_BLE_CMD(BLE_PAIRING_CONFIRM)\
        AT_BLE_CMD(BLE_PASSKEY_RESP)\
        AT_BLE_CMD(BLE_IO_CAPA)\
        AT_BLE_CMD(BLE_SET_AUTHEN_REQ)\
        AT_BLE_CMD(BLE_CLEAR_DB)\
        AT_BLE_CMD(BLE_RST)\
        AT_BLE_CMD(BLE_CFG_RAND_ADDR)\
        AT_BLE_CMD(BLE_PUB_ADDR)\
        AT_BLE_CMD(BLE_RAND_ADDR)\
        AT_BLE_CMD(BLE_NAME)\
        AT_BLE_CMD(BLE_RF_POWER)\
        AT_BLE_CMD(BLE_NOTIF_VAL)\
        AT_BLE_CMD(BLE_INDIC_VAL)\
        AT_BLE_CMD(BLE_CONN_INT)\
        AT_BLE_CMD(BLE_NONE)\

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define GENERATE_AT_BLE_CB_PROTO(CMD) uint8_t stm32wb_at_##CMD##_cb(stm32wb_at_##CMD##_t *param);
#define GENERATE_AT_BLE_CB_FUNC(CMD) __weak uint8_t stm32wb_at_##CMD##_cb(stm32wb_at_##CMD##_t *param){UNUSED(param);return 1;}
#define GENERATE_AT_BLE_SERVER_CB_PROTO(CMD) uint8_t stm32wb_at_##CMD##_server_cb(char *buff);
#define GENERATE_AT_BLE_SERVER_CB_FUNC(CMD) __weak uint8_t stm32wb_at_##CMD##_server_cb(char *buff){return 1;}

#define FOREACH_AT_BLE_EVT(AT_BLE_EVT)\
        AT_BLE_EVT(BLE_EVT_WRITE)\
        AT_BLE_EVT(BLE_EVT_CONN)\
        AT_BLE_EVT(BLE_EVT_UPD_CONN)\
        AT_BLE_EVT(BLE_EVT_VALUE_CONFIRM)\
        AT_BLE_EVT(BLE_EVT_PASSKEY_REQ)\
        AT_BLE_EVT(BLE_EVT_PAIRING)\
        AT_BLE_EVT(BLE_EVT_NONE)\

#define GENERATE_AT_BLE_EVT_CB_PROTO(CMD) uint8_t stm32wb_at_##CMD##_cb(stm32wb_at_##CMD##_t *param);
#define GENERATE_AT_BLE_EVT_CB_FUNC(CMD) __weak uint8_t stm32wb_at_##CMD##_cb(stm32wb_at_##CMD##_t *param){UNUSED(param);return 1;}

/* Exported types ------------------------------------------------------------*/
/* BLE_TEST */
typedef struct
{
  uint8_t dummy;
} stm32wb_at_BLE_TEST_t;

/* BLE_INIT */
typedef struct
{
  uint8_t init_set;
} stm32wb_at_BLE_INIT_t;

/* BLE_ADV_PARAM */
typedef struct
{
  uint8_t entry_number;
  uint8_t adv_type;
  float adv_int_min;
  float adv_int_max;
} stm32wb_at_BLE_ADV_PARAM_t;

/* BLE_ADV */
typedef struct
{
  uint8_t enable;
} stm32wb_at_BLE_ADV_t;

/* BLE_SVC */
typedef struct
{
  uint8_t index;
  uint8_t svc_uuid_type;
  uint16_t svc_uuid;
  uint8_t max_attr_record;
} stm32wb_at_BLE_SVC_t;

/* BLE_CHAR_ADD */
typedef struct
{
  uint8_t svc_id;
  uint8_t char_id;
  uint8_t char_uuid_type;
  uint16_t char_uuid;
  uint16_t char_value_len;
  uint8_t char_properties;
  uint8_t sec_permission;
  uint8_t gatt_evt_mask;
} stm32wb_at_BLE_CHAR_ADD_t;

/* BLE_SLAVE_SEC_REQ */
typedef struct
{
  uint8_t enable;
} stm32wb_at_BLE_PERIPH_SEC_REQ_t;

/* BLE_PAIRING_CONFIRM */
typedef struct
{
  uint8_t confirm_yes_no;
} stm32wb_at_BLE_PAIRING_CONFIRM_t;

/* BLE_PASSKEY_RESP */
typedef struct
{
  uint32_t pass_key_resp;
} stm32wb_at_BLE_PASSKEY_RESP_t;

/* BLE_IO_CAPA */
typedef struct
{
  uint8_t io_capability;
} stm32wb_at_BLE_IO_CAPA_t;

/* BLE_SET_AUTHEN_REQ */
typedef struct
{
  uint8_t bonding_mode;
  uint8_t mitm;
  uint8_t sc_support;
  uint8_t use_fixed_pin;
  uint32_t fixed_pin;
  uint8_t identity_address_type;
} stm32wb_at_BLE_SET_AUTHEN_REQ_t;

/* BLE_CLEAR_DB */
typedef struct
{
  uint8_t dummy;
} stm32wb_at_BLE_CLEAR_DB_t;

/* BLE_RST */
typedef struct
{
  uint8_t reset;
} stm32wb_at_BLE_RST_t;

/* BLE_PUB_ADDR */
typedef struct
{
  uint8_t BD_address[6];
} stm32wb_at_BLE_PUB_ADDR_t;

/* BLE_RAND_ADDR */
typedef struct
{
  uint8_t BD_address[6];
} stm32wb_at_BLE_RAND_ADDR_t;

/* BLE_CFG_RAND_ADDR */
typedef struct
{
  uint8_t cfg_rand_addr;
} stm32wb_at_BLE_CFG_RAND_ADDR_t;

/* BLE_NAME */
typedef struct
{
  char name[20];
} stm32wb_at_BLE_NAME_t;

/* BLE_RF_POWER */
typedef struct
{
  float tx_power;
  uint8_t tx_power_code;
} stm32wb_at_BLE_RF_POWER_t;

/* BLE_NOTIF_VAL */
typedef struct
{
  uint8_t svc_index;
  uint8_t char_index;
  uint8_t val_tab[64];
  uint8_t val_tab_len;
} stm32wb_at_BLE_NOTIF_VAL_t;

/* BLE_INDIC_VAL */
typedef struct
{
  uint8_t svc_index;
  uint8_t char_index;
  uint8_t val_tab[64];
  uint8_t val_tab_len;
} stm32wb_at_BLE_INDIC_VAL_t;

/* BLE_CONN_INT */
typedef struct
{
  float min;
  float max;
} stm32wb_at_BLE_CONN_INT_t;

/* BLE_NONE */
typedef struct
{
  uint8_t dummy;
} stm32wb_at_BLE_NONE_t;

/* BLE_EVT_WRITE */
typedef struct
{
  uint8_t svc_index;
  uint8_t char_index;
  uint8_t val_tab[245];
  uint8_t val_tab_len;
} stm32wb_at_BLE_EVT_WRITE_t;

/* BLE_EVT_CONN */
typedef struct
{
  uint8_t status;
} stm32wb_at_BLE_EVT_CONN_t;

/* BLE_EVT_UPD_CONN */
typedef struct
{
  uint8_t status;
} stm32wb_at_BLE_EVT_UPD_CONN_t;

/* BLE_EVT_VALUE_CONFIRM */
typedef struct
{
  uint32_t numeric_value;
} stm32wb_at_BLE_EVT_VALUE_CONFIRM_t;

/* BLE_EVT_PASSKEY_REQ */
typedef struct
{
  uint8_t dummy;
} stm32wb_at_BLE_EVT_PASSKEY_REQ_t;

typedef struct
{
  uint8_t status;
  uint8_t reason;
} stm32wb_at_BLE_EVT_PAIRING_t;

/* BLE_EVT_NONE */
typedef struct
{
  uint8_t dummy;
} stm32wb_at_BLE_EVT_NONE_t;

/* Exported constants --------------------------------------------------------*/
typedef enum {
  FOREACH_AT_BLE_CMD(GENERATE_ENUM)
}stm32wb_at_BLE_CMD_t;

typedef enum {
  FOREACH_AT_BLE_EVT(GENERATE_ENUM)
}stm32wb_at_BLE_EVT_t;

extern const char *AT_BLE_CMD_STRING[BLE_NONE+1];
extern const char *AT_BLE_EVT_STRING[BLE_EVT_NONE+1];

/* Exported macros -----------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
FOREACH_AT_BLE_CMD(GENERATE_AT_BLE_CB_PROTO)
FOREACH_AT_BLE_EVT(GENERATE_AT_BLE_EVT_CB_PROTO)

#ifdef __cplusplus
}
#endif

#endif /* STM32WB_AT_BLE_H */
