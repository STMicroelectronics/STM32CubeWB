/**
 ******************************************************************************
 * @file    tl_zigbee_hci.h
 * @author  MCD Application Team
 * @brief   Constants and functions for managing Thread TL
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

#ifndef __TL_ZIGBEE_HCI_H_
#define __TL_ZIGBEE_HCI_H_

/* Includes ------------------------------------------------------------------*/
#include "zigbee_core.h"
#include "stm32wbxx_core_interface_def.h"
#include "tl.h"
#include <string.h>


/* Exported functions  ------------------------------------------------------------*/
void Pre_ZigbeeCmdProcessing(void);
void ZIGBEE_CmdTransfer(void);
Zigbee_Cmd_Request_t* ZIGBEE_Get_OTCmdPayloadBuffer(void);
Zigbee_Cmd_Request_t* ZIGBEE_Get_OTCmdRspPayloadBuffer(void);
Zigbee_Cmd_Request_t* ZIGBEE_Get_NotificationPayloadBuffer(void);
Zigbee_Cmd_Request_t* ZIGBEE_Get_M0RequestPayloadBuffer(void);

/* Exported defines -----------------------------------------------------------*/


#endif /* __TL_ZIGBEE_HCI_H_*/
