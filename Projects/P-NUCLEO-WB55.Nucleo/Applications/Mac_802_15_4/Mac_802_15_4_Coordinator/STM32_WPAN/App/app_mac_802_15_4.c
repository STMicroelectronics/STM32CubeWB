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

/* Global variables ----------------------------------------------------------*/
extern RNG_HandleTypeDef hrng;

/* Private define ------------------------------------------------------------*/
#define SEND_TIMEOUT  (500) /**< 500ms */

/* Private macro -------------------------------------------------------------*/
/* function prototypes -------------------------------------------------------*/
void generate_pan_id(uint16_t *pan_id);
                     
/* Private variables ---------------------------------------------------------*/
static uint8_t g_dataHandle = 0x02;
static uint8_t rfBuffer[256];
static uint8_t g_channel_page = 0x00;

/* Public variables ---------------------------------------------------------*/
MAC_callbacks_t macCbConfig;
MAC_associateInd_t g_MAC_associateInd;
uint16_t shortAssociationAddrList[MAX_DEVICES_ASSO]; // Tab with all address associated devices
uint8_t idAssociationList = 0; //number of elements in the tab
uint8_t indDeviceSendData = 0; //indice to check if the device still connected
uint8_t status_check_device = 0; // Return device status when BT2 is pushed
uint16_t tab_panId_already_use[MAX_TAB_PAN_ID]; //tab with all PAN ID already use in the local environment

/* All variables for configuration and set data */
uint8_t extAddr[8]                           = {0x01,0x00,0x00,00,0x00,0x48,0xDE,0xAC};
uint16_t shortAddr                           = COORD_ADDRESS;
uint16_t panId                               = 0x0000;
uint8_t g_channel                            = 11; // General channel
uint8_t Beacon_payload[BEACON_PAYLOAD_SIZE]  = {BEACON_PAYLOAD};

/* Private function prototypes -----------------------------------------------*/
static void APP_MAC_802_15_4_TraceError(char * pMess, uint32_t ErrCode);
static void APP_MAC_802_15_4_Config(void);
static uint8_t xorSign( const char * pmessage, uint8_t message_len);
static void Prepare_MLME_Set_Req(uint8_t PIB_attribute_indice, uint8_t * PIB_attribute_value);

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
  MacStatus = MAC_MLMESetReq(&SetReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("COORD MAC APP - Set PIB attribute Fails\0");
    return;
  }

  /* Wait SET CONFIRMATION */
  UTIL_SEQ_WaitEvt(EVENT_SET_CNF); 
}

/**
  * @brief  This function allows you to use mlme-scan.request primitive.
  *
  * @param  PIB_attribute_indice   : PIB attribute you want to change
  * @param  PIB_attribute_value: PIB attribute value you want to change
  * @retval nothing
  */
static void Prepare_MLME_Scan_Req(uint8_t scan_type, uint8_t scan_duration, uint8_t * select_channel)
{
  MAC_Status_t MacStatus;
  MAC_scanReq_t  ScanReq;
  
  memset(&ScanReq, 0x00, sizeof(MAC_scanReq_t));
  ScanReq.scan_type = scan_type;
  ScanReq.scan_duration = scan_duration; // Max frame duration in µs (16 * 960 * (2^n + 1)) , with n = scan_duration
  ScanReq.channel_page = g_channel_page; // Always this value in 802.15.4 non-beacon enabled
  ScanReq.security_level = g_MAC_UNSECURED_MODE_c; // Unsecure frame
  memcpy(&ScanReq.a_scan_channels, select_channel, 0x04);
   
  MacStatus = MAC_MLMEScanReq(&ScanReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("COORD MAC APP - mlme-Scan.req Fails\r\n");
    return;
  }
  
  /* Wait SCAN CONFIRMATION */
  UTIL_SEQ_WaitEvt(EVENT_SCAN_CNF);  
}
void APPE_Button1Action(void)
{
  /* Display all associated devices */
  if (idAssociationList > 0) {
    for (uint8_t i=0;i<idAssociationList;i++) {
      APP_DBG("COORD MAC APP - Short address device %d: 0x%02x\0",i,shortAssociationAddrList[i]);
    }
  }else{
     APP_DBG("COORD MAC APP - No device connected\0");
  }
}

void APPE_Button2Action(void)
{
  /* Node still associated */
  indDeviceSendData = 0; //reset variable if we put several time on BT2
  APP_MAC_802_15_4_SendData();
}

void APPE_Button3Action(void)
{
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

  UTIL_SEQ_RegTask( 1<<CFG_TASK_SETUP, UTIL_SEQ_RFU, APP_MAC_802_15_4_SetupTask);//Use for mac setup
  
  UTIL_SEQ_RegTask( 1<<CFG_TASK_SEND_DATA, UTIL_SEQ_RFU, APP_MAC_802_15_4_SendData); // Send data
  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_1, UTIL_SEQ_RFU, APPE_Button1Action); // Button 1
  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_2, UTIL_SEQ_RFU, APPE_Button2Action); // Button 2
  UTIL_SEQ_RegTask( 1<<CFG_TASK_BUTTON_3, UTIL_SEQ_RFU, APPE_Button3Action); // Button 3
  /* Configuration MAC 802_15_4 */
  APP_MAC_802_15_4_Config();

  /*Start Main Coordinator - FFD Task*/
  UTIL_SEQ_SetTask( 1<<CFG_TASK_SETUP, CFG_SCH_PRIO_0 );

}


void APP_MAC_802_15_4_SetupTask(void)
{

 MAC_Status_t MacStatus = MAC_ERROR;

  MAC_resetReq_t    ResetReq;
  MAC_startReq_t    StartReq;
  uint8_t PIB_Value = 0x00;
  /** Select all channels 
    * The channel numbers to be scanned. The 27 bits (b0, b1, �, b26) indicate 
    * which channels are to be scanned (1 = scan, 0 = do not scan) for each 
    * of the 27 channels.
   **/
  uint8_t scan_channel[4] = {0x00, 0xF8, 0xFF, 0x07};

  APP_DBG("COORD MAC APP - Coordinator Startup\0");
  /* Reset MAC */
  memset(&ResetReq, 0x00, sizeof(MAC_resetReq_t));
  ResetReq.set_default_PIB = TRUE;
  MacStatus = MAC_MLMEResetReq(&ResetReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("COORD MAC APP - Reset Fails\0");
    return;
  }
  /* Wait RESET CONFIRMATION */
  UTIL_SEQ_WaitEvt(EVENT_RESET_CNF);    
  
  /* Get automatically the extended address */
  get_mac_address((uint8_t *) &extAddr[0]);
  APP_DBG("COORD MAC APP - Extended address: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\0",extAddr[7],
          extAddr[6],extAddr[5],extAddr[4],extAddr[3],extAddr[2],extAddr[1],extAddr[0]);
  
  /* Set Device Extended Address */
  Prepare_MLME_Set_Req(g_MAC_EXTENDED_ADDRESS_c, (uint8_t *) &extAddr[0]);

  /* Set MAC_AUTO_REQUEST to 0 to able to receive beaconNotify.ind */
  PIB_Value = FALSE; 
  Prepare_MLME_Set_Req(g_MAC_AUTO_REQUEST_c, (uint8_t *) &PIB_Value);

  /* Set Beacon payload for node identify our coordinator */
  PIB_Value = BEACON_PAYLOAD_SIZE; 
  Prepare_MLME_Set_Req(g_MAC_BEACON_PAYLOAD_LENGTH_c, (uint8_t *) &PIB_Value);

  /* Set Beacon payload for node identify our coordinator */
  Prepare_MLME_Set_Req(g_MAC_BEACON_PAYLOAD_c, (uint8_t *) &Beacon_payload[0]);
  
  /* Set Device Short Address */
  Prepare_MLME_Set_Req(g_MAC_SHORT_ADDRESS_c, (uint8_t *) &shortAddr);
   
  /* Set Association Permit to 1, allows device to associate with this device */
  PIB_Value = g_TRUE; 
  Prepare_MLME_Set_Req(g_MAC_ASSOCIATION_PERMIT_c, (uint8_t *) &PIB_Value);

  /* Set TX Power to 6 dBm, range between [-20;6] */
  PIB_Value = 6; 
  Prepare_MLME_Set_Req(g_PHY_TRANSMIT_POWER_c, (uint8_t *) &PIB_Value);
  
  /* Set RxOnWhenIdle to 1 to able to receive frames */
  PIB_Value = g_TRUE; 
  Prepare_MLME_Set_Req(g_MAC_RX_ON_WHEN_IDLE_c, (uint8_t *) &PIB_Value);

#ifndef DEMO_CHANNEL
  /* ED Scan and select channel */
  APP_DBG("COORD MAC APP - ED Scan on all channels\0");
  Prepare_MLME_Scan_Req(g_MAC_ED_SCAN_TYPE_c, BASE_SCAN_DURATION, &scan_channel[0]);
#else
  g_channel = DEMO_CHANNEL;
  APP_DBG("COORD MAC APP - Select the channel manually: %d \0", g_channel);
#endif
  /* Active Scan and getting all PANIDs */
  memset(&tab_panId_already_use, 0x00, sizeof(tab_panId_already_use)); // Empty the table
  
  APP_DBG("COORD MAC APP - Active Scan on all channels\0");
  Prepare_MLME_Scan_Req(g_MAC_ACTIVE_SCAN_TYPE_c, BASE_SCAN_DURATION, &scan_channel[0]);
  
  /* We perform a second active scan to increase the chances of getting all PANIDs. */
  APP_DBG("COORD MAC APP - Second active Scan on all channels\0");
  Prepare_MLME_Scan_Req(g_MAC_ACTIVE_SCAN_TYPE_c, BASE_SCAN_DURATION, &scan_channel[0]);
  
  /** Select PANID not used and check if this PANID in tab_panId_already_use 
    * if DEMO_PANID is already use, we take a random other.
   **/
#ifdef MAC_802_15_4_RANDOM_PAN_ID
  uint8_t panid_index = 0;
  while(panid_index < MAX_TAB_PAN_ID) {
    if (tab_panId_already_use[panid_index] == panId || panId == 0x0000 || panId == 0xFFFF) {
      generate_pan_id(&panId);
      panid_index = 0;
    }else {
      panid_index++;
    }
  }
#else
  panId = DEMO_PANID;
#endif
  APP_DBG("COORD MAC APP - Select PAN ID 0x%04x\0", panId);
  
  /* Start COORD Device */  
  memset(&StartReq, 0x00, sizeof(MAC_startReq_t));
  memcpy(StartReq.a_PAN_id, (uint8_t*)&panId, 0x02); // PANID selected to start the network
  StartReq.channel_number   = g_channel; // Channel selected to start the network
  StartReq.beacon_order     = 0x0F; // Always this value in 802.15.4 non-beacon enabled
  StartReq.superframe_order = 0x0F; // Always this value in 802.15.4 non-beacon enabled
  StartReq.PAN_coordinator  = g_TRUE; //Become PAN COORDINATOR
  StartReq.battery_life_extension = g_FALSE; // Always this value in 802.15.4 non-beacon enabled
  
  MacStatus = MAC_MLMEStartReq(&StartReq);
  if (MAC_SUCCESS != MacStatus) {
    APP_DBG("COORD MAC APP - Set Association Permit Fails\0");
    return;
  }
  /* Wait START CONFIRMATION */
  UTIL_SEQ_WaitEvt(EVENT_START_CNF);

  APP_DBG("COORD MAC APP - Ready to handle association and receive data\0");
  BSP_LED_On(LED1);
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
    APP_MAC_802_15_4_TraceError("ERROR : ERR_REC_MULTI_MSG_FROM_RFCore ", ErrCode);
    break;
  case ERR_INTERFACE_IPCC_SEND_ACK_TO_RFCore :
    APP_MAC_802_15_4_TraceError("ERROR : ERR_IPCC_SEND_ACK_TO_RFCore ", ErrCode);
    break;
  default :
    APP_MAC_802_15_4_TraceError("ERROR Unknown ", 0);
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
  APP_DBG("configure COORD MAC 802.15.4 - 2\0");
  /* Register MAC 802.15.4 callback functions */

  memset(&macCbConfig, 0x00, sizeof(MAC_callbacks_t));

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


void APP_MAC_802_15_4_SendData(void)
{
  MAC_Status_t MacStatus = MAC_ERROR;
  MAC_dataReq_t DataReq;
  static uint16_t tmp_dst_a_short_addr = 0x0000;
  char Data[127] ="DEVICE STILL HERE";
  
  /* structure initialization */
  memset(&DataReq, 0x00, sizeof(MAC_dataReq_t));
  DataReq.src_addr_mode = g_SHORT_ADDR_MODE_c; // Address source mode
  DataReq.dst_addr_mode = g_SHORT_ADDR_MODE_c; // Address destination mode
  if (indDeviceSendData<idAssociationList) {
    memcpy(DataReq.a_dst_PAN_id, &panId, 0x02); // PANID destination
    tmp_dst_a_short_addr = shortAssociationAddrList[indDeviceSendData];
    memcpy(DataReq.dst_address.a_short_addr, &tmp_dst_a_short_addr, 0x02); // Address destination 

    DataReq.msdu_handle = g_dataHandle++; // Sequence number
    DataReq.ack_Tx = g_TRUE; // Request ACK = TRUE, otherwise FALSE
    DataReq.GTS_Tx = g_FALSE; // Always this value in 802.15.4 non-beacon enabled
    memcpy(&rfBuffer, Data, strlen(Data));
    rfBuffer[strlen(Data)] = xorSign(Data, strlen(Data));
    DataReq.msduPtr = (uint8_t*) rfBuffer; // Data payload
    DataReq.msdu_length = strlen(Data) + 1; // Data length
    DataReq.security_level = g_MAC_UNSECURED_MODE_c; // Unsecure frame
    MacStatus = MAC_MCPSDataReq(&DataReq);
    if (MAC_SUCCESS != MacStatus) {
      APP_DBG("COORD MAC APP - Data Req Fails\0");
      return;
    }
    
    UTIL_SEQ_WaitEvt(1U << CFG_EVT_DATA_CNF);
    
    if (status_check_device == MAC_SUCCESS) {
      APP_DBG("COORD MAC APP - Check if device 0x%04x still connected -> OK\0", tmp_dst_a_short_addr);
      indDeviceSendData++; // Pass to the next list element
    } else {
      APP_DBG("COORD MAC APP - Check if device 0x%04x still connected -> KO because 0x%02x\0", tmp_dst_a_short_addr, status_check_device);
    }
  }
}

void generate_pan_id(uint16_t *pan_id){
#ifdef MAC_802_15_4_RANDOM_PAN_ID
  uint32_t rng_number;
  
  /* Get RNG semaphore */
  while(LL_HSEM_1StepLock(HSEM, CFG_HW_RNG_SEMID));

  /* Enable RNG */
  __HAL_RNG_ENABLE(&hrng);

  /* Enable HSI48 oscillator */
  LL_RCC_HSI48_Enable();
  /* Wait until HSI48 is ready */
  while(! LL_RCC_HSI48_IsReady());

  if (HAL_RNG_GenerateRandomNumber(&hrng, &rng_number) != HAL_OK)
  {
    /* Random number generation error */
    APP_MAC_802_15_4_TraceError("Random number generation error", 102);
  }

  /* Disable HSI48 oscillator */
  LL_RCC_HSI48_Disable();

  /* Disable RNG */
  __HAL_RNG_DISABLE(&hrng);

  /* Release RNG semaphore */
  LL_HSEM_ReleaseLock(HSEM, CFG_HW_RNG_SEMID, 0);
  
  /* left shift 3 bits for more random */
  *pan_id = (uint16_t)((rng_number >> 3) & 0xFFFF);
#else
  *pan_id = DEMO_PANID;
#endif
  
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


