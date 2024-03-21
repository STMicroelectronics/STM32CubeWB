/**
  ******************************************************************************
* @file    app_mac_802-15-4_process.c
* @author  MDG-RF Team
* @brief   MAC Cli Implementation for Demo Purpose
*
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


#include "app_conf.h"
#include "dbg_trace.h"
#include "shci.h"
#include "stm32_seq.h"
#include "app_mac_802_15_4.h"
#include "802_15_4_mac_sap.h"
#include "app_mac_802_15_4_process.h"
#include "stm_logging.h"

/* Global define -------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
extern MAC_associateCnf_t g_MAC_associateCnf;
extern uint8_t g_channel;
extern MAC_beaconNotifyInd_t  g_BeaconNotifyInd;
extern uint8_t deviceAssociated;
extern uint8_t beacon_coord_received;

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t checkMsgXorSignature(const char * pMessage, uint8_t message_len, uint8_t sign, uint8_t expectedRes);

/* Public variables ----------------------------------------------------------*/
MAC_dataInd_t      g_DataInd;


/* callbacks to be called by the MAC
* Create an NTS message and enqueue it in the IPCC queue
*/

MAC_Status_t APP_MAC_mlmeAssociateCnfCb( const  MAC_associateCnf_t * pAssociateCnf )
{
  if (pAssociateCnf->status == MAC_SUCCESS) { // If success
    APP_DBG("Node MAC APP - Association CNF Received, Short address is 0x%02x%02x on channel %d\0",
            pAssociateCnf->a_assoc_short_address[1],pAssociateCnf->a_assoc_short_address[0],g_channel);
    
    deviceAssociated = 1; // This device is associated now
    UTIL_SEQ_SetEvt(EVENT_ASSOCIATE_CNF);
    BSP_LED_On(LED2);
  } else {
    APP_DBG("Node MAC APP - Association CNF Received with bad status: 0x%02x\0", pAssociateCnf->status);
  }

  memcpy(&g_MAC_associateCnf, pAssociateCnf, sizeof(MAC_associateCnf_t)); // Copy table
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeAssociateIndCb( const  MAC_associateInd_t * pAssociateInd )
{
  /* return */
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeBeaconNotifyIndCb( const  MAC_beaconNotifyInd_t * pBeaconNotifyInd )
{
  uint16_t BeaconPanId = 0x0000; 
  uint8_t Beacon_payload[BEACON_PAYLOAD_SIZE];  
  
  memset(&Beacon_payload, 0x00, BEACON_PAYLOAD_SIZE);
  BeaconPanId = ((pBeaconNotifyInd->PAN_descriptor.a_coord_PAN_id[1])<<8) + pBeaconNotifyInd->PAN_descriptor.a_coord_PAN_id[0];
  if (pBeaconNotifyInd->sdu_length == BEACON_PAYLOAD_SIZE) { // First filter with the size
    memcpy(&Beacon_payload, pBeaconNotifyInd->sduPtr, pBeaconNotifyInd->sdu_length); // Getting the beacon payload
    if (strncmp(BEACON_PAYLOAD, (char const *) &Beacon_payload, BEACON_PAYLOAD_SIZE) == 0) { // Second filter with the payload
      APP_DBG("Node MAC APP - Receive BeaconNotify.Ind with the correct payload, PANID: 0x%04x, channel: %d\0", BeaconPanId, pBeaconNotifyInd->PAN_descriptor.logical_channel);
      memcpy(&g_BeaconNotifyInd, pBeaconNotifyInd, sizeof(MAC_beaconNotifyInd_t)); // Copy table
      beacon_coord_received  = 0x01;
      return MAC_SUCCESS;
    }
  }
  APP_DBG("Node MAC APP - Receive BeaconNotify.Ind with the incorrect payload, PANID: 0x%04x, channel: %d\0", BeaconPanId, pBeaconNotifyInd->PAN_descriptor.logical_channel);
  
  /* return */
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeCommStatusIndCb( const  MAC_commStatusInd_t * pCommStatusInd )
{
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeDisassociateCnfCb( const  MAC_disassociateCnf_t * pDisassociateCnf )
{
  UTIL_SEQ_SetEvt(EVENT_DISASSOCIATE_CNF);
  APP_DBG("Node MAC APP - Disassociate CNF RECEIVED\0");
  deviceAssociated = 0; // This device is disassociated now
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeDisassociateIndCb( const  MAC_disassociateInd_t * pDisassociateInd )
{
  APP_DBG("Node MAC APP - Disassociate.Ind RECEIVED by 0x%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\0", pDisassociateInd->a_device_address[7],
            pDisassociateInd->a_device_address[6],pDisassociateInd->a_device_address[5],pDisassociateInd->a_device_address[4],pDisassociateInd->a_device_address[3],
            pDisassociateInd->a_device_address[2],pDisassociateInd->a_device_address[1],pDisassociateInd->a_device_address[0]);
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeGetCnfCb( const  MAC_getCnf_t * pGetCnf )
{
  UTIL_SEQ_SetEvt(EVENT_GET_CNF);
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeOrphanIndCb( const  MAC_orphanInd_t * pOrphanInd )
{
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmePollCnfCb( const  MAC_pollCnf_t * pPollCnf )
{
  UTIL_SEQ_SetEvt(EVENT_POLL_CNF);
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeResetCnfCb( const  MAC_resetCnf_t * pResetCnf )
{
  UTIL_SEQ_SetEvt(EVENT_RESET_CNF);
  return MAC_SUCCESS;
}


MAC_Status_t APP_MAC_mlmeRxEnableCnfCb( const  MAC_rxEnableCnf_t * pRxEnableCnf )
{
  UTIL_SEQ_SetEvt(EVENT_RX_ON_WHEN_IDLE_CNF);
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeScanCnfCb( const  MAC_scanCnf_t * pScanCnf )
{
  APP_DBG("Node MAC APP - Scan CNF Received\0");
  UTIL_SEQ_SetEvt(EVENT_SCAN_CNF);
  /* return */
  return MAC_SUCCESS;
}


MAC_Status_t APP_MAC_mlmeSetCnfCb( const  MAC_setCnf_t * pSetCnf )
{
  UTIL_SEQ_SetEvt(EVENT_SET_CNF);
  return MAC_SUCCESS;
}


MAC_Status_t APP_MAC_mlmeStartCnfCb( const  MAC_startCnf_t * pStartCnf )
{
  UTIL_SEQ_SetEvt(EVENT_START_CNF);
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mcpsDataIndCb( const  MAC_dataInd_t * pDataInd )
{
  memcpy(&g_DataInd,pDataInd,sizeof(MAC_dataInd_t));
  // Check validity of the received Message extracting associated 
  // simple xor signature
  if (!checkMsgXorSignature((char const *)(pDataInd->msduPtr),
                           pDataInd->msdu_length-1,
                           pDataInd->msduPtr[pDataInd->msdu_length-1],
                           0x00))
  {
    APP_DBG("Node MAC APP - ERROR : CORRUPTED RECEIVED DATA ");
  }
  else
  {
    pDataInd->msduPtr[pDataInd->msdu_length-1] = '\0';//erase signature with EOS
    APP_DBG("Node MAC APP - RECEIVE DATA : %s", (char const *) pDataInd->msduPtr);
    BSP_LED_On(LED3);
    HAL_Delay(300);
    BSP_LED_Off(LED3);
  }
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mcpsDataCnfCb( const  MAC_dataCnf_t * pDataCnf )
{
  UTIL_SEQ_SetEvt(EVENT_DATA_CNF);
  
  /* return */
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mcpsPurgeCnfCb( const  MAC_purgeCnf_t * pPurgeCnf )
{
  UTIL_SEQ_SetEvt(EVENT_PURGE_CNF);
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmePollIndCb( const MAC_pollInd_t * pPollInd )
{
  /* return */
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeSyncLossIndCb( const MAC_syncLoss_t * syncLossPtr )
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}


MAC_Status_t APP_MAC_mlmeCalibrateCnfCb( const MAC_calibrateCnf_t * pCallibrateCnf)
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}



MAC_Status_t APP_MAC_mlmeDpsCnfCb( const MAC_dpsCnf_t * pDpsCnf  )
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}


MAC_Status_t APP_MAC_mlmeDpsIndCb( const MAC_dpsInd_t * pDpsInd )
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}


MAC_Status_t APP_MAC_mlmeSoundingCnfCb( const MAC_soundingCnf_t * pSoudingCnf)
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}

MAC_Status_t APP_MAC_mlmeGtsCnfCb( const MAC_gtsCnf_t * pGtsCnf)
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}


MAC_Status_t APP_MAC_mlmeGtsIndCb( const MAC_GtsInd_t * pGtsInd )
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}

/* Private function Definition -----------------------------------------------*/

uint8_t checkMsgXorSignature(const char * pMessage, uint8_t message_len, 
                             uint8_t sign, uint8_t expectedRes)
{
  uint8_t seed = sign;
  for (uint8_t i=0x00;i<message_len;i++)
    seed = (uint8_t)pMessage[i]^seed;
  return (seed==expectedRes);
}

/******************************************************************************/

//@} \\END CLI Commands

