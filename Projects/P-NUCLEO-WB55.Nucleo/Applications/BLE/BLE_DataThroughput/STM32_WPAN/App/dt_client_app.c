/**
 ******************************************************************************
 * @file    dt_client_app.c
 * @author  MCD Application Team
 * @brief   data throughput client Application
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
#include "app_ble.h"    
#include "dt_client_app.h"
#include "dts.h"

#include "stm32_seq.h"

/* Private defines -----------------------------------------------------------*/
#define UNPACK_2_BYTE_PARAMETER(ptr)  \
    (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
    (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

#define DEFAULT_TS_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */          
#define TIMEUNIT                          1
#define DATA_THROUGHPUT_MEASUREMENT       (TIMEUNIT*DEFAULT_TS_MEASUREMENT_INTERVAL)

/* Private typedef -----------------------------------------------------------*/
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

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DataTransferContext_t DataTransferClientContext;
static uint16_t packet_lost;
static uint32_t DataThroughputValue;
static uint32_t packet_lost_local;
static uint32_t TotalDataReceived = 0;
static uint32_t Nbr_packet = 0;
static DTS_STM_App_Notification_evt_t NotificationData;
static uint8_t TimerDataThroughput_Id;
uint32_t N=0;
uint32_t DataTransfered = 0;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t DTC_Event_Handler( void *Event );
static void DataThroughputTimer( void );
static void DataThroughput_Calculation(void);
static void GattProcReq(GattProcId_t GattProcId);

/* Functions Definition ------------------------------------------------------*/
/*************************************************************
 *
 * PUBLIC FUNCTIONS
 *
 *************************************************************/
void DTC_App_Init( void )
{
  SVCCTL_RegisterCltHandler(DTC_Event_Handler);

  UTIL_SEQ_RegTask( 1<< CFG_TASK_APP_DATA_THROUGHPUT_ID, UTIL_SEQ_RFU, DataThroughput_Calculation);

  DataTransferClientContext.connHandle = 0xFFFF;
  /* Create timer for Data Throughput */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(TimerDataThroughput_Id), hw_ts_SingleShot, DataThroughputTimer);

  APP_DBG_MSG("-- DT CLIENT INITIALIZED \n");

  return;
}

void DTC_App_LinkReadyNotification(uint16_t ConnectionHandle)
{
  tBleStatus status;
  uint16_t enable_notification = 0x0001;

  DataTransferClientContext.connHandle = ConnectionHandle;

#if(CFG_BLE_CENTRAL != 0)
  GattProcReq(GATT_PROC_MTU_UPDATE);
#endif
  GattProcReq(GATT_PROC_DISC_ALL_PRIMARY_SERVICES);
  GattProcReq(GATT_PROC_DISC_ALL_CHAR_OF_DT_SERVICE);
  GattProcReq(GATT_PROC_DISC_TX_CHAR_DESC);

  /**
   * Enable notification on the remote server
   */
  APP_DBG_MSG("Enable notification on the remote server\n");

  status = aci_gatt_write_char_desc(
      DataTransferClientContext.connHandle,
      DataTransferClientContext.DataTransferTxDescHandle,
      2,
      (uint8_t *) &enable_notification);
  if (status != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("Enable notification cmd failure: 0x%x\n", status);
  }
#if(CFG_BLE_CENTRAL != 0)
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
  evt_blue_aci * blue_evt;
  aci_att_exchange_mtu_resp_event_rp0 * exchange_mtu_resp;
  uint8_t CRC_Result;
  uint8_t CRC_Received;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *) (((hci_uart_pckt*) Event)->data);

  switch (event_pckt->evt)
  {
    case EVT_VENDOR:
    {
      blue_evt = (evt_blue_aci*) event_pckt->data;
      switch (blue_evt->ecode)
      {
        case EVT_BLUE_ATT_EXCHANGE_MTU_RESP:
          APP_DBG_MSG("EVT_BLUE_ATT_EXCHANGE_MTU_RESP \n");
          exchange_mtu_resp = (aci_att_exchange_mtu_resp_event_rp0 *)blue_evt->data;
          APP_DBG_MSG("MTU_size = %d \n",exchange_mtu_resp->Server_RX_MTU );
          APP_DBG_MSG("MTU_handle = 0x%x \n",exchange_mtu_resp->Connection_Handle );
          break;

        case EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP:
        {
          aci_att_read_by_group_type_resp_event_rp0 *pr = (void*) blue_evt->data;
          uint8_t numServ, i, idx;
          uint16_t uuid, handle;

          handle = pr->Connection_Handle;

          APP_DBG_MSG("DTC_Event_Handler: EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP\n");

          DataTransferClientContext.connHandle = handle;

          numServ = (pr->Data_Length) / pr->Attribute_Data_Length;

          /* the event data will be
           * 2bytes start handle
           * 2bytes end handle
           * 2 or 16 bytes data
           * we are interested only if the UUID is 16 bit.
           * So check if the data length is 6
           */
#if (UUID_128BIT_FORMAT==1)
          if (pr->Attribute_Data_Length == 20)
          {
            idx = 16;
#else
            if (pr->Attribute_Data_Length == 6)
            {
              idx = 4;
#endif
              for (i = 0; i < numServ; i++)
              {
                uuid = UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx]);
                if (uuid == DATA_TRANSFER_SERVICE_UUID)
                {
#if (UUID_128BIT_FORMAT==1)
                  DataTransferClientContext.DataTransferServiceHandle = UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx-16]);
                  DataTransferClientContext.DataTransferServiceEndHandle = UNPACK_2_BYTE_PARAMETER (&pr->Attribute_Data_List[idx-14]);
#else
                  DataTransferClientContext.DataTransferServiceHandle =
                      UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx - 4]);
                  DataTransferClientContext.DataTransferServiceEndHandle =
                      UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx - 2]);
#endif
                  APP_DBG_MSG("DTC_Event_Handler: DATA_TRANSFER_SERVICE_UUID found !\n");
                }
                idx += 6;
              }
            }
          }
          break;

        case EVT_BLUE_ATT_READ_BY_TYPE_RESP:
        {
          aci_att_read_by_type_resp_event_rp0 *pr = (void*) blue_evt->data;
          uint8_t idx;
          uint16_t uuid, handle;

          APP_DBG_MSG("DTC_Event_Handler: EVT_BLUE_ATT_READ_BY_TYPE_RESP\n");

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
            pr->Data_Length -= 1;
            while(pr->Data_Length > 0)
            {
              uuid = UNPACK_2_BYTE_PARAMETER(&pr->Handle_Value_Pair_Data[idx]);
              /* store the characteristic handle not the attribute handle */
#if (UUID_128BIT_FORMAT==1)
              handle = UNPACK_2_BYTE_PARAMETER(&pr->Handle_Value_Pair_Data[idx-14]);
#else
              handle = UNPACK_2_BYTE_PARAMETER(&pr->Handle_Value_Pair_Data[idx - 2]);
#endif
              if (uuid == DATA_TRANSFER_TX_CHAR_UUID)
              {
                DataTransferClientContext.DataTransferTxCharHdle = handle;
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
        case EVT_BLUE_ATT_FIND_INFORMATION_RESP:
        {
          aci_att_find_info_resp_event_rp0 *pr = (void*) blue_evt->data;
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
                APP_DBG_MSG("DTC_Event_Handler: DTC_DISCOVER_TX_CHAR_DESC ready to enable notification\n");
                DataTransferClientContext.DataTransferTxDescHandle = handle;
              }
              idx += 4;
            }
          }
        }
        break; /*EVT_BLUE_ATT_FIND_INFORMATION_RESP*/

        case EVT_BLUE_GATT_NOTIFICATION:
        {
          aci_gatt_notification_event_rp0 *pr = (void*) blue_evt->data;

          if ((pr->Attribute_Handle == DataTransferClientContext.DataTransferTxCharHdle)
              && (pr->Attribute_Value_Length > (2)))
          {
            NotificationData.DataTransfered.Length = pr->Attribute_Value_Length;
            NotificationData.DataTransfered.pPayload =  pr->Attribute_Value;
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
            CRC_Received = (uint8_t) (pr->Attribute_Value[DATA_NOTIFICATION_MAX_PACKET_SIZE-1]);

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
        break;/* end EVT_BLUE_GATT_NOTIFICATION */

        case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
          UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);
          break; /*EVT_BLUE_GATT_PROCEDURE_COMPLETE*/

        default:
          break;
        }
        default:
          break;
      }
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

    APP_DBG_MSG("DT = %ld bytes/s lost = %ld \n",DataThroughputValue, packet_lost_local);
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

  static void GattProcReq(GattProcId_t GattProcId)
  {
    tBleStatus status;

    switch(GattProcId)
    {
      case GATT_PROC_MTU_UPDATE:
        APP_DBG_MSG("change ATT MTU size \n");

        status = aci_gatt_exchange_config(DataTransferClientContext.connHandle);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("change MTU cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("GATT_PROC_MTU_UPDATE complete event received \n");
        break;

      case GATT_PROC_DISC_ALL_PRIMARY_SERVICES:
        APP_DBG_MSG("Discover all primary services \n");

        status = aci_gatt_disc_all_primary_services(DataTransferClientContext.connHandle);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("Discover all primary services cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("GATT_PROC_DISC_ALL_PRIMARY_SERVICES complete event received \n");
        break;

      case GATT_PROC_DISC_ALL_CHAR_OF_DT_SERVICE:
        APP_DBG_MSG("Discover all char of service \n");

        status = aci_gatt_disc_all_char_of_service(
            DataTransferClientContext.connHandle,
            DataTransferClientContext.DataTransferServiceHandle,
            DataTransferClientContext.DataTransferServiceEndHandle);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("Discover all char of service cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("GATT_PROC_DISC_ALL_CHAR_OF_DT_SERVICE complete event received \n");
        break;

      case GATT_PROC_DISC_TX_CHAR_DESC:
        APP_DBG_MSG("Discover Tx char descriptors \n");

        status = aci_gatt_disc_all_char_desc(
            DataTransferClientContext.connHandle,
            DataTransferClientContext.DataTransferTxCharHdle,
            DataTransferClientContext.DataTransferTxCharHdle + 2);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("Discover Tx char descriptors cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("GATT_PROC_DISC_TX_CHAR_DESC complete event received \n");
        break;

      default:
        break;
    }
    return;
  }


  /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
