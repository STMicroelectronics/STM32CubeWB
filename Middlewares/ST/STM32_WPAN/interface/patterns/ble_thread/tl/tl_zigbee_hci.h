/**
 ******************************************************************************
 * @file    tl_zigbee_hci.h
 * @author  MCD Application Team
 * @brief   Constants and functions for managing Thread TL
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
