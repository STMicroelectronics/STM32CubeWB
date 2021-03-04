/**
 ******************************************************************************
  * File Name          : app_ble_lld_private.h
  * Description        : Header for BLE LLD communication with M0.
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
#ifndef BLE_LLD_PRIVATE_H
#define BLE_LLD_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/* ---------------------------------------------------------------------------*/
/* ------------------------------- BLE LLD -----------------------------------*/
/* ---------------------------------------------------------------------------*/

/* max payload length(255) + header (2) (257 bytes) aligned on 32-bit (65 word = 260 bytes) */
#define RADIO_PKT_RX_BUF_SIZE       260 
#define RADIO_PKT_TX_BUF_SIZE       260 

/* Commands to M0 */
typedef enum
{
  HAL_BLE_LLD_UNUSED_CMDCODE = 0,
  HAL_BLE_LLD_INIT_CMDCODE ,
  HAL_BLE_LLD_SETNETWORKID_CMDCODE,
  HAL_BLE_LLD_SENDPACKET_CMDCODE,
  HAL_BLE_LLD_SENDPACKETWITHACK_CMDCODE,
  HAL_BLE_LLD_RECEIVEPACKET_CMDCODE,
  HAL_BLE_LLD_RECEIVEPACKETWITHACK_CMDCODE,
  BLE_LLD_GETSTATUS_CMDCODE,
  BLE_LLD_SETCHANNELMAP_CMDCODE,
  BLE_LLD_SETCHANNEL_CMDCODE,
  BLE_LLD_SETTXATTRIBUTES_CMDCODE,
  BLE_LLD_SETBACKTOBACKTIME_CMDCODE,
  BLE_LLD_SETTXPOWER_CMDCODE,
  BLE_LLD_SETTX_RX_PHY_CMDCODE,
  BLE_LLD_STOPACTIVITY_CMDCODE,
  BLE_LLD_SETENCRYPTIONCOUNT_CMDCODE,
  BLE_LLD_SETENCRYPTIONATTRIBUTES_CMDCODE,
  BLE_LLD_SETENCRYPTFLAGS_CMDCODE,
  BLE_LLD_ENCRYPTPLAINDATA_CMDCODE,
  BLE_LLD_STARTTONE_CMDCODE,
  BLE_LLD_STOPTONE_CMDCODE,
  BLE_LLD_SETRESERVEDAREA_CMDCODE,
  BLE_LLD_MAKEACTIONPACKETPENDING_CMDCODE,
  BLE_LLD_INIT_CMDCODE,
} BLE_LLD_Code_t;

/* dataRoutine and condRoutine struct */
typedef PACKED_STRUCT
{
  uint8_t condCase;
  uint8_t dataCase;
} setCase_t;

/* Structure for Buffer */
typedef PACKED_STRUCT
{
  uint8_t txBuffer[RADIO_PKT_TX_BUF_SIZE]; /* __I */ /*txBuffer + Encrypt*/
  uint8_t rxBuffer[RADIO_PKT_RX_BUF_SIZE]; /* __O */ /*rxBuffer + radioPacketNb=rxBuffer[257] */
} buffer_BLE_LLD_t;

/* Generic structure for messages to M0, mostly used as a size reference.
   Only returnValue is used.
   A specialized structure (param_hal_BLE_xxx_t or param_BLE_LLD_xxx_t) should
   be used for actual messages. */
typedef PACKED_STRUCT
{
  uint8_t padding1[24];
  uint8_t returnValue;
  uint8_t padding2[5];
  setCase_t padding3;
  buffer_BLE_LLD_t padding4;
} param_BLE_LLD_t;

/** Parameters for command HAL_BLE_SETNETWORKID_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t padding1[2];
  uint32_t id;
} param_BLE_LLD_network_t;

/** Parameters for commands HAL_BLE_SENDPACKET_CMDCODE and
 *  HAL_BLE_LLD_SENDPACKETWITHACK_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t wakeupTime;
  uint32_t receiveTimeout;
  uint32_t packetNumber;
  uint8_t padding1[13];
  uint8_t channel;
  uint8_t padding2;
  uint8_t packetStopRx;
  uint8_t padding3[2];
  setCase_t setCase;
  buffer_BLE_LLD_t txrxBuffer;
} param_BLE_LLD_send_t;

/** Parameters for commands HAL_BLE_RECEIVEPACKET_CMDCODE and
 * HAL_BLE_LLD_RECEIVEPACKETWITHACK_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t wakeupTime;
  uint32_t receiveTimeout;
  uint32_t packetNumber;
  uint8_t padding1[14];
  uint8_t channel;
  uint8_t packetStopRx;
  uint8_t padding2[2];
  setCase_t setCase;
  buffer_BLE_LLD_t txrxBuffer;
} param_BLE_LLD_receive_t;

/** Parameters for command BLE_LLD_GETSTATUS_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t padding1[2];
  uint32_t time;
} param_BLE_LLD_status_t;

/** Parameters for command BLE_LLD_SETENCRYPTIONCOUNT_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[27];
  uint8_t stateMachineNo;
  uint8_t padding2[2];
  setCase_t padding3;
  uint8_t countTx[5];
  uint8_t countRcv[5];
} param_BLE_LLD_cryptCount_t;

/** Parameters for command BLE_LLD_SETENCRYPTIONATTRIBUTES_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[27];
  uint8_t stateMachineNo;
  uint8_t padding2[2];
  setCase_t padding3;
  uint8_t encIv[AES_IV_SIZE];
  uint8_t encKey[AES_KEY_SIZE];
} param_BLE_LLD_cryptAttr_t;

/** Parameters for command BLE_LLD_SETENCRYPTFLAGS_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[25];
  uint8_t encFlagTx;
  uint8_t encFlagRx;
  uint8_t StateMachineNo;
} param_BLE_LLD_cryptFlags_t;

/** Parameters for command BLE_LLD_ENCRYPTPLAINDATA_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[30];
  setCase_t padding2;
  uint8_t key[AES_KEY_SIZE];
  uint8_t padding3[112];
  uint8_t plainData[AES_BLOCK_SIZE];
  uint8_t padding[116];
  uint8_t cypherData[AES_BLOCK_SIZE];
} param_BLE_LLD_crypt_t;

/** Parameters for command BLE_LLD_STARTTONE_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[25];
  uint8_t channel;
  uint8_t padding2;
  uint8_t power;
} param_BLE_LLD_toneStart_t;

/** Parameters for command BLE_LLD_SETCHANNELMAP_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[27];
  uint8_t stateMachineNo;
  uint8_t padding2[2];
  setCase_t padding3;
  uint8_t map[CHAN_MAP_SIZE];
} param_BLE_LLD_chanMap_t;

/** Parameters for command BLE_LLD_SETCHANNEL_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[25];
  uint8_t channel;
  uint8_t channelInc;
  uint8_t stateMachineNo;
} param_BLE_LLD_chan_t;

/** Parameters for command BLE_LLD_SETTXATTRIBUTES_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t crcInit;
  uint32_t sca;
  uint32_t networkId;
  uint8_t padding1[15];
  uint8_t stateMachineNo;
} param_BLE_LLD_txAttr_t;

/** Parameters for command BLE_LLD_SETBACKTOBACKTIME_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t backToBackTime;
} param_BLE_LLD_b2b_t;

/** Parameters for command BLE_LLD_SETTXPOWER_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[27];
  uint8_t power;
} param_BLE_LLD_power_t;

/** Parameters for command BLE_LLD_SETTX_RX_PHY_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t padding1[25];
  uint8_t txPhy;
  uint8_t rxPhy;
  uint8_t stateMachineNo;
} param_BLE_LLD_phy_t;

/** Parameters for command BLE_LLD_SETRESERVEDAREA_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t wakeupTime;
  uint32_t receiveWindowLength;
  uint8_t padding1[17];
  uint8_t nextTrue;
  uint8_t nextFalse;
  uint8_t stateMachineNo;
  uint8_t actionTag;
  uint8_t actionPacketNb;
  setCase_t setCase;
  buffer_BLE_LLD_t txrxBuffer;
} param_BLE_LLD_reserved_t;

/** Parameters for command BLE_LLD_MAKEACTIONPACKETPENDING_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t padding1[2];
  uint32_t packetNumber;
  uint8_t padding2[15];
  uint8_t packetStopRx;
  uint8_t padding3;
  uint8_t actionPacketNb;
} param_BLE_LLD_mkPending_t;

/** Parameters for command BLE_LLD_INIT_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t startupTime;
  uint8_t padding1[24];
  uint8_t lowSpeedOsc;
  uint8_t whitening;
  setCase_t padding2;
  buffer_BLE_LLD_t txrxBuffer;
} param_BLE_LLD_init_t;

/** Parameters for replies */
typedef PACKED_STRUCT
{
  uint8_t padding1[12];
  uint32_t status;
  uint32_t timestampReceive;
  uint32_t rssi;
  uint8_t padding2[6];
  setCase_t padding3;
  buffer_BLE_LLD_t txrxBuffer;
} param_BLE_LLD_m0Reply_t;

/** Command to M0 */
typedef PACKED_STRUCT
{
  uint8_t id;
  param_BLE_LLD_t *params;
} bleCmd_t;

#endif /* BLE_LLD_PRIVATE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
