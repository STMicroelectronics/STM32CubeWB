/**
 ******************************************************************************
  * File Name          : ble_lld.h
  * Description        : Header for BLE LLD application.
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
#ifndef BLE_LLD_H
#define BLE_LLD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_wpan_common.h"
#include "stm32wbxx.h"
#include <stdbool.h>
#include "ipBLE_lld_public.h"
#include "ble_lld_transport.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/* ---------------------------------------------------------------------------*/
/* ------------------------------- BLE LLD -----------------------------------*/
/* ---------------------------------------------------------------------------*/
typedef struct ActPac_s {
  uint8_t StateMachineNo ;      /* This parameter indicates the state machine number for this action. From 0 to 7. */
  uint8_t ActionTag;            /* The configuration of the current action.
                                 * Action Tag: TXRX, TIMER_WAKEUP, INC_CHAN, TIMESTAMP_POSITION */
  uint32_t WakeupTime;          /* Contains the wakeup time in microsecond.
                                 * It only applies if TIMER_WAKEUP flag is set in ActionTag. */
  uint32_t ReceiveWindowLength; /* Sets RX window size in microsecond. Applicable only for RX actions. */
  void *   data;                /* Pointer to the array with the data to send (header, length and data field), for TX. */
  uint8_t dataSize;             /* Size of payload for Tx */
  uint32_t status;              /* The Status Register with the information on the action. */
  int32_t rssi;                 /* The rssi of the packet was received with. RX only. */
  uint8_t nextTrue;            /* Pointer to next ActionPacket if success */
  uint8_t nextFalse;           /* Pointer to next ActionPacket if failure */
  uint8_t actionPacketNb;       /* User callback for managing data. */
  void (*callback)(radioEventType ,   /* Callback to run when action packet has finished */
                   struct ActPac_s *, /* If not used, it must be set to NULL */
                   void *,
                   uint8_t);
} ActionPacket;

typedef void (lldCallback_t)(radioEventType, ActionPacket *, void *, uint8_t);

/* Exported functions ------------------------------------------------------- */
void BLE_LLD_PRX_Init(param_BLE_LLD_t *parameters,
                      ipBLE_lld_txrxdata_Type *transmitBuffer,
                      ipBLE_lld_txrxdata_Type *receiveBuffer,
                      uint8_t (*callbackSend)(BLE_LLD_Code_t bleCmd));
void BLE_LLD_PRX_EventProcessInter(radioEventType event);
void BLE_LLD_PRX_EventProcessTask(void);

uint8_t BLE_LLD_GetStatus(void);
void    BLE_LLD_SetChannel(uint8_t StateMachineNo, uint8_t channel);
void    BLE_LLD_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID);
void    BLE_LLD_SetBackToBackTime(uint32_t backToBackTime);
void    BLE_LLD_SetTxPower(txPower_t powerLevel);
void    BLE_LLD_SetTx_Rx_Phy(uint8_t StateMachineNo, uint8_t txPhy, uint8_t rxPhy);
uint8_t BLE_LLD_StopActivity(void);
void    BLE_LLD_SetEncryptionCount(uint8_t StateMachineNo, const uint8_t (*countTx)[5], const uint8_t (*countRx)[5]);
void    BLE_LLD_SetEncryptionAttributes(uint8_t StateMachineNo, const uint8_t (*encIv)[8], const uint8_t (*encKey)[16]);
void    BLE_LLD_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlag);
void    BLE_LLD_StartTone(uint8_t rfChannel, uint8_t powerLevel);
void    BLE_LLD_StopTone(void);
void    BLE_LLD_SetReservedArea(ActionPacket *p);
uint8_t BLE_LLD_MakeActionPacketPending(const ActionPacket *p);
void    BLE_LLD_Init(uint16_t hsStartupTime, uint8_t lowSpeedOsc, FunctionalState whitening);

uint8_t BLE_LLD_packetPrepareCopy(ipBLE_lld_txrxdata_Type *packet,
                                  const void *data,
                                  uint8_t size,
                                  bool encrypt);
uint8_t BLE_LLD_packetPrepareInPlace(ipBLE_lld_txrxdata_Type *packet,
                                     bool encrypt);
uint8_t BLE_LLD_packetExtractCopy(const ipBLE_lld_txrxdata_Type *packet,
                                  void *data,
                                  bool encrypt);
uint8_t BLE_LLD_packetExtractInPlace(ipBLE_lld_txrxdata_Type *packet,
                                     void **data,
                                     bool encrypt);
uint8_t BLE_LLD_packetGetSize(const ipBLE_lld_txrxdata_Type *packet,
                              bool encrypt);

const char *eventToString(radioEventType evt);
#endif /* BLE_LLD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
