/**
  ******************************************************************************
  * @file    eds_stm.c
  * @author  MCD Application Team
  * @brief   End Device Service (Custom - STM)
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
typedef struct{
  uint16_t	EndDeviceManagementSvcHdle;				/**< Service handle */
  uint16_t	EndDeviceStatusCharHdle;	                /**< Characteristic handle */
 
}EndDeviceManagementContext_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static EndDeviceManagementContext_t aEndDeviceManagementContext;

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t EndDeviceManagement_Event_Handler(void *Event);


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
#define COPY_EDM_SERVICE_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x50,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_EDM_STATUS_CHAR_UUID(uuid_struct)   COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x51,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)


/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t EndDeviceManagement_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0    * attribute_modified;
  EDS_STM_App_Notification_evt_t Notification;

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
          
            if(attribute_modified->Attr_Handle == (aEndDeviceManagementContext.EndDeviceStatusCharHdle + 2))
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
               
                Notification.EDS_Evt_Opcode = EDS_STM_NOTIFY_ENABLED_EVT;
                EDS_STM_App_Notification(&Notification);

              }
              else
              {
              
                Notification.EDS_Evt_Opcode = EDS_STM_NOTIFY_DISABLED_EVT;
                EDS_STM_App_Notification(&Notification);

              }
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
}/* end SVCCTL_EvtAckStatus_t */


/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void EDS_STM_Init(void)
{
  Char_UUID_t  uuid16;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(EndDeviceManagement_Event_Handler);
  
    /**
     *  End Device Mgt Service
     *
     * Max_Attribute_Records = 2*no_of_char + 1
     * service_max_attribute_record = 1 for End Device Mgt service +
     *                                2 for End Device Status characteristic +
     *                                1 for client char configuration descriptor 
     *                                
     */
    COPY_EDM_SERVICE_UUID(uuid16.Char_UUID_128);
    aci_gatt_add_service(UUID_TYPE_128,
                      (Service_UUID_t *) &uuid16,
                      PRIMARY_SERVICE,
                      5,
                      &(aEndDeviceManagementContext.EndDeviceManagementSvcHdle));

    /**
     *  Add End Device Status Characteristic
     */
    COPY_EDM_STATUS_CHAR_UUID(uuid16.Char_UUID_128);
    aci_gatt_add_char(aEndDeviceManagementContext.EndDeviceManagementSvcHdle,
                      UUID_TYPE_128,
                      &uuid16,
                      6,                                   
                      CHAR_PROP_READ|CHAR_PROP_NOTIFY,
                      ATTR_PERMISSION_NONE,
                      GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                      10, /* encryKeySize */
                      1, /* isVariable */
                      &(aEndDeviceManagementContext.EndDeviceStatusCharHdle));
    
     BLE_DBG_EDS_STM_MSG("-- End Device Management Service (EDMS) is added Successfully %04X\n",
                 aEndDeviceManagementContext.EndDeviceManagementSvcHdle);

    return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * 
 */
tBleStatus EDS_STM_Update_Char(uint16_t UUID, uint8_t *pPayload)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  
 
  switch(UUID)
  {
    case END_DEVICE_STATUS_CHAR_UUID:
    
      result = aci_gatt_update_char_value(aEndDeviceManagementContext.EndDeviceManagementSvcHdle,
                             aEndDeviceManagementContext.EndDeviceStatusCharHdle,
                             0, /* charValOffset */
                             6, /* charValueLen */
                             (uint8_t *)  pPayload);
     
      break;

    default:
      break;
  }

  return result;
}/* end BLE_SVC_LedButton_Update_Char() */



