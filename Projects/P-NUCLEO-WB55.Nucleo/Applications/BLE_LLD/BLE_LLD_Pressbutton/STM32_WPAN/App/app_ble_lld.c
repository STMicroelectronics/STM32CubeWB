/**
 ******************************************************************************
  * File Name          : app_ble_lld.c
  * Description        : PRESSBUTTON BLE LLD Application.
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "app_common.h"
#include "utilities_common.h"
#include "app_entry.h"
#include "dbg_trace.h"
#include "tl.h"
#include "shci.h"
#include "stm_logging.h"
#include "stm32_lpm.h"
#include "stm32_seq.h"
#include "gpio_lld.h"
#include "ble_lld.h"
#include "app_ble_lld.h"
#include "ring_buffer.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/*
 *  List of all errors tracked by the application
 *  running on M4. Some of these errors may be fatal
 *  or just warnings
 */
typedef enum
{
  ERR_BLE_LLD_SET_STATE_CB,
  ERR_BLE_LLD_ERASE_PERSISTENT_INFO,
  ERR_BLE_LLD_CHECK_WIRELESS
} ErrAppBleLldIdEnum_t;

/* Private defines -----------------------------------------------------------*/
#define UART_BUFFER_SIZE        64
#define TX_BUFFER_SIZE          268
#define UART_TX_CHUNK_SIZE      16
#define UART_LINE_END           "\r\n"

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void uartTxSendChunk(void);

/* Private variables ---------------------------------------------------------*/
BUF_ALLOC(uartTxBufMem, TX_BUFFER_SIZE);
static Buffer *uartTxBuf = (Buffer *)&uartTxBufMem;
static bool txBusy = false;

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_BLE_LLD_Config_t BleLldConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t BleLldM0CmdPacket;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t BleLldCmdRspPacket;


/* Functions Definition ------------------------------------------------------*/

void APP_BLE_LLD_Init(void)
{
  uint32_t devId = HAL_GetDEVID();
  uint32_t revId = HAL_GetREVID();
  uint8_t  param[8];
  SHCI_CmdStatus_t LldTestsInitStatus;
  
  /* Initialize transport layer */
  BleLldConfigBuffer.p_BleLldCmdRspBuffer = (uint8_t*)&BleLldCmdRspPacket;
  BleLldConfigBuffer.p_BleLldM0CmdBuffer = (uint8_t*)&BleLldM0CmdPacket;
  TL_BLE_LLD_Init(&BleLldConfigBuffer);

    /* Configure UART for receiving CLI command from PC and sending CLI response or notifications to PC */
  APP_BLE_LLD_Init_UART_CLI();

  /* Send LLD tests CLI start information to CLI UART */
  logUart("================================");
#ifdef STM32WB35xx
  logUart("Little DORY RF BLE LLD");
#else
  logUart("DORY RF BLE LLD");
#endif
  logUart("================================");
#if (CFG_DEBUGGER_SUPPORTED == 0U)
  logUart("Debugger de-activated");
#endif
#if (( CFG_DEBUG_TRACE_FULL == 0 ) && ( CFG_DEBUG_TRACE_LIGHT == 0 ))
  logUart("Trace is de-activated");
#endif

  APP_DBG("Test appli initialized on M4, waiting for M0 initialization");

  /* Send CLI start cmd to M0 (with device and revision ID as parameters */
  memcpy(&param[0], &devId, 4 );
  memcpy(&param[4], &revId, 4 );
  LldTestsInitStatus = SHCI_C2_BLE_LLD_Init(8, param);
  if(LldTestsInitStatus != SHCI_Success){
    APP_DBG("!! ERROR during M0 init !!");
  }else{
    APP_DBG("M0 initialized");
  }
}

/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */
void APP_BLE_LLD_Error(uint32_t ErrId, uint32_t ErrCode)
{
  char *msg;
  switch(ErrId)
  {
    case ERR_BLE_LLD_SET_STATE_CB:          msg = "ERROR: ERR_BLE_LLD_SET_STATE_CB";           break;
    case ERR_BLE_LLD_ERASE_PERSISTENT_INFO: msg = "ERROR: ERR_BLE_BLE_LLD_ERASE_PERSISTENT_INFO"; break;
    case ERR_BLE_LLD_CHECK_WIRELESS:        msg = "ERROR: ERR_BLE_LLD_CHECK_WIRELESS ";       break;
    default:                                msg = "ERROR Unknown ";                            break;
  }
  APP_DBG("**** Fatal error = %s (Err = %d)", msg, ErrCode);
  while(true)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500U);
  }
}

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Thread
 *        and display associated informations
 * @param  None
 * @retval None
 */
void CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t  wireless_info_instance;
  WirelessFwInfo_t* p_wireless_info = &wireless_info_instance;
  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success) 
  {
    APP_BLE_LLD_Error(ERR_BLE_LLD_CHECK_WIRELESS, 0);
  }
  else
  {
    APP_DBG("**********************************************************");
    APP_DBG("Loaded M0 TEST FW info:");
    switch(p_wireless_info->StackType)
    {
    case INFO_STACK_TYPE_802154_LLD_TESTS :
      APP_DBG("  M0 FW Type: 802.15.4 and radio LLDs tests");
      break;
    
    case INFO_STACK_TYPE_802154_PHY_VALID :
      APP_DBG("  M0 FW Type: 802.15.4 and radio PHY validation");
      break;
    
    case INFO_STACK_TYPE_BLE_PHY_VALID :
      APP_DBG("  M0 FW Type: BLE and radio PHY validation");
      break;
    
    default :
      APP_BLE_LLD_Error(ERR_BLE_LLD_CHECK_WIRELESS, 0);
      APP_DBG("  M0 FW Type: Unknown !!");
      break;
    }
    APP_DBG("  M0 FW VERSION: v%d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);
    APP_DBG("**********************************************************");
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
/**
 * @brief Perform initialization of CLI UART interface.
 * @param  None
 * @retval None
 */
void APP_BLE_LLD_Init_UART_CLI(void)
{
#if (CFG_HW_USART1_ENABLED == 1)
  MX_USART1_UART_Init();
#endif
  bufInit(uartTxBuf, TX_BUFFER_SIZE);
  txBusy = false;
}

/**
 * @brief Perform de-initialization of CLI UART interface.
 * @param  None
 * @retval None
 */
void APP_BLE_LLD_DeInit_UART_CLI(void)
{
#if (CFG_HW_USART1_ENABLED == 1)
  MX_USART1_UART_DeInit();
#endif
}

void logUart(const char *format, ...)
{
  char out[UART_BUFFER_SIZE];
  int nbChar;
  va_list argp;
  va_start(argp, format);
  nbChar = vsnprintf(out, sizeof(out), format, argp);
  va_end(argp);
  if (nbChar < 0){
    return;
  }
  if (nbChar > (sizeof(out) - ((strlen(UART_LINE_END) + 1)))){
    strcpy(&(out[sizeof(out) - (strlen(UART_LINE_END) + 1)]), UART_LINE_END);
  }else{
    strcat(out, UART_LINE_END);
  }
  logUartRaw(out);
}

void logUartRaw(const char *str)
{
  CRITICAL_BEGIN();
  bufPutString(uartTxBuf, str);
  if (! txBusy){
    uartTxSendChunk();
  }
  CRITICAL_END();
}

// Send multiple chars through the UART
// must be called inside critical section
// loop on itself via the UART callback
static void uartTxSendChunk(void){
  static char hwBuf[UART_TX_CHUNK_SIZE];
  uint32_t count;
  count = bufGetMultiChar(uartTxBuf, hwBuf, UART_TX_CHUNK_SIZE);
  if (count != 0){
    txBusy = true;
    if (HW_UART_Transmit_IT(CFG_CLI_UART, (uint8_t *)hwBuf, count, uartTxSendChunk) != hw_uart_ok){
      APP_DBG("!! HAL_UART_Transmit_IT error on M4");
    }
  }else{
    txBusy = false;
  }
}

/**
 * @brief  This function is called when notification on TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_CmdPacket_t
 * @return  None
 */
void TL_BLE_LLD_ReceiveRsp( TL_CmdPacket_t * Notbuffer )
{
  uint8_t l_size = Notbuffer->cmdserial.cmd.plen;
  char *sourceBuf = (char *)Notbuffer->cmdserial.cmd.payload;
  
  if (l_size > 0)
  {
    if (strcmp(sourceBuf, "Resp_End") == 0)
    {
      /* This is an answer to indicate that command has been completed */
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_RSPACKEVT);
    }
   else
    {
      /* This is just a trace from M0, write to UART */
      logUartRaw(sourceBuf);
    }
  }
  else
  {
    APP_DBG("!! Empty M0 CLI response received by M4 !!");
  }
  TL_BLE_LLD_SendRspAck();
}

/**
 * @brief  This function is called when notification on TL Channel from M0+ is received.
 *
 * @param   cmdBuffer : a pointer to TL_CmdPacket_t
 * @return  None
 */
void TL_BLE_LLD_ReceiveM0Cmd( TL_CmdPacket_t * cmdBuffer )
{
  uint8_t bufferSize = cmdBuffer->cmdserial.cmd.plen;
  char *  bufferAddr = (char *)cmdBuffer->cmdserial.cmd.payload;

  if (bufferSize > 0) {
    if (BLE_LLD_PRX_ReplyInterDispatch(bufferAddr)) {
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }else{
      APP_DBG((char *)"!! Unknown M0 command received by M4 !!");
    }
  }else{
    APP_DBG((char *)"!! Empty M0 command received by M4 !!");
  }
  TL_BLE_LLD_SendM0CmdAck();
}

/**
 * @brief This function is called to Send Command to M0
 *
 * @param[in] command BLE command already packed (by LLD)
 */
void APP_BLE_LLD_SendCmdM0(void *command)
{
  bleCmdIndirect_t *cmdIndirect = (bleCmdIndirect_t *)BleLldCmdRspPacket.cmdserial.cmd.payload;
  cmdIndirect->command = command;
  cmdIndirect->length = 5;

  BleLldCmdRspPacket.cmdserial.cmd.plen = sizeof(bleCmdIndirect_t);
  BleLldCmdRspPacket.cmdserial.cmd.cmdcode = 0x0;

  UTIL_SEQ_ClrEvt(1U << CFG_EVT_RECEIVE_RSPACKEVT);
  TL_BLE_LLD_SendCmd();
  UTIL_SEQ_WaitEvt(1U << CFG_EVT_RECEIVE_RSPACKEVT);
}

/* USER CODE END FD_WRAP_FUNCTIONS */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
