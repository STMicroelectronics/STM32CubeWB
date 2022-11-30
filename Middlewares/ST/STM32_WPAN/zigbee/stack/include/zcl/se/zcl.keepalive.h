/**
 * @file zcl.keepalive.h
 * @copyright Copyright [2009 - 2022] Exegin Technologies Limited. All rights reserved.
 * @heading Keep Alive
 * @brief ZCL Keep Alive cluster header
 *
 * The Keep Alive clusters are typically used in Smart Energy applications.
 *
 * The Keep Alive server and client clusters are allocated by the stack if the application
 * configures the Key Exchange information in the ZbStartup configuration (struct ZbStartupCbkeT).
 * The Keep Alive server is allocated if the tc_keepalive_server_enable flag is set to true,
 * otherwise the Keep Alive client is allocated. Typically, the Keep Alive server is allocated
 * on the Trust Center, and the Keep Alive client is allocated on devices joining the SE network.
 *
 * If the Keep Alive client determines there's a problem communicating with the Trust Center,
 * it will call the application callback 'tcso_callback' configured in the ZbStartup configuration.
 * At which point, the stack will perform the necessary Trust Center Swap Out (TCSO) routines
 * to attempt to find a newly swapped-out Trust Center, or if the current Trust Center has moved
 * to a different channel or other configuration.
 */

/*--------------------------------------------------------------------------
 *  DESCRIPTION
 *      Interface definition for the ZCL Keep Alive cluster.
 *--------------------------------------------------------------------------
 */
#ifndef ZCL_KEEPALIVE_H
# define ZCL_KEEPALIVE_H

#include "zcl/zcl.h"

/** Keep Alive Server Attribute IDs */
enum ZbZclKeepAliveSvrAttrT {
    ZCL_KEEPALIVE_SVR_ATTR_BASE = 0x0000,
    /**< TC Keep-Alive Base - minutes (valid range is from 0x01 to 0xff,
     * but not enforced by cluster, for testing) */

    ZCL_KEEPALIVE_SVR_ATTR_JITTER = 0x0001
        /**< TC Keep-Alive Jitter - seconds (valid range is from 0x0000 to 0x0200) */
};

#define ZCL_KEEPALIVE_BASE_MIN              0x01U
#define ZCL_KEEPALIVE_BASE_MAX              0xffU
#define ZCL_KEEPALIVE_BASE_DEFAULT          10U /* minutes */

#define ZCL_KEEPALIVE_JITTER_MIN            0x0000U
#define ZCL_KEEPALIVE_JITTER_MAX            0x0200U
#define ZCL_KEEPALIVE_JITTER_DEFAULT        300U /* seconds */

/**
 * Create a new instance of the Keep Alive Server cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclKeepAliveServerAlloc(struct ZigBeeT *zb, uint8_t endpoint);

/**
 * Create a new instance of the Keep Alive Client cluster
 * @param zb Zigbee stack instance
 * @param endpoint Endpoint on which to create cluster
 * @param tcso_callback Callback function that will be invoked later when the TCSO is ever started (ZB_TCSO_STATUS_DISCOVERY_UNDERWAY),
 *  and when it completes, with the resultant status
 * @param tcso_arg Pointer to application data that will later be provided back to the callback function when invoked
 * @return Cluster pointer, or NULL if there is an error
 */
struct ZbZclClusterT * ZbZclKeepAliveClientAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    void (*tcso_callback)(enum ZbTcsoStatusT status, void *arg), void *tcso_arg);

/**
 * Start Keep Alive
 * @param zb Zigbee stack instance
 * @return Void
 */
void ZbZclKeepAliveClientStart(struct ZigBeeT *zb);

/**
 * Stop Keep Alive and abort the TCSO
 * @param zb Zigbee stack instance
 * @return Void
 */
void ZbZclKeepAliveClientStop(struct ZigBeeT *zb);

#endif /* __ZCL_KEEPALIVE_H */
