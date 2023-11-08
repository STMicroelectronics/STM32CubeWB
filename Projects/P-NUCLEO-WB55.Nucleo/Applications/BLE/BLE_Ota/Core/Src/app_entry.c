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
#include "app_common.h"

#include "main.h"
#include "app_entry.h"
#include "app_ble.h"

#include "ble.h"
#include "tl.h"

#include "stm32_seq.h"
#include "shci.h"
#include "shci_tl.h"
#include "stm32_lpm.h"


#include "app_debug.h"


/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define POOL_SIZE (CFG_TLBLE_EVT_QUEUE_LENGTH*4*DIVC(( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4))

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t	SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t	BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemPower_Config( void );
static void appe_Tl_Init( void );
static void Led_Init( void );
static void Button_Init( void );
static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status );
static void APPE_SysUserEvtRx( void * pPayload );
static SHCI_TL_UserEventFlowStatus_t APPE_SysevtReadyProcessing( SHCI_C2_Ready_Evt_t *pReadyEvt );

/* Functions Definition ------------------------------------------------------*/
void APPE_Init( void )
{
  SystemPower_Config(); /**< Configure the system Power Mode */

  HW_TS_Init(hw_ts_InitMode_Full, &hrtc); /**< Initialize the TimerServer */

  APPD_Init( );

  /**
   * The Standby mode should not be entered before the initialization is over
   * The default state of the Low Power Manager is to allow the Standby Mode so an request is needed here
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_DISABLE);

  Led_Init();

  Button_Init();
  
  /* clock enable periph CRC */
  LL_C2_AHB1_GRP1_EnableClock(LL_C2_AHB1_GRP1_PERIPH_CRC);

  appe_Tl_Init();	/**< Initialize all transport layers */

  /**
   * From now, the application is waiting for the ready event ( VS_HCI_C2_Ready )
   * received on the system channel before starting the Stack
   * This system event is received with APPE_SysUserEvtRx()
   */

  return;
}

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

static void Led_Init( void )
{
#if (CFG_LED_SUPPORTED == 1)
  /**
   * Leds Initialization
   */

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  BSP_LED_On(LED_GREEN);
#endif

  return;
}

static void Button_Init( void )
{
#if (CFG_BUTTON_SUPPORTED == 1)
  /**
   * Button Initialization
   */

  BSP_PB_Init(BUTTON_SW1, BUTTON_MODE_EXTI);

#endif

  return;
}



static void APPE_SysStatusNot( SHCI_TL_CmdStatus_t status )
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
static void APPE_SysUserEvtRx( void * pPayload )
{
  TL_AsynchEvt_t *p_sys_event;

  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pPayload)->pckt->evtserial.evt.payload);

  switch(p_sys_event->subevtcode)
  {
    case SHCI_SUB_EVT_CODE_READY:
      ((tSHCI_UserEvtRxParam*)pPayload)->status = APPE_SysevtReadyProcessing( (SHCI_C2_Ready_Evt_t*)p_sys_event->payload );
      break;

    default:
      break;
  }

  return;
}

static SHCI_TL_UserEventFlowStatus_t APPE_SysevtReadyProcessing( SHCI_C2_Ready_Evt_t *pReadyEvt )
{
  uint8_t fus_state_value;
  SHCI_TL_UserEventFlowStatus_t return_value;

#if ( CFG_LED_SUPPORTED != 0)
  BSP_LED_Off(LED_BLUE);
#endif

  if(pReadyEvt->sysevt_ready_rsp == WIRELESS_FW_RUNNING)
  {
    return_value = SHCI_TL_UserEventFlow_Enable;
    
    /* clock disable periph CRC */
    LL_C2_AHB1_GRP1_DisableClock(LL_C2_AHB1_GRP1_PERIPH_CRC);

    if(CFG_OTA_REBOOT_VAL_MSG == CFG_REBOOT_ON_CPU2_UPGRADE)
    {
      /**
       * The wireless stack update has been completed
       * Reboot on the firmware application
       */
        CFG_OTA_REBOOT_VAL_MSG = CFG_REBOOT_ON_FW_APP;
        NVIC_SystemReset(); /* it waits until reset */
    }
    else
    {
      /**
       * Run the Application
       */

      /* Enable CPU2 debug feature*/
      APPD_EnableCPU2( );
      
      /* Select which mechanism is used by CPU2 to protect its timing versus flash operation */
      SHCI_C2_SetFlashActivityControl(FLASH_ACTIVITY_CONTROL_SEM7);

      UTIL_LPM_SetOffMode(1 << CFG_LPM_APP, UTIL_LPM_ENABLE);

      APP_BLE_Init( );
    }
  }
  else
  {
    /**
     * FUS is running on CPU2
     */
    return_value = SHCI_TL_UserEventFlow_Disable;

    /**
     * The CPU2 firmware update procedure is starting from now
     * There may be several device reset during CPU2 firmware upgrade
     * The key word at the beginning of SRAM1 shall be changed CFG_REBOOT_ON_CPU2_UPGRADE
     *
     * Wireless Firmware upgrade:
     * Once the upgrade is over, the CPU2 will run the wireless stack
     * When the wireless stack is running, the SRAM1 is checked and when equal to CFG_REBOOT_ON_CPU2_UPGRADE,
     * it means we may restart on the firmware application.
     *
     * FUS Firmware Upgrade:
     * Once the upgrade is over, the CPU2 will run FUS and the FUS return the Idle state
     * The SRAM1 is checked and when equal to CFG_REBOOT_ON_CPU2_UPGRADE,
     * it means we may restart on the firmware application.
     */
    fus_state_value = SHCI_C2_FUS_GetState( NULL );

    if( fus_state_value == 0xFF)
    {
      /**
       * This is the first time in the life of the product the FUS is involved. After this command, it will be properly initialized
       * Request the device to reboot to install the wireless firmware
       */
      NVIC_SystemReset(); /* it waits until reset */
    }
    else if( fus_state_value != 0)
    {
      /**
       * An upgrade is on going
       * Wait to reboot on the wireless stack
       */
#if ( CFG_LED_SUPPORTED != 0)
      BSP_LED_On(LED_BLUE);
#endif
      while(1)
      {
        /**
         * Wait for the FUS to reboot the system when the upgrade is done
         * In case an error is detected during the upgrade process, restart the device
         * The BLE_Ota state machine will request a SHCI_C2_FUS_StartWs() on the next reboot.
         */
        HAL_Delay(10000);   /* Poll the FUS each 10s to make sure process is going fine */
        fus_state_value = SHCI_C2_FUS_GetState( NULL );
        if( (fus_state_value < FUS_STATE_VALUE_FW_UPGRD_ONGOING) || (fus_state_value > FUS_STATE_VALUE_FUS_UPGRD_ONGOING_END) )
        {
          NVIC_SystemReset();
        }
      }
    }
    else
    {
      /**
       * FUS is idle
       * Request an upgrade and wait to reboot on the wireless stack
       * The first two parameters are currently not supported by the FUS
       */
      if(CFG_OTA_REBOOT_VAL_MSG == CFG_REBOOT_ON_CPU2_UPGRADE)
      {
        /**
         * The FUS update has been completed
         * Reboot the CPU2 on the firmware application
         */
        CFG_OTA_REBOOT_VAL_MSG = CFG_REBOOT_ON_FW_APP;
        SHCI_C2_FUS_StartWs( );
  #if ( CFG_LED_SUPPORTED != 0)
        BSP_LED_On(LED_BLUE);
  #endif
        while(1)
        {
          HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
        }
      }
      else
      {
        CFG_OTA_REBOOT_VAL_MSG = CFG_REBOOT_ON_CPU2_UPGRADE;
        /**
         * Note:
         * If a reset occurs now, on the next reboot the FUS will be idle and a CPU2 reboot on the
         * wireless stack will be requested because SRAM1 is set to CFG_REBOOT_ON_CPU2_UPGRADE
         * The device is still operational but no CPU2 update has been done.
         */
        SHCI_C2_FUS_FwUpgrade(0,0);
  #if ( CFG_LED_SUPPORTED != 0)
        BSP_LED_On(LED_BLUE);
  #endif
        while(1)
        {
          /**
           * Wait for the FUS to reboot the system when the upgrade is done
           * In case an error is detected during the upgrade process, restart the device
           * The BLE_Ota state machine will request a SHCI_C2_FUS_StartWs() on the next reboot.
           */
          HAL_Delay(10000);   /* Poll the FUS each 10s to make sure process is going fine */
          fus_state_value = SHCI_C2_FUS_GetState( NULL );
          if( (fus_state_value < FUS_STATE_VALUE_FW_UPGRD_ONGOING) || (fus_state_value > FUS_STATE_VALUE_FUS_UPGRD_ONGOING_END) )
          {
            NVIC_SystemReset();
          }
        }
      }
    }
  }

  return return_value;
}


/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void UTIL_SEQ_Idle( void )
{
#if ( CFG_LPM_SUPPORTED == 1)
  UTIL_LPM_EnterLowPower( );
#endif
  return;
}

#ifdef OTA_SBSFU
void UTIL_SEQ_PostIdle( void )
{
  /* Loader must reload IWDG counter with value defined in the reload register by SBSFU */
  WRITE_REG(IWDG->KR, IWDG_KEY_RELOAD);

  return;
}
#endif /* OTA_SBSFU */

void UTIL_SEQ_EvtIdle( UTIL_SEQ_bm_t task_id_bm, UTIL_SEQ_bm_t evt_waited_bm )
{
  UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );

  return;
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

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  switch (GPIO_Pin)
  {
    case BUTTON_SW1_PIN:
      APP_BLE_Key_Button1_Action();
      break; 

    case BUTTON_SW2_PIN:
      APP_BLE_Key_Button2_Action();
      break; 

    case BUTTON_SW3_PIN:
      APP_BLE_Key_Button3_Action();
      break;

    default:
      break;
  }
  return;
}
