/**
 ******************************************************************************
 * @file    802_15_4_mac_sap.h
 * @author  MDG-RF Team
 * @brief   Header for mac interface API
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef _802_15_4_MAC_SAP_H_
#define _802_15_4_MAC_SAP_H_

/* Includes ------------------------------------------------------------------*/
#include "802_15_4_mac_types.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* general */


/* Request */
MAC_Status_t MAC_MLMEAssociateReq( const  MAC_associateReq_t * pAssociateReq );
MAC_Status_t MAC_MLMEDisassociateReq( const  MAC_disassociateReq_t * pDisassiociateReq );
MAC_Status_t MAC_MLMEGetReq( const  MAC_getReq_t * pGetReq );
MAC_Status_t MAC_MLMEGtsReq( const  MAC_gtsReq_t * pGtsReq );
MAC_Status_t MAC_MLMEResetReq( const  MAC_resetReq_t * pResetReq );
MAC_Status_t MAC_MLMERxEnableReq( const  MAC_rxEnableReq_t * pRxEnableReq );
MAC_Status_t MAC_MLMEScanReq( const  MAC_scanReq_t * pScanReq );
MAC_Status_t MAC_MLMESetReq( const  MAC_setReq_t * pSetReq);
MAC_Status_t MAC_MLMEStartReq( const  MAC_startReq_t * pStartReq);
MAC_Status_t MAC_MLMESyncReq( const  MAC_syncReq_t * pSyncReq );
MAC_Status_t MAC_MLMEPollReq( const MAC_pollReq_t * pPollReq );
MAC_Status_t MAC_MLMEDpsReq( const MAC_dpsReq_t * pDpsReq);
MAC_Status_t MAC_MLMESoundingReq( const MAC_soundingReq_t * pSoundingReq);
MAC_Status_t MAC_MLMECalibrateReq( const MAC_calibrateReq_t * pCalibrateReq);

MAC_Status_t MAC_MCPSDataReq( const MAC_dataReq_t * pDataReq );
MAC_Status_t MAC_MCPSPurgeReq( const MAC_purgeReq_t * pPurgeReq );


/* Response */
MAC_Status_t MAC_MLMEAssociateRes(const MAC_associateRes_t * pAssociateRes);
MAC_Status_t MAC_MLMEOrphanRes(const MAC_orphanRes_t * pOrphanRes);


#endif /* _802_15_4_MAC_SAP_H_ */
