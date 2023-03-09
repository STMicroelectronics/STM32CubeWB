/* USER CODE BEGIN Header */
/**
  ******************************************************************************
 * @file    crc_app.c
 * @author  MCD Application Team
 * @brief   Cable Replacement Client Application
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
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "crc_app.h"
#include "stm32_seq.h"
#include "app_ble.h"
#include "DispTools.h"
#include "stm_queue.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{  
  CRC_UNINITIALIZED                                 = 0,
  CRC_INITIALIZED                                   = 1,
  CRC_IDLE                                          = 2,
  CRC_DISC_ALL_PRIMARY_SERVICES                     = 3,
  CRC_DISCOVER_CHARACS                              = 4,
  CRC_DISCOVER_DESC                                 = 5,
  CRC_READ_TX                                       = 6,
  CRC_READ_RX                                       = 7,
  CRC_READ_RX_CCC                                   = 8,
  CRC_WRITE_TX                                      = 9,
  CRC_ENABLE_RX_NOTIFICATION                        = 10,
  CRC_DISABLE_RX_NOTIFICATION                       = 11,
  CRC_CONNECTED                                     = 12,
  CRC_CONNECTED_ADVERTISING                         = 13,
  CRC_PROC_MTU_UPDATE                               = 14
}  CRC_SVC_ProfileState;
  
typedef struct
{
  /**
   * state of the CRC
   * state machine
   */ 
  CRC_SVC_ProfileState state;

  /**
   * connection handle 
   */ 
  uint16_t connHandle;

  /**
   * handle of the CRC service
   */ 
  uint16_t ServiceHandle;

  /**
   * end handle of the CRC service
   */ 
  uint16_t ServiceEndHandle;

  /**
   * handle of the TX characteristic 
   * 
   */ 
  uint16_t TXCharHdle;

  /**
   * handles of the RX characteristic 
   * 
   */ 
  uint16_t RXCharHdle;

  /**
   * handles of the RX Client Characteristic Configuration descriptor 
   * 
   */ 
  uint16_t RXCCCDescHdle;

}CRC_Context_t;
/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UNPACK_2_BYTE_PARAMETER(ptr)  \
        (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
        (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

#define READ_RX                                                                0
#define READ_TX                                                                1
#define READ_RX_CCC_DESC                                                       2
          
#define WRITE_TX                                                               0
#define WRITE_ENABLE_RX_NOTIFICATION                                           1
#define WRITE_DISABLE_RX_NOTIFICATION                                          2

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

#define TX_CHAR                                                                1
#define RX_CHAR                                                                2
/* USER CODE END PM */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/**
 * START of Section BLE_APP_CONTEXT
 */

static CRC_Context_t CRC_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */
static uint8_t InputCharFromUart; 
static queue_t RxQueue;
static uint8_t a_RxQueueBuffer[RX_BUFFER_SIZE];  
static uint8_t PosXTx, PosYTx, PosXRx, PosYRx;
static char a_SzString[MAX_STRING_SIZE+1];
static uint8_t idx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private functions prototypes-----------------------------------------------*/
static void CRCAPP_Terminal_UART_RxCpltCallback( void );
static void CRCAPP_Terminal_Init(void);
static SVCCTL_EvtAckStatus_t CRCAPP_Event_Handler(void *p_Event);
#if(BLE_CFG_PERIPHERAL != 1)
static void GattProcReq(CRC_SVC_ProfileState GattProcId);
#endif
static void CRCAPP_Transmit(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/*************************************************************
 *
 * PUBLIC FUNCTIONS
 *
 *************************************************************/
/**
 * @brief  Application initialization
 * @param  None
 * @retval None
 */
void CRCAPP_Init(void)
{
  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterCltHandler(CRCAPP_Event_Handler);

  UTIL_SEQ_RegTask( 1<< CFG_TASK_CRC_TX_REQ_ID, UTIL_SEQ_RFU, CRCAPP_Transmit );

  PosXRx = POSXRX;
  PosYRx = POSYRX + 2;
  PosXTx = POSXTX;
  PosYTx = POSYTX + 2;

  CRCAPP_Terminal_Init();

  ClearScreen();

  CRC_Context.state = CRC_IDLE;
  CRC_Context.connHandle = 0xFFFF;

  APP_DBG_MSG("-- CR CLIENT INITIALIZED \n");
  return;
}


#if 0
/**
 * @brief  Service update
 * @param  None
 * @retval None
 */
void CRCAPP_Update_Service()
{
  tBleStatus result;
  uint8_t index;
  
  index = 0;
  while((index < CFG_MAX_CONNECTION) &&
        (CRC_Context.state != CRC_IDLE))
  {
    switch(CRC_Context.state)
    {
      case CRC_CONNECTED:
        {
          APP_DBG_MSG("CRC_CONNECTED\n");
          if(APP_BLE_Get_Client_Connection_Status(CRC_Context.connHandle) == APP_BLE_IDLE)
          {
            APP_DBG_MSG("Handle deconnected !\n");
            CRC_Context.state = CRC_IDLE;
            CRC_Context.connHandle = 0xFFFF;
          }
        }
        break;
        
      case CRC_DISCOVER_CHARACS:
        {
          APP_DBG_MSG("CRC_DISCOVER_CHARACS\n");

          result = aci_gatt_disc_all_char_of_service(CRC_Context.connHandle,
                                                     CRC_Context.ServiceHandle,
                                                     CRC_Context.ServiceEndHandle);

          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("All characteristics discovery sent successfully \n");
          }
          else 
          {
            APP_DBG_MSG("All characteristics discovery sending failed with result: 0x%x\n", result);
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_IDLE; /* redo a CRC_DISCOVER_CHARACS */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_DISCOVER_CHARACS\n");
            }
          }
        }
        break;
        
      case CRC_DISCOVER_DESC:
        {
          APP_DBG_MSG("CRC_DISCOVER_DESC\n");

          result = aci_gatt_disc_all_char_desc(CRC_Context.connHandle,
                                               CRC_Context.ServiceHandle,
                                               CRC_Context.ServiceEndHandle);
        
          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("Descriptors discovery sent successfully \n");
          }
          else 
          {
            APP_DBG_MSG("Descriptors discovery sending failed \n");
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_DISCOVER_CHARACS; /* redo a CRC_DISCOVER_DESC */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_DISCOVER_DESC\n");
            }
          }
        }
        break;
        
      case CRC_READ_RX:
        {
          APP_DBG_MSG("CRC_READ_TEMPERATURE_TYPE\n");

          result = aci_gatt_read_char_value(CRC_Context.connHandle,
                                            CRC_Context.RXCharHdle);
          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("Read CRC Temperature Type sent successfully \n");
          }
          else 
          {
            APP_DBG_MSG("Read CRC RX sending failed \n");
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_READ_RX; /* redo a CRC_READ_RX */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_READ_RX\n");
            }
          }
        }
        break;
        
      case CRC_READ_TX:
        {
          APP_DBG_MSG("CRC_READ_TX\n");

          result = aci_gatt_read_char_value(CRC_Context.connHandle,
                                            CRC_Context.TXCharHdle);
          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("Read CRC TX sent Successfully \n");
          }
          else 
          {
            APP_DBG_MSG("Read TX sent Failed \n");
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_READ_TX; /* redo a CRC_READ_TX */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_READ_TX\n");
            }
          }
        }
        break;
        
      case CRC_READ_RX_CCC:
        {
          APP_DBG_MSG("CRC_READ_TEMPERATURE_MEASUREMENT_CCC\n");

          result = aci_gatt_read_char_desc(CRC_Context.connHandle,
                                           CRC_Context.RXCCCDescHdle);
          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("Read CRC RX CCCC sent successfully \n");
          }
          else 
          {
            APP_DBG_MSG("Read RX sending failed \n");
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_READ_RX_CCC; /* redo a CRC_READ_RX_CCC */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_READ_RX_CCC\n");
            }
          }
        }
        break;
        
      case CRC_WRITE_TX:
        {
          APP_DBG_MSG("CRC_WRITE_TX\n");

          result = aci_gatt_write_without_resp(CRC_Context.connHandle,
                                               CRC_Context.TXCharHdle,
                                               strlen(a_SzString),
                                               (uint8_t *) &a_SzString[0]);

          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("Write CRC TX sent Successfully \n");
            CRC_Context.state = CRC_CONNECTED;
            APP_DBG_MSG("CRC_WRITE_TX -> CRC_CONNECTED\n");
            UTIL_SEQ_SetTask( 1<<CFG_TASK_CRC_DISCOVERY_REQ_ID, CFG_SCH_PRIO_0);
          }
          else 
          {
            APP_DBG_MSG("Write TX sent Failed \n");
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_WRITE_TX; /* redo a CRC_WRITE_TX */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_WRITE_TX\n");
            }
          }
        }
        break;
               
      case CRC_ENABLE_RX_NOTIFICATION:
        {
          uint8_t a_Notification[2] = {0x01, 0x00};

          APP_DBG_MSG("CRC_ENABLE_RX_NOTIFICATION\n");

          result = aci_gatt_write_char_desc(CRC_Context.connHandle,
                                            CRC_Context.RXCCCDescHdle,
                                            2,
                                            (uint8_t *) &a_Notification[0]);
        
          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("Enable CRC RX Notification Sent Successfully \n");
          }
          else 
          {
            APP_DBG_MSG("Enable CRC RX Notification Sent Failed \n");
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_ENABLE_RX_NOTIFICATION; /* redo a CRC_ENABLE_RX_NOTIFICATION */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_ENABLE_RX_NOTIFICATION\n");
            }
          }
        }
        break;
        
      case CRC_DISABLE_RX_NOTIFICATION:
        {
          uint8_t a_Notification[2] = {0x00, 0x00};
          
          APP_DBG_MSG("CRC_DISABLE_RX_NOTIFICATION\n");

          result = aci_gatt_write_char_desc(CRC_Context.connHandle,
                                            CRC_Context.RXCCCDescHdle,
                                            2,
                                            (uint8_t *) &a_Notification[0]);
          if( result == BLE_STATUS_SUCCESS )
          {
            APP_DBG_MSG("Disable CRC RX Notification Sent Successfully \n");
          }
          else 
          {
            APP_DBG_MSG("Disable CRC RX Notification Sent Failed \n");
            if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
            {
              CRC_Context.state = CRC_DISABLE_RX_NOTIFICATION; /* redo a CRC_DISABLE_RX_NOTIFICATION */
              APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_DISABLE_RX_NOTIFICATION\n");
            }
          }
        }
        break;

      default:
        break;
    }
    index++;
  }
}
#endif

void CRCAPP_LinkReadyNotification(uint16_t ConnectionHandle)
{
  CRC_Context.connHandle = ConnectionHandle;

#if(BLE_CFG_PERIPHERAL != 1)
  GattProcReq(CRC_DISC_ALL_PRIMARY_SERVICES);
  GattProcReq(CRC_DISCOVER_CHARACS);
  GattProcReq(CRC_DISCOVER_DESC);
  GattProcReq(CRC_ENABLE_RX_NOTIFICATION);
#endif

  return;
}

/**
 * @brief  Feature Characteristic write
 * @param  index: characteristic index
 * @retval None
 */
static void CRCAPP_Transmit(void)
{
  tBleStatus result;

  APP_DBG_MSG("CRCAPP_Transmit\n");

  result = aci_gatt_write_without_resp(CRC_Context.connHandle,
                                       CRC_Context.TXCharHdle,
                                       strlen(a_SzString),
                                       (uint8_t *) &a_SzString[0]);

  if( result == BLE_STATUS_SUCCESS )
  {
    APP_DBG_MSG("Write CRC TX Successfully \n");
//    CRC_Context.state = CRC_CONNECTED;
//    APP_DBG_MSG("CRC_WRITE_TX -> CRC_CONNECTED\n");
//    UTIL_SEQ_SetTask( 1<<CFG_TASK_CRC_DISCOVERY_REQ_ID, CFG_SCH_PRIO_0);
  }
  else 
  {
    APP_DBG_MSG("Write CRC TX Failed \n");
//    if(result == HCI_COMMAND_DISALLOWED_ERR_CODE)
//    {
//      CRC_Context.state = CRC_WRITE_TX; /* redo a CRC_WRITE_TX */
//      APP_DBG_MSG("HCI_COMMAND_DISALLOWED_ERR_CODE do another CRC_WRITE_TX\n");
//    }
  }

}/* end CRCAPP_Write_Char() */


/**
  * @brief  This function handles USARTx interrupt request.
  * @param  None
  * @retval None
  */
static void CRCAPP_Terminal_UART_RxCpltCallback( void )
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
    a_SzString[idx] = '\0';
    idx = 0;
    if(PosYTx > POSYTX + YSIZE + 2)
      PosYTx = POSYTX + 2;
    ClearLines(PosYTx, 1);
    PrintPcCrt(PosXTx, PosYTx, "");
//    CRCAPP_Write_Char(WRITE_TX);
    UTIL_SEQ_SetTask(1 << CFG_TASK_CRC_TX_REQ_ID, CFG_SCH_PRIO_0);
  }
  else
  {
    char a_Car[2];
    
    a_Car[0] = InputCharFromUart;
    a_Car[1] = 0;
    PrintPcCrt(PosXTx++, PosYTx, "%s", &a_Car[0]);
    a_SzString[idx++] = InputCharFromUart;

    if(idx > MAX_STRING_SIZE - 1)
    {
      a_SzString[idx] = '\0'; 
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
//      CRCAPP_Write_Char(WRITE_TX);
      UTIL_SEQ_SetTask(1 << CFG_TASK_CRC_TX_REQ_ID, CFG_SCH_PRIO_0);
    }
  }

  HW_UART_Receive_IT(CFG_CONSOLE_MENU, &InputCharFromUart, 1, CRCAPP_Terminal_UART_RxCpltCallback);
  return;
 }

/**
  * @brief  This function initialize terminal
  * @param  None
  * @retval None
  */
static void CRCAPP_Terminal_Init(void)
{
  HW_UART_Init(CFG_CONSOLE_MENU);

  CircularQueue_Init(&RxQueue, a_RxQueueBuffer, RX_BUFFER_SIZE, 1, CIRCULAR_QUEUE_NO_WRAP_FLAG); 
  
  HW_UART_Receive_IT(CFG_CONSOLE_MENU, &InputCharFromUart, 1, CRCAPP_Terminal_UART_RxCpltCallback);

  return;
}


/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t CRCAPP_Event_Handler(void *p_Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *p_event_pckt;
  evt_blecore_aci *p_blecore_evt;

  return_value = SVCCTL_EvtNotAck;
  p_event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)p_Event)->data);

  switch(p_event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      {
        p_blecore_evt = (evt_blecore_aci*)p_event_pckt->data;

        APP_DBG_MSG("HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE: 0x%x!\n",
                    p_blecore_evt->ecode);
        
        switch(p_blecore_evt->ecode)
        {

          case ACI_ATT_READ_BY_GROUP_TYPE_RESP_VSEVT_CODE:
            {
              aci_att_read_by_group_type_resp_event_rp0 *p_pr = (void*)p_blecore_evt->data;
              uint8_t numServ, i, idx;
              uint16_t handle;

              handle = p_pr->Connection_Handle;

              APP_DBG_MSG("CRC_Event_Handler: ACI_ATT_READ_BY_GROUP_TYPE_RESP_VSEVT_CODE\n");

              CRC_Context.connHandle = handle;

              numServ = (p_pr->Data_Length) / p_pr->Attribute_Data_Length;

              /* the event data will be
               * 2bytes start handle
               * 2bytes end handle
               * 2 or 16 bytes data
               * we are interested only if the UUID is 128 bits.
               * So check if the data length is 20
               */
              if (p_pr->Attribute_Data_Length == 20)
              {
                idx = 16;
                for (i = 0; i < numServ; i++)
                {
                  uint8_t j;
                  uint8_t a_Service_uuid[] = { CRS_STM_UUID128 };
                  uint8_t result = TRUE;
                    
                  for(j = 0; j < p_pr->Attribute_Data_Length - 4; j++)
                  {
                    if(p_pr->Attribute_Data_List[j+4] != a_Service_uuid[(p_pr->Attribute_Data_Length-5) - j])
                    {
                      result = FALSE;
                      APP_DBG_MSG("Service UUID is not a Cable Replacement Service\n");
                      break;
                    }
                  }
                  if(result == TRUE)
                  {
                    CRC_Context.ServiceHandle = 
                      UNPACK_2_BYTE_PARAMETER(&p_pr->Attribute_Data_List[idx-16]);
                    CRC_Context.ServiceEndHandle = 
                      UNPACK_2_BYTE_PARAMETER (&p_pr->Attribute_Data_List[idx-14]);
                    APP_DBG_MSG("*************************************************************************************************************************\n");
                    APP_DBG_MSG("* Found Cable Replacement service start handle 0x%04x end handle 0x%04x\n",
                                CRC_Context.ServiceHandle,
                                CRC_Context.ServiceEndHandle);
                      APP_DBG_MSG("*************************************************************************************************************************\n");
                  }
                  idx += 20;
                }
              }
            }
            break; /*ACI_ATT_READ_BY_GROUP_TYPE_RESP_VSEVT_CODE*/
            
          case ACI_ATT_READ_BY_TYPE_RESP_VSEVT_CODE:
            {
              aci_att_read_by_type_resp_event_rp0 *p_pr = (void*)p_blecore_evt->data;
              uint8_t idx;
              uint16_t handle;

              handle = UNPACK_2_BYTE_PARAMETER(&p_pr->Handle_Value_Pair_Data[0]);
              if((handle >= CRC_Context.ServiceHandle) &&
                 (handle <= CRC_Context.ServiceEndHandle))
              {
                /* Event for CRC Client */
                APP_DBG_MSG("ACI_ATT_READ_BY_TYPE_RESP_VSEVT_CODE\n");

                /* the event data will be
                 * 2 bytes start handle
                 * 1 byte char properties
                 * 2 bytes handle
                 * 2 or 16 bytes data
                 */
                idx = 17;
                /* we are interested in only 128 bits UUIDs */
                if (p_pr->Handle_Value_Pair_Length == 21)
                {
                  while(p_pr->Data_Length > 0)
                  {
                    uint8_t a_Tx_uuid[] = { CRS_STM_TX_UUID128 };
                    uint8_t a_Rx_uuid[] = { CRS_STM_RX_UUID128 };
                    uint8_t i, result = TX_CHAR | RX_CHAR;
                      
                    /* store the characteristic handle not the attribute handle */
                    handle = UNPACK_2_BYTE_PARAMETER(&p_pr->Handle_Value_Pair_Data[idx-14]);
                    for(i = 0; i < p_pr->Handle_Value_Pair_Length - 5; i++)
                    {
                      if(p_pr->Handle_Value_Pair_Data[i+5] != a_Tx_uuid[(p_pr->Handle_Value_Pair_Length-6) - i])
                      {
                        APP_DBG_MSG("Characteristic UUID is not a Tx UUID Characteristic\n");
                        APP_DBG_MSG("p_pr->Handle_Value_Pair_Data[i+5]: 0x%x != a_Tx_uuid[(p_pr->Handle_Value_Pair_Length-6) - i]: 0x%x\n",
                                    p_pr->Handle_Value_Pair_Data[i+5],
                                    a_Tx_uuid[(p_pr->Handle_Value_Pair_Length-6) - i]);
                        result &= RX_CHAR;    /* Not a TX Characteristic */
                      }
                      if(p_pr->Handle_Value_Pair_Data[i+5] != a_Rx_uuid[(p_pr->Handle_Value_Pair_Length-6) - i])
                      {
                        APP_DBG_MSG("Characteristic UUID is not a Rx UUID Characteristic\n");
                        APP_DBG_MSG("p_pr->Handle_Value_Pair_Data[i+5]: 0x%x != a_Rx_uuid[(p_pr->Handle_Value_Pair_Length-6) - i]: 0x%x\n",
                                    p_pr->Handle_Value_Pair_Data[i+5],
                                    a_Rx_uuid[(p_pr->Handle_Value_Pair_Length-6) - i]);
                        result &= TX_CHAR;   /* Not a RX Characteristic */
                      }
                      if(result == 0)
                        break;
                    }
                    if(result == TX_CHAR)
                    {
                      APP_DBG_MSG("*************************************************************************************************************************\n");
                      APP_DBG_MSG("TX start handle 0x%04x\n", UNPACK_2_BYTE_PARAMETER(&p_pr->Handle_Value_Pair_Data[idx-17]));
                      APP_DBG_MSG("TX properties   0x%02x\n", p_pr->Handle_Value_Pair_Data[idx-15]);
                      APP_DBG_MSG("TX handle       0x%04x\n", handle);
                      APP_DBG_MSG("TX uuid           0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", 
                                  a_Tx_uuid[0], a_Tx_uuid[1], a_Tx_uuid[2],  a_Tx_uuid[3],  a_Tx_uuid[4],  a_Tx_uuid[5],  a_Tx_uuid[6],  a_Tx_uuid[7], 
                                  a_Tx_uuid[8] ,a_Tx_uuid[9], a_Tx_uuid[10], a_Tx_uuid[11], a_Tx_uuid[12], a_Tx_uuid[13], a_Tx_uuid[14], a_Tx_uuid[15]);
                      APP_DBG_MSG("*************************************************************************************************************************\n");
                      CRC_Context.TXCharHdle = handle;
                    }

                    if(result == RX_CHAR)
                    {
                      APP_DBG_MSG("*************************************************************************************************************************\n");
                      APP_DBG_MSG("RX start handle 0x%04x\n", UNPACK_2_BYTE_PARAMETER(&p_pr->Handle_Value_Pair_Data[idx-17]));
                      APP_DBG_MSG("RX properties   0x%02x\n", p_pr->Handle_Value_Pair_Data[idx-15]);
                      APP_DBG_MSG("RX handle       0x%04x\n", handle);
                      APP_DBG_MSG("RX uuid           0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", 
                                  a_Rx_uuid[0], a_Rx_uuid[1], a_Rx_uuid[2],  a_Rx_uuid[3],  a_Rx_uuid[4],  a_Rx_uuid[5],  a_Rx_uuid[6],  a_Rx_uuid[7], 
                                  a_Rx_uuid[8] ,a_Rx_uuid[9], a_Rx_uuid[10], a_Rx_uuid[11], a_Rx_uuid[12], a_Rx_uuid[13], a_Rx_uuid[14], a_Rx_uuid[15]);
                      APP_DBG_MSG("*************************************************************************************************************************\n");
                      CRC_Context.RXCharHdle = handle;
                    }
                    p_pr->Data_Length -= 21;
                    idx += 21;
                  } /* end while(p_pr->Event_Data_Length > 0) */
                } /* end if (p_pr->Handle_Value_Pair_Data == 21) */
              } /* end if(CRC_Context.state ... */
            } 
            break; /*ACI_ATT_READ_BY_TYPE_RESP_VSEVT_CODE*/
            
          case ACI_ATT_FIND_INFO_RESP_VSEVT_CODE:
            {
              aci_att_find_info_resp_event_rp0 *p_pr = (void*)p_blecore_evt->data;
              uint8_t numDesc, idx, i;
              uint16_t uuid, handle;

              handle = UNPACK_2_BYTE_PARAMETER(&p_pr->Handle_UUID_Pair[0]);
              if((handle >= CRC_Context.ServiceHandle) &&
                 (handle <= CRC_Context.ServiceEndHandle))
              {
                /* Event for CRC Client */
                APP_DBG_MSG("ACI_ATT_FIND_INFO_RESP_VSEVT_CODE\n");

                /*
                 * event data will be of the format
                 * 2 bytes handle
                 * 2 bytes UUID
                 */
                numDesc = (p_pr->Event_Data_Length) / 4;
                APP_DBG_MSG("CRC Number of descriptors: %d\n", numDesc);
                  
                /* we are interested only in 16 bit UUIDs */
                idx = 0;
                if (p_pr->Format == UUID_TYPE_16)
                {
                  for (i = 0; i < numDesc; i++)
                  {
                    handle = UNPACK_2_BYTE_PARAMETER(&p_pr->Handle_UUID_Pair[idx]);
                    uuid = UNPACK_2_BYTE_PARAMETER(&p_pr->Handle_UUID_Pair[idx+2]);

                    APP_DBG_MSG("UUID: 0x%x Handle: 0x%x\n", uuid, handle);
                    
                    if(uuid == CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID)
                    {
                      if( (CRC_Context.RXCharHdle)+1 == handle)
                      {
                        APP_DBG_MSG("*************************************************************************************************************************\n");
                        APP_DBG_MSG("RX Client Characteristic Configuration Desc handle 0x%04x\n", handle);
                        APP_DBG_MSG("RX Client Characteristic Configuration Desc uuid   0x%04x\n", uuid);
                        APP_DBG_MSG("*************************************************************************************************************************\n");
                        CRC_Context.RXCCCDescHdle = handle;
                      }
                    } /* end if(uuid == CLIENT_CHAR_CONFIG_DESCRIPTOR_UUID) */
                    idx += 4;
                  } /* end for (i = 0; i < numDesc; i++) */
                } /* end if (p_pr->Format == UUID_TYPE_16) */
              } /* end if(CRC_Context.connHandle == p_pr->Connection_Handle) */
            }
            break; /*ACI_ATT_FIND_INFO_RESP_VSEVT_CODE*/
            
          case ACI_ATT_READ_RESP_VSEVT_CODE:
            {
              aci_att_read_resp_event_rp0 *p_pr = (void*)p_blecore_evt->data;

              if(CRC_Context.state == CRC_READ_TX)
              {
                /* Event for CR Client */
                APP_DBG_MSG("ACI_ATT_READ_RESP_VSEVT_CODE\n");
                APP_DBG_MSG("*************************************************************************************************************************\n");
                APP_DBG_MSG("CRC TX 0x%x:\n",
                             CRC_Context.TXCharHdle);
                APP_DBG_MSG("*************************************************************************************************************************\n");
              }
              else if(CRC_Context.state == CRC_READ_RX)
              {
                /* Event for CR Client */
                APP_DBG_MSG("ACI_ATT_READ_RESP_VSEVT_CODE\n");
                APP_DBG_MSG("*************************************************************************************************************************\n");
                APP_DBG_MSG("CRC RX 0x%x:\n", 
                             CRC_Context.RXCharHdle);
                APP_DBG_MSG("*************************************************************************************************************************\n");
              }
              else if(CRC_Context.state == CRC_READ_RX_CCC)
              {
                /* Event for CR Client */
                APP_DBG_MSG("ACI_ATT_READ_RESP_VSEVT_CODE\n");
                APP_DBG_MSG("*************************************************************************************************************************\n");
                APP_DBG_MSG("CRC RX CCC 0x%x: 0x%04x\n", 
                             CRC_Context.RXCCCDescHdle,
                             p_pr->Attribute_Value[0] + (p_pr->Attribute_Value[1] << 8));
                APP_DBG_MSG("*************************************************************************************************************************\n");
              }
            }
            break; /*ACI_ATT_READ_RESP_VSEVT_CODE*/
            
          case ACI_GATT_NOTIFICATION_VSEVT_CODE:
            {
              aci_gatt_notification_event_rp0 *p_pr = (void*)p_blecore_evt->data;

              if((p_pr->Attribute_Handle >= CRC_Context.ServiceHandle) &&
                 (p_pr->Attribute_Handle <= CRC_Context.ServiceEndHandle))
              {
                /* Event for CRC Client */
                APP_DBG_MSG("ACI_GATT_NOTIFICATION_VSEVT_CODE on connection handle 0x%x\n",
                            p_pr->Connection_Handle);
                if(p_pr->Attribute_Handle == CRC_Context.RXCharHdle)
                {
                  uint8_t a_Text[21], i;
                    
                  /* the event buffer will have the data as follows:
                   * category ID - 1 byte
                   * number of alerts for that category - 1 byte
                   * text string information - optional(varying number of bytes : the rest
                   * of the available bytes as indicated by length has to be considered
                   * as the string
                   */
                  APP_DBG_MSG("p_pr->Attribute_Value_Length %d\n", p_pr->Attribute_Value_Length);
                  for(i = 0; i < p_pr->Attribute_Value_Length; i++)
                    a_Text[i] = p_pr->Attribute_Value[i];
                  a_Text[p_pr->Attribute_Value_Length] = '\0';
                  APP_DBG_MSG("*************************************************************************************************************************\n");
                  APP_DBG_MSG("%s\n", &(a_Text[0]));
                  APP_DBG_MSG("*************************************************************************************************************************\n");
                  if((PosXRx >= XSIZE) || (p_pr->Attribute_Value_Length < MAX_STRING_SIZE))
                  {
                    PosXRx = POSXRX;
                    PosYRx++;
                    if(PosYRx > POSYRX + YSIZE + 2)
                      PosYRx = POSYRX + 2;
                    ClearLines(PosYRx, 1);
                  }
                  PrintPcCrt(PosXRx, PosYRx, "%s", &(a_Text[0]));
                  PosXRx += p_pr->Attribute_Value_Length;
                  PrintPcCrt(PosXTx, PosYTx, "");
                }
              }
            }
            break; /*ACI_GATT_INDICATION_VSEVT_CODE*/
            
          case ACI_GATT_PROC_COMPLETE_VSEVT_CODE:
            {
              UTIL_SEQ_SetEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);
            }
            break; /*ACI_GATT_PROC_COMPLETE_VSEVT_CODE*/

            
          case ACI_GATT_ERROR_RESP_VSEVT_CODE:
            {
              aci_gatt_error_resp_event_rp0 *p_pr = (void*)p_blecore_evt->data;

              if((CRC_Context.state == CRC_DISCOVER_CHARACS) &&
                 (p_pr->Attribute_Handle >= CRC_Context.ServiceHandle) &&
                 (p_pr->Attribute_Handle <= CRC_Context.ServiceEndHandle))
              {
                APP_DBG_MSG("ACI_GATT_ERROR_RESP_VSEVT_CODE\n");
              }
              else if((CRC_Context.state == CRC_IDLE) &&
                      (p_pr->Attribute_Handle >= CRC_Context.ServiceHandle) &&
                      (p_pr->Attribute_Handle <= CRC_Context.ServiceEndHandle))
              {
                APP_DBG_MSG("ACI_GATT_ERROR_RESP_VSEVT_CODE\n");
              }
            }
            break; /*ACI_GATT_ERROR_RESP_VSEVT_CODE*/
            
          default:
            break;
        }
      }
      break; /* HCI_HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE_SPECIFIC */
      
    default:
      break;
  }

  return(return_value);
}/* end CRCAPP_Event_Handler */


/* Public functions ----------------------------------------------------------*/
#if(BLE_CFG_PERIPHERAL != 1)
static void GattProcReq(CRC_SVC_ProfileState GattProcId)
{
  tBleStatus status;

  switch(GattProcId)
  {
    case CRC_PROC_MTU_UPDATE:
      {
        APP_DBG_MSG("change ATT MTU size \n");

        status = aci_gatt_exchange_config(CRC_Context.connHandle);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("change MTU cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("CRC_PROC_MTU_UPDATE complete event received \n");
      }
      break;

    case CRC_DISC_ALL_PRIMARY_SERVICES:
      {
        APP_DBG_MSG("Discover all primary services \n");

        status = aci_gatt_disc_all_primary_services(CRC_Context.connHandle);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("Discover all primary services cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("DTC_DISC_ALL_PRIMARY_SERVICES complete event received \n");
      }
      break;

    case CRC_DISCOVER_CHARACS:
      {
        APP_DBG_MSG("Discover all char of service \n");

        status = aci_gatt_disc_all_char_of_service(
            CRC_Context.connHandle,
            CRC_Context.ServiceHandle,
            CRC_Context.ServiceEndHandle);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("Discover all char of service cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("CRC_DISCOVER_CHARACS complete event received \n");
      }
      break;

    case CRC_DISCOVER_DESC:
      {
        APP_DBG_MSG("Discover char descriptors \n");

        status = aci_gatt_disc_all_char_desc(
            CRC_Context.connHandle,
            CRC_Context.ServiceHandle,
            CRC_Context.ServiceEndHandle);
        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("Discover char descriptors cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        APP_DBG_MSG("DTC_DISCOVER_DESC complete event received \n");
      }
      break;

    case CRC_ENABLE_RX_NOTIFICATION:
      {
        uint8_t a_Notification[2] = {0x01, 0x00};

        APP_DBG_MSG("Enable Rx char descriptors \n");
        status = aci_gatt_write_char_desc(CRC_Context.connHandle,
                                          CRC_Context.RXCCCDescHdle,
                                          2,
                                          (uint8_t *) &a_Notification[0]);

        if (status != BLE_STATUS_SUCCESS)
        {
          APP_DBG_MSG("Enable Rx char descriptors cmd failure: 0x%x\n", status);
        }
        UTIL_SEQ_WaitEvt(1 << CFG_IDLEEVT_GATT_PROC_COMPLETE);

        ClearScreen();
        PrintPcCrt(POSXRX, POSXRX, "Receive:");
        PrintPcCrt(POSXTX, POSYTX, "Transmit:");
        PrintPcCrt(PosXTx, PosYTx, "");

        APP_DBG_MSG("CRC_ENABLE_RX_NOTIFICATION complete event received \n");
      }
      break;

    default:
      break;
  }
  return;
}
#endif

