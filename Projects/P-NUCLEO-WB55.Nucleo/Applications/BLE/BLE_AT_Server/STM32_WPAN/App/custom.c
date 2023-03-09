/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : App/custom.c
  * Description        : Application file for BLE Middleware.
  *
  *****************************************************************************
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "app_common.h"

#include "dbg_trace.h"
#include "ble.h"
#include "tl.h"
#include "app_ble.h"

#include "stm32_seq.h"
#include "shci.h"
#include "stm32_lpm.h"
#include "otp.h"
#include "p2p_server_app.h"
#include "stm32wb_at_ble.h"
#include "ble_at_server.h"
#include "custom_app.h"
#include "custom.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */
typedef enum
{
  CUSTOMS_NOTIFICATION_MASK = (1 << 0),
  CUSTOMS_INDICATION_MASK = (1 << 1),
} ClientCharConfMask_t;

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define APPBLE_GAP_DEVICE_NAME_LENGTH 7
#define FAST_ADV_TIMEOUT               (30*1000*1000/CFG_TS_TICK_VAL) /**< 30s */
#define INITIAL_ADV_TIMEOUT            (60*1000*1000/CFG_TS_TICK_VAL) /**< 60s */

#define BD_ADDR_SIZE_LOCAL    6

/* USER CODE BEGIN PD */
#define LED_ON_TIMEOUT                 (0.005*1000*1000/CFG_TS_TICK_VAL) /**< 5ms */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern BleApplicationContext_t BleApplicationContext;
extern Custom_Context_t Custom_Context;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
 
static SVCCTL_EvtAckStatus_t Custom_Event_Handler(void *Event)
{
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_att_exchange_mtu_resp_event_rp0 *exchange_mtu_resp;
  aci_gatt_attribute_modified_event_rp0 *attribute_modified;
  tBleStatus result;
  SVCCTL_EvtAckStatus_t return_value;
  
  CUSTOMS_App_Notification_evt_t Notification;

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  /* USER CODE BEGIN Custom_Event_Handler */

  /* USER CODE END Custom_Event_Handler */

  switch (event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*) event_pckt->data;
      /* USER CODE BEGIN EVT_VENDOR */

      /* USER CODE END EVT_VENDOR */
      switch (blecore_evt->ecode)
      {
      /* USER CODE BEGIN ecode */

        case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
        {
          printf("ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE \n");
          exchange_mtu_resp = (aci_att_exchange_mtu_resp_event_rp0 *)blecore_evt->data;
          printf("MTU_size = %d \n",exchange_mtu_resp->Server_RX_MTU );
          result = hci_le_set_data_length(BleApplicationContext.BleApplicationContext_legacy.connectionHandle,251,2120);
          if (result != BLE_STATUS_SUCCESS)
          {
            printf("set data length command error %x \n", result);
          }
          else
          {
            printf("set data length command success \n");
          }
        }
        break;
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
        {
          uint8_t i;
          uint8_t id_svc;
          uint8_t id_char;
          
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*) blecore_evt->data;
          
          for (id_svc=3;id_svc<6;id_svc++)
          {
            for (id_char=0;id_char<5;id_char++)
            {
              if (attribute_modified->Attr_Handle == (Custom_Context.CharHdle[id_svc-3][id_char-1] + 2))
              {
                return_value = SVCCTL_EvtAckFlowEnable;
                /**
                * Notify to application to start measurement
                */
                if (attribute_modified->Attr_Data[0] & CUSTOMS_NOTIFICATION_MASK)
                {
                  APP_DBG_MSG("svc: %d char:%d NOTIFICATION ENABLED \n", id_svc, id_char);
                  Notification.Evt_Opcode = CUSTOM_NOTIFICATION_ENABLED;
                  CUSTOMS_Notification(&Notification);
                }
                else if (attribute_modified->Attr_Data[0] & CUSTOMS_INDICATION_MASK)
                {
                  APP_DBG_MSG("svc: %d char:%d INDICATION ENABLED \n", id_svc, id_char);
                  Notification.Evt_Opcode = CUSTOM_INDICATION_ENABLED;
                  CUSTOMS_Notification(&Notification);
                }
                else
                {
                  Notification.Evt_Opcode = CUSTOM_INDICATION_DISABLED;
                  CUSTOMS_Notification(&Notification);
                }
              }
            }
          }
                   
          for (id_svc=3;id_svc<6;id_svc++)
          {
            for (id_char=0;id_char<5;id_char++)
            {
              if(attribute_modified->Attr_Handle == (Custom_Context.CharHdle[id_svc-3][id_char-1] + 1))
              {       
                return_value = SVCCTL_EvtAckFlowEnable;
                printf("WRITE CHARAC svc: %d char: %d - LENGTH = %d \n", id_svc, id_char, attribute_modified->Attr_Data_Length);
                printf("Reception : ");
                for (i = 0; i < attribute_modified->Attr_Data_Length; i++)
                {
                  printf("Data[%x] = 0x%x ", i, attribute_modified->Attr_Data[i]);
                }
                printf(" \r\n");
                Notification.service_index = id_svc;
                Notification.charac_index = id_char;
                Notification.length = attribute_modified->Attr_Data_Length;
                for (i = 0; i < attribute_modified->Attr_Data_Length; i++)
                {
                  Notification.charac_value[i] = attribute_modified->Attr_Data[i];
                }
                Notification.Evt_Opcode = CUSTOM_WRITE_EVT;
                CUSTOMS_Notification(&Notification);
              }
            }
          }
        }
        break;
        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
        {
            uint8_t i;
            uint8_t id_svc;
            uint8_t id_char;
            
            aci_gatt_write_permit_req_event_rp0 * write_perm_req;
            write_perm_req = (aci_gatt_write_permit_req_event_rp0*)blecore_evt->data;
            for (id_svc=3;id_svc<6;id_svc++)
            {
              for (id_char=0;id_char<5;id_char++)
              {
                if(write_perm_req->Attribute_Handle == (Custom_Context.CharHdle[id_svc-3][id_char-1] + 1))
                {     
                  return_value = SVCCTL_EvtAckFlowEnable;
                  printf("WRITE PERMIT REQ CHARAC svc: %d char: %d - LENGTH = %d \n", id_svc, id_char, write_perm_req->Data_Length);
                  printf("Reception : ");
                  for (i = 0; i < write_perm_req->Data_Length; i++)
                  {
                    printf("Data[%x] = 0x%x ", i, write_perm_req->Data[i]);
                  }
                  printf(" \r\n");
                  Notification.service_index = id_svc;
                  Notification.charac_index = id_char;
                  Notification.length = write_perm_req->Data_Length;
                  for (i = 0; i < write_perm_req->Data_Length; i++)
                  {
                    Notification.charac_value[i] = write_perm_req->Data[i];
                  }
                  return_value = SVCCTL_EvtAckFlowEnable;          
                  aci_gatt_write_resp(write_perm_req->Connection_Handle,
                                      write_perm_req->Attribute_Handle,
                                      0x00, /* write_status = 0 (no error))*/
                                      0x00, /* err_code */
                                      write_perm_req->Data_Length,
                                      (uint8_t *)&(write_perm_req->Data[0]));
                  Notification.Evt_Opcode = CUSTOM_WRITE_EVT;
                  CUSTOMS_Notification(&Notification);
                }
              }
            }
          }
          break;  
          
      /* USER CODE END ecode */
#if(RADIO_ACTIVITY_EVENT != 0)
        case ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE:
        /* USER CODE BEGIN RADIO_ACTIVITY_EVENT*/
          BSP_LED_On(LED_GREEN);
          HW_TS_Start(BleApplicationContext.SwitchOffGPIO_timer_Id, (uint32_t)LED_ON_TIMEOUT);
        /* USER CODE END RADIO_ACTIVITY_EVENT*/
          break; /* ACI_HAL_END_OF_RADIO_ACTIVITY_VSEVT_CODE */
#endif

      /* USER CODE BEGIN BLUE_EVT */

      /* USER CODE END BLUE_EVT */
      }
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT */

      /* USER CODE END EVENT_PCKT */

      default:
      /* USER CODE BEGIN ECODE_DEFAULT*/

      /* USER CODE END ECODE_DEFAULT*/
      break;
  }

  return(return_value);
}

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void Custom_SVC_Init(void)
{
  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_Event_Handler);
        
  return;
}
/* USER CODE END FD*/

/* USER CODE END FD_SPECIFIC_FUNCTIONS */

/* USER CODE BEGIN FD_WRAP_FUNCTIONS */

/* USER CODE END FD_WRAP_FUNCTIONS */
