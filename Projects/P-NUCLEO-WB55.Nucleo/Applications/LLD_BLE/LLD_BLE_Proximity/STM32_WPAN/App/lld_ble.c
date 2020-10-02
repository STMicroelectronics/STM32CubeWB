/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : lld_ble.c
  * Description        : LLD BLE Application.
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

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "utilities_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "lld_ble.h"
#include "app_lld_ble.h"
#include "tl.h"
#include "shci.h"
#include "stm_logging.h"
#include "stm32_lpm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private variables -----------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t  *txBuffer_Ptr;
extern uint8_t  *rxBuffer_Ptr[8];
extern uint32_t *rxStatus_Ptr[8];
extern uint32_t *rxTimeStamp_Ptr[8];
extern int      *rxRSSI_Ptr[8];
/* for HAL */
uint32_t hal_status;
uint32_t hal_timestamp_receive;
int hal_rssi;

extern param_hal_BLE_t bleparam_hal_BLE_Packet;

uint32_t*  param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

static uint32_t global_PacketNumber = 1; // NB of Successif PACKET Send or Receive
static uint8_t  global_PacketStopRx = 1; // Stop after RX
/* USER CODE END PV */

/* --------------------------- dataRoutine LEVEL -------------------------------*/
void APP_LLD_BLE_SetdataRoutineOption(uint32_t PacketNumber, uint8_t  PacketStop )
{
  global_PacketNumber = PacketNumber; // NB of Successif PACKET using LL Send or Receive
  global_PacketStopRx = PacketStop; // Stop after RX
}

/* ------------------------------- HAL LEVEL -----------------------------------*/
uint8_t HAL_BLE_Init(void)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  return(APP_LLD_BLE_SendCmdM0(HAL_BLE_INIT_CMDCODE,param_hal_BLE_IPCC));

}

//static uint32_t networkID = 0x71764129;
uint8_t HAL_BLE_SetNetworkID(uint32_t ID)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.InfoTime = ID ;// channel becomes network ID

  return(APP_LLD_BLE_SendCmdM0(HAL_BLE_SETNETWORKID_CMDCODE,param_hal_BLE_IPCC));

}

uint8_t HAL_BLE_SendPacket(uint8_t channel, uint32_t wakeup_time, uint8_t* txBuffer, dataCase_t  dataCaseTx)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  rxStatus_Ptr[0]=&hal_status;
  rxTimeStamp_Ptr[0]=&hal_timestamp_receive;
  rxRSSI_Ptr[0]=&hal_rssi;

  
  txBuffer_Ptr=txBuffer;
  bleparam_hal_BLE_Packet.InfoTime =  global_PacketNumber ; // used benefit of InfoTime to send parameter to M0

  bleparam_hal_BLE_Packet.channel_tx = channel ;
  bleparam_hal_BLE_Packet.WakeupTime = wakeup_time ;
  bleparam_hal_BLE_Packet.setCase.dataCase =  dataCaseTx;
  memcpy((bleparam_hal_BLE_Packet.txrxBuffer.txBuffer),txBuffer_Ptr,txBuffer[1]+2); // to be replaced with \0 ended unit8_t

  return(APP_LLD_BLE_SendCmdM0(HAL_BLE_SENDPACKET_CMDCODE,param_hal_BLE_IPCC));
}

uint8_t HAL_BLE_SendPacketWithAck(uint8_t channel, uint32_t wakeup_time, uint8_t* txBuffer, uint8_t* rxBuffer, uint32_t receive_timeout, dataCase_t  dataCaseTx )
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  rxStatus_Ptr[0]=&hal_status;
  rxTimeStamp_Ptr[0]=&hal_timestamp_receive;
  rxRSSI_Ptr[0]=&hal_rssi;

  txBuffer_Ptr=txBuffer;
  rxBuffer_Ptr[0]=rxBuffer;
  bleparam_hal_BLE_Packet.InfoTime =  global_PacketNumber ; // used benefit of InfoTime to send parameter to M0
  bleparam_hal_BLE_Packet.StateMachineNo= global_PacketStopRx ; // used benefit of StateMachineNo to send parameter to M0

  bleparam_hal_BLE_Packet.channel_tx = channel ;
  bleparam_hal_BLE_Packet.WakeupTime = wakeup_time ;
  bleparam_hal_BLE_Packet.ReceiveWindowLength = receive_timeout ;
  bleparam_hal_BLE_Packet.setCase.dataCase =  dataCaseTx;
  memcpy((bleparam_hal_BLE_Packet.txrxBuffer.txBuffer),txBuffer_Ptr,txBuffer[1]+2); // to be replaced with \0 ended unit8_t

  return(APP_LLD_BLE_SendCmdM0(HAL_BLE_SENDPACKETWITHACK_CMDCODE,param_hal_BLE_IPCC));
}

uint8_t HAL_BLE_ReceivePacket(uint8_t channel, uint32_t wakeup_time, uint8_t* rxBuffer, uint32_t receive_timeout, dataCase_t  dataCaseRx)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  rxStatus_Ptr[0]=&hal_status;
  rxTimeStamp_Ptr[0]=&hal_timestamp_receive;
  rxRSSI_Ptr[0]=&hal_rssi;

  rxBuffer_Ptr[0]=rxBuffer;
  bleparam_hal_BLE_Packet.InfoTime =  global_PacketNumber ; // used benefit of InfoTime to send parameter to M0
  bleparam_hal_BLE_Packet.StateMachineNo= global_PacketStopRx ; // used benefit of StateMachineNo to send parameter to M0


  bleparam_hal_BLE_Packet.channel_rx = channel ;
  bleparam_hal_BLE_Packet.WakeupTime =  wakeup_time;
  bleparam_hal_BLE_Packet.ReceiveWindowLength = receive_timeout ;
  bleparam_hal_BLE_Packet.setCase.dataCase =  dataCaseRx;

  return(APP_LLD_BLE_SendCmdM0(HAL_BLE_RECEIVEPACKET_CMDCODE,param_hal_BLE_IPCC));
}

uint8_t HAL_BLE_ReceivePacketWithAck(uint8_t channel, uint32_t wakeup_time, uint8_t* rxBuffer, uint8_t* txBuffer, uint32_t receive_timeout, dataCase_t  dataCaseRx )
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  rxStatus_Ptr[0]=&hal_status;
  rxTimeStamp_Ptr[0]=&hal_timestamp_receive;
  rxRSSI_Ptr[0]=&hal_rssi;

  txBuffer_Ptr=txBuffer;
  rxBuffer_Ptr[0]=rxBuffer;
  bleparam_hal_BLE_Packet.InfoTime =  global_PacketNumber ; // used benefit of InfoTime to send parameter to M0
  bleparam_hal_BLE_Packet.StateMachineNo= global_PacketStopRx ; // used benefit of StateMachineNo to send parameter to M0

  bleparam_hal_BLE_Packet.channel_rx = channel ;
  bleparam_hal_BLE_Packet.WakeupTime =  wakeup_time;
  bleparam_hal_BLE_Packet.ReceiveWindowLength = receive_timeout ;
  bleparam_hal_BLE_Packet.setCase.dataCase =  dataCaseRx;
  memcpy((bleparam_hal_BLE_Packet.txrxBuffer.txBuffer),txBuffer_Ptr,txBuffer[1]+2); // to be replaced with \0 ended unit8_t

  return(APP_LLD_BLE_SendCmdM0(HAL_BLE_RECEIVEPACKETWITHACK_CMDCODE,param_hal_BLE_IPCC));
}

/* ------------------------------- LL LEVEL -----------------------------------*/

uint8_t LLD_BLE_GetStatus(uint32_t *time) /* return value */
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.InfoTime= NULL;

  uint8_t return_value;
  return_value=(APP_LLD_BLE_SendCmdM0(LLD_BLE_GETSTATUS_CMDCODE,param_hal_BLE_IPCC)); /* return_value used */
  
  *time = (bleparam_hal_BLE_Packet.InfoTime); /* pointer as result */
  return(return_value); /* return_value send */

}

uint8_t LLD_BLE_StopActivity(void)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  return(APP_LLD_BLE_SendCmdM0(LLD_BLE_STOPACTIVITY_CMDCODE,param_hal_BLE_IPCC)); /* return_value send */
}

void LLD_BLE_SetEncryptionCount(uint8_t StateMachineNo, uint8_t *count_tx, uint8_t *count_rcv)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.StateMachineNo=StateMachineNo;

  for(uint8_t i = 0; i < 5; i++) {
     bleparam_hal_BLE_Packet.txrxBuffer.txBuffer[i]=count_tx[i];
     bleparam_hal_BLE_Packet.txrxBuffer.txBuffer[i+5]=count_rcv[i];
  } 

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETENCRYPTIONCOUNT_CMDCODE,param_hal_BLE_IPCC); /* return_value send */
  
  return;
}

void LLD_BLE_SetEncryptionAttributes(uint8_t StateMachineNo, uint8_t *enc_iv, uint8_t *enc_key)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.StateMachineNo=StateMachineNo;

  for(uint8_t i = 0; i < 8; i++) {
     bleparam_hal_BLE_Packet.txrxBuffer.txBuffer[i]=enc_iv[i];
     bleparam_hal_BLE_Packet.txrxBuffer.txBuffer[i+8]=enc_key[i];
     bleparam_hal_BLE_Packet.txrxBuffer.txBuffer[i+16]=enc_key[i+8];
  } 
  
  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETENCRYPTIONATTRIBUTES_CMDCODE,param_hal_BLE_IPCC); /* return_value send */
  
  return;
}

void LLD_BLE_SetEncryptFlags(uint8_t StateMachineNo, FunctionalState EncryptFlagTx, FunctionalState EncryptFlagRcv)
{
  /* Check the parameters */
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.StateMachineNo=StateMachineNo;
  bleparam_hal_BLE_Packet.channel_tx=(uint8_t)EncryptFlagTx;
  bleparam_hal_BLE_Packet.channel_rx=(uint8_t)EncryptFlagRcv;

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETENCRYPTFLAGS_CMDCODE,param_hal_BLE_IPCC); /* return_value send */
  
  return;
}

void LLD_BLE_EncryptPlainData(uint8_t *Key, uint8_t *plainData, uint8_t *cypherData)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  memcpy((bleparam_hal_BLE_Packet.txrxBuffer.txBuffer),Key,128); // to be replaced with \0 ended unit8_t
  memcpy((bleparam_hal_BLE_Packet.txrxBuffer.txBuffer+128),plainData,128); // to be replaced with \0 ended unit8_t

  APP_LLD_BLE_SendCmdM0(LLD_BLE_ENCRYPTPLAINDATA_CMDCODE,param_hal_BLE_IPCC); /* return_value used */
  
  memcpy(cypherData,(bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer),128); // to be replaced with \0 ended unit8_t
  
  return;
}

void LLD_BLE_StartTone(uint8_t RF_channel, uint8_t powerLevel)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.channel_tx= RF_channel;
  bleparam_hal_BLE_Packet.StateMachineNo= powerLevel;

  APP_LLD_BLE_SendCmdM0(LLD_BLE_STARTTONE_CMDCODE,param_hal_BLE_IPCC); /* return_value send */
  
  return;
}

/* without return */
void  LLD_BLE_SetChannelMap(uint8_t StateMachineNo, uint8_t *chan_remap) /*  no return */
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.StateMachineNo=StateMachineNo;
    
  for(uint8_t i = 0; i < 5; i++) {
     bleparam_hal_BLE_Packet.txrxBuffer.txBuffer[i]=chan_remap[i];
  } 

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETCHANNELMAP_CMDCODE,param_hal_BLE_IPCC); /* return_value used */

  return;
}

void LLD_BLE_SetChannel(uint8_t StateMachineNo, uint8_t channel, uint8_t channel_increment) 
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  
  /* use param_hal_BLE_IPCC for Network ID */
  bleparam_hal_BLE_Packet.StateMachineNo=StateMachineNo;
  bleparam_hal_BLE_Packet.channel_tx= channel;
  bleparam_hal_BLE_Packet.channel_rx= channel_increment;

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETCHANNEL_CMDCODE,param_hal_BLE_IPCC); /* return_value used */

  return;
}

void LLD_BLE_SetTxAttributes(uint8_t StateMachineNo, uint32_t NetworkID, uint32_t crc_init, uint32_t sca)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  
  bleparam_hal_BLE_Packet.StateMachineNo=StateMachineNo;
  bleparam_hal_BLE_Packet.InfoTime = NetworkID ;// channel becomes network ID
  bleparam_hal_BLE_Packet.WakeupTime =  crc_init;
  bleparam_hal_BLE_Packet.ReceiveWindowLength =  sca; // not used

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETTXATTRIBUTES_CMDCODE,param_hal_BLE_IPCC); /* return_value used */
  
  return;
}

void LLD_BLE_SetBackToBackTime(uint32_t back_to_back_time)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  
  bleparam_hal_BLE_Packet.WakeupTime =  back_to_back_time;// channel becomes network ID

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETBACKTOBACKTIME_CMDCODE,param_hal_BLE_IPCC); /* return */
  
  return;
}

void LLD_BLE_SetTxPower(uint8_t powerLevel)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  
  bleparam_hal_BLE_Packet.StateMachineNo =  powerLevel;// channel becomes network ID

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETTXPOWER_CMDCODE,param_hal_BLE_IPCC); /* return_value used */

  return;
}

void LLD_BLE_SetTx_Rx_Phy(uint8_t StateMachineNo, uint8_t tx_phy, uint8_t rx_phy)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  
  bleparam_hal_BLE_Packet.StateMachineNo =  StateMachineNo;// channel becomes network ID
  bleparam_hal_BLE_Packet.channel_tx =  tx_phy;// channel becomes network ID
  bleparam_hal_BLE_Packet.channel_rx =  rx_phy;// channel becomes network ID

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETTX_RX_PHY_CMDCODE,param_hal_BLE_IPCC); /* return_value used */
  
  return;
}

void LLD_BLE_StopTone(void)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);

  APP_LLD_BLE_SendCmdM0(LLD_BLE_STOPTONE_CMDCODE,param_hal_BLE_IPCC); /* return_value used */
  
  return;
}

void LLD_BLE_SetReservedArea(ActionPacket *p)
{
    
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  
  bleparam_hal_BLE_Packet.StateMachineNo=p->StateMachineNo;
  bleparam_hal_BLE_Packet.channel_tx=p->next_true;
  bleparam_hal_BLE_Packet.channel_rx=p->next_false;
  bleparam_hal_BLE_Packet.WakeupTime=p->WakeupTime;
  bleparam_hal_BLE_Packet.ReceiveWindowLength=p->ReceiveWindowLength;
  
  bleparam_hal_BLE_Packet.actionPacketNb=(p->actionPacketNb);
  bleparam_hal_BLE_Packet.ActionTag=(p->ActionTag);
  
  rxStatus_Ptr[p->actionPacketNb]=&(p->status);
  rxTimeStamp_Ptr[p->actionPacketNb]=&(p->timestamp_receive);
  rxRSSI_Ptr[p->actionPacketNb]=&(p->rssi);
  
  bleparam_hal_BLE_Packet.setCase.condCase=(condCase_t)p->condRoutine;
  bleparam_hal_BLE_Packet.setCase.dataCase=(dataCase_t)p->dataRoutine;

  if(bleparam_hal_BLE_Packet.ActionTag & TXRX) {
      
    txBuffer_Ptr=p->data;
    memcpy((bleparam_hal_BLE_Packet.txrxBuffer.txBuffer),txBuffer_Ptr,258); // to be replaced with \0 ended unit8_t

  } else {
    rxBuffer_Ptr[p->actionPacketNb]=p->data;
  }

  APP_LLD_BLE_SendCmdM0(LLD_BLE_SETRESERVEDAREA_CMDCODE,param_hal_BLE_IPCC); /* return_value used */
  
  return;

}

uint8_t LLD_BLE_MakeActionPacketPending(ActionPacket *p)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
    
  bleparam_hal_BLE_Packet.actionPacketNb=(p->actionPacketNb);
  bleparam_hal_BLE_Packet.InfoTime =  global_PacketNumber ; // used benefit of time to send parameter to M0
  bleparam_hal_BLE_Packet.StateMachineNo = global_PacketStopRx ;


  return(APP_LLD_BLE_SendCmdM0(LLD_BLE_MAKEACTIONPACKETPENDING_CMDCODE,param_hal_BLE_IPCC)); /* return_value used */
}

void LLD_BLE_Init(uint16_t hs_startup_time, uint8_t low_speed_osc, uint32_t* hot_table, FunctionalState whitening)
{
  //param_hal_BLE_IPCC=(uint32_t*)(&bleparam_hal_BLE_Packet);
  
  bleparam_hal_BLE_Packet.WakeupTime=(uint32_t)(hs_startup_time);
  bleparam_hal_BLE_Packet.ActionTag=low_speed_osc;
  bleparam_hal_BLE_Packet.actionPacketNb=(uint8_t)whitening;
  
  APP_LLD_BLE_SendCmdM0(LLD_BLE_INIT_CMDCODE,param_hal_BLE_IPCC); /* return_value used */
  
  memcpy((uint8_t*)hot_table,(bleparam_hal_BLE_Packet.txrxBuffer.txBuffer),BLE_HOT_ANA_CONFIG_TABLE_LENGTH); // to be replaced with \0 ended unit8_t
  
  return;

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
