/**
 ******************************************************************************
  * File Name          : tl_dbg_conf.h
  * Description        : Debug configuration file for stm32wpan transport layer interface.
  *
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TL_DBG_CONF_H
#define __TL_DBG_CONF_H

/* USER CODE BEGIN Tl_Conf */

/* Includes ------------------------------------------------------------------*/
#include "dbg_trace.h"

/**
 * Enable or Disable traces
 */
#define TL_SHCI_CMD_DBG_EN    0   /* Reports System commands sent to CPU2 and the command response */
#define TL_SHCI_EVT_DBG_EN    0   /* Reports System Asynchronous Events received from CPU2 */
#define TL_HCI_CMD_DBG_EN     0   /* Reports BLE command sent to CPU2 and the command response */
#define TL_HCI_EVT_DBG_EN     0   /* Reports BLE Asynchronous Events received from CPU2 */


/**
 * Macro definition
 */

/**
 * System Transport Layer
 */
#if (TL_SHCI_CMD_DBG_EN != 0)
#define TL_SHCI_CMD_DBG_MSG             PRINT_MESG_DBG
#define TL_SHCI_CMD_DBG_BUF             PRINT_LOG_BUFF_DBG
#else
#define TL_SHCI_CMD_DBG_MSG(...)
#define TL_SHCI_CMD_DBG_BUF(...)
#endif

#if (TL_SHCI_EVT_DBG_EN != 0)
#define TL_SHCI_EVT_DBG_MSG             PRINT_MESG_DBG
#define TL_SHCI_EVT_DBG_BUF             PRINT_LOG_BUFF_DBG
#else
#define TL_SHCI_EVT_DBG_MSG(...)
#define TL_SHCI_EVT_DBG_BUF(...)
#endif

/**
 * BLE Transport Layer
 */
#if (TL_HCI_CMD_DBG_EN != 0)
#define TL_HCI_CMD_DBG_MSG             PRINT_MESG_DBG
#define TL_HCI_CMD_DBG_BUF             PRINT_LOG_BUFF_DBG
#else
#define TL_HCI_CMD_DBG_MSG(...)
#define TL_HCI_CMD_DBG_BUF(...)
#endif

#if (TL_HCI_EVT_DBG_EN != 0)
#define TL_HCI_EVT_DBG_MSG             PRINT_MESG_DBG
#define TL_HCI_EVT_DBG_BUF             PRINT_LOG_BUFF_DBG
#else
#define TL_HCI_EVT_DBG_MSG(...)
#define TL_HCI_EVT_DBG_BUF(...)
#endif

/* USER CODE END Tl_Conf */

#endif /*__TL_DBG_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
