/**
 ******************************************************************************
 * @file    tl_thread_hci.h
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

#ifndef __TL_THREAD_HCI_H_
#define __TL_THREAD_HCI_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32wbxx_core_interface_def.h"

/* Exported functions  ------------------------------------------------------------*/
void Pre_OtCmdProcessing(void);
void Ot_Cmd_Transfer(void);
void Ot_Cmd_TransferWithNotif(void);
Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdPayloadBuffer(void);
Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdRspPayloadBuffer(void);
Thread_OT_Cmd_Request_t* THREAD_Get_NotificationPayloadBuffer(void);
#ifdef OPENTHREAD_RCP 
RCP_Packet_t* THREAD_Get_RCPPayloadBuffer(void);
#endif
/* Exported defines -----------------------------------------------------------*/


#endif /* __TL_THREAD_HCI_H_ */
