/**
  ******************************************************************************
 * @file    dt_client_app.c
 * @author  MCD Application Team
 * @brief   data throughput client Application
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
#include "app_common.h"

#include "dbg_trace.h"

#include "ble.h"
#include "app_ble.h"    
#include "dt_client_app.h"
#include "dts.h"

#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
#if 0
typedef enum
{
  GATT_PROC_MTU_UPDATE,
  GATT_PROC_DISC_ALL_PRIMARY_SERVICES,
  GATT_PROC_DISC_ALL_CHAR_OF_DT_SERVICE,
  GATT_PROC_DISC_TX_CHAR_DESC,
} GattProcId_t;

typedef struct
{
  /**
   * connection handle
   */
  uint16_t connHandle;

  /**
   * handle of the Data Transfer service
   */
  uint16_t DataTransferServiceHandle;

  /**
   * end handle of the Data Transfer service
   */
  uint16_t DataTransferServiceEndHandle;

  /**
   * handle of the Tx characteristic
   *
   */
  uint16_t DataTransferTxCharHdle;

  /**
   * handle of the client configuration
   * descriptor of Tx characteristic
   */
  uint16_t DataTransferTxDescHandle;

} DataTransferContext_t;
#else
typedef enum
{  
  DTC_UNINITIALIZED                                 = 0,
  DTC_INITIALIZED                                   = 1,
  DTC_IDLE                                          = 2,
  DTC_DISC_ALL_PRIMARY_SERVICES                     = 3,
  DTC_DISCOVER_CHARACS                              = 4,
  DTC_DISCOVER_DESC                                 = 5,
  DTC_READ_TX                                       = 6,
  DTC_READ_TX_CCC                                   = 7,
  DTC_READ_RX                                       = 8,
  DTC_WRITE_TX                                      = 9,
  DTC_ENABLE_TX_NOTIFICATION                        = 10,
  DTC_DISABLE_TX_NOTIFICATION                       = 11,
  DTC_ENABLE_THROUGHPUT_NOTIFICATION                = 12,
  DTC_DISABLE_THROUGHPUT_NOTIFICATION               = 13,
  DTC_CONNECTED                                     = 14,
  DTC_CONNECTED_ADVERTISING                         = 15,
  DTC_PROC_MTU_UPDATE                               = 16
}  DTC_SVC_ProfileState;

typedef enum
{
  DTC_APP_FLOW_OFF,
  DTC_APP_FLOW_ON
} DTC_App_Flow_Status_t;

typedef enum
{
  DTC_APP_TRANSFER_REQ_OFF,
  DTC_APP_TRANSFER_REQ_ON
} DTC_App_Transfer_Req_Status_t;

typedef struct
{
  /**
   * state of the CRC
   * state machine
   */ 
  DTC_SVC_ProfileState state;

  /**
   * connection handle 
   */ 
  uint16_t connHandle;

  /**
   * handle of the CRC service
   */ 
  uint16_t ServiceHandle;

  /**
   * end handle of the CRC service
   */ 
  uint16_t ServiceEndHandle;

  /**
   * handle of the TX characteristic 
   * 
   */ 
  uint16_t TXCharHdle;

  /**
   * handles of the TX Client Characteristic Configuration descriptor 
   * 
   */ 
  uint16_t TXCCCDescHdle;

  /**
   * handles of the RX characteristic 
   * 
   */ 
  uint16_t RXCharHdle;

  /**
   * handle of the Throughput characteristic 
   * 
   */ 
  uint16_t ThroughputCharHdle;

  /**
   * handles of the Throughput Client Characteristic Configuration descriptor 
   * 
   */ 
  uint16_t ThroughputCCCDescHdle;

  DTS_STM_Payload_t TxData;
  DTC_App_Transfer_Req_Status_t NotificationTransferReq;
  DTC_App_Transfer_Req_Status_t ButtonTransferReq;
  DTC_App_Flow_Status_t DtFlowStatus;
}DTC_Context_t;

#endif

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define UNPACK_2_BYTE_PARAMETER(ptr)  \
    (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
    (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

#define DEFAULT_TS_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */          
#define TIMEUNIT                          1
#define DATA_THROUGHPUT_MEASUREMENT       (TIMEUNIT*DEFAULT_TS_MEASUREMENT_INTERVAL)

#define READ_RX                                                                0
#define READ_TX                                                                1
#define READ_RX_CCC_DESC                                                       2
          
#define WRITE_TX                                                               0
#define WRITE_ENABLE_RX_NOTIFICATION                                           1
#define WRITE_DISABLE_RX_NOTIFICATION                                          2

#define TX_CHAR                                                                1
#define RX_CHAR                                                                2

#define BOUNCE_THRESHOLD                                                   20U
#define LONG_PRESS_THRESHOLD                                               1000U
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static DTC_Context_t DTC_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */
static uint16_t packet_lost;
static uint32_t DataThroughputValue;
static uint32_t packet_lost_local;
static uint32_t TotalDataReceived = 0;
static uint32_t Nbr_packet = 0;
static DTS_STM_App_Notification_evt_t NotificationData;
static uint8_t TimerDataThroughput_Id;
uint32_t N=0;
uint32_t DataTransfered = 0;
uint16_t Att_Mtu_Exchanged;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private functions prototypes-----------------------------------------------*/
static void DTC_App_Button1_Trigger_Received( void );
static void DTC_App_Button2_Trigger_Received( void );
static void DTC_App_Button3_Trigger_Received( void );
static void SendData(void);
static SVCCTL_EvtAckStatus_t DTC_Event_Handler( void *Event );
static void DataThroughputTimer( void );
static void DataThroughput_Calculation(void);
static void GattProcReq(DTC_SVC_ProfileState GattProcId);
static uint8_t Notification_Data_Buffer[DATA_NOTIFICATION_MAX_PACKET_SIZE]; /* DATA_NOTIFICATION_MAX_PACKET_SIZE data + CRC */
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/*************************************************************
 *
 * PUBLIC FUNCTIONS
 *
 *************************************************************/
void DTC_App_Init( void )
{
  uint8_t i;

  SVCCTL_RegisterCltHandler(DTC_Event_Handler);

  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW1_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DTC_App_Button1_Trigger_Received);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW2_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DTC_App_Button2_Trigger_Received);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW3_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DTC_App_Button3_Trigger_Received);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_APP_DATA_THROUGHPUT_ID, UTIL_SEQ_RFU, DataThroughput_Calculation);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_TRANSFER_UPDATE_ID, UTIL_SEQ_RFU, SendData);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_PHY_UPDATE_ID, UTIL_SEQ_RFU, BLE_SVC_GAP_Change_PHY);

  /* Create timer for Data Throughput */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(TimerDataThroughput_Id), hw_ts_SingleShot, DataThroughputTimer);

  /**
   * Initialize data buffer
   */
  for (i=0 ; i<(DATA_NOTIFICATION_MAX_PACKET_SIZE-1) ; i++)
  {
    Notification_Data_Buffer[i] = i;
  }

  DTC_Context.state = DTC_IDLE;
  DTC_Context.connHandle = 0xFFFF;
  DTC_Context.NotificationTransferReq = DTC_APP_TRANSFER_REQ_OFF;
  DTC_Context.ButtonTransferReq = DTC_APP_TRANSFER_REQ_OFF;
  DTC_Context.DtFlowStatus = DTC_APP_FLOW_ON;

  APP_DBG_MSG("-- DT CLIENT INITIALIZED \n");

  return;
}

void DTC_App_KeyButton1Action( void )
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_SW1_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
}

void DTC_App_KeyButton2Action( void )
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_SW2_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
}

void DTC_App_KeyButton3Action( void )
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_SW3_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
}

void DTC_App_LinkReadyNotification(uint16_t ConnectionHandle)
{
  DTC_Context.connHandle = ConnectionHandle;

#if(BLE_CFG_CENTRAL != 1)
  GattProcReq(DTC_PROC_MTU_UPDATE);
#endif
#if(BLE_CFG_PERIPHERAL != 1)
  GattProcReq(DTC_DISC_ALL_PRIMARY_SERVICES);
  GattProcReq(DTC_DISCOVER_CHARACS);
  GattProcReq(DTC_DISCOVER_DESC);
  GattProcReq(DTC_ENABLE_TX_NOTIFICATION);
  GattProcReq(DTC_ENABLE_THROUGHPUT_NOTIFICATION);
#endif
  
#if(BLE_CFG_CENTRAL != 0)
    UTIL_SEQ_SetTask(1<<CFG_TASK_CONN_UPDATE_ID, CFG_SCH_PRIO_0); 
#endif
  return;
}

/*************************************************************
 *
 * CALLBACK FUNCTIONS
 *
 *************************************************************/
static SVCCTL_EvtAckStatus_t DTC_Event_Handler( void *Event )
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt * event_pckt;
  evt_blecore_aci * blecore_evt;
  uint8_t CRC_Result;
  uint8_t CRC_Received;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *) (((hci_uart_pckt*) Event)->data);

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
    {
      blecore_evt = (evt_blecore_aci*) event_pckt->data;
      switch (blecore_evt->ecode)
      {

        case ACI_GATT_TX_POOL_AVAILABLE_VSEVT_CODE:
        {
          Resume_Write();
        }
        break;
        
        case ACI_ATT_READ_BY_GROUP_TYPE_RESP_VSEVT_CODE:
        {
          aci_att_read_by_group_type_resp_event_rp0 *pr = (void*) blecore_evt->data;
          uint8_t numServ, i, idx;
          uint16_t uuid, handle;

          handle = pr->Connection_Handle;

          APP_DBG_MSG("DTC_Event_Handler: ACI_ATT_READ_BY_GROUP_TYPE_RESP_VSEVT_CODE\n");

          DTC_Context.connHandle = handle;

          numServ = (pr->Data_Length) / pr->Attribute_Data_Length;

          /* the event data will be
           * 2bytes start handle
           * 2bytes end handle
           * 2 or 16 bytes data
           * we are interested only if the UUID is 16 bit.
           * So check if the data length is 6
           */
#if (UUID_128BIT_FORMAT==1)
          idx = 16;
          if (pr->Attribute_Data_Length == 20)
#else
          idx = 4;
          if (pr->Attribute_Data_Length == 6)
#endif
          {
            for (i = 0; i < numServ; i++)
            {
              uuid = UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx]);
              if (uuid == DT_SERVICE_UUID)
              {
#if (UUID_128BIT_FORMAT==1)
                DTC_Context.ServiceHandle = UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx-16]);
                DTC_Context.ServiceEndHandle = UNPACK_2_BYTE_PARAMETER (&pr->Attribute_Data_List[idx-14]);
#else
                DTC_Context.ServiceHandle =
                    UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx - 4]);
                DTC_Context.ServiceEndHandle =
                    UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx - 2]);
#endif
                APP_DBG_MSG("DTC_Event_Handler: DT_SERVICE_UUID found !\n");
              }
              idx += 6;
            }
          }
        }
        break;

        case ACI_ATT_READ_BY_TYPE_RESP_VSEVT_CODE:
        {
          aci_att_read_by_type_resp_event_rp0 *pr = (void*) blecore_evt->data;
          uint8_t idx;
          uint16_t uuid, handle;

          APP_DBG_MSG("DTC_Event_Handler: ACI_ATT_READ_BY_TYPE_RESP_VSEVT_CODE\n");

          /* the event data will be
           * 2 bytes start handle
           * 1 byte char properties
           * 2 bytes handle
           * 2 or 16 bytes data
           */
#if (UUID_128BIT_FORMAT==1)
          idx = 17;
          if (pr->Handle_Value_Pair_Length == 21)
#else
          idx = 5;
          /* we are interested in only 16 bit UUIDs */
          if (pr->Handle_Value_Pair_Length == 7)
#endif
          {
//            pr->Data_Length -= 1;
            while(pr->Data_Length > 0)
            {
              uuid = UNPACK_2_BYTE_PARAMETER(&pr->Handle_Value_Pair_Data[idx]);
              /* store the characteristic handle not the attribute handle */
#if (UUID_128BIT_FORMAT==1)
              handle = UNPACK_2_BYTE_PARAMETER(&pr->Handle_Value_Pair_Data[idx-14]);
#else
              handle = UNPACK_2_BYTE_PARAMETER(&pr->Handle_Value_Pair_Data[idx - 2]);
#endif
              if (uuid == DT_TX_CHAR_UUID)
              {
                DTC_Context.TXCharHdle = handle;
                APP_DBG_MSG("DTC_Event_Handler: DT_TX_CHAR_UUID found !\n");
              }
              else if (uuid == DT_RX_CHAR_UUID)
              {
                DTC_Context.RXCharHdle = handle;
                APP_DBG_MSG("DTC_Event_Handler: DT_RX_CHAR_UUID found !\n");
              }
              else if (uuid == DT_THROUGHPUT_CHAR_UUID)
              {
                DTC_Context.ThroughputCharHdle = handle;
                APP_DBG_MSG("DTC_Event_Handler: DT_THROUGHPUT_CHAR_UUID found !\n");
              }
#if (UUID_128BIT_FORMAT==1)
              pr->Data_Length -= 21;
              idx += 21;
#else
              pr->Data_Length -= 7;
              idx += 7;
#endif
            }
          }
        }
        break;
        
        case ACI_ATT_FIND_INFO_RESP_VSEVT_CODE:
        {
          aci_att_find_info_resp_event_rp0 *pr = (void*) blecore_evt->data;
          uint8_t numDesc, idx, i;
          uint16_t uuid, handle;

          /*
           * event data will be of the format
           * 2 bytes handle
           * 2 bytes UUID
           */
          numDesc = (pr->Event_Data_Length) / 4;
          /* we are interested only in 16 bit UUIDs */
          idx = 0;
          if (pr->Format == UUID_TYPE_16)
          {
            for (i = 0; i < numDesc; i++)
            {
              handle = UNPACK_2_BYTE_PARAMETER(&pr->Handle_UUID_Pair[idx]);
              uuid = UNPACK_2_BYTE_PARAMETER(&pr->Handle_UUID_Pair[idx + 2]);

              if (uuid == CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID)
              {
                if(handle == DTC_Context.TXCharHdle + 1)
                {
                  APP_DBG_MSG("DTC_Event_Handler: DTC_DISCOVER_TX_CHAR_DESC ready to enable notification\n");
                  DTC_Context.TXCCCDescHdle = handle;
                }
                else if(handle == DTC_Context.ThroughputCharHdle + 1) 
                {
                  APP_DBG_MSG("DTC_Event_Handler: DTC_DISCOVER_THROUGHPUT_CHAR_DESC ready to enable notification\n");
                  DTC_Context.ThroughputCCCDescHdle = handle;
                }
              }
              idx += 4;
            }
          }
        }
        break; /*ACI_ATT_FIND_INFO_RESP_VSEVT_CODE*/

        case ACI_GATT_NOTIFICATION_VSEVT_CODE:
        {
          aci_gatt_notification_event_rp0 *pr = (void*) blecore_evt->data;

          if ((pr->Attribute_Handle == DTC_Context.TXCharHdle) && 
              (pr->Attribute_Value_Length > (2)))
          {
            NotificationData.DataTransfered.Length = pr->Attribute_Value_Length;
            NotificationData.DataTransfered.pPayload = (pr->Attribute_Value);
            NotificationData.DataTransfered.pPayload_n = *((uint32_t*) &(pr->Attribute_Value[0]));
            __disable_irq();
            if (NotificationData.DataTransfered.pPayload_n >= (NotificationData.DataTransfered.pPayload_n_1 + 2))
            {
              packet_lost +=
                  ((NotificationData.DataTransfered.pPayload_n - NotificationData.DataTransfered.pPayload_n_1) - 1);
            }
            __enable_irq();
            /* CRC computation */
            CRC_Result = APP_BLE_ComputeCRC8((uint8_t*) (pr->Attribute_Value), (pr->Attribute_Value_Length) - 1);
            /* get low weight byte */
            CRC_Received = (uint8_t) (pr->Attribute_Value[Att_Mtu_Exchanged-1]);

            if (CRC_Received != CRC_Result)
            {
              N+=1;
              APP_DBG_MSG("** data error **  N= %d \r\n",(int)N);
            }
            NotificationData.DataTransfered.pPayload_n_1 = NotificationData.DataTransfered.pPayload_n;
            if (DataTransfered == 0)
            {
              HW_TS_Start(TimerDataThroughput_Id, DATA_THROUGHPUT_MEASUREMENT);
            }
            DataTransfered += NotificationData.DataTransfered.Length;
          }
        }
        break;/* end ACI_GATT_NOTIFICATION_VSEVT_CODE */

        case ACI_GATT_PROC_COMPLETE_VSEVT_CODE:
        {
          UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);
        }
        break; /*ACI_GATT_PROC_COMPLETE_VSEVT_CODE*/

        default:
          break;
      }
    }
    break;
    
    default:
      break;
 }

  return (return_value);
}

  /*************************************************************
   *
   * LOCAL FUNCTIONS
   *
   *************************************************************/
  static void DataThroughput_Calculation( void )
  {
    uint32_t MoyData;

    if (Nbr_packet < 600)
    {
      Nbr_packet++;
      TotalDataReceived += DataThroughputValue;
    }
    if (Nbr_packet == 600)
    {
      MoyData = TotalDataReceived / 600;
      APP_DBG_MSG("Moy = %ld bytes/s \n",MoyData);
      TotalDataReceived = 0;
      Nbr_packet = 0;
    }

    APP_DBG_MSG("DataThroughput = %ld bytes/s lost = %ld \n",DataThroughputValue, packet_lost_local);
  }

  static void DataThroughputTimer( void )
  {
    DataThroughputValue = (uint32_t)(DataTransfered / TIMEUNIT);
    packet_lost_local = (uint32_t)(packet_lost / TIMEUNIT);
    DataTransfered = 0;
    packet_lost = 0;

    UTIL_SEQ_SetTask(1 << CFG_TASK_APP_DATA_THROUGHPUT_ID, CFG_SCH_PRIO_0);
    return;
  }

  static void GattProcReq(DTC_SVC_ProfileState GattProcId)
  {
    tBleStatus status;

    switch(GattProcId)
    {
      case DTC_PROC_MTU_UPDATE:
        {
          APP_DBG_MSG("change ATT MTU size \n");

          status = aci_gatt_exchange_config(DTC_Context.connHandle);
          if (status != BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("change MTU cmd failure: 0x%x\n", status);
          }
          UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

          APP_DBG_MSG("DTC_PROC_MTU_UPDATE complete event received \n");
        }
        break;

      case DTC_DISC_ALL_PRIMARY_SERVICES:
        {
          APP_DBG_MSG("Discover all primary services \n");

          status = aci_gatt_disc_all_primary_services(DTC_Context.connHandle);
          if (status != BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("Discover all primary services cmd failure: 0x%x\n", status);
          }
          UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

          APP_DBG_MSG("DTC_DISC_ALL_PRIMARY_SERVICES complete event received \n");
        }
        break;

      case DTC_DISCOVER_CHARACS:
        {
          APP_DBG_MSG("Discover all char of service \n");

          status = aci_gatt_disc_all_char_of_service(
              DTC_Context.connHandle,
              DTC_Context.ServiceHandle,
              DTC_Context.ServiceEndHandle);
          if (status != BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("Discover all char of service cmd failure: 0x%x\n", status);
          }
          UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

          APP_DBG_MSG("DTC_DISCOVER_CHARACS complete event received \n");
        }
        break;

      case DTC_DISCOVER_DESC:
        {
          APP_DBG_MSG("Discover char descriptors \n");

          status = aci_gatt_disc_all_char_desc(
              DTC_Context.connHandle,
              DTC_Context.ServiceHandle,
              DTC_Context.ServiceEndHandle);
          if (status != BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("Discover char descriptors cmd failure: 0x%x\n", status);
          }
          UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

          APP_DBG_MSG("DTC_DISCOVER_DESC complete event received \n");
        }
        break;

      case DTC_ENABLE_TX_NOTIFICATION:
        {
          uint8_t notification[2] = {0x01, 0x00};

          APP_DBG_MSG("Enable Tx char descriptors \n");
          status = aci_gatt_write_char_desc(DTC_Context.connHandle,
                                            DTC_Context.TXCCCDescHdle,
                                            2,
                                            (uint8_t *) &notification[0]);

          if (status != BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("Enable Tx char descriptors cmd failure: 0x%x\n", status);
          }
          UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

          APP_DBG_MSG("DTC_ENABLE_TX_NOTIFICATION complete event received \n");
        }
        break;

      case DTC_ENABLE_THROUGHPUT_NOTIFICATION:
        {
          uint8_t notification[2] = {0x01, 0x00};

          APP_DBG_MSG("Enable Throughput char descriptors \n");
          status = aci_gatt_write_char_desc(DTC_Context.connHandle,
                                            DTC_Context.ThroughputCCCDescHdle,
                                            2,
                                            (uint8_t *) &notification[0]);

          if (status != BLE_STATUS_SUCCESS)
          {
            APP_DBG_MSG("Enable Throughput char descriptors cmd failure: 0x%x\n", status);
          }
          UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

          APP_DBG_MSG("DTC_ENABLE_THROUGHPUT_NOTIFICATION complete event received \n");
        }
        break;

      default:
        break;
    }
    return;
  }

void DTC_Button1TriggerReceived( void )
{
  if(DTC_Context.ButtonTransferReq != DTC_APP_TRANSFER_REQ_OFF)
  {
    BSP_LED_Off(LED_BLUE);
    APP_DBG_MSG("LED BLUE OFF");
    DTC_Context.ButtonTransferReq = DTC_APP_TRANSFER_REQ_OFF;
  }
  else
  {
    BSP_LED_On(LED_BLUE);
    APP_DBG_MSG("LED BLUE ON");
    DTC_Context.ButtonTransferReq = DTC_APP_TRANSFER_REQ_ON;
    UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
  }

  return;
}

void DTC_Button2TriggerReceived( void )
{
  APP_DBG_MSG("**CHANGE PHY \n");
  UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_PHY_UPDATE_ID, CFG_SCH_PRIO_0);
  return;
}

static void Appli_UpdateButtonState(Button_TypeDef button, int isPressed)
{
  uint32_t t0 = 0,t1 = 1;

  t0 = HAL_GetTick(); /* SW3 press timing */
  
  while(BSP_PB_GetState(button) == BUTTON_PRESSED);
  t1 = HAL_GetTick(); /* SW3 release timing */
  
  if((t1 - t0) > LONG_PRESS_THRESHOLD)
  {
    if(button == BUTTON_SW1)
    {
      /* Button 1 long press action */
      DTC_Button1TriggerReceived();
    }
    else if(button == BUTTON_SW2)
    {
      /* Button 2 long press action */
      DTC_Button2TriggerReceived();
    }
    else if(button == BUTTON_SW3)
    {
      /* Button 3 long press action */
      APP_DBG_MSG("clear database \n");
      BLE_SVC_GAP_Clear_DataBase();
    }
  }
  else if((t1 - t0) > BOUNCE_THRESHOLD)
  {
    if(button == BUTTON_SW1)
    {
      /* Button 1 short press action */
      DTC_Button1TriggerReceived();
    }
    else if(button == BUTTON_SW2)
    {
      /* Button 2 short press action */
      DTC_Button2TriggerReceived();
    }
    else if(button == BUTTON_SW3)
    {
      /* Button 3 short press action */
      APP_DBG_MSG("pairing request \n");
      BLE_GAP_Pairing_Req();
    }
  }
}

static void DTC_App_Button1_Trigger_Received(void)
{
  Appli_UpdateButtonState(BUTTON_SW1, BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED);
  
  return;
}

static void DTC_App_Button2_Trigger_Received(void)
{
  Appli_UpdateButtonState(BUTTON_SW2, BSP_PB_GetState(BUTTON_SW2) == BUTTON_PRESSED);
  
  return;
}

static void DTC_App_Button3_Trigger_Received(void)
{
  Appli_UpdateButtonState(BUTTON_SW3, BSP_PB_GetState(BUTTON_SW3) == BUTTON_PRESSED);
  
  return;
}

static void SendData( void )
{
  tBleStatus status = BLE_STATUS_INVALID_PARAMS;
  uint8_t crc_result;

  if( (DTC_Context.ButtonTransferReq != DTC_APP_TRANSFER_REQ_OFF) && 
      (DTC_Context.DtFlowStatus != DTC_APP_FLOW_OFF) )
  {   
    /*Data Packet to send to remote*/
    Notification_Data_Buffer[0] += 1;
    /* compute CRC */
    crc_result = APP_BLE_ComputeCRC8((uint8_t*) Notification_Data_Buffer, (Att_Mtu_Exchanged - 1));
    Notification_Data_Buffer[Att_Mtu_Exchanged - 1] = crc_result;

    DTC_Context.TxData.pPayload = Notification_Data_Buffer;
    DTC_Context.TxData.Length =  Att_Mtu_Exchanged;

    
    status = aci_gatt_write_without_resp(DTC_Context.connHandle,
                                         DTC_Context.RXCharHdle,
                                         Att_Mtu_Exchanged,
                                         (const uint8_t*)(DTC_Context.TxData.pPayload));
    
    if (status == BLE_STATUS_INSUFFICIENT_RESOURCES)
    {
      DTC_Context.DtFlowStatus = DTC_APP_FLOW_OFF;
      (Notification_Data_Buffer[0])-=1;
    }
    else
    {
      UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
    }
  }
  return;
}

void Resume_Write(void)
{
  DTC_Context.DtFlowStatus = DTC_APP_FLOW_ON;
}
/* USER CODE BEGIN FD */

/* USER CODE END FD */
