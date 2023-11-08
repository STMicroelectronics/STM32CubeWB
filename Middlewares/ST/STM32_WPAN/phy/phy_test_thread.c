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
#include "stm32wbxx_core_interface_def.h"
#include "tl_thread_hci.h"
#include "phy_test.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

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
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();
  
  p_ot_req->ID = MSG_M4TOM0_PHY_TEST_MODE;
  
  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)enableDisable;
  
  Ot_Cmd_Transfer();
  
  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

/**
 * @brief  PHY test set channel
 *
 * @param  channel_nb: channel to set (11 to 26)
 * @retval 0 if successful, 2 if bad argument, 0x92 if radio is not initialized
 */
uint8_t phyTestSetChannel(uint8_t channel_nb)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();
  
  p_ot_req->ID = MSG_M4TOM0_PHY_SET_CHANNEL;
  
  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)channel_nb;
  
  Ot_Cmd_Transfer();
  
  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

/**
 * @brief  PHY test continuous wave start
 *
 * @param  frq_mhz: the frequency of the continuous wave (2400 <= frq_mhz <= 2482)
 * @retval 0 if successful, 2 if bad argument
 */
uint8_t phyTestContinuousWaveStart(uint16_t frq_mhz)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();
  
  p_ot_req->ID = MSG_M4TOM0_PHY_CONTINUOUS_WAVE_START;
  
  p_ot_req->Size=1;
  p_ot_req->Data[0] = (uint32_t)frq_mhz;
  
  Ot_Cmd_Transfer();
  
  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

/**
 * @brief  PHY test continuous wave stop
 *
 * @param  None
 * @retval 0 if successful, 0xFF otherwise
 */
uint8_t phyTestContinuousWaveStop(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();
  
  p_ot_req->ID = MSG_M4TOM0_PHY_CONTINUOUS_WAVE_STOP;
  
  p_ot_req->Size=0;
  
  Ot_Cmd_Transfer();
  
  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

/**
 * @brief  PHY test Tx start
 *
 * @param  nb_frames: number of frames to send
 * @param  size_of_frame: number of bytes in the frame (maximum 20 bytes)
 * @param  tx_frame: the frame to transmit
 * @retval 0 if successful, 0xFF otherwise
 */
uint8_t phyTestTxStart(uint32_t nb_frames, uint8_t size_of_frame, uint8_t *tx_frame)
{   
  if( size_of_frame > (OT_CMD_BUFFER_SIZE-1) )
  {
    return 0xFF;
  }
  else
  {
    Pre_OtCmdProcessing();
    /* prepare buffer */
    Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();
    
    p_ot_req->ID = MSG_M4TOM0_PHY_TX_START;
    
    p_ot_req->Size=1;
    p_ot_req->Data[0] = nb_frames;
    
    while( p_ot_req->Size != (size_of_frame+1) )
    {
      p_ot_req->Data[p_ot_req->Size] = (uint32_t)tx_frame[p_ot_req->Size-1];
      p_ot_req->Size++;
    }
    
    Ot_Cmd_Transfer();
    
    p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
    return (uint8_t)p_ot_req->Data[0];
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
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();
  
  p_ot_req->ID = MSG_M4TOM0_PHY_RX_START;
  
  p_ot_req->Size=0;
  
  Ot_Cmd_Transfer();
  
  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint8_t)p_ot_req->Data[0];
}

/**
 * @brief  PHY test Rx stop
 *
 * @param  None
 * @retval the number of packets received
 */
uint32_t phyTestRxStop(void)
{
  Pre_OtCmdProcessing();
  /* prepare buffer */
  Thread_OT_Cmd_Request_t* p_ot_req = THREAD_Get_OTCmdPayloadBuffer();
  
  p_ot_req->ID = MSG_M4TOM0_PHY_RX_STOP;
  
  p_ot_req->Size=0;
  
  Ot_Cmd_Transfer();
  
  p_ot_req = THREAD_Get_OTCmdRspPayloadBuffer();
  return (uint32_t)p_ot_req->Data[0];
}