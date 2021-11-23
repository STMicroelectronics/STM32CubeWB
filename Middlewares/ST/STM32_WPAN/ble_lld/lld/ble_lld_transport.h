/**
  ******************************************************************************
  * @file    ble_lld_transport.h
  * @author  MCD Application Team
  * @brief   Definitions of command id and structures for transport
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2012 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef BLE_LLD_TRANSPORT_H
#define BLE_LLD_TRANSPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32_wpan_common.h"
#include "ipBLE_lld_public.h"

/* Commands to radio MCU */
typedef enum
{
  BLE_LLD_UNUSED_CMDCODE = 0,
  BLE_LLD_GETSTATUS_CMDCODE,
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

/* Responses from radio MCU */
typedef enum
{
  BLE_LLD_RSP_END,
} BLE_LLD_rsp_t;

/** Parameters for command BLE_LLD_SETENCRYPTIONCOUNT_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t stateMachineNo;
  uint8_t countTx[5];
  uint8_t countRcv[5];
} param_BLE_LLD_cryptCount_t;

/** Parameters for command BLE_LLD_SETENCRYPTIONATTRIBUTES_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t stateMachineNo;
  uint8_t encIv[AES_IV_SIZE];
  uint8_t encKey[AES_KEY_SIZE];
} param_BLE_LLD_cryptAttr_t;

/** Parameters for command BLE_LLD_SETENCRYPTFLAGS_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t encFlag;
  uint8_t StateMachineNo;
} param_BLE_LLD_cryptFlags_t;

/** Parameters for command BLE_LLD_ENCRYPTPLAINDATA_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t key[AES_KEY_SIZE];
  uint8_t plainData[AES_BLOCK_SIZE];
  uint8_t cypherData[AES_BLOCK_SIZE];
} param_BLE_LLD_crypt_t;

/** Parameters for command BLE_LLD_STARTTONE_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t channel;
  uint8_t power;
} param_BLE_LLD_toneStart_t;

/** Parameters for command BLE_LLD_SETCHANNEL_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t channel;
  uint8_t stateMachineNo;
} param_BLE_LLD_chan_t;

/** Parameters for command BLE_LLD_SETTXATTRIBUTES_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t networkId;
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
  uint8_t power;
} param_BLE_LLD_power_t;

/** Parameters for command BLE_LLD_SETTX_RX_PHY_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t txPhy;
  uint8_t rxPhy;
  uint8_t stateMachineNo;
} param_BLE_LLD_phy_t;

/** Parameters for command BLE_LLD_SETRESERVEDAREA_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t wakeupTime;
  uint32_t receiveWindowLength;
  uint8_t nextTrue;
  uint8_t nextFalse;
  uint8_t stateMachineNo;
  uint8_t actionTag;
  uint8_t actionPacketNb;
  ipBLE_lld_txrxdata_Type *buffer; // Rx or Tx buffer in shared memory
} param_BLE_LLD_reserved_t;

/** Parameters for command BLE_LLD_MAKEACTIONPACKETPENDING_CMDCODE */
typedef PACKED_STRUCT
{
  uint8_t actionPacketNb;
} param_BLE_LLD_mkPending_t;

/** Parameters for command BLE_LLD_INIT_CMDCODE */
typedef PACKED_STRUCT
{
  uint32_t startupTime;
  uint8_t lowSpeedOsc;
  uint8_t whitening;
} param_BLE_LLD_init_t;

/** Parameters for replies */
typedef PACKED_STRUCT
{
  uint32_t status;
  uint32_t timestampReceive;
  uint32_t rssi;
  uint8_t actionPacketNb;
} param_BLE_LLD_m0Reply_t;

/** Parameters for commands and response */
typedef union
{
  param_BLE_LLD_cryptCount_t cryptCount;
  param_BLE_LLD_cryptAttr_t cryptAttr;
  param_BLE_LLD_cryptFlags_t cryptFlags;
  param_BLE_LLD_crypt_t encrypt;
  param_BLE_LLD_toneStart_t toneStart;
  param_BLE_LLD_chan_t chan;
  param_BLE_LLD_txAttr_t txAttr;
  param_BLE_LLD_b2b_t b2b;
  param_BLE_LLD_power_t power;
  param_BLE_LLD_phy_t phy;
  param_BLE_LLD_reserved_t reserved;
  param_BLE_LLD_mkPending_t pending;
  param_BLE_LLD_init_t init;
  param_BLE_LLD_m0Reply_t reply;
} __attribute__((packed)) param_BLE_LLD_t;

typedef PACKED_STRUCT
{
  uint8_t returnValue;
  param_BLE_LLD_t params;
} msg_BLE_LLD_t;

// Used to pack message pointer into generic payload
typedef PACKED_STRUCT
{
  msg_BLE_LLD_t *msg;
} payload_BLE_LLD_t;

#ifdef __cplusplus
}
#endif

#endif /* BLE_LLD_TRANSPORT_H */
