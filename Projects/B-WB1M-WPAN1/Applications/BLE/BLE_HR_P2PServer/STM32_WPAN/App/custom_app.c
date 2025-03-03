/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
  uint8_t                               SW1_Status;
  uint8_t                               TimerMeasurement_Id;
  uint8_t                               ResetEnergyExpended;
  Custom_STM_HRS_BodySensorLocation_t   BodySensorLocationChar;
  Custom_STM_HRS_MeasVal_t              MeasurementvalueChar;
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
#define HRSAPP_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */
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
static void HrMeas(void);
static uint32_t HRSAPP_Read_RTC_SSR_SS (void);
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */
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
      if(pNotification->DataTransfered.pPayload[1] == 0x01)
      {
        BSP_LED_On(LED_BLUE);
      }
      if(pNotification->DataTransfered.pPayload[1] == 0x00)
      {
        BSP_LED_Off(LED_BLUE); 
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

      Custom_App_Context.Switch_c_Notification_Status = TOGGLE_OFF;       /* My_Switch_Char notification status has been disabled */
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
      APP_DBG_MSG("\r\n\r** CUSTOM_STM_HRS_M_NOTIFY_ENABLED_EVT \n");
      Custom_App_Context.Hrs_m_Notification_Status = TOGGLE_ON;           /* My_HRS_Meas notification status has been enabled */
      /**
       * It could be the enable notification is received twice without the disable notification in between
       */
      HW_TS_Stop(Custom_App_Context.TimerMeasurement_Id);
      HW_TS_Start(Custom_App_Context.TimerMeasurement_Id, HRSAPP_MEASUREMENT_INTERVAL);       
      /* USER CODE END CUSTOM_STM_HRS_M_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT */
      APP_DBG_MSG("\r\n\r** CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT \n");
      Custom_App_Context.Hrs_m_Notification_Status = TOGGLE_OFF;           /* My_HRS_Meas notification status has been disabled */
      HW_TS_Stop(Custom_App_Context.TimerMeasurement_Id);
      /* USER CODE END CUSTOM_STM_HRS_M_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_HRS_SL_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HRS_SL_READ_EVT */

      /* USER CODE END CUSTOM_STM_HRS_SL_READ_EVT */
      break;

    case CUSTOM_STM_HRS_CTRL_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_HRS_CTRL_WRITE_EVT */
      PRINT_MESG_DBG("ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE My_HRS_CTRL_Point Write\n");

      /* reset energy expended */
      Custom_App_Context.MeasurementvalueChar.EnergyExpended = 0;
      Custom_App_Context.ResetEnergyExpended = 1;
      /* USER CODE END CUSTOM_STM_HRS_CTRL_WRITE_EVT */
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
  Custom_App_Context.ResetEnergyExpended = 0;
  Custom_App_Context.BodySensorLocationChar = CUSTOM_STM_HRS_BODY_SENSOR_LOCATION_WRIST;

  Custom_STM_App_Update_Char(CUSTOM_STM_HRS_SL, (uint8_t *) &Custom_App_Context.BodySensorLocationChar);       /*initialise My_Sensor_Loc char to wrist */
  
  Custom_Switch_c_Update_Char();
  Custom_Hrs_m_Update_Char();

  UTIL_SEQ_RegTask(1<< CFG_TASK_SW1_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, Custom_Switch_c_Send_Notification);
  UTIL_SEQ_RegTask(1<< CFG_TASK_MEAS_REQ_ID, UTIL_SEQ_RFU, Custom_Hrs_m_Send_Notification);
  
  Custom_App_Context.Switch_c_Notification_Status = TOGGLE_OFF;   
  Custom_App_Context.SW1_Status = 0;

  /**
   * Set Flags for measurement value
   */

  Custom_App_Context.MeasurementvalueChar.Flags = ( CUSTOM_STM_HRS_HRM_VALUE_FORMAT_UINT16       |
                                                    CUSTOM_STM_HRS_HRM_SENSOR_CONTACTS_PRESENT   |
                                                    CUSTOM_STM_HRS_HRM_SENSOR_CONTACTS_SUPPORTED |
                                                    CUSTOM_STM_HRS_HRM_ENERGY_EXPENDED_PRESENT   |
                                                    CUSTOM_STM_HRS_HRM_RR_INTERVAL_PRESENT );

#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  if(Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_ENERGY_EXPENDED_PRESENT)
    Custom_App_Context.MeasurementvalueChar.EnergyExpended = 10;
#endif
  
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
  if(Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_RR_INTERVAL_PRESENT)
  {
    uint8_t i;
    
    Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues = BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG;
    for(i = 0; i < BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG; i++)
      Custom_App_Context.MeasurementvalueChar.aRRIntervalValues[i] = 1024;
  }
#endif  
  /**
   * Create timer for Heart Rate Measurement
   */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR, &(Custom_App_Context.TimerMeasurement_Id), hw_ts_Repeated, HrMeas);
  
  
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
  uint32_t measurement;
  uint8_t index = 0;
  
  if (Custom_App_Context.Hrs_m_Notification_Status == TOGGLE_ON)
  {
    updateflag = 1;

    measurement = ((HRSAPP_Read_RTC_SSR_SS()) & 0x07) + 65;
  
    Custom_App_Context.MeasurementvalueChar.MeasurementValue = measurement;
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  if((Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_ENERGY_EXPENDED_PRESENT) &&
     (Custom_App_Context.ResetEnergyExpended == 0))
    Custom_App_Context.MeasurementvalueChar.EnergyExpended += 5;
  else if(Custom_App_Context.ResetEnergyExpended == 1)
    Custom_App_Context.ResetEnergyExpended = 0;
#endif    

  /**
   * Flags update
   */  
  NotifyCharData[index] = (uint8_t)Custom_App_Context.MeasurementvalueChar.Flags;
  index++;
  /**
   *  Heart Rate Measurement Value
   */
  if(Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_VALUE_FORMAT_UINT16)
  {
    NotifyCharData[index] = (uint8_t)(Custom_App_Context.MeasurementvalueChar.MeasurementValue & 0xFF);
    index++;
    NotifyCharData[index] = (uint8_t)(Custom_App_Context.MeasurementvalueChar.MeasurementValue >> 8);
    index++;
  }
  else
  {
    NotifyCharData[index] = (uint8_t)Custom_App_Context.MeasurementvalueChar.MeasurementValue;
    index++;
  }

#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  /**
   *  Energy Expended
   */
  if(Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_ENERGY_EXPENDED_PRESENT)
  {
    NotifyCharData[index] = (uint8_t)(Custom_App_Context.MeasurementvalueChar.EnergyExpended & 0xFF);
    index++;
    NotifyCharData[index] = (uint8_t)(Custom_App_Context.MeasurementvalueChar.EnergyExpended >> 8);
    index++;
  }
#endif

#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
  /**
   *  RR Interval Values
   */
  if (Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_RR_INTERVAL_PRESENT)
  {
    uint8_t i;
    uint8_t rr_interval_number;

    if(Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_VALUE_FORMAT_UINT16)
    {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
      if (Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_ENERGY_EXPENDED_PRESENT)
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-2))
        /**
         * When the HRM is on 16bits and the Energy expended info is present
         * The maximum RR values is 7
         */
        if(Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-2))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-2);
        }
        else
#endif
        {
          rr_interval_number = Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues;
        }
      }
      else
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        /**
         * When the HRM is on 16bits and the Energy expended info is not present
         * The maximum RR values is 8
         */
        if(Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
        }
        else
#endif
        {
          rr_interval_number = Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues;
        }
      }
#else /**< (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0) */
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
      /**
       * When the HRM is on 16bits and the Energy expended info is not present
       * The maximum RR values is 8
       */
      if(Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
      {
        rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
      }
      else
#endif
      {
        rr_interval_number = Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues;
      }
#endif
    }
    else
    {
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
      if (Custom_App_Context.MeasurementvalueChar.Flags & CUSTOM_STM_HRS_HRM_ENERGY_EXPENDED_PRESENT)
      {
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        /**
         * When the HRM is on 8bits and the Energy expended info is present
         * The maximum RR values is 8
         */
        if(Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues > (HRS_MAX_NBR_RR_INTERVAL_VALUES-1))
        {
          rr_interval_number = (HRS_MAX_NBR_RR_INTERVAL_VALUES-1);
        }
        else
#endif
        {
          rr_interval_number = Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues;
        }
      }
      else
      {
        rr_interval_number = Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues;
      }
#else /**< (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0) */
      rr_interval_number = Custom_App_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues;
#endif
    }

    for ( i = 0 ; i < rr_interval_number ; i++ )
    {
      NotifyCharData[index] = (uint8_t)(Custom_App_Context.MeasurementvalueChar.aRRIntervalValues[i] & 0xFF);
      index++;
      NotifyCharData[index] = (uint8_t)(Custom_App_Context.MeasurementvalueChar.aRRIntervalValues[i] >> 8);
      index++;
    }
  }

#endif  /**< (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0) */  

    APP_DBG_MSG("-- CUSTOM APPLICATION SERVER  : INFORM CLIENT HEARTRATE MEASUREMENT \n");
  }
  else
  {
    APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n");
  }  
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

static void HrMeas(void)
{
  /**
   * The code shall be executed in the background as aci command may be sent
   * The background is the only place where the application can make sure a new aci command
   * is not sent if there is a pending one
   */
  UTIL_SEQ_SetTask(1<<CFG_TASK_MEAS_REQ_ID, CFG_SCH_PRIO_0);

  return;
}

static uint32_t HRSAPP_Read_RTC_SSR_SS(void)
{
  return ((uint32_t)(READ_BIT(RTC->SSR, RTC_SSR_SS)));
}

void P2PS_APP_SW1_Button_Action(void)
{
  UTIL_SEQ_SetTask(1<<CFG_TASK_SW1_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);

  return;
}
/* USER CODE END FD_LOCAL_FUNCTIONS*/
