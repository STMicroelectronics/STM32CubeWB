/**
  ******************************************************************************
  * @file    tps.c
  * @author  MCD Application Team
  * @brief   TX Power Service
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint16_t SvcHdle;				/**< Service handle */
  uint16_t LevelCharHdle;	        /**< Characteristic handle */
}TPS_Context_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_DRIVER_CONTEXT
 */

PLACE_IN_SECTION("BLE_DRIVER_CONTEXT")  static TPS_Context_t TPS_Context;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */


/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void TPS_Init(void)
{
  uint16_t uuid;
  tBleStatus hciCmdResult = BLE_STATUS_SUCCESS;

  /**
   *  Add TX Power Service
   *
   * Max_Attribute_Records = 2*no_of_char + 1
   * service_max_attribute_record = 1 for Tx Power service +
   *                                2 for Tx Power Level measurement characteristic +
   */
  uuid = TX_POWER_SERVICE_UUID;
  hciCmdResult = aci_gatt_add_service(UUID_TYPE_16,
                                   (Service_UUID_t *) &uuid,
                                   PRIMARY_SERVICE,
                                   3,
                                   &(TPS_Context.SvcHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_TPS_MSG ("Tx Power Service (TPS) is added Successfully %04X\n", 
                  TPS_Context.SvcHdle);
  }
  else
  {
    BLE_DBG_TPS_MSG ("FAILED to add Tx Power Service (TPS), Error: %02X !!\n", 
                 hciCmdResult);
  }
  
  /**
   *  Add Tx Power Level Characteristic
   */
  uuid = TX_POWER_LEVEL_CHARACTERISTIC_UUID;
  hciCmdResult = aci_gatt_add_char(TPS_Context.SvcHdle,
                    UUID_TYPE_16,
                    (Char_UUID_t *) &uuid ,
                    1,
                    CHAR_PROP_READ,
                    ATTR_PERMISSION_NONE,
                    GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                    10, /* encryKeySize */
                    1, /* isVariable */
                    &(TPS_Context.LevelCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_TPS_MSG ("Tx Power Level Characteristic Added Successfully  %04X \n", 
                        TPS_Context.LevelCharHdle);
  }
  else
  {
    BLE_DBG_TPS_MSG ("FAILED to add Tx Power Level Characteristic, Error: %02X !!\n", 
                        hciCmdResult);
  }

  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @retval TxPowerLevelValue: The new value to be written
 */
tBleStatus TPS_Update_Char(uint16_t UUID, uint8_t *pPayload)
{
  tBleStatus return_value=0;
  switch(UUID)
  {
    case TX_POWER_LEVEL_CHARACTERISTIC_UUID:
      return_value = aci_gatt_update_char_value(TPS_Context.SvcHdle,
                                                TPS_Context.LevelCharHdle,
                                                0, /* charValOffset */
                                                1, /* charValueLen */
                                                (uint8_t *) pPayload);

      break;

    default:
      break;
  }

  return return_value;
}/* end TPS_Update_Char() */



