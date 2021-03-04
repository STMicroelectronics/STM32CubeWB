/**
  ******************************************************************************
  * @file    openthread_api_wb.h
  * @author  MCD Application Team
  * @brief   Contains STM32WB specificities requested to control the OpenThread
  *          interface.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENTHREAD_API_WB
#define OPENTHREAD_API_WB

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "link.h"
#include "ip6.h"
#include "instance.h"
#include "thread.h"
#include "commissioner.h"
#include "dns.h"
#include "udp.h"
#include "icmp6.h"
#include "joiner.h"
#include "link_raw.h"
#include "coap.h"
#include "channel_manager.h"
#include "channel_monitor.h"
#include "child_supervision.h"
#include "jam_detection.h"
#include "server.h"
#include "thread_ftd.h"
#include "network_time.h"
#include "coap_secure.h"
#include "sntp.h"
#include "coap_secure.h"

#include "stm32wbxx_core_interface_def.h"
#include "stm32wbxx_hal.h"
#include "stm32wbxx_hal_def.h"
#include "stm32wbxx_hal_cortex.h"
/* #include "stm32wbxx_core_interface.h" */

#include "dbg_trace.h"


/**
  * @brief  This function is used to manage all the callbacks used by the
  *         OpenThread interface. These callbacks are used for example to
  *         notify the application as soon as the state of a device has been
  *         modified.
  *
  *         Important Note: This function must be called each time a message
  *         is sent from the M0 to the M4.
  *
  * @param  None
  * @retval None
  */

HAL_StatusTypeDef OpenThread_CallBack_Processing(void);

HAL_StatusTypeDef OpenThread_CallBack_Trace_Processing(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OPENTHREAD_API_WB */

