/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_ble_hr.c
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
#include "tl.h"
#include "app_ble_hr.h"

#include "stm32_seq.h"
#include "shci.h"
#include "stm32_lpm.h"
#include "otp.h"
#include "dis_app.h"
#include "hrs_app.h"
#include "uart_app.h"
   
extern Disconnection_Status_t disconnection_status; //0 disconnection from Remote; 1 disconnection from Application to switch of BLE App; 2 disconnection ordered by AT command

extern PairingContext_t PairingContext;

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
   /**
   * ID of the Advertising Timeout
   */
   uint8_t Advertising_mgr_timer_Id;

}BleApplicationContext_t;
/* USER CODE BEGIN PTD */
  
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
*   Identity root key used to derive LTK and CSRK 
*/
static const uint8_t BLE_CFG_IR_VALUE[16] = CFG_BLE_IRK;

/**
* Encryption root key used to derive LTK and CSRK
*/
static const uint8_t BLE_CFG_ER_VALUE[16] = CFG_BLE_ERK;

PLACE_IN_SECTION("BLE_APP_CONTEXT") static BleApplicationContext_t BleApplicationContext;
PLACE_IN_SECTION("BLE_APP_CONTEXT") static uint16_t AdvIntervalMin, AdvIntervalMax;

static const char local_name[] = { AD_TYPE_COMPLETE_LOCAL_NAME ,'S','T','A','T','H','R'};//{ AD_TYPE_COMPLETE_LOCAL_NAME ,'H','R','S','T','M'};
uint8_t  manuf_data_hr[14] = {
    sizeof(manuf_data_hr)-1, AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x01/*SKD version */,
    0x00 /* Generic*/,
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
/* USER CODE BEGIN PV */

HRS_APP_ConnHandle_Not_evt_t handleNotificationHR;

APP_BLE_ConnStatus_t HR_Connection_Status;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void BLE_UserEvtRx( void * pPayload );
static void BLE_StatusNot( HCI_TL_CmdStatus_t status );
static void Ble_Tl_Init( void );
static void Ble_Hci_Gap_Gatt_Init(void);
static const uint8_t* BleGetBdAddress( void );
static void Adv_Request( APP_BLE_ConnStatus_t New_Status );
//static void Add_Advertisment_Service_UUID( uint16_t servUUID );
static void Adv_Mgr( void );
static void Adv_Update( void );

/* USER CODE BEGIN PFP */

static void APP_BLE_HR_GAP_Disconnect( void);
static void Confirm_Pairing( void );
static void Pairing_Request ( void );

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_HR_Init( void )
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
   * Initialization of the BLE App Context
   */
  BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
  HR_Connection_Status = APP_BLE_IDLE;
  BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0xFFFF;  
  /**
   * From here, all initialization are BLE application specific
   */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_ADV_UPDATE_ID, UTIL_SEQ_RFU, Adv_Update);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_HR_GAP_DISCON_ID, UTIL_SEQ_RFU, APP_BLE_HR_GAP_Disconnect);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONFIRM_PAIRING_ID, UTIL_SEQ_RFU, Confirm_Pairing);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_REQUEST_PAIRING_ID, UTIL_SEQ_RFU, Pairing_Request);
  /**
   * Initialization of ADV - Ad Manufacturer Element
   */
  
  /**
   * Initialize DIS Application
   */
  DISAPP_Init();

  /**
   * Initialize HRS Application
   */
  HRSAPP_Init();

  /**
   * Create timer to handle the connection state machine
   */

  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(BleApplicationContext.Advertising_mgr_timer_Id), hw_ts_SingleShot, Adv_Mgr);

  /**
   * Make device discoverable
   */
  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[0] = NULL;//AD_TYPE_16_BIT_SERV_UUID;
  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen = 0;//1;
//  Add_Advertisment_Service_UUID(HEART_RATE_SERVICE_UUID);
  /* Initialize intervals for reconnexion without intervals update */
  AdvIntervalMin = CFG_FAST_CONN_ADV_INTERVAL_MIN;
  AdvIntervalMax = CFG_FAST_CONN_ADV_INTERVAL_MAX;

  /**
  * Start to Advertise to be connected by Collector
   */
   Adv_Request(APP_BLE_FAST_ADV);

/* USER CODE BEGIN APP_BLE_Init_2 */  
   
   /**
   * Send a acknowledgement of the correct initialization of the p2p application
   */
   UART_App_SendData("\r\nHR OK\r\n", 9);
   

/* USER CODE END APP_BLE_Init_2 */
  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_HR_Notification( void *pckt )
{
  hci_event_pckt *event_pckt;
  evt_le_meta_event *meta_evt;
  evt_blue_aci *blue_evt;
  /* Pairing */
  aci_gap_numeric_comparison_value_event_rp0 *evt_numeric_value;
  aci_gap_pairing_complete_event_rp0 *pairing_complete;
  uint32_t numeric_value;
  char numeric_value_str[20];
  /*****/

  event_pckt = (hci_event_pckt*) ((hci_uart_pckt *) pckt)->data;

  switch (event_pckt->evt)
  {
    case EVT_DISCONN_COMPLETE:
    {
      hci_disconnection_complete_event_rp0 *disconnection_complete_event;
      disconnection_complete_event = (hci_disconnection_complete_event_rp0 *) event_pckt->data;

      if (disconnection_complete_event->Connection_Handle == BleApplicationContext.BleApplicationContext_legacy.connectionHandle)
      {
        BleApplicationContext.BleApplicationContext_legacy.connectionHandle = 0;
        BleApplicationContext.Device_Connection_Status = APP_BLE_IDLE;
        HR_Connection_Status = APP_BLE_IDLE;
#if(CFG_DEBUG_APP_TRACE != 0)
        APP_DBG_MSG("\r\n\r** DISCONNECTION EVENT WITH CLIENT \n");
#endif        
      }
      
      if (disconnection_status == TO_SWITCH_APP) //Disconnection done by application for switching app
      {
        handleNotificationHR.HR_Evt_Opcode = HR_DISCON_SWITCH_HANDLE_EVT;
        handleNotificationHR.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
        HRS_APP_Notification(&handleNotificationHR);
      }
      else { //Deconnection done by user/client
        PairingContext.PairingComplete = 0; //The pairing is reset to allow the user to perform a pairing on next connection
        
        handleNotificationHR.HR_Evt_Opcode = HR_DISCON_HANDLE_EVT;
        handleNotificationHR.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
        HRS_APP_Notification(&handleNotificationHR);
        /* restart advertising */
        Adv_Request(APP_BLE_FAST_ADV);
      }
}

    break; /* EVT_DISCONN_COMPLETE */

    case EVT_LE_META_EVENT:
    {
      meta_evt = (evt_le_meta_event*) event_pckt->data;
      /* USER CODE BEGIN EVT_LE_META_EVENT */

      /* USER CODE END EVT_LE_META_EVENT */
      switch (meta_evt->subevent)
      {
        case EVT_LE_CONN_UPDATE_COMPLETE: 
#if(CFG_DEBUG_APP_TRACE != 0)
          APP_DBG_MSG("\r\n\r** CONNECTION UPDATE EVENT WITH CLIENT \n");
#endif
          /* USER CODE BEGIN EVT_LE_CONN_UPDATE_COMPLETE */

          /* USER CODE END EVT_LE_CONN_UPDATE_COMPLETE */
          break;
        case EVT_LE_CONN_COMPLETE:
          {
          hci_le_connection_complete_event_rp0 *connection_complete_event;

          /**
           * The connection is done, there is no need anymore to schedule the LP ADV
           */
          connection_complete_event = (hci_le_connection_complete_event_rp0 *) meta_evt->data;
          
          HW_TS_Stop(BleApplicationContext.Advertising_mgr_timer_Id);

#if(CFG_DEBUG_APP_TRACE != 0)
          APP_DBG_MSG("EVT_LE_CONN_COMPLETE for connection handle 0x%x\n",
          connection_complete_event->Connection_Handle);
#endif
            if (BleApplicationContext.Device_Connection_Status == APP_BLE_LP_CONNECTING)
            {
              /* Connection as client */
              BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_CLIENT;
              HR_Connection_Status = APP_BLE_CONNECTED_CLIENT;
            }
            else
            {
              /* Connection as server */
              BleApplicationContext.Device_Connection_Status = APP_BLE_CONNECTED_SERVER;
              HR_Connection_Status = APP_BLE_CONNECTED_SERVER;
            }
            BleApplicationContext.BleApplicationContext_legacy.connectionHandle =
                connection_complete_event->Connection_Handle;
          /* USER CODE BEGIN HCI_EVT_LE_CONN_COMPLETE */
            
            handleNotificationHR.HR_Evt_Opcode = HR_CONN_HANDLE_EVT;
            handleNotificationHR.ConnectionHandle = BleApplicationContext.BleApplicationContext_legacy.connectionHandle;
            HRS_APP_Notification(&handleNotificationHR);

          /* USER CODE END HCI_EVT_LE_CONN_COMPLETE */
          }
        break; /* HCI_EVT_LE_CONN_COMPLETE */

        default:
          /* USER CODE BEGIN SUBEVENT_DEFAULT */

          /* USER CODE END SUBEVENT_DEFAULT */
          break;
      }
    }
    break; /* HCI_EVT_LE_META_EVENT */

    case EVT_VENDOR:
      blue_evt = (evt_blue_aci*) event_pckt->data;
      /* USER CODE BEGIN EVT_VENDOR */

      /* USER CODE END EVT_VENDOR */
      switch (blue_evt->ecode)
      {
      /* USER CODE BEGIN ecode */

      /* USER CODE END ecode */
        case EVT_BLUE_GAP_PROCEDURE_COMPLETE:
#if(CFG_DEBUG_APP_TRACE != 0)
        APP_DBG_MSG("\r\n\r** EVT_BLUE_GAP_PROCEDURE_COMPLETE \n");
#endif
        /* USER CODE BEGIN EVT_BLUE_GAP_PROCEDURE_COMPLETE */

        /* USER CODE END EVT_BLUE_GAP_PROCEDURE_COMPLETE */
        break; /* EVT_BLUE_GAP_PROCEDURE_COMPLETE */
        
        /* Pairing */
        case EVT_BLUE_GAP_NUMERIC_COMPARISON_VALUE:
            UART_App_SendData("\r\n", 2);
            PairingContext.PairingConfirmRequested = 1;
            evt_numeric_value = (aci_gap_numeric_comparison_value_event_rp0 *)blue_evt->data;
            numeric_value = evt_numeric_value->Numeric_Value;
            sprintf(numeric_value_str, "%d", (int)numeric_value);
            UART_App_SendData((char *)numeric_value_str, (uint8_t)(strlen(numeric_value_str)));
            UART_App_SendData(":PREQ?\r\n", 8);
//            APP_DBG_MSG("numeric_value = %x\n", numeric_value);
//            aci_gap_numeric_comparison_value_confirm_yesno(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,0x01);
        break;
        
        case EVT_BLUE_GAP_PAIRING_CMPLT: //aci_gap_pairing_complete_event_process
            pairing_complete = (aci_gap_pairing_complete_event_rp0*)blue_evt->data;
//            APP_DBG_MSG("BLE_CTRL_App_Notification: EVT_BLUE_GAP_PAIRING_CMPLT, pairing_complete->Status = %d\n",pairing_complete->Status);
            if (pairing_complete->Status == 0)
            {
              UART_App_SendData("\r\nP\r\n", 5);
              PairingContext.PairingComplete = 1;
            }
            else UART_App_SendData("\r\nF\r\n", 5);
        break;
      }
      break; /* EVT_VENDOR */

        default:
        /* USER CODE BEGIN ECODE_DEFAULT*/

        /* USER CODE END ECODE_DEFAULT*/
          break;
  }

  return (SVCCTL_UserEvtFlowEnable);
}

APP_BLE_ConnStatus_t APP_BLE_HR_Get_Server_Connection_Status(void)
{
    return BleApplicationContext.Device_Connection_Status;
}

/**
 * @brief  Disconnect the Link before switching to another BLE Application
 * @param  None
 * @retval None
 */
static void APP_BLE_HR_GAP_Disconnect(){
    tBleStatus result;
    result = aci_gap_terminate(BleApplicationContext.BleApplicationContext_legacy.connectionHandle, ERR_RMT_USR_TERM_CONN);
    
    if (result == BLE_STATUS_SUCCESS)
    {
      APP_DBG_MSG(" \r\n\r** TERMINATE CONNECTION **  \r\n\r");
    }
    else
    {
      APP_DBG_MSG("-- APP_BLE_HR_GAP_Disconnect, Failed \r\n\r");
      BSP_LED_On(LED_RED);
    }
  return;
}

/* USER CODE BEGIN FD*/
void APP_BLE_HR_Key_Button1_Action(void)
{

}

void APP_BLE_HR_Key_Button2_Action(void)
{
  
}

void APP_BLE_HR_Key_Button3_Action(void)
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

  /* BLE MAC in ADV Packet */
  manuf_data_hr[ sizeof(manuf_data_hr)-6] = bd_addr[5];
  manuf_data_hr[ sizeof(manuf_data_hr)-5] = bd_addr[4];
  manuf_data_hr[ sizeof(manuf_data_hr)-4] = bd_addr[3];
  manuf_data_hr[ sizeof(manuf_data_hr)-3] = bd_addr[2];
  manuf_data_hr[ sizeof(manuf_data_hr)-2] = bd_addr[1];
  manuf_data_hr[ sizeof(manuf_data_hr)-1] = bd_addr[0];
  
  /**
   * Write Identity root key used to derive LTK and CSRK 
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
   * Write random bd_address
   */
   /* random_bd_address = R_bd_address;
    aci_hal_write_config_data(CONFIG_DATA_RANDOM_ADDRESS_WR,
    CONFIG_DATA_RANDOM_ADDRESS_LEN,
                            (uint8_t*) random_bd_address);
  */

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
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.OOB_Data_Present = 0;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMin = 8;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.encryptionKeySizeMax = 16;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Use_Fixed_Pin = 1;
  BleApplicationContext.BleApplicationContext_legacy.bleSecurityParam.Fixed_Pin = 1111;
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

    /**
     * Stop the timer, it will be restarted for a new shot
     * It does not hurt if the timer was not running
     */
    HW_TS_Stop(BleApplicationContext.Advertising_mgr_timer_Id);

#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("First index in %d state \n",
    BleApplicationContext.Device_Connection_Status);
#endif
    if ((New_Status == APP_BLE_LP_ADV)
        && ((BleApplicationContext.Device_Connection_Status == APP_BLE_FAST_ADV)
            || (BleApplicationContext.Device_Connection_Status == APP_BLE_LP_ADV)))
    {
      /* Connection in ADVERTISE mode have to stop the current advertising */
      ret = aci_gap_set_non_discoverable();
      if (ret == BLE_STATUS_SUCCESS)
      {
#if(CFG_DEBUG_APP_TRACE != 0)
        APP_DBG_MSG("Successfully Stopped Advertising");
#endif
        }
      else
      {
#if(CFG_DEBUG_APP_TRACE != 0)
        APP_DBG_MSG("Stop Advertising Failed , result: %d \n", ret);
#endif
        }
    }

    BleApplicationContext.Device_Connection_Status = New_Status;
    HR_Connection_Status = New_Status;
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
    
    /* Update Advertising data */
    ret = aci_gap_update_adv_data(sizeof(manuf_data_hr), (uint8_t*) manuf_data_hr);

    if (ret == BLE_STATUS_SUCCESS)
    {
      if (New_Status == APP_BLE_FAST_ADV)
      {
        APP_DBG_MSG("Successfully Start Fast Advertising " );
        /* Start Timer to STOP ADV - TIMEOUT */
        HW_TS_Start(BleApplicationContext.Advertising_mgr_timer_Id, INITIAL_ADV_TIMEOUT);
      }
      else
      {
#if(CFG_DEBUG_APP_TRACE != 0)
        APP_DBG_MSG("Successfully Start Low Power Advertising ");
#endif
      }
      UART_App_SendData("\r\nA\r\n", 5); 
    }
    else
    {
      if (New_Status == APP_BLE_FAST_ADV)
      {
#if(CFG_DEBUG_APP_TRACE != 0)
        APP_DBG_MSG("Start Fast Advertising Failed , result: %d \n", ret);
#endif
      }
      else
      {
#if(CFG_DEBUG_APP_TRACE != 0)
        APP_DBG_MSG("Start Low Power Advertising Failed , result: %d \n", ret);
#endif
      }
      UART_App_SendData("\r\nF\r\n", 5);
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

/* USER CODE BEGIN FD_LOCAL_FUNCTION */
static void Confirm_Pairing ( void )
{
  aci_gap_numeric_comparison_value_confirm_yesno(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,0x01);
}

static void Pairing_Request ( void )
{
  aci_gap_slave_security_req(BleApplicationContext.BleApplicationContext_legacy.connectionHandle);
}
/* USER CODE END FD_LOCAL_FUNCTION */

/*************************************************************
 *
 *SPECIFIC FUNCTIONS
 *
 *************************************************************/
//static void Add_Advertisment_Service_UUID( uint16_t servUUID )
//{
//  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] =
//      (uint8_t) (servUUID & 0xFF);
//  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;
//  BleApplicationContext.BleApplicationContext_legacy.advtServUUID[BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen] =
//      (uint8_t) (servUUID >> 8) & 0xFF;
//  BleApplicationContext.BleApplicationContext_legacy.advtServUUIDlen++;
//
//  return;
//}

static void Adv_Mgr( void )
{
  /**
   * The code shall be executed in the background as an aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask(1 << CFG_TASK_ADV_UPDATE_ID, CFG_SCH_PRIO_0);

  return;
}

static void Adv_Update( void )
{
  Adv_Request(APP_BLE_LP_ADV);

  return;
}

/* USER CODE BEGIN FD_SPECIFIC_FUNCTIONS */

/* USER CODE END FD_SPECIFIC_FUNCTIONS */
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

//void hci_cmd_resp_release(uint32_t flag)
//{
//  UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
//  return;
//}

//void hci_cmd_resp_wait(uint32_t timeout)
//{
//  UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
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
