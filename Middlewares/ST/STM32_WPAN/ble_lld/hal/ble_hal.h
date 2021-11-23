  /**
  ******************************************************************************
  * @file    ble_hal.h
  * @author  MCD Application Team
  * @brief   Header for ble_hal.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLE_HAL_H
#define BLE_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "ble_lld.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
uint8_t HAL_BLE_LLD_Init(uint16_t hsStartupTime, bool lsOscInternal);
uint8_t HAL_BLE_LLD_Configure(txPower_t txPower,
                              uint8_t channel,
                              bool phy2mbps,
                              uint32_t b2bTimeUs,
                              uint32_t networkId);
uint8_t HAL_BLE_LLD_SendPacket(void *data,
                           uint8_t size,
                           lldCallback_t *callback);
uint8_t HAL_BLE_LLD_SendPacketWithAck(void *data,
                                  uint8_t size,
                                  uint32_t receiveTimeout,
                                  lldCallback_t *callback);
uint8_t HAL_BLE_LLD_ReceivePacket(uint32_t receiveTimeout,
                                  lldCallback_t *callback);
uint8_t HAL_BLE_LLD_ReceivePacketWithAck(void *ack,
                                     uint8_t size,
                                     uint32_t receiveTimeout,
                                     lldCallback_t *callback);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BLE_HAL_H */
