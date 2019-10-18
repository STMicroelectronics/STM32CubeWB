/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tm.c
 * @author  MCD Application Team
 * @brief   Transparent mode
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
#include "tl.h"
#include "mbox_def.h"
#include "lhci.h"
#include "shci.h"
#include "stm_list.h"
#include "tm.h"
#include "stm32_lpm.h"
#include "shci_tl.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  LOW_POWER_MODE_DISABLE,
  LOW_POWER_MODE_ENABLE,
}LowPowerModeStatus_t;

typedef enum
{
  WAITING_TYPE,
  WAITING_LENGTH,
  WAITING_PAYLOAD
}HciReceiveStatus_t;

typedef enum
{
  TX_ONGOING,
  TX_DONE
}HostTxStatus_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static TL_CmdPacket_t BleCmdBuffer;
PLACE_IN_SECTION("MB_MEM2") ALIGN(4) static uint8_t HciAclDataBuffer[sizeof(TL_PacketHeader_t) + 5 + 251];

static uint8_t RxHostData[5];
static HciReceiveStatus_t HciReceiveStatus;
static TL_CmdPacket_t SysLocalCmd;
static uint8_t *pHostRx;
static tListNode  HostTxQueue;
static TL_EvtPacket_t *pTxToHostPacket;
static HostTxStatus_t HostTxStatus;
static MB_RefTable_t * p_RefTable;
static uint8_t SysLocalCmdStatus;
static LowPowerModeStatus_t LowPowerModeStatus;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void RxCpltCallback(void);
static void HostTxCb( void );
static void TM_SysLocalCmd( void );
static void TM_TxToHost( void );
static void TM_BleEvtRx( TL_EvtPacket_t *phcievt );
static void TM_AclDataAck( void );
#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
#endif
#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
#endif
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void TM_Init( void  )
{
/* USER CODE BEGIN TM_Init_1 */

/* USER CODE END TM_Init_1 */
  TL_BLE_InitConf_t tl_ble_init_conf;
  uint32_t ipccdba;
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
    CFG_BLE_SLAVE_SCA,
    CFG_BLE_MASTER_SCA,
    CFG_BLE_LSE_SOURCE,
    CFG_BLE_MAX_CONN_EVENT_LENGTH,
    CFG_BLE_HSE_STARTUP_TIME,
    CFG_BLE_VITERBI_MODE,
    CFG_BLE_LL_ONLY,
    0}
  };

  ipccdba = READ_BIT( FLASH->IPCCBR, FLASH_IPCCBR_IPCCDBA );
  p_RefTable = (MB_RefTable_t*)((ipccdba<<2) + SRAM2A_BASE);

  tl_ble_init_conf.p_cmdbuffer = (uint8_t*)&BleCmdBuffer;
  tl_ble_init_conf.p_AclDataBuffer = HciAclDataBuffer;
  tl_ble_init_conf.IoBusEvtCallBack = TM_BleEvtRx;
  tl_ble_init_conf.IoBusAclDataTxAck = TM_AclDataAck;
  TL_BLE_Init( (void*) &tl_ble_init_conf );

  UTIL_LPM_SetOffMode(1 << CFG_LPM_APP_BLE, UTIL_LPM_DISABLE);
  UTIL_LPM_SetStopMode( 1<<CFG_LPM_APP_BLE, UTIL_LPM_DISABLE);
  LowPowerModeStatus = LOW_POWER_MODE_DISABLE;

  SysLocalCmdStatus = 0;

  SHCI_C2_BLE_Init( &ble_init_cmd_packet );

  UTIL_SEQ_RegTask( 1<< CFG_TASK_SYS_LOCAL_CMD_ID, UTIL_SEQ_RFU, TM_SysLocalCmd);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_BLE_HCI_CMD_ID, UTIL_SEQ_RFU, (void (*)( void )) TL_BLE_SendCmd);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_TX_TO_HOST_ID, UTIL_SEQ_RFU, TM_TxToHost);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SYS_HCI_CMD_ID, UTIL_SEQ_RFU, (void (*)( void )) TL_SYS_SendCmd);
  UTIL_SEQ_RegTask( 1<< CFG_TASK_HCI_ACL_DATA_ID, UTIL_SEQ_RFU, (void (*)( void )) TL_BLE_SendAclData);

  HostTxStatus = TX_DONE;
  pTxToHostPacket = 0;

  LST_init_head (&HostTxQueue);

  MX_USART1_UART_Init();

  pHostRx = RxHostData;
  HciReceiveStatus = WAITING_TYPE;

  HW_UART_Receive_IT(CFG_UART_GUI, pHostRx, 1, RxCpltCallback);

/* USER CODE BEGIN TM_Init_2 */

/* USER CODE END TM_Init_2 */
  return;
}

void TM_SysCmdRspCb (TL_EvtPacket_t * p_cmd_resp)
{
/* USER CODE BEGIN TM_SysCmdRspCb_1 */

/* USER CODE END TM_SysCmdRspCb_1 */
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
/* USER CODE BEGIN TM_SysCmdRspCb_2 */

/* USER CODE END TM_SysCmdRspCb_2 */
  return;
}

/* USER CODE BEGIN FD */
void TM_SetLowPowerMode( void )
{
  if(LowPowerModeStatus == LOW_POWER_MODE_DISABLE)
  {
    BSP_LED_Off(LED_GREEN);
    LowPowerModeStatus = LOW_POWER_MODE_ENABLE;
    UTIL_LPM_SetStopMode( 1<<CFG_LPM_APP_BLE, UTIL_LPM_ENABLE);
  }
  else
  {
    BSP_LED_On(LED_GREEN);
    LowPowerModeStatus = LOW_POWER_MODE_DISABLE;
    UTIL_LPM_SetStopMode( 1<<CFG_LPM_APP_BLE, UTIL_LPM_DISABLE);
  }
  return;
}
/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void TM_TxToHost( void )
{
  BACKUP_PRIMASK();

  if(HostTxStatus == TX_DONE)
  {
    HostTxStatus = TX_ONGOING;

    LST_remove_head( &HostTxQueue, (tListNode **)&pTxToHostPacket );

    if(LowPowerModeStatus == LOW_POWER_MODE_DISABLE)
    {
      if(pTxToHostPacket->evtserial.type == TL_ACL_DATA_PKT_TYPE)
      {
        /**
         * The uart interrupt shall be disable when the HAL is called to send data
         * This is because in the Rx uart handler, the HAL is called to receive new data
         */
        DISABLE_IRQ();
        HW_UART_Transmit_IT(CFG_UART_GUI, (uint8_t *)&((TL_AclDataPacket_t *)pTxToHostPacket)->AclDataSerial, ((TL_AclDataPacket_t *)pTxToHostPacket)->AclDataSerial.length + 5, HostTxCb);
        RESTORE_PRIMASK();
      }
      else
      {
        /**
         * The uart interrupt shall be disable when the HAL is called to send data
         * This is because in the Rx uart handler, the HAL is called to receive new data
         */
        DISABLE_IRQ();
        HW_UART_Transmit_IT(CFG_UART_GUI, (uint8_t *)&pTxToHostPacket->evtserial, pTxToHostPacket->evtserial.evt.plen + TL_EVT_HDR_SIZE, HostTxCb);
        RESTORE_PRIMASK();
      }
    }
    else
    {
      HostTxCb( );
    }
  }

  return;
}

static void TM_SysLocalCmd ( void )
{
  switch( SysLocalCmd.cmdserial.cmd.cmdcode )
  {
    case LHCI_OPCODE_C1_WRITE_REG:
      LHCI_C1_Write_Register( &SysLocalCmd );
      break;

    case LHCI_OPCODE_C1_READ_REG:
      LHCI_C1_Read_Register( &SysLocalCmd );
      break;

    case LHCI_OPCODE_C1_DEVICE_INF:
      LHCI_C1_Read_Device_Information( &SysLocalCmd );
      break;

    default:
      ((TL_CcEvt_t *)(((TL_EvtPacket_t*)&SysLocalCmd)->evtserial.evt.payload))->cmdcode = SysLocalCmd.cmdserial.cmd.cmdcode;
      ((TL_CcEvt_t *)(((TL_EvtPacket_t*)&SysLocalCmd)->evtserial.evt.payload))->payload[0] = 0x01;
      ((TL_CcEvt_t *)(((TL_EvtPacket_t*)&SysLocalCmd)->evtserial.evt.payload))->numcmd = 1;
      ((TL_EvtPacket_t*)&SysLocalCmd)->evtserial.type = TL_LOCRSP_PKT_TYPE;
      ((TL_EvtPacket_t*)&SysLocalCmd)->evtserial.evt.evtcode = TL_BLEEVT_CC_OPCODE;
      ((TL_EvtPacket_t*)&SysLocalCmd)->evtserial.evt.plen = TL_EVT_CS_PAYLOAD_SIZE;

      break;
  }

  LST_insert_tail (&HostTxQueue, (tListNode *)&SysLocalCmd);
  UTIL_SEQ_SetTask( 1<<CFG_TASK_TX_TO_HOST_ID,CFG_SCH_PRIO_0);

  return;
}

static void RxCpltCallback( void )
{
  uint16_t nb_bytes_to_receive=0;
  uint16_t buffer_index=0;
  uint8_t packet_indicator;

  switch (HciReceiveStatus)
  {
    case WAITING_TYPE:
    {
      packet_indicator = pHostRx[0];
      switch(packet_indicator)
      {
        case TL_BLECMD_PKT_TYPE:
        case TL_SYSCMD_PKT_TYPE:
        case TL_LOCCMD_PKT_TYPE:
          HciReceiveStatus = WAITING_LENGTH;
          nb_bytes_to_receive = 3;
          buffer_index = 1;
          break;

        case TL_ACL_DATA_PKT_TYPE:
          HciReceiveStatus = WAITING_LENGTH;
          nb_bytes_to_receive = 4;
          buffer_index = 1;
          break;

        default:
          nb_bytes_to_receive = 1;
          buffer_index = 0;
          break;
      }
    }
    break;

    case WAITING_LENGTH:
      packet_indicator = pHostRx[0];

      switch( packet_indicator )
      {
        case TL_SYSCMD_PKT_TYPE:
          nb_bytes_to_receive = pHostRx[3];
          pHostRx = (uint8_t*)&(((TL_CmdPacket_t*)(p_RefTable->p_sys_table->pcmd_buffer))->cmdserial);
          memcpy(pHostRx, RxHostData, 4);
          buffer_index = 4;
          break;

        case TL_LOCCMD_PKT_TYPE:
          nb_bytes_to_receive = pHostRx[3];
          pHostRx = (uint8_t*)&SysLocalCmd.cmdserial;
          memcpy(pHostRx, RxHostData, 4);
          buffer_index = 4;
          break;

        case TL_ACL_DATA_PKT_TYPE:
          nb_bytes_to_receive = pHostRx[3] + (pHostRx[4] << 8);
          pHostRx = (uint8_t*)&(((TL_AclDataPacket_t*)(p_RefTable->p_ble_table->phci_acl_data_buffer))->AclDataSerial);
          memcpy(pHostRx, RxHostData, 5);
          buffer_index = 5;
          break;

        default:
          nb_bytes_to_receive = pHostRx[3];
          pHostRx = (uint8_t*)&(((TL_CmdPacket_t*)(p_RefTable->p_ble_table->pcmd_buffer))->cmdserial);
          memcpy(pHostRx, RxHostData, 4);
          buffer_index = 4;
          break;
      }

      if(nb_bytes_to_receive)
      {
        HciReceiveStatus = WAITING_PAYLOAD;
      }
      else
      {
        switch ( packet_indicator )
        {
          case TL_SYSCMD_PKT_TYPE:
            UTIL_SEQ_SetTask( 1<<CFG_TASK_SYS_HCI_CMD_ID,CFG_SCH_PRIO_0);
            break;

          case TL_LOCCMD_PKT_TYPE:
            UTIL_SEQ_SetTask( 1<<CFG_TASK_SYS_LOCAL_CMD_ID,CFG_SCH_PRIO_0);
            break;

          case TL_ACL_DATA_PKT_TYPE:
            UTIL_SEQ_SetTask( 1<<CFG_TASK_HCI_ACL_DATA_ID,CFG_SCH_PRIO_0);
            break;

          default:
            UTIL_SEQ_SetTask( 1<<CFG_TASK_BLE_HCI_CMD_ID,CFG_SCH_PRIO_0);
            break;
        }

        HciReceiveStatus = WAITING_TYPE;
        nb_bytes_to_receive = 1;
        buffer_index = 0;
        pHostRx = RxHostData;
      }
      break;

        case WAITING_PAYLOAD:
          packet_indicator = pHostRx[0];

          switch ( packet_indicator )
          {
            case TL_SYSCMD_PKT_TYPE:
              UTIL_SEQ_SetTask( 1<<CFG_TASK_SYS_HCI_CMD_ID,CFG_SCH_PRIO_0);
              break;

            case TL_LOCCMD_PKT_TYPE:
              UTIL_SEQ_SetTask( 1<<CFG_TASK_SYS_LOCAL_CMD_ID,CFG_SCH_PRIO_0);
              break;

            case TL_ACL_DATA_PKT_TYPE:
              UTIL_SEQ_SetTask( 1<<CFG_TASK_HCI_ACL_DATA_ID,CFG_SCH_PRIO_0);
              break;

            default:
              UTIL_SEQ_SetTask( 1<<CFG_TASK_BLE_HCI_CMD_ID,CFG_SCH_PRIO_0);
              break;
          }

          HciReceiveStatus = WAITING_TYPE;
          nb_bytes_to_receive = 1;
          buffer_index = 0;
          pHostRx = RxHostData;

          break;

            default:
              break;
  }

  HW_UART_Receive_IT(CFG_UART_GUI, &pHostRx[buffer_index], nb_bytes_to_receive, RxCpltCallback);

  return;
}

static void HostTxCb( void )
{
  HostTxStatus = TX_DONE;

  if( (pTxToHostPacket >= (TL_EvtPacket_t *)(p_RefTable->p_mem_manager_table->blepool)) && (pTxToHostPacket < ( (TL_EvtPacket_t *)((p_RefTable->p_mem_manager_table->blepool) + p_RefTable->p_mem_manager_table->blepoolsize))))
  {
    TL_MM_EvtDone(pTxToHostPacket);
  }

  if ( LST_is_empty( &HostTxQueue ) == FALSE )
  {
    UTIL_SEQ_SetTask( 1<<CFG_TASK_TX_TO_HOST_ID,CFG_SCH_PRIO_0 );
  }

  return;
}

static void TM_BleEvtRx( TL_EvtPacket_t *phcievt )
{
  LST_insert_tail ( &HostTxQueue, (tListNode *)phcievt );

  UTIL_SEQ_SetTask( 1<<CFG_TASK_TX_TO_HOST_ID,CFG_SCH_PRIO_0 );

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

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
/*************************************************************
 *
 * WRAP FUNCTIONS
 *
 *************************************************************/
void shci_send( uint16_t cmd_code, uint8_t len_cmd_payload, uint8_t * p_cmd_payload, TL_EvtPacket_t * p_rsp_status )
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
  memcpy( &(p_rsp_status->evtserial), p_cmd_buffer, ((TL_EvtSerial_t*)p_cmd_buffer)->evt.plen + TL_EVT_HDR_SIZE );

  return;
}


/* USER CODE BEGIN FD_WRAP_FUNCTIONS*/

/* USER CODE END FD_WRAP_FUNCTIONS*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
