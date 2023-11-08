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
 * Copyright (c) 2018-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
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

/* MAC command cmdcode range 0x280 .. 0x3DF = 352 */
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

/** @defgroup STM32WB55_MAC_CORE STM32WBxx Proxy MAC 802.15.4 Core
  * @brief           Proxy MAC module allows application to exchange in both ways MAC messages
  *                  between Application and RF Cores.
  *                  On application or network upper layer SAP-MAC primitive call, MAC request is
  *                  interpreted by the Proxy MAC layer and sent to the RF-Core.<br>
  *                  On RF-Core MAC notification, MAC messages are parsed and sent back to the upper
  *                  user implemented layer using user defined callback.
  *
  * @{
  */
/* List of messages sent by the M4 to the M0 */
typedef enum
{
  /*! MAC Message ID to request a MAC association */
  MSG_M4TOM0_MAC_MLME_ASSOCIATE_REQ=0x00,
  /*! MAC Message ID to respond to a previously received MAC Association Indication */
  MSG_M4TOM0_MAC_MLME_ASSOCIATE_RES,
  /*! MAC Message ID to request a MAC Disassociation */
  MSG_M4TOM0_MAC_MLME_DISASSOCIATE_REQ,
  /*! MAC Message ID to retrieve PIB Attribute value */
  MSG_M4TOM0_MAC_MLME_GET_REQ,
  /*! MAC Message ID to perform a GTS request*/
  MSG_M4TOM0_MAC_MLME_GTS_REQ,
  /*! MAC Message ID to respond to a previously received MAC Orphan Indication */
  MSG_M4TOM0_MAC_MLME_ORPHAN_RES,
  /*! MAC Message ID to perform a Reset request*/
  MSG_M4TOM0_MAC_MLME_RESET_REQ,
  /*! MAC Message ID to perform a RX Enable request*/
  MSG_M4TOM0_MAC_MLME_RX_ENABLE_REQ,
  /*! MAC Message ID to perform a RX Scan request*/
  MSG_M4TOM0_MAC_MLME_SCAN_REQ,
  /*! MAC Message ID to setup a MAC/PHY attribute*/
  MSG_M4TOM0_MAC_MLME_SET_REQ,
  /*! MAC Message ID to request a MAC Start*/
  MSG_M4TOM0_MAC_MLME_START_REQ,
  /*! MAC Message ID to request a MAC Synchronization*/
  MSG_M4TOM0_MAC_MLME_SYNC_REQ,
  /*! MAC Message ID to request a MAC Polling*/
  MSG_M4TOM0_MAC_MLME_POLL_REQ,
  /*! MAC Message ID to request a Dynamic Preamble Selection*/
  MSG_M4TOM0_MAC_MLME_DPS_REQ,
  /*! MAC Message ID to request a PHY respond with channel sounding information*/
  MSG_M4TOM0_MAC_MLME_SOUNDING_REQ,
  /*! MAC Message ID to request a PHY respond with RMARKER offset*/
  MSG_M4TOM0_MAC_MLME_CALIBRATE_REQ,
  /*! MAC Message ID to request a MAC data transfer to other device*/
  MSG_M4TOM0_MAC_MCPS_DATA_REQ,
  /*! MAC Message ID to request a purge of the pending MSDU from Transaction Queue*/
  MSG_M4TOM0_MAC_MCPS_PURGE_REQ,
  /*! PHY Message ID to request test mode enablement/disblement */
  MSG_M4TOM0_PHY_TEST_MODE,
  /*! PHY Message ID to request a change of channel */
  MSG_M4TOM0_PHY_SET_CHANNEL,
  /*! PHY Message ID to request a continuous wave start */
  MSG_M4TOM0_PHY_CONTINUOUS_WAVE_START,
  /*! PHY Message ID to request a continuous wave stop */
  MSG_M4TOM0_PHY_CONTINUOUS_WAVE_STOP,
  /*! PHY Message ID to request a Tx start */
  MSG_M4TOM0_PHY_TX_START,
  /*! PHY Message ID to request a Rx start */
  MSG_M4TOM0_PHY_RX_START,
  /*! PHY Message ID to request a Rx stop */
  MSG_M4TOM0_PHY_RX_STOP,
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

/**
  * @}
  */

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
 * @brief   This system function is used to set defined command ID in message between
 *          Application core and Rf core.
 *
 *
 * @param  None
 * @retval None
 */

void utils_mac_set_cmdCode(uint16_t cmdCode);

/** @addtogroup STM32WB55_MAC_CORE
 *
 */

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

/**
  * @}
  */


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _INC_802_15_4_MAC_CORE_H_ */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
