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
/* USER CODE END Header */

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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
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
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define DEFAULT_TS_MEASUREMENT_INTERVAL   (1000000/CFG_TS_TICK_VAL)  /**< 1s */
#define DELAY_1s  (1*DEFAULT_TS_MEASUREMENT_INTERVAL)
#define TIMEUNIT  1

#define BOUNCE_THRESHOLD                20U
#define LONG_PRESS_THRESHOLD            1000U
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

DTS_App_Context_t DTS_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */
static uint8_t Notification_Data_Buffer[DATA_NOTIFICATION_MAX_PACKET_SIZE]; /* DATA_NOTIFICATION_MAX_PACKET_SIZE data + CRC */
extern uint32_t DTS_packet_lost;
extern uint32_t DTS_N;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private functions prototypes-----------------------------------------------*/
static void DTS_App_Button1_Trigger_Received( void );
static void DTS_App_Button2_Trigger_Received( void );
static void DTS_App_Button3_Trigger_Received( void );
static void SendData(void);
static void BLE_App_Delay_DataThroughput( void );
extern uint16_t Att_Mtu_Exchanged;
extern uint8_t TimerDataThroughputWrite_Id;
/* USER CODE BEGIN PFP */
static uint32_t DTS_DataReceived;
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void DTS_App_Init(void)
{
  uint8_t i;

  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW1_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DTS_App_Button1_Trigger_Received);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW2_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DTS_App_Button2_Trigger_Received);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SW3_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, DTS_App_Button3_Trigger_Received);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_TRANSFER_UPDATE_ID, UTIL_SEQ_RFU, SendData);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_PHY_UPDATE_ID, UTIL_SEQ_RFU, BLE_SVC_GAP_Change_PHY);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_WRITE_ID, UTIL_SEQ_RFU, BLE_App_Delay_DataThroughput);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_CONN_INTERV_UPDATE_ID, UTIL_SEQ_RFU, BLE_SVC_L2CAP_Conn_Update_11_25);

  /**
   * Initialize data buffer
   */
  for (i=0 ; i<(DATA_NOTIFICATION_MAX_PACKET_SIZE-1) ; i++)
  {
    Notification_Data_Buffer[i] = i;
  }

  DTS_Context.NotificationTransferReq = DTS_APP_TRANSFER_REQ_OFF;
  DTS_Context.ButtonTransferReq = DTS_APP_TRANSFER_REQ_OFF;
  DTS_Context.DtFlowStatus = DTS_APP_FLOW_ON;
}

void DTS_App_KeyButton1Action( void )
{
  UTIL_SEQ_SetTask(1 << CFG_TASK_SW1_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
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
  DTS_Context.DtFlowStatus = DTS_APP_FLOW_ON;
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
    case DTS_TX_NOTIFICATION_ENABLED:
    {
      DTS_Context.NotificationTransferReq = DTS_APP_TRANSFER_REQ_ON;
      UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
    }
    break;

    case DTS_TX_NOTIFICATION_DISABLED:
    {
      DTS_Context.NotificationTransferReq = DTS_APP_TRANSFER_REQ_OFF;
    }
    break;
      
    case DTS_THROUGHPUT_NOTIFICATION_ENABLED:
    {
      //BLE_SVC_L2CAP_Conn_Update_7_5();
    }
    break;
      
    case DTS_THROUGHPUT_NOTIFICATION_DISABLED:
    {
      APP_DBG_MSG("write data notification disabled \n");
    }
    break;
      
    case DTS_DATA_RECEIVED:
    {
      if (DTS_DataReceived == 0)
      {
        /* start timer */
        DTS_DataReceived += pNotification->DataTransfered.Length;
        HW_TS_Start(TimerDataThroughputWrite_Id, DELAY_1s);
      }
      else
      {
        DTS_DataReceived += pNotification->DataTransfered.Length;
      }
    }
    break;

    case DTS_STM_GATT_TX_POOL_AVAILABLE:
    {
      DTS_Context.DtFlowStatus = DTS_APP_FLOW_ON;
      UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
    }
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

  if( (DTS_Context.ButtonTransferReq != DTS_APP_TRANSFER_REQ_OFF)
      && (DTS_Context.NotificationTransferReq != DTS_APP_TRANSFER_REQ_OFF)
      && (DTS_Context.DtFlowStatus != DTS_APP_FLOW_OFF) )
  {   
    /*Data Packet to send to remote*/
    Notification_Data_Buffer[0] += 1;
    /* compute CRC */
    crc_result = APP_BLE_ComputeCRC8((uint8_t*) Notification_Data_Buffer, (Att_Mtu_Exchanged - 1));
    Notification_Data_Buffer[Att_Mtu_Exchanged - 1] = crc_result;

    DTS_Context.TxData.pPayload = Notification_Data_Buffer;
    DTS_Context.TxData.Length =  Att_Mtu_Exchanged;

    status = DTS_STM_UpdateChar(DT_TX_CHAR_UUID, (uint8_t *) &DTS_Context.TxData);
    if (status == BLE_STATUS_INSUFFICIENT_RESOURCES)
    {
      DTS_Context.DtFlowStatus = DTS_APP_FLOW_OFF;
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
  DTS_Context.DtFlowStatus = DTS_APP_FLOW_ON;
}

void DTS_Button1TriggerReceived( void )
{
  if(DTS_Context.ButtonTransferReq != DTS_APP_TRANSFER_REQ_OFF)
  {
    BSP_LED_Off(LED_BLUE);
    DTS_Context.ButtonTransferReq = DTS_APP_TRANSFER_REQ_OFF;
  }
  else
  {
    BSP_LED_On(LED_BLUE);
    DTS_Context.ButtonTransferReq = DTS_APP_TRANSFER_REQ_ON;
    UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
  }

  return;
}

void DTS_Button2TriggerReceived( void )
{
  APP_DBG_MSG("**CHANGE PHY \n");
  UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_PHY_UPDATE_ID, CFG_SCH_PRIO_0);
  return;
}

void DTS_Button2LongTriggerReceived( void )
{
  APP_DBG_MSG("**CHANGE CONN INTERVAL \n");
  UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_INTERV_UPDATE_ID, CFG_SCH_PRIO_0);
  return;
}

static void Appli_UpdateButtonState(Button_TypeDef button, int isPressed)
{
  uint32_t t0 = 0,t1 = 1;

  t0 = HAL_GetTick(); /* SW3 press timing */
  
  while(BSP_PB_GetState(button) == BUTTON_PRESSED);
  t1 = HAL_GetTick(); /* SW3 release timing */
  
  if((t1 - t0) > LONG_PRESS_THRESHOLD)
  {
    if(button == BUTTON_SW1)
    {
      /* Button 1 long press action */
      DTS_Button1TriggerReceived();
    }
    else if(button == BUTTON_SW2)
    {
      /* Button 2 long press action */
      //DTS_Button2TriggerReceived();
      DTS_Button2LongTriggerReceived();
    }
    else if(button == BUTTON_SW3)
    {
      /* Button 3 long press action */
      APP_DBG_MSG("clear database \n");
      BLE_SVC_GAP_Clear_DataBase();
    }
  }
  else if((t1 - t0) > BOUNCE_THRESHOLD)
  {
    if(button == BUTTON_SW1)
    {
      /* Button 1 short press action */
      DTS_Button1TriggerReceived();
    }
    else if(button == BUTTON_SW2)
    {
      /* Button 2 short press action */
      DTS_Button2TriggerReceived();
    }
    else if(button == BUTTON_SW3)
    {
      /* Button 3 short press action */
      APP_DBG_MSG("slave security request \n");
      BLE_SVC_GAP_Security_Req();
    }
  }
}

static void DTS_App_Button1_Trigger_Received(void)
{
  Appli_UpdateButtonState(BUTTON_SW1, BSP_PB_GetState(BUTTON_SW1) == BUTTON_PRESSED);
  
  return;
}

static void DTS_App_Button2_Trigger_Received(void)
{
  Appli_UpdateButtonState(BUTTON_SW2, BSP_PB_GetState(BUTTON_SW2) == BUTTON_PRESSED);
  
  return;
}

static void DTS_App_Button3_Trigger_Received(void)
{
  Appli_UpdateButtonState(BUTTON_SW3, BSP_PB_GetState(BUTTON_SW3) == BUTTON_PRESSED);
  
  return;
}

static void BLE_App_Delay_DataThroughput(void)
{
  uint32_t DataThroughput;
  DTS_STM_Payload_t ThroughputToSend; 
  
  DataThroughput = (uint32_t)(DTS_DataReceived/TIMEUNIT);
//  APP_DBG_MSG("DataThroughput = %ld  bytes/s\n", DataThroughput);
  APP_DBG_MSG("DataThroughput = %ld  bytes/s lost = %ld \n", DataThroughput, DTS_packet_lost);
  
  ThroughputToSend.Length = 4;
  ThroughputToSend.pPayload = (uint8_t*)&DataThroughput;
  
  DTS_STM_UpdateChar(DT_THROUGHPUT_CHAR_UUID, (uint8_t*)&ThroughputToSend );
  DTS_DataReceived = 0;
  DTS_packet_lost = 0;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */
