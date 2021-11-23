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
#include "app_ffd_mac_802_15_4.h"

#include "802_15_4_mac_sap.h"
#include "app_ffd_mac_802_15_4_process.h"

#include "dbg_trace.h"
#include "shci.h"
#include "stm_logging.h"


#define DEMO_CHANNEL 16


MAC_associateInd_t g_MAC_associateInd;


/* Private function prototypes -----------------------------------------------*/
static void APP_FFD_MAC_802_15_4_TraceError(char * pMess, uint32_t ErrCode);
static void APP_FFD_MAC_802_15_4_Config(void);

MAC_callbacks_t macCbConfig ;


uint8_t g_srvSerReq;
uint8_t g_srvDataReq;


__IO ITStatus CertifOutputPeripheralReady = SET;


/* Functions Definition ------------------------------------------------------*/

void APP_FFD_MAC_802_15_4_Init( APP_MAC_802_15_4_InitMode_t InitMode, TL_CmdPacket_t* pCmdBuffer)
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

  UTIL_SEQ_RegTask( 1<<CFG_TASK_FFD, UTIL_SEQ_RFU,APP_FFD_MAC_802_15_4_SetupTask);

  UTIL_SEQ_RegTask( 1<<CFG_TASK_SERVICE_COORD, UTIL_SEQ_RFU,APP_FFD_MAC_802_15_4_CoordSrvTask);

  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_COORD, UTIL_SEQ_RFU,APP_FFD_MAC_802_15_4_CoordDataTask);

  /* Configuration MAC 802_15_4 */
  APP_FFD_MAC_802_15_4_Config();


  /*Start Main Coordinator - FFD Task*/
  UTIL_SEQ_SetTask( 1<< CFG_TASK_FFD, CFG_SCH_PRIO_0 );

}


void APP_FFD_MAC_802_15_4_CoordSrvTask(void)
{
  MAC_Status_t MacStatus = MAC_ERROR;

  MAC_associateRes_t AssociateRes;
  uint16_t shortAssociationAddr = 0x3344;

  APP_DBG("Srv task :");

  switch (g_srvSerReq)
  {
  case CFG_ASSO_PENDING:
    APP_DBG("Srv task : Response to Association Indication");

    memcpy(AssociateRes.a_device_address,g_MAC_associateInd.a_device_address,0x08);
    memcpy(AssociateRes.a_assoc_short_address,&shortAssociationAddr,0x2);
    AssociateRes.security_level = 0x00;
    AssociateRes.status = MAC_SUCCESS;

    MacStatus = MAC_MLMEAssociateRes(&AssociateRes);
    if ( MAC_SUCCESS != MacStatus ) {
      APP_DBG("FFD MAC - Association Response Fails");
      return;
    }
    break;
  default:
    APP_DBG("Srv task : unknown Request");
  }
  g_srvSerReq = CFG_SRV_SER_REQ_NBR;
}

void APP_FFD_MAC_802_15_4_CoordDataTask(void)
{
  APP_DBG("Data task :");
  switch (g_srvDataReq)
  {
    case CFG_DATA_PENDING:
      BSP_LED_Toggle(LED3);
      APP_DBG("Data task : Data Indication Receive");
    default:
      APP_DBG("Data task : unknown Request");
  }
  g_srvDataReq = CFG_SRV_DATA_REQ_NBR;
}


void APP_FFD_MAC_802_15_4_SetupTask(void)
{

  MAC_Status_t MacStatus = MAC_ERROR;

  MAC_resetReq_t    ResetReq;
  MAC_setReq_t      SetReq;
  MAC_startReq_t    StartReq;

  long long extAddr = 0xACDE480000000001;
  uint16_t shortAddr   = 0x1122;
  uint16_t panId       = 0x1AAA;
  uint8_t channel      = DEMO_CHANNEL;
  uint8_t PIB_Value = 0x00;
  
  int8_t tx_power_pib_value = 0;

  APP_DBG("Run FFD MAC 802.15.4 - 2 - FFD Startup");

  /* Reset FFD Device */
  /* Reset MAC */
  memset(&ResetReq,0x00,sizeof(MAC_resetReq_t));
  ResetReq.set_default_PIB = TRUE;

  MacStatus = MAC_MLMEResetReq( &ResetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    APP_DBG("FFD MAC - Reset Fails");
    return;
  }
  /* Wait for Reset Confirmation */
  UTIL_SEQ_WaitEvt( 1U<< CFG_EVT_DEVICE_RESET_CNF );
  APP_DBG("FFD MAC APP - Reset CNF Received");

  /* Set Device Extended Address */
  APP_DBG("FFD MAC APP - Set Extended Address");
  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_MAC_EXTENDED_ADDRESS_c;
  SetReq.PIB_attribute_valuePtr = (uint8_t*) &extAddr;
  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    APP_DBG("FFD MAC - Set Extended Addr Fails");
    return;
  }
  UTIL_SEQ_WaitEvt( 1U<< CFG_EVT_SET_CNF );
  APP_DBG("FFD MAC APP - Set Extended Address CNF Received");


  /* Set Device Short Address */
  APP_DBG("FFD MAC APP - Set Short Address");
  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_MAC_SHORT_ADDRESS_c;
  SetReq.PIB_attribute_valuePtr =(uint8_t*) &shortAddr;
  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    APP_DBG("FFD MAC - Set Short Addr Fails");
    return;
  }

  UTIL_SEQ_WaitEvt( 1U << CFG_EVT_SET_CNF );
  APP_DBG("FFD MAC APP - Set Short Address CNF Received");

  /* Set Association Permit */
  APP_DBG("FFD MAC APP - Set Association Permit");

  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_MAC_ASSOCIATION_PERMIT_c;
  PIB_Value = g_TRUE;
  SetReq.PIB_attribute_valuePtr = &PIB_Value;

  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    APP_DBG("FFD MAC - Set Association Permit Fails");
    return;
  }
  UTIL_SEQ_WaitEvt( 1U << CFG_EVT_SET_CNF );
  APP_DBG("FFD MAC APP - Set Association Permit CNF Received");



  /* Set Tx Power */

  APP_DBG("FFD MAC APP - Set TX Power");
  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_PHY_TRANSMIT_POWER_c;
  tx_power_pib_value = 2;                       /* int8 dBm value in the range [-21;6] */
  SetReq.PIB_attribute_valuePtr = (uint8_t *)&tx_power_pib_value;

  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    APP_DBG("FFD MAC - Set Tx Power Fails");
    return;
  }
  UTIL_SEQ_WaitEvt( 1U << CFG_EVT_SET_CNF );
  
  
  /* Start Device */
  APP_DBG("FFD MAC APP - Start FFD Device");
  memset(&StartReq,0x00,sizeof(MAC_startReq_t));
  memcpy(StartReq.a_PAN_id,(uint8_t*)&panId,0x02);
  StartReq.channel_number   = channel;
  StartReq.beacon_order     = 0x0F;
  StartReq.superframe_order = 0x0F;
  StartReq.PAN_coordinator  = g_TRUE;
  StartReq.battery_life_extension = g_FALSE;

  MacStatus = MAC_MLMEStartReq( &StartReq);
  if ( MAC_SUCCESS != MacStatus ) {
    APP_DBG("FFD MAC - Set Association Permit Fails");
    return;
  }
  UTIL_SEQ_WaitEvt( 1U << CFG_EVT_DEVICE_STARTED_CNF );
  APP_DBG("FFD MAC APP - Start FFD Device CNF Received");


  /* Set RxOnWhenIdle */
  APP_DBG("FFD MAC APP - Set RX On When Idle");
  memset(&SetReq,0x00,sizeof(MAC_setReq_t));
  SetReq.PIB_attribute = g_MAC_RX_ON_WHEN_IDLE_c;
  PIB_Value = g_TRUE;
  SetReq.PIB_attribute_valuePtr = &PIB_Value;

  MacStatus = MAC_MLMESetReq( &SetReq );
  if ( MAC_SUCCESS != MacStatus ) {
    APP_DBG("FFD MAC - Set Rx On When Idle Fails");
    return;
  }
  UTIL_SEQ_WaitEvt( 1U << CFG_EVT_SET_CNF );
  APP_DBG("FFD MAC APP - Set RX On When Idle CNF Received");
  /* Go in Echo loop */
  APP_DBG("FFD MAC APP - Ready to Handle Association Req and Receive Data");
  BSP_LED_Toggle(LED1);
}

/**
  * @brief  Trace the error or the warning reported.
  * @param  ErrId :
  * @param  ErrCode
  * @retval None
  */

void APP_FFD_MAC_802_15_4_Error(uint32_t ErrId, uint32_t ErrCode)
{

  switch(ErrId)
  {
  case ERR_APPLI_REC_MULTI_MSG_FROM_RFCore :
    APP_FFD_MAC_802_15_4_TraceError("ERROR : ERR_REC_MULTI_MSG_FROM_RFCore ",ErrCode);
    break;
  case ERR_INTERFACE_IPCC_SEND_ACK_TO_RFCore :
    APP_FFD_MAC_802_15_4_TraceError("ERROR : ERR_IPCC_SEND_ACK_TO_RFCore ",ErrCode);
    break;
  default :
    APP_FFD_MAC_802_15_4_TraceError("ERROR Unknown ",0);
    break;
  }
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void APP_FFD_MAC_802_15_4_Config()
{
  APP_DBG("configure FFD MAC 802.15.4 - 2");
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

/**
  * @brief  Warn the user that an error has occurred.In this case,
  *         the LEDs on the Board will start blinking.
  *
  * @param  pMess  : Message associated to the error.
  * @param  ErrCode: Error code associated to the module (OpenThread or other module if any)
  * @retval None
  */
static void APP_FFD_MAC_802_15_4_TraceError(char * pMess, uint32_t ErrCode)
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


/************** APP FFD ********************/

/* Private variables ---------------------------------------------------------*/
/** @defgroup APP FFD private variables
 * @{
 */

/**
 * @}
 */

/* Global variables ----------------------------------------------------------*/
/** @defgroup APP FFD Global variable
 * @{
 */
/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup APP FFD private function prototypes
 * @{
 */


/**
 * @}
 */


/* Private Functions Definition ------------------------------------------------------*/
/** @defgroup APP FFD Private function
 * @{
 */
/**
 * @}
 */

/* Functions Definition ------------------------------------------------------*/
/** @defgroup APP FFD function prototypes
 * @{
 */


/**
 * @}
 */


