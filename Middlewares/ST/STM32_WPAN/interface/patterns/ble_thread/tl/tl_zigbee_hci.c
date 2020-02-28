/**
 ******************************************************************************
 * @file    tl_zigbee_hci.c
 * @author  MCD Application Team
 * @brief   Function for managing HCI interface.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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
__weak void ZIGBEE_CmdTransfer(void){return;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_OTCmdPayloadBuffer(void){return 0;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_OTCmdRspPayloadBuffer(void){return 0;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_NotificationPayloadBuffer(void){return 0;}
__weak Zigbee_Cmd_Request_t* ZIGBEE_Get_M0RequestPayloadBuffer(void){return 0;}
