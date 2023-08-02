/**
  ******************************************************************************
  * @file    motenv_stm.c
  * @author  SRA/AST
  * @brief   MOTENV Service (Custom STM)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2023 STMicroelectronics.
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
#include "motenv_stm_conf.h"

/* Exported Variables ------------------------------------------------------- */
uint8_t ToF_BoardPresent= 0;

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
  uint16_t	HWAccEventCharHdle; /**< Characteristic handle */
  uint16_t	HWToFCharHdle;      /**< Characteristic handle */

  /* Handles for SW Service and Chars */
  uint16_t	SWSvcHdle;               /**< Service handle */
  uint16_t	SWQuaternionsCharHdle;   /**< Characteristic handle */
  uint16_t	SWECompassCharHdle;      /**< Characteristic handle */
  uint16_t	SWActivityRecCharHdle;   /**< Characteristic handle */
  uint16_t	SWCarryPositionCharHdle; /**< Characteristic handle */
  uint16_t	SWGestureRecCharHdle;    /**< Characteristic handle */
  uint16_t	SWPedometerCharHdle;     /**< Characteristic handle */
  uint16_t	SWIntensityDetCharHdle;  /**< Characteristic handle */
#if(MOTENVWB_CFG_OTA_REBOOT_CHAR != 0)
  uint16_t  RebootReqCharHdle;                /**< Characteristic handle */
#endif

  /* Handles for Config Service and Chars */
  uint16_t	ConfigSvcHdle;  /**< Service handle */
  uint16_t	ConfigCharHdle; /**< Characteristic handle */

  /* Handles for Console Service and Chars */
  uint16_t	ConsoleSvcHdle;        /**< Service handle */
  uint16_t	ConsoleTermCharHdle;   /**< Characteristic handle */
  uint16_t	ConsoleStderrCharHdle; /**< Characteristic handle */
} MotenvContext_t;

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)
/**
 * Reboot Characteristic UUID
 * 0000fe11-8e22-4541-9d4c-21edae82ed19
 */
#if(MOTENVWB_CFG_OTA_REBOOT_CHAR != 0)
#if (UUID_128_SUPPORTED == 1)
static const uint8_t BM_REQ_CHAR_UUID[16] = {0x19, 0xed, 0x82, 0xae,
                                       0xed, 0x21, 0x4c, 0x9d,
                                       0x41, 0x45, 0x22, 0x8e,
                                       0x11, 0xFE, 0x00, 0x00};
#else
static const uint8_t BM_REQ_CHAR_UUID[2] = {0x11, 0xFE};
#endif
#endif

PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static MotenvContext_t aMotenvContext;

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Motenv_Event_Handler(void *Event);


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
#define COPY_HW_MOTION_CHAR_UUID(uuid_struct)     COPY_UUID_128(uuid_struct,0x00,0xE0,0x00,0x00,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_HW_ENV_CHAR_UUID(uuid_struct)        COPY_UUID_128(uuid_struct,0x00,0x1D,0x00,0x00,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_HW_ACC_EVENT_CHAR_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x00,0x00,0x04,0x00,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_TOF_MOBJ_CHAR_UUID(uuid_struct)      COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x13,0x00,0x02,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)

#define HW_CHAR_NUMBER (4)

/* Software Service and Characteristics */
#define COPY_SW_SERVICE_UUID(uuid_struct)               COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x00,0x02,0x11,0xE1,0x9A,0xB4,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_SW_QUATERNIONS_CHAR_UUID(uuid_struct)      COPY_UUID_128(uuid_struct,0x00,0x00,0x01,0x00,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_SW_ECOMPASS_CHAR_UUID(uuid_struct)         COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x40,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)

#define COPY_SW_ACTIVITY_REC_CHAR_UUID(uuid_struct)     COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x10,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_SW_CARRY_POSITION_CHAR_UUID(uuid_struct)   COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x08,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_SW_GESTURE_REC_CHAR_UUID(uuid_struct)      COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x02,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_SW_PEDOMETER_CHAR_UUID(uuid_struct)        COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x01,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_SW_INTENSITY_DET_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x20,0x00,0x01,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)

#define SW_CHAR_NUMBER (8)

/* Configuration Service and Characteristics */
#define COPY_CONFIG_SERVICE_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x00,0x0F,0x11,0xE1,0x9A,0xB4,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_CONFIG_CHAR_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x02,0x00,0x0F,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)

#define CONFIG_CHAR_NUMBER (1)

/* Console Service and Characteristics */
#define COPY_CONSOLE_SERVICE_UUID(uuid_struct)   COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x00,0x0E,0x11,0xE1,0x9A,0xB4,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_TERM_CHAR_UUID(uuid_struct)         COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x01,0x00,0x0E,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)
#define COPY_STDERR_CHAR_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x02,0x00,0x0E,0x11,0xE1,0xAC,0x36,0x00,0x02,0xA5,0xD5,0xC5,0x1B)

#define CONSOLE_CHAR_NUMBER (2)

/* Characteristic Length */
#define TIMESTAMP_LEN      (2)

/* Hardware Characteristic Length */
#define MOTION_CHAR_LEN    (TIMESTAMP_LEN+(3*3*2)) //(ACC+GYRO+MAG)*(X+Y+Z)*2BYTES
#define ENV_CHAR_LEN       (TIMESTAMP_LEN+(2*2)+2+4) //(2BYTES*2TEMP)+(2BYTES*HUM)+(4BYTES*PRESS)
#define ACC_EVENT_CHAR_LEN (TIMESTAMP_LEN+3)
#define TOF_CHAR_LEN       (TIMESTAMP_LEN+(2*4)+1)

/* Software Characteristic Length */
#define QUATERNION_NUM          (3)
#define QUATERNION_CHAR_LEN     (TIMESTAMP_LEN+(6*QUATERNION_NUM))
#define ECOMPASS_CHAR_LEN       (TIMESTAMP_LEN+2)
#define ACTIVITY_REC_CHAR_LEN   (TIMESTAMP_LEN+1)
#define CARRY_POSITION_CHAR_LEN (TIMESTAMP_LEN+1)
#define GESTURE_REC_CHAR_LEN    (TIMESTAMP_LEN+1)
#define PEDOMETER_CHAR_LEN      (TIMESTAMP_LEN+4+2)
#define INTENSITY_DET_CHAR_LEN  (TIMESTAMP_LEN+1)

/* Configuration Characteristic Length */
#define CONFIG_CHAR_LEN (TIMESTAMP_LEN+18)

/* Console Characteristic Length */
#define CONSOLE_CHAR_LEN (TIMESTAMP_LEN+18)

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
          /* Acc Event char */
          if(read_permit_req->Attribute_Handle == (aMotenvContext.HWAccEventCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ ACC EVENT CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = HW_ACC_EVENT_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          /* Activity Rec char */
          else if(read_permit_req->Attribute_Handle == (aMotenvContext.SWActivityRecCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ ACTIVITY REC CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = SW_ACTIVITY_REC_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          /* CarryPosition char */
          else if(read_permit_req->Attribute_Handle == (aMotenvContext.SWCarryPositionCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ CARRY POSITION CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = SW_CARRY_POSITION_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          /* GestureRec char */
          else if(read_permit_req->Attribute_Handle == (aMotenvContext.SWGestureRecCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ GESTURE REC CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = SW_GESTURE_REC_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          /* Pedometer char */
          else if(read_permit_req->Attribute_Handle == (aMotenvContext.SWPedometerCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ PEDOMETER CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = SW_PEDOMETER_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          else
          {
            /* do nothing */
          }
          /* Console Term char */
          if(read_permit_req->Attribute_Handle == (aMotenvContext.ConsoleTermCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ CONSOLE TERM CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = CONSOLE_TERM_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          /* Console Stderr char */
          if(read_permit_req->Attribute_Handle == (aMotenvContext.ConsoleStderrCharHdle + 1U))
          {
            /**
            * Notify to application
            */
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : READ CONSOLE STDERR CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = CONSOLE_STDERR_READ_EVT;
            MOTENV_STM_App_Notification(&Notification);
          }
          (void)aci_gatt_allow_read(read_permit_req->Connection_Handle);
          break;
        }

        /* Handle Write request or Notification enabling from GATT Client */
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
       {
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

          /* Acc Event char */
          if(attribute_modified->Attr_Handle == (aMotenvContext.HWAccEventCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = HW_ACC_EVENT_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = HW_ACC_EVENT_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }
          
          /* ToF char */
          if(attribute_modified->Attr_Handle == (aMotenvContext.HWToFCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = HW_TOF_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = HW_TOF_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }
          
          /* ToF char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.HWToFCharHdle + 1U))
          {
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : WRITE TOF CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = HW_TOF_WRITE_EVT;
            Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
            Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
            MOTENV_STM_App_Notification(&Notification);
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

          /* MotionFX (Quat) char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.SWQuaternionsCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = SW_MOTIONFX_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = SW_MOTIONFX_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* MotionFX (ECompass) char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.SWECompassCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = SW_ECOMPASS_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = SW_ECOMPASS_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* Activity Rec char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.SWActivityRecCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = SW_ACTIVITY_REC_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = SW_ACTIVITY_REC_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* Carry Position char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.SWCarryPositionCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = SW_CARRY_POSITION_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = SW_CARRY_POSITION_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* GestureRec char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.SWGestureRecCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = SW_GESTURE_REC_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = SW_GESTURE_REC_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* Pedometer char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.SWPedometerCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = SW_PEDOMETER_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = SW_PEDOMETER_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* IntensityDet char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.SWIntensityDetCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = SW_INTENSITY_DET_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = SW_INTENSITY_DET_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* Configuration char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.ConfigCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = CONFIG_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = CONFIG_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* Configuration char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.ConfigCharHdle + 1U))
          {
            BLE_DBG_TEMPLATE_STM_MSG("-- GATT : WRITE CONFIG CHAR INFO RECEIVED\n");
            Notification.Motenv_Evt_Opcode = CONFIG_WRITE_EVT;
            Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
            Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
            MOTENV_STM_App_Notification(&Notification);
          }

          /* Console Term char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.ConsoleTermCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = CONSOLE_TERM_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = CONSOLE_TERM_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }

          /* Console Stderr char */
          else if(attribute_modified->Attr_Handle == (aMotenvContext.ConsoleStderrCharHdle + 2U))
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
              Notification.Motenv_Evt_Opcode = CONSOLE_STDERR_NOTIFY_ENABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
            else
            {
              Notification.Motenv_Evt_Opcode = CONSOLE_STDERR_NOTIFY_DISABLED_EVT;
              MOTENV_STM_App_Notification(&Notification);
            }
          }
          /* OTA char */
#if(MOTENVWB_CFG_OTA_REBOOT_CHAR != 0)
          else if(attribute_modified->Attr_Handle == (aMotenvContext.RebootReqCharHdle + 1))
          {
            BLE_DBG_P2P_STM_MSG("-- GATT : REBOOT REQUEST RECEIVED\n");
            Notification.Motenv_Evt_Opcode = MOTENV_STM_BOOT_REQUEST_EVT;
            Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
            Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
            MOTENV_STM_App_Notification(&Notification);
          }
#endif
          else
          {
            /* do nothing */
          }
        break;
       }

        default:
          break;
      }
      break; /* HCI_HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE_SPECIFIC */
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

     if(ToF_BoardPresent)
     { 
      /**
       *   Add ToF Characteristic for HW Service
       */
       COPY_TOF_MOBJ_CHAR_UUID(uuid16.Char_UUID_128);
       (void)aci_gatt_add_char(aMotenvContext.HWSvcHdle,
                               UUID_TYPE_128, &uuid16,
                               TOF_CHAR_LEN,
                               CHAR_PROP_NOTIFY|CHAR_PROP_WRITE_WITHOUT_RESP|CHAR_PROP_WRITE,
                               ATTR_PERMISSION_NONE,
                               GATT_NOTIFY_ATTRIBUTE_WRITE|GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                               16, /* encryKeySize */
                               1, /* isVariable: 1 */
                               &(aMotenvContext.HWToFCharHdle));
     }

    /**
     *   Add Acc Event Characteristic for HW Service
     */
    COPY_HW_ACC_EVENT_CHAR_UUID(uuid16.Char_UUID_128);
    (void)aci_gatt_add_char(aMotenvContext.HWSvcHdle,
                            UUID_TYPE_128, &uuid16,
                            ACC_EVENT_CHAR_LEN,
                            CHAR_PROP_NOTIFY|CHAR_PROP_READ,
                            ATTR_PERMISSION_NONE,
                            GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                            16, /* encryKeySize */
                            1, /* isVariable: 1 */
                            &(aMotenvContext.HWAccEventCharHdle));

  /**
   *   Add SW Service
   */
  COPY_SW_SERVICE_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid16,
                             PRIMARY_SERVICE,
                             1+(3*SW_CHAR_NUMBER), /*Max_Attribute_Records*/
                             &(aMotenvContext.SWSvcHdle));

  /**
   *   Add Quaternions Characteristic for SW Service
   */
  COPY_SW_QUATERNIONS_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          QUATERNION_CHAR_LEN,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.SWQuaternionsCharHdle));

  /**
   *   Add ECompass Characteristic for SW Service
   */
  COPY_SW_ECOMPASS_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          ECOMPASS_CHAR_LEN,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.SWECompassCharHdle));

  /**
   *   Add Activity Rec Characteristic for SW Service
   */
  COPY_SW_ACTIVITY_REC_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          ACTIVITY_REC_CHAR_LEN,
                          CHAR_PROP_NOTIFY | CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.SWActivityRecCharHdle));

  /**
   *   Add Carry Position Characteristic for SW Service
   */
  COPY_SW_CARRY_POSITION_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          CARRY_POSITION_CHAR_LEN,
                          CHAR_PROP_NOTIFY | CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.SWCarryPositionCharHdle));

  /**
   *   Add Gesture Rec Characteristic for SW Service
   */
  COPY_SW_GESTURE_REC_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          GESTURE_REC_CHAR_LEN,
                          CHAR_PROP_NOTIFY | CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.SWGestureRecCharHdle));

  /**
   *   Add Pedometer Characteristic for SW Service
   */
  COPY_SW_PEDOMETER_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          PEDOMETER_CHAR_LEN,
                          CHAR_PROP_NOTIFY | CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.SWPedometerCharHdle));

  /**
   *   Add IntensityDet Characteristic for SW Service
   */
  COPY_SW_INTENSITY_DET_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          INTENSITY_DET_CHAR_LEN,
                          CHAR_PROP_NOTIFY,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.SWIntensityDetCharHdle));

  /**
   *   Add Config Service
   */
  COPY_CONFIG_SERVICE_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid16,
                             PRIMARY_SERVICE,
                             1+(3*CONFIG_CHAR_NUMBER), /*Max_Attribute_Records*/
                             &(aMotenvContext.ConfigSvcHdle));

  /**
   *   Add Config Characteristic for Config Service
   */
  COPY_CONFIG_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.ConfigSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          CONFIG_CHAR_LEN,
                          CHAR_PROP_NOTIFY | CHAR_PROP_WRITE_WITHOUT_RESP,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          0, /* isVariable: 1 */
                          &(aMotenvContext.ConfigCharHdle));

  /**
   *   Add Console Service
   */
  COPY_CONSOLE_SERVICE_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_service(UUID_TYPE_128,
                             (Service_UUID_t *) &uuid16,
                             PRIMARY_SERVICE,
                             1+(3*CONSOLE_CHAR_NUMBER), /*Max_Attribute_Records*/
                             &(aMotenvContext.ConsoleSvcHdle));
  /**
   *   Add Console Term Characteristic for Config Service
   */
  COPY_TERM_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.ConsoleSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          CONSOLE_CHAR_LEN,
                          CHAR_PROP_NOTIFY | CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_WRITE | CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          1, /* isVariable: 1 */
                          &(aMotenvContext.ConsoleTermCharHdle));
  /**
   *   Add Console Stderr Characteristic for Config Service
   */
  COPY_STDERR_CHAR_UUID(uuid16.Char_UUID_128);
  (void)aci_gatt_add_char(aMotenvContext.ConsoleSvcHdle,
                          UUID_TYPE_128, &uuid16,
                          CONSOLE_CHAR_LEN,
                          CHAR_PROP_NOTIFY | CHAR_PROP_READ,
                          ATTR_PERMISSION_NONE,
                          GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, /* gattEvtMask */
                          16, /* encryKeySize */
                          1, /* isVariable: 1 */
                          &(aMotenvContext.ConsoleStderrCharHdle));

#if(MOTENVWB_CFG_OTA_REBOOT_CHAR != 0)
    /**
     *  Add Boot Request Characteristic
     */
    (void)aci_gatt_add_char(aMotenvContext.SWSvcHdle,
                            BM_UUID_LENGTH,
                            (Char_UUID_t *)BM_REQ_CHAR_UUID,
                            BM_REQ_CHAR_SIZE,
                            CHAR_PROP_WRITE_WITHOUT_RESP,
                            ATTR_PERMISSION_NONE,
                            GATT_NOTIFY_ATTRIBUTE_WRITE,
                            10,
                            0,
                            &(aMotenvContext.RebootReqCharHdle));
#endif
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

      
    case TOF_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.HWSvcHdle,
                                         aMotenvContext.HWToFCharHdle,
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

    case ACC_EVENT_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.HWSvcHdle,
                                         aMotenvContext.HWAccEventCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
    
      break;

    case MOTION_FX_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.SWSvcHdle,
                                         aMotenvContext.SWQuaternionsCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case ECOMPASS_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.SWSvcHdle,
                                         aMotenvContext.SWECompassCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case ACTIVITY_REC_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.SWSvcHdle,
                                         aMotenvContext.SWActivityRecCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case CARRY_POSITION_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.SWSvcHdle,
                                         aMotenvContext.SWCarryPositionCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case GESTURE_REC_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.SWSvcHdle,
                                         aMotenvContext.SWGestureRecCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case PEDOMETER_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.SWSvcHdle,
                                         aMotenvContext.SWPedometerCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case INTENSITY_DET_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.SWSvcHdle,
                                         aMotenvContext.SWIntensityDetCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case CONFIG_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.ConfigSvcHdle,
                                         aMotenvContext.ConfigCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case CONSOLE_TERM_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.ConsoleSvcHdle,
                                         aMotenvContext.ConsoleTermCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    case CONSOLE_STDERR_CHAR_UUID:

     result = aci_gatt_update_char_value(aMotenvContext.ConsoleSvcHdle,
                                         aMotenvContext.ConsoleStderrCharHdle,
                                         0, /* charValOffset */
                                         payloadLen, /* charValueLen */
                                         pPayload);
     break;

    default:
      break;
  }

  return result;
}


