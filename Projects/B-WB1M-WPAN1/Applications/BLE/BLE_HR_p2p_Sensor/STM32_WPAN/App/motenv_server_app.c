/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : motenv_server_app.c
  * Description        : MOTENV Server Application
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
/* USER CODE END Header */
/* USER CODE BEGIN UserCode */
/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "motenv_server_app.h"
#include "stm32_seq.h"

#include "custom_stm.h"
#include "env_server_app.h"
#include "motion_server_app.h"

#include "b_wb1m_wpan1.h"
#include "b_wb1m_wpan1_bus.h"
#include "b_wb1m_wpan1_env_sensors.h"
   
#include "custom_stm.h"
    
/* Private defines -----------------------------------------------------------*/
#define ENVIRONMENT_UPDATE_PERIOD       (uint32_t)(0.5*1000*1000/CFG_TS_TICK_VAL) /*500ms*/
#define ACC_GYRO_MAG_UPDATE_PERIOD      (uint32_t)(0.05*1000*1000/CFG_TS_TICK_VAL) /*50ms (20Hz)*/

/* Private typedef -----------------------------------------------------------*/

/**
 * @brief  MOTENV Server Context structure definition
 *         Include just the Timer Ids for the Notifications
 */
typedef struct
{
  uint8_t AccGyroMag_Update_Timer_Id;
  uint8_t Env_Update_Timer_Id;
} MOTENV_Server_App_Context_t;

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_APP_CONTEXT
 */
static MOTENV_Server_App_Context_t MOTENV_Server_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */
/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void MOTENV_AccGyroMagUpdate_Timer_Callback(void);
static void MOTENV_EnvUpdate_Timer_Callback(void);
static void MOTENV_APP_context_Init(void);

/* Functions Definition ------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Handle the request from the GATT Client
 *         (e.g., on notification enabling/disabling request, start/stop the timer)
 * @param  pNotification: Request data coming from the GATT Client
 * @retval None
 */
void MOTENV_STM_App_Notification(MOTENV_STM_App_Notification_evt_t *pNotification)
{
  switch(pNotification->Motenv_Evt_Opcode)
  {
    /*
     * Env char notification enabled
     */
    case HW_ENV_NOTIFY_ENABLED_EVT:
      ENV_Set_Notification_Status(1);
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : ENV NOTIFICATION ENABLED\n");
      APP_DBG_MSG(" \n\r");
      /* Start the timer used to update the Env characteristic */
      HW_TS_Start(MOTENV_Server_App_Context.Env_Update_Timer_Id, ENVIRONMENT_UPDATE_PERIOD);
      break; /* HW_ENV_NOTIFY_ENABLED_EVT */

    /*
     * Motion char notification enabled
     */
    case HW_MOTION_NOTIFY_ENABLED_EVT:

      MOTION_Set_Notification_Status(1);
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : MOTION NOTIFICATION ENABLED\n");
      APP_DBG_MSG(" \n\r");
      /* Start the timer used to update the AccGyroMag characteristic */
      HW_TS_Start(MOTENV_Server_App_Context.AccGyroMag_Update_Timer_Id, ACC_GYRO_MAG_UPDATE_PERIOD);
      break; /* HW_MOTION_NOTIFY_ENABLED_EVT */

    /*
     * Env char notification disabled
     */
    case HW_ENV_NOTIFY_DISABLED_EVT:
      ENV_Set_Notification_Status(0);
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : ENV NOTIFICATION DISABLED\n");
      APP_DBG_MSG(" \n\r");
      /* Stop the timer used to update the Env characteristic */
      HW_TS_Stop(MOTENV_Server_App_Context.Env_Update_Timer_Id);
      break; /* HW_ENV_NOTIFY_DISABLED_EVT */

    /*
     * Motion char notification disabled
     */
    case HW_MOTION_NOTIFY_DISABLED_EVT:
      MOTION_Set_Notification_Status(0);
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : MOTION NOTIFICATION DISABLED\n");
      APP_DBG_MSG(" \n\r");

      /* Stop the timer used to update the Motion characteristic */
      HW_TS_Stop(MOTENV_Server_App_Context.AccGyroMag_Update_Timer_Id);
      break; /* HW_ENV_NOTIFY_DISABLED_EVT */

    /*
     * Env char read request
     */
    case HW_ENV_READ_EVT:
      ENV_Update();
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : ENV READ\n");
      APP_DBG_MSG(" \n\r");
      break; /* HW_ENV_READ_EVT */
      
    default:
      break; /* DEFAULT */
  }

  return;
}

/**
 * @brief  Handle disconnection (Stop all timers)
 * @param  None
 * @retval None
 */
void MOTENV_APP_HandleDisconnection( void )
{
  ENV_Set_Notification_Status(0);
  /* Stop the timer used to update the Env characteristic */
  HW_TS_Stop(MOTENV_Server_App_Context.Env_Update_Timer_Id);

  MOTION_Set_Notification_Status(0);
  /* Stop the timer used to update the Motion characteristic */
  HW_TS_Stop(MOTENV_Server_App_Context.AccGyroMag_Update_Timer_Id);
}

/**
 * @brief  Init the MOTENV APP (Register Tasks, Create Notification timers)
 * @param  None
 * @retval None
 */
void MOTENV_APP_Init(void)
{
  UTIL_SEQ_RegTask( 1<<CFG_TASK_NOTIFY_ACC_GYRO_MAG_ID, UTIL_SEQ_RFU, MOTION_Send_Notification_Task);
  /* Create timer to get the AccGyroMag params and update charecteristic */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR,
        &(MOTENV_Server_App_Context.AccGyroMag_Update_Timer_Id),
        hw_ts_Repeated,
        MOTENV_AccGyroMagUpdate_Timer_Callback);

  UTIL_SEQ_RegTask( 1<<CFG_TASK_NOTIFY_ENVIRONMENT_ID, UTIL_SEQ_RFU, ENV_Send_Notification_Task);
  /* Create timer to change the Environment params and update charecteristic */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR,
        &(MOTENV_Server_App_Context.Env_Update_Timer_Id),
        hw_ts_Repeated,
        MOTENV_EnvUpdate_Timer_Callback);

  /**
   * Initialize MOTENV application context
   */
  MOTENV_APP_context_Init();

  return;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  On timeout, trigger the task
 *         for Motion Char (Acc-Gyro-Mag) notification
 * @param  None
 * @retval None
 */
static void MOTENV_AccGyroMagUpdate_Timer_Callback(void)
{
  UTIL_SEQ_SetTask(1<<CFG_TASK_NOTIFY_ACC_GYRO_MAG_ID, CFG_SCH_PRIO_0);
}

/**
 * @brief  On timeout, trigger the task
 *         for Environmental Char notification
 * @param  None
 * @retval None
 */
static void MOTENV_EnvUpdate_Timer_Callback(void)
{
  UTIL_SEQ_SetTask(1<<CFG_TASK_NOTIFY_ENVIRONMENT_ID, CFG_SCH_PRIO_0);
}

/**
 * @brief  Init Context for each Service exposed by MOTENV Server App
 * @param  None
 * @retval None
 */
static void MOTENV_APP_context_Init(void)
{
  /* Init ENV context */
  ENV_Context_Init();

  /* Init MOTION Context */
  MOTION_Context_Init();

}

/* USER CODE END UserCode */
