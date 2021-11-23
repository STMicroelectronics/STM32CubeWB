/**
 ******************************************************************************
 * @file    tl_thread_hci.c
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
#include "stm_list.h"
#include "tl.h"
#include "tl_thread_hci.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/



/* Private macros ------------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ----------------------------------------------------------*/
__WEAK void Pre_OtCmdProcessing(void){return;}
__WEAK void Ot_Cmd_Transfer(void){return;}
__WEAK Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdPayloadBuffer(void){return 0;}
__WEAK Thread_OT_Cmd_Request_t* THREAD_Get_OTCmdRspPayloadBuffer(void){return 0;}
__WEAK Thread_OT_Cmd_Request_t* THREAD_Get_NotificationPayloadBuffer(void){return 0;}


