/**
  ******************************************************************************
  * @file    template_stm.c
  * @author  MCD Application Team
  * @brief   Template Service (Custom STM)
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
  uint16_t	TemplateSvcHdle;				        /**< Service handle */
  uint16_t	TemplateWriteClientToServerCharHdle;	  /**< Characteristic handle */
  uint16_t	TemplateNotifyServerToClientCharHdle;	/**< Characteristic handle */
  uint16_t  RebootReqCharHdle;                /**< Characteristic handle */
}TemplateContext_t;

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)


/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/**
 * Reboot Characteristic UUID
 * 0000fe11-8e22-4541-9d4c-21edae82ed19
 */
#if(BLE_CFG_OTA_REBOOT_CHAR != 0)
#if (UUID_128_SUPPORTED == 1)
static const uint8_t BM_REQ_CHAR_UUID[16] = {0x19, 0xed, 0x82, 0xae,
                                       0xed, 0x21, 0x4c, 0x9d,
                                       0x41, 0x45, 0x22, 0x8e,
                                       0x11, 0xFE, 0x00, 0x00};
#else
static const uint8_t BM_REQ_CHAR_UUID[2] = {0x11, 0xFE};
#endif
#endif

PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static TemplateContext_t aTemplateContext;

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Template_Event_Handler(void *Event);


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
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_TEMPLATE_SERVICE_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x00,0x00,0xAA,0xBB,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_TEMPLATE_WRITE_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xAA,0xCC,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_TEMPLATE_NOTIFY_UUID(uuid_struct)        COPY_UUID_128(uuid_struct,0x00,0x00,0xAA,0xDD,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)



/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Template_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0    * attribute_modified;
  TEMPLATE_STM_App_Notification_evt_t Notification;

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
            if(attribute_modified->Attr_Handle == (aTemplateContext.TemplateNotifyServerToClientCharHdle + 2))
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
                Notification.Template_Evt_Opcode = TEMPLATE_STM_NOTIFY_ENABLED_EVT;
                TEMPLATE_STM_App_Notification(&Notification);
              }
              else
              {
                Notification.Template_Evt_Opcode = TEMPLATE_STM_NOTIFY_DISABLED_EVT;
                TEMPLATE_STM_App_Notification(&Notification);
              }
            }
            
            else if(attribute_modified->Attr_Handle == (aTemplateContext.TemplateWriteClientToServerCharHdle + 1))
            {
              BLE_DBG_TEMPLATE_STM_MSG("-- GATT : WRITE CHAR INFO RECEIVED\n");
              Notification.Template_Evt_Opcode = TEMPLATE_STM_WRITE_EVT;
              Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
              Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
              TEMPLATE_STM_App_Notification(&Notification);  
            }
          
            else if(attribute_modified->Attr_Handle == (aTemplateContext.RebootReqCharHdle + 1))
            {
              BLE_DBG_TEMPLATE_STM_MSG("-- GATT : REBOOT REQUEST RECEIVED\n");
              Notification.Template_Evt_Opcode = TEMPLATE_STM_BOOT_REQUEST_EVT;
              Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
              Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
              TEMPLATE_STM_App_Notification(&Notification);
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
void SVCCTL_InitCustomSvc(void)
{
 
  Char_UUID_t  uuid16;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Template_Event_Handler);
  
    /**
     *  Peer To Peer Service
     *
     * Max_Attribute_Records = 2*no_of_char + 1
     * service_max_attribute_record = 1 for Template service +
     *                                2 for Template Write characteristic +
     *                                2 for Template Notify characteristic +
     *                                1 for client char configuration descriptor +
     *                                
     */

    COPY_TEMPLATE_SERVICE_UUID(uuid16.Char_UUID_128);
    aci_gatt_add_service(UUID_TYPE_128,
                      (Service_UUID_t *) &uuid16,
                      PRIMARY_SERVICE,
                      8, /*Max_Attribute_Records*/
                      &(aTemplateContext.TemplateSvcHdle));

    /**
     *  Add Write Characteristic
     */
    COPY_TEMPLATE_WRITE_CHAR_UUID(uuid16.Char_UUID_128);
    aci_gatt_add_char(aTemplateContext.TemplateSvcHdle,
                      UUID_TYPE_128, &uuid16,
                      2,                                   
                      CHAR_PROP_WRITE_WITHOUT_RESP|CHAR_PROP_READ,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                      10, /* encryKeySize */
                      1, /* isVariable */
                      &(aTemplateContext.TemplateWriteClientToServerCharHdle));

    /**
     *   Add Notify Characteristic
     */
    COPY_TEMPLATE_NOTIFY_UUID(uuid16.Char_UUID_128);
    aci_gatt_add_char(aTemplateContext.TemplateSvcHdle,
                      UUID_TYPE_128, &uuid16,
                      2,
                      CHAR_PROP_NOTIFY,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                      10, /* encryKeySize */
                      1, /* isVariable: 1 */
                      &(aTemplateContext.TemplateNotifyServerToClientCharHdle));
  
 #if(OTA_REBOOT_SUPPORT != 0)   
    /**
     *  Add Boot Request Characteristic
     */
   aci_gatt_add_char(aTemplateContext.TemplateSvcHdle,
                      BM_UUID_LENGTH,
                      (Char_UUID_t *)BM_REQ_CHAR_UUID,
                      BM_REQ_CHAR_SIZE,
                      CHAR_PROP_WRITE_WITHOUT_RESP,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE,
                      10,
                      0,
                      &(aTemplateContext.RebootReqCharHdle));
#endif  
  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * 
 */
tBleStatus TEMPLATE_STM_App_Update_Char(uint16_t UUID, uint8_t *pPayload) 
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  switch(UUID)
  {
    case 0x0000:
      
     result = aci_gatt_update_char_value(aTemplateContext.TemplateSvcHdle,
                             aTemplateContext.TemplateNotifyServerToClientCharHdle,
                              0, /* charValOffset */
                             2, /* charValueLen */
                             (uint8_t *)  pPayload);
    
      break;

    default:
      break;
  }

  return result;
}/* end TEMPLATE_STM_Init() */


