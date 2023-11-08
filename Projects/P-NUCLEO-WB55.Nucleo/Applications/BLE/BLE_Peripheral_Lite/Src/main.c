/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file    main.c
* @author  MCD Application Team
* @brief   BLE application with BLE core
*
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
@verbatim
==============================================================================
##### IMPORTANT NOTE #####
==============================================================================

This application requests having the stm32wb5x_BLE_Stack_fw.bin binary
flashed on the Wireless Coprocessor.
If it is not the case, you need to use STM32CubeProgrammer to load the appropriate
binary.

All available binaries are located under following directory:
/Projects/STM32_Copro_Wireless_Binaries

Refer to UM2237 to learn how to use/install STM32CubeProgrammer.
Refer to /Projects/STM32_Copro_Wireless_Binaries/ReleaseNote.html for the
detailed procedure to change the Wireless Coprocessor binary.

@endverbatim
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_common.h"
#include "hw_conf.h"
#include "otp.h"
#include "main.h"
#include "ble.h"
#include "hci_tl.h"
#include "shci_tl.h"
#include "shci.h"
#include "app_debug.h"
#include "gatt_service.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define EVT_END_OF_RADIO_ACTIVITY           0x0004

#define BD_ADDR_SIZE_LOCAL                  6
#define APP_BLE_GAP_DEVICE_NAME_LENGTH      sizeof(gap_device_name)
#define EVENT_POOL_SIZE                    (CFG_TLBLE_EVT_QUEUE_LENGTH*4U*DIVC(( sizeof(TL_PacketHeader_t) + TL_BLE_EVENT_FRAME_SIZE ), 4U))

#define APP_FLAG_CPU2_INITIALIZED           0
#define APP_FLAG_CPU2_ERROR                24
#define APP_FLAG_WIRELESS_FW_RUNNING        1
#define APP_FLAG_FUS_FW_RUNNING             2
#define APP_FLAG_BLE_INITIALIZATION_ERROR  25
#define APP_FLAG_BLE_INITIALIZED            3
#define APP_FLAG_BLE_ADVERTISING            4
#define APP_FLAG_BLE_CONNECTED              5
#define APP_FLAG_HCI_EVENT_PENDING         18
#define APP_FLAG_SHCI_EVENT_PENDING        19
#define APP_FLAG_GET(flag)                  VariableBit_Get_BB(((uint32_t)&APP_State), flag)
#define APP_FLAG_SET(flag)                  VariableBit_Set_BB(((uint32_t)&APP_State), flag)
#define APP_FLAG_RESET(flag)                VariableBit_Reset_BB(((uint32_t)&APP_State), flag)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static volatile uint32_t APP_State = 0x00000000;

PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t EvtPool[EVENT_POOL_SIZE];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t SystemCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static uint8_t SystemSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t BleSpareEvtBuffer[sizeof(TL_PacketHeader_t) + TL_EVT_HDR_SIZE + 255];

static uint8_t bd_address_udn[BD_ADDR_SIZE_LOCAL];

static uint8_t myVeryOwnNotifyCharacteristicData[MY_VERY_OWN_NOTIFY_CHARACTERISTIC_VALUE_LENGTH] = {0x00, 0x00};

/* Generic Access GATT Service Characteristics configuration data  */
static const char gap_device_name[] = { 'H', 'E', 'L', 'L', 'O', '!' };
static const uint16_t gap_appearance = BLE_CFG_GAP_APPEARANCE;

/* GAP Advertising data */
static const char ad_local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'H', 'E', 'L', 'L', 'O', '!' };
static uint8_t ad_manufacturer_specific_data[14] = { /* Manufacturer specific data used to get compliant with ST BLE Sensor smart phone apk */
    sizeof(ad_manufacturer_specific_data)-1,
    AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 
    0x01, /* BlueST Protocol version */
    0x83, /* BlueST Device Id: 0x83 - P2PServer1 - for more details please see BLE_p2pServer example project */
    0x00, /* BlueST Feature Mask bits 24~31 */
    0x00, /* BlueST Feature Mask bits 16~23 */
    0x00, /* BlueST Feature Mask bits 8~15 */
    0x00, /* BlueST Feature Mask bits 0~7 */
    0x00, /* BlueST Device MAC byte 5 */
    0x00, /* BlueST Device MAC byte 4 */
    0x00, /* BlueST Device MAC byte 3 */
    0x00, /* BlueST Device MAC byte 2 */
    0x00, /* BlueST Device MAC byte 1 */
    0x00  /* BlueST Device MAC byte 0 */
};

static const uint8_t a_MBdAddr[BD_ADDR_SIZE_LOCAL] =
{
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000000000FF)),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00000000FF00) >> 8),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x000000FF0000) >> 16),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000FF000000) >> 24),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00FF00000000) >> 32),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0xFF0000000000) >> 40)
};


/* More details about BlueST protocol and how it is used in our demos and examples
   can be found in the related documentation, e.g. in UM2496 */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RF_Init(void);
/* USER CODE BEGIN PFP */
static void Tune_HSE(void);
static void CPU2_Init(void);
static void Ble_Tl_Init(void);
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* Ble_GetBdAddress(void);
static void SYS_UserEventReceivedCallback(void * pData);
static void SYS_StatusNotificationCallback(SHCI_TL_CmdStatus_t status);
static void SYS_ProcessEvent(void);
static void BLE_UserEventReceivedCallback(void * pData);
static void BLE_StatusNotificationCallback(HCI_TL_CmdStatus_t status);
static void BLE_Init(void);
static void BLE_Advertising(FlagStatus setReset);
static void BLE_ProcessEvent(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /**
   * The OPTVERR flag is wrongly set at power on
   * It shall be cleared before using any HAL_FLASH_xxx() api
   */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  uint32_t prevTick = 0;
  /* USER CODE END 1 */
  
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* Tune the HSE internal load capacitors - P-NUCLEO-WB55.Nucleo board */
  Tune_HSE();
  
  /* Configure the debug support if needed */
  APPD_Init();
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RF_Init();
  /* USER CODE BEGIN 2 */
  
  /* Initialize the GPIO pins for P-NUCLEO-WB55.Nucleo board LEDs control */  
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  
  /* Initialize all transport layers */
  CPU2_Init();
   
  /* Set the red LED On to indicate that the CPU2 is initializing */
  BSP_LED_On(LED_RED); 
  
  /* Wait until the CPU2 gets initialized */
  while((APP_FLAG_GET(APP_FLAG_CPU2_INITIALIZED) == 0) \
        || (APP_FLAG_GET(APP_FLAG_WIRELESS_FW_RUNNING) == 0))
  {
    /* Process pending SYSTEM event coming from CPU2 (if any) */
    SYS_ProcessEvent();
  }
    
  /* Configure the CPU2 Debug (Optional) */
  APPD_EnableCPU2();
  
  /* Set the red LED Off to indicate that the CPU2 is initialized */
  BSP_LED_Off(LED_RED);
  
  /* Set the green LED On to indicate that the wireless stack FW is running */
  BSP_LED_On(LED_GREEN);
  
  /* At this point it is still unknown from the app perspective, which wireless stack
     and which version is installed on CPU2. It is expected that a BLE stack is installed.
     In order to check that, SHCI_GetWirelessFwInfo(...) can be used to read out
     the information about the CM0+ wireless stack FW running since the Device Information Table
     is initialized. For more information on this topic, please refer to AN5289 and AN5185. */
  
  /* Initialize BLE (BLE TL, BLE stack, HAL, HCI, GATT, GAP) */
  BLE_Init();

  /* Set the blue LED On to indicate that the BLE stack is initialized */
  BSP_LED_On(LED_BLUE);

  /* Initialize My Very Own GATT Service - user may also implement SVCCTL_InitCustomSvc()
     interface function as explained in AN5289. SVCCTL_InitCustomSvc() is called at the end of
     SVCCTL_Init() called from BLE_Init() */
  MyVeryOwnService_Init();
  
  /* Reset BLUE LED => Will be used by the example */
  BSP_LED_Off(LED_BLUE);
  
  /* Start BLE advertising */
  BLE_Advertising(SET);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Process pending BLE event coming from CPU2 (if any) */
    BLE_ProcessEvent();
    /* Process pending SYSTEM event coming from CPU2 (if any) */
    SYS_ProcessEvent();
        
    /* Update the My Very Own Notify Characteristic every ~1 second and only if BLE connected.
       It might be also done only after the GATT client enables the notifications,
       but that is out of scope of this basic example */
    if (APP_FLAG_GET(APP_FLAG_BLE_CONNECTED) != 0x00)
    {
      if ((HAL_GetTick() - prevTick) > 1000)
      {
        prevTick = HAL_GetTick();
        myVeryOwnNotifyCharacteristicData[1] ^= 0x01;
        if (MyVeryOwnWriteCharacteristic_Update(MY_VERY_OWN_NOTIFY_CHARACTERISTIC_UUID,
                                                MY_VERY_OWN_NOTIFY_CHARACTERISTIC_VALUE_LENGTH,
                                                myVeryOwnNotifyCharacteristicData) != BLE_STATUS_SUCCESS)
        {
          Error_Handler(); /* UNEXPECTED */
        }
      }
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_RFWAKEUP;
  PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSE;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RF Initialization Function
  * @param None
  * @retval None
  */
static void MX_RF_Init(void)
{

  /* USER CODE BEGIN RF_Init 0 */

  /* USER CODE END RF_Init 0 */

  /* USER CODE BEGIN RF_Init 1 */

  /* USER CODE END RF_Init 1 */
  /* USER CODE BEGIN RF_Init 2 */

  /* USER CODE END RF_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/**
* @brief This function initializes and releases the CPU2 subsystem
* @param None
* @retval None
*/
static void CPU2_Init( void )
{
  TL_MM_Config_t tl_mm_config;
  SHCI_TL_HciInitConf_t SHci_Tl_Init_Conf;
  
  /**< Reference table initialization */
  TL_Init();
  
  /**< System channel initialization */
  SHci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&SystemCmdBuffer;
  SHci_Tl_Init_Conf.StatusNotCallBack = SYS_StatusNotificationCallback;
  shci_init(SYS_UserEventReceivedCallback, (void*) &SHci_Tl_Init_Conf);
  
  /**< Memory Manager channel initialization */
  tl_mm_config.p_AsynchEvtPool = EvtPool;
  tl_mm_config.p_BleSpareEvtBuffer = BleSpareEvtBuffer; /* UNUSED, but kept for future compatibility */
  tl_mm_config.p_SystemSpareEvtBuffer = SystemSpareEvtBuffer; /* UNUSED, but kept for future compatibility, but used by FUS today only */
  tl_mm_config.AsynchEvtPoolSize = EVENT_POOL_SIZE;
  TL_MM_Init( &tl_mm_config );
  
  /**< Release the CPU2 */
  TL_Enable();
  
  return;
}

/**
* @brief This function initializes the BLE stack
* @param None
* @retval None
*/
static void BLE_Init( void )
{
  SHCI_CmdStatus_t ret;
  
  SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet =
  {
    {{0,0,0}},                          /**< Header unused */
    {0,                                 /** pBleBufferAddress not used */
    0,                                  /** BleBufferSize not used */
    CFG_BLE_NUM_GATT_ATTRIBUTES,
    CFG_BLE_NUM_GATT_SERVICES,
    CFG_BLE_ATT_VALUE_ARRAY_SIZE,
    CFG_BLE_NUM_LINK,
    CFG_BLE_DATA_LENGTH_EXTENSION,
    CFG_BLE_PREPARE_WRITE_LIST_SIZE,
    CFG_BLE_MBLOCK_COUNT,
    CFG_BLE_MAX_ATT_MTU,
    CFG_BLE_PERIPHERAL_SCA,
    CFG_BLE_CENTRAL_SCA,
    CFG_BLE_LS_SOURCE,
    CFG_BLE_MAX_CONN_EVENT_LENGTH,
    CFG_BLE_HSE_STARTUP_TIME,
    CFG_BLE_VITERBI_MODE,
    CFG_BLE_OPTIONS,
    0,
    CFG_BLE_MAX_COC_INITIATOR_NBR,
    CFG_BLE_MIN_TX_POWER,
    CFG_BLE_MAX_TX_POWER,
    CFG_BLE_RX_MODEL_CONFIG,
     CFG_BLE_MAX_ADV_SET_NBR, 
     CFG_BLE_MAX_ADV_DATA_LEN,
     CFG_BLE_TX_PATH_COMPENS,
     CFG_BLE_RX_PATH_COMPENS,
     CFG_BLE_CORE_VERSION,
     CFG_BLE_OPTIONS_EXT
    }
  };
  
  /**
  * Initialize Ble Transport Layer
  */
  Ble_Tl_Init( );
  
  /**
  * Starts the BLE Stack on CPU2
  */
  ret = SHCI_C2_BLE_Init( &ble_init_cmd_packet );
  if (ret != SHCI_Success)
  {
    Error_Handler(); /* UNEXPECTED */
  }
  
  /**
  * Initialization of HCI & GATT & GAP layer
  */
  Ble_Hci_Gap_Gatt_Init();
  
  /**
  * Initialization of the BLE Services
  */
  SVCCTL_Init();
  
  return;
}

/**
* @brief This function is used to process all events coming from BLE stack by executing the related callback
* @param None
* @retval None
*/
static void BLE_ProcessEvent(void)
{
  if (APP_FLAG_GET(APP_FLAG_HCI_EVENT_PENDING) == 1)
  {
    APP_FLAG_RESET(APP_FLAG_HCI_EVENT_PENDING);
    hci_user_evt_proc();
  }
}

/**
* @brief This function initializes the BLE Transport Layer
* @param None
* @retval None
*/
static void Ble_Tl_Init( void )
{
  HCI_TL_HciInitConf_t Hci_Tl_Init_Conf;
  
  /**< BLE channel initialization */
  Hci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&BleCmdBuffer;
  Hci_Tl_Init_Conf.StatusNotCallBack = BLE_StatusNotificationCallback;
  hci_init(BLE_UserEventReceivedCallback, (void*) &Hci_Tl_Init_Conf);
  
  return;
}

/**
* @brief This function starts or stops the BLE advertising
* @param newState: SET to start the advertising, RESET to stop
* @retval None
*/
static void BLE_Advertising(FlagStatus newState)
{
  tBleStatus ret = BLE_STATUS_SUCCESS;
  
  if (newState == SET)
  {
    if (APP_FLAG_GET(APP_FLAG_BLE_ADVERTISING) == 0)
    {
      /**
      * Put the device in a advertising & connectable mode.
      */
      ret = aci_gap_set_discoverable(ADV_IND,                                       /*< Advertise as connectable, undirected. */
                                     CFG_FAST_CONN_ADV_INTERVAL_MIN,                /*< Set the advertising interval min value. */
                                     CFG_FAST_CONN_ADV_INTERVAL_MAX,                /*< Set the advertising interval max value. */
                                     GAP_PUBLIC_ADDR,                                   /*< Use the public address. */
                                     NO_WHITE_LIST_USE,                             /*< No white list. */
                                     sizeof(ad_local_name), (uint8_t*)ad_local_name,/*< Use a local name. */
                                     0, NULL,                                       /*< Do not include the service UUID list. (no adopted services) */
                                     0x0000, 0x0000);                               /*< NaN, do not put in advertising data. */
      if (ret != BLE_STATUS_SUCCESS)
      {
        Error_Handler(); /* UNEXPECTED */
      }
      
      /**
      * Update the advertising data.
      */
      ret = aci_gap_update_adv_data(sizeof(ad_manufacturer_specific_data), (uint8_t*)ad_manufacturer_specific_data);
      if (ret != BLE_STATUS_SUCCESS)
      {
        Error_Handler(); /* UNEXPECTED */
      }
      APP_FLAG_SET(APP_FLAG_BLE_ADVERTISING);
    }
  }
  else {
    /**
     * Stop device advertising.
     */
    ret = aci_gap_set_non_discoverable();
    if (ret != BLE_STATUS_SUCCESS)
    {
      Error_Handler(); /* UNEXPECTED */
    }
    APP_FLAG_RESET(APP_FLAG_BLE_ADVERTISING);
  }

  return;
}

/**
* @brief This function initializes the BLE stack and all its modules
* @param None
* @retval None
*/
static void Ble_Hci_Gap_Gatt_Init(void)
{
  uint16_t gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
  const uint8_t *bd_address;
  uint32_t srd_bd_address[2];
  tBleStatus ret = BLE_STATUS_SUCCESS;
  
  /**
  * BLE HCI Reset to synchronize BLE Stack
  */
  hci_reset();
  
  /**
  * Write the BD Address
  */
  bd_address = Ble_GetBdAddress();
  aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                            CONFIG_DATA_PUBADDR_LEN,
                            (uint8_t*) bd_address);
  
  /**
  * Put the BD address in the manufacturer specific advertising data (for iOS devices)
  */
  ad_manufacturer_specific_data[sizeof(ad_manufacturer_specific_data)-6] = bd_address[5];
  ad_manufacturer_specific_data[sizeof(ad_manufacturer_specific_data)-5] = bd_address[4];
  ad_manufacturer_specific_data[sizeof(ad_manufacturer_specific_data)-4] = bd_address[3];
  ad_manufacturer_specific_data[sizeof(ad_manufacturer_specific_data)-3] = bd_address[2];
  ad_manufacturer_specific_data[sizeof(ad_manufacturer_specific_data)-2] = bd_address[1];
  ad_manufacturer_specific_data[sizeof(ad_manufacturer_specific_data)-1] = bd_address[0];
  
  /**
  * Static random Address
  * The two upper bits shall be set to 1
  * The lowest 32bits is read from the UDN to differentiate between devices
  * The RNG may be used to provide a random number on each power on
  */
  srd_bd_address[1] =  0x0000ED6E;
  srd_bd_address[0] =  LL_FLASH_GetUDN( );
  ret = aci_hal_write_config_data(CONFIG_DATA_RANDOM_ADDRESS_OFFSET,
                                  CONFIG_DATA_RANDOM_ADDRESS_LEN,
                                  (uint8_t*)srd_bd_address);
  
  /**
  * Set TX Power.
  */
  ret = aci_hal_set_tx_power_level(0, CFG_TX_POWER);
  if (ret != BLE_STATUS_SUCCESS)
  {
    Error_Handler(); /* UNEXPECTED */
  }
  
  /**
  * Set Radio activity event mask.
  */
  ret = aci_hal_set_radio_activity_mask(CFG_RADIO_ACTIVITY_EVENT_MASK);
  if (ret != BLE_STATUS_SUCCESS)
  {
    Error_Handler(); /* UNEXPECTED */
  }
  
  /**
  * Initialize GATT
  */
  ret = aci_gatt_init();
  if (ret != BLE_STATUS_SUCCESS)
  {
    Error_Handler(); /* UNEXPECTED */
  }
  
  /**
  * Initialize GAP
  */
  ret = aci_gap_init(GAP_PERIPHERAL_ROLE,
                     PRIVACY_DISABLED,
                     APP_BLE_GAP_DEVICE_NAME_LENGTH,
                     &gap_service_handle,
                     &gap_dev_name_char_handle,
                     &gap_appearance_char_handle);
  if (ret != BLE_STATUS_SUCCESS)
  {
    Error_Handler(); /* UNEXPECTED */
  }
  
  /**
  * Update GAP Service Device Name characteristic value
  */
  ret = aci_gatt_update_char_value(gap_service_handle,
                                   gap_dev_name_char_handle,
                                   0,
                                   sizeof(gap_device_name),
                                   (uint8_t *)gap_device_name);
  if (ret != BLE_STATUS_SUCCESS)
  {
    Error_Handler(); /* UNEXPECTED */
  }
  
  /**
  * Update GAP Service Appearance  characteristic value
  */
  ret = aci_gatt_update_char_value(gap_service_handle,
                                gap_appearance_char_handle,
                                0,
                                sizeof(gap_appearance),
                                (uint8_t *)&gap_appearance);
  if (ret != BLE_STATUS_SUCCESS)
  {
    Error_Handler(); /* UNEXPECTED */
  }
  
  return;
}

/**
* @brief This function generates the unique BD address from the UDN
* @param None
* @retval Pointer to the array holding the BD address
*/
const uint8_t* Ble_GetBdAddress(void)
{
  uint8_t *p_otp_addr;
  const uint8_t *p_bd_addr;
  uint32_t udn;
  uint32_t company_id;
  uint32_t device_id;

  udn = LL_FLASH_GetUDN();

  if (udn != 0xFFFFFFFF)
  {
    company_id = LL_FLASH_GetSTCompanyID();
    device_id = LL_FLASH_GetDeviceID();

    /**
     * Public Address with the ST company ID
     * bit[47:24] : 24bits (OUI) equal to the company ID
     * bit[23:16] : Device ID.
     * bit[15:0] : The last 16bits from the UDN
     * Note: In order to use the Public Address in a final product, a dedicated
     * 24bits company ID (OUI) shall be bought.
     */
    bd_address_udn[0] = (uint8_t)(udn & 0x000000FF);
    bd_address_udn[1] = (uint8_t)((udn & 0x0000FF00) >> 8);
    bd_address_udn[2] = (uint8_t)device_id;
    bd_address_udn[3] = (uint8_t)(company_id & 0x000000FF);
    bd_address_udn[4] = (uint8_t)((company_id & 0x0000FF00) >> 8);
    bd_address_udn[5] = (uint8_t)((company_id & 0x00FF0000) >> 16);

    p_bd_addr = (const uint8_t *)bd_address_udn;
  }
  else
  {
    p_otp_addr = OTP_Read(0);
    if (p_otp_addr)
    {
      p_bd_addr = ((OTP_ID0_t*)p_otp_addr)->bd_address;
    }
    else
    {
      p_bd_addr = a_MBdAddr;
    }
  }

  return p_bd_addr;
}

/**
* @brief  Interrupt service routine that must be called when the system channel
*         reports a packet has been received
*         As stated in AN5289, this API notifies the user that a system user event has been received.
*         The user has to call the shci_user_evt_proc() to process
*         the notification in the system transport layer.
*         As the shci_notify_asynch_evt() notification is called from the IPCC
*         Interrupt Service Routine, it is strongly recommended to implement
*         a background mechanism to call shci_user_evt_proc()
*         (out of IPCC Interrupt Service Routine).
* @param  pdata: Pointer to the packet or event data
* @retval None
*/
void shci_notify_asynch_evt(void* pdata)
{
  APP_FLAG_SET(APP_FLAG_SHCI_EVENT_PENDING);
  return;
}

/**
* @brief As stated in AN5289, this is the system event user callback. It is
*        registered and passed as argument to shci_init() function.
*        This reports the received system user event.
*        The buffer holding the received event is freed on return
*        of this function.
* @param pData: pointer to a structure of tSHCI_UserEvtRxParam type
*
*               typedef struct
*               {
*                 SHCI_TL_UserEventFlowStatus_t status;
*                 TL_EvtPacket_t *pckt;
*               } tSHCI_UserEvtRxParam;
*
*               pckt: holds the address of the received event
*               status: provides a way for user to notify the system transport layer that the received packet
*                       has not been processed and must not be thrown away. When not filled by the user on return
*                       of UserEvtRx(), this parameter is set to SHCI_TL_UserEventFlow_Enable, which means the
*                       user has processed the received event
* @retval None
*/
static void SYS_UserEventReceivedCallback( void * pData )
{
  TL_AsynchEvt_t *p_sys_event;
  SHCI_C2_Ready_Evt_t *p_sys_ready_event;
  SCHI_SystemErrCode_t *p_sys_error_code;

  p_sys_event = (TL_AsynchEvt_t*)(((tSHCI_UserEvtRxParam*)pData)->pckt->evtserial.evt.payload);
  
  /* We have received some event from CPU2, so CPU2 to be considered as running and responding */
  APP_FLAG_SET(APP_FLAG_CPU2_INITIALIZED);
  
  switch(p_sys_event->subevtcode)
  {
  case SHCI_SUB_EVT_CODE_READY:
    p_sys_ready_event = (SHCI_C2_Ready_Evt_t*)p_sys_event->payload;
    if (p_sys_ready_event->sysevt_ready_rsp == WIRELESS_FW_RUNNING)
    {
      APP_FLAG_RESET(APP_FLAG_FUS_FW_RUNNING);
      APP_FLAG_SET(APP_FLAG_WIRELESS_FW_RUNNING);
      /* RF stack installed and ready */
    }
    else if (p_sys_ready_event->sysevt_ready_rsp == FUS_FW_RUNNING)
    {
      APP_FLAG_SET(APP_FLAG_FUS_FW_RUNNING);
      APP_FLAG_RESET(APP_FLAG_WIRELESS_FW_RUNNING);
      
      /* No RF stack installed most probably */
      Error_Handler(); /* UNEXPECTED */
    }
    else {
      APP_FLAG_SET(APP_FLAG_CPU2_ERROR);
      Error_Handler(); /* UNEXPECTED */
    }
    break; /* SHCI_SUB_EVT_CODE_READY */
  case SHCI_SUB_EVT_ERROR_NOTIF:
    APP_FLAG_SET(APP_FLAG_CPU2_ERROR);
    
    p_sys_error_code = (SCHI_SystemErrCode_t*)p_sys_event->payload;
    if (p_sys_error_code == ERR_BLE_INIT)
    {
      /* Error during BLE stack initialization */
      APP_FLAG_SET(APP_FLAG_BLE_INITIALIZATION_ERROR);
      Error_Handler(); /* UNEXPECTED */
    }
    else {
      Error_Handler(); /* UNEXPECTED */
    }
    break; /* SHCI_SUB_EVT_ERROR_NOTIF */
  default:
    break;
  }
  
  ((tSHCI_UserEvtRxParam *)pData)->status = SHCI_TL_UserEventFlow_Disable;
  
  return;
}

/**
* @brief As stated in AN5289, this is the callback used to acknowledge
*        if a system command can be sent. It is registered in shci_init()
*        It must be used in a multi-thread application where the system commands
*        may be sent from different threads.
*  
*        switch (status)
*        {
*        case SHCI_TL_CmdBusy:
*          break;
*        case SHCI_TL_CmdAvailable:
*          break;
*        default:
*          break;
*
* @param status: SHCI_TL_CmdBusy in case the system transport layer is busy and no
*                new system command are be sent, SHCI_TL_CmdAvailable otherwise
* @retval None
*/
static void SYS_StatusNotificationCallback( SHCI_TL_CmdStatus_t status )
{
  /* Callback not implemented - code flow under control of the developer */
  UNUSED(status);
  return;
}

/**
* @brief This function is used to process all events coming from BLE stack by executing the related callback
* @param None
* @retval None
*/
static void SYS_ProcessEvent(void)
{
  if (APP_FLAG_GET(APP_FLAG_SHCI_EVENT_PENDING) == 1)
  {
    APP_FLAG_RESET(APP_FLAG_SHCI_EVENT_PENDING);
    shci_user_evt_proc();
  }
}

/**
* @brief  Callback called from related IPCC RX Interrupt Service Routine, called when the BLE core (CPU2)
*         reports a packet received or an event to the host.
*         As stated in AN5289, this API notifies the user that a BLE user event has been received.
*         The user has to call the hci_user_evt_proc() to process
*         the notification in the BLE transport layer.
*         As the hci_notify_asynch_evt() notification is called from the IPCC
*         Interrupt Service Routine, it is strongly recommended to implement
*         a background mechanism to call hci_user_evt_proc()
*         (out of IPCC Interrupt Service Routine).
* @param  pdata: Pointer to the packet or event data
* @retval None
*/
void hci_notify_asynch_evt(void* pdata)
{
  APP_FLAG_SET(APP_FLAG_HCI_EVENT_PENDING);
  return;
}

/**
* @brief As stated in AN5289, this is the BLE event user callback. It is
*        registered and passed as argument to hci_init() function.
*        This reports the received BLE user event.
*        The buffer holding the received event is freed on return
*        of this function.
* @param pData: pointer to a structure of tHCI_UserEvtRxParam type
*
*               typedef struct
*               {
*                 HCI_TL_UserEventFlowStatus_t status;
*                 TL_EvtPacket_t *pckt;
*               } tHCI_UserEvtRxParam;
*
*               pckt: holds the address of the received event
*               status: provides a way for the user to notify the HCI transport layer that the received packet has not been processed and
*               must not be thrown away. When not filled by the user on return of UserEvtRx(), this parameter is set to HCI_TL_UserEventFlow_Enable
*               which means the user has processed the received event.
* @retval None
*/
static void BLE_UserEventReceivedCallback( void * pData )
{
  SVCCTL_UserEvtFlowStatus_t svctl_return_status;
  tHCI_UserEvtRxParam *pParam;
  
  pParam = (tHCI_UserEvtRxParam *)pData; 
  
  svctl_return_status = SVCCTL_UserEvtRx((void *)&(pParam->pckt->evtserial));
  if (svctl_return_status != SVCCTL_UserEvtFlowDisable)
  {
    pParam->status = HCI_TL_UserEventFlow_Enable;
  }
  else
  {
    pParam->status = HCI_TL_UserEventFlow_Disable;
  }
}

/**
* @brief As stated in AN5289, this is the callback used to acknowledge
*        if a BLE command can be sent. It is registered in hci_init()
*        It must be used in a multi-thread application where the BLE commands
*        may be sent from different threads.
*  
*        switch (status)
*        {
*        case HCI_TL_CmdBusy:
*          break;
*        case HCI_TL_CmdAvailable:
*          break;
*        default:
*          break;
*
* @param status: HCI_TL_CmdBusy in case HCI transport layer is busy and no new
*        BLE command can be sent, HCI_TL_CmdAvailable otherwise
* @retval None
*/
static void BLE_StatusNotificationCallback(HCI_TL_CmdStatus_t status)
{
  /* Callback not implemented - code flow under control of the developer */
  UNUSED(status);
  return;
}

/**
* @brief Read the HSE trimming value from OTP memory
* @param None
* @retval None
*/
static void Tune_HSE(void)
{
/* !!! WARNING !!! Following code is valid only for P-NUCLEO-WB55 boards. 
Code must be reviewed and optionally reimplemented depending on the target HW 
and HSE capacitor tuning value storage location. 
Please read AN5042 - HSE trimming for RF applications using the STM32WB series. */

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

/**
* @brief This callback is triggered when either
*          + a GAP event is received from the BLE core device.
*          + a GATT event that has not been positively acknowledged by the registered handler is received from the
*            BLE core device.
*        The event is returned in a HCI packet. The full HCI packet is stored in a single buffer and is available when
*        this callback is triggered. However, an ACI event may be longer than a HCI packet and could be fragmented over
*        several HCI packets. The HCI layer only handles HCI packets so when an ACI packet is split over several HCI
*        packets, this callback is triggered for each HCI fragment. It is the responsibility of the application to
*        reassemble the ACI event.
*        This callback is triggered in the TL_BLE_HCI_UserEvtProc() context
*
* @param  pckt: The user event received from the BLE core device
* @retval None
*/
SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  evt_le_meta_event *le_meta_evt;
  
  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
  
  switch (event_pckt->evt)
  {
  case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
    APP_FLAG_RESET(APP_FLAG_BLE_CONNECTED);
    /* Start advertising */
    BLE_Advertising(SET);
    break; /* HCI_DISCONNECTION_COMPLETE_EVT_CODE */
  case HCI_LE_META_EVT_CODE:
    le_meta_evt = (evt_le_meta_event *)(event_pckt->data);
    switch (le_meta_evt->subevent)
    {
    case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
      APP_FLAG_RESET(APP_FLAG_BLE_ADVERTISING);
      APP_FLAG_SET(APP_FLAG_BLE_CONNECTED);
      break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */
    default:
      break;
    }
    break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */
  case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
    blecore_evt = (evt_blecore_aci*) event_pckt->data;
    switch (blecore_evt->ecode)
    {
    case EVT_END_OF_RADIO_ACTIVITY:
      BSP_LED_Toggle(LED_GREEN);
      break; /* EVT_END_OF_RADIO_ACTIVITY */
    }
    break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */
    
  default:
    break;
  }
  
  return (SVCCTL_UserEvtFlowEnable);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  
  while(1)
  {
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(250);
  }
  
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
  tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
