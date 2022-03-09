/**
 ******************************************************************************
 * @file    bls.c
 * @author  MCD Application Team
 * @brief   Blood Pressure Service
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
  uint16_t SvcHdle;				          /**< Service handle */
  uint16_t MeasurementCharHdle;                    /**< Characteristic handle */
#if (BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE != 0)
uint16_t	IntermediateCuffCharHdle;                        /**< Characteristic handle */
#endif
#if (BLE_CFG_BLS_SUPPORTED_FEATURES != 0)
  uint16_t FeatureCharHdle;                        /**< Characteristic handle */  
#endif
}BLS_Context_t;


/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )


/* Private variables ---------------------------------------------------------*/
static BLS_Context_t BLS_Context;


/* Private function prototypes -----------------------------------------------*/
static void BLS_Update_Char_Value(uint16_t CharHandle, 
                                  BLS_Value_t *pBloodPressureValue);
#if (BLE_CFG_BLS_SUPPORTED_FEATURES != 0)
static void BLS_Update_Char_Feature(uint16_t CharHandle, 
                                    uint16_t *pBloodPressureFeature);
#endif
static SVCCTL_EvtAckStatus_t BLS_Event_Handler(void *Event);


/* Functions Definition ------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t BLS_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0* attribute_modified;
  BLS_App_Notification_evt_t Notification;


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
          if(attribute_modified->Attr_Handle == (BLS_Context.MeasurementCharHdle + 2))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
              /**
               * Notify to application to start measurement
               */
              if(attribute_modified->Attr_Data[0] & COMSVC_Indication)
              {
                Notification.BLS_Evt_Opcode = BLS_MEASUREMENT_IND_ENABLED_EVT;
                BLS_App_Notification(&Notification);

              }
              else
              {
                Notification.BLS_Evt_Opcode = BLS_MEASUREMENT_IND_DISABLED_EVT;
                BLS_App_Notification(&Notification);

              }
            }
#if (BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE != 0)
          if(attribute_modified->Attr_Handle == (BLS_Context.IntermediateCuffCharHdle + 2))
            {
              return_value = SVCCTL_EvtAckFlowEnable;
              /**
               * Notify to application to start intermediate cuff pressure measure
               */
              if(attribute_modified->Attr_Data[0] & COMSVC_Notification)
              {
                Notification.BLS_Evt_Opcode = BLS_INTERMEDIATE_CUFF_PRESSURE_NOTIF_ENABLED_EVT;
                BLS_App_Notification(&Notification);
              }
              else
              {
                Notification.BLS_Evt_Opcode = BLS_INTERMEDIATE_CUFF_PRESSURE_NOTIF_DISABLED_EVT;
                BLS_App_Notification(&Notification);
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
}/* end BLS_Event_Handler */


#if (BLE_CFG_BLS_SUPPORTED_FEATURES != 0)
/**
 * @brief  Blood Pressure Feature Characteristic update
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pBloodPressureFeature: The address of the new value to be written
 * @retval None
 */
static void BLS_Update_Char_Feature(uint16_t CharHandle, uint16_t *pBloodPressureFeature)
{
  uint8_t buffer[2];
  
  STORE_LE_16(&buffer[0], *pBloodPressureFeature);
  
  aci_gatt_update_char_value(BLS_Context.SvcHdle,
                             BLS_Context.FeatureCharHdle,
                             0, /* charValOffset */
                             1, /* charValueLen */
                             (uint8_t *) & buffer[0]);

  return;
}/* end BLS_Update_Char_Feature() */
#endif


/**
 * @brief  Blood Pressure Measurement Characteristic update
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pBloodPressureValue: The address of the new value to be written
 * @retval None
 */
static void BLS_Update_Char_Value(uint16_t CharHandle, 
                                  BLS_Value_t *pBloodPressureValue)
{
  uint8_t bpm_value[
#if (BLE_CFG_BLS_TIME_STAMP_FLAG != 0)
                    7 +
#endif
#if (BLE_CFG_BLS_MEASUREMENT_STATUS_FLAG != 0)
                    2 +
#endif
#if (BLE_CFG_BLS_PULSE_RATE_FLAG != 0)
                    2 +
#endif
#if (BLE_CFG_BLS_USER_ID_FLAG != 0)
                    1 +
#endif
                    1 + 6
                    ];
  uint8_t bpm_char_length = 0;

  /**
   * Flags update
   */
  bpm_value[bpm_char_length++] = (uint8_t)pBloodPressureValue->Flags;

  /**
   *  Blood Pressure Measurement Value
   */
  bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->MeasurementValue_Systolic);
  bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->MeasurementValue_Systolic >> 8);
  bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->MeasurementValue_Diastolic);
  bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->MeasurementValue_Diastolic >> 8);
  bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->MeasurementValue_Mean);
  bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->MeasurementValue_Mean >> 8);

#if (BLE_CFG_BLS_TIME_STAMP_FLAG != 0)
  if ((pBloodPressureValue->Flags) & TIME_STAMP_PRESENT)
  {
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->TimeStamp.Year);
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->TimeStamp.Year >> 8);
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->TimeStamp.Month);
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->TimeStamp.Day);
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->TimeStamp.Hours);
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->TimeStamp.Minutes);
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->TimeStamp.Seconds);
  }
#endif

 
#if (BLE_CFG_BLS_PULSE_RATE_FLAG != 0)
  /**
   *  Pulse Rate field
   */
  if ((pBloodPressureValue->Flags) & PULSE_RATE_PRESENT)
  {
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->PulseRate & 0xFF);
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->PulseRate >> 8);
  }
#endif  
 
#if (BLE_CFG_BLS_USER_ID_FLAG != 0)
  /**
   *  User Id field
   */
  if ((pBloodPressureValue->Flags) & USER_ID_PRESENT)
  {
    bpm_value[bpm_char_length++] = (uint8_t)(pBloodPressureValue->UserID);
  }
#endif  
  
#if (BLE_CFG_BLS_MEASUREMENT_STATUS_FLAG != 0)
  
  if ((pBloodPressureValue->Flags) & MEASUREMENT_STATUS_PRESENT)
  {
 
    bpm_value[bpm_char_length] = (uint8_t)pBloodPressureValue->MeasurementStatus;
    bpm_char_length++;
    bpm_value[bpm_char_length] = (uint8_t)(pBloodPressureValue->MeasurementStatus >> 8);
    bpm_char_length++;

  }
#endif  
    
    
    
  aci_gatt_update_char_value(BLS_Context.SvcHdle,
                             CharHandle,
                             0,           /* charValOffset */
                             bpm_char_length, /* charValueLen */
                             (uint8_t *) &bpm_value[0]);


  return;

}/* end BLS_Update_Char_Value() */


/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void BLS_Init(void)
{
  uint16_t uuid;
  tBleStatus hciCmdResult = BLE_STATUS_SUCCESS;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(BLS_Event_Handler);

    /**
     *  Add Blood Pressure Service
     *
     * service_max_attribute_record = 1 for Blood Pressure service +
     *                                3 for Blood Pressure measurement (2 + 1 desc) +
     *                                3 for Intermediate Cuff Pressure (2 + 1 desc) +    
     *                                2 for Blood Pressure Feature
     */
    uuid = BLOOD_PRESSURE_SERVICE_UUID;
    hciCmdResult = aci_gatt_add_service(UUID_TYPE_16,
                                        (Service_UUID_t *) &uuid,
                                        PRIMARY_SERVICE,
#if (BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE == 1)
                                        3 +
#endif
#if (BLE_CFG_BLS_SUPPORTED_FEATURES == 1)
                                        2 +
#endif
                                        4,
                                        &(BLS_Context.SvcHdle));

    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      BLE_DBG_BLS_MSG("Blood Pressure Service (BLS) is added Successfully %04X\n", 
                BLS_Context.SvcHdle);
    }
    else 
    {
      BLE_DBG_BLS_MSG ("FAILED to add Blood Pressure Service (BLS), Error: %02X !!\n", 
                   hciCmdResult);
    }
    
    /**
     *  Add Blood Pressure Measurement Characteristic
     */
    uuid = BLOOD_PRESSURE_MEASUREMENT_CHAR_UUID;
  hciCmdResult = aci_gatt_add_char(BLS_Context.SvcHdle,
                                     UUID_TYPE_16,
                                     (Char_UUID_t *) &uuid ,
#if (BLE_CFG_BLS_TIME_STAMP_FLAG != 0)
                                     7 +
#endif
#if (BLE_CFG_BLS_PULSE_RATE_FLAG != 0)
                                     2 +
#endif
#if (BLE_CFG_BLS_USER_ID_FLAG != 0)
                                     1 +
#endif
#if (BLE_CFG_BLS_MEASUREMENT_STATUS_FLAG != 0)
                                     2 +
#endif
                                     1 + 6,      /**< flags on 1 uint8_t + Measurement value on 3 uint16_t */
                                     CHAR_PROP_INDICATE,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                   &(BLS_Context.MeasurementCharHdle));

    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      BLE_DBG_BLS_MSG("BLS Measurement Characteristic Added Successfully  %04X \n", 
                   BLS_Context.MeasurementCharHdle); 
    }
    else 
    {
      BLE_DBG_BLS_MSG ("FAILED to add BLS Measurement Characteristic,  Error: %02X !!\n", 
                    hciCmdResult);
    }  
#if (BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE != 0)
    /**
     *  Add Intermediate cuff pressure measurement Characteristic
     */
  uuid = INTERMEDIATE_CUFF_PRESSURE_CHAR_UUID;
  hciCmdResult = aci_gatt_add_char(BLS_Context.SvcHdle,
                                    UUID_TYPE_16,
                                     (Char_UUID_t *) &uuid ,
#if (BLE_CFG_BLS_TIME_STAMP_FLAG != 0)
                                     7 +
#endif
#if (BLE_CFG_BLS_PULSE_RATE_FLAG != 0)
                                     2 +
#endif
#if (BLE_CFG_BLS_USER_ID_FLAG != 0)
                                     1 +
#endif
#if (BLE_CFG_BLS_MEASUREMENT_STATUS_FLAG != 0)
                                     2 +
#endif
                                     1 + 6,      /**< flags on 1 uint8_t + Measurement value on 3 uint16_t */
                                     CHAR_PROP_NOTIFY,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_VARIABLE, /* isVariable */
                                     &(BLS_Context.IntermediateCuffCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
      BLE_DBG_BLS_MSG("BLS Intermediate Cuff Pressure Characteristic Added Successfully  %04X \n", 
                  BLS_Context.IntermediateCuffCharHdle); 
    }
    else 
    {
      BLE_DBG_BLS_MSG ("FAILED to add BLS Intermediate Cuff Pressure Characteristic,  Error: %02X !!\n", 
                  hciCmdResult);
    }  
#endif

#if (BLE_CFG_BLS_SUPPORTED_FEATURES == 1)
    /**
     *  Add Blood Pressure supported features Characteristic
     */
  uuid = BLOOD_PRESSURE_FEATURE_CHAR_UUID;
  hciCmdResult = aci_gatt_add_char(BLS_Context.SvcHdle,
                                     UUID_TYPE_16,
                                     (Char_UUID_t *) &uuid ,
                                     2,
                                     CHAR_PROP_READ,
                                     ATTR_PERMISSION_NONE,
                                     GATT_DONT_NOTIFY_EVENTS, /* gattEvtMask */
                                     10, /* encryKeySize */
                                     CHAR_VALUE_LEN_CONSTANT, /* isVariable */
                                   &(BLS_Context.FeatureCharHdle));
    if (hciCmdResult == BLE_STATUS_SUCCESS)
    {
    BLE_DBG_BLS_MSG("BLS Pressure Feature Characteristic Added Successfully  %04X \n", 
                BLS_Context.FeatureCharHdle); 
    }
    else 
    {
    BLE_DBG_BLS_MSG ("FAILED to add BLS Pressure Feature Characteristic,  Error: %02X !!\n", 
                   hciCmdResult);
    }  
#endif


  return;
}


/**
 * @brief  Characteristic update
 * @param  UUID: UUID of the characteristic
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 * @param  pPayload
 */
void BLS_Update_Char(uint16_t UUID, uint8_t *pPayload)
{
  switch(UUID)
  {
    case BLOOD_PRESSURE_MEASUREMENT_CHAR_UUID:
      BLS_Update_Char_Value(BLS_Context.MeasurementCharHdle, 
                            (BLS_Value_t*) pPayload);
      break;

#if (BLE_CFG_BLS_INTERMEDIATE_CUFF_PRESSURE != 0)
    case INTERMEDIATE_CUFF_PRESSURE_CHAR_UUID:
      BLS_Update_Char_Value(BLS_Context.IntermediateCuffCharHdle, 
                            (BLS_Value_t*) pPayload);
      break;
#endif

#if (BLE_CFG_BLS_SUPPORTED_FEATURES != 0)
    case BLOOD_PRESSURE_FEATURE_CHAR_UUID:
      BLS_Update_Char_Feature(BLS_Context.FeatureCharHdle, 
                              (uint16_t*) pPayload);

      break;
#endif

    default:
      break;
  }

  return;
}/* end BLS_Update_Char() */



