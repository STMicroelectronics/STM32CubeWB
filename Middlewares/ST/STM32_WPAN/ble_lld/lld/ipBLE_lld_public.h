/**
  ******************************************************************************
  * @file    ipBLE_lld_public.h
  * @author  MCD Application Team
  * @brief   Public structures and definitions for BLE radio low level driver
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

#ifndef IPBLE_LLD_PUBLIC_H
#define IPBLE_LLD_PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
/* ----------------------------- Includes ------------------------------------*/
/* ---------------------------------------------------------------------------*/


/* ---------------------------------------------------------------------------*/
/* ----------------------------- Defines -------------------------------------*/
/* ---------------------------------------------------------------------------*/

#define RADIO_PKT_BUF_SIZE     sizeof(ipBLE_lld_txrxdata_Type)

#define AES_KEY_SIZE                16
#define AES_BLOCK_SIZE              16
#define AES_IV_SIZE                 8
#define AES_BLOCK_SIZE              16

// MIC added at the end of user payload when encryption is enabled
#define MIC_SIZE                    4

#define CHAN_MAP_SIZE               5 // 37 bits

#define IPBLE_BLE_LLDANT_MAX_PAYLOAD_SIZE 255

#define ACTION_PACKET_NB            8

#define SUCCESS_0               0
#define INVALID_PARAMETER_C0    0xC0
#define WAKEUP_NOTSET_C2        0xC2
#define RADIO_BUSY_C4           0xC4
#define COMMAND_DISALLOWED      0xC5

#define RX_PHY_1MBPS 0x00
#define RX_PHY_2MBPS 0x10
#define TX_PHY_1MBPS 0x00
#define TX_PHY_2MBPS 0x01

#define RADIO_IS_READY(E) ((E == TX_OK_READY) || (E == TX_FAIL_READY) || \
(E == RX_OK_READY) || (E == RX_TIMEOUT_READY) || (E == RX_CRC_KO_READY) || \
(E == RX_FAIL_READY))

#define RADIO_IS_RX_OK(E) ((E == RX_OK_BUSY) || (E == RX_OK_READY))

/**
 * Packet to send/receive
 */
typedef struct {
  // interpreted by the hardware as BLE flags, so reserved in LLD
  uint8_t header;
  // size of actual payload
  uint8_t length;
  /* user data, when crypto is enabled 4 bytes starting at payload[length] are
     reserved for MIC */
  uint8_t payload[IPBLE_BLE_LLDANT_MAX_PAYLOAD_SIZE];
} ipBLE_lld_txrxdata_Type;

/* ---------------------------------------------------------------------------*/
/* ----------------------------- BLUENRG -------------------------------------*/
/* ---------------------------------------------------------------------------*/

#define BLUE_IDLE_0             0
#define BLUE_BUSY_NOWAKEUP_T1   1
#define BLUE_BUSY_NOWAKEUP_T2   2
#define BLUE_BUSY_WAKEUP        3
#define BLUE_BUSY_TONE          4
#define BLUE_TONE_DESTROY       5

/** @defgroup Radio_ActionTag_BitMask ActionTag BitMask
*
*/

/* This bit activates the radio frequency PLL calibration.
 * 0: Radio frequency calibration disabled.
 * 1: Radio frequency calibration enabled.
 */
#define PLL_TRIG                    0x01

/* This bit determines if the action is an RX action or a TX action.
 * 1: TX
 * 0: RX
 */
#define TXRX                        0x02

/* Selects the timer used to start radio operation (Rx or Tx)
 * 0: back-to-back timer
 *      - global timer, same value for all action packets
 *      - configured with BLE_LLD_SetBackToBackTime()
 *      - radio stays on while the timer is running
 *      - can be used for low values (down to ~150us)
 * 1: wakeup timer
 *      - local timer, each action packet can have a different value
 *      - configured with WakeupTime field in ActionPacket
 *      - radio sleeps while the timer is running
 *      - cannot be used for low values (minimum is ~700us)
 */

/* The bit determines if the action (RX or TX) is going to be executed based on the back-to-back time or based on the WakeupTime.
 * 0: Based on the back-to-back time (default 150 us).
 * 1: Based on the WakeupTime.
 */
#define TIMER_WAKEUP                0x04

/* This bit activates automatic NS Enable.
 * 0: Disable
 * 1: Enable
 */
#define NS_EN                       0x08

/* This bit activates automatic channel increment.
 * 0: No increment
 * 1: Automatic increment
 */
#define INC_CHAN                    0x10

/* This bit sets where the position of time stamp is taken, the beginning of the packet or the end of it. RX only.
 * 0: End of the Packet
 * 1: Beginning of the packet
 */
#define TIMESTAMP_POSITION          0x80

// Event sent by radio CPU to application CPU at the end of an action packet
// Because of filtering implementation, first event must be 1 and there must
// be at most 32 events
typedef enum
{
  TX_OK_BUSY=1,       // transmission succeeded, radio busy
  TX_OK_READY,        // transmission succeeded, radio ready
  TX_FAIL_BUSY,       // transmission failed, radio busy
  TX_FAIL_READY,      // transmission failed, radio ready
  RX_OK_BUSY,         // reception succeeded, radio busy
  RX_OK_READY,        // reception succeeded, radio ready
  RX_TIMEOUT_BUSY,    // reception timed out, radio busy
  RX_TIMEOUT_READY,   // reception timed out, radio ready
  RX_CRC_KO_BUSY,     // reception failed CRC, radio busy
  RX_CRC_KO_READY,    // reception failed CRC, radio ready
  RX_FAIL_BUSY,       // reception failed for another reason, radio busy
  RX_FAIL_READY,      // reception failed for another reason, radio ready
} radioEventType;

typedef enum {
  STATE_MACHINE_0 = 0,
  STATE_MACHINE_1,
  STATE_MACHINE_2,
  STATE_MACHINE_3,
  STATE_MACHINE_4,
  STATE_MACHINE_5,
  STATE_MACHINE_6,
  STATE_MACHINE_7,
  STATE_MACHINE_NB,
} StateMachine_t;

/* Action packets available on M0 */
typedef enum
{
  APACKET_0 = 0,
  APACKET_1,
  APACKET_2,
  APACKET_3,
  APACKET_4,
  APACKET_5,
  APACKET_6,
  APACKET_7,
  APACKET_STOP=0xFF, // to be placed at the end
} ActionPacket_Nb;

/* Transmit power settings (TX_POW_MIN_20_85 means -20.85dB) */
typedef enum{
  TX_POW_MIN_40_DB = 0,
  TX_POW_MIN_20_85_DB,
  TX_POW_MIN_19_85_DB,
  TX_POW_MIN_18_85_DB,
  TX_POW_MIN_17_6_DB,
  TX_POW_MIN_16_5_DB,
  TX_POW_MIN_15_25_DB,
  TX_POW_MIN_14_1_DB,
  TX_POW_MIN_13_15_DB,
  TX_POW_MIN_12_05_DB,
  TX_POW_MIN_10_9_DB,
  TX_POW_MIN_9_9_DB,
  TX_POW_MIN_8_85_DB,
  TX_POW_MIN_7_8_DB,
  TX_POW_MIN_6_9_DB,
  TX_POW_MIN_5_9_DB,
  TX_POW_MIN_4_95_DB,
  TX_POW_MIN_4_DB,
  TX_POW_MIN_3_15_DB,
  TX_POW_MIN_2_45_DB,
  TX_POW_MIN_1_8_DB,
  TX_POW_MIN_1_3_DB,
  TX_POW_MIN_0_85_DB,
  TX_POW_MIN_0_5_DB,
  TX_POW_MIN_0_15_DB,
  TX_POW_PLUS_0_DB,
  TX_POW_PLUS_1_DB,
  TX_POW_PLUS_2_DB,
  TX_POW_PLUS_3_DB,
  TX_POW_PLUS_4_DB,
  TX_POW_PLUS_5_DB,
  TX_POW_PLUS_6_DB
} txPower_t;

#ifdef __cplusplus
}
#endif

#endif /* IPBLE_LLD_PUBLIC_H */
