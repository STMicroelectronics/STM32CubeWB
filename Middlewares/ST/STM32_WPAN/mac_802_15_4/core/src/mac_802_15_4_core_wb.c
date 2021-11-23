/**
 ******************************************************************************
 * @file    mac_802_15_4_api_wb.c
 * @author  MCD Application Team
 * @brief   Contains STM32WB specificities requested to control the 802.15.4
 *          MAC interface.
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


/* Includes ------------------------------------------------------------------*/

#include "802_15_4_mac_errors.h"
#include "802_15_4_mac_config.h"
#include "802_15_4_mac_types.h"
#include "802_15_4_mac_sap.h"
#include "802_15_4_mac_core.h"

#include "dbg_trace.h"
#include "stm_logging.h"

#include "tl.h"
#include "tl_mac_802_15_4.h"


/** @defgroup STM32WB55_MAC STM32WBxx MAC 802.15.4 API
  * @brief Provides MAC 802.15.4 functionalities to address MAC primitives accessed through
  *                  MLME-SAP and MCPS-SAP.<br>
  *                  Once RF-Core is initialized, Zigbee or proprietary network layer address RF-Core
  *                  using MLME or MCPS SAP primitives.<br>
  *                  On completion or on MAC-RF event, RF-Core sends back notification (ie.: MAC confirmation or MAC indication)
  *                  to the running application.<br>
  *                  Zigbee or proprietary network layer is notified (MAC_802_15_4_CallBack_Processing(void)) by the corresponding user defined callback call
  *                  that has been previously initialized in MAC_callbacks_t macCbConfig.<br>
  * @{
  */

/**
 * @brief  MAC Callbacks Structure registering user callback to be notify from MAC layers on
 *         MAC Confirmation and MAC Indications.
 */
extern MAC_callbacks_t macCbConfig;

/**
  * @}
  */

/**
 * @brief   This system function is used to set defined command ID in message between
 *          Application core and Rf core.
 *
 *
 * @param  None
 * @retval None
 */

static void utils_mac_set_cmdCode(uint16_t cmdCode)
{
  /* Set ID of the cmd in cmdcode field */

  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();
  p_mac_cmd_req->cmdserial.cmd.cmdcode = (0x3F << 9) | (MAC_802_15_4_CMD_OPCODE_OFFSET + cmdCode);
}

/** @addtogroup STM32WB55_MAC_CORE
 *
 */

/**
 * @brief  Handle MAC SAP's confirmations and indications coming from
 *         RFCore's MAC layer.
 *         On received RFCore Mac Message, calls upper layer user defined associated callbacks from MAC_callbacks_t macCbConfig .
 *         Important Note: This method is called on every MAC message from RFCore.
 *
 * @param  None
 * @retval None
 */
HAL_StatusTypeDef MAC_802_15_4_CallBack_Processing(void)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Get pointer on received event buffer from M0 */
  TL_Evt_t * p_notif_evt = MAC_802_15_4_GetNotificationBuffer();
  MAC_802_15_4_Notification_t* p_mac_evt = (MAC_802_15_4_Notification_t*)(p_notif_evt->payload);

  uint8_t subEvtCode = p_mac_evt->subEvtCode;

  switch(subEvtCode)
  {
  case MSG_M0TOM4_MAC_MLME_RESET_CNF:
    {
      MAC_resetCnf_t resetCnf;
      memcpy(&resetCnf,p_mac_evt->notPayload, sizeof(MAC_resetCnf_t));
      macCbConfig.mlmeResetCnfCb(&resetCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_ASSOCIATE_CNF:
    {
      MAC_associateCnf_t associateCnf;
      memcpy(&associateCnf,p_mac_evt->notPayload, sizeof(MAC_associateCnf_t));
      macCbConfig.mlmeAssociateCnfCb(&associateCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_DISASSOCIATE_CNF:
    {
      MAC_disassociateCnf_t  disassociateCnf;
      memcpy(&disassociateCnf,p_mac_evt->notPayload, sizeof(MAC_disassociateCnf_t));
      macCbConfig.mlmeDisassociateCnfCb(&disassociateCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_GET_CNF:
    {
      MAC_getCnf_t  getCnf;
      memcpy(&getCnf,p_mac_evt->notPayload, sizeof(MAC_getCnf_t));
      macCbConfig.mlmeGetCnfCb(&getCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_GTS_CNF:
    {
      MAC_gtsCnf_t gtsCnf;
      memcpy(&gtsCnf,p_mac_evt->notPayload, sizeof(MAC_gtsCnf_t));
      macCbConfig.mlmeGtsCnfCb(&gtsCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_RX_ENABLE_CNF:
    {
      MAC_rxEnableCnf_t RxEnable;
      memcpy(&RxEnable,p_mac_evt->notPayload, sizeof(MAC_rxEnableCnf_t));
      macCbConfig.mlmeRxEnableCnfCb(&RxEnable);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_SCAN_CNF:
    {
      MAC_scanCnf_t scanCnf;
      memcpy(&scanCnf,p_mac_evt->notPayload, sizeof(MAC_scanCnf_t));
      macCbConfig.mlmeScanCnfCb(&scanCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_SET_CNF:
    {
      MAC_setCnf_t setCnf;
      memcpy(&setCnf,p_mac_evt->notPayload, sizeof(MAC_setCnf_t));
      macCbConfig.mlmeSetCnfCb(&setCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_START_CNF:
    {
      MAC_startCnf_t startCnf;
      memcpy(&startCnf,p_mac_evt->notPayload, sizeof(MAC_startCnf_t));
      macCbConfig.mlmeStartCnfCb(&startCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_POLL_CNF:
    {
      MAC_pollCnf_t pollCnf;
      memcpy(&pollCnf,p_mac_evt->notPayload, sizeof(MAC_pollCnf_t));
      macCbConfig.mlmePollCnfCb(&pollCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_DPS_CNF:
    {
      MAC_dpsCnf_t dpsCnf;
      memcpy(&dpsCnf,p_mac_evt->notPayload, sizeof(MAC_dpsCnf_t));
      macCbConfig.mlmeDpsCnfCb(&dpsCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_SOUNDING_CNF:
    {
      MAC_soundingCnf_t soudingCnf;
      memcpy(&soudingCnf,p_mac_evt->notPayload, sizeof(MAC_soundingCnf_t));
      macCbConfig.mlmeSoundingCnfCb(&soudingCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_CALIBRATE_CNF:
    {
      MAC_calibrateCnf_t calibrateCnf;
      memcpy(&calibrateCnf,p_mac_evt->notPayload, sizeof(MAC_calibrateCnf_t));
      macCbConfig.mlmeCalibrateCnfCb(&calibrateCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MCPS_DATA_CNF:
    {
      MAC_dataCnf_t dataCnf;
      memcpy(&dataCnf,p_mac_evt->notPayload, sizeof(MAC_dataCnf_t));
      macCbConfig.mcpsDataCnfCb(&dataCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MCPS_PURGE_CNF:
    {
      MAC_purgeCnf_t purgeCnf;
      memcpy(&purgeCnf,p_mac_evt->notPayload, sizeof(MAC_purgeCnf_t));
      macCbConfig.mcpsPurgeCnfCb(&purgeCnf);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_ASSOCIATE_IND:
    {
      MAC_associateInd_t associateInd;
      memcpy(&associateInd,p_mac_evt->notPayload, sizeof(MAC_associateInd_t));
      macCbConfig.mlmeAssociateIndCb(&associateInd);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_DISASSOCIATE_IND:
    {
      MAC_disassociateInd_t disassociateInd;
      memcpy(&disassociateInd,p_mac_evt->notPayload, sizeof(MAC_disassociateInd_t));
      macCbConfig.mlmeDisassociateIndCb(&disassociateInd);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_BEACON_NOTIFY_IND:
    {
      MAC_beaconNotifyInd_t beaconNotifyInd;
      memcpy(&beaconNotifyInd,p_mac_evt->notPayload, sizeof(MAC_beaconNotifyInd_t));
      macCbConfig.mlmeBeaconNotifyIndCb(&beaconNotifyInd);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_COMM_STATUS_IND:
    {
      MAC_commStatusInd_t commStatusInd;
      memcpy(&commStatusInd,p_mac_evt->notPayload, sizeof(MAC_commStatusInd_t));
      macCbConfig.mlmeCommStatusIndCb(&commStatusInd);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_GTS_IND:
    {
      MAC_GtsInd_t gtsInd;
      memcpy(&gtsInd,p_mac_evt->notPayload, sizeof(MAC_GtsInd_t));
      macCbConfig.mlmeGtsIndCb(&gtsInd);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_ORPHAN_IND:
    {
      MAC_orphanInd_t orphanInd;
      memcpy(&orphanInd,p_mac_evt->notPayload, sizeof(MAC_orphanInd_t));
      macCbConfig.mlmeOrphanIndCb(&orphanInd);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_SYNC_LOSS_IND:
    {
      MAC_syncLoss_t syncLoss;
      memcpy(&syncLoss,p_mac_evt->notPayload, sizeof(MAC_syncLoss_t));
      macCbConfig.mlmeSyncLossIndCb(&syncLoss);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_DPS_IND:
    {
      MAC_dpsInd_t dpsInd;
      memcpy(&dpsInd,p_mac_evt->notPayload, sizeof(MAC_dpsInd_t));
      macCbConfig.mlmeDpsIndCb(&dpsInd);
    }
    break;
    case MSG_M0TOM4_MAC_MCPS_DATA_IND:
    {
      MAC_dataInd_t dataInd ;
      memcpy(&dataInd,p_mac_evt->notPayload, sizeof(MAC_dataInd_t));
      macCbConfig.mcpsDataIndCb(&dataInd);
    }
    break;
    case MSG_M0TOM4_MAC_MLME_POLL_IND:
    {
      MAC_pollInd_t pollInd ;
      memcpy(&pollInd,p_mac_evt->notPayload, sizeof(MAC_pollInd_t));
      macCbConfig.mlmePollIndCb(&pollInd);
    }
    break;
  default:
    status = HAL_ERROR;
    break;
  }


  /* Send Ack to M0 */
  TL_MAC_802_15_4_SendAck();
  return status;

}

/**
  * @}
  */


/** \addtogroup STM32WB55_MAC
  * @{
  */

/**
 * @brief  This function is used to handle a MAC MLME Associate REQ as described
 *         in IEEE Std 802.15.4-2011 standard.
 *
 *
 * @param  MAC_associateReq_t ref
 * @retval MAC_Status_t
 */

MAC_Status_t MAC_MLMEAssociateReq( const  MAC_associateReq_t * pAssociateReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_ASSOCIATE_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_associateReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pAssociateReq, sizeof(MAC_associateReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Disassociate REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_disassociateReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEDisassociateReq( const  MAC_disassociateReq_t * pDisassiociateReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_DISASSOCIATE_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_disassociateReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pDisassiociateReq, sizeof(MAC_disassociateReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Get REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_getReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEGetReq( const  MAC_getReq_t * pGetReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_GET_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_getReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pGetReq, sizeof(MAC_getReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME GTS REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_gtsReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEGtsReq( const  MAC_gtsReq_t * pGtsReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_GTS_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_gtsReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pGtsReq, sizeof(MAC_gtsReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Reset REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_resetReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEResetReq( const  MAC_resetReq_t * pResetReq )
{
    MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;


    Mac_802_15_4_PreCmdProcessing();

    /* prepare buffer */
    TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

    utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_RESET_REQ);

    /* plen = pitAttributeValueLen + 1 byte (corresponding to pibAttributeID) */
    p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_resetReq_t);

    /* Here copy to payload */
    memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pResetReq, sizeof(MAC_resetReq_t) );

    Mac_802_15_4_CmdTransfer();

    /* If return parameter */
    TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

    status = p_mac_rsp_evt->payload[0];

    return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Rx Enabled REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_rxEnableReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMERxEnableReq( const  MAC_rxEnableReq_t * pRxEnableReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_RX_ENABLE_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_rxEnableReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pRxEnableReq, sizeof(MAC_rxEnableReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}


/**
* @brief  This function is used to handle a MAC MLME Scan REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_scanReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEScanReq( const  MAC_scanReq_t * pScanReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_SCAN_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_scanReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pScanReq, sizeof(MAC_scanReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
* @brief  This function is used to handle a MAC MLME Set REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_setReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMESetReq( const  MAC_setReq_t * pSetReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_SET_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_setReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pSetReq, sizeof(MAC_setReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
* @brief  This function is used to handle a MAC MLME Start REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_startReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEStartReq( const  MAC_startReq_t * pStartReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_START_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_startReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pStartReq, sizeof(MAC_startReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
* @brief  This function is used to handle a MAC MLME Sync REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_syncReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMESyncReq( const  MAC_syncReq_t * pSyncReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_SYNC_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_syncReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pSyncReq, sizeof(MAC_syncReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}


/**
* @brief  This function is used to handle a MAC MLME Poll REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 *
 * @param  MAC_pollReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEPollReq( const MAC_pollReq_t * pPollReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_POLL_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_pollReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pPollReq, sizeof(MAC_pollReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Dps REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_dpsReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEDpsReq( const MAC_dpsReq_t * pDpsReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_DPS_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_dpsReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pDpsReq, sizeof(MAC_dpsReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Associate REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_soundingReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMESoundingReq( const MAC_soundingReq_t * pSoundingReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_SOUNDING_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_soundingReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pSoundingReq, sizeof(MAC_soundingReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Calibrate REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_calibrateReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMECalibrateReq( const MAC_calibrateReq_t * pCalibrateReq)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_CALIBRATE_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_calibrateReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pCalibrateReq, sizeof(MAC_calibrateReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MCPS Data REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_dataReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MCPSDataReq( const MAC_dataReq_t * pDataReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MCPS_DATA_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_dataReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pDataReq, sizeof(MAC_dataReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MCPD Purge REQ as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_purgeReq_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MCPSPurgeReq( const MAC_purgeReq_t * pPurgeReq )
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MCPS_PURGE_REQ);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_purgeReq_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pPurgeReq, sizeof(MAC_purgeReq_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}


/**
 * @brief  This function is used to handle a MAC MLME Associate RES as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_associateRes_t ref
 * @retval MAC_Status_t
 */

MAC_Status_t MAC_MLMEAssociateRes(const MAC_associateRes_t * pAssociateRes)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_ASSOCIATE_RES);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_associateRes_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pAssociateRes, sizeof(MAC_associateRes_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

/**
 * @brief  This function is used to handle a MAC MLME Orphan RES as described
 *         in IEEE Std 802.15.4-2011 standard
 *
 * @param  MAC_orphanRes_t ref
 * @retval MAC_Status_t
 */
MAC_Status_t MAC_MLMEOrphanRes(const MAC_orphanRes_t * pOrphanRes)
{
  MAC_Status_t status = MAC_NOT_IMPLEMENTED_STATUS;

  Mac_802_15_4_PreCmdProcessing();

  /* prepare buffer */
  TL_CmdPacket_t* p_mac_cmd_req = MAC_802_15_4_GetCmdBuffer();

  utils_mac_set_cmdCode(MSG_M4TOM0_MAC_MLME_ORPHAN_RES);

  p_mac_cmd_req->cmdserial.cmd.plen = sizeof(MAC_orphanRes_t);

  /* Here copy to payload */
  memcpy( p_mac_cmd_req->cmdserial.cmd.payload, pOrphanRes, sizeof(MAC_orphanRes_t) );

  Mac_802_15_4_CmdTransfer();

  /* If return parameter */
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();

  status = p_mac_rsp_evt->payload[0];

  return status;
}

