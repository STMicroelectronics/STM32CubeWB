/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : chat_app.c
  * Description        : CHAT LLD BLE Application.
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

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "utilities_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "ble_lld.h"
#include "ble_hal.h"
#include "app_ble_lld.h"
#include "tl.h"
#include "shci.h"
#include "stm_logging.h"
#include "stm32_lpm.h"
#include "stm32_seq.h"
#include "gpio_lld.h"
#include "chat_app.h"

/* Private includes -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

typedef PACKED_STRUCT
{
  char message[200];
} userPayloadMsg;

// Defines user data format in payload
typedef PACKED_STRUCT
{
  uint8_t type;
  union{
    userPayloadMsg msg;
  } __attribute__((packed)) content;
} userPayload;


/* Private defines -----------------------------------------------------------*/
#define CHANNEL           12            // Radio channel
#define POWER             TX_POW_PLUS_6_DB // Transmit power
#define NET_ID            0xB55A54AA    // Network ID (address) must be the same for the two boards
#define RX_TIMEOUT_US     (5*1000*1000) // max delay radio will listen for a packet
#define RX_ACK_TIMEOUT_US 1000          // Timeout for acknowledge reception

#define PROMPT "BLE LLD > "

enum{
  PAYLOAD_MSG,
  PAYLOAD_ACK,
};

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void CHAT_Led(void);
static void CHAT_ToggleEncrypt(void);
static void sendPacketStart(char *text);
static void sendPacketEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);
static void receivePacketStart(void);
static void receivePacketEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size);

static void uartRxBufferProcess(void);
static void uartRxCallback(char received);

static void radioInit(void);

static uint8_t payloadMsgPrepare(userPayload *payload, char *text);
static bool payloadMsgCheck(userPayload *payload);
static void payloadMsgExtract(userPayload *payload, char *text);
static uint8_t payloadAckPrepare(userPayload *payload);
static bool payloadAckCheck(userPayload *payload);

/* Private variables -----------------------------------------------*/
/* LED */
static bool chatCryptoLed  = false;
static bool chatTxLed      = false;
static bool chatRxLed     = false;


/* Encryption */
static const uint8_t chatcountTx[5]     = {0x00,0x00,0xAF,0x00,0x08};
static const uint8_t chatcountRx[5]    = {0x00,0x00,0xAF,0x00,0x08};
// Key must be kept secret and unique for a given set of devices communicating with each other
static const uint8_t chatenc_key[16]     = {0xBF,0x01,0xFB,0x9D,0x4E,0xF3,0xBC,0x36,0xD8,0x74,0xF5,0x39,0x41,0x38,0x68,0x56};
// IV must be chosen randomly at the beginning of each secure session
static const uint8_t chatencIv[8]       = {0x00,0x00,0xAF,0x00,0x08,0x00,0x00,0x00};

/* Parameters */
static bool chatEncrypt = true;

/* Variables used for UART reception */
static char uartRxChar;
static char uartRxBuf[IPBLE_BLE_LLDANT_MAX_PAYLOAD_SIZE];

/* Functions Definition ------------------------------------------------------*/

void CHAT_APP_Init(void)
{
  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  CheckWirelessFirmwareInfo();

  /* Do not allow standby in the application */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE);
  /* Disable low power mode for now, may be enable later depending on configuration */
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE_LLD, UTIL_LPM_DISABLE );

  /* Register tasks for event processing */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_ENCRYPT , UTIL_SEQ_RFU, CHAT_ToggleEncrypt);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PROCESS_UART_RX_BUFFER, UTIL_SEQ_RFU, uartRxBufferProcess);

  APP_BLE_LLD_Init();


  radioInit();
  uartWrite("");
  uartWrite("************ ID: 0x%X / channel: %u **************", NET_ID, CHANNEL);
  uartWriteRaw(PROMPT);

  APP_BLE_LLD_uartRxStart(uartRxCallback);
  receivePacketStart();
}

static void radioInit(void)
{
  HAL_BLE_LLD_Init(CFG_HS_STARTUP_TIME, true);
  HAL_BLE_LLD_Configure(POWER, CHANNEL, true, CFG_BACK2BACK_TIME, NET_ID);
  
/* Encryption Parameters */
  if (chatEncrypt) {
    BLE_LLD_SetEncryptFlags(STATE_MACHINE_0, ENABLE);
    BLE_LLD_SetEncryptionAttributes(STATE_MACHINE_0, &chatencIv, &chatenc_key);
    BLE_LLD_SetEncryptionCount(STATE_MACHINE_0, &chatcountTx, &chatcountRx);
    chatCryptoLed = true;
    CHAT_Led();
  }else{
    BLE_LLD_SetEncryptFlags(STATE_MACHINE_0, DISABLE);
    chatCryptoLed = false;
    CHAT_Led();
  }
}

// Stores the received character and schedules its processing
static void uartRxCallback(char received)
{
  uartRxChar = received;
  UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_BUFFER, CFG_SCH_PRIO_0);
}

// Processes received character (send packet when line complete)
static void uartRxBufferProcess(void)
{
  static uint8_t uartRxBufIdx = 0;
  if (uartRxChar == '\n' || uartRxChar == '\r'){
    uartRxBuf[uartRxBufIdx] = '\0';
    uartRxBufIdx = 0;
    sendPacketStart(uartRxBuf);
  } else if (uartRxBufIdx < (sizeof(uartRxBuf) - 1)){
    char echo[2];
    uartRxBuf[uartRxBufIdx] = uartRxChar;
    uartRxBufIdx++;
    // Send echo
    echo[0] = uartRxChar;
    echo[1] = '\0';
    uartWriteRaw(echo);
  }else{
  }
}

/* Appli common functions */


/* Appli custom functions */
static void CHAT_Led(void) {
  aPwmLedGsData_TypeDef led_Chat;

  led_Chat[PWM_LED_RED]   = chatTxLed ? PWM_LED_GSDATA_7_0:PWM_LED_GSDATA_OFF;
  led_Chat[PWM_LED_GREEN] = chatRxLed ? PWM_LED_GSDATA_7_0:PWM_LED_GSDATA_OFF;
  led_Chat[PWM_LED_BLUE]  = chatCryptoLed ? PWM_LED_GSDATA_7_0:PWM_LED_GSDATA_OFF;
  LED_On(led_Chat);
}

static void CHAT_ToggleEncrypt(void)
{
  BLE_LLD_StopActivity();
  chatEncrypt = !chatEncrypt;
  // Radio was destroyed by StopActivity
  radioInit();
  uartWrite("");
  uartWrite("************ %s **************", chatEncrypt ? "Encrypted" : "UnEncrypted");
  uartWriteRaw(PROMPT);
  receivePacketStart();
}

static void sendPacketStart(char *text)
{
  userPayload payload;
  uint8_t payloadSize;
  BLE_LLD_StopActivity();
  radioInit();

  payloadSize = payloadMsgPrepare(&payload, uartRxBuf);
  HAL_BLE_LLD_SendPacketWithAck(&payload,
                                payloadSize,
                                RX_ACK_TIMEOUT_US,
                                sendPacketEnd);
}

static void sendPacketEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  userPayload *payload = data;

  if (cmd == RX_OK_READY){
    if (! payloadAckCheck(payload)){
    }else{
      uartWrite("");
      uartWriteRaw(PROMPT);
    }
  }else if (cmd != TX_OK_BUSY){
    uartWrite("");
    uartWrite("ERROR: message delivery failed");
    uartWriteRaw(PROMPT);
  }
  if (RADIO_IS_READY(cmd)){
    receivePacketStart();
  }
}

static void receivePacketStart(void)
{
  uint8_t status;
  userPayload payload;
  uint8_t payloadSize;
  payloadSize = payloadAckPrepare(&payload);
  status = HAL_BLE_LLD_ReceivePacketWithAck(&payload,
                                            payloadSize,
                                            RX_TIMEOUT_US,
                                            receivePacketEnd);
  if (SUCCESS_0 != status){
  }
}

static void receivePacketEnd(radioEventType cmd, ActionPacket *ap, void *data, uint8_t size)
{
  char display[sizeof(userPayloadMsg)];
  userPayload *payload = data;
  // Display data as soon as it is received (ACK is still being sent)
  if (cmd == RX_OK_BUSY){
    if (!payloadMsgCheck(payload)){
    }else{
      payloadMsgExtract(payload, display);
      uartWriteRaw(display);
      uartWrite("");
      uartWriteRaw(PROMPT);
    }
  }
  if (RADIO_IS_READY(cmd)){
    receivePacketStart();
  }
}

static uint8_t payloadMsgPrepare(userPayload *payload, char *text)
{
  payload->type = PAYLOAD_MSG;
  strncpy(payload->content.msg.message, uartRxBuf, (sizeof(userPayloadMsg) - 1));
  payload->content.msg.message[sizeof(userPayloadMsg) - 1] = '\0';
  // Be careful to update computed size if structure changes
  return (sizeof(userPayload) - sizeof(userPayloadMsg) +
          strlen(payload->content.msg.message) + 1);
}

static bool payloadMsgCheck(userPayload *payload)
{
  return (payload->type == PAYLOAD_MSG);
}

static void payloadMsgExtract(userPayload *payload, char *text)
{
  strncpy(text, payload->content.msg.message, (sizeof(userPayloadMsg) - 1));
  text[sizeof(userPayloadMsg) - 1] = '\0';
}

static uint8_t payloadAckPrepare(userPayload *payload)
{
  payload->type = PAYLOAD_ACK;
  // Be careful to update computed size if structure changes
  return (sizeof(userPayload) - sizeof(userPayloadMsg));
}

static bool payloadAckCheck(userPayload *payload)
{
  return (payload->type == PAYLOAD_ACK);
}


/* USER CODE END FD */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  switch (GPIO_Pin)
  {
    case BUTTON_USER1_PIN:
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_ENCRYPT, CFG_SCH_PRIO_0);
      break;

    case BUTTON_USER2_PIN:
      break;

    default:
      break;
  }
  return;
}

