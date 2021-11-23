/**
  ******************************************************************************
  * File Name          : pressbutton_app.c
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
#include "pressbutton_app.h"

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
#define RX_TIMEOUT_US     (1*1000*1000) // max delay radio will listen for a packet
#define DEBOUNCE_MS       250 // debouncing delay (ms)

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void radioInit(void);
static void listenStart(void);
static void listenEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);
static void sendStart(void);
static void sendEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);

static inline uint32_t timeDifference(uint32_t old, uint32_t new);

/* Private variables -----------------------------------------------*/
static uint32_t ledToToggle;

/* Functions Definition ------------------------------------------------------*/

void PRESSBUTTON_APP_Init(void)
{
  CheckWirelessFirmwareInfo();

  /* Disable low power */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE );

  /* Register tasks for event processing */
  UTIL_SEQ_RegTask(1<<CFG_TASK_BUTTON, UTIL_SEQ_RFU, sendStart);

  APP_BLE_LLD_Init();
  radioInit();
  listenStart();
}

static void radioInit(void)
{
  HAL_BLE_LLD_Init(CFG_HS_STARTUP_TIME, true);
  HAL_BLE_LLD_Configure(POWER, CHANNEL, true, CFG_BACK2BACK_TIME, NET_ID);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t latest = 0;

  // Debounce
  if (timeDifference(latest, HAL_GetTick()) > DEBOUNCE_MS){
    latest = HAL_GetTick();
    switch (GPIO_Pin){
      case BUTTON_SW1_PIN: ledToToggle = LED1; break;
      case BUTTON_SW2_PIN: ledToToggle = LED2; break;
      case BUTTON_SW3_PIN: ledToToggle = LED3; break;
      default: return;
    }
    UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON, CFG_SCH_PRIO_0);
  }
}

static void listenStart(void)
{
  uint8_t status;
  status = HAL_BLE_LLD_ReceivePacket(RX_TIMEOUT_US, listenEnd);
  if (status != SUCCESS_0){
    APP_DBG("ERROR: %s: HAL call failed", __func__);
  }
}

static void listenEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  userPayload *payload = data;
  Led_TypeDef led;
  APP_DBG("%s: event %s", __func__, eventToString(cmd));
  if (cmd == RX_OK_READY){
    if (size != sizeof(userPayload)){
      APP_DBG("%s: wrong payload length, packet discarded", __func__);
    }else{
      led = (Led_TypeDef)payload->led;
      if (led != LED1 && led != LED2 && led != LED3){
        APP_DBG("%s: wrong led value, packet discarded", __func__);
      }else{
        BSP_LED_Toggle(led);
        uartWrite("Led %d is Toggling", led);
      }
    }
  }
  listenStart();
}


static void sendStart(void)
{
  userPayload payload;

  // Need to interrupt radio while it is listening, then to reinit
  BLE_LLD_StopActivity();
  radioInit();

  payload.led = ledToToggle;
  HAL_BLE_LLD_SendPacket(&payload, sizeof(payload),sendEnd);
}

static void sendEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  APP_DBG("%s: event %s", __func__, eventToString(cmd));
  listenStart();
}

// Computes difference between old and new timestamps with rollover handling
static inline uint32_t timeDifference(uint32_t old, uint32_t new){
  if(new < old){
    // Rollover happened
    return UINT32_MAX - old + 1 + new;
  }else{
    return new - old;
  }
}

