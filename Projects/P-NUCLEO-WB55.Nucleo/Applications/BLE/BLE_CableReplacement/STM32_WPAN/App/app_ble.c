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

#include "crs_app.h"
#include "crc_app.h"
#include "DispTools.h"

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
  /* USER CODE BEGIN tSecurityParams*/

  /* USER CODE END tSecurityParams */
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
  /* USER CODE BEGIN BleGlobalContext_t*/

  /* USER CODE END BleGlobalContext_t */
} BleGlobalContext_t;

typedef struct
{
  BleGlobalContext_t BleApplicationContext_legacy;
  APP_BLE_ConnStatus_t Device_Connection_Status;

  /**
   * ID of the Advertising Timeout
   */
  uint8_t Advertising_mgr_timer_Id;
  /* USER CODE BEGIN PTD_1*/
  uint8_t DeviceServerFound;
  /* USER CODE END PTD_1 */
} BleApplicationContext_t;

enum
{
  NO_DEVICE_FOUND,
  AT_LEAST_ONE_DEVICE_FOUND
};

typedef enum
{
  GAP_PROC_PAIRING,
  GAP_PROC_SET_PHY,
} GapProcId_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define FAST_ADV_TIMEOUT              (30*1000*1000/CFG_TS_TICK_VAL) /**< 30s */
#define INITIAL_ADV_TIMEOUT           (60*1000*1000/CFG_TS_TICK_VAL) /**< 60s */

#define BD_ADDR_SIZE_LOCAL    6

/* USER CODE BEGIN PD */
#define LED_ON_TIMEOUT             (0.005*1000*1000/CFG_TS_TICK_VAL) /**< 5ms */
#define CONN_P1_7_5                                                (CONN_P(7.5))
#define CONN_P2_7_5                                                (CONN_P(7.5))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM1") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;

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

/**
*   Identity root key used to derive IRK and DHK(Legacy) 
*/
static const uint8_t a_BLE_CfgIrValue[16] = CFG_BLE_IR;

/**
* Encryption root key used to derive LTK(Legacy) and CSRK
*/
static const uint8_t a_BLE_CfgErValue[16] = CFG_BLE_ER;

static BleApplicationContext_t BleApplicationContext;
#if(BLE_CFG_PERIPHERAL != 0)
static uint16_t AdvIntervalMin, AdvIntervalMax;
static const char a_LocalName[] = {AD_TYPE_COMPLETE_LOCAL_NAME , 'C', 'R', 'S'};
#endif

uint8_t a_ManufData[14] = 
{
  sizeof(a_ManufData)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
  0x01/*SKD version */,
  CFG_DEV_ID_CR_SERVER /* STM32WB - CR Server*/,
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

/* Global variables ----------------------------------------------------------*/
tBDAddr SERVER_REMOTE_BDADDR;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * p_Payload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t Status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress( void );
#if (BLE_CFG_PERIPHERAL != 0)
static void Adv_Request(APP_BLE_ConnStatus_t NewStatus);
static void Adv_Mgr(void);
static void Adv_Update(void);
//static void DataThroughput_proc(void);
#endif
#if (BLE_CFG_CENTRAL != 0)
static void Connect_Request(void);
static void Scan_Request(void);
#endif
static void LinkConfiguration(void);


/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Init( void )
{
  SHCI_CmdStatus_t status;
#if (RADIO_ACTIVITY_EVENT != 0)
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
#endif /* RADIO_ACTIVITY_EVENT != 0 */
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
#if(BLE_CFG_PERIPHERAL != 0)
  UTIL_SEQ_RegTask(1<<CFG_TASK_ADV_UPDATE_ID, UTIL_SEQ_RFU, Adv_Update);
#endif

#if(BLE_CFG_CENTRAL != 0)
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SCAN_REQ_ID, UTIL_SEQ_RFU, Scan_Request);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONN_REQ_ID, UTIL_SEQ_RFU, Connect_Request);
#endif

  UTIL_SEQ_RegTask( 1<<CFG_TASK_LINK_CONFIG_ID, UTIL_SEQ_RFU, LinkConfiguration);

  BleApplicationContext.DeviceServerFound = NO_DEVICE_FOUND;

  /**
   * Initialization of the BLE App Context
   */
#if (RADIO_ACTIVITY_EVENT != 0)
  ret = aci_hal_set_radio_activity_mask(0x0006);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_set_radio_activity_mask command, result: 0x%x \n\r", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_set_radio_activity_mask command\n\r");
  }
#endif /* RADIO_ACTIVITY_EVENT != 0 */

#if (BLE_CFG_CENTRAL == 1)
  /**
   * Initialize CRC Application
   */
  CRCAPP_Init();
#endif

#if (BLE_CFG_PERIPHERAL == 1)
  /**
   * Initialize CRS Application
   */
  CRSAPP_Init();
#endif

  /* USER CODE BEGIN APP_BLE_Init_3 */

  /* USER CODE END APP_BLE_Init_3 */

  /**
   * Make device discoverable
   */
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = NULL;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 0;
#if(BLE_CFG_PERIPHERAL != 0)
  /**
   * Create timer to handle the connection state machine
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BleApplicationContext.Advertising_mgr_timer_Id), hw_ts_SingleShot, Adv_Mgr);

  /* Initialize intervals for reconnexion without intervals update */
  AdvIntervalMin = CFG_FAST_CONN_ADV_INTERVAL_MIN;
  AdvIntervalMax = CFG_FAST_CONN_ADV_INTERVAL_MAX;

  /**
   * Start to Advertise to be connected by CR Client
   */
  Adv_Request(APP_BLE_FAST_ADV);
#endif

#if(BLE_CFG_CENTRAL != 0)
  /* Start Scan to connect to DT server */
  UTIL_SEQ_SetTask(1<<CFG_TASK_SCAN_REQ_ID, CFG_SCH_PRIO_0);
#endif

  /* USER CODE BEGIN APP_BLE_Init_2 */

  /* USER CODE END APP_BLE_Init_2 */

  return;
}

static void LinkConfiguration(void)
{
  CRCAPP_LinkReadyNotification(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);

  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *p_Pckt )
{
  hci_event_pckt *p_event_pckt;

  p_event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) p_Pckt)->data;

  /* USER CODE BEGIN SVCCTL_App_Notification */

  /* USER CODE END SVCCTL_App_Notification */

  switch (p_event_pckt->evt)
  {
    case HCI_DISCONNECTION_COMPLETE_EVT_CODE:
      {
        hci_disconnection_complete_event_rp0 *p_disconnection_complete_event;
        p_disconnection_complete_event = (hci_disconnection_complete_event_rp0 *) p_event_pckt->data;

        if (p_disconnection_complete_event->Connection_Handle == BleApplicationContext.BleApplicationContext_legacy.connectionHandle)
        {
          BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0;
          BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
          APP_DBG_MSG(">>== HCI_DISCONNECTION_COMPLETE_EVT_CODE\n");
          APP_DBG_MSG("     - Connection Handle:   0x%x\n     - Reason:    0x%x\n\r",
                      p_disconnection_complete_event->Connection_Handle,
                      p_disconnection_complete_event->Reason);
        }

        /* restart advertising */
#if(BLE_CFG_PERIPHERAL != 0)
        Adv_Request(APP_BLE_FAST_ADV);
#endif      
      }   
      /* USER CODE BEGIN EVT_DISCONN_COMPLETE */

      /* USER CODE END EVT_DISCONN_COMPLETE */
      break; /* HCI_DISCONNECTION_COMPLETE_EVT_CODE */

    case HCI_LE_META_EVT_CODE:
      {
        evt_le_meta_event * p_meta_evt;
        float Connection_Interval;
        float Supervision_Timeout;
        
        p_meta_evt = (evt_le_meta_event*) p_event_pckt->data;
        /* USER CODE BEGIN EVT_LE_META_EVENT */

        /* USER CODE END EVT_LE_META_EVENT */
        switch (p_meta_evt->subevent)
        {
          case HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE:
            {
              hci_le_connection_update_complete_event_rp0 *p_connection_update_complete_event;
              
              p_connection_update_complete_event = (hci_le_connection_update_complete_event_rp0 *) p_meta_evt->data;
              APP_DBG_MSG(">>== HCI_LE_CONNECTION_UPDATE_COMPLETE_SUBEVT_CODE\n");
              APP_DBG_MSG("     - Connection Interval:   %.2f ms\n     - Connection latency:    %d\n     - Supervision Timeout: %d ms\n\r",
                           p_connection_update_complete_event->Conn_Interval*1.25,
                           p_connection_update_complete_event->Conn_Latency,
                           p_connection_update_complete_event->Supervision_Timeout*10);

              /* USER CODE BEGIN EVT_LE_CONN_UPDATE_COMPLETE */

              /* USER CODE END EVT_LE_CONN_UPDATE_COMPLETE */
            }
            break;

          case HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE:
            {
              hci_le_phy_update_complete_event_rp0 *p_evt_le_phy_update_complete;
              tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
              uint8_t Tx_phy, Rx_phy;

              p_evt_le_phy_update_complete = (hci_le_phy_update_complete_event_rp0*)p_meta_evt->data;
              APP_DBG_MSG("==>> HCI_LE_PHY_UPDATE_COMPLETE_SUBEVT_CODE - ");
              if (p_evt_le_phy_update_complete->Status == 0)
              {
                APP_DBG_MSG("status ok \n");
              }
              else
              {
                APP_DBG_MSG("status nok \n");
              }

              ret = hci_le_read_phy(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,&Tx_phy,&Rx_phy);
              if (ret != BLE_STATUS_SUCCESS)
              {
                APP_DBG_MSG("==>> hci_le_read_phy : fail\n\r");
              }
              else
              {
                APP_DBG_MSG("==>> hci_le_read_phy - Success \n");

                if ((Tx_phy == TX_2M) && (Rx_phy == RX_2M))
                {
                  APP_DBG_MSG("==>> PHY Param  TX= %d, RX= %d \n\r", Tx_phy, Rx_phy);
                }
                else
                {
                  APP_DBG_MSG("==>> PHY Param  TX= %d, RX= %d \n\r", Tx_phy, Rx_phy);
                }
              }
              UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_GAP_PROC_COMPLETE);
              /* USER CODE BEGIN EVT_LE_PHY_UPDATE_COMPLETE */

              /* USER CODE END EVT_LE_PHY_UPDATE_COMPLETE */
            }
            break;

          case HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE:
            {
              hci_le_connection_complete_event_rp0 *p_connection_complete_event;

              /**
               * The connection is done, there is no need anymore to schedule the LP ADV
               */
              p_connection_complete_event = (hci_le_connection_complete_event_rp0 *) p_meta_evt->data;

#if(BLE_CFG_PERIPHERAL != 0)
//              HW_TS_Stop(BleApplicationContext.Advertising_mgr_timer_Id);

              APP_DBG_MSG(">>== HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE - Connection handle: 0x%x\n", p_connection_complete_event->Connection_Handle);
              APP_DBG_MSG("     - Connection established with Central: @:%02x:%02x:%02x:%02x:%02x:%02x\n",
                          p_connection_complete_event->Peer_Address[5],
                          p_connection_complete_event->Peer_Address[4],
                          p_connection_complete_event->Peer_Address[3],
                          p_connection_complete_event->Peer_Address[2],
                          p_connection_complete_event->Peer_Address[1],
                          p_connection_complete_event->Peer_Address[0]);
              APP_DBG_MSG("     - Connection Interval:   %.2f ms\n     - Connection latency:    %d\n     - Supervision Timeout: %d ms\n\r",
                          p_connection_complete_event->Conn_Interval*1.25,
                          p_connection_complete_event->Conn_Latency,
                          p_connection_complete_event->Supervision_Timeout*10
                         );

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
#endif
              BleApplicationContext.BleApplicationContext_legacy.connectionHandle = p_connection_complete_event->Connection_Handle;

#if(BLE_CFG_CENTRAL != 0)
              APP_DBG_MSG("CONNECTION COMPLETED - CENTRAL \n");
#endif
              Connection_Interval = p_connection_complete_event->Conn_Interval * 1.25;
            
              APP_DBG_MSG("**INTERVAL = %.2f ms \n",(double)Connection_Interval);
              APP_DBG_MSG("**LATENCY = 0x%x \n",p_connection_complete_event->Conn_Latency);
              Supervision_Timeout = p_connection_complete_event->Supervision_Timeout * 10;
              APP_DBG_MSG("**SUPERVISION_TIMEOUT = %.2f ms \n",(double)Supervision_Timeout);
              APP_DBG_MSG("\r\n\r");
            
              UTIL_SEQ_SetTask(1 << CFG_TASK_LINK_CONFIG_ID, CFG_SCH_PRIO_0);
              /* USER CODE BEGIN HCI_EVT_LE_CONN_COMPLETE */

              /* USER CODE END HCI_EVT_LE_CONN_COMPLETE */
            }
            break; /* HCI_LE_CONNECTION_COMPLETE_SUBEVT_CODE */
            
          case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE:
            {
              hci_le_enhanced_connection_complete_event_rp0 *p_enhanced_connection_complete_event;
              p_enhanced_connection_complete_event = (hci_le_enhanced_connection_complete_event_rp0 *) p_meta_evt->data;
              
              /**
              * The connection is done, there is no need anymore to schedule the LP ADV
              */
              
              HW_TS_Stop(BleApplicationContext.Advertising_mgr_timer_Id);
              
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
              BleApplicationContext.BleApplicationContext_legacy.connectionHandle = p_enhanced_connection_complete_event->Connection_Handle;
              /* USER CODE BEGIN HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE */
              
              /* USER CODE END HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE */
              break; /* HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVT_CODE */
            }

          case HCI_LE_ADVERTISING_REPORT_SUBEVT_CODE:
            {
              hci_le_advertising_report_event_rp0 * p_le_advertising_event;
              uint8_t event_type, event_data_size;
              uint8_t *adv_report_data;
              int k = 0;

              p_le_advertising_event = (hci_le_advertising_report_event_rp0 *) p_meta_evt->data;

              event_type = p_le_advertising_event->Advertising_Report[0].Event_Type;

              event_data_size = p_le_advertising_event->Advertising_Report[0].Length_Data;

              adv_report_data = (uint8_t*)(&p_le_advertising_event->Advertising_Report[0].Length_Data) + 1;
              k = 0;

              /* search AD TYPE AD_TYPE_COMPLETE_LOCAL_NAME (Complete Local Name) */
              /* search AD Type AD_TYPE_16_BIT_SERV_UUID (16 bits UUIDS) */
              if (event_type == ADV_IND)
              {
                /*ISOLATION OF BD ADDRESS AND LOCAL NAME*/
                while(k < event_data_size)
                {
                  uint8_t adtype, adlength;
                  
                  adlength = adv_report_data[k];
                  adtype = adv_report_data[k + 1];
                  switch (adtype)
                  {
                    case AD_TYPE_FLAGS: /* now get flags */
                      break;

                    case AD_TYPE_TX_POWER_LEVEL: /* Tx power level */
                      break;

                    case AD_TYPE_MANUFACTURER_SPECIFIC_DATA: /* Manufacturer Specific */
                      {
                        if (adlength >= 7 && adv_report_data[k + 2] == 0x01)
                        { /* ST VERSION ID 01 */
                          APP_DBG_MSG("--- ST MANUFACTURER ID --- \n");
                          switch (adv_report_data[k + 3])
                          { /* Demo ID */
                            case CFG_DEV_ID_CR_SERVER:   /* (Periph Server CR) */
                              {
                                APP_DBG_MSG("-- SERVER DETECTED -- VIA MAN ID\n");
                                BleApplicationContext.DeviceServerFound = AT_LEAST_ONE_DEVICE_FOUND;
                                SERVER_REMOTE_BDADDR[0] = p_le_advertising_event->Advertising_Report[0].Address[0];
                                SERVER_REMOTE_BDADDR[1] = p_le_advertising_event->Advertising_Report[0].Address[1];
                                SERVER_REMOTE_BDADDR[2] = p_le_advertising_event->Advertising_Report[0].Address[2];
                                SERVER_REMOTE_BDADDR[3] = p_le_advertising_event->Advertising_Report[0].Address[3];
                                SERVER_REMOTE_BDADDR[4] = p_le_advertising_event->Advertising_Report[0].Address[4];
                                SERVER_REMOTE_BDADDR[5] = p_le_advertising_event->Advertising_Report[0].Address[5];

                                /* The device has been found - scan may be stopped */
                                aci_gap_terminate_gap_proc(GAP_GENERAL_DISCOVERY_PROC);
                              }
                              break;

                            default:
                              break;
                          }
                        }
                      }
                      break;

                    case AD_TYPE_SERVICE_DATA: /* service data 16 bits */
                      break;

                    default:
                      break;
                  } /* end switch adtype */
                  k += adlength + 1;
                } /* end while */

              } /*end if ADV_IND */
            }
            break;

          /* USER CODE BEGIN META_EVT */

          /* USER CODE END META_EVT */

          default:
            {
              /* USER CODE BEGIN SUBEVENT_DEFAULT */

              /* USER CODE END SUBEVENT_DEFAULT */
            }
            break;
        }
      }
      break; /* HCI_LE_META_EVT_CODE */
          
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      {
        evt_blecore_aci * p_blecore_evt;
        
        p_blecore_evt = (evt_blecore_aci*) p_event_pckt->data;
        /* USER CODE BEGIN EVT_VENDOR */

        /* USER CODE END EVT_VENDOR */
        switch (p_blecore_evt->ecode)
        {
          case ACI_GAP_PROC_COMPLETE_VSEVT_CODE:
          {
            aci_gap_proc_complete_event_rp0 *p_gap_evt_proc_complete = (void*) p_blecore_evt->data;
            /* CHECK GAP GENERAL DISCOVERY PROCEDURE COMPLETED & SUCCEED */
            if (p_gap_evt_proc_complete->Procedure_Code == GAP_GENERAL_DISCOVERY_PROC)
            {
              if( p_gap_evt_proc_complete->Status != BLE_STATUS_SUCCESS )
              {
                APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE FAILED 0x%x\n",
                             p_gap_evt_proc_complete->Status);
              }
              else
              {
                BSP_LED_Off(LED_BLUE);

                /*if a device found, connect to it, device 1 being chosen first if both found*/
                if (BleApplicationContext.DeviceServerFound != NO_DEVICE_FOUND)
                {
                  APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE COMPLETED\n");
                  UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_REQ_ID, CFG_SCH_PRIO_0);
                }
                else
                {
                  APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE COMPLETED WITH NO DEVICE FOUND\n");
                }
              }
            }
          }
          break; /* ACI_GAP_PROC_COMPLETE_VSEVT_CODE */

          case ACI_GAP_PAIRING_COMPLETE_VSEVT_CODE:
          {
            APP_DBG_MSG("Pairing complete \n");
            UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_GAP_PROC_COMPLETE);
          }
          break;

          case ACI_GAP_PASS_KEY_REQ_VSEVT_CODE:
          {
            APP_DBG_MSG("PAIRING PHASE - PASSKEY REQUEST 111111 \n");
            aci_gap_pass_key_resp(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, 111111);
          }
          break;
        
          case (ACI_GAP_NUMERIC_COMPARISON_VALUE_VSEVT_CODE):
          {
            APP_DBG_MSG("PAIRING PHASE - NUMERIC COMPARISON \r\n");
            APP_DBG_MSG("numeric_value = %ld\n",
                        ((aci_gap_numeric_comparison_value_event_rp0 *)(p_blecore_evt->data))->Numeric_Value);
            APP_DBG_MSG("Hex_value = %lx\n",
                        ((aci_gap_numeric_comparison_value_event_rp0 *)(p_blecore_evt->data))->Numeric_Value);

            aci_gap_numeric_comparison_value_confirm_yesno(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, YES); /* CONFIRM_YES = 1 */

            APP_DBG_MSG("\r\n\r** CONFIRM YES \n");
          }
          break;

  #if (RADIO_ACTIVITY_EVENT != 0)
          case ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE:
          {
            /* USER CODE BEGIN RADIO_ACTIVITY_EVENT*/
            BSP_LED_On(LED_GREEN);
            HW_TS_Start(BleApplicationContext.SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
            /* USER CODE END RADIO_ACTIVITY_EVENT*/
          }
          break; /* ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE */
  #endif /* RADIO_ACTIVITY_EVENT != 0 */

          case ACI_GATT_PROC_COMPLETE_VSEVT_CODE:
          {
            UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);
          }
          break; /*ACI_GATT_PROC_COMPLETE_VSEVT_CODE*/

          /* USER CODE BEGIN BLUE_EVT */

          /* USER CODE END BLUE_EVT */
          default:
            break;
        }
      }
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

    /* USER CODE BEGIN EVENT_PCKT */

    /* USER CODE END EVENT_PCKT */

    default:
      {
        /* USER CODE BEGIN ECODE_DEFAULT*/

        /* USER CODE END ECODE_DEFAULT*/
      }
      break;
  }

  return (SVCCTL_UserEvtFlowEnable);
}

APP_BLE_ConnStatus_t APP_BLE_Get_Client_Connection_Status(uint16_t Connection_Handle)
{
  return BleApplicationContext.Device_Connection_Status;
}

/* USER CODE BEGIN FD*/
void APP_BLE_Key_Button1_Action(void)
{
}

void APP_BLE_Key_Button2_Action(void)
{
}

void APP_BLE_Key_Button3_Action(void)
{
}

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

static void Ble_Hci_Gap_Gatt_Init(void)
{
  uint8_t role;
  uint16_t gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
  const uint8_t *p_bd_addr;

#if (CFG_BLE_ADDRESS_TYPE != GAP_PUBLIC_ADDR)
  uint32_t a_srd_bd_addr[2] = {0,0};
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
  p_bd_addr = BleGetBdAddress();
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBLIC_ADDRESS_OFFSET, 
                                  CONFIG_DATA_PUBLIC_ADDRESS_LEN, 
                                  (uint8_t*) p_bd_addr);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_hal_write_config_data command - CONFIG_DATA_PUBLIC_ADDRESS_OFFSET, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_hal_write_config_data command - CONFIG_DATA_PUBLIC_ADDRESS_OFFSET\n");
    APP_DBG_MSG("  Public Bluetooth Address: %02x:%02x:%02x:%02x:%02x:%02x\n",p_bd_addr[5],p_bd_addr[4],p_bd_addr[3],p_bd_addr[2],p_bd_addr[1],p_bd_addr[0]);
  }

#if (CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR)
  /* BLE MAC in ADV Packet */
  a_ManufData[ sizeof(a_ManufData)-6] = p_bd_addr[5];
  a_ManufData[ sizeof(a_ManufData)-5] = p_bd_addr[4];
  a_ManufData[ sizeof(a_ManufData)-4] = p_bd_addr[3];
  a_ManufData[ sizeof(a_ManufData)-3] = p_bd_addr[2];
  a_ManufData[ sizeof(a_ManufData)-2] = p_bd_addr[1];
  a_ManufData[ sizeof(a_ManufData)-1] = p_bd_addr[0];
#endif /* CFG_BLE_ADDRESS_TYPE == GAP_PUBLIC_ADDR */

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
#else
  /* Get RNG semaphore */
  while(LL_HSEM_1StepLock(HSEM, CFG_HW_RNG_SEMID));

  /* Enable RNG */
  __HAL_RNG_ENABLE(&hrng);

  /* Enable HSI48 oscillator */
  LL_RCC_HSI48_Enable();
  /* Wait until HSI48 is ready */
  while(! LL_RCC_HSI48_IsReady());

  if (HAL_RNG_GenerateRandomNumber(&hrng, &a_srd_bd_addr[1]) != HAL_OK)
  {
    /* Random number generation error */
    Error_Handler();
  }
  if (HAL_RNG_GenerateRandomNumber(&hrng, &a_srd_bd_addr[0]) != HAL_OK)
  {
    /* Random number generation error */
    Error_Handler();
  }
  a_srd_bd_addr[1] |= 0xC000; /* The two upper bits shall be set to 1 */

  /* Disable HSI48 oscillator */
  LL_RCC_HSI48_Disable();

  /* Disable RNG */
  __HAL_RNG_DISABLE(&hrng);

  /* Release RNG semaphore */
  LL_HSEM_ReleaseLock(HSEM, CFG_HW_RNG_SEMID, 0);
#endif /* CFG_STATIC_RANDOM_ADDRESS */
#endif

#if (CFG_BLE_ADDRESS_TYPE == GAP_STATIC_RANDOM_ADDR)
  /* BLE MAC in ADV Packet */
  a_ManufData[ sizeof(a_ManufData)-6] = a_srd_bd_addr[1] >> 8 ;
  a_ManufData[ sizeof(a_ManufData)-5] = a_srd_bd_addr[1];
  a_ManufData[ sizeof(a_ManufData)-4] = a_srd_bd_addr[0] >> 24;
  a_ManufData[ sizeof(a_ManufData)-3] = a_srd_bd_addr[0] >> 16;
  a_ManufData[ sizeof(a_ManufData)-2] = a_srd_bd_addr[0] >> 8;
  a_ManufData[ sizeof(a_ManufData)-1] = a_srd_bd_addr[0];
#endif

#if (CFG_BLE_ADDRESS_TYPE != GAP_PUBLIC_ADDR)
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
#endif /* CFG_BLE_ADDRESS_TYPE != GAP_PUBLIC_ADDR */

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
    const char *name = "CRS";
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
   * Initialize Default PHY
   */
  ret = hci_le_set_default_phy(ALL_PHYS_PREFERENCE,
                               TX_2M_PREFERRED,
                               RX_2M_PREFERRED);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : hci_le_set_default_phy command, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: hci_le_set_default_phy command\n");
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
  /* USER CODE BEGIN Ble_Hci_Gap_Gatt_Init_1*/

  /* USER CODE END Ble_Hci_Gap_Gatt_Init_1*/

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


#if (BLE_CFG_PERIPHERAL != 0)
static void Adv_Request( APP_BLE_ConnStatus_t NewStatus )
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  uint16_t Min_Inter, Max_Inter;

  if (NewStatus == APP_BLE_FAST_ADV)
  {
    Min_Inter = AdvIntervalMin;
    Max_Inter = AdvIntervalMax;
  }
  else
  {
    Min_Inter = CFG_LP_CONN_ADV_INTERVAL_MIN;
    Max_Inter = CFG_LP_CONN_ADV_INTERVAL_MAX;
  }

    /**
     * Stop the timer, it will be restarted for a new shot
     * It does not hurt if the timer was not running
     */
  HW_TS_Stop(BleApplicationContext.Advertising_mgr_timer_Id);

  if ((NewStatus == APP_BLE_LP_ADV)
      && ((BleApplicationContext.Device_Connection_Status == APP_BLE_FAST_ADV)
          || (BleApplicationContext.Device_Connection_Status == APP_BLE_LP_ADV)))
    {
      /* Connection in ADVERTISE mode have to stop the current advertising */
      ret = aci_gap_set_non_discoverable();
    if (ret != BLE_STATUS_SUCCESS)
      {
      APP_DBG_MSG("==>> aci_gap_set_non_discoverable - Stop Advertising Failed , result: %d \n", ret);
      }
      else
      {
      APP_DBG_MSG("==>> aci_gap_set_non_discoverable - Successfully Stopped Advertising \n");
      }
    }

  BleApplicationContext.Device_Connection_Status = NewStatus;
    /* Start Fast or Low Power Advertising */
  ret = aci_gap_set_discoverable(ADV_IND,
                                 Min_Inter,
                                 Max_Inter,
                                 CFG_BLE_ADDRESS_TYPE,
                                 NO_WHITE_LIST_USE, /* use white list */
                                 sizeof(a_LocalName),
                                 (uint8_t*) &a_LocalName,
                                 BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen,
                                 BleApplicationContext.BleApplicationContext_legacy.advtServUUID,
                                 0,
                                 0);
  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("==>> aci_gap_set_discoverable - fail, result: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("==>> aci_gap_set_discoverable - Success\n");
  }

  /* Update Advertising data */
  ret = aci_gap_update_adv_data(sizeof(a_ManufData), (uint8_t*) a_ManufData);
  if (ret != BLE_STATUS_SUCCESS)
  {
    if (NewStatus == APP_BLE_FAST_ADV)
    {
      APP_DBG_MSG("==>> Start Fast Advertising Failed , result: %d \n\r", ret);
    }
    else
    {
      APP_DBG_MSG("==>> Start Low Power Advertising Failed , result: %d \n\r", ret);
    }
  }
  else
  {
    if (NewStatus == APP_BLE_FAST_ADV)
    {
      APP_DBG_MSG("==>> Success: Start Fast Advertising \n\r");
      /* Start Timer to STOP ADV - TIMEOUT - and next Restart Low Power Advertising */
      HW_TS_Start(BleApplicationContext.Advertising_mgr_timer_Id, INITIAL_ADV_TIMEOUT);
    }
    else
    {
      APP_DBG_MSG("==>> Success: Start Low Power Advertising \n\r");
    }
  }
  
  return;
}

static void Adv_Mgr(void)
{
  /**
   * The code shall be executed in the background as an aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask(1 << CFG_TASK_ADV_UPDATE_ID, CFG_SCH_PRIO_0);

  return;
}

static void Adv_Update(void)
{
  Adv_Request(APP_BLE_LP_ADV);

  return;
}
#endif

const uint8_t* BleGetBdAddress( void )
{
  uint8_t *p_otp_addr;
  const uint8_t *p_bd_addr;
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

/* USER CODE BEGIN FD_LOCAL_FUNCTION */

/* USER CODE END FD_LOCAL_FUNCTION */

/* USER CODE BEGIN FD_SPECIFIC_FUNCTIONS */

/* USER CODE END FD_SPECIFIC_FUNCTIONS */

#if(BLE_CFG_CENTRAL != 0)
static void Scan_Request( void )
{
  tBleStatus result;

  if (BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
  {
    BSP_LED_On(LED_BLUE);
    result = aci_gap_start_general_discovery_proc(SCAN_P, SCAN_L, GAP_PUBLIC_ADDR, 1);
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG(" \r\n\r** START GENERAL DISCOVERY (SCAN) **  \r\n\r");
    }
    else
    {
      APP_DBG_MSG("-- aci_gap_start_general_discovery_proc, Failed 0x%x\r\n\r", result);
      BSP_LED_On(LED_RED);
    }
  }

  return;
}

static void Connect_Request( void )
{
  tBleStatus result;
  APP_DBG_MSG("\r\n\r** CREATE CONNECTION TO SERVER **  \r\n\r");

  result = aci_gap_create_connection(SCAN_P,
                                     SCAN_L,
                                     GAP_PUBLIC_ADDR,
                                     SERVER_REMOTE_BDADDR,
                                     GAP_PUBLIC_ADDR,
                                     CONN_P1_7_5,
                                     CONN_P2_7_5,
                                     0,
                                     0x3e8, 
                                     0x0000, 
                                     0x3E8);

  if (result != BLE_STATUS_SUCCESS)
  {
    BSP_LED_On(LED_RED);
  }

  return;
}
#endif

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void hci_notify_asynch_evt(void* p_Data)
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);

  return;
}

void hci_cmd_resp_release(uint32_t Flag)
{
  UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);

  return;
}

void hci_cmd_resp_wait(uint32_t Timeout)
{
  UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);

  return;
}

static void BLE_UserEvtRx(void *p_Payload)
{
  SVCCTL_UserEvtFlowStatus_t svctl_return_status;
  tHCI_UserEvtRxParam *p_param;

  p_param = (tHCI_UserEvtRxParam *)p_Payload;

  svctl_return_status = SVCCTL_UserEvtRx((void *)&(p_param->pckt->evtserial));
  if (svctl_return_status != SVCCTL_UserEvtFlowDisable)
  {
    p_param->status = HCI_TL_UserEventFlow_Enable;
  }
  else
  {
    p_param->status = HCI_TL_UserEventFlow_Disable;
  }

  return;
}

static void BLE_StatusNot(HCI_TL_CmdStatus_t Status)
{
  uint32_t task_id_list;
  switch (Status)
  {
    case HCI_TL_CmdBusy:
      /**
       * All tasks that may send an aci/hci commands shall be listed here
       * This is to prevent a new command is sent while one is already pending
       */
      task_id_list = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
      UTIL_SEQ_PauseTask(task_id_list);
      /* USER CODE BEGIN HCI_TL_CmdBusy */

      /* USER CODE END HCI_TL_CmdBusy */
      break;

    case HCI_TL_CmdAvailable:
      /**
       * All tasks that may send an aci/hci commands shall be listed here
       * This is to prevent a new command is sent while one is already pending
       */
      task_id_list = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
      UTIL_SEQ_ResumeTask(task_id_list);
      /* USER CODE BEGIN HCI_TL_CmdAvailable */

      /* USER CODE END HCI_TL_CmdAvailable */
      break;

    default:
      /* USER CODE BEGIN Status */

      /* USER CODE END Status */
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
