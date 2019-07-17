/**
  ******************************************************************************
  * @file    802_15_4_mac_core.h
  * @author  MCD Application Team
  * @brief   This file contains all the defines and structures used for the
  *          communication between the two core M0 and M4.
  *          This file is shared between the code running on M4 and the code
  *          running on M0.
  *
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

#ifndef _INC_802_15_4_MAC_CORE_H_
#define _INC_802_15_4_MAC_CORE_H_

#include <stdint.h>

#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_def.h"
#include "stm32wbxx_hal_cortex.h"
#include "stm32_wpan_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OpenThread OT command cmdcode range 0x280 .. 0x3DF = 352
 *  --> will be reattributed to MAC 802.15.4 */
#define MAC_802_15_4_CMD_OPCODE_OFFSET 0x280

#define MASK_CMD_CODE_OCF 0x3FF

/* Structure of the messages exchanged between M0 and M4 */
#define OT_CMD_BUFFER_SIZE 20U
typedef struct
{
  uint32_t  ID;
  uint32_t  Size;
  uint32_t  Data[OT_CMD_BUFFER_SIZE];
}MAC_802_15_4_CmdReq_OT_LIKE_t;

typedef PACKED_STRUCT
{
  uint16_t  subEvtCode;
  uint8_t   notPayload[1];
}MAC_802_15_4_Notification_t;

/* List of messages sent by the M4 to the M0 */
typedef enum
{
  MSG_M4TOM0_MAC_MLME_ASSOCIATE_REQ=0x00,
  MSG_M4TOM0_MAC_MLME_ASSOCIATE_RES,
  MSG_M4TOM0_MAC_MLME_DISASSOCIATE_REQ,
  MSG_M4TOM0_MAC_MLME_GET_REQ,
  MSG_M4TOM0_MAC_MLME_GTS_REQ,
  MSG_M4TOM0_MAC_MLME_ORPHAN_RES,
  MSG_M4TOM0_MAC_MLME_RESET_REQ,
  MSG_M4TOM0_MAC_MLME_RX_ENABLE_REQ,
  MSG_M4TOM0_MAC_MLME_SCAN_REQ,
  MSG_M4TOM0_MAC_MLME_SET_REQ,
  MSG_M4TOM0_MAC_MLME_START_REQ,
  MSG_M4TOM0_MAC_MLME_SYNC_REQ,
  MSG_M4TOM0_MAC_MLME_POLL_REQ,
  MSG_M4TOM0_MAC_MLME_DPS_REQ,
  MSG_M4TOM0_MAC_MLME_SOUNDING_REQ,
  MSG_M4TOM0_MAC_MLME_CALIBRATE_REQ,
  MSG_M4TOM0_MAC_MCPS_DATA_REQ,
  MSG_M4TOM0_MAC_MCPS_PURGE_REQ,
} MAC_802_15_4_MsgIdM4ToM0_t;

/* List of messages sent by the M0 to the M4 */
typedef enum
{
  MSG_M0TOM4_MAC_MLME_ASSOCIATE_CNF=0x00,
  MSG_M0TOM4_MAC_MLME_DISASSOCIATE_CNF,
  MSG_M0TOM4_MAC_MLME_GET_CNF,
  MSG_M0TOM4_MAC_MLME_GTS_CNF,
  MSG_M0TOM4_MAC_MLME_RESET_CNF,
  MSG_M0TOM4_MAC_MLME_RX_ENABLE_CNF,
  MSG_M0TOM4_MAC_MLME_SCAN_CNF,
  MSG_M0TOM4_MAC_MLME_SET_CNF,
  MSG_M0TOM4_MAC_MLME_START_CNF,
  MSG_M0TOM4_MAC_MLME_POLL_CNF,
  MSG_M0TOM4_MAC_MLME_DPS_CNF,
  MSG_M0TOM4_MAC_MLME_SOUNDING_CNF,
  MSG_M0TOM4_MAC_MLME_CALIBRATE_CNF,
  MSG_M0TOM4_MAC_MCPS_DATA_CNF,
  MSG_M0TOM4_MAC_MCPS_PURGE_CNF,
  MSG_M0TOM4_MAC_MLME_ASSOCIATE_IND,
  MSG_M0TOM4_MAC_MLME_DISASSOCIATE_IND,
  MSG_M0TOM4_MAC_MLME_BEACON_NOTIFY_IND,
  MSG_M0TOM4_MAC_MLME_COMM_STATUS_IND,
  MSG_M0TOM4_MAC_MLME_GTS_IND,
  MSG_M0TOM4_MAC_MLME_ORPHAN_IND,
  MSG_M0TOM4_MAC_MLME_SYNC_LOSS_IND,
  MSG_M0TOM4_MAC_MLME_DPS_IND,
  MSG_M0TOM4_MAC_MCPS_DATA_IND,
  MSG_M0TOM4_MAC_MLME_POLL_IND
} MAC_802_15_4_MsgIdM0ToM4_t;

/* List of modes available for UART configuration */
typedef enum
{
  SYS_LPUART1_CLI,
  SYS_USART1_CLI,
} Sys_ConfigUart_Enum_t;

/* Gravity error level */
typedef enum
{
  ERR_INTERFACE_FATAL= 1U,
  ERR_INTERFACE_WARNING = 2U
} Error_Interface_Level_Enum_t;

/**
  * @brief  This function is used to manage all the callbacks used by the
  *         MAC 802.15.4 interface.
  *
  *         Important Note: This function must be called each time a message
  *         is sent from the M0 to the M4.
  *
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef MAC_802_15_4_CallBack_Processing(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _INC_802_15_4_MAC_CORE_H_ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
