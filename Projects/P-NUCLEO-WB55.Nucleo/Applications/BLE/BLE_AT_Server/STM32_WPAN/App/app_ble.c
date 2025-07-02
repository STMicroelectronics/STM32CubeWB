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
#include "dis_app.h"
#include "hrs_app.h"
#include "p2p_server_app.h"
#include "stm32wb_at_ble.h"
#include "ble_at_server.h"
#include "custom_app.h"
#include "custom.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */
typedef enum
{
  CUSTOMS_NOTIFICATION_MASK = (1 << 0),
  CUSTOMS_INDICATION_MASK = (1 << 1),
} ClientCharConfMask_t;

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define FAST_ADV_TIMEOUT               (30*1000*1000/CFG_TS_TICK_VAL) /**< 30s */
#define INITIAL_ADV_TIMEOUT            (60*1000*1000/CFG_TS_TICK_VAL) /**< 60s */

#define BD_ADDR_SIZE_LOCAL    6

/* USER CODE BEGIN PD */
#define LED_ON_TIMEOUT                 (0.005*1000*1000/CFG_TS_TICK_VAL) /**< 5ms */

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
*   Identity root key used to derive IRK and DHK(Legacy)
*/
static const uint8_t BLE_CFG_IR_VALUE[16] = CFG_BLE_IR;

/**
* Encryption root key used to derive LTK(Legacy) and CSRK
*/
static const uint8_t BLE_CFG_ER_VALUE[16] = CFG_BLE_ER;

/**
 * These are the two tags used to manage a power failure during OTA
 * The MagicKeywordAdress shall be mapped @0x140 from start of the binary image
 * The MagicKeywordvalue is checked in the ble_ota application
 */
PLACE_IN_SECTION("TAG_OTA_END") const uint32_t MagicKeywordValue = 0x94448A29 ;
PLACE_IN_SECTION("TAG_OTA_START") const uint32_t MagicKeywordAddress = (uint32_t)&MagicKeywordValue;

BleApplicationContext_t BleApplicationContext;
static uint16_t AdvIntervalMin, AdvIntervalMax;

P2PS_APP_ConnHandle_Not_evt_t handleNotification;
Custom_Context_t Custom_Context;
extern RNG_HandleTypeDef hrng;

#if L2CAP_REQUEST_NEW_CONN_PARAM != 0
#define SIZE_TAB_CONN_INT            2
float tab_conn_interval[SIZE_TAB_CONN_INT] = {50, 1000} ; /* ms */
uint8_t index_con_int, mutex;
#endif

/**
 * Advertising Data
 */
#if (P2P_SERVER1 != 0)
uint8_t manuf_data[14] = {
    sizeof(manuf_data)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01/*SKD version */,
    CFG_DEV_ID_P2P_SERVER1 /* STM32WB - P2P Server 1*/,
    0x00 /* GROUP A Feature  */,
    0x00 /* GROUP A Feature */,
    0x00 /* GROUP B Feature */,
    0x00 /* GROUP B Feature */,
    0x00, /* BLE MAC start -MSB */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, /* BLE MAC stop */
};
#endif
/**
 * Advertising Data
 */
#if (P2P_SERVER2 != 0)
static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'P', '2', 'P', 'S', 'R', 'V', '2'};
uint8_t manuf_data[14] = {
    sizeof(manuf_data)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01/*SKD version */,
    CFG_DEV_ID_P2P_SERVER2 /* STM32WB - P2P Server 2*/,
    0x00 /* GROUP A Feature  */,
    0x00 /* GROUP A Feature */,
    0x00 /* GROUP B Feature */,
    0x00 /* GROUP B Feature */,
    0x00, /* BLE MAC start -MSB */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, /* BLE MAC stop */
};

#endif

#if (P2P_SERVER3 != 0)
static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'P', '2', 'P', 'S', 'R', 'V', '3'};
uint8_t manuf_data[14] = {
    sizeof(manuf_data)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01/*SKD version */,
    CFG_DEV_ID_P2P_SERVER3 /* STM32WB - P2P Server 3*/,
    0x00 /* GROUP A Feature  */,
    0x00 /* GROUP A Feature */,
    0x00 /* GROUP B Feature */,
    0x00 /* GROUP B Feature */,
    0x00, /* BLE MAC start -MSB */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, /* BLE MAC stop */
};
#endif

#if (P2P_SERVER4 != 0)
static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'P', '2', 'P', 'S', 'R', 'V', '4'};
uint8_t manuf_data[14] = {
    sizeof(manuf_data)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01/*SKD version */,
    CFG_DEV_ID_P2P_SERVER4 /* STM32WB - P2P Server 4*/,
    0x00 /* GROUP A Feature  */,
    0x00 /* GROUP A Feature */,
    0x00 /* GROUP B Feature */,
    0x00 /* GROUP B Feature */,
    0x00, /* BLE MAC start -MSB */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, /* BLE MAC stop */
};
#endif

#if (P2P_SERVER5 != 0)
static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'P', '2', 'P', 'S', 'R', 'V', '5'};
uint8_t manuf_data[14] = {
    sizeof(manuf_data)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01/*SKD version */,
    CFG_DEV_ID_P2P_SERVER5 /* STM32WB - P2P Server 5*/,
    0x00 /* GROUP A Feature  */,
    0x00 /* GROUP A Feature */,
    0x00 /* GROUP B Feature */,
    0x00 /* GROUP B Feature */,
    0x00, /* BLE MAC start -MSB */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, /* BLE MAC stop */
};
#endif

#if (P2P_SERVER6 != 0)
static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'P', '2', 'P', 'S', 'R', 'V', '6'};
uint8_t manuf_data[14] = {
    sizeof(manuf_data)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01/*SKD version */,
    CFG_DEV_ID_P2P_SERVER6 /* STM32WB - P2P Server 1*/,
    0x00 /* GROUP A Feature  */,
    0x00 /* GROUP A Feature */,
    0x00 /* GROUP B Feature */,
    0x00 /* GROUP B Feature */,
    0x00, /* BLE MAC start -MSB */
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, /* BLE MAC stop */
};
#endif

/* USER CODE BEGIN PV */
char local_name_var[] = { AD_TYPE_COMPLETE_LOCAL_NAME , 0, 0, 0, 0, 0, 0, 0, 0};

extern uint8_t global_svc_index;
extern float global_conn_int_min;
extern float global_conn_int_max;
extern uint8_t global_notif_serv_index;
extern uint8_t global_notif_char_index;
extern uint8_t global_notif_val_tab[64];
extern uint8_t global_notif_val_len;
extern uint8_t global_indic_serv_index;
extern uint8_t global_indic_char_index;
extern uint8_t global_indic_val_tab[64];
extern uint8_t global_indic_val_len;
extern uint8_t global_svc_uuid_type;
extern uint16_t global_svc_uuid;
extern uint8_t global_max_attr;
extern uint8_t global_svc_id;
extern uint8_t global_char_id;
extern uint8_t global_char_uuid_type;
extern uint16_t global_char_uuid;
extern uint16_t global_char_value_len;
extern uint8_t global_char_properties;
extern uint8_t global_char_sec_permission;
extern uint8_t global_gatt_event_mask;
extern uint8_t global_pairing_confirm;
extern uint32_t global_pass_key_resp;
extern uint8_t global_io_capability;
extern uint8_t global_bonding;
extern uint8_t global_mitm;
extern uint8_t global_scsupport;
extern uint8_t global_usefixedpin;
extern uint32_t global_fixedpin;
extern uint8_t global_addrtype;
extern uint8_t global_bdaddress[6];
extern uint8_t global_rand_bdaddress[6];
extern uint8_t global_ble_addr_type;
extern uint8_t global_ble_cfg_addr_type;
extern char global_name[];
extern uint8_t global_tx_power_code;
extern uint8_t global_entry_number;
extern uint8_t global_adv_type;
extern uint16_t global_adv_int_min;
extern uint16_t global_adv_int_max;
extern uint8_t global_adv_enable;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * pPayload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
const uint8_t* BleGetBdAddress( void );
static void Adv_Request( APP_BLE_ConnStatus_t New_Status );
static void Add_Advertisment_Service_UUID( uint16_t servUUID );
static void Switch_OFF_GPIO( void );

#if(L2CAP_REQUEST_NEW_CONN_PARAM != 0)
static void BLE_SVC_L2CAP_Conn_Update(uint16_t Connection_Handle);
static void Connection_Interval_Update_Req( void );
#endif

/* USER CODE BEGIN PFP */

extern HRSAPP_Context_t HRSAPP_Context;

static void Manage_Start_Stop_Adv_Init(void);
static void Manage_Service_Init(void);
static void Manage_Update_Connection(void);
static void Manage_Update_Charac(void);
static void Manage_Indication_Update_Charac(void);
static void Manage_Custom_Service_Init(void);
static void Manage_Charac_Init(void);
static void Manage_Peripheral_Security_Request(void);
static void Manage_Pairing_Confirm_Answer(void);
static void Manage_Pass_Key_Resp(void);
static void Manage_IO_Capability(void);
static void Manage_set_authen_req(void);
static void Manage_clear_db(void);
static void Manage_hci_reset(void);
static void Manage_Ble_Init(void);

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Init( void )
{
  SHCI_CmdStatus_t status;
  uint8_t i,j;
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

  /* Initialization of custom services and characteristics handles */ 
  for (i = 0 ; i < CUSTOM_APP_SVC_COUNT ; i++)
  {
    Custom_Context.SvcHdle[i] = 0;
    for (j = 0 ; j < CUSTOM_APP_CHAR_COUNT ; j++)
    {
      Custom_Context.CharHdle[i][j] = 0;
    }
  }
  
  /* start/stop advertising */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MANAGE_START_STOP_ADV_ID, UTIL_SEQ_RFU, Manage_Start_Stop_Adv_Init);
  /* start init */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MANAGE_INIT_ID, UTIL_SEQ_RFU, Manage_Ble_Init);
  /* P2P_server or Heart rate */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MANAGE_SERVICE_INIT_ID, UTIL_SEQ_RFU, Manage_Service_Init);
  /* Custom Service */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MANAGE_CUSTOM_SERVICE_INIT_ID, UTIL_SEQ_RFU, Manage_Custom_Service_Init);
  /* Custom Charac */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MANAGE_CHARAC_INIT_ID, UTIL_SEQ_RFU, Manage_Charac_Init);
  /* Peripheral Security Request */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MANAGE_PERIPHERAL_SECUTITY_REQ_ID, UTIL_SEQ_RFU, Manage_Peripheral_Security_Request);
  /* Pairing Confirm Answer */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PAIRING_CONFIRM_ID, UTIL_SEQ_RFU, Manage_Pairing_Confirm_Answer);
  /* Pass_Key_Resp */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_PASS_KEY_RESP_ID, UTIL_SEQ_RFU, Manage_Pass_Key_Resp);
  /* io_capability */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_IO_CAPA_ID, UTIL_SEQ_RFU, Manage_IO_Capability);
  /* authentication_req */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SET_AUTHEN_REQ_ID, UTIL_SEQ_RFU, Manage_set_authen_req);
  /* clear_security_db */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CLEAR_DB_ID, UTIL_SEQ_RFU, Manage_clear_db);
  /* hci_reset */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HCIRESET_ID, UTIL_SEQ_RFU, Manage_hci_reset);
  /* Connection interval update */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MANAGE_CONN_PARAM_ID, UTIL_SEQ_RFU, Manage_Update_Connection);
  /* notification update*/
  UTIL_SEQ_RegTask( 1<<CFG_TASK_UPDATE_CHAR_ID, UTIL_SEQ_RFU, Manage_Update_Charac);  
  /* indication update */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_INDICATION_UPDATE_CHAR_ID, UTIL_SEQ_RFU, Manage_Indication_Update_Charac);

  /**
   * Create timer to handle the Led Switch OFF
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BleApplicationContext.SwitchOffGPIO_timer_Id), hw_ts_SingleShot, Switch_OFF_GPIO);
  
#if (L2CAP_REQUEST_NEW_CONN_PARAM != 0)
  UTIL_SEQ_RegTask(1<<CFG_TASK_CONN_UPDATE_REG_ID, UTIL_SEQ_RFU, Connection_Interval_Update_Req);
  index_con_int = 0;
  mutex = 1;
#endif /* L2CAP_REQUEST_NEW_CONN_PARAM != 0 */  
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt ) 
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  evt_blecore_aci *blecore_evt;
  hci_le_phy_update_complete_event_rp0 *evt_le_phy_update_complete;
  hci_le_connection_update_complete_event_rp0 *connection_update_complete_event;
  uint8_t TX_PHY, RX_PHY;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  stm32wb_at_BLE_EVT_CONN_t param_BLE_EVT_CONN;
  stm32wb_at_BLE_EVT_UPD_CONN_t param_BLE_EVT_UPD_CONN;
  stm32wb_at_BLE_EVT_PASSKEY_REQ_t param_BLE_EVT_PASSKEY_REQ;
  stm32wb_at_BLE_EVT_VALUE_CONFIRM_t param_BLE_EVT_NUMERIC_VALUE;
  stm32wb_at_BLE_EVT_PAIRING_t param_BLE_EVT_PAIRING;

  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;

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
        global_conn_int_min = 0;
        global_conn_int_max = 0;

        printf("\r\n\r** DISCONNECTION EVENT WITH CLIENT \n");
        param_BLE_EVT_CONN.status = 0;
        ble_at_server_Send_evt(BLE_EVT_CONN, &param_BLE_EVT_CONN);
      }

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
          printf("\r\n\r** CONNECTION UPDATE EVENT WITH CLIENT \n");
          connection_update_complete_event = (hci_le_connection_update_complete_event_rp0*) meta_evt->data;
          global_conn_int_min = (uint8_t)(connection_update_complete_event->Conn_Interval)*1.25;
          global_conn_int_max = (uint8_t)(connection_update_complete_event->Conn_Interval)*1.25;
          
          param_BLE_EVT_UPD_CONN.status = 1;
          ble_at_server_Send_evt(BLE_EVT_UPD_CONN, &param_BLE_EVT_UPD_CONN);

          /* USER CODE BEGIN EVT_LE_CONN_UPDATE_COMPLETE */

          /* USER CODE END EVT_LE_CONN_UPDATE_COMPLETE */
          break;
        case HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE:
          printf("EVT_UPDATE_PHY_COMPLETE \n");
          evt_le_phy_update_complete = (hci_le_phy_update_complete_event_rp0*)meta_evt->data;
          if (evt_le_phy_update_complete->Status == 0)
          {
            APP_DBG_MSG("EVT_UPDATE_PHY_COMPLETE, status ok \n");
          }
          else
          {
            APP_DBG_MSG("EVT_UPDATE_PHY_COMPLETE, status nok \n");
          }

          ret = hci_le_read_phy(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,&TX_PHY,&RX_PHY);
          if (ret == BLE_STATUS_SUCCESS)
          {
            printf("Read_PHY success \n");

            if ((TX_PHY == TX_2M) && (RX_PHY == RX_2M))
            {
              printf("PHY Param  TX= %d, RX= %d \n", TX_PHY, RX_PHY);
            }
            else
            {
              printf("PHY Param  TX= %d, RX= %d \n", TX_PHY, RX_PHY);
            }
          }
          else
          {
            printf("Read conf not success \n");
          }
          /* USER CODE BEGIN EVT_LE_PHY_UPDATE_COMPLETE */

          /* USER CODE END EVT_LE_PHY_UPDATE_COMPLETE */
          break;
        case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
        {
          hci_le_connection_complete_event_rp0 *connection_complete_event;

          /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */
          connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;
          global_conn_int_min = (uint8_t)(connection_complete_event->Conn_Interval);
          global_conn_int_max = (uint8_t)(connection_complete_event->Conn_Interval);

          printf("HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE for connection handle 0x%x\n", connection_complete_event->Connection_Handle);
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

          param_BLE_EVT_CONN.status = 1;
          ble_at_server_Send_evt(BLE_EVT_CONN, &param_BLE_EVT_CONN);

          BleApplicationContext.BleApplicationContext_legacy.connectionHandle = connection_complete_event->Connection_Handle;
          ret = aci_gatt_exchange_config(connection_complete_event->Connection_Handle);
          if (ret == BLE_STATUS_SUCCESS)
          {
            printf("exchange_config, Successfully \r\n\r");
          }
          else
          {
            printf("exchange_config, Failed \r\n\r");
          }
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
        aci_gap_pairing_complete_event_rp0 *pairing_complete;

      case ACI_GAP_LIMITED_DISCOVERABLE_VSEVT_CODE:
        APP_DBG_MSG("\r\n\r** ACI_GAP_LIMITED_DISCOVERABLE_VSEVT_CODE \n");
          break; /* ACI_GAP_LIMITED_DISCOVERABLE_VSEVT_CODE */
      case ACI_GAP_PASS_KEY_REQ_VSEVT_CODE:
        param_BLE_EVT_PASSKEY_REQ.dummy = 1;
        ble_at_server_Send_evt(BLE_EVT_PASSKEY_REQ, &param_BLE_EVT_PASSKEY_REQ);
          break; /* ACI_GAP_PASS_KEY_REQ_VSEVT_CODE */
      case ACI_GAP_AUTHORIZATION_REQ_VSEVT_CODE:
          break; /* ACI_GAP_AUTHORIZATION_REQ_VSEVT_CODE */
      case ACI_GAP_BOND_LOST_VSEVT_CODE:
          break; /* ACI_GAP_BOND_LOST_VSEVT_CODE */
      case ACI_GAP_ADDR_NOT_RESOLVED_VSEVT_CODE:
          break; /* ACI_GAP_ADDR_NOT_RESOLVED_VSEVT_CODE */
      case (ACI_GAP_KEYPRESS_NOTIFICATION_VSEVT_CODE):        
        break; /* ACI_GAP_KEYPRESS_NOTIFICATION_VSEVT_CODE */
      case (ACI_GAP_NUMERIC_COMPARISON_VALUE_VSEVT_CODE):
          param_BLE_EVT_NUMERIC_VALUE.numeric_value = ((aci_gap_numeric_comparison_value_event_rp0 *)(blecore_evt->data))->Numeric_Value;  
          
          ble_at_server_Send_evt(BLE_EVT_VALUE_CONFIRM, &param_BLE_EVT_NUMERIC_VALUE);
        break;
      case (ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE):
        {
            pairing_complete = (aci_gap_pairing_complete_event_rp0*)blecore_evt->data;
            param_BLE_EVT_PAIRING.status = pairing_complete->Status;  
            param_BLE_EVT_PAIRING.reason = pairing_complete->Reason;
            
            ble_at_server_Send_evt(BLE_EVT_PAIRING, &param_BLE_EVT_PAIRING);
        }
        break;
          
      /* USER CODE END ecode */
      /**
       * SPECIFIC to P2P Server APP
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
          BSP_LED_On(LED_GREEN);
          HW_TS_Start(BleApplicationContext.SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
        /* USER CODE END RADIO_ACTIVITY_EVENT*/
          break; /* ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE */
#endif

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

#if (CFG_BUTTON_SUPPORTED == 1)
/* USER CODE BEGIN FD*/
void APP_BLE_Key_Button1_Action(void)
{
  P2PS_APP_SW1_Button_Action();
}

void APP_BLE_Key_Button2_Action(void)
{
#if (L2CAP_REQUEST_NEW_CONN_PARAM != 0 )
  UTIL_SEQ_SetTask( 1<<CFG_TASK_CONN_UPDATE_REG_ID, CFG_SCH_PRIO_0);
#endif

  return;
}

void APP_BLE_Key_Button3_Action(void)
{
}
#endif

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
  uint32_t srd_bd_addr[2];
  uint16_t appearance[1] = { BLE_CFG_GAP_APPEARANCE };
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  
  /**
   * Initialize HCI layer
   */
  /*HCI Reset to synchronise BLE Stack*/
  hci_reset();

  /**
   * Write the BD Address
   */
  if ((global_ble_addr_type == GAP_PUBLIC_ADDR) && (global_ble_cfg_addr_type == GAP_PUBLIC_ADDR))
  {
    ret = aci_hal_write_config_data(CONFIG_DATA_PUBLIC_ADDRESS_OFFSET,
                              CONFIG_DATA_PUBLIC_ADDRESS_LEN,
                              (uint8_t*) global_bdaddress);
    
    if (ret != BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_PUBLIC_ADDRESS_OFFSET, result: 0x%x \n", ret);
    }
    else
    {
      APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_PUBLIC_ADDRESS_OFFSET\n");
      APP_DBG_MSG("  Public Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n",global_bdaddress[5],global_bdaddress[4],global_bdaddress[3],global_bdaddress[2],global_bdaddress[1],global_bdaddress[0]);
    } 
    
    /* BLE MAC in ADV Packet */
    manuf_data[ sizeof(manuf_data)-6] = global_bdaddress[5];
    manuf_data[ sizeof(manuf_data)-5] = global_bdaddress[4];
    manuf_data[ sizeof(manuf_data)-4] = global_bdaddress[3];
    manuf_data[ sizeof(manuf_data)-3] = global_bdaddress[2];
    manuf_data[ sizeof(manuf_data)-2] = global_bdaddress[1];
    manuf_data[ sizeof(manuf_data)-1] = global_bdaddress[0];
    
    memcpy(&global_bdaddress[0], BleGetBdAddress(), sizeof(global_bdaddress));
  }
  else if (global_ble_addr_type == GAP_STATIC_RANDOM_ADDR)
  {
   /**
   * Static random Address
   * The two upper bits shall be set to 1
   * The lowest 32bits is read from the UDN to differentiate between devices
   * The RNG may be used to provide a random number on each power on
   */
    /* set the two upper bits to fit with random address definition */
    global_rand_bdaddress[5] |= 0xC0;
  
    ret = aci_hal_write_config_data(CONFIG_DATA_RANDOM_ADDRESS_OFFSET,
                              CONFIG_DATA_RANDOM_ADDRESS_LEN,
                              (uint8_t*) global_rand_bdaddress);
 
    if (ret != BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET, result: 0x%x \n", ret);
    }
    else
    {
      APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET\n");
      APP_DBG_MSG("  Random Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n", (uint8_t)(global_rand_bdaddress[5]),
                                                                                 (uint8_t)(global_rand_bdaddress[4]),
                                                                                 (uint8_t)(global_rand_bdaddress[3]),
                                                                                 (uint8_t)(global_rand_bdaddress[2]),
                                                                                 (uint8_t)(global_rand_bdaddress[1]),
                                                                                 (uint8_t)(global_rand_bdaddress[0]));
    }
  
    /* BLE MAC in ADV Packet */
    manuf_data[ sizeof(manuf_data)-6] = global_rand_bdaddress[5];
    manuf_data[ sizeof(manuf_data)-5] = global_rand_bdaddress[4];
    manuf_data[ sizeof(manuf_data)-4] = global_rand_bdaddress[3];
    manuf_data[ sizeof(manuf_data)-3] = global_rand_bdaddress[2];
    manuf_data[ sizeof(manuf_data)-2] = global_rand_bdaddress[1];
    manuf_data[ sizeof(manuf_data)-1] = global_rand_bdaddress[0];
    
  }
  else if (global_ble_cfg_addr_type == GAP_STATIC_RANDOM_ADDR)
  {
#if defined(CFG_STATIC_RANDOM_ADDRESS)
    srd_bd_addr[0] = CFG_STATIC_RANDOM_ADDRESS & 0xFFFFFFFF;
    srd_bd_addr[1] = (uint32_t)((uint64_t)CFG_STATIC_RANDOM_ADDRESS >> 32);
    srd_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */
#else
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
    
    memcpy(&global_rand_bdaddress[0], srd_bd_addr, sizeof(global_rand_bdaddress));
    /* BLE MAC in ADV Packet */
    manuf_data[ sizeof(manuf_data)-6] = srd_bd_addr[1] >> 8 ;
    manuf_data[ sizeof(manuf_data)-5] = srd_bd_addr[1];
    manuf_data[ sizeof(manuf_data)-4] = srd_bd_addr[0] >> 24;
    manuf_data[ sizeof(manuf_data)-3] = srd_bd_addr[0] >> 16;
    manuf_data[ sizeof(manuf_data)-2] = srd_bd_addr[0] >> 8;
    manuf_data[ sizeof(manuf_data)-1] = srd_bd_addr[0];
    
    ret = aci_hal_write_config_data( CONFIG_DATA_RANDOM_ADDRESS_OFFSET, CONFIG_DATA_RANDOM_ADDRESS_LEN, (uint8_t*)srd_bd_addr );

    if (ret != BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET, result: 0x%x \n", ret);
    }
    else
    {
      APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_RANDOM_ADDRESS_OFFSET\n");
      APP_DBG_MSG("  Random Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n", (uint8_t)(srd_bd_addr[1] >> 8),
                                                                                 (uint8_t)(srd_bd_addr[1]),
                                                                                 (uint8_t)(srd_bd_addr[0] >> 24),
                                                                                 (uint8_t)(srd_bd_addr[0] >> 16),
                                                                                 (uint8_t)(srd_bd_addr[0] >> 8),
                                                                                 (uint8_t)(srd_bd_addr[0]));
    }
    
  }

  /**
   * Write Identity root key used to derive IRK and DHK(Legacy)
   */
  aci_hal_write_config_data(CONFIG_DATA_IR_OFFSET,
                            CONFIG_DATA_IR_LEN,
                            (uint8_t*) BLE_CFG_IR_VALUE);

   /**
   * Write Encryption root key used to derive LTK and CSRK
   */
  aci_hal_write_config_data(CONFIG_DATA_ER_OFFSET,
                            CONFIG_DATA_ER_LEN,
                            (uint8_t*) BLE_CFG_ER_VALUE);


  /**
   * Write Identity root key used to derive IRK and DHK(Legacy)
   */
    aci_hal_write_config_data( CONFIG_DATA_IR_OFFSET, CONFIG_DATA_IR_LEN, (uint8_t*)BLE_CFG_IR_VALUE );

   /**
   * Write Encryption root key used to derive LTK and CSRK
   */
    aci_hal_write_config_data( CONFIG_DATA_ER_OFFSET, CONFIG_DATA_ER_LEN, (uint8_t*)BLE_CFG_ER_VALUE );

  /**
   * Set TX Power.
   */
    aci_hal_set_tx_power_level(1, global_tx_power_code);

  /**
   * Set adv name.
   */
    strcpy(local_name_var + 1, global_name);

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
#endif /* BLE_CFG_CENTRAL == 1 */

/* USER CODE BEGIN Role_Mngt*/

/* USER CODE END Role_Mngt */

  if (role > 0)
  {
    aci_gap_init(role, 0,
                 APPBLE_GAP_DEVICE_NAME_LENGTH,
                 &gap_service_handle, &gap_dev_name_char_handle, &gap_appearance_char_handle);

    if (aci_gatt_update_char_value(gap_service_handle, gap_dev_name_char_handle, 0, strlen(local_name_var)-1, (uint8_t *) &(local_name_var[1])))
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

  Custom_SVC_Init();
  /**
   * Initialize IO capability
   */
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability = global_io_capability;

  aci_gap_set_io_capability(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.ioCapability);

  /**
   * Initialize authentication
   */
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode = global_mitm;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.sc_support = global_scsupport;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = CFG_ENCRYPTION_KEY_SIZE_MIN;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = CFG_ENCRYPTION_KEY_SIZE_MAX;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin = global_usefixedpin;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin = global_fixedpin;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode = global_bonding;

  aci_gap_set_authentication_requirement(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.sc_support,
                                         CFG_KEYPRESS_NOTIFICATION_SUPPORT,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin,
                                         global_ble_addr_type | global_ble_cfg_addr_type
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
  uint16_t Min_Inter, Max_Inter;

  if (New_Status == APP_BLE_FAST_ADV)
  {
    Min_Inter = AdvIntervalMin;
    Max_Inter = AdvIntervalMax;
  }
  else
  {
    Min_Inter = CFG_LP_CONN_ADV_INTERVAL_MIN;
    Max_Inter = CFG_LP_CONN_ADV_INTERVAL_MAX;
  }

    APP_DBG_MSG("First index in %d state \n", BleApplicationContext.Device_Connection_Status);

    if ((New_Status == APP_BLE_LP_ADV)
        && ((BleApplicationContext.Device_Connection_Status == APP_BLE_FAST_ADV)
            || (BleApplicationContext.Device_Connection_Status == APP_BLE_LP_ADV)))
    {
      /* Connection in ADVERTISE mode have to stop the current advertising */
      ret = aci_gap_set_non_discoverable();
      if (ret == BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("Successfully Stopped Advertising \n");
      }
      else
      {
        APP_DBG_MSG("Stop Advertising Failed , result: %d \n", ret);
      }
    }

    BleApplicationContext.Device_Connection_Status = New_Status;
    /* Start Fast or Low Power Advertising */
    ret = aci_gap_set_discoverable(
        ADV_IND,
        Min_Inter,
        Max_Inter,
        global_ble_addr_type | global_ble_cfg_addr_type,
        NO_WHITE_LIST_USE, /* use white list */
        strlen(local_name_var),
        (uint8_t*) &local_name_var,
        BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen,
        BleApplicationContext.BleApplicationContext_legacy.advtServUUID,
        0,
        0);

    /* Update Advertising data */
    ret = aci_gap_update_adv_data(sizeof(manuf_data), (uint8_t*) manuf_data);
    if (ret == BLE_STATUS_SUCCESS)
    {
      if (New_Status == APP_BLE_FAST_ADV)
      {
        APP_DBG_MSG("Successfully Start Fast Advertising \n" );
      }
      else
      {
        APP_DBG_MSG("Successfully Start Low Power Advertising \n");
      }
    }
    else
    {
      if (New_Status == APP_BLE_FAST_ADV)
      {
        APP_DBG_MSG("Start Fast Advertising Failed , result: %d \n", ret);
      }
      else
      {
        APP_DBG_MSG("Start Low Power Advertising Failed , result: %d \n", ret);
      }
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
 *SPECIFIC FUNCTIONS
 *
 *************************************************************/
static void Add_Advertisment_Service_UUID( uint16_t servUUID )
{
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] =
      (uint8_t) (servUUID & 0xFF);
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] =
      (uint8_t) (servUUID >> 8) & 0xFF;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;

  return;
}

/*************************************************************
 *
 *SPECIFIC FUNCTIONS FOR P2P SERVER
 *
 *************************************************************/
static void Switch_OFF_GPIO(){
/* USER CODE BEGIN Switch_OFF_GPIO */
  BSP_LED_Off(LED_GREEN);
/* USER CODE END Switch_OFF_GPIO */
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
    uint16_t peripheral_latency = L2CAP_PERIPHERAL_LATENCY;
    uint16_t timeout_multiplier = L2CAP_TIMEOUT_MULTIPLIER;
    tBleStatus result;

    result = aci_l2cap_connection_parameter_update_req(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,
                                                       interval_min, interval_max,
                                                       peripheral_latency, timeout_multiplier);
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

static void Manage_Service_Init(void)
{
  /* if P2P_server */
  if (global_svc_index == 1)
  {
    strcpy(global_name, "P2PSRV1");
    local_name_var[0] = AD_TYPE_COMPLETE_LOCAL_NAME;
    strcpy(&local_name_var[1], global_name);
    
    manuf_data[0] = sizeof(manuf_data)-1;
    manuf_data[1] = AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
    manuf_data[2] = 0x01;
    manuf_data[3] = CFG_DEV_ID_P2P_SERVER1;
    manuf_data[4] = 0x00;
    manuf_data[5] = 0x00;
    manuf_data[6] = 0x00;
    manuf_data[7] = 0x00;
    manuf_data[8] = 0x00;
    manuf_data[9] = 0x00;
    manuf_data[10] = 0x00;
    manuf_data[11] = 0x00;
    manuf_data[12] = 0x00;
    manuf_data[13] = 0x00;
  }
  /* if Heart rate */
  else if (global_svc_index == 2)
  {
    strcpy(global_name, "HRSTM");
    local_name_var[0] = AD_TYPE_COMPLETE_LOCAL_NAME;
    strcpy(&local_name_var[1], global_name);

    manuf_data[0] = sizeof(manuf_data)-1;
    manuf_data[1] = AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
    manuf_data[2] = 0x01;
    manuf_data[3] = 0x00;
    manuf_data[4] = 0x00;
    manuf_data[5] = 0x00;
    manuf_data[6] = 0x00;
    manuf_data[7] = 0x00;
    manuf_data[8] = 0x00;
    manuf_data[9] = 0x00;
    manuf_data[10] = 0x00;
    manuf_data[11] = 0x00;
    manuf_data[12] = 0x00;
    manuf_data[13] = 0x00;
  }

  SVCCTL_Init();

  Ble_Hci_Gap_Gatt_Init();

  if (global_svc_index == 1)
  {
  /**
   * Initialize P2P Server Application
   */
   P2PS_STM_Init();
   P2PS_APP_Init();
   /**
   * Make device discoverable
   */
   BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = NULL;
   BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 0;

   /* Initialize intervals for reconnexion without intervals update */
   AdvIntervalMin = CFG_FAST_CONN_ADV_INTERVAL_MIN;
   AdvIntervalMax = CFG_FAST_CONN_ADV_INTERVAL_MAX;
  }
  else if (global_svc_index == 2)
  {
    /**
    * Initialize DIS Application
    */
    DISAPP_Init();
    /**
    * Initialize HRS Application
    */
    HRS_Init();
    HRSAPP_Init();
    /**
    * Make device discoverable
    */
    BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = AD_TYPE_16_BIT_SERV_UUID;
    BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 1;
    Add_Advertisment_Service_UUID(HEART_RATE_SERVICE_UUID);

    /* Initialize intervals for reconnexion without intervals update */
    AdvIntervalMin = CFG_FAST_CONN_ADV_INTERVAL_MIN;
    AdvIntervalMax = CFG_FAST_CONN_ADV_INTERVAL_MAX;
  }
  
  Adv_Request(APP_BLE_FAST_ADV);
  
  return;
}

static void Manage_Start_Stop_Adv_Init(void)
{
  tBleStatus result;
 
  /* stop advertising */
  if (global_adv_enable == 0)
  {
    result = aci_gap_set_non_discoverable();
    if (result == BLE_STATUS_SUCCESS)
    {
      printf("non discoverable, Successfully \r\n\r");
    }
    else
    {
      printf("non discoverable, Failed \r\n\r");
    }
  }
  /* start advertising */
  else if (global_adv_enable == 1)
  {
    result = aci_gap_set_discoverable(global_adv_type, 
                                      global_adv_int_min, 
                                      global_adv_int_max, 
                                      global_ble_addr_type | global_ble_cfg_addr_type, 
                                      0x00, 
                                      strlen(local_name_var),
                                      (uint8_t*) &local_name_var,
                                      BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen,
                                      BleApplicationContext.BleApplicationContext_legacy.advtServUUID, 0x0000, 0x0000);
    if (result == BLE_STATUS_SUCCESS)
    {
      printf("set discoverable, Successfully \r\n\r");
    }
    else
    {
      printf("set discoverable, Failed 0x%x\r\n\r", result);
    }
  }
  
  return;
}

static void Manage_Custom_Service_Init(void)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  uint8_t service_uuid[16] = {0x8f, 0xe5, 0xb3, 0xd5, 0x2e, 0x7f, 0x4a, 0x98, 0x2a, 0x48, 0x7a, 0xcc, 0x00, 0x00, 0x00, 0x00};
  
  service_uuid[12] = (uint8_t)(global_svc_uuid);
  service_uuid[13] = (uint8_t)(global_svc_uuid >> 8);
  
  Custom_SVC_Init();
    
  if (global_svc_uuid_type == UUID_TYPE_16)
  {
    result = aci_gatt_add_service(global_svc_uuid_type, (Service_UUID_t *)&global_svc_uuid, 
                         PRIMARY_SERVICE, global_max_attr, &(Custom_Context.SvcHdle[global_svc_index - CUSTOM_APP_SVC_OFFSET]));
  }
  else if (global_svc_uuid_type == UUID_TYPE_128)
  {
    /* convert uuid_16b into uuid_128b */
    result = aci_gatt_add_service(global_svc_uuid_type, (Service_UUID_t *)service_uuid, 
                         PRIMARY_SERVICE, global_max_attr, &(Custom_Context.SvcHdle[global_svc_index - CUSTOM_APP_SVC_OFFSET]));
  }
  if (result == BLE_STATUS_SUCCESS)
  {
    printf("Add service, Successfully \r\n\r");
  }
  else
  {
    printf("Add service, Failed \r\n\r");
  }
  
  return;
}

static void Manage_Ble_Init(void)
{
  manuf_data[0] = sizeof(manuf_data)-1;
  manuf_data[1] = AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
  manuf_data[2] = 0x01;
  manuf_data[3] = 0x00;
  manuf_data[4] = 0x00;
  manuf_data[5] = 0x00;
  manuf_data[6] = 0x00;
  manuf_data[7] = 0x00;
  manuf_data[8] = 0x00;
  manuf_data[9] = 0x00;
  manuf_data[10] = 0x00;
  manuf_data[11] = 0x00;
  manuf_data[12] = 0x00;
  manuf_data[13] = 0x00;
    
  Ble_Hci_Gap_Gatt_Init();
  
  return;
}

static void Manage_Charac_Init(void)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  uint8_t char_uuid[16] = {0x19, 0xed, 0x82, 0xae, 0xed, 0x21, 0x4c, 0x9d, 0x41, 0x45, 0x22, 0x8e, 0x00, 0x00, 0x00, 0x00};
  
  char_uuid[12] = (uint8_t)(global_char_uuid);
  char_uuid[13] = (uint8_t)(global_char_uuid >> 8);
  
  if (global_char_uuid_type == UUID_TYPE_16)
  {
    result = aci_gatt_add_char( Custom_Context.SvcHdle[global_svc_id - CUSTOM_APP_SVC_OFFSET], global_char_uuid_type, (Char_UUID_t *)&global_char_uuid, global_char_value_len,
                               global_char_properties, global_char_sec_permission, global_gatt_event_mask,
                               10, 1, &(Custom_Context.CharHdle[global_svc_id - CUSTOM_APP_SVC_OFFSET][global_char_id - 1]));
  }
  else if (global_char_uuid_type == UUID_TYPE_128)
  {
    /* convert uuid_16b into uuid_128b */
    result = aci_gatt_add_char( Custom_Context.SvcHdle[global_svc_id - CUSTOM_APP_SVC_OFFSET], global_char_uuid_type, (Char_UUID_t *)char_uuid, global_char_value_len,
                               global_char_properties, global_char_sec_permission, global_gatt_event_mask,
                               10, 1, &(Custom_Context.CharHdle[global_svc_id - CUSTOM_APP_SVC_OFFSET][global_char_id - 1]));
  }
  if (result == BLE_STATUS_SUCCESS)
  {
    printf("Add characteristic, Successfully \r\n\r");
  }
  else
  {
    printf("Add characteristic, Failed \r\n\r");
  }
  
  return;
}

static void Manage_Peripheral_Security_Request(void)
{
  tBleStatus result;
  
  result = aci_gap_slave_security_req(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
  if( result == BLE_STATUS_SUCCESS )
  {
    printf("Peripheral_security_req(), Successfully \r\n\r");
  }
  else
  {
    printf("Peripheral_security_req(), Failed \r\n\r");
  }
  
  return;
}

static void Manage_Pairing_Confirm_Answer(void)
{
  tBleStatus result;
  
  result = aci_gap_numeric_comparison_value_confirm_yesno(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,
                                                           global_pairing_confirm);
  if( result == BLE_STATUS_SUCCESS )
  {
    printf("Pairing_Confirm_Answer(), Successfully \r\n\r");
  }
  else
  {
    printf("Pairing_Confirm_Answer(), Failed \r\n\r");
  }
  
  return;
}

static void Manage_Pass_Key_Resp(void)
{
  tBleStatus result;
  
  result = aci_gap_pass_key_resp(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,
                                 global_pass_key_resp);
  
  if( result == BLE_STATUS_SUCCESS )
  {
    printf("Pass_Key_Resp(), Successfully \r\n\r");
  }
  else
  {
    printf("Pass_Key_Resp(), Failed \r\n\r");
  }
  
  return;
}

static void Manage_IO_Capability(void)
{
  tBleStatus result;
  
  result = aci_gap_set_io_capability(global_io_capability);
  if( result == BLE_STATUS_SUCCESS )
  {
    printf("IO_Capability(), Successfully \r\n\r");
  }
  else
  {
    printf("IO_Capability(), Failed \r\n\r");
  }
  
  return;
}

static void Manage_set_authen_req(void)
{
  tBleStatus result;
  
  result = aci_gap_set_authentication_requirement(global_bonding,global_mitm,global_scsupport,0x00,0x07,0x10,
                                                  global_usefixedpin,global_fixedpin,global_addrtype);
  if( result == BLE_STATUS_SUCCESS )
  {
    printf("set_authentication_req(), Successfully \r\n\r");
  }
  else
  {
    printf("set_authentication_req(), Failed \r\n\r");
  }
  
  return;
}

static void Manage_clear_db(void)
{
  tBleStatus result;
  
  result = aci_gap_clear_security_db();
  if( result == BLE_STATUS_SUCCESS )
  {
    printf("Clear_security_db(), Successfully \r\n\r");
  }
  else
  {
    printf("Clear_security_db(), Failed \r\n\r");
  }
  
  return;
}

static void Manage_hci_reset(void)
{
  hci_reset();
  
  return;
}

static void Manage_Update_Connection(void)
{
  tBleStatus result;

  result = aci_l2cap_connection_parameter_update_req(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,
                                                     (uint16_t)(global_conn_int_min / 1.25f),
                                                     (uint16_t)(global_conn_int_max / 1.25f),
                                                     L2CAP_PERIPHERAL_LATENCY, L2CAP_TIMEOUT_MULTIPLIER);
  if( result == BLE_STATUS_SUCCESS )
  {
    printf("BLE_SVC_L2CAP_Conn_Update(), Successfully \r\n\r");
  }
  else
  {
    printf("BLE_SVC_L2CAP_Conn_Update(), Failed \r\n\r");
  }
  
  return;
}

static void Manage_Update_Charac(void)
{
  tBleStatus result;

  if (global_notif_serv_index == 1)
  {
    if (global_notif_char_index == 2)
    {
      if (global_notif_val_tab[0] == 1)
      {
        P2PS_Send_Notification_ATcommand();
      }
    }
  }
  else if (global_notif_serv_index == 2)
  {
    if (global_notif_char_index == 1)
    {
      HRSAPP_Context.MeasurementvalueChar.MeasurementValue = global_notif_val_tab[0];
      HRSAPP_Context.MeasurementvalueChar.EnergyExpended = 0;
    }
    else if (global_notif_char_index == 3)
    {
      HRSAPP_Context.MeasurementvalueChar.EnergyExpended = global_notif_val_tab[0];
    }
    result = HRS_UpdateChar(HEART_RATE_MEASURMENT_UUID, (uint8_t*)&HRSAPP_Context.MeasurementvalueChar);
    if (result == BLE_STATUS_SUCCESS)
    {
      printf("HRS_UpdateChar, Successfully \r\n\r");
    }
    else
    {
      printf("HRS_UpdateChar, Failed \r\n\r");
    }
  }
  else
  {
    Custom_Send_Notification_ATcommand();
  }
  
  return;
}

static void Manage_Indication_Update_Charac(void)
{
  Custom_Send_Indication_ATcommand();
  
  return;
}
//#endif
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

void SVCCTL_SvcInit(void)
{
  return;
}

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
