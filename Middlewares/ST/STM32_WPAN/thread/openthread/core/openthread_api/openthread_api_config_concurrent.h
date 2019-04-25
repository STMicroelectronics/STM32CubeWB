/**
  ******************************************************************************
  * @file    openthread_api_config_concurrent.h
  * @author  MCD Application Team
  * @brief   This file contains configuration header for configuring OpenThread
  *          stack in BLE Thread Concurrent Mode.
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
#ifndef OPENTHREAD_API_CONFIG_CONCURRENT_H_
#define OPENTHREAD_API_CONFIG_CONCURRENT_H_

#ifndef OPENTHREAD_FTD
#define OPENTHREAD_FTD 1
#endif

/* Define to 1 if you want to enable support for multiple OpenThread
   instances. */
#define OPENTHREAD_ENABLE_MULTIPLE_INSTANCES 0

/* Define to 1 if you want to enable default log output. */
#define OPENTHREAD_CONFIG_LOG_OUTPUT  OPENTHREAD_CONFIG_LOG_OUTPUT_APP

/* Define to 1 to enable COAP. */
#define OPENTHREAD_ENABLE_APPLICATION_COAP 1

/* Define to 1 to enable the commissioner role. */
#define OPENTHREAD_ENABLE_COMMISSIONER 1

/* Define to 1 if you want to use diagnostics module */
#define OPENTHREAD_ENABLE_DIAG 0

/* Define to 1 if you want to enable legacy network. */
#define OPENTHREAD_ENABLE_LEGACY 0

/* Define to 1 to enable dtls support. */
#define OPENTHREAD_ENABLE_DTLS 1

/* Define to 1 to enable the joiner role. */
#define OPENTHREAD_ENABLE_JOINER 1

/* Define to 1 to enable the jam detection. */
#define OPENTHREAD_ENABLE_JAM_DETECTION 0

/* Define to 1 to enable channel manager. */
#define OPENTHREAD_ENABLE_CHANNEL_MANAGER 0

/* Define to 1 to enable channel monitor. */
#define OPENTHREAD_ENABLE_CHANNEL_MONITOR 0

/* Define to 1 to enable child supervision. */
#define OPENTHREAD_ENABLE_CHILD_SUPERVISION 0

/* Define to 1 to enable DHCPv6 Client. */
#define OPENTHREAD_ENABLE_DHCP6_CLIENT 1

/* Define to 1 to enable DHCPv6 SERVER. */
#define OPENTHREAD_ENABLE_DHCP6_SERVER 1

/* Define to 1 to enable MAC whitelist/blacklist feature. */
#define OPENTHREAD_ENABLE_MAC_FILTER 1

/* Define to 1 to enable TMF proxy feature. */
#define OPENTHREAD_ENABLE_TMF_PROXY 0

/* Define to 1 to enable raw link-layer API. */
#define OPENTHREAD_ENABLE_RAW_LINK_API 0

/* Define to 1 to enable Border Router feature. */
#define OPENTHREAD_ENABLE_BORDER_ROUTER 1

/* Define to 1 to enable Service feature. */
#define OPENTHREAD_ENABLE_SERVICE 1

#endif /* OPENTHREAD_API_CONFIG_CONCURRENT_H_ */
