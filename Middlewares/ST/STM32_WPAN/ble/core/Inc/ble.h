/**
  ******************************************************************************
  * @file    ble.h
  * @author  MCD Application Team
  * @brief   BLE interface
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLE_H
#define __BLE_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include "ble_conf.h"
#include "ble_dbg_conf.h"

/**< core */
#include "uuid.h"
#include "ble_hal.h"
#include "ble_status.h"
#include "link_layer.h"
#include "hci_const.h"
#include "ble_gap_aci.h"
#include "ble_gap.h"
#include "ble_gatt_aci.h"
#include "ble_hal_aci.h"
#include "ble_l2cap_aci.h"
#include "ble_gatt_server.h"
#include "ble_types.h"
#include "ble_hci_le.h"

/**< blesvc */
#include "bls.h"
#include "crs_stm.h"
#include "dis.h"
#include "eds_stm.h"
#include "hids.h"
#include "hrs.h"
#include "hts.h"
#include "ias.h"
#include "lls.h"
#include "tps.h"
#include "p2p_stm.h"
#include "otas_stm.h"
#include "template_stm.h"
#include "mesh.h"  
  
#include "svc_ctl.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define EVT_BLUE_GATT_ATTRIBUTE_MODIFIED          (0x0C01)
#define EVT_BLUE_GATT_PROCEDURE_TIMEOUT           (0x0C02)
#define EVT_BLUE_ATT_EXCHANGE_MTU_RESP            (0x0C03)
#define EVT_BLUE_ATT_FIND_INFORMATION_RESP        (0x0C04)
#define EVT_BLUE_ATT_FIND_BY_TYPE_VAL_RESP        (0x0C05)
#define EVT_BLUE_ATT_READ_BY_TYPE_RESP            (0x0C06)
#define EVT_BLUE_ATT_READ_RESP                    (0x0C07)
#define EVT_BLUE_ATT_READ_BLOB_RESP               (0x0C08)
#define EVT_BLUE_ATT_READ_MULTIPLE_RESP           (0x0C09)
#define EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP      (0x0C0A)
#define EVT_BLUE_ATT_PREPARE_WRITE_RESP           (0x0C0C)
#define EVT_BLUE_ATT_EXEC_WRITE_RESP              (0x0C0D)
#define EVT_BLUE_GATT_INDICATION                  (0x0C0E)
#define EVT_BLUE_GATT_NOTIFICATION                (0x0C0F)
#define EVT_BLUE_GATT_PROCEDURE_COMPLETE          (0x0C10)
#define EVT_BLUE_GATT_ERROR_RESP                  (0x0C11)
#define EVT_BLUE_GATT_DISC_READ_CHAR_BY_UUID_RESP (0x0C12)
#define EVT_BLUE_GATT_WRITE_PERMIT_REQ            (0x0C13)
#define EVT_BLUE_GATT_READ_PERMIT_REQ             (0x0C14)
#define EVT_BLUE_GATT_READ_MULTI_PERMIT_REQ       (0x0C15)
#define EVT_BLUE_GATT_TX_POOL_AVAILABLE           (0x0C16)
#define EVT_BLUE_GATT_SERVER_CONFIRMATION_EVENT   (0x0C17)
#define EVT_BLUE_GATT_PREPARE_WRITE_PERMIT_REQ    (0x0C18)

#define EVT_BLUE_GAP_LIMITED_DISCOVERABLE         (0x0400)
#define EVT_BLUE_GAP_PAIRING_CMPLT                (0x0401)
#define EVT_BLUE_GAP_PASS_KEY_REQUEST             (0x0402)
#define EVT_BLUE_GAP_AUTHORIZATION_REQUEST        (0x0403)
#define EVT_BLUE_GAP_SLAVE_SECURITY_INITIATED     (0X0404)
#define EVT_BLUE_GAP_BOND_LOST                    (0X0405)
#define EVT_BLUE_GAP_DEVICE_FOUND                 (0x0406)
#define EVT_BLUE_GAP_PROCEDURE_COMPLETE           (0x0407)
#define EVT_BLUE_GAP_ADDR_NOT_RESOLVED            (0x0408)
#define EVT_BLUE_GAP_NUMERIC_COMPARISON_VALUE     (0x0409)
#define EVT_BLUE_GAP_KEYPRESS_NOTIFICATION        (0x040A)

#define EVT_BLUE_L2CAP_CONNECTION_UPDATE_REQ      (0x0802)
#define EVT_BLUE_L2CAP_CONNECTION_UPDATE_RESP     (0x0800)

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /*__BLE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
