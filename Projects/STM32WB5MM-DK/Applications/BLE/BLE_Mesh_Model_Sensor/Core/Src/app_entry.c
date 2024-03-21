/* USER CODE BEGIN Header */
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "main.h"
#include "app_entry.h"
#include "app_ble.h"
#include "ble.h"
#include "tl.h"
#include "stm32_seq.h"
#include "shci_tl.h"
#include "stm32_lpm.h"
#include "app_debug.h"

#include "appli_mesh.h"
#include "appli_nvm.h"
#include "pal_nvm.h"
#include "lp_timer.h"
#include "mesh_cfg.h"

#include "app_vl53l0x.h"
#include "app_stts22h.h"

#include "shci.h"
#include "dbg_trace.h"

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION    
extern MOBLEUINT8 PowerOnOff_flag;
#endif
#ifdef ENABLE_OCCUPANCY_SENSOR           
extern MOBLEUINT8 Occupancy_Flag;
#endif
extern const void *mobleNvmBase; 
extern const void *appNvmBase;
extern const void *prvsnr_data;
#if (LOW_POWER_FEATURE == 1)
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;
#if ( CFG_LPM_SUPPORTED == 1)
static uint32_t BleMesh_sleepTime;
#endif
extern volatile uint8_t BleProcessInit;
#endif

/* USER CODE BEGIN PTD */
/* Section specific to button management using UART */
EXTI_HandleTypeDef exti_handle;
extern uint8_t LongPressButton;
extern uint8_t button_emulation;

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH*4U*DIVC(( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4U))

/* USER CODE BEGIN PD */
/* Section specific to button management using UART */
#define C_SIZE_CMD_STRING       256U
#define RX_BUFFER_SIZE          8U
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t	BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];

/* USER CODE BEGIN PV */
/* Section specific to button management using UART */
static uint8_t aRxBuffer[RX_BUFFER_SIZE];
static uint8_t CommandString[C_SIZE_CMD_STRING];
static uint16_t indexReceiveChar = 0;
/* USER CODE END PV */

/* Private functions prototypes-----------------------------------------------*/
static void SystemPower_Config( void );
static void appe_Tl_Init( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );

static void APPE_SysUserEvtRx( void * pPayload );
static void APPE_SysEvtReadyProcessing( void * pPayload );
static void APPE_SysEvtError( void * pPayload);
#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif

/* USER CODE BEGIN PFP */
static void Button_Init( void );

/* Section specific to button management using UART */
static void RxUART_Init(void);
static void RxCpltCallback(void);
static void UartCmdExecute(void);

extern void Appli_LongButtonPress(void);
extern void Appli_ShortButtonPress(void);

/* USER CODE END PFP */

uint8_t Mesh_Stop_Mode;

/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  MOBLEUINT32 last_user_flash_address = ((READ_BIT(FLASH->SFR, FLASH_SFR_SFSA) >> FLASH_SFR_SFSA_Pos) << 12) + FLASH_BASE;
  
  SystemPower_Config(); /**< Configure the system Power Mode */
  
  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

/* USER CODE BEGIN APPE_Init_1 */
  APPD_Init( );

  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

  //Switch RGB LED off
  LED_Off();
  Button_Init();

  RxUART_Init();

  BSP_I2C3_Init();

  mobleNvmBase = (const void *)(last_user_flash_address - NVM_SIZE);
  appNvmBase   = (const void *)(last_user_flash_address - NVM_SIZE - APP_NVM_SIZE);
  prvsnr_data  = (const void *)(last_user_flash_address - NVM_SIZE - APP_NVM_SIZE - PRVN_NVM_PAGE_SIZE);    
  
#if (LOW_POWER_FEATURE == 1)
  /**
   * Initialize the lp timer to be used when the systick is stopped in low power mode
   */
  LpTimerInit();
#endif
  
/* USER CODE END APPE_Init_1 */
  appe_Tl_Init();	/*  Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */
/* USER CODE BEGIN APPE_Init_2 */

/* USER CODE END APPE_Init_2 */
  return;
}
/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
/**
 * @brief  Configure the system for power optimization
 *
 * @note  This API configures the system to be ready for low power mode
 *
 * @param  None
 * @retval None
 */
static void SystemPower_Config( void )
{

  /**
   * Select HSI as system clock source after Wake Up from Stop mode
   */
  LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);

  /* Initialize low power manager */
  UTIL_LPM_Init( );
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
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SYSTEM_HCI_ASYNCH_EVT_ID, UTIL_SEQ_RFU, shci_user_evt_proc );
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = APPE_SysStatusNot;
  shci_init(APPE_SysUserEvtRx, (void*) &SHci_Tl_Init_Conf);

  /**< Memory Manager channel initialization */
  tl_mm_config.p_BleSpareEvtBuffer = BleSpareEvtBuffer;
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
  WirelessFwInfo_t WirelessInfo;
  
  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);
  
  /* Read the firmware version of both the wireless firmware and the FUS */
  SHCI_GetWirelessFwInfo( &WirelessInfo );
  APP_DBG_MSG("Wireless Firmware version %d.%d.%d\n", WirelessInfo.VersionMajor, WirelessInfo.VersionMinor, WirelessInfo.VersionSub);
  APP_DBG_MSG("Wireless Firmware build %d\n", WirelessInfo.VersionReleaseType);
  APP_DBG_MSG("FUS version %d.%d.%d\n\n", WirelessInfo.FusVersionMajor, WirelessInfo.FusVersionMinor, WirelessInfo.FusVersionSub);
  
  switch(p_sys_event->subevtcode)
  {
  case SHCI_SUB_EVT_CODE_READY:
    APPE_SysEvtReadyProcessing(pPayload);
    break;
    
  case SHCI_SUB_EVT_ERROR_NOTIF:
    APPE_SysEvtError(pPayload);
    break;
    
  case SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE:
    APP_DBG_MSG("-- BLE NVM RAM HAS BEEN UPDATED BY CMO+ \n");
    APP_DBG_MSG("SHCI_SUB_EVT_BLE_NVM_RAM_UPDATE : StartAddress = %lx , Size = %ld\n",
        ((SHCI_C2_BleNvmRamUpdate_Evt_t*)p_sys_event->payload)->StartAddress,
        ((SHCI_C2_BleNvmRamUpdate_Evt_t*)p_sys_event->payload)->Size);
    break;
    
  case SHCI_SUB_EVT_NVM_START_WRITE:
    APP_DBG_MSG("SHCI_SUB_EVT_NVM_START_WRITE : NumberOfWords = %ld\n",
                ((SHCI_C2_NvmStartWrite_Evt_t*)p_sys_event->payload)->NumberOfWords);
    break;
    
  case SHCI_SUB_EVT_NVM_END_WRITE:
    APP_DBG_MSG("SHCI_SUB_EVT_NVM_END_WRITE\n");
    break;
    
  case SHCI_SUB_EVT_NVM_START_ERASE:
    APP_DBG_MSG("SHCI_SUB_EVT_NVM_START_ERASE : NumberOfSectors = %ld\n",
                ((SHCI_C2_NvmStartErase_Evt_t*)p_sys_event->payload)->NumberOfSectors);
    break;
    
  case SHCI_SUB_EVT_NVM_END_ERASE:
    APP_DBG_MSG("SHCI_SUB_EVT_NVM_END_ERASE\n");
    break;
    
  default:
    break;
  }
  
  return;
}

/**
* @brief Notify when ready system event from the M0 firmware
*/
static void APPE_SysEvtReadyProcessing( void * pPayload )
{
  TL_AsynchEvt_t *p_sys_event;
  SHCI_C2_Ready_Evt_t *p_sys_ready_event;
  
  SHCI_C2_CONFIG_Cmd_Param_t config_param = {0};
  uint32_t RevisionID=0;
  uint32_t DeviceID=0;
  
  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);
  p_sys_ready_event = (SHCI_C2_Ready_Evt_t*) p_sys_event->payload;
  
  if(p_sys_ready_event->sysevt_ready_rsp == WIRELESS_FW_RUNNING)
  {
    /**
    * The wireless firmware is running on the CPU2
    */
    APP_DBG_MSG("SHCI_SUB_EVT_CODE_READY - WIRELESS_FW_RUNNING \n");
    
    /* Traces channel initialization */
    APPD_EnableCPU2( );
    
    /* Enable all events Notification */
    config_param.PayloadCmdSize = SHCI_C2_CONFIG_PAYLOAD_CMD_SIZE;
    config_param.EvtMask1 = SHCI_C2_CONFIG_EVTMASK1_BIT0_ERROR_NOTIF_ENABLE  
      +  SHCI_C2_CONFIG_EVTMASK1_BIT1_BLE_NVM_RAM_UPDATE_ENABLE
        +  SHCI_C2_CONFIG_EVTMASK1_BIT2_THREAD_NVM_RAM_UPDATE_ENABLE
          +  SHCI_C2_CONFIG_EVTMASK1_BIT3_NVM_START_WRITE_ENABLE   
            +  SHCI_C2_CONFIG_EVTMASK1_BIT4_NVM_END_WRITE_ENABLE
              +  SHCI_C2_CONFIG_EVTMASK1_BIT5_NVM_START_ERASE_ENABLE
                +  SHCI_C2_CONFIG_EVTMASK1_BIT6_NVM_END_ERASE_ENABLE;

    
    /* Read revision identifier */
    /**
    * @brief  Return the device revision identifier
    * @note   This field indicates the revision of the device.
    * @rmtoll DBGMCU_IDCODE REV_ID        LL_DBGMCU_GetRevisionID
    * @retval Values between Min_Data=0x00 and Max_Data=0xFFFF
    */
    RevisionID = LL_DBGMCU_GetRevisionID();
    
    APP_DBG_MSG("DBGMCU_GetRevisionID= %lx \n\n", RevisionID);
    
    config_param.RevisionID = (uint16_t)RevisionID;
    
    DeviceID = LL_DBGMCU_GetDeviceID();
    APP_DBG_MSG(">>== DBGMCU_GetDeviceID= %lx \n\r", DeviceID);
    config_param.DeviceID = (uint16_t)DeviceID;
    (void)SHCI_C2_Config(&config_param);
    
    APP_BLE_Init( );
    UTIL_LPM_SetOffMode(1U << CFG_LPM_APP, UTIL_LPM_ENABLE);
    
  }
  else  if (p_sys_ready_event->sysevt_ready_rsp == FUS_FW_RUNNING) 
  {
    /**
    * The FUS firmware is running on the CPU2
    * In the scope of this application, there should be no case when we get here
    */
    APP_DBG_MSG("SHCI_SUB_EVT_CODE_READY - FUS_FW_RUNNING \n");
    
    /* The packet shall not be released as this is not supported by the FUS */
    ((tSHCI_UserEvtRxParam*)pPayload)->status = SHCI_TL_UserEventFlow_Disable;
    
  } else {
    
    APP_DBG_MSG("SHCI_SUB_EVT_CODE_READY - UNEXPECTED CASE \n");
  }
  return;
}

/**
* @brief Notify a system error coming from the M0 firmware
*/
static void APPE_SysEvtError( void * pPayload)
{
  TL_AsynchEvt_t *p_sys_event;
  SCHI_SystemErrCode_t *p_sys_error_code;
    
  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);
  p_sys_error_code = (SCHI_SystemErrCode_t*) p_sys_event->payload;
       
  APP_DBG_MSG("SHCI_SUB_EVT_ERROR_NOTIF WITH REASON %x \n",(*p_sys_error_code));
  
  if ((*p_sys_error_code) == ERR_BLE_INIT) {
    /* Error during BLE stack initialization */
    APP_DBG_MSG("SHCI_SUB_EVT_ERROR_NOTIF WITH REASON - ERR_BLE_INIT \n");
  } else {
    APP_DBG_MSG("SHCI_SUB_EVT_ERROR_NOTIF WITH REASON - BLE ERROR \n");    
  }
  return;
}
static void Button_Init( void )
{
#if (CFG_BUTTON_SUPPORTED == 1)
  /**
   * Button Initialization
   */
#if 0
  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);
  BSP_PB_Init(BUTTON_SW2, BUTTON_MODE_EXTI);
#else
  BSP_PB_Init(BUTTON_USER1, BUTTON_MODE_EXTI);    
  BSP_PB_Init(BUTTON_USER2, BUTTON_MODE_EXTI);
#endif
#endif

  return;
}
/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void UTIL_SEQ_Idle( void )
{
#if ( CFG_LPM_SUPPORTED == 1)
#if (LOW_POWER_FEATURE == 1)
  if(BleProcessInit != 0)
  {
    BleMesh_sleepTime = (uint32_t)BLEMesh_GetSleepDuration();

    if (BleMesh_sleepTime > 0)
    {
      LpTimerStart(BleMesh_sleepTime);

      UTIL_LPM_EnterLowPower( );

      uwTick += (uwTickFreq*LpGetElapsedTime());
    }
    UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_REQ_ID, CFG_SCH_PRIO_0);
  }
#else
  UTIL_LPM_EnterLowPower( );
#endif
#else
#if (LOW_POWER_FEATURE == 1)
  if(BleProcessInit != 0)
  {
    UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_REQ_ID, CFG_SCH_PRIO_0);
  }
#endif
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
#if (LOW_POWER_FEATURE == 1)
  UTIL_SEQ_Run( 0 );
#else
  UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );
#endif
}

void shci_notify_asynch_evt(void* pdata)
{
  UTIL_SEQ_SetTask( 1<<CFG_TASK_SYSTEM_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
  return;
}

void shci_cmd_resp_release(uint32_t flag)
{
  UTIL_SEQ_SetEvt( 1<< CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID );
  return;
}

void shci_cmd_resp_wait(uint32_t timeout)
{
  UTIL_SEQ_WaitEvt( 1<< CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID );
  return;
}

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  switch (GPIO_Pin)
  {
#ifdef SAVE_MODEL_STATE_POWER_FAILURE_DETECTION    
    case POWEROFF_PIN:
      {
        PowerOnOff_flag = 1;
      }
      break;
#endif

  case GPIO_PIN_12: //BUTTON_SW1_PIN
      {
       if (button_emulation != 1){
          UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_SW1_REQ_ID, CFG_SCH_PRIO_0);
      }
      else{
        if (LongPressButton==1){
#ifdef ENABLE_SENSOR_MODEL_CLIENT
          /* Button 1 long press action */
          APP_DBG_MSG("The Client ask the Time Of Flight measurement to the Server \n");
          Appli_LongButtonPress();
#endif
          LongPressButton = 0;
        }
        else{
#ifdef ENABLE_SENSOR_MODEL_CLIENT 
          /* Button 1 short press action */
          APP_DBG_MSG("The Client ask the Temperature to the Server \n");
          Appli_ShortButtonPress();
#endif
        }
        button_emulation = 0;
      }        
      }
      break;

#ifdef ENABLE_OCCUPANCY_SENSOR       
  case GPIO_PIN_1:      //BUTTON_SW3_PIN
      {
        Occupancy_Flag = 1;
        UTIL_SEQ_SetTask( 1<<CFG_TASK_MESH_SW3_REQ_ID, CFG_SCH_PRIO_0);
      }
      break;
#endif

      
  default:
      break;

  }
  return;
}



static void RxUART_Init(void)
{
  HW_UART_Receive_IT((hw_uart_id_t)CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    if (aRxBuffer[0] == '\r')
    {
      APP_DBG_MSG("received %s\n", CommandString);

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
  HW_UART_Receive_IT((hw_uart_id_t)CFG_DEBUG_TRACE_UART, aRxBuffer, 1U, RxCpltCallback);
}

static void UartCmdExecute(void)
{
  /* Parse received CommandString */
  if(strcmp((char const*)CommandString, "SW1") == 0)
  {
    APP_DBG_MSG("SW1 OK\n");
    exti_handle.Line = BUTTON_USER1_EXTI_LINE;
    HAL_EXTI_GenerateSWI(&exti_handle);
    button_emulation=1;
  }
  else if (strcmp((char const*)CommandString, "SW2") == 0)
  {
    APP_DBG_MSG("SW2 OK\n");
    exti_handle.Line = BUTTON_USER2_EXTI_LINE;
    HAL_EXTI_GenerateSWI(&exti_handle);
    button_emulation=1;
  }
  else if (strcmp((char const*)CommandString, "LONG_PRESS") == 0)
  {
    APP_DBG_MSG("LONG_PRESS OK\n");
    LongPressButton=1;
  } 
  else
  {
    APP_DBG_MSG("NOT RECOGNIZED COMMAND : %s\n", CommandString);
  }
}

/* USER CODE END FD_WRAP_FUNCTIONS */
