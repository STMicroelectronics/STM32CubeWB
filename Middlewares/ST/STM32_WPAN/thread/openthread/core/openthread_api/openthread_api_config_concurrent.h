/**
  ******************************************************************************
  * @file    openthread_api_config_concurrent.h
  * @author  MCD Application Team
  * @brief   This file contains configuration header for configuring OpenThread
  *          stack in BLE Thread Concurrent Mode.
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
#ifndef OPENTHREAD_API_CONFIG_CONCURRENT_H_
#define OPENTHREAD_API_CONFIG_CONCURRENT_H_

#ifndef OPENTHREAD_FTD
#define OPENTHREAD_FTD 1
#endif

/* Define to 1 if you want to enable support for multiple OpenThread
   instances. */
#define OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE 0

/* Define to 1 if you want to enable default log output. */
#define OPENTHREAD_CONFIG_LOG_OUTPUT  OPENTHREAD_CONFIG_LOG_OUTPUT_APP

/* Define to 1 to enable COAP. */
#define OPENTHREAD_CONFIG_COAP_API_ENABLE 1

/* Define to 1 to enable the commissioner role. */
#define OPENTHREAD_CONFIG_COMMISSIONER_ENABLE 1

/* Define to 1 if you want to use diagnostics module */
#define OPENTHREAD_CONFIG_DIAG_ENABLE 0

/* Define to 1 if you want to enable legacy network. */
#define OPENTHREAD_CONFIG_LEGACY_ENABLE 0

/* Define to 1 to enable dtls support. */
#define OPENTHREAD_CONFIG_DTLS_ENABLE 1

/* Define to 1 to enable the joiner role. */
#define OPENTHREAD_CONFIG_JOINER_ENABLE 1

/* Define to 1 to enable the jam detection. */
#define OPENTHREAD_CONFIG_JAM_DETECTION_ENABLE 0

/* Define to 1 to enable channel manager. */
#define OPENTHREAD_CONFIG_CHANNEL_MANAGER_ENABLE 0

/* Define to 1 to enable channel monitor. */
#define OPENTHREAD_CONFIG_CHANNEL_MONITOR_ENABLE 0

/* Define to 1 to enable child supervision. */
#define OPENTHREAD_CONFIG_CHILD_SUPERVISION_ENABLE 0

/* Define to 1 to enable DHCPv6 Client. */
#define OPENTHREAD_CONFIG_DHCP6_CLIENT_ENABLE 1

/* Define to 1 to enable DHCPv6 SERVER. */
#define OPENTHREAD_CONFIG_DHCP6_SERVER_ENABLE 1

/* Define to 1 to enable MAC whitelist/blacklist feature. */
#define OPENTHREAD_CONFIG_MAC_FILTER_ENABLE 1

/* Define to 1 to enable TMF proxy feature. */
//#define OPENTHREAD_ENABLE_TMF_PROXY 0

/* Define to 1 to enable raw link-layer API. */
#define OPENTHREAD_CONFIG_LINK_RAW_ENABLE 0

/* Define to 1 to enable Border Router feature. */
#define OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE 1

/* Define to 1 to enable Service feature. */
#define OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE 1

/* Define to 1 to enable logging on the platform */
#define OPENTHREAD_CONFIG_LOG_PLATFORM 1

/* Define Log Level */
#define OPENTHREAD_CONFIG_LOG_LEVEL OT_LOG_LEVEL_INFO

/* Define to 1 to enable CoAP Secure API */
#define OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE 1

/* Define to 1 to enable IPv6 global addresses configuration utilities */
#define OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE 1

#endif /* OPENTHREAD_API_CONFIG_CONCURRENT_H_ */
