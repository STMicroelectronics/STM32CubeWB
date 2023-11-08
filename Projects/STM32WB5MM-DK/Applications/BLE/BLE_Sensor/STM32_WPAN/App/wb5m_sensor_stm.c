/**
  ******************************************************************************
  * @file    motenv_stm.c
  * @author  SRA/AST
  * @brief   MOTENV Service (Custom STM)
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
#include "wb5m_sensor_stm.h"
/* Private typedef -----------------------------------------------------------*/
/**
 * @brief  MOTENV Context structure definition
 */
typedef struct
{
  /* Handles for HW Service and Chars */
  uint16_t	HWSvcHdle;          /**< Service handle */
  uint16_t	HWMotionCharHdle;   /**< Characteristic handle */
  uint16_t	HWEnvCharHdle;      /**< Characteristic handle */
} MotenvContext_t;

typedef enum
{
  COMSVC_Notification = ( 1 << 0 ),
  COMSVC_Indication =   ( 1 << 1 ),
} COMSVC_ClientCharConfMask_t;
/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static MotenvContext_t aMotenvContext;

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Motenv_Event_Handler(void *pckt);


/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Service and Characteristics */
#define COPY_HW_SERVICE_UUID(uuid_struct)         COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x00,0x01,0x11,0xE1,0x9A,0xB4,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_HW_MOTION_CHAR_UUID(uuid_struct)     COPY_UUID_128(uuid_struct,0x00,0xC0,0x00,0x00,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_HW_ENV_CHAR_UUID(uuid_struct)        COPY_UUID_128(uuid_struct,0x00,0x04,0x00,0x00,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)

#define HW_CHAR_NUMBER (3)

/* Characteristic Length */
#define TIMESTAMP_LEN      (2)

/* Hardware Characteristic Length */
#define MOTION_CHAR_LEN    (TIMESTAMP_LEN+(2*3*2)) //(ACC+GYRO)*(X+Y+Z)*2BYTES
#define ENV_CHAR_LEN       (TIMESTAMP_LEN+2) //(2BYTES*TEMP)

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Motenv_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0    * attribute_modified;
  aci_gatt_read_permit_req_event_rp0 *read_permit_req;
  MOTENV_STM_App_Notification_evt_t Notification;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
    {
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch(blecore_evt->ecode)
      {
        /* Handle Read request from GATT Client */
        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE:
        {
          read_permit_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          /* Env char */
          if(read_permit_req->Attribute_Handle == (aMotenvContext.HWEnvCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ ENV CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = HW_ENV_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          else
          {
            /* do nothing */
          }
          (void)aci_gatt_allow_read(read_permit_req->Connection_Handle);
          break;
        }

        /* Handle Write request or Notification enabling from GATT Client */
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
       {
          read_permit_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          /* Env char */
          if(attribute_modified->Attr_Handle == (aMotenvContext.HWEnvCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = HW_ENV_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = HW_ENV_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* Motion char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.HWMotionCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = HW_MOTION_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = HW_MOTION_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          else
          {
            /* do nothing */
          }
          break;
       }

        default:
          break;
      }
      break; /* HCI_EVT_VENDOR_SPECIFIC */
    }

    default:
      break;
  }

  return(return_value);
}/* end Motenv_Event_Handler */


/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void MOTENV_STM_Init(void)
{
  Char_UUID_t uuid16;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Motenv_Event_Handler);
  
  /**
   *   Add HW Service
   */
  COPY_HW_SERVICE_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid16,
                             PRIMARY_SERVICE,
                             1+(3*HW_CHAR_NUMBER), /*Max_Attribute_Records*/
                             &(aMotenvContext.HWSvcHdle));
  /**
   *   Add Motion Characteristic for HW Service
   */
  COPY_HW_MOTION_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.HWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          MOTION_CHAR_LEN,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.HWMotionCharHdle));

    /**
     *   Add Env Characteristic for HW Service
     */
    COPY_HW_ENV_CHAR_UUID(uuid16.Char_UUID_128);
    (void)aci_gatt_add_char(aMotenvContext.HWSvcHdle,
                            UUID_TYPE_128, &uuid16,
                            ENV_CHAR_LEN,
                            CHAR_PROP_NOTIFY|CHAR_PROP_READ,
                            ATTR_PERMISSION_NONE,
                            GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                            16, /* encryKeySize */
                            0, /* isVariable: 1 */
                            &(aMotenvContext.HWEnvCharHdle));

  return;
} /* end MOTENV_STM_Init */

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  payloadLen: Length of the char value to be notified
 * @param  pPayload: Char value to be notified
 * @retval BLE status
 */
tBleStatus MOTENV_STM_App_Update_Char(uint16_t UUID, uint8_t payloadLen, uint8_t *pPayload) 
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  switch(UUID)
  {
    case ENV_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.HWSvcHdle,
                                         aMotenvContext.HWEnvCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
    
      break;

    case MOTION_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.HWSvcHdle,
                                         aMotenvContext.HWMotionCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);

      break;

    default:
      break;
  }

  return result;
}
