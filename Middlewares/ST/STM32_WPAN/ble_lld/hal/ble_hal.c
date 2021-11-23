/**
  ******************************************************************************
  * @file    ble_hal.c
  * @author  MCD Application Team
  * @brief   HAL based on BLE LLD
  *          This file provides functions for a simpler access to radio than LLD
  *          HAL relies on the LLD API.
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

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "ble_lld.h"
#include "ble_hal.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define WAKEUP_TIME_US 1000 

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ActionPacket actPack[ACTION_PACKET_NB];

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief Initializes the radio
 *
 * Whithening is forced.
 * Before actual use, radio must be configured with HAL_BLE_LLD_Configure().
 *
 * @param[in] hsStartupTime Startup time (system time unit)
 * @param[in] lsOscInternal Use internal RO for the 32 kHz slow speed clock
 *                          (else external crystal)
 */
uint8_t HAL_BLE_LLD_Init(uint16_t hsStartupTime, bool lsOscInternal)
{
  BLE_LLD_Init(hsStartupTime, lsOscInternal, ENABLE);
  return SUCCESS_0;
}

/**
 * @brief Configures the radio
 *
 * @param[in] txPower Transmit power for outgoing packets
 * @param[in] channel Radio channel (0 - 39)
 * @param[in] phy2mbps Use 2Mb/s PHY speed (else 1Mb/s)
 * @param[in] b2bTimeUs Back to back time (us), delay between packet and ACK
 * @param[in] networkId Network ID (access address)
 */
uint8_t HAL_BLE_LLD_Configure(txPower_t txPower,
                              uint8_t channel,
                              bool phy2mbps,
                              uint32_t b2bTimeUs,
                              uint32_t networkId)
{
  BLE_LLD_SetTxPower(txPower);
  BLE_LLD_SetChannel(STATE_MACHINE_0, channel);
  BLE_LLD_SetTx_Rx_Phy(STATE_MACHINE_0,
                       phy2mbps ? TX_PHY_2MBPS : TX_PHY_1MBPS,
                       phy2mbps ? RX_PHY_2MBPS : RX_PHY_1MBPS);
  BLE_LLD_SetBackToBackTime(b2bTimeUs);
  BLE_LLD_SetTxAttributes(STATE_MACHINE_0, networkId);
  return SUCCESS_0;
}

/**
 * @brief Sends one packet without listening for an acknowledge.
 *
 * @param[in] data Data to transmit
 * @param[in] size Size of data to transmit
 * @param[in] callback Function that will be called once radio operation is done
 *
 * @retval SUCCESS_0 if success
 * @retval INVALID_PARAMETER_C0 if invalid parameters
 * @retval RADIO_BUSY_C4 if radio is busy
 */
uint8_t HAL_BLE_LLD_SendPacket(void *data,
                           uint8_t size,
                           lldCallback_t *callback)
{
  ActionPacket *ap;

  // Packet to send
  ap = &actPack[APACKET_0];
  ap->actionPacketNb = APACKET_0;
  ap->StateMachineNo = STATE_MACHINE_0;
  ap->ActionTag = TXRX | TIMER_WAKEUP;
  ap->WakeupTime = WAKEUP_TIME_US;
  ap->data = data;
  ap->dataSize = size;
  ap->nextTrue = APACKET_STOP;
  ap->nextFalse = APACKET_STOP;
  ap->callback = callback;
  BLE_LLD_SetReservedArea(ap);

  BLE_LLD_MakeActionPacketPending(ap);
  return SUCCESS_0;
}

/**
 * @brief Sends one packet and listen for an acknowledge.
 *
 * @param[in] data Data to transmit
 * @param[in] size Size of data to transmit
 * @param[in] receiveTimeout Timeout for ACK reception (us)
 * @param[in] callback Function that will be called once radio operation is done
 *
 * @retval SUCCESS_0 if success
 * @retval INVALID_PARAMETER_C0 if invalid parameters
 * @retval RADIO_BUSY_C4 if radio is busy
 */
uint8_t HAL_BLE_LLD_SendPacketWithAck(void *data,
                                  uint8_t size,
                                  uint32_t receiveTimeout,
                                  lldCallback_t *callback)
{
  ActionPacket *ap;

  // Packet to send
  ap = &actPack[APACKET_0];
  ap->actionPacketNb = APACKET_0;
  ap->StateMachineNo = STATE_MACHINE_0;
  ap->ActionTag = TXRX | TIMER_WAKEUP;
  ap->WakeupTime = WAKEUP_TIME_US;
  ap->data = data;
  ap->dataSize = size;
  ap->nextTrue = APACKET_1;
  ap->nextFalse = APACKET_STOP;
  ap->callback = callback;
  BLE_LLD_SetReservedArea(ap);

  // ACK to receive
  ap = &actPack[APACKET_1];
  ap->actionPacketNb = APACKET_1;
  ap->StateMachineNo = STATE_MACHINE_0;
  ap->ActionTag = 0;
  ap->ReceiveWindowLength = receiveTimeout;
  ap->nextTrue = APACKET_STOP;
  ap->nextFalse = APACKET_STOP;
  ap->callback = callback;
  BLE_LLD_SetReservedArea(ap);

  BLE_LLD_MakeActionPacketPending(&actPack[APACKET_0]);
  return SUCCESS_0;
}

/**
 * @brief Receives one packet without transmitting an acknowledge.
 *
 * @param[in] receiveTimeout Timeout for data reception (us)
 * @param[in] callback Function that will be called once radio operation is done
 *
 * @retval SUCCESS_0 if success
 * @retval INVALID_PARAMETER_C0 if invalid parameters
 * @retval RADIO_BUSY_C4 if radio is busy
 */
uint8_t HAL_BLE_LLD_ReceivePacket(uint32_t receiveTimeout,
                              lldCallback_t *callback)
{
  ActionPacket *ap;

  // Packet to receive
  ap = &actPack[APACKET_2];
  ap->actionPacketNb = APACKET_2;
  ap->StateMachineNo = STATE_MACHINE_0;
  ap->ActionTag = TIMER_WAKEUP;
  ap->WakeupTime = WAKEUP_TIME_US;
  ap->ReceiveWindowLength = receiveTimeout;
  ap->nextTrue = APACKET_STOP;
  ap->nextFalse = APACKET_STOP;
  ap->callback = callback;
  BLE_LLD_SetReservedArea(ap);

  BLE_LLD_MakeActionPacketPending(ap);
  return SUCCESS_0;
}

/**
 * @brief Receives one packet and transmits an acknowledge.
 *
 * @param[in] ack Acknowledge to transmit
 * @param[in] size Size of acknowledge to transmit
 * @param[in] receiveTimeout Timeout for data reception (us)
 * @param[in] callback Function that will be called once radio operation is done
 *
 * @retval SUCCESS_0 if success
 * @retval INVALID_PARAMETER_C0 if invalid parameters
 * @retval RADIO_BUSY_C4 if radio is busy
 */
uint8_t HAL_BLE_LLD_ReceivePacketWithAck(void *ack,
                                     uint8_t size,
                                     uint32_t receiveTimeout,
                                     lldCallback_t *callback)
{
  ActionPacket *ap;

  // Packet to receive
  ap = &actPack[APACKET_2];
  ap->actionPacketNb = APACKET_2;
  ap->StateMachineNo = STATE_MACHINE_0;
  ap->ActionTag = TIMER_WAKEUP;
  ap->WakeupTime = WAKEUP_TIME_US;
  ap->ReceiveWindowLength = receiveTimeout;
  ap->nextTrue = APACKET_3;
  ap->nextFalse = APACKET_STOP;
  ap->callback = callback;
  BLE_LLD_SetReservedArea(ap);

  // ACK to send
  ap = &actPack[APACKET_3];
  ap->actionPacketNb = APACKET_3;
  ap->StateMachineNo = STATE_MACHINE_0;
  ap->ActionTag = TXRX;
  ap->data = ack;
  ap->dataSize = size;
  ap->nextTrue = APACKET_STOP;
  ap->nextFalse = APACKET_STOP;
  ap->callback = callback;
  BLE_LLD_SetReservedArea(ap);

  BLE_LLD_MakeActionPacketPending(&actPack[APACKET_2]);
  return SUCCESS_0;
}
