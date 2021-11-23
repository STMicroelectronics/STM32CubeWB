/**
 ******************************************************************************
 * @file    hst.c
 * @author  MCD Application Team
 * @brief   Health Thermometer Service
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
  uint16_t SvcHdle;				  /**< Service handle */
  uint16_t TemperatureMeasurementCharHdle;   /**< Characteristic handle */
#if(BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 1)
  uint16_t TemperatureTypeCharHdle;		      /**< Characteristic handle */
#endif
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE != 0)
  uint16_t IntermediateTemperatureCharHdle;  /**< Characteristic handle */
#endif
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL != 0)
  uint16_t  MeasurementIntervalCharHdle;      /**< Characteristic handle */
  uint16_t  ValidRangeDescHdle;               /**< Descriptor handle */
#endif
}HTS_Context_t;


/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define INTERVAL_OUT_OF_RANGE       (0x80)


/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static HTS_Context_t HTS_Context;

#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_WR_PROP != 0)
static const uint8_t aValidRangeInterval[4] = 
{
  (uint8_t)BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE, \
  (uint8_t)(BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE>>8), \
  (uint8_t)BLE_CFG_HTS_TEMPERATURE_INTERVAL_MAX_VALUE, \
  (uint8_t)(BLE_CFG_HTS_TEMPERATURE_INTERVAL_MAX_VALUE>>8) \
};
#endif


/* Private function prototypes -----------------------------------------------*/
static tBleStatus Update_Char_TemperatureValue(uint16_t CharHandle, HTS_TemperatureValue_t *pTemperatureValue);
static SVCCTL_EvtAckStatus_t HTS_Event_Handler(void *Event);


/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t HTS_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0    * attribute_modified;
  HTS_App_Notification_evt_t Notification;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
    {
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch(blecore_evt->ecode)
      {
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL != 0)
        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
        {
          aci_gatt_write_permit_req_event_rp0 * write_perm_req;
          
          write_perm_req = (aci_gatt_write_permit_req_event_rp0*)blecore_evt->data;

          if(write_perm_req->Attribute_Handle == (HTS_Context.MeasurementIntervalCharHdle + 1))
          {
            uint32_t validrangevalue;
            
            return_value = SVCCTL_EvtAckFlowEnable;
            validrangevalue = write_perm_req->Data[0] + (write_perm_req->Data[1] << 8);

            if ((validrangevalue == 0) ||
                ((validrangevalue >= BLE_CFG_HTS_TEMPERATURE_INTERVAL_MIN_VALUE) &&
                 (validrangevalue <= BLE_CFG_HTS_TEMPERATURE_INTERVAL_MAX_VALUE)))
            {
              /**
               * received a correct value
               */

              aci_gatt_write_resp(write_perm_req->Connection_Handle,
                                      write_perm_req->Attribute_Handle,
                                      0x00, /* write_status = 0 (no error))*/
                                      0x00, /* err_code */
                                      write_perm_req->Data_Length,
                                      (uint8_t *)&write_perm_req->Data[0]);

              /**
               * Notify to the application a new interval has been received
               */
              Notification.RangeInterval = (uint16_t)validrangevalue;
              Notification.HTS_Evt_Opcode = HTS_MEASUREMENT_INTERVAL_RECEIVED_EVT;
              HTS_App_Notification(&Notification);
            }
            else
            {
              /**
               * received a value out of range
               */
              aci_gatt_write_resp(write_perm_req->Connection_Handle,
                                      write_perm_req->Attribute_Handle,
                                      0x1, /* write_status = 1 (error))*/
                                      INTERVAL_OUT_OF_RANGE, /* err_code */
                                      write_perm_req->Data_Length,
                                      (uint8_t *)&write_perm_req->Data[0]);
            }
          }
        }
        break;
#endif

        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
        {
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;
          if(attribute_modified->Attr_Handle == (HTS_Context.TemperatureMeasurementCharHdle + 2))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /**
             * Notify to application to start measurement
             */
            if(attribute_modified->Attr_Data[0] & COMSVC_Indication)
            {
              Notification.HTS_Evt_Opcode = HTS_MEASUREMENT_IND_ENABLED_EVT;
              HTS_App_Notification(&Notification);

            }
            else
            {
              Notification.HTS_Evt_Opcode = HTS_MEASUREMENT_IND_DISABLED_EVT;
              HTS_App_Notification(&Notification);

            }
          }
#if(BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE != 0)
          if(attribute_modified->Attr_Handle == (HTS_Context.IntermediateTemperatureCharHdle + 2))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /**
             * Notify to application to start measurement
             */
            if(attribute_modified->Attr_Data[0] & COMSVC_Notification)
            {
              Notification.HTS_Evt_Opcode = HTS_INTERMEDIATE_TEMPERATURE_NOT_ENABLED_EVT;
              HTS_App_Notification(&Notification);

            }
            else
            {
              Notification.HTS_Evt_Opcode = HTS_INTERMEDIATE_TEMPERATURE_NOT_DISABLED_EVT;
              HTS_App_Notification(&Notification);

            }
          }
#endif
#if(BLE_CFG_HTS_MEASUREMENT_INTERVAL != 0)
          if(attribute_modified->Attr_Handle == (HTS_Context.MeasurementIntervalCharHdle + 2))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /**
             * Notify to application to start measurement
             */
            if(attribute_modified->Attr_Data[0] & COMSVC_Indication)
            {
              Notification.HTS_Evt_Opcode = HTS_MEASUREMENT_INTERVAL_IND_ENABLED_EVT;
              HTS_App_Notification(&Notification);

            }
            else
            {
              Notification.HTS_Evt_Opcode = HTS_MEASUREMENT_INTERVAL_IND_DISABLED_EVT;
              HTS_App_Notification(&Notification);

            }
          }
#endif
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
}/* end HTS_Event_Handler */

/**
 * @brief  Temperature Measurement Characteristic update
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pTemperatureValue: The address of the new value to be written
 * @retval None
 */
static tBleStatus Update_Char_TemperatureValue(uint16_t CharHandle, 
                                               HTS_TemperatureValue_t *pTemperatureValue)
{
  tBleStatus return_value;

  uint8_t atm_value[
#if (BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
                    7 +
#endif
#if (BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 0)
                    1 +
#endif
                    1 + 4
                    ];
  uint8_t tm_char_length;

  /**
   * Flags update
   */
  atm_value[0] = (uint8_t)pTemperatureValue->Flags;

  /**
   *  Temperature Measurement Value
   */
  atm_value[1] = (uint8_t)(pTemperatureValue->MeasurementValue);
  atm_value[2] = (uint8_t)(pTemperatureValue->MeasurementValue >> 8);
  atm_value[3] = (uint8_t)(pTemperatureValue->MeasurementValue >> 16);
  atm_value[4] = (uint8_t)(pTemperatureValue->MeasurementValue >> 24);

  tm_char_length = 5;

#if (BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
  if (pTemperatureValue->Flags &  SENSOR_TIME_STAMP_PRESENT)
  {
    atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TimeStamp.Year);
    tm_char_length++;
    atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TimeStamp.Year >> 8);
    tm_char_length++;
    atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TimeStamp.Month);
    tm_char_length++;
    atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TimeStamp.Day);
    tm_char_length++;
    atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TimeStamp.Hours);
    tm_char_length++;
    atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TimeStamp.Minutes);
    tm_char_length++;
    atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TimeStamp.Seconds);
    tm_char_length++;
  }
#endif

#if (BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 0)
  /**
   * The Temperature Type Info flag shall be set by the application
   */
  atm_value[tm_char_length] = (uint8_t)(pTemperatureValue->TemperatureType);
  tm_char_length++;
#endif

  return_value = aci_gatt_update_char_value(HTS_Context.SvcHdle,
                                            CharHandle,
                                            0,           /* charValOffset */
                                            tm_char_length, /* charValueLen */
                                            (uint8_t *) &atm_value[0]);


  return return_value;

}/* end Update_Char_TemperatureValue() */


/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void HTS_Init(void)
{
  uint16_t uuid;
  tBleStatus hciCmdResult = BLE_STATUS_SUCCESS;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(HTS_Event_Handler);

  /**
   *  Add Health Thermometer Service
   *
   * service_max_attribute_record = 1 for health thermometer service +
   *                                3 for temperature measurement (2 + 1 desc) +
   *                                2 for temperature type +
   *                                3 for intermediate temperature ( 2 + 1 desc) +
   *                                2 for measurement interval +
   *                                1 for measurement interval indicate descriptor +
   *                                1 for measurement interval write descriptor +
   */
  uuid = HEALTH_THERMOMETER_SERVICE_UUID;
  hciCmdResult = aci_gatt_add_service(UUID_TYPE_16,
                                      (Service_UUID_t *) &uuid,
                                      PRIMARY_SERVICE,
#if (BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 1)
                                      2+
#endif
#if (BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE != 0)
                                      3+
#endif
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL != 0)
                                      2+
#endif
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_IND_PROP != 0)
                                      1+
#endif
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_WR_PROP != 0)
                                      1+
#endif
                                      4,
                                      &(HTS_Context.SvcHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HTS_MSG ("Health Thermometer Service (HTS) is added Successfully %04X\n", 
                 HTS_Context.SvcHdle);
  }
  else
  {
    BLE_DBG_HTS_MSG ("FAILED to add Health Thermometer Service (HTS), Error: %02X !!\n", 
                 hciCmdResult);
  }
  
  /**
   *  Add Temperature Measurement Characteristic
   */
  uuid = TEMPERATURE_MEASUREMENT_CHAR_UUID;
  hciCmdResult = aci_gatt_add_char(HTS_Context.SvcHdle,
                    UUID_TYPE_16,
                    (Char_UUID_t *) &uuid ,
#if (BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
                    7 +
#endif
#if (BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 0)
                    1 +
#endif
                    1 + 4,    /**< flags + Temp value*/
                    CHAR_PROP_INDICATE,
                    ATTR_PERMISSION_NONE,
                    GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                    10, /* encryKeySize */
                    CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                    &(HTS_Context.TemperatureMeasurementCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HTS_MSG ("Temperature Measurement Characteristic Added Successfully  %04X \n", 
                 HTS_Context.TemperatureMeasurementCharHdle);
  }
  else
  {
    BLE_DBG_HTS_MSG ("FAILED to add Temperature Measurement Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }

#if (BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 1)
  /**
   *  Add Temperature Type Characteristic
   */
  uuid = TEMPERATURE_TYPE_CHAR_UUID;
  hciCmdResult = aci_gatt_add_char(HTS_Context.SvcHdle,
                    UUID_TYPE_16,
                    (Char_UUID_t *) &uuid ,
                    1,
                    CHAR_PROP_READ,
                    ATTR_PERMISSION_NONE,
                    GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                    10, /* encryKeySize */
                    CHAR_VALUE_LEN_CONSTANT, /* isVariable */
                    &(HTS_Context.TemperatureTypeCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HTS_MSG ("Temperature Type Characteristic Added Successfully  %04X \n", 
                 HTS_Context.TemperatureTypeCharHdle);
  }
  else
  {
    BLE_DBG_HTS_MSG ("FAILED to add Temperature Type Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }
#endif

#if (BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE != 0)
  /**
   *  Add Intermediate Temperature Characteristic
   */
  uuid = INTERMEDIATE_TEMPERATURE_CHAR_UUID;
  hciCmdResult = aci_gatt_add_char(HTS_Context.SvcHdle,
                    UUID_TYPE_16,
                    (Char_UUID_t *) &uuid ,
#if (BLE_CFG_HTS_TIME_STAMP_FLAG != 0)
                    7 +
#endif
#if (BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 0)
                    1 +
#endif
                    1 + 4,    /**< flags + Temp value*/
                    CHAR_PROP_NOTIFY,
                    ATTR_PERMISSION_NONE,
                    GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                    10, /* encryKeySize */
                    CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                    &(HTS_Context.IntermediateTemperatureCharHdle));

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HTS_MSG ("Intermediate Temperature Characteristic Added Successfully  %04X \n", 
                 HTS_Context.IntermediateTemperatureCharHdle);
  }
  else
  {
    BLE_DBG_HTS_MSG ("FAILED to add Intermediate Temperature Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }
#endif

#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL != 0)
  /**
   *  Add Measurement interval Characteristic
   */
  uuid = MEASUREMENT_INTERVAL_CHAR_UUID;
  hciCmdResult = aci_gatt_add_char(HTS_Context.SvcHdle,
                    UUID_TYPE_16,
                    (Char_UUID_t *) &uuid ,
                    2,
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_WR_PROP != 0)
                    CHAR_PROP_WRITE |
#endif
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_IND_PROP != 0)
                    CHAR_PROP_INDICATE |
#endif
                    CHAR_PROP_READ,
                    ATTR_PERMISSION_AUTHEN_WRITE,
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_WR_PROP != 0)
                    GATT_NOTIFY_ATTRIBUTE_WRITE |
#endif
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_IND_PROP != 0)
                    GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP |
#endif
                    GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                    10, /* encryKeySize */
                    CHAR_VALUE_LEN_CONSTANT, /* isVariable */
                    &(HTS_Context.MeasurementIntervalCharHdle));


  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HTS_MSG ("Measurement Interval Characteristic Added Successfully  %04X \n", 
                 HTS_Context.MeasurementIntervalCharHdle);
  }
  else
  {
    BLE_DBG_HTS_MSG ("FAILED to add Measurement Interval Characteristic, Error: %02X !!\n", 
                hciCmdResult);
  }

  /* Reset manually the permission of the Client Characteristic Configuration descriptor to NONE */
  hciCmdResult = aci_gatt_set_security_permission(HTS_Context.SvcHdle, 
                                   HTS_Context.MeasurementIntervalCharHdle+2,
                                   ATTR_PERMISSION_NONE);

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HTS_MSG ("Set Permission None to Measurement Interval CCC Successfully  %04X \n", 
                 HTS_Context.MeasurementIntervalCharHdle+2);
  }
  else
  {
    BLE_DBG_HTS_MSG ("FAILED to set Permission None to Measurement Interval CCC, Error: %02X !!\n", 
                hciCmdResult);
  }

  /**
   * Add the valid range descriptor
   */
#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL_WR_PROP != 0)
  uuid = VALID_RANGE_DESCRIPTOR_UUID;
  hciCmdResult = aci_gatt_add_char_desc(HTS_Context.SvcHdle,
                         HTS_Context.MeasurementIntervalCharHdle,
                         UUID_TYPE_16,
                         (Char_Desc_Uuid_t *)&uuid,
                         4,
                         4,
                         (void *)&aValidRangeInterval,
                         ATTR_PERMISSION_NONE,
                         ATTR_ACCESS_READ_ONLY,
                         GATT_DONT_NOTIFY_EVENTS,
                         10,
                         CHAR_VALUE_LEN_CONSTANT,
                         &HTS_Context.ValidRangeDescHdle);

  if (hciCmdResult == BLE_STATUS_SUCCESS)
  {
    BLE_DBG_HTS_MSG ("Valid Range Descriptor Added Successfully  %04X \n", 
                 HTS_Context.ValidRangeDescHdle);
  }
  else
  {
    BLE_DBG_HTS_MSG ("FAILED to add Valid Range Descriptor, Error: %02X !!\n", 
                hciCmdResult);
  }

#endif
#endif

  return;
}


/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pPayload
 */
tBleStatus HTS_Update_Char(uint16_t UUID, 
                           uint8_t *pPayload)
{
  tBleStatus return_value=0;

  switch(UUID)
  {
    case TEMPERATURE_MEASUREMENT_CHAR_UUID:
      return_value = Update_Char_TemperatureValue(HTS_Context.TemperatureMeasurementCharHdle, 
                                                  (HTS_TemperatureValue_t*) pPayload);
      break;

#if (BLE_CFG_HTS_TEMPERATURE_TYPE_VALUE_STATIC == 1)
    case TEMPERATURE_TYPE_CHAR_UUID:
      return_value = aci_gatt_update_char_value(HTS_Context.SvcHdle,
                                                HTS_Context.TemperatureTypeCharHdle,
                                                0,           /* charValOffset */
                                                1, /* charValueLen */
                                                (uint8_t *)pPayload);

      break;
#endif

#if (BLE_CFG_HTS_INTERMEDIATE_TEMPERATURE != 0)
    case INTERMEDIATE_TEMPERATURE_CHAR_UUID:
      return_value = Update_Char_TemperatureValue(HTS_Context.IntermediateTemperatureCharHdle, 
                                                  (HTS_TemperatureValue_t*) pPayload);
      break;
#endif

#if (BLE_CFG_HTS_MEASUREMENT_INTERVAL != 0)
    case MEASUREMENT_INTERVAL_CHAR_UUID:
      return_value = aci_gatt_update_char_value(HTS_Context.SvcHdle,
                                                HTS_Context.MeasurementIntervalCharHdle,
                                                0,           /* charValOffset */
                                                2, /* charValueLen */
                                                (uint8_t *)pPayload);
      break;
#endif

    default:
      break;
  }

  return return_value;
}/* end HTS_Update_Char() */



