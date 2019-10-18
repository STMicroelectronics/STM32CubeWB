/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_ble_common.c
 * @author  MCD Application Team
 * @brief   BLE Application
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
#include "main.h"

#include "app_common.h"

#include "ble.h"
#include "tl.h"
#include "app_ble_common.h"
#include "app_ble_sv.h"
#include "app_ble_hr.h"
#include "app_ble_cl.h"

#include "stm32_seq.h"
#include "shci.h"
#include "stm32_lpm.h"
#include "otp.h"

extern APP_Mode_t APP_MODE;
/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void APP_BLE_Common_Init( void );

/* Functions Definition ------------------------------------------------------*/
void APP_BLE_Common_Init( void )
{

  return;
}

SVCCTL_UserEvtFlowStatus_t SVCCTL_App_Notification( void *pckt )
{
  switch(APP_MODE){
    case P2P_SERVER :
      SVCCTL_App_SV_Notification(pckt);
      break;
    case P2P_CLIENT :
      SVCCTL_App_CL_Notification(pckt);
      break;
    case HEART_RATE :
      SVCCTL_App_HR_Notification(pckt);
      break;
    default : break;
  }

  return (SVCCTL_UserEvtFlowEnable);
}

void SVCCTL_SvcInit(void)
{
  if(APP_MODE == HEART_RATE) DIS_Init();

  if(APP_MODE == HEART_RATE) HRS_Init();

  if(APP_MODE == P2P_SERVER || APP_MODE == P2P_CLIENT) P2PS_STM_Init();
  
  return;
}

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void hci_notify_asynch_evt(void* pdata)
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_HCI_ASYNCH_EVT_ID, CFG_SCH_PRIO_0);
  return;
}

void hci_cmd_resp_release(uint32_t flag)
{
  UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
  return;
}

void hci_cmd_resp_wait(uint32_t timeout)
{
  UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID);
  return;
}

void SVCCTL_ResumeUserEventFlow( void )
{
  hci_resume_flow();
  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
