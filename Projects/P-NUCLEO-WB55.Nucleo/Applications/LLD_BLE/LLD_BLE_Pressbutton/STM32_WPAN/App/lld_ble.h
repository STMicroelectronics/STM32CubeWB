/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_lld_ble.h
  * Description        : Header for LLD BLE application.
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
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LLD_BLE_H
#define LLD_BLE_H

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
/* ------------------------------- LLD BLE -----------------------------------*/
/* ---------------------------------------------------------------------------*/
#define IRQ_RCV_OK              (1UL<<31)       /* The packet is received, and the CRC is valid. */
#define BIT_TX_MODE             (1UL<<18)       /* The packet has been sent successfully. */

#define HS_STARTUP_TIME         (uint16_t)(0x001A)  /* High Speed start up time 64 us */ 
#define BLE_HOT_ANA_CONFIG_TABLE_LENGTH         60 /* LLD TESTING */

#define RX_PHY_1MBPS 0x00
#define RX_PHY_2MBPS 0x10
#define TX_PHY_1MBPS 0x00
#define TX_PHY_2MBPS 0x01
#define BACK2BACK_TIME 50

#define PLL_TRIG                    0x01
#define TXRX                        0x02
#define TIMER_WAKEUP                0x04
#define NS_EN                       0x08
#define INC_CHAN                    0x10
#define RELATIVE                    0x20
#define TIMESTAMP_POSITION          0x80 

typedef enum {
  STATE_MACHINE_0 = 0,
  STATE_MACHINE_1,
  STATE_MACHINE_2,
  STATE_MACHINE_3,
  STATE_MACHINE_4,
  STATE_MACHINE_5,
  STATE_MACHINE_6,
  STATE_MACHINE_7,
} StateMachine_t; 

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

/* dataRoutine and condRoutine struct */
typedef struct {
  condCase_t  condCase ; /* __I */
  dataCase_t  dataCase ; /* __I */
} setCase_t;

/* Structure for Buffer */
typedef struct {
  uint8_t txBuffer[260]; /* __I */ /*txBuffer + Encrypt*/
  uint8_t rxBuffer[260]; /* __O */ /*rxBuffer + radioPacketNb=rxBuffer[257] */
} buffer_hal_BLE_t;

/* Parameter HAL LLD BLE Data Structure */
typedef struct {
  uint32_t WakeupTime;          /* __I  status_32[0]*/ /*WakeupTime + back2back + crc_init + hs_startup_time*/
  uint32_t ReceiveWindowLength; /* __I  status_32[1]*/ /*ReceiveWindowLength*/
  uint32_t InfoTime;            /* __I  status_32[2]*/ /*InfoTime + NetworkID + global_PacketNumber */
  uint32_t status;              /* __O  status_32[3]*/ /*status*/
  uint32_t timestamp_receive;   /* __O  status_32[4]*/ /*timestamp_receive*/
  uint32_t rssi;                /* __O  status_32[5]*/ /*rssi*/
  uint8_t return_value;         /* __IO */ /*return value*/
  uint8_t channel_tx;           /* __I  status_8[0]*/ /*tx + flagTx  + channel   + tx_phy + next_true + RF_channel */
  uint8_t channel_rx;           /* __I  status_8[1]*/ /*rx + flagRcv + increment + rx_phy + next_false */
  uint8_t StateMachineNo;       /* __I  status_8[2]*/ /*StateMachineNo + powerLevel + global_PacketStopRx*/
  uint8_t ActionTag;            /* __I  status_8[3]*/ /*ActionTag +  low_speed_osc*/
  uint8_t actionPacketNb;       /* __I  status_8[4]*/ /*actionPacketNb + whitening*/
  setCase_t setCase;            /* __I  status_8[5] status_8[6]*/ /*condRoutine , dataRoutine*/
  buffer_hal_BLE_t txrxBuffer; /*data*/
} param_hal_BLE_t;

typedef struct {
  uint8_t StateMachineNo ;      /* This parameter indicates the state machine number for this action. From 0 to 7. */
  uint8_t ActionTag;            /* The configuration of the current action. 
                                 * Action Tag: PLL_TRIG, TXRX, TIMER_WAKEUP, INC_CHAN, TIMESTAMP_POSITION, RELATIVE */
  uint32_t WakeupTime;          /* Contains the wakeup time in microsecond if it is relative.
                                 * It should not be more than 24 bits if it is absolute. 
                                 * It only applies if TIMER_WAKEUP flag is set in ActionTag. */
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
    APACKET_NULL=0xFF, // to be place at the end
} ActionPacket_Nb;

/* Enum for Cmd Code */
typedef enum
  {
    /* hal_BLE.c part */
    HAL_BLE_UNUSED_CMDCODE = 0, 
    HAL_BLE_INIT_CMDCODE ,
    HAL_BLE_SETNETWORKID_CMDCODE,
    HAL_BLE_SENDPACKET_CMDCODE,
    HAL_BLE_SENDPACKETWITHACK_CMDCODE,
    HAL_BLE_RECEIVEPACKET_CMDCODE,
    HAL_BLE_RECEIVEPACKETWITHACK_CMDCODE,
    /* ipBLE_lld.c part */
    LLD_BLE_GETSTATUS_CMDCODE,
    LLD_BLE_SETCHANNELMAP_CMDCODE,
    LLD_BLE_SETCHANNEL_CMDCODE,
    LLD_BLE_SETTXATTRIBUTES_CMDCODE,
    LLD_BLE_SETBACKTOBACKTIME_CMDCODE,
    LLD_BLE_SETTXPOWER_CMDCODE,
    LLD_BLE_SETTX_RX_PHY_CMDCODE,
    LLD_BLE_STOPACTIVITY_CMDCODE,
    LLD_BLE_SETENCRYPTIONCOUNT_CMDCODE,
    LLD_BLE_SETENCRYPTIONATTRIBUTES_CMDCODE,
    LLD_BLE_SETENCRYPTFLAGS_CMDCODE,
    LLD_BLE_ENCRYPTPLAINDATA_CMDCODE,
    LLD_BLE_STARTTONE_CMDCODE,
    LLD_BLE_STOPTONE_CMDCODE,
    LLD_BLE_SETRESERVEDAREA_CMDCODE, 
    LLD_BLE_MAKEACTIONPACKETPENDING_CMDCODE,
    LLD_BLE_INIT_CMDCODE,
} hal_BLE_Code_t;

uint8_t HAL_BLE_Init(void);
uint8_t HAL_BLE_SetNetworkID(uint32_t ID);
uint8_t HAL_BLE_SendPacket(uint8_t channel, uint32_t wakeup_time, uint8_t* txBuffer, dataCase_t  dataCaseTx);
uint8_t HAL_BLE_SendPacketWithAck(uint8_t channel, uint32_t wakeup_time, uint8_t* txBuffer, uint8_t* rxBuffer, uint32_t receive_timeout, dataCase_t  dataCaseTxAck );
uint8_t HAL_BLE_ReceivePacket(uint8_t channel, uint32_t wakeup_time, uint8_t* rxBuffer, uint32_t receive_timeout, dataCase_t  dataCaseTx  );
uint8_t HAL_BLE_ReceivePacketWithAck(uint8_t channel, uint32_t wakeup_time, uint8_t* rxBuffer, uint8_t* txBuffer, uint32_t receive_timeout, dataCase_t  dataCaseTxAck );

uint8_t LLD_BLE_GetStatus(uint32_t *time);
void    LLD_BLE_SetChannelMap(uint8_t StateMachineNo,uint8_t *chan_remap);
void    LLD_BLE_SetChannel(uint8_t StateMachineNo, uint8_t channel,uint8_t channel_increment); 
void    LLD_BLE_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID, uint32_t crc_init, uint32_t sca);
void    LLD_BLE_SetBackToBackTime(uint32_t back_to_back_time);  
void    LLD_BLE_SetTxPower(uint8_t powerLevel);
void    LLD_BLE_SetTx_Rx_Phy(uint8_t StateMachineNo, uint8_t tx_phy, uint8_t rx_phy);
uint8_t LLD_BLE_StopActivity(void);
void    LLD_BLE_SetEncryptionCount(uint8_t StateMachineNo, uint8_t *count_tx, uint8_t *count_rcv);    
void    LLD_BLE_SetEncryptionAttributes(uint8_t StateMachineNo, uint8_t *enc_iv, uint8_t *enc_key); 
void    LLD_BLE_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlagTx, FunctionalState EncryptFlagRcv);
void    LLD_BLE_EncryptPlainData(uint8_t *Key, uint8_t *plainData, uint8_t *cypherData);
void    LLD_BLE_StartTone(uint8_t RF_channel, uint8_t powerLevel);
void    LLD_BLE_StopTone(void);

//void    LLD_BLE_CrystalCheck(void);
void    LLD_BLE_SetReservedArea(ActionPacket *p); 
uint8_t LLD_BLE_MakeActionPacketPending(ActionPacket *p);
void    LLD_BLE_Init(uint16_t hs_startup_time, uint8_t low_speed_osc, uint32_t* hot_table, FunctionalState whitening);
//void    LLD_BLE_IRQHandler(void);


/* only for M4 */
void   APP_LLD_BLE_SetdataRoutineOption(uint32_t PacketNumber, uint8_t  PacketStop );

#endif /* LLD_BLE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
