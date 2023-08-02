/**
  ******************************************************************************
 * @file    dts.c
 * @author  MCD Application Team
 * @brief   Data Transfer Service (Custom)
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


/* Includes ------------------------------------------------------------------*/
#include "ble_common.h"
#include "ble.h"
#include "dbg_trace.h"
#include "app_ble.h" 
#include "dts.h"   

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  DTS_STM_NOTIFICATION_MASK = (1 << 0),
  DTS_STM_INDICATION_MASK = (1 << 1),
} ClientCharConfMask_t;

typedef struct
{
uint16_t DataTransferSvcHdle;            /**< Service handle */
uint16_t DataTransferTxCharHdle;         /**< Characteristic handle */
uint16_t DataTransferRxCharHdle;         /**< Characteristic handle */
uint16_t DataTransferThroughputCharHdle; /**< Characteristic handle */
} DataTransferSvcContext_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#if (UUID_128BIT_FORMAT == 1)
#define DT_UUID_LENGTH  UUID_TYPE_128
#else
#define DT_UUID_LENGTH  UUID_TYPE_16
#endif
#define DATA_TRANSFER_NOTIFICATION_LEN_MAX                                 (255)

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
#define COPY_DT_SERVICE_UUID(uuid_struct)              COPY_UUID_128(uuid_struct,0x00,0x00,0xFE,0x80,0x8E,0x22,0x45,0x41,0x9D,0x4C,0x21,0xED,0xAE,0x82,0xED,0x19)
#define COPY_DT_TX_CHAR_UUID(uuid_struct)              COPY_UUID_128(uuid_struct,0x00,0x00,0xFE,0x81,0x8E,0x22,0x45,0x41,0x9D,0x4C,0x21,0xED,0xAE,0x82,0xED,0x19)
#define COPY_DT_RX_CHAR_UUID(uuid_struct)              COPY_UUID_128(uuid_struct,0x00,0x00,0xFE,0x82,0x8E,0x22,0x45,0x41,0x9D,0x4C,0x21,0xED,0xAE,0x82,0xED,0x19)
#define COPY_DT_THOUGHPUT_CHAR_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x00,0x00,0xFE,0x83,0x8E,0x22,0x45,0x41,0x9D,0x4C,0x21,0xED,0xAE,0x82,0xED,0x19)

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static DataTransferSvcContext_t aDataTransferContext;

/**
 * END of Section BLE_APP_CONTEXT
 */

extern uint16_t Att_Mtu_Exchanged;

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t DTS_Event_Handler( void *pckt );
static DTS_STM_App_Notification_evt_t Notification;
extern void BLE_SVC_L2CAP_Conn_Update_7_5(void);
uint32_t DTS_packet_lost = 0;
uint32_t DTS_N = 0;

/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t DTS_Event_Handler( void *Event )
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt * event_pckt;
  evt_blecore_aci * blecore_evt;
  aci_gatt_attribute_modified_event_rp0 * attribute_modified;
  aci_att_exchange_mtu_resp_event_rp0 * exchange_mtu_resp;
  aci_gatt_write_permit_req_event_rp0 * write_permit_req ;
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
        case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
        {
          exchange_mtu_resp = (aci_att_exchange_mtu_resp_event_rp0 *)blecore_evt->data;
          APP_DBG_MSG("**MTU_size = %d \n",exchange_mtu_resp->Server_RX_MTU );
          APP_DBG_MSG("\r\n\r");
          if (exchange_mtu_resp->Server_RX_MTU < DATA_NOTIFICATION_MAX_PACKET_SIZE)
          {
            Att_Mtu_Exchanged = exchange_mtu_resp->Server_RX_MTU - 3;
          }
          else
          {
            Att_Mtu_Exchanged = DATA_NOTIFICATION_MAX_PACKET_SIZE;
          }
        }
        break;
        /* server */
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
        {
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*) blecore_evt->data;
          if (attribute_modified->Attr_Handle == (aDataTransferContext.DataTransferTxCharHdle + 2))
          {
            /**
             * Notify to application to start measurement
             */
            if (attribute_modified->Attr_Data[0] & DTS_STM_NOTIFICATION_MASK)
            {
              APP_DBG_MSG("**NOTIFICATION ENABLED \n");
              APP_DBG_MSG("\r\n\r");
              Notification.Evt_Opcode = DTS_TX_NOTIFICATION_ENABLED;
              DTS_Notification(&Notification);
            }
            else
            {
              APP_DBG_MSG("**NOTIFICATION DISABLED \n");
              APP_DBG_MSG("\r\n\r");
              Notification.Evt_Opcode = DTS_TX_NOTIFICATION_DISABLED;
              DTS_Notification(&Notification);
            }
          }
          if (attribute_modified->Attr_Handle == (aDataTransferContext.DataTransferThroughputCharHdle + 2))
          {
            /**
            * Notify to application to start measurement
            */
            if (attribute_modified->Attr_Data[0] & DTS_STM_NOTIFICATION_MASK)
            {
              APP_DBG_MSG("**WRITE RESULT NOTIFICATION ENABLED \n");
              APP_DBG_MSG("\r\n\r");
              //BLE_SVC_L2CAP_Conn_Update_7_5();
              Notification.Evt_Opcode = DTS_THROUGHPUT_NOTIFICATION_ENABLED;
              DTS_Notification(&Notification);
            }
            else
            {
              APP_DBG_MSG("**WRITE RESULT NOTIFICATION DISABLED \n");
              APP_DBG_MSG("\r\n\r");
              Notification.Evt_Opcode = DTS_THROUGHPUT_NOTIFICATION_DISABLED;
              DTS_Notification(&Notification);
            }
          }
          if(attribute_modified->Attr_Handle == (aDataTransferContext.DataTransferRxCharHdle + 1))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            
            
            Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
            

            Notification.DataTransfered.pPayload = (attribute_modified->Attr_Data);
            Notification.DataTransfered.pPayload_n = *((uint32_t*) &(attribute_modified->Attr_Data[0]));

            if ((Notification.DataTransfered.pPayload_n - Notification.DataTransfered.pPayload_n_1) != 0)
            {
              __disable_irq();
              if (Notification.DataTransfered.pPayload_n >= (Notification.DataTransfered.pPayload_n_1 + 2))
              {
                DTS_packet_lost +=
                  ((Notification.DataTransfered.pPayload_n - Notification.DataTransfered.pPayload_n_1) - 1);
              }
              __enable_irq();
              /* CRC computation */
              CRC_Result = APP_BLE_ComputeCRC8((uint8_t*) (attribute_modified->Attr_Data), (attribute_modified->Attr_Data_Length) - 1);
              /* get low weight byte */
              CRC_Received = (uint8_t) (attribute_modified->Attr_Data[Notification.DataTransfered.Length - 1]);
              
              if (CRC_Received != CRC_Result)
              {
                DTS_N+=1;
                APP_DBG_MSG("** data error **  N= %d \r\n",(int)DTS_N);
              }
            }

            Notification.DataTransfered.pPayload_n_1 = Notification.DataTransfered.pPayload_n;
            
            
            Notification.Evt_Opcode = DTS_DATA_RECEIVED;
            DTS_Notification(&Notification); 
          }
        }
        break;
        
        case ACI_GATT_TX_POOL_AVAILABLE_VSEVT_CODE:
        {
          Resume_Notification();
        }
        break; 
          
        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
        {
          //APP_DBG_MSG("WRITE PERMIT RESP \r\n");
          write_permit_req = (aci_gatt_write_permit_req_event_rp0 *) blecore_evt->data;
          aci_gatt_write_resp( write_permit_req->Connection_Handle, 
                              write_permit_req->Attribute_Handle, 
                              0, 
                              0, 
                              write_permit_req->Data_Length, 
                              write_permit_req->Data); 
        }
        break;

        default:
          break;
      }
    }
    break; /* HCI_HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE_SPECIFIC */

    default:
      break;
  }

  return (return_value);
}/* end SVCCTL_EvtAckStatus_t */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void DTS_STM_Init( void )
{
  tBleStatus hciCmdResult = BLE_STATUS_FAILED;
  Char_UUID_t  uuid16;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(DTS_Event_Handler);

  /* DT service and characteristics */
  COPY_DT_SERVICE_UUID(uuid16.Char_UUID_128);
  hciCmdResult = aci_gatt_add_service(DT_UUID_LENGTH, (Service_UUID_t *) &uuid16,
                                      PRIMARY_SERVICE,
                                      10, 
                                      &(aDataTransferContext.DataTransferSvcHdle));
  if (hciCmdResult != 0)
  {
    APP_DBG_MSG("error add service 0x%x\n", hciCmdResult);
  }

  /**
   *  Add Data Transfer TX Characteristic
   */
  COPY_DT_TX_CHAR_UUID(uuid16.Char_UUID_128);
  hciCmdResult = aci_gatt_add_char(aDataTransferContext.DataTransferSvcHdle,
                                   DT_UUID_LENGTH,
                                   (Char_UUID_t *) &uuid16,
                                   DATA_TRANSFER_NOTIFICATION_LEN_MAX,
                                   CHAR_PROP_NOTIFY,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   1, /* isVariable */
                                   &(aDataTransferContext.DataTransferTxCharHdle));
  if (hciCmdResult != 0)
  {
    APP_DBG_MSG("error add char Tx 0x%x\n", hciCmdResult);
  }

  /**
   *  Add Data Transfer RX Characteristic
   */
  COPY_DT_RX_CHAR_UUID(uuid16.Char_UUID_128);
  hciCmdResult = aci_gatt_add_char(aDataTransferContext.DataTransferSvcHdle,
                                   DT_UUID_LENGTH,
                                   (Char_UUID_t *) &uuid16,
                                   255, /* DATA_TRANSFER_NOTIFICATION_LEN_MAX, */
                                   CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   1, /* isVariable */
                                   &(aDataTransferContext.DataTransferRxCharHdle));
  if (hciCmdResult != 0)
  {
    APP_DBG_MSG("error add char Tx\n");
  }
  
  /**
   *  Add Data Transfer THROUGHPUT Characteristic
   */
  COPY_DT_THOUGHPUT_CHAR_UUID(uuid16.Char_UUID_128);
  hciCmdResult = aci_gatt_add_char(aDataTransferContext.DataTransferSvcHdle,
                                   DT_UUID_LENGTH,
                                   (Char_UUID_t *) &uuid16,
                                   255, /* DATA_TRANSFER_NOTIFICATION_LEN_MAX, */
                                   CHAR_PROP_NOTIFY,
                                   ATTR_PERMISSION_NONE,
                                   GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   1, /* isVariable */
                                   &(aDataTransferContext.DataTransferThroughputCharHdle));
  if (hciCmdResult != 0)
  {
    APP_DBG_MSG("error add char Tx\n");
  }
  
  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * 
 */
tBleStatus DTS_STM_UpdateChar(uint16_t UUID, uint8_t *pPayload)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  DTS_STM_Payload_t *pDataValue = (DTS_STM_Payload_t*) pPayload;
  
  switch (UUID)
  {
    case DT_TX_CHAR_UUID:
    {
      /**
       *  Notification Data Transfer Packet
       */
      result = aci_gatt_update_char_value(aDataTransferContext.DataTransferSvcHdle,
                                          aDataTransferContext.DataTransferTxCharHdle,
                                          0, /* charValOffset */
                                          pDataValue->Length, /* charValueLen */
                                          (uint8_t *) pDataValue->pPayload);
    }
    break;

    case DT_THROUGHPUT_CHAR_UUID:
    {
      /**
       *  Notification Throughput Packet
       */
      result = aci_gatt_update_char_value(aDataTransferContext.DataTransferSvcHdle,
                                          aDataTransferContext.DataTransferThroughputCharHdle,
                                          0, /* charValOffset */
                                          pDataValue->Length, /* charValueLen */
                                          (uint8_t *) pDataValue->pPayload);
    }
    break;

    default:
      break;
  }
  return result;
}/* end DTS_STM_UpdateChar() */

#if 0
tBleStatus DTS_STM_UpdateCharThroughput(DTS_STM_Payload_t *pDataValue )
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  /**
   *  Notification Data Transfer Packet
   */
  result = aci_gatt_update_char_value(
                                   aDataTransferContext.DataTransferSvcHdle,
                                   aDataTransferContext.DataTransferTxChar3Hdle,
                                   0, /* charValOffset */
                                   pDataValue->Length, /* charValueLen */
                                   (uint8_t *) pDataValue->pPayload);
  return result;
}/* end DTS_STM_UpdateChar() */
#endif
