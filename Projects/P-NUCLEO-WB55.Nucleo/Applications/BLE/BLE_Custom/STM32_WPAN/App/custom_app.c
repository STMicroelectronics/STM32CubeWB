/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* My_P2P_Server */
  uint8_t               Switch_c_Notification_Status;
  uint8_t               Long_c_Notification_Status;
  /* My_Heart_Rate */
  uint8_t               Hrs_m_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */
  uint8_t               SW1_Status;
  uint8_t               SW2_Status;
  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CUSTOM_STM_HRS_VALUE_MIN        0x44
#define CUSTOM_STM_HRS_VALUE_MAX        0x49
#define CUSTOM_STM_HRS_ENERGY_MIN       0x0100
#define CUSTOM_STM_HRS_ENERGY_RESET     0x00
#define CUSTOM_STM_HRS_ENERGY_NOT_RESET 0x01

#define TOGGLE_ON                       1
#define TOGGLE_OFF                      0
/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[512];
uint8_t NotifyCharData[512];
uint16_t Connection_Handle;
/* USER CODE BEGIN PV */
uint8_t hr_energy_reset = CUSTOM_STM_HRS_ENERGY_NOT_RESET;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* My_P2P_Server */
static void Custom_Switch_c_Update_Char(void);
static void Custom_Switch_c_Send_Notification(void);
/* My_Heart_Rate */
static void Custom_Hrs_m_Update_Char(void);
static void Custom_Hrs_m_Send_Notification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */
  static uint16_t hr_value, hr_energy;
  tBleStatus        ret = BLE_STATUS_INVALID_PARAMS;
    
  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* My_P2P_Server */
    case CUSTOM_STM_LED_C_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LED_C_READ_EVT */
       PRINT_MESG_DBG("ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE My_Led_Char Read\n");

      /* USER CODE END CUSTOM_STM_LED_C_READ_EVT */
      break;

    case CUSTOM_STM_LED_C_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LED_C_WRITE_NO_RESP_EVT */
      APP_DBG_MSG("\r\n\r** CUSTOM_STM_LED_C_WRITE_NO_RESP_EVT \n");
      APP_DBG_MSG("\r\n\r** Write Data: 0x%02X %02X \n", pNotification->DataTransfered.pPayload[0], pNotification->DataTransfered.pPayload[1]);
      if (pNotification->DataTransfered.pPayload[1] == 0x01)
      {
        HAL_GPIO_WritePin(Blue_Led_GPIO_Port, Blue_Led_Pin, GPIO_PIN_SET); 
      }
      if (pNotification->DataTransfered.pPayload[1] == 0x00)
      {
        HAL_GPIO_WritePin(Blue_Led_GPIO_Port, Blue_Led_Pin, GPIO_PIN_RESET); 
      } 
      /* USER CODE END CUSTOM_STM_LED_C_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_SWITCH_C_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_SWITCH_C_NOTIFY_ENABLED_EVT */
      APP_DBG_MSG("\r\n\r** CUSTOM_STM_BUTTON_C_NOTIFY_ENABLED_EVT \n");

      Custom_App_Context.Switch_c_Notification_Status = TOGGLE_ON;        /* My_Switch_Char notification status has been enabled */
      /* USER CODE END CUSTOM_STM_SWITCH_C_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_SWITCH_C_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_SWITCH_C_NOTIFY_DISABLED_EVT */
      APP_DBG_MSG("\r\n\r** CUSTOM_STM_BUTTON_C_NOTIFY_DISABLED_EVT \n");

      Custom_App_Context.Switch_c_Notification_Status = TOGGLE_OFF;        /* My_Switch_Char notification status has been disabled */
      /* USER CODE END CUSTOM_STM_SWITCH_C_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_LONG_C_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LONG_C_NOTIFY_ENABLED_EVT */
      APP_DBG_MSG("\r\n\r** CUSTOM_STM_LONG_C_NOTIFY_ENABLED_EVT \n");
      Custom_App_Context.Long_c_Notification_Status = TOGGLE_ON;
      ret = aci_gatt_exchange_config(Connection_Handle);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("aci_gatt_exchange_config failure: reason=0x%02X\n", ret);
      }
      else
      {
        APP_DBG_MSG("==>> aci_gatt_exchange_config : Success\n");
      }
      /* USER CODE END CUSTOM_STM_LONG_C_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_LONG_C_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LONG_C_NOTIFY_DISABLED_EVT */
      APP_DBG_MSG("\r\n\r** CUSTOM_STM_LONG_C_NOTIFY_DISABLED_EVT \n");
      Custom_App_Context.Long_c_Notification_Status = TOGGLE_OFF;
      /* USER CODE END CUSTOM_STM_LONG_C_NOTIFY_DISABLED_EVT */
      break;

    /* My_Heart_Rate */
    case CUSTOM_STM_HRS_M_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HRS_M_NOTIFY_ENABLED_EVT */
       PRINT_MESG_DBG("ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE My_HRS_Meas NOTIFICATION_ENABLED\n");

       Custom_App_Context.Hrs_m_Notification_Status = 1;         /* Hrs_m notification status has been enabled */

       /* Set Heart Rate Measurement value */
       if ((hr_value >= CUSTOM_STM_HRS_VALUE_MIN) && (hr_value <= CUSTOM_STM_HRS_VALUE_MAX))
       {
         hr_value += 1;
         
         /* Check if energy expanded has been reset */
         if (hr_energy_reset == CUSTOM_STM_HRS_ENERGY_RESET)
         {
           hr_energy = (uint16_t)CUSTOM_STM_HRS_ENERGY_MIN;
           hr_energy_reset = (uint8_t)CUSTOM_STM_HRS_ENERGY_NOT_RESET;
         }
         else
         {
           hr_energy += 1;
         }
       }
       else
       {
         hr_value = (uint16_t)CUSTOM_STM_HRS_VALUE_MIN;
         hr_energy = (uint16_t)CUSTOM_STM_HRS_ENERGY_MIN;
       }
       PRINT_MESG_DBG("hr_value : %d, hr_energy : %d\n", hr_value, hr_energy);
       
       /* NotifyCharData array size depends on SizeHrs_M defined in custom_stm.c file */
       NotifyCharData[0] = HRS_HRM_ENERGY_EXPENDED_PRESENT | HRS_HRM_VALUE_FORMAT_UINT16;     /* Heart Rate Value Format is set to UINT16. Units: beats per minute (bpm). Energy Expended field is present. Units: kilo Joules  */
       NotifyCharData[1] = (uint8_t)hr_value;           /* Heart Rate Measurement Value LSB*/
       NotifyCharData[2] = (uint8_t)(hr_value >> 8);    /* Heart Rate Measurement Value MSB*/
       NotifyCharData[3] = (uint8_t)hr_energy;          /* Energy Expended LSB*/
       NotifyCharData[4] = (uint8_t)(hr_energy >> 8);   /* Energy Expended MSB*/
       
       Custom_Hrs_m_Send_Notification();                       
      /* USER CODE END CUSTOM_STM_HRS_M_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT */
      PRINT_MESG_DBG("ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE My_HRS_Meas NOTIFICATION_DISABLED\n");
      
      Custom_App_Context.Hrs_m_Notification_Status = 0;         /* Hrs_m notification status has been disabled */
      /* USER CODE END CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_HRS_SL_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HRS_SL_READ_EVT */

      /* USER CODE END CUSTOM_STM_HRS_SL_READ_EVT */
      break;

    case CUSTOM_STM_HRS_CTRLP_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HRS_CTRLP_WRITE_EVT */
      PRINT_MESG_DBG("ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE My_HRS_CTRL_Point Write\n");
      
      /* reset energy expended */
      hr_energy_reset = CUSTOM_STM_HRS_ENERGY_RESET;
      /* USER CODE END CUSTOM_STM_HRS_CTRLP_WRITE_EVT */
      break;

    case CUSTOM_STM_NOTIFICATION_COMPLETE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */

      /* USER CODE END CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */
      Connection_Handle = pNotification->ConnectionHandle;
      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */
      Connection_Handle = pNotification->ConnectionHandle;
      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */
  uint8_t sensor_loc;

  sensor_loc = CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_WRIST;
  Custom_STM_App_Update_Char(CUSTOM_STM_HRS_SL, (uint8_t *) &sensor_loc);       /*initialise My_Sensor_Loc char to wrist */
  
  Custom_Switch_c_Update_Char();
  Custom_Hrs_m_Update_Char();

  UTIL_SEQ_RegTask(1<< CFG_TASK_SW1_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, Custom_Switch_c_Send_Notification);
  
  Custom_App_Context.Long_c_Notification_Status = TOGGLE_OFF; 
  Custom_App_Context.Switch_c_Notification_Status = TOGGLE_OFF;   
  Custom_App_Context.SW1_Status = 0; 
  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* My_P2P_Server */
__USED void Custom_Switch_c_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Switch_c_UC_1*/

  /* USER CODE END Switch_c_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SWITCH_C, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Switch_c_UC_Last*/

  /* USER CODE END Switch_c_UC_Last*/
  return;
}

void Custom_Switch_c_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Switch_c_NS_1*/
  if (Custom_App_Context.Switch_c_Notification_Status == TOGGLE_ON)
  {
    updateflag = 1;
    
    if (Custom_App_Context.SW1_Status == 0)
    {
      Custom_App_Context.SW1_Status = 1;
      NotifyCharData[0] = 0x00;
      NotifyCharData[1] = 0x01;
    }
    else
    {
      Custom_App_Context.SW1_Status = 0;
      NotifyCharData[0] = 0x00;
      NotifyCharData[1] = 0x00;
    }

    APP_DBG_MSG("-- CUSTOM APPLICATION SERVER  : INFORM CLIENT BUTTON 1 PUSHED \n");
  }
  else
  {
    APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n");
  }     
  /* USER CODE END Switch_c_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SWITCH_C, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Switch_c_NS_Last*/

  /* USER CODE END Switch_c_NS_Last*/

  return;
}

/* My_Heart_Rate */
__USED void Custom_Hrs_m_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Hrs_m_UC_1*/

  /* USER CODE END Hrs_m_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_HRS_M, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Hrs_m_UC_Last*/

  /* USER CODE END Hrs_m_UC_Last*/
  return;
}

void Custom_Hrs_m_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Hrs_m_NS_1*/

  /* USER CODE END Hrs_m_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_HRS_M, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Hrs_m_NS_Last*/

  /* USER CODE END Hrs_m_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

void SW1_Button_Action(void)
{
  UTIL_SEQ_SetTask( 1<<CFG_TASK_SW1_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);

  return;
}

/* USER CODE END FD_LOCAL_FUNCTIONS*/
