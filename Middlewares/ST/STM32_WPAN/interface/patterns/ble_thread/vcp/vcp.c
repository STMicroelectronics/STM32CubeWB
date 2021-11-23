/**
 ******************************************************************************
 * @file    vcp.c
 * @author  MCD Application Team
 * @brief   Virtual Com Port
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
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

#if (CFG_USB_INTERFACE_ENABLE != 0)

#include "vcp_conf.h"
#include "vcp.h"

#include "stm32_seq.h"

#include "usbd_cdc.h"
#include "usbd_desc.h"


/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  VCP_TX_NOT_READY,
  VCP_TX_READY,
} VCP_TxStatus_t;

typedef enum
{
  TX_DATA_NOT_PENDING,
  TX_DATA_PENDING,
} TxDataStatus_t;

typedef struct
{
uint8_t *pTxBuffer;
uint8_t *pRxBuffer;
void (*pf_CallBack)( void );
VCP_TxStatus_t VcpStatus;
TxDataStatus_t TxDataStatus;
uint16_t TxDataSize;
uint8_t VcpTxReadyTid;
} VCP_Context_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static VCP_Context_t VCP_Context;

static USBD_CDC_LineCodingTypeDef LineCoding = { VCP_BAUD_RATE, /* baud rate*/
0x00, /* stop bits-1*/
0x00, /* parity - none*/
0x08 /* nb. of bits 8*/
};

/* USB handler declaration */
static USBD_HandleTypeDef VCP_Device;

static int8_t Vcp_Itf_Init( void );
static int8_t Vcp_Itf_DeInit( void );
static int8_t Vcp_Itf_Control( uint8_t cmd , uint8_t* pbuf , uint16_t length );
static int8_t Vcp_Itf_Receive( uint8_t* pbuf , uint32_t *Len );

static USBD_CDC_ItfTypeDef VCP_CDC_fops = { Vcp_Itf_Init, Vcp_Itf_DeInit, Vcp_Itf_Control, Vcp_Itf_Receive, VCP_TransmitCplt };

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SendData( void );
static void EnableTxPath( void );

/* Functions Definition ------------------------------------------------------*/
void VCP_Init( uint8_t *p_tx_buffer , uint8_t *p_rx_buffer )
{
  VCP_Context.VcpStatus = VCP_TX_NOT_READY;
  VCP_Context.TxDataStatus = TX_DATA_NOT_PENDING;
  VCP_Context.pTxBuffer = p_tx_buffer;
  VCP_Context.pRxBuffer = p_rx_buffer;

  /* Init Device Library */
  USBD_Init(&VCP_Device, &VCP_Desc, 0);

  /* Register the CDC class */
  USBD_RegisterClass(&VCP_Device, &USBD_CDC);

  /* Add CDC Interface Class */
  USBD_CDC_RegisterInterface(&VCP_Device, &VCP_CDC_fops);

  /* Start Device Process */
  USBD_Start(&VCP_Device);

#ifdef  VCP_TX_PATH_INTERFACE_READY_SETUP_TIME
  HW_TS_Create(VCP_TIMER_PROC_ID, &VCP_Context.VcpTxReadyTid, hw_ts_SingleShot, EnableTxPath);
#endif

  UTIL_SEQ_RegTask(1<<VCP_TASK_ID, UTIL_SEQ_RFU, SendData);
  
  return;
}

void VCP_SendData( uint8_t *p_data , uint16_t size , void (*cb)( void ) )
{
  BACKUP_PRIMASK();

  memcpy(VCP_Context.pTxBuffer, p_data, size);

  VCP_Context.pf_CallBack = cb;
  VCP_Context.TxDataSize = size;

  DISABLE_IRQ();
  if (VCP_Context.VcpStatus != VCP_TX_NOT_READY)
  {
    UTIL_SEQ_SetTask(1 << VCP_TASK_ID, VCP_TASK_PRIO);
  }
  else
  {
    VCP_Context.TxDataStatus = TX_DATA_PENDING;
  }
  RESTORE_PRIMASK();

  return;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void SendData( void )
{
  USBD_CDC_SetTxBuffer(&VCP_Device, VCP_Context.pTxBuffer, VCP_Context.TxDataSize);

  if (USBD_CDC_TransmitPacket(&VCP_Device) != USBD_OK)
  {
    UTIL_SEQ_SetTask(1 << VCP_TASK_ID, VCP_TASK_PRIO);
  }
  else
  {
    VCP_Context.pf_CallBack();
  }

  return;
}

static void EnableTxPath( void )
{
  BACKUP_PRIMASK();

  DISABLE_IRQ();
  VCP_Context.VcpStatus = VCP_TX_READY;

  if (VCP_Context.TxDataStatus != TX_DATA_NOT_PENDING)
  {
    UTIL_SEQ_SetTask(1 << VCP_TASK_ID, VCP_TASK_PRIO);
  }
  RESTORE_PRIMASK();

#ifdef  VCP_TX_PATH_INTERFACE_READY_SETUP_TIME
  HW_TS_Delete(VCP_Context.VcpTxReadyTid);
#endif

  return;
}

/**
 * @brief  Vcp_Itf_Init
 *         Initializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t Vcp_Itf_Init( void )
{
  USBD_CDC_SetRxBuffer(&VCP_Device, VCP_Context.pRxBuffer);

  /* Initiate next USB packet transfer */
  USBD_CDC_ReceivePacket(&VCP_Device);

#ifdef  VCP_TX_PATH_INTERFACE_READY_SETUP_TIME
  HW_TS_Start(VCP_Context.VcpTxReadyTid, VCP_TX_PATH_INTERFACE_READY_SETUP_TIME);
#else
  EnableTxPath();
#endif

  return (USBD_OK);
}

/**
 * @brief  Vcp_Itf_DeInit
 *         DeInitializes the CDC media low layer
 * @param  None
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t Vcp_Itf_DeInit( void )
{
  return (USBD_OK);
}

/**
 * @brief  Vcp_Itf_Control
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t Vcp_Itf_Control( uint8_t cmd , uint8_t* pbuf , uint16_t length )
{
  switch (cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:
      /* Add your code here */
      break;

    case CDC_GET_ENCAPSULATED_RESPONSE:
      /* Add your code here */
      break;

    case CDC_SET_COMM_FEATURE:
      /* Add your code here */
      break;

    case CDC_GET_COMM_FEATURE:
      /* Add your code here */
      break;

    case CDC_CLEAR_COMM_FEATURE:
      /* Add your code here */
      break;

    case CDC_SET_LINE_CODING:
      LineCoding.bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
 (pbuf[2] << 16) | (pbuf[3] << 24));
      LineCoding.format = pbuf[4];
      LineCoding.paritytype = pbuf[5];
      LineCoding.datatype = pbuf[6];

      break;

    case CDC_GET_LINE_CODING:
      pbuf[0] = (uint8_t) (LineCoding.bitrate);
      pbuf[1] = (uint8_t) (LineCoding.bitrate >> 8);
      pbuf[2] = (uint8_t) (LineCoding.bitrate >> 16);
      pbuf[3] = (uint8_t) (LineCoding.bitrate >> 24);
      pbuf[4] = LineCoding.format;
      pbuf[5] = LineCoding.paritytype;
      pbuf[6] = LineCoding.datatype;
      break;

    case CDC_SET_CONTROL_LINE_STATE:
      /* Add your code here */
      break;

    case CDC_SEND_BREAK:
      /* Add your code here */
      break;

    default:
      break;
  }

  return (USBD_OK);
}

/**
 * @brief  CDC_Itf_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data to be transmitted
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t Vcp_Itf_Receive( uint8_t* Buf , uint32_t *Len )
{
  VCP_DataReceived(Buf, Len);

  /* Initiate next USB packet transfer */
  USBD_CDC_ReceivePacket(&VCP_Device);

  return (USBD_OK);
}

__weak void VCP_DataReceived( uint8_t* Buf , uint32_t *Len )
{
  return;
}
__weak int8_t VCP_TransmitCplt( uint8_t *Buf, uint32_t *Len, uint8_t epnum )
{
  return(USBD_OK);
}



#endif /* (CFG_USB_INTERFACE_ENABLE != 0) */
