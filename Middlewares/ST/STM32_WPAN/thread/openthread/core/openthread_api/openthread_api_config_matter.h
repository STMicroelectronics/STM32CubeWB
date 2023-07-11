/**
  ******************************************************************************
  * @file    openthread_api_config_mtd.h
  * @author  MCD Application Team
  * @brief   This file contains configuration header for configuring OpenThread
  *          stack in MTD mode.
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
#ifndef OPENTHREAD_API_CONFIG_MATTER_H_
#define OPENTHREAD_API_CONFIG_MATTER_H_


#ifndef OPENTHREAD_MTD
#define OPENTHREAD_MTD 1
#endif

#ifndef OPENTHREAD_CONFIG_MATTER 
#define OPENTHREAD_CONFIG_MATTER 
#endif 
/******************************  MATTER CONFIG  *********************/


// When operating in a less than ideal RF environment, having a more forgiving configuration
// of OpenThread makes thread a great deal more reliable.
#define OPENTHREAD_CONFIG_TMF_ADDRESS_QUERY_MAX_RETRY_DELAY 120    // default is 28800
#define OPENTHREAD_CONFIG_MAC_DEFAULT_MAX_FRAME_RETRIES_DIRECT 15  // default is 3
#define OPENTHREAD_CONFIG_MAC_DEFAULT_MAX_FRAME_RETRIES_INDIRECT 1 // default is 0
#define OPENTHREAD_CONFIG_MAC_MAX_TX_ATTEMPTS_INDIRECT_POLLS 16    // default is 4

// Enable periodic parent search to speed up finding a better parent.
#define OPENTHREAD_CONFIG_PARENT_SEARCH_ENABLE 1              // default is 0
#define OPENTHREAD_CONFIG_PARENT_SEARCH_RSS_THRESHOLD -45          // default is -65
#define OPENTHREAD_CONFIG_MLE_INFORM_PREVIOUS_PARENT_ON_REATTACH 1 // default is 0

// Use smaller maximum interval to speed up reattaching.
#define OPENTHREAD_CONFIG_MLE_ATTACH_BACKOFF_MAXIMUM_INTERVAL (60 * 10 * 1000) // default 1200000 ms


#if CHIP_DEVICE_CONFIG_THREAD_FTD
#define OPENTHREAD_CONFIG_NUM_MESSAGE_BUFFERS 44
#else
#define OPENTHREAD_CONFIG_NUM_MESSAGE_BUFFERS 22
#endif

// Limit CLI buffers
#define OPENTHREAD_CONFIG_CLI_UART_RX_BUFFER_SIZE 512 
#define OPENTHREAD_CONFIG_CLI_UART_TX_BUFFER_SIZE 512

#define OPENTHREAD_CONFIG_NCP_HDLC_ENABLE 1

#define OPENTHREAD_CONFIG_UDP_FORWARD_ENABLE 0

/* Define to 1 if you want to enable support for multiple OpenThread
   instances. */
#define OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE 0

/* Define to 1 if you want to enable default log output. */
#define OPENTHREAD_CONFIG_LOG_OUTPUT  OPENTHREAD_CONFIG_LOG_OUTPUT_APP

/* Define to 1 to enable COAP. */
#define OPENTHREAD_CONFIG_COAP_API_ENABLE 1

/* Define to 1 to enable the commissioner role. */
#define OPENTHREAD_CONFIG_COMMISSIONER_ENABLE 0

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
#define OPENTHREAD_CONFIG_DHCP6_CLIENT_ENABLE 0

/* Define to 1 to enable DHCPv6 SERVER. */
#define OPENTHREAD_CONFIG_DHCP6_SERVER_ENABLE 0

/* Define to 1 to enable MAC whitelist/blacklist feature. */
#define OPENTHREAD_CONFIG_MAC_FILTER_ENABLE 1

/* Define to 1 to enable TMF proxy feature. */
//#define OPENTHREAD_ENABLE_TMF_PROXY 0

/* Define to 1 to enable raw link-layer API. */
#define OPENTHREAD_CONFIG_LINK_RAW_ENABLE 0

/* Define to 1 to enable Border Router feature. */
#define OPENTHREAD_CONFIG_BORDER_ROUTER_ENABLE 0 

/* Define to 1 to enable Service feature. */
#define OPENTHREAD_CONFIG_TMF_NETDATA_SERVICE_ENABLE 1

/* Define to 1 to enable logging on the platform */
#define OPENTHREAD_CONFIG_LOG_PLATFORM 1

/* Define Log Level */
#define OPENTHREAD_CONFIG_LOG_LEVEL  OT_LOG_LEVEL_INFO

/* Define to 1 to enable CoAP Secure API */
#define OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE 0

/* Define to 1 to enable IPv6 global addresses configuration utilities */
#define OPENTHREAD_CONFIG_IP6_SLAAC_ENABLE 1

/* Define as 1 to enable saving the settings in RAM instead of flash */
#define OPENTHREAD_SETTINGS_RAM 0

/* Define to 1 to use flash API */
#define OPENTHREAD_CONFIG_PLATFORM_FLASH_API_ENABLE 1

/* Define to 1 to support ping cli command */
#define OPENTHREAD_CONFIG_PING_SENDER_ENABLE 1

/* Define to 1 to enable Mle Link Metrics Initiator */
#define OPENTHREAD_CONFIG_MLE_LINK_METRICS_INITIATOR_ENABLE 1

/* Define to 1 to enable Mle Link Metrics Subject */
#define OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE 0

/* Define to 1 to enable Backbone Router feature */
#define OPENTHREAD_CONFIG_BACKBONE_ROUTER_ENABLE 0

/* Define to 1 to support CSL feature */
#define OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE 0

/* Define to 1 if you want to enable microsecond backoff timer implemented in platform. */
#define OPENTHREAD_CONFIG_PLATFORM_USEC_TIMER_ENABLE 1

#define OPENTHREAD_CONFIG_MAC_RETRY_SUCCESS_HISTOGRAM_ENABLE 1

/* Define to 1 to support DUA feature */
#define OPENTHREAD_CONFIG_DUA_ENABLE 0

/* Define to 1 to enable Backbone router DUA ND Proxy */
#define OPENTHREAD_CONFIG_BACKBONE_ROUTER_DUA_NDPROXYING_ENABLE 0

/* Define to 1 to enable Backbone Router Multicast Routing */
//#define OPENTHREAD_CONFIG_BACKBONE_ROUTER_MULTICAST_ROUTING_ENABLE 1

/* Define to 1 to enable Thread Test Harness reference device support. */
#define OPENTHREAD_CONFIG_REFERENCE_DEVICE_ENABLE 1

/* Define to 1 to enable Config MLR */
#define OPENTHREAD_CONFIG_MLR_ENABLE 1

/* Define to 1 to enable TCP feature */
#define OPENTHREAD_CONFIG_TCP_ENABLE 0

/* Define to 1 to enable Multicast Listener Registration parent proxing */
//#define OPENTHREAD_CONFIG_TMF_PROXY_MLR_ENABLE 1

/* Define to 1 to enable DNS Client support */
#define OPENTHREAD_CONFIG_DNS_CLIENT_ENABLE 1

/* Define to 1 to enable DNS-SD Server support */
//#define OPENTHREAD_CONFIG_DNSSD_SERVER_ENABLE 0

/* Define to 1 to enable SRP Client support */
#define OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE 1

/* Define to 1 to enable ECDSA support */
#define OPENTHREAD_CONFIG_ECDSA_ENABLE 1

/* Define to 1 for the SRP client implementation to provide APIs that get/set the domain name */
#define OPENTHREAD_CONFIG_SRP_CLIENT_DOMAIN_NAME_API_ENABLE 1

#define OPENTHREAD_CONFIG_SRP_CLIENT_BUFFERS_ENABLE OPENTHREAD_CONFIG_SRP_CLIENT_ENABLE

/* Define to 1 to enable SRP Client auto-start feature and its APIs */
#define OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_API_ENABLE 1

#define OPENTHREAD_CONFIG_SRP_CLIENT_AUTO_START_DEFAULT_MODE 1


#define OPENTHREAD_CONFIG_DNS_CLIENT_SERVICE_DISCOVERY_ENABLE 1

/* Define to 1 to enable SRP Server support */
#define OPENTHREAD_CONFIG_SRP_SERVER_ENABLE 0

#endif /* OPENTHREAD_API_CONFIG_MATTER_H_ */
