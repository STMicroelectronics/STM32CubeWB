/**
 ******************************************************************************
 * @file    dt_server_app.c
 * @author  MCD Application Team
 * @brief   data throughput server Application
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


/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "dbg_trace.h"

#include "ble.h"
#include "app_ble.h"

#include "dt_server_app.h"
#include "dt_client_app.h"
#include "dts.h"

#include "scheduler.h"
#include "lpm.h"

#include "ble_common.h"
#include "svc_ctl.h"

typedef enum
{
  DT_FLOW_OFF,
  DT_FLOW_ON
} DT_Flow_Status_t;

typedef struct
{
DTS_STM_Payload_t TxData;
DTS_STM_Payload_t RxData;
uint8_t TimerDataTransfer_Id;
uint8_t NotificationTransferFlag;
uint8_t StartTransferFlag;
DT_Flow_Status_t DtFlowStatus;
} DT_Server_App_Context_t;

DT_ClientContext_t aDTClientContext[BLE_CFG_CLT_MAX_NBR_CB];
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DT_Server_App_Context_t DataTransferServerContext;
uint32_t Notification_Data_Buffer[62];

#define DEFAULT_TS_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */
#define DATA_STOP                         (4*DEFAULT_TS_MEASUREMENT_INTERVAL)

/* Global variables ----------------------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
static void DT_App_Button_Trigger_Received( void );
static void BLE_App_dts_Send_Data( void );

extern DataTransferContext_t aDataTransferClientContext[MAX_CONNECTION];
/* Private function prototypes -----------------------------------------------*/

void DTS_Notification( DTS_STM_App_Notification_evt_t *pNotification )
{
  switch (pNotification->Evt_Opcode)
  {
    case DTS_STM__NOTIFICATION_ENABLED:
      DataTransferServerContext.NotificationTransferFlag = 0x01;
      if (BUTTON_SERVER == 0)
      {
        APP_DBG_MSG("enable notification appli\n");
        DataTransferServerContext.StartTransferFlag = 1;
        SCH_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
      }
      break;

    case DTS_STM_NOTIFICATION_DISABLED:
      if (BUTTON_SERVER == 0)
      {
        APP_DBG_MSG("disable notification appli\n");
        DataTransferServerContext.StartTransferFlag = 0;
      }
      DataTransferServerContext.NotificationTransferFlag = 0x00;
      break;

    default:
      break;
  }

  return;
}

void BLE_App_dts_Send_Data( void )
{
  tBleStatus status = BLE_STATUS_INVALID_PARAMS;
  uint8_t CRC_Result;
  uint32_t mask_byte_low = 0xFFFFFF00;


  if (DataTransferServerContext.StartTransferFlag)
  {   
    /*Data Packet to send to remote*/
      Notification_Data_Buffer[0] += 1;
      /* compute CRC */
      CRC_Result = APP_BLE_ComputeCRC8((uint8_t*) Notification_Data_Buffer, (DATA_NOTIFICATION_MAX_PACKET_SIZE - 4));
      Notification_Data_Buffer[59] &= mask_byte_low;
      Notification_Data_Buffer[59] |= (uint32_t) CRC_Result;
      
      DataTransferServerContext.TxData.pPayload = Notification_Data_Buffer;
      DataTransferServerContext.TxData.Length = DATA_NOTIFICATION_MAX_PACKET_SIZE; /* DATA_NOTIFICATION_MAX_PACKET_SIZE */      
      
    if(DataTransferServerContext.DtFlowStatus == DT_FLOW_ON)
    {      
      status = DTS_STM_UpdateChar(DATA_TRANSFER_TX_CHAR_UUID, (uint8_t *) &DataTransferServerContext.TxData);
      if (status == BLE_STATUS_INSUFFICIENT_RESOURCES)
      {
        DataTransferServerContext.DtFlowStatus = DT_FLOW_OFF;
        (Notification_Data_Buffer[0])-=1;
      }
    }
    else 
    {
      (Notification_Data_Buffer[0])-=1;
    }
    
    SCH_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
  }
  return;
}

void Resume_Notification(void)
{
  DataTransferServerContext.DtFlowStatus = DT_FLOW_ON;
}

void DT_App_Button_Trigger_Received( void )
{
  
  if (BUTTON_SERVER == 0)
  {
    if (aDTClientContext[0].NotificationStatus == 0)
    {
      aDataTransferClientContext[0].state = DTC_ENABLE_NOTIFICATION_TX_DESC;
      SCH_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0);
      aDTClientContext[0].NotificationStatus = 1;
    }
    else
    {
      aDataTransferClientContext[0].state = DTC_DISABLE_NOTIFICATION_TX_DESC;
      SCH_SetTask(1 << CFG_IdleTask_DataTransfer_ClientDiscovery, CFG_SCH_PRIO_0); 
      aDTClientContext[0].NotificationStatus = 0;
    }
  }
  else
  {
      if(DataTransferServerContext.StartTransferFlag == 0)
      {
        if (DataTransferServerContext.NotificationTransferFlag == 1)
        {
          BSP_LED_On(LED_BLUE);
          Delay_StartDataReq();
        }
      } 
      else
      {
        BSP_LED_Off(LED_BLUE);
        DataTransferServerContext.StartTransferFlag = 0; 
      }
  }
  
  return;
}

void Delay_StartDataReq( void)
{
  DataTransferServerContext.StartTransferFlag = 1;
  SCH_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0); 
  
  return;
}

void DT_Server_App_Key_Button_Action( void )
{
  SCH_SetTask(1 << CFG_TASK_BUTTON_ID, CFG_SCH_PRIO_0);
}

void DT_Server_App_Init( void )
{
  uint8_t i;
  
  SCH_RegTask(CFG_TASK_BUTTON_ID, DT_App_Button_Trigger_Received);
  SCH_RegTask(CFG_TASK_DATA_TRANSFER_UPDATE_ID, BLE_App_dts_Send_Data);

  /**
   * Initialize data buffer
   */
  for (i=0;i<62;i++)
  {
    Notification_Data_Buffer[i] = (uint32_t)i;
  }
}

void SVCCTL_InitCustomSvc( void )
{
  DTS_STM_Init();
  DataTransferServerContext.NotificationTransferFlag = 0x00;
  DataTransferServerContext.StartTransferFlag = 0x00;
  DataTransferServerContext.DtFlowStatus = DT_FLOW_ON;
  
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
