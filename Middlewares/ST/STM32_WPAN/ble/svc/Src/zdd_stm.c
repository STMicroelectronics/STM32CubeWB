/**
  ******************************************************************************
  * @file    zdd_stm.c
  * @author  MCD Application Team
  * @brief   ZDD Service for Zigbee Direct (Custom STM)
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
#include "ble_events.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t	ZddP2pSvcHdle;				                /**< Service handle */
  uint16_t	ZddP2pWriteClientToServerCharHdle;	  /**< Characteristic handle */
  uint16_t	ZddP2pNotifyServerToClientCharHdle;	  /**< Characteristic handle */

  uint16_t  ZddSecSvcHdle;                        /**< Security Service handle */
  uint16_t  ZddSecP256CharHdle;                   /**< P_256 Characteristic handle */
  uint16_t  ZddSecCurve25519CharHdle;             /**< Curve25519 Characteristic handle */

  uint16_t  ZddCommSvcHdle;                       /**< Commissioning Service handle */
  uint16_t  ZddCommFormNwkCharHdle;               /**< FormNetwork Characteristic handle */
  uint16_t  ZddCommJoinNwkCharHdle;               /**< JoinNetwork Characteristic handle */
  uint16_t  ZddCommPermitJoinCharHdle;            /**< PermitJoin Characteristic handle */
  uint16_t  ZddCommLeaveNwkCharHdle;              /**< LeaveNetwork Characteristic handle */
  uint16_t  ZddCommStatusCharHdle;                /**< CommissioningStatus Characteristic handle */

  uint16_t  ZddTunnSvcHdle;                       /**< Tunneling Service handle */
  uint16_t  ZddTunnZdtsNpduCharHdle;              /**< ZDTS-NPDU Characteristic handle */

}ZDDContext_t;

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
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static ZDDContext_t aZDDContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */
/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t ZDD_Event_Handler(void *Event);
static void ZDD_SECURITY_STM_Init(void);
static void ZDD_COMMISSIONING_STM_Init(void);
static void ZDD_TUNNELING_STM_Init(void);
static void ZDD_Event_Write_Handler(aci_gatt_attribute_modified_event_rp0 *attribute_modified, ZDD_STM_Opcode_evt_t Write_Event, int8_t *Char_Name);
static void ZDD_Event_Notify_Handler(aci_gatt_attribute_modified_event_rp0 *attribute_modified, ZDD_STM_Opcode_evt_t Enable_Event, ZDD_STM_Opcode_evt_t Disable_Event);
static void ZDD_Event_Indicate_Handler(aci_gatt_attribute_modified_event_rp0 *attribute_modified, ZDD_STM_Opcode_evt_t Enable_Event, ZDD_STM_Opcode_evt_t Disable_Event);


/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_16(uuid_struct, uuid_0)   uuid_struct = uuid_0;

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
    uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
    uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
  Default P2P Server UUID
 0000fe40-CC7A-482A-984A-7F2ED5B3E58F: Service 128bits UUID
 */
#define COPY_ZDD_SERVICE_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x40,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_ZDD_WRITE_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x41,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_ZDD_NOTIFY_UUID(uuid_struct)        COPY_UUID_128(uuid_struct,0x00,0x00,0xfe,0x42,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)

/* ZDD Security Service */
/*
  ZDD Security Service UUID
 29144AF4-0000-4481-BFE9-6D0299B429E3: Service 128bits UUID
 29144AF4-0002-4481-BFE9-6D0299B429E3: Characteristic_P-256 128bits UUID
 29144AF4-0003-4481-BFE9-6D0299B429E3: Characteristic_Curve25519 128bits UUID
 */
#define COPY_ZDD_SECURITY_SERVICE_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x29,0x14,0x4A,0xF4,0x00,0x00,0x44,0x81,0xBF,0xE9,0x6D,0x02,0x99,0xB4,0x29,0xE3)
#define COPY_ZDD_SECURITY_P_256_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x29,0x14,0x4A,0xF4,0x00,0x02,0x44,0x81,0xBF,0xE9,0x6D,0x02,0x99,0xB4,0x29,0xE3)
#define COPY_ZDD_SECURITY_25519_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x29,0x14,0x4A,0xF4,0x00,0x03,0x44,0x81,0xBF,0xE9,0x6D,0x02,0x99,0xB4,0x29,0xE3)

#define ZDD_SEC_CHAR_NUMBER (2)

/* ZDD Commissioning Service */
/*
  ZDD Commissioning Service UUID
 FFF7: Service 16bits UUID
 7072377D-0001-421C-B163-491C27333A61: Characteristic_FormNetwork 128bits UUID
 7072377D-0002-421C-B163-491C27333A61: Characteristic_JoinNetwork 128bits UUID
 7072377D-0003-421C-B163-491C27333A61: Characteristic_PermitJoining 128bits UUID
 7072377D-0004-421C-B163-491C27333A61: Characteristic_LeaveNetwork 128bits UUID
 7072377D-0005-421C-B163-491C27333A61: Characteristic_CommissioningStatus 128bits UUID
 */
#define COPY_ZDD_COMMISSIONING_SERVICE_UUID(uuid_struct)  COPY_UUID_16(uuid_struct,ZIGBEE_DIRECT_COMM_SERVICE_UUID)
#define COPY_ZDD_COMM_FORM_NWK_CHAR_UUID(uuid_struct)     COPY_UUID_128(uuid_struct,0x70,0x72,0x37,0x7D,0x00,0x01,0x42,0x1C,0xB1,0x63,0x49,0x1C,0x27,0x33,0x3A,0x61)
#define COPY_ZDD_COMM_JOIN_NWK_CHAR_UUID(uuid_struct)     COPY_UUID_128(uuid_struct,0x70,0x72,0x37,0x7D,0x00,0x02,0x42,0x1C,0xB1,0x63,0x49,0x1C,0x27,0x33,0x3A,0x61)
#define COPY_ZDD_COMM_PERMIT_JOIN_CHAR_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x70,0x72,0x37,0x7D,0x00,0x03,0x42,0x1C,0xB1,0x63,0x49,0x1C,0x27,0x33,0x3A,0x61)
#define COPY_ZDD_COMM_LEAVE_NWK_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x70,0x72,0x37,0x7D,0x00,0x04,0x42,0x1C,0xB1,0x63,0x49,0x1C,0x27,0x33,0x3A,0x61)
#define COPY_ZDD_COMM_STATUS_CHAR_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x70,0x72,0x37,0x7D,0x00,0x05,0x42,0x1C,0xB1,0x63,0x49,0x1C,0x27,0x33,0x3A,0x61)

#define ZDD_COMM_CHAR_NUMBER (5)

/* ZDD Tunneling Service */
/*
  ZDD Tunneling Service UUID
 8BD178FD-0000-45F4-8120-B2378BD5313F: Service 128bits UUID
 8BD178FD-0001-45F4-8120-B2378BD5313F: Characteristic_ZDTS-NPDU 128bits UUID
 */
#define COPY_ZDD_TUNNELING_SERVICE_UUID(uuid_struct)      COPY_UUID_128(uuid_struct,0x8B,0xD1,0x78,0xFD,0x00,0x00,0x45,0xF4,0x81,0x20,0xB2,0x37,0x8B,0xD5,0x31,0x3F)
#define COPY_ZDD_TUNN_ZDTS_NPDU_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x8B,0xD1,0x78,0xFD,0x00,0x01,0x45,0xF4,0x81,0x20,0xB2,0x37,0x8B,0xD5,0x31,0x3F)

#define ZDD_TUNN_CHAR_NUMBER (1)

/**
 * @brief  Event Notify handler
 * @param  Enable_Event, Disable_Event: Events to Notify
 * @retval None
 */
static void ZDD_Event_Notify_Handler(aci_gatt_attribute_modified_event_rp0 *attribute_modified,
    ZDD_STM_Opcode_evt_t Enable_Event, ZDD_STM_Opcode_evt_t Disable_Event)
{
  ZDD_STM_App_Notification_evt_t Notification;

  /**
  * Notify to application
  */
  if(attribute_modified->Attr_Data[0] & COMSVC_Notification)
  {
    Notification.ZDD_Evt_Opcode = Enable_Event;
    ZDD_STM_App_Notification(&Notification);
  }
  else
  {
    Notification.ZDD_Evt_Opcode = Disable_Event;
    ZDD_STM_App_Notification(&Notification);
  }
}

/**
 * @brief  Event Indicate handler
 * @param  Enable_Event, Disable_Event: Events to Notify
 * @retval None
 */
static void ZDD_Event_Indicate_Handler(aci_gatt_attribute_modified_event_rp0 *attribute_modified,
    ZDD_STM_Opcode_evt_t Enable_Event, ZDD_STM_Opcode_evt_t Disable_Event)
{
  ZDD_STM_App_Notification_evt_t Notification;

  /**
  * Indicate to application
  */
  if(attribute_modified->Attr_Data[0] & COMSVC_Indication) // Indicate enabled
  {
    Notification.ZDD_Evt_Opcode = Enable_Event;
    ZDD_STM_App_Notification(&Notification);
  }
  else
  {
    Notification.ZDD_Evt_Opcode = Disable_Event;
    ZDD_STM_App_Notification(&Notification);
  }
}

/**
 * @brief  Event Write handler
 * @param  Write_Event: Event opcode
 * @param  Char_Name: Characteristic name to display
 * @retval None
 */
static void ZDD_Event_Write_Handler(aci_gatt_attribute_modified_event_rp0 *attribute_modified,
    ZDD_STM_Opcode_evt_t Write_Event, int8_t *Char_Name)
{
  ZDD_STM_App_Notification_evt_t Notification;

  Notification.ZDD_Evt_Opcode = Write_Event;
  Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
  Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
  BLE_DBG_ZDD_STM_MSG("\n-- GATT : WRITE %s CHAR RECEIVED (Len = %d)\n", Char_Name, Notification.DataTransfered.Length);
  ZDD_STM_App_Notification(&Notification);
}

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t ZDD_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0    * attribute_modified;
  aci_gatt_read_permit_req_event_rp0 *attribute_read;
  ZDD_STM_App_Notification_evt_t Notification;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
  case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
  {
    blecore_evt = (evt_blecore_aci*)event_pckt->data;
    switch(blecore_evt->ecode)
    {
    /****************************************/
    /* Handle Read request from GATT Client */
    /****************************************/
    case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE:
    {
      attribute_read = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
      /* Env char */
      if(attribute_read->Attribute_Handle == (aZDDContext.ZddCommStatusCharHdle + 1U))
      {
        BLE_DBG_ZDD_STM_MSG("\n-- GATT : READ COMM STATUS CHAR INFO RECEIVED\n");
        /* Notify to application */
        Notification.ZDD_Evt_Opcode = ZDD_COMM_STATUS_READ_EVT;
        ZDD_STM_App_Notification(&Notification);
      }
      aci_gatt_allow_read(attribute_read->Connection_Handle);
    }
    break;

    /******************************************************************/
    /* Handle Write request or Notification enabling from GATT Client */
    /******************************************************************/
    case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
    {
      attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;

      /***********************************/
      /* Handle SECURITY Characteristics */
      /***********************************/

      /* Write Security P_256 char */
      if(attribute_modified->Attr_Handle == (aZDDContext.ZddSecP256CharHdle + 1))
      {
        ZDD_Event_Write_Handler(attribute_modified, ZDD_SEC_P_256_WRITE_EVT, "P_256");
      }

      /* Indicate Security P_256 char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddSecP256CharHdle + 2U))
      {
        return_value = SVCCTL_EvtAckFlowEnable;
        ZDD_Event_Indicate_Handler(attribute_modified, ZDD_SEC_P_256_INDICATE_ENABLED_EVT, ZDD_SEC_P_256_INDICATE_DISABLED_EVT);
      }

      /* Write Security Curve25519 char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddSecCurve25519CharHdle + 1))
      {
        ZDD_Event_Write_Handler(attribute_modified, ZDD_SEC_CURVE25519_WRITE_EVT, "Curve25519");
      }

      /* Indicate Security Curve25519 char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddSecCurve25519CharHdle + 2U))
      {
        return_value = SVCCTL_EvtAckFlowEnable;
        ZDD_Event_Indicate_Handler(attribute_modified, ZDD_SEC_CURVE25519_INDICATE_ENABLED_EVT, ZDD_SEC_CURVE25519_INDICATE_DISABLED_EVT);
      }

      /****************************************/
      /* Handle COMMISSIONING Characteristics */
      /****************************************/

      /* Write Commissioning FormNetwork char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddCommFormNwkCharHdle + 1))
      {
        ZDD_Event_Write_Handler(attribute_modified, ZDD_COMM_FORM_NWK_WRITE_EVT, "FormNetwork");
      }

      /* Write Commissioning JoinNetwork char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddCommJoinNwkCharHdle + 1))
      {
        ZDD_Event_Write_Handler(attribute_modified, ZDD_COMM_JOIN_NWK_WRITE_EVT, "JoinNetwork");
      }

      /* Write Commissioning PermitJoin char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddCommPermitJoinCharHdle + 1))
      {
        ZDD_Event_Write_Handler(attribute_modified, ZDD_COMM_PERMIT_JOIN_WRITE_EVT, "PermitJoin");
      }

      /* Write Commissioning LeaveNetwork char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddCommLeaveNwkCharHdle + 1))
      {
        ZDD_Event_Write_Handler(attribute_modified, ZDD_COMM_LEAVE_NWK_WRITE_EVT, "LeaveNetwork");
      }

      /* Notify Commissioning Status char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddCommStatusCharHdle + 2U))
      {
        return_value = SVCCTL_EvtAckFlowEnable;
        ZDD_Event_Notify_Handler(attribute_modified, ZDD_COMM_STATUS_NOTIFY_ENABLED_EVT, ZDD_COMM_STATUS_NOTIFY_DISABLED_EVT);
      }

      /****************************************/
      /* Handle Tunnelling Characteristics    */
      /****************************************/

      /* Write Tunnelling ZDTS-NPDU char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddTunnZdtsNpduCharHdle + 1))
      {
        ZDD_Event_Write_Handler(attribute_modified, ZDD_TUNN_ZDTS_NPDU_WRITE_EVT, "ZDTS-NPDU");
      }

      /* Indicate Tunnelling ZDTS-NPDU char */
      else if(attribute_modified->Attr_Handle == (aZDDContext.ZddTunnZdtsNpduCharHdle + 2U))
      {
        return_value = SVCCTL_EvtAckFlowEnable;
        ZDD_Event_Indicate_Handler(attribute_modified, ZDD_TUNN_ZDTS_NPDU_INDICATE_ENABLED_EVT, ZDD_TUNN_ZDTS_NPDU_INDICATE_DISABLED_EVT);
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
 * @brief  ZDD Service initialization
 * @param  None
 * @retval None
 */
void ZDD_STM_Init(void)
{
  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(ZDD_Event_Handler);

  /* Init ZDD Security Service */
  ZDD_SECURITY_STM_Init();

  /* Init ZDD Commissioning Service */
  ZDD_COMMISSIONING_STM_Init();

  /* Init ZDD Tunneling Service */
  ZDD_TUNNELING_STM_Init();

}

/**
 * @brief  ZDD Security Service initialization
 * @param  None
 * @retval None
 */
static void ZDD_SECURITY_STM_Init(void)
{
  Char_UUID_t  uuid_sec;

  /**
   *  Security Service
   */
  COPY_ZDD_SECURITY_SERVICE_UUID(uuid_sec.Char_UUID_128);
  aci_gatt_add_service(UUID_TYPE_128,
      (Service_UUID_t *) &uuid_sec,
      PRIMARY_SERVICE,
      1+(3*ZDD_SEC_CHAR_NUMBER), /*Max_Attribute_Records*/
      &(aZDDContext.ZddSecSvcHdle));

  /**
   *  Add P-256 Characteristic
   */
  COPY_ZDD_SECURITY_P_256_CHAR_UUID(uuid_sec.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddSecSvcHdle,
      UUID_TYPE_128, &uuid_sec,
      90,
      CHAR_PROP_WRITE|CHAR_PROP_INDICATE,
      ATTR_PERMISSION_NONE,
//      ATTR_PERMISSION_ENCRY_WRITE,
      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable */
      &(aZDDContext.ZddSecP256CharHdle));

  /**
   *  Add Curve25519 Characteristic
   */
  COPY_ZDD_SECURITY_25519_CHAR_UUID(uuid_sec.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddSecSvcHdle,
      UUID_TYPE_128, &uuid_sec,
      90,
      CHAR_PROP_WRITE|CHAR_PROP_INDICATE,
      ATTR_PERMISSION_NONE,
//      ATTR_PERMISSION_ENCRY_WRITE,
      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable */
      &(aZDDContext.ZddSecCurve25519CharHdle));

  return;
}

/**
 * @brief  ZDD Commissioning Service initialization
 * @param  None
 * @retval None
 */
static void ZDD_COMMISSIONING_STM_Init(void)
{
  Char_UUID_t  uuid_com;

  /**
   *  Commissioning Service
   */
  COPY_ZDD_COMMISSIONING_SERVICE_UUID(uuid_com.Char_UUID_16);
  aci_gatt_add_service(UUID_TYPE_16,
      (Service_UUID_t *) &uuid_com,
      PRIMARY_SERVICE,
      1+(3*ZDD_COMM_CHAR_NUMBER), /*Max_Attribute_Records*/
      &(aZDDContext.ZddCommSvcHdle));

  /**
   *  Add FormNetwork Characteristic
   */
  COPY_ZDD_COMM_FORM_NWK_CHAR_UUID(uuid_com.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddCommSvcHdle,
      UUID_TYPE_128, &uuid_com,
      90,
      CHAR_PROP_WRITE,
//      ATTR_PERMISSION_NONE,
      ATTR_PERMISSION_AUTHEN_WRITE,
      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable */
      &(aZDDContext.ZddCommFormNwkCharHdle));

  /**
   *  Add JoinNetwork Characteristic
   */
  COPY_ZDD_COMM_JOIN_NWK_CHAR_UUID(uuid_com.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddCommSvcHdle,
      UUID_TYPE_128, &uuid_com,
      90,
      CHAR_PROP_WRITE,
//      ATTR_PERMISSION_NONE,
      ATTR_PERMISSION_AUTHEN_WRITE,
      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable */
      &(aZDDContext.ZddCommJoinNwkCharHdle));

  /**
   *  Add PermitJoin Characteristic
   */
  COPY_ZDD_COMM_PERMIT_JOIN_CHAR_UUID(uuid_com.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddCommSvcHdle,
      UUID_TYPE_128, &uuid_com,
      2,
      CHAR_PROP_WRITE,
//      ATTR_PERMISSION_NONE,
      ATTR_PERMISSION_AUTHEN_WRITE,
      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable */
      &(aZDDContext.ZddCommPermitJoinCharHdle));

  /**
   *  Add LeaveNetwork Characteristic
   */
  COPY_ZDD_COMM_LEAVE_NWK_CHAR_UUID(uuid_com.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddCommSvcHdle,
      UUID_TYPE_128, &uuid_com,
      4,
      CHAR_PROP_WRITE,
//      ATTR_PERMISSION_NONE,
      ATTR_PERMISSION_AUTHEN_WRITE,
      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable */
      &(aZDDContext.ZddCommLeaveNwkCharHdle));

  /**
   *  Add CommissioningStatus Characteristic
   */
  COPY_ZDD_COMM_STATUS_CHAR_UUID(uuid_com.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddCommSvcHdle,
      UUID_TYPE_128, &uuid_com,
      85,
      CHAR_PROP_READ|CHAR_PROP_NOTIFY,
//      ATTR_PERMISSION_NONE,
      ATTR_PERMISSION_AUTHEN_READ,
      GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable: 1 */
      &(aZDDContext.ZddCommStatusCharHdle));

  return;
}

/**
 * @brief  ZDD Tunneling Service initialization
 * @param  None
 * @retval None
 */
static void ZDD_TUNNELING_STM_Init(void)
{
  Char_UUID_t  uuid_tun;

  /**
   *  Tunneling Service
   */
  COPY_ZDD_TUNNELING_SERVICE_UUID(uuid_tun.Char_UUID_128);
  aci_gatt_add_service(UUID_TYPE_128,
      (Service_UUID_t *) &uuid_tun,
      PRIMARY_SERVICE,
      1+(3*ZDD_TUNN_CHAR_NUMBER), /*Max_Attribute_Records*/
      &(aZDDContext.ZddTunnSvcHdle));

  /**
   *  Add ZDTS-NPDU Characteristic
   */
  COPY_ZDD_TUNN_ZDTS_NPDU_CHAR_UUID(uuid_tun.Char_UUID_128);
  aci_gatt_add_char(aZDDContext.ZddTunnSvcHdle,
      UUID_TYPE_128, &uuid_tun,
      250,
      CHAR_PROP_WRITE|CHAR_PROP_INDICATE,
//      ATTR_PERMISSION_NONE,
      ATTR_PERMISSION_AUTHEN_WRITE|ATTR_PERMISSION_ENCRY_WRITE,
      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
      10, /* encryKeySize */
      1, /* isVariable */
      &(aZDDContext.ZddTunnZdtsNpduCharHdle));

  return;
}

/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  payloadLen: Length of the char value to be notified
 * @param  pPayload: Char value to be notified
 * @retval BLE status
 * 
 */
tBleStatus ZDD_STM_App_Update_Char(uint16_t UUID, uint8_t payloadLen, uint8_t *pPayload)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  switch(UUID)
  {
  case ZDD_SEC_P_256_CHAR_UUID:
    result = aci_gatt_update_char_value(aZDDContext.ZddSecSvcHdle,
        aZDDContext.ZddSecP256CharHdle,
        0, /* charValOffset */
        payloadLen, /* charValueLen */
        pPayload);
    break;

  case ZDD_SEC_CURVE25519_CHAR_UUID:
    result = aci_gatt_update_char_value(aZDDContext.ZddSecSvcHdle,
        aZDDContext.ZddSecCurve25519CharHdle,
        0, /* charValOffset */
        payloadLen, /* charValueLen */
        pPayload);
    break;

  case ZDD_COMM_STATUS_CHAR_UUID:
    result = aci_gatt_update_char_value(aZDDContext.ZddCommSvcHdle,
        aZDDContext.ZddCommStatusCharHdle,
        0, /* charValOffset */
        payloadLen, /* charValueLen */
        pPayload);
    break;

  case ZDD_TUNN_ZDTS_NPDU_CHAR_UUID:
    result = aci_gatt_update_char_value(aZDDContext.ZddTunnSvcHdle,
        aZDDContext.ZddTunnZdtsNpduCharHdle,
        0, /* charValOffset */
        payloadLen, /* charValueLen */
        pPayload);
    break;

  default:
    break;
  }

  return result;
}/* end ZDD_STM_Init() */


