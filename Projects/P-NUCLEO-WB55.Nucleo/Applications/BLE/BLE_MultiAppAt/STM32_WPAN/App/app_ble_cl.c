/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_ble_cl.c
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
#include "app_common.h"

#include "dbg_trace.h"

#include "ble.h"
#include "tl.h"
#include "app_ble_cl.h"

#include "stm32_seq.h"
#include "shci.h"
#include "stm32_lpm.h"
#include "otp.h"

#include "p2p_client_app.h"
#include "uart_app.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

extern uint8_t BD_Addr[6];

extern Disconnection_Status_t disconnection_status; //0 disconnection from Remote; 1 disconnection from Application to switch of BLE App; 2 disconnection ordered by AT command

extern uint8_t autoconn_status;

extern PairingContext_t PairingContext;

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

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define BD_ADDR_SIZE_LOCAL    6

/* USER CODE BEGIN PD */
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

tBDAddr SERVER_REMOTE_BDADDR[5];
uint8_t index_bd_addr;

P2PC_APP_ConnHandle_Not_evt_t handleNotificationCL;

PLACE_IN_SECTION("BLE_APP_CONTEXT") static BleApplicationContext_t BleApplicationContext;

/* USER CODE BEGIN PV */

APP_BLE_ConnStatus_t CL_Connection_Status;

static char bd_address_str[12];

static aci_l2cap_connection_update_req_event_rp0 *l2cap_conn_upd_req_event;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * pPayload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress( void );
static void Scan_Request( void );
static void Connect_Request( void );
//static void Switch_OFF_GPIO( void );
static void APP_BLE_CL_GAP_Disconnect( void );
static void BLE_CL_L2CAP_Conn_Update( void );
static void Confirm_Pairing( void );
static void Pairing_Request ( void );
 
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_CL_Init( void )
{
/* USER CODE BEGIN APP_BLE_Init_1 */
  
  index_bd_addr = 0;

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
    CFG_BLE_LL_ONLY,
    0}
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
  UTIL_SEQ_RegTask( 1<<CFG_TASK_START_SCAN_ID, UTIL_SEQ_RFU, Scan_Request);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONN_DEV_1_ID, UTIL_SEQ_RFU, Connect_Request);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CL_GAP_DISCON_ID, UTIL_SEQ_RFU, APP_BLE_CL_GAP_Disconnect);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CL_L2CAP_CONN_UPDATE_RESP_ID, UTIL_SEQ_RFU, BLE_CL_L2CAP_Conn_Update);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONFIRM_PAIRING_ID, UTIL_SEQ_RFU, Confirm_Pairing);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_REQUEST_PAIRING_ID, UTIL_SEQ_RFU, Pairing_Request);

  /**
   * Initialization of the BLE App Context
   */

  BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
  CL_Connection_Status = APP_BLE_IDLE;
  
  /**
   * Initialize P2P Client Application
   */
  P2PC_APP_Init();

/* USER CODE BEGIN APP_BLE_Init_2 */
  
  /**
   * Send a acknowledgement of the correct initialization of the p2p application
   */
   UART_App_SendData("\r\nCL OK\r\n", 9);

/* USER CODE END APP_BLE_Init_2 */
  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_CL_Notification( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  hci_le_connection_complete_event_rp0 * connection_complete_event;
  evt_blue_aci *blue_evt;
  hci_le_advertising_report_event_rp0 * le_advertising_event;
  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;
  hci_disconnection_complete_event_rp0 *cc = (void *) event_pckt->data;
  
  /* Pairing */
  aci_gap_numeric_comparison_value_event_rp0 *evt_numeric_value;
  aci_gap_pairing_complete_event_rp0 *pairing_complete;
  uint32_t numeric_value;
  char numeric_value_str[20];
  /*****/
  
  uint8_t result;
  uint8_t event_type, event_data_size;
  int k = 0;
  uint8_t adtype, adlength;
  
  int i;
  uint8_t different;

  switch (event_pckt->evt)
  {
    /* USER CODE BEGIN evt */

    /* USER CODE END evt */
    case EVT_VENDOR:
    {
      handleNotificationCL.P2P_Evt_Opcode = PEERC_DISCON_HANDLE_EVT;
      blue_evt = (evt_blue_aci*) event_pckt->data;
      /* USER CODE BEGIN EVT_VENDOR */

      /* USER CODE END EVT_VENDOR */
      switch (blue_evt->ecode)
      {
      /* USER CODE BEGIN ecode */
        case EVT_BLUE_GAP_SLAVE_SECURITY_INITIATED:
//          UTIL_SEQ_SetTask( 1<<CFG_TASK_REQUEST_PAIRING_ID, CFG_SCH_PRIO_0);
          break;
        
      case EVT_BLUE_GAP_NUMERIC_COMPARISON_VALUE :
            UART_App_SendData("\r\n", 2);
            PairingContext.PairingConfirmRequested = 1;
            evt_numeric_value = (aci_gap_numeric_comparison_value_event_rp0 *)blue_evt->data;
            numeric_value = evt_numeric_value->Numeric_Value;
            sprintf(numeric_value_str, "%d", (int)numeric_value);
            UART_App_SendData((char *)numeric_value_str, (uint8_t)(strlen(numeric_value_str)));
            UART_App_SendData(":PREQ?\r\n", 8);
        break;  
        
      case EVT_BLUE_GAP_PAIRING_CMPLT :
            pairing_complete = (aci_gap_pairing_complete_event_rp0*)blue_evt->data;
            if (pairing_complete->Status == 0)
            {
              UART_App_SendData("\r\nP\r\n", 5);
              PairingContext.PairingComplete = 1;
            }
            else UART_App_SendData("\r\nF\r\n", 5);
        break;
      /* USER CODE END ecode */

        case EVT_BLUE_GAP_PROCEDURE_COMPLETE:
        {
        /* USER CODE BEGIN EVT_BLUE_GAP_PROCEDURE_COMPLETE */

        /* USER CODE END EVT_BLUE_GAP_PROCEDURE_COMPLETE */
          aci_gap_proc_complete_event_rp0 *gap_evt_proc_complete = (void*) blue_evt->data;
          /* CHECK GAP GENERAL DISCOVERY PROCEDURE COMPLETED & SUCCEED */
          if (gap_evt_proc_complete->Procedure_Code == GAP_GENERAL_DISCOVERY_PROC
              && gap_evt_proc_complete->Status == 0x00)
          {
              /* USER CODE BEGIN GAP_GENERAL_DISCOVERY_PROC */
              BSP_LED_Off(LED_BLUE);
              if(!autoconn_status) UART_App_SendData("\r\nOK\r\n", 6);
              /* USER CODE END GAP_GENERAL_DISCOVERY_PROC */
#if(CFG_DEBUG_APP_TRACE != 0)
            APP_DBG_MSG("-- GAP GENERAL DISCOVERY PROCEDURE_COMPLETED\n");
#endif
            if(autoconn_status)
            {
              /* if the bd address given by the user correspond to the device found, connect to it */
              if (BleApplicationContext.DeviceServerFound == 0x01 && BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
              {
                UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_DEV_1_ID, CFG_SCH_PRIO_0);
              }
              else if(BleApplicationContext.DeviceServerFound == 0x00) UART_App_SendData("\r\nNF\r\n", 6);
              autoconn_status = 0;
            }
          }
        }
        break;
              
        case EVT_BLUE_L2CAP_CONNECTION_UPDATE_REQ :
          l2cap_conn_upd_req_event = (aci_l2cap_connection_update_req_event_rp0 *) blue_evt->data;
          
          UTIL_SEQ_SetTask(1 << CFG_TASK_CL_L2CAP_CONN_UPDATE_RESP_ID, CFG_SCH_PRIO_0);          
          break;
        default:
          /* USER CODE BEGIN ecode_default */

          /* USER CODE END ecode_default */
          break;
       
      }
    }
    break; 

    case EVT_DISCONN_COMPLETE:
    {
      /* USER CODE BEGIN EVT_DISCONN_COMPLETE */

      /* USER CODE END EVT_DISCONN_COMPLETE */
      if (cc->Connection_Handle == BleApplicationContext.BleApplicationContext_legacy.connectionHandle)
      {
          BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0;
          BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
          CL_Connection_Status = APP_BLE_IDLE;
          BleApplicationContext.DeviceServerFound = 0x00;
#if(CFG_DEBUG_APP_TRACE != 0)
          APP_DBG_MSG("\r\n\r** DISCONNECTION EVENT WITH SERVER \n");
#endif
          if (disconnection_status == TO_SWITCH_APP) //Disconnection done by application for switching app
          {
            handleNotificationCL.P2P_Evt_Opcode = PEERC_DISCON_SWITCH_HANDLE_EVT;
            handleNotificationCL.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
            P2PC_APP_Notification(&handleNotificationCL);
          }
          else { //Deconnection done by user/client or by order from an AT command
            handleNotificationCL.P2P_Evt_Opcode = PEERC_DISCON_HANDLE_EVT;
            handleNotificationCL.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
            P2PC_APP_Notification(&handleNotificationCL);
          }
        }
     }
     break; /* EVT_DISCONN_COMPLETE */

    case EVT_LE_META_EVENT:
    {
      /* USER CODE BEGIN EVT_LE_META_EVENT */

      /* USER CODE END EVT_LE_META_EVENT */
      meta_evt = (evt_le_meta_event*) event_pckt->data;

      switch (meta_evt->subevent)
      {
      /* USER CODE BEGIN subevent */

      /* USER CODE END subevent */
          case EVT_LE_CONN_COMPLETE:
          /* USER CODE BEGIN EVT_LE_CONN_COMPLETE */

          /* USER CODE END EVT_LE_CONN_COMPLETE */
          /**
           * The connection is done, 
           */
          connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;
          BleApplicationContext.BleApplicationContext_legacy.connectionHandle = connection_complete_event->Connection_Handle;
          BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_CLIENT;
          CL_Connection_Status = APP_BLE_CONNECTED_CLIENT;

          /* CONNECTION WITH CLIENT */
#if(CFG_DEBUG_APP_TRACE != 0)
          APP_DBG_MSG("\r\n\r** CONNECTION EVENT WITH SERVER \n");
#endif
          handleNotificationCL.P2P_Evt_Opcode = PEERC_CONN_HANDLE_EVT;
          handleNotificationCL.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
          P2PC_APP_Notification(&handleNotificationCL);

          result = aci_gatt_disc_all_primary_services(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
          if (result == BLE_STATUS_SUCCESS)
          {
#if(CFG_DEBUG_APP_TRACE != 0)
            APP_DBG_MSG("\r\n\r** GATT SERVICES & CHARACTERISTICS DISCOVERY  \n");
            APP_DBG_MSG("* GATT :  Start Searching Primary Services \r\n\r");
#endif
          }
          else
          {
#if(CFG_DEBUG_APP_TRACE != 0)
            APP_DBG_MSG("BLE_CTRL_App_Notification(), All services discovery Failed \r\n\r");
#endif
          }

          break; /* HCI_EVT_LE_CONN_COMPLETE */

        case EVT_LE_ADVERTISING_REPORT:
        {
          uint8_t *adv_report_data;
          /* USER CODE BEGIN EVT_LE_ADVERTISING_REPORT */

          /* USER CODE END EVT_LE_ADVERTISING_REPORT */
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
                case AD_TYPE_FLAGS: /* now get flags */
                /* USER CODE BEGIN AD_TYPE_FLAGS */

                /* USER CODE END AD_TYPE_FLAGS */
                  break;

                case AD_TYPE_TX_POWER_LEVEL: /* Tx power level */
                /* USER CODE BEGIN AD_TYPE_TX_POWER_LEVEL */

                /* USER CODE END AD_TYPE_TX_POWER_LEVEL */
                break;
                case AD_TYPE_MANUFACTURER_SPECIFIC_DATA: /* Manufactureur Specific */
                /* USER CODE BEGIN AD_TYPE_MANUFACTURER_SPECIFIC_DATA */

                /* USER CODE END AD_TYPE_MANUFACTURER_SPECIFIC_DATA */
                  if (adlength >= 7 && adv_report_data[k + 2] == 0x01)
                  { /* ST VERSION ID 01 */
#if(CFG_DEBUG_APP_TRACE != 0)
                    APP_DBG_MSG("--- ST MANUFACTURER ID --- \n");
#endif
                    switch (adv_report_data[k + 3])
                    {   /* Demo ID */
                      case CFG_DEV_ID_AT_P2P_SERVER: /* (0End Device 1) */
#if(CFG_DEBUG_APP_TRACE != 0)
                        APP_DBG_MSG("-- SERVER DETECTED -- VIA MAN ID\n");
#endif
                        if (autoconn_status)
                        {
                          if (strncmp((char *)(BD_Addr), (char *)(le_advertising_event->Advertising_Report[0].Address), 6) == 0)
                          {
                            BleApplicationContext.DeviceServerFound = 0x01;
                          }
                        }
                        else //autoconn_status == 0
                        {
                          /* Send via UART the 5 first bd addresses found */
                          different = 0; //Variable to check if the currently stored BD Address correspond to the one chosen by the user, if not "different" is incremented
                          
                          for (i = 0; i < 5; i++)
                          {
                            if (strncmp((char *)(SERVER_REMOTE_BDADDR[i]), (char *)(le_advertising_event->Advertising_Report[0].Address), 6) != 0)
                            {
                              different++;
                            }
                          }
                          if ( different == 5 && index_bd_addr < 4) //The found BD Address is not registered and the array is not full
                          {
                            /* We register the BD address found */
                            for(i = 0; i < 6; i++) SERVER_REMOTE_BDADDR[index_bd_addr][i] = le_advertising_event->Advertising_Report[0].Address[i];
                            
                            UART_App_ConvertBleDataToString((uint8_t*)(le_advertising_event->Advertising_Report[0].Address), bd_address_str, 12);
                            UART_App_SendData("\r\n", 2);
                            UART_App_SendData((char *)bd_address_str, 12);
                            UART_App_SendData("\r\n", 2);
                            
                            index_bd_addr++;
                          }
                        }
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
        
        default:
          /* USER CODE BEGIN subevent_default */

          /* USER CODE END subevent_default */
          break;

      }
    }
    break; /* HCI_EVT_LE_META_EVENT */

    default:
      /* USER CODE BEGIN evt_default */

      /* USER CODE END evt_default */
      break;
  }

  return (SVCCTL_UserEvtFlowEnable);
}

APP_BLE_ConnStatus_t APP_BLE_CL_Get_Client_Connection_Status( uint16_t Connection_Handle )
{

  if (BleApplicationContext.BleApplicationContext_legacy.connectionHandle == Connection_Handle)
  {
    return BleApplicationContext.Device_Connection_Status;
  }
  return APP_BLE_IDLE;
}


/* USER CODE BEGIN FD */

/**
 * @brief  Disconnect the Link before switching to another BLE Application
 * @param  None
 * @retval None
 */
static void APP_BLE_CL_GAP_Disconnect(){
    tBleStatus result;
    result = aci_gap_terminate(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, ERR_RMT_USR_TERM_CONN);
    
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG(" \r\n\r** TERMINATE CONNECTION **  \r\n\r");
    }
    else
    {
      APP_DBG_MSG("-- APP_BLE_CL_GAP_Disconnect, Failed \r\n\r");
      BSP_LED_On(LED_RED);
    }
    return;
}


void APP_BLE_CL_Key_Button1_Action(void)
{
      P2PC_APP_SW1_Button_Action();
}

void APP_BLE_CL_Key_Button2_Action(void)
{
}

void APP_BLE_CL_Key_Button3_Action(void)
{
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
  uint8_t index;
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
    const char *name = "STM32WB";

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
    BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode = 0;
    for (index = 0; index < 16; index++)
    {
      BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data[index] = (uint8_t) index;
    }

    aci_gap_set_authentication_requirement(BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.bonding_mode,
                                           BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.mitm_mode,
                                           1,
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
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG(" \r\n\r** START GENERAL DISCOVERY (SCAN) **  \r\n\r");
#endif
    }
    else
    {
    /* USER CODE BEGIN BLE_SCAN_FAILED */
    UART_App_SendData("\r\nF\r\n", 5);
    BSP_LED_On(LED_RED);
    /* USER CODE END BLE_SCAN_FAILED */
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- BLE_App_Start_Limited_Disc_Req, Failed \r\n\r");
#endif
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
#if(CFG_DEBUG_APP_TRACE != 0)
  APP_DBG_MSG("\r\n\r** CREATE CONNECTION TO SERVER **  \r\n\r");
#endif

  if (BleApplicationContext.Device_Connection_Status != APP_BLE_CONNECTED_CLIENT)
  {
    result = aci_gap_create_connection(SCAN_P,
                                       SCAN_L,
                                       PUBLIC_ADDR, BD_Addr,//SERVER_REMOTE_BDADDR,
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
      CL_Connection_Status = APP_BLE_LP_CONNECTING;
      BSP_LED_Off(LED_RED);

    }
    else
    {
      /* USER CODE BEGIN BLE_CONNECT_FAILED */
      UART_App_SendData("\r\nF\r\n", 5);
      BSP_LED_On(LED_RED);
      /* USER CODE END BLE_CONNECT_FAILED */
      BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
      CL_Connection_Status = APP_BLE_IDLE;

    }
  }
  /* USER CODE BEGIN Connect_Request_2 */

  /* USER CODE END Connect_Request_2 */
  return;
}

//static void Switch_OFF_GPIO(){
///* USER CODE BEGIN Switch_OFF_GPIO */
//  BSP_LED_Off(LED_GREEN);
///* USER CODE END Switch_OFF_GPIO */
//}

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
/* USER CODE BEGIN FD_LOCAL_FUNCTIONS */
                              
static void BLE_CL_L2CAP_Conn_Update(void)
{
    tBleStatus result;
    
    result = aci_l2cap_connection_parameter_update_resp(l2cap_conn_upd_req_event->Connection_Handle,
                                                       l2cap_conn_upd_req_event->Interval_Min, l2cap_conn_upd_req_event->Interval_Max,
                                                       l2cap_conn_upd_req_event->Slave_Latency, l2cap_conn_upd_req_event->Timeout_Multiplier,
                                                       (l2cap_conn_upd_req_event->Interval_Min)/2, (l2cap_conn_upd_req_event->Interval_Max)/2,
                                                       l2cap_conn_upd_req_event->Identifier,
                                                       0x01);
    
    if( result == BLE_STATUS_SUCCESS )
    {
      UART_App_SendData("\r\nOK\r\n", 6);
    }
    else
    {
      UART_App_SendData("\r\nF\r\n", 5);
    }
    return;
}

static void Confirm_Pairing ( void )
{
  tBleStatus result;
  
  result =  aci_gap_numeric_comparison_value_confirm_yesno(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,0x01);
  
  if( result != BLE_STATUS_SUCCESS )
  {
    UART_App_SendData("\r\nF\r\n", 5);
  }
  
  return;
}

static void Pairing_Request ( void )
{
  tBleStatus result;
  
  result = aci_gap_send_pairing_req(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, 0x01);
  
  if( result != BLE_STATUS_SUCCESS )
  {
    UART_App_SendData("\r\nF\r\n", 5);
  }
  
  return;
}

/* USER CODE END FD_LOCAL_FUNCTIONS */
/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
//void hci_notify_asynch_evt(void* pdata)
//{
//  UTIL_SEQ_SetTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
//  return;
//}
//
//void hci_cmd_resp_release(uint32_t flag)
//{
//  UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
//  return;
//}
//
//void hci_cmd_resp_wait(uint32_t timeout)
//{
//  UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_HCI_CMD_EVT_RSP_ID);
//  return;
//}

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

//void SVCCTL_ResumeUserEventFlow( void )
//{
//  hci_resume_flow();
//  return;
//}

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
