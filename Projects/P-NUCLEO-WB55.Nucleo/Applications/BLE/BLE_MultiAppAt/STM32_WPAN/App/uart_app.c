/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    uart_app.c
 * @author  MCD Application Team
 * @brief   UART Application
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
#include "dbg_trace.h"
#include "ble.h"
#include "uart_app.h"
#include "stm32_seq.h"
#include "app_ble_common.h"
#include "p2p_client_app.h"

#if (CFG_HW_USART1_ENABLED == 1)
extern void MX_USART1_UART_Init(void);
extern UART_HandleTypeDef huart1;
#endif
#if (CFG_HW_LPUART1_ENABLED == 1)
extern void MX_LPUART1_UART_Init(void);
extern UART_HandleTypeDef hlpuart1;
#endif

extern APP_Mode_t APP_MODE;

extern APP_BLE_ConnStatus_t SV_Connection_Status;
extern APP_BLE_ConnStatus_t CL_Connection_Status;
extern APP_BLE_ConnStatus_t HR_Connection_Status;

extern tBDAddr SERVER_REMOTE_BDADDR[5];
extern uint8_t index_bd_addr;

PairingContext_t PairingContext;

APP_Mode_t Next_Mode;

uint8_t WriteCharData[2];
uint8_t NotifyCharData[2];

uint8_t BD_Addr[6];

Disconnection_Status_t disconnection_status;

HR_Notify_Context_t HR_Notify_Context;

uint16_t Connection_Update_Interval;

uint8_t autoconn_status;

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* AT Commands */

typedef enum
{
  AT = 0,
  AT_SV,
  AT_SV_ADV_START,
  AT_SV_ADV_STOP,
  AT_SV_NOTIFY,
  AT_SV_CONN_UPD,
  AT_CL,
  AT_CL_WRITE,
  AT_CL_SCAN,
  AT_CL_CONN,
  AT_CL_DISCONN,
  AT_CL_AUTOCONN,
  AT_CL_EN,
  AT_CL_DIS,
  AT_HR,
  AT_HR_NOTIFY,
  AT_PAIRING_START,
  AT_PAIRING_CONFIRM,
  AT_CLEAR_BONDING,
  AT_END_CMD,
} AT_Cmd_Type_t;

typedef enum
{
  NO_PARAM = 0,
  PARAM,
} Param_t;

typedef enum
{
  NO_MULTI_PARAM = 0,
  MULTI_PARAM,
} Multi_Param_state_t;

typedef struct {
  Multi_Param_state_t state;
  uint8_t index;
} Multi_Param_t;

typedef struct {
  char *ATCmdStr;
  int SizeATCmdStr;
  AT_Cmd_Type_t AT_Cmd_Type;
} AT_CMD_t;

/* Private defines ------------------------------------------------------------*/
/* Private macros -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static char pDataRx[32];  /* Buffer used for AT cmd reception */
static uint8_t indexRx = 0;
static uint8_t pCharRx[5]; /* Buffer used for Rx input character */

static Param_t param;
static Multi_Param_t multi_param;

/*
 * Used to compare the command received from user.
 * In direct relation with AT_Cmd_Type_t
 */
static const AT_CMD_t AT_CMD[] = {
  {"AT\r",sizeof("AT\r"),AT},
  {"AT+SV\r",sizeof("AT+SV\r"),AT_SV},
  {"AT+SV$ADV_START\r",sizeof("AT+SV$ADV_START\r"),AT_SV_ADV_START},
  {"AT+SV$ADV_STOP\r",sizeof("AT+SV$ADV_STOP\r"),AT_SV_ADV_STOP},
  {"AT+SV$NOTIFY",sizeof("AT+SV$NOTIFY"),AT_SV_NOTIFY}, /* command with param */
  {"AT+SV$CONN_UPD",sizeof("AT+SV$CONN_UPD"),AT_SV_CONN_UPD}, /* command with param */
  {"AT+CL\r",sizeof("AT+CL\r"),AT_CL},
  {"AT+CL$WRITE",sizeof("AT+CL$WRITE"),AT_CL_WRITE}, /* command with param */
  {"AT+CL$SCAN\r",sizeof("AT+CL$SCAN\r"),AT_CL_SCAN},
  {"AT+CL$CONN",sizeof("AT+CL$CONN"),AT_CL_CONN}, /* command with param */
  {"AT+CL$DISCONN\r",sizeof("AT+CL$DISCONN\r"),AT_CL_DISCONN},
  {"AT+CL$AUTOCONN",sizeof("AT+CL$AUTOCONN"),AT_CL_AUTOCONN}, /* command with param */
  {"AT+CL$EN\r",sizeof("AT+CL$EN\r"),AT_CL_EN},
  {"AT+CL$DIS\r",sizeof("AT+CL$DIS\r"),AT_CL_DIS},
  {"AT+HR\r",sizeof("AT+HR\r"),AT_HR},
  {"AT+HR$NOTIFY",sizeof("AT+HR$NOTIFY"),AT_HR_NOTIFY}, /* command with param */
  {"AT+S$PAIRING_START\r",sizeof("AT+S$PAIRING_START\r"),AT_PAIRING_START},
  {"AT+S$PAIRING_CONFIRM",sizeof("AT+S$PAIRING_CONFIRM"),AT_PAIRING_CONFIRM}, /* command with param */
  {"AT+S$CLEAR_BONDING\r",sizeof("AT+S$CLEAR_BONDING\r"),AT_CLEAR_BONDING},
  {"unknown cmd\r",sizeof("unknown cmd\r"),AT_END_CMD}};

/* Private functions prototypes-----------------------------------------------*/
static void RxCpltCallback(void);
static void at_cmd_analysing(void);
static uint8_t* hex_decode(const char *in, size_t len, uint8_t *out);
//static uint16_t dec_decode(const char *in, uint16_t *out, size_t len);


void UART_App_Init( void ) {
  
    Next_Mode = P2P_SERVER;    
    param = NO_PARAM;
    multi_param.state = NO_MULTI_PARAM;
    disconnection_status = FROM_REMOTE;
    autoconn_status = 0;
    
    PairingContext.PairingConfirmRequested = 0;
    PairingContext.PairingComplete = 0;
  
    UTIL_SEQ_RegTask( 1<< CFG_TASK_AT_CMD_ANALYSING_ID, UTIL_SEQ_RFU, at_cmd_analysing);
    
#if (CFG_HW_USART1_ENABLED == 1)
    MX_USART1_UART_Init();
#endif
#if (CFG_HW_LPUART1_ENABLED == 1)
    MX_LPUART1_UART_Init();
#endif
    
     
    /*cleanup the pDataRx buffer*/
    memset(pDataRx, 0x00, 32);
    
    HW_UART_Receive_IT(CFG_AT_UART, (uint8_t *)pCharRx, 1, RxCpltCallback);
    
    return;
}

char* UART_App_ConvertBleDataToString(const uint8_t *in, char *out, size_t len)
{
  int i, j;
  char in_separated[32];
  for(i = 0, j = 0; i < len; i+=2, j++)
  {
    in_separated[i+1] = in[j] & 0x0F;
    in_separated[i] = (in[j]>>4) & 0x0F;
    out[i] = in_separated[i] > 9 ? (in_separated[i] - 10) + 'A' : in_separated[i] + '0';
    out[i+1] = in_separated[i+1] > 9 ? (in_separated[i+1] - 10) + 'A' : in_separated[i+1] + '0';
  }
  
  return out;
}

void UART_App_SendData(char * message, uint8_t len)
{
  HAL_UART_Transmit(&hlpuart1, (uint8_t*)message, len, 5000);
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

static void RxCpltCallback( void )
{
  pDataRx[indexRx++] = pCharRx[0];
  if(*pCharRx == '\r') { //End of AT Command
    UTIL_SEQ_SetTask( 1<<CFG_TASK_AT_CMD_ANALYSING_ID, CFG_SCH_PRIO_0);
  }
  else if(*pCharRx == '=') param = PARAM;
  else if(*pCharRx == ',')
  {
    multi_param.state = MULTI_PARAM;
    multi_param.index = indexRx - 1;
  }

  HW_UART_Receive_IT(CFG_AT_UART, (uint8_t *)pCharRx, 1, RxCpltCallback);

  return;
}

static void at_cmd_analysing( void )
{
  int i, j;
  AT_Cmd_Type_t at_type = AT_END_CMD;
  uint8_t bd_addr_str[12];
  uint8_t write_char_data_str[4];
  uint8_t notify_char_data_str[4];
  
  for (i = 0; i < AT_END_CMD; i++)
  {   
    if (strncmp(pDataRx, AT_CMD[i].ATCmdStr, (AT_CMD[i].SizeATCmdStr)-1) == 0)
    {
      /* command has been found*/
      at_type = AT_CMD[i].AT_Cmd_Type;
      break;
    }
  }
  
  switch(at_type){
    case AT :
      UART_App_SendData("\r\nOK\r\n", 6);
      break;
    case AT_SV :
      Next_Mode = P2P_SERVER;
      if(APP_MODE == P2P_SERVER) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        /* Switch to P2P Server */
        disconnection_status = TO_SWITCH_APP;
        if((APP_MODE == HEART_RATE) && (HR_Connection_Status == APP_BLE_CONNECTED_SERVER))
        {
          UTIL_SEQ_SetTask( 1<<CFG_TASK_HR_GAP_DISCON_ID, CFG_SCH_PRIO_0);
        }
        else if((APP_MODE == P2P_CLIENT) && (CL_Connection_Status == APP_BLE_CONNECTED_CLIENT))
        {
          UTIL_SEQ_SetTask( 1<<CFG_TASK_CL_GAP_DISCON_ID, CFG_SCH_PRIO_0);
        }
        else {
          *(volatile uint32_t*) SRAM1_BASE = SRAM1_BASE_P2P_SERVER;
          NVIC_SystemReset();
        }        
      }
      break;
    case AT_SV_ADV_START :
      if( APP_MODE != P2P_SERVER ) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        /* Request advertising */
        UTIL_SEQ_SetTask(1 << CFG_TASK_ADV_REQ_ID, CFG_SCH_PRIO_0);
      }
      break;
    case AT_SV_ADV_STOP :
      if( APP_MODE != P2P_SERVER ) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        /* Cancel advertising */
        UTIL_SEQ_SetTask(1 << CFG_TASK_ADV_CANCEL_ID, CFG_SCH_PRIO_0);
      }
      break;
    case AT_SV_NOTIFY :
      if((APP_MODE != P2P_SERVER) || (param != PARAM)) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        if((pDataRx[12] == '=') && (pDataRx[17] == '\r')) 
        {
          for(i = 0; i < 4; i++) notify_char_data_str[i] = pDataRx[i+13];
          hex_decode((char *)notify_char_data_str, 4, NotifyCharData);
          /* Notify the Client */
          UTIL_SEQ_SetTask( 1<<CFG_TASK_NOTIFY_ID, CFG_SCH_PRIO_0);
        }
        else UART_App_SendData("\r\nERROR\r\n", 9);
      }
      break;
    case AT_SV_CONN_UPD :
      if(APP_MODE != P2P_SERVER) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        uint8_t lenConnUpdStr;
        lenConnUpdStr = (indexRx-1)-(14+1); /* indexRx - 1 to compensate the indexRx++ when filling the pDataRx buffer */
        if(pDataRx[14] == '=' && lenConnUpdStr <= 4 )
        { //The format of the command is correct and the length of the parameter as well
          char ConnUpdStr[5];          
          for(i = 0; i < lenConnUpdStr; i++) ConnUpdStr[i] = pDataRx[15+i];
          /* TODO : Verify that the entered values are correct. Ex: Discard a value like .1#2 */
          Connection_Update_Interval = (uint16_t) atoi(ConnUpdStr);
          if(Connection_Update_Interval <= 4000 && Connection_Update_Interval >= 10)
          {
            /* Change connection interval */
            UTIL_SEQ_SetTask( 1<<CFG_TASK_CONN_UPDATE_ID, CFG_SCH_PRIO_0);
          }
          else UART_App_SendData("\r\nERROR\r\n", 9);
        }
        else UART_App_SendData("\r\nERROR\r\n", 9);
       
      }
      break;
    case AT_CL :
      Next_Mode = P2P_CLIENT;
      if(APP_MODE == P2P_CLIENT) UART_App_SendData("\r\nERROR\r\n", 9);
      else {  
        /* Switch to P2P Client */
        disconnection_status = TO_SWITCH_APP;
        if((APP_MODE == HEART_RATE) && (HR_Connection_Status == APP_BLE_CONNECTED_SERVER))
        {
          UTIL_SEQ_SetTask( 1<<CFG_TASK_HR_GAP_DISCON_ID, CFG_SCH_PRIO_0);
        }
        else if((APP_MODE == P2P_SERVER) && (SV_Connection_Status == APP_BLE_CONNECTED_SERVER))
        {
          UTIL_SEQ_SetTask( 1<<CFG_TASK_SV_GAP_DISCON_ID, CFG_SCH_PRIO_0);
        }
        else {
          *(volatile uint32_t*) SRAM1_BASE = SRAM1_BASE_P2P_CLIENT;
          NVIC_SystemReset();
        }        
      }
      break;
    case AT_CL_WRITE :
      if(APP_MODE != P2P_CLIENT || (param != PARAM)) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        if((pDataRx[11] == '=') && (pDataRx[16] == '\r')) 
        {
          for(i = 0; i < 4; i++) write_char_data_str[i] = pDataRx[i+12];
          hex_decode((char *)write_char_data_str, 4, WriteCharData);
          /* Send a write command to turn on/off the Led of the Server */
          UTIL_SEQ_SetTask(1<<CFG_TASK_SEND_DATA_TO_SERVER_ID, CFG_SCH_PRIO_0);
        }
        else UART_App_SendData("\r\nERROR\r\n", 9);
      }
      break;
    case AT_CL_SCAN :
      if( APP_MODE != P2P_CLIENT ) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        /* Clear BD Address buffer in order to be able to re-send the BD addresses discovered during a previous scan on the UART */
        for(i = 0; i < 5; i++)
        {
          for(j = 0; j < 6; j++)
          {
            SERVER_REMOTE_BDADDR[i][j] = 0;
          }
        }
        index_bd_addr = 0;
        /* Scan the devices and send the found ones via UART */
        UTIL_SEQ_SetTask(1<<CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
      }
      break;
    case AT_CL_CONN :
      if((APP_MODE != P2P_CLIENT) || (param != PARAM)) UART_App_SendData("\r\nERROR\r\n", 9); //Make sure we are in P2P Client mode and the parameter is set
      else {
        if((pDataRx[10] == '=') && (pDataRx[23] == '\r')) 
        {
          /* Command is correct regarding the bd address input */
          for(i = 0; i < 12; i++) bd_addr_str[i] = pDataRx[i+11];
          hex_decode((char *)bd_addr_str, 12, BD_Addr);
          /* Connect to the indicated device */
          UTIL_SEQ_SetTask(1 << CFG_TASK_CONN_DEV_1_ID, CFG_SCH_PRIO_0);
        }
        else UART_App_SendData("\r\nERROR\r\n", 9);
      }
      break;
    case AT_CL_DISCONN :
      if((APP_MODE != P2P_CLIENT) || (CL_Connection_Status != APP_BLE_CONNECTED_CLIENT)) UART_App_SendData("\r\nERROR\r\n", 9); //Make sure we are in P2P Client mode and the Client is connected to a Server
      else {
        disconnection_status = FROM_AT_CMD;
        /* Disconnect from the connected device */
        UTIL_SEQ_SetTask( 1<<CFG_TASK_CL_GAP_DISCON_ID, CFG_SCH_PRIO_0);
      }
      break;
    case AT_CL_AUTOCONN :
      if((APP_MODE != P2P_CLIENT) || (param != PARAM)) UART_App_SendData("\r\nERROR\r\n", 9); //Make sure we are in P2P Client mode and the parameter is set
      else {
        if((pDataRx[14] == '=') && (pDataRx[27] == '\r')) 
        {
          /* Command is correct */
          autoconn_status = 1;
          for(i = 0; i < 12; i++) bd_addr_str[i] = pDataRx[i+15];
          hex_decode((char *)bd_addr_str, 12, BD_Addr);
          /* Scan the devices and if one correspond to the BD address given by the user, connect to it */
          UTIL_SEQ_SetTask(1<<CFG_TASK_START_SCAN_ID, CFG_SCH_PRIO_0);
        }
        else UART_App_SendData("\r\nERROR\r\n", 9);
      }
      break;
    case AT_CL_EN :
      if(APP_MODE != P2P_CLIENT) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        /* Enable notifications */
        Enable_Notification();
      }
      break;
    case AT_CL_DIS :
      if(APP_MODE != P2P_CLIENT) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        /* Disable notifications */
        Disable_Notification();
      }
      break;
    case AT_HR :
      Next_Mode = HEART_RATE;
      if(APP_MODE == HEART_RATE) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        /* Switch to Heart Rate */
        disconnection_status = TO_SWITCH_APP;
        if((APP_MODE == P2P_SERVER) && (SV_Connection_Status == APP_BLE_CONNECTED_SERVER))
        {
          UTIL_SEQ_SetTask( 1<<CFG_TASK_SV_GAP_DISCON_ID, CFG_SCH_PRIO_0);
        }
        else if((APP_MODE == P2P_CLIENT) && (CL_Connection_Status == APP_BLE_CONNECTED_CLIENT))
        {
          UTIL_SEQ_SetTask( 1<<CFG_TASK_CL_GAP_DISCON_ID, CFG_SCH_PRIO_0);
        }
        else {
          *(volatile uint32_t*) SRAM1_BASE = SRAM1_BASE_HEART_RATE;
          NVIC_SystemReset();
        }
      }
      break;
    case AT_HR_NOTIFY :
      if((APP_MODE != HEART_RATE) || (param != PARAM) || (multi_param.state != MULTI_PARAM)) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        if(pDataRx[12] == '=') 
        {
          /* Parse data */
          uint8_t len_val1;
          uint8_t len_val2;
          char val1_str[16];
          char val2_str[16];
          
          len_val1 = multi_param.index - (12 + 1);
          len_val2 = indexRx - (multi_param.index +1);
          
          for(i = 0; i < len_val1; i++) val1_str[i] = pDataRx[i+13];
          for(i = 0; i < len_val2; i++) val2_str[i] = pDataRx[i+multi_param.index+1];
          
          HR_Notify_Context.Measurement = (uint16_t) atoi(val1_str);
          HR_Notify_Context.EnergyExpended = (uint16_t) atoi(val2_str);
          
          /* Notify the Client */
          UTIL_SEQ_SetTask( 1<<CFG_TASK_MEAS_REQ_ID, CFG_SCH_PRIO_0);
        }
        else UART_App_SendData("\r\nERROR\r\n", 9);
      }
      break;
    case AT_PAIRING_START :
        if(!PairingContext.PairingComplete)
        {
          UTIL_SEQ_SetTask( 1<<CFG_TASK_REQUEST_PAIRING_ID, CFG_SCH_PRIO_0);
        }
      break;
    case AT_PAIRING_CONFIRM :
      if(param != PARAM) UART_App_SendData("\r\nERROR\r\n", 9);
      else {
        if(pDataRx[20] == '=') 
        {
          if(pDataRx[21] == 'Y' && PairingContext.PairingConfirmRequested == 1)
          {
            PairingContext.PairingConfirmRequested = 0;
            UTIL_SEQ_SetTask( 1<<CFG_TASK_CONFIRM_PAIRING_ID, CFG_SCH_PRIO_0);
          }
        }
      }
      break;
    case AT_CLEAR_BONDING :
        aci_gap_clear_security_db();
        UART_App_SendData("\r\nOK\r\n", 6);
      break;
    default : /* Error */
      UART_App_SendData("\r\nERROR\r\n", 9);
      break;
  }
  
  param = NO_PARAM;
  
  /*cleanup the pDataRx buffer*/
  memset(pDataRx, 0x00, 32);
  indexRx = 0;
}

static uint8_t* hex_decode(const char *in, size_t len,uint8_t *out)
{
    unsigned int i, t, hn, ln;

    for (t = 0,i = 0; i < len; i+=2,++t) {
            hn = in[i] > '9' ? in[i] - 'A' + 10 : in[i] - '0'; //upper part of the hexa value
            ln = in[i+1] > '9' ? in[i+1] - 'A' + 10 : in[i+1] - '0'; //lower part of the hexa value
            out[t] = (hn << 4 ) | ln;
    }
    return out;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
