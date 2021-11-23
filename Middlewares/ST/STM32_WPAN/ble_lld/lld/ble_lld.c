/**
  ******************************************************************************
  * @file    ble_lld.c
  * @author  MCD Application Team
  * @brief   BLE LLD messages management
  *          This file is responsible for command packing and response unpacking
  *          following ABI defined in ble_lld_transport.h
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
#include "app_common.h"
#include "ble_lld.h"
#include "ble_lld_transport.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static uint8_t sendCommand(BLE_LLD_Code_t bleCmd);

/* Private variables ---------------------------------------------------------*/

/** When sending command to M0, store pointer to action packet for later use
when receiving reply from M0 */
static ActionPacket *actionPackets[ACTION_PACKET_NB];

/* Encryption status is needed to prepare packet */
static bool encryptStatus[STATE_MACHINE_NB] = {0}; // all flags are initialized to false

/** Stores event received in interruption for later processing in a task */
static radioEventType radioEvent;

/** Stores action packet associated to event received for later processing in a task */
static ActionPacket *radioEventAp;

// Parameters stored by init

/** Parameters for command and response to/from radio core */
static param_BLE_LLD_t *params = NULL;

/** Buffer for packet to send */
static ipBLE_lld_txrxdata_Type *txBuffer = NULL;

/** Buffer for packet received */
static ipBLE_lld_txrxdata_Type *rxBuffer = NULL;

/** Callback to actually send commands to M0 */
static uint8_t (*CB_sendCommand)(BLE_LLD_Code_t bleCmd) = NULL;

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief Initializes BLE LLD proxy parameters.
 *
 * This function must be called before any BLE LLD function.
 *
 * @param[in] parameters Parameters for command and response to/from M0
 * @param[in] transmitBuffer Buffer for packet to send
 * @param[in] receiveBuffer Buffer for packet received
 * @param[in] callbackSend Function to send commands to M0
 */
void BLE_LLD_PRX_Init(param_BLE_LLD_t *parameters,
                      ipBLE_lld_txrxdata_Type *transmitBuffer,
                      ipBLE_lld_txrxdata_Type *receiveBuffer,
                      uint8_t (*callbackSend)(BLE_LLD_Code_t bleCmd))
{
  params = parameters;
  txBuffer = transmitBuffer;
  rxBuffer = receiveBuffer;
  CB_sendCommand = callbackSend;
}

/**
 * @brief Initializes the radio
 *
 * Whitening improves receiver lock, disabling it is only intended for test mode.
 * When whitening is disabled, data payload is limited to 45 bytes (including header).
 *
 * @param[in] hsStartupTime Startup time (system time unit)
 * @param[in] lowSpeedOsc Source for the 32 kHz slow speed clock:
 *                            1: internal RO
 *                            0: external crystal
 * @param[in] whitening ENABLE or DISABLE whitening for transmission and reception
 */
void BLE_LLD_Init(uint16_t hsStartupTime, uint8_t lowSpeedOsc, FunctionalState whitening)
{
  params->init.startupTime = (uint32_t)(hsStartupTime);
  params->init.lowSpeedOsc = lowSpeedOsc;
  params->init.whitening = (uint8_t)whitening;
  sendCommand(BLE_LLD_INIT_CMDCODE);
}

/**
 * @brief Checks if the radio is busy.
 *
 * @retval BLUE_IDLE_0 Radio is not busy
 * @retval BLUE_BUSY_NOWAKEUP_T2 Radio is busy, but there is no wakeup timer on the schedule but timer2 is
 * @retval BLUE_BUSY_WAKEUP Radio is busy and wakeup timer is on the schedule
 * @retval BLUE_BUSY_TONE Radio is in Tone
 * @retval BLUE_TONE_DESTROY Radio Tone has destroyed BLE: need an Init
 */
uint8_t BLE_LLD_GetStatus(void)
{
  return sendCommand(BLE_LLD_GETSTATUS_CMDCODE);
}

/**
 * @brief Sets the 40 bits receive and transmit packet count, used in encryption.
 *
 * Both set the 39-bit count + 1 bit MSB as defined in the Bluetooth Low Energy specifications
 * for encryption nonce calculation.
 *
 * @param [in] StateMachineNo State machine (0 - 7)
 * @param [in] countTx 40-bit transmit packet count
 * @param [in] countRx 40-bit receive packet count
 */
void BLE_LLD_SetEncryptionCount(uint8_t StateMachineNo, const uint8_t (*countTx)[5], const uint8_t (*countRx)[5])
{
  params->cryptCount.stateMachineNo = StateMachineNo;
  memcpy(params->cryptCount.countTx, countTx, sizeof(params->cryptCount.countTx));
  memcpy(params->cryptCount.countRcv, countRx, sizeof(params->cryptCount.countRcv));
  sendCommand(BLE_LLD_SETENCRYPTIONCOUNT_CMDCODE);
}

/**
 * @brief Sets the encryption initialization vector and the encryption key.
 *
 * @param[in] StateMachineNo State machine (0 - 7)
 * @param[in] encIv 8-byte encryption initialization vector
 * @param[in] encKey 16-byte encryption key
 */
void BLE_LLD_SetEncryptionAttributes(uint8_t StateMachineNo, const uint8_t (*encIv)[8], const uint8_t (*encKey)[16])
{
  params->cryptAttr.stateMachineNo = StateMachineNo;
  memcpy(params->cryptAttr.encIv, encIv, sizeof(params->cryptAttr.encIv));
  memcpy(params->cryptAttr.encKey, encKey, sizeof(params->cryptAttr.encKey));
  sendCommand(BLE_LLD_SETENCRYPTIONATTRIBUTES_CMDCODE);
}

/**
 * @brief Enables or disables encryption.
 *
 * @param[in] StateMachineNo State machine (0 - 7)
 * @param[in] EncryptFlag Encryption state:
 *                              0: disable
 *                              1: enable
 */
void BLE_LLD_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlag)
{
  params->cryptFlags.StateMachineNo = StateMachineNo;
  params->cryptFlags.encFlag = (uint8_t)EncryptFlag;
  encryptStatus[StateMachineNo] = EncryptFlag;
  sendCommand(BLE_LLD_SETENCRYPTFLAGS_CMDCODE);
}

/**
 * @brief Sets the radio channel.
 *
 * @param[in] StateMachineNo State machine (0 - 7)
 * @param[in] channel Radio channel (0 - 39)
 */
void BLE_LLD_SetChannel(uint8_t StateMachineNo, uint8_t channel)
{
  params->chan.stateMachineNo = StateMachineNo;
  params->chan.channel = channel;
  sendCommand(BLE_LLD_SETCHANNEL_CMDCODE);
}

/**
 * @brief Sets the network ID (access address).
 *
 * Network ID is not constrained, but some values might lead to bad radio behavior (whitening).
 * See BLE specs for safer values.
 *
 * @param[in] StateMachineNo State machine (0 - 7)
 * @param[in] NetworkID Network ID
 */
void BLE_LLD_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID)
{
  params->txAttr.stateMachineNo = StateMachineNo;
  params->txAttr.networkId = NetworkID;
  sendCommand(BLE_LLD_SETTXATTRIBUTES_CMDCODE);
}

/**
 * @brief Sets the back-to-back time.
 *
 * Back-to-back is a mode where two packets are chained with a short pause between them.
 * The back-to-back time is a global parameter.
 * Back-to-back time must be at least 50us.
 *
 * @param[in] backToBackTime Time between two packets in back-to-back mode (us)
 */
void BLE_LLD_SetBackToBackTime(uint32_t backToBackTime)
{
  params->b2b.backToBackTime = backToBackTime;
  sendCommand(BLE_LLD_SETBACKTOBACKTIME_CMDCODE);
}

/**
 * @brief Sets the transmit power level.
 *
 * See txPower_t for actual power output.
 *
 * @param[in] powerLevel Transmit power level (0 - 31)
 */
void BLE_LLD_SetTxPower(txPower_t powerLevel)
{
  params->power.power = powerLevel;
  sendCommand(BLE_LLD_SETTXPOWER_CMDCODE);
}

/**
 * @brief Sets the bitrate for transmission and reception.
 *
 * @param[in] StateMachineNo State machine (0 - 7)
 * @param[in] txPhy Speed for transmission: TX_PHY_1MBPS / TX_PHY_2MBPS
 * @param[in] rxPhy Speed for reception: RX_PHY_1MBPS / RX_PHY_2MBPS
 */
void BLE_LLD_SetTx_Rx_Phy(uint8_t StateMachineNo, uint8_t txPhy, uint8_t rxPhy)
{
  params->phy.stateMachineNo =  StateMachineNo;
  params->phy.txPhy = txPhy;
  params->phy.rxPhy = rxPhy;
  sendCommand(BLE_LLD_SETTX_RX_PHY_CMDCODE);
}

/**
 * @brief Configures an action packet.
 *
 * This function must be called after the relevant ActionPacket fields are set.
 *
 * @param[in] p Action packet to prepare, memory lifetime must extend until response processing
 */
void BLE_LLD_SetReservedArea(ActionPacket *p)
{
  actionPackets[p->actionPacketNb] = p;
  actionPackets[p->actionPacketNb]->callback = p->callback;

  params->reserved.stateMachineNo = p->StateMachineNo;
  params->reserved.nextTrue = p->nextTrue;
  params->reserved.nextFalse = p->nextFalse;
  params->reserved.wakeupTime = p->WakeupTime;
  params->reserved.receiveWindowLength = p->ReceiveWindowLength;
  params->reserved.actionPacketNb = p->actionPacketNb;
  params->reserved.actionTag = p->ActionTag;
  if(params->reserved.actionTag & TXRX) {
    params->reserved.buffer = txBuffer;
    BLE_LLD_packetPrepareCopy(txBuffer, p->data, p->dataSize, encryptStatus[p->StateMachineNo]);
  }else{
    params->reserved.buffer = rxBuffer;
  }
  sendCommand(BLE_LLD_SETRESERVEDAREA_CMDCODE);
}

/**
 * @brief Starts the radio FSM.
 *
 * This function schedules an action packet to be the first executed by the radio FSM.
 * BLE_LLD_SetReservedArea() must have been called first to prepare the action packet.
 *
 * @param[in] p Action packet to schedule, memory lifetime must extend until response processing
 *
 * @retval SUCCESS_0 if success
 * @retval RADIO_BUSY_C4 if radio is busy
 */
uint8_t BLE_LLD_MakeActionPacketPending(const ActionPacket *p)
{
  params->pending.actionPacketNb = p->actionPacketNb;
  return(sendCommand(BLE_LLD_MAKEACTIONPACKETPENDING_CMDCODE));
}

/**
 * @brief Stops the radio FSM.
 *
 * After a call to this function ISR will not be triggered, unless
 * MakeActionPacketPending() is called again.
 * This function returns when radio is ready to be initialized.
 *
 * @retval true
 */
uint8_t BLE_LLD_StopActivity(void)
{
  return(sendCommand(BLE_LLD_STOPACTIVITY_CMDCODE));
}

/**
 * @brief Starts tone transmission.
 *
 * This function is dedicated to tests and destroys context and multistate.
 * So, after calling this function the radio must be re-initialized.
 *
 * @param[in] rfChannel Radio channel (0 - 39)
 * @param[in] PowerLevel Output power level (0 - 31)
 */
void BLE_LLD_StartTone(uint8_t rfChannel, uint8_t powerLevel)
{
  params->toneStart.channel = rfChannel;
  params->toneStart.power = powerLevel;
  sendCommand(BLE_LLD_STARTTONE_CMDCODE);
}

/**
 * @brief Stops tone transmission.
 *
 * This function is dedicated to tests and destroys context and multistate.
 * So, after calling this function the radio must be re-initialized.
 */
void BLE_LLD_StopTone(void)
{
  sendCommand(BLE_LLD_STOPTONE_CMDCODE);
}


/**
 * @brief Processes received event from radio core during interruption.
 *
 * Must be called during the interruption triggered when a radio event is
 * received from radio core.
 * Stores event data for further processing after interruption.
 *
 * @param[in] event Radio core event type
 *
 */
void BLE_LLD_PRX_EventProcessInter(radioEventType event)
{
  // Action packet for which the event is coming back
  ActionPacket *ap = actionPackets[params->reply.actionPacketNb];
  radioEvent = event;
  radioEventAp = ap;
  ap->status = params->reply.status;
  if (RADIO_IS_RX_OK(event)){
    ap->rssi = params->reply.rssi;
  }
}

/**
 * @brief Processes received event from radio core after interruption.
 *
 * Must be called after the interruption triggered when a radio event is
 * received from radio core, i.e. in a task.
 * Runs the callback registered for the received event.
 *
 */
void BLE_LLD_PRX_EventProcessTask(void)
{
  void *data = NULL;
  uint8_t size = 0;
  const bool encrypt = encryptStatus[radioEventAp->StateMachineNo];
  if (radioEventAp->callback != NULL){
    if (RADIO_IS_RX_OK(radioEvent)){
      size = BLE_LLD_packetExtractInPlace(rxBuffer, &data, encrypt);
    }
    radioEventAp->callback(radioEvent, radioEventAp, data, size);
  }
}

// Utility functions

static uint8_t sendCommand(BLE_LLD_Code_t bleCmd){
  return CB_sendCommand(bleCmd);
}

const char *eventToString(radioEventType evt){
  switch(evt){
    case TX_OK_BUSY:        return "TX_OK_BUSY";
    case TX_OK_READY:       return "TX_OK_READY";
    case TX_FAIL_BUSY:      return "TX_FAIL_BUSY";
    case TX_FAIL_READY:     return "TX_FAIL_READY";
    case RX_OK_BUSY:        return "RX_OK_BUSY";
    case RX_OK_READY:       return "RX_OK_READY";
    case RX_TIMEOUT_BUSY:   return "RX_TIMEOUT_BUSY";
    case RX_TIMEOUT_READY:  return "RX_TIMEOUT_READY";
    case RX_CRC_KO_BUSY:    return "RX_CRC_KO_BUSY";
    case RX_CRC_KO_READY:   return "RX_CRC_KO_READY";
    case RX_FAIL_BUSY:      return "RX_FAIL_BUSY";
    case RX_FAIL_READY:     return "RX_FAIL_READY";
    default:                return "ERROR: invalid event";
  }
}

/* Header field is interpreted by hardware and has an impact on some flags, so
   user should not change it. This is a default safe value */
#define LLD_HEADER 0x55

uint8_t BLE_LLD_packetPrepareCopy(ipBLE_lld_txrxdata_Type *packet,
                                  const void *data,
                                  uint8_t size,
                                  bool encrypt)
{
  uint32_t actual_size = size;
  if (encrypt){
    actual_size += MIC_SIZE;
  }
  assert_param(actual_size <= sizeof(packet->payload));

  packet->header = LLD_HEADER;
  packet->length = actual_size;
  memcpy(packet->payload, data, actual_size);
  return (uint8_t)actual_size; // Max value has been checked
}

uint8_t BLE_LLD_packetPrepareInPlace(ipBLE_lld_txrxdata_Type *packet,
                                     bool encrypt)
{
  uint32_t actual_size = packet->length;
  if (encrypt){
    actual_size += MIC_SIZE;
  }
  assert_param(actual_size <= sizeof(packet->payload));

  packet->header = LLD_HEADER;
  packet->length = actual_size;
  return (uint8_t)actual_size; // Max value has been checked
}

uint8_t BLE_LLD_packetExtractCopy(const ipBLE_lld_txrxdata_Type *packet,
                                  void *data,
                                  bool encrypt)
{
  uint8_t actual_size = packet->length;
  if (encrypt){
    actual_size -= MIC_SIZE;
  }

  memcpy(data, packet->payload, actual_size);
  return actual_size;
}

uint8_t BLE_LLD_packetExtractInPlace(ipBLE_lld_txrxdata_Type *packet,
                                     void **data,
                                     bool encrypt)
{
  uint8_t actual_size = packet->length;
  if (encrypt){
    actual_size -= MIC_SIZE;
  }

  *data = packet->payload;
  return actual_size;
}

uint8_t BLE_LLD_packetGetSize(const ipBLE_lld_txrxdata_Type *packet,
                              bool encrypt)
{
  uint8_t actual_size = packet->length;
  if (encrypt){
    actual_size -= MIC_SIZE;
  }
  return actual_size;
}
