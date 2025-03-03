/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "app_ble.h"


typedef enum
{
  PWR_CO_APP_TRANSFER_REQ_OFF,
  PWR_CO_APP_TRANSFER_REQ_ON
} PWR_CO_App_Transfer_Req_Status_t;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* PwrConso_Service */
  uint8_t               Pwrco_notify_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */


typedef enum
{
  Pwr_co_rx_NOTIFICATION_OFF,
  Pwr_co_rx_NOTIFICATION_ON,
  
  PWR_CO_APP_SENDINFORMATION_LAST
} PWR_CO_APP_SendInformation_t;
/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t D_buffer[USER_PAYLOAD_LENGTH] = {0};
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* PwrConso_Service */
static void Custom_Pwrco_notify_Update_Char(void);
static void Custom_Pwrco_notify_Send_Notification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* PwrConso_Service */
    case CUSTOM_STM_PWRCO_WRITE_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWRCO_WRITE_READ_EVT */

      /* USER CODE END CUSTOM_STM_PWRCO_WRITE_READ_EVT */
      break;

    case CUSTOM_STM_PWRCO_WRITE_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWRCO_WRITE_WRITE_NO_RESP_EVT */

      /* USER CODE END CUSTOM_STM_PWRCO_WRITE_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_PWRCO_NOTIFY_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWRCO_NOTIFY_NOTIFY_ENABLED_EVT */
      Custom_App_Context.Pwrco_notify_Notification_Status = 1;
      APP_DBG_MSG("-- PwrCO APPLICATION Peripheral : NOTIFICATION ENABLED\n\n"); 
      /* USER CODE END CUSTOM_STM_PWRCO_NOTIFY_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_PWRCO_NOTIFY_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_PWRCO_NOTIFY_NOTIFY_DISABLED_EVT */
      Custom_App_Context.Pwrco_notify_Notification_Status = 0;
      APP_DBG_MSG("-- PwrCO APPLICATION Peripheral : NOTIFICATION DISABLED\n\n");

      /* USER CODE END CUSTOM_STM_PWRCO_NOTIFY_NOTIFY_DISABLED_EVT */
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

      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

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
  uint8_t i=0;
  
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SEND_DATA_ID, UTIL_SEQ_RFU, Send_Data);
  Custom_App_Context.Pwrco_notify_Notification_Status = 0;
  
  for (i = 0 ; i < (USER_PAYLOAD_LENGTH) ; i++)
  {
    D_buffer[i] = i;
  }
  
  Custom_Pwrco_notify_Update_Char();
  Custom_Pwrco_notify_Send_Notification();
  
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

/* PwrConso_Service */
__USED void Custom_Pwrco_notify_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pwrco_notify_UC_1*/

  /* USER CODE END Pwrco_notify_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PWRCO_NOTIFY, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Pwrco_notify_UC_Last*/

  /* USER CODE END Pwrco_notify_UC_Last*/
  return;
}

void Custom_Pwrco_notify_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Pwrco_notify_NS_1*/

  /* USER CODE END Pwrco_notify_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PWRCO_NOTIFY, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Pwrco_notify_NS_Last*/

  /* USER CODE END Pwrco_notify_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
void Send_Data_Task(void)
{

  UTIL_SEQ_SetTask( 1<<CFG_TASK_SEND_DATA_ID, CFG_SCH_PRIO_0);
}

void Send_Data(void)
{
  D_buffer[0] += 1;
  
  Custom_STM_Data_t Data_Notify;
  Data_Notify.pPayload = (uint8_t*)D_buffer;
  Data_Notify.Length = USER_PAYLOAD_LENGTH;
  
  if(Custom_App_Context.Pwrco_notify_Notification_Status != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_PWRCO_NOTIFY, Data_Notify.pPayload);
    APP_DBG_MSG("\t Notif sent \n");
  }
}

/* USER CODE END FD_LOCAL_FUNCTIONS*/
