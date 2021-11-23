/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : proximity_app.c
  * Description        : BLE LLD validation Application.
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
/* USER CODE END Header */

/* Protocol:
A device periodically (2s) broadcasts its ID, and listens for other devices the
rest of the time (no power save implemented for now).

A fixed period could allow 2 devices to always transmit at the same time, so
they would never detect each other. To avoid this issue, a random offset is
added at the end of each period before actual transmission.
*/

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "assert.h"
#include "shci.h"
#include "stm32_seq.h"
#include "stm32_lpm.h"
#include "stm_logging.h"
#include "dbg_trace.h"
#include "ble_hal.h"
#include "app_ble_lld.h"
#include "proximity_app.h"

/* Private includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
// Defines user data format in payload
typedef PACKED_STRUCT
{
  uint32_t deviceId;
} userPayload;

// Stores data about a remote device
typedef struct
{
  uint32_t id;          // ID of the device
  uint8_t power;        // latest power measured for this device
  uint32_t timestamp;   // latest date this device was seen
} remoteDevice;

/* Private defines -----------------------------------------------------------*/
#define CHANNEL         13
#define POWER           TX_POW_MIN_16_5_DB
#define NET_ID          0x752D5525
#define TX_PERIOD_US    (2*1000*1000)

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void radioInit(void);

static void listenStart(bool reduceTimeout);
static void listenEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);
static void sendStart(void);
static void sendEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);

static void peersInit(void);
static void peersAdd(remoteDevice *p);
static void peersPrint(void);

/* Private variables -----------------------------------------------*/

/* Functions Definition ------------------------------------------------------*/

void PROXIMITY_APP_Init(void)
{
  CheckWirelessFirmwareInfo();

  /* Disable low power */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE );

  srand(LL_FLASH_GetUDN());
  peersInit();
  APP_BLE_LLD_Init();
  radioInit();

  listenStart(false);
}

static void radioInit(void)
{
  HAL_BLE_LLD_Init(CFG_HS_STARTUP_TIME, true);
  HAL_BLE_LLD_Configure(POWER, CHANNEL, true, CFG_BACK2BACK_TIME, NET_ID);
}

static void listenStart(bool reduceTimeout)
{
  uint8_t status;
  static uint32_t timeout;

  if (reduceTimeout){
    // This avoids Tx starvation
    timeout /= 2;
  }else{
    /* We add a random offset (between 0 and 100ms) to the period so that even
       if 2 devices are in phase, their transmissions will rarely collide. */
    timeout = TX_PERIOD_US + (rand() % (100*1000));
  }

  status = HAL_BLE_LLD_ReceivePacket(timeout, listenEnd);
  if (status != SUCCESS_0){
    APP_DBG("ERROR: %s: HAL call failed", __func__);
  }
}

static void listenEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  userPayload *payload = data;
  remoteDevice peer;
  APP_DBG("%s: event %s", __func__, eventToString(cmd));
  if (cmd == RX_OK_READY){
    if (size != sizeof(userPayload)){
      APP_DBG("%s: wrong payload length, packet discarded", __func__);
    }else{
      APP_DBG("Received ID: %u", payload->deviceId);
      peer.id = payload->deviceId;
      peer.power = ap->rssi;
      peer.timestamp = HAL_GetTick();
      peersAdd(&peer);
      peersPrint();
    }
    // In that case, halve timeout value, else device may never transmit
    listenStart(true);
  }
  if (RADIO_IS_READY(cmd)){
    sendStart();
  }
}

static void sendStart(void)
{
  userPayload payload;

  payload.deviceId = LL_FLASH_GetUDN();
  HAL_BLE_LLD_SendPacket(&payload, sizeof(userPayload), sendEnd);
}

static void sendEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  listenStart(false);
}

/* Data presentation layer ---------------------------------------------------*/

#define PEERS_MAX       10
#define ID_INVALID      UINT32_MAX
#define INDEX_INVALID   UINT32_MAX

static remoteDevice peers[PEERS_MAX];

// Initializes peers array
static void peersInit(void)
{
  for (uint32_t i = 0; i < PEERS_MAX; i++){
    remoteDevice *p = &peers[i];
    p->id = ID_INVALID;
    p->power = 0;
    p->timestamp = 0;
  }
}

// Adds a device to peers array
static void peersAdd(remoteDevice *p)
{
  bool update = false;
  // Try to update existing entry
  for (uint32_t i = 0; i < PEERS_MAX; i++){
    if (peers[i].id == p->id){
      peers[i] = *p;
      update = true;
      break;
    }
  }
  if (!update){
    // Create new entry
    for (uint32_t i = 0; i < PEERS_MAX; i++){
      if (peers[i].id == ID_INVALID){
        peers[i] = *p;
        break;
      }
    }
  }
}

// Prints devices from peers array to UART (sorted by ID)
static void peersPrint(void)
{
  uint32_t i, idMin, idCur, indexMin;
  uint32_t idMinPrev = 0;
  // Clear screen, then cursor to home
  uartWriteRaw("\x1B[2J\x1B[H");
  uartWrite("Device ID: %u", LL_FLASH_GetUDN());
  uartWrite("");
  uartWrite("Nearby devices:");
  uartWrite("    ID   | power | timestamp");
  uartWrite("----------------------------");

  while (true){
    // Search for the next minimum ID
    idMin = UINT32_MAX;
    indexMin = INDEX_INVALID;
    for (i = 0; i < PEERS_MAX; i++){
      idCur = peers[i].id;
      if ((idCur != ID_INVALID) && (idCur < idMin) && (idCur > idMinPrev)){
        idMin = idCur;
        indexMin = i;
      }
    }
    if (indexMin == INDEX_INVALID){
      break;
    }
    remoteDevice *p = &peers[indexMin];
    uartWrite("%8u | %5u | %8u", p->id, p->power, p->timestamp);
    idMinPrev = idMin;
  }
}

