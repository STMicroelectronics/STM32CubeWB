/**
 ******************************************************************************
 * @file    tl_zigbee_hci.c
 * @author  MCD Application Team
 * @brief   Function for managing HCI interface.
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
#include "stm32_wpan_common.h"
#include "hw.h"

#include "stm_list.h"

#include "tl.h"
#include "tl_zigbee_hci.h"


/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/



/* Private macros ------------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ----------------------------------------------------------*/
__weak void Pre_ZigbeeCmdProcessing(void){return;}
__weak void Post_ZigbeeCmdProcessing(void){return;}
__weak void ZIGBEE_CmdTransfer(void){return;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_OTCmdPayloadBuffer(void){return 0;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_OTCmdRspPayloadBuffer(void){return 0;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_NotificationPayloadBuffer(void){return 0;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_M0RequestPayloadBuffer(void){return 0;}
