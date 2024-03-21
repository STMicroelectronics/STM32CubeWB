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

#include "app_mac_802_15_4_process.h"

#include "app_conf.h"
#include "dbg_trace.h"
#include "shci.h"
#include "stm32_seq.h"
#include "app_mac_802_15_4.h"
#include "802_15_4_mac_sap.h"
#include "stm_logging.h"

/* Global define -------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
extern uint8_t g_channel; // Channel selected with ED scan
extern uint16_t shortAssociationAddrList[MAX_DEVICES_ASSO]; // table with all devices short address
extern uint8_t idAssociationList; // Number of devices associated
extern uint8_t indDeviceSendData; // Which device to send data
extern uint16_t tab_panId_already_use[MAX_TAB_PAN_ID]; // Table with all PAN ID already use in the local environment
extern MAC_associateInd_t g_MAC_associateInd;
extern uint8_t status_check_device; // Return device status when BT2 is pushed

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t checkMsgXorSignature(const char * pMessage, uint8_t message_len, 
                             uint8_t sign, uint8_t expectedRes);

/* Public variables ----------------------------------------------------------*/
MAC_dataInd_t      g_DataInd;


/*! callbacks to be called by the MAC layer */
MAC_Status_t APP_MAC_mlmeAssociateCnfCb( const  MAC_associateCnf_t * pAssociateCnf )
{
  UTIL_SEQ_SetEvt(CFG_EVT_ASSOCIATE_CNF);
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeAssociateIndCb( const  MAC_associateInd_t * pAssociateInd )
{
  /* Store ASSOCIATION IND */
  MAC_Status_t MacStatus = MAC_ERROR;
  MAC_associateRes_t AssociateRes;
  uint8_t address_selected = 0;
  uint16_t shortAssociationAddr = COORD_ADDRESS; // Reset this variable each time we have association
  
  /* Selected association address in a range of FIRST_ASSOCIATED_SHORT_ADDRESS to FIRST_ASSOCIATED_SHORT_ADDRESS + MAX_DEVICES_ASSO */
  for (uint8_t indiceAsso=0;indiceAsso<MAX_DEVICES_ASSO && shortAssociationAddr==COORD_ADDRESS ;indiceAsso++) {
      for (uint8_t i=0;i<idAssociationList;i++){
        if (shortAssociationAddrList[i] == (FIRST_ASSOCIATED_SHORT_ADDRESS + indiceAsso)) {
         address_selected = 1;
        } 
    }
    if (!address_selected){
      shortAssociationAddr = FIRST_ASSOCIATED_SHORT_ADDRESS + indiceAsso; // This value is selected
    }
    address_selected = 0;
  }

  /* If any address is free */
  if (shortAssociationAddr == 0x2000) {
    APP_DBG("COORD MAC APP - Association Response Fails max device associated\0");
    UTIL_SEQ_SetTask(1U<<CFG_TASK_SEND_DATA, CFG_SCH_PRIO_0);
    return MacStatus;
  }
  
  shortAssociationAddrList[idAssociationList] = shortAssociationAddr; // Add in tab the address of the new device
  idAssociationList++; // Increase number of device

  memcpy(&g_MAC_associateInd,pAssociateInd,sizeof(MAC_associateInd_t));

  /* structure initialization */
  memset(&AssociateRes, 0x00, sizeof(MAC_associateRes_t));
  memcpy(AssociateRes.a_device_address, g_MAC_associateInd.a_device_address, 0x08);
  memcpy(AssociateRes.a_assoc_short_address, &shortAssociationAddr, 0x02);
  AssociateRes.security_level = 0x00; // Unsecure frame
  AssociateRes.status = MAC_SUCCESS;

  MacStatus = MAC_MLMEAssociateRes(&AssociateRes);
  if ( MAC_SUCCESS != MacStatus ) {
   APP_DBG("COORD MAC APP - Association Response Fails\0");
  }
  BSP_LED_On(LED2);
  APP_DBG("COORD MAC APP - Association done and the short address given 0x%04x\0", shortAssociationAddr);
  /* return */
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeBeaconNotifyIndCb( const  MAC_beaconNotifyInd_t * pBeaconNotifyInd )
{
  uint16_t BeaconPanId = 0x0000; 
  static uint8_t indice_tab = 0;
  uint8_t tmp_pan_id_use = 0;
    
  BeaconPanId = ((pBeaconNotifyInd->PAN_descriptor.a_coord_PAN_id[1])<<8) + pBeaconNotifyInd->PAN_descriptor.a_coord_PAN_id[0];
  APP_DBG("COORD MAC APP - Receive BeaconNotify.Ind with PANID: 0x%04x\0", BeaconPanId);
  
  /* Check if the pan id is not in the tab */
  for (uint8_t i=0;i<MAX_TAB_PAN_ID;i++) {
    if (tab_panId_already_use[i] == BeaconPanId) {
      tmp_pan_id_use = 1;
    }
  }
  if (!tmp_pan_id_use && indice_tab <= MAX_TAB_PAN_ID) {
    tab_panId_already_use[indice_tab++] = BeaconPanId;
  }
  /* return */
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeCommStatusIndCb( const  MAC_commStatusInd_t * pCommStatusInd )
{
  if (pCommStatusInd->status == MAC_SUCCESS) {
    if (pCommStatusInd->dst_addr_mode == g_SHORT_ADDR_MODE_c) {
      APP_DBG("COORD MAC APP - Association succeed for 0x%02x%02x\0", pCommStatusInd->dst_address.a_short_addr[0], pCommStatusInd->dst_address.a_short_addr[1]);
    }else {
      if (pCommStatusInd->dst_addr_mode == g_EXTENDED_ADDR_MODE_c) {
        APP_DBG("COORD MAC APP - Association succeed for 0x%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\0", pCommStatusInd->dst_address.a_extend_addr[7], 
                pCommStatusInd->dst_address.a_extend_addr[6],pCommStatusInd->dst_address.a_extend_addr[5],pCommStatusInd->dst_address.a_extend_addr[4],
                pCommStatusInd->dst_address.a_extend_addr[3],pCommStatusInd->dst_address.a_extend_addr[2],pCommStatusInd->dst_address.a_extend_addr[1],
                pCommStatusInd->dst_address.a_extend_addr[0]);
      }
    }
  }
  /* return */
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeDisassociateCnfCb( const  MAC_disassociateCnf_t * pDisassociateCnf )
{
  UTIL_SEQ_SetEvt(EVENT_DISASSOCIATE_CNF);
  APP_DBG("COORD MAC APP - Disassociate CNF RECEIVED\0");
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeDisassociateIndCb( const  MAC_disassociateInd_t * pDisassociateInd )
{
  APP_DBG("COORD MAC APP - Disassociate IND RECEIVED by 0x%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\0", pDisassociateInd->a_device_address[7],
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
  uint8_t tmp_energy = 0xFF;
  uint8_t i = 0;
  /* Select the best channel */
  if(pScanCnf->status == MAC_SUCCESS) {
    if (pScanCnf->scan_type==0x00) {// ED scan 
      while(i < g_MAX_ED_SCAN_RESULTS_SUPPORTED_c) { 
        /* Select the least noisy channel*/
        if (pScanCnf->a_energy_detect_list[i] < tmp_energy ) {
          //APP_DBG("FFD MAC APP - channel %d : %x\0", i+11, pScanCnf->a_energy_detect_list[i]);
          g_channel = 11 + i;
          tmp_energy = pScanCnf->a_energy_detect_list[i];
        }
        i++;
    }
    APP_DBG("COORD MAC APP - Channel selected %d\0", g_channel);
    }
  }
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
    APP_DBG("COORD MAC APP - ERROR : CORRUPTED RECEIVED DATA\0");
  }
  else
  {
    pDataInd->msduPtr[pDataInd->msdu_length-1] = '\0';//erase signature with EOS
    APP_DBG("COORD MAC APP - RECEIVE DATA : %s", (char const *) pDataInd->msduPtr);
    BSP_LED_On(LED3);
    HAL_Delay(300);
    BSP_LED_Off(LED3);
  }
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mcpsDataCnfCb( const  MAC_dataCnf_t * pDataCnf )
{
  UTIL_SEQ_SetEvt(EVENT_DATA_CNF);
  UTIL_SEQ_SetTask(1U << CFG_TASK_SEND_DATA, CFG_SCH_PRIO_0);
  
  status_check_device = pDataCnf->status;
  if (pDataCnf->status != MAC_SUCCESS) {
    /* The device is disconnected */
    uint16_t tmp_shortAssociationAddrList[MAX_DEVICES_ASSO]; // Create temporary tab
    memset(&tmp_shortAssociationAddrList, 0x00, idAssociationList*2); // Add 0 in the tab
    memcpy(&tmp_shortAssociationAddrList, &shortAssociationAddrList, idAssociationList*2); // Copy table
    uint8_t indiceAsso = 0;
    for (uint8_t i=0;i<idAssociationList;i++) {
      if (indDeviceSendData != i) {
        shortAssociationAddrList[indiceAsso] = tmp_shortAssociationAddrList[i];
        indiceAsso++;
      }
    }
    /* Element to remove -> shortAssociationAddrList[indDeviceSendData] */
    idAssociationList--;
  }
  
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
    if (pPollInd->addr_mode == g_SHORT_ADDR_MODE_c) { // Display only short address
    APP_DBG("COORD MAC APP - POLL IND RECEIVED by 0x%02x%02x\0", pPollInd->request_address.a_short_addr[1],pPollInd->request_address.a_short_addr[0]);
  }
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

