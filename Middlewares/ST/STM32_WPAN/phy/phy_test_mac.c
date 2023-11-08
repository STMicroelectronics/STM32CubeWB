/**
  ******************************************************************************
  * @file    phy_test.c
  * @author  MCD Application Team
  * @brief   This file contains the PHY interface shared between
  *          M0 and M4.
  ******************************************************************************
  * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_hal.h"
#include "802_15_4_mac_core.h"
#include "tl_mac_802_15_4.h"
#include "802_15_4_mac_core.h"
#include "phy_test.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define MAX_CMD_BUFFER_SIZE 20U

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  PHY test enable or disable test mode
 *
 * @param  enableDisable: 1 to enable PHY test mode, 0 to disable PHY test mode
 * @retval 0 if successful, 0xFF otherwise
 */
uint8_t phyTestMode(uint8_t enableDisable)
{
  Mac_802_15_4_PreCmdProcessing();
  /* prepare buffer */
  TL_CmdPacket_t* p_mac_req = MAC_802_15_4_GetCmdBuffer();
  
  utils_mac_set_cmdCode(MSG_M4TOM0_PHY_TEST_MODE);
  
  p_mac_req->cmdserial.cmd.plen=1;
  p_mac_req->cmdserial.cmd.payload[0] = (uint32_t)enableDisable;
  
  Mac_802_15_4_CmdTransfer();
  
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();
  return (uint8_t)p_mac_rsp_evt->payload[0];
}

/**
 * @brief  PHY test set channel
 *
 * @param  channel_nb: channel to set (11 to 26)
 * @retval 0 if successful, 2 if bad argument, 0x92 if radio is not initialized
 */
uint8_t phyTestSetChannel(uint8_t channel_nb)
{
  Mac_802_15_4_PreCmdProcessing();
  /* prepare buffer */
  TL_CmdPacket_t* p_mac_req = MAC_802_15_4_GetCmdBuffer();
  
  utils_mac_set_cmdCode(MSG_M4TOM0_PHY_SET_CHANNEL);
  
  p_mac_req->cmdserial.cmd.plen=1;
  p_mac_req->cmdserial.cmd.payload[0] = (uint32_t)channel_nb;
  
  Mac_802_15_4_CmdTransfer();
  
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();
  return (uint8_t)p_mac_rsp_evt->payload[0];
}

/**
 * @brief  PHY test continuous wave start
 *
 * @param  frq_mhz: the frequency of the continuous wave
 * @retval 0 if successful, 2 if bad argument
 */
uint8_t phyTestContinuousWaveStart(uint16_t frq_mhz)
{
  Mac_802_15_4_PreCmdProcessing();
  /* prepare buffer */
  TL_CmdPacket_t* p_mac_req = MAC_802_15_4_GetCmdBuffer();
  
  utils_mac_set_cmdCode(MSG_M4TOM0_PHY_CONTINUOUS_WAVE_START);
  
  p_mac_req->cmdserial.cmd.plen=2;
  p_mac_req->cmdserial.cmd.payload[0] = (uint8_t)frq_mhz;
  p_mac_req->cmdserial.cmd.payload[1] = (uint8_t)(frq_mhz >> 8);
  
  Mac_802_15_4_CmdTransfer();
  
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();
  return (uint8_t)p_mac_rsp_evt->payload[0];
}

/**
 * @brief  PHY test continuous wave stop
 *
 * @param  None
 * @retval 0 if successful, 0xFF otherwise
 */
uint8_t phyTestContinuousWaveStop(void)
{
  Mac_802_15_4_PreCmdProcessing();
  /* prepare buffer */
  TL_CmdPacket_t* p_mac_req = MAC_802_15_4_GetCmdBuffer();
  
  utils_mac_set_cmdCode(MSG_M4TOM0_PHY_CONTINUOUS_WAVE_STOP);
  
  p_mac_req->cmdserial.cmd.plen=0;
  
  Mac_802_15_4_CmdTransfer();
  
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();
  return (uint8_t)p_mac_rsp_evt->payload[0];
}

/**
 * @brief  PHY test Tx start
 *
 * @param  nb_frames: number of frames to send
 * @param  size_of_frame: number of bytes in the frame
 * @param  tx_frame: the frame to transmit
 * @retval 0 if successful, 0xFF otherwise
 */
uint8_t phyTestTxStart(uint32_t nb_frames, uint8_t size_of_frame, uint8_t *tx_frame)
{   
  if( size_of_frame > (MAX_CMD_BUFFER_SIZE-1) )
  {
    return 0xFF;
  }
  else
  {
    Mac_802_15_4_PreCmdProcessing();
    /* prepare buffer */
    TL_CmdPacket_t* p_mac_req = MAC_802_15_4_GetCmdBuffer();
    
    utils_mac_set_cmdCode(MSG_M4TOM0_PHY_TX_START);
    
    p_mac_req->cmdserial.cmd.plen=1;
    p_mac_req->cmdserial.cmd.payload[0] = nb_frames;
    
    while( p_mac_req->cmdserial.cmd.plen != (size_of_frame+1) )
    {
      p_mac_req->cmdserial.cmd.payload[p_mac_req->cmdserial.cmd.plen] = tx_frame[p_mac_req->cmdserial.cmd.plen-1];
      p_mac_req->cmdserial.cmd.plen++;
    }
    
    Mac_802_15_4_CmdTransfer();
    
    TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();
    return (uint8_t)p_mac_rsp_evt->payload[0];
  }
}

/**
 * @brief  PHY test Rx start
 *
 * @param  None
 * @retval 0 if successful, 0xFF otherwise
 */
uint8_t phyTestRxStart(void)
{
  Mac_802_15_4_PreCmdProcessing();
  /* prepare buffer */
  TL_CmdPacket_t* p_mac_req = MAC_802_15_4_GetCmdBuffer();
  
  utils_mac_set_cmdCode(MSG_M4TOM0_PHY_RX_START);
  
  p_mac_req->cmdserial.cmd.plen=0;
  
  Mac_802_15_4_CmdTransfer();
  
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();
  return (uint8_t)p_mac_rsp_evt->payload[0];
}

/**
 * @brief  PHY test Rx stop
 *
 * @param  None
 * @retval the number of packets received
 */
uint32_t phyTestRxStop(void)
{
  Mac_802_15_4_PreCmdProcessing();
  /* prepare buffer */
  TL_CmdPacket_t* p_mac_req = MAC_802_15_4_GetCmdBuffer();
  
  utils_mac_set_cmdCode(MSG_M4TOM0_PHY_RX_STOP);
  
  p_mac_req->cmdserial.cmd.plen=0;
  
  Mac_802_15_4_CmdTransfer();
  
  TL_Evt_t* p_mac_rsp_evt = MAC_802_15_4_GetRspPayEvt();
  return (uint32_t)((uint32_t)p_mac_rsp_evt->payload[0] + (uint32_t)(p_mac_rsp_evt->payload[1] << 8));
}