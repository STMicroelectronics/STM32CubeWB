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

#include "app_rfd_mac_802_15_4_process.h"

#include "app_conf.h"
#include "dbg_trace.h"
#include "shci.h"
#include "stm32_seq.h"
#include "app_rfd_mac_802_15_4.h"
#include "stm_logging.h"

/* Global define -------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/


#ifndef FALSE
#define FALSE false
#endif
#ifndef TRUE
#define TRUE true
#endif


/* Private defines -----------------------------------------------------------*/
#define MAX_PIB_ATTRIBUTE_VALUE_LENGTH 52



/* Private macros ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

extern MAC_associateCnf_t g_MAC_associateCnf;

/*
* Init
*/

/******************************************************************************/


/******************************************************************************/


/* callbacks to be called by the MAC
* Create an NTS message and enqueue it in the IPCC queue
*/

MAC_Status_t APP_MAC_mlmeAssociateCnfCb( const  MAC_associateCnf_t * pAssociateCnf )
{
  APP_DBG("RFD MAC APP - Association CNF Received");
  memset(&g_MAC_associateCnf,0x00,sizeof(MAC_associateCnf_t));
  memcpy(&g_MAC_associateCnf,pAssociateCnf,sizeof(MAC_associateCnf_t));
  UTIL_SEQ_SetEvt(EVENT_SRV_ASSOC_CNF);
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeAssociateIndCb( const  MAC_associateInd_t * pAssociateInd )
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}

MAC_Status_t APP_MAC_mlmeBeaconNotifyIndCb( const  MAC_beaconNotifyInd_t * pBeaconNotifyInd )
{
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeCommStatusIndCb( const  MAC_commStatusInd_t * pCommStatusInd )
{
  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeDisassociateCnfCb( const  MAC_disassociateCnf_t * pDisassociateCnf )
{

  return MAC_SUCCESS;
}

MAC_Status_t APP_MAC_mlmeDisassociateIndCb( const  MAC_disassociateInd_t * pDisassociateInd )
{
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeGetCnfCb( const  MAC_getCnf_t * pGetCnf )
{
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeOrphanIndCb( const  MAC_orphanInd_t * pOrphanInd )
{
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmePollCnfCb( const  MAC_pollCnf_t * pPollCnf )
{
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeResetCnfCb( const  MAC_resetCnf_t * pResetCnf )
{
  UTIL_SEQ_SetEvt(EVENT_DEVICE_RESET_CNF);
  return MAC_SUCCESS;
}


MAC_Status_t APP_MAC_mlmeRxEnableCnfCb( const  MAC_rxEnableCnf_t * pRxEnableCnf )
{
  return MAC_SUCCESS;

}

MAC_Status_t APP_MAC_mlmeScanCnfCb( const  MAC_scanCnf_t * pScanCnf )
{
  return MAC_SUCCESS;
}


MAC_Status_t APP_MAC_mlmeSetCnfCb( const  MAC_setCnf_t * pSetCnf )
{
  UTIL_SEQ_SetEvt(EVENT_SET_CNF);
  return MAC_SUCCESS;
}


MAC_Status_t APP_MAC_mlmeStartCnfCb( const  MAC_startCnf_t * pStartCnf )
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}


MAC_Status_t APP_MAC_mcpsDataIndCb( const  MAC_dataInd_t * pDataInd )
{
  APP_DBG("COORD : RECEIVE DATA : %s ", (char const *) pDataInd->msduPtr);
  return MAC_SUCCESS;
}


MAC_Status_t APP_MAC_mcpsDataCnfCb( const  MAC_dataCnf_t * pDataCnf )
{
  UTIL_SEQ_SetEvt( EVENT_DATA_CNF );
  return MAC_SUCCESS;

}


MAC_Status_t APP_MAC_mcpsPurgeCnfCb( const  MAC_purgeCnf_t * pPurgeCnf )
{
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

// APP_MAC_mlmePollIndCbPtr  mlmePollIndCb;
MAC_Status_t APP_MAC_mlmePollIndCb( const MAC_pollInd_t * pPollInd )
{
  return MAC_NOT_IMPLEMENTED_STATUS;
}
/******************************************************************************/

//@} \\END CLI Commands

