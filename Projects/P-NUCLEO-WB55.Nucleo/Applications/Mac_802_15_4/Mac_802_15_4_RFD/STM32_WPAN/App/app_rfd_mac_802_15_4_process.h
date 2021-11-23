
/**
  ******************************************************************************
  * @file    app_rfd_mac_802_15_4_process.h
  * @author  MCD Application Team
  * @brief   Header for RFD MAC 802.15.4 processing activtities
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_RFD_MAC_802_15_4_PROCESS_H
#define APP_RFD_MAC_802_15_4_PROCESS_H


#ifdef __cplusplus
extern "C" {
#endif

#include "802_15_4_mac_types.h"


void callMLMEGetReq( uint8_t *p_nts_message);
void callMLMESetReq( uint8_t *p_nts_message);
void callMLMEResetReq(uint8_t *p_nts_message);

void callMLMEDisassociateReq(uint8_t *p_nts_message);
void callMCPSPurgeReq(uint8_t *p_nts_message);
void callMLMEAssociateReq(uint8_t *p_nts_message);
void callMLMEAssociateRes(uint8_t *p_nts_message);
void callMLMEStartReq(uint8_t *p_nts_message);
void callMLMEOrphanRes(uint8_t *p_nts_message);
void callMLMERxEnableReq(uint8_t *p_nts_message);
void callMLMEScanReq(uint8_t *p_nts_message);

void callMLMEPollReq(uint8_t *p_nts_message);
void callMCPSDataReq(uint8_t *p_nts_message);


MAC_Status_t APP_MAC_mlmeAssociateCnfCb( const  MAC_associateCnf_t * pAssociateCnf );
MAC_Status_t APP_MAC_mlmeAssociateIndCb( const  MAC_associateInd_t * pAssociateInd );
MAC_Status_t APP_MAC_mlmeBeaconNotifyIndCb( const  MAC_beaconNotifyInd_t * pBeaconNotifyInd );
MAC_Status_t APP_MAC_mlmeCommStatusIndCb( const  MAC_commStatusInd_t * pCommStatusInd );
MAC_Status_t APP_MAC_mlmeDisassociateCnfCb( const  MAC_disassociateCnf_t * pDisassociateCnf );
MAC_Status_t APP_MAC_mlmeDisassociateIndCb( const  MAC_disassociateInd_t * pDisassociateInd );
MAC_Status_t APP_MAC_mlmeGetCnfCb( const  MAC_getCnf_t * pGetCnf );
MAC_Status_t APP_MAC_mlmeOrphanIndCb( const  MAC_orphanInd_t * pOrphanInd );
MAC_Status_t APP_MAC_mlmePollCnfCb( const  MAC_pollCnf_t * pPollCnf );
MAC_Status_t APP_MAC_mlmeResetCnfCb( const  MAC_resetCnf_t * pResetCnf );
MAC_Status_t APP_MAC_mlmeRxEnableCnfCb( const  MAC_rxEnableCnf_t * pRxEnableCnf );
MAC_Status_t APP_MAC_mlmeScanCnfCb( const  MAC_scanCnf_t * pScanCnf );
MAC_Status_t APP_MAC_mlmeSetCnfCb( const  MAC_setCnf_t * pSetCnf );
MAC_Status_t APP_MAC_mlmeStartCnfCb( const  MAC_startCnf_t * pStartCnf );
MAC_Status_t APP_MAC_mcpsDataIndCb( const  MAC_dataInd_t * pDataInd );
MAC_Status_t APP_MAC_mcpsDataCnfCb( const  MAC_dataCnf_t * pDataCnf );
MAC_Status_t APP_MAC_mcpsPurgeCnfCb( const  MAC_purgeCnf_t * pPurgeCnf );
MAC_Status_t APP_MAC_mlmePollIndCb( const MAC_pollInd_t * pPollInd );


MAC_Status_t APP_MAC_mlmeSyncLossIndCb( const MAC_syncLoss_t * syncLossPtr );
MAC_Status_t APP_MAC_mlmeCalibrateCnfCb( const MAC_calibrateCnf_t * pCallibrateCnf);
MAC_Status_t APP_MAC_mlmeDpsCnfCb( const MAC_dpsCnf_t * pDpsCnf  );
MAC_Status_t APP_MAC_mlmeDpsIndCb( const MAC_dpsInd_t * pDpsInd );
MAC_Status_t APP_MAC_mlmeSoundingCnfCb( const MAC_soundingCnf_t * pSoudingCnf);
MAC_Status_t APP_MAC_mlmeGtsCnfCb( const MAC_gtsCnf_t * pGtsCnf);
MAC_Status_t APP_MAC_mlmeGtsIndCb( const MAC_GtsInd_t * pGtsInd );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_RFD_MAC_802_15_4_PROCESS_H */
