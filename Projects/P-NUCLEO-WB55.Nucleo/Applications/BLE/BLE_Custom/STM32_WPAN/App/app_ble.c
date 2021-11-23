/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/app_ble.c
  * Description        : Application file for BLE Middleware.
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
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "app_common.h"

#include "dbg_trace.h"
#include "ble.h"
#include "tl.h"
#include "app_ble.h"

#include "stm32_seq.h"
#include "shci.h"
#include "stm32_lpm.h"
#include "otp.h"
#include "custom_app.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/**
 * security parameters structure
 */
typedef struct _tSecurityParams
{
  /**
   * IO capability of the device
   */
  uint8_t ioCapability;

  /**
   * Authentication requirement of the device
   * Man In the Middle protection required?
   */
  uint8_t mitm_mode;

  /**
   * bonding mode of the device
   */
  uint8_t bonding_mode;

  /**
   * this variable indicates whether to use a fixed pin
   * during the pairing process or a passkey has to be
   * requested to the application during the pairing process
   * 0 implies use fixed pin and 1 implies request for passkey
   */
  uint8_t Use_Fixed_Pin;

  /**
   * minimum encryption key size requirement
   */
  uint8_t encryptionKeySizeMin;

  /**
   * maximum encryption key size requirement
   */
  uint8_t encryptionKeySizeMax;

  /**
   * fixed pin to be used in the pairing process if
   * Use_Fixed_Pin is set to 1
   */
  uint32_t Fixed_Pin;

  /**
   * this flag indicates whether the host has to initiate
   * the security, wait for pairing or does not have any security
   * requirements.\n
   * 0x00 : no security required
   * 0x01 : host should initiate security by sending the slave security
   *        request command
   * 0x02 : host need not send the clave security request but it
   * has to wait for paiirng to complete before doing any other
   * processing
   */
  uint8_t initiateSecurity;
}tSecurityParams;

/**
 * global context
 * contains the variables common to all
 * services
 */
typedef struct _tBLEProfileGlobalContext
{

  /**
   * security requirements of the host
   */
  tSecurityParams bleSecurityParam;

  /**
   * gap service handle
   */
  uint16_t gapServiceHandle;

  /**
   * device name characteristic handle
   */
  uint16_t devNameCharHandle;

  /**
   * appearance characteristic handle
   */
  uint16_t appearanceCharHandle;

  /**
   * connection handle of the current active connection
   * When not in connection, the handle is set to 0xFFFF
   */
  uint16_t connectionHandle;

  /**
   * length of the UUID list to be used while advertising
   */
  uint8_t advtServUUIDlen;

  /**
   * the UUID list to be used while advertising
   */
  uint8_t advtServUUID[100];

}BleGlobalContext_t;

typedef struct
{
  BleGlobalContext_t BleApplicationContext_legacy;
  APP_BLE_ConnStatus_t Device_Connection_Status;

}BleApplicationContext_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define FAST_ADV_TIMEOUT               (30*1000*1000/CFG_TS_TICK_VAL) /**< 30s */
#define INITIAL_ADV_TIMEOUT            (60*1000*1000/CFG_TS_TICK_VAL) /**< 60s */

#define BD_ADDR_SIZE_LOCAL    6

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;

static const uint8_t M_bd_addr[BD_ADDR_SIZE_LOCAL] =
    {
        (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000000000FF)),
        (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00000000FF00) >> 8),
        (uint8_t)((CFG_ADV_BD_ADDRESS & 0x000000FF0000) >> 16),
        (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000FF000000) >> 24),
        (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00FF00000000) >> 32),
        (uint8_t)((CFG_ADV_BD_ADDRESS & 0xFF0000000000) >> 40)
    };

static uint8_t bd_addr_udn[BD_ADDR_SIZE_LOCAL];

/**
*   Identity root key used to derive LTK and CSRK
*/
static const uint8_t BLE_CFG_IR_VALUE[16] = CFG_BLE_IRK;

/**
* Encryption root key used to derive LTK and CSRK
*/
static const uint8_t BLE_CFG_ER_VALUE[16] = CFG_BLE_ERK;

/**
 * These are the two tags used to manage a power failure during OTA
 * The MagicKeywordAdress shall be mapped @0x140 from start of the binary image
 * The MagicKeywordvalue is checked in the ble_ota application
 */
PLACE_IN_SECTION("TAG_OTA_END") const uint32_t MagicKeywordValue = 0x94448A29 ;
PLACE_IN_SECTION("TAG_OTA_START") const uint32_t MagicKeywordAddress = (uint32_t)&MagicKeywordValue;

PLACE_IN_SECTION("BLE_APP_CONTEXT") static BleApplicationContext_t BleApplicationContext;

Custom_App_ConnHandle_Not_evt_t handleNotification;

#if L2CAP_REQUEST_NEW_CONN_PARAM != 0
#define SIZE_TAB_CONN_INT            2
float tab_conn_interval[SIZE_TAB_CONN_INT] = {50, 1000} ; /* ms */
uint8_t index_con_int, mutex;
#endif

/**
 * Advertising Data
 */
uint8_t ad_data[19] = {
    2, AD_TYPE_TX_POWER_LEVEL, 0 /* 0dBm */, /* Transmission Power */
    6, AD_TYPE_COMPLETE_LOCAL_NAME, 'M', 'y', 'C', 'S', 'T',  /* Complete name */
    3, AD_TYPE_16_BIT_SERV_UUID_CMPLT_LIST, 0x0D, 0x18,
    4, AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x30, 0x00, 0x00 /*  */,
};

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * pPayload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress( void );
static void Adv_Request( APP_BLE_ConnStatus_t New_Status );
static void Adv_Cancel( void );
#if(L2CAP_REQUEST_NEW_CONN_PARAM != 0)
static void BLE_SVC_L2CAP_Conn_Update(uint16_t Connection_Handle);
static void Connection_Interval_Update_Req( void );
#endif

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Init( void )
{
/* USER CODE BEGIN APP_BLE_Init_1 */

/* USER CODE END APP_BLE_Init_1 */
  SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet =
  {
    {{0,0,0}},                          /**< Header unused */
    {0,                                 /** pBleBufferAddress not used */
     0,                                 /** BleBufferSize not used */
     CFG_BLE_NUM_GATT_ATTRIBUTES,
     CFG_BLE_NUM_GATT_SERVICES,
     CFG_BLE_ATT_VALUE_ARRAY_SIZE,
     CFG_BLE_NUM_LINK,
     CFG_BLE_DATA_LENGTH_EXTENSION,
     CFG_BLE_PREPARE_WRITE_LIST_SIZE,
     CFG_BLE_MBLOCK_COUNT,
     CFG_BLE_MAX_ATT_MTU,
     CFG_BLE_SLAVE_SCA,
     CFG_BLE_MASTER_SCA,
     CFG_BLE_LSE_SOURCE,
     CFG_BLE_MAX_CONN_EVENT_LENGTH,
     CFG_BLE_HSE_STARTUP_TIME,
     CFG_BLE_VITERBI_MODE,
     CFG_BLE_OPTIONS,
     0,
     CFG_BLE_MAX_COC_INITIATOR_NBR,
     CFG_BLE_MIN_TX_POWER,
     CFG_BLE_MAX_TX_POWER,
     CFG_BLE_RX_MODEL_CONFIG}
  };

  /**
   * Initialize Ble Transport Layer
   */
  Ble_Tl_Init( );

  /**
   * Do not allow standby in the application
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE, UTIL_LPM_DISABLE);

  /**
   * Register the hci transport layer to handle BLE User Asynchronous Events
   */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HCI_ASYNCH_EVT_ID, UTIL_SEQ_RFU, hci_user_evt_proc);

  /**
   * Starts the BLE Stack on CPU2
   */
  if (SHCI_C2_BLE_Init( &ble_init_cmd_packet ) != SHCI_Success)
  {
    Error_Handler();
  }

  /**
   * Initialization of HCI & GATT & GAP layer
   */
  Ble_Hci_Gap_Gatt_Init();

  /**
   * Initialization of the BLE Services
   */
  SVCCTL_Init();

  /**
   * Initialization of the BLE App Context
   */
  BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
  BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0xFFFF;
  /**
   * From here, all initialization are BLE application specific
   */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_ADV_CANCEL_ID, UTIL_SEQ_RFU, Adv_Cancel);
  /**
   * Initialization of ADV - Ad Manufacturer Element - Support OTA Bit Mask
   */
#if(RADIO_ACTIVITY_EVENT != 0)
  aci_hal_set_radio_activity_mask(0x0006);
#endif

#if (L2CAP_REQUEST_NEW_CONN_PARAM != 0 )
  index_con_int = 0;
  mutex = 1;
#endif
  /**
   * Initialize Custom Template Application
   */
  Custom_APP_Init();

/* USER CODE BEGIN APP_BLE_Init_3 */

/* USER CODE END APP_BLE_Init_3 */

  /**
   * Make device discoverable
   */
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = NULL;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 0;

  /**
   * Start to Advertise to be connected by a Client
   */
   Adv_Request(APP_BLE_FAST_ADV);

/* USER CODE BEGIN APP_BLE_Init_2 */

/* USER CODE END APP_BLE_Init_2 */
  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  evt_blecore_aci *blecore_evt;

  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;

  /* PAIRING */
  aci_gap_numeric_comparison_value_event_rp0 *evt_numeric_value;
  aci_gap_pairing_complete_event_rp0 *pairing_complete;
  uint32_t numeric_value;
  /* PAIRING */

  /* USER CODE BEGIN SVCCTL_App_Notification */

  /* USER CODE END SVCCTL_App_Notification */

  switch (event_pckt->evt)
  {
    case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
    {
      hci_disconnection_complete_event_rp0 *disconnection_complete_event;
      disconnection_complete_event = (hci_disconnection_complete_event_rp0 *) event_pckt->data;

      if (disconnection_complete_event->Connection_Handle == BleApplicationContext.BleApplicationContext_legacy.connectionHandle)
      {
        BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0;
        BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;

        APP_DBG_MSG("\r\n\r** DISCONNECTION EVENT WITH CLIENT \n");
      }

      /* restart advertising */
      Adv_Request(APP_BLE_FAST_ADV);

      /**
       * SPECIFIC to Custom Template APP
       */
      handleNotification.Custom_Evt_Opcode = CUSTOM_DISCON_HANDLE_EVT;
      handleNotification.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
      Custom_APP_Notification(&handleNotification);
      /* USER CODE BEGIN EVT_DISCONN_COMPLETE */

      /* USER CODE END EVT_DISCONN_COMPLETE */
    }

    break; /* HCI_DISCONNECTION_COMPLETE_EVT_CODE */

    case HCI_LE_META_EVT_CODE:
    {
      meta_evt = (evt_le_meta_event*) event_pckt->data;
      /* USER CODE BEGIN EVT_LE_META_EVENT */

      /* USER CODE END EVT_LE_META_EVENT */
      switch (meta_evt->subevent)
      {
        case HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE:
          APP_DBG_MSG("\r\n\r** CONNECTION UPDATE EVENT WITH CLIENT \n");

          /* USER CODE BEGIN EVT_LE_CONN_UPDATE_COMPLETE */

          /* USER CODE END EVT_LE_CONN_UPDATE_COMPLETE */
          break;
        case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
        {
          hci_le_connection_complete_event_rp0 *connection_complete_event;

          /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */
          connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;

          APP_DBG_MSG("HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE for connection handle 0x%x\n", connection_complete_event->Connection_Handle);
          if (BleApplicationContext.Device_Connection_Status == APP_BLE_LP_CONNECTING)
          {
            /* Connection as client */
            BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_CLIENT;
          }
          else
          {
            /* Connection as server */
            BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_SERVER;
          }
          BleApplicationContext.BleApplicationContext_legacy.connectionHandle = connection_complete_event->Connection_Handle;
          /**
           * SPECIFIC to Custom Template APP
           */
          handleNotification.Custom_Evt_Opcode = CUSTOM_CONN_HANDLE_EVT;
          handleNotification.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
          Custom_APP_Notification(&handleNotification);
          /* USER CODE BEGIN HCI_EVT_LE_CONN_COMPLETE */

          /* USER CODE END HCI_EVT_LE_CONN_COMPLETE */
        }
        break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */

        /* USER CODE BEGIN META_EVT */

        /* USER CODE END META_EVT */

        default:
          /* USER CODE BEGIN SUBEVENT_DEFAULT */

          /* USER CODE END SUBEVENT_DEFAULT */
          break;
      }
    }
    break; /* HCI_LE_META_EVT_CODE */

    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*) event_pckt->data;
      /* USER CODE BEGIN EVT_VENDOR */

      /* USER CODE END EVT_VENDOR */
      switch (blecore_evt->ecode)
      {
      /* USER CODE BEGIN ecode */

      /* USER CODE END ecode */
      /**
       * SPECIFIC to Custom Template APP
       */
        case ACI_L2CAP_CONNECTION_UPDATE_RESP_VSEVT_CODE:
#if (L2CAP_REQUEST_NEW_CONN_PARAM != 0 )
          mutex = 1;
#endif
      /* USER CODE BEGIN EVT_BLUE_L2CAP_CONNECTION_UPDATE_RESP */

      /* USER CODE END EVT_BLUE_L2CAP_CONNECTION_UPDATE_RESP */
      break;
        case ACI_GAP_PROC_COMPLETE_VSEVT_CODE:
        APP_DBG_MSG("\r\n\r** ACI_GAP_PROC_COMPLETE_VSEVT_CODE \n");
        /* USER CODE BEGIN EVT_BLUE_GAP_PROCEDURE_COMPLETE */

        /* USER CODE END EVT_BLUE_GAP_PROCEDURE_COMPLETE */
          break; /* ACI_GAP_PROC_COMPLETE_VSEVT_CODE */
#if(RADIO_ACTIVITY_EVENT != 0)
        case ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE:
        /* USER CODE BEGIN RADIO_ACTIVITY_EVENT*/

        /* USER CODE END RADIO_ACTIVITY_EVENT*/
          break; /* ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE */
#endif

        /* PAIRING */
        case (ACI_GAP_KEYPRESS_NOTIFICATION_VSEVT_CODE):
         APP_DBG_MSG("\r\n\r** ACI_GAP_KEYPRESS_NOTIFICATION_VSEVT_CODE \n");
        break;

        case ACI_GAP_PASS_KEY_REQ_VSEVT_CODE:
            aci_gap_pass_key_resp(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, CFG_FIXED_PIN);
        break;

        case ACI_GAP_NUMERIC_COMPARISON_VALUE_VSEVT_CODE:
            evt_numeric_value = (aci_gap_numeric_comparison_value_event_rp0 *)blecore_evt->data;
            numeric_value = evt_numeric_value->Numeric_Value;
            APP_DBG_MSG("numeric_value = %ld\n", numeric_value);
            aci_gap_numeric_comparison_value_confirm_yesno(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, YES);
        break;

        case ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE:
            pairing_complete = (aci_gap_pairing_complete_event_rp0*)blecore_evt->data;
            APP_DBG_MSG("BLE_CTRL_App_Notification: ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE, pairing_complete->Status = %d\n",pairing_complete->Status);
        break;
        /* PAIRING */

      /* USER CODE BEGIN BLUE_EVT */

      /* USER CODE END BLUE_EVT */
      }
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT */

      /* USER CODE END EVENT_PCKT */

      default:
      /* USER CODE BEGIN ECODE_DEFAULT*/

      /* USER CODE END ECODE_DEFAULT*/
      break;
  }

  return (SVCCTL_UserEvtFlowEnable);
}

APP_BLE_ConnStatus_t APP_BLE_Get_Server_Connection_Status(void)
{
    return BleApplicationContext.Device_Connection_Status;
}

/* USER CODE BEGIN FD*/

/* USER CODE END FD*/
/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void Ble_Tl_Init( void )
{
  HCI_TL_HciInitConf_t Hci_Tl_Init_Conf;

  Hci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&BleCmdBuffer;
  Hci_Tl_Init_Conf.StatusNotCallBack = BLE_StatusNot;
  hci_init(BLE_UserEvtRx, (void*) &Hci_Tl_Init_Conf);

  return;
}

static void Ble_Hci_Gap_Gatt_Init(void){

  uint8_t role;
  uint16_t gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
  const uint8_t *bd_addr;
  uint16_t appearance[1] = { BLE_CFG_GAP_APPEARANCE };

  /**
   * Initialize HCI layer
   */
  /*HCI Reset to synchronise BLE Stack*/
  hci_reset();

  /**
   * Write the BD Address
   */

  bd_addr = BleGetBdAddress();
  aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                            CONFIG_DATA_PUBADDR_LEN,
                            (uint8_t*) bd_addr);

  /**
   * Static random Address
   * The two upper bits shall be set to 1
   * The lowest 32bits is read from the UDN to differentiate between devices
   * The RNG may be used to provide a random number on each power on
   */
#if defined(CFG_STATIC_RANDOM_ADDRESS)
  srd_bd_addr[0] = CFG_STATIC_RANDOM_ADDRESS & 0xFFFFFFFF;
  srd_bd_addr[1] = (uint32_t)((uint64_t)CFG_STATIC_RANDOM_ADDRESS >> 32);
  srd_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */
#elif (CFG_BLE_ADDRESS_TYPE == RANDOM_ADDR)
  /* Get RNG semaphore */
  while( LL_HSEM_1StepLock( HSEM, CFG_HW_RNG_SEMID ) );

  /* Enable RNG */
  __HAL_RNG_ENABLE(&hrng);

  /* Enable HSI48 oscillator */
  LL_RCC_HSI48_Enable();
  /* Wait until HSI48 is ready */
  while( ! LL_RCC_HSI48_IsReady( ) );

  if (HAL_RNG_GenerateRandomNumber(&hrng, &srd_bd_addr[1]) != HAL_OK)
  {
    /* Random number generation error */
    Error_Handler();
  }
  if (HAL_RNG_GenerateRandomNumber(&hrng, &srd_bd_addr[0]) != HAL_OK)
  {
    /* Random number generation error */
    Error_Handler();
  }
  srd_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */

  /* Disable HSI48 oscillator */
  LL_RCC_HSI48_Disable();

  /* Disable RNG */
  __HAL_RNG_DISABLE(&hrng);

  /* Release RNG semaphore */
  LL_HSEM_ReleaseLock( HSEM, CFG_HW_RNG_SEMID, 0 );
#endif

#if (CFG_BLE_ADDRESS_TYPE == STATIC_RANDOM_ADDR)
  aci_hal_write_config_data( CONFIG_DATA_RANDOM_ADDRESS_OFFSET, CONFIG_DATA_RANDOM_ADDRESS_LEN, (uint8_t*)srd_bd_addr );
#endif

  /**
   * Write Identity root key used to derive LTK and CSRK
   */
  aci_hal_write_config_data( CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, (uint8_t*)BLE_CFG_IR_VALUE );

  /**
   * Write Encryption root key used to derive LTK and CSRK
   */
  aci_hal_write_config_data( CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, (uint8_t*)BLE_CFG_ER_VALUE );

  /**
   * Set TX Power to 0dBm.
   */
  aci_hal_set_tx_power_level(1, CFG_TX_POWER);

  /**
   * Initialize GATT interface
   */
  aci_gatt_init();

  /**
   * Initialize GAP interface
   */
  role = 0;

#if (BLE_CFG_PERIPHERAL == 1)
  role |= GAP_PERIPHERAL_ROLE;
#endif

#if (BLE_CFG_CENTRAL == 1)
  role |= GAP_CENTRAL_ROLE;
#endif

  if (role > 0)
  {
    const char *name = CFG_GAP_DEVICE_NAME;
    aci_gap_init(role,
#if ((CFG_BLE_ADDRESS_TYPE == RESOLVABLE_PRIVATE_ADDR) || (CFG_BLE_ADDRESS_TYPE == NON_RESOLVABLE_PRIVATE_ADDR))
                 2,
#else
                 0,
#endif
                 CFG_GAP_DEVICE_NAME_LENGTH,
                 &gap_service_handle,
                 &gap_dev_name_char_handle,
                 &gap_appearance_char_handle);

    if (aci_gatt_update_char_value(gap_service_handle, gap_dev_name_char_handle, 0, strlen(name), (uint8_t *) name))
    {
      BLE_DBG_SVCCTL_MSG("Device Name aci_gatt_update_char_value failed.\n");
    }
  }

  if(aci_gatt_update_char_value(gap_service_handle,
                                gap_appearance_char_handle,
                                0,
                                2,
                                (uint8_t *)&appearance))
  {
    BLE_DBG_SVCCTL_MSG("Appearance aci_gatt_update_char_value failed.\n");
  }
  /**
   * Initialize Default PHY
   */
  hci_le_set_default_phy(ALL_PHYS_PREFERENCE,TX_2M_PREFERRED,RX_2M_PREFERRED);

  /**
   * Initialize IO capability
   */
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability = CFG_IO_CAPABILITY;
  aci_gap_set_io_capability(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability);

  /**
   * Initialize authentication
   */
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode = CFG_MITM_PROTECTION;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = CFG_ENCRYPTION_KEY_SIZE_MIN;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = CFG_ENCRYPTION_KEY_SIZE_MAX;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin = CFG_USED_FIXED_PIN;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin = CFG_FIXED_PIN;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode = CFG_BONDING_MODE;

  aci_gap_set_authentication_requirement(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode,
                                         CFG_SC_SUPPORT,
                                         CFG_KEYPRESS_NOTIFICATION_SUPPORT,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin,
                                         CFG_BLE_ADDRESS_TYPE
                                         );

  /**
   * Initialize whitelist
   */
   if (BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode)
   {
     aci_gap_configure_whitelist();
   }
}

static void Adv_Request(APP_BLE_ConnStatus_t New_Status)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;

    BleApplicationContext.Device_Connection_Status = New_Status;
    /* Start Fast or Low Power Advertising */
    ret = aci_gap_set_discoverable(
        ADV_TYPE,
        CFG_FAST_CONN_ADV_INTERVAL_MIN,
        CFG_FAST_CONN_ADV_INTERVAL_MAX,
        CFG_BLE_ADDRESS_TYPE,
        ADV_FILTER,
        0,
        0,
        0,
        0,
        0,
        0);

    /* Update Advertising data */
    ret = aci_gap_update_adv_data(sizeof(ad_data), (uint8_t*) ad_data);

    if (ret == BLE_STATUS_SUCCESS)
    {
        APP_DBG_MSG("Successfully Start Fast Advertising \n" );
    }
    else
    {
        APP_DBG_MSG("Start Fast Advertising Failed , result: %d \n", ret);
    }

  return;
}

const uint8_t* BleGetBdAddress( void )
{
  uint8_t *otp_addr;
  const uint8_t *bd_addr;
  uint32_t udn;
  uint32_t company_id;
  uint32_t device_id;

  udn = LL_FLASH_GetUDN();

  if(udn != 0xFFFFFFFF)
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
    bd_addr_udn[0] = (uint8_t)(udn & 0x000000FF);
    bd_addr_udn[1] = (uint8_t)( (udn & 0x0000FF00) >> 8 );
    bd_addr_udn[2] = (uint8_t)device_id;
    bd_addr_udn[3] = (uint8_t)(company_id & 0x000000FF);
    bd_addr_udn[4] = (uint8_t)( (company_id & 0x0000FF00) >> 8 );
    bd_addr_udn[5] = (uint8_t)( (company_id & 0x00FF0000) >> 16 );

    bd_addr = (const uint8_t *)bd_addr_udn;
  }
  else
  {
    otp_addr = OTP_Read(0);
    if(otp_addr)
    {
      bd_addr = ((OTP_ID0_t*)otp_addr)->bd_address;
    }
    else
    {
      bd_addr = M_bd_addr;
    }
  }

  return bd_addr;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTION */

/* USER CODE END FD_LOCAL_FUNCTION */

/*************************************************************
 *
 *SPECIFIC FUNCTIONS FOR CUSTOM
 *
 *************************************************************/
static void Adv_Cancel( void )
{
/* USER CODE BEGIN Adv_Cancel_1 */

/* USER CODE END Adv_Cancel_1 */

  if (BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_SERVER)

  {

    tBleStatus result = 0x00;

    result = aci_gap_set_non_discoverable();

    BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  \r\n\r");APP_DBG_MSG("** STOP ADVERTISING **  \r\n\r");
    }
    else
    {
      APP_DBG_MSG("** STOP ADVERTISING **  Failed \r\n\r");
    }

  }

/* USER CODE BEGIN Adv_Cancel_2 */

/* USER CODE END Adv_Cancel_2 */
  return;
}

#if(L2CAP_REQUEST_NEW_CONN_PARAM != 0)
void BLE_SVC_L2CAP_Conn_Update(uint16_t Connection_Handle)
{
/* USER CODE BEGIN BLE_SVC_L2CAP_Conn_Update_1 */

/* USER CODE END BLE_SVC_L2CAP_Conn_Update_1 */
  if(mutex == 1) {
    mutex = 0;
    index_con_int = (index_con_int + 1)%SIZE_TAB_CONN_INT;
    uint16_t interval_min = CONN_P(tab_conn_interval[index_con_int]);
    uint16_t interval_max = CONN_P(tab_conn_interval[index_con_int]);
    uint16_t slave_latency = L2CAP_SLAVE_LATENCY;
    uint16_t timeout_multiplier = L2CAP_TIMEOUT_MULTIPLIER;
    tBleStatus result;

    result = aci_l2cap_connection_parameter_update_req(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,
                                                       interval_min, interval_max,
                                                       slave_latency, timeout_multiplier);
    if( result == BLE_STATUS_SUCCESS )
    {
      APP_DBG_MSG("BLE_SVC_L2CAP_Conn_Update(), Successfully \r\n\r");
    }
    else
    {
      APP_DBG_MSG("BLE_SVC_L2CAP_Conn_Update(), Failed \r\n\r");
    }
  }
/* USER CODE BEGIN BLE_SVC_L2CAP_Conn_Update_2 */

/* USER CODE END BLE_SVC_L2CAP_Conn_Update_2 */
  return;
}
#endif

#if (L2CAP_REQUEST_NEW_CONN_PARAM != 0 )
static void Connection_Interval_Update_Req( void )
{
  if (BleApplicationContext.Device_Connection_Status != APP_BLE_FAST_ADV && BleApplicationContext.Device_Connection_Status != APP_BLE_IDLE)
  {
    BLE_SVC_L2CAP_Conn_Update(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
  }
  return;
}
#endif

/* USER CODE BEGIN FD_SPECIFIC_FUNCTIONS */

/* USER CODE END FD_SPECIFIC_FUNCTIONS */
/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void hci_notify_asynch_evt(void* pdata)
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
  return;
}

void hci_cmd_resp_release(uint32_t flag)
{
  UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
  return;
}

void hci_cmd_resp_wait(uint32_t timeout)
{
  UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
  return;
}

static void BLE_UserEvtRx( void * pPayload )
{
  SVCCTL_UserEvtFlowStatus_t svctl_return_status;
  tHCI_UserEvtRxParam *pParam;

  pParam = (tHCI_UserEvtRxParam *)pPayload;

  svctl_return_status = SVCCTL_UserEvtRx((void *)&(pParam->pckt->evtserial));
  if (svctl_return_status != SVCCTL_UserEvtFlowDisable)
  {
    pParam->status = HCI_TL_UserEventFlow_Enable;
  }
  else
  {
    pParam->status = HCI_TL_UserEventFlow_Disable;
  }

  return;
}

static void BLE_StatusNot( HCI_TL_CmdStatus_t status )
{
  uint32_t task_id_list;
  switch (status)
  {
    case HCI_TL_CmdBusy:
      /**
       * All tasks that may send an aci/hci commands shall be listed here
       * This is to prevent a new command is sent while one is already pending
       */
      task_id_list = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
      UTIL_SEQ_PauseTask(task_id_list);

      break;

    case HCI_TL_CmdAvailable:
      /**
       * All tasks that may send an aci/hci commands shall be listed here
       * This is to prevent a new command is sent while one is already pending
       */
      task_id_list = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
      UTIL_SEQ_ResumeTask(task_id_list);

      break;

    default:
      break;
  }
  return;
}

void SVCCTL_ResumeUserEventFlow( void )
{
  hci_resume_flow();
  return;
}

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
