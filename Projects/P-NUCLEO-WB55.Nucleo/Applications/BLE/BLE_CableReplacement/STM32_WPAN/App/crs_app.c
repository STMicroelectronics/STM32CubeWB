/**
  ******************************************************************************
  * @file    crs_app.c
  * @author  MCD Application Team
  * @brief   Cable Replacement Service Application
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
#include "crs_app.h"
#include "stm32_seq.h"
#include "DispTools.h"
#include "stm_queue.h"


/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define RX_BUFFER_SIZE                                                        64
#define CR                                                                  0x0d
#define LF                                                                  0x0a
#define POSXRX                                                                10
#define POSYRX                                                                10
#define XSIZE                                                                 10
#define POSXTX                                                                10
#define POSYTX                                                                30
#define YSIZE                                                                 10
#define MAX_STRING_SIZE                                                       20

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

/**
 * END of Section BLE_APP_CONTEXT
 */
static uint8_t InputCharFromUart; 
static queue_t RxQueue;
static uint8_t RxQueueBuffer[RX_BUFFER_SIZE];  
static uint8_t PosXTx, PosYTx, PosXRx, PosYRx;
static char szString[MAX_STRING_SIZE+1];
static uint8_t idx;

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ----------------------------------------------------------*/
static void CRSAPP_Terminal_UART_RxCpltCallback( void );
static void CRSAPP_Terminal_Init(void);
static void CRSAPP_Update(void);

/**
  * @brief  This function handles USARTx interrupt request.
  * @param  None
  * @retval None
  */
static void CRSAPP_Terminal_UART_RxCpltCallback( void )
{
  if(InputCharFromUart == BKSP)
  {
    if(idx > 0)
    {
      idx--;
      PosXTx--;
      PrintPcCrt(PosXTx, PosYTx, " ");
      PrintPcCrt(PosXTx, PosYTx, "");
    }
  }
  else if(InputCharFromUart == CR)
  {
    PosYTx++;
    PosXTx = POSXTX;
    szString[idx] = '\0'; 
    idx = 0;
    if(PosYTx > POSYTX + YSIZE + 2)
      PosYTx = POSYTX + 2;
    ClearLines(PosYTx, 1);
    PrintPcCrt(PosXTx, PosYTx, "");
    CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&szString[0]);
  }
  else
  {
    char car[2];
    
    car[0] = InputCharFromUart;
    car[1] = 0;
    PrintPcCrt(PosXTx++, PosYTx, "%s", &car[0]);
    szString[idx++] = InputCharFromUart;

    if(idx > MAX_STRING_SIZE - 1)
  {
    szString[idx] = '\0'; 
    idx = 0;
      if(PosXTx > XSIZE)
      {
        PosYTx++;
        PosXTx = POSXTX;
      }
      if(PosYTx > POSYTX + YSIZE + 2)
      {
      PosYTx = POSYTX + 2;
    ClearLines(PosYTx, 1);
    PrintPcCrt(PosXTx, PosYTx, "");
      }
      CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&szString[0]);
    }
  }

  HW_UART_Receive_IT(CFG_CONSOLE_MENU, &InputCharFromUart, 1, CRSAPP_Terminal_UART_RxCpltCallback);
  return;
 }

/**
  * @brief  This function initialize terminal
  * @param  None
  * @retval None
  */
static void CRSAPP_Terminal_Init(void)
{
  HW_UART_Init(CFG_CONSOLE_MENU);

  CircularQueue_Init(&RxQueue, RxQueueBuffer, RX_BUFFER_SIZE, 1, CIRCULAR_QUEUE_NO_WRAP_FLAG); 
  
  HW_UART_Receive_IT(CFG_CONSOLE_MENU, &InputCharFromUart, 1, CRSAPP_Terminal_UART_RxCpltCallback);

  return;
}


/* Public functions ----------------------------------------------------------*/

static void CRSAPP_Update()
{
  CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&szString[0]);
}

void CRSAPP_Notification(CRSAPP_Notification_evt_t *pNotification)
{
  switch(pNotification->CRS_Evt_Opcode)
  {
    case CRS_WRITE_EVT:
      {
        APP_DBG_MSG("CRS_WRITE_EVT: Data received: %s \n", pNotification->DataTransfered.pPayload);
        pNotification->DataTransfered.pPayload[pNotification->DataTransfered.Length] = '\0';
        PrintPcCrt(PosXRx, PosYRx, "%s", pNotification->DataTransfered.pPayload);
        PosXRx += pNotification->DataTransfered.Length;
        if((PosXRx >= XSIZE) || (pNotification->DataTransfered.Length < MAX_STRING_SIZE))
        {
          PosXRx = POSXRX;
        PosYRx++;
          if(PosYRx > POSYRX + YSIZE + 2)
          PosYRx = POSYRX + 2;
          ClearLines(PosYRx, 1);
        }
        PrintPcCrt(PosXTx, PosYTx, "");
      }
      break;
      
    case CRS_NOTIFY_ENABLED_EVT:
      {
        APP_DBG_MSG("CRS_NOTIFY_ENABLED_EVT\n");
        ClearScreen();
        PrintPcCrt(POSXRX, POSXRX, "Receive:");
        PrintPcCrt(POSXTX, POSYTX, "Transmit:");
        PrintPcCrt(PosXTx, PosYTx, "");
      }
      break;

    case CRS_NOTIFY_DISABLED_EVT:
      {
        APP_DBG_MSG("CRS_NOTIFY_DISABLED_EVT\n");
        ClearScreen();
      }
      break;

   default:
      break;
  }

  return;
}

void CRSAPP_Init(void)
{
  UTIL_SEQ_RegTask( 1<< CFG_TASK_CRS_UPDATE_REQ_ID, UTIL_SEQ_RFU, CRSAPP_Update );

  PosXRx = POSXRX;
  PosYRx = POSYRX + 2;
  PosXTx = POSXTX;
  PosYTx = POSYTX + 2;
  idx = 0;
  
  CRSAPP_Terminal_Init();

  ClearScreen();

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
