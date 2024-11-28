/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : app_entry.c
  * Description        : Entry application source file for STM32WPAN Middleware.
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
#include "main.h"
#include "app_entry.h"
#include "app_zigbee.h"
#include "stm32_seq.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "dbg_trace.h"
#include "shci.h"
#include "otp.h"
#include "stlogo.h"
#include "stm_logging.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* POOL_SIZE = 2(TL_PacketHeader_t) + 258 (3(TL_EVT_HDR_SIZE) + 255(Payload size)) */
#define POOL_SIZE (CFG_TL_EVT_QUEUE_LENGTH * 4U * DIVC(( sizeof(TL_PacketHeader_t) + TL_EVENT_FRAME_SIZE ), 4U))

/* USER CODE BEGIN PD */
aPwmLedGsData_TypeDef aPwmLedGsData;
uint32_t LCD_Inst;

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
extern uint8_t g_ot_notification_allowed;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Global function prototypes -----------------------------------------------*/
#if(CFG_DEBUG_TRACE != 0)
size_t DbgTraceWrite(int handle, const unsigned char * buf, size_t bufSize);
#endif

/* USER CODE BEGIN GFP */

/* USER CODE END GFP */

/* Private functions prototypes-----------------------------------------------*/
static void Config_HSE(void);
static void Reset_Device( void );
#if (CFG_HW_RESET_BY_FW == 1)
static void Reset_IPCC(void);
static void Reset_BackupDomain(void);
#endif /* CFG_HW_RESET_BY_FW == 1*/
static void System_Init(void);
static void SystemPower_Config( void );
static void Init_Debug( void );
static void appe_Tl_Init( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx( void * pPayload );
static void APPE_SysEvtReadyProcessing( void );
static void APPE_SysEvtError( SCHI_SystemErrCode_t ErrorCode);
static void Init_Rtc(void);

/* USER CODE BEGIN PFP */
static void Led_Init(void);
static void Button_Init(void);
static void LCD_DisplayInit(void);

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U

static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;
EXTI_HandleTypeDef exti_handle;

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void MX_APPE_Config( void )
{
  /**
   * The OPTVERR flag is wrongly set at power on
   * It shall be cleared before using any HAL_FLASH_xxx() api
   */
  __HAL_FLASH_CLEAR_FLAG( FLASH_FLAG_OPTVERR );

  /**
   * Reset some configurations so that the system behave in the same way
   * when either out of nReset or Power On
   */
  Reset_Device( );

  /* Configure HSE Tuning */
  Config_HSE();

  return;
}

void MX_APPE_Init( void )
{
  System_Init( );       /**< System initialization */

  SystemPower_Config(); /**< Configure the system Power Mode */

  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

/* USER CODE BEGIN MX_APPE_Init_1 */
  Init_Debug();
  
  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);
  LCD_DisplayInit();

  Led_Init();
  /* Initialize user buttons */
  Button_Init();
  RxUART_Init();
  
/* USER CODE END MX_APPE_Init_1 */

  appe_Tl_Init();	/* Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
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

/* USER CODE BEGIN FD */
/**
 * @brief  LED Deinitialisation to avoid conflict with LCD display
 * @param  None
 * @retval None
 */
void LED_Deinit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure = {0};
  
  /* RGB Led de-init */
  BSP_PWM_LED_DeInit();
 
  /* configure SPIx MOSI for LCD */
  GPIO_InitStructure.Pin       = BUS_SPI1_MOSI_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Alternate = BUS_SPI1_AF;
  HAL_GPIO_Init(BUS_SPI1_GPIO_PORTA, &GPIO_InitStructure);
}

/**
 * @brief  Wrapper for RGB LED on to avoid conflict with LCD Display
 * @param  None
 * @retval None
 */
void LED_On(void)
{
  LED_Set_rgb(PWM_LED_GSDATA_47_0, PWM_LED_GSDATA_47_0, PWM_LED_GSDATA_47_0);
}

/**
 * @brief  Wrapper for RGB LED off to avoid conflict with LCD Display
 * @param  None
 * @retval None
 */
void LED_Off(void)
{
  BSP_PWM_LED_Init();
  BSP_PWM_LED_Off();
  LED_Deinit();
}

/**
 * @brief  Wrapper to set RGB LED to avoid conflict with LCD Display
 * @param  None
 * @retval None
 */
void LED_Set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  aPwmLedGsData_TypeDef aPwmLedGsData;
  
  BSP_PWM_LED_Init();
  aPwmLedGsData[PWM_LED_RED] = r;
  aPwmLedGsData[PWM_LED_GREEN] = g;
  aPwmLedGsData[PWM_LED_BLUE] = b;
  BSP_PWM_LED_On(aPwmLedGsData);
  LED_Deinit();
}

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
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

static void Init_Debug( void )
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

#if(CFG_DEBUG_TRACE != 0)
  DbgTraceInit();
#endif

  return;
}

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

static void System_Init( void )
{
  Init_Smps( );

  Init_Exti( );

  Init_Rtc( );

  return;
}

static void Init_Rtc( void )
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
  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init();
  /* Initialize the CPU2 reset value before starting CPU2 with C2BOOT */
  LL_C2_PWR_SetPowerMode(LL_PWR_MODE_SHUTDOWN);

#if (CFG_USB_INTERFACE_ENABLE != 0)
  /**
   *  Enable USB power
   */
  HAL_PWREx_EnableVddUSB();
#endif

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
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
  shci_init(APPE_SysUserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  memset(&tl_mm_config, 0, sizeof(TL_MM_Config_t));
  tl_mm_config.p_BleSpareEvtBuffer = 0;
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer;
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.AsynchEvtPoolSize = POOL_SIZE;
  TL_MM_Init( &tl_mm_config );

  TL_Enable();

  return;
}

static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status )
{
  UNUSED(status);
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
static void APPE_SysUserEvtRx( void * pPayload )
{
  TL_AsynchEvt_t *p_sys_event;
  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);

  switch(p_sys_event->subevtcode)
  {
     case SHCI_SUB_EVT_CODE_READY:
         APPE_SysEvtReadyProcessing();
         break;
     case SHCI_SUB_EVT_ERROR_NOTIF:
         APPE_SysEvtError((SCHI_SystemErrCode_t) (p_sys_event->payload[0]));
         break;
     default:
         break;
  }
  return;
}

/**
 * @brief Notify a system error coming from the M0 firmware
 * @param  ErrorCode  : errorCode detected by the M0 firmware
 *
 * @retval None
 */
static void APPE_SysEvtError( SCHI_SystemErrCode_t ErrorCode)
{
  switch(ErrorCode)
  {
  case ERR_ZIGBEE_UNKNOWN_CMD:
       APP_DBG("** ERR_ZIGBEE : UNKNOWN_CMD \n");
       break;
  default:
       APP_DBG("** ERR_ZIGBEE : ErroCode=%d \n",ErrorCode);
       break;
  }
  return;
}

static void APPE_SysEvtReadyProcessing( void )
{
  /* Traces channel initialization */
  TL_TRACES_Init( );

  APP_ZIGBEE_Init();
  UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
/**
 * @brief  RGB LED initialisation for the application
 * @param  None
 * @retval None
 */
static void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1U)
  /**
   * Leds Initialization
   */
  // LED Red;
  LED_Set_rgb(PWM_LED_GSDATA_47_0, PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_OFF);
  HAL_Delay(200);
  LED_Off();
  HAL_Delay(200);
  // LED Green;
  LED_Set_rgb(PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_47_0, PWM_LED_GSDATA_OFF);
  HAL_Delay(200);
  LED_Off();
  HAL_Delay(200);
  // LED Blue;
  LED_Set_rgb(PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_OFF, PWM_LED_GSDATA_47_0);
  HAL_Delay(200);
  LED_Off();
#endif

  return;
}

/**
 * @brief  Buttons initialisation used in application
 * @param  None
 * @retval None
 */
static void Button_Init( void )
{
#if (CFG_BUTTON_SUPPORTED == 1U)
  /**
   * Button Initialization
   */
    BSP_PB_Init(BUTTON_USER1, BUTTON_MODE_EXTI);
    BSP_PB_Init(BUTTON_USER2, BUTTON_MODE_EXTI);
#endif

  return;
}

/**
 * @brief  LCD initialisation, display the ST logo at the startup of the board
 * @param  None
 * @retval None
 */
void LCD_DisplayInit(void)
{
  BSP_LCD_Init(LCD_Inst, LCD_ORIENTATION_LANDSCAPE);
  /* Set LCD Foreground Layer  */
  UTIL_LCD_SetFuncDriver(&LCD_Driver); /* SetFunc before setting device */
  UTIL_LCD_SetDevice(LCD_Inst);            /* SetDevice after funcDriver is set */
  BSP_LCD_Clear(LCD_Inst, SSD1315_COLOR_BLACK);
  BSP_LCD_DisplayOn(LCD_Inst);
  BSP_LCD_Refresh(LCD_Inst);
  UTIL_LCD_SetFont(&Font12);
  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(SSD1315_COLOR_WHITE);
  UTIL_LCD_SetBackColor(SSD1315_COLOR_BLACK);
  BSP_LCD_Clear(LCD_Inst,SSD1315_COLOR_BLACK);
  BSP_LCD_Refresh(LCD_Inst);
  /* Display ST Logo */
  BSP_LCD_Clear(LCD_Inst,SSD1315_COLOR_BLACK);
  BSP_LCD_Refresh(LCD_Inst);
  BSP_LCD_DrawBitmap(LCD_Inst, 0, 0, (uint8_t *)stlogo);
  BSP_LCD_Refresh(LCD_Inst);
  HAL_Delay(2000);
}
/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void MX_APPE_Process(void)
{
  UTIL_SEQ_Run(UTIL_SEQ_DEFAULT);
}

void UTIL_SEQ_Idle( void )
{
#if ( CFG_LPM_SUPPORTED == 1)
  UTIL_LPM_EnterLowPower( );
#endif
  return;
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
	/* Check the notification condition */
	if (g_ot_notification_allowed) {
		UTIL_SEQ_Run(1U << CFG_TASK_NOTIFY_FROM_M0_TO_M4);
	}	
  switch(evt_waited_bm)
  {
  case EVENT_ACK_FROM_M0_EVT:
    /**
     * Run only the task CFG_TASK_REQUEST_FROM_M0_TO_M4 to process
     * direct requests from the M0 (e.g. ZbMalloc), but no stack notifications
     * until we're done the request to the M0.
     */
    UTIL_SEQ_Run((1U << CFG_TASK_REQUEST_FROM_M0_TO_M4));
    break;
  case EVENT_SYNCHRO_BYPASS_IDLE:
    UTIL_SEQ_SetEvt(EVENT_SYNCHRO_BYPASS_IDLE);
    /* Process notifications and requests from the M0 */
    UTIL_SEQ_Run((1U << CFG_TASK_NOTIFY_FROM_M0_TO_M4) | (1U << CFG_TASK_REQUEST_FROM_M0_TO_M4));
    break;
  default :
    /* default case */
  UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
    break;
  }
}

void shci_notify_asynch_evt(void* pdata)
{
  UNUSED(pdata);
  UTIL_SEQ_SetTask(1U << CFG_TASK_SYSTEM_HCI_ASYNCH_EVT, CFG_SCH_PRIO_0);
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UNUSED(flag);
  UTIL_SEQ_SetEvt(1U << CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP);
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UNUSED(timeout);
  UTIL_SEQ_WaitEvt(1U << CFG_EVT_SYSTEM_HCI_CMD_EVT_RESP);
  return;
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
  * @brief  Initialisation of the trace mechanism
  * @param  None
  * @retval None
  */
#if(CFG_DEBUG_TRACE != 0)
void DbgOutputInit( void )
{
#ifdef CFG_DEBUG_TRACE_UART
  MX_USART1_UART_Init();
  return;
#endif
}

/**
  * @brief  Management of the traces
  * @param  p_data : data
  * @param  size : size
  * @param  call-back :
  * @retval None
  */
void DbgOutputTraces(  uint8_t *p_data, uint16_t size, void (*cb)(void) )
{
  HW_UART_Transmit_DMA(CFG_DEBUG_TRACE_UART, p_data, size, cb);

  return;
}
#endif

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
/**
  * @brief This function manage the Push button action
  * @param  Button which has been activated
  * @retval None
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  //HAL_Delay(200);
  switch (Button)
  {
  case BUTTON_USER1:
    UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON_SW1, CFG_SCH_PRIO_1);
    break;

  case BUTTON_USER2:
    UTIL_SEQ_SetTask(1U << CFG_TASK_BUTTON_SW2, CFG_SCH_PRIO_1);
    break;

  default:
    break;
  }
}

static void RxUART_Init(void)
{
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    if (aRxBuffer[0] == '\r')
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
  HW_UART_Receive_IT(CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void UartCmdExecute(void)
{
  /* Parse received CommandString */
  if(strcmp((char const*)CommandString, "SW1") == 0)
  {
    APP_DBG("SW1 OK");
//    exti_handle.Line = EXTI_LINE_4;
//    HAL_EXTI_GenerateSWI(&exti_handle);
    BSP_PB_Callback(BUTTON_USER1);
  }
  else if (strcmp((char const*)CommandString, "SW2") == 0)
  {
    APP_DBG("SW2 OK");
//    exti_handle.Line = EXTI_LINE_0;
//    HAL_EXTI_GenerateSWI(&exti_handle);
    BSP_PB_Callback(BUTTON_USER2);
  }
  else
  {
    APP_DBG("NOT RECOGNIZED COMMAND : %s", CommandString);
  }
}


/* USER CODE END FD_WRAP_FUNCTIONS */
