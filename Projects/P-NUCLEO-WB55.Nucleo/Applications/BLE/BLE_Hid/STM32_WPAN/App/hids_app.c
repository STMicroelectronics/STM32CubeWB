/**
  ******************************************************************************
  * @file    hids_app.c
  * @author  MCD Application Team
  * @brief   Human Interface Device Service Application
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
#include "app_common.h"

#include "dbg_trace.h"
#include "app_ble.h"
#include "ble.h"
#include "hids_app.h"
#include "stm32_seq.h"
#include <time.h>
#include "hids_menu.h"


/* Private typedef -----------------------------------------------------------*/
#if((BLE_CFG_HIDS_INPUT_REPORT_NB != 0) || (BLE_CFG_HIDS_KEYBOARD_DEVICE != 0) || (BLE_CFG_HIDS_MOUSE_DEVICE != 0))
typedef struct
{
#if(BLE_CFG_HIDS_INPUT_REPORT_NB != 0)
  uint8_t ReportNotificationEnabled[BLE_CFG_HIDS_INPUT_REPORT_NB];
#endif
#if(BLE_CFG_HIDS_KEYBOARD_DEVICE != 0)
  uint8_t KeyboardInputNotificationEnabled;
#endif
#if(BLE_CFG_HIDS_MOUSE_DEVICE != 0)
  uint8_t MouseInputNotificationEnabled;
#endif
} HIDSAPP_Context_t;
#endif

typedef struct
{
  uint8_t buttons;
  int8_t x;
  int8_t y;
  int8_t wheel;
} mouse_report_t;


/* Private defines -----------------------------------------------------------*/
#define MOUSE_REPORT_SIZE       52

#define ENABLED         1
#define DISABLED        0

  
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t report_mouse[MOUSE_REPORT_SIZE] =
{
  0x05, 0x01,         /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x02,         /* USAGE (Mouse) */
  0xa1, 0x01,         /* COLLECTION (Application) */
  0x09, 0x01,         /*   USAGE (Pointer) */
  0xa1, 0x00,         /*   COLLECTION (Physical) */
  0x05, 0x09,         /*     USAGE_PAGE (Button) */
  0x19, 0x01,         /*     USAGE_MINIMUM (Button 1) */
  0x29, 0x03,         /*     USAGE_MAXIMUM (Button 3) */
  0x15, 0x00,         /*     LOGICAL_MINIMUM (0) */
  0x25, 0x01,         /*     LOGICAL_MAXIMUM (1) */
  0x95, 0x03,         /*     REPORT_COUNT (3) */
  0x75, 0x01,         /*     REPORT_SIZE (1) */
  0x81, 0x02,         /*     INPUT (Data,Var,Abs) */
  0x95, 0x01,         /*     REPORT_COUNT (1) */
  0x75, 0x05,         /*     REPORT_SIZE (5) */
  0x81, 0x03,         /*     INPUT (Cnst,Var,Abs) */
  0x05, 0x01,         /*     USAGE_PAGE (Generic Desktop) */
  0x09, 0x30,         /*     USAGE (X) */
  0x09, 0x31,         /*     USAGE (Y) */
  0x09, 0x38,         /*     USAGE (Wheel) */
  0x15, 0x81,         /*     LOGICAL_MINIMUM (-127) */
  0x25, 0x7f,         /*     LOGICAL_MAXIMUM (127) */
  0x75, 0x08,         /*     REPORT_SIZE (8) */
  0x95, 0x03,         /*     REPORT_COUNT (3) */
  0x81, 0x06,         /*     INPUT (Data,Var,Rel) */
  0xc0,               /*   END_COLLECTION (Physical) */
  0xc0,               /* END_COLLECTION (Application) */
};


/**
 * START of Section BLE_APP_CONTEXT
 */

#if((BLE_CFG_HIDS_INPUT_REPORT_NB != 0) || (BLE_CFG_HIDS_KEYBOARD_DEVICE != 0) || (BLE_CFG_HIDS_MOUSE_DEVICE != 0))
HIDSAPP_Context_t HIDSAPP_Context[BLE_CFG_HIDS_NUMBER];
#endif

/**
 * END of Section BLE_APP_CONTEXT
 */

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void HIDS_Notification(HIDS_App_Notification_evt_t *pNotification)
{
  switch(pNotification->HIDS_Evt_Opcode)
  {

#if(BLE_CFG_HIDS_INPUT_REPORT_NB != 0)
    case HIDS_REPORT_NOTIFICATION_ENABLED:
      {
        BLE_DBG_APP_MSG("HIDS_REPORT_NOTIFICATION_ENABLED\n");
        HIDSAPP_Context[pNotification->Index].ReportNotificationEnabled[pNotification->Index] = ENABLED;
      }
      break;

    case HIDS_REPORT_NOTIFICATION_DISABLED:
      {
        BLE_DBG_APP_MSG("HIDS_REPORT_NOTIFICATION_DISABLED\n");
        HIDSAPP_Context[pNotification->Index].ReportNotificationEnabled[pNotification->Index] = DISABLED;
      }
      break;
#endif
      
#if(BLE_CFG_HIDS_KEYBOARD_DEVICE != 0)
    case HIDS_KEYB_INPUT_NOTIFY_ENABLED:
      {
        BLE_DBG_APP_MSG("HIDS_KEYB_INPUT_NOTIFY_ENABLED\n");
        HIDSAPP_Context[pNotification->Index].KeyboardInputNotificationEnabled = ENABLED;
      }
      break;

    case HIDS_KEYB_INPUT_NOTIFY_DISABLED:
      {
        BLE_DBG_APP_MSG("HIDS_KEYB_INPUT_NOTIFY_DISABLED\n");
        HIDSAPP_Context[pNotification->Index].KeyboardInputNotificationEnabled = DISABLED;
      }
      break;
#endif

#if(BLE_CFG_HIDS_MOUSE_DEVICE != 0)          
    case HIDS_MOUSE_INPUT_NOTIFY_ENABLED:
      {
        BLE_DBG_APP_MSG("HIDS_MOUSE_INPUT_NOTIFY_ENABLED\n");
        HIDSAPP_Context[pNotification->Index].MouseInputNotificationEnabled = ENABLED;
      }
      break;

    case HIDS_MOUSE_INPUT_NOTIFY_DISABLED:
      {
        BLE_DBG_APP_MSG("HIDS_MOUSE_INPUT_NOTIFY_DISABLED\n");
        HIDSAPP_Context[pNotification->Index].MouseInputNotificationEnabled = DISABLED;
      }
      break;
#endif

#if(BLE_CFG_HIDS_OUTPUT_REPORT_NB != 0)
    case HIDS_OUTPUT_REPORT:
      {
        uint8_t i;
        
        BLE_DBG_APP_MSG("HIDS_OUTPUT_REPORT\n");
        BLE_DBG_HIDS_MSG("HID Instance %d Report %d \n", 
                          pNotification->Instance,
                          pNotification->Index); 
    
        for(i = 0; i < pNotification->ReportLength; i++)
          BLE_DBG_HIDS_MSG("Report[%d] 0x%X \n",
                           i,
                           pNotification->pReport[i]);
      }
      break;
#endif
    
#if((BLE_CFG_HIDS_MOUSE_DEVICE != 0) && (BLE_CFG_HIDS_MOUSE_INPUT_WRITE != 0))
    case HIDS_MOUSE_INPUT_REPORT:
      {
        uint8_t i;
        
        BLE_DBG_APP_MSG("HIDS_MOUSE_INPUT_REPORT\n");
        BLE_DBG_HIDS_MSG("HID Instance %d Report %d \n", 
                          pNotification->Instance,
                          pNotification->Index); 
    
        for(i = 0; i < pNotification->ReportLength; i++)
          BLE_DBG_HIDS_MSG("Report[%d] 0x%X \n",
                           i,
                           pNotification->pReport[i]);
      }
      break;
#endif
      
#if((BLE_CFG_HIDS_KEYBOARD_DEVICE != 0) && (BLE_CFG_HIDS_KEYBOARD_INPUT_WRITE != 0))
    case HIDS_KEYBOARD_INPUT_REPORT:
      {
        uint8_t i;
        
        BLE_DBG_APP_MSG("HIDS_KEYBOARD_INPUT_REPORT\n");
        BLE_DBG_HIDS_MSG("HID Instance %d Report %d \n", 
                          pNotification->Instance,
                          pNotification->Index); 
    
        for(i = 0; i < pNotification->ReportLength; i++)
          BLE_DBG_HIDS_MSG("Report[%d] 0x%X \n",
                           i,
                           pNotification->pReport[i]);
      }
      break;

    case HIDS_KEYBOARD_OUTPUT_REPORT:
      {
        uint8_t i;
        
        BLE_DBG_APP_MSG("HIDS_KEYBOARD_OUTPUT_REPORT\n");
        BLE_DBG_HIDS_MSG("HID Instance %d Report %d \n", 
                          pNotification->Instance,
                          pNotification->Index); 
    
        for(i = 0; i < pNotification->ReportLength; i++)
          BLE_DBG_HIDS_MSG("Report[%d] 0x%X \n",
                           i,
                           pNotification->pReport[i]);
      }
      break;
#endif
      
    default:
      break;
  }

  return;
}


void HIDSAPP_Init(void)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;

  UTIL_SEQ_RegTask( 1<< CFG_TASK_HID_UPDATE_REQ_ID, UTIL_SEQ_RFU, HIDSAPP_Profile_UpdateChar );

  result = HIDS_Update_Char(REPORT_MAP_CHAR_UUID, 
                            0, 
                            0, 
                            MOUSE_REPORT_SIZE,
                            (uint8_t *)&report_mouse);

  if( result == BLE_STATUS_SUCCESS )
  {
    BLE_DBG_APP_MSG("Report Map Successfully Sent\n");
  }
  else 
  {
    BLE_DBG_APP_MSG("Sending of Report Map Failed error 0x%X\n", result);
  }
}


/**
 * @brief  Alert Notification Application service update characteristic
 * @param  None
 * @retval None
 */
void HIDSAPP_Profile_UpdateChar(void)
{
  uint8_t action_type;
  mouse_report_t mouse_report;
  
  HIDS_Menu(&action_type, (uint8_t *)&mouse_report);
    
  if(action_type == 1)
  {
    tBleStatus result = BLE_STATUS_INVALID_PARAMS;

    result = HIDS_Update_Char(REPORT_CHAR_UUID, 
                              0, 
                              0, 
                              sizeof(mouse_report_t),
                              (uint8_t *)& mouse_report);

    if( result == BLE_STATUS_SUCCESS )
    {
      BLE_DBG_APP_MSG("Mouse Report 0x%x %d %d %d Successfully Sent\n",
                       mouse_report.buttons,
                       mouse_report.x,
                       mouse_report.y,
                       mouse_report.wheel);
    }
    else 
    {
      BLE_DBG_APP_MSG("Sending of Mouse Report Failed error 0x%X\n", result);
    }
  }
}

