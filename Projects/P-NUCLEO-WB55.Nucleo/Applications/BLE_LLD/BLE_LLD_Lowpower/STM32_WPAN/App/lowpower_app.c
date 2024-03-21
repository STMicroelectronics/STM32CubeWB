/**
  ******************************************************************************
  * File Name          : lowpower_app.c
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
#include "lowpower_app.h"

/* Private includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
// Allows to pack and unpack user data in payload (must not exceed 255 bytes)
typedef PACKED_STRUCT
{
  uint8_t led;
} userPayload;

/* Private defines -----------------------------------------------------------*/
#define CHANNEL           8 // radio channel
#define POWER             TX_POW_PLUS_6_DB // Transmit power
#define NET_ID            0x5A964129 // network ID, both devices must use the same
#define WAKEUP_US         10000 // delay before starting radio operation
#define RX_TIMEOUT_US     (1*1000*1000) // max delay radio will listen for a packet
#define LED_ON_TIMER      (2*1000*1000/CFG_TS_TICK_VAL) /**< 2000ms */ // delay of Toggle

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void radioInit(void);
static void sendStart(void);
static void sendLed1(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);
static void sendLed2(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);
static void sendLed3(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);

/* Private variables -----------------------------------------------*/
static ActionPacket apSend[ACTION_PACKET_NB];

uint8_t SwitchOffGPIO_timer_Id;

/* Functions Definition ------------------------------------------------------*/

void LOWPOWER_APP_Init(void)
{
  CheckWirelessFirmwareInfo();

  /* Disable low power */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE );

  /* Register tasks for event processing */
  UTIL_SEQ_RegTask(1<<CFG_TASK_TIMER, UTIL_SEQ_RFU, sendStart);

  APP_BLE_LLD_Init();
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &SwitchOffGPIO_timer_Id, hw_ts_SingleShot, Appli_TS_Callback);

  radioInit();
  HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMER);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE ); // TO ENABLE

}

static void radioInit(void)
{
  HAL_BLE_LLD_Init(CFG_HS_STARTUP_TIME, true);
  HAL_BLE_LLD_Configure(POWER, CHANNEL, true, CFG_BACK2BACK_TIME, NET_ID);

  ActionPacket *ap1;
  userPayload payload1;
  payload1.led = LED1;
  uint32_t wakeup_time1 = WAKEUP_US;
  // Packet to send
  ap1 = &apSend[APACKET_1];
  ap1->actionPacketNb = APACKET_1;
  ap1->StateMachineNo = STATE_MACHINE_0;
  ap1->ActionTag = TXRX | TIMER_WAKEUP;
  ap1->WakeupTime = wakeup_time1;
  ap1->data = &payload1;
  ap1->dataSize = sizeof(payload1);
  ap1->nextTrue = APACKET_2;
  ap1->nextFalse = APACKET_STOP;
  ap1->callback = sendLed1;
  BLE_LLD_SetReservedArea(ap1);
  uartWrite("radioInit(): MSG LED1");
  
  ActionPacket *ap2;
  userPayload payload2;
  payload2.led = LED2;
  uint32_t wakeup_time2 = 2*WAKEUP_US;
  // Packet to send
  ap2 = &apSend[APACKET_2];
  ap2->actionPacketNb = APACKET_2;
  ap2->StateMachineNo = STATE_MACHINE_0;
  ap2->ActionTag = TXRX | TIMER_WAKEUP;
  ap2->WakeupTime = wakeup_time2;
  ap2->data = &payload2;
  ap2->dataSize = sizeof(payload2);
  ap2->nextTrue = APACKET_3;
  ap2->nextFalse = APACKET_STOP;
  ap2->callback = sendLed2;
  BLE_LLD_SetReservedArea(ap2);
  uartWrite("radioInit(): MSG LED2");
  
  ActionPacket *ap3;
  userPayload payload3;
  payload3.led = LED3;
  uint32_t wakeup_time3 = 4*WAKEUP_US;
  // Packet to send
  ap3 = &apSend[APACKET_3];
  ap3->actionPacketNb = APACKET_3;
  ap3->StateMachineNo = STATE_MACHINE_0;
  ap3->ActionTag = TXRX | TIMER_WAKEUP;
  ap3->WakeupTime = wakeup_time3;
  ap3->data = &payload3;
  ap3->dataSize = sizeof(payload3);
  ap3->nextTrue = APACKET_STOP;
  ap3->nextFalse = APACKET_STOP;
  ap3->callback = sendLed3;
  BLE_LLD_SetReservedArea(ap3);
  uartWrite("radioInit(): MSG LED3");
}


static void sendStart(void)
{
  BLE_LLD_MakeActionPacketPending(&apSend[APACKET_1]);
  uartWrite("sendStart(): BLE_LLD_MakeActionPacketPending");
}

static void sendLed1(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
}

static void sendLed2(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
}

static void sendLed3(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_ENABLE ); // TO ENABLE
  HW_TS_Start(SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMER);
}


void Appli_TS_Callback(void)
{
  UTIL_SEQ_SetTask(1U << CFG_TASK_TIMER, CFG_SCH_PRIO_0);
}

