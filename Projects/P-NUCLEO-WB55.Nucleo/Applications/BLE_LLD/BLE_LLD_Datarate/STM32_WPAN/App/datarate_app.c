/**
  ******************************************************************************
  * File Name          : datarate_app.c
  * Description        : BLE LLD simple demo to control LEDs remotely
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
#include "stdbool.h"
#include "app_common.h"
#include "stm32_seq.h"
#include "stm32_lpm.h"
#include "stm_logging.h"
#include "dbg_trace.h"
#include "ble_hal.h"
#include "ipBLE_lld_public.h"
#include "app_ble_lld.h"
#include "datarate_app.h"

/* Private includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
// Allows to pack and unpack user data in payload (must not exceed 255 bytes)
typedef PACKED_STRUCT
{
  uint8_t led;
} userPayload;

/* Private defines -----------------------------------------------------------*/
#define CHANNEL           8 // radio channel
#define CHANNEL_HOP       19 // radio channel hop
#define POWER             TX_POW_PLUS_6_DB // Transmit power
#define NET_ID            0x5A964129 // network ID, both devices must use the same
#define WAKEUP_US         10000 // delay before starting radio operation
#define RX_TIMEOUT_US     (1*1000*1000) // max delay radio will listen for a packet
#define DEBOUNCE_MS       250 // debouncing delay (ms)

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void radioInit(void);
static void buttonStart(void);
static void receiveStart(void);
static void receiveCb(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);
static void transmitStart(void);
static void transmitCbMaster(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);
static void transmitCbSlave(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);

/* Private variables -----------------------------------------------*/
static ActionPacket apReceiver[ACTION_PACKET_NB];
static ActionPacket apTransmit[ACTION_PACKET_NB];

uint8_t SwitchOffGPIO_timer_Id;
dataRate_t dataRateMode = dataRateOff;
/* Functions Definition ------------------------------------------------------*/

void DATARATE_APP_Init(void)
{
  CheckWirelessFirmwareInfo();

  /* Disable low power */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE );

  /* Register tasks for event processing */
  UTIL_SEQ_RegTask(1<<CFG_TASK_BUTTON, UTIL_SEQ_RFU, buttonStart);

  APP_BLE_LLD_Init();
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &SwitchOffGPIO_timer_Id, hw_ts_SingleShot, Appli_TS_Callback);

  radioInit();
  //HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)RX_TIMEOUT_US);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE ); // TO ENABLE

}

static void radioInit(void)
{
  HAL_BLE_LLD_Init(CFG_HS_STARTUP_TIME, true);

  BLE_LLD_SetTxPower(POWER);
  BLE_LLD_SetBackToBackTime(CFG_BACK2BACK_TIME);

  BLE_LLD_SetChannel(STATE_MACHINE_0, CHANNEL);
  BLE_LLD_SetTx_Rx_Phy(STATE_MACHINE_0,TX_PHY_2MBPS,RX_PHY_2MBPS);
  BLE_LLD_SetTxAttributes(STATE_MACHINE_0, NET_ID);
 
  BLE_LLD_SetChannel(STATE_MACHINE_1, CHANNEL_HOP);
  BLE_LLD_SetTx_Rx_Phy(STATE_MACHINE_1,TX_PHY_2MBPS,RX_PHY_2MBPS);
  BLE_LLD_SetTxAttributes(STATE_MACHINE_1, ~NET_ID);
  
  uint32_t wakeup_time = WAKEUP_US;

  uint32_t window_time1 = RX_TIMEOUT_US;
  uint32_t window_time2 = 400;
  uint32_t window_time3 = 450;

    // RECEPTION    
    ActionPacket *ap1;
    ap1 = &apReceiver[APACKET_1];
    ap1->actionPacketNb = APACKET_1;
    ap1->StateMachineNo = STATE_MACHINE_0;
    ap1->ActionTag =  TIMER_WAKEUP;
    ap1->WakeupTime = wakeup_time;
    ap1->ReceiveWindowLength = window_time1;
    ap1->nextTrue = APACKET_2;
    ap1->nextFalse = APACKET_1;
    ap1->callback = receiveCb;
    BLE_LLD_SetReservedArea(ap1);

    ActionPacket *ap2;
    ap2 = &apReceiver[APACKET_2];
    ap2->actionPacketNb = APACKET_2;
    ap2->StateMachineNo = STATE_MACHINE_0;
    ap2->ActionTag =  PLL_TRIG;
    ap2->ReceiveWindowLength = window_time2;
    ap2->nextTrue = APACKET_3;
    ap2->nextFalse = APACKET_3;
    ap2->callback = receiveCb;
    BLE_LLD_SetReservedArea(ap2);

    ActionPacket *ap3;
    ap3 = &apReceiver[APACKET_3];
    ap3->actionPacketNb = APACKET_3;
    ap3->StateMachineNo = STATE_MACHINE_1;
    ap3->ActionTag =  PLL_TRIG;
    ap3->ReceiveWindowLength = window_time2;
    ap3->nextTrue = APACKET_2;
    ap3->nextFalse = APACKET_2;
    ap3->callback = receiveCb;
    BLE_LLD_SetReservedArea(ap3);

    // TRANSMISSION
    ActionPacket *ap0;
    ap0 = &apTransmit[APACKET_0];
    ap0->actionPacketNb = APACKET_0;
    ap0->StateMachineNo = STATE_MACHINE_0;
    ap0->ActionTag =  TIMER_WAKEUP;
    ap0->WakeupTime = wakeup_time;
    ap0->ReceiveWindowLength = window_time1;
    ap0->nextTrue = APACKET_4;
    ap0->nextFalse = APACKET_6;
    ap0->callback = transmitCbMaster;
    BLE_LLD_SetReservedArea(ap0);

    // Slave LED3
    ActionPacket *ap4;
    userPayload payload4;
    payload4.led = LED3;
    // Packet to transmit
    ap4 = &apTransmit[APACKET_4];
    ap4->actionPacketNb = APACKET_4;
    ap4->StateMachineNo = STATE_MACHINE_1;
    ap4->ActionTag =  TXRX | PLL_TRIG;
    ap4->data = &payload4;
    ap4->dataSize = sizeof(payload4);
    ap4->nextTrue = APACKET_5;
    ap4->nextFalse = APACKET_5;
    ap4->callback = transmitCbSlave;
    BLE_LLD_SetReservedArea(ap4);
    
    ActionPacket *ap5;
    ap5 = &apTransmit[APACKET_5];
    ap5->actionPacketNb = APACKET_5;
    ap5->StateMachineNo = STATE_MACHINE_0;
    ap5->ActionTag =  PLL_TRIG;
    ap5->ReceiveWindowLength = window_time3;
    ap5->nextTrue = APACKET_4;
    ap5->nextFalse = APACKET_4;
    ap5->callback = transmitCbSlave;
    BLE_LLD_SetReservedArea(ap5);
    
    // Master LED2
    ActionPacket *ap6;
    ap6 = &apTransmit[APACKET_6];
    ap6->actionPacketNb = APACKET_6;
    ap6->StateMachineNo = STATE_MACHINE_1;
    ap6->ActionTag =  PLL_TRIG;
    ap6->ReceiveWindowLength = window_time3;
    ap6->nextTrue = APACKET_7;
    ap6->nextFalse = APACKET_7;
    ap6->callback = transmitCbMaster;
    BLE_LLD_SetReservedArea(ap6);
    
    ActionPacket *ap7;
    userPayload payload7;
    payload7.led = LED2;
    // Packet to transmit
    ap7 = &apTransmit[APACKET_7];
    ap7->actionPacketNb = APACKET_7;
    ap7->StateMachineNo = STATE_MACHINE_0;
    ap7->ActionTag =  TXRX | PLL_TRIG;
    ap7->data = &payload7;
    ap7->dataSize = sizeof(payload7);
    ap7->nextTrue = APACKET_6;
    ap7->nextFalse = APACKET_6;
    ap7->callback = transmitCbMaster;
    BLE_LLD_SetReservedArea(ap7);

}

uint8_t led1Toggle=0;
uint8_t led2Toggle=0;
uint8_t led3Toggle=0;

static void receiveStart(void)
{
  BLE_LLD_MakeActionPacketPending(&apReceiver[APACKET_1]);
}

static void receiveCb(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  userPayload *payload = data;
  Led_TypeDef led;
  if ((cmd == RX_OK_READY) | (cmd == RX_OK_BUSY)){
    led = (Led_TypeDef)payload->led;
    if (led != LED1 && led != LED2 && led != LED3){
    }else if (led==LED1){
      if(led1Toggle==100)
      {
        led1Toggle=0;
        BSP_LED_Off(LED2);
        BSP_LED_Toggle(LED1);
        uartWrite("receiveCb(): LED2 OFF");
        uartWrite("receiveCb(): LED1 TOGGLE");
      } else {
        led1Toggle++;
      }
    }else if (led==LED2){
      if(led2Toggle==100)
      {
        led2Toggle=0;
        BSP_LED_Off(LED1);
        BSP_LED_Toggle(LED2);
        uartWrite("receiveCb(): LED1 OFF");
        uartWrite("receiveCb(): LED2 TOGGLE");
      } else {
        led2Toggle++;
      }
    }else if (led==LED3){
      if(led3Toggle==100)
      {
        led3Toggle=0;
        BSP_LED_Toggle(LED3);
        uartWrite("receiveCb(): LED3 TOGGLE");
      } else {
        led3Toggle++;
      }
    }
  }
}

int16_t ledChange=0;
Led_TypeDef ledSwap = LED1;

static void transmitStart(void)
{
  BLE_LLD_MakeActionPacketPending(&apTransmit[APACKET_0]);
}

static void transmitCbMaster(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  userPayload *payload = data;
  Led_TypeDef led;
  if ((cmd == RX_OK_READY) | (cmd == RX_OK_BUSY)){
    led = (Led_TypeDef)payload->led;
    if (led != LED1 && led != LED2 && led != LED3){
    }else{
      if(led3Toggle==100)
      {
        led3Toggle=0;
        BSP_LED_Toggle(LED3);
        uartWrite("transmitCbMaster(): LED3 TOGGLE");
      } else {
        led3Toggle++;
      }
    }
  }
  if ((cmd == TX_OK_READY) | (cmd == TX_OK_BUSY)){
    if(ledChange==1000)
    {
      userPayload payloadSwap;
      ledChange=0;
      if (ledSwap == LED1){
        payloadSwap.led = LED2;
        ledSwap = LED2;
      } else {
        payloadSwap.led = LED1;
        ledSwap = LED1;
      }
      ap->data = &payloadSwap;
      BLE_LLD_SetReservedArea(ap);
    }
    ledChange++;
  }
}

static void transmitCbSlave(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  userPayload *payload = data;
  Led_TypeDef led;
  if ((cmd == RX_OK_READY) | (cmd == RX_OK_BUSY)){
    led = (Led_TypeDef)payload->led;
    if (led != LED1 && led != LED2 && led != LED3){
    }else if (led==LED1){
      if(led1Toggle==100)
      {
        led1Toggle=0;
        BSP_LED_Off(LED2);
        BSP_LED_Toggle(LED1);
        uartWrite("transmitCbSlave(): LED2 OFF");
        uartWrite("transmitCbSlave(): LED1 TOGGLE");
      } else {
        led1Toggle++;
      }
    }else if (led==LED2){
      if(led2Toggle==100)
      {
        led2Toggle=0;
        BSP_LED_Off(LED1);
        BSP_LED_Toggle(LED2);
        uartWrite("transmitCbSlave(): LED1 OFF");
        uartWrite("transmitCbSlave(): LED2 TOGGLE");
      } else {
        led2Toggle++;
      }
    }
  }
}

static void buttonStart(void)
{
  radioInit();
  if(dataRateMode==dataRateReceive)
  {
    receiveStart() ;
  } else if (dataRateMode==dataRateTransmit) {
    transmitStart() ;
  } else {
    UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_ENABLE ); // TO ENABLE
    HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)RX_TIMEOUT_US);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  BLE_LLD_StopActivity();
    switch (GPIO_Pin){
      case BUTTON_SW1_PIN: dataRateMode = dataRateReceive; break;
      case BUTTON_SW2_PIN: dataRateMode = dataRateTransmit; break;
      case BUTTON_SW3_PIN: dataRateMode = dataRateOff; break;
      default: return;
    }
  UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON, CFG_SCH_PRIO_0);
}

void Appli_TS_Callback(void)
{
  UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON, CFG_SCH_PRIO_0);
}

