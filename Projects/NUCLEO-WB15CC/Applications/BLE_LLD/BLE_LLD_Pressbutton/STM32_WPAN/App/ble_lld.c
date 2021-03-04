/**
 ******************************************************************************
  * File Name          : ble_lld.c
  * Description        : BLE LLD Application.
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

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "ble_lld.h"
#include "ble_lld_private.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void unpackRx(param_BLE_LLD_m0Reply_t *reply);
static uint8_t sendCommand(BLE_LLD_Code_t bleCmd , param_BLE_LLD_t *bleParams);

/* Private variables ---------------------------------------------------------*/

/** When sending command to M0, store pointer to action packet for later use
when receiving reply from M0 */
static ActionPacket *actionPackets[ACTION_PACKET_NB];

/** This variable is not a full fledged action packet, its purpose is to allow HAL
functions to use the same code as LLD functions for data comming back from M0.
Hence, only fields used for reply data are ever used. */

/** Memory used to send/receive data and parameters to/from M0 */
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static param_BLE_LLD_t bleparam_BLE_LLD_Packet;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static bleCmd_t command;

/** Configuration for data routines multi */
static struct {
  uint32_t packetNumber; // NB of Successif PACKET Send or Receive
  uint8_t  packetStopRx; // Stop after RX
} dataRoutineParams = {1, 1};

/** Stores command received in interruption for later processing in a task */
static cmdFromM0_t m0Cmd;

/** Callback to send commands to M0 */
static void (*CB_sendCmdM0)(void *) = NULL;

/* Functions Definition ------------------------------------------------------*/

/**
 * @brief Initializes BLE LLD proxy.
 *
 * This function must be called before any BLE LLD function.
 *
 * @param[in] callbackSendCmdM0 Function to send commands to M0
 */
void BLE_LLD_PRX_Init(void (*callbackSendCmdM0)(void *))
{
  CB_sendCmdM0 = callbackSendCmdM0;
}

/**
 * @brief Sets configuration for data routines multi
 *
 * @param[in] PacketNumber Number of Successif PACKET using LL Send or Receive
 * @param[in] PacketStop Stop after RX
 */
void BLE_LLD_PRX_SetdataRoutineMultiOptions(uint32_t PacketNumber, uint8_t PacketStop)
{
  dataRoutineParams.packetNumber = PacketNumber;
  dataRoutineParams.packetStopRx = PacketStop;
}


/* ------------------------------- LL LEVEL -----------------------------------*/
/**
 * @brief Initializes the BLE Radio in LL mode.
 *
 * Whitening improves receiver lock, disabling it is only intended for test mode.
 * When whitening is disabled, data payload is limited to 45 bytes (including header).
 *
 * @param[in] hs_startup_time Startup time (system time unit)
 * @param[in] low_speed_osc Source for the 32 kHz slow speed clock:
 *                            1: internal RO
 *                            0: external crystal
 * @param[in] hot_table Config table of the Radio and SPI
 * @param[in] whitening ENABLE or DISABLE whitening for transmission and reception
 */
void BLE_LLD_Init(uint16_t hs_startup_time, uint8_t low_speed_osc, uint32_t* hot_table, FunctionalState whitening)
{
  param_BLE_LLD_init_t *params = (param_BLE_LLD_init_t *)&bleparam_BLE_LLD_Packet;
  params->startupTime = (uint32_t)(hs_startup_time);
  params->lowSpeedOsc = low_speed_osc;
  params->whitening = (uint8_t)whitening;
  sendCommand(BLE_LLD_INIT_CMDCODE, &bleparam_BLE_LLD_Packet);
  memcpy((uint8_t*)hot_table, params->txrxBuffer.txBuffer, BLE_HOT_ANA_CONFIG_TABLE_LENGTH);
}

/**
 * @brief Checks if the radio is busy.
 *
 * If radio is busy, the time argument will be filled with WakeupTimer value.
 * User can use this value (in comparison with current time) to check that how
 * long it is far to force the device to sleep or not.
 *
 * @param[out] time WakeupTimer value
 *
 * @retval BLUE_IDLE_0 Radio is not busy
 * @retval BLUE_BUSY_NOWAKEUP_T2 Radio is busy, but there is no wakeup timer on the schedule but timer2 is
 * @retval BLUE_BUSY_WAKEUP Radio is busy and wakeup timer is on the schedule
 * @retval BLUE_BUSY_TONE. Radio is in Tone
 * @retval BLUE_TONE_DESTROY. Radio Tone has destroyed BLE: need an Init
 */
uint8_t BLE_LLD_GetStatus(uint32_t *time)
{
  uint8_t return_value;
  param_BLE_LLD_status_t *params = (param_BLE_LLD_status_t *)&bleparam_BLE_LLD_Packet;
  params->time = NULL;
  return_value=sendCommand(BLE_LLD_GETSTATUS_CMDCODE, &bleparam_BLE_LLD_Packet);
  *time = params->time;
  return(return_value);
}

/**
 * @brief Sets the 40 bits receive and transmit packet count, used in encryption.
 *
 * Both set the 39-bit count + 1 bit MSB as defined in the Bluetooth Low Energy specifications
 * for encryption nonce calculation.
 *
 * @param [in] StateMachineNo	State machine number in multistate (between 0 and 7)
 * @param [in] count_tx 40-bit transmit packet count
 * @param [in] count_rcv 40-bit receive packet count
 */
void BLE_LLD_SetEncryptionCount(uint8_t StateMachineNo, uint8_t (*count_tx)[5], uint8_t (*count_rcv)[5])
{
  param_BLE_LLD_cryptCount_t *params = (param_BLE_LLD_cryptCount_t *)&bleparam_BLE_LLD_Packet;
  params->stateMachineNo = StateMachineNo;
  memcpy(params->countTx, count_tx, sizeof(params->countTx));
  memcpy(params->countRcv, count_rcv, sizeof(params->countRcv));
  sendCommand(BLE_LLD_SETENCRYPTIONCOUNT_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Set the 8-byte encryption initialization vector and the 16-byte encryption key.
 *
 * @param[in] StateMachineNo State machine number in multistate (between 0 and 7)
 * @param[in] enc_iv 8-byte encryption initialization vector
 * @param[in] enc_key 16-byte encryption key
 */
void BLE_LLD_SetEncryptionAttributes(uint8_t StateMachineNo, uint8_t (*enc_iv)[8], uint8_t (*enc_key)[16])
{
  param_BLE_LLD_cryptAttr_t *params = (param_BLE_LLD_cryptAttr_t *)&bleparam_BLE_LLD_Packet;
  params->stateMachineNo = StateMachineNo;
  memcpy(params->encIv, enc_iv, sizeof(params->encIv));
  memcpy(params->encKey, enc_key, sizeof(params->encKey));
  sendCommand(BLE_LLD_SETENCRYPTIONATTRIBUTES_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Enables or disables encryption.
 *
 * There is only one bit in hardware for both Tx and Rx, so encryption is
 * enabled for both if either of the bits is set.
 *
 * Another point is that, when encryption is enabled the hardware will add
 * 4 bytes at the end of the packet as MAC (Message Authentication Code).
 * So, the user needs to add 4 to the length of packet when encryption is ON.
 *
 * @param[in] StateMachineNo State Number in multistate (between 0 and 7)
 * @param[in] EncryptFlagTx Encryption Flag for TX:
 *                              0: Encryption is turned off
 *                              1: encryption is turned on
 * @param[in] EncryptFlagRcv Encryption Flag for RX:
 *                              0: Encryption is turned off
 *                              1: encryption is turned on
 */
void BLE_LLD_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlagTx, FunctionalState EncryptFlagRcv)
{
  param_BLE_LLD_cryptFlags_t *params = (param_BLE_LLD_cryptFlags_t *)&bleparam_BLE_LLD_Packet;
  params->StateMachineNo=StateMachineNo;
  params->encFlagTx=(uint8_t)EncryptFlagTx;
  params->encFlagRx=(uint8_t)EncryptFlagRcv;
  sendCommand(BLE_LLD_SETENCRYPTFLAGS_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Encrypts data using AES
 *
 * @param[in] Key Encryption key (128 bits)
 * @param[in] plainData Data to encrypt (128 bits)
 * @param[out] cypherData Encrypted data (128 bits)
 */
void BLE_LLD_EncryptPlainData(uint8_t (*Key)[16], uint8_t (*plainData)[16], uint8_t (*cypherData)[16])
{
  param_BLE_LLD_crypt_t *params = (param_BLE_LLD_crypt_t *)&bleparam_BLE_LLD_Packet;
  memcpy(params->key, Key, sizeof(params->key));
  memcpy(params->plainData, plainData, sizeof(params->plainData));
  sendCommand(BLE_LLD_ENCRYPTPLAINDATA_CMDCODE, &bleparam_BLE_LLD_Packet);
  memcpy(cypherData, params->cypherData, sizeof(params->cypherData));
}

/**
 * @brief Sets channel map
 *
 * If the channel map is not in use, do not define it.
 * The LSB corresponds to bit 0. When the corresponding bit is set, the channel
 * is in use. When the corresponding bit it is cleared, there will be an
 * automatic remap to another channel, conforming to the Bluetooth Low Energy
 * specification.
 * Setting all bits of the chan_remap vector to ‘1’ disables the channel
 * remapping. This is the expected mode when the Bluetooth channel remap is not
 * in use.
 *
 * @param[in] StateMachineNo State machine number in multistate (between 0 and 7)
 * @param[in] chan_remap Bitmap of channels to use if 1 (between 0 and 36)
 */
void BLE_LLD_SetChannelMap(uint8_t StateMachineNo, uint8_t (*chan_remap)[5])
{
  param_BLE_LLD_chanMap_t *params = (param_BLE_LLD_chanMap_t *)&bleparam_BLE_LLD_Packet;
  params->stateMachineNo = StateMachineNo;
  memcpy(params->map, chan_remap, sizeof(params->map));
  sendCommand(BLE_LLD_SETCHANNELMAP_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Sets the channel and the channel increment.
 *
 * @param[in] StateMachineNo State machine number in multistate (between 0 and 7)
 * @param[in] channel Frequency channel (between 0 and 39)
 * @param[in] channel_increment Channel increment value determines the hoping value
 */
void BLE_LLD_SetChannel(uint8_t StateMachineNo, uint8_t channel, uint8_t channel_increment) 
{
  param_BLE_LLD_chan_t *params = (param_BLE_LLD_chan_t *)&bleparam_BLE_LLD_Packet;
  params->stateMachineNo = StateMachineNo;
  params->channel = channel;
  params->channelInc = channel_increment;
  sendCommand(BLE_LLD_SETCHANNEL_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Sets the access address (AccessAddress), the CRC initialization value and the Slow Clock Accuracy (SCA).
 *
 * @param[in] StateMachineNo State machine number in multistate (between 0 and 7)
 * @param[in] NetworkID BLE NetworkID
 * @param[in] crc_init CRC initialization value, must be 0x555555
 * @param[in] sca Parameter not used
 */
void BLE_LLD_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID, uint32_t crc_init, uint32_t sca)
{
  param_BLE_LLD_txAttr_t *params = (param_BLE_LLD_txAttr_t *)&bleparam_BLE_LLD_Packet;
  params->stateMachineNo = StateMachineNo;
  params->networkId = NetworkID ;
  params->crcInit = crc_init;
  params->sca = sca; // not used
  sendCommand(BLE_LLD_SETTXATTRIBUTES_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Sets the time between back-to-back radio transmissions.
 *
 * A minimum value of 50us must be set.
 *
 * @param[in] back_to_back_time Time between two frames in back to back mode (us)
 */
void BLE_LLD_SetBackToBackTime(uint32_t back_to_back_time)
{
  param_BLE_LLD_b2b_t *params = (param_BLE_LLD_b2b_t *)&bleparam_BLE_LLD_Packet;
  params->backToBackTime = back_to_back_time;
  sendCommand(BLE_LLD_SETBACKTOBACKTIME_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Sets the transmit power level.
 *
 * @param[in] powerLevel Transmit power level (between 0 and 31)
 */
void BLE_LLD_SetTxPower(uint8_t powerLevel)
{
  param_BLE_LLD_power_t *params = (param_BLE_LLD_power_t *)&bleparam_BLE_LLD_Packet;
  params->power = powerLevel;
  sendCommand(BLE_LLD_SETTXPOWER_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Sets the data speed for transmission and reception.
 *
 * @param[in] StateMachineNo State machine number in multistate (between 0 and 7)
 * @param[in] tx_phy Speed for transmission: TX_PHY_1MBPS / TX_PHY_2MBPS
 * @param[in] rx_phy Speed for reception RX_PHY_1MBPS / RX_PHY_2MBPS
 */
void BLE_LLD_SetTx_Rx_Phy(uint8_t StateMachineNo, uint8_t tx_phy, uint8_t rx_phy)
{
  param_BLE_LLD_phy_t *params = (param_BLE_LLD_phy_t *)&bleparam_BLE_LLD_Packet;
  params->stateMachineNo =  StateMachineNo;
  params->txPhy = tx_phy;
  params->rxPhy = rx_phy;
  sendCommand(BLE_LLD_SETTX_RX_PHY_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Prepares an action packet for execution.
 *
 * This function must be called after the action packet fields are set.
 *
 * @param[in] p Action packet to prepare, memory lifetime must extend until response processing
 */
void BLE_LLD_SetReservedArea(ActionPacket *p)
{
  param_BLE_LLD_reserved_t *params = (param_BLE_LLD_reserved_t *)&bleparam_BLE_LLD_Packet;
  
  actionPackets[p->actionPacketNb] = p;
  
  params->stateMachineNo = p->StateMachineNo;
  params->nextTrue = p->next_true;
  params->nextFalse = p->next_false;
  params->wakeupTime = p->WakeupTime;
  params->receiveWindowLength = p->ReceiveWindowLength;
  params->actionPacketNb = p->actionPacketNb;
  params->actionTag = p->ActionTag;
  params->setCase.condCase = (condCase_t)p->condRoutine;
  params->setCase.dataCase = (dataCase_t)p->dataRoutine;
  if(params->actionTag & AT_TXRX) {
    memcpy((params->txrxBuffer.txBuffer), p->data, TXRX_BUF_SIZE);
  }
  sendCommand(BLE_LLD_SETRESERVEDAREA_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Schedules an action packet for execution on the radio.
 *
 * BLE_LLD_SetReservedArea() must have been called first to prepare the action packet.
 *
 * @param[in] p Action packet to schedule, memory lifetime must extend until response processing
 *
 * @retval SUCCESS_0 if success
 * @retval RADIO_BUSY_C4 if radio is busy
 */
uint8_t BLE_LLD_MakeActionPacketPending(ActionPacket *p)
{
  param_BLE_LLD_mkPending_t *params = (param_BLE_LLD_mkPending_t *)&bleparam_BLE_LLD_Packet;
  params->actionPacketNb = p->actionPacketNb;
  params->packetNumber = dataRoutineParams.packetNumber;
  params->packetStopRx = dataRoutineParams.packetStopRx;
  return(sendCommand(BLE_LLD_MAKEACTIONPACKETPENDING_CMDCODE, &bleparam_BLE_LLD_Packet));
}

/**
 * @brief Stops the radio
 *
 * After a call to this function ISR will not be triggered, unless
 * MakeActionPacketPending() is called again.
 *
 * @retval true
 */
uint8_t BLE_LLD_StopActivity(void)
{
  return(sendCommand(BLE_LLD_STOPACTIVITY_CMDCODE, &bleparam_BLE_LLD_Packet));
}

/**
 * @brief Starts tone transmission on selected channel.
 *
 * This function is dedicated to tests and destroys context and multistate.
 * So, after calling this function the radio must be re-initialized.
 *
 * @param[in] RF_channel Radio frequency channel (between 0 and 39)
 * @param[in] PowerLevel Output power level (between 0 and 31)
 */
void BLE_LLD_StartTone(uint8_t RF_channel, uint8_t powerLevel)
{
  param_BLE_LLD_toneStart_t *params = (param_BLE_LLD_toneStart_t *)&bleparam_BLE_LLD_Packet;
  params->channel = RF_channel;
  params->power = powerLevel;
  sendCommand(BLE_LLD_STARTTONE_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * @brief Stops tone transmission.
 *
 * This function is dedicated to tests and destroys context and multistate.
 * So, after calling this function the radio must be re-initialized.
 */
void BLE_LLD_StopTone(void)
{
  sendCommand(BLE_LLD_STOPTONE_CMDCODE, &bleparam_BLE_LLD_Packet);
}

/**
 * Events processing.
 *
 * On radio events, M0 sends notifications.
 * Application can register callbacks to do some custom processing on
 * these events.
 * For the same event, custom processing can be executed in two different
 * context: in interruption context or after interruption (typicallyd in a task).
 */

/* --------------------- M0 REPLY PROCESSING IN INTERRUPT --------------------*/

static uint8_t APIndex(param_BLE_LLD_m0Reply_t *reply)
{
  return reply->txrxBuffer.rxBuffer[257];
}

static void itCbNop(void){}

/** Callbacks for the different events for processing during interruption */
static struct {
  void(*Stop)(void);
  void(*End)(void);
  void(*RxAck)(void);
  void(*RxOk)(void);
  void(*RxAckEnd)(void);
  void(*RxOkEnd)(void);
} interReplyCb = {itCbNop, itCbNop, itCbNop, itCbNop, itCbNop, itCbNop};

/**
 * @brief Registers a callback on an event for processing during interruption.
 *
 * @param[in] event Event on which to execute
 * @param[in] cb Callback
 */
void BLE_LLD_PRX_ReplyInterCbRegister(cmdFromM0_t event, void(*cb)(void)){
  switch (event) {
    case CMD_FROM_M0_RADIO_STOP:     interReplyCb.Stop = cb;     break;
    case CMD_FROM_M0_RADIO_END:      interReplyCb.End = cb;      break;
    case CMD_FROM_M0_RADIO_RXACK:    interReplyCb.RxAck = cb;    break;
    case CMD_FROM_M0_RADIO_RXOK:     interReplyCb.RxOk = cb;     break;
    case CMD_FROM_M0_RADIO_RXACKEND: interReplyCb.RxAckEnd = cb; break;
    case CMD_FROM_M0_RADIO_RXOKEND:  interReplyCb.RxOkEnd = cb;  break;
    default:                                                     break;
  }
}

/**
 * @brief Executes the registered callback for an event during interruption.
 *
 * This function also performs unpacking of received packets.
 *
 * @param[in] cmd Command string identifying the event
 *
 * @retval true if cmd has been recognized
 */
bool BLE_LLD_PRX_ReplyInterDispatch(const char *cmd)
{
  param_BLE_LLD_m0Reply_t *reply = (param_BLE_LLD_m0Reply_t *)&bleparam_BLE_LLD_Packet;
  actionPackets[APIndex(reply)]->status = reply->status;

  m0Cmd = CMD_FROM_M0_UNKNOWN;
  if (0 == strcmp(cmd, "Radio_Stop")) {
    m0Cmd = CMD_FROM_M0_RADIO_STOP;
    interReplyCb.Stop();
  } else if (0 == strcmp(cmd, "Radio_End")) {
    m0Cmd = CMD_FROM_M0_RADIO_END;
    interReplyCb.End();
  } else if (0 == strcmp(cmd, "Radio_RxAck")) {
    m0Cmd = CMD_FROM_M0_RADIO_RXACK;
    unpackRx(reply);
    interReplyCb.RxAck();
  } else if (0 == strcmp(cmd, "Radio_RxOk")) {
    m0Cmd = CMD_FROM_M0_RADIO_RXOK;
    unpackRx(reply);
    interReplyCb.RxOk();
  } else if (0 == strcmp(cmd, "Radio_RxAckEnd")) {
    m0Cmd = CMD_FROM_M0_RADIO_RXACKEND;
    unpackRx(reply);
    interReplyCb.RxAckEnd();
  } else if (0 == strcmp(cmd, "Radio_RxOkEnd")) {
    m0Cmd = CMD_FROM_M0_RADIO_RXOKEND;
    unpackRx(reply);
    interReplyCb.RxOkEnd();
  }
  return (CMD_FROM_M0_UNKNOWN != m0Cmd);
}

static void unpackRx(param_BLE_LLD_m0Reply_t *reply){
  actionPackets[APIndex(reply)]->timestamp_receive = reply->timestampReceive;
  actionPackets[APIndex(reply)]->rssi = reply->rssi;
  memcpy(actionPackets[APIndex(reply)]->data, reply->txrxBuffer.rxBuffer, TXRX_BUF_SIZE);
}

static uint8_t sendCommand(BLE_LLD_Code_t bleCmd, param_BLE_LLD_t *bleParams){
  // build command
  command.id = bleCmd;
  command.params = bleParams;

  // send it via callback
  CB_sendCmdM0(&command);

  return bleParams->returnValue;
}

/* ----------------------- M0 REPLY PROCESSING IN TASK -----------------------*/
static void tskCbNop(void){}

/** Callbacks for the different events for processing after interruption */
static struct {
  void(*Stop)(void);
  void(*End)(void);
  void(*RxAck)(void);
  void(*RxOk)(void);
  void(*RxAckEnd)(void);
  void(*RxOkEnd)(void);
} taskReplyCb = {tskCbNop, tskCbNop, tskCbNop, tskCbNop, tskCbNop, tskCbNop};


/**
 * @brief Registers a callback on an event for processing after interruption.
 *
 * @param[in] event Event on which to execute
 * @param[in] cb Callback
 */
void BLE_LLD_PRX_ReplyTaskCbRegister(cmdFromM0_t event, void(*cb)(void)){
  switch (event) {
    case CMD_FROM_M0_RADIO_STOP:     taskReplyCb.Stop = cb;     break;
    case CMD_FROM_M0_RADIO_END:      taskReplyCb.End = cb;      break;
    case CMD_FROM_M0_RADIO_RXACK:    taskReplyCb.RxAck = cb;    break;
    case CMD_FROM_M0_RADIO_RXOK:     taskReplyCb.RxOk = cb;     break;
    case CMD_FROM_M0_RADIO_RXACKEND: taskReplyCb.RxAckEnd = cb; break;
    case CMD_FROM_M0_RADIO_RXOKEND:  taskReplyCb.RxOkEnd = cb;  break;
    default:                                                    break;
  }
}

/**
 * @brief Executes the registered callback for an event after interruption.
 */
void BLE_LLD_PRX_ReplyTaskCbDispatch(void){
  param_BLE_LLD_m0Reply_t *reply = (param_BLE_LLD_m0Reply_t *)&bleparam_BLE_LLD_Packet;
  actionPackets[APIndex(reply)]->status = reply->status;

  switch (m0Cmd) {
    case CMD_FROM_M0_RADIO_STOP:     taskReplyCb.Stop();     break;
    case CMD_FROM_M0_RADIO_END:      taskReplyCb.End();      break;
    case CMD_FROM_M0_RADIO_RXACK:    taskReplyCb.RxAck();    break;
    case CMD_FROM_M0_RADIO_RXOK:     taskReplyCb.RxOk();     break;
    case CMD_FROM_M0_RADIO_RXACKEND: taskReplyCb.RxAckEnd(); break;
    case CMD_FROM_M0_RADIO_RXOKEND:  taskReplyCb.RxOkEnd();  break;
    default:                                                 break;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
