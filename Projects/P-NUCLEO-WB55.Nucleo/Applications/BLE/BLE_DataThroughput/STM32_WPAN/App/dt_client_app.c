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
extern DT_ClientContext_t aDTClientContext[BLE_CFG_CLT_MAX_NBR_CB];
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DataTransferContext_t aDataTransferClientContext[MAX_CONNECTION];
static uint16_t packet_lost;
static uint32_t DataThroughputValue;
static uint32_t TotalDataReceived = 0;
static uint32_t Nbr_packet = 0;
static DTS_STM_App_Notification_evt_t NotificationData;
static uint8_t TimerDataThroughput_Id;
uint32_t N=0;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void DTC_App_Update_Service( void );
static void DTC_App_Notification( DTS_STM_App_Notification_evt_t *pNotification );
static SVCCTL_EvtAckStatus_t DataTransfer_Client_Event_Handler( void *Event );
static void DataThroughputTimer( void );
static void DataThroughput_Calculation(void);

/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
void BLE_APP_Disconnection( uint16_t conn_handle , uint8_t reason );
void disable_notification(void);

/* Public functions ----------------------------------------------------------*/

/****************************************************************/
/*                                                              */
/*                      GATT CLIENT PART                           */
/****************************************************************/
/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
void DT_Client_App_Init( void )
{
  if (BUTTON_SERVER == 0)
  {
    uint8_t index = 0;
    for (index = 0; index < BLE_CFG_CLT_MAX_NBR_CB; index++)
      aDTClientContext[index].state = DT_IDLE;
  }
  
  SVCCTL_RegisterCltHandler(DataTransfer_Client_Event_Handler);

  UTIL_SEQ_RegTask( 1<<CFG_IdleTask_DataTransfer_ClientDiscovery, UTIL_SEQ_RFU, DTC_App_Update_Service);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_APP_DATA_THROUGHPUT_ID, UTIL_SEQ_RFU, DataThroughput_Calculation);

  aDataTransferClientContext[0].state = DTC_IDLE;
  aDataTransferClientContext[0].connHandle = 0xFFFF;
  /* Create timer for Data Throughput */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(TimerDataThroughput_Id), hw_ts_SingleShot, DataThroughputTimer);

  APP_DBG_MSG("-- DT CLIENT INITIALIZED \n");

  return;
}

static SVCCTL_EvtAckStatus_t DataTransfer_Client_Event_Handler( void *Event )
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt * event_pckt;
  evt_blue_aci * blue_evt;
#if ( CFG_DEBUG_APP_TRACE != 0)			
  aci_att_exchange_mtu_resp_event_rp0 * exchange_mtu_resp;
#endif	
  Connection_Context_t Notification;
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
#if ( CFG_DEBUG_APP_TRACE != 0)						
          exchange_mtu_resp = (aci_att_exchange_mtu_resp_event_rp0 *)blue_evt->data;
#endif
          APP_DBG_MSG("MTU_size = %d \n",exchange_mtu_resp->Server_RX_MTU );
          Notification.Evt_Opcode = DT_MTU_EXCHANGED;
          DT_App_Notification(&Notification);
          break;
          
        case EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP:
        {
          aci_att_read_by_group_type_resp_event_rp0 *pr = (void*) blue_evt->data;
          uint8_t numServ, i, idx;
          uint16_t uuid, handle;
          uint8_t index;

          handle = pr->Connection_Handle;
          index = 0;
          while((index < MAX_CONNECTION) && (aDataTransferClientContext[index].state != DTC_IDLE))
          {
            index++;
          }

          if (index < MAX_CONNECTION)
          {
            APP_DBG_MSG("DataTransfer_Client_Event_Handler: EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP\n");

            aDataTransferClientContext[index].connHandle = handle;

            numServ = (pr->Data_Length) / pr->Attribute_Data_Length;

            /* the event data will be
             * 2bytes start handle
             * 2bytes end handle
             * 2 or 16 bytes data
             * we are intersted only if the UUID is 16 bit.
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
                  aDataTransferClientContext[index].DataTransferServiceHandle = UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx-16]);
                  aDataTransferClientContext[index].DataTransferServiceEndHandle = UNPACK_2_BYTE_PARAMETER (&pr->Attribute_Data_List[idx-14]);
#else
                  aDataTransferClientContext[index].DataTransferServiceHandle =
                      UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx - 4]);
                  aDataTransferClientContext[index].DataTransferServiceEndHandle =
                      UNPACK_2_BYTE_PARAMETER(&pr->Attribute_Data_List[idx - 2]);
#endif
                  aDataTransferClientContext[index].state = DTC_DISCOVER_CHARACS;
                  APP_DBG_MSG("DataTransfer_Client_Event_Handler: DATA_TRANSFER_SERVICE_UUID found !\n");
                }
                idx += 6;
              }
            }
          }
          else
          {
            APP_DBG_MSG("DataTransfer_Client_Event_Handler: EVT_BLUE_ATT_READ_BY_GROUP_TYPE_RESP, failed no free index in connection table !\n");
          }
        }
          break;
        case EVT_BLUE_ATT_READ_BY_TYPE_RESP:
        {
          aci_att_read_by_type_resp_event_rp0 *pr = (void*) blue_evt->data;
          uint8_t idx;
          uint16_t uuid, handle;
          uint8_t index;

          index = 0;
          while((index < MAX_CONNECTION) && (aDataTransferClientContext[index].connHandle != pr->Connection_Handle))
            index++;

          if (index < MAX_CONNECTION)
          {
            APP_DBG_MSG("DataTransfer_Client_Event_Handler: EVT_BLUE_ATT_READ_BY_TYPE_RESP\n");

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
                  aDataTransferClientContext[index].state = DTC_DISCOVER_TX_CHAR_DESC;
                  aDataTransferClientContext[index].DataTransferTxCharHdle = handle;
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
          else
            APP_DBG_MSG("DataTransfer_Client_Event_Handler: EVT_BLUE_ATT_READ_BY_TYPE_RESP, failed handle not found in connection table !\n");
        }
          break;
        case EVT_BLUE_ATT_FIND_INFORMATION_RESP:
        {
          aci_att_find_info_resp_event_rp0 *pr = (void*) blue_evt->data;
          uint8_t numDesc, idx, i;
          uint16_t uuid, handle;
          uint8_t index;

          index = 0;
          while((index < MAX_CONNECTION) && (aDataTransferClientContext[index].connHandle != pr->Connection_Handle))
            index++;

          if (index < MAX_CONNECTION)
          {
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
                  if (aDataTransferClientContext[index].state == DTC_DISCOVER_TX_CHAR_DESC)
                  {
                    APP_DBG_MSG("DataTransfer_Client_Event_Handler: DTC_DISCOVER_TX_CHAR_DESC ready to enable notification\n");
                    aDataTransferClientContext[index].DataTransferTxDescHandle = handle;
                    if (BUTTON_SERVER)
                    {
                    aDataTransferClientContext[index].state = DTC_ENABLE_NOTIFICATION_TX_DESC;
                    }
                    else
                    {
                      aDataTransferClientContext[index].state = DTC_CONNECTED;
                    }
                  }
                }
                idx += 4;
              }
            }
          }
          else
            APP_DBG_MSG("DataTransfer_Client_Event_Handler: EVT_BLUE_ATT_FIND_INFORMATION_RESP, failed handle not found in connection table !\n");
        }
          break; /*EVT_BLUE_ATT_FIND_INFORMATION_RESP*/

        case EVT_BLUE_GATT_NOTIFICATION:
        {
          aci_gatt_notification_event_rp0 *pr = (void*) blue_evt->data;

          uint8_t index;

          index = 0;
          while((index < MAX_CONNECTION) && (aDataTransferClientContext[index].connHandle != pr->Connection_Handle))
            index++;

          if (index < MAX_CONNECTION)
          {
            if ((pr->Attribute_Handle == aDataTransferClientContext[index].DataTransferTxCharHdle)
                && (pr->Attribute_Value_Length > (2)))
            {
              NotificationData.Evt_Opcode = DTS_STM_DATA_RECEIVED;
              NotificationData.DataTransfered.Length = pr->Attribute_Value_Length;
              NotificationData.DataTransfered.pPayload = (uint32_t*) (&(pr->Attribute_Value[0]));
              NotificationData.DataTransfered.pPayload_n = *((uint32_t*) &(pr->Attribute_Value[0]));
              __disable_irq();
              if (NotificationData.DataTransfered.pPayload_n >= (NotificationData.DataTransfered.pPayload_n_1 + 2))
              {
              packet_lost +=
                  ((NotificationData.DataTransfered.pPayload_n - NotificationData.DataTransfered.pPayload_n_1) - 1);
              }
              __enable_irq();
              /* CRC computation */
              CRC_Result = APP_BLE_ComputeCRC8((uint8_t*) (pr->Attribute_Value), (pr->Attribute_Value_Length) - 4);
              /* get low weight byte */
              CRC_Received = (uint8_t) (pr->Attribute_Value[236]);

              if (CRC_Received != CRC_Result)
              {
                N+=1;
                APP_DBG_MSG("** data error **  N= %d \r\n",(int)N);
              }
              NotificationData.DataTransfered.pPayload_n_1 = NotificationData.DataTransfered.pPayload_n;
              DTC_App_Notification(&NotificationData);
            }
          }
          else
            APP_DBG_MSG("DataTransfer_Client_Event_Handler: EVT_BLUE_GATT_NOTIFICATION, failed handle not found in connection table !\n");
        }
          break;/* end EVT_BLUE_GATT_NOTIFICATION */
        case EVT_BLUE_GATT_PROCEDURE_COMPLETE:
        {
          uint8_t index;
          aci_att_read_by_group_type_resp_event_rp0 *pr = (void*) blue_evt->data;

          index = 0;
          while((index < MAX_CONNECTION) && (aDataTransferClientContext[index].connHandle != pr->Connection_Handle))
            index++;

          if (index < MAX_CONNECTION)
          {
            switch (aDataTransferClientContext[index].state)
            {
              case DTC_DISCOVER_CHARACS:
              {

                /* we have found the Data Transfer service and now
                 * we need to find the characteristics of Data Transfer
                 * service
                 */
                UTIL_SEQ_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0);
              }
                break;
              case DTC_DISCOVER_NOTIFICATION_CHAR_DESC:
              {
                UTIL_SEQ_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0);
              }
                break;
              case DTC_DISCOVER_TX_CHAR_DESC:
              {
                UTIL_SEQ_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0);
              }
                break;
              case DTC_DISCOVER_RX_CHAR_DESC:
              {
                UTIL_SEQ_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0);
              }
                break;
              case DTC_ENABLE_NOTIFICATION_TX_DESC:
              {
                if (BUTTON_SERVER == 0)
                {
                  APP_DBG_MSG("enable notification \n"); 
                }
                else
                {
                  APP_DBG_MSG("enable notification \n");
                  UTIL_SEQ_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0);
                }
              }
                break;
              case DTC_DISABLE_NOTIFICATION_TX_DESC:
              {
                if (BUTTON_SERVER == 0)
                {
                  APP_DBG_MSG("disable notification \n");
                }
                else
                {
                  APP_DBG_MSG("disable notification \n");
                  UTIL_SEQ_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0);
                }
              }
                break;
              default:
            	break;
            }
          }
          else
            APP_DBG_MSG("DataTransfer_Client_Event_Handler: EVT_BLUE_GATT_PROCEDURE_COMPLETE, failed handle not found in connection table !\n");
        }
          break; /*EVT_BLUE_GATT_PROCEDURE_COMPLETE*/
        case EVT_BLUE_GATT_TX_POOL_AVAILABLE:
          Resume_Notification();
          break;
      }
      default:
      break;
    }
  }

  return (return_value);
}/* end BLE_CTRL_Event_Acknowledged_Status_t */

uint32_t DataTransfered = 0;
uint8_t scenario_name;
uint8_t Result;

void BLE_APP_Disconnection( uint16_t conn_handle , uint8_t reason )
{
  aci_gap_terminate(conn_handle, reason);
}

void DataThroughput_Calculation( void )
{
#if ( CFG_DEBUG_APP_TRACE != 0)			
  uint32_t packet_lost_local;
  uint32_t MoyData;
#endif	

  DataThroughputValue = (uint32_t)(DataTransfered / TIMEUNIT);
#if ( CFG_DEBUG_APP_TRACE != 0)			
  packet_lost_local = (uint32_t)(packet_lost / TIMEUNIT);
#endif	
  DataTransfered = 0;
  
  if (Nbr_packet < 600)
  {
    Nbr_packet++;
    TotalDataReceived += DataThroughputValue;
  }
  if (Nbr_packet == 600)
  {
#if ( CFG_DEBUG_APP_TRACE != 0)		
    MoyData = TotalDataReceived / 600;
#endif		
    APP_DBG_MSG("Moy = %ld bytes/s \n",MoyData);
    TotalDataReceived = 0;
    Nbr_packet = 0;
  }


  APP_DBG_MSG("DT = %ld bytes/s lost = %ld \n",DataThroughputValue, packet_lost_local);
#if ( CFG_DEBUG_APP_TRACE != 0)			
  __disable_irq();
  packet_lost = 0;
  __enable_irq();
#endif	
}

void DTC_App_Notification( DTS_STM_App_Notification_evt_t *pNotification )
{
  switch (pNotification->Evt_Opcode)
  {
    case DTS_STM_DATA_RECEIVED:
      if (DataTransfered == 0)
      {
        HW_TS_Start(TimerDataThroughput_Id, DATA_THROUGHPUT_MEASUREMENT);
      }
      DataTransfered += pNotification->DataTransfered.Length;
      break;

    default:
      break;
  }
  return;
}

void DTC_App_Update_Service( )
{
  uint16_t enable = 0x0001;
  uint16_t disable = 0x0000;
  tBleStatus result;
  uint8_t index;

  index = 0;
  while((index < MAX_CONNECTION) && (aDataTransferClientContext[index].state != DTC_IDLE))
  {
    switch (aDataTransferClientContext[index].state)
    {
      case DTC_CONNECTED:
        APP_DBG_MSG("DTC_App_Update_Service: DTC_CONNECTED\n");
        aDataTransferClientContext[index].state = DTC_CONNECTED_ADVERTISING;
        break;
      case DTC_DISCOVER_SERVICES:
        break;
      case DTC_DISCOVER_CHARACS:
        APP_DBG_MSG("DTC_App_Update_Service: DTC_DISCOVER_CHARACS\n");

        result = aci_gatt_disc_all_char_of_service(
                                                   aDataTransferClientContext[index].connHandle,
                                                   aDataTransferClientContext[index].DataTransferServiceHandle,
                                                   aDataTransferClientContext[index].DataTransferServiceEndHandle);
        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All characteristics discovered Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All characteristics discovery Failed \n");
        }
        break;
      case DTC_DISCOVER_TX_CHAR_DESC:
        APP_DBG_MSG("DTC_App_Update_Service: DTC_DISCOVER_TX_CHAR_DESC\n");

        result = aci_gatt_disc_all_char_desc(
                                             aDataTransferClientContext[index].connHandle,
                                             aDataTransferClientContext[index].DataTransferTxCharHdle,
                                             aDataTransferClientContext[index].DataTransferTxCharHdle + 2);
        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All TX characteristics discovered Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All TX characteristics discovery Failed \n");
        }
        break;
      case DTC_DISCOVER_NOTIFICATION_CHAR_DESC:
        APP_DBG_MSG("DTC_App_Update_Service: DTC_DISCOVER_NOTIFICATION_CHAR_DESC\n");
        result = aci_gatt_disc_all_char_desc(
                                             aDataTransferClientContext[index].connHandle,
                                             aDataTransferClientContext[index].DTNotificationCharHdle,
                                             aDataTransferClientContext[index].DTNotificationCharHdle + 2);
        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All P2P characteristics discovered Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All P2P characteristics discovery Failed \n");
        }
        break;
      case DTC_DISCOVER_RX_CHAR_DESC:
        APP_DBG_MSG("DTC_App_Update_Service: DTC_DISCOVER_RX_CHAR_DESC\n");

        result = aci_gatt_disc_all_char_desc(
                                             aDataTransferClientContext[index].connHandle,
                                             aDataTransferClientContext[index].DataTransferRxCharHdle,
                                             aDataTransferClientContext[index].DataTransferRxCharHdle + 2);
        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All RX characteristics discovered Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), All RX characteristics discovery Failed \n");
        }
        break;
      case DTC_ENABLE_NOTIFICATION_TX_DESC:
        APP_DBG_MSG("DTC_App_Update_Service: DTC_ENABLE_NOTIFICATION_TX_DESC\n");

        result = aci_gatt_write_char_desc(
                                          aDataTransferClientContext[index].connHandle,
                                          aDataTransferClientContext[index].DataTransferTxDescHandle,
                                          2,
                                          (uint8_t *) &enable);
        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notification enabled Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notification enabled Failed \n");
        }
        if (BUTTON_SERVER)
        {
          aDataTransferClientContext[index].state = DTC_CONNECTED;
        }
        break;
      case DTC_DISABLE_NOTIFICATION_TX_DESC:
        APP_DBG_MSG("DTC_App_Update_Service: DTC_DISABLE_NOTIFICATION_TX_DESC\n");

        result = aci_gatt_write_char_desc(
                                          aDataTransferClientContext[index].connHandle,
                                          aDataTransferClientContext[index].DataTransferTxDescHandle,
                                          2,
                                          (uint8_t *) &disable);

        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notfication disabled Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notfication disabled Failed \n");
        }
        if (BUTTON_SERVER)
        {
          aDataTransferClientContext[index].state = DTC_CONNECTED;
        }
        break;

      default:
        break;
    }
    index++;
  }
  return;
}

static void DataThroughputTimer( void )
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_APP_DATA_THROUGHPUT_ID, CFG_SCH_PRIO_0);
  return;
}

void enable_notification(void)
{
  uint16_t enable = 0x0001;
  tBleStatus result;
      APP_DBG_MSG("DTC_App_Update_Service: DTC_ENABLE_NOTIFICATION_TX_DESC\n");

        result = aci_gatt_write_char_desc(
                                          aDataTransferClientContext[0].connHandle,
                                          aDataTransferClientContext[0].DataTransferTxDescHandle,
                                          2,
                                          (uint8_t *) &enable);
        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notification enabled Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notification enabled Failed \n");
        }
        aDataTransferClientContext[0].state = DTC_CONNECTED;
        return;
}

void disable_notification(void)
{
    uint16_t disable = 0x0000;
  tBleStatus result;;
        APP_DBG_MSG("DTC_App_Update_Service: DTC_DISABLE_NOTIFICATION_TX_DESC\n");

        result = aci_gatt_write_char_desc(
                                          aDataTransferClientContext[0].connHandle,
                                          aDataTransferClientContext[0].DataTransferTxDescHandle,
                                          2,
                                          (uint8_t *) &disable);

        if (result == BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notfication disabled Successfully \n");
        }
        else
        {
          APP_DBG_MSG("DTC_App_Update_Service(), TX notfication disabled Failed \n");
        }
        aDataTransferClientContext[0].state = DTC_CONNECTED;
        return;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
