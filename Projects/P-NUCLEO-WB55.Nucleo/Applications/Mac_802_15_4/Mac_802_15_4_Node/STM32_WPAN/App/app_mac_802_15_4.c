/**
  ******************************************************************************
 * @file    app_ffd_mac_802_15_4.c
 * @author  MCD Application Team
 * @brief   Application based on MAC 802.15.4
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
#include "stm_queue.h"

#include "utilities_common.h"

#include "app_entry.h"
#include "app_mac_802_15_4.h"

#include "802_15_4_mac_sap.h"
#include "app_mac_802_15_4_process.h"
#include "dbg_trace.h"
#include "shci.h"
#include "stm_logging.h"


/* Private define ------------------------------------------------------------*/
/* Set the Maximum value of the counter (Auto-Reload) that defines the Period */
#define HW_TS_SERVER_5S_NB_TICKS                    (5*1000*1000/CFG_TS_TICK_VAL) /* 5s */ 

/* Private macro -------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static uint8_t xorSign( const char * pmessage, uint8_t message_len);
static void APP_MAC_802_15_4_TraceError(char * pMess, uint32_t ErrCode);
static void APP_MAC_802_15_4_Config(void);
static void SendTimerCallback(void);

/* Private variables ---------------------------------------------------------*/
static uint8_t TS_ID1;
static uint8_t rfBuffer[256];

/* Public variables ---------------------------------------------------------*/
MAC_callbacks_t macCbConfig ; // Structure with all callbacks
MAC_associateCnf_t g_MAC_associateCnf;
MAC_scanCnf_t  g_ScanCnf;
MAC_beaconNotifyInd_t  g_BeaconNotifyInd;
uint8_t deviceAssociated = 0; //This variable allows to know if the device is associated or not, associated = 1 otherwise 0
uint8_t enablePollTimer = 0; //This variable allows to enable the poll Timer which send data poll every 5s
uint8_t beacon_coord_received  = 0x00; // This vraiable allows to check if we have receive beacon or not

/*  All variables for configuration and set data */
uint16_t g_panId                = 0x0000;
uint16_t g_coordShortAddr       = 0x0000;;
uint16_t g_BroacastShortAddr    = 0xFFFF;
uint8_t  g_coordExtendedAddr[8] = {0x01,0x00,0x00,00,0x00,0x48,0xDE,0xAC};
uint8_t  g_dataHandle           = 0x02;
uint8_t  extAddr[8]             = {0x02,0x00,0x00,00,0x00,0x48,0xDE,0xAC};
uint8_t  g_channel              = 11; // General channel
uint8_t  g_channel_page         = 0x00;

/**
  * @brief  compute  simple xor signature of the data to transmit
  *
  * @param  pmessage   : Message to sign.
  * @param  message_len: Message Len
  * @retval Message Signature
  */
static uint8_t xorSign( const char * pmessage, uint8_t message_len)
{
  uint8_t seed = 0x00;
  for (uint8_t i=0x00;i<message_len;i++)
    seed = (uint8_t)pmessage[i]^seed;
  return seed; 
}

static void get_mac_address(uint8_t *addr) 
{
  uint32_t lsb_mac, msb_mac;

  lsb_mac = (uint32_t)(READ_REG(*((uint32_t *)UID64_BASE)));
  msb_mac = (uint32_t)(READ_REG(*((uint32_t *)UID64_BASE + 1U)));
  
  if(lsb_mac == 0xFFFFFFFF) {
    return;
  }
  
  /**
   * MAC address of Nucleo board is set from the device unique ID.
   */
  addr[0] = (uint8_t)(lsb_mac & 0x000000FF);
  addr[1] = (uint8_t)((lsb_mac & 0x0000FF00) >> 8);
  addr[2] = (uint8_t)((lsb_mac & 0x00FF0000) >> 16);
  addr[3] = (uint8_t)((lsb_mac & 0xFF000000) >> 24);
  addr[4] = (uint8_t)(msb_mac & 0x000000FF);
  addr[5] = (uint8_t)((msb_mac & 0x0000FF00) >> 8);
  addr[6] = (uint8_t)((msb_mac & 0x00FF0000) >> 16);
  addr[7] = (uint8_t)((msb_mac & 0xFF000000) >> 24);
}

/**
  * @brief  This function allows you to set a PIB attribute to put in parameters 
  * only the PIB attribute and this value.
  *
  * @param  PIB_attribute_indice   : PIB attribute you want to change
  * @param  PIB_attribute_value: PIB attribute value you want to change
  * @retval nothing
  */
static void Prepare_MLME_Set_Req(uint8_t PIB_attribute_indice, uint8_t * PIB_attribute_value)
{
  MAC_Status_t MacStatus;
  MAC_setReq_t   SetReq;
  
  memset(&SetReq, 0x00, sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = PIB_attribute_indice;
  SetReq.PIB_attribute_valuePtr = PIB_attribute_value;
  MacStatus = MAC_MLMESetReq( &SetReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("Node MAC APP - Set PIB attribute Fails\0");
    return;
  }
  
  /* Wait SET CONFIRMATION */
  UTIL_SEQ_WaitEvt(EVENT_SET_CNF); 
}

/**
  * @brief  Task to release the IRQ for POLL.req
  *
  * @param  arg
  * @retval Nothing
  */
static void SendTimerCallback(void)
{
  // Run Data pool task
  UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_POOL, CFG_SCH_PRIO_0 );
}

/* USER CODE BEGIN FD*/
void APPE_Button1Action(void)
{
  /* Program timer which send a data to the coordinator every 5s */
  APP_MAC_802_15_4_poll();
  if (!enablePollTimer) {
    enablePollTimer = 1;
    HW_TS_Start(TS_ID1, HW_TS_SERVER_5S_NB_TICKS);
  }else{
    enablePollTimer = 0;
    HW_TS_Stop(TS_ID1);
  }
}

void APPE_Button2Action(void)
{
  /* Send data in broadcast */
  APP_MAC_802_15_4_SendData_broacast("Node MAC APP - Data In Broadcast\0");
}

void APPE_Button3Action(void)
{
  /* Disassociation with the coordinator */
  if (deviceAssociated){
    APP_MAC_802_15_4_Disassociation();
  }else{
   APP_DBG("Node MAC APP - This device is not associated\0");
  }

}

/* Functions Definition ------------------------------------------------------*/
void APP_MAC_802_15_4_Init( APP_MAC_802_15_4_InitMode_t InitMode, TL_CmdPacket_t* pCmdBuffer)
{
  /* Register cmdbuffer */
  APP_ENTRY_RegisterCmdBuffer(pCmdBuffer);

  /* Init config buffer and call TL_THREAD_Init */
  APP_ENTRY_TL_MAC_802_15_4_Init();

  /* Send MAC_802_15_4 start system cmd to RF Core */
  SHCI_C2_MAC_802_15_4_Init();

  /* Register task */
  /* Create the different tasks */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MSG_FROM_RF_CORE, UTIL_SEQ_RFU, APP_ENTRY_ProcessMsgFromRFCoreTask);//Use for IPCC

  UTIL_SEQ_RegTask( 1<<CFG_TASK_SETUP, UTIL_SEQ_RFU, APP_MAC_802_15_4_SetupTask); //Use for mac setup
  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_1, UTIL_SEQ_RFU, APPE_Button1Action); // Button 1
  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_2, UTIL_SEQ_RFU, APPE_Button2Action); // Button 2
  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_3, UTIL_SEQ_RFU, APPE_Button3Action); // Button 3
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_POOL, UTIL_SEQ_RFU, APP_MAC_802_15_4_poll); // Data pool task

  /* Configuration MAC 802_15_4 */
  APP_MAC_802_15_4_Config();
  
  /* Create RTC Timer */
  HW_TS_Create(CFG_TIM_MAC_APP_ONOFF_TOOGLE_REPEAT, &TS_ID1, hw_ts_Repeated, SendTimerCallback);

  /* Start Main Coordinator - NODE Task*/
  UTIL_SEQ_SetTask( 1<< CFG_TASK_SETUP, CFG_SCH_PRIO_0 );

}

void APP_MAC_802_15_4_SetupTask(void)
{
  MAC_Status_t MacStatus;
  MAC_resetReq_t ResetReq;
  MAC_scanReq_t ScanReq;
  uint8_t PIB_Value = 0x00;
  static uint8_t max_retry_scan = 0;
  static uint8_t scan_duration = 5;
  /** Select all channels 
   * The channel numbers to be scanned. The 27 bits (b0, b1, �, b26) indicate 
   * which channels are to be scanned (1 = scan, 0 = do not scan) for each 
   * of the 27 channels.
  **/
  uint8_t scan_channel[4] = {0x00, 0xF8, 0xFF, 0x07};

  APP_DBG("Node MAC APP -  Node Startup\0");
  /* Reset MAC */
  memset(&ResetReq, 0x00, sizeof(MAC_resetReq_t));
  ResetReq.set_default_PIB = TRUE;
  
  MacStatus = MAC_MLMEResetReq(&ResetReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("Node MAC APP - Reset Fails\0");
    return;
  }
  /* Wait RESET CONFIRMATION */
  UTIL_SEQ_WaitEvt(EVENT_RESET_CNF);     

  /* Get automatically the extended address */
  get_mac_address((uint8_t *) &extAddr[0]);
  APP_DBG("Node MAC APP - Extended address: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\0",extAddr[7],
          extAddr[6],extAddr[5],extAddr[4],extAddr[3],extAddr[2],extAddr[1],extAddr[0]);
  
  /* Set Device Extended Address */
  Prepare_MLME_Set_Req(g_MAC_EXTENDED_ADDRESS_c, (uint8_t *) &extAddr);

  /* Set MAC_AUTO_REQUEST to 0 to able to receive beaconNotify.ind */  
  PIB_Value = g_FALSE; 
  Prepare_MLME_Set_Req(g_MAC_AUTO_REQUEST_c, (uint8_t *) &PIB_Value);
  
  /* Set RxOnWhenIdle to 1 to able to receive frames */
  PIB_Value = g_TRUE;
  Prepare_MLME_Set_Req(g_MAC_RX_ON_WHEN_IDLE_c, (uint8_t *) &PIB_Value);
    
  /* Set Tx Power to 6 dBm, range between [-20;6] */
  PIB_Value = 6;
  Prepare_MLME_Set_Req(g_PHY_TRANSMIT_POWER_c, (uint8_t *) &PIB_Value);
  
  /* Scan Active */
  while (!beacon_coord_received && max_retry_scan < MAC_RETRY_SCAN) {
  APP_DBG("Node MAC APP - Active Scan\0");
  scan_duration = BASE_SCAN_DURATION + max_retry_scan; // Increase scan duration each retry, to have more change to receive coordinator beacon's
  memset(&ScanReq, 0x00, sizeof(MAC_scanReq_t));
  ScanReq.scan_type = g_MAC_ACTIVE_SCAN_TYPE_c; // Active scan
  ScanReq.scan_duration = scan_duration; // Max frame duration 960 � (2^n + 1)], with n = scan_duration
  ScanReq.channel_page = g_channel_page; // Always this value in 802.15.4 non-beacon enabled
  memcpy(&ScanReq.a_scan_channels, (uint8_t *) &scan_channel, 0x04);
    
  MacStatus = MAC_MLMEScanReq(&ScanReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("Node MAC APP - Scan active Fails\0");
    return;
  }
  /* Wait SCAN CONFIRMATION */
  UTIL_SEQ_WaitEvt(1U << CFG_EVT_SCAN_CNF);
  max_retry_scan++;
  }

  /* Association to the coordinator */
  if (beacon_coord_received) {
  APP_MAC_802_15_4_Association();
 
  /* Set Device Short Address */
  Prepare_MLME_Set_Req(g_MAC_SHORT_ADDRESS_c, (uint8_t *) &g_MAC_associateCnf.a_assoc_short_address);

  APP_DBG("Node MAC APP - Ready \0"); // To Handle Association Req and Receive Data
  BSP_LED_On(LED1);
  APP_MAC_802_15_4_poll(); // Send data to the coordinator
  
  }else {
    APP_DBG("Node MAC APP - No coordinator detected after several retries...\r\n");
  }
}

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */

void APP_MAC_802_15_4_Error(uint32_t ErrId, uint32_t ErrCode)
{

  switch(ErrId)
  {
  case ERR_APPLI_REC_MULTI_MSG_FROM_RFCore :
    APP_MAC_802_15_4_TraceError("ERROR : ERR_REC_MULTI_MSG_FROM_RFCore ",ErrCode);
    break;
  case ERR_INTERFACE_IPCC_SEND_ACK_TO_RFCore :
    APP_MAC_802_15_4_TraceError("ERROR : ERR_IPCC_SEND_ACK_TO_RFCore ",ErrCode);
    break;
  default :
    APP_MAC_802_15_4_TraceError("ERROR Unknown ",0);
    break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void APP_MAC_802_15_4_Config()
{
  APP_DBG("configure NODE MAC 802.15.4 - 2\0");
  /* Register MAC 802.15.4 callback functions */

  memset(&macCbConfig,0x00,sizeof(MAC_callbacks_t));

  /* Mac Call Back Initialization */
  macCbConfig.mlmeResetCnfCb = APP_MAC_mlmeResetCnfCb;
  macCbConfig.mlmeScanCnfCb = APP_MAC_mlmeScanCnfCb;
  macCbConfig.mlmeAssociateCnfCb = APP_MAC_mlmeAssociateCnfCb;
  macCbConfig.mlmeAssociateIndCb = APP_MAC_mlmeAssociateIndCb;
  macCbConfig.mlmeBeaconNotifyIndCb = APP_MAC_mlmeBeaconNotifyIndCb;
  macCbConfig.mlmeCalibrateCnfCb = APP_MAC_mlmeCalibrateCnfCb;
  macCbConfig.mlmeCommStatusIndCb = APP_MAC_mlmeCommStatusIndCb;
  macCbConfig.mlmeDisassociateCnfCb = APP_MAC_mlmeDisassociateCnfCb;
  macCbConfig.mlmeDisassociateIndCb = APP_MAC_mlmeDisassociateIndCb;
  macCbConfig.mlmeDpsCnfCb = APP_MAC_mlmeDpsCnfCb;
  macCbConfig.mlmeDpsIndCb = APP_MAC_mlmeDpsIndCb;
  macCbConfig.mlmeGetCnfCb = APP_MAC_mlmeGetCnfCb;
  macCbConfig.mlmeGtsCnfCb = APP_MAC_mlmeGtsCnfCb;
  macCbConfig.mlmeGtsIndCb = APP_MAC_mlmeGtsIndCb;
  macCbConfig.mlmeOrphanIndCb = APP_MAC_mlmeOrphanIndCb;
  macCbConfig.mlmePollCnfCb = APP_MAC_mlmePollCnfCb;
  macCbConfig.mlmeRxEnableCnfCb = APP_MAC_mlmeRxEnableCnfCb;
  macCbConfig.mlmeSetCnfCb = APP_MAC_mlmeSetCnfCb;
  macCbConfig.mlmeSoundingCnfCb = APP_MAC_mlmeSoundingCnfCb;
  macCbConfig.mlmeStartCnfCb = APP_MAC_mlmeStartCnfCb;
  macCbConfig.mlmeSyncLossIndCb = APP_MAC_mlmeSyncLossIndCb;
  macCbConfig.mcpsDataIndCb = APP_MAC_mcpsDataIndCb;
  macCbConfig.mcpsDataCnfCb = APP_MAC_mcpsDataCnfCb;
  macCbConfig.mcpsPurgeCnfCb = APP_MAC_mcpsPurgeCnfCb;
  macCbConfig.mlmePollIndCb = APP_MAC_mlmePollIndCb;
}

void APP_MAC_802_15_4_SendData_broacast(const char * data)
{
  MAC_Status_t MacStatus = MAC_ERROR;
  MAC_dataReq_t DataReq;
  BSP_LED_On(LED3);
  
  APP_DBG("Node MAC APP - Send Data to 0x%04x\0", g_BroacastShortAddr);
  /* structure initialization */
  memset(&DataReq, 0x00, sizeof(MAC_dataReq_t));
  DataReq.src_addr_mode = g_SHORT_ADDR_MODE_c; // If the device is associated, we send the broadcast with the short address
  if (!deviceAssociated) {
    DataReq.src_addr_mode = g_EXTENDED_ADDR_MODE_c; // If the device is not associated, we send the broadcast with the extended address
  }
  DataReq.dst_addr_mode = g_SHORT_ADDR_MODE_c; // Address source mode

  memcpy(DataReq.a_dst_PAN_id, &g_panId, 0x02); // PANID destination
  memcpy(DataReq.dst_address.a_short_addr, &g_BroacastShortAddr,0x02); // Address destination 

  DataReq.msdu_handle = g_dataHandle++; // Sequence number
  DataReq.ack_Tx = FALSE; // Request ACK = TRUE, otherwise FALSE
  DataReq.GTS_Tx = FALSE; // Always this value in 802.15.4 non-beacon enabled
  memcpy(&rfBuffer, data, strlen(data));
  rfBuffer[strlen(data)] = xorSign(data, strlen(data));
  DataReq.msduPtr = (uint8_t*) rfBuffer; // Data payload
  DataReq.msdu_length = strlen(data) + 1; // Data length
  DataReq.security_level = g_MAC_UNSECURED_MODE_c; // Unsecure frame
  MacStatus = MAC_MCPSDataReq(&DataReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("Node MAC APP - Data Req Fails\0");
    return;
  }
  BSP_LED_Off(LED3);
}

void APP_MAC_802_15_4_Disassociation(void)
{
  MAC_Status_t MacStatus = MAC_ERROR;
  MAC_disassociateReq_t DisassiociateReq;
  BSP_LED_On(LED3);
  
  /* If poll timer is enable stop it*/
  if (enablePollTimer){
    enablePollTimer = 0;
    HW_TS_Stop(TS_ID1);
  }
  
  /* structure initialization */
  memset(&DisassiociateReq, 0x00, sizeof(MAC_disassociateReq_t));
  DisassiociateReq.device_addr_mode = g_SHORT_ADDR_MODE_c; // Short address mode
  memcpy(DisassiociateReq.a_device_PAN_id, &g_panId, 0x02); // Device PAN ID
  DisassiociateReq.disassociate_reason = g_DEVICE_REQUESTED_c; // The device wishes to leave the PAN
  memcpy(DisassiociateReq.device_address.a_short_addr, &g_coordShortAddr,0x02); // Coordinator Address 
  DisassiociateReq.tx_Indirect = FALSE;
  DisassiociateReq.security_level = g_MAC_UNSECURED_MODE_c; // Unsecure frame
  MacStatus = MAC_MLMEDisassociateReq(&DisassiociateReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("Node MAC APP - Disassociate Req Fails\0");
    return;
  }

  BSP_LED_Off(LED3);
}

void APP_MAC_802_15_4_poll(void)
{
  MAC_Status_t MacStatus = MAC_ERROR;
  MAC_pollReq_t PollReq;
  BSP_LED_On(LED3);
  
  
  /* structure initialization */
  memset(&PollReq, 0x00, sizeof(MAC_pollReq_t));
  PollReq.coord_addr_mode = g_SHORT_ADDR_MODE_c; // Short address mode
  memcpy(PollReq.coord_address.a_short_addr, &g_coordShortAddr,0x02); // Coordinator address
  memcpy(PollReq.a_coord_PAN_id, &g_panId, 0x02); // PANID destination
  PollReq.security_level = g_MAC_UNSECURED_MODE_c; // Unsecure frame
  
  MacStatus = MAC_MLMEPollReq(&PollReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("Node MAC APP - Disassociate Req Fails\0");
    return;
  }

  BSP_LED_Off(LED3);
}

void APP_MAC_802_15_4_Association(void)
{
  MAC_Status_t MacStatus = MAC_ERROR;
  MAC_associateReq_t AssociateReq;
  
  memset(&AssociateReq, 0x00, sizeof(MAC_associateReq_t));
  APP_DBG("Node MAC APP - Association REQ\0");
  g_channel = g_BeaconNotifyInd.PAN_descriptor.logical_channel; // Getting the channel in the beacon
  g_panId = ((g_BeaconNotifyInd.PAN_descriptor.a_coord_PAN_id[1]) << 8) + g_BeaconNotifyInd.PAN_descriptor.a_coord_PAN_id[0]; // Getting the PANID in the beacon
  if (g_BeaconNotifyInd.PAN_descriptor.coord_addr_mode == g_SHORT_ADDR_MODE_c) {
    g_coordShortAddr = ((g_BeaconNotifyInd.PAN_descriptor.coord_addr.a_short_addr[1]) << 8 ) + g_BeaconNotifyInd.PAN_descriptor.coord_addr.a_short_addr[0]; 
    /* Prepare coordinator address for association */
    AssociateReq.coord_addr_mode  = g_SHORT_ADDR_MODE_c;
    memcpy(AssociateReq.coord_address.a_short_addr, &g_coordShortAddr, 0x02);
      
  }else{ if (g_BeaconNotifyInd.PAN_descriptor.coord_addr_mode == g_EXTENDED_ADDR_MODE_c) {
    memcpy(g_coordExtendedAddr, &(g_BeaconNotifyInd.PAN_descriptor.coord_addr.a_extend_addr[0]), 0x08);
    /* Prepare coordinator address for association */
    AssociateReq.coord_addr_mode  = g_SHORT_ADDR_MODE_c;
    memcpy(AssociateReq.coord_address.a_extend_addr, &g_coordExtendedAddr, 0x08);
  }else {
    APP_DBG("Node MAC APP - Association REQ ERROR No extended or short address\0");
    return;
  }
  }
  /* Prepare channel for association */
  AssociateReq.channel_number   = g_channel;
  AssociateReq.channel_page     = g_channel_page; // Always this value in 802.15.4 non-beacon enabled
    
  /* Allows you to allocate address and receive ON when IDLE*/
  AssociateReq.capability_information = 0x88;
    
  /* Prepare PANID for association */
  memcpy(AssociateReq.a_coord_PAN_id, &g_panId, 0x02);
  AssociateReq.security_level = g_MAC_UNSECURED_MODE_c; // Unsecure frame
  MacStatus = MAC_MLMEAssociateReq(&AssociateReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("Node MAC APP - Association Req Fails\0");
    return;
  }
  
  /* Wait ASSOCIATION CONFIRMATION */
  UTIL_SEQ_WaitEvt(1U << CFG_EVT_ASSOCIATE_CNF);
}


/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (OpenThread or other module if any)
  * @retval None
  */
static void APP_MAC_802_15_4_TraceError(char * pMess, uint32_t ErrCode)
{
  APP_DBG(pMess);
  while(1 == 1)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(500);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500);
    BSP_LED_Toggle(LED3);
    HAL_Delay(500);
  }
}