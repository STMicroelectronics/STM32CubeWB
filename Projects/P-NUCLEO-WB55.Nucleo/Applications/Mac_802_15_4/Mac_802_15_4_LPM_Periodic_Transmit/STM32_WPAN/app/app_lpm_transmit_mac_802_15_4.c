/**
  ******************************************************************************
 * @file    app_lpm_transmit_mac_802_15_4.c
 * @author  MCD Application Connectivity Team
 * @brief   Application implementing periodic data transmission using the
 *          Proxy MAC interfacing 802.15.4 RF core.
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
#include "app_conf.h"
#include "utilities_common.h"
#include "app_entry.h"
#include "main.h"

#include "app_lpm_transmit_mac_802_15_4.h"
#include "802_15_4_mac_sap.h"
#include "app_lpm_transmit_mac_802_15_4_process.h"


#include "shci.h"
#include "stm32_lpm.h"

#define DEMO_CHANNEL 16

#define DATA "DATA From low powered STM32WBx5xx device \0"

/* Private function prototypes -----------------------------------------------*/


uint8_t xorSign( const char * pmessage, uint8_t message_len);


static void APP_LPM_TRANSMIT_MAC_802_15_4_TraceError(char * pMess, uint32_t ErrCode);
static void APP_LPM_TRANSMIT_MAC_802_15_4_Config (void);

static uint8_t rfBuffer[256];


static uint16_t     g_panId             = 0x1AAA;
static uint8_t      g_dataHandle        = 0x02;
static uint16_t     g_shortAddr         = 0xABCD;

static uint16_t     g_broadcastAddr     = 0xFFFF; /* Broadcast Addr */
static uint8_t      g_channel           = DEMO_CHANNEL;

MAC_callbacks_t macCbConfig ;
MAC_associateCnf_t g_MAC_associateCnf;



__IO ITStatus CertifOutputPeripheralReady = SET;


/* Functions Definition ------------------------------------------------------*/

void APP_LPM_TRANSMIT_MAC_802_15_4_Init( APP_MAC_802_15_4_InitMode_t InitMode, TL_CmdPacket_t* pCmdBuffer)
{
  /* Register cmdbuffer */
  APP_ENTRY_RegisterCmdBuffer(pCmdBuffer);

  /* Init config buffer and call TL_THREAD_Init */
  APP_ENTRY_TL_MAC_802_15_4_Init();

  /* Send MAC_802_15_4 start system cmd to RF Core */
  SHCI_C2_MAC_802_15_4_Init();

  /* Register task */
  /* Create the different tasks */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_MSG_FROM_RF_CORE, UTIL_SEQ_RFU, APP_ENTRY_ProcessMsgFromRFCoreTask);

  UTIL_SEQ_RegTask( 1<<CFG_TASK_RFD, UTIL_SEQ_RFU,APP_LPM_TRANSMIT_MAC_802_15_4_SetupTask);

  /* Configuration MAC 802_15_4 */
  APP_LPM_TRANSMIT_MAC_802_15_4_Config();

  /*Start Main Node - RFD Task*/
  UTIL_SEQ_SetTask( 1<< CFG_TASK_RFD, CFG_SCH_PRIO_0 );

}


void APP_LPM_TRANSMIT_MAC_802_15_4_NodeSrvTask(void)
{
}

void APP_LPM_TRANSMIT_MAC_802_15_4_NodeDataTask(void)
{
  APP_LPM_TRANSMIT_MAC_802_15_4_SendData(DATA);
}


void APP_LPM_TRANSMIT_MAC_802_15_4_SetupTask(void)
{

  MAC_Status_t MacStatus = MAC_ERROR;

  MAC_resetReq_t    ResetReq;
  MAC_setReq_t      SetReq;

  /* Reset RFD Device */
  
  /* Reset MAC */
  memset(&ResetReq,0x00,sizeof(MAC_resetReq_t));
  ResetReq.set_default_PIB = TRUE;

  MacStatus = MAC_MLMEResetReq( &ResetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    return;
  }
  /* Wait for Reset Confirmation */
  UTIL_SEQ_WaitEvt( 1U<< CFG_EVT_DEVICE_RESET_CNF );

  /* Setup device for Silent Start */

  /* Set Device Short Address */
  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_MAC_SHORT_ADDRESS_c;
  SetReq.PIB_attribute_valuePtr = (uint8_t*)&g_shortAddr;

  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    return;
  }
  UTIL_SEQ_WaitEvt( 1U<< CFG_EVT_SET_CNF );
  
  /* Set PanID Address */
  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_MAC_PAN_ID_c;
  SetReq.PIB_attribute_valuePtr = (uint8_t*)&g_panId;

  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    return;
  }
  UTIL_SEQ_WaitEvt( 1U<< CFG_EVT_SET_CNF );
  
  /* Setup TX Channel */
  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_PHY_CURRENT_CHANNEL_c;
  SetReq.PIB_attribute_valuePtr = (uint8_t*)&g_channel;

  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    return;
  }

  UTIL_SEQ_WaitEvt( 1U<< CFG_EVT_SET_CNF );

  /* Register Data emiter node task */
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_NODE, UTIL_SEQ_RFU, APP_LPM_TRANSMIT_MAC_802_15_4_NodeDataTask);

  /* Start LPTimer to trigg the data emiter node task */
  if (HAL_LPTIM_SetOnce_Start_IT(&hlptim1, PERIOD, TIMEOUT) != HAL_OK)
  {
    Error_Handler();
  }
  
  UTIL_LPM_SetStopMode(CFG_INIT_TASK, UTIL_LPM_ENABLE);
}

void APP_LPM_TRANSMIT_MAC_802_15_4_SendData(const char * data)
{
  MAC_Status_t MacStatus = MAC_ERROR;

  MAC_dataReq_t DataReq;
  DataReq.src_addr_mode = g_SHORT_ADDR_MODE_c;
  DataReq.dst_addr_mode = g_SHORT_ADDR_MODE_c;

  memcpy(DataReq.a_dst_PAN_id,&g_panId,0x02);
  memcpy(DataReq.dst_address.a_short_addr,&g_broadcastAddr,0x02);

  DataReq.msdu_handle = g_dataHandle++;
  /* No ACK is required */
  DataReq.ack_Tx =0x00;
  /* GTS not enabled */
  DataReq.GTS_Tx = FALSE;

  memcpy(&rfBuffer,data,strlen(data));

  /* Xor Data to potentially verified data paylaod on receiver side */
  rfBuffer[strlen(data)] = xorSign(data,strlen(data));


  DataReq.msduPtr = (uint8_t*) rfBuffer;
  DataReq.msdu_length = strlen(data)+1;

  /* No Security @ MAC level */
  DataReq.security_level = 0x00;

  /* Request Data transmission */

  MacStatus = MAC_MCPSDataReq( &DataReq );

  /* Treat Error in case of failure */
  if ( MAC_SUCCESS != MacStatus ) {
    /* To be implemented according to the application */
    return;
  }

  /* Wait for MCPS data Cnf - Cf. Data Cnf Callback */
  UTIL_SEQ_WaitEvt( 1U << CFG_EVT_DATA_DATA_CNF );
  
}
/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */

void APP_LPM_TRANSMIT_MAC_802_15_4_Error(uint32_t ErrId, uint32_t ErrCode)
{

  switch(ErrId)
  {
  case ERR_APPLI_REC_MULTI_MSG_FROM_RFCore :
    APP_LPM_TRANSMIT_MAC_802_15_4_TraceError("ERROR : ERR_REC_MULTI_MSG_FROM_RFCore ",ErrCode);
    break;
  case ERR_INTERFACE_IPCC_SEND_ACK_TO_RFCore :
    APP_LPM_TRANSMIT_MAC_802_15_4_TraceError("ERROR : ERR_IPCC_SEND_ACK_TO_RFCore ",ErrCode);
    break;
  default :
    APP_LPM_TRANSMIT_MAC_802_15_4_TraceError("ERROR Unknown ",0);
    break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void APP_LPM_TRANSMIT_MAC_802_15_4_Config()
{
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
  macCbConfig.mlmePollIndCb =  APP_MAC_mlmePollIndCb;
}

/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (OpenThread or other module if any)
  * @retval None
  */
static void APP_LPM_TRANSMIT_MAC_802_15_4_TraceError(char * pMess, uint32_t ErrCode)
{
  while(1 == 1)
  {
#if (CFG_LED_SUPPORTED == 1U)
    BSP_LED_Toggle(LED1);
    HAL_Delay(500);
    BSP_LED_Toggle(LED2);
    HAL_Delay(500);
    BSP_LED_Toggle(LED3);
#endif
    HAL_Delay(500);
  }
}


/************** APP RFD ********************/

/* Private variables ---------------------------------------------------------*/
/** @defgroup APP RFD private variables
 * @{
 */

/**
 * @}
 */

/* Global variables ----------------------------------------------------------*/
/** @defgroup APP RFD Global variable
 * @{
 */
/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup APP RFD private function prototypes
 * @{
 */

/* USER CODE BEGIN 4 */
/**
  * @brief  Compare match callback in non blocking mode 
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
  /* Timeout was reached, toggle LED2 */
  #if ( CFG_LED_SUPPORTED == 0x01 ) && ( CFG_LED_TRACE_LPTIMER == 0x01 )
  BSP_LED_Toggle(LED2);
  #endif
  HAL_LPTIM_SetOnce_Stop_IT(&hlptim1);
#if 1
#if 0
  if (HAL_LPTIM_SetOnce_Start_IT(&hlptim1, PERIOD, TIMEOUT) != HAL_OK)
  {
    BSP_LED_Toggle(LED1);
    Error_Handler();
  }
#endif
  // Request Data Transmit on Compare Match callback
  UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_NODE, CFG_SCH_PRIO_0 );
#endif
}
/* USER CODE END 4 */


/**
  * @brief  compute  simple xor signature of the data to transmit
  *
  * @param  pmessage   : Message to sign.
  * @param  message_len: Message Len
  * @retval Message Signature
  */

uint8_t xorSign( const char * pmessage, uint8_t message_len)
{
  uint8_t seed = 0x00;
  for (uint8_t i=0x00;i<message_len;i++)
    seed = (uint8_t)pmessage[i]^seed;
  return seed; 
}

/**
 * @}
 */


/* Private Functions Definition ------------------------------------------------------*/
/** @defgroup APP RFD Private function
 * @{
 */
/**
 * @}
 */

/* Functions Definition ------------------------------------------------------*/
/** @defgroup APP RFD function prototypes
 * @{
 */


/**
 * @}
 */


