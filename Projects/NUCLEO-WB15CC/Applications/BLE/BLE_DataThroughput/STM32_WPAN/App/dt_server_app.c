/**
  ******************************************************************************
 * @file    dt_server_app.c
 * @author  MCD Application Team
 * @brief   data throughput server Application
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

#include "ble.h"
#include "app_ble.h"

#include "dt_server_app.h"
#include "dt_client_app.h"
#include "dts.h"

#include "stm32_seq.h"
#include "stm32_lpm.h"

#include "ble_common.h"
#include "ble_clock.h"

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
DTS_App_Context_t DataTransferServerContext;
static uint8_t Notification_Data_Buffer[DATA_NOTIFICATION_MAX_PACKET_SIZE]; /* DATA_NOTIFICATION_MAX_PACKET_SIZE data + CRC */
uint32_t DataReceived;

/* Global variables ----------------------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
static void ButtonTriggerReceived(void);
static void DT_App_Button2_Trigger_Received( void );
static void DT_App_Button3_Trigger_Received( void );
static void SendData(void);
static void BLE_App_Delay_DataThroughput( void );
extern uint16_t Att_Mtu_Exchanged;
extern uint8_t TimerDataThroughputWrite_Id;

#define DEFAULT_TS_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */
#define DELAY_1s  (1*DEFAULT_TS_MEASUREMENT_INTERVAL)
#define TIMEUNIT  1

#define BOUNCE_THRESHOLD                20U
#define LONG_PRESS_THRESHOLD            1000U
/*************************************************************
 *
 * PUBLIC FUNCTIONS
 *
 *************************************************************/
void DTS_App_Init(void)
{
  uint8_t i;

  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_ID, UTIL_SEQ_RFU, ButtonTriggerReceived);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW2_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DT_App_Button2_Trigger_Received);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW3_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DT_App_Button3_Trigger_Received);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_TRANSFER_UPDATE_ID, UTIL_SEQ_RFU, SendData);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_WRITE_ID, UTIL_SEQ_RFU, BLE_App_Delay_DataThroughput);

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

void DTS_App_KeyButton2Action( void )
{
    UTIL_SEQ_SetTask(1 << CFG_TASK_SW2_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
}

void DTS_App_KeyButton3Action( void )
{
    UTIL_SEQ_SetTask(1 << CFG_TASK_SW3_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
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
      
    case DTC_NOTIFICATION_ENABLED:
      BLE_SVC_L2CAP_Conn_Update_7_5();
      //DataTransferServerContext.NotificationClientTransferFlag = 0x01;
      break;
      
    case DTC_NOTIFICATION_DISABLED:
      //DataTransferServerContext.NotificationClientTransferFlag = 0x00;
      APP_DBG_MSG("write data notification disabled \n");
      break;
      
    case DTS_STM_DATA_RECEIVED:
      if (DataReceived == 0)
      {
        /* start timer */
        DataReceived += pNotification->DataTransfered.Length;
        HW_TS_Start(TimerDataThroughputWrite_Id, DELAY_1s);
      }
      else
      {
        DataReceived += pNotification->DataTransfered.Length;
      }
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
    //DataTransferServerContext.TxData.Length = DATA_NOTIFICATION_MAX_PACKET_SIZE; /* DATA_NOTIFICATION_MAX_PACKET_SIZE */
    DataTransferServerContext.TxData.Length =  DATA_NOTIFICATION_MAX_PACKET_SIZE; //Att_Mtu_Exchanged-10;

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
void Resume_Notification(void)
{
  DataTransferServerContext.DtFlowStatus = DTS_APP_FLOW_ON;
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

static void DT_App_Button2_Trigger_Received( void )
{
  APP_DBG_MSG("**CHANGE PHY \n");
  BLE_SVC_GAP_Change_PHY();
  return;
}

static void Appli_UpdateButtonState(int isPressed)
{
  uint32_t t0 = 0,t1 = 1;

  t0 = Clock_Time(); /* SW3 press timing */
  
  while(BSP_PB_GetState(BUTTON_SW3) == BUTTON_PRESSED);
  t1 = Clock_Time(); /* SW3 release timing */
  
  if((t1 - t0) > LONG_PRESS_THRESHOLD)
  {
    /* Button 3 long press action */
    APP_DBG_MSG("clear database \n");
    BLE_SVC_GAP_Clear_DataBase();
  }
  else if((t1 - t0) > BOUNCE_THRESHOLD)
  {
    /* Button 3 short press action */
    APP_DBG_MSG("slave security request \n");
    BLE_SVC_GAP_Security_Req();
  }
}

static void DT_App_Button3_Trigger_Received(void)
{
  Appli_UpdateButtonState(BSP_PB_GetState(BUTTON_SW3) == BUTTON_PRESSED);
  APP_DBG_MSG("SW3 \n");
  return;
}

static void BLE_App_Delay_DataThroughput(void)
{
  uint32_t DataThroughput;
  DTS_STM_Payload_t ThroughputToSend; 
  
  DataThroughput = (uint32_t)(DataReceived/TIMEUNIT);
  APP_DBG_MSG("DataThroughput = %ld  bytes/s\n", DataThroughput);
  
  ThroughputToSend.Length = 4;
  ThroughputToSend.pPayload = (uint8_t*)&DataThroughput;
  
  DTS_STM_UpdateCharThroughput( (DTS_STM_Payload_t*) &ThroughputToSend);  
  DataReceived = 0;
}
