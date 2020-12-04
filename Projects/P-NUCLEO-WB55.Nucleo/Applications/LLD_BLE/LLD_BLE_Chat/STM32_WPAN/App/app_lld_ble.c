/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_lld_ble.c
  * Description        : CHAT LLD BLE Application.
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
#include "gpio_lld.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
  CMD_FROM_M0_STOP0_ON           = 0,
  CMD_FROM_M0_STOP1_ON           = 1,
  CMD_FROM_M0_STOP2_ON           = 2,
  CMD_FROM_M0_STOP_OFF           = 3,
  CMD_FROM_M0_GO_IN_WFI          = 4,
  CMD_FROM_M0_GO_DIRECTLY_IN_WFI = 5,
  CMD_FROM_M0_USE_MSI            = 6,
  CMD_FROM_M0_USE_MSI_PLL        = 7,
  CMD_FROM_M0_USE_HSE            = 8,
  CMD_FROM_M0_USE_HSE_PLL        = 9,
  CMD_FROM_M0_EXT_PA_EN          = 10,
  CMD_FROM_M0_EXT_PA_DIS         = 11,
  CMD_FROM_M0_RADIO_STOP         = 12,
  CMD_FROM_M0_RADIO_END          = 13,
  CMD_FROM_M0_RADIO_RXACK        = 14,  
  CMD_FROM_M0_RADIO_RXOK         = 15, 
  CMD_FROM_M0_RADIO_RXACKEND     = 16,  
  CMD_FROM_M0_RADIO_RXOKEND      = 17,  
} cmdFromM0_t;

enum 
{
  CHAR_CTRLC = 3,
  CHAR_ESC   = 27,
  CHAR_DEL   = 127
};

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define RX_BUFFER_SIZE     258
#define CMD_HISTORY_LENGTH  10
#define CLI_BUFFER_SIZE     30
#define TX_BUFFER_SIZE     268
#define CMD_BUFFER_SIZE      8

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
static void CheckWirelessFirmwareInfo(void);
static void LldBleTraceError(const char * pMess, uint32_t ErrCode);

static void SendM0CmdAckToM0(void);
static void SendCmdToM0(void);
static void SendRspAckToM0(void);

static void uartRxCpltCallback(void);
static void uartRxItProcess(void);
static void uartRxBufferProcessMode(uint8_t uartRxCmdEnable);
static void uartRxBufferProcess(void);
static void uartClearChars(int nbChar);

static void uartTxBufferAdd(const char * str);
static void uartTxItProcess(void);
static void uartTxCpltCallback(void);

static void m0RadioProcess(void);
static void m0CmdProcess(void);
static void m0CmdStopRequired(uint32_t stopRequired);
static void m4ConfigBeforeStop(void);
static void m4ConfigAfterStop(void);

static void Appli_ProcessMode(void);
static void Appli_RegTask(void);
static void Appli_Init(void);
static void Appli_uartRxBufferPrint(void);

void Appli_m0RadioProcess_RadioStop(void);
void Appli_m0RadioProcess_RadioEnd(void);
void Appli_m0RadioProcess_RxAck(void);
void Appli_m0RadioProcess_RxOk(void);
void Appli_m0RadioProcess_RxAckEnd(void);
void Appli_m0RadioProcess_RxOkEnd(void);

void Appli_m0CmdProcess_RadioStop(void);
void Appli_m0CmdProcess_RadioEnd(void);
void Appli_m0CmdProcess_RxAck(void);
void Appli_m0CmdProcess_RxOk(void);
void Appli_m0CmdProcess_RxAckEnd(void);
void Appli_m0CmdProcess_RxOkEnd(void);

/* USER CODE BEGIN PFP */
void CHAT_BLE_Init(void);
void CHAT_Encrypt(void);
void CHAT_UnEncrypt(void);
void CHAT_ToggleEncrypt(void);
void CHAT_ListenPacket(void);
void CHAT_SendPacket(void);
void CHAT_StopRadio(void);
void CHAT_StartTone(void);
void CHAT_StopTone(void);
/* USER CODE END PFP */

/* debug function prototypes */
uint32_t Debug_Start_timer(void);
uint32_t Debug_Stop_timer(void);

/* Private variables -----------------------------------------------*/
static cmdFromM0_t m0Cmd = CMD_FROM_M0_STOP_OFF;

static uint8_t  txBusy;
static uint8_t  rxCmdAllowed = 0;
static uint8_t  txBuffer_Tab[TX_BUFFER_SIZE];
static uint32_t txBuffer_wrPtr;
static uint32_t txBuffer_rdPtr;
static uint8_t *pTxBuff_currentWr;
static uint8_t  rxBuffer_Tab[RX_BUFFER_SIZE];
static uint32_t rxBuffer_wrPtr;
static uint32_t rxBuffer_rdPtr;
static uint8_t  currentCommand[CMD_BUFFER_SIZE];
static uint32_t currentCommandPos;
static uint8_t  commandHistory[CMD_HISTORY_LENGTH][RX_BUFFER_SIZE];
static int      commandHistoryIdx;
static int      commandHistoryIdxSav;
static uint32_t txtCommandPos;
static uint8_t  txtCommand[TX_BUFFER_SIZE];
static char     cliPrompt[CLI_BUFFER_SIZE] = "Unknown M0 appli > ";
static uint8_t  uartRxCmd = 0;
static uint32_t delayBeforeSleepOnM4 = 100000;
static uint8_t  uartLastChar;
static uint8_t  uartPayload[RX_BUFFER_SIZE];

/* used with lld_ble */
extern uint8_t  *txBuffer_Ptr;    
uint8_t  *rxBuffer_Ptr[8]; 
uint32_t *rxStatus_Ptr[8]; 
uint32_t *rxTimeStamp_Ptr[8];
int32_t  *rxRSSI_Ptr[8];

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_LLD_BLE_Config_t LldBleConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t LldBleM0CmdPacket;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t LldBleCmdRspPacket;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t  currentCommand[CMD_BUFFER_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) param_hal_BLE_t bleparam_hal_BLE_Packet;

/* debug */
#define LLD_BLE_DEBUG
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint32_t debug_timer[4];
uint32_t start_timer;
uint32_t stop_timer;
extern TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* Parameters */
uint8_t  init_done =0;
static char     chatPrompt[258] = "Unknown Text received > ";
uint32_t chatID = 0xB55A54AA ; //0x8E89BED6
uint8_t  chatChannel = 12;
uint32_t chatWakeup = 18760;
uint32_t chatReceive = 29876;
uint32_t chatReceiveAck = 576;

/* data buffer Tab to send TX */
uint8_t chattxBufferTab[258] ={0x80,0x08,0x73,0x65,0x70,0x68,0x26,0x48,0x6F,0x73};
uint8_t* chattxBuffer =  chattxBufferTab;

/* data buffer ACK Tab to send TX ACK after RX */
uint8_t chatAcktxBufferTab[19] ={0x60,0x0D,0x52,0x65,0x63,0x65,0x69,0x76,0x65,0x64,0x20,0x4d,0x61,0x69,0x6c};
uint8_t* chatAcktxBuffer =  chatAcktxBufferTab;

/* data buffer Tab to receive RX (Not empty)*/
uint8_t chatrxBufferTab[258]={0x0A,0x0C,0xAB,0xBA,0x67,0x76,0x55,0x55,0x4E,0xE4,0x9B,0xB9,0xAB,0xBA};
uint8_t* chatrxBuffer =  chatrxBufferTab;

/* data buffer ACK Tab to receive RX ACK after TX */
uint8_t chatAckrxBufferTab[258]={0x0A,0x0C,0x3E,0xE3,0x28,0x82,0x64,0x46,0xC1,0x1C,0x78,0x87,0xDC,0xCD};
uint8_t* chatAckrxBuffer= chatAckrxBufferTab;

/* Routines */
/* dataRoutine enum for LL and HAL dataRoutine() */
dataCase_t dataRoutineDone_hal_BLE = dataCase_Custom0;
dataCase_t dataRoutineTx_hal_BLE   = dataCase_Custom1;
dataCase_t dataRoutineRx_hal_BLE   = dataCase_Custom2;
dataCase_t dataRoutineStop_hal_BLE = dataCase_Custom3;

/* dataRoutine enum for LL dataRoutine() */
dataCase_t dataRoutine_LL_TxAck = dataCase_Custom4;
dataCase_t dataRoutine_LL_RxAck = dataCase_Custom5;
dataCase_t dataRoutine_Action   = dataCase_Custom6;

/* dataRoutine enum for HAL dataRoutine() */
dataCase_t dataRoutine_HAL_Tx    = dataCase_Custom4;
dataCase_t dataRoutine_HAL_TxAck = dataCase_Custom5;
dataCase_t dataRoutine_HAL_Rx    = dataCase_Custom6;
dataCase_t dataRoutine_HAL_RxAck = dataCase_Custom7;

/* Routines options */
uint32_t chatPacketNumberTx = 5; // NB of Successif PACKET using HAL Send 
uint32_t chatPacketNumberRx = 10; // NB of Successif PACKET using HAL Receive
uint8_t  chatPacketStopRx = 1; // Stop after RX

/* Encryption */
uint8_t chatcount_tx[5]     = {0x00,0x00,0xAF,0x00,0x08};
uint8_t chatcount_rcv[5]    = {0x00,0x00,0xAF,0x00,0x08};
uint8_t chatenc_key[16]     = {0xBF,0x01,0xFB,0x9D,0x4E,0xF3,0xBC,0x36,0xD8,0x74,0xF5,0x39,0x41,0x38,0x68,0x56};
uint8_t chatenc_iv[8]       = {0x00,0x00,0xAF,0x00,0x08,0x00,0x00,0x00};
uint8_t chatEncrypt         = 0;

/* Hot config */
uint32_t LLD_BLE_hot_ana_config_table[BLE_HOT_ANA_CONFIG_TABLE_LENGTH/4];

/* Variable used with m0RadioProcess/m0CmdProcess */
static uint8_t radioPacketNb=0;
static uint8_t radioLength=0;
/* USER CODE END PV */

/* Functions Definition ------------------------------------------------------*/
void APP_LLD_BLE_Init( void )
{
  uint32_t devId = HAL_GetDEVID();
  uint32_t revId = HAL_GetREVID();
  uint8_t  param[8];
  char     traceBuff[50];
  
  SHCI_CmdStatus_t LldTestsInitStatus;
  
  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  CheckWirelessFirmwareInfo();
  
  /**
   * Do not allow standby in the application
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_LLD_BLE, UTIL_LPM_DISABLE);
  
  /* No need to activate the System low power mode as it is managed by the low-power test itself */
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_LLD_BLE, UTIL_LPM_DISABLE );
  
  /* Init config buffer and call TL_LLD_BLE_Init */
  APP_LLD_BLE_Init_TL(); 

  /* Create a task to send CLI commands to M0 via IPCC */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SEND_TO_M0, UTIL_SEQ_RFU, SendCmdToM0);

  /* Create a task to process data received from UART and create CLI commands */
  Appli_ProcessMode();
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PROCESS_UART_RX_BUFFER, UTIL_SEQ_RFU, uartRxBufferProcess);
  
  /* Create tasks to process interrupt from/to UART to avoid locking UART during IT processing */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PROCESS_UART_RX_IT, UTIL_SEQ_RFU, uartRxItProcess);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PROCESS_UART_TX_IT, UTIL_SEQ_RFU, uartTxItProcess);
  
  /* Create a task to manage commands from M0 */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_CMD_FROM_M0_TO_M4, UTIL_SEQ_RFU, m0CmdProcess);

  /* Create Task for Appli */
  Appli_RegTask();

  /* Configure UART for receiving CLI command from PC and sending CLI response or notifications to PC */
  txBusy = 0;
  rxCmdAllowed = 0;
  txBuffer_Tab[TX_BUFFER_SIZE-1] = 0;
  txBuffer_wrPtr = 0;
  txBuffer_rdPtr = 0;
  rxBuffer_wrPtr = 0;
  rxBuffer_rdPtr = 0;
  currentCommandPos = 0;
  commandHistoryIdx = 0;
  commandHistoryIdxSav = 0;
  APP_LLD_BLE_Init_UART_CLI();
  
  /* Send LLD tests CLI start information to CLI UART */
  uartTxBufferAdd("\r\n\n================================\r\n");
#ifdef STM32WB35xx
  sprintf(traceBuff, "= Little DORY");
#else
  sprintf(traceBuff, "= DORY");
#endif
  sprintf(traceBuff, "%s RF LLD BLE \r\n", traceBuff );
  uartTxBufferAdd(traceBuff);
  uartTxBufferAdd("================================\r\n");
#if (CFG_FULL_LOW_POWER == 1U)
  uartTxBufferAdd("Low-power mode is activated\r\n");
#endif
#if (CFG_DEBUGGER_SUPPORTED == 0U)
  uartTxBufferAdd("Debugger de-activated\r\n");
#endif
#if (( CFG_DEBUG_TRACE_FULL == 0 ) && ( CFG_DEBUG_TRACE_LIGHT == 0 ))
  uartTxBufferAdd("Trace is de-activated\r\n");
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
  LldTestsInitStatus = SHCI_C2_LLD_BLE_Init(8, param);
  if(LldTestsInitStatus != SHCI_Success)
    APP_DBG((char *)"!! ERROR during M0 init !!");
  
  /* M0 init done, send first command to have M0 code info and thus, a first prompt will be printed automatically */
  /* Do not accept new command from UART until this one is managed */
  rxCmdAllowed = 0;
  currentCommandPos = 5;
  currentCommand[0] = HAL_BLE_UNUSED_CMDCODE ;/* HAL_BLE_INIT_CMDCODE cmdcode */
  currentCommand[1] = 'i';
  currentCommand[2] = 'n';
  currentCommand[3] = 'f';
  currentCommand[4] = 'o';
  currentCommand[5] = 0;
  /* Set corresponding task to send this command to M0 */
  UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_TO_M0, CFG_SCH_PRIO_0);
  
  /* Activate UART RX buffer processing task to allow USER command comming from UART */
  UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_BUFFER, CFG_SCH_PRIO_0);

  Appli_Init();
  
}

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */
void APP_LLD_BLE_Error(uint32_t ErrId, uint32_t ErrCode)
{
  switch(ErrId)
  {
  case ERR_LLD_BLE_SET_STATE_CB :
    LldBleTraceError("ERROR : ERR_LLD_BLE_SET_STATE_CB ",ErrCode);
    break;
    
  case ERR_LLD_BLE_ERASE_PERSISTENT_INFO :
    LldBleTraceError("ERROR : ERR_LLDT_BLE_ERASE_PERSISTENT_INFO ",ErrCode);
    break;
    
  case ERR_LLD_BLE_CHECK_WIRELESS :
    LldBleTraceError("ERROR : ERR_LLD_BLE_CHECK_WIRELESS ",ErrCode);
    break;
    
  default :
    LldBleTraceError("ERROR Unknown ", 0);
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
static void LldBleTraceError(const char * pMess, uint32_t ErrCode)
{
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
}

/**
 * @brief Check if the Coprocessor Wireless Firmware loaded supports Thread
 *        and display associated informations
 * @param  None
 * @retval None
 */
static void CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t  wireless_info_instance;
  WirelessFwInfo_t* p_wireless_info = &wireless_info_instance;
  char              m0FwType[50] = "M0 FW Type : Unknown !! ";

  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success) 
  {
    APP_LLD_BLE_Error((uint32_t)ERR_LLD_BLE_CHECK_WIRELESS, 0);
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
    
    default :
      /* FW not supported */
      APP_LLD_BLE_Error((uint32_t)ERR_LLD_BLE_CHECK_WIRELESS, 0);
      break;
    }
    APP_DBG(m0FwType);
    
    /* Print version */
    APP_DBG("  M0 FW VERSION = v%d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);
    
    APP_DBG("**********************************************************");
  }
}

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
void APP_LLD_BLE_Init_UART_CLI(void)
{
#if (CFG_HW_USART1_ENABLED == 1)
  #if (CFG_FULL_LOW_POWER == 0)
  MX_USART1_UART_Init();
  
  /* Put the UART device in reception mode and wait for interrupt */
  if (HW_UART_Receive_IT(CFG_CLI_UART, &rxBuffer_Tab[rxBuffer_wrPtr], 1, uartRxCpltCallback) != hw_uart_ok)
    APP_DBG((char *)"!! HAL_UART_Receive_IT error on M4 in APP_LLD_BLE_Init_UART_CLI !!");
  #endif
#endif
}

/**
 * @brief Perform de-initialization of CLI UART interface.
 * @param  None
 * @retval None
 */
void APP_LLD_BLE_DeInit_UART_CLI(void)
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
       - when number of bytes specified in last call to HW_UART_Receive_IT() is acheived */
  
  /* Prepare buffer to receive next character */
  if ( rxCmdAllowed ) {
    /* Increment read pointer index and manage buffer rollover */
    rxBuffer_wrPtr++;
    if(rxBuffer_wrPtr >= RX_BUFFER_SIZE)
      rxBuffer_wrPtr = 0;
  }
  
  /* Re-put UART device in reception mode (not during IT to avoid locking the device while perhaps in transmit phase) */
  UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_IT, CFG_SCH_PRIO_0);
}

static void uartRxItProcess(void)
{
  /* Put the UART device in reception mode and wait for interrupt */
  if (HW_UART_Receive_IT(CFG_CLI_UART, &rxBuffer_Tab[rxBuffer_wrPtr], 1, uartRxCpltCallback) != hw_uart_ok)
    APP_DBG((char *)"!! HAL_UART_Receive_IT error on M4 in uartRxItProcess !!");
}

static void uartClearChars(int nbChar)
{
  char clearCmd[2];
  
  clearCmd[0] = CHAR_DEL;
  clearCmd[1] = 0;
  for (int i = 0; i < nbChar; i++) {
    uartTxBufferAdd(clearCmd);
  }
}

static void uartRxBufferProcessMode(uint8_t uartRxCmdEnable) /* 1 for Cmd ; 0 for Print */
{
  uartRxCmd=uartRxCmdEnable;
}

static void uartRxBufferProcess( void )
{
  char last_char[2];
  static int8_t escape = -1;
  static uint8_t escape_seq[3] = {0};

  /* Ensure that last_char buffer contains a string with the char and a \0 (usefull to send echo or CHAR_DEL to UART) */
  last_char[1] = 0;
  
  if(uartRxCmd) {
  /* Interpret and Command the character reception from UART */
  while (rxBuffer_rdPtr != rxBuffer_wrPtr ) {
    last_char[0] = rxBuffer_Tab[rxBuffer_rdPtr];
    if( escape >= 0 ) {
      escape_seq[escape++] = last_char[0];
      if ( escape == 2 ) {
        if ( strcmp((char*)escape_seq, "[A") == 0 ) { // UP
          uartClearChars(currentCommandPos);
          commandHistoryIdx = (commandHistoryIdx > 0) ? (commandHistoryIdx - 1) : (CMD_HISTORY_LENGTH - 1);
          strcpy((char*)currentCommand,(char*)commandHistory[commandHistoryIdx]);
          currentCommandPos = strlen((char*)currentCommand);
          uartTxBufferAdd((char*)currentCommand);
        }
        if ( strcmp((char*)escape_seq, "[B") == 0 ) { // DOWN
          uartClearChars(currentCommandPos);
          commandHistoryIdx = (commandHistoryIdx < (CMD_HISTORY_LENGTH - 1)) ? (commandHistoryIdx + 1) : 0;
          strcpy((char*)currentCommand,(char*)commandHistory[commandHistoryIdx]);
          currentCommandPos = strlen((char*)currentCommand);
          uartTxBufferAdd((char*)currentCommand);
        }
        escape = -1;
      }
    }
    else if ( last_char[0] == CHAR_DEL )  {
      if ( currentCommandPos > 0 ) {
        currentCommandPos--;
        uartTxBufferAdd(last_char);
      }
    }
    else if ( last_char[0] == CHAR_ESC )  {
      escape = 0;
    }
    else if ( last_char[0] == CHAR_CTRLC )  {
      /* Just send a new prompt to wait for a new command */
      currentCommandPos = 0;
      uartTxBufferAdd("\r\n");
      uartTxBufferAdd(cliPrompt);
    }
    else if ( ( last_char[0] == '\r' ) || ( last_char[0] == '\n' ))  {
      if (currentCommandPos == 0) {
        /* User just typed 'enter' without any command, so, just send a new prompt to wait for a new command */
        uartTxBufferAdd("\r\n");
        uartTxBufferAdd(cliPrompt);
      } else {
        /* Do not accept new command from UART until this one is managed */
        rxCmdAllowed = 0;
        
        /* Put a end of line in UART TX buffer to have the echo sent */
        uartTxBufferAdd("\r\n");
        
        /* Copy the current command in history buffer before to send it to M0 */
        memcpy(commandHistory[commandHistoryIdxSav],currentCommand,currentCommandPos);
        /* add a \0 to avoid reseting end of command history buffer */
        commandHistory[commandHistoryIdxSav][currentCommandPos] = 0;
        commandHistoryIdxSav = (commandHistoryIdxSav+1) % CMD_HISTORY_LENGTH;
        commandHistoryIdx = commandHistoryIdxSav;
        
        /* UART task scheduling to send it to M0 */
        UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_TO_M0, CFG_SCH_PRIO_0);
      }
    }
    else {
      /* Put the char in UART TX buffer to have the echo sent */
      uartTxBufferAdd(last_char);
      /* Put the char in the current command buffer */
      currentCommand[currentCommandPos++] = last_char[0];
    }
    
    /* Increment read pointer index and manage buffer rollover */
    rxBuffer_rdPtr++;
    if(rxBuffer_rdPtr >= RX_BUFFER_SIZE)
      rxBuffer_rdPtr = 0;
  }
  } else {
  /* Interpret and Print the character reception from UART */
  while (rxBuffer_rdPtr != rxBuffer_wrPtr ) {
    last_char[0] = rxBuffer_Tab[rxBuffer_rdPtr];
    if( escape >= 0 ) {
      escape_seq[escape++] = last_char[0];
      if ( escape == 2 ) {
        if ( strcmp((char*)escape_seq, "[A") == 0 ) { // UP
          uartClearChars(txtCommandPos);
          commandHistoryIdx = (commandHistoryIdx > 0) ? (commandHistoryIdx - 1) : (CMD_HISTORY_LENGTH - 1);
          strcpy((char*)txtCommand,(char*)commandHistory[commandHistoryIdx]);
          txtCommandPos = strlen((char*)txtCommand);
          uartTxBufferAdd((char*)txtCommand);
        }
        if ( strcmp((char*)escape_seq, "[B") == 0 ) { // DOWN
          uartClearChars(txtCommandPos);
          commandHistoryIdx = (commandHistoryIdx < (CMD_HISTORY_LENGTH - 1)) ? (commandHistoryIdx + 1) : 0;
          strcpy((char*)txtCommand,(char*)commandHistory[commandHistoryIdx]);
          txtCommandPos = strlen((char*)txtCommand);
          uartTxBufferAdd((char*)txtCommand);
        }
        escape = -1;
      }
    }
    else if ( last_char[0] == CHAR_DEL )  {
      if ( txtCommandPos > 0 ) {
        txtCommandPos--;
        uartTxBufferAdd(last_char);
      }
    }
    else if ( last_char[0] == CHAR_ESC )  {
      escape = 0;
    }
    else if ( last_char[0] == CHAR_CTRLC )  {
      /* Just send a new prompt to wait for a new command */
      txtCommandPos = 0;
      uartTxBufferAdd("\r\n");
      uartTxBufferAdd(cliPrompt);
    }
    else if ( ( last_char[0] == '\r' ) || ( last_char[0] == '\n' ))  {
      if (txtCommandPos == 0) {
        /* User just typed 'enter' without any command, so, just send a new prompt to wait for a new command */
        uartTxBufferAdd("\r\n");
        uartTxBufferAdd(cliPrompt);
        uartLastChar=0;

      } else {
        /* Do not accept new command from UART until this one is managed */
        rxCmdAllowed = 0;
        
        /* Put a end of line in UART TX buffer to have the echo sent */
        uartTxBufferAdd("\r\n");
        
        /* Copy the current command in history buffer before to send it to M0 */
        memcpy(commandHistory[commandHistoryIdxSav],txtCommand,txtCommandPos);
        /* add a \0 to avoid reseting end of command history buffer */
        commandHistory[commandHistoryIdxSav][txtCommandPos] = 0;
        commandHistoryIdxSav = (commandHistoryIdxSav+1) % CMD_HISTORY_LENGTH;
        commandHistoryIdx = commandHistoryIdxSav;
        
        /* UART task scheduling to send it to M0 */
        /* Send Packet with UART Char as Payload */
        uartPayload[0]= 0xAA; // Appli Header
        uartPayload[1]= txtCommandPos; // Appli Length
        memcpy(&uartPayload[2],txtCommand,txtCommandPos);
        txtCommandPos=0;
        Appli_uartRxBufferPrint();
        uartLastChar=1;
        
      }
    }
    else {
      /* Put the char in UART TX buffer to have the echo sent */
      uartTxBufferAdd(last_char);
      /* Put the char in the current command buffer */
      txtCommand[txtCommandPos++] = last_char[0];
    }
    
    /* Increment read pointer index and manage buffer rollover */
    rxBuffer_rdPtr++;
    if(rxBuffer_rdPtr >= RX_BUFFER_SIZE)
      rxBuffer_rdPtr = 0;
  }
  }
  
  /* Re-activate UART RX buffer processing task */
  UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_BUFFER, CFG_SCH_PRIO_0);
}

uint32_t txBufferFullCount;
static void uartTxBufferAdd(const char * str)
{
  uint16_t bytesToWrite = strlen(str);
  uint32_t txBufferFull = 0;
  
  if (bytesToWrite) {
    CRITICAL_BEGIN();
    {
      uint16_t  remainingBytesToWrite = bytesToWrite;
      uint32_t  currentWrPtr  = txBuffer_wrPtr;
      uint8_t * currentWrAddr = &txBuffer_Tab[txBuffer_wrPtr];
      
      while ((remainingBytesToWrite > 0) && (txBufferFull == 0)) {
        if ((txBuffer_rdPtr == (currentWrPtr + 1)) || ((txBuffer_rdPtr == 0) && (currentWrPtr == (TX_BUFFER_SIZE - 1)))) {
          /* If there is not enougth place (i.e. write pointer is just behind read pointer in the circular buffer), 
             buffer is full, so do not consider new string */
          /* !! No trace here as it is under CRITICAL section */
          txBufferFull = 1;
        } else {
          *currentWrAddr = str[bytesToWrite - remainingBytesToWrite];
          remainingBytesToWrite--;
          
          /* Increment write pointer index and manage buffer rollover */
          currentWrPtr++;
          if(currentWrPtr >= TX_BUFFER_SIZE)
            currentWrPtr = 0;
          currentWrAddr = &txBuffer_Tab[currentWrPtr];
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
    pTxBuff_currentWr = &txBuffer_Tab[txBuffer_rdPtr];
    UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_TX_IT, CFG_SCH_PRIO_0);
    
    /* Increment write pointer index and manage buffer rollover */
    txBuffer_rdPtr++;
    if(txBuffer_rdPtr >= TX_BUFFER_SIZE)
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
static void SendCmdToM0(void)
{
  memset(LldBleCmdRspPacket.cmdserial.cmd.payload, 0x0U, 255U);
  *(uint32_t *)LldBleCmdRspPacket.cmdserial.cmd.payload = (uint32_t)currentCommand;
  *(uint32_t *)&LldBleCmdRspPacket.cmdserial.cmd.payload[4] = currentCommandPos;
  LldBleCmdRspPacket.cmdserial.cmd.plen = 8;
  LldBleCmdRspPacket.cmdserial.cmd.cmdcode = 0x0;
  
  TL_LLD_BLE_SendCmd();
}

/**
 * @brief Perform initialization of TL for LLD tests.
 * @param  None
 * @retval None
 */
void APP_LLD_BLE_Init_TL(void)
{
  LldBleConfigBuffer.p_LldBleCmdRspBuffer = (uint8_t*)&LldBleCmdRspPacket;
  LldBleConfigBuffer.p_LldBleM0CmdBuffer = (uint8_t*)&LldBleM0CmdPacket;
  
  TL_LLD_BLE_Init( &LldBleConfigBuffer );
}

static void m0RadioProcess(void)
{
  switch (m0Cmd) {
    case CMD_FROM_M0_RADIO_STOP :
      Appli_m0RadioProcess_RadioStop();
      break;

    case CMD_FROM_M0_RADIO_END :
      Appli_m0RadioProcess_RadioEnd();
      break;
     
    case CMD_FROM_M0_RADIO_RXACK :
      Appli_m0RadioProcess_RxAck();
      break;  

    case CMD_FROM_M0_RADIO_RXOK :
      Appli_m0RadioProcess_RxOk();
      break;
       
   case CMD_FROM_M0_RADIO_RXACKEND :
      Appli_m0RadioProcess_RxAckEnd();
      break;  
               
    case CMD_FROM_M0_RADIO_RXOKEND :
      Appli_m0RadioProcess_RxOkEnd();
      break;
   
    default:
      break;
  }
}

static void m0CmdProcess(void)
{
  switch (m0Cmd) {
    
    case CMD_FROM_M0_RADIO_STOP :
      Appli_m0CmdProcess_RadioStop();
      break;
      
    case CMD_FROM_M0_RADIO_END :
      Appli_m0CmdProcess_RadioEnd();
      break;
      
    case CMD_FROM_M0_RADIO_RXACK :
      Appli_m0CmdProcess_RxAck();
      break;
      
    case CMD_FROM_M0_RADIO_RXOK :
      Appli_m0CmdProcess_RxOk();
      break;
  
    case CMD_FROM_M0_RADIO_RXACKEND :
      Appli_m0CmdProcess_RxAckEnd();
      break;

    case CMD_FROM_M0_RADIO_RXOKEND :
      Appli_m0CmdProcess_RxOkEnd();
      break;
    
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
      SystemClock_Config_MSI(0);
      break;
    
    case CMD_FROM_M0_USE_MSI_PLL :
      SystemClock_Config_MSI(1);
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
    
    default:
      break;
  }
}

static void m4ConfigBeforeStop(void)
{
  // Clear C1 Stop flag if any remaining
  LL_PWR_ClearFlag_C1STOP_C1STB();
}

static void m4ConfigAfterStop(void)
{
  // Clear C1 Stop flag
  LL_PWR_ClearFlag_C1STOP_C1STB();
}

static void m0CmdStopRequired(uint32_t stopRequired)
{
  switch (stopRequired) {
    case 0:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      HAL_Delay(delayBeforeSleepOnM4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLD_BLE_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_BEGIN();
      
      // Prepare system to be stopped
      m4ConfigBeforeStop();
      
      // Enter selected STOP mode
      HAL_PWREx_EnterSTOP0Mode(PWR_STOPENTRY_WFI);
      
      // Restart system
      m4ConfigAfterStop();
      
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLD_BLE_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      #if(CFG_DEBUG_TRACE != 0)
      MX_LPUART1_UART_Init();
      #endif
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_END();
      
      APP_DBG("M4 back from STOP0");
      break;
    
    case 1:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      HAL_Delay(delayBeforeSleepOnM4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLD_BLE_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_BEGIN();
      
      // Prepare system to be stopped
      m4ConfigBeforeStop();
      
      // Enter selected STOP mode
      HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
      
      // Restart system
      m4ConfigAfterStop();
      
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLD_BLE_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      #if(CFG_DEBUG_TRACE != 0)
      MX_LPUART1_UART_Init();
      #endif
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_END();
      
      APP_DBG("M4 back from STOP1");
      break;
    
    case 2:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      HAL_Delay(delayBeforeSleepOnM4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLD_BLE_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_BEGIN();
      
      // Prepare system to be stopped
      m4ConfigBeforeStop();
      
      // Enter selected STOP mode
      HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
      
      // Restart system
      m4ConfigAfterStop();
      
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLD_BLE_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      #if(CFG_DEBUG_TRACE != 0)
      MX_LPUART1_UART_Init();
      #endif
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_END();
      
      APP_DBG("M4 back from STOP2");
      break;
    
    case 3:
      // flush IPC and trace before sleeping. Let time to M0 to set RF in sleep and to be in STOP if needed
      HAL_Delay(delayBeforeSleepOnM4);
      
      // Stop UART and its GPIOs to reduce power consumption
      APP_LLD_BLE_DeInit_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      MX_LPUART1_UART_DeInit();
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_BEGIN();
      
      __WFI();
      // Restart UART before to go out of critical area to not have to send trace from M0 before to restart it
      APP_LLD_BLE_Init_UART_CLI();
#if (CFG_HW_LPUART1_ENABLED == 1)
      #if(CFG_DEBUG_TRACE != 0)
      MX_LPUART1_UART_Init();
      #endif
#endif
      
      // trial for no IT (ex RF) at low speed
      CRITICAL_END();
      
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
static void SendRspAckToM0(void)
{
  /* Notify M0 that CLI notification has been taken into account */
  TL_LLD_BLE_SendRspAck();
}

/**
 * @brief  This function is called when notification on CLI TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_CmdPacket_t
 * @return  None
 */
void TL_LLD_BLE_ReceiveRsp( TL_CmdPacket_t * Notbuffer )
{

  uint8_t         l_size = Notbuffer->cmdserial.cmd.plen;
  char *          sourceBuf = (char *)Notbuffer->cmdserial.cmd.payload;
  
  if (l_size > 0)
  {
    if (strncmp(sourceBuf, "Resp_End", 9) == 0)
    {
      /* This is an answer to indicate that the CLI command has been completed */
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_RSPACKEVT);

      /* Write a promp to UART and allow to receive a new command */
      //sprintf(cliPrompt, "LLD BLE > ");
      //uartTxBufferAdd("\r\n");
      //uartTxBufferAdd(cliPrompt);

      currentCommandPos = 0;
      rxCmdAllowed = 1;
    }
   else
    {
      /* This is just a trace from M0, write to CLI UART buffer */
      uartTxBufferAdd(sourceBuf);
    }
  }
  else
  {
    APP_DBG((char *)"!! Empty M0 CLI response received by M4 !!");
  }
  SendRspAckToM0();
}

/**
 * @brief Send Ack to M0 CLI Response channel.
 * @param  None
 * @retval None
 */
static void SendM0CmdAckToM0(void)
{
  /* Notify M0 that CLI notification has been taken into account */
  TL_LLD_BLE_SendM0CmdAck();
}

/**
 * @brief  This function is called when notification on CLI TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_CmdPacket_t
 * @return  None
 */
void TL_LLD_BLE_ReceiveM0Cmd( TL_CmdPacket_t * cmdBuffer )
{
  //TL_CmdPacket_t* l_CliBuffer = (TL_CmdPacket_t*)Notbuffer;
  uint8_t bufferSize = cmdBuffer->cmdserial.cmd.plen;
  char *  bufferAddr = (char *)cmdBuffer->cmdserial.cmd.payload;
  //uint32_t        length = *(uint32_t *)&l_CliBuffer->cmdserial.cmd.payload[4];

  if (bufferSize > 0)
  {
    if (strncmp(bufferAddr, "Radio_Stop", 10) == 0)
    {
      m0Cmd = CMD_FROM_M0_RADIO_STOP;
      
      /* Action under IT */
      m0RadioProcess();

      /* Action under Sequencer */
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);

    }
    else if (strncmp(bufferAddr, "Radio_End", 9) == 0)
    {
      m0Cmd = CMD_FROM_M0_RADIO_END;

      /* Action under IT */
      m0RadioProcess();

      /* Action under Sequencer */
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
      
    } 
    else if (strncmp(bufferAddr, "Radio_RxAckEnd", 14) == 0)
    {
      m0Cmd = CMD_FROM_M0_RADIO_RXACKEND;

      /* Action under IT */
      m0RadioProcess();

      /* Action under Sequencer */
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
      
    }    
    else if (strncmp(bufferAddr, "Radio_RxOkEnd", 13) == 0)
    {
      m0Cmd = CMD_FROM_M0_RADIO_RXOKEND;

      /* Action under IT */
      m0RadioProcess();

      /* Action under Sequencer */
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);

    }
    else if (strncmp(bufferAddr, "Radio_RxAck", 11) == 0)
    {
      m0Cmd = CMD_FROM_M0_RADIO_RXACK;

      /* Action under IT */
      m0RadioProcess();

      /* Action under Sequencer */
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
      
    }    
    else if (strncmp(bufferAddr, "Radio_RxOk", 10) == 0)
    {
      m0Cmd = CMD_FROM_M0_RADIO_RXOK;

      /* Action under IT */
      m0RadioProcess();

      /* Action under Sequencer */
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);

    }
    else if (strncmp(bufferAddr, "stop0_on", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP0_ON;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "stop1_on", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP1_ON;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "stop2_on", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP2_ON;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "stop_off", 8) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_STOP_OFF;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "go_in_wfi", 9) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_GO_IN_WFI;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "go_directly_in_wfi", 18) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_GO_DIRECTLY_IN_WFI;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "use_msi_no_pll", 14) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_MSI;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "use_msi_pll", 11) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_MSI_PLL;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "use_hse_no_pll", 14) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_HSE;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "use_hse_pll", 11) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_USE_HSE_PLL;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "ext_pa_enable", 13) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_EXT_PA_EN;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
    }
    else if (strncmp(bufferAddr, "ext_pa_disable", 14) == 0)
    {
      // Save the command
      m0Cmd = CMD_FROM_M0_EXT_PA_DIS;
      // Set the task to process the command
      UTIL_SEQ_SetTask(1U << CFG_TASK_CMD_FROM_M0_TO_M4, CFG_SCH_PRIO_0);
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

/**
 * @brief  This function is called to Send Command to M0, Every lld_ble API call this function
 *
 * @param   currentCmd : a value of the lld_ble API
 * @param   currentPt : a pointer to param_hal_BLE_t
 * @return  M0 return value
 */
uint8_t APP_LLD_BLE_SendCmdM0(uint8_t currentCmd , uint32_t* currentPt)
{
  /* M0 init done, send first command to have M0 code info and thus, a first prompt will be printed automatically */
  /* Do not accept new command from UART until this one is managed */
  rxCmdAllowed = 0;
  currentCommandPos = 5;
  currentCommand[0] = currentCmd ;
  currentCommand[1] = (uint8_t)((((uint32_t)(currentPt)) >> 0) & 0xFF);
  currentCommand[2] = (uint8_t)((((uint32_t)(currentPt)) >> 8) & 0xFF);
  currentCommand[3] = (uint8_t)((((uint32_t)(currentPt)) >> 16) & 0xFF);
  currentCommand[4] = (uint8_t)((((uint32_t)(currentPt)) >> 24) & 0xFF);
  
  currentCommand[5] = 0;

  /* Set corresponding task to send this command to M0 */
  //UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_TO_M0, CFG_SCH_PRIO_0);
  UTIL_SEQ_ClrEvt(1U << CFG_EVT_RECEIVE_RSPACKEVT);
  SendCmdToM0();
  
  UTIL_SEQ_WaitEvt(1U << CFG_EVT_RECEIVE_RSPACKEVT);

  /* Activate UART RX buffer processing task to allow USER command comming from UART */
  //UTIL_SEQ_SetTask(1U << CFG_TASK_PROCESS_UART_RX_BUFFER, CFG_SCH_PRIO_0);
  return(((param_hal_BLE_t*)(currentPt))->return_value);

}

/* USER CODE BEGIN FD */
/* Appli common functions */
void Appli_ProcessMode(void)
{
  uartRxBufferProcessMode(0); /* print mode */
}

void Appli_RegTask(void)
{
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_INIT , UTIL_SEQ_RFU, CHAT_BLE_Init);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_ENCRYPT , UTIL_SEQ_RFU, CHAT_ToggleEncrypt);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_SENDPACKET , UTIL_SEQ_RFU, CHAT_SendPacket);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_LISTENPACKET , UTIL_SEQ_RFU, CHAT_ListenPacket);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_STARTTONE , UTIL_SEQ_RFU, CHAT_StartTone);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HAL_BLE_STOPTONE , UTIL_SEQ_RFU, CHAT_StopTone);
}

void Appli_Init(void)
{
  sprintf(cliPrompt, "LLD BLE > ");
  uartTxBufferAdd("\r\n");
  uartTxBufferAdd(cliPrompt);

  // Init CHAT Appli
  UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_INIT, CFG_SCH_PRIO_0);
}

void Appli_uartRxBufferPrint(void)
{
        sprintf(cliPrompt, "LLD BLE > ");
        uartTxBufferAdd(cliPrompt);
}

/* Appli radio functions */
void Appli_m0RadioProcess_RadioStop(void)
{
      /* copy status from M0 BLE Radio */
      radioPacketNb=bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer[257]; 
      *rxStatus_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.status; // status
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
}

void Appli_m0RadioProcess_RadioEnd(void)
{
      /* copy status from M0 BLE Radio */
      radioPacketNb=bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer[257];
      *rxStatus_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.status; // status
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
}

void Appli_m0RadioProcess_RxAck(void)
{
      /* copy status from M0 BLE Radio */
      radioPacketNb=bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer[257];
      *rxStatus_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.status; // status
      *rxTimeStamp_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.timestamp_receive; // timestamp_receive
      *rxRSSI_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.rssi; // rssi
      /* copy payload from M0 BLE Radio */
      memcpy(rxBuffer_Ptr[radioPacketNb],(bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer),258); 
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
}

void Appli_m0RadioProcess_RxOk(void)
{
      /* copy status from M0 BLE Radio */
      radioPacketNb=bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer[257];
      *rxStatus_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.status; // status
      *rxTimeStamp_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.timestamp_receive; // timestamp_receive
      *rxRSSI_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.rssi; // rssi
      /* copy payload from M0 BLE Radio */
      memcpy(rxBuffer_Ptr[radioPacketNb],(bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer),258); 
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
}

void Appli_m0RadioProcess_RxAckEnd(void)
{
      /* copy status from M0 BLE Radio */
      radioPacketNb=bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer[257];
      *rxStatus_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.status; // status
      *rxTimeStamp_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.timestamp_receive; // timestamp_receive
      *rxRSSI_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.rssi; // rssi
      /* copy payload from M0 BLE Radio */
      memcpy(rxBuffer_Ptr[radioPacketNb],(bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer),258); 
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
}

void Appli_m0RadioProcess_RxOkEnd(void)
{
      /* copy status from M0 BLE Radio */
      radioPacketNb=bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer[257];
      *rxStatus_Ptr[radioPacketNb]=bleparam_hal_BLE_Packet.status; // status
      /* copy payload from M0 BLE Radio */
      memcpy(rxBuffer_Ptr[radioPacketNb],(bleparam_hal_BLE_Packet.txrxBuffer.rxBuffer),258); 
      UTIL_SEQ_SetEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
}

void Appli_m0CmdProcess_RadioStop(void)
{
}

void Appli_m0CmdProcess_RadioEnd(void)
{
      if (uartLastChar)
      {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0); // Chat
        uartLastChar=0;
      } else {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_LISTENPACKET, CFG_SCH_PRIO_0); // Chat
      }
}

void Appli_m0CmdProcess_RxAck(void)
{
      sprintf(chatPrompt, "\0");
      if (chatEncrypt!=0) {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]-4; // Chat Length - 4 for Encrypt
      } else {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]; // Chat Length
      }

      for (uint8_t i=0 ; i < radioLength ; i++)
      {
        sprintf(chatPrompt, "%s%c",chatPrompt, rxBuffer_Ptr[radioPacketNb][i+2]); // Record ID from Stack_Packet
      }
      uartTxBufferAdd(chatPrompt);  
      uartTxBufferAdd("\r\n");
      sprintf(cliPrompt, "LLD BLE > ");
      uartTxBufferAdd(cliPrompt);
      
      if (uartLastChar)
      {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0); // Chat
        uartLastChar=0;
      } else {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_LISTENPACKET, CFG_SCH_PRIO_0); // Chat
      }
}

void Appli_m0CmdProcess_RxOk(void)
{
      sprintf(chatPrompt, "\0");
      if (chatEncrypt!=0) {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]-4; // Chat Length - 4 for Encrypt
      } else {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]; // Chat Length
      }

      for (uint8_t i=0 ; i < radioLength ; i++)
      {
        sprintf(chatPrompt, "%s%c",chatPrompt, rxBuffer_Ptr[radioPacketNb][i+2]); // Record ID from Stack_Packet
      }
      uartTxBufferAdd(chatPrompt);  
      uartTxBufferAdd("\r\n");
      sprintf(cliPrompt, "LLD BLE > ");
      uartTxBufferAdd(cliPrompt);  

      if (uartLastChar)
      {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0); // Chat
        uartLastChar=0;
      } else {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_LISTENPACKET, CFG_SCH_PRIO_0); // Chat
      }
}

void Appli_m0CmdProcess_RxAckEnd(void)
{
      sprintf(chatPrompt, "\0");
      if (chatEncrypt!=0) {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]-4; // Chat Length - 4 for Encrypt
      } else {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]; // Chat Length
      }

      for (uint8_t i=0 ; i < radioLength ; i++)
      {
        sprintf(chatPrompt, "%s%c",chatPrompt, rxBuffer_Ptr[radioPacketNb][i+2]); // Record ID from Stack_Packet
      }
      uartTxBufferAdd(chatPrompt);  
      uartTxBufferAdd("\r\n");
      sprintf(cliPrompt, "LLD BLE > ");
      uartTxBufferAdd(cliPrompt);
      
      if (uartLastChar)
      {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0); // Chat
        uartLastChar=0;
      } else {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_LISTENPACKET, CFG_SCH_PRIO_0); // Chat
      }
}

void Appli_m0CmdProcess_RxOkEnd(void)
{
      sprintf(chatPrompt, "\0");
      if (chatEncrypt!=0) {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]-4; // Chat Length - 4 for Encrypt
      } else {
         radioLength=rxBuffer_Ptr[radioPacketNb][1]; // Chat Length
      }

      for (uint8_t i=0 ; i < radioLength ; i++)
      {
        sprintf(chatPrompt, "%s%c",chatPrompt, rxBuffer_Ptr[radioPacketNb][i+2]); // Record ID from Stack_Packet
      }
      uartTxBufferAdd(chatPrompt);  
      uartTxBufferAdd("\r\n");
      sprintf(cliPrompt, "LLD BLE > ");
      uartTxBufferAdd(cliPrompt);  

      if (uartLastChar)
      {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_SENDPACKET, CFG_SCH_PRIO_0); // Chat
        uartLastChar=0;
      } else {
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_LISTENPACKET, CFG_SCH_PRIO_0); // Chat
      }
}

/* Appli custom functions */
void CHAT_BLE_Init(void)
{        
        #ifdef LLD_BLE_DEBUG
        bleparam_hal_BLE_Packet.InfoTime=(uint32_t)debug_timer;
        #endif
        
        HAL_BLE_Init();
        HAL_BLE_SetNetworkID(chatID);
        
        /* Encryption Parameters */
        LLD_BLE_SetEncryptionAttributes(STATE_MACHINE_0, chatenc_iv, chatenc_key);
        LLD_BLE_SetEncryptionCount(STATE_MACHINE_0, &chatcount_tx[0], &chatcount_rcv[0]); 
        
        BSP_LED_On(LED_BLUE);
        BSP_LED_Off(LED_GREEN);
        BSP_LED_Off(LED_RED);
        UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_LISTENPACKET , CFG_SCH_PRIO_0);
}

void CHAT_Encrypt(void)
{
       // enable Encrypt
      chatEncrypt = 1;
      LLD_BLE_SetEncryptFlags(STATE_MACHINE_0, ENABLE, ENABLE);
      // end Encrypt

      BSP_LED_Off(LED_BLUE);
}

void CHAT_UnEncrypt(void)
{
       // disable Encrypt
      chatEncrypt = 0;
      LLD_BLE_SetEncryptFlags(STATE_MACHINE_0, DISABLE, DISABLE);
      // end Encrypt

      BSP_LED_On(LED_BLUE);
}

void CHAT_ToggleEncrypt(void)

{
      LLD_BLE_StopActivity();
      uint32_t iTimeTone = 0 ;
      uint32_t TimeTone;
      while((LLD_BLE_GetStatus(&TimeTone)) != 0)
      {
        iTimeTone++;
        HAL_Delay(100);
      }  
      if (chatEncrypt!=0) {
        CHAT_UnEncrypt();
        uartTxBufferAdd("\r\n");
        sprintf(cliPrompt, "LLD BLE > ************ UnEncrypted **************");
        uartTxBufferAdd(cliPrompt);
        uartTxBufferAdd("\r\n");
        sprintf(cliPrompt, "LLD BLE > ");
        uartTxBufferAdd(cliPrompt);

      } else {
        CHAT_Encrypt();
        uartTxBufferAdd("\r\n");
        sprintf(cliPrompt, "LLD BLE > ************* Encrypted ***************");
        uartTxBufferAdd(cliPrompt);
        uartTxBufferAdd("\r\n");
        sprintf(cliPrompt, "LLD BLE > ");
        uartTxBufferAdd(cliPrompt);
      }
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_LISTENPACKET , CFG_SCH_PRIO_0);
      


}

void CHAT_SendPacket(void)
{
      chattxBuffer[0]= 0x68; // Chat Header
      if (chatEncrypt!=0) {
         chattxBuffer[1]= uartPayload[1]+4; // Chat Length + 4 for Encrypt
      } else {
         chattxBuffer[1]= uartPayload[1]; // Chat Length
      }
      memcpy(&chattxBuffer[2],&uartPayload[2],uartPayload[1]);
      APP_LLD_BLE_SetdataRoutineOption(chatPacketNumberTx,chatPacketStopRx);
        #ifdef LLD_BLE_DEBUG
        start_timer=Debug_Start_timer();
        #endif
      HAL_BLE_SendPacketWithAck(chatChannel, chatWakeup, chattxBuffer, chatAckrxBuffer, chatReceiveAck, dataRoutine_HAL_TxAck);
        #ifdef LLD_BLE_DEBUG
        stop_timer=Debug_Stop_timer();
        #endif
      UTIL_SEQ_ClrEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
      BSP_LED_Off(LED_GREEN);
      HAL_NVIC_EnableIRQ(BUTTON_SW1_EXTI_IRQn);
}

void CHAT_ListenPacket(void)
{
      if (chatEncrypt!=0) {
         chatAcktxBuffer[1]= 19; // Chat Length + 4 for Encrypt
      } else {
         chatAcktxBuffer[1]= 13; // Chat Length
      }
      APP_LLD_BLE_SetdataRoutineOption(chatPacketNumberRx,chatPacketStopRx);
      HAL_BLE_ReceivePacketWithAck(chatChannel, chatWakeup, chatrxBuffer, chatAcktxBuffer, chatReceive, dataRoutine_HAL_RxAck);
      UTIL_SEQ_ClrEvt(1U << CFG_EVT_RECEIVE_ENDPACKEVT);
      BSP_LED_On(LED_GREEN);
      BSP_LED_On(LED_RED);
      HAL_NVIC_EnableIRQ(BUTTON_SW1_EXTI_IRQn);
}

void CHAT_StopRadio(void)
{
      LLD_BLE_StopActivity();
      BSP_LED_Off(LED_GREEN);
      uint32_t iTimeTone = 0 ;
      uint32_t TimeTone;
      while((LLD_BLE_GetStatus(&TimeTone)) != 0)
      {
        iTimeTone++;
        HAL_Delay(100);
      }  
      BSP_LED_Off(LED_RED);
}
     
void CHAT_StartTone(void)
{
      CHAT_StopRadio(); 

      LLD_BLE_StartTone(14, 30); /* 14 for BLE channel 12*/
      BSP_LED_On(LED_GREEN);
}

void CHAT_StopTone(void)
{
      LLD_BLE_StopTone();
      BSP_LED_Off(LED_GREEN);
      BSP_LED_On(LED_RED);
}

/* USER CODE END FD */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
void Appli_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case BUTTON_SW1_PIN:
      HAL_NVIC_DisableIRQ(BUTTON_SW1_EXTI_IRQn);
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_ENCRYPT, CFG_SCH_PRIO_0);
      break; 

    case BUTTON_SW2_PIN:
      HAL_NVIC_DisableIRQ(BUTTON_SW2_EXTI_IRQn);
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_STARTTONE, CFG_SCH_PRIO_0);
      break; 

    case BUTTON_SW3_PIN:
      HAL_NVIC_DisableIRQ(BUTTON_SW3_EXTI_IRQn);
      UTIL_SEQ_SetTask(1U << CFG_TASK_HAL_BLE_STOPTONE, CFG_SCH_PRIO_0);
      break;

    default:
      break;

  }
  return;
}
void Appli_TIM_IC_CaptureCallback(void)
{
}

void Appli_TIM_PeriodElapsedCallback(void)
{
}
/* USER CODE END FD_WRAP_FUNCTIONS */

/* debug */
uint32_t Debug_Start_timer(void)
{
  debug_timer[0]=__HAL_TIM_GET_COUNTER(&htim2);
  if(!debug_timer[2])
  {
  return 0;
  } else
  {
  return(debug_timer[0]-debug_timer[2]);
  }
}
uint32_t Debug_Stop_timer(void)
{
  debug_timer[1]=__HAL_TIM_GET_COUNTER(&htim2);
  return(debug_timer[1]-debug_timer[0]);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
