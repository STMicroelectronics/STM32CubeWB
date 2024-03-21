/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : App/app_thread.c
 * Description        : Thread Application.
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
#include "app_thread.h"
#include "stm32wbxx_core_interface_def.h"
#include "openthread_api_wb.h"
#include "shci.h"
#include "stm_logging.h"
#include "app_conf.h"
#include "stm32_lpm.h"
#include "stm32_seq.h"
#include "ee.h"
#include "hw_flash.h"
#if (CFG_USB_INTERFACE_ENABLE != 0)
#include "vcp.h"
#include "vcp_conf.h"
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
#ifdef ENABLE_OPENTHREAD_CLI
#include "uart.h"
#endif /* ENABLE_OPENTHREAD_CLI */

/* Private includes -----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define C_SIZE_CMD_STRING     256U

#define PANID_RESET_VALUE     0xFFFFU
#define C_PANID               0x1111U

#define CHANNEL_NB_RESET_VALUE          11U
#define CHANNEL_NB_12                   12U
#define CHANNEL_NB_13                   13U
#define CHANNEL_NB_14                   14U

/* USER CODE BEGIN PD */
#define C_RESSOURCE             "light"
#define COAP_PAYLOAD_LENGTH 2
#define WAIT_TIMEOUT                     (5*1000*1000/CFG_TS_TICK_VAL) /**< 5s */

#define THREAD_FLASH_PROCESS_TRIGGER            (1*1000*1000/CFG_TS_TICK_VAL) /**< 1s */
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
static void APP_THREAD_NVM_Init(void);
static bool APP_THREAD_NVM_Read(void);
static bool APP_THREAD_NVM_Write(void);

static void APP_THREAD_CheckWirelessFirmwareInfo(void);
static void APP_THREAD_DeviceConfig(void);
static void APP_THREAD_StateNotif(uint32_t NotifFlags, void *pContext);
static void APP_THREAD_TraceError(const char * pMess, uint32_t ErrCode);
#if (CFG_FULL_LOW_POWER == 0)
static void Send_CLI_To_M0(void);
#endif /* (CFG_FULL_LOW_POWER == 0) */
static void Send_CLI_Ack_For_OT(void);
static void HostTxCb( void );
static void Wait_Getting_Ack_From_M0(void);
static void Receive_Ack_From_M0(void);
static void Receive_Notification_From_M0(void);
#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif
#if (CFG_USB_INTERFACE_ENABLE != 0)
static uint32_t ProcessCmdString(uint8_t* buf , uint32_t len);
#else
#if (CFG_FULL_LOW_POWER == 0)
static void RxCpltCallback(void);
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
#ifdef ENABLE_OPENTHREAD_CLI
extern void otAppCliInit(otInstance *aInstance);
#endif /* ENABLE_OPENTHREAD_CLI */

/* USER CODE BEGIN PFP */
static void APP_THREAD_TestDeviceConfig(uint8_t ChannelRef, uint16_t PanidRef);
static void APP_THREAD_TestDeviceConfigStart(void);
static otError APP_THREAD_CheckConfigValues(uint8_t ChannelRef, uint16_t PanidRef);

static void APP_THREAD_NvmSave(void);
static void APP_THREAD_NvmProcess(void);
static void APP_THREAD_FlashTestProcess(void);
/* USER CODE END PFP */

/* Private variables -----------------------------------------------*/
#if (CFG_USB_INTERFACE_ENABLE != 0)
static uint8_t TmpString[C_SIZE_CMD_STRING];
static uint8_t VcpRxBuffer[sizeof(TL_CmdSerial_t)];        /* Received Data over USB are stored in this buffer */
static uint8_t VcpTxBuffer[sizeof(TL_EvtPacket_t) + 254U]; /* Transmit buffer over USB */
#else
#if (CFG_FULL_LOW_POWER == 0)
static uint8_t aRxBuffer[C_SIZE_CMD_STRING];
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_FULL_LOW_POWER == 0)
static uint8_t CommandString[C_SIZE_CMD_STRING];
#endif /* (CFG_FULL_LOW_POWER == 0) */
static __IO uint16_t indexReceiveChar = 0;
static __IO uint16_t CptReceiveCmdFromUser = 0;

static TL_CmdPacket_t *p_thread_otcmdbuffer;
static TL_EvtPacket_t *p_thread_notif_M0_to_M4;
static __IO uint32_t  CptReceiveMsgFromM0 = 0;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_TH_Config_t ThreadConfigBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadOtCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t ThreadNotifRspEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255U];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadCliCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t ThreadCliNotBuffer;
extern uint8_t g_ot_notification_allowed;

/* OT NVM RAM buffer to be used by OT stack on Coprocessor side */
uint32_t OT_NvmRamBuffer[CFG_THREAD_NVM_RAM_SIZE_IN_BYTES/4];

/* USER CODE BEGIN PV */
static uint8_t threadTimerID;
static uint8_t g_bootnbr = 0;
/* USER CODE END PV */

/* Functions Definition ------------------------------------------------------*/

void APP_THREAD_Init( void )
{
  /* USER CODE BEGIN APP_THREAD_INIT_1 */

  /* USER CODE END APP_THREAD_INIT_1 */

  SHCI_CmdStatus_t ThreadInitStatus;

  /* Check the compatibility with the Coprocessor Wireless Firmware loaded */
  APP_THREAD_CheckWirelessFirmwareInfo();

#if (CFG_USB_INTERFACE_ENABLE != 0)
  VCP_Init(&VcpTxBuffer[0], &VcpRxBuffer[0]);
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

  /* Register cmdbuffer */
  APP_THREAD_RegisterCmdBuffer(&ThreadOtCmdBuffer);

  /**
   * Do not allow standby in the application
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_THREAD, UTIL_LPM_DISABLE);

  /* Init config buffer and call TL_THREAD_Init */
  APP_THREAD_TL_THREAD_INIT();

  /* Configure UART for sending CLI command from M4 */
  APP_THREAD_Init_UART_CLI();

  /* NVM Init */
  APP_THREAD_NVM_Init();

  /* Register task */
  /* Create the different tasks */
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_MSG_FROM_M0_TO_M4, UTIL_SEQ_RFU, APP_THREAD_ProcessMsgM0ToM4);

  /* Send Thread start system cmd to M0 */
  ThreadInitStatus = SHCI_C2_THREAD_Init();

  /* Prevent unused argument(s) compilation warning */
  UNUSED(ThreadInitStatus);

  /* USER CODE BEGIN INIT TASKS */
  UTIL_SEQ_RegTask( 1<<(uint32_t)CFG_TASK_NVM, UTIL_SEQ_RFU, APP_THREAD_NvmProcess);

  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &threadTimerID, hw_ts_SingleShot, APP_THREAD_FlashTestProcess);

  /* USER CODE END INIT TASKS */

  /* Initialize and configure the Thread device*/
  APP_THREAD_DeviceConfig();

  /* USER CODE BEGIN APP_THREAD_INIT_2 */
  /* USER CODE END APP_THREAD_INIT_2 */
}

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */
void APP_THREAD_Error(uint32_t ErrId, uint32_t ErrCode)
{
  /* USER CODE BEGIN APP_THREAD_Error_1 */

  /* USER CODE END APP_THREAD_Error_1 */
  switch(ErrId)
  {
  case ERR_REC_MULTI_MSG_FROM_M0 :
    APP_THREAD_TraceError("ERROR : ERR_REC_MULTI_MSG_FROM_M0 ", ErrCode);
    break;
  case ERR_THREAD_SET_STATE_CB :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_STATE_CB ",ErrCode);
    break;
  case ERR_THREAD_SET_CHANNEL :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_CHANNEL ",ErrCode);
    break;
  case ERR_THREAD_SET_PANID :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_PANID ",ErrCode);
    break;
  case ERR_THREAD_IPV6_ENABLE :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_IPV6_ENABLE ",ErrCode);
    break;
  case ERR_THREAD_START :
    APP_THREAD_TraceError("ERROR: ERR_THREAD_START ", ErrCode);
    break;
  case ERR_THREAD_ERASE_PERSISTENT_INFO :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_ERASE_PERSISTENT_INFO ",ErrCode);
    break;
  case ERR_THREAD_SET_NETWORK_KEY :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_SET_NETWORK_KEY ",ErrCode);
    break;
  case ERR_THREAD_CHECK_WIRELESS :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_CHECK_WIRELESS ",ErrCode);
    break;
  /* USER CODE BEGIN APP_THREAD_Error_2 */
  case ERR_THREAD_COAP_START :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_START ",ErrCode);
    break;
  case ERR_THREAD_COAP_ADD_RESSOURCE :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_ADD_RESSOURCE ",ErrCode);
    break;
  case ERR_THREAD_MESSAGE_READ :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_MESSAGE_READ ",ErrCode);
    break;
  case ERR_THREAD_COAP_SEND_RESPONSE :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_SEND_RESPONSE ",ErrCode);
    break;
  case ERR_THREAD_COAP_APPEND :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_APPEND ",ErrCode);
    break;
  case ERR_THREAD_COAP_SEND_REQUEST :
    APP_THREAD_TraceError("ERROR : ERR_THREAD_COAP_SEND_REQUEST ",ErrCode);
    break;
  case ERR_THREAD_MSG_COMPARE_FAILED:
    APP_THREAD_TraceError("ERROR : ERR_THREAD_MSG_COMPARE_FAILED ",ErrCode);
    break;
  /* USER CODE END APP_THREAD_Error_2 */
  default :
    APP_THREAD_TraceError("ERROR Unknown ", 0);
    break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/**
 * @brief Thread initialization.
 * @param  None
 * @retval None
 */
static void APP_THREAD_DeviceConfig(void)
{
  otError error;
  APP_DBG("APP_THREAD_DeviceConfig");

#ifdef ENABLE_OPENTHREAD_CLI
  static otInstance *PtOpenThreadInstance;
  otInstanceFinalize(NULL);
  PtOpenThreadInstance = otInstanceInitSingle();
  otAppCliInit(PtOpenThreadInstance);
#endif /* ENABLE_OPENTHREAD_CLI */
  error = otSetStateChangedCallback(NULL, APP_THREAD_StateNotif, NULL);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error((uint32_t)ERR_THREAD_SET_STATE_CB, (uint32_t)ERR_INTERFACE_FATAL);
  }

  /* Start the timer */
  HW_TS_Start(threadTimerID, (uint32_t)THREAD_FLASH_PROCESS_TRIGGER);
}

/**
 * @brief Thread initialization for Flash testing.
 *        This function configures the Thread mesh network.
 *
 *        It sets the following parameters:
 *        - channel Id
 *        - panId
 *        and enables Thread:
 *        - IPV6
 *        - Thread
 *        then disables Thread:
 *        - Thread
 *
 * @param  None
 * @retval None
 */
static void APP_THREAD_TestDeviceConfig(uint8_t ChannelRef, uint16_t PanidRef)
{
  otError error;
  otNetworkKey networkKey = {{0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}};

  /* Set the channel */
  error = otLinkSetChannel(NULL, ChannelRef);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_CHANNEL, error);
  }

  APP_DBG("Set channel to %d", ChannelRef);

  /* Set the pandId */
  error = otLinkSetPanId(NULL, PanidRef);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_PANID, error);
  }

  APP_DBG("Set panid to %d", PanidRef);

  error = otThreadSetNetworkKey(NULL, &networkKey);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_SET_NETWORK_KEY,error);
  }
  
  APP_THREAD_TestDeviceConfigStart();
}

static void APP_THREAD_TestDeviceConfigStart(void)
{
  otError error;
  /* Activate IPV6 */
  error = otIp6SetEnabled(NULL, true);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_IPV6_ENABLE, error);
  }

  /* Start Thread */
  error = otThreadSetEnabled(NULL, true);
  if (error != OT_ERROR_NONE)
  {
    APP_THREAD_Error(ERR_THREAD_START, error);
  }
}

/**
 * @brief Process Flash testing.
 * @param  None
 * @retval None
 */
static void APP_THREAD_FlashTestProcess(void)
{
  otError error = OT_ERROR_NONE;
  uint8_t retrievedChannel;
  uint16_t retrievedPanid;

  APP_DBG("APP_THREAD_FlashTestProcess called");

  /* Get the channel and check it is between reset value (11) and 14*/
  retrievedChannel = otLinkGetChannel(NULL);
  APP_DBG("  - Channel = %d", retrievedChannel);
  if ((retrievedChannel != CHANNEL_NB_RESET_VALUE) && (retrievedChannel != CHANNEL_NB_14))
  {
    APP_THREAD_Error(ERR_THREAD_GET_CHANNEL, 0U);
  }

  /****** FIRST BOOT ******/
  if(retrievedChannel == CHANNEL_NB_RESET_VALUE)
  {
    g_bootnbr = 1U;
    APP_DBG("STEP1: FIRST BOOT");

    /* Check that the parameters in Flash have been erased */
    retrievedPanid = otLinkGetPanId(NULL);
    if(retrievedPanid != PANID_RESET_VALUE)
    {
      APP_DBG("ERROR: Panid in Flash has not been erased");
    }

    APP_THREAD_TestDeviceConfig(CHANNEL_NB_14, C_PANID);

    /* Green LED shall toggle as device is Leader, then Thread is stopped */
    /* Once in disabled state, the data is stored in Flash */
  }

  /****** SECOND BOOT ******/
  else if(retrievedChannel == CHANNEL_NB_14)
  {
    g_bootnbr = 2U;
    APP_DBG("STEP2: SECOND BOOT");
    /* Check that the parameters in Flash have been stored */
    error = APP_THREAD_CheckConfigValues(CHANNEL_NB_14, C_PANID);
    if(error != OT_ERROR_NONE)
    {
      APP_DBG("ERROR: parameters in Flash have not been stored");
      APP_THREAD_Error(ERR_THREAD_GET_CHANNEL, 0U);
    }

    APP_DBG("Thread Credentials correctly retrieved from FLASH");

    APP_THREAD_TestDeviceConfigStart();

    /* Green LED shall toggle as device is Leader, then Thread is stopped */
    /* Once in disabled state, the data is stored in Flash */
  }
  else
  {
    /* when falling in this case, following actions must be performed:
     *   - Erase stored credentials by calling otInstanceErasePersistentInfo, it will erase RAM buffer
     *   - Write this RAM buffer in FLash by calling APP_THREAD_NVM_Write
     *
     */

    /* error, it can happen when other test case run prior to this application */
    APP_DBG("channel number shall be 11 or 14");

    /* Erase Thread parameters in FLASH */
    APP_DBG("Erase Persistent Info");
    error = otInstanceErasePersistentInfo(NULL);
    if (error != OT_ERROR_NONE)
    {
      APP_THREAD_Error(ERR_THREAD_ERASE_PERSISTENT_INFO,error);
    }
    APP_DBG("Erase Persistent Info Done!");

    APP_THREAD_NVM_Write();
    APP_DBG("perform a NVIC_RESET");
    NVIC_SystemReset();
  }

}

/**
 * @brief Thread notification when the state changes.
 * @param  aFlags  : Define the item that has been modified
 *         aContext: Context
 *
 * @retval None
 */
static void APP_THREAD_StateNotif(uint32_t NotifFlags, void *pContext)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pContext);

  /* USER CODE BEGIN APP_THREAD_STATENOTIF */
  otError error;
  /* USER CODE END APP_THREAD_STATENOTIF */

  if ((NotifFlags & (uint32_t)OT_CHANGED_THREAD_ROLE) == (uint32_t)OT_CHANGED_THREAD_ROLE)
  {
    switch (otThreadGetDeviceRole(NULL))
    {
    case OT_DEVICE_ROLE_DISABLED:
      /* USER CODE BEGIN OT_DEVICE_ROLE_DISABLED */
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      UTIL_SEQ_SetTask(TASK_NVM,CFG_SCH_PRIO_1);
      /* USER CODE END OT_DEVICE_ROLE_DISABLED */
      break;
    case OT_DEVICE_ROLE_DETACHED:
      /* USER CODE BEGIN OT_DEVICE_ROLE_DETACHED */
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      /* USER CODE END OT_DEVICE_ROLE_DETACHED */
      break;
    case OT_DEVICE_ROLE_CHILD:
      /* USER CODE BEGIN OT_DEVICE_ROLE_CHILD */
      BSP_LED_Off(LED2);
      BSP_LED_On(LED3);
      /* USER CODE END OT_DEVICE_ROLE_CHILD */
      break;
    case OT_DEVICE_ROLE_ROUTER :
      /* USER CODE BEGIN OT_DEVICE_ROLE_ROUTER */
      BSP_LED_Off(LED2);
      BSP_LED_On(LED3);
      /* USER CODE END OT_DEVICE_ROLE_ROUTER */
      break;
    case OT_DEVICE_ROLE_LEADER :
      /* USER CODE BEGIN OT_DEVICE_ROLE_LEADER */
      BSP_LED_On(LED2);
      BSP_LED_Off(LED3);
      HAL_Delay(3000); /* wait to have time to see LED2 ON */
      /* Stop Thread */
      error = otThreadSetEnabled(NULL, false);
      if (error != OT_ERROR_NONE)
      {
        APP_THREAD_Error(ERR_THREAD_STOP, error);
      }
      /* USER CODE END OT_DEVICE_ROLE_LEADER */
      break;
    default:
      /* USER CODE BEGIN DEFAULT */
      BSP_LED_Off(LED2);
      BSP_LED_Off(LED3);
      /* USER CODE END DEFAULT */
      break;
    }
  }
}

/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (OpenThread or other module if any)
  * @retval None
  */
static void APP_THREAD_TraceError(const char * pMess, uint32_t ErrCode)
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
static void APP_THREAD_CheckWirelessFirmwareInfo(void)
{
  WirelessFwInfo_t wireless_info_instance;
  WirelessFwInfo_t* p_wireless_info = &wireless_info_instance;

  if (SHCI_GetWirelessFwInfo(p_wireless_info) != SHCI_Success)
  {
    APP_THREAD_Error((uint32_t)ERR_THREAD_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
  }
  else
  {
    APP_DBG("**********************************************************");
    APP_DBG("WIRELESS COPROCESSOR FW:");
    /* Print version */
    APP_DBG("VERSION ID = %d.%d.%d", p_wireless_info->VersionMajor, p_wireless_info->VersionMinor, p_wireless_info->VersionSub);

    switch(p_wireless_info->StackType)
    {
    case INFO_STACK_TYPE_THREAD_FTD :
      APP_DBG("FW Type : Thread FTD");
      break;
    case INFO_STACK_TYPE_THREAD_MTD :
      APP_DBG("FW Type : Thread MTD");
      break;
    case INFO_STACK_TYPE_BLE_THREAD_FTD_STATIC :
      APP_DBG("FW Type : Static Concurrent Mode BLE/Thread");
      break;
    default :
      /* No Thread device supported ! */
      APP_THREAD_Error((uint32_t)ERR_THREAD_CHECK_WIRELESS, (uint32_t)ERR_INTERFACE_FATAL);
      break;
    }
    APP_DBG("**********************************************************");
  }
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
/**
 * @brief  Init the NVM
 * @param  None
 * @retval None
 */
static void APP_THREAD_NVM_Init(void)
{
  int eeprom_init_status;

  /* Take semaphore to make sure there will be no modification of the NVM Ram buffer during Flash operation */
  while((LL_HSEM_1StepLock( HSEM, CFG_HW_THREAD_NVM_SRAM_SEMID ))==1 )
  {
	  HAL_Delay(1U);
  }

  APP_DBG("Flash starting address = %x", HW_FLASH_ADDRESS  + CFG_NVM_BASE_ADDRESS);
  eeprom_init_status = EE_Init( 0 , HW_FLASH_ADDRESS + CFG_NVM_BASE_ADDRESS );

  if(eeprom_init_status != EE_OK)
  {
    /* format NVM since init failed */
    eeprom_init_status= EE_Init( 1, HW_FLASH_ADDRESS + CFG_NVM_BASE_ADDRESS );
  }
  APP_DBG("EE_init status = %d",eeprom_init_status);

  /* Retrieve data from NVM to RAM buffer */
  APP_DBG("Retrieving persistent data from FLASH");
  if ( APP_THREAD_NVM_Read() == false ){
    APP_DBG("APP_ZIGBEE_NVM_Read failed!");
  }

  /* Release semaphore to allow operation of NVM Ram buffer on CPU2 side */
  LL_HSEM_ReleaseLock( HSEM, CFG_HW_THREAD_NVM_SRAM_SEMID, 0 );
}

/**
 *@brief  Read the persistent data from NVM
 * @param  None
 * @retval true if success , false if failed
 */
static bool APP_THREAD_NVM_Read(void)
{
  bool status = true;
//  int ee_status = 0;
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);

  /**
   * In the current application, the NVMA_Read() is used only to dump the NVM to the SRAM
   * at startup.
   *
   * In case there are some use cases where the application needs to read only 1 data,
   * a test versus the Size needs to be implemented
   * The algorithm to use depends on the way the application stores the data in NVM but most of the time,
   * EE_Read() should be used when Size = 1 and EE_Dump() otherwise
   */
  EE_Dump( 0, 0, OT_NvmRamBuffer, CFG_EE_BANK0_MAX_NB);

//  for (uint16_t index = 0; index < CFG_THREAD_NVM_RAM_SIZE; index++)
//  {
//    ee_status = EE_Read(0, index, &OT_NvmRamBuffer[index]);
//    if (ee_status != EE_OK)
//    {
//      APP_DBG("Read not found leaving");
//      status = false;
//      break;
//    }
//  }

  HAL_FLASH_Lock();
  return status;
}

/**
 * @brief  Write the persistent data in NVM
 * @param  None
 * @retval None
 */
static bool APP_THREAD_NVM_Write(void)
{
  int ee_status = 0;
  int index = 0;

  /* Take semaphore to make sure there will be no modification of the NVM Ram buffer during Flash operation */
  while( LL_HSEM_1StepLock( HSEM, CFG_HW_THREAD_NVM_SRAM_SEMID ) );

  for (index = 0; index < CFG_EE_BANK0_MAX_NB; index++)
  {
    ee_status = EE_Write(0, (uint16_t)index, OT_NvmRamBuffer[index]);
    if (ee_status != EE_OK)
    {
      if(ee_status == EE_CLEAN_NEEDED) /* Shall not be there if CFG_EE_AUTO_CLEAN = 1*/
      {
        APP_DBG("CLEAN NEEDED, CLEANING");
        EE_Clean(0,0);
      }
      else
      {
        /* Failed to write , an Erase shall be done */
        APP_DBG("APP_THREAD_NVM_Write failed @ %d status %d", index, ee_status);
        break;
      }
    }
  }

  /* Release semaphore to allow operation of NVM Ram buffer on CPU2 side */
  LL_HSEM_ReleaseLock( HSEM, CFG_HW_THREAD_NVM_SRAM_SEMID, 0 );

  if(ee_status != EE_OK)
  {
    APP_DBG("WRITE STOPPED, need a FLASH ERASE");
    return false;
  }

  return true;
}

static void APP_THREAD_NvmSave(void)
{
  APP_DBG("--- APP_THREAD_NvmSave START ---");
  if(APP_THREAD_NVM_Write() == true)
  {
    APP_DBG("APP_THREAD_NVM_Write SUCCESS!");
  }
  else
  {
    APP_DBG("APP_THREAD_NVM_Write FAIL!");
  }
  APP_DBG("--- APP_THREAD_NvmSave END ---");
}

static void APP_THREAD_NvmProcess(void)
{
  otError error;
  
  if(g_bootnbr == 1)
  {
    APP_THREAD_NvmSave();

    APP_DBG("\n**** Now you can reboot(POWER OFF/POWER ON) the device to perform STEP2 ****\n");

    /* Toggle the Blue LED */
    for (int var = 0; var < 100; ++var) {
      BSP_LED_Toggle(LED1);
      HAL_Delay(50U);
    }
    BSP_LED_Off(LED1);
  }
  if(g_bootnbr == 2)
  {
    /* Erase Thread parameters in FLASH */
    APP_DBG("Erase Persistent Info");
    error = otInstanceErasePersistentInfo(NULL);
    if (error != OT_ERROR_NONE)
    {
      APP_THREAD_Error(ERR_THREAD_ERASE_PERSISTENT_INFO,error);
    }
    APP_DBG("Erase Persistent Info Done!");

    APP_DBG("Once RAM buffer erased, copy in flash");
    APP_THREAD_NvmSave();
    APP_DBG("Flash update done!");

    APP_DBG("\n**** Now you can reboot(POWER OFF/POWER ON) the device to restart from STEP1 ****\n");

    /* Toggle the Blue LED */
    for (int var = 0; var < 10; ++var) {
      BSP_LED_Toggle(LED1);
      HAL_Delay(500U);
    }
    BSP_LED_Off(LED1);
  }
}

/**
 * @brief Thread configuration check.
 *        This function checks the values stored in Flash.
 *
 *        It checks the following parameters in FLASH:
 *        - channel Id
 *        - panId
 *
 * @param  None
 * @retval None
 */
static otError APP_THREAD_CheckConfigValues(uint8_t ChannelRef, uint16_t PanidRef)
{
  uint8_t channel;
  uint16_t panid;
  otError error = OT_ERROR_NONE;

  /* Get the channel */
  channel = otLinkGetChannel(NULL);
  if (channel != ChannelRef)
  {
    error = OT_ERROR_FAILED;
  }

  /* Get the pandId */
  panid = otLinkGetPanId(NULL);
  if (panid != PanidRef)
  {
    error = OT_ERROR_FAILED;
  }

  return error;
}
/* USER CODE END FD_LOCAL_FUNCTIONS */

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/

void APP_THREAD_RegisterCmdBuffer(TL_CmdPacket_t* p_buffer)
{
  p_thread_otcmdbuffer = p_buffer;
}

Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)p_thread_otcmdbuffer->cmdserial.cmd.payload;
}

Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdRspPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)((TL_EvtPacket_t *)p_thread_otcmdbuffer)->evtserial.evt.payload;
}

Thread_OT_Cmd_Request_t* THREAD_Get_NotificationPayloadBuffer(void)
{
  return (Thread_OT_Cmd_Request_t*)(p_thread_notif_M0_to_M4)->evtserial.evt.payload;
}

static void Ot_Cmd_Transfer_Common(void)
{

  /* OpenThread OT command cmdcode range 0x280 .. 0x3DF = 352 */
  p_thread_otcmdbuffer->cmdserial.cmd.cmdcode = 0x280U;
  /* Size = otCmdBuffer->Size (Number of OT cmd arguments : 1 arg = 32bits so multiply by 4 to get size in bytes)
   * + ID (4 bytes) + Size (4 bytes) */
  uint32_t l_size = ((Thread_OT_Cmd_Request_t*)(p_thread_otcmdbuffer->cmdserial.cmd.payload))->Size * 4U + 8U;
  p_thread_otcmdbuffer->cmdserial.cmd.plen = l_size;

  TL_OT_SendCmd();

  /* Wait completion of cmd */
  Wait_Getting_Ack_From_M0();

}

/**
 * @brief  This function is used to transfer the Ot commands from the
 *         M4 to the M0.
 *
 * @param   None
 * @return  None
 */
void Ot_Cmd_Transfer(void)
{
  Ot_Cmd_Transfer_Common();
}

/**
 * @brief  This function is used to transfer the Ot commands from the
 *         M4 to the M0 with Notification M0 to M4 allowed.
 *
 * @param   None
 * @return  None
 */
void Ot_Cmd_TransferWithNotif(void)
{
  /* Flag to specify to UTIL_SEQ_EvtIdle that M0 to M4 notifications are allowed */
  g_ot_notification_allowed = 1U;

  Ot_Cmd_Transfer_Common();
}

/**
 * @brief  This function is called when acknowledge from OT command is received from the M0+.
 *
 * @param   Otbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_OT_CmdEvtReceived( TL_EvtPacket_t * Otbuffer )
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Otbuffer);

  Receive_Ack_From_M0();

  /* Does not allow OpenThread M0 to M4 notification */
  g_ot_notification_allowed = 0U;
}

/**
 * @brief  This function is called when notification from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_THREAD_NotReceived( TL_EvtPacket_t * Notbuffer )
{
  p_thread_notif_M0_to_M4 = Notbuffer;

  Receive_Notification_From_M0();
}

/**
  * @brief  This function is called before sending any ot command to the M0
  *         core. The purpose of this function is to be able to check if
  *         there are no notifications coming from the M0 core which are
  *         pending before sending a new ot command.
  * @param  None
  * @retval None
  */
void Pre_OtCmdProcessing(void)
{
  UTIL_SEQ_WaitEvt(EVENT_SYNCHRO_BYPASS_IDLE);
}

/**
  * @brief  This function waits for getting an acknowledgment from the M0.
  *
  * @param  None
  * @retval None
  */
static void Wait_Getting_Ack_From_M0(void)
{
  UTIL_SEQ_WaitEvt(EVENT_ACK_FROM_M0_EVT);
}

/**
  * @brief  Receive an acknowledgment from the M0+ core.
  *         Each command send by the M4 to the M0 are acknowledged.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Ack_From_M0(void)
{
  UTIL_SEQ_SetEvt(EVENT_ACK_FROM_M0_EVT);
}

/**
  * @brief  Receive a notification from the M0+ through the IPCC.
  *         This function is called under interrupt.
  * @param  None
  * @retval None
  */
static void Receive_Notification_From_M0(void)
{
  CptReceiveMsgFromM0++;
  UTIL_SEQ_SetTask(TASK_MSG_FROM_M0_TO_M4,CFG_SCH_PRIO_0);
}

#if (CFG_USB_INTERFACE_ENABLE != 0)
#else
#if (CFG_FULL_LOW_POWER == 0)
static void RxCpltCallback(void)
{
  /* Filling buffer and wait for '\r' char */
  if (indexReceiveChar < C_SIZE_CMD_STRING)
  {
    CommandString[indexReceiveChar++] = aRxBuffer[0];
    if (aRxBuffer[0] == '\r')
    {
      CptReceiveCmdFromUser = 1U;

      /* UART task scheduling*/
      UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_CLI_TO_M0, CFG_SCH_PRIO_0);
    }
  }

  /* Once a character has been sent, put back the device in reception mode */
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1U, RxCpltCallback);
}
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
 * @brief Process the command strings.
 *        As soon as a complete command string has been received, the task
 *        in charge of sending the command to the M0 is scheduled
 * @param  None
 * @retval None
 */
static uint32_t  ProcessCmdString( uint8_t* buf , uint32_t len )
{
  uint32_t i,j,tmp_start;
  tmp_start = 0;
  uint32_t res = 0;

  i= 0;
  while ((buf[i] != '\r') && (i < len))
  {
    i++;
  }

  if (i != len)
  {
    memcpy(CommandString, buf,(i+1));
    indexReceiveChar = i + 1U; /* Length of the buffer containing the command string */
    UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_CLI_TO_M0, CFG_SCH_PRIO_0);
    tmp_start = i;
    for (j = 0; j < (len - tmp_start - 1U) ; j++)
    {
      buf[j] = buf[tmp_start + j + 1U];
    }
    res = len - tmp_start - 1U;
  }
  else
  {
    res = len;
  }
  return res; /* Remaining characters in the temporary buffer */
}
#endif/* (CFG_USB_INTERFACE_ENABLE != 0) */

#if (CFG_FULL_LOW_POWER == 0)
/**
 * @brief Process sends receive CLI command to M0.
 * @param  None
 * @retval None
 */
static void Send_CLI_To_M0(void)
{ 
#ifdef ENABLE_OPENTHREAD_CLI
  /* Don't use the ThreadCliCmdBuffer when buffer is too large as data may be overwritten.
     Use locals variables instead */
  uint16_t l_plen = 0;
  uint8_t l_ThreadCliCmdBuffer[255] = {0};
  
  memcpy(l_ThreadCliCmdBuffer, CommandString, indexReceiveChar);
  l_plen = indexReceiveChar;
  
    /* Clear receive buffer, character counter and command complete */
  CptReceiveCmdFromUser = 0;
  indexReceiveChar = 0;
  memset(CommandString, 0, C_SIZE_CMD_STRING);
  
  APP_DBG("[Send_CLI_To_M0] payload : %s", l_ThreadCliCmdBuffer);

  otPlatUartReceived(l_ThreadCliCmdBuffer, l_plen);  
#else
  memset(ThreadCliCmdBuffer.cmdserial.cmd.payload, 0x0U, 255U);
  memcpy(ThreadCliCmdBuffer.cmdserial.cmd.payload, CommandString, indexReceiveChar);
  ThreadCliCmdBuffer.cmdserial.cmd.plen = indexReceiveChar;
  ThreadCliCmdBuffer.cmdserial.cmd.cmdcode = 0x0;

  /* Clear receive buffer, character counter and command complete */
  CptReceiveCmdFromUser = 0;
  indexReceiveChar = 0;
  memset(CommandString, 0, C_SIZE_CMD_STRING);

  APP_DBG("[Send_CLI_To_M0] payload : %s", ThreadCliCmdBuffer.cmdserial.cmd.payload);

  TL_CLI_SendCmd();
#endif /* ENABLE_OPENTHREAD_CLI */
}
#endif /* (CFG_FULL_LOW_POWER == 0) */

#ifdef ENABLE_OPENTHREAD_CLI
/**
 * @brief  Wrapper function to flush UART data (called from the OpenThread stack)
           Not used but definition needed.
 * @param  None
 * @retval OT_ERROR_NONE
 */
otError otPlatUartFlush(void)
{
  return OT_ERROR_NONE;
}

/**
 * @brief  Wrapper function to send data through the UART from the OpenThread stack
 * @param  aBuf: Buffer of data to transmit
 * @param  aBufLength: Number of data to transmit (in bytes)
 * @retval OT_ERROR_NONE
 */
otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
   /* WORKAROUND: if string to output is "> " then respond directly to M0 and do not output it */
  if (strcmp((const char *)aBuf, "> ") != 0)
  {
    /* Write to CLI UART */
    HW_UART_Transmit(CFG_CLI_UART, (uint8_t*)aBuf, aBufLength, 100);
  }

  otPlatUartSendDone();
  
  return OT_ERROR_NONE;
}
#endif /* ENABLE_OPENTHREAD_CLI */

/**
 * @brief Send notification for CLI TL Channel.
 * @param  None
 * @retval None
 */
static void Send_CLI_Ack_For_OT(void)
{

  /* Notify M0 that characters have been sent to UART */
  TL_THREAD_CliSendAck();
}

/**
 * @brief Perform initialization of CLI UART interface.
 * @param  None
 * @retval None
 */
void APP_THREAD_Init_UART_CLI(void)
{
#if (CFG_FULL_LOW_POWER == 0)
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SEND_CLI_TO_M0, UTIL_SEQ_RFU,Send_CLI_To_M0);
#endif /* (CFG_FULL_LOW_POWER == 0) */

#if (CFG_USB_INTERFACE_ENABLE != 0)
#else
#if (CFG_FULL_LOW_POWER == 0)
  MX_LPUART1_UART_Init();
  HW_UART_Receive_IT(CFG_CLI_UART, aRxBuffer, 1, RxCpltCallback);
#endif /* (CFG_FULL_LOW_POWER == 0) */
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
}

/**
 * @brief Perform initialization of TL for THREAD.
 * @param  None
 * @retval None
 */
void APP_THREAD_TL_THREAD_INIT(void)
{
  ThreadConfigBuffer.p_ThreadOtCmdRspBuffer = (uint8_t*)&ThreadOtCmdBuffer;
  ThreadConfigBuffer.p_ThreadNotAckBuffer = (uint8_t*)ThreadNotifRspEvtBuffer;
  ThreadConfigBuffer.p_ThreadCliRspBuffer = (uint8_t*)&ThreadCliCmdBuffer;
  ThreadConfigBuffer.p_ThreadCliNotBuffer = (uint8_t*)&ThreadCliNotBuffer;

  TL_THREAD_Init( &ThreadConfigBuffer );
}

/**
 * @brief  This function is called when notification on CLI TL Channel from M0+ is received.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void TL_THREAD_CliNotReceived( TL_EvtPacket_t * Notbuffer )
{
  TL_CmdPacket_t* l_CliBuffer = (TL_CmdPacket_t*)Notbuffer;
  uint8_t l_size = l_CliBuffer->cmdserial.cmd.plen;

  /* WORKAROUND: if string to output is "> " then respond directly to M0 and do not output it */
  if (strcmp((const char *)l_CliBuffer->cmdserial.cmd.payload, "> ") != 0)
  {
    /* Write to CLI UART */
#if (CFG_USB_INTERFACE_ENABLE != 0)
    VCP_SendData( l_CliBuffer->cmdserial.cmd.payload, l_size, HostTxCb);
#else
    HW_UART_Transmit_IT(CFG_CLI_UART, l_CliBuffer->cmdserial.cmd.payload, l_size, HostTxCb);
#endif /*USAGE_OF_VCP */
  }
  else
  {
    Send_CLI_Ack_For_OT();
  }
}

/**
 * @brief  End of transfer callback for CLI UART sending.
 *
 * @param   Notbuffer : a pointer to TL_EvtPacket_t
 * @return  None
 */
void HostTxCb(void)
{
  Send_CLI_Ack_For_OT();
}

/**
 * @brief Process the messages coming from the M0.
 * @param  None
 * @retval None
 */
void APP_THREAD_ProcessMsgM0ToM4(void)
{
  if (CptReceiveMsgFromM0 != 0)
  {
    /* If CptReceiveMsgFromM0 is > 1. it means that we did not serve all the events from the radio */
    if (CptReceiveMsgFromM0 > 1U)
    {
      APP_THREAD_Error(ERR_REC_MULTI_MSG_FROM_M0, 0);
    }
    else
    {
      OpenThread_CallBack_Processing();
    }
    /* Reset counter */
    CptReceiveMsgFromM0 = 0;
  }
}

#if (CFG_USB_INTERFACE_ENABLE != 0)
/**
 * @brief  This function is called when thereare some data coming
 *         from the Hyperterminal via the USB port
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data received
 * @param  Len: Number of data received (in bytes)
 * @retval Number of characters remaining in the buffer and not yet processed
 */
void VCP_DataReceived(uint8_t* Buf , uint32_t *Len)
{
  uint32_t i,flag_continue_checking = TRUE;
  uint32_t char_remaining = 0;
  static uint32_t len_total = 0;

  /* Copy the characters in the temporary buffer */
  for (i = 0; i < *Len; i++)
  {
    TmpString[len_total++] = Buf[i];
  }

  /* Process the buffer commands one by one     */
  /* A command is limited by a \r caracaters    */
  while (flag_continue_checking == TRUE)
  {
    char_remaining = ProcessCmdString(TmpString,len_total);
    /* If char_remaining is equal to len_total, it means that the command string is not yet
     * completed.
     * If char_remaining is equal to 0, it means that the command string has
     * been entirely processed.
     */
    if ((char_remaining == 0) || (char_remaining == len_total))
    {
      flag_continue_checking = FALSE;
    }
    len_total = char_remaining;
  }
}
#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
