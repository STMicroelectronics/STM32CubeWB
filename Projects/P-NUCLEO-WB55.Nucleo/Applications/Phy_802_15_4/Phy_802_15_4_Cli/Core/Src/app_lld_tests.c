/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_lld_tests.c
 * Description        : RF LLDs tests Application.
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
#include "app_lld_tests.h"
#include "tl.h"
#include "shci.h"
#include "stm_logging.h"
#include "stm32_lpm.h"
#include "stm32_lpm_if.h"
#include "stm32_seq.h"
#include "gpio_lld.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define CLI_CMD_BUFFER_SIZE     1024U
#define CLI_CMD_HISTORY_LENGTH  10U
#define UART_TX_BUFFER_SIZE     4096U
// Delay to let M4 flushing traces or CLI printf : a delay is already set on M0 side and seems enough
#define DELAY_BEFORE_WFI_ON_M4  10U

/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
static void APP_LLDTESTS_CheckWirelessFirmwareInfo(void);
static void APP_LLDTESTS_TraceError(const char * pMess, uint32_t ErrCode);

static void SendCliCmdToM0(void);
static void SendCliRspAckToM0(void);
static void SendM0CmdAckToM0(void);

static void uartRxCpltCallback(void);
static void uartRxItProcess(void);
static void uartRxBufferProcess(void);
static void uartClearChars(int nbChar);
static void uartTxBufferAdd(const char * str);
static void uartTxLlAdd(const char * str);
static void uartTxItProcess(void);
static void uartTxCpltCallback(void);

static void m0CmdProcess(void);
static void m0CmdStopRequired(uint32_t stopRequired);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Global variables -----------------------------------------------*/

/* Private variables -----------------------------------------------*/
typedef enum 
{
  CMD_FROM_M0_STOP0_ON           = 0,
  CMD_FROM_M0_STOP1_ON           = 1,
  CMD_FROM_M0_STOP2_ON           = 2,
  CMD_FROM_M0_STOP_OFF           = 3,
  CMD_FROM_M0_GO_IN_WFI          = 4,
  CMD_FROM_M0_GO_DIRECTLY_IN_WFI = 5,
  CMD_FROM_M0_USE_MSI            = 6,
  CMD_FROM_M0_USE_MSI_HSE        = 7,
  CMD_FROM_M0_USE_MSI_PLL        = 8,
  CMD_FROM_M0_USE_MSI_PLL_HSE    = 9,
  CMD_FROM_M0_USE_HSE            = 10,
  CMD_FROM_M0_USE_HSE_PLL        = 11,
  CMD_FROM_M0_EXT_PA_EN          = 12,
  CMD_FROM_M0_EXT_PA_DIS         = 13,
  CMD_FROM_M0_GENE_HARD_FAULT    = 14,
  CMD_FROM_M0_HSE_TUNE           = 15,
  CMD_FROM_M0_HSE_TUNE_DEF       = 16,
} cmdFromM0_t;
static cmdFromM0_t m0Cmd = CMD_FROM_M0_STOP_OFF;

enum 
{
  CHAR_CTRLC = 3,
  CHAR_ESC   = 27,
  CHAR_DEL   = 8
};

static uint8_t  txBusy;
static uint8_t  rxCmdAllowed = 0;
static uint8_t  txBuffer[UART_TX_BUFFER_SIZE+1];
static uint32_t txBuffer_wrPtr;
static uint32_t txBuffer_rdPtr;
static uint8_t *pTxBuff_currentWr;
static uint32_t txBufferFullCount;
static uint8_t  rxBuffer[CLI_CMD_BUFFER_SIZE];
static uint32_t rxBuffer_wrPtr;
static uint32_t rxBuffer_rdPtr;
static uint8_t  currentCommand[CLI_CMD_BUFFER_SIZE];
static uint32_t currentCommandPos;
static uint8_t  commandHistory[CLI_CMD_HISTORY_LENGTH][CLI_CMD_BUFFER_SIZE];
static int      commandHistoryIdx;
static int      commandHistoryIdxSav;
static char     cliPrompt[30] = "Unknown M0 appli > ";
static char *   CmdFromM0Array[2];

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_LLD_tests_Config_t LldTestsConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t LldTestsM0CmdPacket;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t LldTestsCliCmdRspPacket;

/* USER CODE BEGIN PV */

/* Functions Definition ------------------------------------------------------*/

void APP_LLDTESTS_Init( void )
{
  uint32_t devId = HAL_GetDEVID();
  uint32_t revId = HAL_GetREVID();
  uint8_t  param[8];
  char     traceBuff[50];
  
  /* USER CODE BEGIN APP_LLDTESTS_INIT_1 */
  /* Do not allow stop mode before Thread is initialized */
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_LLDTESTS, UTIL_LPM_DISABLE);
  /* USER CODE END APP_LLDTESTS_INIT_1 */

  SHCI_CmdStatus_t LldTestsInitStatus;
  
  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_LLDTESTS_CheckWirelessFirmwareInfo();
  
  /**
   * Do not allow standby in the application
   */
  UTIL_LPM_SetOffMode(1U << CFG_LPM_APP_LLDTESTS, UTIL_LPM_DISABLE);
  
  
  /* Init config buffer and call TL_LLDTESTS_Init */
  APP_LLDTESTS_Init_TL();
  
  /* Create a task to send CLI commands to M0 via IPCC */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SEND_CLI_TO_M0, UTIL_SEQ_RFU, SendCliCmdToM0);
  
  /* Create a task to process data received from UART and create CLI commands */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PROCESS_UART_RX_BUFFER, UTIL_SEQ_RFU, uartRxBufferProcess);
  
  /* Create tasks to process interrupt from/to UART to avoid locking UART during IT processing */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PROCESS_UART_RX_IT, UTIL_SEQ_RFU, uartRxItProcess);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PROCESS_UART_TX_IT, UTIL_SEQ_RFU, uartTxItProcess);
  
  /* Create a task to manage commands from M0 */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_CMD_FROM_M0_TO_M4, UTIL_SEQ_RFU, m0CmdProcess);
  
  /* Configure UART for receiving CLI command from PC and sending CLI response or notifications to PC */
  txBusy = 0;
  rxCmdAllowed = 0;
  txBuffer[UART_TX_BUFFER_SIZE] = 0;
  txBuffer_wrPtr = 0;
  txBuffer_rdPtr = 0;
  rxBuffer_wrPtr = 0;
  rxBuffer_rdPtr = 0;
  currentCommandPos = 0;
  commandHistoryIdx = 0;
  commandHistoryIdxSav = 0;
  APP_LLDTESTS_Init_UART_CLI();
  
  /* Send LLD tests CLI start information to CLI UART */
  uartTxBufferAdd("\r\n\n=================================\r\n");
#ifdef STM32WB35xx
  sprintf(traceBuff, "(STM32WB35)");
#elif STM32WB55xx
  sprintf(traceBuff, "(STM32WB55)");
#else
#error Unnknown target
#endif
  sprintf(traceBuff, "%s RF 802_15_4 LLDs test\r\n", strtok(traceBuff, "\0"));
  uartTxBufferAdd(traceBuff);
  uartTxBufferAdd("=================================\r\n");
  
#if (CFG_LPM_SUPPORTED == 1U)
  uartTxBufferAdd("Low-power manager is activated\r\n");
#endif
#if (CFG_DEBUGGER_SUPPORTED == 0U)
  uartTxBufferAdd("Debugger de-activated\r\n");
#endif
#if (( CFG_DEBUG_TRACE_FULL == 0 ) && ( CFG_DEBUG_TRACE_LIGHT == 0 ))
  uartTxBufferAdd("Trace is de-activated\r\n");
#endif
  
  /* Debug initialization */
#ifdef COMMON_RF_DBG_GPIO
  gpio_lld_phy_dbg_init();
#endif
  
#if 0
  APP_DBG("EXTI status");
  sprintf(traceBuff, "  IMR1 0x%08X   IMR2 0x%08X", EXTI->IMR1, EXTI->IMR2);
  APP_DBG(traceBuff);
  sprintf(traceBuff, "  EMR1 0x%08X   EMR2 0x%08X", EXTI->EMR1, EXTI->EMR2);
  APP_DBG(traceBuff);
  sprintf(traceBuff, "C2IMR1 0x%08X C2IMR2 0x%08X", EXTI->C2IMR1, EXTI->C2IMR2);
  APP_DBG(traceBuff);
  sprintf(traceBuff, "C2EMR1 0x%08X C2EMR2 0x%08X", EXTI->C2EMR1, EXTI->C2EMR2);
  APP_DBG(traceBuff);
#endif
  
#if 0
  // Read the RCC_CR register
  APP_DBG( "RCC_CR (0x%08X) = 0x%08X", (uint32_t)&(RCC->CR), RCC->CR);
  // RCC CFGR register
  APP_DBG( "RCC_CFGR (0x%08X) = 0x%08X", (uint32_t)&(RCC->CFGR), RCC->CFGR);
  // PLL_CFG register
  APP_DBG( "RCC_PLLCFGR (0x%08X) = 0x%08X", (uint32_t)&(RCC->PLLCFGR), RCC->PLLCFGR);
#endif
  
  /* Indicate end of M4 initialization */
  APP_DBG("Test appli initialized on M4, wait for M0 initialization");
  
  /* Send CLI start cmd to M0 (with device and revision ID as parameters */
  memcpy(&param[0], &devId, 4 );
  memcpy(&param[4], &revId, 4 );
  LldTestsInitStatus = SHCI_C2_LLDTESTS_Init(8, param);
  if(LldTestsInitStatus != SHCI_Success)
    APP_DBG((char *)"!! ERROR during M0 init !!");
  
  /* M0 init done, send first command to have M0 code info and thus, a first prompt will be printed automatically */
  /* Do not accept new command from UART until this one is managed */
  rxCmdAllowed = 0;
  currentCommandPos = 4;
  currentCommand[0] = 'i';
  currentCommand[1] = 'n';
  currentCommand[2] = 'f';
  currentCommand[3] = 'o';
  currentCommand[4] = 0;
  /* Set corresponding task to send this command to M0 */
  UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_CLI_TO_M0, CFG_SCH_PRIO_0);
  
  /* Activate UART RX buffer processing task to allow USER command coming from UART */
  UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_BUFFER, CFG_SCH_PRIO_0);
  
  /* USER CODE BEGIN APP_LLDTESTS_INIT_2 */
  /* Allow stop mode after Thread initialization*/
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_LLDTESTS, UTIL_LPM_ENABLE);
  /* USER CODE END APP_LLDTESTS_INIT_2 */
}

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */
void APP_LLDTESTS_Error(uint32_t ErrId, uint32_t ErrCode)
{
  /* USER CODE BEGIN APP_LLDTESTS_Error_1 */

  /* USER CODE END APP_LLDTESTS_Error_1 */
  switch(ErrId)
  {
  case ERR_LLDTESTS_SET_STATE_CB :
    APP_LLDTESTS_TraceError("ERROR : ERR_LLDTESTS_SET_STATE_CB ",ErrCode);
    break;
    
  case ERR_LLDTESTS_ERASE_PERSISTENT_INFO :
    APP_LLDTESTS_TraceError("ERROR : ERR_LLDTESTS_ERASE_PERSISTENT_INFO ",ErrCode);
    break;
    
  case ERR_LLDTESTS_CHECK_WIRELESS :
    APP_LLDTESTS_TraceError("ERROR : ERR_LLDTESTS_CHECK_WIRELESS ",ErrCode);
    break;
    
  /* USER CODE BEGIN APP_LLDTESTS_Error_2 */
  
  /* USER CODE END APP_LLDTESTS_Error_2 */
    
  default :
    APP_LLDTESTS_TraceError("ERROR Unknown ", 0);
    break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (OpenThread or other module if any)
  * @retval None
  */
static void APP_LLDTESTS_TraceError(const char * pMess, uint32_t ErrCode)
{
  /* USER CODE BEGIN TRACE_ERROR */
  APP_DBG("**** Fatal error = %s (Err = %d)", pMess, ErrCode);
  while(1U == 1U)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500U);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500U);
  }
  /* USER CODE END TRACE_ERROR */
}

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Thread
 *        and display associated information
 * @param  None
 * @retval None
 */
static void APP_LLDTESTS_CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t wireless_info_instance;
  WirelessFwInfo_t* p_wireless_info = &wireless_info_instance;
  char              m0FwType[50] = "M0 FW Type : Unknown !! ";

  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success)
  {
    APP_LLDTESTS_Error((uint32_t)ERR_LLDTESTS_CHECK_WIRELESS, 0);
  }
  else
  {
    APP_DBG("**********************************************************");
    APP_DBG("Loaded M0 TEST FW info :");
    switch(p_wireless_info->StackType)
    {
    case INFO_STACK_TYPE_802154_LLD_TESTS :
      sprintf(m0FwType, "  M0 FW Type : 802.15.4 and radio LLDs tests");
      sprintf(cliPrompt, "802.15.4 TESTS > ");
      break;
    
    case INFO_STACK_TYPE_802154_PHY_VALID :
      sprintf(m0FwType, "  M0 FW Type : 802.15.4 and radio PHY validation");
      sprintf(cliPrompt, "802.15.4 valid cli > ");
      break;
    
    case INFO_STACK_TYPE_BLE_PHY_VALID :
      sprintf(m0FwType, "  M0 FW Type : BLE and radio PHY validation");
      sprintf(cliPrompt, "BLE valid cli > ");
      break;
    
    case INFO_STACK_TYPE_BLE_LLD_TESTS :
      sprintf(m0FwType, "  M0 FW Type : BLE and radio LLDs tests");
      sprintf(cliPrompt, "BLE TESTS > ");
      break;
        
    default :
      /* FW not supported */
      APP_LLDTESTS_Error((uint32_t)ERR_LLDTESTS_CHECK_WIRELESS, 0);
      break;
    }
    APP_DBG(m0FwType);
    
    /* Print version */
    APP_DBG("  M0 FW VERSION = v%d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);
    
    APP_DBG("**********************************************************");
  }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

/* USER CODE END FD_LOCAL_FUNCTIONS */

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
void APP_LLDTESTS_Init_UART_CLI(void)
{
#if (CFG_HW_USART1_ENABLED == 1)
  MX_USART1_UART_Init();
  
  /* Put the UART device in reception mode and wait for interrupt */
  if (HW_UART_Receive_IT(CFG_CLI_UART, &rxBuffer[rxBuffer_wrPtr], 1, uartRxCpltCallback) != hw_uart_ok)
    APP_DBG((char *)"!! HAL_UART_Receive_IT error on M4 in APP_LLDTESTS_Init_UART_CLI !!");
#endif
}

/**
 * @brief Perform de-initialization of CLI UART interface.
 * @param  None
 * @retval None
 */
void APP_LLDTESTS_DeInit_UART_CLI(void)
{
#if (CFG_HW_USART1_ENABLED == 1)
  MX_USART1_UART_DeInit();
#endif
}

static void uartRxCpltCallback(void)
{
  /* This callback is called :
       - during RX isr (huart->RxISR)
       - after bytes copy in buffer specified in last call to HW_UART_Receive_IT()
       - when number of bytes specified in last call to HW_UART_Receive_IT() is achieved */
  
  /* Prepare buffer to receive next character */
  if ( rxCmdAllowed ) {
    /* Increment read pointer index and manage buffer rollover */
    rxBuffer_wrPtr++;
    if(rxBuffer_wrPtr >= CLI_CMD_BUFFER_SIZE)
      rxBuffer_wrPtr = 0;
  }
  
  /* Re-put UART device in reception mode (not during IT to avoid locking the device while perhaps in transmit phase) */
  UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_IT, CFG_SCH_PRIO_0);
}

static void uartRxItProcess(void)
{
  /* Put the UART device in reception mode and wait for interrupt */
  if (HW_UART_Receive_IT(CFG_CLI_UART, &rxBuffer[rxBuffer_wrPtr], 1, uartRxCpltCallback) != hw_uart_ok)
    APP_DBG((char *)"!! HAL_UART_Receive_IT error on M4 in uartRxItProcess !!");
}

static void uartClearChars(int nbChar)
{
  char clearCmd[2];
  
  clearCmd[0] = CHAR_DEL;
  clearCmd[1] = 0;
  for (int i = 0; i < nbChar; i++) {
    uartTxLlAdd(clearCmd);
  }
}

static void uartRxBufferProcess( void )
{
  char last_char[2];
  static int8_t escape = -1;
  static uint8_t escape_seq[3] = {0};
  
  /* Ensure that last_char buffer contains a string with the char and a \0 (useful to send echo or CHAR_DEL to UART) */
  last_char[1] = 0;
  
  /* Interpret the character reception from UART (only if rxCmdAllowed is activated and read pointer reached write one)*/
  while (rxBuffer_rdPtr != rxBuffer_wrPtr ) {
    last_char[0] = rxBuffer[rxBuffer_rdPtr];
    if( escape >= 0 ) {
      escape_seq[escape++] = last_char[0];
      if ( escape == 2 ) {
        if ( strcmp((char*)escape_seq, "[A") == 0 ) { // UP
          uartClearChars(currentCommandPos);
          commandHistoryIdx = (commandHistoryIdx > 0) ? (commandHistoryIdx - 1) : (CLI_CMD_HISTORY_LENGTH - 1);
          strcpy((char*)currentCommand,(char*)commandHistory[commandHistoryIdx]);
          currentCommandPos = strlen((char*)currentCommand);
          uartTxLlAdd((char*)currentCommand);
        }
        if ( strcmp((char*)escape_seq, "[B") == 0 ) { // DOWN
          uartClearChars(currentCommandPos);
          commandHistoryIdx = (commandHistoryIdx < (CLI_CMD_HISTORY_LENGTH - 1)) ? (commandHistoryIdx + 1) : 0;
          strcpy((char*)currentCommand,(char*)commandHistory[commandHistoryIdx]);
          currentCommandPos = strlen((char*)currentCommand);
          uartTxLlAdd((char*)currentCommand);
        }
        escape = -1;
      }
    }
    else if ( last_char[0] == CHAR_DEL )  {
      if ( currentCommandPos > 0 ) {
        currentCommandPos--;
        uartTxLlAdd(last_char);
      }
    }
    else if ( last_char[0] == CHAR_ESC )  {
      escape = 0;
    }
    else if ( last_char[0] == CHAR_CTRLC )  {
      /* Just send a new prompt to wait for a new command */
      currentCommandPos = 0;
      uartTxLlAdd("\r\n");
      uartTxLlAdd(cliPrompt);
    }
    else if ( ( last_char[0] == '\r' ) || ( last_char[0] == '\n' ))  {
      if (currentCommandPos == 0) {
        /* User just typed 'enter' without any command, so, just send a new prompt to wait for a new command */
        uartTxLlAdd("\r\n");
        uartTxLlAdd(cliPrompt);
      } else {
        /* Do not accept new command from UART until this one is managed */
        rxCmdAllowed = 0;
        
        /* Put a end of line in UART TX buffer to have the echo sent */
        uartTxLlAdd("\r\n");
        /* add a \0 to indicate end of command to M0 and to have it in command history (needed to use strlen()) */
        currentCommand[currentCommandPos++] = 0;
     	
        /* Copy the current command in history buffer before to send it to M0 */
        memcpy(commandHistory[commandHistoryIdxSav],currentCommand,currentCommandPos);
        commandHistoryIdxSav = (commandHistoryIdxSav+1) % CLI_CMD_HISTORY_LENGTH;
        commandHistoryIdx = commandHistoryIdxSav;
        
        /* UART task scheduling to send it to M0 */
        UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_CLI_TO_M0, CFG_SCH_PRIO_0);
        
      }
    }
    else {
      /* Put the char in UART TX buffer to have the echo sent */
      uartTxLlAdd(last_char);
      /* Put the char in the current command buffer */
      currentCommand[currentCommandPos++] = last_char[0];
    }
    
    /* Increment read pointer index and manage buffer rollover */
    rxBuffer_rdPtr++;
    if(rxBuffer_rdPtr >= CLI_CMD_BUFFER_SIZE)
      rxBuffer_rdPtr = 0;
  }
  
  /* Re-activate UART RX buffer processing task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_BUFFER, CFG_SCH_PRIO_0);
}

static void uartTxLlAdd(const char * str)
{
  uint16_t bytesToWrite = strlen(str);
  
  if (bytesToWrite) {
    uint16_t  remainingBytesToWrite = bytesToWrite;
    
    while (remainingBytesToWrite > 0) {
      HW_UART_Transmit_LL(CFG_CLI_UART, str[bytesToWrite - remainingBytesToWrite]);
      remainingBytesToWrite--;
    }
  }
}

static void uartTxBufferAdd(const char * str)
{
  uint16_t bytesToWrite = strlen(str);
  uint32_t txBufferFull = 0;
  
  if (bytesToWrite) {
    CRITICAL_BEGIN();
    {
      uint16_t  remainingBytesToWrite = bytesToWrite;
      uint32_t  currentWrPtr  = txBuffer_wrPtr;
      uint8_t * currentWrAddr = &txBuffer[txBuffer_wrPtr];
      
      while ((remainingBytesToWrite > 0) && (txBufferFull == 0)) {
        if ((txBuffer_rdPtr == (currentWrPtr + 1)) || ((txBuffer_rdPtr == 0) && (currentWrPtr == (UART_TX_BUFFER_SIZE - 1)))) {
          /* If there is not enough place (i.e. write pointer is just behind read pointer in the circular buffer), 
             buffer is full, so do not consider new string */
          /* !! No trace here as it is under CRITICAL section */
          txBufferFull = 1;
        } else {
          *currentWrAddr = str[bytesToWrite - remainingBytesToWrite];
          remainingBytesToWrite--;
          
          /* Increment write pointer index and manage buffer rollover */
          currentWrPtr++;
          if(currentWrPtr >= UART_TX_BUFFER_SIZE)
            currentWrPtr = 0;
          currentWrAddr = &txBuffer[currentWrPtr];
        }
      }
      
      if (txBufferFull == 0) {
        txBuffer_wrPtr = currentWrPtr;
      }
    }
    CRITICAL_END();
    
    if (txBufferFull == 0) {
      if (txBusy == 0) {
        txBusy = 1;
        uartTxCpltCallback();
      }
    } else {
      txBufferFullCount++;
      APP_DBG((char *)"!! TX buffer full : %u", txBufferFullCount);
    }
  }
}

static void uartTxCpltCallback(void)
{
  /* Prepare buffer to receive next character */
  if ( txBuffer_rdPtr != txBuffer_wrPtr ) {
    /* Re-put UART device in reception mode (not during IT to avoid locking the device while perhaps in transmit phase) */
    pTxBuff_currentWr = &txBuffer[txBuffer_rdPtr];
    UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_TX_IT, CFG_SCH_PRIO_0);
    
    /* Increment write pointer index and manage buffer rollover */
    txBuffer_rdPtr++;
    if(txBuffer_rdPtr >= UART_TX_BUFFER_SIZE)
      txBuffer_rdPtr = 0;
  } else {
    txBusy = 0;
  }
}

static void uartTxItProcess(void)
{
  /* Put the UART device in transmission mode and wait for interrupts :
       - 1 to write the data in TDR register 
       - 1 when char is sent
       - 1 to indicate end of transmit if several chars are to be transmitted */
  if (HW_UART_Transmit_IT(CFG_CLI_UART, pTxBuff_currentWr, 1, uartTxCpltCallback) != hw_uart_ok)
    APP_DBG((char *)"!! HAL_UART_Transmit_IT error on M4 in uart TX callback !!");
}

/**
 * @brief Process sends CLI command to M0.
 * @param  None
 * @retval None
 */
static void SendCliCmdToM0(void)
{
#if (CFG_LED_SUPPORTED == 1U)
  BSP_LED_Off((LED_RED));
#endif
  memset(LldTestsCliCmdRspPacket.cmdserial.cmd.payload, 0x0U, 255U);
  *(uint32_t *)LldTestsCliCmdRspPacket.cmdserial.cmd.payload = (uint32_t)currentCommand;
  *(uint32_t *)&LldTestsCliCmdRspPacket.cmdserial.cmd.payload[4] = currentCommandPos;
  LldTestsCliCmdRspPacket.cmdserial.cmd.plen = 8;
  LldTestsCliCmdRspPacket.cmdserial.cmd.cmdcode = 0x0;
#if 0
  if (strcmp((char *)currentCommand, "info") == 0)
    APP_DBG((char *)"!! info command sent to M0 : txwr %u - txrd %u - full cnt %u - rxwr %u - rxrd %u", txBuffer_wrPtr, txBuffer_rdPtr, txBufferFullCount, rxBuffer_wrPtr, rxBuffer_rdPtr);
  if (strcmp((char *)currentCommand, "RX_get_LQI") == 0)
    APP_DBG((char *)"!! RX_get_LQI command sent to M0 : txwr %u - txrd %u - full cnt %u - rxwr %u - rxrd %u", txBuffer_wrPtr, txBuffer_rdPtr, txBufferFullCount, rxBuffer_wrPtr, rxBuffer_rdPtr);
#endif
  
  TL_LLDTESTS_SendCliCmd();
}

/**
 * @brief Perform initialization of TL for LLD tests.
 * @param  None
 * @retval None
 */
void APP_LLDTESTS_Init_TL(void)
{
  LldTestsConfigBuffer.p_LldTestsCliCmdRspBuffer = (uint8_t*)&LldTestsCliCmdRspPacket;
  LldTestsConfigBuffer.p_LldTestsM0CmdBuffer = (uint8_t*)&LldTestsM0CmdPacket;
  
  TL_LLDTESTS_Init( &LldTestsConfigBuffer );
}

static void m0CmdProcess(void)
{
  int8_t result, delta;

  switch (m0Cmd) {
    case CMD_FROM_M0_STOP0_ON :
      m0CmdStopRequired(0);
      break;
    
    case CMD_FROM_M0_STOP1_ON :
      m0CmdStopRequired(1);
      break;
    
    case CMD_FROM_M0_STOP2_ON :
      m0CmdStopRequired(2);
      break;
    
    case CMD_FROM_M0_STOP_OFF :
      // Nothing done here as the goal of this command is just to have an IPCC interrupt
      // that will make the M4 going out of STOP or WFI
      break;
    
    case CMD_FROM_M0_GO_IN_WFI :
      m0CmdStopRequired(3);
      break;
    
    case CMD_FROM_M0_GO_DIRECTLY_IN_WFI :
      m0CmdStopRequired(4);
      break;
    
    case CMD_FROM_M0_USE_MSI :
      SystemClock_Config_MSI(0, 1);
      break;
    
    case CMD_FROM_M0_USE_MSI_HSE :
      SystemClock_Config_MSI(0, 0);
      break;
    
    case CMD_FROM_M0_USE_MSI_PLL :
      SystemClock_Config_MSI(1, 1);
      break;
    
    case CMD_FROM_M0_USE_MSI_PLL_HSE :
      SystemClock_Config_MSI(1, 0);
      break;
    
    case CMD_FROM_M0_USE_HSE :
      SystemClock_Config_HSE(0);
      break;
    
    case CMD_FROM_M0_USE_HSE_PLL :
      SystemClock_Config_HSE(1);
      break;
    
#if (CFG_HW_EXTPA_ENABLED == 1)
    case CMD_FROM_M0_EXT_PA_EN :
      gpio_lld_extPa_init();
      // Indicate to M0 which GPIO must be managed
      SHCI_C2_ExtpaConfig((uint32_t)GPIO_EXT_PA_EN_PORT, GPIO_EXT_PA_EN_PIN, EXT_PA_ENABLED_HIGH, EXT_PA_ENABLED);
      break;
    
    case CMD_FROM_M0_EXT_PA_DIS :
      gpio_lld_extPa_deInit();
      SHCI_C2_ExtpaConfig((uint32_t)GPIO_EXT_PA_EN_PORT, GPIO_EXT_PA_EN_PIN, EXT_PA_ENABLED_HIGH, EXT_PA_DISABLED);
      break;
#endif
    
    case CMD_FROM_M0_GENE_HARD_FAULT :
      {
        // Use this debug command to have a hard fault
        volatile uint32_t * badAddrPointer = (uint32_t *)0x20050000;
        // No trace possible here as another task must be scheduled
        //APP_DBG("Hard fault generated on M4");
        //us_delay(1000);
        *badAddrPointer = 0xF;
      }
      break;
    case CMD_FROM_M0_HSE_TUNE :
      // Get current tuning value and delta
      delta = (int8_t)atol(CmdFromM0Array[1]);
      result = LL_RCC_HSE_GetCapacitorTuning() + delta;

      // Check out of range
      if (result > 63) {
        result = 63;
      } else if (result < 0) {
        result = 0;
      }

      LL_RCC_HSE_SetCapacitorTuning(result);
      break;

    case CMD_FROM_M0_HSE_TUNE_DEF:
      // 7:0 contains the tuning value, bit 31 is flag for valid value
      Config_HSE_tuning( (uint32_t) (HSE_TRIM_VALID |
                                     (uint8_t) LL_RCC_HSE_GetCapacitorTuning() ));
      break;

    default:
      break;
  }
}

static void m0CmdStopRequired(uint32_t stopRequired)
{
  switch (stopRequired) {
    case 0:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      us_delay(DELAY_BEFORE_WFI_ON_M4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLDTESTS_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      M4_LPM_mode = LPM_STOP_0;
      UTIL_LPM_EnterLowPower();
      
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLDTESTS_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_Init();
#endif
      
      APP_DBG("M4 back from STOP0");
      break;
    
    case 1:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      us_delay(DELAY_BEFORE_WFI_ON_M4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLDTESTS_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      M4_LPM_mode = LPM_STOP_1;
      UTIL_LPM_EnterLowPower();
      
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLDTESTS_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_Init();
#endif
      
      APP_DBG("M4 back from STOP1");
      break;
    
    case 2:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      us_delay(DELAY_BEFORE_WFI_ON_M4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLDTESTS_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      M4_LPM_mode = LPM_STOP_2;
      UTIL_LPM_EnterLowPower();
      
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLDTESTS_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_Init();
#endif
      
      APP_DBG("M4 back from STOP2");
      break;
    
    case 3:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      us_delay(DELAY_BEFORE_WFI_ON_M4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLDTESTS_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      M4_LPM_mode = LPM_NO_STOP;
      __WFI();
      
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLDTESTS_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_Init();
#endif
      
      APP_DBG("M4 back from WFI");
      break;
    
    case 4:
      // Put M4 in WFI without delay :
      //   - In some cases, it is recomanded to limit the M4 activity to avoid Flash access which can slow down the M0 which share it with M4
      //   - But, be carrefull as WFI can be blocking as there is no more interrupts
      //   - As example, if not used, SF timer test is failed.
      __WFI();
      APP_DBG("M4 back from WFI (without delay)");
      break;
    
    default:
      break;
  }
}

/**
 * @brief Send Ack to M0 CLI Response channel.
 * @param  None
 * @retval None
 */
static void SendCliRspAckToM0(void)
{
  /* Notify M0 that CLI notification has been taken into account */
  TL_LLDTESTS_SendCliRspAck();
}

/**
 * @brief  This function is called when notification on CLI TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_CmdPacket_t
 * @return  None
 */
void TL_LLDTESTS_ReceiveCliRsp( TL_CmdPacket_t * Notbuffer )
{
  TL_CmdPacket_t* l_CliBuffer = (TL_CmdPacket_t*)Notbuffer;
  uint8_t         l_size = l_CliBuffer->cmdserial.cmd.plen;
  char *          sourceBuf = (char *)*(uint32_t *)l_CliBuffer->cmdserial.cmd.payload;
  uint32_t        length = *(uint32_t *)&l_CliBuffer->cmdserial.cmd.payload[4];
  
  if ((l_size > 0) && (length > 0))
  {
    if (strncmp(sourceBuf, "CLI_Resp end", 12) == 0)
    {
      /* This is an answer to indicate that the CLI command has been completed */
#if (CFG_LED_SUPPORTED == 1U)
      BSP_LED_On(LED_RED);
#endif
      /* Write a promp to UART and allow to receive a new command */
      uartTxBufferAdd(cliPrompt);
      
      currentCommandPos = 0;
      rxCmdAllowed = 1;
    }
    else
    {
      /* This is a test result or a test trace from M0 to write to CLI UART buffer */
      uartTxBufferAdd(sourceBuf);
    }
  }
  else
  {
    APP_DBG((char *)"!! Empty M0 CLI response received by M4 !!");
  }
  SendCliRspAckToM0();
}

/**
 * @brief Send Ack to M0 CLI Response channel.
 * @param  None
 * @retval None
 */
static void SendM0CmdAckToM0(void)
{
  /* Notify M0 that CLI notification has been taken into account */
  TL_LLDTESTS_SendM0CmdAck();
}

/**
 * @brief  This function is called when notification on CLI TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_CmdPacket_t
 * @return  None
 */
void TL_LLDTESTS_ReceiveM0Cmd( TL_CmdPacket_t * cmdBuffer )
{
  uint8_t bufferSize = cmdBuffer->cmdserial.cmd.plen;
  char *cmdToken = strtok((char *)cmdBuffer->cmdserial.cmd.payload, " ");
  uint8_t cmdToken_index = 0;
  
  while (cmdToken != NULL) {
    CmdFromM0Array[cmdToken_index++] = cmdToken;
    cmdToken = strtok(NULL, " ");
  }

  if (bufferSize > 0)
  {
    if (strncmp(CmdFromM0Array[0], "stop0_on", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP0_ON;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "stop1_on", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP1_ON;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "stop2_on", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP2_ON;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "stop_off", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP_OFF;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "go_in_wfi", 9) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_GO_IN_WFI;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "go_directly_in_wfi", 18) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_GO_DIRECTLY_IN_WFI;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "use_msi_no_pll_hse_off", 22) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_MSI;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "use_msi_no_pll_hse_on", 21) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_MSI_HSE;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "use_msi_pll_hse_off", 19) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_MSI_PLL;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "use_msi_pll_hse_on", 18) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_MSI_PLL_HSE;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "use_hse_no_pll", 14) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_HSE;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "use_hse_pll", 11) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_HSE_PLL;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "ext_pa_enable", 13) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_EXT_PA_EN;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "ext_pa_disable", 14) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_EXT_PA_DIS;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "generate_hard_fault", 19) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_GENE_HARD_FAULT;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "hse_tune_def", 12) == 0)
    {
      m0Cmd = CMD_FROM_M0_HSE_TUNE_DEF;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(CmdFromM0Array[0], "hse_tune", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_HSE_TUNE;
      // Set the task to process the command, if all parameters have been given
      if (cmdToken_index == 2) {
          UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
      }
    }
    else
    {
      APP_DBG((char *)"!! Unknown M0 command received by M4 !!");
    }
  }
  else
  {
    APP_DBG((char *)"!! Empty M0 command received by M4 !!");
  }
  SendM0CmdAckToM0();
}

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
