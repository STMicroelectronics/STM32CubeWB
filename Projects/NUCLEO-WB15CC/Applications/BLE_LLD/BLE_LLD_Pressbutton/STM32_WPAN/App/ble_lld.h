/**
 ******************************************************************************
  * File Name          : app_ble_lld.h
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
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/* ---------------------------------------------------------------------------*/
/* ------------------------------- BLE LLD -----------------------------------*/
/* ---------------------------------------------------------------------------*/
#define IRQ_RCV_OK              (1UL<<31)       /* The packet is received, and the CRC is valid. */
#define BIT_TX_MODE             (1UL<<18)       /* The packet has been sent successfully. */

#define HS_STARTUP_TIME         (uint16_t)(0x0099)  /* High Speed start up for LP */

#define BLE_HOT_ANA_CONFIG_TABLE_LENGTH         60

#define ACTION_PACKET_NB            8
#define TXRX_BUF_SIZE               258
#define AES_KEY_SIZE                16
#define AES_BLOCK_SIZE              16
#define AES_IV_SIZE                 8
#define CHAN_MAP_SIZE               5 // 37 bits

/* Radio Status */
#define SUCCESS_0               0
#define INVALID_PARAMETER_C0    0xC0
#define RADIO_BUSY_C4           0xC4
  
/* BLE Status */
#define BLUE_IDLE_0             0  
#define BLUE_BUSY_NOWAKEUP_T2   2 
#define BLUE_BUSY_WAKEUP        3
#define BLUE_BUSY_TONE          4
#define BLUE_TONE_DESTROY       5

#define RX_PHY_1MBPS                0x00
#define RX_PHY_2MBPS                0x10
#define TX_PHY_1MBPS                0x00
#define TX_PHY_2MBPS                0x01
#define BACK2BACK_TIME 50

/* Action tag bitfield */
#define AT_PLL_TRIG                 0x01
#define AT_TXRX                     0x02
#define AT_TIMER_WAKEUP             0x04
#define AT_NS_EN                    0x08
#define AT_INC_CHAN                 0x10
#define AT_RELATIVE                 0x20
#define AT_TIMESTAMP_POSITION       0x80

/* Events from M0 */
typedef enum
{
  CMD_FROM_M0_RADIO_STOP         = 12,
  CMD_FROM_M0_RADIO_END          = 13,
  CMD_FROM_M0_RADIO_RXACK        = 14,
  CMD_FROM_M0_RADIO_RXOK         = 15,
  CMD_FROM_M0_RADIO_RXACKEND     = 16,
  CMD_FROM_M0_RADIO_RXOKEND      = 17,
  CMD_FROM_M0_UNKNOWN            = 255,
} cmdFromM0_t;

/* Finite state machines available on M0 */
typedef enum
{
  STATE_MACHINE_0 = 0,
  STATE_MACHINE_1,
  STATE_MACHINE_2,
  STATE_MACHINE_3,
  STATE_MACHINE_4,
  STATE_MACHINE_5,
  STATE_MACHINE_6,
  STATE_MACHINE_7,
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
  APACKET_NULL=0xFF, // to be placed at the end
} ActionPacket_Nb;

/* condRoutine enum for condRoutine() */
typedef enum
{
  condCase_Done = 0,
  condCase_Tx,
  condCase_Rx,
  condCase_Stop,
} condCase_t;

/* dataRoutine enum for dataRoutine() */
typedef enum
{
  dataCase_Custom0 = 0,
  dataCase_Custom1,
  dataCase_Custom2,
  dataCase_Custom3,
  dataCase_Custom4,
  dataCase_Custom5,
  dataCase_Custom6,
  dataCase_Custom7,
} dataCase_t;

/* Data routines definitions to use after a call to HAL_BLE_Init() */
#define DATA_ROUT_HAL_DONE            dataCase_Custom0
#define DATA_ROUT_HAL_TX              dataCase_Custom1
#define DATA_ROUT_HAL_RX              dataCase_Custom2
#define DATA_ROUT_HAL_STOP            dataCase_Custom3
#define DATA_ROUT_HAL_TXMULTI         dataCase_Custom4
#define DATA_ROUT_HAL_TXMULTIACK      dataCase_Custom5
#define DATA_ROUT_HAL_RXMULTI         dataCase_Custom6
#define DATA_ROUT_HAL_RXMULTIACK      dataCase_Custom7

/* Data routines definitions to use after a call to BLE_LLD_Init() */
#define DATA_ROUT_LL_DONE             dataCase_Custom0
#define DATA_ROUT_LL_TX               dataCase_Custom1
#define DATA_ROUT_LL_RX               dataCase_Custom2
#define DATA_ROUT_LL_STOP             dataCase_Custom3
#define DATA_ROUT_LL_TXMULTIACK       dataCase_Custom4
#define DATA_ROUT_LL_RXMULTIACK       dataCase_Custom5
#define DATA_ROUT_LL_ACTION           dataCase_Custom6

typedef struct {
  uint8_t StateMachineNo ;      /* This parameter indicates the state machine number for this action. From 0 to 7. */
  uint8_t ActionTag;            /* The configuration of the current action. 
                                 * Action Tag: AT_PLL_TRIG, AT_TXRX, AT_TIMER_WAKEUP, AT_INC_CHAN, AT_TIMESTAMP_POSITION, AT_RELATIVE */
  uint32_t WakeupTime;          /* Contains the wakeup time in microsecond if it is relative.
                                 * It should not be more than 24 bits if it is absolute. 
                                 * It only applies if AT_TIMER_WAKEUP flag is set in ActionTag. */
  uint32_t ReceiveWindowLength; /* Sets RX window size in microsecond. Applicable only for RX actions. */
  uint8_t *data;                /* Pointer to the array with the data to send (header, length and data field), for TX.
                                 * Pointer to the array where the data received are copied, for RX.
                                 * In case of RX, the array must have the max size MAX_PACKET_LENGTH. */
  uint32_t status;              /* The Status Register with the information on the action. */
  uint32_t timestamp_receive;   /* This field contains the timestamp when a packet is received.
                                 * Intended to be used in the dataRoutine() callback routine. RX only. */
  int32_t rssi;                 /* The rssi of the packet was received with. RX only. */
  uint8_t next_true;            /* Pointer to next ActionPacket if condRoutine() returns TRUE */
  uint8_t next_false;           /* Pointer to next ActionPacket if condRoutine() returns FALSE */
  uint8_t condRoutine;          /* User callback that decide the next ActionPacket to use.
                                 * It is time critical. Routine must end within 45 us. */
  uint8_t dataRoutine;          /* User callback for managing data. */
  uint8_t actionPacketNb;       /* User callback for managing data. */
} ActionPacket;

/* Exported functions ------------------------------------------------------- */
void    BLE_LLD_PRX_Init(void (*callbackSendCmdM0)(void *));
void    BLE_LLD_PRX_ReplyInterCbRegister(cmdFromM0_t event, void(*cb)(void));
bool    BLE_LLD_PRX_ReplyInterDispatch(const char *cmd);
void    BLE_LLD_PRX_ReplyTaskCbDispatch(void);
void    BLE_LLD_PRX_ReplyTaskCbRegister(cmdFromM0_t event, void(*cb)(void));
void    BLE_LLD_PRX_SetdataRoutineMultiOptions(uint32_t PacketNumber, uint8_t  PacketStop );

uint8_t BLE_LLD_GetStatus(uint32_t *time);
void    BLE_LLD_SetChannelMap(uint8_t StateMachineNo, uint8_t (*chan_remap)[5]);
void    BLE_LLD_SetChannel(uint8_t StateMachineNo, uint8_t channel,uint8_t channel_increment); 
void    BLE_LLD_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID, uint32_t crc_init, uint32_t sca);
void    BLE_LLD_SetBackToBackTime(uint32_t back_to_back_time);  
void    BLE_LLD_SetTxPower(uint8_t powerLevel);
void    BLE_LLD_SetTx_Rx_Phy(uint8_t StateMachineNo, uint8_t tx_phy, uint8_t rx_phy);
uint8_t BLE_LLD_StopActivity(void);
void    BLE_LLD_SetEncryptionCount(uint8_t StateMachineNo, uint8_t (*count_tx)[5], uint8_t (*count_rcv)[5]);
void    BLE_LLD_SetEncryptionAttributes(uint8_t StateMachineNo, uint8_t (*enc_iv)[8], uint8_t (*enc_key)[16]);
void    BLE_LLD_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlagTx, FunctionalState EncryptFlagRcv);
void    BLE_LLD_EncryptPlainData(uint8_t (*Key)[16], uint8_t (*plainData)[16], uint8_t (*cypherData)[16]);
void    BLE_LLD_StartTone(uint8_t RF_channel, uint8_t powerLevel);
void    BLE_LLD_StopTone(void);
void    BLE_LLD_SetReservedArea(ActionPacket *p); 
uint8_t BLE_LLD_MakeActionPacketPending(ActionPacket *p);
void    BLE_LLD_Init(uint16_t hs_startup_time, uint8_t low_speed_osc, uint32_t* hot_table, FunctionalState whitening);

#endif /* BLE_LLD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
