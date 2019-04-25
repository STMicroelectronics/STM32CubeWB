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

#include "dt_client_app.h"
#include "dt_server_app.h"
#include "dts.h"

#include "scheduler.h"
#include "shci.h"
#include "lpm.h"
#include "otp.h"

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define DEFAULT_TS_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */
#define DELAY_FOR_PAIRING                 (2*DEFAULT_TS_MEASUREMENT_INTERVAL)
#define DATA_START_STOP                   (1*DEFAULT_TS_MEASUREMENT_INTERVAL)
#define DATA_STOP                         (1*DEFAULT_TS_MEASUREMENT_INTERVAL)
#define LP_CONN_ADV_INTERVAL_MIN          (0x640) /**< 1s */
#define LP_CONN_ADV_INTERVAL_MAX          (0xFA0) /**< 2.5s */
#define FAST_CONN_ADV_INTERVAL_MIN  (0x20)  /**< 20ms */
#define FAST_CONN_ADV_INTERVAL_MAX  (0x30)  /**< 30ms */

#define FORCE_REBOND                      0x01
#define CONN_P1_7_5                        (CONN_P(7.5))
#define CONN_P2_7_5                        (CONN_P(7.5))
#define CONN_P1_50                        (CONN_P(50))
#define CONN_P2_50                        (CONN_P(50))
#define CONN_P1_400                        (CONN_P(400))
#define CONN_P2_400                        (CONN_P(400))

#define BD_ADDR_SIZE_LOCAL    6
/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  BLE_APP_DCS_IDLE,
  BLE_APP_DCS_FAST_ADV,
  BLE_APP_DCS_LP_ADV,
  BLE_APP_DCS_LP_CONNECTING,
  BLE_APP_DCS_CONNECTED,
  BLE_APP_DCS_CONNECTED_SERVER,
  BLE_APP_DCS_CONNECTED_CLIENT

} BLE_APP_Device_Connection_Status_t;

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
 * global profile context
 * contains the variables common to all
 * profiles
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
  uint16_t connectionHandle[MAX_CONNECTION];

  /**
   * length of the UUID list to be used while advertising
   */
  uint8_t advtServUUIDlen;

  /**
   * the UUID list to be used while advertising
   */
  uint8_t advtServUUID[100];

} tBLEProfileGlobalContext;

typedef struct
{
  tBLEProfileGlobalContext DTContext_legacy;
  DT_Gap_Gatt_State Connection_Status;
  uint16_t connection_handle;
  uint8_t Connection_mgr_timer_Id;
  uint8_t DeviceServerFound;
  uint8_t PairingReq_Id;
  uint8_t StartData_Id;
  uint8_t StopData_Id;
} DTContext_t;

typedef struct
{
  tBLEProfileGlobalContext BleApplicationContext_legacy;
  BLE_APP_Device_Connection_Status_t Device_Connection_Status[MAX_CONNECTION];
  BLE_APP_Device_Connection_Status_t Device1_Connection_Status;
  uint8_t Connection_mgr_timer_Id;
} BleApplicationContext_t;

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


static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME, 'D', 'T', '_', 'S', 'E', 'R', 'V', 'E', 'R' };
uint8_t const manuf_data[22] = { 2, AD_TYPE_TX_POWER_LEVEL, 0x00 /* 0 dBm */, /* Trasmission Power */
    10, AD_TYPE_COMPLETE_LOCAL_NAME, 'D', 'T', '_', 'S', 'E', 'R', 'V', 'E', 'R', /* Complete Name */
    7, AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x01/*SKD version */,
    CFG_DEV_ID_PERIPH_SERVER, /* NUCLEO-Board WB - DT Periph Server*/
    0x00 /*  */, 0x00 /*  */, 0x00 /*  */,
    CFG_FEATURE_DT /* Data Throughput Service features */
};

/* Global variables ----------------------------------------------------------*/
BleApplicationContext_t BleApplicationContext;
tBDAddr SERVER_REMOTE_BDADDR;
DTContext_t DTContext;

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * pPayload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress( void );
static void Connect_Request( void );
static void Scan_Request( void );
static void Adv_Request( void );
void Delay_PairingReq( void );

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Init( void )
{
	SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet =
	  {
		{{0,0,0}},                          /**< Header unused */
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
  LPM_SetOffMode(1 << CFG_LPM_APP_BLE, LPM_OffMode_Dis);

  /**
   * Register the hci transport layer to handle BLE User Asynchronous Events
   */
  SCH_RegTask(CFG_TASK_HCI_ASYNCH_EVT_ID, hci_user_evt_proc);

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
  SCH_RegTask(CFG_TASK_START_ADV_ID, Adv_Request);
  SCH_RegTask(CFG_TASK_START_SCAN_ID, Scan_Request);
  SCH_RegTask(CFG_TASK_CONN_DEV_1_ID, Connect_Request);

  /**
   * Initialization of the BLE App Context
   */

  DTContext.Connection_Status = DT_IDLE;

  /**
   * Clear DataBase
   */
  aci_gap_clear_security_db();

  DT_Client_App_Init();

  DT_Server_App_Init();

#ifdef BLE_PERIPHERAL

  /* Start Advertise to be connected by P2P Client */
  SCH_SetTask(1 << CFG_TASK_START_ADV_ID, CFG_SCH_PRIO_0);

#endif
#ifdef BLE_CENTRAL

  /* Start Scan to connect to DT server */
  SCH_SetTask(1<<CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
#endif

  return;
}

uint8_t APP_BLE_ComputeCRC8( uint8_t *DataPtr , uint8_t Datalen )
{
  uint8_t i, j;
  const uint8_t PolynomeCRC = 0x97;
  uint8_t CRC8 = 0x00;

  for (i = 0; i < Datalen; i++)
  {
    CRC8 ^= DataPtr[i];
    for (j = 0; j < 8; j++)
    {
      if ((CRC8 & 0x80) != 0)
      {
        CRC8 = (uint8_t) ((CRC8 << 1) ^ PolynomeCRC);
      }
      else
      {
        CRC8 <<= 1;
      }
    }
  }
  return (CRC8);
}

void APP_BLE_Key_Button1_Action(void)
{
   DT_Server_App_Key_Button_Action();
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
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin = 0;
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

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt )
{
  evt_le_meta_event * meta_evt;
  hci_le_connection_complete_event_rp0 * connection_complete_event;
  hci_le_advertising_report_event_rp0 * le_advertising_event;
  hci_le_phy_update_complete_event_rp0 *evt_le_phy_update_complete;
  evt_blue_aci * blue_evt;
  hci_event_pckt * event_pckt;
  Connection_Context_t Notification;
  uint16_t connection_handle;
  uint8_t event_type, event_data_size;
  int k = 0;
  uint8_t adtype, adlength;
  uint8_t *adv_report_data;

  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
  hci_disconnection_complete_event_rp0 *cc = (void *) event_pckt->data;

  switch (event_pckt->evt)
  {
    case EVT_DISCONN_COMPLETE:

      if (cc->Connection_Handle == DTContext.connection_handle)
      {
        DTContext.Connection_Status = DT_IDLE;
        APP_DBG_MSG("BLE_CTRL_App_Notification: EVT_DISCONN_COMPLETE disconnection\n");
        Notification.Evt_Opcode = DT_DISCONN_COMPLETE;
        DT_App_Notification(&Notification);
      }
      break; /* EVT_DISCONN_COMPLETE */

    case EVT_LE_META_EVENT:
    {
      meta_evt = (evt_le_meta_event*) event_pckt->data;

      switch (meta_evt->subevent)
      {
        case EVT_LE_PHY_UPDATE_COMPLETE:

          APP_DBG_MSG("EVT_UPDATE_PHY_COMPLETE \n");
          evt_le_phy_update_complete = (hci_le_phy_update_complete_event_rp0*)meta_evt->data;
          if (evt_le_phy_update_complete->Status == 0)
          {
            APP_DBG_MSG("EVT_UPDATE_PHY_COMPLETE, status ok \n");
            Notification.Evt_Opcode = DT_SET_PHY;
            DT_App_Notification(&Notification);
          }
          else
          {
            APP_DBG_MSG("EVT_UPDATE_PHY_COMPLETE, status nok \n");
          }
          break;
        case EVT_LE_CONN_COMPLETE:
          /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */
          connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;

          connection_handle = connection_complete_event->Connection_Handle;

          DTContext.connection_handle = connection_handle;
          Notification.Evt_Opcode = DT_CONNECTION_ESTABLISHED;
          Notification.connection_handle = connection_complete_event->Connection_Handle;
#ifdef BLE_PERIPHERAL
          APP_DBG_MSG("BLE_CTRL_App_Notification: EVT_LE_CONN_COMPLETE connection as slave\n");
#endif

#ifdef BLE_CENTRAL   
          APP_DBG_MSG("BLE_CTRL_App_Notification: EVT_LE_CONN_COMPLETE connection as master\n");
          HW_TS_Stop(BleApplicationContext.Connection_mgr_timer_Id);          
#endif
          DT_App_Notification(&Notification);
          break; /* HCI_EVT_LE_CONN_COMPLETE */

        case EVT_LE_ADVERTISING_REPORT:

          le_advertising_event = (hci_le_advertising_report_event_rp0 *) meta_evt->data;

          event_type = le_advertising_event->Advertising_Report[0].Event_Type;

          event_data_size = le_advertising_event->Advertising_Report[0].Length_Data;
          
          adv_report_data = (uint8_t*)(&le_advertising_event->Advertising_Report[0].Length_Data) + 1;
          k = 0;

          /* search AD TYPE 0x09 (Complete Local Name) */
          /* search AD Type 0x02 (16 bits UUIDS) */
          if (event_type == ADV_IND)
          {

            /*ISOLATION OF BD ADDRESS AND LOCAL NAME*/

            while(k < event_data_size)
            {
              adlength = adv_report_data[k];
              adtype = adv_report_data[k + 1];
              switch (adtype)
              {
                case 0x01: /* now get flags */
                  break;

                case 0x0A: /* Tx power level */
                  break;
                case 0xFF: /* Manufacturer Specific */
                  if (adlength >= 7 && adv_report_data[k + 2] == 0x01)
                  { /* ST VERSION ID 01 */
                    APP_DBG_MSG("--- ST MANUFACTURER ID --- \n");
                    switch (adv_report_data[k + 3])
                    {   /* Demo ID */
                      case CFG_DEV_ID_PERIPH_SERVER:   /* (Periph Server DT) */
                        APP_DBG_MSG("-- SERVER DETECTED -- VIA MAN ID\n");
                        DTContext.DeviceServerFound = 0x01;
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
                case 0x16: /* service data 16 bits */
                  break;
                default:
                  break;
              } /* end switch data_RSSI[k+adlength] */
              k += adlength + 1;
            } /* end while */

          } /*end if ADV_IND */
          break;

        default:
          break;
      }
    }
    break; /* HCI_EVT_LE_META_EVENT */

    case EVT_VENDOR:
      blue_evt = (evt_blue_aci*) event_pckt->data;

      switch (blue_evt->ecode)
      {
        case EVT_BLUE_GAP_PROCEDURE_COMPLETE:
        {
          aci_gap_proc_complete_event_rp0 *gap_evt_proc_complete = (void*) blue_evt->data;
          /* CHECK GAP GENERAL DISCOVERY PROCEDURE COMPLETED & SUCCEED */
          if (gap_evt_proc_complete->Procedure_Code == GAP_GENERAL_DISCOVERY_PROC
              && gap_evt_proc_complete->Status == 0x00)
          {
            BSP_LED_Off(LED_BLUE);
            APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE_COMPLETED\n");

            /*if a device found, connect to it, device 1 being chosen first if both found*/
            if (DTContext.DeviceServerFound == 0x01 && DTContext.Connection_Status != DT_CONNECTED)
            {
              SCH_SetTask(1 << CFG_TASK_CONN_DEV_1_ID, CFG_SCH_PRIO_0);
            }
          }
        }
        break; /* EVT_BLUE_GAP_PROCEDURE_COMPLETE */
        case EVT_BLUE_GAP_PAIRING_CMPLT:
        {
          APP_DBG_MSG("Pairing complete \n");
          Notification.Evt_Opcode = DT_PAIRING_COMPLETE;
          DT_App_Notification(&Notification);
        }
        break;
        case EVT_BLUE_GAP_PASS_KEY_REQUEST:
        {
          APP_DBG_MSG("passkey request\n");
          Notification.Evt_Opcode = DT_PASSKEY_REQ_EVT;
          DT_App_Notification(&Notification);
        }
        break;
        default:
          break;
      }

        default:
          break;
  }
  return (SVCCTL_UserEvtFlowEnable);
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
  SCH_SetTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
  return;
}

void hci_cmd_resp_release(uint32_t flag)
{
  SCH_SetEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
  return;
}

void hci_cmd_resp_wait(uint32_t timeout)
{
  SCH_WaitEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
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
      SCH_PauseTask(task_id_list);

      break;

    case HCI_TL_CmdAvailable:
      /**
       * All tasks that may send an aci/hci commands shall be listed here
       * This is to prevent a new command is sent while one is already pending
       */
      task_id_list = (1 << CFG_LAST_TASK_ID_WITH_HCICMD) - 1;
      SCH_ResumeTask(task_id_list);

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

void DT_App_Notification( Connection_Context_t *pNotification )
{
  uint8_t TX_PHY;
  uint8_t RX_PHY;
#if defined(PHY_2M)
  Connection_Context_t Notification;
#endif
  uint8_t status;

  switch (pNotification->Evt_Opcode)
  {
    case DT_CONNECTION_ESTABLISHED:
      DTContext.connection_handle = pNotification->connection_handle;

#if (defined (ENCRYPTION_ON) && defined (BLE_CENTRAL)) 
      aci_gap_send_pairing_req(DTContext.connection_handle, FORCE_REBOND);
      APP_DBG_MSG("send pairing req \n");
#elif BLE_CENTRAL        
      aci_gatt_exchange_config(DTContext.connection_handle);
#endif
      break;

    case DT_PAIRING_COMPLETE:
#ifdef BLE_CENTRAL
      aci_gatt_exchange_config(DTContext.connection_handle);
#endif
      break;

    case DT_MTU_EXCHANGED:
#if defined(PHY_2M)
      Notification.Evt_Opcode = DT_READ_PHY;
      DT_App_Notification(&Notification);
#else

      status = hci_le_set_data_length(DTContext.connection_handle,250,2112);
      if (status == 0)
      {
        APP_DBG_MSG("set data length ok \n");
        /* discover primary service */
        aci_gatt_disc_all_primary_services(DTContext.connection_handle);
      }

#endif
      break;

    case DT_READ_PHY:
      status = hci_le_read_phy(DTContext.connection_handle,&TX_PHY,&RX_PHY);
      if (status == 0)
      {
        APP_DBG_MSG("Read_PHY success \n");
        if ((TX_PHY == TX_1M) && (RX_PHY == RX_1M))
        {
          APP_DBG_MSG("Original config is 1M, correct \n");
        }
        else
        {
          APP_DBG_MSG("Original config not correct TX= %x, RX= %x \n", TX_PHY, RX_PHY);
        }
#ifdef BLE_CENTRAL
        status = hci_le_set_phy(DTContext.connection_handle,ALL_PHYS_PREFERENCE,TX_2M_PREFERRED,RX_2M_PREFERRED,0);
        if (status == 0)
        {
          APP_DBG_MSG("set PHY cmd ok\n");
        }
        else 
        {
          APP_DBG_MSG("set PHY cmd NOK\n");
        }
#endif
      }
      else
      {
        APP_DBG_MSG("Read config error \n");
      }
      break;

    case DT_SET_PHY:
      APP_DBG_MSG("Set_PHY success \n");
      status = hci_le_read_phy(DTContext.connection_handle,&TX_PHY,&RX_PHY);
      if (status == 0)
      {
        APP_DBG_MSG("Read_PHY success \n");
        if ((TX_PHY == TX_2M) && (RX_PHY == RX_2M))
        {
          APP_DBG_MSG("Reconf param is 2M, correct \n"); 
          status = hci_le_set_data_length(DTContext.connection_handle,250,2112);
          if (status == 0)
          {
            APP_DBG_MSG("set data length ok \n");
            aci_gatt_disc_all_primary_services(DTContext.connection_handle);
          }
        }
        else
        {
          APP_DBG_MSG("Reconf param not correct TX= %d, RX= %d \n", TX_PHY, RX_PHY);
        } 
      }
      else
      {
        APP_DBG_MSG("Read conf not succeess \n");
      }
      break;

    case DT_PASSKEY_REQ_EVT:
      APP_DBG_MSG("Passkey response \n");
      aci_gap_pass_key_resp(DTContext.connection_handle, 0x00001234);
      break;

    case DT_DISCONN_COMPLETE:
      APP_DBG_MSG("Disconnection \n");
      break;

    default:
      break;

  }
  return;
}/* end BLE_SVC_TimeServer_App_Notification() */

void Delay_PairingReq( void )
{
  aci_gap_send_pairing_req(DTContext.connection_handle, FORCE_REBOND);
  APP_DBG_MSG("send pairing req \n");
}

void Scan_Request( void )
{
  tBleStatus result;
  if (DTContext.Connection_Status != DT_CONNECTED)
  {
    BSP_LED_On(LED_BLUE);
    result = aci_gap_start_general_discovery_proc(SCAN_P, SCAN_L, PUBLIC_ADDR, 1);
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG(" \r\n\r** START GENERAL DISCOVERY (SCAN) **  \r\n\r");
    }
    else
    {
      APP_DBG_MSG("-- BLE_App_Start_Limited_Disc_Req, Failed \r\n\r");
      BSP_LED_On(LED_RED);
    }
  }
  return;
}

void Connect_Request( void )
{
  tBleStatus result;
  APP_DBG_MSG("\r\n\r** CREATE CONNECTION TO SERVER **  \r\n\r");

  if (DTContext.Connection_Status != DT_CONNECTED)
  {
    result = aci_gap_create_connection(SCAN_P,
                                       SCAN_L,
                                       PUBLIC_ADDR, SERVER_REMOTE_BDADDR,
                                       PUBLIC_ADDR,
                                       CONN_P1_50,
                                       CONN_P2_50,
                                       0, 0x3e8, 0x0000, 0x3E8);

    if (result == BLE_STATUS_SUCCESS)
    {
      DTContext.Connection_Status = DT_CONNECTING;

    }
    else
    {
      BSP_LED_On(LED_RED);
      DTContext.Connection_Status = DT_IDLE;

    }
  }

  return;
}
void Adv_Request( void )
{
  if (DTContext.Connection_Status != DT_CONNECTED)
  {
    tBleStatus result = 0x00;

    result = aci_gap_set_discoverable(ADV_IND,
                                      FAST_CONN_ADV_INTERVAL_MIN,
                                      FAST_CONN_ADV_INTERVAL_MAX,
                                      PUBLIC_ADDR,
                                      NO_WHITE_LIST_USE, /* use white list */
                                      sizeof(local_name), (uint8_t*) local_name, 0,
                                      NULL,
                                      6, 8);
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  \r\n\r");APP_DBG_MSG("** START ADVERTISING **  \r\n\r");
    }
    else
    {
      APP_DBG_MSG("** START ADVERTISING **  Failed \r\n\r");
    }

    /* Send Advertising data */
    result = aci_gap_update_adv_data(22, (uint8_t*) manuf_data);

    DTContext.Connection_Status = DT_ADV;
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG("  \r\n\r");APP_DBG_MSG("** add ADV data **  \r\n\r");
    }
    else
    {
      APP_DBG_MSG("** add ADV data **  Failed \r\n\r");
    }
  }
  return;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
