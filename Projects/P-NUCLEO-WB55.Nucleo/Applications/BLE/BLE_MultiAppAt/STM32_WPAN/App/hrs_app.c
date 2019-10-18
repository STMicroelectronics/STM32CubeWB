/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hrs_app.c
  * @author  MCD Application Team
  * @brief   Heart Rate Service Application
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license 
  * SLA0044, the "License"; You may not use this file except in compliance with 
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_common.h"

#include "ble.h"
#include "hrs_app.h"
#include "stm32_seq.h"
#include "uart_app.h"

extern APP_Mode_t Next_Mode;

extern HR_Notify_Context_t HR_Notify_Context;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  HRS_BodySensorLocation_t BodySensorLocationChar;
  HRS_MeasVal_t MeasurementvalueChar;
  uint8_t ResetEnergyExpended;
  uint8_t TimerMeasurement_Id;
  uint8_t Notifications_Status;
} HRSAPP_Context_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define HRSAPP_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static HRSAPP_Context_t HRSAPP_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private functions prototypes-----------------------------------------------*/
static void HRSAPP_Measurement(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void HRS_Notification(HRS_App_Notification_evt_t *pNotification)
{
/* USER CODE BEGIN HRS_Notification_1 */

/* USER CODE END HRS_Notification_1 */
  switch(pNotification->HRS_Evt_Opcode)
  {
/* USER CODE BEGIN HRS_Notification_HRS_Evt_Opcode */

/* USER CODE END HRS_Notification_HRS_Evt_Opcode */
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
    case HRS_RESET_ENERGY_EXPENDED_EVT:
/* USER CODE BEGIN HRS_RESET_ENERGY_EXPENDED_EVT */
      HRSAPP_Context.MeasurementvalueChar.EnergyExpended = 0;
      HRSAPP_Context.ResetEnergyExpended = 1;
      UART_App_SendData("\r\nR=01\r\n", 8);
/* USER CODE END HRS_RESET_ENERGY_EXPENDED_EVT */
      break;
#endif
      
    case HRS_NOTIFICATION_ENABLED:
/* USER CODE BEGIN HRS_NOTIFICATION_ENABLED */
      
      HRSAPP_Context.Notifications_Status = 1;
      UART_App_SendData("\r\nN=1\r\n", 7);
/* USER CODE END HRS_NOTIFICATION_ENABLED */
      break;

    case HRS_NOTIFICATION_DISABLED:
/* USER CODE BEGIN HRS_NOTIFICATION_DISABLED */
       HRSAPP_Context.Notifications_Status = 0;
      UART_App_SendData("\r\nN=0\r\n", 7);
/* USER CODE END HRS_NOTIFICATION_DISABLED */
      break;
      
   default:
/* USER CODE BEGIN HRS_Notification_Default */

/* USER CODE END HRS_Notification_Default */
      break;
  }
/* USER CODE BEGIN HRS_Notification_2 */

/* USER CODE END HRS_Notification_2 */
  return;
}

void HRS_APP_Notification(HRS_APP_ConnHandle_Not_evt_t *pNotification)
{
  switch(pNotification->HR_Evt_Opcode)
  {
    case HR_CONN_HANDLE_EVT :
      UART_App_SendData("\r\nC\r\n", 5);
      break;

    case HR_DISCON_HANDLE_EVT :
      UART_App_SendData("\r\nD\r\n", 5);
      break;
    
    case HR_DISCON_SWITCH_HANDLE_EVT :
      switch (Next_Mode) {
        case P2P_CLIENT :
          *(volatile uint32_t*) SRAM1_BASE = SRAM1_BASE_P2P_CLIENT;
          break;
        case P2P_SERVER :
          *(volatile uint32_t*) SRAM1_BASE = SRAM1_BASE_P2P_SERVER;
          break;
        default : break;
      }
      NVIC_SystemReset();
      break;
    
    default:
      break;
  }
  return;
}

void HRSAPP_Init(void)
{
  UTIL_SEQ_RegTask( 1<< CFG_TASK_MEAS_REQ_ID, UTIL_SEQ_RFU, HRSAPP_Measurement );
/* USER CODE BEGIN HRSAPP_Init */
  
  HRSAPP_Context.Notifications_Status = 0;
  /**
   * Set Body Sensor Location
   */
  HRSAPP_Context.ResetEnergyExpended = 0;
  HRSAPP_Context.BodySensorLocationChar = HRS_BODY_SENSOR_LOCATION_HAND;
  HRS_UpdateChar(SENSOR_LOCATION_UUID, (uint8_t *)&HRSAPP_Context.BodySensorLocationChar);


  /**
   * Set Flags for measurement value
   */

  HRSAPP_Context.MeasurementvalueChar.Flags = ( HRS_HRM_VALUE_FORMAT_UINT16      | 
                                                  HRS_HRM_SENSOR_CONTACTS_PRESENT   | 
                                                  HRS_HRM_SENSOR_CONTACTS_SUPPORTED |
                                                  HRS_HRM_ENERGY_EXPENDED_PRESENT  |
                                                  HRS_HRM_RR_INTERVAL_PRESENT );

#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
  if(HRSAPP_Context.MeasurementvalueChar.Flags & HRS_HRM_ENERGY_EXPENDED_PRESENT)
    HRSAPP_Context.MeasurementvalueChar.EnergyExpended = 10;
#endif
  
#if (BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG != 0)
  if(HRSAPP_Context.MeasurementvalueChar.Flags & HRS_HRM_RR_INTERVAL_PRESENT)
  {
    uint8_t i;
    
    HRSAPP_Context.MeasurementvalueChar.NbreOfValidRRIntervalValues = BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG;
    for(i = 0; i < BLE_CFG_HRS_ENERGY_RR_INTERVAL_FLAG; i++)
      HRSAPP_Context.MeasurementvalueChar.aRRIntervalValues[i] = 1024;
  }
#endif

/* USER CODE END HRSAPP_Init */
  return;
}

static void HRSAPP_Measurement(void)
{
/* USER CODE BEGIN HRSAPP_Measurement */
  if(HRSAPP_Context.Notifications_Status == 1)
  {

    HRSAPP_Context.MeasurementvalueChar.MeasurementValue = HR_Notify_Context.Measurement;
#if (BLE_CFG_HRS_ENERGY_EXPENDED_INFO_FLAG != 0)
    if((HRSAPP_Context.MeasurementvalueChar.Flags & HRS_HRM_ENERGY_EXPENDED_PRESENT) &&
      (HRSAPP_Context.ResetEnergyExpended == 0))
      HRSAPP_Context.MeasurementvalueChar.EnergyExpended = HR_Notify_Context.EnergyExpended;
    else if(HRSAPP_Context.ResetEnergyExpended == 1)
      HRSAPP_Context.ResetEnergyExpended = 0;
#endif

    HRS_UpdateChar(HEART_RATE_MEASURMENT_UUID, (uint8_t *)&HRSAPP_Context.MeasurementvalueChar);
    
    UART_App_SendData("\r\nOK\r\n", 6);
  }
  else UART_App_SendData("\r\nF\r\n", 5);

/* USER CODE END HRSAPP_Measurement */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
