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

#include "stm32_seq.h"
#include "stm32_lpm.h"

#include "ble_common.h"

typedef enum
{
  DTS_APP_FLOW_OFF,
  DTS_APP_FLOW_ON
} DTS_App_Flow_Status_t;

typedef enum
{
  DTS_APP_TRANSFER_REQ_OFF,
  DTS_APP_TRANSFER_REQ_ON
} DTS_App_Transfer_Req_Status_t;

typedef struct
{
  DTS_STM_Payload_t TxData;
  DTS_App_Transfer_Req_Status_t NotificationTransferReq;
  DTS_App_Transfer_Req_Status_t ButtonTransferReq;
  DTS_App_Flow_Status_t DtFlowStatus;
} DTS_App_Context_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DTS_App_Context_t DataTransferServerContext;
static uint8_t Notification_Data_Buffer[DATA_NOTIFICATION_MAX_PACKET_SIZE]; /* DATA_NOTIFICATION_MAX_PACKET_SIZE data + CRC */

/* Global variables ----------------------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
static void ButtonTriggerReceived(void);
static void SendData(void);

/*************************************************************
 *
 * PUBLIC FUNCTIONS
 *
 *************************************************************/
void DTS_App_Init(void)
{
  uint8_t i;

  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_ID, UTIL_SEQ_RFU, ButtonTriggerReceived);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_TRANSFER_UPDATE_ID, UTIL_SEQ_RFU, SendData);

  /**
   * Initialize data buffer
   */
  for (i=0 ; i<(DATA_NOTIFICATION_MAX_PACKET_SIZE-1) ; i++)
  {
    Notification_Data_Buffer[i] = i;
  }

  DataTransferServerContext.NotificationTransferReq = DTS_APP_TRANSFER_REQ_OFF;
  DataTransferServerContext.ButtonTransferReq = DTS_APP_TRANSFER_REQ_OFF;
  DataTransferServerContext.DtFlowStatus = DTS_APP_FLOW_ON;
}

void DTS_App_KeyButtonAction( void )
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_BUTTON_ID, CFG_SCH_PRIO_0);
}

void DTS_App_TxPoolAvailableNotification(void)
{
  DataTransferServerContext.DtFlowStatus = DTS_APP_FLOW_ON;
  UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);

  return;
}

/*************************************************************
 *
 * CALLBACK FUNCTIONS
 *
 *************************************************************/
void DTS_Notification( DTS_STM_App_Notification_evt_t *pNotification )
{
  switch (pNotification->Evt_Opcode)
  {
    case DTS_STM__NOTIFICATION_ENABLED:
      DataTransferServerContext.NotificationTransferReq = DTS_APP_TRANSFER_REQ_ON;
      UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
      break;

    case DTS_STM_NOTIFICATION_DISABLED:
      DataTransferServerContext.NotificationTransferReq = DTS_APP_TRANSFER_REQ_OFF;
      break;

    case DTS_STM_GATT_TX_POOL_AVAILABLE:
      DataTransferServerContext.DtFlowStatus = DTS_APP_FLOW_ON;
      UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
      break;

    default:
      break;
  }

  return;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void SendData( void )
{
  tBleStatus status = BLE_STATUS_INVALID_PARAMS;
  uint8_t crc_result;

  if( (DataTransferServerContext.ButtonTransferReq != DTS_APP_TRANSFER_REQ_OFF)
      && (DataTransferServerContext.NotificationTransferReq != DTS_APP_TRANSFER_REQ_OFF)
      && (DataTransferServerContext.DtFlowStatus != DTS_APP_FLOW_OFF) )
  {   
    /*Data Packet to send to remote*/
    Notification_Data_Buffer[0] += 1;
    /* compute CRC */
    crc_result = APP_BLE_ComputeCRC8((uint8_t*) Notification_Data_Buffer, (DATA_NOTIFICATION_MAX_PACKET_SIZE - 1));
    Notification_Data_Buffer[DATA_NOTIFICATION_MAX_PACKET_SIZE - 1] = crc_result;

    DataTransferServerContext.TxData.pPayload = Notification_Data_Buffer;
    DataTransferServerContext.TxData.Length = DATA_NOTIFICATION_MAX_PACKET_SIZE; /* DATA_NOTIFICATION_MAX_PACKET_SIZE */

    status = DTS_STM_UpdateChar(DATA_TRANSFER_TX_CHAR_UUID, (uint8_t *) &DataTransferServerContext.TxData);
    if (status == BLE_STATUS_INSUFFICIENT_RESOURCES)
    {
      DataTransferServerContext.DtFlowStatus = DTS_APP_FLOW_OFF;
      (Notification_Data_Buffer[0])-=1;
    }
    else
    {
      UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
    }
  }
  return;
}

static void ButtonTriggerReceived( void )
{
  if(DataTransferServerContext.ButtonTransferReq != DTS_APP_TRANSFER_REQ_OFF)
  {
    BSP_LED_Off(LED_BLUE);
    DataTransferServerContext.ButtonTransferReq = DTS_APP_TRANSFER_REQ_OFF;
  }
  else
  {
    BSP_LED_On(LED_BLUE);
    DataTransferServerContext.ButtonTransferReq = DTS_APP_TRANSFER_REQ_ON;
    UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
  }

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
