/**
  ******************************************************************************
 * @file    tm.c
 * @author  MCD Application Team
 * @brief   Transparent mode
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
#include "main.h"
#include "app_common.h"

#include "tl.h"
#include "mbox_def.h"
#include "lhci.h"
#include "shci.h"
#include "stm_list.h"
#include "stm32_seq.h"
#include "tm.h"
#include "stm32_lpm.h"
#include "shci_tl.h"
#include "ble_bufsize.h"

#include "vcp.h"
#include "dbg_trace.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  TM_WAITING_PACKET_START,
  TM_WAITING_PACKET_CONT,
}TM_RxStateMachine_t;

typedef struct
{
  uint8_t * p_DestBuffer;
  uint32_t WaitedPacketLength;
  TM_RxStateMachine_t RxStateMachine;
  uint8_t PacketIndicator;
} TM_RxContext_t;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t HciAclDataBuffer[sizeof(TL_PacketHeader_t) + 5 + 251];

ALIGN(4) static TL_CmdPacket_t SysLocalCmd;
static tListNode HostTxQueue;
static TL_EvtPacket_t *pTxToHostPacket;
static MB_RefTable_t * p_RefTable;
static uint8_t SysLocalCmdStatus;

TM_RxContext_t TM_RxContext;

static uint8_t VcpRxBuffer[sizeof(TL_CmdSerial_t)]; /* Received Data over USB are stored in this buffer */
static uint8_t VcpTxBuffer[sizeof(TL_EvtPacket_t) + 254]; /* Transmit buffer over USB */

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void HostTxCb( void );
static void TxVcpDone( void );
static void ReportRxPacket( void );
static void TM_SysLocalCmd( void );
static void TM_TxToHost( void );
static void TM_BleEvtRx( TL_EvtPacket_t *phcievt );
static void TM_AclDataAck( void );

/* Functions Definition ------------------------------------------------------*/

void TM_Init( void  )
{
  TL_BLE_InitConf_t tl_ble_init_conf;
  uint32_t ipccdba;
  SHCI_CmdStatus_t status;
  
  SHCI_C2_CONFIG_Cmd_Param_t config_param = {0};
  uint32_t RevisionID=0;
  uint32_t DeviceID=0;
   
  SHCI_C2_Ble_Init_Cmd_Packet_t ble_init_cmd_packet =
  {
    {{0,0,0}},                          /**< Header unused */
    {0,                                 /** pBleBufferAddress not used */
    0,                                  /** BleBufferSize not used */
    CFG_BLE_NUM_GATT_ATTRIBUTES,
    CFG_BLE_NUM_GATT_SERVICES,
    CFG_BLE_ATT_VALUE_ARRAY_SIZE,
    CFG_BLE_NUM_LINK,
    CFG_BLE_DATA_LENGTH_EXTENSION,
    CFG_BLE_PREPARE_WRITE_LIST_SIZE,
    CFG_BLE_MBLOCK_COUNT,
    CFG_BLE_MAX_ATT_MTU,
    CFG_BLE_PERIPHERAL_SCA,
    CFG_BLE_CENTRAL_SCA,
    CFG_BLE_LS_SOURCE,
    CFG_BLE_MAX_CONN_EVENT_LENGTH,
    CFG_BLE_HSE_STARTUP_TIME,
    CFG_BLE_VITERBI_MODE,
    CFG_BLE_OPTIONS,
    0,
    CFG_BLE_MAX_COC_INITIATOR_NBR,
    CFG_BLE_MIN_TX_POWER,
    CFG_BLE_MAX_TX_POWER,
    CFG_BLE_RX_MODEL_CONFIG,
    CFG_BLE_MAX_ADV_SET_NBR, 
    CFG_BLE_MAX_ADV_DATA_LEN,
    CFG_BLE_TX_PATH_COMPENS,
    CFG_BLE_RX_PATH_COMPENS,
    CFG_BLE_CORE_VERSION,
     CFG_BLE_OPTIONS_EXT,
     CFG_BLE_MAX_ADD_EATT_BEARERS
    }
  };

  ipccdba = READ_BIT( FLASH->IPCCBR, FLASH_IPCCBR_IPCCDBA );
  p_RefTable = (MB_RefTable_t*)((ipccdba<<2) + SRAM2A_BASE);

  tl_ble_init_conf.p_cmdbuffer = (uint8_t*)&BleCmdBuffer;
  tl_ble_init_conf.p_AclDataBuffer = HciAclDataBuffer;
  tl_ble_init_conf.IoBusEvtCallBack = TM_BleEvtRx;
  tl_ble_init_conf.IoBusAclDataTxAck = TM_AclDataAck;
  TL_BLE_Init( (void*) &tl_ble_init_conf );

  UTIL_LPM_SetStopMode(1 << CFG_LPM_APP_BLE, UTIL_LPM_DISABLE);

  SysLocalCmdStatus = 0;
  TM_RxContext.RxStateMachine = TM_WAITING_PACKET_START;

  config_param.PayloadCmdSize = SHCI_C2_CONFIG_PAYLOAD_CMD_SIZE;

  RevisionID = LL_DBGMCU_GetRevisionID();
  APP_DBG_MSG(">>== DBGMCU_GetRevisionID= %lx \n\r", RevisionID);
  config_param.RevisionID = RevisionID;

  DeviceID = LL_DBGMCU_GetDeviceID();
  APP_DBG_MSG(">>== DBGMCU_GetDeviceID= %lx \n\r", DeviceID);
  config_param.DeviceID = DeviceID;

  status = SHCI_C2_Config(&config_param);
  if (status != SHCI_Success)
  {
    /* if you are here, maybe CPU2 doesn't contain STM32WB_Copro_Wireless_Binaries, see Release_Notes.html */
    Error_Handler();
  }

  status = SHCI_C2_BLE_Init(&ble_init_cmd_packet);
  if (status != SHCI_Success)
  {
    /* if you are here, maybe CPU2 doesn't contain STM32WB_Copro_Wireless_Binaries, see Release_Notes.html */
    Error_Handler();
  }

  UTIL_SEQ_RegTask( 1<<CFG_TASK_SYS_LOCAL_CMD_ID, UTIL_SEQ_RFU, TM_SysLocalCmd);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_BLE_HCI_CMD_ID, UTIL_SEQ_RFU, (void (*)( void )) TL_BLE_SendCmd);
  UTIL_SEQ_RegTask( 1<<CFG_TASK_TX_TO_HOST_ID, UTIL_SEQ_RFU, TM_TxToHost);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SYS_HCI_CMD_ID, UTIL_SEQ_RFU, (void (*)( void )) TL_SYS_SendCmd);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_HCI_ACL_DATA_ID, UTIL_SEQ_RFU, (void (*)( void )) TL_BLE_SendAclData);

  pTxToHostPacket = 0;

  LST_init_head(&HostTxQueue);

  VCP_Init(&VcpTxBuffer[0], &VcpRxBuffer[0]);

  return;
}

void TM_SysCmdRspCb (TL_EvtPacket_t * p_cmd_resp)
{
  if(SysLocalCmdStatus != 0)
  {
    SysLocalCmdStatus = 0;
    UTIL_SEQ_SetEvt( 1<< CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID );
  }
  else
  {
    LST_insert_tail (&HostTxQueue, (tListNode *)p_cmd_resp);

    UTIL_SEQ_SetTask( 1<<CFG_TASK_TX_TO_HOST_ID,CFG_SCH_PRIO_0);
  }

  return;
}


/*************************************************************
 *
 * EXTERN FUNCTIONS
 *
 *************************************************************/
/**
 * @brief  CDC_Itf_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data to be transmitted
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
void VCP_DataReceived( uint8_t* Buf , uint32_t *Len )
{
  uint32_t parsing_status;
  uint8_t * p_data_buffer_to_be_processed;
  uint32_t buffer_len_to_be_processed;
  uint32_t data_len_to_copy;

  parsing_status = 1;
  p_data_buffer_to_be_processed = Buf;
  buffer_len_to_be_processed = (*Len);

  while(parsing_status != 0)
  {
    if(TM_RxContext.RxStateMachine != TM_WAITING_PACKET_CONT)
    {
      /* The buffer begins with the start of a HCI packet */

      if(((TL_CmdSerial_t*)p_data_buffer_to_be_processed)->cmd.plen < (buffer_len_to_be_processed - TL_CMD_HDR_SIZE))
      {
        /* more than one command has been received ( this can be the case only on different channel )*/
        data_len_to_copy = ((TL_CmdSerial_t*)p_data_buffer_to_be_processed)->cmd.plen + TL_CMD_HDR_SIZE;
      }
      else
      {
        /* one or less packet has been received */
        data_len_to_copy = buffer_len_to_be_processed;
      }

      TM_RxContext.PacketIndicator = ((TL_CmdSerial_t*)p_data_buffer_to_be_processed)->type;
      switch (TM_RxContext.PacketIndicator)
      {
        case TL_ACL_DATA_PKT_TYPE:
          TM_RxContext.p_DestBuffer = (uint8_t*) &(((TL_AclDataPacket_t*)(p_RefTable->p_ble_table->phci_acl_data_buffer))->AclDataSerial);
          memcpy(TM_RxContext.p_DestBuffer, p_data_buffer_to_be_processed, data_len_to_copy);
          break;

        case TL_SYSCMD_PKT_TYPE:
          TM_RxContext.p_DestBuffer = (uint8_t*) &(((TL_CmdPacket_t*)(p_RefTable->p_sys_table->pcmd_buffer))->cmdserial);
          memcpy(TM_RxContext.p_DestBuffer, p_data_buffer_to_be_processed, data_len_to_copy);
          break;

        case TL_LOCCMD_PKT_TYPE:
          TM_RxContext.p_DestBuffer = (uint8_t*) &SysLocalCmd.cmdserial;
          memcpy(TM_RxContext.p_DestBuffer, p_data_buffer_to_be_processed, data_len_to_copy);
          break;

        case TL_BLECMD_PKT_TYPE:
          TM_RxContext.p_DestBuffer = (uint8_t*) &(((TL_CmdPacket_t*)(p_RefTable->p_ble_table->pcmd_buffer))->cmdserial);
          memcpy(TM_RxContext.p_DestBuffer, p_data_buffer_to_be_processed, data_len_to_copy);
          break;

        default:
          break;
      }

      if((((TL_CmdSerial_t*)p_data_buffer_to_be_processed)->cmd.plen + TL_CMD_HDR_SIZE) > data_len_to_copy)
      {
        /* The received packet was not full */
        TM_RxContext.RxStateMachine = TM_WAITING_PACKET_CONT;
        TM_RxContext.WaitedPacketLength = (((TL_CmdSerial_t*)p_data_buffer_to_be_processed)->cmd.plen + TL_CMD_HDR_SIZE) - data_len_to_copy;
        TM_RxContext.p_DestBuffer += data_len_to_copy;

        /* stop parsing the buffer */
        parsing_status = 0;
      }
      else if(data_len_to_copy < buffer_len_to_be_processed)
      {
        /* a full packet has been received and another packet is waiting*/
        ReportRxPacket( );

        p_data_buffer_to_be_processed += data_len_to_copy ;
        buffer_len_to_be_processed -= data_len_to_copy;
      }
      else
      {
        /* a full packet has been received and no other packet are in the received buffer */
        ReportRxPacket( );

        /* stop parsing the buffer */
        parsing_status = 0;
      }
    }
    else
    {
      /* the data received is a packet follow up */

      if(buffer_len_to_be_processed > TM_RxContext.WaitedPacketLength)
      {
        /* the end of the previous command has been received with another one */
        memcpy(TM_RxContext.p_DestBuffer, p_data_buffer_to_be_processed, TM_RxContext.WaitedPacketLength);
        ReportRxPacket( );
        TM_RxContext.RxStateMachine = TM_WAITING_PACKET_START;
        p_data_buffer_to_be_processed += TM_RxContext.WaitedPacketLength;
        buffer_len_to_be_processed -= TM_RxContext.WaitedPacketLength;
      }
      else if(buffer_len_to_be_processed < TM_RxContext.WaitedPacketLength)
      {
        /* the command packet is still not fully received */
        memcpy(TM_RxContext.p_DestBuffer, p_data_buffer_to_be_processed, buffer_len_to_be_processed);
        TM_RxContext.WaitedPacketLength -= buffer_len_to_be_processed;
        TM_RxContext.p_DestBuffer += buffer_len_to_be_processed;
        parsing_status = 0;
      }
      else
      {
        /* the end of the previous command has been received with no other one */
        memcpy(TM_RxContext.p_DestBuffer, p_data_buffer_to_be_processed, buffer_len_to_be_processed);
        ReportRxPacket( );
        TM_RxContext.RxStateMachine = TM_WAITING_PACKET_START;
        parsing_status = 0;
      }
    }
  }

  return;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void ReportRxPacket( void )
{
  switch (TM_RxContext.PacketIndicator)
  {
    case TL_ACL_DATA_PKT_TYPE:
      UTIL_SEQ_SetTask(1 << CFG_TASK_HCI_ACL_DATA_ID, CFG_SCH_PRIO_0);
      break;

    case TL_SYSCMD_PKT_TYPE:
      UTIL_SEQ_SetTask(1 << CFG_TASK_SYS_HCI_CMD_ID, CFG_SCH_PRIO_0);
      break;

    case TL_LOCCMD_PKT_TYPE:
      UTIL_SEQ_SetTask(1 << CFG_TASK_SYS_LOCAL_CMD_ID, CFG_SCH_PRIO_0);
      break;

    case TL_BLECMD_PKT_TYPE:
      UTIL_SEQ_SetTask(1 << CFG_TASK_BLE_HCI_CMD_ID, CFG_SCH_PRIO_0);
      break;

    default:
      break;
  }

  return;
}

static void TM_TxToHost( void )
{
  UTIL_SEQ_PauseTask(1 << CFG_TASK_TX_TO_HOST_ID);

  LST_remove_head(&HostTxQueue, (tListNode **) &pTxToHostPacket);

  if(pTxToHostPacket->evtserial.type == TL_ACL_DATA_PKT_TYPE)
  {
    VCP_SendData((uint8_t *) &pTxToHostPacket->evtserial, ((TL_AclDataPacket_t *)pTxToHostPacket)->AclDataSerial.length + 5, TxVcpDone);
  }
  else
  {
    VCP_SendData((uint8_t *)&((TL_AclDataPacket_t *)pTxToHostPacket)->AclDataSerial, pTxToHostPacket->evtserial.evt.plen + TL_EVT_HDR_SIZE, TxVcpDone);
  }

  return;
}

static void TM_SysLocalCmd( void )
{
  switch (SysLocalCmd.cmdserial.cmd.cmdcode)
  {
    case LHCI_OPCODE_C1_WRITE_REG:
      LHCI_C1_Write_Register(&SysLocalCmd);
      break;

    case LHCI_OPCODE_C1_READ_REG:
      LHCI_C1_Read_Register(&SysLocalCmd);
      break;

    case LHCI_OPCODE_C1_DEVICE_INF:
      LHCI_C1_Read_Device_Information(&SysLocalCmd);
      break;

    default:
      ((TL_CcEvt_t *)(((TL_EvtPacket_t*)&SysLocalCmd)->evtserial.evt.payload))->cmdcode = SysLocalCmd.cmdserial.cmd.cmdcode;
      ((TL_CcEvt_t *) (((TL_EvtPacket_t*) &SysLocalCmd)->evtserial.evt.payload))->payload[0] = 0x01;
      ((TL_CcEvt_t *) (((TL_EvtPacket_t*) &SysLocalCmd)->evtserial.evt.payload))->numcmd = 1;
      ((TL_EvtPacket_t*) &SysLocalCmd)->evtserial.type = TL_LOCRSP_PKT_TYPE;
      ((TL_EvtPacket_t*) &SysLocalCmd)->evtserial.evt.evtcode = TL_BLEEVT_CC_OPCODE;
      ((TL_EvtPacket_t*) &SysLocalCmd)->evtserial.evt.plen = TL_EVT_CS_PAYLOAD_SIZE;

      break;
  }

  LST_insert_tail(&HostTxQueue, (tListNode *) &SysLocalCmd);
  UTIL_SEQ_SetTask(1 << CFG_TASK_TX_TO_HOST_ID, CFG_SCH_PRIO_0);

  return;
}

static void HostTxCb( void )
{
  if( (pTxToHostPacket >= (TL_EvtPacket_t *)(p_RefTable->p_mem_manager_table->blepool)) && (pTxToHostPacket < ( (TL_EvtPacket_t *)((p_RefTable->p_mem_manager_table->blepool) + p_RefTable->p_mem_manager_table->blepoolsize))))
  {
    TL_MM_EvtDone(pTxToHostPacket);
  }

  if ( LST_is_empty( &HostTxQueue ) == FALSE )
  {
    UTIL_SEQ_SetTask(1 << CFG_TASK_TX_TO_HOST_ID, CFG_SCH_PRIO_0);
  }

  return;
}

static void TM_BleEvtRx( TL_EvtPacket_t *phcievt )
{
  LST_insert_tail (&HostTxQueue, (tListNode *)phcievt);

  UTIL_SEQ_SetTask( 1<<CFG_TASK_TX_TO_HOST_ID,CFG_SCH_PRIO_0);

  return;
}

static void TxVcpDone( void )
{
  HostTxCb();
  UTIL_SEQ_ResumeTask(1 << CFG_TASK_TX_TO_HOST_ID);

  return;
}

static void TM_AclDataAck( void )
{
  /**
   * The current implementation assumes the GUI will not send a new HCI ACL DATA packet before this ack is received
   * ( which means the CPU2 has handled the previous packet )
   * In order to implement a secure mechanism, it is required either
   * - a flow control with the GUI
   * - a local pool of buffer to store packets received from the GUI
   */
  return;
}

/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void shci_send( uint16_t cmd_code, uint8_t len_cmd_payload, uint8_t * p_cmd_payload, TL_EvtPacket_t * p_rsp )
{
  TL_CmdPacket_t *p_cmd_buffer;
  uint32_t ipccdba;
  MB_RefTable_t * p_ref_table;

  ipccdba = READ_BIT( FLASH->IPCCBR, FLASH_IPCCBR_IPCCDBA );
  p_ref_table = (MB_RefTable_t*)((ipccdba<<2) + SRAM2A_BASE);

  SysLocalCmdStatus = 1;

  p_cmd_buffer = (TL_CmdPacket_t *)(p_ref_table->p_sys_table->pcmd_buffer);

  p_cmd_buffer->cmdserial.cmd.cmdcode = cmd_code;
  p_cmd_buffer->cmdserial.cmd.plen = len_cmd_payload;

  memcpy(p_cmd_buffer->cmdserial.cmd.payload, p_cmd_payload, len_cmd_payload );

  TL_SYS_SendCmd( 0, 0 );

  UTIL_SEQ_WaitEvt( 1<< CFG_IDLEEVT_SYSTEM_HCI_CMD_EVT_RSP_ID );

  /**
   * The command complete of a system command does not have the header
   * It starts immediately with the evtserial field
   */
  memcpy( &(p_rsp->evtserial), p_cmd_buffer, ((TL_EvtSerial_t*)p_cmd_buffer)->evt.plen + TL_EVT_HDR_SIZE );

  return;
}


