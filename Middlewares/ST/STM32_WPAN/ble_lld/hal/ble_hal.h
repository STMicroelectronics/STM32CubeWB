/**
 ******************************************************************************
  * File Name          : ble_hal.h
  * Description        : Header for HAL based on LLD
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
uint8_t HAL_BLE_LLD_SendPacket(void *txBuffer,
                           uint8_t size,
                           lldCallback_t *callback);
uint8_t HAL_BLE_LLD_SendPacketWithAck(void *txBuffer,
                                  uint8_t size,
                                  uint32_t receiveTimeout,
                                  lldCallback_t *callback);
uint8_t HAL_BLE_LLD_ReceivePacket(uint32_t receiveTimeout,
                                  lldCallback_t *callback);
uint8_t HAL_BLE_LLD_ReceivePacketWithAck(void *txBuffer,
                                     uint8_t size,
                                     uint32_t receiveTimeout,
                                     lldCallback_t *callback);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
