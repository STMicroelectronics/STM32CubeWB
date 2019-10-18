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


/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "dbg_trace.h"

#include "ble.h"
#include "tl.h"
#include "app_ble.h"

#include "crs_app.h"
#include "crc_app.h"

#include "stm32_seq.h"
#include "shci.h" 
#include "stm32_lpm.h"
#include "otp.h" 

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
   * Flag to tell whether OOB data has
   * to be used during the pairing process
   */
  uint8_t OOB_Data_Present;

  /**
   * OOB data to be used in the pairing process if
   * OOB_Data_Present is set to TRUE
   */
  uint8_t OOB_Data[16];

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
  uint16_t connectionHandle[CFG_MAX_CONNECTION];

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
  APP_BLE_ConnStatus_t Device_Connection_Status[CFG_MAX_CONNECTION];
  uint8_t Connection_mgr_timer_Id;
  uint8_t DeviceServerFound;
} BleApplicationContext_t;

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define FAST_ADV_TIMEOUT            (30*1000*1000/CFG_TS_TICK_VAL) /**< 30s */

#define BD_ADDR_SIZE_LOCAL    6

/* Private macros ------------------------------------------------------------*/
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


PLACE_IN_SECTION("BLE_APP_CONTEXT") static BleApplicationContext_t BleApplicationContext;
PLACE_IN_SECTION("BLE_APP_CONTEXT") static uint16_t AdvIntervalMin, AdvIntervalMax;



uint8_t Test_Status = 0x00;

static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'C', 'R', 'S' };
static const uint8_t CRS_STM_UUID[] = { CRS_STM_UUID128 }; 
tBDAddr SERVER_REMOTE_BDADDR;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * pPayload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress( void );
#if(GATT_CLIENT == 0)
static void Add_Advertisment_Service_UUID( const uint8_t *servUUID, uint8_t UUIDLength );
#endif
static void Adv_Request( APP_BLE_ConnStatus_t New_Status );
static void ConnMgr( void );
static void AdvUpdate( void );
#if(GATT_CLIENT == 1)
static void Scan_Request( void );
static void Connect_Request( void );
#endif

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Init( void )
{
  uint8_t index;
  SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet =
  {
    {{0,0,0}},                              /**< Header unused */
    {0,                                  /** pBleBufferAddress not used */
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
    CFG_BLE_LL_ONLY,
    0},
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
  SHCI_C2_BLE_Init( &ble_init_cmd_packet );

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
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONN_MGR_ID, UTIL_SEQ_RFU, AdvUpdate);
#if(GATT_CLIENT != 0)
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SCAN_REQ_ID, UTIL_SEQ_RFU, Scan_Request);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONN_REQ_ID, UTIL_SEQ_RFU, Connect_Request);
#endif

  /**
   * Initialization of the BLE App Context
   */
  for (index = 0; index < CFG_MAX_CONNECTION; index++)
  {
    BleApplicationContext.Device_Connection_Status[index] = APP_BLE_IDLE;
    BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] = 0xFFFF;
  }

#if(GATT_CLIENT == 0)
  /**
   * Initialize CRS Application
   */
  CRSAPP_Init();
#else
  /**
   * Initialize CRC Application
   */
  CRCAPP_Init();
#endif

  /**
   * Create timer to handle the connection state machine
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BleApplicationContext.Connection_mgr_timer_Id), hw_ts_SingleShot, ConnMgr);

#if(GATT_CLIENT == 0)
  /**
   * Make device discoverable
   */
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = AD_TYPE_128_BIT_SERV_UUID;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 1;

  Add_Advertisment_Service_UUID(&CRS_STM_UUID[0], sizeof(CRS_STM_UUID));

  /* Initialize intervals for reconnexion without intervals update */
  AdvIntervalMin = CFG_FAST_CONN_ADV_INTERVAL_MIN;
  AdvIntervalMax = CFG_FAST_CONN_ADV_INTERVAL_MAX;

  Adv_Request(APP_BLE_FAST_ADV);
#else
  /**
   * Start scanning
   */
  UTIL_SEQ_SetTask(1 << CFG_TASK_SCAN_REQ_ID, CFG_SCH_PRIO_0);

#endif

  return;
}


SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  uint8_t index;

  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;

  switch (event_pckt->evt)
  {
    case EVT_VENDOR:
    {
      evt_blue_aci *blue_evt;

      blue_evt = (evt_blue_aci*) event_pckt->data;

      switch (blue_evt->ecode)
      {
        case EVT_BLUE_GAP_PROCEDURE_COMPLETE:
        {
          aci_gap_proc_complete_event_rp0 *gap_evt_proc_complete = (void*) blue_evt->data;

          /* CHECK GAP GENERAL DISCOVERY PROCEDURE COMPLETED & SUCCEED */
          if ((gap_evt_proc_complete->Procedure_Code == GAP_GENERAL_DISCOVERY_PROC) &&
              (gap_evt_proc_complete->Status == 0x00))
          {
            BSP_LED_Off(LED_BLUE);
            APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE_COMPLETED\n");
            if(BleApplicationContext.DeviceServerFound == 0x01)
            {
              UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_REQ_ID, CFG_SCH_PRIO_0);
            }
          }
        }
        break; /* EVT_BLUE_GAP_PAIRING_CMPLT */
      }
    }
    break; /* EVT_VENDOR */

    case EVT_DISCONN_COMPLETE:
    {
      hci_disconnection_complete_event_rp0 * disconnection_complete_event;

      disconnection_complete_event = (hci_disconnection_complete_event_rp0 *) event_pckt->data;

      APP_DBG_MSG("EVT_DISCONN_COMPLETE for connection handle 0x%x\n",
                  disconnection_complete_event->Connection_Handle);
      /* Find index of the handle deconnected */
      index = 0;
      while((index < CFG_MAX_CONNECTION)
          && (BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index]
                                                                                  != disconnection_complete_event->Connection_Handle))
      {
        index++;
      }

      if (index < CFG_MAX_CONNECTION)
      {
        APP_DBG_MSG("Index of the handle deconnected: %d\n", index);
        BleApplicationContext.Device_Connection_Status[index] = APP_BLE_IDLE;
        BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] = 0xFFFF;
      }
      else
        APP_DBG_MSG("No index found for the handle discconnected !\n");

#if(GATT_CLIENT == 0)
      /* restart advertising */
      Adv_Request(APP_BLE_FAST_ADV);
#endif
    }
    break; /* EVT_DISCONN_COMPLETE */

    case EVT_LE_META_EVENT:
    {
      meta_evt = (evt_le_meta_event*) event_pckt->data;

      switch (meta_evt->subevent)
      {
        case EVT_LE_CONN_COMPLETE:
        {
          hci_le_connection_complete_event_rp0 * connection_complete_event;

          /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */
          connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;
          HW_TS_Stop(BleApplicationContext.Connection_mgr_timer_Id);

          APP_DBG_MSG("EVT_LE_CONN_COMPLETE for connection handle 0x%x\n",
                      connection_complete_event->Connection_Handle);

          /* Find index of a connection not in APP_BLE_IDLE, APP_BLE_CONNECTED_SERVER or APP_BLE_CONNECTED_CLIENT state */
          index = 0;
          while((index < CFG_MAX_CONNECTION) &&
              ((BleApplicationContext.Device_Connection_Status[index] == APP_BLE_IDLE)             ||
                  (BleApplicationContext.Device_Connection_Status[index] == APP_BLE_CONNECTED_SERVER) ||
                  (BleApplicationContext.Device_Connection_Status[index] == APP_BLE_CONNECTED_CLIENT)))
          {
            index++;
          }

          if (index < CFG_MAX_CONNECTION)
          {
            APP_DBG_MSG("First index in state %d: %d\n",
                        BleApplicationContext.Device_Connection_Status[index],
                        index);
            if (BleApplicationContext.Device_Connection_Status[index] == APP_BLE_LP_CONNECTING)
            {
              /* Connection as client */
              BleApplicationContext.Device_Connection_Status[index] = APP_BLE_CONNECTED_CLIENT;
            }
            else
            {
              /* Connection as server */
              BleApplicationContext.Device_Connection_Status[index] = APP_BLE_CONNECTED_SERVER;
            }
            BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] =
                connection_complete_event->Connection_Handle;
          }
          else
            APP_DBG_MSG("No stored connection in state different than APP_BLE_IDLE, APP_BLE_CONNECTED_CLIENT and APP_BLE_CONNECTED_SERVER!\n");

#if (GATT_CLIENT == 1)
          {
            tBleStatus result;

            APP_DBG_MSG("aci_gatt_disc_all_primary_services\n");
            result = aci_gatt_disc_all_primary_services(BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index]);
            if( result == BLE_STATUS_SUCCESS )
            {
              APP_DBG_MSG("Discovery of all primary services sent Successfully \n");
            }
            else
            {
              APP_DBG_MSG("Discovery of all primary services sent Failed with error: 0x%x\n", result);
            }
          }
#endif
        }
        break; /* HCI_EVT_LE_CONN_COMPLETE */

        case EVT_LE_ADVERTISING_REPORT:
        {
          hci_le_advertising_report_event_rp0 * le_advertising_event;
          uint8_t event_type, event_data_size;
          uint8_t adtype, adlength;
          uint8_t *adv_report_data; 
          uint8_t k;

          le_advertising_event = (hci_le_advertising_report_event_rp0 *) meta_evt->data;

          event_type = le_advertising_event->Advertising_Report[0].Event_Type;

          event_data_size = le_advertising_event->Advertising_Report[0].Length_Data;
          
          /* WARNING: be careful when decoding advertising report as its raw format cannot be mapped on a C structure. 
          The data and RSSI values could not be directly decoded from the RAM using the data and RSSI field from hci_le_advertising_report_event_rp0 structure.
          Instead they must be read by using offsets (please refer to BLE specification).
          RSSI = *(uint8_t*) (adv_report_data + le_advertising_event->Advertising_Report[0].Length_Data);
          */
          adv_report_data = (uint8_t*)(&le_advertising_event->Advertising_Report[0].Length_Data) + 1;
          k = 0;

          if (event_type == ADV_IND)
          {
            /*ISOLATION OF BD ADDRESS AND LOCAL NAME*/

            while(k < event_data_size)
            {
              adlength = adv_report_data[k];
              APP_DBG_MSG("adlength: 0x%X\n",
                          adlength);
              adtype = adv_report_data[k + 1];
              APP_DBG_MSG("adtype: 0x%X\n",
                          adtype);
              switch (adtype)
              {
                case AD_TYPE_FLAGS: /* now get flags */
                {
                  uint8_t i;

                  APP_DBG_MSG("AD_TYPE_FLAGS: 0x");
                  for(i = 0; i < adlength; i++)
                  {
                    APP_DBG_MSG("%X",
                                adv_report_data[k + 2 + i]);
                  }
                  APP_DBG_MSG("\n");
                }
                break;

                case AD_TYPE_COMPLETE_LOCAL_NAME:
                {
                  uint8_t i;

                  APP_DBG_MSG("AD_TYPE_COMPLETE_LOCAL_NAME: ");
                  for(i = 0; i < adlength; i++)
                  {
                    APP_DBG_MSG("%c",
                                adv_report_data[k + 2 + i]);
                  }
                  APP_DBG_MSG("\n");
                }
                break;

                case AD_TYPE_16_BIT_SERV_UUID:
                {
                  uint8_t i;
                  uint16_t uuid16;

                  APP_DBG_MSG("AD_TYPE_16_BIT_SERV_UUID: 0x");
                  for(i = 0; i < adlength; i++)
                  {
                    APP_DBG_MSG("%X",
                                adv_report_data[k + 2 + i]);
                  }
                  APP_DBG_MSG("\n");
                  uuid16 = (adv_report_data[k + 2 + 1] << 8) +
                      adv_report_data[k + 2 + 0];
                  if(uuid16 == CRS_SERVICE_UUID)
                  {
                    APP_DBG_MSG("-- Cable Replacement Service DETECTED -- VIA 16 bits UUID\n");
                    BleApplicationContext.DeviceServerFound = 0x01;
                    APP_DBG_MSG("SERVER_REMOTE_BDADDR: 0x\n");
                    for(i = 0; i < 5; i++)
                    {
                      SERVER_REMOTE_BDADDR[i] = le_advertising_event->Advertising_Report[0].Address[i];
                      APP_DBG_MSG("%X",
                                  le_advertising_event->Advertising_Report[0].Address[i]);
                    }
                    APP_DBG_MSG("\n");
                  }
                }
                break;

                case AD_TYPE_TX_POWER_LEVEL: /* Tx power level */
                {
                  uint8_t i;

                  APP_DBG_MSG("AD_TYPE_TX_POWER_LEVEL: 0x");
                  for(i = 0; i < adlength; i++)
                  {
                    APP_DBG_MSG("%X",
                                adv_report_data[k + 2 + i]);
                  }
                  APP_DBG_MSG("\n");
                }
                break;

                case AD_TYPE_128_BIT_SERV_UUID:
                {
                  uint8_t i, result = TRUE;

                  APP_DBG_MSG("AD_TYPE_128_BIT_SERV_UUID: 0x");
                  for(i = 0; i < adlength-1; i++)
                  {
                    APP_DBG_MSG("%X",
                                adv_report_data[k + 2 + i]);
                    if(CRS_STM_UUID[i] != adv_report_data[k + 2 + i])
                    {
                      APP_DBG_MSG("CRS_STM_UUID[%d] 0x%x != adv_report_data[%d + 2 + %d] 0x%x\n",
                                  i, CRS_STM_UUID[i], k, i, adv_report_data[k + 2 + i]);
                      APP_DBG_MSG("AD_TYPE_128_BIT_SERV_UUID not a Cable Replacement Service\n");
                      result = FALSE;
                      break;
                    }
                  }
                  APP_DBG_MSG("\n");
                  if(result == TRUE)
                  {
                    APP_DBG_MSG("-- Cable Replacement Service DETECTED -- VIA 128 bits UUID\n");
                    BleApplicationContext.DeviceServerFound = 0x01;
                    APP_DBG_MSG("SERVER_REMOTE_BDADDR: 0x");
                    for(i = 0; i < 6; i++)
                    {
                      SERVER_REMOTE_BDADDR[i] = le_advertising_event->Advertising_Report[0].Address[i];
                      APP_DBG_MSG("%X",
                                  le_advertising_event->Advertising_Report[0].Address[i]);
                    }
                    APP_DBG_MSG("\n");
                  }
                }
                break;

                case AD_TYPE_SERV_SOLICIT_16_BIT_UUID_LIST: /* 16 bit Service UUIDs list */
                {
                  uint8_t i;

                  APP_DBG_MSG("AD_TYPE_SERV_SOLICIT_16_BIT_UUID_LIST: 0x");
                  for(i = 0; i < adlength; i++)
                  {
                    APP_DBG_MSG("%X",
                                adv_report_data[k + 2 + i]);
                  }
                  APP_DBG_MSG("\n");
                }
                break;

                case AD_TYPE_MANUFACTURER_SPECIFIC_DATA: /* Manufactureur Specific */
                {
                } /* AD_TYPE_MANUFACTURER_SPECIFIC_DATA */
                break;

                case AD_TYPE_SERVICE_DATA: /* service data 16 bits */
                {
                  uint8_t i;

                  APP_DBG_MSG("AD_TYPE_SERVICE_DATA: 0x");
                  for(i = 0; i < adlength; i++)
                  {
                    APP_DBG_MSG("%X",
                                adv_report_data[k + 2 + i]);
                  }
                  APP_DBG_MSG("\n");
                }
                break;

                default:
                  break;
              } /* End switch (adtype) */
              k += adlength + 1;
            } /* End while(k < event_data_size) */
          } /* End if (event_type == ADV_IND) */
        }
        break; /* EVT_LE_ADVERTISING_REPORT */

        default:
          break;
      }
    }
    break; /* EVT_LE_META_EVENT */

    default:
      break;
  }

  return (SVCCTL_UserEvtFlowEnable);
}

APP_BLE_ConnStatus_t APP_BLE_Get_Client_Connection_Status(uint16_t Connection_Handle)
{
  uint8_t index;

  /* Find index of the handle */
  index = 0;        
  while((index < CFG_MAX_CONNECTION) &&
      (BleApplicationContext.BleApplicationContext_legacy.connectionHandle[index] != Connection_Handle))
  {
    index++;
  }

  if(index == CFG_MAX_CONNECTION)
  {
    APP_DBG_MSG("No 0x%x connection handle found in table !\n", Connection_Handle);
    return APP_BLE_IDLE;
  }
  else
  {
    APP_DBG_MSG("Found 0x%x connection handle in table at index: %d!\n", Connection_Handle, index);
    return BleApplicationContext.Device_Connection_Status[index];
  }
}

APP_BLE_ConnStatus_t APP_BLE_Get_Server_Connection_Status(void)
{
  uint8_t index;

  /* Find index in connected state */
  index = 0;        
  while((index < CFG_MAX_CONNECTION) &&
      (BleApplicationContext.Device_Connection_Status[index] == APP_BLE_IDLE))
  {
    index++;
  }

  if(index == CFG_MAX_CONNECTION)
  {
    return APP_BLE_IDLE;
  }
  else
  {
    return BleApplicationContext.Device_Connection_Status[index];
  }
}

void APP_BLE_Key_Button1_Action(void)
{
}

void APP_BLE_Key_Button2_Action(void)
{
}

void APP_BLE_Key_Button3_Action(void)
{
}

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
  uint8_t index;
  uint16_t gap_service_handle, gap_dev_name_char_handle, gap_appearance_char_handle;
  const uint8_t *bd_addr;
  uint32_t srd_bd_addr[2];
  uint16_t appearance[1] = { CFG_GAP_APPEARANCE }; /* Generic Heart Rate Sensor */

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
    const char *name = "BLEcore";

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
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data_Present = 0;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = 8;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = 16;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin = 1;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin = 111111;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode = 1;
  for (index = 0; index < 16; index++)
  {
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data[index] = (uint8_t) index;
  }

  aci_gap_set_authentication_requirement(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode,
                                         0,
                                         0,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin,
                                         BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin,
                                         0
  );

  /**
   * Initialize whitelist
   */
   if (BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode)
   {
     aci_gap_configure_whitelist();
   }

}

#if (GATT_CLIENT == 1)   
static void Scan_Request( void )
{
  tBleStatus result;
  uint8_t index;

  index = 0;        
  while((index < CFG_MAX_CONNECTION) &&
      (BleApplicationContext.Device_Connection_Status[index] != APP_BLE_IDLE))
  {
    index++;
  }

  if (index < CFG_MAX_CONNECTION)
  {
    BSP_LED_On(LED_BLUE);
    result = aci_gap_start_general_discovery_proc(SCAN_P, SCAN_L, PUBLIC_ADDR, 1);
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("** START GENERAL DISCOVERY (SCAN) **\n");
    }
    else
    {
      APP_DBG_MSG("-- aci_gap_start_general_discovery_proc, Failed\n");
      BSP_LED_On(LED_RED);
    }
  }
  else
  {
    APP_DBG_MSG("No stored connection in state APP_BLE_IDLE!\n");
  }
  return;
}

static void Connect_Request( void )
{
  tBleStatus result;
  uint8_t index;

  index = 0;        
  while((index < CFG_MAX_CONNECTION) &&
      (BleApplicationContext.Device_Connection_Status[index] != APP_BLE_IDLE))
  {
    index++;
  }

  APP_DBG_MSG("** CREATE CONNECTION TO SERVER **\n");

  if (index < CFG_MAX_CONNECTION)
  {
    result = aci_gap_create_connection(
        SCAN_P,
        SCAN_L,
        PUBLIC_ADDR,
        SERVER_REMOTE_BDADDR,
        PUBLIC_ADDR,
        CONN_P1,
        CONN_P2,
        0,
        SUPERV_TIMEOUT,
        CONN_L1,
        CONN_L2);

    if (result == BLE_STATUS_SUCCESS)
    {
      BleApplicationContext.Device_Connection_Status[index] = APP_BLE_LP_CONNECTING;
    }
    else
    {
      BSP_LED_On(LED_RED);
      BleApplicationContext.Device_Connection_Status[index] = APP_BLE_IDLE;
    }
  }
  else
  {
    APP_DBG_MSG("No stored connection in state APP_BLE_CONNECTED_CLIENT!\n");
  }

  return;
}
#endif

static void AdvUpdate( void )
{
  Adv_Request(APP_BLE_LP_ADV);

  return;
}

static void ConnMgr( void )
{
  /**
   * The code shall be executed in the background as an aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_MGR_ID, CFG_SCH_PRIO_0);

  return;
}

#if(GATT_CLIENT == 0)
static void Add_Advertisment_Service_UUID( const uint8_t *servUUID, uint8_t UUIDLength )
{
  uint8_t i;

  for(i = 0; i < UUIDLength; i++)
  {
    BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] =
        servUUID[i];
    BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;
  }

  return;
}
#endif

static void Adv_Request( APP_BLE_ConnStatus_t New_Status )
{
  uint8_t index;
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  uint16_t Min_Inter, Max_Inter;

  index = 0;
  if (New_Status == APP_BLE_FAST_ADV)
  {
    Min_Inter = AdvIntervalMin;
    Max_Inter = AdvIntervalMax;

    /* Find first index of connection in ADVERTISE or IDLE mode */
    while((index < CFG_MAX_CONNECTION) && (BleApplicationContext.Device_Connection_Status[index] != APP_BLE_IDLE))
    {
      index++;
    }
  }
  else
  {
    Min_Inter = CFG_LP_CONN_ADV_INTERVAL_MIN;
    Max_Inter = CFG_LP_CONN_ADV_INTERVAL_MAX;

    /* Find first index of connection in Fast ADVERTISE mode */
    while((index < CFG_MAX_CONNECTION) && (BleApplicationContext.Device_Connection_Status[index] != APP_BLE_FAST_ADV))
    {
      index++;
    }
  }

  if (index < CFG_MAX_CONNECTION)
  {
    /**
     * Stop the timer, it will be restarted for a new shot
     * It does not hurt if the timer was not running
     */
    HW_TS_Stop(BleApplicationContext.Connection_mgr_timer_Id);

    APP_DBG_MSG("First index in %d state: %d\n",
                BleApplicationContext.Device_Connection_Status[index],
                index);

    if ((New_Status == APP_BLE_LP_ADV)
        && ((BleApplicationContext.Device_Connection_Status[index] == APP_BLE_FAST_ADV)
            || (BleApplicationContext.Device_Connection_Status[index] == APP_BLE_LP_ADV)))
    {
      /* Connection in ADVERTISE mode have to stop the current advertising */
      ret = aci_gap_set_non_discoverable();
      if (ret == BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("Successfully Stopped Advertising at index: %d\n", index);
      }
      else
      {
        APP_DBG_MSG("Stop Advertising Failed at index: %d, result: %d \n", index, ret);
      }
    }

    BleApplicationContext.Device_Connection_Status[index] = New_Status;
    /* Start Fast or Low Power Advertising */
    ret = aci_gap_set_discoverable(
        ADV_IND,
        Min_Inter,
        Max_Inter,
        PUBLIC_ADDR,
        NO_WHITE_LIST_USE, /* use white list */
        sizeof(local_name),
        (uint8_t*) &local_name,
        BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen,
        BleApplicationContext.BleApplicationContext_legacy.advtServUUID,
        0,
        0);
    if (ret == BLE_STATUS_SUCCESS)
    {
      if (New_Status == APP_BLE_FAST_ADV)
      {
        APP_DBG_MSG("Successfully Start Fast Advertising at index: %d\n", index);
      }
      else
      {
        APP_DBG_MSG("Successfully Start Low Power Advertising at index: %d\n", index);
      }
    }
    else
    {
      if (New_Status == APP_BLE_FAST_ADV)
      {
        APP_DBG_MSG("Start Fast Advertising Failed at index: %d, result: 0x%x \n", index, ret);
      }
      else
      {
        APP_DBG_MSG("Start Low Power Advertising Failed at index: %d, result: 0x%x \n", index, ret);
      }
    }
    if (New_Status == APP_BLE_FAST_ADV)
    {
      /* Fast advertising during FAST_ADV_TIMEOUT */
      HW_TS_Start(BleApplicationContext.Connection_mgr_timer_Id, FAST_ADV_TIMEOUT);
    }
  }
  else
    APP_DBG_MSG("No index in APP_BLE_IDLE state !\n");

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

    bd_addr_udn[0] = (uint8_t)(udn & 0x000000FF);
    bd_addr_udn[1] = (uint8_t)( (udn & 0x0000FF00) >> 8 );
    bd_addr_udn[2] = (uint8_t)( (udn & 0x00FF0000) >> 16 );
    bd_addr_udn[3] = (uint8_t)device_id;
    bd_addr_udn[4] = (uint8_t)(company_id & 0x000000FF);;
    bd_addr_udn[5] = (uint8_t)( (company_id & 0x0000FF00) >> 8 );

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
