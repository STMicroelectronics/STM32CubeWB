/**
  ******************************************************************************
  * @file    openthread_api_wb.h
  * @author  MCD Application Team
  * @brief   Contains STM32WB specificities requested to control the OpenThread
  *          interface.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENTHREAD_API_WB
#define OPENTHREAD_API_WB

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "backbone_router_ftd.h"
#include "backbone_router.h"
#include "dns_client.h"
#include "dnssd_server.h"
#include "link.h"
#include "ip6.h"
#include "instance.h"
#include "thread.h"
#include "commissioner.h"
#include "udp.h"
#include "icmp6.h"
#include "joiner.h"
#include "link_raw.h"
#include "link_metrics.h"
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
#include "dataset_updater.h"
#include "srp_client.h"
#include "srp_client_buffers.h"
#include "srp_server.h"
#include "netdata_publisher.h"
#include "netdiag.h"
#include "ping_sender.h"
#include "tcp.h"

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

