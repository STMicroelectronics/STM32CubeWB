/**
  ******************************************************************************
  * @file    crs_stm.c
  * @author  MCD Application Team
  * @brief   Cable Replacement Service (Custom STM)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint16_t	SvcHdle;				  /**< Service handle */
  uint16_t	CRSTXCharHdle;	                   /**< Characteristic handle */
  uint16_t	CRSRXCharHdle;                     /**< Characteristic handle */
}CRSContext_t;

/* Private defines -----------------------------------------------------------*/
#define BM_UUID_LENGTH  UUID_TYPE_128

#define BM_REQ_CHAR_SIZE    (3)


/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/**
 * Request Characteristic UUID
 * 0000fe11-8e22-4541-9d4c-21edae82ed19
 */
const uint8_t BM_REQ_CHAR_UUID[16] = {0x19, 0xed, 0x82, 0xae,
                                       0xed, 0x21, 0x4c, 0x9d,
                                       0x41, 0x45, 0x22, 0x8e,
                                       0x11, 0xFE, 0x00, 0x00};

/**
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static CRSContext_t CRSContext;
/**
 * END of Section BLE_DRIVER_CONTEXT
 */
/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t CRS_Event_Handler(void *Event);


/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 daf4562c-1dfc-11e7-93ae-92361f002671
 daf458b6-1dfc-11e7-93ae-92361f002671
 daf45d16-1dfc-11e7-93ae-92361f002671
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_CRS_UUID(uuid_struct, uuid) \
  COPY_UUID_128(uuid_struct, \
    uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7], \
    uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15])

#define CRS_MAX_RX_CHAR_LEN                                     CRS_MAX_DATA_LEN        /**< Maximum length of the RX Characteristic (in bytes). */
#define CRS_MAX_TX_CHAR_LEN                                     CRS_MAX_DATA_LEN        /**< Maximum length of the TX Characteristic (in bytes). */


/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t CRS_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0    * attribute_modified;
  CRS_STM_Notification_evt_t Notification;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      {
        blecore_evt = (evt_blecore_aci*)event_pckt->data;
        switch(blecore_evt->ecode)
        {
          case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
           {
              attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
              if(attribute_modified->Attr_Handle == (CRSContext.CRSRXCharHdle + 2))
              {
                /**
                 * Descriptor handle
                 */
                return_value = SVCCTL_EvtAckFlowEnable;
                /**
                 * Notify to application
                 */
                if(attribute_modified->Attr_Data[0] & COMSVC_Notification)
                {
                  Notification.CRS_Evt_Opcode = CRS_NOTIFY_ENABLED_EVT;
                  CRS_STM_Notification(&Notification);

                }
                else
                {
                  Notification.CRS_Evt_Opcode = CRS_NOTIFY_DISABLED_EVT;

                  CRS_STM_Notification(&Notification);

                }
              }
              
              else if(attribute_modified->Attr_Handle == (CRSContext.CRSTXCharHdle + 1))
              {
                /*value handle */
                BLE_DBG_CRS_STM_MSG("-- GATT : RX\n");
                Notification.CRS_Evt_Opcode = CRS_WRITE_EVT;
                Notification.DataTransfered.Length = attribute_modified->Attr_Data_Length;
                Notification.DataTransfered.pPayload = attribute_modified->Attr_Data;
                CRS_STM_Notification(&Notification);  
              }            
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

  return(return_value);
}/* end CRS_Event_Handler */


/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void CRS_STM_Init(void)
{
  Char_UUID_t  uuid;
  tBleStatus hciCmdResult = BLE_STATUS_SUCCESS;
  uint8_t service_uuid[] = { CRS_STM_UUID128 }; 
  uint8_t rx_uuid[]      = { CRS_STM_RX_UUID128 }; 
  uint8_t tx_uuid[]      = { CRS_STM_TX_UUID128 }; 

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(CRS_Event_Handler);
  
  /**
   *  Cable Replacement Service
   *
   * Max_Attribute_Records = 2*no_of_char + 1
   * service_max_attribute_record = 1 for Cable Replacement service +
   *                                2 for CRS TX characteristic +
   *                                2 for CRS RX characteristic +
   *                                1 for client char configuration descriptor
   *                                
   */
  COPY_CRS_UUID(uuid.Char_UUID_128, service_uuid);
  hciCmdResult = aci_gatt_add_service(
                                      UUID_TYPE_128,
                                      (Service_UUID_t *) &uuid,
                                      PRIMARY_SERVICE,
                                      6,
                                      &(CRSContext.SvcHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_CRS_STM_MSG("Cable Replacement (CR) Service is added Successfully 0x%02X\n", 
                 CRSContext.SvcHdle);
  }
  else 
  {
    BLE_DBG_CRS_STM_MSG ("FAILED to add Cable Replacement (CR) Service, Error: 0x%02X !!\n", 
                         hciCmdResult);
  }

  /**
   *  Add TX Characteristic
   */
  COPY_CRS_UUID(uuid.Char_UUID_128, tx_uuid);
  hciCmdResult = aci_gatt_add_char(
                                   CRSContext.SvcHdle,
                                   UUID_TYPE_128, 
                                   &uuid ,
                                   CRS_MAX_TX_CHAR_LEN,                                   
                                   CHAR_PROP_WRITE_WITHOUT_RESP|CHAR_PROP_READ,
                                   ATTR_PERMISSION_NONE,
                                   GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   1, /* isVariable */
                                   &(CRSContext.CRSTXCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_CRS_STM_MSG("TX Characteristic is added Successfully 0x%02X\n", 
                        CRSContext.CRSTXCharHdle);
  }
  else 
  {
    BLE_DBG_CRS_STM_MSG ("FAILED to add TX Characteristic, Error: 0x%02X !!\n", 
                         hciCmdResult);
  }

  /**
   *   Add RX Characteristic
   */
  COPY_CRS_UUID(uuid.Char_UUID_128, rx_uuid);
  hciCmdResult = aci_gatt_add_char(CRSContext.SvcHdle,
                                   UUID_TYPE_128,
                                   &uuid,
                                   CRS_MAX_RX_CHAR_LEN,
                                   CHAR_PROP_READ|CHAR_PROP_NOTIFY,
                                   ATTR_PERMISSION_NONE,
                                   GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                                   10, /* encryKeySize */
                                   1, /* isVariable: 1 */
                                   &(CRSContext.CRSRXCharHdle));    
  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_CRS_STM_MSG("RX Characteristic is added Successfully 0x%02X\n", 
                        CRSContext.CRSRXCharHdle);
  }
  else 
  {
    BLE_DBG_CRS_STM_MSG ("FAILED to add RX Characteristic, Error: 0x%02X !!\n", 
                 hciCmdResult);
  }

  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * 
 */
tBleStatus CRS_STM_Update_Char(uint16_t UUID, uint8_t *p_Payload) 
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  switch(UUID)
  {
    case CRS_RX_CHAR_UUID:
    {
      uint8_t size;
      
      size = 0;
      while(p_Payload[size] != '\0')
      {
        size++;
      }
      result = aci_gatt_update_char_value(CRSContext.SvcHdle,
                                          CRSContext.CRSRXCharHdle,
                                          0, /* charValOffset */
                                          size/*CRS_MAX_RX_CHAR_LEN*/, /* charValueLen */
                                          (uint8_t *)  p_Payload);
    }
    break;

    default:
      break;
  }

  return result;
}/* end CRS_STM_Update_Char() */



