 /**
  ******************************************************************************
  * @file    ble_lld.h
  * @author  MCD Application Team
  * @brief   Header for ble_lld.c module
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
/** Parameters for an action packet */
typedef struct ActPac_s {
  uint8_t StateMachineNo ;      /* State machine number (0 - 7) */
  uint8_t ActionTag;            /* Bitfield for configuration of the action packet
                                 * PLL_TRIG, TXRX, TIMER_WAKEUP, TIMESTAMP_POSITION */
  uint32_t WakeupTime;          /* Time before running the action packet (us) */
                                /* Applicable only if TIMER_WAKEUP flag is set in ActionTag */
  uint32_t ReceiveWindowLength; /* Rx window size in (us). Applicable only for Rx */
  void * data;                  /* Payload to send. Applicable only for Tx */
  uint8_t dataSize;             /* Size of payload. Applicable only for Tx */
  uint32_t status;              /* Interrupt status register from the hardware */
  int32_t rssi;                 /* RSSI of the received packet. Applicable only for Rx */
  uint8_t nextTrue;             /* Next action packet to run if success */
  uint8_t nextFalse;            /* Next action packet to run if failure */
  uint8_t actionPacketNb;       /* Action packet number (0 - 7) */
  void (*callback)(radioEventType,    /* Function to run when action packet has finished */
                   struct ActPac_s *, /* If not used, it must be set to NULL */
                   void *,
                   uint8_t);
} ActionPacket;

/** Callback type for end of action packets */
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

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BLE_LLD_H */
