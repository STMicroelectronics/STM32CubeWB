/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_ble.c
 * @author  MCD Application Team
 * @brief   BLE Application
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

#include "p2p_client_app.h"

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
} tSecurityParams;

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

} BleGlobalContext_t;

typedef struct
{
BleGlobalContext_t BleApplicationContext_legacy;
APP_BLE_ConnStatus_t Device_Connection_Status;
uint8_t SwitchOffGPIO_timer_Id;
uint8_t DeviceServerFound;
} BleApplicationContext_t;

#if OOB_DEMO != 0
typedef struct
{
  uint8_t  Identifier;
  uint16_t L2CAP_Length;
  uint16_t Interval_Min;
  uint16_t Interval_Max;
  uint16_t Slave_Latency;
  uint16_t Timeout_Multiplier;
} APP_BLE_p2p_Conn_Update_req_t;
#endif

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define BD_ADDR_SIZE_LOCAL    6

/* USER CODE BEGIN PD */
#if OOB_DEMO != 0 
#define LED_ON_TIMEOUT            (0.005*1000*1000/CFG_TS_TICK_VAL) /**< 5ms */
#endif 
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
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

tBDAddr SERVER_REMOTE_BDADDR;

P2PC_APP_ConnHandle_Not_evt_t handleNotification;

PLACE_IN_SECTION("BLE_APP_CONTEXT") static BleApplicationContext_t BleApplicationContext;

#if OOB_DEMO != 0
APP_BLE_p2p_Conn_Update_req_t APP_BLE_p2p_Conn_Update_req;
#endif

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * pPayload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress( void );
static void Scan_Request( void );
static void Connect_Request( void );
static void Switch_OFF_GPIO( void );

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Init( void )
{
/* USER CODE BEGIN APP_BLE_Init_1 */

/* USER CODE END APP_BLE_Init_1 */
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
    CFG_BLE_MAX_TX_POWER}
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
	/* if you are here may be CPU2 doesn't contain STM32WB_Copro_Wireless_Binaries see Release_Notes.html */
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
   * From here, all initialization are BLE application specific
   */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_START_SCAN_ID, UTIL_SEQ_RFU, Scan_Request);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONN_DEV_1_ID, UTIL_SEQ_RFU, Connect_Request);

  /**
   * Initialization of the BLE App Context
   */

      BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;

    /*Radio mask Activity*/
#if (OOB_DEMO != 0)
  aci_hal_set_radio_activity_mask(0x0020);
#endif
  /**
   * Initialize P2P Client Application
   */
  P2PC_APP_Init();

#if (OOB_DEMO != 0)
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BleApplicationContext.SwitchOffGPIO_timer_Id), hw_ts_SingleShot, Switch_OFF_GPIO);
#endif

#if (OOB_DEMO == 0)
  /**
   * Start scanning
   */
  UTIL_SEQ_SetTask(1 << CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
#endif
/* USER CODE BEGIN APP_BLE_Init_2 */

/* USER CODE END APP_BLE_Init_2 */
  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  hci_le_connection_complete_event_rp0 * connection_complete_event;
  evt_blecore_aci *blecore_evt;
  hci_le_advertising_report_event_rp0 * le_advertising_event;
  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
  hci_disconnection_complete_event_rp0 *cc = (void *) event_pckt->data;
  uint8_t result;
  uint8_t event_type, event_data_size;
  int k = 0;
  uint8_t adtype, adlength;

  switch (event_pckt->evt)
  {
    /* USER CODE BEGIN evt */

    /* USER CODE END evt */
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
    {
      handleNotification.P2P_Evt_Opcode = PEER_DISCON_HANDLE_EVT;
      blecore_evt = (evt_blecore_aci*) event_pckt->data;
      /* USER CODE BEGIN EVT_VENDOR */

      /* USER CODE END EVT_VENDOR */
      switch (blecore_evt->ecode)
      {
      /* USER CODE BEGIN ecode */

      /* USER CODE END ecode */

        case ACI_GAP_PROC_COMPLETE_VSEVT_CODE:
        {
        /* USER CODE BEGIN EVT_BLUE_GAP_PROCEDURE_COMPLETE */

        /* USER CODE END EVT_BLUE_GAP_PROCEDURE_COMPLETE */
          aci_gap_proc_complete_event_rp0 *gap_evt_proc_complete = (void*) blecore_evt->data;
          /* CHECK GAP GENERAL DISCOVERY PROCEDURE COMPLETED & SUCCEED */
          if (gap_evt_proc_complete->Procedure_Code == GAP_GENERAL_DISCOVERY_PROC
              && gap_evt_proc_complete->Status == 0x00)
          {
            /* USER CODE BEGIN GAP_GENERAL_DISCOVERY_PROC */
            BSP_LED_Off(LED_BLUE);
            /* USER CODE END GAP_GENERAL_DISCOVERY_PROC */
            APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE_COMPLETED\n");
            /*if a device found, connect to it, device 1 being chosen first if both found*/
            if (BleApplicationContext.DeviceServerFound == 0x01 && BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
            {
              UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_DEV_1_ID, CFG_SCH_PRIO_0);
            }
          }
        }
        break;
#if (OOB_DEMO != 0)
        case ACI_L2CAP_CONNECTION_UPDATE_REQ_VSEVT_CODE:
        {
          /* USER CODE BEGIN EVT_BLUE_L2CAP_CONNECTION_UPDATE_REQ */

          /* USER CODE END EVT_BLUE_L2CAP_CONNECTION_UPDATE_REQ */
          aci_l2cap_connection_update_req_event_rp0 *pr = (aci_l2cap_connection_update_req_event_rp0 *) blecore_evt->data;
          aci_hal_set_radio_activity_mask(0x0000);
          APP_BLE_p2p_Conn_Update_req.Identifier = pr->Identifier;
          APP_BLE_p2p_Conn_Update_req.L2CAP_Length = pr->L2CAP_Length;
          APP_BLE_p2p_Conn_Update_req.Interval_Min = pr->Interval_Min;
          APP_BLE_p2p_Conn_Update_req.Interval_Max = pr->Interval_Max;
          APP_BLE_p2p_Conn_Update_req.Slave_Latency = pr->Slave_Latency;
          APP_BLE_p2p_Conn_Update_req.Timeout_Multiplier = pr->Timeout_Multiplier;

          result = aci_l2cap_connection_parameter_update_resp(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,
                                                           APP_BLE_p2p_Conn_Update_req.Interval_Min,
                                                           APP_BLE_p2p_Conn_Update_req.Interval_Max,
                                                           APP_BLE_p2p_Conn_Update_req.Slave_Latency,
                                                           APP_BLE_p2p_Conn_Update_req.Timeout_Multiplier,
                                                           CONN_L1,
                                                           CONN_L2,
                                                           APP_BLE_p2p_Conn_Update_req.Identifier,
                                                           0x01);
          if(result != BLE_STATUS_SUCCESS) {
              /* USER CODE BEGIN BLE_STATUS_SUCCESS */
              BSP_LED_On(LED_RED);
              /* USER CODE END BLE_STATUS_SUCCESS */
          }
          aci_hal_set_radio_activity_mask(0x0020);

        }

        break;

        case 0x0004:
        {
          /* USER CODE BEGIN RADIO_ACTIVITY_EVENT */
          BSP_LED_On(LED_GREEN);
          HW_TS_Start(BleApplicationContext.SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
          /* USER CODE END RADIO_ACTIVITY_EVENT */
        }
        break;
#endif

        /* USER CODE BEGIN BLUE_EVT */

        /* USER CODE END BLUE_EVT */

        default:
          /* USER CODE BEGIN ecode_default */

          /* USER CODE END ecode_default */
          break;

      }
    }
    break;

    case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
      {
      /* USER CODE BEGIN EVT_DISCONN_COMPLETE */

      /* USER CODE END EVT_DISCONN_COMPLETE */
      if (cc->Connection_Handle == BleApplicationContext.BleApplicationContext_legacy.connectionHandle)
          {
            BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0;
            BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
            APP_DBG_MSG("\r\n\r** DISCONNECTION EVENT WITH SERVER \n");
            handleNotification.P2P_Evt_Opcode = PEER_DISCON_HANDLE_EVT;
            handleNotification.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
            P2PC_APP_Notification(&handleNotification);
          }
       }
      break; /* HCI_DISCONNECTION_COMPLETE_EVT_CODE */

    case HCI_LE_META_EVT_CODE:
    {
      /* USER CODE BEGIN EVT_LE_META_EVENT */

      /* USER CODE END EVT_LE_META_EVENT */
      meta_evt = (evt_le_meta_event*) event_pckt->data;

      switch (meta_evt->subevent)
      {
      /* USER CODE BEGIN subevent */

      /* USER CODE END subevent */
          case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
          /* USER CODE BEGIN EVT_LE_CONN_COMPLETE */

          /* USER CODE END EVT_LE_CONN_COMPLETE */
          /**
           * The connection is done,
           */
          connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;
          BleApplicationContext.BleApplicationContext_legacy.connectionHandle = connection_complete_event->Connection_Handle;
          BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_CLIENT;

          /* CONNECTION WITH CLIENT */
          APP_DBG_MSG("\r\n\r** CONNECTION EVENT WITH SERVER \n");
          handleNotification.P2P_Evt_Opcode = PEER_CONN_HANDLE_EVT;
          handleNotification.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
          P2PC_APP_Notification(&handleNotification);

          result = aci_gatt_disc_all_primary_services(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
          if (result == BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("\r\n\r** GATT SERVICES & CHARACTERISTICS DISCOVERY  \n");
            APP_DBG_MSG("* GATT :  Start Searching Primary Services \r\n\r");
          }
          else
          {
            APP_DBG_MSG("BLE_CTRL_App_Notification(), All services discovery Failed \r\n\r");
          }

          break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */

        case HCI_LE_ADVERTISING_REPORT_SUBEVT_CODE:
        {
          uint8_t *adv_report_data;
          /* USER CODE BEGIN EVT_LE_ADVERTISING_REPORT */

          /* USER CODE END EVT_LE_ADVERTISING_REPORT */
          le_advertising_event = (hci_le_advertising_report_event_rp0 *) meta_evt->data;

          event_type = le_advertising_event->Advertising_Report[0].Event_Type;

          event_data_size = le_advertising_event->Advertising_Report[0].Length_Data;

          /* WARNING: be careful when decoding advertising report as its raw format cannot be mapped on a C structure.
          The data and RSSI values could not be directly decoded from the RAM using the data and RSSI field from hci_le_advertising_report_event_rp0 structure.
          Instead they must be read by using offsets (please refer to BLE specification).
          RSSI = (int8_t)*(uint8_t*) (adv_report_data + le_advertising_event->Advertising_Report[0].Length_Data);
          */
          adv_report_data = (uint8_t*)(&le_advertising_event->Advertising_Report[0].Length_Data) + 1;
          k = 0;

          /* search AD TYPE 0x09 (Complete Local Name) */
          /* search AD Type 0x02 (16 bits UUIDS) */
          if (event_type == ADV_IND)
          {

            /* ISOLATION OF BD ADDRESS AND LOCAL NAME */

            while(k < event_data_size)
            {
              adlength = adv_report_data[k];
              adtype = adv_report_data[k + 1];
              switch (adtype)
              {
                case AD_TYPE_FLAGS: /* now get flags */
                /* USER CODE BEGIN AD_TYPE_FLAGS */

                /* USER CODE END AD_TYPE_FLAGS */
                  break;

                case AD_TYPE_TX_POWER_LEVEL: /* Tx power level */
                /* USER CODE BEGIN AD_TYPE_TX_POWER_LEVEL */

                /* USER CODE END AD_TYPE_TX_POWER_LEVEL */
                break;
                case AD_TYPE_MANUFACTURER_SPECIFIC_DATA: /* Manufacturer Specific */
                /* USER CODE BEGIN AD_TYPE_MANUFACTURER_SPECIFIC_DATA */

                /* USER CODE END AD_TYPE_MANUFACTURER_SPECIFIC_DATA */
                  if (adlength >= 7 && adv_report_data[k + 2] == 0x01)
                  { /* ST VERSION ID 01 */
                    APP_DBG_MSG("--- ST MANUFACTURER ID --- \n");
                    switch (adv_report_data[k + 3])
                    {   /* Demo ID */
                      case CFG_DEV_ID_P2P_SERVER1: /* End Device 1 */
                        APP_DBG_MSG("-- SERVER DETECTED -- VIA MAN ID\n");
                        BleApplicationContext.DeviceServerFound = 0x01;
                        SERVER_REMOTE_BDADDR[0] = le_advertising_event->Advertising_Report[0].Address[0];
                        SERVER_REMOTE_BDADDR[1] = le_advertising_event->Advertising_Report[0].Address[1];
                        SERVER_REMOTE_BDADDR[2] = le_advertising_event->Advertising_Report[0].Address[2];
                        SERVER_REMOTE_BDADDR[3] = le_advertising_event->Advertising_Report[0].Address[3];
                        SERVER_REMOTE_BDADDR[4] = le_advertising_event->Advertising_Report[0].Address[4];
                        SERVER_REMOTE_BDADDR[5] = le_advertising_event->Advertising_Report[0].Address[5];
                        break;

                      default:
                        break;
                    }

                  }
                  break;
                case AD_TYPE_SERVICE_DATA: /* service data 16 bits */
                  /* USER CODE BEGIN AD_TYPE_SERVICE_DATA */

                  /* USER CODE END AD_TYPE_SERVICE_DATA */
                  break;
                default:
                  /* USER CODE BEGIN adtype_default */

                  /* USER CODE END adtype_default */
                  break;
              } /* end switch Data[k+adlength] */
              k += adlength + 1;
            } /* end while */

          } /* end if ADV_IND */
        }

          break;

        /* USER CODE BEGIN META_EVT */

        /* USER CODE END META_EVT */

        default:
          /* USER CODE BEGIN subevent_default */

          /* USER CODE END subevent_default */
          break;

      }
    }
    break; /* HCI_LE_META_EVT_CODE */

    /* USER CODE BEGIN EVENT_PCKT */

    /* USER CODE END EVENT_PCKT */

    default:
      /* USER CODE BEGIN evt_default */

      /* USER CODE END evt_default */
      break;
  }

  return (SVCCTL_UserEvtFlowEnable);
}

APP_BLE_ConnStatus_t APP_BLE_Get_Client_Connection_Status( uint16_t Connection_Handle )
{

  if (BleApplicationContext.BleApplicationContext_legacy.connectionHandle == Connection_Handle)
  {
    return BleApplicationContext.Device_Connection_Status;
  }
  return APP_BLE_IDLE;
}
/* USER CODE BEGIN FD */
void APP_BLE_Key_Button1_Action(void)
{
#if OOB_DEMO == 0 
      P2PC_APP_SW1_Button_Action();
#else 
      if(P2P_Client_APP_Get_State () != APP_BLE_CONNECTED_CLIENT)
      {
        UTIL_SEQ_SetTask(1 << CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
      }
      else 
      {
        P2PC_APP_SW1_Button_Action();
      }   
#endif 
}

/* USER CODE END FD */
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
  uint32_t srd_bd_addr[2];
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
  srd_bd_addr[1] =  0x0000ED6E;
  srd_bd_addr[0] =  LL_FLASH_GetUDN( );
  aci_hal_write_config_data( CONFIG_DATA_RANDOM_ADDRESS_OFFSET, CONFIG_DATA_RANDOM_ADDRESS_LEN, (uint8_t*)srd_bd_addr );

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
    const char *name = "P2P_C";

    aci_gap_init(role, 0,
                 APPBLE_GAP_DEVICE_NAME_LENGTH,
                 &gap_service_handle, &gap_dev_name_char_handle, &gap_appearance_char_handle);

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
                                           PUBLIC_ADDR
                                           );

    /**
     * Initialize whitelist
     */
    if (BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode)
    {
      aci_gap_configure_whitelist();
    }

}

static void Scan_Request( void )
{
  /* USER CODE BEGIN Scan_Request_1 */

  /* USER CODE END Scan_Request_1 */
  tBleStatus result;
  if (BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
  {
    /* USER CODE BEGIN APP_BLE_CONNECTED_CLIENT */
    BSP_LED_On(LED_BLUE);
    /* USER CODE END APP_BLE_CONNECTED_CLIENT */
    result = aci_gap_start_general_discovery_proc(SCAN_P, SCAN_L, PUBLIC_ADDR, 1);
    if (result == BLE_STATUS_SUCCESS)
    {
    /* USER CODE BEGIN BLE_SCAN_SUCCESS */

    /* USER CODE END BLE_SCAN_SUCCESS */
      APP_DBG_MSG(" \r\n\r** START GENERAL DISCOVERY (SCAN) **  \r\n\r");
    }
    else
    {
    /* USER CODE BEGIN BLE_SCAN_FAILED */
      BSP_LED_On(LED_RED);
    /* USER CODE END BLE_SCAN_FAILED */
      APP_DBG_MSG("-- BLE_App_Start_Limited_Disc_Req, Failed \r\n\r");
    }
  }
  /* USER CODE BEGIN Scan_Request_2 */

  /* USER CODE END Scan_Request_2 */
  return;
}

static void Connect_Request( void )
{
  /* USER CODE BEGIN Connect_Request_1 */

  /* USER CODE END Connect_Request_1 */
  tBleStatus result;

  APP_DBG_MSG("\r\n\r** CREATE CONNECTION TO SERVER **  \r\n\r");

  if (BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
  {
    result = aci_gap_create_connection(SCAN_P,
                                       SCAN_L,
                                       PUBLIC_ADDR, SERVER_REMOTE_BDADDR,
                                       PUBLIC_ADDR,
                                       CONN_P1,
                                       CONN_P2,
                                       0,
                                       SUPERV_TIMEOUT,
                                       CONN_L1,
                                       CONN_L2);

    if (result == BLE_STATUS_SUCCESS)
    {
    /* USER CODE BEGIN BLE_CONNECT_SUCCESS */

    /* USER CODE END BLE_CONNECT_SUCCESS */
    BleApplicationContext.Device_Connection_Status = APP_BLE_LP_CONNECTING;

    }
    else
    {
    /* USER CODE BEGIN BLE_CONNECT_FAILED */
      BSP_LED_On(LED_RED);
    /* USER CODE END BLE_CONNECT_FAILED */
      BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;

    }
  }
  /* USER CODE BEGIN Connect_Request_2 */

  /* USER CODE END Connect_Request_2 */
  return;
}

static void Switch_OFF_GPIO(){
/* USER CODE BEGIN Switch_OFF_GPIO */
  BSP_LED_Off(LED_GREEN);
/* USER CODE END Switch_OFF_GPIO */
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
/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */

/* USER CODE END FD_LOCAL_FUNCTIONS */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
