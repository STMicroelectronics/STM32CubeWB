/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_ble.c
  * @author  MCD Application Team
  * @brief   BLE Application
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
   * requirements.
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
  uint16_t Latency;
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
#define L_BUFF_SIZE 256
/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;

#if (CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR) 
static const uint8_t a_MBdAddr[BD_ADDR_SIZE_LOCAL] =
{
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000000000FF)),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00000000FF00) >> 8),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x000000FF0000) >> 16),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x0000FF000000) >> 24),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0x00FF00000000) >> 32),
  (uint8_t)((CFG_ADV_BD_ADDRESS & 0xFF0000000000) >> 40)
};

static uint8_t a_BdAddrUdn[BD_ADDR_SIZE_LOCAL];
#endif

/**
 *   Identity root key used to derive IRK and DHK(Legacy)
 */
static const uint8_t a_BLE_CfgIrValue[16] = CFG_BLE_IR;

/**
 * Encryption root key used to derive LTK(Legacy) and CSRK
 */
static const uint8_t a_BLE_CfgErValue[16] = CFG_BLE_ER;

tBDAddr SERVER_REMOTE_BDADDR;
tBDAddr ext_adv_server_remote_bd_addr;
uint8_t ext_adv_address_type;

P2PC_APP_ConnHandle_Not_evt_t handleNotification;

static BleApplicationContext_t BleApplicationContext;

#if OOB_DEMO != 0
APP_BLE_p2p_Conn_Update_req_t APP_BLE_p2p_Conn_Update_req;
#endif

/* USER CODE BEGIN PV */
char l_buff[L_BUFF_SIZE];
uint16_t l_buff_pos;
volatile uint8_t disp_filter = 0;
volatile uint8_t scan_running = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx(void * pPayload);
static void BLE_StatusNot(HCI_TL_CmdStatus_t status);
static void Ble_Tl_Init(void);
static void Ble_Hci_Gap_Gatt_Init(void);
#if (CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR) 
static const uint8_t* BleGetBdAddress(void);
#endif
static void Scan_Request(void);
static void Connect_Request(void);
static void Switch_OFF_GPIO(void);

/* USER CODE BEGIN PFP */
static void Scan_Terminate(void);
/* USER CODE END PFP */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Init(void)
{
  SHCI_CmdStatus_t status;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
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
     CFG_BLE_OPTIONS_EXT,
     CFG_BLE_MAX_ADD_EATT_BEARERS
    }
  };

  /**
   * Initialize Ble Transport Layer
   */
  Ble_Tl_Init();

  /**
   * Do not allow standby in the application
   */
  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE, UTIL_LPM_DISABLE);

  /**
   * Register the hci transport layer to handle BLE User Asynchronous Events
   */
  UTIL_SEQ_RegTask(1<<CFG_TASK_HCI_ASYNCH_EVT_ID, UTIL_SEQ_RFU, hci_user_evt_proc);

  /**
   * Starts the BLE Stack on CPU2
   */
  status = SHCI_C2_BLE_Init(&ble_init_cmd_packet);
  if (status != SHCI_Success)
  {
    APP_DBG_MSG("  Fail   : SHCI_C2_BLE_Init command, result: 0x%02x\n\r", status);
    /* if you are here, maybe CPU2 doesn't contain STM32WB_Copro_Wireless_Binaries, see Release_Notes.html */
    Error_Handler();
  }
  else
  {
    APP_DBG_MSG("  Success: SHCI_C2_BLE_Init command\n\r");
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
  UTIL_SEQ_RegTask(1<<CFG_TASK_START_SCAN_ID, UTIL_SEQ_RFU, Scan_Request);
  UTIL_SEQ_RegTask(1<<CFG_TASK_TERMINATE_SCAN_ID, UTIL_SEQ_RFU, Scan_Terminate);
  UTIL_SEQ_RegTask(1<<CFG_TASK_CONN_DEV_1_ID, UTIL_SEQ_RFU, Connect_Request);

  /**
   * Initialization of the BLE App Context
   */
  BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;

  /*Radio mask Activity*/
#if (OOB_DEMO != 0)
  ret = aci_hal_set_radio_activity_mask(0x0020);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_set_radio_activity_mask command, result: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_set_radio_activity_mask command\n\r");
  }
  APP_DBG_MSG("\n");
#endif
  /**
   * Initialize P2P Client Application
   */
  P2PC_APP_Init();

  /* USER CODE BEGIN APP_BLE_Init_3 */

  /* USER CODE END APP_BLE_Init_3 */

#if (OOB_DEMO != 0)
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BleApplicationContext.SwitchOffGPIO_timer_Id), hw_ts_SingleShot, Switch_OFF_GPIO);
#endif

  /**
   * Start scanning
   */
  if (scan_running != 0)
  {
    UTIL_SEQ_SetTask(1 << CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
  }
  /* USER CODE BEGIN APP_BLE_Init_2 */

  /* USER CODE END APP_BLE_Init_2 */
  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification(void *pckt)
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  hci_le_connection_complete_event_rp0 * connection_complete_event;
  evt_blecore_aci *blecore_evt;
  hci_le_extended_advertising_report_event_rp0 * ext_adv_evt;
  
  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
  hci_disconnection_complete_event_rp0 *cc = (void *) event_pckt->data;
  uint8_t result;
  int k = 0;
  uint8_t adtype, adlength;
#if (OOB_DEMO != 0)
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
#endif

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
            APP_DBG_MSG("LED BLUE OFF\n");
            if (scan_running != 0)
            {
              UTIL_SEQ_SetTask(1 << CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
            }
             /* USER CODE END GAP_GENERAL_DISCOVERY_PROC */

             APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE_COMPLETED\n\r");
          }
        }
        break;

#if (OOB_DEMO != 0)
        case ACI_L2CAP_CONNECTION_UPDATE_REQ_VSEVT_CODE:
        {
          /* USER CODE BEGIN EVT_BLUE_L2CAP_CONNECTION_UPDATE_REQ */

          /* USER CODE END EVT_BLUE_L2CAP_CONNECTION_UPDATE_REQ */

          aci_l2cap_connection_update_req_event_rp0 *pr = (aci_l2cap_connection_update_req_event_rp0 *) blecore_evt->data;
          
          ret = aci_hal_set_radio_activity_mask(0x0000);
          if (ret != BLE_STATUS_SUCCESS)
          {
             APP_DBG_MSG("  Fail   : aci_hal_set_radio_activity_mask command, result: 0x%x \n\r", ret);
          }
          else
          {
             APP_DBG_MSG("  Success: aci_hal_set_radio_activity_mask command\n\r");
          }

          APP_BLE_p2p_Conn_Update_req.Identifier = pr->Identifier;
          APP_BLE_p2p_Conn_Update_req.L2CAP_Length = pr->L2CAP_Length;
          APP_BLE_p2p_Conn_Update_req.Interval_Min = pr->Interval_Min;
          APP_BLE_p2p_Conn_Update_req.Interval_Max = pr->Interval_Max;
          APP_BLE_p2p_Conn_Update_req.Latency = pr->Latency;
          APP_BLE_p2p_Conn_Update_req.Timeout_Multiplier = pr->Timeout_Multiplier;

          ret = aci_l2cap_connection_parameter_update_resp(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,
                                                           APP_BLE_p2p_Conn_Update_req.Interval_Min,
                                                           APP_BLE_p2p_Conn_Update_req.Interval_Max,
                                                           APP_BLE_p2p_Conn_Update_req.Latency,
                                                           APP_BLE_p2p_Conn_Update_req.Timeout_Multiplier,
                                                           CONN_L1,
                                                           CONN_L2,
                                                           APP_BLE_p2p_Conn_Update_req.Identifier,
                                                           0x01);
          if(ret != BLE_STATUS_SUCCESS)
          {
             APP_DBG_MSG("  Fail   : aci_l2cap_connection_parameter_update_resp command, result: 0x%x \n\r", ret);
             /* USER CODE BEGIN BLE_STATUS_SUCCESS */
             BSP_LED_On(LED_RED);
             /* USER CODE END BLE_STATUS_SUCCESS */
          }
          else
          {
            APP_DBG_MSG("  Success: aci_l2cap_connection_parameter_update_resp command\n\r");
          }

          ret = aci_hal_set_radio_activity_mask(0x0020);
          if (ret != BLE_STATUS_SUCCESS)
          {
             APP_DBG_MSG("  Fail   : aci_hal_set_radio_activity_mask command, result: 0x%x \n\r", ret);
          }
          else
          {
            APP_DBG_MSG("  Success: aci_hal_set_radio_activity_mask command\n\r");
          }
        }
        break;

          case ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE:
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
          APP_DBG_MSG("\r\n\r** DISCONNECTION EVENT WITH SERVER \n\r");
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
          
          APP_DBG_MSG(">>== HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE - Connection handle: 0x%x\n", connection_complete_event->Connection_Handle);
          APP_DBG_MSG("     - Connection established with Central: @:%02x:%02x:%02x:%02x:%02x:%02x\n",
                      connection_complete_event->Peer_Address[5],
                      connection_complete_event->Peer_Address[4],
                      connection_complete_event->Peer_Address[3],
                      connection_complete_event->Peer_Address[2],
                      connection_complete_event->Peer_Address[1],
                      connection_complete_event->Peer_Address[0]);
          APP_DBG_MSG("     - Connection Interval:   %.2f ms\n     - Connection latency:    %d\n     - Supervision Timeout: %d ms\n\r",
                      connection_complete_event->Conn_Interval*1.25,
                      connection_complete_event->Conn_Latency,
                      connection_complete_event->Supervision_Timeout*10
                      );

          /* CONNECTION WITH CLIENT */
          APP_DBG_MSG("\r\n\r**  CONNECTION COMPLETE EVENT WITH SERVER \n\r");
          handleNotification.P2P_Evt_Opcode = PEER_CONN_HANDLE_EVT;
          handleNotification.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
          P2PC_APP_Notification(&handleNotification);

          result = aci_gatt_disc_all_primary_services(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
          if (result == BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("\r\n\r** GATT SERVICES & CHARACTERISTICS DISCOVERY  \n\r");
            APP_DBG_MSG("* GATT :  Start Searching Primary Services \r\n\r");
          }
          else
          {
            APP_DBG_MSG("BLE_CTRL_App_Notification(), All services discovery Failed \r\n\r");
          }

          break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */
          
      case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
        {
          hci_le_enhanced_connection_complete_event_rp0 *p_enhanced_connection_complete_event;
          p_enhanced_connection_complete_event = (hci_le_enhanced_connection_complete_event_rp0 *) meta_evt->data;
          
          BleApplicationContext.BleApplicationContext_legacy.connectionHandle = p_enhanced_connection_complete_event->Connection_Handle;
          BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_CLIENT;
          
          APP_DBG_MSG(">>== HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE - Connection handle: 0x%x\n", p_enhanced_connection_complete_event->Connection_Handle);
          APP_DBG_MSG("     - Connection established with Central: @:%02x:%02x:%02x:%02x:%02x:%02x\n",
                      p_enhanced_connection_complete_event->Peer_Address[5],
                      p_enhanced_connection_complete_event->Peer_Address[4],
                      p_enhanced_connection_complete_event->Peer_Address[3],
                      p_enhanced_connection_complete_event->Peer_Address[2],
                      p_enhanced_connection_complete_event->Peer_Address[1],
                      p_enhanced_connection_complete_event->Peer_Address[0]);
          APP_DBG_MSG("     - Connection Interval:   %.2f ms\n     - Connection latency:    %d\n     - Supervision Timeout: %d ms\n\r",
                      p_enhanced_connection_complete_event->Conn_Interval*1.25,
                      p_enhanced_connection_complete_event->Conn_Latency,
                      p_enhanced_connection_complete_event->Supervision_Timeout*10
                      );
          
          APP_DBG_MSG("\r\n\r**  ENHANCED CONNECTION COMPLETE EVENT WITH SERVER \n\r");
          handleNotification.P2P_Evt_Opcode = PEER_CONN_HANDLE_EVT;
          handleNotification.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
          P2PC_APP_Notification(&handleNotification);

          result = aci_gatt_disc_all_primary_services(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
          if (result == BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("\r\n\r** GATT SERVICES & CHARACTERISTICS DISCOVERY  \n\r");
            APP_DBG_MSG("* GATT :  Start Searching Primary Services \r\n\r");
          }
          else
          {
            APP_DBG_MSG("BLE_CTRL_App_Notification(), All services discovery Failed \r\n\r");
          }
          
          /* USER CODE BEGIN HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE */
          
          /* USER CODE END HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE */
          break; /* HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE */
        }

        case HCI_LE_ADVERTISING_REPORT_SUBEVT_CODE:
        {
          /* USER CODE BEGIN EVT_LE_ADVERTISING_REPORT */

          /* USER CODE END EVT_LE_ADVERTISING_REPORT */

        }
          break;
        case HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVT_CODE:
        { 
          ext_adv_evt = (hci_le_extended_advertising_report_event_rp0 *) meta_evt->data;
          
          if ((disp_filter != 0) && HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0010))
          {
            break;
          }
          
          l_buff_pos = 0;
          memset(&l_buff[l_buff_pos], 0, L_BUFF_SIZE);          

          l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, 
                                  "%02x:%02x:%02x:%02x:%02x:%02x | "
                                  ,ext_adv_evt->Address[5], ext_adv_evt->Address[4]
                                  ,ext_adv_evt->Address[3] ,ext_adv_evt->Address[2]
                                  ,ext_adv_evt->Address[1] ,ext_adv_evt->Address[0]);
          
          if (HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0010))
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE, "LEGACY   |");
          }
          else
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE, "EXTENDED |");
          }
          
          l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "%4d | ",(int8_t)ext_adv_evt->RSSI );
                    
          if (HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0001))
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "CONN |");
          }
          else
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "     |");
          }
          
          if (HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0002))
          {
            if (HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0008))
            {
              l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "SCAN RSP |");
            }
            else
            {
              l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "SCAN     |");
            }
          }
          else
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "         |");
          }
          
          l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "%3d | ",ext_adv_evt->Data_Length );
          
          if (ext_adv_evt->Advertising_SID != 0xFF)
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "%2d | ",ext_adv_evt->Advertising_SID );
          }
          
          if (HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0020))
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "more data   | ");
          }
          else if (HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0040))
          {
            l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "trunc data | ");
          }
          else
          {
            /* Nothing to do */
          }          

          k = 0;
          while ( k < ext_adv_evt->Data_Length )
          {
            adlength = ext_adv_evt->Data[k];
            adtype = ext_adv_evt->Data[k + 1];
            if ( adtype == AD_TYPE_MANUFACTURER_SPECIFIC_DATA )
            {
              if ( (adlength >= 7) && (ext_adv_evt->Data[k + 2] == 0x01) )
              {
                if (ext_adv_evt->Data[k + 3] == CFG_DEV_ID_P2P_SERVER1)
                {
                  if ( HAL_IS_BIT_CLR(ext_adv_evt->Event_Type, 0x0010) && 
                        HAL_IS_BIT_SET(ext_adv_evt->Event_Type, 0x0001))
                  {
                    l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "server detected via MAN ID, address recorded to connect. | ");
                    BleApplicationContext.DeviceServerFound = 0x01;
                    ext_adv_server_remote_bd_addr[0] = ext_adv_evt->Address[0];
                    ext_adv_server_remote_bd_addr[1] = ext_adv_evt->Address[1];
                    ext_adv_server_remote_bd_addr[2] = ext_adv_evt->Address[2];
                    ext_adv_server_remote_bd_addr[3] = ext_adv_evt->Address[3];
                    ext_adv_server_remote_bd_addr[4] = ext_adv_evt->Address[4];
                    ext_adv_server_remote_bd_addr[5] = ext_adv_evt->Address[5];
                    ext_adv_address_type = ext_adv_evt->Address_Type;
                  }
                }
              }
            }
            else if(adtype == AD_TYPE_COMPLETE_LOCAL_NAME)
            {
              memcpy(&l_buff[l_buff_pos], &ext_adv_evt->Data[k + 2], adlength - 1);
              l_buff_pos += adlength - 1;
            }
            k += adlength + 1;
          }
          l_buff_pos += snprintf(&l_buff[l_buff_pos], L_BUFF_SIZE - l_buff_pos, "\n");
          
          APP_DBG_MSG("%s",l_buff);
          break;
        }
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

APP_BLE_ConnStatus_t APP_BLE_Get_Client_Connection_Status(uint16_t Connection_Handle)
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
  if (P2P_Client_APP_Get_State () != APP_BLE_CONNECTED_CLIENT)
  {
    if (scan_running == 1)
    {
      UTIL_SEQ_SetTask(1 << CFG_TASK_TERMINATE_SCAN_ID, CFG_SCH_PRIO_0);
      scan_running = 0;
    }
    UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_DEV_1_ID, CFG_SCH_PRIO_0);
  }
  else 
  {
    P2PC_APP_SW1_Button_Action();
  }   
#endif
}

void APP_BLE_Key_Button2_Action(void)
{
  disp_filter = !disp_filter;
}

void APP_BLE_Key_Button3_Action(void)
{
  if (scan_running == 0)
  {
    UTIL_SEQ_SetTask(1 << CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
    scan_running = 1;
  }
  else
  {
    UTIL_SEQ_SetTask(1 << CFG_TASK_TERMINATE_SCAN_ID, CFG_SCH_PRIO_0);
    scan_running = 0;
  }
  
}

/* USER CODE END FD */
/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void Ble_Tl_Init(void)
{
  HCI_TL_HciInitConf_t Hci_Tl_Init_Conf;

  Hci_Tl_Init_Conf.p_cmdbuffer = (uint8_t*)&BleCmdBuffer;
  Hci_Tl_Init_Conf.StatusNotCallBack = BLE_StatusNot;
  hci_init(BLE_UserEvtRx, (void*) &Hci_Tl_Init_Conf);

  return;
}

static void Ble_Hci_Gap_Gatt_Init(void)
{
  uint8_t role;
  uint16_t gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
#if (CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR)  
  const uint8_t *p_bd_addr;
#endif  
#if (CFG_BLE_ADDRESS_TYPE != GAP_PUBLIC_ADDR)
  uint32_t a_srd_bd_addr[2];
#endif
  uint16_t a_appearance[1] = {BLE_CFG_GAP_APPEARANCE};
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Ble_Hci_Gap_Gatt_Init*/

  /* USER CODE END Ble_Hci_Gap_Gatt_Init*/

  APP_DBG_MSG("==>> Start Ble_Hci_Gap_Gatt_Init function\n");

  /**
   * Initialize HCI layer
   */
  /*HCI Reset to synchronise BLE Stack*/
  ret = hci_reset();
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : hci_reset command, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: hci_reset command\n");
  }

  /**
   * Write the BD Address
   */
#if (CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR)  
  p_bd_addr = BleGetBdAddress();
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBLIC_ADDRESS_OFFSET, CONFIG_DATA_PUBLIC_ADDRESS_LEN, (uint8_t*) p_bd_addr);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_PUBLIC_ADDRESS_OFFSET, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_PUBLIC_ADDRESS_OFFSET\n");
    APP_DBG_MSG("  Public Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n",p_bd_addr[5],p_bd_addr[4],p_bd_addr[3],p_bd_addr[2],p_bd_addr[1],p_bd_addr[0]);
  }
#endif

  /**
   * Static random Address
   * The two upper bits shall be set to 1
   * The lowest 32bits is read from the UDN to differentiate between devices
   * The RNG may be used to provide a random number on each power on
   */
#if (CFG_IDENTITY_ADDRESS == GAP_STATIC_RANDOM_ADDR)
#if defined(CFG_STATIC_RANDOM_ADDRESS)
  a_srd_bd_addr[0] = CFG_STATIC_RANDOM_ADDRESS & 0xFFFFFFFF;
  a_srd_bd_addr[1] = (uint32_t)((uint64_t)CFG_STATIC_RANDOM_ADDRESS >> 32);
  a_srd_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */

  ret = aci_hal_write_config_data(CONFIG_DATA_RANDOM_ADDRESS_OFFSET, CONFIG_DATA_RANDOM_ADDRESS_LEN, (uint8_t*)a_srd_bd_addr);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET\n");
    APP_DBG_MSG("  Random Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n", (uint8_t)(a_srd_bd_addr[1] >> 8),
                                                                               (uint8_t)(a_srd_bd_addr[1]),
                                                                               (uint8_t)(a_srd_bd_addr[0] >> 24),
                                                                               (uint8_t)(a_srd_bd_addr[0] >> 16),
                                                                               (uint8_t)(a_srd_bd_addr[0] >> 8),
                                                                               (uint8_t)(a_srd_bd_addr[0]));
  }
#else
  UNUSED(a_srd_bd_addr);
  APP_DBG_MSG(" !! => Warning: define CFG_STATIC_RANDOM_ADDRESS when GAP_STATIC_RANDOM_ADDR selected otherwise configure RNG module !!\n");
#endif /* CFG_STATIC_RANDOM_ADDRESS */
#endif

  /**
   * Write Identity root key used to derive IRK and DHK(Legacy)
   */
  ret = aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, (uint8_t*)a_BLE_CfgIrValue);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_IR_OFFSET, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_IR_OFFSET\n");
  }

  /**
   * Write Encryption root key used to derive LTK and CSRK
   */
  ret = aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, (uint8_t*)a_BLE_CfgErValue);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_ER_OFFSET, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_ER_OFFSET\n");
  }

  /**
   * Set TX Power.
   */
  ret = aci_hal_set_tx_power_level(1, CFG_TX_POWER);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_set_tx_power_level command, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_set_tx_power_level command\n");
  }

  /**
   * Initialize GATT interface
   */
  ret = aci_gatt_init();
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_init command, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_init command\n");
  }

  /**
   * Initialize GAP interface
   */
  role = 0;

#if (BLE_CFG_PERIPHERAL == 1)
  role |= GAP_PERIPHERAL_ROLE;
#endif /* BLE_CFG_PERIPHERAL == 1 */

#if (BLE_CFG_CENTRAL == 1)
  role |= GAP_CENTRAL_ROLE;
#endif /* BLE_CFG_CENTRAL == 1 */

/* USER CODE BEGIN Role_Mngt*/

/* USER CODE END Role_Mngt */

  if (role > 0)
  {
    const char *name = "P2PCLI";

    ret = aci_gap_init(role,
                       CFG_PRIVACY,
                       APPBLE_GAP_DEVICE_NAME_LENGTH,
                       &gap_service_handle,
                       &gap_dev_name_char_handle,
                       &gap_appearance_char_handle);

    if (ret != BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  Fail   : aci_gap_init command, result: 0x%x \n", ret);
    }
    else
    {
      APP_DBG_MSG("  Success: aci_gap_init command\n");
    }

    ret = aci_gatt_update_char_value(gap_service_handle, gap_dev_name_char_handle, 0, strlen(name), (uint8_t *) name);
    if (ret != BLE_STATUS_SUCCESS)
    {
      BLE_DBG_SVCCTL_MSG("  Fail   : aci_gatt_update_char_value - Device Name\n");
    }
    else
    {
      BLE_DBG_SVCCTL_MSG("  Success: aci_gatt_update_char_value - Device Name\n");
    }
  }

  ret = aci_gatt_update_char_value(gap_service_handle,
                                   gap_appearance_char_handle,
                                   0,
                                   2,
                                   (uint8_t *)&a_appearance);
  if (ret != BLE_STATUS_SUCCESS)
  {
    BLE_DBG_SVCCTL_MSG("  Fail   : aci_gatt_update_char_value - Appearance\n");
  }
  else
  {
    BLE_DBG_SVCCTL_MSG("  Success: aci_gatt_update_char_value - Appearance\n");
  }

  /**
   * Initialize IO capability
   */
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability = CFG_IO_CAPABILITY;
  ret = aci_gap_set_io_capability(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gap_set_io_capability command, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gap_set_io_capability command\n");
  }

  /**
   * Initialize authentication
   */
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode = CFG_MITM_PROTECTION;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = CFG_ENCRYPTION_KEY_SIZE_MIN;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = CFG_ENCRYPTION_KEY_SIZE_MAX;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin = CFG_USED_FIXED_PIN;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin = CFG_FIXED_PIN;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode = CFG_BONDING_MODE;

  ret = aci_gap_set_authentication_requirement(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode,
                                               BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode,
                                               CFG_SC_SUPPORT,
                                               CFG_KEYPRESS_NOTIFICATION_SUPPORT,
                                               BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin,
                                               BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax,
                                               BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin,
                                               BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin,
                                               CFG_IDENTITY_ADDRESS);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gap_set_authentication_requirement command, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gap_set_authentication_requirement command\n");
  }

  /**
   * Initialize whitelist
   */
  if (BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode)
  {
    ret = aci_gap_configure_whitelist();
    if (ret != BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  Fail   : aci_gap_configure_whitelist command, result: 0x%x \n", ret);
    }
    else
    {
      APP_DBG_MSG("  Success: aci_gap_configure_whitelist command\n");
    }
  }
  APP_DBG_MSG("==>> End Ble_Hci_Gap_Gatt_Init function\n\r");
}

static void Scan_Request(void)
{
  /* USER CODE BEGIN Scan_Request_1 */

  /* USER CODE END Scan_Request_1 */
  tBleStatus result;
  if (BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
  {
    /* USER CODE BEGIN APP_BLE_CONNECTED_CLIENT */
    BSP_LED_On(LED_BLUE);
    APP_DBG_MSG("LED BLUE ON\n");
    APP_DBG_MSG("    BD address    |   Type   |rssi |Conn  | Scan    |data|SID | info\n");
    /* USER CODE END APP_BLE_CONNECTED_CLIENT */
    result = aci_gap_start_general_discovery_proc(SCAN_P, SCAN_L, CFG_BLE_ADDRESS_TYPE, 1);
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

static void Scan_Terminate( void )
{
  tBleStatus result;  
  
  result = aci_gap_terminate_gap_proc(GAP_GENERAL_DISCOVERY_PROC);
  if (result == BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("-- BLE_App_Terminate_Disc_Req, Success\r\n");
  }
  else
  {
    BSP_LED_On(LED_RED);
    APP_DBG_MSG("-- BLE_App_Terminate_Disc_Req, Failed\r\n");
  }
  return;
}

static void Connect_Request(void)
{
  /* USER CODE BEGIN Connect_Request_1 */

  /* USER CODE END Connect_Request_1 */
  tBleStatus result;

  APP_DBG_MSG("\r\n\r** CREATE CONNECTION TO SERVER **  \r\n\r");
  
  if (BleApplicationContext.DeviceServerFound == 0)
  {
    APP_DBG_MSG("No server available\r\n");
  }
  else
  {
    if (BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
    {
      result = aci_gap_create_connection(SCAN_P,
                                         SCAN_L,
                                         ext_adv_address_type, ext_adv_server_remote_bd_addr,
                                         CFG_BLE_ADDRESS_TYPE,
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
  }
  /* USER CODE BEGIN Connect_Request_2 */

  /* USER CODE END Connect_Request_2 */
  return;
}

static void Switch_OFF_GPIO()
{
  /* USER CODE BEGIN Switch_OFF_GPIO */
  BSP_LED_Off(LED_GREEN);
  /* USER CODE END Switch_OFF_GPIO */
}

#if (CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR) 
const uint8_t* BleGetBdAddress(void)
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
    a_BdAddrUdn[0] = (uint8_t)(udn & 0x000000FF);
    a_BdAddrUdn[1] = (uint8_t)((udn & 0x0000FF00) >> 8);
    a_BdAddrUdn[2] = (uint8_t)device_id;
    a_BdAddrUdn[3] = (uint8_t)(company_id & 0x000000FF);
    a_BdAddrUdn[4] = (uint8_t)((company_id & 0x0000FF00) >> 8);
    a_BdAddrUdn[5] = (uint8_t)((company_id & 0x00FF0000) >> 16);

    p_bd_addr = (const uint8_t *)a_BdAddrUdn;
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
#endif
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

static void BLE_UserEvtRx(void * pPayload)
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

static void BLE_StatusNot(HCI_TL_CmdStatus_t status)
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

void SVCCTL_ResumeUserEventFlow(void)
{
  hci_resume_flow();
  return;
}

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
