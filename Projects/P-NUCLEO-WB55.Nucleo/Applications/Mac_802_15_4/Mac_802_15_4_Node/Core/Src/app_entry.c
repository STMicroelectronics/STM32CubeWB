/**
  ******************************************************************************
 * @file    app_entry.c
 * @author  MCD Application Team
 * @brief   Entry point of the Application
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
#include <stdio.h>
#include <string.h>
#include "app_entry.h"

#include "app_mac_802_15_4.h"
#include "app_conf.h"
#include "utilities_common.h"
#include "utilities_conf.h"
#include "otp.h"
#include "stm32_seq.h"
#include "stm_logging.h"
#include "stm32wbxx_ll_rcc.h"
#include "shci.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "tl_mac_802_15_4.h"
#include "tl.h"
#include "dbg_trace.h"

#include "stm_logging.h"

#define HOST_SYS_EVTCODE                (0xFFU)
#define HOST_SYS_SUBEVTCODE_BASE        (0x9200U)
#define HOST_SYS_SUBEVTCODE_READY        (HOST_SYS_SUBEVTCODE_BASE + 0U)
#define POOL_SIZE (CFG_TL_EVT_QUEUE_LENGTH * 4U * DIVC(( sizeof(TL_PacketHeader_t) + TL_EVENT_FRAME_SIZE ), 4U))

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

#define C_SIZE_CMD_STRING               256U
#define RX_BUFFER_SIZE          8U

extern RTC_HandleTypeDef hrtc;
static uint8_t aRxBuffer[RX_BUFFER_SIZE];
EXTI_HandleTypeDef exti_handle;
/* Global variables  -------------------------------------------------*/
char CommandString[C_SIZE_CMD_STRING];
__IO uint16_t indexReceiveChar = 0U;
__IO uint16_t remainingRxChar = 0U;

/* Private function definition -------------------------------------------------*/

PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_MAC_802_15_4_Config_t Mac_802_15_4_ConfigBuffer;

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t Mac_802_15_4_CmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t Mac_802_15_4_NotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) char mac_802_15_4_CnfIndNot[C_SIZE_CMD_STRING];

/*----------------------------------------------------------------------------*/
static TL_CmdPacket_t *p_mac_802_15_4_cmdbuffer;
static TL_EvtPacket_t *p_mac_802_15_4_notif_RFCore_to_M4;

static volatile uint8_t  pendingMsgFromRFCore = 0U; /* Pending Tx From Core Protocol status */


/* Global function prototypes -----------------------------------------------*/
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);

/* Private function prototypes -----------------------------------------------*/
static void appe_Tl_Init(void);
static void Led_Init(void);
static void Button_Init(void);
static void Wait_Getting_Ack_From_RFCore(void);
static void Receive_Ack_From_RFCore(void);
static void Receive_Notification_From_RFCore(void);
static void APPE_StatusNot(SHCI_TL_CmdStatus_t status);
static void APPE_UserEvtRx(void * pPayload);
static void Config_HSE(void);
static void Reset_Device(void);
#if (CFG_HW_RESET_BY_FW == 1)
static void Reset_IPCC(void);
static void Reset_BackupDomain(void);
#endif /* CFG_HW_RESET_BY_FW == 1*/
static void System_Init(void);
static void SystemPower_Config(void);
static void Init_Debug(void);
static void Init_Rtc(void);

void MX_APPE_Config(void)
{
  /**
   * The OPTVERR flag is wrongly set at power on
   * It shall be cleared before using any HAL_FLASH_xxx() api
   */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  /**
   * Reset some configurations so that the system behave in the same way
   * when either out of nReset or Power On
   */
  Reset_Device();

  /* Configure HSE Tuning */
  Config_HSE();

  return;
}

static void Init_Debug(void)
{
#if (CFG_DEBUGGER_SUPPORTED == 1)
  /**
   * Keep debugger enabled while in any low power mode
   */
  HAL_DBGMCU_EnableDBGSleepMode();

  /***************** ENABLE DEBUGGER *************************************/
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);
  LL_C2_EXTI_EnableIT_32_63(LL_EXTI_LINE_48);

#else

  GPIO_InitTypeDef gpio_config = {0};

  gpio_config.Pull = GPIO_NOPULL;
  gpio_config.Mode = GPIO_MODE_ANALOG;

  gpio_config.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &gpio_config);
  __HAL_RCC_GPIOA_CLK_DISABLE();

  gpio_config.Pin = GPIO_PIN_4 | GPIO_PIN_3;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &gpio_config);
  __HAL_RCC_GPIOB_CLK_DISABLE();

  HAL_DBGMCU_DisableDBGSleepMode();
  HAL_DBGMCU_DisableDBGStopMode();
  HAL_DBGMCU_DisableDBGStandbyMode();

#endif /* (CFG_DEBUGGER_SUPPORTED == 1) */

#if (CFG_DEBUG_TRACE != 0)
  DbgTraceInit();
#endif /* CFG_DEBUG_TRACE != 0 */

  return;
}
static void Reset_Device(void)
{
#if (CFG_HW_RESET_BY_FW == 1)
  Reset_BackupDomain();

  Reset_IPCC();
#endif /* CFG_HW_RESET_BY_FW == 1 */

  return;
}

#if (CFG_HW_RESET_BY_FW == 1)
static void Reset_BackupDomain(void)
{
  if ((LL_RCC_IsActiveFlag_PINRST() != FALSE) && (LL_RCC_IsActiveFlag_SFTRST() == FALSE))
  {
    HAL_PWR_EnableBkUpAccess(); /**< Enable access to the RTC registers */

    /**
     *  Write twice the value to flush the APB-AHB bridge
     *  This bit shall be written in the register before writing the next one
     */
    HAL_PWR_EnableBkUpAccess();

    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();
  }

  return;
}

static void Reset_IPCC(void)
{
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_IPCC);

  LL_C1_IPCC_ClearFlag_CHx(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_ClearFlag_CHx(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C1_IPCC_DisableTransmitChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_DisableTransmitChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C1_IPCC_DisableReceiveChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  LL_C2_IPCC_DisableReceiveChannel(
      IPCC,
      LL_IPCC_CHANNEL_1 | LL_IPCC_CHANNEL_2 | LL_IPCC_CHANNEL_3 | LL_IPCC_CHANNEL_4
      | LL_IPCC_CHANNEL_5 | LL_IPCC_CHANNEL_6);

  return;
}
#endif /* CFG_HW_RESET_BY_FW == 1 */

static void Config_HSE(void)
{
    OTP_ID0_t * p_otp;

  /**
   * Read HSE_Tuning from OTP
   */
  p_otp = (OTP_ID0_t *) OTP_Read(0);
  if (p_otp)
  {
    LL_RCC_HSE_SetCapacitorTuning(p_otp->hse_tuning);
  }

  return;
}


void Init_Smps(void)
{
#if (CFG_USE_SMPS != 0)
  /**
   *  Configure and enable SMPS
   *
   *  The SMPS configuration is not yet supported by CubeMx
   *  when SMPS output voltage is set to 1.4V, the RF output power is limited to 3.7dBm
   *  the SMPS output voltage shall be increased for higher RF output power
   */
  LL_PWR_SMPS_SetStartupCurrent(LL_PWR_SMPS_STARTUP_CURRENT_80MA);
  LL_PWR_SMPS_SetOutputVoltageLevel(LL_PWR_SMPS_OUTPUT_VOLTAGE_1V40);
  LL_PWR_SMPS_Enable();
#endif /* CFG_USE_SMPS != 0 */

  return;
}

void Init_Exti(void)
{
  /* Enable IPCC(36), HSEM(38) wakeup interrupts on CPU1 */
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_36 | LL_EXTI_LINE_38);

  return;
}


static void System_Init(void)
{
  Init_Smps();

  Init_Exti();

  Init_Rtc();

  return;
}

static void Init_Rtc(void)
{
  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);

  LL_RTC_WAKEUP_SetClock(RTC, CFG_RTC_WUCKSEL_DIVIDER);

  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);

  return;
}

/**
 * @brief  Configure the system for power optimization
 *
 * @note  This API configures the system to be ready for low power mode
 *
 * @param  None
 * @retval None
 */
static void SystemPower_Config(void)
{
  /* Before going to stop or standby modes, do the settings so that system clock and IP80215.4 clock start on HSI automatically */
  LL_RCC_HSI_EnableAutoFromStop();

  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init();
  /* Initialize the CPU2 reset value before starting CPU2 with C2BOOT */
  LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

  /* Disable Stop & Off Modes until Initialisation is complete */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

#if (CFG_USB_INTERFACE_ENABLE != 0)
  /**
   *  Enable USB power
   */
  HAL_PWREx_EnableVddUSB();
#endif /* CFG_USB_INTERFACE_ENABLE != 0 */

  return;
}

void MX_APPE_Init(void)
{
  System_Init();       /**< System initialization */

  SystemPower_Config(); /**< Configure the system Power Mode */

  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

/* USER CODE BEGIN APPE_Init_1 */
  Init_Debug();

  Led_Init();
  Button_Init();
  RxUART_Init();

/* USER CODE END APPE_Init_1 */
  appe_Tl_Init();	/* Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event (VS_HCI_C2_Ready)
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */

   return;
}

static void appe_Tl_Init( void )
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;

  /**< Reference table initialization */
  TL_Init();

  /**< System channel initialization */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SYSTEM_HCI_ASYNCH_EVT, UTIL_SEQ_RFU, shci_user_evt_proc );
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_StatusNot;
  shci_init(APPE_UserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  tl_mm_config.p_BleSpareEvtBuffer = 0U;
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init( &tl_mm_config );

  TL_Enable();

  return;
}

static void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1U)
  /**
   * Leds Initialization
   */
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
#endif
  return;
}

static void Button_Init( void )
{
#if (CFG_BUTTON_SUPPORTED == 1U)
  /* Button Initialization */
  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW2, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW3, BUTTON_MODE_EXTI);
#endif /* (CFG_BUTTON_SUPPORTED == 1U) */

  return;
}

static void APPE_StatusNot( SHCI_TL_CmdStatus_t status )
{
  return;
}

/**
 * The type of the payload for a system user event is tSHCI_UserEvtRxParam
 * When the system event is both :
 *    - a ready event (subevtcode = SHCI_SUB_EVT_CODE_READY)
 *    - reported by the FUS (sysevt_ready_rsp == FUS_FW_RUNNING)
 * The buffer shall not be released
 * ( eg ((tSHCI_UserEvtRxParam*)pPayload)->status shall be set to SHCI_TL_UserEventFlow_Disable )
 * When the status is not filled, the buffer is released by default
 */
static void APPE_UserEvtRx( void * pPayload )
{
  /* Traces channel initialization */
  TL_TRACES_Init( );

  APP_MAC_802_15_4_Init(APP_MAC_802_15_4_FULL, &Mac_802_15_4_CmdBuffer);

  UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);

  return;
}

void APP_ENTRY_TL_MAC_802_15_4_Init(void)
{
  Mac_802_15_4_ConfigBuffer.p_Mac_802_15_4_CmdRspBuffer = (uint8_t*)&Mac_802_15_4_CmdBuffer;
  Mac_802_15_4_ConfigBuffer.p_Mac_802_15_4_NotAckBuffer = (uint8_t*)Mac_802_15_4_NotifRspEvtBuffer;
  TL_MAC_802_15_4_Init( &Mac_802_15_4_ConfigBuffer );
}


/**
 * @brief Process the messages coming from the RF Core.
 * @param  None
 * @retval None
 */
void APP_ENTRY_ProcessMsgFromRFCoreTask(void)
{
  if (pendingMsgFromRFCore != 0U)
  {
      pendingMsgFromRFCore = 0U;
      MAC_802_15_4_CallBack_Processing();
  }
}

/* Received trace buffer from M0 */
void TL_TRACES_EvtReceived( TL_EvtPacket_t * hcievt )
{
#if(CFG_DEBUG_TRACE != 0)
  /* Call write/print function using DMA from dbg_trace */
  /* - Cast to TL_AsynchEvt_t* to get "real" payload (without Sub Evt code 2bytes),
     - (-2) to size to remove Sub Evt Code */
  DbgTraceWrite(1U, (const unsigned char *) ((TL_AsynchEvt_t *)(hcievt->evtserial.evt.payload))->payload, hcievt->evtserial.evt.plen - 2U);
#endif /* CFG_DEBUG_TRACE */
  /* Release buffer */
  TL_MM_EvtDone( hcievt );
}

/**
  * @brief  This function is called by the scheduler each time an event
  *         is pending.
  *
  * @param  evt_waited_bm : Event pending.
  * @retval None
  */
void UTIL_SEQ_EvtIdle( UTIL_SEQ_bm_t task_id_bm, UTIL_SEQ_bm_t evt_waited_bm )
{
  switch(evt_waited_bm)
  {
  case EVENT_ACK_FROM_RFCore_EVT:
    UTIL_SEQ_Run(0);
    break;
  case EVENT_RESET_CNF:
  case EVENT_SET_CNF:
  case EVENT_GET_CNF:
  case EVENT_START_CNF:
  case EVENT_DATA_CNF:
  case EVENT_RX_ON_WHEN_IDLE_CNF:
  case EVENT_SCAN_CNF:
  case EVENT_POLL_CNF:
  case EVENT_DISASSOCIATE_CNF:
    UTIL_SEQ_Run(TASK_MSG_FROM_RF_CORE);
    break;
  case EVENT_SYNCHRO_BYPASS_IDLE:
    UTIL_SEQ_SetEvt(EVENT_SYNCHRO_BYPASS_IDLE);
    break;
  default :
    /* default case */
    UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
    break;
  }
}

void shci_notify_asynch_evt(void* pdata)
{
  UTIL_SEQ_SetTask( 1U<<CFG_TASK_SYSTEM_HCI_ASYNCH_EVT, CFG_SCH_PRIO_0);
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UTIL_SEQ_SetEvt( 1U<< CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP);
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UTIL_SEQ_WaitEvt( 1U<< CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP );
  return;
}

/**
  * @brief  Initialisation of the trace mechansimn
  * @param  None
  * @retval None
  */
#if(CFG_DEBUG_TRACE != 0U)
void DbgOutputInit( void )
{
  HW_UART_Init(CFG_DEBUG_TRACE_UART);
}

/**
  * @brief  Management of the traces
  * @param  p_data : data
  * @param  size : size
  * @param  call-back :
  * @retval None
  */
void DbgOutputTraces( uint8_t *p_data, uint16_t size, void (*cb)(void) )
{
  HW_UART_Transmit_DMA(CFG_DEBUG_TRACE_UART, p_data, size, cb);
}

#endif


/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_ENTRY_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer)
{
  p_mac_802_15_4_cmdbuffer = p_buffer;
}

TL_CmdPacket_t* MAC_802_15_4_GetCmdBuffer(void)
{
  return (TL_CmdPacket_t*)p_mac_802_15_4_cmdbuffer;
}

TL_Evt_t* MAC_802_15_4_GetRspPayEvt(void)
{
  return &((TL_EvtPacket_t *)p_mac_802_15_4_cmdbuffer)->evtserial.evt;
}

TL_Evt_t* MAC_802_15_4_GetNotificationBuffer(void)
{
  return &(p_mac_802_15_4_notif_RFCore_to_M4->evtserial.evt);
}

MAC_802_15_4_Notification_t* MAC_802_15_4_GetNotificationPayloadBuffer(void)
{
  return (MAC_802_15_4_Notification_t*)(p_mac_802_15_4_notif_RFCore_to_M4)->evtserial.evt.payload;
}

/**
 * @brief  This function is used to transfer the MAC 802.15.4 commands from the
 *         M4 to the RFCore.
 *
 * @param   pCmdBuffer : pointer to the buffer to send
 * @return  None
 */
void Mac_802_15_4_CmdTransfer(void)
{
  TL_MAC_802_15_4_SendCmd();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_RFCore();
}

/* For reception of MAC 802.15.4 Cmd return */
void TL_MAC_802_15_4_CmdEvtReceived( TL_EvtPacket_t * Otbuffer )
{
  Receive_Ack_From_RFCore();
}

/* For reception of MAC 802.15.4 Notification from RFCore */
void TL_MAC_802_15_4_NotReceived( TL_EvtPacket_t * Notbuffer )
{
  p_mac_802_15_4_notif_RFCore_to_M4 = Notbuffer;

  Receive_Notification_From_RFCore();
}

/**
  * @brief  This function is called before sending any ot command to the RFCore
  *         core. The purpose of this function is to be able to check if
  *         there are no notifications coming from the RFCore core which are
  *         pending before sending a new ot command.
  * @param  None
  * @retval None
  */
void Mac_802_15_4_PreCmdProcessing(void)
{
  UTIL_SEQ_WaitEvt( EVENT_SYNCHRO_BYPASS_IDLE );
}

/*************************************************************
 *
 * STATIC FUNCTIONS
 *
 *************************************************************/

/**
  * @brief  This function waits for getting an acknowledgment from the RFCore.
  *
  * @param  None
  * @retval None
  */
static void Wait_Getting_Ack_From_RFCore(void)
{
  UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_RFCore_EVT);
}

/**
  * @brief  Receive an acknowledgment from the RFCore core.
  *         Each command send by the M4 to the RFCore are acknowledged.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Ack_From_RFCore(void)
{
  UTIL_SEQ_SetEvt(EVENT_ACK_FROM_RFCore_EVT);
}

/**
  * @brief  Receive a notification from the RFCore+ through the IPCC.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Notification_From_RFCore(void)
{
  pendingMsgFromRFCore = 1;
  UTIL_SEQ_SetTask(TASK_MSG_FROM_RF_CORE,CFG_SCH_PRIO_0);
}

static void RxUART_Init(void)
{
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    if (aRxBuffer[0] == '\r' || aRxBuffer[0] == '\n') 
    {
      APP_DBG("received %s", CommandString);

      UartCmdExecute();

      /* Clear receive buffer and character counter*/
      indexReceiveChar = 0;
      memset(CommandString, 0, C_SIZE_CMD_STRING);
    }
    else
    {
      CommandString[indexReceiveChar++] = aRxBuffer[0];
    }
  }

  /* Once a character has been sent, put back the device in reception mode */
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void UartCmdExecute(void)
{
  /* Parse received CommandString */
  if(strcmp((char const*)CommandString, "SW1") == 0)
  {
    APP_DBG("SW1 OK");
    exti_handle.Line = EXTI_LINE_4;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW2") == 0)
  {
    APP_DBG("SW2 OK");
    exti_handle.Line = EXTI_LINE_0;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else if (strcmp((char const*)CommandString, "SW3") == 0)
  {
    APP_DBG("SW3 OK");
    exti_handle.Line = EXTI_LINE_1;
    HAL_EXTI_GenerateSWI(&exti_handle);
  }
  else
  {
    APP_DBG("NOT RECOGNIZED COMMAND : %s", CommandString);
  }
}
